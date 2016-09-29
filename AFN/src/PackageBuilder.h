#pragma once

#include "IPackage.h"
#include <map>
#include <list>

#ifndef _WIN32
#include <pthread.h>
#include <errno.h>
#else
#include <windows.h>
#endif

typedef int (*pfnDoHandleRequest)(std::list<IPackage*>& reqLst,std::list<IPackage*>& ackLst);
typedef void* (*pfnDoHandleAck)(IPackage* ackPkg);

struct PkgHandler{
	pfnDoHandleRequest reqHander;
	pfnDoHandleAck ackHandler;
};
class PackageBuilder
{
public:
	PackageBuilder(void);
	virtual ~PackageBuilder(void);
	//注册处理函数
	void Register(int code,pfnDoHandleRequest reqHandler,pfnDoHandleAck ackHandler);
	
	//reqLst:多帧请求，ackLst:返回响应帧（可能0～多帧）,如果返回了响应帧，则需要发送到终端
	int HandlePkg(IPackage* reqPkg,std::list<IPackage*>& ackLst);
	int HandlePkg(std::list<IPackage*>& reqLst,std::list<IPackage*>& ackLst);

protected:
	//请求帧处理
	int DoHandleRequest(std::list<IPackage*>& reqLst,std::list<IPackage*>& ackLst);
	//响应帧处理
	int DoHandleAck(std::list<IPackage*>& ackLst);
	//构造回复包
	virtual IPackage* CreateAck(IPackage* reqPkg, void* data=NULL) = 0;

#ifdef _WIN32
	CRITICAL_SECTION CritSection;
	CONDITION_VARIABLE ConditionVar;
#else
	pthread_mutex_t CritSection;  
	pthread_cond_t ConditionVar; 
#endif
	//帧处理MAP
	std::map< int,PkgHandler> handlerMap;
	typedef std::map< int,PkgHandler>::iterator mapIter;
};

