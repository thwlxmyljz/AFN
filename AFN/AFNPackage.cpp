#include "stdafx.h"
#include "AFNPackage.h"
#include "YQErrCode.h"

DWORD AFNPackage::s_pkgID = 1;

AFNPackage::AFNPackage()
	:m_nId(s_pkgID)
{
}
AFNPackage::~AFNPackage(void)
{
}
int AFNPackage::parseProto(BYTE* data,DWORD len)
{
	if (len < (PKG_HEADLEN+PKG_USER_HEADLEN+PKG_TAILLEN))
		return YQER_PKG_Err(1);
	//生成包头域
	memcpy(&pkgHeader,data,PKG_HEADLEN);
	//生成包尾域
	memcpy(&pkgTail,data+len-2,PKG_TAILLEN);

	//判断包合法性
	if (!valid()){
		//包标识字符非法
		return YQER_PKG_Err(2);
	}
	if (len != pkgHeader.L.LEN+PKG_HEADLEN+PKG_TAILLEN){
		//包长度不正确
		return YQER_PKG_Err(3);
	}
	BYTE cs = GetCS(data+PKG_HEADLEN,pkgHeader.L.LEN);
	if (cs != pkgTail.CS){
		//校验和错误
		return YQER_PKG_Err(4);
	}

	//生成用户数据区
	data += PKG_HEADLEN;
	memcpy(&userHeader,data+PKG_HEADLEN,PKG_USER_HEADLEN);

	//生成AFN对象
	data += PKG_USER_HEADLEN;
	pAfn = new Pkg_Afn(data,pkgHeader.L.LEN-PKG_USER_HEADLEN);//应用数据长度=用户数据长度-用户头长度
	
	return YQER_OK;
}
int AFNPackage::serialize(BYTE* buf,DWORD bufLen)
{
	return YQER_OK;
}
int AFNPackage::HandlePkg()
{
	return YQER_OK;
}