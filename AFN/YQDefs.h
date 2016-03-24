#ifndef __YQDEFS_H__
#define __YQDEFS_H__

#if (defined(WIN32))
	#include "windows.h"
#else

#ifndef CONST
#define CONST               const
#endif

#ifndef TYPEDEF_H
#define TYPEDEF_H

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL				*PBOOL;
typedef BOOL				*LPBOOL;
typedef BYTE				*PBYTE;
typedef BYTE				*LPBYTE;
typedef int					*PINT;
typedef int					*LPINT;
typedef WORD				*PWORD;
typedef WORD				*LPWORD;
typedef long				*LPLONG;
typedef DWORD				*PDWORD;
typedef DWORD				*LPDWORD;
typedef void				*LPVOID;
typedef CONST void			*LPCVOID;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

typedef char *PCHAR, *LPCH, *PCH;
typedef const char *LPCCH, *PCCH;
typedef char *NPSTR, *LPSTR, *PSTR;
typedef PSTR *PZPSTR;
typedef const PSTR *PCZPSTR;
typedef const char *LPCSTR, *PCSTR;
typedef PCSTR *PZPCSTR;

#endif // end of #ifndef TYPEDEF_H


#define __stdcall   

#ifndef CALLBACK
#define CALLBACK    __stdcall
#endif // CALLBACK

#ifndef WINAPI
#define WINAPI      __stdcall
#endif	// WINAPI

#define WINAPIV     __cdecl
#define APIPRIVATE  __stdcall
#define PASCAL      __stdcall
#define APIENTRY    WINAPI


#define FALSE				0
#define TRUE				1

#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif


#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKEDWORD(a, b)      ((DWORD)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

/*±‡“Î≤ªπ˝DWORD_PTR -»•µÙ
#define MAKEDWORD(a, b)      ((DWORD)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define MAKEWORD(a, b)      ((WORD)(((BYTE)((DWORD_PTR)(a) & 0xff)) | ((WORD)((BYTE)((DWORD_PTR)(b) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)((DWORD_PTR)(l) & 0xffff))
#define HIWORD(l)           ((WORD)((DWORD_PTR)(l) >> 16))
#define LOBYTE(w)           ((BYTE)((DWORD_PTR)(w) & 0xff))
#define HIBYTE(w)           ((BYTE)((DWORD_PTR)(w) >> 8))
*/

// 
#define stricmp strcasecmp
#define memicmp strncasecmp


#endif // (defined(WIN32))

#endif // __YQDEFS_H__
