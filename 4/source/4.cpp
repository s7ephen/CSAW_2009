// -------------------
/*
    CSAW 2009 
    Reversing Challenge #4

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

__declspec(dllexport) void tumbler1(char *buf){
    short counter;
    for(counter = 0; counter < strlen(buf); counter ++){
        buf[counter] ^= 0x4e; //N 
    };  
};

__forceinline BOOL BeingDebugged() {
// A custom IsDebuggerPresent(). Why? Because it is fun to show off. ^_^
// Actually really it is because we dont want them to catch  a call to
// OpenProcess() or IsDebuggerPresent() to 
    DWORD dw;
    __asm
    {
        push eax
        push ecx
        mov eax, fs:[0x18] //TIB->PEB technique
        mov eax, dword ptr [eax + 0x30] //PEB jawnz
        mov ecx, dword ptr [eax]
        mov dw, ecx
        pop ecx
        pop eax
    }
    return (BOOL)(dw & 0x00010000 ? TRUE : FALSE);
}

__forceinline ULONG find_ie(){
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
            printf((char *)&b);printf((char *)&c);printf((char *)&d); //Gotta do this so 
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
        #include "evil.h"
        if (BeingDebugged()) {
            MessageBox(NULL, "Don't be debugging me!?", "WTF!?", MB_OK);
            ExitProcess(0);
        }
        //I wantd to obfuscate the string here a bit more but ran out of time.
        if (wcsncmp(p->ProcessName.Buffer, L"15a1481fba554e8002ca89013ceae50b.exe", (p->ProcessName.Length/2)) == 0 && p->ProcessId != 0) {//Sometimes UNICODE_STRINGS are not null terminated so we have to do this gayness.
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
    #include "evil.h"
    sa7_key *keyptr;
    void *ct;
    void *key;
    __asm {
    lea eax, ciphertext
    lea ebx, thekey
    jmp done
    ciphertext:
    _emit 0xd8
    _emit 0xb1
    _emit 0x56
    _emit 0xe5
    _emit 0x8b
    _emit 0x1e
    _emit 0x37
    _emit 0x45
    _emit 0xc9
    _emit 0x11
    _emit 0x96
    _emit 0x4b
    _emit 0xcd
    _emit 0x1e
    _emit 0x00
    thekey:
    _emit 0x11
    _emit 0x29
    _emit 0x6c
    _emit 0x61
    _emit 0xd
    _emit 0x15
    _emit 0x13
    _emit 0x6d
    _emit 0x23
    _emit 0x26
    _emit 0x39
    _emit 0x8
    _emit 0x1e
    _emit 0x34
    _emit 0x00
    done:
    mov ct, eax
    mov key, ebx
    }
    keyptr = (sa7_key *)GlobalAlloc(GPTR, sizeof(sa7_key)); //This is for the
                                                            //state machine
    prepare_key((unsigned char *)key, strlen((char *)key), keyptr);
    sa7((unsigned char *)ct, strlen((char *)ct), keyptr);
    if (find_ie() == 0){
        MessageBox(NULL, "All these unfamiliar faces.", "Challenge #4 :-(", MB_OK);
    } else {
        MessageBox(NULL, (char *)ct, "Challenge #4 CONGRATS! The flag is:", MB_OK);
    };
    return 0;
}
