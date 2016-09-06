#include "svr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include "Api.h"
#include "LogFileu.h"

//---------------------------------------------------------------------------
//MG进程
//---------------------------------------------------------------------------
void TSvr::BeforeRun()
{
	TYQProcessMsg::BeforeRun();
	//MG的初始化
	YQOpen((void*)&TLogFileMp::CallBack,0,NULL);
}
//---------------------------------------------------------------------------
void TSvr::OnRun()
{
	YQLoop();
}
//---------------------------------------------------------------------------
void TSvr::OnSignal(int Signal)
{
	TYQProcessMsg::OnSignal(Signal);
	if (Signal == SIGTERM)
	{
		kill(GetPid(),SIGINT);
	}
}
//---------------------------------------------------------------------------
void TSvr::AfterRun()
{
	TYQProcessMsg::AfterRun();
	YQClose();
}
//---------------------------------------------------------------------------
//MG管理进程
//---------------------------------------------------------------------------
void TSvrAdm::OnSignal(int Signal)
{
	TYQService::OnSignal(Signal);
	if (Signal == SIGCHLD)	//处理子进程退出
	{
		pid_t SubPid = wait(NULL);
		if (FMyMg && (SubPid == FMyMg->GetPid()))
		{
			YQLogInfo("MG非正常退出，将会被自动重新运行");
			delete FMyMg;
			FMyMg = NULL;
		}
	}
}
//---------------------------------------------------------------------------
void TSvrAdm::BeforeRun()
{
	FMyMg = NULL;	
	//创建进程锁
	FYQPrcSync = new TYQPrcSync(NULL);
	FYQPrcSync->Create(2, 1);	//个临界锁,1个读写锁
	//创建多进程日志
	LogFile = new TLogFileMp(FYQPrcSync, "/var/log/afn.log", 1024000, YQGetName(), YQVersion());

	//设置子进程退出监视
	SetSignal(SIGCHLD);
	//成为服务，关闭不必要的资源
	TYQService::BeforeRun();
}
//---------------------------------------------------------------------------
void TSvrAdm::OnRun()
{
	if (FMyMg == NULL)
	{
		FMyMg = new TSvr();
		FMyMg->Run();
	}
	else
		;
	sleep(1);
}
//---------------------------------------------------------------------------
void TSvrAdm::AfterRun()
{
	TYQService::AfterRun();
	if (FMyMg)
	{
		FMyMg->Terminate();
		FMyMg->Wait();
		delete FMyMg;
		FMyMg = NULL;
	}	
	delete LogFile;
	FYQPrcSync->Destroy();
	delete FYQPrcSync;
}
//---------------------------------------------------------------------------
