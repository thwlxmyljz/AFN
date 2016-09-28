#include "ConnectionList.h"
#include "WaterPackageBuilder.h"
#include "YQErrCode.h"
#include "Lock.h"
#include "LogFileu.h"
#include "Log.h"
#include "Utility.h"

WaterPackageBuilder* WaterPackageBuilder::single = NULL;
WaterPackageBuilder::WaterPackageBuilder(void)
	:PackageBuilder()
{
}
WaterPackageBuilder::~WaterPackageBuilder(void)
{
}

IPackage* WaterPackageBuilder::CreateAck(IPackage* _reqPkg, void* data)
{
	return NULL;
}

int WaterPackageBuilder::DoHandleAck(std::list<IPackage*>& ackLst)
{	
	if (ackLst.size() == 0){
		return -1;
	}
	return YQER_OK;;
}
