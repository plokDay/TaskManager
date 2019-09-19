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
	//1.����ģ�����
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	if (hSnap == INVALID_HANDLE_VALUE) {
		return;
	}

	//2.��һ�α���ģ��
	MODULEENTRY32 stcMd = { sizeof(stcMd) };
	if (Module32First(hSnap, &stcMd))
	{
		//3.ѭ������ģ��Next
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
		nState = "��������";
		return;
	case -1:
		nState = "������";
		return;
	}
	nState = "�ѽ���";


}
void EnumThread(DWORD dwPid, std::vector<THREADENTRY32>* thdList)
{
	//1.�����߳̿���
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	//2.��һ�α����߳�
	THREADENTRY32 th32 = { sizeof(th32) };
	if (Thread32First(hSnap, &th32))
	{
		//3.ѭ�������߳�
		do
		{
			//�жϸ��߳��Ƿ������������
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