// CWindow.cpp: 实现文件
//

#include "stdafx.h"
#include <windows.h>
#include "MFCApplication1.h"
#include "CWindow.h"
#include "afxdialogex.h"
#include "Tool.h"
#include<stdlib.h>

// CWindow 对话框

IMPLEMENT_DYNAMIC(CWindow, CDialogEx)

CWindow::CWindow(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CWindow, pParent)
{

}

CWindow::~CWindow()
{
}

void CWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listW);
}


BEGIN_MESSAGE_MAP(CWindow, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CWindow 消息处理程序


BOOL CWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 初始化图标链表
	m_imgList.Create(16, 16, ILC_MASK | ILC_COLOR16, 0, 1);

	m_listW.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listW.InsertColumn(0, L"窗口名", 0, 300);
	m_listW.InsertColumn(1, L"窗口类名", 0, 400);
	// 给列表控件设置图标链表
	m_listW.SetImageList(&m_imgList, LVSIL_SMALL);

	DetectWnd(&m_wnd);
	int nIdx = 0, imgIndex = -1;
	CWnd* pWnd = NULL;
	CString buffer;
	for (auto&i:m_wnd)
	{
		TCHAR className[50];
		pWnd = FromHandle(i);
		GetClassName(i, className, 50);
			
		pWnd->GetWindowText(buffer);
		HICON hIcon = (HICON)::GetClassLong(i, GCL_HICON);
		imgIndex = m_imgList.Add(hIcon);
		m_listW.InsertItem(nIdx, buffer, imgIndex);
		m_listW.SetItemText(nIdx, 1, className);
		imgIndex++;
		nIdx++;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CWindow::OnTimer(UINT_PTR nIDEvent)
{
	//DetectWnd();

	CDialogEx::OnTimer(nIDEvent);
}

