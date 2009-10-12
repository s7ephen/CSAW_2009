#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define TARGET_DLL "7.dll"
typedef FARPROC (__stdcall *PGetProcAddress)(char*);

int main(int argc, char **argv) {
	HMODULE htargetdll;
    PGetProcAddress tumbler1, tumbler2, tumbler4, tumbler3, tumbler5;
    char ciphertext[] = "\x2b\x26\x37\x3c\x35\x31\x2c\x2a\x2b\x65\x16\x31\x24\x37\x31\x20\x21\x4f\x11\x2d\x20\x65\x0e\x20\x3c\x65\x2c\x36\x7f\x65\x62\x11\x2d\x20\x65\x31\x2c\x28\x20\x65\x2d\x24\x36\x65\x26\x2a\x28\x20\x65\x31\x2d\x20\x65\x32\x24\x29\x37\x30\x36\x65\x36\x24\x2c\x21\x6b\x62\00";

	//Load DLL	
	htargetdll = LoadLibrary(TARGET_DLL);
	if (htargetdll == NULL) {
		printf("\n\t Error loading %s. Quitting", TARGET_DLL);
		return 1;
	};
	
	//Import functions from DLL
	tumbler1 = (PGetProcAddress)GetProcAddress(htargetdll,"tumbler1");
	tumbler2 = (PGetProcAddress)GetProcAddress(htargetdll,"tumbler2");
	tumbler3 = (PGetProcAddress)GetProcAddress(htargetdll,"tumbler3");
	tumbler4 = (PGetProcAddress)GetProcAddress(htargetdll,"tumbler4");
	tumbler5 = (PGetProcAddress)GetProcAddress(htargetdll,"tumbler5");

	//Call Imported Functions
	printf("CIPHERTEXT:\n \t%s\n",(char *)&ciphertext);
    tumbler5((char *)&ciphertext);
    tumbler3((char *)&ciphertext);
    tumbler4((char *)&ciphertext);
    tumbler2((char *)&ciphertext);
    tumbler1((char *)&ciphertext);
	printf("PLAINTEXT:\n \t%s",(char *)&ciphertext);

	//Free DLL
	FreeLibrary(htargetdll);
}
