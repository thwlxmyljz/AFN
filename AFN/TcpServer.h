#pragma once

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>

#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include "event2/listener.h"
#include "event2/util.h"
#include "event2/event.h"

class ZjqList;

class TcpServer
{
public:
	TcpServer(unsigned int port);
	~TcpServer(void);
	//运行服务器
	int Run();
private:
	//libevent回调
	static void signal_cb(evutil_socket_t sig, short events, void *user_data);
	static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,\
							struct sockaddr *sa, int socklen, void *user_data);
private:
	//侦听端口
	unsigned int m_svrPort;
	//libevent对象
	struct event_base *base;
    struct evconnlistener *listener;
    struct event *signal_event;
};

