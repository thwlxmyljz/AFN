#include "event2/thread.h"
#include "TcpServer.h"
#include "YQErrCode.h"
#include "LogFileu.h"
#include "Connection.h"
#include "AFNPackage.h"
#include "YQUtils.h"
#include "AFNPackageBuilder.h"

#define VER "1.0.0"

TcpServer::TcpServer(unsigned int port)
	:m_svrPort(port),base(NULL),listener(NULL),signal_event(NULL)
{
	LogFile = new TLogFile("afn.log",16*1024*1024,"APN",VER);
	g_JzqConList = new JzqList();	
	g_JzqConList->LoadJzq();	
}
TcpServer::~TcpServer(void)
{
	delete LogFile;
	LogFile = NULL;
	delete g_JzqConList;
	g_JzqConList = NULL;
}
int TcpServer::Run()
{
	if (base){
		YQLogInfo("Server has already run!");
		return 0;
	}

#ifdef _WIN32    
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);
	evthread_use_windows_threads();//win上设置
#endif
	struct sockaddr_in sin;
    base = event_base_new();
    if (!base) {
        YQLogInfo("Could not initialize libevent!");
        return YQER_SVR_Err(1);
    }
	evthread_make_base_notifiable(base);

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

    event_base_dispatch(base);

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
