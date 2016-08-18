#include "event2/thread.h"
#include "TelnetServer.h"
#include "YQErrCode.h"
#include "LogFileu.h"
#include "Connection.h"
#include "ConnectionList.h"
#include "AFNPackage.h"
#include "YQUtils.h"
#include "AFNPackageBuilder.h"
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;

#define ERR_PRINT() char errmsg[] = "param error\r\n$"; \
			bufferevent_write(bev, errmsg, strlen(errmsg));

#define TMOUT_KWH 10 //secs采集电力
struct timeval kwh_lasttime;//上次采集命令发送时间

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

//暂支持单客户登录,保存客户端命令
std::string g_cmd = "";
//
TelnetServer::TelnetServer(unsigned int port)
	:m_svrPort(port),base(NULL),listener(NULL)
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
#ifdef _WIN32    
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);
	evthread_use_windows_threads();//win上设置
#endif
	struct sockaddr_in sin;
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
	YQLogInfo("TelnetServer start ok");

	/* Initalize heartbeat timeout event */
	event_assign(&timeout_event_kwh, base, -1, 0, timeout_cb_kwh, (void*) &timeout_cb_kwh);
	struct timeval tv;
	evutil_timerclear(&tv);
	tv.tv_sec = TMOUT_KWH;
	evutil_gettimeofday(&kwh_lasttime, NULL);
	event_add(&timeout_event_kwh, &tv);	

	/*run loop*/
    event_base_dispatch(base);

	/*exit */
	evconnlistener_free(listener);
    event_base_free(base);

    YQLogInfo("TelnetServer shut down");
	return 0;
}
void TelnetServer::timeout_cb_kwh(evutil_socket_t fd, short event, void *arg)
{
	//对所有在线集中器采集数据
	g_JzqConList->AutoGetAllKwh();

	resetTimer(fd,event,arg);
}
//定时器回调后重设当前定时器
void TelnetServer::resetTimer(evutil_socket_t fd, short event, void *arg)
{
	struct timeval newtime, difference;
	struct event *timeout = (struct event *)arg;
	double elapsed;

	evutil_gettimeofday(&newtime, NULL);
	evutil_timersub(&newtime, &kwh_lasttime, &difference);
	elapsed = difference.tv_sec +
	    (difference.tv_usec / 1.0e6);

	printf("TelnetServer timeout_cb called at %d: %.3f seconds elapsed.\n",
	    (int)newtime.tv_sec, elapsed);
	kwh_lasttime = newtime;

	struct timeval tv;
	evutil_timerclear(&tv);
	tv.tv_sec = TMOUT_KWH;
	event_add(timeout, &tv);
}

//命令说明
char helpstr[] = "\r\n++++++++++++++++++++++++++++++++++++++++++++++\r\n"
				"command: ls,quit,setpointparams,setpointstatus,getclock,getstatus\r\n++++++++++++++++++++++++++++++++++++++++++++++\r\n\r\n"
				"1, ls: list all zjq\r\n---------------------------------------------------\r\n"
				"2, quit: quit\r\n---------------------------------------------------\r\n"
				"3, setpointparams [name] [pn]:not implement\r\n---------------------------------------------------\r\n"				
				"4, setpointstatus [name]:not implement\r\n---------------------------------------------------\r\n"
				"5, getclock [name]:终端日历时钟未显示星期，测试命令(getclock test)\r\n---------------------------------------------------\r\n"
				"6, getstatus [name]:终端集中抄表状态信息，测试命令(getstatus test)\r\n---------------------------------------------------\r\n"
				"7, getallkwh [name] [pn]:读取电表数据,测试命令(getallkwh test 1)\r\n---------------------------------------------------\r\n"
				"\r\n\r\n$";
