#pragma once

#include "EventConnection.h"
//------------------------------------------------------------------------------------
class ConnectionWater : public EventConnection
{
public:
	ConnectionWater(struct event_base *base,struct bufferevent *_bev,evutil_socket_t _fd,struct sockaddr *sa);
	virtual ~ConnectionWater(void);
	
protected:
	//构造包
	virtual IPackage* createPackage();
	//处理包
	virtual int handlePackage(IPackage* pkg);
};
