// AFN.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "TcpServer.h"
#include "Api.h"
#include "Log.h"

#ifdef _WIN32
	#include <tchar.h>
	int _tmain(int argc, _TCHAR* argv[])
	{
		YQOpen(NULL,0,NULL);
		YQLoop();
		YQClose();
	}
#else
	#ifdef RUNASCHILD
	#include "svr.h"
	int main()
	{
		TSvrAdm Adm;
		Adm.Run();
		return 0;
	}
	#else
	int main()
	{
		YQOpen(NULL,0,NULL);
		YQLoop();
		YQClose();
	}
#endif
#endif
