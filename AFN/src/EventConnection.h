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
	//读取帧
	int RecBuf();
	//发送帧
	int SendBuf(const void* cmd,unsigned int cmdlen);
	//发送包
	int SendPkg(const IPackage* pkg);
	int SendPkg(std::list<IPackage*>& pkgLst);
	//比较连接
	BOOL Compare(struct bufferevent *_bev);
	//清空接收缓冲包
	void ClearRecPkgList();

protected:
	//构造包
	virtual IPackage* createPackage() = 0;
	//处理包
	virtual int handlePackage(IPackage* ipkg) = 0;

protected:
	//收到的未解析的数据缓存
	BYTE* m_buffer;
	int m_buffer_size;
	void appendData2Buffer(BYTE* msg, DWORD msgLen);
	void ClearPkgList(std::list<IPackage*>& lst);

	//libevent数据通道
	struct bufferevent *bev;	
	//fd
	evutil_socket_t fd;
	//对端地址
	sockaddr_in m_remoteAddr;
	//接收到多帧报文列表缓存，单帧处理完后直接丢弃
	std::list<IPackage*> m_pkgList;
	typedef std::list<IPackage*>::iterator Iter;

	//包管理器
	PackageBuilder& builder;
};
