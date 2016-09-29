#include "ConnectionPower.h"
#include "ConnectionList.h"
#include "AFNPackageBuilder.h"
#include "YQErrCode.h"
#include "Lock.h"
#include "AFN04.h"
#include "AFN0C.h"
#include "AFNAck.h"
#include "LogFileu.h"
#include "Log.h"
#include "Utility.h"

#define LOCK_CONNECTION(name) AUTO_LOCK()\
					ConnectionPower* con = g_JzqConList->getJzqConnection(name);\
					if (!con){\
						return YQER_JZQ_NOTOK;\
					}\
					con->ClearRecPkgList();

#define BEGIN_CALL() AppCall call;\
					int ret = YQER_OK;

#define REQUEST_DATA(con,afnData,n) do{AFNPackage* sndPkg = new AFNPackage();\
	sndPkg->userHeader.C._C.DIR = 0x00;\
	sndPkg->userHeader.C._C.PRM = 0x01;\
	sndPkg->userHeader.C._C.FCV = 0x00;\
	sndPkg->userHeader.C._C.FCB = 0x00;\
	sndPkg->userHeader.A3._A3.TAG = 0;\
	sndPkg->userHeader.A3._A3.MSA = Jzq::s_MSA;\
	sndPkg->userHeader.A1 = con->m_jzq.m_areacode;\
	sndPkg->userHeader.A2 = con->m_jzq.m_number;\
	sndPkg->pAfn = afnData;\
	sndPkg->pAfn->afnHeader.SEQ._SEQ.PRSEQ = g_JzqConList->GetPSEQ(sndPkg->userHeader.A1,sndPkg->userHeader.A2);\
	sndPkg->userHeader.C._C.FUN = n;\
	sndPkg->okPkg();\
	ret = con->SendPkg(sndPkg);\
	call.AFN = sndPkg->pAfn->afnHeader.AFN;\
	call.m_areacode = con->m_jzq.m_areacode;\
	call.m_number = con->m_jzq.m_number;\
	call.m_fn = AFNPackage::GetFn(sndPkg->pAfn->pAfnData->m_Tag.DT1,sndPkg->pAfn->pAfnData->m_Tag.DT2);\
	call.m_pn = AFNPackage::Getpn(sndPkg->pAfn->pAfnData->m_Tag.DA1,sndPkg->pAfn->pAfnData->m_Tag.DA2);\
	delete sndPkg;}while(0)

/*请求1级数据*/
#define REQUEST_DATA_I(con,afnData) REQUEST_DATA(con,afnData,10)
/*请求2级数据*/
#define REQUEST_DATA_II(con,afnData) REQUEST_DATA(con,afnData,11)

#define WAIT_CALL(val) Pkg_Afn_Data* p = Wait(call);\
					if (val){\
						(*val) = p;\
					}\
					return ret;

AFNPackageBuilder* AFNPackageBuilder::single = NULL;
AFNPackageBuilder::AFNPackageBuilder(void)
	:PackageBuilder()
{
}
AFNPackageBuilder::~AFNPackageBuilder(void)
{
}

IPackage* AFNPackageBuilder::CreateAck(IPackage* _reqPkg, void* data)
{
	AFNPackage* reqPkg = (AFNPackage*)_reqPkg;
	AFNPackage* ackPkg = new AFNPackage();
	ackPkg->userHeader.C._C.DIR = 0x00;
	ackPkg->userHeader.C._C.PRM = 0x00;
	ackPkg->userHeader.C._C.FCV = 0x00;
	ackPkg->userHeader.C._C.FCB = 0x00;
	ackPkg->userHeader.C._C.FUN = 11;//链路状态，
	ackPkg->userHeader.A3._A3.TAG = 0;//单地址
	ackPkg->userHeader.A3._A3.MSA = Jzq::s_MSA;
	ackPkg->userHeader.A1 = reqPkg->userHeader.A1;
	ackPkg->userHeader.A2 = reqPkg->userHeader.A2;
	ackPkg->pAfn->afnHeader.AFN = Pkg_Afn_Header::AFN00;//确认
	ackPkg->pAfn->afnHeader.SEQ._SEQ.CON = Pkg_Afn_Header::SEQ_CON_NOANSWER;
	ackPkg->pAfn->afnHeader.SEQ._SEQ.FIN = 1;
	ackPkg->pAfn->afnHeader.SEQ._SEQ.FIR = 1;
	ackPkg->pAfn->afnHeader.SEQ._SEQ.TPV = Pkg_Afn_Header::SEQ_TPV_NO;						
	ackPkg->pAfn->afnHeader.SEQ._SEQ.PRSEQ = g_JzqConList->GetRSEQ(reqPkg->userHeader.A1,reqPkg->userHeader.A2);	
	if (!data) data = new AFNAck_Data();
	ackPkg->pAfn->pAfnData = (Pkg_Afn_Data*)data;
	ackPkg->okPkg();
	return ackPkg;
}

