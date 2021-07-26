#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>

using namespace std;

int listThreadsInsideAProcess(DWORD processId) {

	HANDLE ThreadSnapshotHandle = INVALID_HANDLE_VALUE;
	THREADENTRY32 threadInfo = { 0 };
	ThreadSnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	threadInfo.dwSize = sizeof(THREADENTRY32);
	int threadCount = 0;
	BOOL firstThread;

	if (ThreadSnapshotHandle == INVALID_HANDLE_VALUE) {
		cout << "Snapshot creation failed with error code : " << GetLastError() << endl;
	}

	cout << "Threads snapshot created" << endl;

	firstThread = Thread32First(ThreadSnapshotHandle, &threadInfo);

	if (firstThread == FALSE) {
		cout << "First thread not retrieved" << endl;
		CloseHandle(ThreadSnapshotHandle);
	}
	do {
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "Thread " << ++threadCount << ": " << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "Thread ID: " << threadInfo.th32ThreadID << endl;
		cout << "Base Priority:" << threadInfo.tpBasePri << endl;
		cout << "Delta Priority: " << threadInfo.tpDeltaPri << endl;
	} while (Thread32Next(ThreadSnapshotHandle, &threadInfo));

	CloseHandle(ThreadSnapshotHandle);
	return 0;
}

int main() {

	HANDLE hSnapshot = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 processInfo = { 0 };
	processInfo.dwSize = sizeof(PROCESSENTRY32);
	int processCount = 0;
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL firstProcess;

	if (hSnapshot == INVALID_HANDLE_VALUE) {
		cout << "Snapshot creation failed with error code : " << GetLastError() << endl;
	}
	cout << "Snapshot created" << endl;

	firstProcess = Process32First(hSnapshot, &processInfo);
	if (!firstProcess) {
		cout << "error" << endl;
		CloseHandle(hSnapshot);
	}
	listThreadsInsideAProcess(processInfo.th32ProcessID);
	cout << "==================================================================================================" << endl;
	cout << "Process " << ++processCount << ": " << endl;
	cout << "==================================================================================================" << endl;
	cout << "Size : " << processInfo.dwSize << endl;
	cout << "Priority class: " << processInfo.pcPriClassBase << endl;
	_tprintf(TEXT("Executable link:  %s"), processInfo.szExeFile);
	cout << endl << "Parent process ID: " << processInfo.th32ParentProcessID << endl;
	cout << "Process ID: " << processInfo.th32ProcessID << endl;
	cout << "Number of threads in the process : " << processInfo.cntThreads << endl;
	cout << "==================================================================================================" << endl;
		
	if (hSnapshot) {
		CloseHandle(hSnapshot);
	}

	return 0;

}