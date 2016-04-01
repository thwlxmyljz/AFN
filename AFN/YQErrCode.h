//---------------------------------------------------------------------------
#ifndef YQErrCodeH
#define YQErrCodeH

//---------------------------------------------------------------------------
#ifndef _LOGMSG
#define _LOGMSG

#include "YQDefs.h"

// ������־�������
typedef void (WINAPI * PFN_WRITELOG)(const char* DllName, const char *DllVersion, int LogLevel, const char *LogText);
// ��־��Ϣ����
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
//������룬��YQER_��ͷ
#define YQER_OK					0					//�ɹ�������
//---------------------------------------------------------------------------
#define YQER_JZQ_NOLOGIN  -96 //�Ҳ������������������ӣ�����δ��¼
#define YQER_JZQ_NOTFOUND -97 //�Ҳ���������
#define YQER_PARAMERR  -98 //��������
#define YQER_NOMEMORY  -99 //�ڴ����
//����������
#define YQER_SVR_Base				-100
#define YQER_SVR_Err(x)			(YQER_SVR_Base - (x))
//���Ӵ���
#define YQER_CON_Base				-200
#define YQER_CON_Err(x)			(YQER_CON_Base - (x))
//�������
#define YQER_PKG_Base               -300
#define YQER_PKG_Err(x)			(YQER_PKG_Base - (x))
#endif

