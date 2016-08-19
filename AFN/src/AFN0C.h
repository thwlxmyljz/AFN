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
	本项数据块个数n	BIN	1

	终端通信端口号	BIN	1
	要抄电表总数	BIN	2
	当前抄表工作状态标志	BS8	1
	抄表成功块数	BIN	2
	抄重点表成功块数	BIN	1
	抄表开始时间	见附录A.1	6
	抄表结束时间	见附录A.1	6
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
	终端抄表时间	见附录A.15	分时日月年	5
	费率数M（1≤M≤12）	BIN	个	1
	当前正向有功总电能示值	见附录A.14	kWh	5
	当前费率1正向有功总电能示值	见附录A.14	kWh	5
	……	……	……	……
	当前费率M正向有功总电能示值	见附录A.14	kWh	5
	当前正向无功（组合无功1）总电能示值	见附录A.11	kvarh	4
	当前费率1正向无功（组合无功1）总电能示值	见附录A.11	kvarh	4
	……	……	……	……
	当前费率M正向无功（组合无功1）总电能示值	见附录A.11	kvarh	4
	当前一象限无功总电能示值	见附录A.11	kvarh	4
	当前一象限费率1无功电能示值	见附录A.11	kvarh	4
	……	……	……	……
	当前一象限费率M无功电能示值	见附录A.11	kvarh	4
	当前四象限无功总电能示值	见附录A.11	kvarh	4
	当前四象限费率1无功电能示值	见附录A.11	kvarh	4
	……	……	……	……
	当前四象限费率M无功电能示值	见附录A.11	kvarh	4
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

	//5.12.2.4.1　F2：终端日历时钟
	int CreateClock();
	//5.12.2.4.10　F11：终端集中抄表状态信息
	int CreateRunStatus();
	//5.12.2.4.14　F17：当前总加有功功率
	int CreateCurTotalKwh();
	//5.12.2.4.15　F18：当前总加无功功率
	int CreateCurTotalKvarh();
	//5.12.2.4.22　F25：当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率
	int CreateCurABCTotal(WORD pn);
	int CreateCurABCTotal(WORD* pN,int len);
	//5.12.2.4.30　F33：当前正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
	int CreateAllKwh(WORD pn);
	int CreateAllKwh(WORD* pN,int len);
};