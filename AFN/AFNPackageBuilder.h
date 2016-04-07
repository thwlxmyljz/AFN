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
	//行政区号
	WORD m_areacode;
	//地址编码
	WORD m_number;
	//AFN命令
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

	//Builder单一对象
	static AFNPackageBuilder* single;
	//报文处理MAP
	std::map< Pkg_Afn_Header::AFN_CODE,PkgHandler> handlerMap;
	typedef std::map< Pkg_Afn_Header::AFN_CODE,PkgHandler>::iterator mapIter;
	//app call request list
	std::map<AppCall,Pkg_Afn_Data*> cmdMap;
	typedef std::map<AppCall,Pkg_Afn_Data*>::iterator CmdMapIter;
	//
	CRITICAL_SECTION CritSection;
	CONDITION_VARIABLE ConditionVar;
public:		
	//注册处理函数
	void Register(Pkg_Afn_Header::AFN_CODE code,pfnDoHandleRequest reqHandler,pfnDoHandleAck ackHandler);

	/*
	接受帧处理,tcpserver main thread call
	*/
	//reqLst:多帧请求，ackLst:返回响应帧（可能0～多帧）,如果返回了响应帧，则需要发送到终端
	int HandlePkg(AFNPackage* reqPkg,std::list<AFNPackage*>& ackLst);
	int HandlePkg(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);
	int Notify(const AppCall& call,Pkg_Afn_Data* data);

	/*
	构造请求帧,app thread call
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
	//请求帧处理
	int DoHandleRequest(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);
	//响应帧处理
	int DoHandleAck(std::list<AFNPackage*>& reqLst);
};

