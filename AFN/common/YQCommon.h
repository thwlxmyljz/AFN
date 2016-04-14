//---------------------------------------------------------------------------
#ifndef YQCommon_h
#define YQCommon_h
#include <errno.h>
//---------------------------------------------------------------------------
#include "YQDefs.h"

#define YQSysErr (new TYQException(__FILE__, __LINE__, errno))
#define YQErr(ErrMsg)	(new TYQException(__FILE__, __LINE__, ErrMsg)) 

//---------------------------------------------------------------------------
/*
	异常类
	使用方法：
	try
	{
		...
		throw (new TYQException(...));
		或者使用宏抛出系统错误
		throw YQSysErr;
		使用使用宏抛出其它错误
		throw YQErr("Error message");
		...
	}
	catch(TYQException *E)
	{
		处理E->Message();
		delete E;
	}
*/
//---------------------------------------------------------------------------
class TYQException
{
private:
	char* FMsg;
	void GenErrMsg(LPCSTR FileName, int LineNO, LPCSTR Msg);
public:
	/* 传入字符串异常*/
	TYQException(LPCSTR Msg);
	/* 传入字符串异常*/
	TYQException(LPCSTR FileName, int LineNO, LPCSTR Msg);
	/* 传入整型类异常*/
	TYQException(int Msg);
	/* 传入带文件位置的系统异常*/
	TYQException(LPCSTR FileName, int LineNO, int ErrNo);
	
	~TYQException();
	
	LPCSTR Message();
};
//---------------------------------------------------------------------------
#endif
