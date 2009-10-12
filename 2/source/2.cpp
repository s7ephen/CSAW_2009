// -------------------
/*
    CSAW 2009 
    Reversing Challenge #2

    S.A. Ridley
    stephen@sa7ori.org
*/
// ------------------
#include "sa7_win.h"

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

int main(int argc, char* argv[]) {
    char *ct; 
    char *key; 
    sa7_key *keyptr;
    char *tmp;
    char ct_v[]="\xea\x20\xf6\x16\xdc\xab\x55\xdb\x7a\x1e\x8e\xb0\x03\xfe\x26\x27\xe5\x74\x73\xbd\xbb\x1b\xea\x4b\x92\xc1\x11\x50\x9b\x5b\x6a\x04"; ct=(char *)&ct_v;
    char key_v[]="\x24\x53\x60\x08\x54\x5d\x1f\x56\x1b\x77\x57\x10\x50\x69\x4e\x43\x5d\x2e\x15\x21\x5c\x08\x57\x02\x40\x48\x7f\x62\x50\x55\x29\x35"; key=(char*)&key_v;

    keyptr = (sa7_key *)GlobalAlloc(GPTR, sizeof(sa7_key)); //This is for the
                                                            //state machine
    prepare_key((unsigned char *)key, strlen(key), keyptr);
    sa7((unsigned char *)ct, strlen(ct), keyptr);

    MessageBox(NULL, "I am going to crash now. Jump it to prevent it, and get the flag!", "Challenge #2", MB_OK);
    __asm { //exception they have to jump
        mov eax, 0x0
        call eax
    } 
    if (BeingDebugged()) {
        MessageBox(NULL, "Don't be debugging me!?", "WTF!?", MB_OK);
        ExitProcess(0);
    } else {
        MessageBox(NULL, (char *)ct, "Congratulations! The flag is:", MB_OK);
        ExitProcess(0);
   }
};
