#include "YQUtils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/timeb.h>
#include "LogFileu.h"
#ifdef _WIN32
#pragma warning(disable:4996)
#endif
//下面两个模板函数可以是CHAR或WCHAR(UNICODE)类型
//Str1开始位置是否包含Str2
template<class T>
bool  _YQStrFPosi(T Str1, T Str2)
{
	while((*Str1 != 0) && (*Str2 != 0))
    {
    	if (*Str1++ != *Str2++)
        	return false;
    }
    return (*Str2 == 0);
}
//Str1任意位置是否包含Str2
template<class T>
bool  _YQStrPosi(T Str1, T Str2)
{
	while(*Str1 != 0)
    {
    	if (_YQStrFPosi(Str1++, Str2))
        	return true;
    }
    return false;
}

//---------------------------------------------------------------------------
LPCSTR TYQUtils::GetFileName(LPCSTR Path_File)
{
	LPCSTR Ptr = Path_File + strlen(Path_File);
	while ((*Ptr != '/') && ((int)Ptr != (int)Path_File - 1))
		Ptr--;
	return ++Ptr;
}
//---------------------------------------------------------------------------
char* TYQUtils::GetFilePath(LPCSTR Path_File)
{
	char* Buffer = (char*)malloc(strlen(Path_File) + 1);
	if (Buffer)
	{
		strcpy(Buffer, Path_File);
		char* Ptr = (char*)GetFileName(Buffer);
		if ((int)Ptr == (int)Buffer)
			*Ptr = '\0';
		else
			*(--Ptr) = '\0';
	}
	return Buffer;
}
//---------------------------------------------------------------------------
void TYQUtils::TimeStart(DWORD& StartTime)
{
	struct timeb tp;
	ftime(&tp);
	StartTime = ((DWORD)(tp.time) * 1000) + ((DWORD)(tp.millitm) % 1000);
}
//---------------------------------------------------------------------------
DWORD TYQUtils::TimeElapse(const DWORD& StartTime)
{
    DWORD MaxVal = 0;
	DWORD Tick   = 0;
	DWORD RetVal = 0;

    MaxVal = ~MaxVal;
    TimeStart(Tick);
    if (Tick < StartTime)
        RetVal = (MaxVal - StartTime) + 1 + Tick;
    else
        RetVal = Tick - StartTime;
    return RetVal;
}
// 替换字符串中特征字符串为指定字符串
void TYQUtils::ReplaceStr(char *sSrc, LPCSTR sMatchStr,LPCSTR sReplaceStr)
{
    int  StringLen;
    char caNewString[256];

	if((NULL == sSrc)||(NULL == sMatchStr)||(NULL == sReplaceStr))
		return;

    char *FindPos = strstr(sSrc, sMatchStr);
    if( (!FindPos) || (!sMatchStr) )
         return;

    while( FindPos )
    {
        memset(caNewString, 0,256);
        StringLen = FindPos - sSrc;
        strncpy(caNewString, sSrc, StringLen);
        strcat(caNewString, sReplaceStr);
        strcat(caNewString, FindPos + strlen(sMatchStr));
        strcpy(sSrc, caNewString);

        FindPos = strstr(sSrc, sMatchStr);
    }
}
char* TYQUtils::ReplaceChar(char * src,char oldChar,char newChar)
{
	char * head = src;
	while(*src!='\0'){
	if(*src==oldChar) *src=newChar;
		src++;
	}
	return head;
}
/******************************************************************************
* 函 数 名：DeleteChar
* 作    用：删除字符串中某个字符
* 输入参数：Src -源串 -将删除的串
* 输出参数：操作后的字符
*******************************************************************************/
void TYQUtils::DeleteChar(char * Src,char DelChar)
{
	char   Dst[128];
	memset(Dst,0,128);

	if(Src == NULL)
		return;
	int I = 0;
	for(unsigned int J = 0; J < strlen(Src); J++)
	{
		if(Src[J] != DelChar)
		{
			Dst[I] = Src[J];
			I++;
		}
	}
	Dst[I] = '\0';
	strcpy(Src,Dst);
}
//---------------------------------------------------------------------------
BOOL TYQUtils::ReadBit(const int& Var, BYTE Pos)
{
    int RetVal = 1;
    RetVal <<= Pos;
    return (Var & RetVal);
}
//---------------------------------------------------------------------------
void TYQUtils::SetBit(int& Var, BYTE Pos, BOOL Val)
{
    Var = WriteBit(Var, Pos, Val);
}
void  TYQUtils::SetBit(DWORD& Var, BYTE Pos, BOOL Val)
{
	Var = WriteBit(Var, Pos, Val);
}
//---------------------------------------------------------------------------
int TYQUtils::WriteBit(const int& Var, BYTE Pos, BOOL Val)
{
    int RetVal = 1;
    RetVal <<= Pos;
    if (Val)
        RetVal |= Var;
    else
        RetVal = Var & (~RetVal);
    return RetVal;
}

