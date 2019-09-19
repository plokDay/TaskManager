#pragma once

#include <TlHelp32.h>

// CMudule 对话框

class CMudule : public CDialogEx
{
	DECLARE_DYNAMIC(CMudule)

public:
	CMudule(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMudule();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODULE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	CListCtrl m_listM;
	std::vector<MODULEENTRY32> mmodule;

};
