//---------------------------------------------------------------------------
#include "Threadu.h"
#include "LogFileu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//---------------------------------------------------------------------------
/*
	多线程互斥锁类
*/
//---------------------------------------------------------------------------
TCriticalSection::TCriticalSection()
{
	int MutexAttrType = PTHREAD_MUTEX_RECURSIVE;
	pthread_mutexattr_t MutexAttr;
	FRetVal = pthread_mutexattr_gettype(&MutexAttr, &MutexAttrType);
	if (FRetVal != 0)
		return;

	FRetVal = pthread_mutexattr_settype(&MutexAttr, PTHREAD_MUTEX_RECURSIVE);
	if (FRetVal != 0)
		return;

	FRetVal = pthread_mutex_init(&FMutex, &MutexAttr);
}
//---------------------------------------------------------------------------
bool TCriticalSection::IsCreateOK()
{
	return (FRetVal == 0);
}
//---------------------------------------------------------------------------
void TCriticalSection::Enter()
{
	pthread_mutex_lock(&FMutex);
}
//---------------------------------------------------------------------------
void TCriticalSection::Leave()
{
	pthread_mutex_unlock(&FMutex);
}
//---------------------------------------------------------------------------
/*
	多线程读写锁类
*/
//---------------------------------------------------------------------------
TReadWriteLock::TReadWriteLock()
{
}
//---------------------------------------------------------------------------
void TReadWriteLock::Create()
{
	int Resul;
	//使用多处理器初始化锁属性
	Resul = pthread_rwlockattr_init(&FAttr);
	if (Resul != 0)
		throw (new TYQException(__FILE__, __LINE__, Resul));

	int PShared;
	Resul = pthread_rwlockattr_getpshared(&FAttr, &PShared);
	if (Resul != 0)
	{
		pthread_rwlockattr_destroy(&FAttr);
		throw (new TYQException(__FILE__, __LINE__, Resul));
	}

	PShared = PTHREAD_PROCESS_SHARED;
	Resul = pthread_rwlockattr_setpshared(&FAttr, PShared);
	if (Resul != 0)
	{
		pthread_rwlockattr_destroy(&FAttr);
		throw (new TYQException(__FILE__, __LINE__, Resul));
	}
	//初始化锁
	Resul = pthread_rwlock_init(&FRWLock, &FAttr);
	if (Resul != 0)
	{
		pthread_rwlockattr_destroy(&FAttr);
		throw (new TYQException(__FILE__, __LINE__, Resul));
	}
}
//---------------------------------------------------------------------------
void TReadWriteLock::Destroy()
{
	pthread_rwlock_destroy(&FRWLock);
	pthread_rwlockattr_destroy(&FAttr);
}
//---------------------------------------------------------------------------
void TReadWriteLock::ReadLock(long Timeout)
{
	if (Timeout == 0)
		pthread_rwlock_rdlock(&FRWLock);
	else
	{
		struct timespec Tm;
		Tm.tv_sec = Timeout / 1000;	//秒
		Tm.tv_nsec = (Timeout % 1000) * 1000;	//纳秒
		pthread_rwlock_timedrdlock(&FRWLock, &Tm);
	}
}
//---------------------------------------------------------------------------
void TReadWriteLock::ReadUnlock()
{
	pthread_rwlock_unlock(&FRWLock);
}
//---------------------------------------------------------------------------
void TReadWriteLock::WriteLock(long Timeout)
{
	if (Timeout == 0)
		pthread_rwlock_wrlock(&FRWLock);
	else
	{
		struct timespec Tm;
		Tm.tv_sec = Timeout / 1000;	//秒
		Tm.tv_nsec = (Timeout % 1000) * 1000;	//纳秒
		pthread_rwlock_timedwrlock(&FRWLock, &Tm);
	}
}
//---------------------------------------------------------------------------
void TReadWriteLock::WriteUnlock()
{
	pthread_rwlock_unlock(&FRWLock);
}
//---------------------------------------------------------------------------
/*
	多线程日志类
*/
//---------------------------------------------------------------------------
TLogFileMth::TLogFileMth(LPCSTR FileName, DWORD MaxLen, LPCSTR AppName, LPCSTR AppVersn)
		: TLogFile(FileName, MaxLen, AppName, AppVersn)
{
	FCriticalSection = new TCriticalSection();
}
//---------------------------------------------------------------------------
TLogFileMth::~TLogFileMth()
{
	delete FCriticalSection;
}
//---------------------------------------------------------------------------
void TLogFileMth::Lock()
{
	FCriticalSection->Enter();
}
//---------------------------------------------------------------------------
void TLogFileMth::Unlock()
{
	FCriticalSection->Leave();
}
//---------------------------------------------------------------------------
/*
	线程类：详见Testu的测试
*/
//---------------------------------------------------------------------------
TThread::TThread(WORD Priority)
{
	FTerminated = false;
	FPriority = Priority;
}
//---------------------------------------------------------------------------
TThread::~TThread()
{
}
//---------------------------------------------------------------------------
void TThread::Start()
{
	pthread_attr_t Attr, *pAttr;
	pAttr = &Attr;
	struct sched_param AttrParam;
	pthread_attr_init(pAttr);
	if (pthread_attr_getschedparam(pAttr, &AttrParam) != 0)
		pAttr = NULL;
	else
	{
		AttrParam.sched_priority = FPriority;
		if (pthread_attr_setschedparam(pAttr, &AttrParam) != 0)
			pAttr = NULL;
	}
	if (pthread_create(&FID, pAttr, ThreadRun, (void*)this) != 0)
		throw YQSysErr;	//将系统错误抛出
}
//---------------------------------------------------------------------------
void* TThread::ThreadRun(void* Thread)
{
	((TThread*)Thread)->Run();
	return NULL;
}
//---------------------------------------------------------------------------
void TThread::Run()
{
	try
	{
		BeforeRun();
		while (!FTerminated)
		{
			try
			{
				OnRun();
				usleep(10000);
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
void TThread::YQOccurException(TYQException* YQException)
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
void TThread::OnException(TYQException* YQException)
{
	if (LogFile == NULL)
		return;
	if (YQException)
		LogFile->WriteLog(YQException->Message(), LOG_MAJOR);
	else
		LogFile->WriteLog("Unknown exception", LOG_MAJOR);
}
//---------------------------------------------------------------------------
void TThread::BeforeRun()
{
}
//---------------------------------------------------------------------------
void TThread::AfterRun()
{
}
//---------------------------------------------------------------------------
void TThread::Terminate()
{
	FTerminated = true;
}
//---------------------------------------------------------------------------
void TThread::WaitQuit()
{
	if (!FTerminated)
		FTerminated = true;
	pthread_join(FID, NULL);
}
//---------------------------------------------------------------------------

