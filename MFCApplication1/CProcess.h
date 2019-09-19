#pragma once
#include <afxdialogEx.h>
#include "Tool.h"
// CProcess 对话框

class CProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CProcess)

public:
	CProcess(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CProcess();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CProcess };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	CMenu m_menu;
	//CMudule m_module;
	std::vector<PROCESSINFO> m_procList;
	virtual BOOL OnInitDialog();
	void WINAPI RefreshPro();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFile32771();
	afx_msg void OnModule();
	afx_msg void OnFile32773();
};
