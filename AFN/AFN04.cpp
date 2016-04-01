#include "stdafx.h"
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
int AFN04::CreateF25(WORD pn,BOOL hasPW)
{
	afnHeader.AFN = Pkg_Afn_Header::AFN04;
	afnHeader.SEQ._SEQ.CON = Pkg_Afn_Header::SEQ_CON_MBANSWER;
	afnHeader.SEQ._SEQ.FIN = 1;
	afnHeader.SEQ._SEQ.FIR = 1;
	afnHeader.SEQ._SEQ.TPV = Pkg_Afn_Header::SEQ_TPV_NO;	

	pAfnData = new Pkg_Afn_Data();
	AFNPackage::GetDA(pn,pAfnData->m_Tag.DA1,pAfnData->m_Tag.DA2);
	AFNPackage::GetDT(25,pAfnData->m_Tag.DT1,pAfnData->m_Tag.DT2);
	
	Pkg_Afn_Aux_Down* p = new Pkg_Afn_Aux_Down(FALSE,hasPW);
	pAux = p;
	return YQER_OK;
}

