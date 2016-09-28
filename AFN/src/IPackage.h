#ifndef IPackage_H
#define IPackage_H
#include "YQDefs.h"

class IPackage
{
public:
	virtual ~IPackage() {}
	//����CSУ��
	virtual void CreateCS() = 0;
	//�������ݳ���
	virtual void SetL() = 0;
	//���
	/*
	@param data [in] - �յ�����
	@param dataLen [in] - �յ������ݳ���
	@param eatLen [out] ��������Ч�ĳ���(һ�������ܳ���)
	@return =YQER_OK�� �ɹ�
	        =YQER_PKG_Err(1)�����ݲ����������ȴ���������
			��=YQER_PKG_Err(1)�����ݷǷ�
	*/
	virtual int ParseProto(BYTE* data,DWORD dataLen,DWORD& eatLen) = 0;
	//���
	virtual int Serialize(BYTE* buf,DWORD bufLen) const = 0;
	//У����Ϸ���
	virtual BOOL valid() = 0;
	//��д�����Լ�CS,������һ��
	virtual void okPkg() = 0;
	//�Ƿ�������
	virtual BOOL isRequest() = 0;
	//��ȡ������
	virtual int GetCode() = 0;
};

#endif // _SOCKETS_IMutex_H

