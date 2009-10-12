// -------------------
/*
    CSAW 2009 
    Reversing Challenge #5

    S.A. Ridley
    stephen@sa7ori.org
*/
// ------------------

#include "sa7_win.h"
#include <psapi.h>
#include <wchar.h>
#include <string.h>
#include <shlwapi.h>
__declspec(dllexport) void function1(){
    //printf("\nHello, I am function1\n");
};

__declspec(dllexport) void function2(){
	//printf("\nHello, I am function2.\n");
};

//__declspec(dllexport) void dll_entry(){
BOOLEAN WINAPI DllMain( IN HINSTANCE hDllHandle, 
         IN DWORD     nReason, 
         IN LPVOID    Reserved ){
    HANDLE h_self;
    LPTSTR pName;
    void *ct;
    void *key;
    sa7_key *keyptr;

    __asm {
    lea eax, ciphertext
    lea ebx, thekey
    jmp done
    ciphertext:
    _emit 0x7e
    _emit 0xc8
    _emit 0xfc
    _emit 0x76
    _emit 0x2b
    _emit 0x91
    _emit 0xdc
    _emit 0x26
    _emit 0xc6
    _emit 0x33
    _emit 0xe2
    _emit 0x5e
    _emit 0xb1
    _emit 0x24
    _emit 0x45
    _emit 0xc3
    _emit 0x60
    _emit 0x9e
    _emit 0x99
    _emit 0x82
    _emit 0x87
    _emit 0xfe
    _emit 0x99
    _emit 0x2
    _emit 0x00
    thekey:
    _emit 0x36
    _emit 0x61
    _emit 0x45
    _emit 0x5f
    _emit 0x68
    _emit 0x34
    _emit 0x4c
    _emit 0x45
    _emit 0x4f
    _emit 0x9
    _emit 0x1d
    _emit 0x3d
    _emit 0x44
    _emit 0x4a
    _emit 0x56
    _emit 0x6f
    _emit 0x40
    _emit 0x16
    _emit 0x6b
    _emit 0x16
    _emit 0x9
    _emit 0x6e
    _emit 0x40
    _emit 0x73
    _emit 0x00
    done:
    mov ct, eax
    mov key, ebx
    }
    keyptr = (sa7_key *)GlobalAlloc(GPTR, sizeof(sa7_key)); //This is for the
                                                            //state machine
    prepare_key((unsigned char *)key, strlen((char *)key), keyptr);
    sa7((unsigned char *)ct, strlen((char *)ct), keyptr);

    h_self = GetCurrentProcess();
    pName = (LPTSTR)GlobalAlloc(GPTR, 256);
    GetProcessImageFileName(h_self,pName,256);
    if (strstr(pName,"loader.exe") == NULL){
        MessageBox(NULL, "You aren't my Mama. I quit!", "Challenge #5", MB_OK);
        ExitProcess(1);
    };
    MessageBox(NULL, (char *)ct, "#5 Congratulations! The Flag is:", MB_OK);
    ExitProcess(1);
    return TRUE;
};
