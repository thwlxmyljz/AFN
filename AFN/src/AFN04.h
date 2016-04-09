#pragma once

#include "proto.h"
#include <list>

class AFNPackage;

//回复
class AFN04Ack_Data : public Pkg_Afn_Data
{
public:
	AFN04Ack_Data(void);
	~AFN04Ack_Data(void);
};

class AFN04 : public Pkg_Afn {
public:
	AFN04(void);
	~AFN04(void);

	static int HandleRequest(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);

public:
	int Create(WORD pn,WORD Fn);

	//5.5.1.3.24　F25：测量点基本参数
	int CreatePointBaseSetting(WORD pn,BOOL hasPW);
	//5.5.1.3.66　F150：测量点状态
	int CreatePointStatus();
};