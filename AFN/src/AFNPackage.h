#pragma once

#include "proto.h"
#include <list>
#include "IPackage.h"

class AFNPackage : public Pkg, public IPackage
{
	friend class Connection;
	friend class AFNPackageBuilder;
public:
	AFNPackage(void);
	~AFNPackage(void);
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
	//填写ACK包的相关字段
	void SetAckType2ZJQ();
	//填写REQ包的相关字段
	void SetReqType2ZJQ();
	//获取序列化后帧长度
	int GetFrameL(){return PKG_HEADLEN+pkgHeader.L._L.LEN+PKG_TAILLEN;}
	//计算CS值
	static BYTE GetCS(BYTE* buf,DWORD len);
	//根据pn(1-2040)获取DA1,DA2值(DA2值从1开始)
	static void GetDA(WORD pn,BYTE& DA1,BYTE& DA2);
	//根据DA1,DA2获取pn值(1-2040)
	static WORD Getpn(BYTE DA1,BYTE DA2);
	//根据Fn(1-248)获取DT1,DT2值(DT2值从0开始)
	static void GetDT(WORD Fn,BYTE& DT1,BYTE& DT2);
	//根据DT1,DT2获取Fn值(1-248)
	static WORD GetFn(BYTE DT1,BYTE DT2);
public:
	//应用层包唯一ID计数
	static DWORD s_pkgID;
	//应用层包唯一ID
	DWORD m_nId;
	//Fn,pn,值为对应的n
	WORD Fn;
	WORD pn;
};

