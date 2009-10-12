#ifndef __ulnkhdr_h__
#define __ulnkhdr_h__

#ifndef _WINDEF_
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif
CONST BYTE * __cdecl GetExeString(HMODULE h, LPDWORD pSz);


void _cdecl dpmiNlsInit(void);
extern BOOL (WINAPI *dpmiCharToOem)(LPCSTR, LPSTR);
extern BOOL (WINAPI *dpmiCharToOemBuff)(LPCSTR, LPSTR, DWORD);
extern BOOL (WINAPI *dpmiOemToChar)(LPCSTR, LPSTR);
extern BOOL (WINAPI *dpmiOemToCharBuff)(LPCSTR, LPSTR, DWORD);

#ifdef __cplusplus
};
#endif

#endif //__ulnkhdr_h__

