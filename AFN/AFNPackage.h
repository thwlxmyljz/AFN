#pragma once

#include "proto.h"
#include <list>

class AFNPackage : public Pkg
{
	friend class Connection;
	friend class AFNPackageBuilder;

	//填写ACK包的相关字段
	void SetAckType2ZJQ();
	//填写REQ包的相关字段
	void SetReqType2ZJQ();
public:
	AFNPackage(void);
	~AFNPackage(void);

	//分析收到的数据帧生产包
	int ParseProto(BYTE* data,DWORD len);
	//包序列化到帧buf中
	int Serialize(BYTE* buf,DWORD bufLen);

	//计算CS值
	static BYTE GetCS(BYTE* buf,DWORD len)
	{
		BYTE cs = 0x00;
		for (DWORD i = 0; i < len; i++){
			cs += buf[i];
		}
		return cs;
	}

	//校验包合法性
	inline BOOL valid()
	{		
		return pkgHeader.S1==0x68 && pkgHeader.S2==0x68 && pkgTail.E==0x16;
	}
	//设置响应包RSEQ
	inline void SetRSEQ()
	{
		pAfn->afnHeader.SEQ._SEQ.PRSEQ = s_RSEQ++;
		if (s_RSEQ > 15){
			s_RSEQ = 0;
		}
	}
	//生产CS校验
	inline void CreateCS()
	{
		pkgTail.CS = 0x00;
		pkgTail.CS += userHeader.C.c;
		pkgTail.CS += userHeader.A1;
		pkgTail.CS += userHeader.A1>>8;
		pkgTail.CS += userHeader.A2;
		pkgTail.CS += userHeader.A2>>8;
		pkgTail.CS += userHeader.A3.a3;
		pkgTail.CS += pAfn->afnHeader.AFN;
		pkgTail.CS += pAfn->afnHeader.SEQ.seq;
		pkgTail.CS += pAfn->pAfnData->m_Tag.DA1;
		pkgTail.CS += pAfn->pAfnData->m_Tag.DA2;
		pkgTail.CS += pAfn->pAfnData->m_Tag.DT1;
		pkgTail.CS += pAfn->pAfnData->m_Tag.DT2;
		for (DWORD i = 0; i < pAfn->pAfnData->m_nLen; i++){
			pkgTail.CS += pAfn->pAfnData->m_pData[i];
		}
	}
	inline void SetL()
	{
		pkgHeader.L._L.LEN = PKG_USER_HEADLEN+pAfn->GetDataLen();
		pkgHeader.L1._L1.LEN = PKG_USER_HEADLEN+pAfn->GetDataLen();
	}
public:
	//应用层包唯一ID计数
	static DWORD s_pkgID;
	//响应帧序号
	static BYTE s_RSEQ;
	//请求帧序号
	static BYTE s_PSEQ;
	//主站MSA值
	static BYTE s_MSA;
	//应用层包唯一ID
	DWORD m_nId;	
};

