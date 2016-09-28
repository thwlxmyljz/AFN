#ifndef IPackage_H
#define IPackage_H
#include "YQDefs.h"

class IPackage
{
public:
	virtual ~IPackage() {}
	//生产CS校验
	virtual void CreateCS() = 0;
	//设置数据长度
	virtual void SetL() = 0;
	//解包
	/*
	@param data [in] - 收到数据
	@param dataLen [in] - 收到的数据长度
	@param eatLen [out] 分析了有效的长度(一个包的总长度)
	@return =YQER_OK， 成功
	        =YQER_PKG_Err(1)，数据不够，继续等待其他数据
			！=YQER_PKG_Err(1)，数据非法
	*/
	virtual int ParseProto(BYTE* data,DWORD dataLen,DWORD& eatLen) = 0;
	//封包
	virtual int Serialize(BYTE* buf,DWORD bufLen) const = 0;
	//校验包合法性
	virtual BOOL valid() = 0;
	//填写长度以及CS,封包最后一步
	virtual void okPkg() = 0;
	//是否是请求
	virtual BOOL isRequest() = 0;
	//获取功能码
	virtual int GetCode() = 0;
};

#endif // _SOCKETS_IMutex_H

