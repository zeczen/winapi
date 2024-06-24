// Compiling: g++ dllinjection.cpp -o dllinjection
// Usage: dllinjection pid

#include<iostream>
#include<windows.h>

int main(int argc, char** argv) {
	int pid = atoi(*(argv+1));
	
	char injdll[] = "myhw.dll";
	// load 
	HMODULE k = GetModuleHandleA(
		"C:\\Windows\\SysWOW64\\kernel32.dll"  // 32bit
						// [in] LPCSTR lpLibFileName
	);
	if (!k) {printf("ERROR GetModuleHandleA: %d\n", GetLastError()); return 1;}

	// get func address
	LPTHREAD_START_ROUTINE func = (LPTHREAD_START_ROUTINE)GetProcAddress(
		k, 				// [in] HMODULE hModule,
		"LoadLibraryA"	// [in] LPCSTR  lpProcName
	);
	if (!func) {printf("ERROR GetProcAddress: %d\n", GetLastError()); return 1;}

	// create handle
	HANDLE h = OpenProcess(
		PROCESS_CREATE_THREAD | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION , 
						// [in] DWORD dwDesiredAccess,
		FALSE, 			// [in] BOOL  bInheritHandle,
		pid				// [in] DWORD dwProcessId
	);
	if (!h) {printf("ERROR OpenProcess: %d\n", GetLastError()); return 1;}
	
	// memory allocation
	LPVOID addr = VirtualAllocEx(
		h,				// [in]           HANDLE hProcess,
		NULL,			// [in, optional] LPVOID lpAddress,
		sizeof injdll,	// [in]           SIZE_T dwSize,
		MEM_RESERVE | MEM_COMMIT,
						// [in]           DWORD  flAllocationType,
		PAGE_EXECUTE_READWRITE
						// [in]           DWORD  flProtect
	);
	if (!h) {printf("ERROR VirtualAllocEx: %d\n", GetLastError()); return 1;}

	// write
	BOOL s = WriteProcessMemory(
		h, 				// [in]  HANDLE  hProcess,
		addr, 			// [in]  LPVOID  lpBaseAddress,
		injdll, 		// [in]  LPCVOID lpBuffer,
		sizeof(injdll)+1,
						// [in]  SIZE_T  nSize,
		NULL			// [out] SIZE_T  *lpNumberOfBytesWritten
	);
	if (!s) {printf("ERROR WriteProcessMemory: %d\n", GetLastError()); return 1;}

	// create thread
	HANDLE t = CreateRemoteThread(
		h, 				// [in]  HANDLE                 hProcess,
		NULL, 			// [in]  LPSECURITY_ATTRIBUTES  lpThreadAttributes,
		0, 				// [in]  SIZE_T                 dwStackSize,	
		func,			// [in]  LPTHREAD_START_ROUTINE lpStartAddress,
		addr,			// [in]  LPVOID                 lpParameter,
		0, 				// [in]  DWORD                  dwCreationFlags,
		NULL			// [out] LPDWORD                lpThreadId
	);
	if (!t) {printf("ERROR CreateRemoteThread: %d\n", GetLastError()); return 1;}
	
	//WaitForSingleObject(t, INFINITE);  // wait till the dll detached
	CloseHandle(h); 

	std::cout << "Done" << std::endl;
	
	return 0;
}