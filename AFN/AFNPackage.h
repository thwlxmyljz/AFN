#pragma once

#include "proto.h"

class AFNPackage : public Pkg
{
	friend class Connection;
	friend class AFNPackageBuilder;
public:
	AFNPackage(void);
	~AFNPackage(void);

	//���ݷ�����
	int parseProto(BYTE* data,DWORD len);
	//�����л���buf��
	int serialize(BYTE* buf,DWORD bufLen);

	//������
	virtual int HandlePkg();

	//����CSֵ
	static BYTE GetCS(BYTE* buf,DWORD len)
	{
		BYTE cs = 0x00;
		for (DWORD i = 0; i < len; i++){
			cs += buf[i];
		}
		return cs;
	}

	/*У����Ϸ���*/
	inline BOOL valid()
	{		
		return pkgHeader.S1==0x68 && pkgHeader.S2==0x68 && pkgTail.E==0x16;
	}
private:
	//Ӧ�ò��ΨһID����
	static DWORD s_pkgID;
	//Ӧ�ò��ΨһID
	DWORD m_nId;
};

