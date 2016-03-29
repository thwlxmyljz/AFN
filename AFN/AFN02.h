#pragma once

#include "AFNData.h"
#include "AFNPackage.h"

/*
��·�ӿڼ��
//���а�
68H
L
L
68H
C
A
AFN=02H
SEQ
���ݵ�Ԫ��ʶ��DA=0,DT1=1��¼��DT1=2�˳���¼��DT1=3������
���ݵ�Ԫ(��)
CS
16H
*/

class AFN02Data_Ack : public Pkg_Afn_Data{
public:
	AFN02Data_Ack(Pkg_Afn_DataTag _what);
	virtual ~AFN02Data_Ack();
public:
	BYTE WhatAckAFN;//1�ֽ�
	Pkg_Afn_DataTag WhatAckDataTag;//4�ֽ�
	BYTE Err;//1�ֽ�
};
class AFN02 : public Pkg_Afn{
public:
	static int HandleRequest(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);
};

