cl /nologo /Ob2 /Z7 /DMYOFFSET=823 4.cpp /c
link /nologo user32.lib psapi.lib /SECTION:.text,ERW /out:4.exe 4.obj
