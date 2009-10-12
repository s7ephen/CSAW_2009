cls;
rem build the loader executable.
cl /nologo loader.cpp /c
link /nologo /out:loader.exe loader.obj
copy loader.exe wrongload.exe

rem build the dll 

cl /nologo 5.cpp /DMYOFFSET=774 /c
rem link /dll /def:5.def /ENTRY:dll_entry /SECTION:.text,ERW user32.lib 5.obj
link psapi.lib user32.lib kernel32.lib /SECTION:.text,ERW /ENTRY:DllMain /dll /def:5.def 5.obj
