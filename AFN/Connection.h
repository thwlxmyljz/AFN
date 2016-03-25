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

/*����������*/
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

	//�������ж�
	BOOL Compare(const string& name);
	BOOL Compare(const string& areaCode,const string& number);
	BOOL Compare(struct bufferevent *_bev);
private:
	//libevent����ͨ��
	struct bufferevent *bev;
	//file descript
	evutil_socket_t fd;
	//�Զ˵�ַ
	Ipv4Address m_remoteAddr;
	//����
	string m_name;
	//��������
	string m_areacode;
	//��ַ����
	string m_number;
	//��¼״̬
	STATE m_state;
};

class ZjqList : public list<Connection*>
{
public:
	virtual ~ZjqList();
	typedef list<Connection*>::iterator Iter;
	typedef list<Connection*>::const_iterator cIter;
public:
	//�µļ���������
	int newConnection(struct event_base *base,evutil_socket_t fd, struct sockaddr *sa);
	//ɾ������������
	void delConnection(const string& name);
	void delConnection(const string& areaCode,const string& number);
	void delConnection(struct bufferevent *bev);
public:
	/*libevent�¼�����*/
	static void conn_writecb(struct bufferevent *bev, void *user_data);
	static void conn_readcb(struct bufferevent *bev, void *user_data);
	static void conn_eventcb(struct bufferevent *bev, short events, void *user_data);
};
extern ZjqList* zjqList;
