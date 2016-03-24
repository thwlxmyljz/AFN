//---------------------------------------------------------------------------
#ifndef YQUtils_h
#define YQUtils_h

#include "YQDefs.h"
//---------------------------------------------------------------------------
class TYQUtils
{
public:
	/*�ļ�����*/

	//�Ӵ�·�����ļ�����ȡ�ļ���
	static LPCSTR GetFileName(LPCSTR Path_File);
	//�Ӵ�·�����ļ�����ȡ·������free�ͷŷ��صĿռ�
	static char* GetFilePath(LPCSTR Path_File);
	
	/*��ʱ��*/
	static void TimeStart(DWORD& StartTime);
	static DWORD TimeElapse(const DWORD& StartTime); //���ش�YQTimeStart�����ڵĺ���ֵ,�49��

	/*�ַ�������*/
	static BOOL StrEqu(LPCSTR S1, LPCSTR S2);
	static BOOL  YQStrFPosi(LPCSTR Str1, LPCSTR Str2);
	static BOOL  YQStrPosi(LPCSTR Str1, LPCSTR Str2);
	static char *Trim(char *str);
    static WORD IsAllNumber(LPCSTR Str);
	static char *Left(char *str,char c);
	static void LeftTrimStr(char* Called, int DelCount);
	static void LeftAddStr(char* Called, LPCSTR AddStr);
	static bool StrRightCmp(LPCSTR LongStr, LPCSTR ShortStr);	//��ƥ��
	static char* ReplaceChar(char * src,char oldChar,char newChar);
	static void ReplaceStr(char *sSrc, LPCSTR sMatchStr,LPCSTR sReplaceStr);
	static void DeleteChar(char * Src,char DelChar);
	/*λ����*/
	static int WriteBit(const int& Var, BYTE Pos, BOOL Val);
	static BOOL  ReadBit(const int& Var, BYTE Pos);
	static void  SetBit(int& Var, BYTE Pos, BOOL Val);
    static void  SetBit(DWORD& Var, BYTE Pos, BOOL Val);
    static BOOL FileExists(const char *filename);
	/*���紦��*/
	static BOOL IsSameNet(LPCSTR NetMask, LPCSTR CallerRegIP, LPCSTR CalledRegIP);
	static BOOL IsSameNet(LPCSTR CallerAddr,unsigned long  CallerMask,
					LPCSTR CalledAddr, unsigned long CalledMask);
    //ʱ�䴦��
    static const char*GetDataTimeStamp(char *buffer);
    static const char*GetDataStamp(char *buffer);
};
//---------------------------------------------------------------------------
#endif

