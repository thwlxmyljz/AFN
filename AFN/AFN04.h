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
	//创建召测测量点包
	int CreateF25(WORD pn,BOOL hasPW);
};