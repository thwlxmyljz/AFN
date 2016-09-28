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

struct AppCall{
	//��������
	WORD m_areacode;
	//��ַ����
	WORD m_number;
	//AFN����
	BYTE AFN;
	//FN
	WORD m_fn;
	//PN
	WORD m_pn;	

	bool operator < (const AppCall& o) const {
		if (m_areacode < o.m_areacode)
			return true;
		else if (m_areacode > o.m_areacode)
			return false;
		if (m_number < o.m_number)
			return true;
		else if (m_number > o.m_number)
			return false;
		if (AFN < o.AFN)
			return true;
		else if (AFN > o.AFN)
			return false;
		if (m_fn < o.m_fn)
			return true;
		else if (m_fn > o.m_fn)
			return false;
		if (m_pn < o.m_pn)
			return true;
		else if (m_pn > o.m_pn)
			return false;
		return false;
	}
	bool operator == (const AppCall& o) const {
		return m_areacode==o.m_areacode && m_number==o.m_number && AFN==o.AFN && m_fn==o.m_fn&& m_pn==o.m_pn;
	}
};
class AFNPackageBuilder : public PackageBuilder
{
private:
	AFNPackageBuilder(void);
	~AFNPackageBuilder(void);

	static AFNPackageBuilder* single;

	//app call request list
	std::map<AppCall,Pkg_Afn_Data*> cmdMap;
	typedef std::map<AppCall,Pkg_Afn_Data*>::iterator CmdMapIter;

	//�Զ���������ݿ�
	void saveResultToDB(const AppCall& call,Pkg_Afn_Data* data);
	//�յ��������Ľ������֪ͨ
	int notify(const AppCall& call,Pkg_Afn_Data* data);

	int DoHandleAck(std::list<IPackage*>& ackLst);
public:		
	/*
	Ϊ�����������reqPkg����һ���ظ�����������data=NULLʱ��F1ȫ��ȷ�Ͻ��лظ�
	*/
	IPackage* CreateAck(IPackage* reqPkg, void* data = NULL);
	/*--------------------------------------------------------
	telnet thread's user call these function, user sync wait for result
	*/
	Pkg_Afn_Data* Wait(const AppCall& call);
	//�ٲ������,pn�������,0���в�����
	int setpointparams(Pkg_Afn_Data** val,std::string name,WORD pn);
	int setpointstatus(Pkg_Afn_Data** val,std::string name);
	//��ȡ������ʱ��
	int getclock(Pkg_Afn_Data** val,std::string name);
	//�ն˼��г���״̬��Ϣ
	int getstatus(Pkg_Afn_Data** val,std::string name);
	//������Ϣ
	int getallkwh(Pkg_Afn_Data** val,std::string name,WORD pn);

	/*
	telnet thread auto call these function, async call
	*/
	//�ɼ��˼�������Ͻ���е������
	int getallkwh_async(Jzq* pJzq);
	//�ɼ��˼������������(pn)����
	int getallkwh_async(std::string name,WORD pn); 

public:
	static AFNPackageBuilder& Instance(){
		if (single == NULL){
			single = new AFNPackageBuilder;
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

