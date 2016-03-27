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

	//���ɰ�ͷ��
	memcpy(&pkgHeader,data,PKG_HEADLEN);
	//���ɰ�β��
	memcpy(&pkgTail,data+len-2,PKG_TAILLEN);

	//�жϰ��Ϸ���
	if (!valid()){
		//����ʶ�ַ��Ƿ�
		return YQER_PKG_Err(2);
	}
	if (len != pkgHeader.L._L.LEN+PKG_HEADLEN+PKG_TAILLEN){
		//�����Ȳ���ȷ
		return YQER_PKG_Err(3);
	}
	BYTE cs = GetCS(data+PKG_HEADLEN,pkgHeader.L._L.LEN);
	if (cs != pkgTail.CS){
		//У��ʹ���
		return YQER_PKG_Err(4);
	}
	data += PKG_HEADLEN;

	//�����û�������
	memcpy(&userHeader,data,PKG_USER_HEADLEN);
	data += PKG_USER_HEADLEN;

	//�����û�����AFN����
	pAfn->unPackData(data,pkgHeader.L._L.LEN-PKG_USER_HEADLEN);//Ӧ�����ݳ���=�û����ݳ���-�û�ͷ����
	
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
		//��·����
		if (pAfn->afnHeader.AFN == Pkg_Afn_Header::AFN02 && \
			pAfn->afnHeader.SEQ._SEQ.CON == Pkg_Afn_Header::SEQ_CON_MBANSWER)
		{
			//��·�ӿڼ��,������Ӧ֡,��������֡���PSEQΪ��Ӧ֡��ʼ���			
			if (pAfn->pAfnData->m_Tag.DA1 == 0 && pAfn->pAfnData->m_Tag.DA2 == 0)
			{
				if (pAfn->pAfnData->m_Tag.DT1 == 1 && pAfn->pAfnData->m_Tag.DT2 == 0){
					//��¼,������
					s_RSEQ = pAfn->afnHeader.SEQ._SEQ.PRSEQ;
					//���ݰ�F3��Ӧ��ERR�������£�=0��ȷ��=1��������=2���ַ�ظ���3~255����
					AFNPackage* ackPkg = new AFNPackage();
					if (!ackPkg){
						return YQER_NOMEMORY;
					}
					ackPkg->userHeader.C._C.DIR = 0x00;
					ackPkg->userHeader.C._C.PRM = 0x00;
					ackPkg->userHeader.C._C.FCV = 0x00;
					ackPkg->userHeader.C._C.FCB = 0x00;
					ackPkg->userHeader.C._C.FUN = 11;//��·״̬
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
					//�˳���¼,������
				}
				else if (pAfn->pAfnData->m_Tag.DT1 == 3 && pAfn->pAfnData->m_Tag.DT2 == 0){
					//����,6�ֽ��ն�ʱ������
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