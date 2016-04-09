#include "Connection.h"
#include "AFNPackageBuilder.h"
#include "YQErrCode.h"
#include "Lock.h"
#include "AFN04.h"
#include "AFN0C.h"
#include "LogFileu.h"

#define LOCK_GETDEV() Lock lock(g_JzqConListMutex);\
	Jzq* dev = g_JzqConList->getJzq(name);\
	if (!dev){\
		return YQER_JZQ_NOTFOUND;\
	}\
	if (!dev->m_conn){\
		return YQER_JZQ_NOLOGIN;\
	}

#define SET_COMMPARAMS(ackPkg,dev,afnData) ackPkg->userHeader.C._C.DIR = 0x00;\
	ackPkg->userHeader.C._C.PRM = 0x01;\
	ackPkg->userHeader.C._C.FCV = 0x00;\
	ackPkg->userHeader.C._C.FCB = 0x00;\
	ackPkg->userHeader.A3._A3.TAG = 0;\
	ackPkg->userHeader.A3._A3.MSA = Jzq::s_MSA;\
	ackPkg->userHeader.A1 = dev->m_areacode;\
	ackPkg->userHeader.A2 = dev->m_number;\
	ackPkg->pAfn = afnData;\
	ackPkg->pAfn->afnHeader.SEQ._SEQ.PRSEQ = g_JzqConList->GetPSEQ(ackPkg->userHeader.A1,ackPkg->userHeader.A2);

#define BEGIN_CALL() AppCall call;\
					int ret = YQER_OK;

#define SET_CALL(call) call.AFN = ackPkg->pAfn->afnHeader.AFN;\
		call.m_areacode = dev->m_areacode;\
		call.m_number = dev->m_number;

#define END_CALL_WAIT() Pkg_Afn_Data* p = Wait(call);\
					if (val){\
						(*val) = p;\
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
			return DoHandleRequest(reqLst,ackLst);
		}
		else{
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
	AFNPackage* ackPkg = *(ackLst.begin());	
	for (mapIter it = handlerMap.begin(); it != handlerMap.end(); it++){
		PkgHandler& h = (*it).second;
		if (h.ackHandler && (*it).first == ackPkg->pAfn->afnHeader.AFN){
			return h.ackHandler(ackLst);
		}
	}
	return -1;
}
Pkg_Afn_Data* AFNPackageBuilder::Wait(const AppCall& call)
{
#ifdef _WIN32
	Pkg_Afn_Data* p = NULL;
	EnterCriticalSection(&CritSection);
	cmdMap[call] = NULL;
	while( cmdMap[call] == NULL )
	{
		if (SleepConditionVariableCS(&ConditionVar, &CritSection, 2000) == 0){
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
		outtime.tv_sec = now.tv_sec + 2;
		outtime.tv_nsec = now.tv_usec * 1000;
		if (pthread_cond_timedwait(&ConditionVar,&CritSection,&outtime) == ETIMEDOUT){
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
#ifdef _WIN32
	EnterCriticalSection(&CritSection);
	if (cmdMap.find(call) != cmdMap.end()){
		cmdMap[call] = data;		
		WakeAllConditionVariable(&ConditionVar);
	}
	LeaveCriticalSection(&CritSection);
	return 0;
#else
	pthread_mutex_lock(&CritSection);
	if (cmdMap.find(call) != cmdMap.end()){
		cmdMap[call] = data;
		pthread_cond_broadcast(&ConditionVar);
	}
	pthread_mutex_unlock(&CritSection);
	return 0;
#endif
}
int AFNPackageBuilder::setpointparams(Pkg_Afn_Data** val,std::string name,WORD pn)
{
	BEGIN_CALL()
	{
		LOCK_GETDEV()
		AFN04* afnData = new AFN04();
		afnData->CreatePointBaseSetting(pn,TRUE);
		AFNPackage* ackPkg = new AFNPackage();	
		SET_COMMPARAMS(ackPkg,dev,afnData)
		ackPkg->userHeader.C._C.FUN = 11;//请求2级数据		
		ackPkg->okPkg();
		ret = dev->m_conn->SendPkg(ackPkg);
		SET_CALL(call)
	}
	END_CALL_WAIT()
}
int AFNPackageBuilder::setpointstatus(Pkg_Afn_Data** val,std::string name)
{
	BEGIN_CALL()
	{
		LOCK_GETDEV()
		AFN04* afnData = new AFN04();
		afnData->CreatePointStatus();
		AFNPackage* ackPkg = new AFNPackage();	
		SET_COMMPARAMS(ackPkg,dev,afnData)
		ackPkg->userHeader.C._C.FUN = 10;//请求1级数据		
		ackPkg->okPkg();
		ret = dev->m_conn->SendPkg(ackPkg);
		SET_CALL(call)
	}
	END_CALL_WAIT()
}
int AFNPackageBuilder::getclock(Pkg_Afn_Data** val,std::string name)
{
	BEGIN_CALL()
	{
		LOCK_GETDEV()
		AFN0C* afnData = new AFN0C();
		afnData->CreateClock();
		AFNPackage* ackPkg = new AFNPackage();	
		SET_COMMPARAMS(ackPkg,dev,afnData)
		ackPkg->userHeader.C._C.FUN = 10;//请求1级数据	
		ackPkg->okPkg();
		ret = dev->m_conn->SendPkg(ackPkg);
		SET_CALL(call)
	}
	END_CALL_WAIT()
}
int AFNPackageBuilder::getstatus(Pkg_Afn_Data** val,std::string name)
{
	BEGIN_CALL()
	{
		LOCK_GETDEV()
		AFN0C* afnData = new AFN0C();
		afnData->CreateRunStatus();
		AFNPackage* ackPkg = new AFNPackage();	
		SET_COMMPARAMS(ackPkg,dev,afnData)
		ackPkg->userHeader.C._C.FUN = 10;//请求1级数据	
		ackPkg->okPkg();
		ret = dev->m_conn->SendPkg(ackPkg);
		SET_CALL(call)
	}
	END_CALL_WAIT()
}
int AFNPackageBuilder::getallkwh(Pkg_Afn_Data** val,std::string name,WORD pn)
{
	BEGIN_CALL()
	{
		LOCK_GETDEV()
		AFN0C* afnData = new AFN0C();
		afnData->CreateAllKwh(pn);
		AFNPackage* ackPkg = new AFNPackage();	
		SET_COMMPARAMS(ackPkg,dev,afnData)
		ackPkg->userHeader.C._C.FUN = 10;//请求1级数据	
		ackPkg->okPkg();
		ret = dev->m_conn->SendPkg(ackPkg);
		SET_CALL(call)
	}
	END_CALL_WAIT()
}
