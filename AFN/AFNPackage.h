#pragma once
#include "proto.h"
class AFNPackage
{
	friend class Connection;
	friend class AFNPackageBuilder;
public:
	AFNPackage(void);
	~AFNPackage(void);

	//ͨ��data���ݷ�����m_pkg��
	int parseProto(BYTE* data,DWORD len);
	//��m_pkg�����л���buf��
	int serialize(BYTE* buf,DWORD bufLen);
private:
	//Ӧ�ò��ΨһID����
	static unsigned int s_pkgID;
	//Ӧ�ò��ΨһID
	unsigned int m_nId;
	//��
	pkg m_pkg;
};

