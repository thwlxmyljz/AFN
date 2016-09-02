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
	//����CSУ��
	void CreateCS();
	//�����û����ݳ���
	void SetL();
	//��ȡ���л���֡����
	int GetFrameL(){return PKG_HEADLEN+pkgHeader.L._L.LEN+PKG_TAILLEN;}
public:
	AFNPackage(void);
	~AFNPackage(void);

	//�����յ�������֡������
	/*
	@param data [in] - �յ�����
	@param dataLen [in] - �յ������ݳ���
	@param eatLen [out] ��������Ч�ĳ���(һ�������ܳ���)
	@return =YQER_OK�� �ɹ�
	        =YQER_PKG_Err(1)�����ݲ����������ȴ���������
			��=YQER_PKG_Err(1)�����ݷǷ�
	*/
	int ParseProto(BYTE* data,DWORD dataLen,DWORD& eatLen);

	//�����л���֡buf��
	int Serialize(BYTE* buf,DWORD bufLen) const;

	//У����Ϸ���
	BOOL valid();
	//��д�����Լ�CS,������һ��
	void okPkg();

	//����CSֵ
	static BYTE GetCS(BYTE* buf,DWORD len);
	//����pn(1-2040)��ȡDA1,DA2ֵ(DA2ֵ��1��ʼ)
	static void GetDA(WORD pn,BYTE& DA1,BYTE& DA2);
	//����DA1,DA2��ȡpnֵ(1-2040)
	static WORD Getpn(BYTE DA1,BYTE DA2);
	//����Fn(1-248)��ȡDT1,DT2ֵ(DT2ֵ��0��ʼ)
	static void GetDT(WORD Fn,BYTE& DT1,BYTE& DT2);
	//����DT1,DT2��ȡFnֵ(1-248)
	static WORD GetFn(BYTE DT1,BYTE DT2);
public:
	//Ӧ�ò��ΨһID����
	static DWORD s_pkgID;
	//Ӧ�ò��ΨһID
	DWORD m_nId;
	//Fn,pn,ֵΪ��Ӧ��n
	WORD Fn;
	WORD pn;
};

