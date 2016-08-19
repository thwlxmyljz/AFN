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
#include "event2/event_struct.h"

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
	static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,struct sockaddr *sa, int socklen, void *user_data);
	
	/*集中器在线定时检测*/
	static void timeout_cb_heart(evutil_socket_t fd, short event, void *arg);
	/*定时采集电力数据*/
	static void timeout_cb_kwh(evutil_socket_t fd, short event, void *arg);
private:
	//侦听端口
	unsigned int m_svrPort;
	//libevent对象
	struct event_base *base;
    struct evconnlistener *listener;
    struct event *signal_event;

	//集中器超时断线检测	
	struct event timeout_event_checkjzq;
	//定时采集集中器数据
	struct event timeout_event_kwh;
};

