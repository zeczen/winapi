// Compiling: g++ -shared myhelloworldll.cpp -o myhw.dll

#include<windows.h>
#include <iostream>

#define DLL_EXPORT
#include "myhelloworldll.hpp"


extern "C"
{
	DECLDIR void Inject(char* msg) {
		std::cout << msg << std::endl;
		char data[127];
		sprintf(data, "1NJ3CT3D, pid=%d, tid=%d", GetCurrentProcessId(), GetCurrentThreadId());
		MessageBox(
			NULL, 
			(LPCTSTR)data, 
			(LPCTSTR)msg, 
			MB_OK
		);
	}
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: Inject((char*)"DLL_PROCESS_ATTACH"); break;
		case DLL_THREAD_ATTACH: Inject((char*)"DLL_THREAD_ATTACH"); break;
		case DLL_THREAD_DETACH: Inject((char*)"DLL_THREAD_DETACH"); break;
		case DLL_PROCESS_DETACH: Inject((char*)"DLL_PROCESS_DETACH"); break;
	} return TRUE;
}