#include "stdafx.h"
#include "TelnetServer.h"
#include "YQErrCode.h"
#include "LogFileu.h"
#include "Connection.h"
#include "AFNPackage.h"
#include "YQUtils.h"
#include "AFNPackageBuilder.h"
#include "stdafx.h"
#include <vector>
#include <string>
using namespace std;
int split(const string& str, vector<string>& ret_, string sep)
{
	if (str.empty())
	{
		return 0;
	}

	string tmp;
	string::size_type pos_begin = str.find_first_not_of(sep);
	string::size_type comma_pos = 0;

	while (pos_begin != string::npos)
	{
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}

		if (!tmp.empty())
		{
			ret_.push_back(tmp);
			tmp.clear();
		}
	}
	return 0;
}
char g_cmd[] = "cmd:\n"
				"ls,send\n"
				"ls:list all zjq\n"
				"send [name] [afnxx]:send afnxx command to zjq name\n";
TelnetServer::TelnetServer(unsigned int port)
	:m_svrPort(port),base(NULL),listener(NULL),signal_event(NULL)
{
}
TelnetServer::~TelnetServer(void)
{
}
int TelnetServer::Run()
{
	if (base){
		YQLogInfo("TelnetServer has already run!");
		return 0;
	}
    struct sockaddr_in sin;
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);
	
    base = event_base_new();
    if (!base) {
        YQLogInfo("TelnetServer Could not initialize libevent!");
        return YQER_SVR_Err(1);
    }
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
	sin.sin_port = htons(m_svrPort);

    listener = evconnlistener_new_bind(base, TelnetServer::listener_cb, (void *)base,
        LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
        (struct sockaddr*)&sin,
        sizeof(sin));

    if (!listener)  {
        YQLogInfo("TelnetServer Could not create a listener!");
        return YQER_SVR_Err(2);
    }

    signal_event = evsignal_new(base, SIGINT, TelnetServer::signal_cb, (void *)base);
    if (!signal_event || event_add(signal_event, NULL)<0)  {
        YQLogInfo("TelnetServer Could not create/add a signal event!");
        return YQER_SVR_Err(3);
    }
	YQLogInfo("TelnetServer start ok");

	//解包测试
	/*
	BYTE data[20] = {0x68,0x32,0x00,0x32,0x00,0x68,0xc9,0x00,0x10,0x4d,0x04,0x00,0x02,0x75,0x00,0x00,0x01,0x00,0xa2,0x16};
	string hex = TYQUtils::Byte2Hex(data,20);
	printf("%s\n",hex.c_str());

	AFNPackage pkg;
	pkg.ParseProto(data,20);
	std::list<AFNPackage* > ackLst;
	AFNPackageBuilder::Instance().HandlePkg(&pkg,ackLst);
	AFNPackage* ackPkg = (*ackLst.begin());
	*/
	BYTE data[18] = {0x0B, 0xFF , 0xFF , 0xFF , 0xFF , 0x02 , 0x00 , 0x61 , 0x00 , 0x00 , 0x03 , 0x00 , 0x02 , 0x00 , 0x00 , 0x01 , 0x00 , 0x00};
	BYTE cs = AFNPackage::GetCS(data,18);

    event_base_dispatch(base);

	evconnlistener_free(listener);
    event_free(signal_event);
    event_base_free(base);

    YQLogInfo("TelnetServer shut down");
	return 0;
}
void TelnetServer::conn_readcb(struct bufferevent *bev, void *user_data)
{
	char cmd[512];
	size_t len = bufferevent_read(bev, cmd, sizeof(cmd)-1 );
	cmd[len] = '\0';
	std::vector<string> params;
	split(cmd,params," ");
	if (params.size() <= 0)
		return;
	if (params[0] == "ls")
	{
		char msg[] = "ls ok\n";
		bufferevent_write(bev, msg, strlen(msg));
	}
	else if (params[0] == "send")
	{
		char msg[] = "send ok\n";
		bufferevent_write(bev, msg, strlen(msg));
	}
	else{
		char msg[] = "not support command\n";
		bufferevent_write(bev, msg, strlen(msg));		
		bufferevent_write(bev, g_cmd, strlen(g_cmd));
		//bufferevent_enable(bev, EV_WRITE);
	}
}
void TelnetServer::conn_writecb(struct bufferevent *bev, void *user_data)
{
	//
}

void TelnetServer::conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
    if (events & BEV_EVENT_EOF)  {
        YQLogInfo("Connection closed.");
    } 
    else if (events & BEV_EVENT_ERROR) {
        YQLogMin("Got an error on the connection");/*XXX win32*/
    }
}
//信号处理
void TelnetServer::signal_cb(evutil_socket_t sig, short events, void *user_data)
{
    struct event_base *base = (struct event_base *)user_data;
    struct timeval delay = { 2, 0 };

    YQLogInfo("TelnetServer Caught an interrupt signal; exiting cleanly in two seconds");

    event_base_loopexit(base, &delay);
}
//新客户端连接
void TelnetServer::listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
	event_base *base = (struct event_base *)user_data;
	struct bufferevent *bev;
    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev)  {
		YQLogInfo("TelnetServer Caught an interrupt signal; exiting cleanly in two seconds");
		return;
	}

    bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, NULL);
	//注册写事件
    bufferevent_enable(bev, EV_WRITE);
	
	bufferevent_write(bev, g_cmd, strlen(g_cmd));
	//注册读事件
    bufferevent_enable(bev, EV_READ);
	bufferevent_enable(bev, EV_PERSIST);
}
void TelnetThread::Run()
{
	svr.Run();
}