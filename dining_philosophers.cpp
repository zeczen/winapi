// Usage: dining_philosophers [# of philos]  

#include<iostream>
#include<windows.h>
#define N 5  // # of times each philos eat
using namespace std; 

CRITICAL_SECTION* mutexs;
CRITICAL_SECTION console;
boolean finish = false;

int n;  // # of philos

DWORD WINAPI phil(LPVOID p) {
	int id = (int)p;
	int c = 0;
	int odd = id % 2;
	
	while (!finish and ++c <= N) {
		// odds firstly aquire the right, while evens firstly aquire the left
		EnterCriticalSection(mutexs+(id+odd)%n);
		EnterCriticalSection(mutexs+(id+1-odd)%n);
		
		EnterCriticalSection(&console);
		cout << "" << id << ": " << c << endl;
		LeaveCriticalSection(&console);
		
		Sleep(500); //eat
		
		LeaveCriticalSection(mutexs+(id+odd)%n);
		LeaveCriticalSection(mutexs+(id+1-odd)%n);
		
	    Sleep(500); //sleep
	} if (c > N) {
		EnterCriticalSection(&console);
		cout << "Philo" << id << ": Done" << endl;
		LeaveCriticalSection(&console);
	}

}

BOOL WINAPI consoleHandler(DWORD signal) {
	// Handle CTRL C event
    if (signal == CTRL_C_EVENT) {
        cout << "Ctrl-C handled\n" << endl;
		finish = true;
	}
    return TRUE;
}

int main(int argc, char** argv){
	n = (argc == 1)? 5:(int)atoi(*(argv+1));

	// limit the number of philos by 2 and 64
	if (n > 64) n = 64;  
	if (n < 2) n = 2;
	
	HANDLE* phils = new HANDLE[n];
	mutexs = new CRITICAL_SECTION[n];

	for (int i = 0; i < n; i++) InitializeCriticalSection(mutexs+i);
	InitializeCriticalSection(&console);
	
	// Set the ctrl c behavior
	if (!SetConsoleCtrlHandler(consoleHandler, TRUE)) {  
        printf("\nERROR: Could not set control handler"); 
        return 1;
    }
	
	for (int i = 0; i < n; i++) {
        // creating the philo
		*(phils + i) = CreateThread(NULL, 0, phil, (LPVOID)i, 0, NULL);
	}
	
	WaitForMultipleObjects(n, phils, TRUE, INFINITE);
	
	// Cleaning
	for (int i = 0; i < n; i++)
		 DeleteCriticalSection(mutexs+i);
	 DeleteCriticalSection(&console);
	 
	delete[] phils;
	delete[] mutexs;
	
	return 0;
}
