/****************************************************************************
公司名称：深圳市亿桥科技有限公司
版权申明：版权所有 (C) 2008
文件标识： 
创建时间：2008-08
作           者：陈群祥
****************************************************************************/
#ifndef YQProcess_h
#define YQProcess_h
#include "YQCommon.h"
#include "LogFileu.h"
#include "YQTcpMsg.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pthread.h>

//---------------------------------------------------------------------------
/*
	进程通信基类
*/
//---------------------------------------------------------------------------
class TIPC
{
protected:
	key_t FIpcKey;
public:
	TIPC(LPCSTR KeyPath);
	virtual ~TIPC();
};
//---------------------------------------------------------------------------
/*
	进程同步类
	同时创建多个临界锁或读写锁，创建锁的进程使用Create/Destroy
	非创建锁的进程使用SetSyncSetID加入外部进程锁，不能使用Create/Destroy
*/
//---------------------------------------------------------------------------
#define SyncCountMax 12
class TYQPrcSync : public TIPC
{
private:
	int FSyncSetID;
	WORD FCriticalCount;
	WORD FReadWriteCount;
	WORD FFlag[SyncCountMax];		//信号量进入标志位
	pthread_mutex_t FMutex;			//锁定信号量标志位
	
	void CheckSyncID(WORD SyncID, WORD Count);
	int SemOpTimout(int SemId, struct sembuf *Sops, unsigned NSops, long Timeout);
public:
	TYQPrcSync(LPCSTR KeyPath);

	void Create(WORD CriticalCount, WORD ReadWriteCount);	//创建两种锁
	void Destroy();
	int GetSyncSetID();
	void SetSyncSetID(int SyncSetID, WORD CriticalCount, WORD ReadWriteCount);

	//临界锁,Timeout为毫秒,=0表示无限等待
	void GetCriticalLockInfo(char* Resul, WORD SyncID);	//Resul由调用者分配256字节的空间
	void CriticalLock(WORD SyncID, long Timeout);		//SyncID:0~(CriticalCount-1)
	void CriticalUnlock(WORD SyncID);		//SyncID:0~(CriticalCount-1)
	//读写锁,Timeout为毫秒,=0表示无限等待
	void GetReadWriteLockInfo(char* Resul, WORD SyncID);	//Resul由调用者分配256字节的空间
	void ReadLock(WORD SyncID, long Timeout);	//SyncID:0~(ReadWriteCount-1)
	void ReadUnlock(WORD SyncID);	//SyncID:0~(ReadWriteCount-1)

	void WriteLock(WORD SyncID, long Timeout);	//SyncID:0~(ReadWriteCount-1)
	void WriteUnlock(WORD SyncID);	//SyncID:0~(ReadWriteCount-1)
};
//---------------------------------------------------------------------------
/*
	共享内存类
	读写内存时没有加锁，由外部调用者实现
	创建共享内存的进程:
		使用Create/Destroy不使用Attach/Deattach
	非创建共享内存的进程:
		使用Attach/Deattach不使用Create/Destroy
*/
//---------------------------------------------------------------------------
class TYQShareMem : public TIPC
{
private:
	int FMemID;
	void* FMem;
	DWORD FMemSize;
	bool FLocalCreate;
public:
	TYQShareMem(LPCSTR KeyPath);

	void Create(int MemSize);
	void Destroy();

	void Attach(int MemID, int MemSize);
	void Deattach();

	DWORD AdrToOffset(LPCVOID BaseAdr, LPCVOID OffsetAdr); 

	void* GetMemAdr();
	int GetMemID();
	void Read(void* Resul, DWORD Offset, DWORD Size);
	void Write(DWORD Offset, LPCVOID Source, DWORD Size);
};
//---------------------------------------------------------------------------
/*
	消息队列类
	读写内存时没有加锁，由外部调用者实现
	创建消息队列的进程:
		使用Create/Destroy不使用SetMsgQID
	非创建消息队列的进程:
		使用SetMsgQID不使用Create/Destroy
*/
//---------------------------------------------------------------------------
class TYQMsgQueue : public TIPC
{
private:
	int FMsgQID;
protected:
	struct msginfo FMsgInfo;
public:
	TYQMsgQueue(LPCSTR KeyPath);
	
	void Create();
	void Destroy();

