#include "AFN0C.h"
#include "AFNPackage.h"
#include "YQErrCode.h"
#include "AFNData.h"
#include "AFNPackageBuilder.h"
#include <sstream>

#define GO() p += len;\
		left -= len;
AFN0CAck_Data_AllKwh::AFN0CAck_Data_AllKwh()
	:Pkg_Afn_Data()
{
}
AFN0CAck_Data_AllKwh::AFN0CAck_Data_AllKwh(const Pkg_Afn_Data* _origin)
	:Pkg_Afn_Data(*_origin)
{
}
AFN0CAck_Data_AllKwh::~AFN0CAck_Data_AllKwh(void)
{
}
AFN0CAck_Data_AllKwh* AFN0CAck_Data_AllKwh::New()
{
	return new AFN0CAck_Data_AllKwh();
}
int AFN0CAck_Data_AllKwh::HandleData()
{
	do{
		int left = m_nLen;
		BYTE* p = m_pData;
		
		int len = AFNData::parseDateTime(p,left,dt);
		if (len <= 0){return YQER_PKG_DATA_Err(2);}
		GO()

		if (left < 1){return YQER_PKG_DATA_Err(2);}
		m = p[0];
		len = 1;
		if (m <1 ||m > 12){return YQER_PKG_DATA_Err(3);}
		GO()

		len = AFNData::parseTable14(p,left,userkwh);
		if (len <= 0){return YQER_PKG_DATA_Err(2);}
		GO()

		userkwh_fee = new float[m];
		for (int i = 0; i < m; i++){
			len = AFNData::parseTable14(p,left,userkwh_fee[i]);
			if (len <= 0){return YQER_PKG_DATA_Err(2);}
			GO()
		}

		len = AFNData::parseTable11(p,left,devkwh);
		GO()

		devkwh_fee = new float[m];
		for (int i = 0; i < m; i++){
			len = AFNData::parseTable11(p,left,devkwh_fee[i]);
			if (len <= 0){return YQER_PKG_DATA_Err(2);}
			GO()
		}

		len = AFNData::parseTable11(p,left,onekwh);
		if (len <= 0){return YQER_PKG_DATA_Err(2);}
		GO()

		onekwh_fee = new float[m];
		for (int i = 0; i < m; i++){
			len = AFNData::parseTable11(p,left,onekwh_fee[i]);
			if (len <= 0){return YQER_PKG_DATA_Err(2);}
			GO()
		}

		len = AFNData::parseTable11(p,left,fourkwh);
		if (len <= 0){return YQER_PKG_DATA_Err(2);}
		GO()

		fourkwh_fee = new float[m];
		for (int i = 0; i < m; i++){
			len = AFNData::parseTable11(p,left,fourkwh_fee[i]);
			if (len <= 0){return YQER_PKG_DATA_Err(2);}
			GO()
		}
	}while (FALSE);
	
	return YQER_OK;
}
std::string AFN0CAck_Data_AllKwh::toString()
{
	std::ostringstream os;
	os << "终端抄表时间:" << dt << "\r\n";
	os << "当前正向有功总电能示值:"<< userkwh << "\r\n";
	for (int i = 0;  i < m ; i++){
		os << "  当前费率" << i+1 << "正向有功总电能示值:"<<userkwh_fee[i]<<"\r\n";
	}
	os << "当前正向无功（组合无功1）总电能示值:"<< devkwh << "\r\n";
	for (int i = 0;  i < m ; i++){
		os << "  当前费率" << i+1 << "正向无功（组合无功1）总电能示值:"<<devkwh_fee[i]<<"\r\n";
	}
	os << "当前一象限无功总电能示值:"<< onekwh << "\r\n";
	for (int i = 0;  i < m ; i++){
		os << "  当前一象限费率" << i+1 << "无功电能示值:"<<onekwh_fee[i]<<"\r\n";
	}
	os << "当前四象限无功总电能示值:"<< fourkwh << "\r\n";
	for (int i = 0;  i < m ; i++){
		os << "  当前四象限费率" << i+1 << "无功电能示值:"<<fourkwh_fee[i]<<"\r\n";
	}
	return os.str();
}

AFN0C::AFN0C(void)
{
}


AFN0C::~AFN0C(void)
{
}
int AFN0C::HandleAck(std::list<AFNPackage*>& ackLst)
{	
	if (ackLst.size() == 0){
		return YQER_OK;
	}
	//AFN0C响应帧
	AFNPackage* ackPkg = *(ackLst.begin());	
	if (ackPkg->pAfn->afnHeader.AFN == Pkg_Afn_Header::AFN0C)
	{
		if (ackPkg->userHeader.C._C.FUN == Pkg_User_Header::UH_FUNC_SUB8){
			//用户数据
			Pkg_Afn_Data* pData = NULL;
			switch (ackPkg->Fn)
			{
				case 33:
					//当前正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
					pData = new AFN0CAck_Data_AllKwh(ackPkg->pAfn->pAfnData);			
				default:
					break;
			} 
			if (pData){
				pData->HandleData();
				AppCall call;
				call.AFN = ackPkg->pAfn->afnHeader.AFN;
				call.m_areacode = ackPkg->userHeader.A1;
				call.m_number = ackPkg->userHeader.A2;
				AFNPackageBuilder::Instance().Notify(call,pData);
			}
		}
		else if(ackPkg->userHeader.C._C.FUN == Pkg_User_Header::UH_FUNC_SUB9){
			//否认：无所召唤的数据
			AppCall call;
			call.AFN = ackPkg->pAfn->afnHeader.AFN;
			call.m_areacode = ackPkg->userHeader.A1;
			call.m_number = ackPkg->userHeader.A2;
			AFNPackageBuilder::Instance().Notify(call,(Pkg_Afn_Data*)-1);
		}
	}
	return -1;
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
	
	Pkg_Afn_Aux_Down* p = new Pkg_Afn_Aux_Down(FALSE,TRUE);
	pAux = p;
	return YQER_OK;
}
int AFN0C::CreateClock()
{
	return Create(0/*p0*/,2);
}
int AFN0C::CreateRunStatus()
{
	return Create(0/*p0*/,11);
}
int AFN0C::CreateCurTotalKwh()
{
	return Create(0/*总加组号?*/,17);
}		
int AFN0C::CreateCurTotalKvarh()
{
	return Create(0/*总加组号?*/,18);
}
int AFN0C::CreateCurABCTotal(WORD pn)
{
	return Create(pn,25);
}
int AFN0C::CreateCurABCTotal(WORD* pN,int len)
{
	return 0;
}
int AFN0C::CreateAllKwh(WORD pn)
{
	return Create(pn,33);
}
int AFN0C::CreateAllKwh(WORD* pN,int len)
{
	return 0;
}