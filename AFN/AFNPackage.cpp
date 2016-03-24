#include "stdafx.h"
#include "AFNPackage.h"
#include "YQErrCode.h"

unsigned int AFNPackage::s_pkgID = 1;
unsigned int pkg::PKG_HEADLEN = 6; 
AFNPackage::AFNPackage()
	:m_nId(s_pkgID)
{
}
AFNPackage::~AFNPackage(void)
{
}
int AFNPackage::parseProto(BYTE* data,unsigned int len)
{
	if (len < 14)
		return YQER_PKG_Err(1);

	S1 = data[0];
	E = data[len-1];
	if (S1 != 0x68 || E != 0x16)
		return YQER_PKG_Err(2);
	memcpy(Ldata+1,
	return 0;
}
