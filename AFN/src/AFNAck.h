#pragma once

#include "AFNData.h"
#include "proto.h"
#include <list>
class AFNPackage;
/*
�����ظ����в���ʶ�����ݿ��
*/

class AFNAck_Data : public Pkg_Afn_Data{
public:
	AFNAck_Data();
	virtual ~AFNAck_Data();
public:
	BYTE WhatAckAFN;//1�ֽ�
	Pkg_Afn_DataTag WhatAckDataTag;//4�ֽ�
	BYTE Err;//1�ֽ�
};

