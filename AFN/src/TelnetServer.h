#pragma once
#include "Thread.h"
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

#include <string>

class TelnetServer
{
public:
	TelnetServer(unsigned int port=5555);
	~TelnetServer(void);
	//���з�����
	int Run();
private:
	//libevent�ص�
	static void conn_readcb(struct bufferevent *bev, void *user_data);
	static void conn_writecb(struct bufferevent *bev, void *user_data);
	static void conn_eventcb(struct bufferevent *bev, short events, void *user_data);

	static void signal_cb(evutil_socket_t sig, short events, void *user_data);
	static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,struct sockaddr *sa, int socklen, void *user_data);

	/*�ɼ�����*/
	static void timeout_cb_kwh(evutil_socket_t fd, short event, void *arg);

	//��ʱ���ص������赱ǰ��ʱ��
	static void resetTimer(evutil_socket_t fd, short event, void *arg);

private:
	//�����˿�
	unsigned int m_svrPort;

	//libevent����
	struct event_base *base;
   	struct evconnlistener *listener;	

	//��ʱ�ɼ�����������
	struct event timeout_event_kwh;
};

class TelnetThread : public Thread
{
public:
	virtual void  Run();
private:
	TelnetServer svr;
};