	int GetMsgQID();
	void SetMsgQID(int MsgQID);

	//发送指定MsgType类型的消息
	//IsBlock：当队列空间不足时是否阻塞
	//返回：非阻塞发送队列空间不足时为否，其它抛出异常。
	bool Send(LPCVOID Msg, long MsgSize, long MsgType, bool IsBlock = true);

	//阻塞式/非阻塞式接收指定类型的消息
	//MsgType=0:接收全部类型的消息
	//返回实际收到的字节数及消息类型,RetMsgType = NULL时不需要返回消息类型
	long Receive(void* Msg, long MsgSize, long MsgType, long* RetMsgType = NULL, bool IsBlock = true);
};
//---------------------------------------------------------------------------
/*
	进程间使用结构TYQTcpMsg通信类
	Execute是一个非阻塞式函数，当有消息时，调用OnReceive
*/
//---------------------------------------------------------------------------
class TYQPsMsg : public TYQMsgQueue
{
private:
	long FMsgType;
protected:
	virtual void OnReceive(const TYQTcpMsg* YQMsg, DWORD StreamSize, LPCVOID Stream) = 0;
public:
	TYQPsMsg(long MsgType, LPCSTR KeyPath);

	void Execute();
	//MsgStr最长不超过48字节，超过需要使用Stream
	void SendMsg(long MsgType, int Flow, int MsgInt1 = 0, int MsgInt2 = 0, DWORD MsgSize = 0, LPCVOID MsgStr = NULL,
			DWORD StreamSize = 0, LPCVOID Stream = NULL);
};
//---------------------------------------------------------------------------
/*
	多进程日志类
*/
//---------------------------------------------------------------------------
class TLogFileMp : public TLogFile
{
private:
	TYQPrcSync* FYQPrcSync;
	pthread_mutex_t FMutex;			//线程锁
protected:
	virtual void Lock();
	virtual void Unlock();
public:
	TLogFileMp(TYQPrcSync* YQPrcSync, LPCSTR FileName, DWORD MaxLen, LPCSTR AppName, LPCSTR AppVersn);
};
//---------------------------------------------------------------------------
/*
	进程基类
	可继承的事件：
		OnStart/OnParentRun/OnException
	可供外部调用的方法：
		Run/Wait/GetPid
*/
//---------------------------------------------------------------------------
class TYQProcess
{
private:
	pid_t FPid;

	void Run(bool IsSubProc);
	static TYQProcess* SubPrcPtr;
	static void SigHandler(int Signal);
protected:
	void YQOccurException(TYQException* YQException);

	virtual void OnStart() = 0;
	virtual void OnParentRun();	/*供服务使用*/
	virtual void OnException(TYQException* YQException);
	virtual void OnSignal(int Signal);	//捕捉到信号
public:
	TYQProcess();
	virtual ~TYQProcess();

	void Run();
	pid_t GetPid();
	void Wait();	//父进程等待子进程退出
	void SetSignal(int Signal);	//设置被捕捉的信号，捕捉到信号后触发OnSignal事件
};
//---------------------------------------------------------------------------
/*
	消息进程类：循环执行OnRun，直到收到信号SIGTERM才停止
	可继承的事件：
		BeforeRun/OnRun/AfterRun/OnParentRun/OnException
	可供外部调用的方法：
		Run/Wait/Terminate/GetPid
*/
//---------------------------------------------------------------------------
class TYQProcessMsg : public TYQProcess
{
private:
	bool FTerminated;
	virtual void OnStart();
protected:
	virtual void BeforeRun();
	virtual void OnRun() = 0;
	virtual void AfterRun();
	virtual void OnSignal(int Signal);
public:
	void Terminate();	//父进程终止子进程
};
//---------------------------------------------------------------------------
/*
	守护进程（服务）类
	继承类如果继承BeforeRun，一定要调用TYQService::BeforeRun()
	可继承的事件：
		BeforeRun/OnRun/AfterRun/OnException
	可供外部调用的方法：
		Run/Wait/Terminate/GetPid
*/
//---------------------------------------------------------------------------
class TYQService : public TYQProcessMsg
{
private:
	pid_t FSid;

	void OnParentRun();	
protected:
	virtual void BeforeRun();
};
//---------------------------------------------------------------------------
#endif

