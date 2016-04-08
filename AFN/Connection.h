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
class Jzq;

/*集中器连接*/
class Connection
{
	friend class ZjqList;
public:
	Connection(struct event_base *base,struct bufferevent *_bev,evutil_socket_t _fd,struct sockaddr *sa);
	virtual ~Connection(void);
	//集中器
	Jzq* m_jzq;

public:
	//读取连接数据包
	int RecBuf();

	//发送数据包
	int SendBuf(const void* cmd,unsigned int cmdlen);
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
	//接收到报文列表(多帧)
	std::list<AFNPackage*> m_pkgList;
	typedef std::list<AFNPackage*>::iterator Iter;
};
//------------------------------------------------------------------------------------
class Jzq{
public:
	//名称
	string m_name;
	//行政区号
	WORD m_areacode;
	//地址编码
	WORD m_number;
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
	//网络连接
	Connection* m_conn;
	//最近的心跳时间
	DWORD m_heart;
	//主站MSA值
	static BYTE s_MSA;
public:
	Jzq();
	Jzq(string _name,WORD _areaCode,WORD _number,BYTE _tag);
	~Jzq();
	BOOL operator==(const Jzq& o);
};
//------------------------------------------------------------------------------------
/*
集中器管理列表,连接链表+集中器链表
*/
class JzqList : public list<Connection*>
{
private:
	//集中器列表
	list<Jzq*> m_jzqList;
public:	
	typedef list<Connection*>::iterator conIter;
	typedef list<Jzq*>::iterator jzqIter;
public:
	virtual ~JzqList();
	//从数据库加载集中器
	void LoadJzq();
	//集中器列表打印
	std::string printJzq();
	//新的集中器连接
	int newConnection(struct event_base *base,evutil_socket_t fd, struct sockaddr *sa);
	//删除集中器连接
	void delConnection(struct bufferevent *bev);
	//获取连接对象
	Connection* getConnection(struct bufferevent *bev);
	//获取集中器对象
	Jzq* getJzq(WORD _areacode,WORD _number);
	Jzq* getJzq(std::string _name);
	//登录相关
	void ReportLoginState(WORD _areacode,WORD _number,WORD _Fn,BYTE _pseq);
	//获取集中器的响应RSEQ
	BYTE GetRSEQ(WORD _areacode,WORD _number,BOOL _increase=TRUE);
	//获取集中器的请求PSEQ
	BYTE GetPSEQ(WORD _areacode,WORD _number,BOOL _increase=TRUE);
public:
	/*libevent事件处理*/
	static void conn_writecb(struct bufferevent *bev, void *user_data);
	static void conn_readcb(struct bufferevent *bev, void *user_data);
	static void conn_eventcb(struct bufferevent *bev, short events, void *user_data);
};
extern Mutex g_JzqConListMutex;
extern JzqList* g_JzqConList;
