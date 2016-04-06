#pragma once

#include "AFNPackage.h"
#include <map>

typedef int (*pfnDoHandleRequest)(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);
typedef int (*pfnDoHandleAck)(std::list<AFNPackage*>& ackLst);

struct PkgHandler{
	pfnDoHandleRequest reqHander;
	pfnDoHandleAck ackHandler;
};

class AFNPackageBuilder
{
	AFNPackageBuilder(void);
	~AFNPackageBuilder(void);
public:		
	//注册处理函数
	void Register(Pkg_Afn_Header::AFN_CODE code,pfnDoHandleRequest reqHandler,pfnDoHandleAck ackHandler);

	//接受帧处理,reqLst:多帧请求，ackLst:返回响应帧（可能0～多帧）,如果返回了响应帧，则需要发送到终端
	int HandlePkg(AFNPackage* reqPkg,std::list<AFNPackage*>& ackLst);
	int HandlePkg(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);

	/*
	构造请求帧
	*/
	//召测测量点,pn测量点号,0所有测量点
	int setpointparams(std::string name,WORD pn);
	int setpointstatus(std::string name);

	//获取集中器时钟
	int getclock(std::string name);
	//终端集中抄表状态信息
	int getstatus(std::string name);
	//抄表信息
	int getallkwh(std::string name,WORD pn);
public:
	static AFNPackageBuilder& Instance(){
		if (single == NULL){
			single = new AFNPackageBuilder;
		}
		return (*single);
	}	
protected:
	//请求帧处理
	int DoHandleRequest(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);
	//响应帧处理
	int DoHandleAck(std::list<AFNPackage*>& reqLst);
private:
	//Builder单一对象
	static AFNPackageBuilder* single;
	//报文处理MAP
	std::map< Pkg_Afn_Header::AFN_CODE,PkgHandler> handlerMap;
	typedef std::map< Pkg_Afn_Header::AFN_CODE,PkgHandler>::iterator mapIter;
};

