#include "Api.h"
#include "Log.h"
#include "YQUtils.h"
#include <signal.h>
#include "TcpServer.h"
#include "AFN01.h"
#include "AFN02.h"
#include "AFN04.h"
#include "AFN0C.h"
#include "AFNPackageBuilder.h"
#include "TelnetServer.h"
#include "WaterPackageBuilder.h"
const char* gVer = "1.0.0";

const char * YQVersion()
{
	return gVer;
}

int YQOpen(void* LogFunc, int MsgQueneID, void *Userdata)
{
	LOG(LOG_INFORMATION,"YQOpen ...");
	return YQER_OK;
}
void YQClose()
{	
	LOG(LOG_INFORMATION,"YQClose ...");
	return;
}

void YQLoop()
{	
	TcpServer tcpsvr;

	AFNPackageBuilder::Instance().Register(Pkg_Afn_Header::AFN02,&AFN02::HandleRequest,NULL);
	AFNPackageBuilder::Instance().Register(Pkg_Afn_Header::AFN0C,NULL,&AFN0C::HandleAck);

	WaterPackageBuilder::Instance().Register(0x1,NULL,NULL);

	LOG(LOG_INFORMATION,"YQLoop start TelnetThread...");
	TelnetThread telsvr;
	telsvr.Start();

	LOG(LOG_INFORMATION,"YQLoop start tcpsvr...");
	tcpsvr.Run();

	return;
}

const char * YQGetName()
{
	return "afn";
}
	
