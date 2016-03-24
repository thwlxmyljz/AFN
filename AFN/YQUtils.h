//---------------------------------------------------------------------------
#ifndef YQUtils_h
#define YQUtils_h

#include "YQDefs.h"
//---------------------------------------------------------------------------
class TYQUtils
{
public:
	/*文件操作*/

	//从带路径的文件中提取文件名
	static LPCSTR GetFileName(LPCSTR Path_File);
	//从带路径的文件中提取路径，用free释放返回的空间
	static char* GetFilePath(LPCSTR Path_File);
	
	/*计时器*/
	static void TimeStart(DWORD& StartTime);
	static DWORD TimeElapse(const DWORD& StartTime); //返回从YQTimeStart到现在的毫秒值,最长49天

	/*字符串操作*/
	static BOOL StrEqu(LPCSTR S1, LPCSTR S2);
	static BOOL  YQStrFPosi(LPCSTR Str1, LPCSTR Str2);
	static BOOL  YQStrPosi(LPCSTR Str1, LPCSTR Str2);
	static char *Trim(char *str);
    static WORD IsAllNumber(LPCSTR Str);
	static char *Left(char *str,char c);
	static void LeftTrimStr(char* Called, int DelCount);
	static void LeftAddStr(char* Called, LPCSTR AddStr);
	static bool StrRightCmp(LPCSTR LongStr, LPCSTR ShortStr);	//右匹配
	static char* ReplaceChar(char * src,char oldChar,char newChar);
	static void ReplaceStr(char *sSrc, LPCSTR sMatchStr,LPCSTR sReplaceStr);
	static void DeleteChar(char * Src,char DelChar);
	/*位处理*/
	static int WriteBit(const int& Var, BYTE Pos, BOOL Val);
	static BOOL  ReadBit(const int& Var, BYTE Pos);
	static void  SetBit(int& Var, BYTE Pos, BOOL Val);
    static void  SetBit(DWORD& Var, BYTE Pos, BOOL Val);
    static BOOL FileExists(const char *filename);
	/*网络处理*/
	static BOOL IsSameNet(LPCSTR NetMask, LPCSTR CallerRegIP, LPCSTR CalledRegIP);
	static BOOL IsSameNet(LPCSTR CallerAddr,unsigned long  CallerMask,
					LPCSTR CalledAddr, unsigned long CalledMask);
    //时间处理
    static const char*GetDataTimeStamp(char *buffer);
    static const char*GetDataStamp(char *buffer);
};
//---------------------------------------------------------------------------
#endif

