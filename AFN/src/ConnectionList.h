#pragma once

#include "Connection.h"
#include "Lock.h"
#include "Mutex.h"
#include <list>

using namespace std;

class AFNPackage;
//------------------------------------------------------------------------------------
/*
集中器管理列表,tcp连接链表+集中器链表
*/
class JzqList : public list<Connection*>
{
	friend class TcpServer;
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
	//获取集中器
	Jzq* getJzq(WORD _areacode,WORD _number);
	Jzq* getJzq(const std::string& _name);
	//集中器登录相关
	void ReportLoginState(WORD _areacode,WORD _number,WORD _Fn,BYTE _pseq,BOOL _add=TRUE);
	//获取集中器的响应RSEQ
	BYTE GetRSEQ(WORD _areacode,WORD _number,BOOL _increase=TRUE);
	//获取集中器的请求PSEQ
	BYTE GetPSEQ(WORD _areacode,WORD _number,BOOL _increase=TRUE);

protected:
	//新的集中器连接
	int newConnection(struct event_base *base,evutil_socket_t fd, struct sockaddr *sa);
	//删除集中器连接
	void delConnection(struct bufferevent *bev);

public:
	//获取连接对象,非libevent callback线程需要调用AUTO_LOCK()加锁
	Connection* getConnection(struct bufferevent *bev);
	Connection* getConnection(WORD _areacode,WORD _number);
	Connection* getConnection(const std::string& _name);
	
	int GetEleID(WORD areacode,WORD addr, WORD pn);
	//检测心跳
	void CheckConnection();

	//定时执行所有在线采集器的电力采集
	void AutoGetAllKwh();

	/*libevent事件处理*/
	static void conn_writecb(struct bufferevent *bev, void *user_data);
	static void conn_readcb(struct bufferevent *bev, void *user_data);
	static void conn_eventcb(struct bufferevent *bev, short events, void *user_data);
};

extern Mutex g_JzqConList_Mutex;
extern JzqList* g_JzqConList;
#define AUTO_LOCK() Lock lock(g_JzqConList_Mutex);
