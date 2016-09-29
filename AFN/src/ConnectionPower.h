#pragma once

#ifndef _WIN32
#include <sys/types.h>
#include <netinet/in.h>  
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include "event2/util.h"
#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include "event2/event.h"

#include "proto.h"
#include "Mutex.h"
#include <list>
#include <map>
#include <string>
#include "Element.h"
#include "EventConnection.h"
#include "Jzq.h"

using namespace std;

class AFNPackage;

/*集中器连接*/
class ConnectionPower : public EventConnection
{
	friend class ZjqList;

public:
	ConnectionPower(struct event_base *base,struct bufferevent *_bev,evutil_socket_t _fd,struct sockaddr *sa);
	virtual ~ConnectionPower(void);

	//连接对应的集中器标记
	Jzq::JzqA1A2 m_jzq;

protected:
	//构造包
	virtual IPackage* createPackage();
	//处理包
	virtual int handlePackage(IPackage* ipkg);
};

