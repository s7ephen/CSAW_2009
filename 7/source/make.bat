cls;
rem build the unlock executable.
cl /nologo unlock.cpp /c
link /nologo /out:unlock.exe unlock.obj

rem build the dll 
cl /nologo 7.cpp /DMYOFFSET=956 /c
link psapi.lib user32.lib kernel32.lib /ENTRY:DllMain /dll /def:7.def user32.lib 7.obj
