#pragma once

#include "proto.h"
#include <list>
#include <string>

class AFNPackage;

class AFN0CAck_Data_AllKwh : public Pkg_Afn_Data
{
public:
	AFN0CAck_Data_AllKwh(const Pkg_Afn_Data* _origin);
	AFN0CAck_Data_AllKwh();
	~AFN0CAck_Data_AllKwh(void);
public:
	virtual AFN0CAck_Data_AllKwh* New();
	virtual int HandleData();
	std::string toString();
	/*
	�ն˳���ʱ��	����¼A.15	��ʱ������	5
	������M��1��M��12��	BIN	��	1
	��ǰ�����й��ܵ���ʾֵ	����¼A.14	kWh	5
	��ǰ����1�����й��ܵ���ʾֵ	����¼A.14	kWh	5
	����	����	����	����
	��ǰ����M�����й��ܵ���ʾֵ	����¼A.14	kWh	5
	��ǰ�����޹�������޹�1���ܵ���ʾֵ	����¼A.11	kvarh	4
	��ǰ����1�����޹�������޹�1���ܵ���ʾֵ	����¼A.11	kvarh	4
	����	����	����	����
	��ǰ����M�����޹�������޹�1���ܵ���ʾֵ	����¼A.11	kvarh	4
	��ǰһ�����޹��ܵ���ʾֵ	����¼A.11	kvarh	4
	��ǰһ���޷���1�޹�����ʾֵ	����¼A.11	kvarh	4
	����	����	����	����
	��ǰһ���޷���M�޹�����ʾֵ	����¼A.11	kvarh	4
	��ǰ�������޹��ܵ���ʾֵ	����¼A.11	kvarh	4
	��ǰ�����޷���1�޹�����ʾֵ	����¼A.11	kvarh	4
	����	����	����	����
	��ǰ�����޷���M�޹�����ʾֵ	����¼A.11	kvarh	4
	*/
	std::string dt;
	int m;
	float userkwh;
	float* userkwh_fee;
	float devkwh;
	float* devkwh_fee;
	float onekwh;
	float* onekwh_fee;
	float fourkwh;
	float* fourkwh_fee;
};

class AFN0C : public Pkg_Afn {
public:
	AFN0C(void);
	~AFN0C(void);

	static int HandleAck(std::list<AFNPackage*>& ackLst);

public:
	int Create(WORD pn,WORD Fn);

	//5.12.2.4.1��F2���ն�����ʱ��
	int CreateClock();
	//5.12.2.4.10��F11���ն˼��г���״̬��Ϣ
	int CreateRunStatus();
	//5.12.2.4.14��F17����ǰ�ܼ��й�����
	int CreateCurTotalKwh();
	//5.12.2.4.15��F18����ǰ�ܼ��޹�����
	int CreateCurTotalKvarh();
	//5.12.2.4.22��F25����ǰ���༰����/�޹����ʡ����������������ѹ��������������������ڹ���
	int CreateCurABCTotal(WORD pn);
	int CreateCurABCTotal(WORD* pN,int len);
	//5.12.2.4.30��F33����ǰ������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
	int CreateAllKwh(WORD pn);
	int CreateAllKwh(WORD* pN,int len);
};