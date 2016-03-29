#pragma once

#include "AFNData.h"
#include "AFNPackage.h"

/*
链路接口检测
//上行包
68H
L
L
68H
C
A
AFN=02H
SEQ
数据单元标识（DA=0,DT1=1登录，DT1=2退出登录，DT1=3心跳）
数据单元(无)
CS
16H
*/

class AFN02Data_Ack : public Pkg_Afn_Data{
public:
	AFN02Data_Ack(Pkg_Afn_DataTag _what);
	virtual ~AFN02Data_Ack();
public:
	BYTE WhatAckAFN;//1字节
	Pkg_Afn_DataTag WhatAckDataTag;//4字节
	BYTE Err;//1字节
};
class AFN02 : public Pkg_Afn{
public:
	static int HandleRequest(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);
};

