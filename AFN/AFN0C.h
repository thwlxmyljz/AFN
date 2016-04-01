#pragma once

#include "proto.h"
#include <list>

class AFNPackage;

class AFN0CAck_Data : public Pkg_Afn_Data
{
public:
	AFN0CAck_Data(void);
	~AFN0CAck_Data(void);
};

class AFN0C : public Pkg_Afn {
public:
	AFN0C(void);
	~AFN0C(void);

	static int HandleRequest(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);

public:
	int Create(WORD pn,WORD Fn);
	int CreateClock();
};