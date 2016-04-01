//---------------------------------------------------------------------------
#ifndef YQErrCodeH
#define YQErrCodeH

//---------------------------------------------------------------------------
#ifndef _LOGMSG
#define _LOGMSG

#include "YQDefs.h"

// 声明日志输出函数
typedef void (WINAPI * PFN_WRITELOG)(const char* DllName, const char *DllVersion, int LogLevel, const char *LogText);
// 日志信息级别
enum LOGCLS
{
   LOG_INFORMATION	= 0,
    LOG_MINOR		= 1,
    LOG_MAJOR		= 2, 
    LOG_CRITICAL	= 3
};
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//错误代码，以YQER_开头
#define YQER_OK					0					//成功或正常
//---------------------------------------------------------------------------
#define YQER_JZQ_NOLOGIN  -96 //找不到集中器的网络连接，或者未登录
#define YQER_JZQ_NOTFOUND -97 //找不到集中器
#define YQER_PARAMERR  -98 //参数错误
#define YQER_NOMEMORY  -99 //内存错误
//服务器错误
#define YQER_SVR_Base				-100
#define YQER_SVR_Err(x)			(YQER_SVR_Base - (x))
//连接错误
#define YQER_CON_Base				-200
#define YQER_CON_Err(x)			(YQER_CON_Base - (x))
//解包错误
#define YQER_PKG_Base               -300
#define YQER_PKG_Err(x)			(YQER_PKG_Base - (x))
#endif

