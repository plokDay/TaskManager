// CProcess.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "CProcess.h"
#include "afxdialogex.h"
#include "resource.h"
#include<vector>
#include"CMudule.h"
#include"CThread.h"


// CProcess 对话框

IMPLEMENT_DYNAMIC(CProcess, CDialogEx)

CProcess::CProcess(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CProcess, pParent)
{

}

CProcess::~CProcess()
{
}

void CProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CProcess, CDialogEx)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CProcess::OnRclickList1)
	ON_COMMAND(ID_FILE_32771, &CProcess::OnFile32771)
	ON_COMMAND(ID_FILE_32772, &CProcess::OnModule)
	ON_COMMAND(ID_FILE_32773, &CProcess::OnFile32773)
END_MESSAGE_MAP()


// CProcess 消息处理程序


BOOL CProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_menu.LoadMenu(IDR_MENU1);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	const TCHAR* title[] = { L"进程名",L"PID",L"CPU",L"内存" };
	int nIdx = 0;
	for (auto&i : title)
	{
		m_list.InsertColumn(nIdx, i, 0, 120);
		++nIdx;
	}

	
	SetTimer(0,/*定时器的ID,功能类似于控件ID*/
		500,/*间隔时间*/
		NULL/*回调函数, 如果为空,那么本窗口将接收到WM_TIMER的消息*/);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void WINAPI CProcess::RefreshPro()
{
	std::vector<PROCESSINFO> newProcList;

	getAllProc(&newProcList);
	if (m_procList.size() == 0)
	{
		m_procList.swap(newProcList);
	}
	else if (newProcList.size() > m_procList.size())
	{
		CString buffer;
		int index = newProcList.size() - 1;
		m_procList.push_back(newProcList[index]);

		m_list.InsertItem(newProcList.size() - 1, _T(""));
		buffer.Format(_T("%d"), m_procList[index].th32ProcessID);
		m_list.SetItemText(index, 0, m_procList[index].szExeFile);
		m_list.SetItemText(index, 1, buffer);
		m_list.SetItemText(index, 2, L"1%");
		buffer.Format(_T("%.2lfKb"), (double)m_procList[index].dwMemUsg / 1024);
		m_list.SetItemText(index, 3, buffer);
		return;
	}
	else if (newProcList.size() < m_procList.size())
	{
		m_procList.pop_back();
		m_list.DeleteItem(m_procList.size());
		return;
	}
	else if (newProcList.size() == m_procList.size())
	{
		return;
	}
	
	int index = 0;
	for (auto&i : m_procList) {
		// 插入一行节点
		//m_procList.push_back(i);
		// 插入到列表控件中
		CString buffer;
		m_list.InsertItem(index, _T(""));
		buffer.Format(_T("%d"), i.th32ProcessID);
		m_list.SetItemText(index, 0, i.szExeFile);
		m_list.SetItemText(index, 1, buffer);
		m_list.SetItemText(index, 2, L"1%");
		buffer.Format(_T("%.2lfKb"), (double)i.dwMemUsg / 1024);
		m_list.SetItemText(index, 3, buffer);
		index++;
	}
}


void CProcess::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RefreshPro();
	CDialogEx::OnTimer(nIDEvent);
}
void CProcess::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CMenu* pSubMenu = m_menu.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	pSubMenu->TrackPopupMenu(0, pos.x, pos.y, this);
}

//结束进程
void CProcess::OnFile32771()
{
	int index = (int)m_list.GetFirstSelectedItemPosition();
	if (index == 0) {
		return;
	}
	index -= 1;


	CString strPid = m_list.GetItemText(index, 1);// 第1列是pid
	DWORD dwPid = _wtoi(strPid);
	HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);
	if (hProc == 0) {
		CString error;
		error.Format(L"打开进程失败:%d", GetLastError());
		MessageBox(error);
	}
	else {
		TerminateProcess(hProc, 0);
		CloseHandle(hProc);
	}
}


void CProcess::OnModule()
{
	int index = (int)m_list.GetFirstSelectedItemPosition();
	if (index == 0) {
		return;
	}
	index -= 1;

	CString strPid = m_list.GetItemText(index, 1);// 第1列是pid
	DWORD dwPid = _wtoi(strPid);
	CMudule mModule;
	EnumModule(dwPid, &mModule.mmodule);
	mModule.DoModal();
}

//线程
void CProcess::OnFile32773()
{
	
	int index = (int)m_list.GetFirstSelectedItemPosition();
	if (index == 0) {
		return;
	}
	index -= 1;

	CString strPid = m_list.GetItemText(index, 1);// 第1列是pid
	DWORD dwPid = _wtoi(strPid);
	CThread mThread;
	EnumThread(dwPid, &mThread.m_thread);
	mThread.DoModal();
}
