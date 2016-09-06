#include "svr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include "Api.h"
#include "LogFileu.h"

//---------------------------------------------------------------------------
//MG����
//---------------------------------------------------------------------------
void TSvr::BeforeRun()
{
	TYQProcessMsg::BeforeRun();
	//MG�ĳ�ʼ��
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
//MG�������
//---------------------------------------------------------------------------
void TSvrAdm::OnSignal(int Signal)
{
	TYQService::OnSignal(Signal);
	if (Signal == SIGCHLD)	//�����ӽ����˳�
	{
		pid_t SubPid = wait(NULL);
		if (FMyMg && (SubPid == FMyMg->GetPid()))
		{
			YQLogInfo("MG�������˳������ᱻ�Զ���������");
			delete FMyMg;
			FMyMg = NULL;
		}
	}
}
//---------------------------------------------------------------------------
void TSvrAdm::BeforeRun()
{
	FMyMg = NULL;	
	//����������
	FYQPrcSync = new TYQPrcSync(NULL);
	FYQPrcSync->Create(2, 1);	//���ٽ���,1����д��
	//�����������־
	LogFile = new TLogFileMp(FYQPrcSync, "/var/log/afn.log", 1024000, YQGetName(), YQVersion());

	//�����ӽ����˳�����
	SetSignal(SIGCHLD);
	//��Ϊ���񣬹رղ���Ҫ����Դ
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
