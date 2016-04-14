//---------------------------------------------------------------------------
#include "YQCommon.h"
#include "YQUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
//---------------------------------------------------------------------------
/*
	异常类：详见Testu的测试
*/
//---------------------------------------------------------------------------
TYQException::TYQException(LPCSTR Msg)
{
	FMsg = NULL;
	if (Msg && (strlen(Msg)))
	{
		FMsg = (char*)malloc(strlen(Msg) + 1);
		strcpy(FMsg, Msg);
	}
}
//---------------------------------------------------------------------------
TYQException::TYQException(LPCSTR FileName, int LineNO, LPCSTR Msg)
{
	GenErrMsg(FileName, LineNO, Msg);
}
//---------------------------------------------------------------------------
TYQException::TYQException(int Msg)
{
	FMsg = (char*)malloc(32);
	sprintf(FMsg, "Exception, value=%d", Msg);
}
//---------------------------------------------------------------------------
TYQException::TYQException(LPCSTR FileName, int LineNO, int ErrNo)
{
	GenErrMsg(FileName, LineNO, strerror(ErrNo));
}
//---------------------------------------------------------------------------
TYQException::~TYQException()
{
	if (FMsg)
		free(FMsg);
}
//---------------------------------------------------------------------------
void TYQException::GenErrMsg(LPCSTR FileName, int LineNO, LPCSTR Msg)
{
	FMsg = (char*)malloc(strlen(Msg) + 128);
	sprintf(FMsg, "File=%s,Line=%d\r\n%s", TYQUtils::GetFileName(FileName), LineNO, Msg);
}
//---------------------------------------------------------------------------
LPCSTR TYQException::Message()
{
	if (FMsg)
		return FMsg;
	else
	{
		FMsg = (char*)malloc(32);
		strcpy(FMsg, "YQException:No message!");
		return FMsg;
	}
}
//---------------------------------------------------------------------------

