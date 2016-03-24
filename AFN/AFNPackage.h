#pragma once
#include "proto.h"
class AFNPackage : public pkg
{
	friend class Connection;
	friend class AFNPackageBuilder;
public:
	AFNPackage(void);
	~AFNPackage(void);

	int parseProto(BYTE* data,unsigned int len);
private:
	//应用层包唯一ID计数
	static unsigned int s_pkgID;
	//应用层包唯一ID
	unsigned int m_nId;
};

