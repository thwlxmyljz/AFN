#pragma once

#include "AFNPackage.h"
#include <map>

class Jzq;

#ifndef _WIN32
#include <pthread.h>
#include <errno.h>
#else
#include <windows.h>
#endif

typedef int (*pfnDoHandleRequest)(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);
typedef Pkg_Afn_Data* (*pfnDoHandleAck)(AFNPackage* ackPkg);

struct PkgHandler{
	pfnDoHandleRequest reqHander;
	pfnDoHandleAck ackHandler;
};
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
class AFNPackageBuilder
{
private:
	AFNPackageBuilder(void);
	~AFNPackageBuilder(void);

	static AFNPackageBuilder* single;

	//֡����MAP
	std::map< Pkg_Afn_Header::AFN_CODE,PkgHandler> handlerMap;
	typedef std::map< Pkg_Afn_Header::AFN_CODE,PkgHandler>::iterator mapIter;

	//app call request list
	std::map<AppCall,Pkg_Afn_Data*> cmdMap;
	typedef std::map<AppCall,Pkg_Afn_Data*>::iterator CmdMapIter;
	
#ifdef _WIN32
	CRITICAL_SECTION CritSection;
	CONDITION_VARIABLE ConditionVar;
#else
	pthread_mutex_t CritSection;  
	pthread_cond_t ConditionVar; 
#endif

	//�Զ���������ݿ�
	void saveResultToDB(const AppCall& call,Pkg_Afn_Data* data);
	//�յ��������Ľ������֪ͨ
	int notify(const AppCall& call,Pkg_Afn_Data* data);

public:		
	//ע�ᴦ����
	void Register(Pkg_Afn_Header::AFN_CODE code,pfnDoHandleRequest reqHandler,pfnDoHandleAck ackHandler);
	/*
	Ϊ�����������reqPkg����һ���ظ�����������data=NULLʱ��F1ȫ��ȷ�Ͻ��лظ�
	*/
	AFNPackage* CreateAck(AFNPackage* reqPkg, Pkg_Afn_Data* data=NULL);

	/*-------------------------------------------------------
	tcpserver thread call these function when received jzq data
	*/
	//reqLst:��֡����ackLst:������Ӧ֡������0����֡��,�����������Ӧ֡������Ҫ���͵��ն�
	int HandlePkg(AFNPackage* reqPkg,std::list<AFNPackage*>& ackLst);
	int HandlePkg(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);

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
protected:
	//����֡����
	int DoHandleRequest(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);
	//��Ӧ֡����
	int DoHandleAck(std::list<AFNPackage*>& ackLst);
};

