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
	//Ӧ�ò��ΨһID����
	static unsigned int s_pkgID;
	//Ӧ�ò��ΨһID
	unsigned int m_nId;
};

