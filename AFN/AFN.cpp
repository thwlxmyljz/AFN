// AFN.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TcpServer.h"
#include "AFN01.h"
#include "AFN02.h"
#include "AFN04.h"
#include "AFN0C.h"
#include "AFNPackageBuilder.h"
#include "TelnetServer.h"

static const int PORT = 9027;

int _tmain(int argc, _TCHAR* argv[])
{
	TcpServer tcpsvr(PORT);
	AFNPackageBuilder::Instance().Register(Pkg_Afn_Header::AFN02,&AFN02::HandleRequest,NULL);
	AFNPackageBuilder::Instance().Register(Pkg_Afn_Header::AFN0C,NULL,&AFN0C::HandleAck);
	TelnetThread telsvr;
	telsvr.Start();
	tcpsvr.Run();
	telsvr.Stop();
	return 0;
}

