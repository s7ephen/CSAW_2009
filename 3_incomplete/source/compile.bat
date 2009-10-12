rem bcc32 -c tls.cpp
rem ulink -Lc:\progra~1\borland\cbuild~1\lib c0x32.obj tls.obj  uuid.lib cw32.lib
.\UniLinker\ulink.exe -L"c:\Program Files\Microsoft Visual Studio 8\VC\lib" c0x32.obj tls.obj  uuid.lib cw32.lib
