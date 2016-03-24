#include "stdafx.h"
#include "AFNPackage.h"
#include "YQErrCode.h"

unsigned int AFNPackage::s_pkgID = 1;

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

	return 0;
}
int AFNPackage::serialize(BYTE* buf,DWORD bufLen)
{
	return 0;
}