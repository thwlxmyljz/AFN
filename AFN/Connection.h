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

/*����������*/
class Connection
{
	friend class ZjqList;
public:
	Connection(struct event_base *base,struct bufferevent *_bev,evutil_socket_t _fd,struct sockaddr *sa);
	virtual ~Connection(void);

public:
	//��ȡ�������ݰ�
	int RecBuf();

	//�������ݰ�
	int SendBuf(const void* cmd,unsigned int cmdlen);
	int SendPkg(const AFNPackage* pkg);
	int SendPkg(std::list<AFNPackage*>& pkgLst);	

	//�ж��Ƿ������
	BOOL Compare(struct bufferevent *_bev);
protected:
	void ClearRecPkgList();
private:
	//libevent����ͨ��
	struct bufferevent *bev;
	//������
	Jzq* m_jzq;
	//fd
	evutil_socket_t fd;
	//�Զ˵�ַ
	Ipv4Address m_remoteAddr;
	//���յ������б�(��֡)
	std::list<AFNPackage*> m_pkgList;
	typedef std::list<AFNPackage*>::iterator Iter;
};
//------------------------------------------------------------------------------------
class Jzq{
public:
	//����
	string m_name;
	//��������
	WORD m_areacode;
	//��ַ����
	WORD m_number;
	//��λ��ʾ����
	/*
	bit0:1�Ѵ������ݿ�����,0δ����
	bit1:1�ѵ�¼��0δ��¼
	*/
	BYTE m_tag;	
	//��Ӧ֡���
	BYTE m_RSEQ;
	//����֡���
	BYTE m_PSEQ;
	//��������
	Connection* m_conn;
	//���������ʱ��
	DWORD m_heart;
	//��վMSAֵ
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
�����������б�
*/
class JzqList : public list<Connection*>
{
public:
	virtual ~JzqList();
	typedef list<Connection*>::iterator conIter;
	typedef list<Jzq*>::iterator jzqIter;
public:
	//�����ݿ���ؼ�����
	void LoadJzq();
	//�µļ���������
	int newConnection(struct event_base *base,evutil_socket_t fd, struct sockaddr *sa);
	//ɾ������������
	void delConnection(struct bufferevent *bev);
	//��ȡ���Ӷ���
	Connection* getConnection(struct bufferevent *bev);
	//��¼���
	void ReportLoginState(WORD _areacode,WORD _number,BYTE _Fn,BYTE _pseq);
	//��ȡ��������RSEQ
	BYTE GetRSEQ(WORD _areacode,WORD _number);
public:
	/*libevent�¼�����*/
	static void conn_writecb(struct bufferevent *bev, void *user_data);
	static void conn_readcb(struct bufferevent *bev, void *user_data);
	static void conn_eventcb(struct bufferevent *bev, short events, void *user_data);
private:
	list<Jzq*> m_jzq;
};
extern JzqList* g_JzqConList;
