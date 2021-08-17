#pragma once


// CoverDlg 对话框

class CoverDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CoverDlg)

public:
	CoverDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CoverDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COVER_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CRect m_rect;
	CClientDC* m_pDC;
public:
	bool OnSetPixelFormat();
	virtual BOOL OnInitDialog();
	void ChangeSize(UINT nID, int x, int y);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
