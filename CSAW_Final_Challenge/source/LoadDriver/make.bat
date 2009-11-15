cl /nologo /Z7 load.c /I"C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Include" /c
link /nologo /libpath:"C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Lib" advapi32.lib kernel32.lib /out:load_driver.exe load.obj
