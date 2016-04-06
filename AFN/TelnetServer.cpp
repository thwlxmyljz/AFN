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
#include <sstream>

using namespace std;

#define ERR_PRINT() char errmsg[] = "param error\r\n$"; \
			bufferevent_write(bev, errmsg, strlen(errmsg));

#define BUD() AFNPackageBuilder::Instance()

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
	YQLogInfo("TelnetServer start ok");

	//测试
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
	/*
	BYTE data[18] = {0x0B, 0xFF , 0xFF , 0xFF , 0xFF , 0x02 , 0x00 , 0x61 , 0x00 , 0x00 , 0x03 , 0x00 , 0x02 , 0x00 , 0x00 , 0x01 , 0x00 , 0x00};
	BYTE cs = AFNPackage::GetCS(data,18);
	*/
	/*
	BYTE DT1;
	BYTE DT2;
	WORD Fn;
	AFNPackage::GetDT(3,DT1,DT2);
	Fn = AFNPackage::GetFn(DT1,DT2);
	AFNPackage::GetDT(248,DT1,DT2);
	Fn = AFNPackage::GetFn(DT1,DT2);
	AFNPackage::GetDT(8,DT1,DT2);
	Fn = AFNPackage::GetFn(DT1,DT2);
	AFNPackage::GetDT(9,DT1,DT2);
	Fn = AFNPackage::GetFn(DT1,DT2);
	AFNPackage::GetDT(16,DT1,DT2);
	Fn = AFNPackage::GetFn(DT1,DT2);
	*/
	/*
	BYTE DT1;
	BYTE DT2;
	WORD Fn;
	AFNPackage::GetDA(0,DT1,DT2);
	Fn = AFNPackage::Getpn(DT1,DT2);
	AFNPackage::GetDA(2041,DT1,DT2);
	Fn = AFNPackage::Getpn(DT1,DT2);
	AFNPackage::GetDA(3,DT1,DT2);
	Fn = AFNPackage::Getpn(DT1,DT2);
	AFNPackage::GetDA(248,DT1,DT2);
	Fn = AFNPackage::Getpn(DT1,DT2);
	AFNPackage::GetDA(8,DT1,DT2);
	Fn = AFNPackage::Getpn(DT1,DT2);
	AFNPackage::GetDA(9,DT1,DT2);
	Fn = AFNPackage::Getpn(DT1,DT2);
	AFNPackage::GetDA(16,DT1,DT2);
	Fn = AFNPackage::Getpn(DT1,DT2);
	AFNPackage::GetDA(2040,DT1,DT2);
	Fn = AFNPackage::Getpn(DT1,DT2);
	Fn = AFNPackage::Getpn(0xff,0x0);
	*/
    event_base_dispatch(base);

	evconnlistener_free(listener);
    event_base_free(base);

    YQLogInfo("TelnetServer shut down");
	return 0;
}
//命令说明
char helpstr[] = "cmd:\r\n+++++++++++++++++++++++\r\n"
				"ls,setpointparams,setpointstatus,getclock,getstatus\r\n+++++++++++++++++++++++\r\n"
				"1,ls:list all zjq\r\n-----------------\r\n"
				"2,setpointparams [name] [pn]:\r\n-----------------\r\n"				
				"3,setpointstatus [name]:\r\n-----------------\r\n"
				"4,getclock [name]:\r\n-----------------\r\n"
				"5,getstatus [name]:\r\n-----------------\r\n"
				"6,getallkwh [name] [pn]:\r\n-----------------\r\n"
				"\r\n$";
void TelnetServer::conn_readcb(struct bufferevent *bev, void *user_data)
{
	char cmd[512];
	size_t len = bufferevent_read(bev, cmd, sizeof(cmd)-1 );
	cmd[len] = '\0';
	if (cmd[len-1] == '\b'){
		//backspace
		g_cmd = g_cmd.substr(0,g_cmd.length()-1);
		char msg[512];
		sprintf_s(msg,"\r\n$%s",g_cmd.c_str());
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
		else if (params[0] == "setpointparams"){
			//召测测量点
			if (params.size() < 3){
				ERR_PRINT()
			}
			else{
				int ret = BUD().setpointparams(params[1],atoi(params[2].c_str()));
				ostringstream os;
				os << "setpointparams return " << ret <<"\r\n$";	
				bufferevent_write(bev, os.str().c_str(), os.str().length());
			}
		}
		else if (params[0] == "setpointstatus"){
			if (params.size() < 2){
				ERR_PRINT()
			}
			else{
				int ret = BUD().setpointstatus(params[1]);
				ostringstream os;
				os << "setpointstatus return " << ret <<"\r\n$";	
				bufferevent_write(bev, os.str().c_str(), os.str().length());
			}
		}
		else if (params[0] == "getclock"){
			if (params.size() < 2){
				ERR_PRINT()
			}
			else{
				int ret = BUD().getclock(params[1]);
				ostringstream os;
				os << "getclock return " << ret <<"\r\n$";	
				bufferevent_write(bev, os.str().c_str(), os.str().length());
			}
		}
		else if (params[0] == "getstatus"){
			if (params.size() < 2){
				ERR_PRINT()
			}
			else{
				int ret = BUD().getstatus(params[1]);
				ostringstream os;
				os << "getstatus return " << ret <<"\r\n$";	
				bufferevent_write(bev, os.str().c_str(), os.str().length());
			}
		}
		else if (params[0] == "getallkwh"){
			if (params.size() < 3){
				ERR_PRINT()
			}
			else{
				int ret = BUD().getallkwh(params[1],atoi(params[2].c_str()));
				ostringstream os;
				os << "getallkwh return " << ret <<"\r\n$";	
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