#pragma once

#include "AFNData.h"
#include "AFNPackage.h"
#include <list>
class AFN02Ack : public Pkg_Afn_Data{
public:
	AFN02Ack(Pkg_Afn_DataTag _what);
	virtual ~AFN02Ack();
public:
	BYTE WhatAckAFN;//1�ֽ�
	Pkg_Afn_DataTag WhatAckDataTag;//4�ֽ�
	BYTE Err;//1�ֽ�
};
class AFN02
{
public:
	AFN02(void);
	~AFN02(void);
public:
	static int HandleRequest(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);
};

