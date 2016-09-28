//�ֽ���
#ifndef __PROTO_WATER_H__
#define __PROTO_WATER_H__

#include "YQDefs.h"
#include <string.h>
#include <string>

#pragma pack(1) //1�ֽڶ���
/*
������
*/
class WaterData
{
public:
	BYTE* m_data;
	int m_len;
	WaterData():m_data(NULL),m_len(0)
	{
	}
	WaterData(BYTE* _data,int _len)
	{
		m_data = new BYTE[_len];
		m_len = _len;
		memcpy(m_data,_data,_len);
	}
	~WaterData()
	{
		delete[] m_data;
		m_data = NULL;
		m_len = 0;
	}
};
class PkgWater {
public:
	/*��ͷ*/
	BYTE pkgHeader;
	/*���Ҵ���*/
	BYTE factoryCode[2];
	/*����*/
	BYTE length[2];
	/*������*/
	BYTE code[2];
	/*CS*/
	BYTE cs;
	/*��β*/
	BYTE pkgTail;
	/*����*/
	WaterData water;
};
#endif