BOOL TYQUtils::StrEqu(LPCSTR S1, LPCSTR S2)
{
    while((*S1 != 0) && (*S2 != 0))
    {
        if (*S1++ != *S2++)
            return FALSE;
    }
    return *S1 == *S2;
}
char *TYQUtils::Trim(char *str)
{   
	if(NULL == str)
		return NULL;

	if(*str == '\0')
		return str;
	char   *it;   
	while(*str ==' ' && *str)
		++str;   
	it = str;   
	while(*it != ' ' && *it)
	{
		++it;   
	}
	*it='\0';   
	return str;   
}  
/******************************************************************************
* 函 数 名：StrRightCmp
* 作    用：LongStr右边包含ShortStr返回真
* 输入参数：LongStr-字符串 ShortStr-字符
* 输出参数：
*******************************************************************************/
bool TYQUtils::StrRightCmp(LPCSTR LongStr, LPCSTR ShortStr)
{
	LPCSTR LongStrPtr = LongStr;
	LPCSTR ShortStrPtr = ShortStr;
	while(*LongStrPtr != '\0')
		LongStrPtr++;
    while(*ShortStrPtr != '\0')
		ShortStrPtr++;
	while(*LongStrPtr == *ShortStrPtr)
	{
		if (ShortStrPtr == ShortStr)
			return true;
		if (LongStrPtr == LongStr)
			break;
		LongStrPtr--;
		ShortStrPtr--;
	}
	return false;
}
/******************************************************************************
* 函 数 名：Left
* 作    用：在一个字符串中取某个字符前字符串
* 输入参数：str-字符串 c-字符
* 输出参数：
*******************************************************************************/
char *TYQUtils::Left(char *str,char c)
{
	if(str == NULL)
		return NULL;
	char* BufPtr = str;
	while ((*BufPtr != '\0') &&(*BufPtr != c))
	{
		BufPtr++;
	}
	*BufPtr = '\0';
	return str;
}
/******************************************************************************
* 函 数 名：LeftTrimStr
* 作    用：截掉字符符串左边几个字符
* 输入参数：Called-字符串 DelCount-删除个数
* 输出参数：无
*******************************************************************************/
void TYQUtils::LeftTrimStr(char* Called, int DelCount)
{
	if (strlen(Called) < (unsigned int)DelCount)
		Called[0] = '\0';
	else
	{
		char* BufPtr = Called + DelCount;
	    while (*BufPtr != 0)
        	*Called++ = *BufPtr++;
    	*Called = '\0';
    }

	
}
/******************************************************************************
* 函 数 名：LeftAddStr
* 作    用：在字符串后面加入字符
* 输入参数：Called-字符串 AddStr-要加的字符
* 输出参数：无
*******************************************************************************/
void TYQUtils::LeftAddStr(char* Called, LPCSTR AddStr)
{
	char Buf[64];
	strcpy(Buf, Called);
	strcpy(Called, AddStr);
	strcat(Called, Buf);
}

