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

const char* gVer = "1.0.0";
const int PORT = 9027;

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
	LOG(LOG_INFORMATION,"YQLoop ...");
	TcpServer tcpsvr(PORT);
	AFNPackageBuilder::Instance().Register(Pkg_Afn_Header::AFN02,&AFN02::HandleRequest,NULL);
	AFNPackageBuilder::Instance().Register(Pkg_Afn_Header::AFN0C,NULL,&AFN0C::HandleAck);
	TelnetThread telsvr;
	telsvr.Start();
	tcpsvr.Run();
	return;
}

const char * YQGetName()
{
	return "afn";
}
	
