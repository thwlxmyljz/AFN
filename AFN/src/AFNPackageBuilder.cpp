#include "Connection.h"
#include "ConnectionList.h"
#include "AFNPackageBuilder.h"
#include "YQErrCode.h"
#include "Lock.h"
#include "AFN04.h"
#include "AFN0C.h"
#include "LogFileu.h"

#define AUTOLOCK_GETCONNECTION(name) AUTO_LOCK()\
					Connection* con = g_JzqConList->getConnection(name);\
					if (!con){\
						return YQER_JZQ_NOTOK;\
					}\
					con->ClearRecPkgList();

#define SET_COMMPARAMS(ackPkg,con,afnData) ackPkg->userHeader.C._C.DIR = 0x00;\
	ackPkg->userHeader.C._C.PRM = 0x01;\
	ackPkg->userHeader.C._C.FCV = 0x00;\
	ackPkg->userHeader.C._C.FCB = 0x00;\
	ackPkg->userHeader.A3._A3.TAG = 0;\
	ackPkg->userHeader.A3._A3.MSA = Jzq::s_MSA;\
	ackPkg->userHeader.A1 = con->m_jzq.m_areacode;\
	ackPkg->userHeader.A2 = con->m_jzq.m_number;\
	ackPkg->pAfn = afnData;\
	ackPkg->pAfn->afnHeader.SEQ._SEQ.PRSEQ = g_JzqConList->GetPSEQ(ackPkg->userHeader.A1,ackPkg->userHeader.A2);

#define BEGIN_CALL() AppCall call;\
					int ret = YQER_OK;

#define SET_CALL() call.AFN = sndPkg->pAfn->afnHeader.AFN;\
		call.m_areacode = con->m_jzq.m_areacode;\
		call.m_number = con->m_jzq.m_number;\
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
							p->toDB(call.m_areacode,call.m_number);\
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
		if (SleepConditionVariableCS(&ConditionVar, &CritSection, 2000) == 0){
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
		outtime.tv_sec = now.tv_sec + 2;
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
#ifdef _WIN32
	EnterCriticalSection(&CritSection);
	if (cmdMap.find(call) != cmdMap.end()){
		cmdMap[call] = data;		
		WakeAllConditionVariable(&ConditionVar);
	}
	else{		
		YQLogInfo("Notify ,but not found call");
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
		YQLogInfo("Notify ,but not found call");
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
		SET_CALL()
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
		SET_CALL()
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
		SET_CALL()
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
		SET_CALL()
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
		ret = con->SendPkg(sndPkg);
		SET_CALL()
	}
	WAIT_CALL_SAVETODB(val)
}
