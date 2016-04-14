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
	�쳣��
	ʹ�÷�����
	try
	{
		...
		throw (new TYQException(...));
		����ʹ�ú��׳�ϵͳ����
		throw YQSysErr;
		ʹ��ʹ�ú��׳���������
		throw YQErr("Error message");
		...
	}
	catch(TYQException *E)
	{
		����E->Message();
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
	/* �����ַ����쳣*/
	TYQException(LPCSTR Msg);
	/* �����ַ����쳣*/
	TYQException(LPCSTR FileName, int LineNO, LPCSTR Msg);
	/* �����������쳣*/
	TYQException(int Msg);
	/* ������ļ�λ�õ�ϵͳ�쳣*/
	TYQException(LPCSTR FileName, int LineNO, int ErrNo);
	
	~TYQException();
	
	LPCSTR Message();
};
//---------------------------------------------------------------------------
#endif
