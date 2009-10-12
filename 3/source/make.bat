cl /nologo /Ob2 /Z7 /DMYOFFSET=942 3.cpp /c
link /nologo user32.lib psapi.lib /SECTION:.text,ERW /out:3.exe 3.obj
