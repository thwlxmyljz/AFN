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
#include "Element.h"
#include "EventConnection.h"

using namespace std;

class AFNPackage;

class Jzq{
public:
	class JzqA1A2{	
	public:
		//��������,0��Ч
		WORD m_areacode;
		//��ַ����,0��Ч
		WORD m_number;
		void Invalid(){
			m_areacode = 0;
			m_number = 0;
		}
		BOOL isOK(){
			return m_areacode != 0 && m_number != 0;
		}
		bool operator==(const JzqA1A2& o){
			return m_areacode == o.m_areacode && m_number == o.m_number;
		}
	} m_a1a2;

	//����
	string m_name;
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
	//����֡֡��ż�����PFC
	BYTE m_PFC;
	//���������ʱ��
	DWORD m_heartTimer;
	//����Ĳɼ�ʱ��
	DWORD m_kwhTimer;
	//��վMSAֵ
	static BYTE s_MSA;
	//���������ĵ��
	std::list<Element> eleLst;
	typedef std::list<Element>::iterator EleIter;
public:
	Jzq();
	Jzq(string _name,WORD _areaCode,WORD _number,BYTE _tag);
	~Jzq();
	BOOL operator==(const Jzq& o);
	std::string printInfo();
	/*
	��¼����
	_Fn:1(login in),2(login out),3(heart beart)
	_pseq:login inʱ��¼֡���
	_log:��־��¼
	*/
	void LoginState(WORD _Fn,BYTE _pseq,BOOL _log=TRUE);
	/*
	����TRUE:timeout
	*/
	BOOL CheckTimeout();
};
/*����������*/
class Connection : public EventConnection
{
	friend class ZjqList;

public:
	Connection(struct event_base *base,struct bufferevent *_bev,evutil_socket_t _fd,struct sockaddr *sa);
	virtual ~Connection(void);

	//���Ӷ�Ӧ�ļ��������
	Jzq::JzqA1A2 m_jzq;

protected:
	//�����
	virtual IPackage* createPackage();
	//�����
	virtual int handlePackage(IPackage* ipkg);
};

