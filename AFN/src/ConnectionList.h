#pragma once

#include "Connection.h"
#include "Lock.h"
#include "Mutex.h"
#include <list>

using namespace std;

class AFNPackage;
//------------------------------------------------------------------------------------
/*
�����������б�,tcp��������+����������
*/
class JzqList : public list<Connection*>
{
	friend class TcpServer;
private:
	//�������б�
	list<Jzq*> m_jzqList;
public:	
	typedef list<Connection*>::iterator conIter;
	typedef list<Jzq*>::iterator jzqIter;
public:
	virtual ~JzqList();
	//�����ݿ���ؼ�����
	void LoadJzq();
	//�������б��ӡ
	std::string printJzq();
	//��ȡ������
	Jzq* getJzq(WORD _areacode,WORD _number);
	Jzq* getJzq(const std::string& _name);
	//��������¼���
	void ReportLoginState(WORD _areacode,WORD _number,WORD _Fn,BYTE _pseq,BOOL _add=TRUE);
	//��ȡ����������ӦRSEQ
	BYTE GetRSEQ(WORD _areacode,WORD _number,BOOL _increase=TRUE);
	//��ȡ������������PSEQ
	BYTE GetPSEQ(WORD _areacode,WORD _number,BOOL _increase=TRUE);

protected:
	//�µļ���������
	int newConnection(struct event_base *base,evutil_socket_t fd, struct sockaddr *sa);
	//ɾ������������
	void delConnection(struct bufferevent *bev);

public:
	//��ȡ���Ӷ���,��libevent callback�߳���Ҫ����AUTO_LOCK()����
	Connection* getConnection(struct bufferevent *bev);
	Connection* getConnection(WORD _areacode,WORD _number);
	Connection* getConnection(const std::string& _name);
	
	int GetEleID(WORD areacode,WORD addr, WORD pn);
	//�������
	void CheckConnection();

	//��ʱִ���������߲ɼ����ĵ����ɼ�
	void AutoGetAllKwh();

	/*libevent�¼�����*/
	static void conn_writecb(struct bufferevent *bev, void *user_data);
	static void conn_readcb(struct bufferevent *bev, void *user_data);
	static void conn_eventcb(struct bufferevent *bev, short events, void *user_data);
};

extern Mutex g_JzqConList_Mutex;
extern JzqList* g_JzqConList;
#define AUTO_LOCK() Lock lock(g_JzqConList_Mutex);
