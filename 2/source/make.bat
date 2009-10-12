rem cl /nologo /Ob2 /Z7 2.cpp /c
cl /nologo /Ob2 /Z7 /DMYOFFSET=245 2.cpp /c
link /nologo user32.lib /SECTION:.text,ERW /out:2.exe 2.obj
