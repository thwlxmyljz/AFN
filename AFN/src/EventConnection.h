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

#include "YQDefs.h"
#include "IPackage.h"
#include "PackageBuilder.h"
#include <list>
using namespace std;

class PackageBuilder;
//------------------------------------------------------------------------------------
class EventConnection
{ 
public:
	EventConnection(struct event_base *base,struct bufferevent *_bev,evutil_socket_t _fd,struct sockaddr *sa,PackageBuilder& _builder);
	virtual ~EventConnection(void);
	//��ȡ֡
	int RecBuf();
	//����֡
	int SendBuf(const void* cmd,unsigned int cmdlen);
	//���Ͱ�
	int SendPkg(const IPackage* pkg);
	int SendPkg(std::list<IPackage*>& pkgLst);
	//�Ƚ�����
	BOOL Compare(struct bufferevent *_bev);
	//��ս��ջ����
	void ClearRecPkgList();

protected:
	//�����
	virtual IPackage* createPackage() = 0;
	//�����
	virtual int handlePackage(IPackage* ipkg) = 0;

protected:
	//�յ���δ���������ݻ���
	BYTE* m_buffer;
	int m_buffer_size;
	void appendData2Buffer(BYTE* msg, DWORD msgLen);
	void ClearPkgList(std::list<IPackage*>& lst);

	//libevent����ͨ��
	struct bufferevent *bev;	
	//fd
	evutil_socket_t fd;
	//�Զ˵�ַ
	sockaddr_in m_remoteAddr;
	//���յ���֡�����б��棬��֡�������ֱ�Ӷ���
	std::list<IPackage*> m_pkgList;
	typedef std::list<IPackage*>::iterator Iter;

	//��������
	PackageBuilder& builder;
};
