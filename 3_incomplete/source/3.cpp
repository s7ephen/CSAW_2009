// -------------------
/*
    CSAW 2009 
    Reversing Challenge #3

    S.A. Ridley
    stephen@sa7ori.org

    This fantastic TLS example was mostly ripped from Ilfak's 2005 posting:
    http://www.hexblog.com/2005/10/tls_callbacks.html
    
    Also lots and lots of thanks to
    Louri Kharon (cabel.net)
    who helped me use his fantastic linker!

*/
// ------------------


#include <windows.h>
#include <stdio.h>
#include "ulnfeat.h"

/*
     This is a TLS callback. It
*/

void __stdcall callback(void * /*instance*/,
                        DWORD reason,
                        void * /*reserved*/)
{
  if ( reason == DLL_PROCESS_ATTACH )
  {
    MessageBox(NULL, "Hello, world!", "Hidden message", MB_OK);
    printf("Hello World!");
    ExitProcess(0);
  }
}

TLS_CALLBACK(c1, callback);     // Unilink trick to declare callbacks

/*
     This is the main function.
     It will never be executed since the callback will call ExitProcess().
*/
int main(void) {
    printf("main\n");
    return tmp;
}

