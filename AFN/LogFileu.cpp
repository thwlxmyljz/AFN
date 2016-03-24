//---------------------------------------------------------------------------
#include "stdafx.h"
#include "LogFileu.h"
#include <time.h>

#pragma warning(disable:4996)
//---------------------------------------------------------------------------
TLogFile* LogFile = NULL;
TLogFile* TLogFile::ThisPtr = NULL;
//---------------------------------------------------------------------------
TLogFile::TLogFile(LPCSTR FileName, DWORD MaxLen, LPCSTR AppName, LPCSTR AppVersn)
{
	LogFile = this;
	FFileName = (char*)malloc(strlen(FileName) + 1);
	strcpy(FFileName, FileName);
	//FHandle = open(FFileName, O_RDWR|O_CREAT, 0666);
	FMaxLen = MaxLen;
	strcpy(FVersion, AppVersn);
	strcpy(FModule, AppName);
	strcpy(FLogCls, "Info");
	strcpy(FLogCls + 8, "Minor");
	strcpy(FLogCls + 16, "Major");
	strcpy(FLogCls + 24, "Critical");
	FWriteLog = NULL;
	ThisPtr = this;
	FOupLogCls = 0xFFFF;	//默认所有级别的日志都输出
}
//---------------------------------------------------------------------------
TLogFile::TLogFile(PFN_WRITELOG WriteLogFunc, LPCSTR DllName, LPCSTR DllVersn)
{
	FFileName = NULL;
	FHandle = 0;
	FWriteLog = WriteLogFunc;
	strcpy(FVersion, DllVersn);
	strcpy(FModule, DllName);
}
//---------------------------------------------------------------------------
TLogFile::~TLogFile()
{
	if (FFileName)
		free(FFileName);
//	if (FHandle != -1)
//		close(FHandle);
}
//---------------------------------------------------------------------------
void TLogFile::SetLogClsOutput(LOGCLS LogCls, bool Output)
{
	WORD Buf = 1;
	Buf <<= LogCls;
	if (Output)
		FOupLogCls |= Buf;
	else
		FOupLogCls &= (~Buf);
}
//---------------------------------------------------------------------------
void TLogFile::Log(LPCSTR DllName, LPCSTR DllVersion, int LogCls, LPCSTR Log)
{
#ifdef _DEBUG
	static const char* LogLevelString[4] = {"Info","Minor","Major","Critical"};
	fprintf(stderr,"[%s(%s)] %s:%s\n",DllName,DllVersion,(LogCls>=0&&LogCls<4)?LogLevelString[LogCls]:"",Log);
#endif
	if (FWriteLog)
		FWriteLog(DllName, DllVersion, LogCls, Log);
	else if (FOupLogCls & ( 1 << LogCls))
	{
		//去掉Log尾部的回车换行
		/*
		if (strlen(Log) > 1)
		{
			char* LogPtr = (char*)Log + strlen(Log) - 2;
			for (char I = 0; I < 2; I++)
			{
				if ((LogPtr[I] == '\r') || (LogPtr[I] == '\n'))
					LogPtr[I] = '\0';
			}
		}
		*/
		char* Inf = (char*)malloc(strlen(Log) + 512);
		try
		{
			time_t timep;
			struct tm *p;
			time(&timep);
			p = localtime(&timep);
			sprintf(Inf, "\n+++ %s %s %s %d-%02d-%02d %02d:%02d:%02d +++\n%s\n--- Log end ---\n",
				FLogCls + (LogCls * 8), DllName, DllVersion,
				1900+p->tm_year, 1+p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, Log);
			Write(Inf, strlen(Inf));

			free(Inf);
		}
		catch(...)
		{
			free(Inf);
		}
	}	
}
//---------------------------------------------------------------------------
void TLogFile::FmtLog(LPCSTR File, int Line, LOGCLS LogCls, LPCSTR FmtStr, ...)
{
	char Buf0[1024], Buf1[1024];

	va_list argptr;
	va_start(argptr, FmtStr);
   	int ret = vsnprintf(Buf0, 800, FmtStr, argptr);
	va_end(argptr);
	
	if(ret < 0)
	{
		Buf0[800] = '.';
		Buf0[801] = '.';
		Buf0[802] = '.';
		Buf0[803] = 0;	
	}

	sprintf(Buf1, "File=%s, Line=%d\n%s", File, Line, Buf0);

	Log(FModule, FVersion, LogCls, Buf1);
}
//---------------------------------------------------------------------------
void TLogFile::FmtLog(LOGCLS LogCls, LPCSTR FmtStr, ...)
{
	char Buf0[1024];

	va_list argptr;
	va_start(argptr, FmtStr);
   	int ret = vsnprintf(Buf0, 800, FmtStr, argptr);
	va_end(argptr);
	
	if(ret < 0)
	{
		Buf0[800] = '.';
		Buf0[801] = '.';
		Buf0[802] = '.';
		Buf0[803] = 0;	
	}

	Log(FModule, FVersion, LogCls, Buf0);
}
//---------------------------------------------------------------------------
void TLogFile::WriteLog(LPCSTR Msg, LOGCLS LogCls)
{
	Log(FModule, FVersion, LogCls, Msg);
}
//---------------------------------------------------------------------------
void TLogFile::Write(LPCSTR Cont, DWORD ContLen)
{
	if (FWriteLog || (ContLen > FMaxLen))
		return;

	Lock();
	char DestFile[128];
	FILE *fp = NULL;
	try
	{		
		fp = fopen(FFileName, "a+");
		if(fp == NULL)
		{
			Unlock();
			return;
		}
		fseek(fp, 0 ,SEEK_END);
		DWORD FileLen = ftell(fp);
		while (FMaxLen < FileLen + ContLen) //剩余空间不足
		{
			fclose(fp);
			time_t timep;
			struct tm *p;
			time(&timep);
			p = localtime(&timep);
			sprintf(DestFile, "%s.%02d%02d_%02d%02d%02d",
				FFileName,
				1+p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
			
			rename(FFileName, DestFile);
			fp = fopen(FFileName, "a+");
			if(fp == NULL)
			{
				Unlock();
				return;
			}
			FileLen = ftell(fp);
		}
		fwrite(Cont, ContLen, 1, fp);
		fclose(fp);
		Unlock();
	}
	catch(...)
	{
		Unlock();
	}
}
//---------------------------------------------------------------------------
void TLogFile::Lock()
{
}
//---------------------------------------------------------------------------
void TLogFile::Unlock()
{
}
//---------------------------------------------------------------------------
void TLogFile::CallBack(LPCSTR DllName, LPCSTR DllVersion, int LogLevel, LPCSTR LogText)
{
	if (ThisPtr)
		ThisPtr->Log(DllName, DllVersion, LogLevel, LogText);
}
//---------------------------------------------------------------------------