void TelnetServer::conn_readcb(struct bufferevent *bev, void *user_data)
{
	char cmd[512];
	size_t len = bufferevent_read(bev, cmd, sizeof(cmd)-1 );
	cmd[len] = '\0';
	if (cmd[len-1] == '\b'){
		//backspace
		g_cmd = g_cmd.substr(0,g_cmd.length()-1);
		char msg[512];
#ifdef _WIN32
		sprintf_s(msg,"\r\n$%s",g_cmd.c_str());
#else
		sprintf(msg,"\r\n$%s",g_cmd.c_str());
#endif
		bufferevent_write(bev, msg, strlen(msg));
		return;
	}
	g_cmd += cmd; 
	if (g_cmd.find("\n") != std::string::npos){
		//执行命令
		std::vector<string> params;
		std::size_t pos = 0;
		std::string tmp;
		while (pos < g_cmd.size())
		{
			if (g_cmd[pos] == ' ' || g_cmd[pos] == '\t' || g_cmd[pos] == '\r'|| g_cmd[pos] == '\n'){
				if (tmp != "")
					params.push_back(tmp);
				tmp = "";
			}
			else{
				tmp += g_cmd[pos];
			}
			pos++;
		}
		if (params.size() == 0){
			return;		
		}
		if (params[0] == "ls"){			
			std::string ss = g_JzqConList->printJzq();
			ss += "$";
			bufferevent_write(bev, ss.c_str(), ss.size());
		}
		else if (params[0] == "quit"){			
			bufferevent_free(bev);
		}
		else if (params[0] == "setpointparams"){
			//召测测量点
			if (params.size() < 3){
				ERR_PRINT()
			}
			else{
				Pkg_Afn_Data* p = NULL;
				int ret = AFNPackageBuilder::Instance().setpointparams(&p,params[1],atoi(params[2].c_str()));
				ostringstream os;
				os << "setpointparams return " << ret <<"\r\n";	
				if (p){
					os << p->toString() <<"\r\n$";
					delete p;
				}
				else{
					os << "$";
				}
				bufferevent_write(bev, os.str().c_str(), os.str().length());
			}
		}
		else if (params[0] == "setpointstatus"){
			if (params.size() < 2){
				ERR_PRINT()
			}
			else{
				Pkg_Afn_Data* p = NULL;
				int ret = AFNPackageBuilder::Instance().setpointstatus(&p,params[1]);
				ostringstream os;
				os << "setpointstatus return " << ret <<"\r\n";	
				if (p){
					os << p->toString() <<"\r\n$";
					delete p;
				}
				else{
					os << "$";
				}
				bufferevent_write(bev, os.str().c_str(), os.str().length());
			}
		}
		else if (params[0] == "getclock"){
			if (params.size() < 2){
				ERR_PRINT()
			}
			else{
				Pkg_Afn_Data* p = NULL;
				int ret = AFNPackageBuilder::Instance().getclock(&p,params[1]);
				ostringstream os;
				os << "getclock return " << ret <<"\r\n";
				if (p){
					os << p->toString() <<"\r\n$";
					delete p;
				}
				else{
					os << "$";
				}
				bufferevent_write(bev, os.str().c_str(), os.str().length());
			}
		}
		else if (params[0] == "getstatus"){
			if (params.size() < 2){
				ERR_PRINT()
			}
			else{
				Pkg_Afn_Data* p = NULL;
				int ret = AFNPackageBuilder::Instance().getstatus(&p,params[1]);
				ostringstream os;
				os << "getstatus return " << ret <<"\r\n";	
				if (p){
					os  << p->toString() <<"\r\n$";
					delete p;
				}
				else{
					os << "$";
				}
				bufferevent_write(bev, os.str().c_str(), os.str().length());
			}
		}
		else if (params[0] == "getallkwh"){
			if (params.size() < 3){
				ERR_PRINT()
			}
			else{
				Pkg_Afn_Data* p = NULL;
				int ret = AFNPackageBuilder::Instance().getallkwh(&p,params[1],atoi(params[2].c_str()));
				ostringstream os;
				os << "getallkwh return " << ret <<"\r\n";	
				if (p){
					os << p->toString() <<"\r\n$";					
					delete p;
				}
				else{
					os << "$";
				}
				bufferevent_write(bev, os.str().c_str(), os.str().length());
			}
		}
		else{
			char msg[] = "not support command\r\n";
			bufferevent_write(bev, msg, strlen(msg));		
			bufferevent_write(bev, helpstr, strlen(helpstr));			
		}
		g_cmd = "";
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
    bufferevent_enable(bev, EV_WRITE|EV_READ);
	bufferevent_write(bev, helpstr, strlen(helpstr));
}
void TelnetThread::Run()
{
	svr.Run();
}
