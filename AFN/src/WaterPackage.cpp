#include "WaterPackage.h"
#include "YQErrCode.h"
#include "Log.h"

WaterPackage::WaterPackage()
{
}
WaterPackage::~WaterPackage(void)
{
}
int WaterPackage::ParseProto(BYTE* data,DWORD dataLen,DWORD& eatLen)
{
	if (dataLen < 0){
		//��δ������
		LOG(LOG_MINOR,"parse pkg error, len too less , wait for left...");
		eatLen = 0;
		return YQER_PKG_Err(1);
	}

	//���ɰ�ͷ��
	if ((int)dataLen < 0){
		//��δ������
		LOG(LOG_MINOR,"parse pkg error, len less pkg.length, wait for left...");
		eatLen = 0;
		return YQER_PKG_Err(1);
	}

	//���ɰ�β��
	eatLen = 0;

	//�жϰ��Ϸ���
	if (!valid()){
		//����ʶ�ַ��Ƿ�
		LOG(LOG_MINOR,"parse pkg error, Tag error, drop");
		return YQER_PKG_Err(2);
	}
	
	//��������

	LOG(LOG_INFORMATION,"parse pkg ok");
	
	return YQER_OK;
}
int WaterPackage::Serialize(BYTE* buf,DWORD bufLen) const
{
	return 0;
}
//У����Ϸ���
BOOL WaterPackage::valid()
{		
	return TRUE;
}
void WaterPackage::okPkg()
{
	SetL();
	CreateCS();
}
//����CSУ��
void WaterPackage::CreateCS()
{
}
void WaterPackage::SetL()
{
}
BOOL WaterPackage::isRequest()
{
	return TRUE;
}
int WaterPackage::GetCode()
{
	return 0x01;
}