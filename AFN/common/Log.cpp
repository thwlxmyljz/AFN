#include "Log.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <cstring>
#include "LogFileu.h"

void LOG(LPCSTR msg,LOGCLS Level)
{
	if (LogFile)
	{
		LogFile->WriteLog(msg, Level);
	}
	else
	{
		fprintf(stderr,"[%d] %s\n",Level,msg);
	}	
}
void LOG(LOGCLS Level, const char* format,...)
{	
	char slask[1024]; // temporary for vsprintf / vsnprintf
	va_list ap;

	va_start(ap, format);
#ifdef _WIN32
	vsprintf(slask, format, ap);
#else
	vsnprintf(slask, 1024, format, ap);
#endif
	va_end(ap);

	if (LogFile)
	{
		LogFile->WriteLog(slask, Level);
	}
	else
	{
		fprintf(stderr,"[%d] %s\n",Level,slask);
	}
}
void LOG(LPCSTR File, int Line, LOGCLS LogCls, LPCSTR format, ...)
{	
	if (LogFile)
	{
		LogFile->FmtLog(File, Line,LogCls,format);
	}
	else
	{
		char slask[1024]; // temporary for vsprintf / vsnprintf
		va_list ap;

		va_start(ap, format);
#ifdef _WIN32
		vsprintf(slask, format, ap);
#else
		vsnprintf(slask, 1024, format, ap);
#endif
		va_end(ap);
		fprintf(stderr,"[%d][%s:%d] %s\n",LogCls,File,Line,slask);
	}
}


