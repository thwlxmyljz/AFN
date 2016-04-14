/****************************************************************************
公司名称：深圳市亿桥科技有限公司
版权申明：版权所有 (C) 2008
单元名称：Threadu.h
文件标识： 
创建时间：2008-08-06
作    者：陈群祥
版    本：V1.0
摘    要：
当前版本：
修  改者：
修改日期：
修改摘要：
****************************************************************************/
//---------------------------------------------------------------------------
#ifndef ThreaduH
#define ThreaduH
#include "YQCommon.h"
#include "LogFileu.h"
#include <pthread.h>
//---------------------------------------------------------------------------
/*
	多线程互斥锁类
*/
class TCriticalSection
{
private:
	pthread_mutex_t FMutex;
	int FRetVal;
public:
	TCriticalSection();
	bool IsCreateOK();	//返回初始化结果

	void Enter();
	void Leave();
};
//---------------------------------------------------------------------------
/*
	多线程读写锁类
*/
class TReadWriteLock
{
private:
	pthread_rwlock_t FRWLock;
	pthread_rwlockattr_t FAttr;
public:
	TReadWriteLock();
	void Create();
	void Destroy();

	//读写锁,Timeout为毫秒,=0表示无限等待
	void ReadLock(long Timeout);
	void ReadUnlock();

	void WriteLock(long Timeout);
	void WriteUnlock();
};
//---------------------------------------------------------------------------
/*
	多线程日志类
*/
//---------------------------------------------------------------------------
class TLogFileMth : public TLogFile
{
private:
	TCriticalSection* FCriticalSection;
protected:
	virtual void Lock();
	virtual void Unlock();
public:
	TLogFileMth(LPCSTR FileName, DWORD MaxLen, LPCSTR AppName, LPCSTR AppVersn);
	~TLogFileMth();
};
//---------------------------------------------------------------------------
/*
	线程类：详见Testu的测试
	Start：开始运行线程
	Terminate：结束线程，不等待线程真正退出。
	WaitQuit：退出线程，线程退出后该函数才返回。
*/
//---------------------------------------------------------------------------
class TThread
{
private:
	pthread_t FID;
	WORD FPriority;
	bool FTerminated;
private:
	static void* ThreadRun(void* Thread);
	void Run();
	void YQOccurException(TYQException* YQException);
protected:
	virtual void BeforeRun();
	virtual void OnRun() = 0;
	virtual void AfterRun();
	virtual void OnException(TYQException* YQException);
public:
	TThread(WORD Priority = 0);
	virtual ~TThread();

	void Start();
	void Terminate();
	void WaitQuit();
};
//---------------------------------------------------------------------------
#endif
