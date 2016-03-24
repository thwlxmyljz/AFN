//---------------------------------------------------------------------------
#ifndef LogFileuH
#define LogFileuH
//---------------------------------------------------------------------------
//#include "YQCommon.h"
#include "YQErrCode.h"
//---------------------------------------------------------------------------
#ifndef _LOGMSG
#define _LOGMSG
// 声明日志输出函数
typedef void (* PFN_WRITELOG)(LPCSTR DllName, LPCSTR DllVersion, int LogLevel, LPCSTR LogText);
#endif

#define YQLogInfo(ErrMsg) LogFile->FmtLog(__FILE__,__LINE__,LOG_INFORMATION,ErrMsg)
#define YQLogMin(ErrMsg) LogFile->FmtLog(__FILE__,__LINE__,LOG_MINOR,ErrMsg)
#define YQLogMaj(ErrMsg) LogFile->FmtLog(__FILE__,__LINE__,LOG_MAJOR,ErrMsg)
//---------------------------------------------------------------------------
/*
	日志类：
		在动态库中使用TLogFile(PFN_WRITELOG WriteLogFunc, LPCSTR DllName, LPCSTR DllVersn);构造
		在应用程序使用：
		1：TLogFile(LPCSTR FileName, DWORD MaxLen, LPCSTR AppName, LPCSTR AppVersn);
		2：在应用程序多线程或多进程使用时需要继承Lock/Unlock函数。
		3：将静态函数TLogFile::CallBack作为DLL的OPEN参数供DLL写日志。
*/
class TLogFile
{
public:
	// 日志信息级别
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
	//供APP构造
	TLogFile(LPCSTR FileName, DWORD MaxLen, LPCSTR AppName, LPCSTR AppVersn);
	//供DLL构造
	TLogFile(PFN_WRITELOG WriteLogFunc, LPCSTR DllName, LPCSTR DllVersn);
	virtual ~TLogFile();

	//设置指定级别的日志写入到日志文件,默认全部级别的日志都写入到日志文件
	void SetLogClsOutput(LOGCLS LogCls, bool Output);

	 //格式化日志调用
	void Log(LPCSTR DllName, LPCSTR DllVersion, int LogCls, LPCSTR Log);
	void FmtLog(LPCSTR File, int Line, LOGCLS LogCls, LPCSTR FmtStr, ...);
	void FmtLog(LOGCLS LogCls, LPCSTR FmtStr, ...);
	void WriteLog(LPCSTR Msg, LOGCLS LogCls = LOG_INFORMATION);
	
	//在APP中供初始化DLL时，作为回调函数的输入参数
	static void WINAPI CallBack(LPCSTR DllName, LPCSTR DllVersion, int LogLevel, LPCSTR LogText);
};
extern TLogFile* LogFile;
//---------------------------------------------------------------------------
#endif
