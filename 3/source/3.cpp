// -------------------
/*
    CSAW 2009 
    Reversing Challenge #3

    S.A. Ridley
    stephen@sa7ori.org
*/
// ------------------
#include "sa7ori_wincludes.h"
#include <dbghelp.h>
#include "sa7debug.h"
#include <tchar.h>
#define ONEK 1024
#define _NO_CVCONST_H
#include <psapi.h> //For EnumProcessModules()
#include <stdio.h>
#include <limits.h>
#include <wchar.h>
#include "sa7_win.h"

ULONG find_ie(){
/***
        I used this technique to prevent them from breaking on EnumProcesses.
        maybe makes it  a bit harder ;-)
***/
    tZwQuerySystemInformation pZwQuerySystemInformation = NULL;
    DWORD status = 0;
    PVOID mySysInfo = NULL; //pointer to mem we get alloc'd
    DWORD sizeReturned = 0;
    SYSTEM_PROCESSES *p = NULL;
    ULONG sizeAlloc = 0;  
    //ULONG sizeIncrement = 0;
    BOOL done = FALSE;
    ULONG targeted_pid = 0;
    //Just some hashes to have in the BSS 
    #include "evil.h"
    char a[] = "15a1481fba554e8002ca89013ceae50b.exe";
    char b[] = "42c87be0d8b21f5488b2576729f4b442.exe";
    char c[] = "4f42b2118ae13f0c0d01e79925db15e9.exe";
    char d[] = "4612f502b8936fab6fffc71d5ba7098c.exe";

   HMODULE hLib = LoadLibrary("ntdll.dll");
    if (pZwQuerySystemInformation == NULL){
        pZwQuerySystemInformation = (tZwQuerySystemInformation)GetProcAddress(hLib, "ZwQuerySystemInformation"); 
    }   
    FreeLibrary(hLib);

    do{ 
        mySysInfo = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeAlloc);
        //printf("%d", sizeAlloc);
        if (!mySysInfo){ //do an error
            printf("\nCould not Alloc. I am made entirely of cheese.");
            printf((char *)&a);printf((char *)&b);printf((char *)&c);printf((char *)&d); //Gotta do this so 
                                                    //unused bss data doesnt get
                                                    //optimized out by compiler.
        }   
        status = pZwQuerySystemInformation(
                SystemProcessesAndThreadsInformation,
                mySysInfo,
                sizeAlloc,
                &sizeReturned);
//        printf("Stat: 0x%x Returned: %x", status, sizeReturned);
        if((STATUS_INFO_LENGTH_MISMATCH != status)){
//            printf("\nSTATUS_INFO_LENGTH_MISMATCH != status");
            break;
        };  

        HeapFree(GetProcessHeap(), 0, mySysInfo);
        mySysInfo = NULL;

        sizeAlloc += sizeReturned;printf(".");

    }while(1);
    for (p = (struct _SYSTEM_PROCESSES *)mySysInfo; !done; p = (SYSTEM_PROCESSES *)(((char*)p) + p->NextEntryDelta)){
        // do some shit with the info for the current process at p->
        //printf("\n%d", p->ProcessId);
        if (wcsncmp(p->ProcessName.Buffer, L"c52e29d93f53d6098cc31d7e185008d0.exe", (p->ProcessName.Length/2)) == 0 && p->ProcessId != 0) {//Sometimes UNICODE_STRINGS are not null terminated so we have to do this gayness.
            //_tprintf("\nFound a familiar face (%ws) at pid %d !!!",p->ProcessName.Buffer, p->ProcessId);
            targeted_pid = p->ProcessId;
        }
        //_tprintf(":%ws", (PWSTR *)p->ProcessName.Buffer);
        done = (p->NextEntryDelta == 0); 
    }
    return targeted_pid;
}

int _tmain(int argc, const TCHAR* argv[])
{
    BOOL bRet = FALSE; 
    //Might have to do some SymSearchPath() stuff later.
    ULONG symAddr;
    char saved_byte[1]; //place to hold the byte that gets overwritten by our breakpoint 

    char *ct;
    char *key;
    sa7_key *keyptr;
    char ct_v[]="\xcd\xca\xfa\xf9\x7b\x80\x0d\x42\xd6\x90\x6f\xa6\x8b\x4d\x5f\x9b\x8a\x3c\x3e\x9e\x68\x89\x40\x8c\x61\x36\x70\x6f\x4e\x95\x84\xd6";ct=(char *)&ct_v;
    char key_v[]="\x3e\x18\x41\x2d\x14\x1c\x13\x79\x25\x27\x47\x49\x0c\x17\x2e\x12\x2a\x26\x21\x7d\x47\x75\x17\x45\x45\x16\x38\x55\x7e\x21\x09\x65";key=(char*)&key_v;
    keyptr = (sa7_key *)GlobalAlloc(GPTR, sizeof(sa7_key)); //This is for the
                                                            //state machine
    prepare_key((unsigned char *)key, strlen((char *)key), keyptr);
    sa7((unsigned char *)ct, strlen((char *)ct), keyptr);
    if (find_ie() == 0){
        MessageBox(NULL, "All these unfamiliar faces.", "Challenge #3 :-(", MB_OK);
    } else {
        MessageBox(NULL, (char *)ct, "Challenge #3 CONGRATS! The flag is:", MB_OK);
    };
    return 0;
}
