#include "ConnectionWater.h"
#include "LogFileu.h"
#include "YQUtils.h"
#include "Log.h"
#include "WaterPackageBuilder.h"
#include "WaterPackage.h"

ConnectionWater::ConnectionWater(struct event_base *base,struct bufferevent *_bev,evutil_socket_t _fd,struct sockaddr *sa)
	:EventConnection(base,_bev,_fd,sa,WaterPackageBuilder::Instance())
{	
}
ConnectionWater::~ConnectionWater(void)
{
}
IPackage* ConnectionWater::createPackage()
{
	return (IPackage*)(new WaterPackage());
}
int ConnectionWater::handlePackage(IPackage* pkg)
{
	YQLogInfo("ConnectionWater::handlePackage ...");
	delete pkg;
	return YQER_OK;
}
