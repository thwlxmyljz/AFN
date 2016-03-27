#include "stdafx.h"
#include "AFNPackage.h"
#include "YQErrCode.h"

DWORD AFNPackage::s_pkgID = 1;
BYTE AFNPackage::s_RSEQ = 0;
BYTE AFNPackage::s_PSEQ = 0;
BYTE AFNPackage::s_MSA = 0x01;

AFNPackage::AFNPackage()
	:m_nId(s_pkgID)
{
	pAfn = new Pkg_Afn();
	pkgHeader.S1 = 0x68;
	pkgHeader.S2 = 0x68;
	pkgHeader.L._L.TAG = 0x01;
	pkgHeader.L1._L1.TAG = 0x01;
	pkgTail.E = 0x16;
}
AFNPackage::~AFNPackage(void)
{
	delete pAfn;
	pAfn = NULL;
}
void AFNPackage::SetAckType2ZJQ()
{
	
}
void AFNPackage::SetReqType2ZJQ()
{

}
int AFNPackage::ParseProto(BYTE* data,DWORD len)
{
	if (len < (PKG_HEADLEN+PKG_USER_HEADLEN+PKG_TAILLEN)){
		return YQER_PKG_Err(1);
	}

	//生成包头域
	memcpy(&pkgHeader,data,PKG_HEADLEN);
	//生成包尾域
	memcpy(&pkgTail,data+len-2,PKG_TAILLEN);

	//判断包合法性
	if (!valid()){
		//包标识字符非法
		return YQER_PKG_Err(2);
	}
	if (len != pkgHeader.L._L.LEN+PKG_HEADLEN+PKG_TAILLEN){
		//包长度不正确
		return YQER_PKG_Err(3);
	}
	BYTE cs = GetCS(data+PKG_HEADLEN,pkgHeader.L._L.LEN);
	if (cs != pkgTail.CS){
		//校验和错误
		return YQER_PKG_Err(4);
	}
	data += PKG_HEADLEN;

	//生成用户数据区
	memcpy(&userHeader,data,PKG_USER_HEADLEN);
	data += PKG_USER_HEADLEN;

	//生成用户数据AFN对象
	pAfn->unPackData(data,pkgHeader.L._L.LEN-PKG_USER_HEADLEN);//应用数据长度=用户数据长度-用户头长度
	
	return YQER_OK;
}
int AFNPackage::Serialize(BYTE* buf,DWORD bufLen)
{
	bufLen -= PKG_HEADLEN;
	if (bufLen < 0){
		return YQER_PKG_Err(99);
	}
	memcpy(buf,&pkgHeader,PKG_HEADLEN);
	buf += PKG_HEADLEN;

	bufLen -= PKG_USER_HEADLEN;
	if (bufLen < 0){
		return YQER_PKG_Err(99);
	}
	memcpy(buf,&userHeader,PKG_USER_HEADLEN);
	buf += PKG_USER_HEADLEN;

	bufLen -= PKG_AFN_HEADLEN;
	if (bufLen < 0){
		return YQER_PKG_Err(99);
	}
	memcpy(buf,&pAfn->afnHeader,PKG_AFN_HEADLEN);
	buf += PKG_AFN_HEADLEN;

	if ((uDataLen = pAfn->pAfnData->PackData(buf,bufLen)) > 0){
		buf += uDataLen;
	}

	return YQER_OK;
}
int AFNPackage::HandlePkg(std::list<AFNPackage*>& ackLst)
{
	if (userHeader.C._C.PRM == 1){
		return HandleRequest(ackLst);
	}
	else{
		return HandleAck(ackLst);
	}
}
int AFNPackage::HandlePkg(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst)
{
	return YQER_OK;
}
int AFNPackage::HandleRequest(std::list<AFNPackage*>& ackLst)
{
	switch (userHeader.C._C.FUN)
	{
	case Pkg_User_Header::UH_FUNC_MAIN1:
		break;
	case Pkg_User_Header::UH_FUNC_MAIN9:
		//链路测试
		if (pAfn->afnHeader.AFN == Pkg_Afn_Header::AFN02 && \
			pAfn->afnHeader.SEQ._SEQ.CON == Pkg_Afn_Header::SEQ_CON_MBANSWER)
		{
			//链路接口检测,发送响应帧,保存启动帧序号PSEQ为响应帧起始序号			
			if (pAfn->pAfnData->m_Tag.DA1 == 0 && pAfn->pAfnData->m_Tag.DA2 == 0)
			{
				if (pAfn->pAfnData->m_Tag.DT1 == 1 && pAfn->pAfnData->m_Tag.DT2 == 0){
					//登录,无数据
					s_RSEQ = pAfn->afnHeader.SEQ._SEQ.PRSEQ;
					//数据按F3回应，ERR定义如下，=0正确，=1其他错误，=2表地址重复，3~255备用
					AFNPackage* ackPkg = new AFNPackage();
					if (!ackPkg){
						return YQER_NOMEMORY;
					}
					ackPkg->userHeader.C._C.DIR = 0x00;
					ackPkg->userHeader.C._C.PRM = 0x00;
					ackPkg->userHeader.C._C.FCV = 0x00;
					ackPkg->userHeader.C._C.FCB = 0x00;
					ackPkg->userHeader.C._C.FUN = 11;//链路状态
					ackPkg->userHeader.A3._A3.TAG = 0;
					ackPkg->userHeader.A3._A3.MSA = s_MSA;
					ackPkg->userHeader.A1 = 0;
					ackPkg->userHeader.A2 = 0;
					ackPkg->pAfn->afnHeader.AFN = Pkg_Afn_Header::AFN00;
					ackPkg->pAfn->afnHeader.SEQ._SEQ.CON = Pkg_Afn_Header::SEQ_CON_NOANSWER;
					ackPkg->pAfn->afnHeader.SEQ._SEQ.FIN = 1;
					ackPkg->pAfn->afnHeader.SEQ._SEQ.FIR = 1;
					ackPkg->pAfn->afnHeader.SEQ._SEQ.TPV = Pkg_Afn_Header::SEQ_TPV_NO;
					ackPkg->DoSetRSEQ();
					ackLst.push_back(ackPkg);
				}
				else if (pAfn->pAfnData->m_Tag.DT1 == 2 && pAfn->pAfnData->m_Tag.DT2 == 0){
					//退出登录,无数据
				}
				else if (pAfn->pAfnData->m_Tag.DT1 == 3 && pAfn->pAfnData->m_Tag.DT2 == 0){
					//心跳,6字节终端时钟数据
				}
			}
		}
		break;
	default:
		break;
	}
	return YQER_OK; 
}
int AFNPackage::HandleAck(std::list<AFNPackage*>& ackLst)
{
	return YQER_OK;
}
void AFNPackage::DoSetRSEQ()
{
	pAfn->afnHeader.SEQ._SEQ.PRSEQ = s_RSEQ++;
	if (s_RSEQ > 15){
		s_RSEQ = 0;
	}
}