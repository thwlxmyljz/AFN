#ifndef LOG_H
#define LOG_H
#include "YQCommon.h"
#include "YQErrCode.h"

//��־��¼
void LOG(LPCSTR msg,LOGCLS Level);
void LOG(LOGCLS Level,const char* format,...);
void LOG(LPCSTR File, int Line, LOGCLS LogCls, LPCSTR format, ...);

#endif

