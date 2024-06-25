// Compiling: g++ -shared myhelloworldll.cpp -o myhw.dll

#include<windows.h>
#include <iostream>

#define DLL_EXPORT
#include "myhelloworldll.hpp"


extern "C"
{
	DECLDIR void Message(char* msg) {
		std::cout << msg << std::endl;
		char data[63];
		char user[257]; // UNLEN +1
		SIZE_T size = sizeof user;
		GetUserNameA(user, &size);
		sprintf(data, "Alert!, pid=%d, tid=%d, user=%s", GetCurrentProcessId(), GetCurrentThreadId(), user);
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
		case DLL_PROCESS_ATTACH: 
			Message((char*)"DLL_PROCESS_ATTACH"); 
			break;
		case DLL_THREAD_ATTACH: 
			Message((char*)"DLL_THREAD_ATTACH"); 
			break;
		case DLL_THREAD_DETACH: 
			Message((char*)"DLL_THREAD_DETACH"); 
			break;
		case DLL_PROCESS_DETACH: 
			Message((char*)"DLL_PROCESS_DETACH"); 
			break;
	} return TRUE;
}