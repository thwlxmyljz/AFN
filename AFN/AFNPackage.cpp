#include "stdafx.h"
#include "AFNPackage.h"
#include "YQErrCode.h"

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

	//生成用户数据头
	memcpy(&userHeader,data,PKG_USER_HEADLEN);
	data += PKG_USER_HEADLEN;

	//生成用户数据区AFN对象,应用数据长度=用户数据长度-用户头长度
	pAfn->unPackData(data,pkgHeader.L._L.LEN-PKG_USER_HEADLEN);

	pn = AFNPackage::Getpn(pAfn->pAfnData->m_Tag.DA1,pAfn->pAfnData->m_Tag.DA2);
	Fn = AFNPackage::GetFn(pAfn->pAfnData->m_Tag.DT1,pAfn->pAfnData->m_Tag.DT2);

	return YQER_OK;
}
int AFNPackage::Serialize(BYTE* buf,DWORD bufLen)
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
