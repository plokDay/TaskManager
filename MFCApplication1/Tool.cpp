#include "stdafx.h"

#include "Tool.h"
#include <Psapi.h>
//#include "CProcess.h"

BOOL getAllProc(std::vector<PROCESSINFO>* procList)
{
	int nRes = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap!=INVALID_HANDLE_VALUE)
	{
		nRes = 1;
		PROCESSENTRY32 proc = { sizeof(PROCESSENTRY32) };
		Process32First(hSnap, &proc);
		do 
		{
			nRes++;
			HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, proc.th32ProcessID);
			PROCESSINFO pi = *(PROCESSINFO*)&proc;
			if (hProc == NULL) {
				pi.dwMemUsg =0;
			}
			else {
				PROCESS_MEMORY_COUNTERS pmc = { sizeof(PROCESS_MEMORY_COUNTERS) };
				
				GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc));
				pi.dwMemUsg = pmc.WorkingSetSize;
				
				CloseHandle(hProc);
			}
			procList->push_back(pi);
		} while (Process32Next(hSnap,&proc));
	}
	return nRes;
}
void EnumModule(DWORD dwPid, std::vector<MODULEENTRY32>* modList)
{
	//1.创建模块快照
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	if (hSnap == INVALID_HANDLE_VALUE) {
		return;
	}

	//2.第一次遍历模块
	MODULEENTRY32 stcMd = { sizeof(stcMd) };
	if (Module32First(hSnap, &stcMd))
	{
		//3.循环遍历模块Next
		do
		{
			modList->push_back(stcMd);
		} while (Module32Next(hSnap, &stcMd));
	}
	CloseHandle(hSnap);

}
void GetThreadState(CString &nState, DWORD nThreadid)
{
	HANDLE nHandle = OpenThread(PROCESS_ALL_ACCESS, FALSE, nThreadid);

	DWORD WaitCode;
	WaitCode = WaitForSingleObject(nHandle, 0);

	CloseHandle(nHandle);

	switch (WaitCode)
	{
	case 258:
		nState = "正在运行";
		return;
	case -1:
		nState = "已销毁";
		return;
	}
	nState = "已结束";


}
void EnumThread(DWORD dwPid, std::vector<THREADENTRY32>* thdList)
{
	//1.创建线程快照
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	//2.第一次遍历线程
	THREADENTRY32 th32 = { sizeof(th32) };
	if (Thread32First(hSnap, &th32))
	{
		//3.循环遍历线程
		do
		{
			//判断该线程是否属于这个进程
			if (th32.th32OwnerProcessID == dwPid)
			{
				thdList->push_back(th32);
			}

		} while (Thread32Next(hSnap, &th32));
	}

		CloseHandle(hSnap);
}
BOOL CALLBACK winProc(HWND hWnd, LPARAM lParam)
{
	std::vector<HWND>* newWnd = (std::vector<HWND>*)lParam;
	
	//newWnd->push_back(hWnd);
	if (IsWindowVisible(hWnd))
	{
		TCHAR buffer[200];
		GetWindowText(hWnd, buffer, _countof(buffer));
		//HICON hIcon = (HICON)::GetClassLong(hWnd, GCL_HICON);
		if (/*hIcon&& */_tcslen(buffer))
		{
			newWnd->push_back(hWnd);
		}
	}
	return 1;

}
void DetectWnd(std::vector<HWND>* wnd)
{
	EnumWindows(winProc, (LPARAM)wnd);
}