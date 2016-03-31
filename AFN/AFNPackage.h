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
	//根据pn(1-2040)获取DA1,DA2值(DA2值从1开始)
	static void GetDA(WORD pn,BYTE& DA1,BYTE& DA2)
	{
		if (pn == 0){
			//终端信息点
			DA1 = 0x0;DA2 = 0x0;
		}
		else if (pn == 2041){
			//除p0外所有有效测量点
			DA1 = 0xff;DA2 = 0x0;
		}
		else if (pn <=2040){
			//信息点标识(1~2040)
			DA2 = ((pn-1)>>3)+1;
			DA1 = 1<<((pn-1)&0x07);
		}		
	}
	//根据DA1,DA2获取pn值(1-2040)
	static WORD Getpn(BYTE DA1,BYTE DA2)
	{
		if (DA1 == 0x0 && DA2 == 0x0){
			return 0x0;
		}
		if (DA1 == 0xff && DA2 == 0x0){
			return 2041;
		}
		else if (DA2 <= 255){
			WORD pn = 0x0;
			pn += ((DA2-1)<<3);
			int n = 0x0;
			for (; n<=0x07;n++){
				if (DA1 & (1<<n)){
					pn += (n+1);
					break;
				}
			}
			return pn;
		}
		return 0x0;
	}
	//根据Fn(1-248)获取DT1,DT2值(DT2值从0开始)
	static void GetDT(WORD Fn,BYTE& DT1,BYTE& DT2)
	{
		if (Fn <=248 && Fn > 0){
			DT2 = (Fn-1)>>3;
			DT1 = 1<<((Fn-1)&0x07);
		}		
	}
	//根据DT1,DT2获取Fn值(1-248)
	static WORD GetFn(BYTE DT1,BYTE DT2)
	{
		if (DT2 <= 30){
			WORD Fn = 0x0;
			Fn += (DT2<<3);
			int n = 0x0;
			for (; n<=0x07;n++){
				if (DT1 & (1<<n)){
					Fn += (n+1);
					break;
				}
			}
			return Fn;
		}
		return 0x0;
	}

	//校验包合法性
	inline BOOL valid()
	{		
		return pkgHeader.S1==0x68 && pkgHeader.S2==0x68 && pkgTail.E==0x16;
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
	//应用层包唯一ID
	DWORD m_nId;
	//Fn,pn,值为对应的n
	WORD Fn;
	WORD pn;
};

