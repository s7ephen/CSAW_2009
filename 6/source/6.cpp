// -------------------
/*
    CSAW 2009 
    Reversing Challenge #6

    S.A. Ridley
    stephen@sa7ori.org
*/
// ------------------
#include "sa7_win.h"

DWORD timea = 0;
DWORD timeb = 0;
DWORD time_delta = 0;

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

void start(){
    timea = GetTickCount();
};

void stop(){
    timeb = GetTickCount();
};

DWORD checktime(){
    time_delta = timeb - timea;
    printf("%d\n", timea);
    printf("%d\n", timeb);
    return time_delta;
};

int main(int argc, char* argv[]) {
    char *ct; 
    char *key; 
    sa7_key *keyptr;
    char *tmp;
    start();
    __asm {
    lea eax, ciphertext
    lea ebx, thekey
    jmp done
    ciphertext:
    _emit 0xc2
    _emit 0x3d
    _emit 0x4d
    _emit 0x72
    _emit 0x81
    _emit 0xfb
    _emit 0xed
    _emit 0x7f
    _emit 0x4d
    _emit 0x32
    _emit 0x38
    _emit 0xb7
    _emit 0xd6
    _emit 0x59
    _emit 0x87
    _emit 0xea
    _emit 0x42
    _emit 0xed
    _emit 0x42
    _emit 0x18
    _emit 0xc
    _emit 0x64
    _emit 0xec
    _emit 0x57
    _emit 0x00
    thekey:
    _emit 0x63
    _emit 0x2e
    _emit 0xf
    _emit 0x37
    _emit 0x7a
    _emit 0x13
    _emit 0x64
    _emit 0x7f
    _emit 0x10
    _emit 0x7f
    _emit 0x5
    _emit 0x4
    _emit 0x6e
    _emit 0x36
    _emit 0x60
    _emit 0x2b
    _emit 0x41
    _emit 0x73
    _emit 0x28
    _emit 0x52
    _emit 0x5e
    _emit 0x17
    _emit 0xc
    _emit 0x3f
    _emit 0x00
    done:
    mov ct, eax
    mov key, ebx
    }
    keyptr = (sa7_key *)GlobalAlloc(GPTR, sizeof(sa7_key)); //This is for the
                                                            //state machine
    prepare_key((unsigned char *)key, strlen(key), keyptr);
    sa7((unsigned char *)ct, strlen(ct), keyptr);
    MessageBox(NULL, "I am going to break now. Jump it to prevent it, and get the flag!", "Challenge #6", MB_OK);
    start();
    __asm { //exception they have to jump
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
    }
    stop();
    if (BeingDebugged()) {
        MessageBox(NULL, "Don't be debugging me!?", "WTF!?", MB_OK);
        ExitProcess(0);
    } else {
        if (checktime() > 1){
            MessageBox(NULL, "Hah! I can still see you are debugging me, don't do that!", "WTF!?", MB_OK);
            ExitProcess(0);
        }
        MessageBox(NULL, (char *)ct, "Congratulations! The flag is:", MB_OK);
        ExitProcess(0);
   }
};
