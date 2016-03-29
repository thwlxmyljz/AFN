#pragma once

#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include "event2/util.h"
#include "event2/event.h"
#include "thirdparty/Ipv4Address.h"

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
	void ClearRecPkgList();
private:
	//libevent数据通道
	struct bufferevent *bev;
	//集中器
	Jzq* m_jzq;
	//fd
	evutil_socket_t fd;
	//对端地址
	Ipv4Address m_remoteAddr;
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
	//网络连接
	Connection* m_conn;
	//最近的心跳时间
	DWORD m_heart;
	//主站MSA值
	static BYTE s_MSA;
public:
	Jzq():m_name(""),m_areacode(0),m_number(0),m_tag(0),m_conn(NULL),m_heart(0)
	{
		m_RSEQ = 0x0;
		m_PSEQ = 0x0;
	}
	Jzq(string _name,WORD _areaCode,WORD _number,BYTE _tag)
		:m_name(_name),m_areacode(_areaCode),m_number(_number),m_tag(_tag),m_conn(NULL),m_heart(0)
	{
		m_RSEQ = 0x0;
		m_PSEQ = 0x0;
	}
	BOOL operator==(const Jzq& o)
	{
		return (o.m_areacode == m_areacode && o.m_number == m_number);
	}
};
//------------------------------------------------------------------------------------
/*
集中器管理列表
*/
class JzqList : public list<Connection*>
{
public:
	virtual ~JzqList();
	typedef list<Connection*>::iterator conIter;
	typedef list<Jzq*>::iterator jzqIter;
public:
	//从数据库加载集中器
	void LoadJzq();
	//新的集中器连接
	int newConnection(struct event_base *base,evutil_socket_t fd, struct sockaddr *sa);
	//删除集中器连接
	void delConnection(struct bufferevent *bev);
	//获取连接对象
	Connection* getConnection(struct bufferevent *bev);
	//登录相关
	void ReportLoginState(WORD _areacode,WORD _number,BYTE _Fn,BYTE _pseq);
	//获取集中器的RSEQ
	BYTE GetRSEQ(WORD _areacode,WORD _number);
public:
	/*libevent事件处理*/
	static void conn_writecb(struct bufferevent *bev, void *user_data);
	static void conn_readcb(struct bufferevent *bev, void *user_data);
	static void conn_eventcb(struct bufferevent *bev, short events, void *user_data);
private:
	list<Jzq*> m_jzq;
};
extern JzqList* g_JzqConList;
