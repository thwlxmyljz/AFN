#include "Connection.h"
#include "ConnectionList.h"
#include "AFNPackageBuilder.h"
#include "YQErrCode.h"
#include "Lock.h"
#include "AFN04.h"
#include "AFN0C.h"
#include "LogFileu.h"
#include "Log.h"

#define AUTOLOCK_GETCONNECTION(name) AUTO_LOCK()\
					Connection* con = g_JzqConList->getConnection(name);\
					if (!con){\
						return YQER_JZQ_NOTOK;\
					}\
					con->ClearRecPkgList();

#define SET_COMMPARAMS(pkg,con,afnData) pkg->userHeader.C._C.DIR = 0x00;\
	pkg->userHeader.C._C.PRM = 0x01;\
	pkg->userHeader.C._C.FCV = 0x00;\
	pkg->userHeader.C._C.FCB = 0x00;\
	pkg->userHeader.A3._A3.TAG = 0;\
	pkg->userHeader.A3._A3.MSA = Jzq::s_MSA;\
	pkg->userHeader.A1 = con->m_jzq.m_areacode;\
	pkg->userHeader.A2 = con->m_jzq.m_number;\
	pkg->pAfn = afnData;\
	pkg->pAfn->afnHeader.SEQ._SEQ.PRSEQ = g_JzqConList->GetPSEQ(pkg->userHeader.A1,pkg->userHeader.A2);

#define BEGIN_CALL() AppCall call;\
					int ret = YQER_OK;

#define SET_CALL_DELPKG() call.AFN = sndPkg->pAfn->afnHeader.AFN;\
		call.m_areacode = con->m_jzq.m_areacode;\
		call.m_number = con->m_jzq.m_number;\
		call.m_fn = AFNPackage::GetFn(sndPkg->pAfn->pAfnData->m_Tag.DT1,sndPkg->pAfn->pAfnData->m_Tag.DT2);\
		call.m_pn = AFNPackage::Getpn(sndPkg->pAfn->pAfnData->m_Tag.DA1,sndPkg->pAfn->pAfnData->m_Tag.DA2);\
		delete sndPkg;

#define WAIT_CALL(val) Pkg_Afn_Data* p = Wait(call);\
					if (val){\
						(*val) = p;\
					}\
					return ret;
					
#define WAIT_CALL_SAVETODB(val) Pkg_Afn_Data* p = Wait(call);\
					if (val){\
						(*val) = p;\
						if (p) \
							p->toDB(call.m_areacode,call.m_number,,call.m_fn,call.m_pn);\
					}\
					return ret;


AFNPackageBuilder* AFNPackageBuilder::single = NULL;
AFNPackageBuilder::AFNPackageBuilder(void)
{
}
AFNPackageBuilder::~AFNPackageBuilder(void)
{
#ifdef _WIN32
#else
	pthread_mutex_destroy(&CritSection);  
	pthread_cond_destroy(&ConditionVar);  
#endif
}
void AFNPackageBuilder::Register(Pkg_Afn_Header::AFN_CODE code,pfnDoHandleRequest reqHandler,pfnDoHandleAck ackHandler)
{
	PkgHandler h;
	h.reqHander = reqHandler;
	h.ackHandler = ackHandler;
	handlerMap[code] = h;
}
int AFNPackageBuilder::HandlePkg(AFNPackage* reqPkg,std::list<AFNPackage*>& ackLst)
{
	std::list<AFNPackage*> lst;
	lst.push_back(reqPkg);
	return HandlePkg(lst,ackLst);
}
int AFNPackageBuilder::HandlePkg(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst)
{
	if((reqLst.size()) >= 1){
		AFNPackage* reqPkg = *(reqLst.begin());
		if (reqPkg->userHeader.C._C.PRM == 1){
			//收到集中器作为启动站请求帧
			return DoHandleRequest(reqLst,ackLst);
		}
		else{
			//收到集中器作为从动站响应帧
			return DoHandleAck(reqLst);
		}
	}
	return YQER_PARAMERR;
}
int AFNPackageBuilder::DoHandleRequest(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst)
{
	AFNPackage* reqPkg = *(reqLst.begin());	
	for (mapIter it = handlerMap.begin(); it != handlerMap.end(); it++){
		PkgHandler& h = (*it).second;
		if (h.reqHander && (*it).first == reqPkg->pAfn->afnHeader.AFN){
			return h.reqHander(reqLst,ackLst);		
		}
	}
	return -1;
}

