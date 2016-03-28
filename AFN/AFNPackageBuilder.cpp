#include "stdafx.h"
#include "AFNPackageBuilder.h"
#include "YQErrCode.h"

AFNPackageBuilder* AFNPackageBuilder::single = NULL;
AFNPackageBuilder::AFNPackageBuilder(void)
{
}
AFNPackageBuilder::~AFNPackageBuilder(void)
{
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
			return DoHandleAck(reqLst,ackLst);
		}
	}
	return YQER_PARAMERR;
}
int AFNPackageBuilder::DoHandleRequest(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst)
{
	AFNPackage* reqPkg = *(reqLst.begin());	
	for (mapIter it = handlerMap.begin(); it != handlerMap.end(); it++){
		PkgHandler& h = (*it).second;
		if ((*it).first == reqPkg->pAfn->afnHeader.AFN){
			return h.reqHander(reqLst,ackLst);		
		}
	}
	return -1;
}

int AFNPackageBuilder::DoHandleAck(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst)
{
	AFNPackage* reqPkg = *(reqLst.begin());	
	for (mapIter it = handlerMap.begin(); it != handlerMap.end(); it++){
		PkgHandler& h = (*it).second;
		if ((*it).first == reqPkg->pAfn->afnHeader.AFN){
			return h.ackHandler(reqLst,ackLst);
		}
	}
	return -1;
}

