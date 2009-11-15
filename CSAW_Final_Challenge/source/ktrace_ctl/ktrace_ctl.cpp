#include "stdafx.h"
#include "sa7ori.h"

HANDLE g_hDevice;

void fail(const char* msg)
{
	LPTSTR strErr;
	std::cerr << msg << " ";
	if(::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
					   NULL,
					   ::GetLastError(),
					   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					   (LPTSTR)&strErr,
					   0,
					   NULL))
	{
		std::wcerr << strErr << std::endl;
		::LocalFree(strErr);
	}

	exit(1);
}

std::string get_version()
{
	DWORD bytesReturned;
	char version[1024];
	if(!DeviceIoControl(g_hDevice,
						IOCTL_WRONG,
						0,
						0,
						version,
						sizeof(version),
						&bytesReturned,
						NULL))
	{
		fail("Error getting version.");
	}
	return std::string(version);
}


int _tmain(int argc, _TCHAR* argv[])
{
	g_hDevice = CreateFile("\\\\.\\sa7ori",
						   GENERIC_READ | GENERIC_WRITE,
						   0,
						   NULL,
						   OPEN_EXISTING,
						   FILE_ATTRIBUTE_NORMAL,
						   NULL);
	if(INVALID_HANDLE_VALUE == g_hDevice)
		fail("Error opening device.");
	std::cout << "The driver said: " << get_version() << std::endl;
	return 0;
}
