rem cl /nologo /Ob1 /Z7 1.cpp /c
cl /nologo /Z7 1.cpp /c
link /nologo advapi32.lib user32.lib /SECTION:.text,ERW /out:1.exe 1.obj
