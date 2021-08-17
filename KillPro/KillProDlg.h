
// KillProDlg.h: 头文件
//

#pragma once
#include "CProListDlg.h"
#include "CoverDlg.h"

class CKillProDlgAutoProxy;


// CKillProDlg 对话框
class CKillProDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKillProDlg);
	friend class CKillProDlgAutoProxy;

// 构造
public:
	CKillProDlg(CWnd* pParent = nullptr);	// 标准构造函数
	virtual ~CKillProDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KILLPRO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	CKillProDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
//	virtual void SaveFile();//旧版文件存储
	CProListDlg m_propage;
	CoverDlg m_covDlg;
	CListCtrl m_list;
	CComboBox m_combo;
	NOTIFYICONDATA m_ntIcon;
	virtual void ReadFromJsonFile(const char* jsonFile);
	virtual void WriteToJsonFile(const char* jsonFile);
	virtual bool CheckStartUp();
	virtual bool SetStartUp(bool isStartUp);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddNamePage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
//	afx_msg void OnBnClickedButton3();
//	afx_msg void OnBnClickedButton4();
//	afx_msg void OnBnClickedButton6();
//	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton9();
//	virtual INT_PTR DoModal();
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClickedButton7();
	afx_msg void On32771();
	afx_msg void On32772();
	afx_msg void On32773();
	afx_msg void On32774();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnInputData();
	afx_msg void OnOutputData();
	afx_msg void OnNotifyIconExit();
	afx_msg void OnNotifyIconPauser();
	afx_msg void OnNotifyIconShowWindow();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