int AFNPackageBuilder::DoHandleAck(std::list<IPackage*>& ackLst)
{	
	if (ackLst.size() == 0){
		return -1;
	}
	
	Pkg_Afn_Data* pDataHead = NULL;
	Pkg_Afn_Data* pDataTail = NULL;
	AppCall call;
	
	for (std::list<IPackage*>::iterator it = ackLst.begin(); it != ackLst.end(); it++){
		AFNPackage* ackPkg = (AFNPackage*)(*it);		
		LOG(LOG_INFORMATION,"Handle ack pkg , AFN(0x%x),Fn(%d),pn(%d) ",ackPkg->pAfn->afnHeader.AFN,ackPkg->Fn,ackPkg->pn);
		call.AFN = ackPkg->pAfn->afnHeader.AFN;
		call.m_areacode = ackPkg->userHeader.A1;
		call.m_number = ackPkg->userHeader.A2;
		call.m_fn = ackPkg->Fn;
		call.m_pn = ackPkg->pn;
		if (ackPkg->userHeader.C._C.FUN == Pkg_User_Header::UH_FUNC_SUB8){
			//用户数据
			LOG(LOG_INFORMATION,"Handle ack pkg , data tag setted");
			Pkg_Afn_Data* pData = NULL;
			for (mapIter it = handlerMap.begin(); it != handlerMap.end(); it++){
				PkgHandler& h = (*it).second;
				if (h.ackHandler && (*it).first == ackPkg->pAfn->afnHeader.AFN){
					pData = (Pkg_Afn_Data*)h.ackHandler(ackPkg);
				}
			}
			if (pData){
				LOG(LOG_INFORMATION,"Handle ack pkg , has got data");
				pData->HandleData();			
				if (!pDataHead){
					pDataHead = pData;
					pDataTail = pData;
				}
				else{
					pDataTail->m_next = pData;
					pDataTail = pData;
				}
			}
		}
		else if(ackPkg->userHeader.C._C.FUN == Pkg_User_Header::UH_FUNC_SUB9){
			//否认：无所召唤的数据		
			LOG(LOG_INFORMATION,"Handle ack pkg , no data tag");
		}
		else{
			;
		}
	}
	AFNPackageBuilder::Instance().notify(call,pDataHead);
	return YQER_OK;;
}
Pkg_Afn_Data* AFNPackageBuilder::Wait(const AppCall& call)
{
#ifdef _WIN32
	Pkg_Afn_Data* p = NULL;
	EnterCriticalSection(&CritSection);
	cmdMap[call] = NULL;
	while( cmdMap[call] == NULL )
	{
		if (SleepConditionVariableCS(&ConditionVar, &CritSection, 10000) == 0){
			YQLogInfo("SleepConditionVariableCS timeout");
			break;
		}
	}
	p = cmdMap[call];
	cmdMap.erase(call);
	LeaveCriticalSection(&CritSection);
	return p;
#else
	Pkg_Afn_Data* p = NULL;
	struct timeval now;
	struct timespec outtime;		
	pthread_mutex_lock(&CritSection);
	cmdMap[call] = NULL;
	while( cmdMap[call] == NULL )
	{
		gettimeofday(&now, NULL);
		outtime.tv_sec = now.tv_sec + 10;
		outtime.tv_nsec = now.tv_usec * 1000;
		if (pthread_cond_timedwait(&ConditionVar,&CritSection,&outtime) == ETIMEDOUT){
			YQLogInfo("pthread_cond_timedwait timeout");
			break;
		}
	}
	p = cmdMap[call];
	cmdMap.erase(call);
	pthread_mutex_unlock(&CritSection);
	return p;
#endif
}
void AFNPackageBuilder::saveResultToDB(const AppCall& call,Pkg_Afn_Data* data)
{
	if (call.AFN == Pkg_Afn_Header::AFN0C && data)
	{
		//采集数据自动入数据库
		data->toDB(call.m_areacode,call.m_number,call.m_fn,call.m_pn);
		return;
	}
}
int AFNPackageBuilder::notify(const AppCall& call,Pkg_Afn_Data* data)
{
	LOG(LOG_INFORMATION,"notify with data(0x%X)...",(int)data);
	//called in TcpServer thread
#ifdef _WIN32
	EnterCriticalSection(&CritSection);
	if (cmdMap.find(call) != cmdMap.end()){
		cmdMap[call] = data;
		saveResultToDB(call,data);
		//data 由应用等待结果线程删除
		WakeAllConditionVariable(&ConditionVar);
	}
	else{		
		//异步结果
		saveResultToDB(call,data);
		delete data;
	}
	LeaveCriticalSection(&CritSection);
	LOG(LOG_INFORMATION,"notify with data(0x%X) over",(int)data);
	return 0;
#else
	pthread_mutex_lock(&CritSection);
	if (cmdMap.find(call) != cmdMap.end()){
		cmdMap[call] = data;
		saveResultToDB(call,data);
		//data 由应用等待结果线程删除
		pthread_cond_broadcast(&ConditionVar);
	}
	else{
		//异步结果
		saveResultToDB(call,data);
		delete data;
	}
	pthread_mutex_unlock(&CritSection);
	LOG(LOG_INFORMATION,"notify with data(0x%X) over",(int)data);
	return 0;
#endif
}
int AFNPackageBuilder::setpointparams(Pkg_Afn_Data** val,std::string name,WORD pn)
{
	BEGIN_CALL()
	{
		LOCK_CONNECTION(name)
		AFN04* afnData = new AFN04();
		afnData->CreatePointBaseSetting(pn,TRUE);
		REQUEST_DATA_II(con,afnData);
	}
	WAIT_CALL(val)
}
int AFNPackageBuilder::setpointstatus(Pkg_Afn_Data** val,std::string name)
{
	BEGIN_CALL()
	{
		LOCK_CONNECTION(name)
		AFN04* afnData = new AFN04();
		afnData->CreatePointStatus();
		REQUEST_DATA_I(con,afnData);
	}
	WAIT_CALL(val)
}
int AFNPackageBuilder::getclock(Pkg_Afn_Data** val,std::string name)
{
	BEGIN_CALL()
	{
		LOCK_CONNECTION(name)
		AFN0C* afnData = new AFN0C();
		afnData->CreateClock();
		REQUEST_DATA_I(con,afnData);
	}
	WAIT_CALL(val)
}
int AFNPackageBuilder::getstatus(Pkg_Afn_Data** val,std::string name)
{
	BEGIN_CALL()
	{
		LOCK_CONNECTION(name)
		AFN0C* afnData = new AFN0C();
		afnData->CreateRunStatus();
		REQUEST_DATA_I(con,afnData);
	}
	WAIT_CALL(val)
}
int AFNPackageBuilder::getallkwh(Pkg_Afn_Data** val,std::string name,WORD pn)
{
	BEGIN_CALL()
	{
		LOCK_CONNECTION(name)
		AFN0C* afnData = new AFN0C();
		afnData->CreateAllKwh(pn);
		REQUEST_DATA_I(con,afnData);
	}
	WAIT_CALL(val)
}
int AFNPackageBuilder::getallkwh_async(Jzq* pJzq)
{
	BEGIN_CALL()
	
	LOCK_CONNECTION(pJzq->m_name)
	for (Jzq::EleIter eleIt = pJzq->eleLst.begin(); eleIt != pJzq->eleLst.end(); eleIt++){
		AFN0C* afnData = new AFN0C();
		afnData->CreateAllKwh((*eleIt).pn);
		REQUEST_DATA_I(con,afnData);
		LOG(LOG_INFORMATION,"getallkwh_async (%s,%d)",pJzq->m_name.c_str(),(*eleIt).pn);
		Utility::Sleep(50);
	}
	return ret;
}
int AFNPackageBuilder::getallkwh_async(std::string name,WORD pn)
{
	BEGIN_CALL()
	
	LOCK_CONNECTION(name)
	AFN0C* afnData = new AFN0C();
	afnData->CreateAllKwh(pn);
	REQUEST_DATA_I(con,afnData);

	LOG(LOG_INFORMATION,"getallkwh_async (%s,%d)",name.c_str(),pn);
	return ret;
}
