// AFN.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TcpServer.h"
static const int PORT = 9027;



int _tmain(int argc, _TCHAR* argv[])
{
	TcpServer svr(PORT);
	svr.Run();
	return 0;
}

