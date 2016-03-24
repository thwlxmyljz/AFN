//---------------------------------------------------------------------------
#ifndef LogFileuH
#define LogFileuH
//---------------------------------------------------------------------------
//#include "YQCommon.h"
#include "YQErrCode.h"
//---------------------------------------------------------------------------
#ifndef _LOGMSG
#define _LOGMSG
// ������־�������
typedef void (* PFN_WRITELOG)(LPCSTR DllName, LPCSTR DllVersion, int LogLevel, LPCSTR LogText);
#endif

#define YQLogInfo(ErrMsg) LogFile->FmtLog(__FILE__,__LINE__,LOG_INFORMATION,ErrMsg)
#define YQLogMin(ErrMsg) LogFile->FmtLog(__FILE__,__LINE__,LOG_MINOR,ErrMsg)
#define YQLogMaj(ErrMsg) LogFile->FmtLog(__FILE__,__LINE__,LOG_MAJOR,ErrMsg)
//---------------------------------------------------------------------------
/*
	��־�ࣺ
		�ڶ�̬����ʹ��TLogFile(PFN_WRITELOG WriteLogFunc, LPCSTR DllName, LPCSTR DllVersn);����
		��Ӧ�ó���ʹ�ã�
		1��TLogFile(LPCSTR FileName, DWORD MaxLen, LPCSTR AppName, LPCSTR AppVersn);
		2����Ӧ�ó�����̻߳�����ʹ��ʱ��Ҫ�̳�Lock/Unlock������
		3������̬����TLogFile::CallBack��ΪDLL��OPEN������DLLд��־��
*/
class TLogFile
{
public:
	// ��־��Ϣ����
	/*enum LOGCLS
	{
		LOG_INFO2		= 1,
		LOG_MINOR		= 2,
		LOG_MAJOR		= 3,
		LOG_CRITICAL	= 4
	};*/
private:
	int FHandle;
	WORD FOupLogCls;
	DWORD FMaxLen;
	char* FFileName;
	char FVersion[16];
	char FLogCls[36];
	char FModule[32];
	PFN_WRITELOG FWriteLog;
	static TLogFile* ThisPtr;
	void Write(LPCSTR Cont, DWORD ContLen);
protected:
	virtual void Lock();
	virtual void Unlock();
public:
	//��APP����
	TLogFile(LPCSTR FileName, DWORD MaxLen, LPCSTR AppName, LPCSTR AppVersn);
	//��DLL����
	TLogFile(PFN_WRITELOG WriteLogFunc, LPCSTR DllName, LPCSTR DllVersn);
	virtual ~TLogFile();

	//����ָ���������־д�뵽��־�ļ�,Ĭ��ȫ���������־��д�뵽��־�ļ�
	void SetLogClsOutput(LOGCLS LogCls, bool Output);

	 //��ʽ����־����
	void Log(LPCSTR DllName, LPCSTR DllVersion, int LogCls, LPCSTR Log);
	void FmtLog(LPCSTR File, int Line, LOGCLS LogCls, LPCSTR FmtStr, ...);
	void FmtLog(LOGCLS LogCls, LPCSTR FmtStr, ...);
	void WriteLog(LPCSTR Msg, LOGCLS LogCls = LOG_INFORMATION);
	
	//��APP�й���ʼ��DLLʱ����Ϊ�ص��������������
	static void WINAPI CallBack(LPCSTR DllName, LPCSTR DllVersion, int LogLevel, LPCSTR LogText);
};
extern TLogFile* LogFile;
//---------------------------------------------------------------------------
#endif
