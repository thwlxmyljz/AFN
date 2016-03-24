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

/*集中器连接*/
class Connection
{
	friend class ZjqList;
public:
	Connection(struct event_base *base,struct bufferevent *_bev,evutil_socket_t _fd,struct sockaddr *sa);
	virtual ~Connection(void);	
	typedef enum _STATE{
		EM_ST_NOLOGIN=0,
		EM_ST_LOGINOK
	}STATE;
public:
	int SendBuf(const void* cmd,unsigned int cmdlen);
	int SendPkg(const AFNPackage* pkg);

	STATE GetState(){ return m_state; }

	//集中器判断
	BOOL Compare(const string& name);
	BOOL Compare(const string& areaCode,const string& number);
	BOOL Compare(struct bufferevent *_bev);
private:
	//libevent数据通道
	struct bufferevent *bev;
	//file descript
	evutil_socket_t fd;
	//对端地址
	Ipv4Address m_remoteAddr;
	//名称
	string m_name;
	//行政区号
	string m_areacode;
	//地址编码
	string m_number;
	//登录状态
	STATE m_state;
};

class ZjqList : public list<Connection*>
{
public:
	virtual ~ZjqList();
	typedef list<Connection*>::iterator Iter;
	typedef list<Connection*>::const_iterator cIter;
public:
	//新的集中器连接
	int newConnection(struct event_base *base,evutil_socket_t fd, struct sockaddr *sa);
	//删除集中器连接
	void delConnection(const string& name);
	void delConnection(const string& areaCode,const string& number);
	void delConnection(struct bufferevent *bev);
public:
	/*libevent事件处理*/
	static void conn_writecb(struct bufferevent *bev, void *user_data);
	static void conn_readcb(struct bufferevent *bev, void *user_data);
	static void conn_eventcb(struct bufferevent *bev, short events, void *user_data);
};
extern ZjqList* zjqList;
