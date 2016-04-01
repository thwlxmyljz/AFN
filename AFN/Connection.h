#pragma once

#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include "event2/util.h"
#include "event2/event.h"
#include "thirdparty/Ipv4Address.h"
#include "proto.h"

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
	//������
	Jzq* m_jzq;

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
	Jzq();
	Jzq(string _name,WORD _areaCode,WORD _number,BYTE _tag);
	~Jzq();
	BOOL operator==(const Jzq& o);
};
//------------------------------------------------------------------------------------
/*
�����������б�,��������+����������
*/
class JzqList : public list<Connection*>
{
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
	//�µļ���������
	int newConnection(struct event_base *base,evutil_socket_t fd, struct sockaddr *sa);
	//ɾ������������
	void delConnection(struct bufferevent *bev);
	//��ȡ���Ӷ���
	Connection* getConnection(struct bufferevent *bev);
	//��ȡ����������
	Jzq* getJzq(WORD _areacode,WORD _number);
	Jzq* getJzq(std::string _name);
	//��¼���
	void ReportLoginState(WORD _areacode,WORD _number,WORD _Fn,BYTE _pseq);
	//��ȡ��������RSEQ
	BYTE GetRSEQ(WORD _areacode,WORD _number);
	//�������б��ӡ
	std::string printJzq();
public:
	//�´����������
	int SendCmd(Pkg_Afn_Header::AFN_CODE _code,WORD pn,WORD Fn){}
	//�ٲ������,pn�������,0���в�����
	int ShowPoint(std::string name,WORD pn);
public:
	/*libevent�¼�����*/
	static void conn_writecb(struct bufferevent *bev, void *user_data);
	static void conn_readcb(struct bufferevent *bev, void *user_data);
	static void conn_eventcb(struct bufferevent *bev, short events, void *user_data);
};
extern JzqList* g_JzqConList;
