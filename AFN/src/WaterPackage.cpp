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
		//包未收完整
		LOG(LOG_MINOR,"parse pkg error, len too less , wait for left...");
		eatLen = 0;
		return YQER_PKG_Err(1);
	}

	//生成包头域
	if ((int)dataLen < 0){
		//包未收完整
		LOG(LOG_MINOR,"parse pkg error, len less pkg.length, wait for left...");
		eatLen = 0;
		return YQER_PKG_Err(1);
	}

	//生成包尾域
	eatLen = 0;

	//判断包合法性
	if (!valid()){
		//包标识字符非法
		LOG(LOG_MINOR,"parse pkg error, Tag error, drop");
		return YQER_PKG_Err(2);
	}
	
	//数据生成

	LOG(LOG_INFORMATION,"parse pkg ok");
	
	return YQER_OK;
}
int WaterPackage::Serialize(BYTE* buf,DWORD bufLen) const
{
	return 0;
}
//校验包合法性
BOOL WaterPackage::valid()
{		
	return TRUE;
}
void WaterPackage::okPkg()
{
	SetL();
	CreateCS();
}
//生产CS校验
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