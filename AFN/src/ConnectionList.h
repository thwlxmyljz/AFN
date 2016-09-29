#pragma once

#include "EventConnection.h"
#include "ConnectionPower.h"
#include "ConnectionWater.h"
#include "Jzq.h"
#include "Water.h"
#include "Lock.h"
#include "Mutex.h"
#include <list>

using namespace std;

class AFNPackage;
//------------------------------------------------------------------------------------
/*
集中器管理列表,tcp连接链表+集中器链表
*/
class JzqList : public list<EventConnection*>
{
	friend class TcpServer;
private:
	//电力集中器列表
	list<Jzq*> m_jzqList;
	//水利集中器列表
	list<Water*> m_waterList;
public:	
	typedef list<EventConnection*>::iterator conIter;
	typedef list<Jzq*>::iterator jzqIter;
	typedef list<Water*>::iterator waterIter;
public:
	JzqList();
	virtual ~JzqList();

	void Load();
	std::string Print();

	//获取连接对象,非libevent callback线程需要调用AUTO_LOCK()加锁
	EventConnection* getConnection(struct bufferevent *bev);
	//删除集中器连接
	void delConnection(struct bufferevent *bev);
	//检测心跳
	void CheckConnection();

	/*libevent事件处理*/
	static void conn_writecb(struct bufferevent *bev, void *user_data);
	static void conn_readcb(struct bufferevent *bev, void *user_data);
	static void conn_eventcb(struct bufferevent *bev, short events, void *user_data);

public:
	//电力集中器相关
	void LoadJzq();
	Jzq* getJzq(WORD _areacode,WORD _number);
	Jzq* getJzq(const std::string& _name);

	//集中器登录
	void ReportLoginState(WORD _areacode,WORD _number,WORD _Fn,BYTE _pseq,BOOL _add=TRUE);
	//获取集中器的响应RSEQ
	BYTE GetRSEQ(WORD _areacode,WORD _number,BOOL _increase=TRUE);
	//获取集中器的请求PSEQ
	BYTE GetPSEQ(WORD _areacode,WORD _number,BOOL _increase=TRUE);
	//获取电表ID
	int GetEleID(WORD areacode,WORD addr, WORD pn);

	//新的集中器连接
	int newJzqConnection(struct event_base *base,evutil_socket_t fd, struct sockaddr *sa);
	ConnectionPower* getJzqConnection(WORD _areacode,WORD _number);
	ConnectionPower* getJzqConnection(const std::string& _name);

	//定时执行所有在线采集器的电力采集
	void AutoGetAllKwh();

public:
	//水利相关
	void LoadWater();
	Water* getWater(const std::string& _name);
	//新的集中器连接
	int newWaterConnection(struct event_base *base,evutil_socket_t fd, struct sockaddr *sa);
	ConnectionWater* getWaterConnection(const std::string& _name);
	//定时执行所有在线采集器的水利采集
	void AutoGetAllWater();
};

extern Mutex g_JzqConList_Mutex;
extern JzqList* g_JzqConList;
#define AUTO_LOCK() Lock lock(g_JzqConList_Mutex);
