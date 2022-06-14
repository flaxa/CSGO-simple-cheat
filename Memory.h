#pragma once
#include <Windows.h>
#include <vector>
#include <TlHelp32.h>


class Memory
{

	
public:
	HANDLE handle;
	Memory();
	~Memory();
	DWORD getProcess();
	DWORD getModule(DWORD, const wchar_t*);
	template <class val>
	val read(DWORD addr)
	{
		val x;
		ReadProcessMemory(handle, (LPVOID)addr, &x, sizeof(x), NULL);
		return x;
	}
	template <class val>
	val write(DWORD addr, val x)
	{
		WriteProcessMemory(handle, (LPVOID)addr, &x, sizeof(x), NULL);
		return x;
	}
};

