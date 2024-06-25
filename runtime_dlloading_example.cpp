#include<iostream>
#include<windows.h>
using namespace std;

typedef void (*PVOIDFUNC)(char*);


int main() {
	HMODULE k = LoadLibraryA(
		"myhw.dll"	// [in] LPCSTR lpLibFileName
	);
	if (!k) {printf("ERROR LoadLibraryA: %d\n", GetLastError()); return 1;}
	cout << "Loads success" << endl;

	// get func address
	PVOIDFUNC func = (PVOIDFUNC)GetProcAddress(
		k, 			// [in] HMODULE hModule,
		"Message"	// [in] LPCSTR  lpProcName
	);
	if (!func) {printf("ERROR GetProcAddress: %d\n", GetLastError()); return 1;}


	cout << "Calling dll function" << endl;
	func((char*)"PROG");
	cout << "Exiting" << endl;

    return 0;
}
