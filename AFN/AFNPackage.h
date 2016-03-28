#pragma once

#include "proto.h"
#include <list>

class AFNPackage : public Pkg
{
	friend class Connection;
	friend class AFNPackageBuilder;

	//��дACK��������ֶ�
	void SetAckType2ZJQ();
	//��дREQ��������ֶ�
	void SetReqType2ZJQ();
public:
	AFNPackage(void);
	~AFNPackage(void);

	//�����յ�������֡������
	int ParseProto(BYTE* data,DWORD len);
	//�����л���֡buf��
	int Serialize(BYTE* buf,DWORD bufLen);

	//����CSֵ
	static BYTE GetCS(BYTE* buf,DWORD len)
	{
		BYTE cs = 0x00;
		for (DWORD i = 0; i < len; i++){
			cs += buf[i];
		}
		return cs;
	}

	//У����Ϸ���
	inline BOOL valid()
	{		
		return pkgHeader.S1==0x68 && pkgHeader.S2==0x68 && pkgTail.E==0x16;
	}
	//������Ӧ��RSEQ
	inline void SetRSEQ()
	{
		pAfn->afnHeader.SEQ._SEQ.PRSEQ = s_RSEQ++;
		if (s_RSEQ > 15){
			s_RSEQ = 0;
		}
	}
	//����CSУ��
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
	//Ӧ�ò��ΨһID����
	static DWORD s_pkgID;
	//��Ӧ֡���
	static BYTE s_RSEQ;
	//����֡���
	static BYTE s_PSEQ;
	//��վMSAֵ
	static BYTE s_MSA;
	//Ӧ�ò��ΨһID
	DWORD m_nId;	
};

