#pragma once

#include<vector>
// CWindow 对话框

class CWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CWindow)

public:
	CWindow(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWindow();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CWindow };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listW;
	std::vector<HWND> m_wnd;
	CImageList   m_imgList; // 图标链表
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
};
