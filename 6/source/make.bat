rem cl /nologo /Ob2 /Z7 6.cpp /c
cl /nologo /Ob2 /Z7 /DMYOFFSET=251 6.cpp /c
link /nologo user32.lib /SECTION:.text,ERW /out:6.exe 6.obj
