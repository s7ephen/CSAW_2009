// -------------------
/*
    CSAW 2009 
    Reversing Challenge #1

    S.A. Ridley
    stephen@sa7ori.org
*/
// ------------------
#include "sa7_win.h"

int main(int argc, char* argv[]) {
    char *ct; 
    char *key; 
    sa7_key *keyptr;
    char *tmp;
    char ct_v[]="Stephania_Vu"; ct=(char *)&ct_v;
//    char key_v[]="\x12kS%"; key=(char*)&key_v;
/*
    __asm {
    lea eax, ciphertext
    lea ebx, thekey
    jmp done
    ciphertext:
    _emit 0x5f
    _emit 0x0b
    _emit 0xb1
    _emit 0xaf
    _emit 0x00
    thekey:
    _emit 0x12
    _emit 0x6b
    _emit 0x53
    _emit 0x25
    _emit 0x00
    done:
    mov ct, eax
    mov key, ebx
    }

    keyptr = (sa7_key *)GlobalAlloc(GPTR, sizeof(sa7_key)); //This is for the
                                                            //state machine
    prepare_key((unsigned char *)key, strlen(key), keyptr);
    sa7((unsigned char *)ct, strlen(ct), keyptr);
*/       
    MessageBox(NULL, "Jump Over Me to get the Flag!", "Jump-Me", MB_OK);
    ExitProcess(0);
    MessageBox(NULL, (char *)ct, "Congratulations! The flag is:", MB_OK);
    ExitProcess(0);
}
;
