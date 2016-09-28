#pragma once

#include "AFNPackage.h"
#include <map>
#include "PackageBuilder.h"
class Jzq;

#ifndef _WIN32
#include <pthread.h>
#include <errno.h>
#else
#include <windows.h>
#endif

class WaterPackageBuilder : public PackageBuilder
{
private:
	WaterPackageBuilder(void);
	~WaterPackageBuilder(void);
	static WaterPackageBuilder* single;
	//������
	int DoHandleAck(std::list<IPackage*>& ackLst);
public:		
	/*
	Ϊ�����������reqPkg����һ���ظ���
	*/
	IPackage* CreateAck(IPackage* reqPkg, void* data = NULL);
	static WaterPackageBuilder& Instance(){
		if (single == NULL){
			single = new WaterPackageBuilder;
#ifdef _WIN32
			InitializeConditionVariable(&single->ConditionVar);
			InitializeCriticalSection(&single->CritSection);
#else
			pthread_mutex_init(&single->CritSection,NULL);  
			pthread_cond_init(&single->ConditionVar,NULL);
#endif
		}
		return (*single);
	}
};

