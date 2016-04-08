#include "AFN04.h"
#include "AFNPackage.h"
#include "YQErrCode.h"

AFN04Ack_Data::AFN04Ack_Data(void)
{
}

AFN04Ack_Data::~AFN04Ack_Data(void)
{
}

AFN04::AFN04(void)
{
	
}

AFN04::~AFN04(void)
{
}
int AFN04::Create(WORD pn,WORD Fn)
{
	afnHeader.AFN = Pkg_Afn_Header::AFN0C;
	afnHeader.SEQ._SEQ.CON = Pkg_Afn_Header::SEQ_CON_MBANSWER;
	afnHeader.SEQ._SEQ.FIN = 1;
	afnHeader.SEQ._SEQ.FIR = 1;
	afnHeader.SEQ._SEQ.TPV = Pkg_Afn_Header::SEQ_TPV_NO;

	pAfnData = new Pkg_Afn_Data();
	AFNPackage::GetDA(pn,pAfnData->m_Tag.DA1,pAfnData->m_Tag.DA2);
	AFNPackage::GetDT(Fn,pAfnData->m_Tag.DT1,pAfnData->m_Tag.DT2);
	
	Pkg_Afn_Aux_Down* p = new Pkg_Afn_Aux_Down(FALSE,TRUE);
	pAux = p;
	return YQER_OK;
}
int AFN04::CreatePointBaseSetting(WORD pn,BOOL hasPW)
{
	Create(pn,25);

	return YQER_OK;
}
int AFN04::CreatePointStatus()
{
	Create(0,150);
	
	pAfnData->m_nLen = 256*2;
	pAfnData->m_pData = new BYTE[pAfnData->m_nLen];
	memset(pAfnData->m_pData,0,pAfnData->m_nLen);

	return YQER_OK;
}

