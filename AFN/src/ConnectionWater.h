#pragma once

#include "EventConnection.h"
//------------------------------------------------------------------------------------
class ConnectionWater : public EventConnection
{
public:
	ConnectionWater(struct event_base *base,struct bufferevent *_bev,evutil_socket_t _fd,struct sockaddr *sa);
	virtual ~ConnectionWater(void);
	
protected:
	//�����
	virtual IPackage* createPackage();
	//�����
	virtual int handlePackage(IPackage* pkg);
};