int AFNPackageBuilder::DoHandleAck(std::list<AFNPackage*>& ackLst)
{	
	if (ackLst.size() == 0){
		return -1;
	}
	
	Pkg_Afn_Data* pDataHead = NULL;
	Pkg_Afn_Data* pDataTail = NULL;
	AppCall call;
	
	for (std::list<AFNPackage*>::iterator it = ackLst.begin(); it != ackLst.end(); it++){
		AFNPackage* ackPkg = *it;			
		call.AFN = ackPkg->pAfn->afnHeader.AFN;
		call.m_areacode = ackPkg->userHeader.A1;
		call.m_number = ackPkg->userHeader.A2;
		if (ackPkg->userHeader.C._C.FUN == Pkg_User_Header::UH_FUNC_SUB8){
			//用户数据
			Pkg_Afn_Data* pData = NULL;
			for (mapIter it = handlerMap.begin(); it != handlerMap.end(); it++){
				PkgHandler& h = (*it).second;
				if (h.ackHandler && (*it).first == ackPkg->pAfn->afnHeader.AFN){
					pData = h.ackHandler(ackPkg);
				}
			}
			if (pData){
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
		}
		else{
			;
		}
	}
	AFNPackageBuilder::Instance().Notify(call,pDataHead);
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

int AFNPackageBuilder::Notify(const AppCall& call,Pkg_Afn_Data* data)
{
	//called in TcpServer thread
#ifdef _WIN32
	EnterCriticalSection(&CritSection);
	if (cmdMap.find(call) != cmdMap.end()){
		cmdMap[call] = data;
		if (call.AFN == Pkg_Afn_Header::AFN0C)
		{
			//采集数据自动入数据库
			if (data) 
				data->toDB(call.m_areacode,call.m_number,call.m_fn,call.m_pn);
		}
		WakeAllConditionVariable(&ConditionVar);
	}
	else{		
		//异步结果
		if (call.AFN == Pkg_Afn_Header::AFN0C)
		{
			YQLogInfo("Notify , auto save to ele kwh");
			//采集数据自动入数据库, maybe save data to another thread for save to db
			if (data) 
				data->toDB(call.m_areacode,call.m_number,call.m_fn,call.m_pn);
		}
		else{
			YQLogInfo("Notify ,maybe async call , no handler");
		}
		delete data;
	}
	LeaveCriticalSection(&CritSection);
	return 0;
#else
	pthread_mutex_lock(&CritSection);
	if (cmdMap.find(call) != cmdMap.end()){
		cmdMap[call] = data;
		pthread_cond_broadcast(&ConditionVar);
	}
	else{
		//异步结果
		if (call.AFN == Pkg_Afn_Header::AFN0C)
		{
			YQLogInfo("Notify , auto save to ele kwh");
			//采集数据自动入数据库, maybe save data to another thread for save to db
			if (data) 
				data->toDB(call.m_areacode,call.m_number,call.m_fn,call.m_pn);
		}
		else{
			YQLogInfo("Notify ,maybe async call , no handler");
		}
		delete data;
	}
	pthread_mutex_unlock(&CritSection);
	return 0;
#endif
}
int AFNPackageBuilder::setpointparams(Pkg_Afn_Data** val,std::string name,WORD pn)
{
	BEGIN_CALL()
	{
		AUTOLOCK_GETCONNECTION(name)
		AFN04* afnData = new AFN04();
		afnData->CreatePointBaseSetting(pn,TRUE);
		AFNPackage* sndPkg = new AFNPackage();	
		SET_COMMPARAMS(sndPkg,con,afnData)
		sndPkg->userHeader.C._C.FUN = 11;//请求2级数据		
		sndPkg->okPkg();
		ret = con->SendPkg(sndPkg);
		SET_CALL_DELPKG()
	}
	WAIT_CALL(val)
}
int AFNPackageBuilder::setpointstatus(Pkg_Afn_Data** val,std::string name)
{
	BEGIN_CALL()
	{
		AUTOLOCK_GETCONNECTION(name)
		AFN04* afnData = new AFN04();
		afnData->CreatePointStatus();
		AFNPackage* sndPkg = new AFNPackage();	
		SET_COMMPARAMS(sndPkg,con,afnData)
		sndPkg->userHeader.C._C.FUN = 10;//请求1级数据		
		sndPkg->okPkg();
		ret = con->SendPkg(sndPkg);
		SET_CALL_DELPKG()
	}
	WAIT_CALL(val)
}
int AFNPackageBuilder::getclock(Pkg_Afn_Data** val,std::string name)
{
	BEGIN_CALL()
	{
		AUTOLOCK_GETCONNECTION(name)
		AFN0C* afnData = new AFN0C();
		afnData->CreateClock();
		AFNPackage* sndPkg = new AFNPackage();	
		SET_COMMPARAMS(sndPkg,con,afnData)
		sndPkg->userHeader.C._C.FUN = 10;//请求1级数据	
		sndPkg->okPkg();
		ret = con->SendPkg(sndPkg);
		SET_CALL_DELPKG()
	}
	WAIT_CALL(val)
}
int AFNPackageBuilder::getstatus(Pkg_Afn_Data** val,std::string name)
{
	BEGIN_CALL()
	{
		AUTOLOCK_GETCONNECTION(name)
		AFN0C* afnData = new AFN0C();
		afnData->CreateRunStatus();
		AFNPackage* sndPkg = new AFNPackage();	
		SET_COMMPARAMS(sndPkg,con,afnData)
		sndPkg->userHeader.C._C.FUN = 10;//请求1级数据	
		sndPkg->okPkg();
		ret = con->SendPkg(sndPkg);
		SET_CALL_DELPKG()
	}
	WAIT_CALL(val)
}
int AFNPackageBuilder::getallkwh(Pkg_Afn_Data** val,std::string name,WORD pn)
{
	BEGIN_CALL()
	{
		AUTOLOCK_GETCONNECTION(name)
		AFN0C* afnData = new AFN0C();
		afnData->CreateAllKwh(pn);
		AFNPackage* sndPkg = new AFNPackage();	
		SET_COMMPARAMS(sndPkg,con,afnData)
		sndPkg->userHeader.C._C.FUN = 10;//请求1级数据	
		sndPkg->okPkg();
		LOG(LOG_INFORMATION,"getallkwh_sync(%s,%d) send package...",name.c_str(),pn);
		ret = con->SendPkg(sndPkg);
		SET_CALL_DELPKG()
	}
	WAIT_CALL(val)
}
int AFNPackageBuilder::getallkwh_async(Pkg_Afn_Data** val,std::string name,WORD pn)
{
	AUTOLOCK_GETCONNECTION(name)
	AFN0C* afnData = new AFN0C();
	afnData->CreateAllKwh(pn);
	AFNPackage* sndPkg = new AFNPackage();	
	SET_COMMPARAMS(sndPkg,con,afnData)
	sndPkg->userHeader.C._C.FUN = 10;//请求1级数据	
	sndPkg->okPkg();
	LOG(LOG_INFORMATION,"getallkwh_async(%s,%d) send package...",name.c_str(),pn);
	int ret = con->SendPkg(sndPkg);
	delete sndPkg;
	return ret;
}