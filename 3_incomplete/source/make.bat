cl /nologo /Ob2 /Z7 /DMYOFFSET=781 3.cpp /c
.\UniLinker\ulink.exe -Z- 3.obj kernel32.lib uuid.lib user32.lib kernel32.lib advapi32.lib
