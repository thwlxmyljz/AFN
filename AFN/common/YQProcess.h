/****************************************************************************
��˾���ƣ����������ſƼ����޹�˾
��Ȩ��������Ȩ���� (C) 2008
�ļ���ʶ�� 
����ʱ�䣺2008-08
��           �ߣ���Ⱥ��
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
	����ͨ�Ż���
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
	����ͬ����
	ͬʱ��������ٽ������д�����������Ľ���ʹ��Create/Destroy
	�Ǵ������Ľ���ʹ��SetSyncSetID�����ⲿ������������ʹ��Create/Destroy
*/
//---------------------------------------------------------------------------
#define SyncCountMax 12
class TYQPrcSync : public TIPC
{
private:
	int FSyncSetID;
	WORD FCriticalCount;
	WORD FReadWriteCount;
	WORD FFlag[SyncCountMax];		//�ź��������־λ
	pthread_mutex_t FMutex;			//�����ź�����־λ
	
	void CheckSyncID(WORD SyncID, WORD Count);
	int SemOpTimout(int SemId, struct sembuf *Sops, unsigned NSops, long Timeout);
public:
	TYQPrcSync(LPCSTR KeyPath);

	void Create(WORD CriticalCount, WORD ReadWriteCount);	//����������
	void Destroy();
	int GetSyncSetID();
	void SetSyncSetID(int SyncSetID, WORD CriticalCount, WORD ReadWriteCount);

	//�ٽ���,TimeoutΪ����,=0��ʾ���޵ȴ�
	void GetCriticalLockInfo(char* Resul, WORD SyncID);	//Resul�ɵ����߷���256�ֽڵĿռ�
	void CriticalLock(WORD SyncID, long Timeout);		//SyncID:0~(CriticalCount-1)
	void CriticalUnlock(WORD SyncID);		//SyncID:0~(CriticalCount-1)
	//��д��,TimeoutΪ����,=0��ʾ���޵ȴ�
	void GetReadWriteLockInfo(char* Resul, WORD SyncID);	//Resul�ɵ����߷���256�ֽڵĿռ�
	void ReadLock(WORD SyncID, long Timeout);	//SyncID:0~(ReadWriteCount-1)
	void ReadUnlock(WORD SyncID);	//SyncID:0~(ReadWriteCount-1)

	void WriteLock(WORD SyncID, long Timeout);	//SyncID:0~(ReadWriteCount-1)
	void WriteUnlock(WORD SyncID);	//SyncID:0~(ReadWriteCount-1)
};
//---------------------------------------------------------------------------
/*
	�����ڴ���
	��д�ڴ�ʱû�м��������ⲿ������ʵ��
	���������ڴ�Ľ���:
		ʹ��Create/Destroy��ʹ��Attach/Deattach
	�Ǵ��������ڴ�Ľ���:
		ʹ��Attach/Deattach��ʹ��Create/Destroy
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
	��Ϣ������
	��д�ڴ�ʱû�м��������ⲿ������ʵ��
	������Ϣ���еĽ���:
		ʹ��Create/Destroy��ʹ��SetMsgQID
	�Ǵ�����Ϣ���еĽ���:
		ʹ��SetMsgQID��ʹ��Create/Destroy
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

	//����ָ��MsgType���͵���Ϣ
	//IsBlock�������пռ䲻��ʱ�Ƿ�����
	//���أ����������Ͷ��пռ䲻��ʱΪ�������׳��쳣��
	bool Send(LPCVOID Msg, long MsgSize, long MsgType, bool IsBlock = true);

	//����ʽ/������ʽ����ָ�����͵���Ϣ
	//MsgType=0:����ȫ�����͵���Ϣ
	//����ʵ���յ����ֽ�������Ϣ����,RetMsgType = NULLʱ����Ҫ������Ϣ����
	long Receive(void* Msg, long MsgSize, long MsgType, long* RetMsgType = NULL, bool IsBlock = true);
};
//---------------------------------------------------------------------------
/*
	���̼�ʹ�ýṹTYQTcpMsgͨ����
	Execute��һ��������ʽ������������Ϣʱ������OnReceive
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
	//MsgStr�������48�ֽڣ�������Ҫʹ��Stream
	void SendMsg(long MsgType, int Flow, int MsgInt1 = 0, int MsgInt2 = 0, DWORD MsgSize = 0, LPCVOID MsgStr = NULL,
			DWORD StreamSize = 0, LPCVOID Stream = NULL);
};
//---------------------------------------------------------------------------
/*
	�������־��
*/
//---------------------------------------------------------------------------
class TLogFileMp : public TLogFile
{
private:
	TYQPrcSync* FYQPrcSync;
	pthread_mutex_t FMutex;			//�߳���
protected:
	virtual void Lock();
	virtual void Unlock();
public:
	TLogFileMp(TYQPrcSync* YQPrcSync, LPCSTR FileName, DWORD MaxLen, LPCSTR AppName, LPCSTR AppVersn);
};
//---------------------------------------------------------------------------
/*
	���̻���
	�ɼ̳е��¼���
		OnStart/OnParentRun/OnException
	�ɹ��ⲿ���õķ�����
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
	virtual void OnParentRun();	/*������ʹ��*/
	virtual void OnException(TYQException* YQException);
	virtual void OnSignal(int Signal);	//��׽���ź�
public:
	TYQProcess();
	virtual ~TYQProcess();

	void Run();
	pid_t GetPid();
	void Wait();	//�����̵ȴ��ӽ����˳�
	void SetSignal(int Signal);	//���ñ���׽���źţ���׽���źź󴥷�OnSignal�¼�
};
//---------------------------------------------------------------------------
/*
	��Ϣ�����ࣺѭ��ִ��OnRun��ֱ���յ��ź�SIGTERM��ֹͣ
	�ɼ̳е��¼���
		BeforeRun/OnRun/AfterRun/OnParentRun/OnException
	�ɹ��ⲿ���õķ�����
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
	void Terminate();	//��������ֹ�ӽ���
};
//---------------------------------------------------------------------------
/*
	�ػ����̣�������
	�̳�������̳�BeforeRun��һ��Ҫ����TYQService::BeforeRun()
	�ɼ̳е��¼���
		BeforeRun/OnRun/AfterRun/OnException
	�ɹ��ⲿ���õķ�����
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

