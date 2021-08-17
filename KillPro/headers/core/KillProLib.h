#pragma once
#include <TlHelp32.h>
bool FindProcess(const char* ProName) {
	int i = 0;
	PROCESSENTRY32 pe32;
	USES_CONVERSION;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		i += 0;
	}
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore) {
		char* szExeF = pe32.szExeFile;
		if (stricmp(ProName, szExeF) == 0) {
			return true;
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	return false;
}

int Kill_Pro(const char Kill_Name[], int retrytimes) {
	USES_CONVERSION;
	for (int i = 1; i <= retrytimes; i++) {
		HANDLE hSnapshort = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshort == INVALID_HANDLE_VALUE) {
			MessageBox(NULL, _T("Cannot call CreateToolhelp32Snapshot!\n"), _T("Error"), MB_ICONERROR);
			return -1;
		}

		PROCESSENTRY32 stcProcessInfo;
		stcProcessInfo.dwSize = sizeof(stcProcessInfo);
		BOOL  bRet = Process32First(hSnapshort, &stcProcessInfo);
		while (bRet) {
			char* szExeF = stcProcessInfo.szExeFile;
			if (strcmp(szExeF, Kill_Name) == 0) {
				HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, stcProcessInfo.th32ProcessID);
				::TerminateProcess(hProcess, 0);
				CloseHandle(hProcess);
			}
			bRet = Process32Next(hSnapshort, &stcProcessInfo);
		}
		CloseHandle(hSnapshort);
	}
	return 0;
}