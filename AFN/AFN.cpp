// AFN.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TcpServer.h"
#include "AFN02.h"
#include "AFNPackageBuilder.h"

static const int PORT = 9027;

int _tmain(int argc, _TCHAR* argv[])
{
	TcpServer svr(PORT);
	AFNPackageBuilder::Instance().Register(Pkg_Afn_Header::AFN02,&AFN02::HandleRequest,NULL);
	svr.Run();
	return 0;
}

