/*
     TLS callback demonstration program.
     This program may be used to learn/illustrate the TLS callback concept.
     Copyright 2005 Ilfak Guilfanov <ig@hexblog.com>

     There is no standard way (from compiler vendors) of creating it.
     We use a special linker, UniLink, to create them.
     Please contact Yury Haron <yjh@styx.cabel.net> for more information
     about the linker.

*/

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
    ExitProcess(0);
  }
}

TLS_CALLBACK(c1, callback);     // Unilink trick to declare callbacks

/*
     This is the main function.
     It will never be executed since the callback will call ExitProcess().
*/

int main(void)
{
  return 0;
}

