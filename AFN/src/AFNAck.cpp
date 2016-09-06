#include "Connection.h"
#include "ConnectionList.h"
#include "AFNAck.h"
#include "AFNPackage.h"
#include "YQErrCode.h"
#include "Log.h"


AFNAck_Data::AFNAck_Data()
	:Pkg_Afn_Data()
{
	m_nLen = 0;
	m_pData = NULL;
	m_Tag.DA1 = 0;
	m_Tag.DA2 = 0;
	AFNPackage::GetDT(1/*F1全部确认*/,m_Tag.DT1,m_Tag.DT2);
}
AFNAck_Data::~AFNAck_Data()
{
}

