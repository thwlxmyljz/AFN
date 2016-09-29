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
	//ע�ᴦ����
	void Register(int code,pfnDoHandleRequest reqHandler,pfnDoHandleAck ackHandler);
	
	//reqLst:��֡����ackLst:������Ӧ֡������0����֡��,�����������Ӧ֡������Ҫ���͵��ն�
	int HandlePkg(IPackage* reqPkg,std::list<IPackage*>& ackLst);
	int HandlePkg(std::list<IPackage*>& reqLst,std::list<IPackage*>& ackLst);

protected:
	//����֡����
	int DoHandleRequest(std::list<IPackage*>& reqLst,std::list<IPackage*>& ackLst);
	//��Ӧ֡����
	int DoHandleAck(std::list<IPackage*>& ackLst);
	//����ظ���
	virtual IPackage* CreateAck(IPackage* reqPkg, void* data=NULL) = 0;

#ifdef _WIN32
	CRITICAL_SECTION CritSection;
	CONDITION_VARIABLE ConditionVar;
#else
	pthread_mutex_t CritSection;  
	pthread_cond_t ConditionVar; 
#endif
	//֡����MAP
	std::map< int,PkgHandler> handlerMap;
	typedef std::map< int,PkgHandler>::iterator mapIter;
};

