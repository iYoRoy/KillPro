#pragma once
#include <afxdb.h>
#include <odbcinst.h>
#pragma comment(lib,"legacy_stdio_definitions.lib")
#pragma comment(lib,"odbccp32.lib")

CString GetAccessDriver() {
	char szBuf[2001];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	char* pszBuf = szBuf;
	CString sDriver;
	if (!SQLGetInstalledDrivers(szBuf, cbBufMax, &cbBufOut))
		return "";
	do {
		if (strstr(pszBuf, "Access") != 0) {
			sDriver = CString(pszBuf);
			break;
		}
		pszBuf = strchr(pszBuf, '\0') + 1;
	} while (pszBuf[1] != '\0');
	return sDriver;
}