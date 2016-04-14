/****************************************************************************
��˾���ƣ����������ſƼ����޹�˾
��Ȩ��������Ȩ���� (C) 2008
��Ԫ���ƣ�Threadu.h
�ļ���ʶ�� 
����ʱ�䣺2008-08-06
��    �ߣ���Ⱥ��
��    ����V1.0
ժ    Ҫ��
��ǰ�汾��
��  ���ߣ�
�޸����ڣ�
�޸�ժҪ��
****************************************************************************/
//---------------------------------------------------------------------------
#ifndef ThreaduH
#define ThreaduH
#include "YQCommon.h"
#include "LogFileu.h"
#include <pthread.h>
//---------------------------------------------------------------------------
/*
	���̻߳�������
*/
class TCriticalSection
{
private:
	pthread_mutex_t FMutex;
	int FRetVal;
public:
	TCriticalSection();
	bool IsCreateOK();	//���س�ʼ�����

	void Enter();
	void Leave();
};
//---------------------------------------------------------------------------
/*
	���̶߳�д����
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

	//��д��,TimeoutΪ����,=0��ʾ���޵ȴ�
	void ReadLock(long Timeout);
	void ReadUnlock();

	void WriteLock(long Timeout);
	void WriteUnlock();
};
//---------------------------------------------------------------------------
/*
	���߳���־��
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
	�߳��ࣺ���Testu�Ĳ���
	Start����ʼ�����߳�
	Terminate�������̣߳����ȴ��߳������˳���
	WaitQuit���˳��̣߳��߳��˳���ú����ŷ��ء�
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
