#include "AFNPackage.h"
#include "YQErrCode.h"
#include "Log.h"

DWORD AFNPackage::s_pkgID = 1;

AFNPackage::AFNPackage()
	:m_nId(s_pkgID),Fn(0x0),pn(0x0)
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
int AFNPackage::ParseProto(BYTE* data,DWORD dataLen,DWORD& eatLen)
{
	if (dataLen < (PKG_HEADLEN+PKG_USER_HEADLEN+PKG_TAILLEN)){
		//包未收完整
		LOG(LOG_MINOR,"parse pkg error, len too less , wait for left...");
		eatLen = 0;
		return YQER_PKG_Err(1);
	}

	//生成包头域
	memcpy(&pkgHeader,data,PKG_HEADLEN);
	
	if ((int)dataLen < (int)pkgHeader.L._L.LEN+PKG_HEADLEN+PKG_TAILLEN){
		//包未收完整
		LOG(LOG_MINOR,"parse pkg error, len less pkg.length, wait for left...");
		eatLen = 0;
		return YQER_PKG_Err(1);
	}

	//生成包尾域
	memcpy(&pkgTail,data+PKG_HEADLEN+(int)pkgHeader.L._L.LEN,PKG_TAILLEN);
	eatLen = GetFrameL();

	//判断包合法性
	if (!valid()){
		//包标识字符非法
		LOG(LOG_MINOR,"parse pkg error, Tag error, drop");
		return YQER_PKG_Err(2);
	}
	
	BYTE cs = GetCS(data+PKG_HEADLEN,pkgHeader.L._L.LEN);
	if (cs != pkgTail.CS){
		//校验和错误
		LOG(LOG_MINOR,"parse pkg error, CS error, drop");
		return YQER_PKG_Err(4);
	}
	data += PKG_HEADLEN;

	//生成用户数据头
	memcpy(&userHeader,data,PKG_USER_HEADLEN);
	data += PKG_USER_HEADLEN;

	//生成用户数据区AFN对象,应用数据长度=用户数据长度-用户头长度
	pAfn->unPackData(data,pkgHeader.L._L.LEN-PKG_USER_HEADLEN);

	pn = AFNPackage::Getpn(pAfn->pAfnData->m_Tag.DA1,pAfn->pAfnData->m_Tag.DA2);
	Fn = AFNPackage::GetFn(pAfn->pAfnData->m_Tag.DT1,pAfn->pAfnData->m_Tag.DT2);
	LOG(LOG_INFORMATION,"parse pkg ok, user data len:%d",pkgHeader.L._L.LEN);
	
	return YQER_OK;
}
int AFNPackage::Serialize(BYTE* buf,DWORD bufLen) const
{
	//包头
	bufLen -= PKG_HEADLEN;
	if (bufLen < 0){
		return YQER_PKG_Err(99);
	}
	memcpy(buf,&pkgHeader,PKG_HEADLEN);
	buf += PKG_HEADLEN;

	//用户数据头
	bufLen -= PKG_USER_HEADLEN;
	if (bufLen < 0){
		return YQER_PKG_Err(99);
	}
	memcpy(buf,&userHeader,PKG_USER_HEADLEN);
	buf += PKG_USER_HEADLEN;

	//用户数据
	int afnLen = pAfn->PackData(buf,bufLen);
	if (afnLen <= 0){
		return YQER_PKG_Err(99);
	}
	bufLen -= afnLen;
	buf += afnLen;	

	//包尾
	bufLen -= PKG_TAILLEN;
	if (bufLen < 0){
		return YQER_PKG_Err(99);
	}
	memcpy(buf,&pkgTail,PKG_TAILLEN);

	//返回整个包长度字节数
	return PKG_HEADLEN+PKG_USER_HEADLEN+afnLen+PKG_TAILLEN;
}
//校验包合法性
BOOL AFNPackage::valid()
{		
	return pkgHeader.S1==0x68 && pkgHeader.S2==0x68 && pkgTail.E==0x16;
}
void AFNPackage::okPkg()
{
	SetL();
	CreateCS();
}
//生产CS校验
void AFNPackage::CreateCS()
{
	pkgTail.CS = 0x00;
	pkgTail.CS += userHeader.C.c;
	pkgTail.CS += userHeader.A1;
	pkgTail.CS += userHeader.A1>>8;
	pkgTail.CS += userHeader.A2;
	pkgTail.CS += userHeader.A2>>8;
	pkgTail.CS += userHeader.A3.a3;
	pkgTail.CS += pAfn->afnHeader.AFN;
	pkgTail.CS += pAfn->afnHeader.SEQ.seq;
	pkgTail.CS += pAfn->pAfnData->m_Tag.DA1;
	pkgTail.CS += pAfn->pAfnData->m_Tag.DA2;
	pkgTail.CS += pAfn->pAfnData->m_Tag.DT1;
	pkgTail.CS += pAfn->pAfnData->m_Tag.DT2;
	for (DWORD i = 0; i < pAfn->pAfnData->m_nLen; i++){
		pkgTail.CS += pAfn->pAfnData->m_pData[i];
	}
}
void AFNPackage::SetL()
{
	pkgHeader.L._L.LEN = PKG_USER_HEADLEN+pAfn->GetDataLen();
	pkgHeader.L1._L1.LEN = PKG_USER_HEADLEN+pAfn->GetDataLen();
}
BOOL AFNPackage::isRequest()
{
	if (userHeader.C._C.PRM == 1)
		return TRUE;
	return FALSE;
}
int AFNPackage::GetCode()
{
	return (int)pAfn->afnHeader.AFN;
}
void AFNPackage::SetAckType2ZJQ()
{
}
void AFNPackage::SetReqType2ZJQ()
{
}
BYTE AFNPackage::GetCS(BYTE* buf,DWORD len)
{
	BYTE cs = 0x00;
	for (DWORD i = 0; i < len; i++){
		cs += buf[i];
	}
	return cs;
}
//根据pn(1-2040)获取DA1,DA2值(DA2值从1开始)
void AFNPackage::GetDA(WORD pn,BYTE& DA1,BYTE& DA2)
{
	if (pn == 0){
		//终端信息点
		DA1 = 0x0;DA2 = 0x0;
	}
	else if (pn == 2041){
		//除p0外所有有效测量点
		DA1 = 0xff;DA2 = 0x0;
	}
	else if (pn <=2040){
		//信息点标识(1~2040)
		DA2 = ((pn-1)>>3)+1;
		DA1 = 1<<((pn-1)&0x07);
	}		
}
//根据DA1,DA2获取pn值(1-2040)
WORD AFNPackage::Getpn(BYTE DA1,BYTE DA2)
{
	if (DA1 == 0x0 && DA2 == 0x0){
		return 0x0;
	}
	if (DA1 == 0xff && DA2 == 0x0){
		return 2041;
	}
	else if (DA2 <= 255){
		WORD pn = 0x0;
		pn += ((DA2-1)<<3);
		int n = 0x0;
		for (; n<=0x07;n++){
			if (DA1 & (1<<n)){
				pn += (n+1);
				break;
			}
		}
		return pn;
	}
	return 0x0;
}
//根据Fn(1-248)获取DT1,DT2值(DT2值从0开始)
void AFNPackage::GetDT(WORD Fn,BYTE& DT1,BYTE& DT2)
{
	if (Fn <=248 && Fn > 0){
		DT2 = (Fn-1)>>3;
		DT1 = 1<<((Fn-1)&0x07);
	}		
}
//根据DT1,DT2获取Fn值(1-248)
WORD AFNPackage::GetFn(BYTE DT1,BYTE DT2)
{
	if (DT2 <= 30){
		WORD Fn = 0x0;
		Fn += (DT2<<3);
		int n = 0x0;
		for (; n<=0x07;n++){
			if (DT1 & (1<<n)){
				Fn += (n+1);
				break;
			}
		}
		return Fn;
	}
	return 0x0;
}
