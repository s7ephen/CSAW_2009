// -------------------
/*
    CSAW 2009 
    Reversing Challenge #7

    S.A. Ridley
    stephen@sa7ori.org
*/
// ------------------

#include "sa7_win.h"
#include <psapi.h>
#include <wchar.h>
#include <string.h>
#include <shlwapi.h>

__declspec(dllexport) void tumbler1(char *buf){
    short counter;
    for(counter = 0; counter < strlen(buf); counter ++){
        buf[counter] ^= 0x4e; //N
    };
};

__declspec(dllexport) void tumbler2(char *buf){
    short counter;
    for(counter = 0; counter < strlen(buf); counter ++){
        buf[counter] ^= 0x49; //I
    };
};

__declspec(dllexport) void tumbler3(char *buf){
    short counter;
    for(counter = 0; counter < strlen(buf); counter ++){
        buf[counter] ^= 0x41; //A
    };
};

__declspec(dllexport) void tumbler4(char *buf){
    short counter;
    for(counter = 0; counter < strlen(buf); counter ++){
        buf[counter] ^= 0x56; //V
    };
};

__declspec(dllexport) void tumbler5(char *buf){
    short counter;
    for(counter = 0; counter < strlen(buf); counter ++){
        buf[counter] ^= 0x55; //U
    };
};

//__declspec(dllexport) void dll_entry(){
BOOLEAN WINAPI DllMain( IN HINSTANCE hDllHandle, 
         IN DWORD     nReason, 
         IN LPVOID    Reserved ){
    HANDLE h_self;
    LPTSTR pName;
    h_self = GetCurrentProcess();
    pName = (LPTSTR)GlobalAlloc(GPTR, 256);
    GetProcessImageFileName(h_self,pName,256);
    if (strstr(pName,"unlock.exe") == NULL){
        MessageBox(NULL, "You aren't my proper loader! I quit!", "Challenge #7", MB_OK);
        ExitProcess(1);
    };
    return TRUE;
};
