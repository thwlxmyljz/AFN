#pragma once

#include "proto.h"

class AFNPackage : public Pkg
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

	//计算CS值
	static BYTE GetCS(BYTE* buf,DWORD len)
	{
		BYTE cs = 0x00;
		for (DWORD i = 0; i < len; i++){
			cs += buf[i];
		}
		return cs;
	}
	/*校验包合法性*/
	inline BOOL valid()
	{		
		return pkgHeader.S1==0x68 && pkgHeader.S2==0x68 && pkgTail.E==0x16;
	}
private:
	//应用层包唯一ID计数
	static DWORD s_pkgID;
	//应用层包唯一ID
	DWORD m_nId;
};

