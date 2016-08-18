#include "event2/thread.h"
#include "TcpServer.h"
#include "YQErrCode.h"
#include "LogFileu.h"
#include "Connection.h"
#include "ConnectionList.h"
#include "AFNPackage.h"
#include "YQUtils.h"
#include "AFNPackageBuilder.h"

#ifndef WIN32
#include <sys/queue.h>
#include <unistd.h>
#endif

#include <time.h>

#ifdef WIN32
#include <winsock2.h>
#endif

#define VER "1.0.1"

#define TMOUT_CHECK 60 //connection check timeout , 60 secs
struct timeval check_lasttime;

TcpServer::TcpServer(unsigned int port)
	:m_svrPort(port),base(NULL),listener(NULL),signal_event(NULL)
{
	LogFile = new TLogFile("afn.log",16*1024*1024,"APN",VER);
	g_JzqConList = new JzqList();	
}
TcpServer::~TcpServer(void)
{
	delete g_JzqConList;
	g_JzqConList = NULL;
	delete LogFile;
	LogFile = NULL;
}
int TcpServer::Run()
{
	if (base){
		YQLogInfo("Server has already run!");
		return 0;
	}
	g_JzqConList->LoadJzq();
#ifdef _WIN32    
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);
	evthread_use_windows_threads();//win上设置
	evthread_make_base_notifiable(base);
#endif
	/*initialize tcp server listen socket*/
	struct sockaddr_in sin;
    base = event_base_new();
    if (!base) {
        YQLogInfo("Could not initialize libevent!");
        return YQER_SVR_Err(1);
    }	

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
	sin.sin_port = htons(m_svrPort);

    listener = evconnlistener_new_bind(base, TcpServer::listener_cb, (void *)base,
        LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
        (struct sockaddr*)&sin,
        sizeof(sin));

    if (!listener)  {
        YQLogInfo("Could not create a listener!");
        return YQER_SVR_Err(2);
    }

    signal_event = evsignal_new(base, SIGINT, TcpServer::signal_cb, (void *)base);
    if (!signal_event || event_add(signal_event, NULL)<0)  {
        YQLogInfo("Could not create/add a signal event!");
        return YQER_SVR_Err(3);
    }
	YQLogInfo("Server start ok");

	/* Initalize heartbeat timeout event */
	event_assign(&timeout_event_checkjzq, base, -1, 0, timeout_cb_heart, (void*) &timeout_event_checkjzq);
	struct timeval tv;
	evutil_timerclear(&tv);
	tv.tv_sec = TMOUT_CHECK;
	evutil_gettimeofday(&check_lasttime, NULL);
	event_add(&timeout_event_checkjzq, &tv);
	
	/*run loop*/
    event_base_dispatch(base);

	/*run over , exit*/
	evconnlistener_free(listener);
    event_free(signal_event);
    event_base_free(base);

    YQLogInfo("Server shut down");
	return 0;
}
//信号处理
void TcpServer::signal_cb(evutil_socket_t sig, short events, void *user_data)
{
    struct event_base *base = (struct event_base *)user_data;
    struct timeval delay = { 2, 0 };

    YQLogInfo("Caught an interrupt signal; exiting cleanly in two seconds");

    event_base_loopexit(base, &delay);
}
//新客户端连接
void TcpServer::listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
	g_JzqConList->newConnection((struct event_base *)user_data,fd,sa);
}
//集中器在线定时检测
void TcpServer::timeout_cb_heart(evutil_socket_t fd, short event, void *arg)
{
	g_JzqConList->CheckConnection();

	resetTimer(fd,event,arg);
}
//定时器回调后重设当前定时器
void TcpServer::resetTimer(evutil_socket_t fd, short event, void *arg)
{
	struct timeval newtime, difference;
	struct event *timeout = (struct event *)arg;
	double elapsed;

	evutil_gettimeofday(&newtime, NULL);
	evutil_timersub(&newtime, &check_lasttime, &difference);
	elapsed = difference.tv_sec +
	    (difference.tv_usec / 1.0e6);

	printf("timeout_cb called at %d: %.3f seconds elapsed.\n",
	    (int)newtime.tv_sec, elapsed);
	check_lasttime = newtime;

	struct timeval tv;
	evutil_timerclear(&tv);
	tv.tv_sec = TMOUT_CHECK;
	event_add(timeout, &tv);
}
