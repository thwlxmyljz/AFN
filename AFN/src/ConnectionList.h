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
�����������б�,tcp��������+����������
*/
class JzqList : public list<EventConnection*>
{
	friend class TcpServer;
private:
	//�����������б�
	list<Jzq*> m_jzqList;
	//ˮ���������б�
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

	//��ȡ���Ӷ���,��libevent callback�߳���Ҫ����AUTO_LOCK()����
	EventConnection* getConnection(struct bufferevent *bev);
	//ɾ������������
	void delConnection(struct bufferevent *bev);
	//�������
	void CheckConnection();

	/*libevent�¼�����*/
	static void conn_writecb(struct bufferevent *bev, void *user_data);
	static void conn_readcb(struct bufferevent *bev, void *user_data);
	static void conn_eventcb(struct bufferevent *bev, short events, void *user_data);

public:
	//�������������
	void LoadJzq();
	Jzq* getJzq(WORD _areacode,WORD _number);
	Jzq* getJzq(const std::string& _name);

	//��������¼
	void ReportLoginState(WORD _areacode,WORD _number,WORD _Fn,BYTE _pseq,BOOL _add=TRUE);
	//��ȡ����������ӦRSEQ
	BYTE GetRSEQ(WORD _areacode,WORD _number,BOOL _increase=TRUE);
	//��ȡ������������PSEQ
	BYTE GetPSEQ(WORD _areacode,WORD _number,BOOL _increase=TRUE);
	//��ȡ���ID
	int GetEleID(WORD areacode,WORD addr, WORD pn);

	//�µļ���������
	int newJzqConnection(struct event_base *base,evutil_socket_t fd, struct sockaddr *sa);
	ConnectionPower* getJzqConnection(WORD _areacode,WORD _number);
	ConnectionPower* getJzqConnection(const std::string& _name);

	//��ʱִ���������߲ɼ����ĵ����ɼ�
	void AutoGetAllKwh();

public:
	//ˮ�����
	void LoadWater();
	Water* getWater(const std::string& _name);
	//�µļ���������
	int newWaterConnection(struct event_base *base,evutil_socket_t fd, struct sockaddr *sa);
	ConnectionWater* getWaterConnection(const std::string& _name);
	//��ʱִ���������߲ɼ�����ˮ���ɼ�
	void AutoGetAllWater();
};

extern Mutex g_JzqConList_Mutex;
extern JzqList* g_JzqConList;
#define AUTO_LOCK() Lock lock(g_JzqConList_Mutex);
