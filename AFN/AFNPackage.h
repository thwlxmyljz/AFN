#pragma once
#include "proto.h"
class AFNPackage
{
	friend class Connection;
	friend class AFNPackageBuilder;
public:
	AFNPackage(void);
	~AFNPackage(void);

	//通过data数据分析出m_pkg包
	int parseProto(BYTE* data,DWORD len);
	//将m_pkg包序列化到buf中
	int serialize(BYTE* buf,DWORD bufLen);
private:
	//应用层包唯一ID计数
	static unsigned int s_pkgID;
	//应用层包唯一ID
	unsigned int m_nId;
	//包
	pkg m_pkg;
};

