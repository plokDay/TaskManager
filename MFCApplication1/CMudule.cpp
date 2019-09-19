// CMudule.cpp: 实现文件
//

#include "stdafx.h"
#include <vector>
#include "MFCApplication1.h"
#include "CMudule.h"
#include "afxdialogex.h"


// CMudule 对话框

IMPLEMENT_DYNAMIC(CMudule, CDialogEx)

CMudule::CMudule(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MODULE, pParent)
{

}

CMudule::~CMudule()
{
}

void CMudule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listM);
}


BEGIN_MESSAGE_MAP(CMudule, CDialogEx)
END_MESSAGE_MAP()


// CMudule 消息处理程序


BOOL CMudule::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(L"模块");
	m_listM.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_listM.InsertColumn(0, L"模块名",0,120);
	m_listM.InsertColumn(1, L"模块路径",0,300);


	//得到此时选中项的PID
	
	
	int nIdx = 0;
	for (auto&i:mmodule)
	{
		m_listM.InsertItem(nIdx,i.szModule);
		m_listM.SetItemText(nIdx, 1, i.szExePath);
		nIdx++;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
