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
	//行政区号
	WORD m_areacode;
	//地址编码
	WORD m_number;
	//AFN命令
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

	//帧处理MAP
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

	//自动结果到数据库
	void saveResultToDB(const AppCall& call,Pkg_Afn_Data* data);
	//收到集中器的结果数据通知
	int notify(const AppCall& call,Pkg_Afn_Data* data);

public:		
	//注册处理函数
	void Register(Pkg_Afn_Header::AFN_CODE code,pfnDoHandleRequest reqHandler,pfnDoHandleAck ackHandler);
	/*
	为集中器请求包reqPkg构造一个回复包，当数据data=NULL时按F1全部确认进行回复
	*/
	AFNPackage* CreateAck(AFNPackage* reqPkg, Pkg_Afn_Data* data=NULL);

	/*-------------------------------------------------------
	tcpserver thread call these function when received jzq data
	*/
	//reqLst:多帧请求，ackLst:返回响应帧（可能0～多帧）,如果返回了响应帧，则需要发送到终端
	int HandlePkg(AFNPackage* reqPkg,std::list<AFNPackage*>& ackLst);
	int HandlePkg(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);

	/*--------------------------------------------------------
	telnet thread's user call these function, user sync wait for result
	*/
	Pkg_Afn_Data* Wait(const AppCall& call);
	//召测测量点,pn测量点号,0所有测量点
	int setpointparams(Pkg_Afn_Data** val,std::string name,WORD pn);
	int setpointstatus(Pkg_Afn_Data** val,std::string name);
	//获取集中器时钟
	int getclock(Pkg_Afn_Data** val,std::string name);
	//终端集中抄表状态信息
	int getstatus(Pkg_Afn_Data** val,std::string name);
	//抄表信息
	int getallkwh(Pkg_Afn_Data** val,std::string name,WORD pn);

	/*
	telnet thread auto call these function, async call
	*/
	//采集此集中器管辖所有电表数据
	int getallkwh_async(Jzq* pJzq);
	//采集此集中器单个电表(pn)数据
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
	//请求帧处理
	int DoHandleRequest(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);
	//响应帧处理
	int DoHandleAck(std::list<AFNPackage*>& ackLst);
};

