#include "ConnectionPower.h"
#include "AFNPackage.h"
#include "LogFileu.h"
#include "YQUtils.h"
#include "AFNPackageBuilder.h"
#include "ConnectionList.h"
#include "Lock.h"
#include "Log.h"
#include <sstream>
//-----------------------------------------------------------------------------------
ConnectionPower::ConnectionPower(struct event_base *base,struct bufferevent *_bev,evutil_socket_t _fd,struct sockaddr *sa)
	:EventConnection(base,_bev,_fd,sa,AFNPackageBuilder::Instance())
{	
}
ConnectionPower::~ConnectionPower(void)
{
	Jzq *p = g_JzqConList->getJzq(m_jzq.m_areacode,m_jzq.m_number);
	if (p){
		p->LoginState(2,0);
	}
}
IPackage* ConnectionPower::createPackage()
{
	return (IPackage*)(new AFNPackage());
}
int ConnectionPower::handlePackage(IPackage* ipkg)
{
	YQLogInfo("ConnectionPower::handlePackage ...");
	AFNPackage* pkg = (AFNPackage*)ipkg;
	std::list<IPackage* > ackLst;	
	if (pkg->userHeader.A3._A3.TAG == 0){
		//单地址
		m_jzq.m_areacode = pkg->userHeader.A1;
		m_jzq.m_number = pkg->userHeader.A2;				
	}
	if (pkg->pAfn->afnHeader.SEQ._SEQ.FIN == 1  && pkg->pAfn->afnHeader.SEQ._SEQ.FIR == 1) {
		//单帧
		YQLogInfo("rec single pkg ");
		int nRet = builder.HandlePkg(pkg,ackLst);
		if (nRet == YQER_OK && ackLst.size() > 0){
			SendPkg(ackLst);
			ClearPkgList(ackLst);
		}
		delete pkg;
	}
	else if (pkg->pAfn->afnHeader.SEQ._SEQ.FIN == 0  && pkg->pAfn->afnHeader.SEQ._SEQ.FIR == 1) {
		//多帧，第一帧
		ClearPkgList(m_pkgList);
		YQLogInfo("rec mul pkg , first");
		m_pkgList.push_back(pkg);

		//in test, no another pkg after this first pkg, so handle this pkg
		int nRet = AFNPackageBuilder::Instance().HandlePkg(m_pkgList,ackLst);
		if (nRet == YQER_OK && ackLst.size() > 0){
			SendPkg(ackLst);
			ClearPkgList(ackLst);
		}	
		ClearPkgList(m_pkgList);
		delete pkg;
	}
	else if (pkg->pAfn->afnHeader.SEQ._SEQ.FIR == 0) {
		//多帧，中间帧
		m_pkgList.push_back(pkg);		
		if (pkg->pAfn->afnHeader.SEQ._SEQ.FIN == 0){
			YQLogInfo("rec mul pkg , middle");
		}
		else{
			//多帧，结束帧
			YQLogInfo("rec mul pkg , end");
			int nRet = AFNPackageBuilder::Instance().HandlePkg(m_pkgList,ackLst);
			if (nRet == YQER_OK && ackLst.size() > 0){
				SendPkg(ackLst);
				ClearPkgList(ackLst);
			}	
			ClearPkgList(m_pkgList);
			delete pkg;
		}		
	}
	else{
		YQLogMin("rec pkg,FIN|FIR error");
	}
	return YQER_OK;
}
