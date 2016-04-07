#pragma once

#include "AFNPackage.h"
#include <map>

typedef int (*pfnDoHandleRequest)(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);
typedef int (*pfnDoHandleAck)(std::list<AFNPackage*>& ackLst);

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
		return false;
	}
};
class AFNPackageBuilder
{
	AFNPackageBuilder(void);
	~AFNPackageBuilder(void);

	//Builder��һ����
	static AFNPackageBuilder* single;
	//���Ĵ���MAP
	std::map< Pkg_Afn_Header::AFN_CODE,PkgHandler> handlerMap;
	typedef std::map< Pkg_Afn_Header::AFN_CODE,PkgHandler>::iterator mapIter;
	//app call request list
	std::map<AppCall,Pkg_Afn_Data*> cmdMap;
	typedef std::map<AppCall,Pkg_Afn_Data*>::iterator CmdMapIter;
	//
	CRITICAL_SECTION CritSection;
	CONDITION_VARIABLE ConditionVar;
public:		
	//ע�ᴦ����
	void Register(Pkg_Afn_Header::AFN_CODE code,pfnDoHandleRequest reqHandler,pfnDoHandleAck ackHandler);

	/*
	����֡����,tcpserver main thread call
	*/
	//reqLst:��֡����ackLst:������Ӧ֡������0����֡��,�����������Ӧ֡������Ҫ���͵��ն�
	int HandlePkg(AFNPackage* reqPkg,std::list<AFNPackage*>& ackLst);
	int HandlePkg(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);
	int Notify(const AppCall& call,Pkg_Afn_Data* data);

	/*
	��������֡,app thread call
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
public:
	static AFNPackageBuilder& Instance(){
		if (single == NULL){
			single = new AFNPackageBuilder;
			InitializeConditionVariable(&single->ConditionVar);
			InitializeCriticalSection(&single->CritSection);
		}
		return (*single);
	}	
protected:
	//����֡����
	int DoHandleRequest(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);
	//��Ӧ֡����
	int DoHandleAck(std::list<AFNPackage*>& reqLst);
};

