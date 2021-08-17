// CProListDlg.cpp: 实现文件
//

#include "pch.h"
#include "KillPro.h"
#include "CProListDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


// CProListDlg 对话框

IMPLEMENT_DYNAMIC(CProListDlg, CDialogEx)

CProListDlg::CProListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CProListDlg::~CProListDlg()
{
}

void CProListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list_dlg1);
}


BEGIN_MESSAGE_MAP(CProListDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CProListDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CProListDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CProListDlg::OnBnClickedButton3)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CProListDlg::OnLvnColumnclickList1)
END_MESSAGE_MAP()


// CProListDlg 消息处理程序
int procnt;


void CProListDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < procnt; i++) {
		if (m_list_dlg1.GetCheck(i) == TRUE) {
			CString mName = m_list_dlg1.GetItemText(i, 1);
			mName.TrimLeft();
			mName.TrimRight();
			::SendMessage(this->m_hWnd, WM_ADDNAMELIST, (WPARAM)&mName, NULL);
		}
	}
	ShowWindow(FALSE);
}


void CProListDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(FALSE);
}

void CProListDlg::OnRefreshList()
{
	procnt = 0;
	m_list_dlg1.DeleteAllItems();
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("CreateToolhelp32Snapshot调用失败!\n");
		return;
	}
	//遍历进程快照。轮流显示每个进程的信息  
	CString strPrcNameID;
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		m_list_dlg1.InsertItem(procnt, "");
		m_list_dlg1.SetItemText(procnt, 1, pe32.szExeFile);
		procnt++;
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	//清除snapshot对象  

	::CloseHandle(hProcessSnap);
}


BOOL CProListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_list_dlg1.InsertColumn(0, _T(""), LVCFMT_LEFT, 25);
	m_list_dlg1.InsertColumn(1, _T("进程"), LVCFMT_LEFT, 400);//格式化列表

	LONG lStyle;
	lStyle = GetWindowLong(m_list_dlg1.m_hWnd, GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT;
	SetWindowLong(m_list_dlg1.m_hWnd, GWL_STYLE, lStyle);
	DWORD dwStyle = m_list_dlg1.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_CHECKBOXES;
	m_list_dlg1.SetExtendedStyle(dwStyle);//设置列表风格

	OnRefreshList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CProListDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	OnRefreshList();
}

int sort_column; // 记录点击的列
bool method=true; // 记录比较方法
int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// 从参数中提取所需比较lc的两行数据
	int row1 = (int)lParam1;
	int row2 = (int)lParam2;
	CListCtrl* lc = (CListCtrl*)lParamSort;
	CString lp1 = lc->GetItemText(row1, sort_column);
	CString lp2 = lc->GetItemText(row2, sort_column);
	// 比较，对不同的列，不同比较，注意记录前一次排序方向，下一次要相反排序
	if (method)
		return lp1.CompareNoCase(lp2);
	else
		return lp2.CompareNoCase(lp1);
	return 0;
}

void CProListDlg::OnLvnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	sort_column = pNMLV->iSubItem;//点击的列
	int count = m_list_dlg1.GetItemCount();
	for (int i = 0; i < count; i++)
		m_list_dlg1.SetItemData(i, i); // 每行的比较关键字，此处为列序号（点击的列号），可以设置为其他比较函数的第一二个参数
	m_list_dlg1.SortItems(MyCompareProc, (DWORD_PTR)&m_list_dlg1); // 排序第二个参数是比较函数的第三个参数
	method = !method;
	*pResult = 0;
}
