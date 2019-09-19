#pragma once
#include <windows.h>
#include <vector>
#include <TlHelp32.h>
struct PROCESSINFO :PROCESSENTRY32
{
	DWORD dwMemUsg;
};

BOOL getAllProc(std::vector<PROCESSINFO>* procList);
void EnumModule(DWORD dwPid, std::vector<MODULEENTRY32>* modList);
void GetThreadState(CString &nState, DWORD nThreadid);
void EnumThread(DWORD dwPid, std::vector<THREADENTRY32>* thdList);
BOOL CALLBACK winProc(HWND hWnd, LPARAM lParam);
void DetectWnd(std::vector<HWND>* wnd);