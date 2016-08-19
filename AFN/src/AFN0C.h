#pragma once

#include "proto.h"
#include <list>
#include <string>
#include "AFNData.h"

class AFNPackage;
//--------------------------------------------------------------------------
class AFN0CAck_Data_GetClock : public Pkg_Afn_Data
{
public:
	AFN0CAck_Data_GetClock(Pkg_Afn_Data* _origin);
	AFN0CAck_Data_GetClock();
	~AFN0CAck_Data_GetClock(void);

	virtual AFN0CAck_Data_GetClock* New();
	virtual int HandleData();
	std::string toString();
public:
	std::string dt;
};
//--------------------------------------------------------------------------
class AFN0CAck_Data_GetRunStatus : public Pkg_Afn_Data
{
public:
	AFN0CAck_Data_GetRunStatus(Pkg_Afn_Data* _origin);
	AFN0CAck_Data_GetRunStatus();
	~AFN0CAck_Data_GetRunStatus(void);

	virtual AFN0CAck_Data_GetRunStatus* New();
	virtual int HandleData();
	std::string toString();
public:
	/*
	�������ݿ����n	BIN	1

	�ն�ͨ�Ŷ˿ں�	BIN	1
	Ҫ���������	BIN	2
	��ǰ������״̬��־	BS8	1
	����ɹ�����	BIN	2
	���ص��ɹ�����	BIN	1
	����ʼʱ��	����¼A.1	6
	�������ʱ��	����¼A.1	6
	*/
	int n;
	class Block{
	public:
		BYTE port;
		short num;
		BYTE status;
		short success;
		BYTE success_import;
		std::string sdt;
		std::string edt;

		int parse(const BYTE*p, int len);
		std::string toString();
	};
	Block* pBlock;
};
//--------------------------------------------------------------------------
class AFN0CAck_Data_AllKwh : public Pkg_Afn_Data
{
public:
	AFN0CAck_Data_AllKwh(Pkg_Afn_Data* _origin);
	AFN0CAck_Data_AllKwh();
	~AFN0CAck_Data_AllKwh(void);
public:
	virtual AFN0CAck_Data_AllKwh* New();
	virtual int HandleData();
	virtual std::string toString();
	virtual int toDB(WORD A1,WORD A2,WORD Fn, WORD pn);
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
//--------------------------------------------------------------------------
class AFN0C : public Pkg_Afn {
public:
	AFN0C(void);
	~AFN0C(void);

	static Pkg_Afn_Data* HandleAck(AFNPackage* ackPkg);

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