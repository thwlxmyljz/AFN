#include "stdafx.h"
#include "AFN01.h"


AFN01Req_Data::AFN01Req_Data(void)
{
	m_Tag.DA1 = 0;
	m_Tag.DA2 = 0;
	m_Tag.DT1 = 1;
	m_Tag.DT2 = 0;
}

AFN01Req_Data::~AFN01Req_Data(void)
{
}
AFN01::AFN01(int Fn)
{
	pAfnData = new AFN01Req_Data();
	Pkg_Afn_Aux_Down* p = new Pkg_Afn_Aux_Down(FALSE);
	memset(p->PW,sizeof(p->PW),0);
	pAux = p;
}