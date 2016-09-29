#pragma once

#include "protoWater.h"
#include <list>
#include "IPackage.h"

class WaterPackage : public PkgWater, public IPackage
{
public:
	WaterPackage(void);
	virtual ~WaterPackage(void);
	//解包
	virtual int ParseProto(BYTE* data,DWORD dataLen,DWORD& eatLen);
	//包序列化到帧buf中
	virtual int Serialize(BYTE* buf,DWORD bufLen) const;
	//校验包合法性
	virtual BOOL valid();
	//填写长度以及CS,封包最后一步
	virtual void okPkg();
	//生产CS校验
	virtual void CreateCS();
	//设置用户数据长度
	virtual void SetL();
	//是否是请求
	virtual BOOL isRequest();
	//获取功能码
	virtual int GetCode();
public:
	//计算CS值
	static BYTE GetCS(BYTE* buf,DWORD len);
};