//Str1开始位置是否包含Str2
//bool __fastcall TYQThread::YQStrFPosi(LPCWSTR Str1, LPCWSTR Str2)
//{
//	return _YQStrFPosi(Str1, Str2);  //YQThreadu.cpp的模板函数
//}
//---------------------------------------------------------------------------
BOOL  TYQUtils::YQStrFPosi(LPCSTR Str1, LPCSTR Str2)
{
	return _YQStrFPosi(Str1, Str2);  //YQThreadu.cpp的模板函数
}
//---------------------------------------------------------------------------
//Str1任意位置是否包含Str2
//bool __fastcall TYQThread::YQStrPosi(LPCWSTR Str1, LPCWSTR Str2)
//{
//	return _YQStrPosi(Str1, Str2);  //YQThreadu.cpp的模板函数
//}
//---------------------------------------------------------------------------
BOOL  TYQUtils::YQStrPosi(LPCSTR Str1, LPCSTR Str2)
{
	return _YQStrPosi(Str1, Str2); //YQThreadu.cpp的模板函数
}

BOOL TYQUtils::FileExists(const char *filename)
{
	return TRUE;
}
const char*TYQUtils::GetDataTimeStamp(char *buffer)
{
	if(NULL == buffer)
		return NULL;

	time_t t = time(0);
	struct tm *curtime = localtime(&t);
	sprintf(buffer, 
			"%04d-%02d-%02d %02d:%02d:%02d",
			curtime->tm_year + 1900,
			curtime->tm_mon + 1,
			curtime->tm_mday,
			curtime->tm_hour,
			curtime->tm_min,
			curtime->tm_sec);

	return buffer;
}
const char*TYQUtils::GetDataStamp(char *buffer)
{
	if(NULL == buffer)
		return NULL;

	time_t t = time(0);
	struct tm *curtime = localtime(&t);
	sprintf(buffer, 
			"%04d-%02d-%02d %02d:%02d:%02d",
			curtime->tm_year + 1900,
			curtime->tm_mon + 1,
			curtime->tm_mday,
			curtime->tm_hour,
			curtime->tm_min,
			curtime->tm_sec
			);

	return buffer;
}
WORD TYQUtils::IsAllNumber(LPCSTR Str)
{
    WORD RetVal = 0;
    while(Str[RetVal] != '\0')
    {
        if (('0' <= Str[RetVal]) && (Str[RetVal] <= '9'))
            RetVal++;
        else
            return 0;
    }
    return RetVal;
}
BOOL TYQUtils::IsSameNet(LPCSTR NetMask, LPCSTR CallerRegIP, LPCSTR CalledRegIP)
{
#ifdef _WIN32
	unsigned long iNetMask, iCallerRegIP, iCalledRegIP;
	iNetMask = inet_addr(NetMask);
	iCallerRegIP = inet_addr(CallerRegIP);
	iCalledRegIP = inet_addr(CalledRegIP);
	return ((iCallerRegIP & iNetMask) == (iCalledRegIP & iNetMask));
#else
	return FALSE;
#endif
}
BOOL TYQUtils::IsSameNet(LPCSTR CallerAddr,unsigned long  CallerMask,
					LPCSTR CalledAddr, unsigned long CalledMask)
{
#ifdef _WIN32
	DWORD CallerIP  = inet_addr(CallerAddr);
	DWORD CalledIP  = inet_addr(CalledAddr);

	return((CallerIP & CallerMask) == (CalledIP & CalledMask));
#else
	return FALSE;
#endif
}
string TYQUtils::Byte2Hex(const void* data,unsigned int len)
{
	static char ss[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	string str;
	unsigned char* p = (unsigned char*)data;
	while(len-- > 0){		
		str += ss[(*p&0xf0)>>4];
		str += ss[*p&0x0f];
		str += " ";
		p++;
	}
	return str;
}