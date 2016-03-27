#pragma once

#include "proto.h"
#include <list>

class AFNPackage : public Pkg
{
	friend class Connection;
	friend class AFNPackageBuilder;

	//������������վ������,ackPkg������Ӧ֡������0~��֡��
	int HandleRequest(std::list<AFNPackage*>& ackLst);
	//��Ϊ�Ӷ�վ,������Ӧ����,ackPkg������Ӧ֡������0~��֡��
	int HandleAck(std::list<AFNPackage*>& ackLst);

	//��дACK��������ֶ�
	void SetAckType2ZJQ();
	//��дREQ��������ֶ�
	void SetReqType2ZJQ();
public:
	AFNPackage(void);
	~AFNPackage(void);

	//���ݷ�����
	int ParseProto(BYTE* data,DWORD len);
	//�����л���buf��
	int Serialize(BYTE* buf,DWORD bufLen);

	//��֡����,ackLst������Ӧ֡������0~��֡��
	int HandlePkg(std::list<AFNPackage*>& ackLst);
	//��֡����,reqLst��֡����ackLst������Ӧ֡������0����֡��
	int HandlePkg(std::list<AFNPackage*>& reqLst,std::list<AFNPackage*>& ackLst);

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
	//��Ӧ֡���
	static BYTE s_RSEQ;
	//����֡���
	static BYTE s_PSEQ;
	//��վMSAֵ
	static BYTE s_MSA;
private:
	//Ӧ�ò��ΨһID
	DWORD m_nId;
	void DoSetRSEQ();
};

