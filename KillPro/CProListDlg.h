#pragma once

#define WM_ADDNAMELIST (WM_USER+101)

// CProListDlg 对话框

class CProListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProListDlg)

public:
	CProListDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CProListDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual void OnRefreshList();
	virtual BOOL OnInitDialog();
	CListCtrl m_list_dlg1;
	afx_msg void OnBnClickedButton3();
	void OnLvnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult);
};
