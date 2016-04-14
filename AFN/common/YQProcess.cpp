#include "YQProcess.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//为了使用msginfo结构体，重新定义宏
#ifdef LINUX_ENV
#define _GNU_SOURCE
#endif
//---------------------------------------------------------------------------
/*
	进程通信基类
*/
//---------------------------------------------------------------------------
TIPC::TIPC(LPCSTR KeyPath)
{
	if (KeyPath == NULL)
		FIpcKey = IPC_PRIVATE;
	else
	{
		FIpcKey = ftok(KeyPath, 1);
		if (FIpcKey == -1)
			throw YQSysErr;
	}
}
//---------------------------------------------------------------------------
TIPC::~TIPC()
{
}
//---------------------------------------------------------------------------
/*
	进程同步类
*/
//---------------------------------------------------------------------------
void InitMutex(pthread_mutex_t* Mutex)
{
	int MutexAttrType = PTHREAD_MUTEX_RECURSIVE;
	pthread_mutexattr_t MutexAttr;
	if (pthread_mutexattr_gettype(&MutexAttr, &MutexAttrType) != 0)
		throw YQSysErr;
	if (pthread_mutexattr_settype(&MutexAttr, PTHREAD_MUTEX_RECURSIVE) != 0)
		throw YQSysErr;
	if (pthread_mutex_init(Mutex, &MutexAttr) != 0)
		throw YQSysErr;
}
//---------------------------------------------------------------------------
TYQPrcSync::TYQPrcSync(LPCSTR KeyPath) : TIPC(KeyPath)
{
}
//---------------------------------------------------------------------------
void TYQPrcSync::Create(WORD CriticalCount, WORD ReadWriteCount)	//创建两种锁
{
	int SyncCount = CriticalCount + ReadWriteCount * 3;
	if (SyncCount > SyncCountMax)
		throw YQErr("Count too large");
	FCriticalCount = CriticalCount;
	FReadWriteCount = ReadWriteCount;
	FSyncSetID = semget(FIpcKey, SyncCount, IPC_CREAT|0660);
	if (FSyncSetID == -1)
		throw YQSysErr;
	
	//创建线程互斥
	InitMutex(&FMutex);

	int Val;
	WORD I, J;
	J = CriticalCount + ReadWriteCount;
	Val = 1;
	for (I = 0; I < J ; I++)
		semctl(FSyncSetID, I, SETVAL, Val);
	Val = 0;
	for (I = J; I < SyncCount; I++)
		semctl(FSyncSetID, I, SETVAL, Val);

	memset(FFlag, 0, SyncCountMax * sizeof(WORD));
}
//---------------------------------------------------------------------------
void TYQPrcSync::Destroy()
{
	if (FSyncSetID > 0)
		semctl(FSyncSetID, 0, IPC_RMID, NULL);
}
//---------------------------------------------------------------------------
int TYQPrcSync::GetSyncSetID()
{
	return FSyncSetID;
}
//---------------------------------------------------------------------------
void TYQPrcSync::SetSyncSetID(int SyncSetID, WORD CriticalCount, WORD ReadWriteCount)
{
	FSyncSetID = SyncSetID;
	FCriticalCount = CriticalCount;
	FReadWriteCount = ReadWriteCount;
}
//---------------------------------------------------------------------------
void TYQPrcSync::CheckSyncID(WORD SyncID, WORD Count)
{
	if (SyncID >= Count)
		throw YQErr("SyncID不正确");
}
//---------------------------------------------------------------------------
int TYQPrcSync::SemOpTimout(int SemId, struct sembuf *Sops, unsigned NSops, long Timeout)
{
	int RetVal;
	if (Timeout == 0)
		RetVal = semop(SemId, Sops, NSops);
	else
	{
		struct timespec Tm;
		Tm.tv_sec = Timeout / 1000;	//秒
		Tm.tv_nsec = (Timeout % 1000) * 1000;	//纳秒
		RetVal = semtimedop(SemId, Sops, NSops, &Tm);
	}
	return RetVal;
}
//---------------------------------------------------------------------------
//临界锁
void TYQPrcSync::GetCriticalLockInfo(char* Resul, WORD SyncID)	//Resul由调用者分配256字节的空间
{
	CheckSyncID(SyncID, FCriticalCount);
	sprintf(Resul, "CRLock info: SyncID=%d\r\n\tVAL=%d, PID=%d, ZCNT=%d", SyncID,
			semctl(FSyncSetID, SyncID, GETVAL, NULL),
			semctl(FSyncSetID, SyncID, GETPID, NULL),
			semctl(FSyncSetID, SyncID, GETZCNT, NULL));
}
//---------------------------------------------------------------------------
void TYQPrcSync::CriticalLock(WORD SyncID, long Timeout)		//SyncID:0~(CriticalCount-1)
{
	CheckSyncID(SyncID, FCriticalCount);

	pthread_mutex_lock(&FMutex);
	FFlag[SyncID] ++;
	bool Lock = FFlag[SyncID] == 1;
	pthread_mutex_unlock(&FMutex);

	if(Lock)	//第一次进入，加锁
	{
		struct sembuf Sops;
		Sops.sem_num = SyncID;
		Sops.sem_op = -1;
		Sops.sem_flg = SEM_UNDO ;
		SemOpTimout(FSyncSetID, &Sops, 1, Timeout);
	}
}
//---------------------------------------------------------------------------
void TYQPrcSync::CriticalUnlock(WORD SyncID)		//SyncID:0~(CriticalCount-1)
{
	CheckSyncID(SyncID, FCriticalCount);

	pthread_mutex_lock(&FMutex);
	FFlag[SyncID] --;
	bool Unlock = FFlag[SyncID] == 0;
	pthread_mutex_unlock(&FMutex);

	if(Unlock)	//最后一次即出，解锁
	{
		struct sembuf Sops;
		Sops.sem_num = SyncID;
		Sops.sem_op = 1;
		Sops.sem_flg = SEM_UNDO;
		SemOpTimout(FSyncSetID, &Sops, 1, 0);
	}
}
//---------------------------------------------------------------------------
//读写锁
//---------------------------------------------------------------------------
void TYQPrcSync::GetReadWriteLockInfo(char* Resul, WORD SyncID)	//Resul由调用者分配256字节的空间
{
	CheckSyncID(SyncID, FReadWriteCount);
	WORD WRSyncID = FCriticalCount + FReadWriteCount + SyncID * 2;
	sprintf(Resul, "RWLock info: SyncID=%d\r\n\
\tRR_VAL=%d, RR_PID=%d, RR_ZCNT=%d\r\n\
\tRW_VAL=%d, RW_PID=%d, RW_ZCNT=%d\r\n\
\tWW_VAL=%d, WW_PID=%d, WW_ZCNT=%d",
			SyncID,
			semctl(FSyncSetID, WRSyncID, GETVAL, NULL),
			semctl(FSyncSetID, WRSyncID, GETPID, NULL),
			semctl(FSyncSetID, WRSyncID, GETZCNT, NULL),
			semctl(FSyncSetID, WRSyncID + 1, GETVAL, NULL),
			semctl(FSyncSetID, WRSyncID + 1, GETPID, NULL),
			semctl(FSyncSetID, WRSyncID + 1, GETZCNT, NULL),
			semctl(FSyncSetID, FCriticalCount + SyncID, GETVAL, NULL),
			semctl(FSyncSetID, FCriticalCount + SyncID, GETPID, NULL),
			semctl(FSyncSetID, FCriticalCount + SyncID, GETZCNT, NULL));
}
//---------------------------------------------------------------------------
void TYQPrcSync::ReadLock(WORD SyncID, long Timeout)	//SyncID:0~(ReadWriteCount-1)
{
	CheckSyncID(SyncID, FReadWriteCount);
	WORD WRSyncID = FCriticalCount + FReadWriteCount + SyncID * 2;

	pthread_mutex_lock(&FMutex);
	FFlag[WRSyncID] ++;
	bool Lock = FFlag[WRSyncID] == 1;
	pthread_mutex_unlock(&FMutex);

	if(Lock)	//第一次进入，加锁
	{
		struct sembuf Sops[2];
		Sops[0].sem_num = WRSyncID + 1;
		Sops[0].sem_op = 0;
		Sops[0].sem_flg = 0;
		Sops[1].sem_num = WRSyncID;
		Sops[1].sem_op = 1;
		Sops[1].sem_flg = SEM_UNDO;
		SemOpTimout(FSyncSetID, Sops, 2, Timeout);	//SyncRID锁的值为0时才返回
	}
}
//---------------------------------------------------------------------------
void TYQPrcSync::ReadUnlock(WORD SyncID)	//SyncID:0~(ReadWriteCount-1)
{
	CheckSyncID(SyncID, FReadWriteCount);
	WORD WRSyncID = FCriticalCount + FReadWriteCount + SyncID * 2;

	pthread_mutex_lock(&FMutex);
	FFlag[WRSyncID] --;
	bool Unlock = FFlag[WRSyncID] == 0;
	pthread_mutex_unlock(&FMutex);

	if(Unlock)	//最后一次退出，解锁
	{
		struct sembuf Sops;
		Sops.sem_num = WRSyncID;
		Sops.sem_op = -1;
		Sops.sem_flg = SEM_UNDO;
		SemOpTimout(FSyncSetID, &Sops, 1, 0);
	}
}
//---------------------------------------------------------------------------
void TYQPrcSync::WriteLock(WORD SyncID, long Timeout)	//SyncID:0~(ReadWriteCount-1)
{
	CheckSyncID(SyncID, FReadWriteCount);
	WORD WRSyncID = FCriticalCount + FReadWriteCount + SyncID * 2;

	pthread_mutex_lock(&FMutex);
	FFlag[WRSyncID] ++;
	bool Lock = FFlag[WRSyncID] == 1;
	pthread_mutex_unlock(&FMutex);

	if(Lock)	//第一次进入，加锁
	{
		WORD CriSyncID = FCriticalCount + SyncID;
		struct sembuf Sops[3];
		Sops[0].sem_num = WRSyncID;
		Sops[0].sem_op = 0;
		Sops[0].sem_flg = 0;
		Sops[1].sem_num = CriSyncID;
		Sops[1].sem_op = -1;
		Sops[1].sem_flg = SEM_UNDO;
		Sops[2].sem_num = WRSyncID + 1;
		Sops[2].sem_op = 1;
		Sops[2].sem_flg = SEM_UNDO;
		SemOpTimout(FSyncSetID, Sops, 3, Timeout);	//SyncRID锁的值为0时才返回
	}
}
//---------------------------------------------------------------------------
void TYQPrcSync::WriteUnlock(WORD SyncID)	//SyncID:0~(ReadWriteCount-1)
{
	CheckSyncID(SyncID, FReadWriteCount);
	WORD WRSyncID = FCriticalCount + FReadWriteCount + SyncID * 2;

	pthread_mutex_lock(&FMutex);
	FFlag[WRSyncID] --;
	bool Unlock = FFlag[WRSyncID] == 0;
	pthread_mutex_unlock(&FMutex);

	if(Unlock)	//最后一次退出，解锁
	{
		WORD CriSyncID = FCriticalCount + SyncID;
		struct sembuf Sops[2];
		Sops[0].sem_num = CriSyncID;
		Sops[0].sem_op = 1;
		Sops[0].sem_flg = SEM_UNDO;
		Sops[1].sem_num = WRSyncID + 1;
		Sops[1].sem_op = -1;
		Sops[1].sem_flg = SEM_UNDO;
		SemOpTimout(FSyncSetID, Sops, 2, 0);	//SyncRID锁的值为0时才返回
	}
}
//---------------------------------------------------------------------------
/*
	共享内存类
*/
//---------------------------------------------------------------------------
TYQShareMem::TYQShareMem(LPCSTR KeyPath) : TIPC(KeyPath)
{
	FMem = NULL;
	FMemSize = 0;
	FMemID = 0;
	FLocalCreate = false;
}
//---------------------------------------------------------------------------
void TYQShareMem::Attach(int MemID, int MemSize)
{
	if (FMem != NULL)
		return;
	FMem = shmat(MemID, NULL, 0);
	if ((int)FMem == -1)
	{
		FMem = NULL;
		if (FLocalCreate)
			shmctl(FMemID, IPC_RMID, NULL);
		throw YQSysErr;
	}
	FMemSize = MemSize;
	memset(FMem, 0, FMemSize);
}
//---------------------------------------------------------------------------
void TYQShareMem::Deattach()
{
	if (FMem)
		shmdt(FMem);
	FMem = NULL;
}
//---------------------------------------------------------------------------
void TYQShareMem::Create(int MemSize)
{
	if (FMem)	//重复创建
		throw YQErr("ShareMem already created");
	FMemID = shmget(FIpcKey, MemSize, IPC_CREAT | 0660);
	if (FMemID < 0)
	{
		FMemID = 0;
		throw YQSysErr;
	}
	Attach(FMemID, MemSize);
}
//---------------------------------------------------------------------------
void TYQShareMem::Destroy()
{
	Deattach();
	if (FMemID)
	{
		if (shmctl(FMemID, IPC_RMID, NULL) < 0)
			throw YQSysErr;
	}
}
//---------------------------------------------------------------------------
DWORD TYQShareMem::AdrToOffset(LPCVOID BaseAdr, LPCVOID OffsetAdr)
{
	DWORD Base = (DWORD)BaseAdr;
	DWORD Offset = (DWORD)OffsetAdr;
	if (Offset < Base)
		throw YQErr("AdrToOffset fail");
	return (Offset - Base);
}
//---------------------------------------------------------------------------
void* TYQShareMem::GetMemAdr()
{
	return FMem;
}
//---------------------------------------------------------------------------
int TYQShareMem::GetMemID()
{
	return FMemID;
}
//---------------------------------------------------------------------------
void TYQShareMem::Read(void* Resul, DWORD Offset, DWORD Size)
{
	if ((Offset + Size) > FMemSize)
		throw YQErr("Read share mem fail");

	DWORD ShMem = (DWORD)FMem + Offset;
	memcpy(Resul, (LPCVOID)ShMem, Size);
}
//---------------------------------------------------------------------------
void TYQShareMem::Write(DWORD Offset, const void* Source, DWORD Size)
{
	if ((Offset + Size) > FMemSize)
		throw YQErr("Write share mem fail");

	DWORD ShMem = (DWORD)FMem + Offset;
	memcpy((void*)ShMem, Source, Size);
}
//---------------------------------------------------------------------------
/*
	消息队列类
*/
//---------------------------------------------------------------------------
TYQMsgQueue::TYQMsgQueue(LPCSTR KeyPath) : TIPC(KeyPath)
{
	FMsgQID = -1;
}
//---------------------------------------------------------------------------
void TYQMsgQueue::Create()
{
	if (FMsgQID != -1)	//重复创建
		throw YQErr("MsgQueue already created");
	FMsgQID = msgget(FIpcKey, IPC_CREAT | 0777);
	if (FMsgQID == -1)
		throw YQSysErr;
	if (msgctl(FMsgQID, IPC_INFO, (struct msqid_ds *)(&FMsgInfo)) == -1)
		throw YQSysErr;
}
//---------------------------------------------------------------------------
void TYQMsgQueue::Destroy()
{
	if (FMsgQID != -1)
	{
		if (msgctl(FMsgQID, IPC_RMID, NULL) == -1)
			throw YQSysErr;
	}
}
//---------------------------------------------------------------------------
int TYQMsgQueue::GetMsgQID()
{
	return FMsgQID;
}
//---------------------------------------------------------------------------
void TYQMsgQueue::SetMsgQID(int MsgQID)
{
	FMsgQID = MsgQID;
	if (msgctl(FMsgQID, IPC_INFO, (struct msqid_ds *)(&FMsgInfo)) == -1)
		throw YQSysErr;
}
//---------------------------------------------------------------------------
bool TYQMsgQueue::Send(LPCVOID Msg, long MsgSize, long MsgType, bool IsBlock)
{
	bool SendOK = true;
	if (FMsgQID != -1)
	{
		long LongSize = sizeof(long);
		char* Buf = (char*)malloc(MsgSize + LongSize);
		if (Buf == NULL)
			throw YQErr("Call malloc(...) fail");
		memcpy(Buf, &MsgType, LongSize);
		memcpy(Buf + LongSize, Msg, MsgSize);
		if (msgsnd(FMsgQID, Buf, MsgSize, IsBlock ? 0 : IPC_NOWAIT) == -1)
		{
			if (errno == EAGAIN)
				SendOK = false;
			else
			{
				free(Buf);
				throw YQSysErr;
			}
		}
		free(Buf);
	}
	else
		throw YQErr("MsgQueue none created");
	return SendOK;
}
//---------------------------------------------------------------------------
long TYQMsgQueue::Receive(void* Msg, long MsgSize, long MsgType, long* RetMsgType, bool IsBlock)
{
	long RetVal = -1;
	if (FMsgQID != -1)
	{
		long LongSize = sizeof(long);
		char* Buf = (char*)malloc(MsgSize + LongSize);
		if (Buf == NULL)
			throw YQErr("Call malloc(...) fail");
		int MsgFlg = IsBlock ? MSG_NOERROR : MSG_NOERROR|IPC_NOWAIT;
		
		RetVal = msgrcv(FMsgQID, Buf, MsgSize, MsgType, MsgFlg);
		if (RetVal == -1)
		{
			if (errno == ENOMSG)
				RetVal = 0;
			else
			{
				free(Buf);
				LogFile->FmtLog(__FILE__,__LINE__,LOG_INFORMATION,"err = %d,retVal=%d",errno,RetVal);
				throw YQSysErr;
			}
		}
		if (RetVal > 0)
			memcpy(Msg, Buf + LongSize, MsgSize);
		if (RetMsgType)
			memcpy(RetMsgType, Buf, LongSize);

                free(Buf);
	}
	else
		throw YQErr("MsgQueue none created");
    
	return RetVal;
}
//---------------------------------------------------------------------------
/*
	进程间使用结构TYQTcpMsg通信类
	Execute是一个非阻塞式函数，当有消息时，调用OnReceive
*/
//---------------------------------------------------------------------------
TYQPsMsg::TYQPsMsg(long MsgType, LPCSTR KeyPath) : TYQMsgQueue(KeyPath)
{
	FMsgType = MsgType;
}
//---------------------------------------------------------------------------
void TYQPsMsg::Execute()
{
	char* Buf = (char*)malloc(FMsgInfo.msgmnb);
	if (Buf == NULL)
		throw YQErr("Call malloc(...) fail");
	try
	{
		TYQTcpMsg* YQMsg;
		int MsgSize = sizeof(TYQTcpMsg);
		void* Stream = NULL;

		int RecCount = Receive(Buf,  FMsgInfo.msgmnb, FMsgType, NULL, false);	//非阻塞接收
		if (RecCount >= MsgSize)	//收到消息
		{
			YQMsg = (TYQTcpMsg*)Buf;
			if (RecCount > MsgSize)
				Stream = (void*)(Buf + MsgSize);
			OnReceive(YQMsg, RecCount - MsgSize, Stream);
		}
	}
	catch(TYQException *E)
	{
		free(Buf);
		throw E;
	}
	free(Buf);
}
//---------------------------------------------------------------------------
void TYQPsMsg::SendMsg(long MsgType, int Flow, int MsgInt1, int MsgInt2, DWORD MsgSize, LPCVOID MsgStr,
			DWORD StreamSize, LPCVOID Stream)
{
	//申请发送缓存
	int SendBufSize = sizeof(TYQTcpMsg) + StreamSize;
	char* SendBuf = (char*)malloc(SendBufSize);
	if (SendBuf == NULL)
		throw YQErr("Call malloc(...) fail");
	memset(SendBuf, 0, SendBufSize);
	//加载数据到缓存
	TYQTcpMsg* Temp = (TYQTcpMsg*)SendBuf;
	Temp->Flow = Flow;
	Temp->MsgInt1 = MsgInt1;
	Temp->MsgInt2 = MsgInt2;
	if (MsgSize > 0)
	{
		if (MsgSize > 48)
			MsgSize = 48;
		memcpy(Temp->MsgStr, MsgStr, MsgSize);
	}
	if (StreamSize > 0)
		memcpy(SendBuf + sizeof(TYQTcpMsg), Stream, StreamSize);
	//发送数据
	try
	{
		//队列空间不足时接收队列的数据，防止互相等待造成死锁。
		while(Send(SendBuf, SendBufSize, MsgType, false) == false)
		{
			Execute();
		}
	}
	catch(TYQException *E)
	{
		free(SendBuf);
		throw E;
	}
	free(SendBuf);
}
//---------------------------------------------------------------------------
/*
	多进程日志类
*/
//---------------------------------------------------------------------------
TLogFileMp::TLogFileMp(TYQPrcSync* YQPrcSync, LPCSTR FileName, DWORD MaxLen, LPCSTR AppName, LPCSTR AppVersn)
		: TLogFile(FileName, MaxLen, AppName, AppVersn)
{
	FYQPrcSync = YQPrcSync;
	InitMutex(&FMutex);
}
//---------------------------------------------------------------------------
void TLogFileMp::Lock()
{
	pthread_mutex_lock(&FMutex);
	FYQPrcSync->CriticalLock(0, 0);
}
//---------------------------------------------------------------------------
void TLogFileMp::Unlock()
{
	FYQPrcSync->CriticalUnlock(0);
	pthread_mutex_unlock(&FMutex);
}
//---------------------------------------------------------------------------
/*
	进程基类
*/
//---------------------------------------------------------------------------
TYQProcess* TYQProcess::SubPrcPtr = NULL;
TYQProcess::TYQProcess()
{
	FPid = 0;
}
//---------------------------------------------------------------------------
TYQProcess::~TYQProcess()
{
}
//---------------------------------------------------------------------------
void TYQProcess::Run()
{
	FPid = fork();
	if (FPid < 0)
		throw YQSysErr;
	else if (FPid == 0)
	{
		SubPrcPtr = this;
		FPid = getpid();
		Run(true);
	}
	else
		Run(false);
}
//---------------------------------------------------------------------------
void TYQProcess::Run(bool IsSubProc)
{
	try
	{
		if (IsSubProc)
			OnStart();
		else
			OnParentRun();
	}
	catch(TYQException* E)
	{
		YQOccurException(E);
	}
	catch(...)
	{
		YQOccurException(NULL);
	}
	if (IsSubProc)
		exit(0);
}
//---------------------------------------------------------------------------
void TYQProcess::OnParentRun()
{
}
//---------------------------------------------------------------------------
void TYQProcess::YQOccurException(TYQException* YQException)
{
	try
	{
		OnException(YQException);
	}
	catch(...)
	{
	}
	if (YQException)
		delete YQException;
}
//---------------------------------------------------------------------------
void TYQProcess::OnException(TYQException* YQException)
{
	if (LogFile == NULL)
		return;
	if (YQException)
		LogFile->WriteLog(YQException->Message(), LOG_MAJOR);
	else
		LogFile->WriteLog("Unknown exception", LOG_MAJOR);
}
//---------------------------------------------------------------------------
pid_t TYQProcess::GetPid()
{
	return FPid;
}
//---------------------------------------------------------------------------
void TYQProcess::Wait()
{
	waitpid(FPid, NULL, 0);
}
//---------------------------------------------------------------------------
//信号处理
void TYQProcess::SetSignal(int Signal)
{
	if (signal(Signal, SigHandler) == SIG_ERR)
		throw YQSysErr;
}
//---------------------------------------------------------------------------
void TYQProcess::SigHandler(int Signal)
{
	if (SubPrcPtr == NULL)
		return;
	try
	{
		SubPrcPtr->OnSignal(Signal);
	}
	catch(TYQException* E)
	{
		SubPrcPtr->YQOccurException(E);
	}
	catch(...)
	{
		SubPrcPtr->YQOccurException(NULL);
	}
}
//---------------------------------------------------------------------------
void TYQProcess::OnSignal(int Signal)
{
}
//---------------------------------------------------------------------------
/*
	消息进程类：循环执行OnRun，直到收到信号SIGTERM才停止
*/
//---------------------------------------------------------------------------
void TYQProcessMsg::OnStart()
{
	SetSignal(SIGTERM);
	try
	{
		FTerminated = false;
		BeforeRun();
		while (!FTerminated)
		{
			try
			{
				OnRun();
				usleep(10000);	//10毫秒
			}
			catch(TYQException* E)
			{
				YQOccurException(E);
			}
			catch(...)
			{
				YQOccurException(NULL);
			}
		}
		AfterRun();
	}
	catch(TYQException* E)
	{
		YQOccurException(E);
	}
	catch(...)
	{
		YQOccurException(NULL);
	}

}
//---------------------------------------------------------------------------
void TYQProcessMsg::OnSignal(int Signal)
{
	TYQProcess::OnSignal(Signal);
	if (Signal == SIGTERM)
		FTerminated = true;
}
//---------------------------------------------------------------------------
void TYQProcessMsg::BeforeRun()
{
}
//---------------------------------------------------------------------------
void TYQProcessMsg::AfterRun()
{
}
//---------------------------------------------------------------------------
void TYQProcessMsg::Terminate()
{
	kill(GetPid(), SIGTERM);
}
//---------------------------------------------------------------------------
/*
	守护进程（服务）类
*/
//---------------------------------------------------------------------------
void TYQService::OnParentRun()
{
	exit(0);
}
//---------------------------------------------------------------------------
void TYQService::BeforeRun()
{
	FSid = setsid();
	if (FSid < 0)
		throw YQSysErr;

	if (chdir("/") < 0)
		throw YQSysErr;

	umask(0);

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

}
//---------------------------------------------------------------------------

