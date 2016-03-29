#pragma once
#include "proto.h"
class AFN01Req_Data : public Pkg_Afn_Data
{
public:
	AFN01Req_Data(void);
	~AFN01Req_Data(void);
};
class AFN01 : public Pkg_Afn{
public:
	AFN01(int Fn);
};

