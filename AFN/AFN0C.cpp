#include "stdafx.h"
#include "AFN0C.h"
#include "AFNPackage.h"
#include "YQErrCode.h"

AFN0CAck_Data::AFN0CAck_Data(void)
{
}

AFN0CAck_Data::~AFN0CAck_Data(void)
{
}


AFN0C::AFN0C(void)
{
}


AFN0C::~AFN0C(void)
{
}
int AFN0C::Create(WORD pn,WORD Fn)
{
	afnHeader.AFN = Pkg_Afn_Header::AFN0C;
	afnHeader.SEQ._SEQ.CON = Pkg_Afn_Header::SEQ_CON_MBANSWER;
	afnHeader.SEQ._SEQ.FIN = 1;
	afnHeader.SEQ._SEQ.FIR = 1;
	afnHeader.SEQ._SEQ.TPV = Pkg_Afn_Header::SEQ_TPV_NO;

	pAfnData = new Pkg_Afn_Data();
	AFNPackage::GetDA(pn,pAfnData->m_Tag.DA1,pAfnData->m_Tag.DA2);
	AFNPackage::GetDT(Fn,pAfnData->m_Tag.DT1,pAfnData->m_Tag.DT2);
	
	Pkg_Afn_Aux_Down* p = new Pkg_Afn_Aux_Down(FALSE,FALSE);
	pAux = p;
	return YQER_OK;
}
int AFN0C::CreateClock()
{
	return Create(0,2);
}