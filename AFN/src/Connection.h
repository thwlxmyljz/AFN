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

using namespace std;

class AFNPackage;

class Jzq{
public:
	class JzqA1A2{	
	public:
		//行政区号,0无效
		WORD m_areacode;
		//地址编码,0无效
		WORD m_number;
		void Invalid(){
			m_areacode = 0;
			m_number = 0;
		}
		BOOL isOK(){
			return m_areacode != 0 && m_number != 0;
		}
		bool operator==(const JzqA1A2& o){
			return m_areacode == o.m_areacode && m_number == o.m_number;
		}
	} m_a1a2;

	//名称
	string m_name;
	//按位表示属性
	/*
	bit0:1已存在数据库配置,0未配置
	bit1:1已登录，0未登录
	*/
	BYTE m_tag;	
	//响应帧序号
	BYTE m_RSEQ;
	//请求帧序号
	BYTE m_PSEQ;
	//启动帧帧序号计数器PFC
	BYTE m_PFC;
	//最近的心跳时间
	DWORD m_heart;
	//主站MSA值
	static BYTE s_MSA;
public:
	Jzq();
	Jzq(string _name,WORD _areaCode,WORD _number,BYTE _tag);
	~Jzq();
	BOOL operator==(const Jzq& o);
	std::string printInfo();
	/*
	登录函数
	_Fn:1(login in),2(login out),3(heart beart)
	_pseq:login in时记录帧序号
	_log:日志记录
	*/
	void LoginState(WORD _Fn,BYTE _pseq,BOOL _log=TRUE);
};
//------------------------------------------------------------------------------------
/*集中器连接*/
class Connection
{
	friend class ZjqList;
public:
	Connection(struct event_base *base,struct bufferevent *_bev,evutil_socket_t _fd,struct sockaddr *sa);
	virtual ~Connection(void);
	//集中器标记
	Jzq::JzqA1A2 m_jzq;
public:
	//读取帧
	int RecBuf();
	//发送帧
	int SendBuf(const void* cmd,unsigned int cmdlen);

	//发送数据包
	int SendPkg(const AFNPackage* pkg);
	int SendPkg(std::list<AFNPackage*>& pkgLst);

	//判断是否此连接
	BOOL Compare(struct bufferevent *_bev);

protected:
	void ClearPkgList(std::list<AFNPackage*>& lst);

private:
	//libevent数据通道
	struct bufferevent *bev;	
	//fd
	evutil_socket_t fd;
	//对端地址
	sockaddr_in m_remoteAddr;
	//接收到多帧报文列表缓存，单帧处理完后直接丢弃
	std::list<AFNPackage*> m_pkgList;
	typedef std::list<AFNPackage*>::iterator Iter;
};

