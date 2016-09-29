#include "ConnectionList.h"
#include "WaterPackage.h"
#include "LogFileu.h"
#include "YQUtils.h"
#include "WaterPackageBuilder.h"
#include "Lock.h"
#include "YQErrCode.h"
#include "Water.h"
#include <sstream>
#include "DBManager.h"
#include "Log.h"

#define TM_GETWATER 60000 //60 secs

//水利相关
void JzqList::LoadWater()
{
}
Water* JzqList::getWater(const std::string& _name)
{
	return NULL;
}
//新的集中器连接
int JzqList::newWaterConnection(struct event_base *base,evutil_socket_t fd, struct sockaddr *sa)
{
	struct bufferevent *bev;
#ifdef _WIN32	
    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE|BEV_OPT_THREADSAFE);
#else
	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
#endif
    if (!bev)  {
		YQLogMaj("Error constructing bufferevent!");        
        return YQER_CON_Err(1);
    }

	ConnectionWater* con = new ConnectionWater(base,bev,fd,sa);
	push_back(con);

    bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, NULL);
    bufferevent_enable(bev, EV_READ|EV_WRITE);

	return YQER_OK;
}
ConnectionWater* JzqList::getWaterConnection(const std::string& _name)
{
	return NULL;
}
//定时执行所有在线采集器的水利采集
void JzqList::AutoGetAllWater()
{
	for (waterIter it = m_waterList.begin(); it != m_waterList.end(); it++){		
		Water* w = (*it);
		if (TYQUtils::TimeElapse(w->m_getTimer) > TM_GETWATER){
			//采集此集中器所带电表数据
			TYQUtils::TimeStart(w->m_getTimer);
			//
		}
	}
}
//-----------------------------------------------------------------------------------