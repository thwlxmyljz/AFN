#pragma once

#include "AFNData.h"
#include "proto.h"
#include <list>
class AFNPackage;
/*
公共回复所有不认识的数据库包
*/

class AFNAck_Data : public Pkg_Afn_Data{
public:
	AFNAck_Data();
	virtual ~AFNAck_Data();
public:
	BYTE WhatAckAFN;//1字节
	Pkg_Afn_DataTag WhatAckDataTag;//4字节
	BYTE Err;//1字节
};

