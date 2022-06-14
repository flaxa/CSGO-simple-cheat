#include "Memory.h"
#include <iostream>



Memory::Memory()
{
	handle = NULL;
}

Memory::~Memory()
{
	CloseHandle(handle);
}


DWORD Memory::getProcess()
{
	DWORD procID;
	HWND hwnd;
	do
	{
		hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive");
		Sleep(1000);
	} while (hwnd == NULL);

	do
	{
		GetWindowThreadProcessId(hwnd, &procID);
	} while (procID == NULL);

	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
	Sleep(1000);
	return procID;
}


DWORD Memory::getModule(DWORD procId, const wchar_t* modName)
{
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procId);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);

	do
	{
		if (!_wcsicmp(mEntry.szModule, modName))
		{
			CloseHandle(hModule);
			return (DWORD)mEntry.hModule;
		}
		
	} while (Module32Next(hModule, &mEntry));
	Sleep(1000);
	return getModule(procId,modName);
}