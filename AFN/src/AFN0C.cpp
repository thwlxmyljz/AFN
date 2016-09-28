#include "AFN0C.h"
#include "AFNPackage.h"
#include "YQErrCode.h"
#include "AFNPackageBuilder.h"
#include "DBManager.h"
#include <sstream>
#include "ConnectionList.h"
#include "Log.h"

#define GO() p += len;\
		left -= len;

AFN0CAck_Data_GetClock::AFN0CAck_Data_GetClock()
	:Pkg_Afn_Data()
{
}
AFN0CAck_Data_GetClock::AFN0CAck_Data_GetClock(Pkg_Afn_Data* _origin)
	:Pkg_Afn_Data(*_origin)
{
}
AFN0CAck_Data_GetClock::~AFN0CAck_Data_GetClock(void)
{
}
AFN0CAck_Data_GetClock* AFN0CAck_Data_GetClock::New()
{
	return new AFN0CAck_Data_GetClock();
}
int AFN0CAck_Data_GetClock::HandleData()
{
	return AFNData::parseDateTime6(m_pData,m_nLen,dt);
}
std::string AFN0CAck_Data_GetClock::toString()
{
	return dt;
}
//---------------------------------------------------------------------------------
int AFN0CAck_Data_GetRunStatus::Block::parse(const BYTE*p, int len)
{
	if (len >= 19){
		port = p[0];
		memcpy(&num,p+1,2);
		status = p[3];
		memcpy(&success,p+4,2);
		success_import = p[6];
		AFNData::parseDateTime6(p+7,6,sdt);
		AFNData::parseDateTime6(p+13,6,edt);
		return 19;
	}
	return 0;
}
std::string AFN0CAck_Data_GetRunStatus::Block::toString()
{
	std::ostringstream os;
	os << "  终端通信端口号:" << (int)port << "\r\n";
	os << "  要抄电表总数:" << num << "\r\n";
	os << "  当前抄表工作状态标志:" << ((status&0x01)?"正在抄表,":"未抄表,") << ((status&0x02)?"时段内完成":"时段内未完成") << "\r\n";
	os << "  抄表成功块数:" << success << "\r\n";
	os << "  抄重点表成功块数:" << (int)success_import << "\r\n";
	os << "  抄表开始时间:" << sdt << "\r\n";
	os << "  抄表结束时间:" << edt << "\r\n";
	return os.str();
}
AFN0CAck_Data_GetRunStatus::AFN0CAck_Data_GetRunStatus()
	:Pkg_Afn_Data(),pBlock(NULL)
{
}
AFN0CAck_Data_GetRunStatus::AFN0CAck_Data_GetRunStatus(Pkg_Afn_Data* _origin)
	:Pkg_Afn_Data(*_origin),pBlock(NULL)
{
}
AFN0CAck_Data_GetRunStatus::~AFN0CAck_Data_GetRunStatus(void)
{
	delete[] pBlock;
}
AFN0CAck_Data_GetRunStatus* AFN0CAck_Data_GetRunStatus::New()
{
	return new AFN0CAck_Data_GetRunStatus();
}
int AFN0CAck_Data_GetRunStatus::HandleData()
{
	int left = m_nLen;
	BYTE* p = m_pData;
		
	if (left < 1){return YQER_PKG_DATA_Err(2);}
	n = p[0];
	int len = 1;
	if (n <1 ||n > 31){return YQER_PKG_DATA_Err(3);}
	GO()

	pBlock = new Block[n];
	for (int i = 0; i < n; i++){
		len = pBlock[i].parse(p,left);
		if (len == 0)
			return YQER_PKG_DATA_Err(3);
		GO()
	}
	return YQER_OK;
}
std::string AFN0CAck_Data_GetRunStatus::toString()
{
	std::ostringstream os;
	os << "本项数据块个数:" << n << "\r\n";
	for (int i = 0;  i < n ; i++){
		os << "第"<<i+1 <<"数据块" << "\r\n";
		os << pBlock[i].toString();
	}
	return os.str();
}
//---------------------------------------------------------------------------------
AFN0CAck_Data_AllKwh::AFN0CAck_Data_AllKwh()
	:Pkg_Afn_Data(),userkwh_fee(NULL),devkwh_fee(NULL),onekwh_fee(NULL),fourkwh_fee(NULL)
{
}
AFN0CAck_Data_AllKwh::AFN0CAck_Data_AllKwh(Pkg_Afn_Data* _origin)
	:Pkg_Afn_Data(*_origin),userkwh_fee(NULL),devkwh_fee(NULL),onekwh_fee(NULL),fourkwh_fee(NULL)
{
}
AFN0CAck_Data_AllKwh::~AFN0CAck_Data_AllKwh(void)
{
	delete[] userkwh_fee;
	delete[] devkwh_fee;
	delete[] onekwh_fee;
	delete[] fourkwh_fee;
}
AFN0CAck_Data_AllKwh* AFN0CAck_Data_AllKwh::New()
{
	return new AFN0CAck_Data_AllKwh();
}
int AFN0CAck_Data_AllKwh::HandleData()
{
	int left = m_nLen;
	BYTE* p = m_pData;
		
	int len = AFNData::parseDateTime5(p,left,dt);
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
	
	return YQER_OK;
}
std::string AFN0CAck_Data_AllKwh::toString()
{
	std::ostringstream os;
	os << "终端抄表时间:" << dt << "\r\n";
	os << "当前正向有功总电能示值:"<< userkwh << "kWh\r\n";
	for (int i = 0;  i < m ; i++){
		os << "  当前费率" << i+1 << "正向有功总电能示值:"<<userkwh_fee[i]<<"kWh\r\n";
	}
	os << "当前正向无功（组合无功1）总电能示值:"<< devkwh << "kvarh\r\n";
	for (int i = 0;  i < m ; i++){
		os << "  当前费率" << i+1 << "正向无功（组合无功1）总电能示值:"<<devkwh_fee[i]<<"kvarh\r\n";
	}
	os << "当前一象限无功总电能示值:"<< onekwh << "kvarh\r\n";
	for (int i = 0;  i < m ; i++){
		os << "  当前一象限费率" << i+1 << "无功电能示值:"<<onekwh_fee[i]<<"kvarh\r\n";
	}
	os << "当前四象限无功总电能示值:"<< fourkwh << "kvarh\r\n";
	for (int i = 0;  i < m ; i++){
		os << "  当前四象限费率" << i+1 << "无功电能示值:"<<fourkwh_fee[i]<<"kvarh\r\n";
	}
	return os.str();
}
int AFN0CAck_Data_AllKwh::toDB(WORD A1,WORD A2,WORD Fn, WORD pn)
{
	/*
	CREATE TABLE `gc_nhfxelect` (
	  `ID` int(24) NOT NULL AUTO_INCREMENT COMMENT '序号',
	  `EquipmentCjqID` int(24) NOT NULL COMMENT '唯一表示采集器 对应gc_equipmentelect的ID值',
	  `USERKWH` float NOT NULL COMMENT '正向有功总电能示值',
	  `DEVKVAR` float NOT NULL COMMENT '正向无功总电能示值',
	  `FIRSTKVAR` float NOT NULL COMMENT '一象限无功总电能示值',
	  `FOURKVAR` float NOT NULL COMMENT '四象限无功总电能示值',
	  `M_USERKWH` varchar(50) DEFAULT NULL COMMENT '正向有功费率1-M示值,#分隔',
	  `M_DEVKVAR` varchar(50) DEFAULT NULL COMMENT '正向无功费率1-M示值,#分隔',
	  `M_FIRSTKVAR` varchar(50) DEFAULT NULL COMMENT '一象限无功费率1-M示值,#分隔',
	  `M_FOURKVAR` varchar(50) DEFAULT NULL COMMENT '四象限无功费率1-M示值,#分隔',
	  `RecordTime` datetime(6) NOT NULL COMMENT '记录时间',
	  `TimeSpace` int(8) NOT NULL COMMENT '时间间隔 单位：秒 5min',
	  `Remarks` varchar(64) DEFAULT NULL COMMENT '备注信息',
	  PRIMARY KEY (`ID`),
	  UNIQUE KEY `EquipmentCjqID` (`EquipmentCjqID`)
	) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
	*/
	std::ostringstream os;
	int eleId = g_JzqConList->GetEleID(A1,A2,pn);
	if (eleId == -1)
	{
		LOG(LOG_MINOR,"drop ele data , not found ele(%d,%d,%d)",A1,A2,pn);
		return -1;
	}
	os << "insert into gc_nhfxelect(EquipmentCjqID,TimeSpace,RecordTime,USERKWH,DEVKVAR,FIRSTKVAR,FOURKVAR,M_USERKWH,M_DEVKVAR,M_FIRSTKVAR,M_FOURKVAR) ";
	os << "values("<<eleId<<","<<60<<",'"<<dt<<"',"<<userkwh<<","<<devkwh<<","<<onekwh<<","<<fourkwh<<",'";
	for (int i = 0; i < m; i++){
		os <<userkwh_fee[i]<<"#";
	}
	os << "','";
	for (int i = 0; i < m; i++){
		os <<devkwh_fee[i]<<"#";
	}
	os << "','";
	for (int i = 0; i < m; i++){
		os <<onekwh_fee[i]<<"#";
	}
	os << "','";
	for (int i = 0; i < m; i++){
		os <<fourkwh_fee[i]<<"#";
	}
	os << "')";
	
	qexec(os.str());
	qclose();
	return YQER_OK;
}
//---------------------------------------------------------------------------------

AFN0C::AFN0C(void)
{
}
AFN0C::~AFN0C(void)
{
}
void* AFN0C::HandleAck(IPackage* _ackPkg)
{	
	Pkg_Afn_Data* pData = NULL;
	AFNPackage* ackPkg = (AFNPackage*)_ackPkg;
	switch (ackPkg->Fn)
	{
		case 2://F2
			pData = new AFN0CAck_Data_GetClock(ackPkg->pAfn->pAfnData);
			break;
		case 11://F11
			pData = new AFN0CAck_Data_GetRunStatus(ackPkg->pAfn->pAfnData);
			break;
		case 33://F33
			//当前正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
			pData = new AFN0CAck_Data_AllKwh(ackPkg->pAfn->pAfnData);
			break;
		default:
			break;
	}
	return (void*)pData;
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