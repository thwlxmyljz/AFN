#pragma once

#include "protoWater.h"
#include <list>
#include "IPackage.h"

class WaterPackage : public PkgWater, public IPackage
{
public:
	WaterPackage(void);
	virtual ~WaterPackage(void);
	//���
	virtual int ParseProto(BYTE* data,DWORD dataLen,DWORD& eatLen);
	//�����л���֡buf��
	virtual int Serialize(BYTE* buf,DWORD bufLen) const;
	//У����Ϸ���
	virtual BOOL valid();
	//��д�����Լ�CS,������һ��
	virtual void okPkg();
	//����CSУ��
	virtual void CreateCS();
	//�����û����ݳ���
	virtual void SetL();
	//�Ƿ�������
	virtual BOOL isRequest();
	//��ȡ������
	virtual int GetCode();
public:
	//����CSֵ
	static BYTE GetCS(BYTE* buf,DWORD len);
};

