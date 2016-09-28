#include "Connection.h"
#include "PackageBuilder.h"
#include "YQErrCode.h"
#include "Lock.h"
#include "LogFileu.h"
#include "Log.h"
#include "Utility.h"

PackageBuilder::PackageBuilder(void)
{
}
PackageBuilder::~PackageBuilder(void)
{
#ifdef _WIN32
#else
	pthread_mutex_destroy(&CritSection);  
	pthread_cond_destroy(&ConditionVar);  
#endif
}
void PackageBuilder::Register(int code,pfnDoHandleRequest reqHandler,pfnDoHandleAck ackHandler)
{
	PkgHandler h;
	h.reqHander = reqHandler;
	h.ackHandler = ackHandler;
	handlerMap[code] = h;
}
int PackageBuilder::HandlePkg(IPackage* reqPkg,std::list<IPackage*>& ackLst)
{
	std::list<IPackage*> lst;
	lst.push_back(reqPkg);
	return HandlePkg(lst,ackLst);
}
int PackageBuilder::HandlePkg(std::list<IPackage*>& reqLst,std::list<IPackage*>& ackLst)
{
	if((reqLst.size()) >= 1){
		IPackage* reqPkg = *(reqLst.begin());
		if (reqPkg->isRequest()){
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
int PackageBuilder::DoHandleRequest(std::list<IPackage*>& reqLst,std::list<IPackage*>& ackLst)
{	
	IPackage* reqPkg = *(reqLst.begin());	
	LOG(LOG_INFORMATION,"Handle request pkg , count(%d) ",(int)reqLst.size());
	for (mapIter it = handlerMap.begin(); it != handlerMap.end(); it++){
		PkgHandler& h = (*it).second;
		if (h.reqHander && (*it).first == reqPkg->GetCode()){
			return h.reqHander(reqLst,ackLst);		
		}
		else{
			//不认识的命令，直接回复确认
			ackLst.push_back(CreateAck(reqPkg,NULL));
			return YQER_OK;
		}
	}
	return -1;
}

int PackageBuilder::DoHandleAck(std::list<IPackage*>& ackLst)
{	
	if (ackLst.size() == 0){
		return -1;
	}
	return YQER_OK;;
}

