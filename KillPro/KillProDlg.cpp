
// KillProDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "KillPro.h"
#include "KillProDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"

#include <fstream>
#include <string>
#include <regex>
#include "headers/json/json.h"
#include "headers/core/KillProLib.h"
#include "headers/core/GetHWNDLib.h"
#include "headers/database/mdb.h"
#include "headers/uac/UAC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#pragma comment(lib,"lib/json_vc71_libmtd.lib")
#else
 #pragma comment(lib,"lib/json_vc71_libmt.lib")
#endif

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
#define WM_ADDNAME			(WM_USER + 1)
#define WM_NOTIFYICON		(WM_USER + 2)


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CKillProDlg 对话框


IMPLEMENT_DYNAMIC(CKillProDlg, CDialogEx);

CKillProDlg::CKillProDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KILLPRO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = nullptr;
}

CKillProDlg::~CKillProDlg()
{
	// 如果该对话框有自动化代理，则
	//  此对话框的返回指针为 null，所以它知道
	//  此代理知道该对话框已被删除。
	if (m_pAutoProxy != nullptr)
		m_pAutoProxy->m_pDialog = nullptr;
}

void CKillProDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
}

BEGIN_MESSAGE_MAP(CKillProDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_ADDNAME, OnAddList)
	ON_MESSAGE(WM_HOTKEY, OnHotKey)
	ON_MESSAGE(WM_ADDNAMELIST, OnAddNamePage)
	ON_MESSAGE(WM_NOTIFYICON, OnNotifyIcon)
	ON_BN_CLICKED(IDCANCEL, &CKillProDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON2, &CKillProDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CKillProDlg::OnBnClickedButton1)
//	ON_BN_CLICKED(IDC_BUTTON3, &CKillProDlg::OnBnClickedButton3)
//	ON_BN_CLICKED(IDC_BUTTON4, &CKillProDlg::OnBnClickedButton4)
//	ON_BN_CLICKED(IDC_BUTTON6, &CKillProDlg::OnBnClickedButton6)
//	ON_BN_CLICKED(IDC_BUTTON8, &CKillProDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON5, &CKillProDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON9, &CKillProDlg::OnBnClickedButton9)
//	ON_WM_CREATE()
//ON_BN_CLICKED(IDC_BUTTON7, &CKillProDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON7, &CKillProDlg::OnClickedButton7)
	ON_COMMAND(ID_32771, &CKillProDlg::On32771)
	ON_COMMAND(ID_32772, &CKillProDlg::On32772)
	ON_COMMAND(ID_32773, &CKillProDlg::On32773)
	ON_COMMAND(ID_32774, &CKillProDlg::On32774)
	ON_BN_CLICKED(IDC_BUTTON3, &CKillProDlg::OnBnClickedButton3)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CKillProDlg::OnCbnSelchangeCombo1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK1, &CKillProDlg::OnBnClickedCheck1)
	ON_COMMAND(ID_32775, &CKillProDlg::OnInputData)
	ON_COMMAND(ID_32776, &CKillProDlg::OnOutputData)
	ON_COMMAND(ID_32779, &CKillProDlg::OnNotifyIconExit)
	ON_COMMAND(ID_32778, &CKillProDlg::OnNotifyIconPauser)
	ON_COMMAND(ID_32777, &CKillProDlg::OnNotifyIconShowWindow)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CKillProDlg 消息处理程序

bool hidewindow = false;//是否隐藏
bool pause = true;//是否暂停
bool isexit = true;//是否不在运行
bool autostartup = false;//是否开机自启
bool usemdb = false;//是否使用Access数据库
bool isMTMode = false;//是否启用多线程模式
					  //多线程模式为每个拦截任务单独启动一个线程，
					  //若拦截项较多则会引发卡顿
bool isThreadReady;
bool isFindThreadReady;
CString strDataPath = "ProcessNames.json";

struct ProcData {
	int type;
	std::string Name;
	CString Title;
};
struct MultiWorkThreadParam {
	CKillProDlg* p_Dlg;
	int counter;
	ProcData dtProc;
};
struct WindowFinderThreadParam {
	CKillProDlg* p_Dlg;
	CoverDlg* p_CovDlg;
};

bool OnCheckMdbDriver() {

	return false;
}

CString GetWorkDir();

DWORD WINAPI KillProMultiWorkThread(LPVOID lpParam) {
	MultiWorkThreadParam* m_param = (MultiWorkThreadParam*)lpParam;
	CListCtrl* p_list = (CListCtrl*)(m_param->p_Dlg->GetDlgItem(IDC_LIST1));
	USES_CONVERSION;
	ProcData mProc;
	int i = 0;
	/*	旧版数据读取
	std::ifstream readf("ProcessName.dat");
	while (std::getline(readf,names[++n]));
	*/

	int n = m_param->counter;
	mProc.type = m_param->dtProc.type;
	mProc.Name = m_param->dtProc.Name;
	mProc.Title = m_param->dtProc.Title;

	isThreadReady = true;

	while (true) {
		Sleep(500);
		if (pause) {//暂停
			continue;
		}
		if (isexit)return 0;
		if (mProc.type == 0) {
			DWORD pid = GetProcessIDByName(mProc.Name.c_str());
			std::vector<HWND> vecHWnds;
			GetHWndsByProcessID(pid, vecHWnds);
			for (const HWND& targetHWnd : vecHWnds)
			{
				char szWindowTitle[512];
				GetWindowText(targetHWnd, szWindowTitle, sizeof(szWindowTitle));
				if (std::regex_match(szWindowTitle, std::regex(mProc.Title)) || szWindowTitle == mProc.Title) {
					ShowWindow(targetHWnd, SW_HIDE);
				}
			}
		}
		else {
			if (FindProcess(mProc.Name.c_str())) {
				Kill_Pro(mProc.Name.c_str(), 5);
			}
		}
	}
}

DWORD WINAPI KillProSingleWorkThread(LPVOID lpParam) {//运行主函数

	CKillProDlg* p_Dlg = (CKillProDlg*)lpParam;
	CListCtrl* p_list = (CListCtrl*)p_Dlg->GetDlgItem(IDC_LIST1);
	ProcData mProc[1024];
	int i = 0;
	/*	旧版数据读取
	std::ifstream readf("ProcessName.dat");
	while (std::getline(readf,names[++n]));
	*/
	
	int n = p_list->GetItemCount();
	for (int i = 0; i < n; i++) {
		mProc[i].type = (p_list->GetItemText(i, 0) == "窗口" ? 0 : 1);
		mProc[i].Name= p_list->GetItemText(i, 1);
		mProc[i].Title = p_list->GetItemText(i, 2);
	}

	while (true) {
		Sleep(500);//每0.5秒执行一次，否则会导致处理器资源占用过高或死机
		i %= n;
		if (pause) {//暂停
			continue;
		}
		if (isexit)return 0;
		if (mProc[i].type == 0) {
			DWORD pid = GetProcessIDByName(mProc[i].Name.c_str());
			std::vector<HWND> vecHWnds;
			GetHWndsByProcessID(pid, vecHWnds);
			for (const HWND& targetHWnd : vecHWnds)
			{
				char szWindowTitle[512];
				GetWindowText(targetHWnd, szWindowTitle, sizeof(szWindowTitle));
				if (std::regex_match(szWindowTitle, std::regex(mProc[i].Title)) || szWindowTitle == mProc[i].Title) {
					ShowWindow(targetHWnd, SW_HIDE);
				}
			}
		}
		else {
			if (FindProcess(mProc[i].Name.c_str())) {
				Kill_Pro(mProc[i].Name.c_str(), 5);
			}
		}
		i++;
	}
}

void CKillProDlg::ReadFromJsonFile(const char* jsonFile) {//从json文件读取进程名信息
	Json::Value root;
	Json::Reader reader;
	std::ifstream ifs(jsonFile);

	if (!reader.parse(ifs, root)) {//读取失败或格式错误
		AfxMessageBox(!hidewindow ? "数据文件读取失败，将自动创建！" : "Error:数据文件为空，KillPro无法执行后台工作命令！");
		if (hidewindow) { PostQuitMessage(-1); ::Shell_NotifyIcon(NIM_DELETE, &m_ntIcon); }
		return;
	}
	
	int n = root["proc"].size();//读取项个数
	for (int i = 0; i < n; i++) {
		int type = root["proc"][i]["type"].asInt();
		CString m_name = root["proc"][i]["name"].asString().c_str();//读取名字
		CString m_title = root["proc"][i]["title"].asString().c_str();
		m_list.InsertItem(m_list.GetItemCount(), (type == 0 ? "窗口" : "进程"));//写入列表
		m_list.SetItemText(m_list.GetItemCount() - 1, 1, m_name);
		m_list.SetItemText(m_list.GetItemCount() - 1, 2, m_title);
	}
	isMTMode = root["isMTMode"].asBool();
	ifs.close();
	return;
}

void CKillProDlg::WriteToJsonFile(const char* jsonFile) {//将数据写入json文件

	std::ofstream ofs(jsonFile);
	Json::Value root = Json::objectValue;
	Json::Reader reader;
	Json::StyledWriter swriter;
	for (int i = 0; i < m_list.GetItemCount(); i++) {
		root["proc"][i]["type"] = (m_list.GetItemText(i, 0) == "窗口" ? 0 : 1);//从列表读取名字并记录
		root["proc"][i]["name"] = (std::string)m_list.GetItemText(i, 1);
		root["proc"][i]["title"] = (std::string)m_list.GetItemText(i, 2);
	}
	root["isMTMode"] = isMTMode;
	std::string str = swriter.write(root);
	ofs << str << std::endl;
	ofs.close();
	return;
}

bool CKillProDlg::CheckStartUp() {
	TCHAR Dir[_MAX_PATH];
	SHGetSpecialFolderPath(this->GetSafeHwnd(), Dir, CSIDL_STARTUP, 0);
	CString StartUpPath;
	StartUpPath.Format("%s\\StartUpKillPro.bat", Dir);
	return GetFileAttributes(StartUpPath) == 32;
}

bool CKillProDlg::SetStartUp(bool isStartUp)
{
	if (isStartUp) {
		CString path;
		CString command, runpath;
		path.Format("%s --nogui", AfxGetApp()->m_pszExeName);
		runpath.Format("cd /d \"%s\"", GetWorkDir());
		command.Format("start \"KillProAutoStart\" %s", path);
		std::ofstream ofs("AutoStartUpKillPro.bat");
		ofs << "@echo off" << std::endl;
		ofs << runpath << std::endl;
		ofs << command << std::endl;
		ofs.close();
		TCHAR Dir[_MAX_PATH];
		SHGetSpecialFolderPath(this->GetSafeHwnd(), Dir, CSIDL_STARTUP, 0);
		CString StartUpPath;
		StartUpPath.Format("%s\\StartUpKillPro.bat", Dir);//生成开机自启文件
		return CopyFile("AutoStartUpKillPro.bat", StartUpPath, false);//复制开机自启文件到自启动文件夹
	}
	else {
		TCHAR Dir[_MAX_PATH];
		SHGetSpecialFolderPath(this->GetSafeHwnd(), Dir, CSIDL_STARTUP, 0);
		CString StartUpPath;
		StartUpPath.Format("%s\\StartUpKillPro.bat", Dir);//生成开机自启文件
		return DeleteFile(StartUpPath);
	}
	return TRUE;
}

BOOL CKillProDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	/*********设置列表********/
	m_list.InsertColumn(0, _T("类型"), LVCFMT_LEFT, 40);
	m_list.InsertColumn(1, _T("进程"), LVCFMT_LEFT, 200);//格式化列表
	m_list.InsertColumn(2, _T("窗口信息"), LVCFMT_LEFT, 280);
	LONG lStyle;
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT;
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);
	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_list.SetExtendedStyle(dwStyle);//设置列表风格

	((CButton*)GetDlgItem(IDC_BUTTON9))->EnableWindow(FALSE); //禁用暂停按钮
		
	/*****设置默认拦截选项*****/
	m_combo.SetCurSel(0);
	((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_EDIT1))->ShowWindow(FALSE);

	/*********注册热键********/
	HWND hWnd = GetSafeHwnd();
	RegisterHotKey(hWnd, 1001, MOD_CONTROL, 'P');
	RegisterHotKey(hWnd, 1002, MOD_CONTROL, 'H');
	 
	/******设置定时保存*******/
//	SetTimer(1, 1000, NULL);

	/******加载进程列表对话框*******/
	m_propage.Create(IDD_DIALOG1, NULL);

	/****加载窗口指示器*****/
	m_covDlg.Create(IDD_COVER_DLG, NULL);

	/*****检测开机是否自启****/
	if (CheckStartUp()) {
		autostartup = true;
		SetDlgItemText(IDC_BUTTON7, "取消开机自启");
	}

	/******注册通知区图标*****/
	m_ntIcon.cbSize = sizeof(NOTIFYICONDATA);
	m_ntIcon.hIcon = AfxGetApp()->LoadIconA(IDR_MAINFRAME);
	m_ntIcon.hWnd = this->m_hWnd;
	char atip[128] = "KillPro";
	strcpy_s(m_ntIcon.szTip, 128, atip);
	m_ntIcon.uCallbackMessage = WM_NOTIFYICON;
	m_ntIcon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	::Shell_NotifyIconA(NIM_ADD, &m_ntIcon);

	/******设置无界面启动*****/
	for (int i = 0; i < __argc; i++){
		CString strCmdParam = __argv[i];
		if (strCmdParam == "-f") {
			if (i + 1 < __argc)strDataPath = __argv[i + 1], i++;
		}
		if(strCmdParam == "--nogui") {
			SetDlgItemText(IDC_BUTTON5, _T("停止"));
			SetDlgItemText(IDC_BUTTON9, _T("暂停"));
			((CButton*)GetDlgItem(IDC_BUTTON9))->EnableWindow(TRUE);
			hidewindow = true;
			pause = false;
			isexit = false;
			ReadFromJsonFile(strDataPath);//读取数据
			if (isMTMode == false)
				CreateThread(NULL, 0, KillProSingleWorkThread, this, 0, 0);
			else {
				((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(BST_CHECKED);
				if (((CListCtrl*)GetDlgItem(IDC_LIST1))->GetItemCount() >= 64) {
					MessageBox("来自KillPro:\r\n你设置的拦截任务数过多（≥64），请使用单线程模式", "Error", MB_OK | MB_ICONERROR);
					::Shell_NotifyIcon(NIM_DELETE, &m_ntIcon);
					AfxPostQuitMessage(-1);
				}
				for (int i = 0; i < m_list.GetItemCount(); i++) {
					MultiWorkThreadParam param = { this,m_list.GetItemCount(),{
							(m_list.GetItemText(i, 0) == "窗口" ? 0 : 1),
							m_list.GetItemText(i,1),
							m_list.GetItemText(i,2) }
					};
					isThreadReady = false;
					CreateThread(NULL, 0, KillProMultiWorkThread, &param, 0, 0);
					while (!isThreadReady)Sleep(5);
				}
			}
		}
	}

	if (m_list.GetItemCount() == 0)ReadFromJsonFile(strDataPath);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CKillProDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKillProDlg::OnPaint()
{
	if (hidewindow)ShowWindow(SW_HIDE);
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CKillProDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 当用户关闭 UI 时，如果控制器仍保持着它的某个
//  对象，则自动化服务器不应退出。  这些
//  消息处理程序确保如下情形: 如果代理仍在使用，
//  则将隐藏 UI；但是在关闭对话框时，
//  对话框仍然会保留在那里。

void CKillProDlg::OnClose()
{
	UnregisterHotKey(GetSafeHwnd(), 1001);
	UnregisterHotKey(GetSafeHwnd(), 1002);
	::Shell_NotifyIcon(NIM_DELETE, &m_ntIcon);
	if (CanExit())
		CDialogEx::OnClose();
}

void CKillProDlg::OnOK()
{
	if (GetFocus() == GetDlgItem(IDC_EDIT1) || GetFocus() == GetDlgItem(IDC_EDIT3) || GetFocus() == GetDlgItem(IDC_EDIT4)) {
		OnBnClickedButton1();
		return;
	}
}

void CKillProDlg::OnCancel()
{
}

BOOL CKillProDlg::CanExit()
{
	// 如果代理对象仍保留在那里，则自动化
	//  控制器仍会保持此应用程序。
	//  使对话框保留在那里，但将其 UI 隐藏起来。
	if (m_pAutoProxy != nullptr)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}



CString mProgressName, mProgressTitle;//For WorkThread


LRESULT CKillProDlg::OnAddList(WPARAM wParam, LPARAM lParam)//将读取到的名字添加到列表
{
	if (m_combo.GetCurSel() == 0) {
		SetDlgItemText(IDC_EDIT3, mProgressName);
		SetDlgItemText(IDC_EDIT4, mProgressTitle);
	}
	else SetDlgItemText(IDC_EDIT1, mProgressName);
	((CButton*)GetDlgItem(IDC_BUTTON2))->EnableWindow(TRUE);//将“手动选择”按钮启用
	return LRESULT();
}

LRESULT CKillProDlg::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
	switch (lParam) {
	case WM_LBUTTONDOWN: {
		if (hidewindow) {
			ShowWindow(SW_SHOW); 
			hidewindow = false;
		}
		else {
			ShowWindow(SW_HIDE);
			hidewindow = true;
		}
		break;
	}
	case WM_RBUTTONDOWN: {
		CMenu popMenu;
		popMenu.LoadMenuA(IDR_MENU2);
		CMenu* pmenu = popMenu.GetSubMenu(0);
		pmenu->ModifyMenu(0, MF_BYPOSITION, ID_32777, (hidewindow ? "显示主界面" : "隐藏主界面"));
		pmenu->ModifyMenu(1, MF_BYPOSITION, ID_32778, (pause ? "继续" : "暂停"));
		pmenu->EnableMenuItem(ID_32778, (isexit ? MF_GRAYED : MF_ENABLED));
		CPoint pos;
		GetCursorPos(&pos);
		pmenu->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, AfxGetMainWnd(), 0);
		break;
	}
	}
	return LRESULT();
}

/*	旧版文件存储
void CKillProDlg::SaveFile() {
	USES_CONVERSION;
	std::ofstream save("ProcessName.dat");
	CString str;
	char* print;
	for (int i = 0; i < cnt; i++) {
		str = m_list.GetItemText(i, 0);
		print = T2A(str);
		save << print << std::endl;
	}
	save.close();
	return;
}
*/

CString BootFileDialog(BOOL bOpenFileDialog, CString lpszFilter) {
	CString strFile = _T("");

	CFileDialog dlgFile(bOpenFileDialog, NULL, NULL, OFN_HIDEREADONLY, lpszFilter, NULL);

	if (dlgFile.DoModal() == IDOK)
	{
		strFile = dlgFile.GetPathName();
	}

	strFile += (bOpenFileDialog == FALSE ? (dlgFile.GetFileExt() == "" ? ".json" : "") : "");

	return strFile;
}


LRESULT CKillProDlg::OnHotKey(WPARAM wParam, LPARAM lParam)//热键处理函数
{
	switch (wParam) {
	case 1001: {//如果按下Ctrl+P
		pause = !pause;
		if (pause) {
			SetDlgItemText(IDC_BUTTON9, _T("继续"));
		}
		else
			SetDlgItemText(IDC_BUTTON9, _T("暂停"));
		break;
	}
	case 1002: {//如果按下Ctrl+H
		hidewindow = !hidewindow;
		if (hidewindow)ShowWindow(SW_HIDE);
		else ShowWindow(SW_SHOW);
		break;
	}
	}
	return LRESULT();
}


CString DisplayProcessName(DWORD processID)//通过进程ID搜索进程名并存储到name变量
{
	CString mPrName;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("CreateToolhelp32Snapshot调用失败!\n");
		return "";
	}
	//遍历进程快照。轮流显示每个进程的信息  
	CString strPrcNameID;
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		if (pe32.th32ProcessID == processID) {
			mPrName = pe32.szExeFile;
			break;
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	//清除snapshot对象  
	::CloseHandle(hProcessSnap);
	return mPrName;
}


DWORD WINAPI AutoGetProcInfo(LPVOID lpParam) {//“手动选择”实现函数
	WindowFinderThreadParam mParam = *(WindowFinderThreadParam*)lpParam;
	isFindThreadReady = true;
	HWND hwndPointBak = NULL;
	while (true)
	{
		mParam.p_CovDlg->ShowWindow(TRUE);
		Sleep(100);
		POINT pNow = { 0,0 };
		if (GetCursorPos(&pNow)) // 获取鼠标当前位置
		{
			HWND hwndPointNow = NULL;
			hwndPointNow = WindowFromPoint(pNow); // 获取鼠标所在窗口的句柄
			if (hwndPointNow)
			{
				//AfxMessageBox(L"Success!");
				char szWindowTitle[128];
				CRect rectWnd;
				CString strShow;
				if (hwndPointNow != hwndPointBak) {
					hwndPointBak = hwndPointNow;
					GetWindowRect(hwndPointNow, rectWnd);
					mParam.p_CovDlg->MoveWindow(rectWnd);
					::GetWindowText(hwndPointNow, szWindowTitle, sizeof(szWindowTitle)); // 获取窗口标题
					DWORD dwProcId = 0;//存放返回的进程ID
					DWORD dwThreadId = 0;//存放返回的主线程ID
					dwThreadId = GetWindowThreadProcessId(hwndPointNow, &dwProcId);//同时得到进程ID和主线程ID.
					mProgressName = DisplayProcessName(dwProcId);
					mProgressTitle = szWindowTitle;
					strShow.Format("进程：%s\r\n窗体名：%s", mProgressName, mProgressTitle);
					mParam.p_CovDlg->GetDlgItem(IDC_STATIC)->SetWindowTextA(strShow);
				}
				if (KEY_DOWN(VK_LBUTTON)) {
					mParam.p_CovDlg->ShowWindow(FALSE);
					mParam.p_Dlg->SendMessage(WM_ADDNAME, 0, 0);
					Sleep(1000);
					::SetWindowPos(mParam.p_Dlg->m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					return 0;
				}
			}
		}
	}
	return 0;
}


void CKillProDlg::OnBnClickedCancel()//退出程序
{
	// TODO: 在此添加控件通知处理程序代码
	/*取消注册热键*/
	UnregisterHotKey(GetSafeHwnd(), 1001);
	UnregisterHotKey(GetSafeHwnd(), 1002);

	::Shell_NotifyIcon(NIM_DELETE, &m_ntIcon);

	PostQuitMessage(0);
	CDialogEx::OnCancel();
}


void CKillProDlg::OnBnClickedButton2()//“手动选择”实现函数
{
	// TODO: 在此添加控件通知处理程序代码
	this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	WindowFinderThreadParam mParam = { this,&m_covDlg };
	isFindThreadReady = false;
	CreateThread(NULL, 0, AutoGetProcInfo, &mParam, 0, 0);
	while (!isFindThreadReady)Sleep(5);
	((CButton*)GetDlgItem(IDC_BUTTON2))->EnableWindow(FALSE); //禁用按钮
	return;
}


void CKillProDlg::OnBnClickedButton1()//添加
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	std::regex regname("[^/\\:*?\"<>|]*.(exe|com)");
	switch (m_combo.GetCurSel()) {
	case 0: {
		CString mProcName, mProcTitle;
		GetDlgItemText(IDC_EDIT3, mProcName);
		GetDlgItemText(IDC_EDIT4, mProcTitle);
		if (!std::regex_match((std::string)mProcName, regname)) {
			MessageBox("错误：输入的进程名非法", "Error", MB_OK | MB_ICONERROR);
			return;
		}
		m_list.InsertItem(m_list.GetItemCount(), "窗口");
		m_list.SetItemText(m_list.GetItemCount() - 1, 1, mProcName);
		m_list.SetItemText(m_list.GetItemCount() - 1, 2, mProcTitle);
		break;
	}
	case 1: {
		CString mProcName;
		GetDlgItemText(IDC_EDIT1, mProcName);
		if (!std::regex_match((std::string)mProcName, regname)) {
			MessageBox("错误：输入的进程名非法", "Error", MB_OK | MB_ICONERROR);
			return;
		}
		m_list.InsertItem(m_list.GetItemCount(), "进程");
		m_list.SetItemText(m_list.GetItemCount() - 1, 1, mProcName);
		break;
	}
	}
	WriteToJsonFile(strDataPath);
	return;
}


//void CKillProDlg::OnBnClickedButton3()//删除数据
//{
//	// TODO: 在此添加控件通知处理程序代码
//	int pos = (int)m_list.GetFirstSelectedItemPosition();
//	if (pos == 0) {
//		AfxMessageBox(_T("请先选中数据项"));
//		return;
//	}
//	m_list.DeleteItem(pos - 1);
//	WriteToJsonFile();
//	return;
//}


//void CKillProDlg::OnBnClickedButton4()//清空列表
//{
//	// TODO: 在此添加控件通知处理程序代码
//	m_list.DeleteAllItems();
//	cnt = 0;
//	WriteToJsonFile();
//	return;
//}


//void CKillProDlg::OnBnClickedButton6()//重载列表
//{
//	// TODO: 在此添加控件通知处理程序代码
//	m_list.DeleteAllItems();
//	ReadFromJsonFile();
//	return;
//}


//void CKillProDlg::OnBnClickedButton8()//隐藏主程序
//{
//	// TODO: 在此添加控件通知处理程序代码
//	hidewindow = true;
//	ShowWindow(SW_HIDE);
//	return;
//}

void CKillProDlg::OnBnClickedButton5()//开始按钮
{
	// TODO: 在此添加控件通知处理程序代码
	isexit = !isexit;
	if (isexit) {	//stop
		SetDlgItemText(IDC_BUTTON5, _T("开始"));
		((CButton*)GetDlgItem(IDC_BUTTON9))->EnableWindow(FALSE); 
	}
	else { //begin
		pause = false;
		if (isMTMode == false)
			CreateThread(NULL, 0, KillProSingleWorkThread, this, 0, 0);
		else {
			if (((CListCtrl*)GetDlgItem(IDC_LIST1))->GetItemCount() >= 64) {
				MessageBox("你设置的拦截任务数过多（≥64），请使用单线程模式", "Error", MB_OK | MB_ICONERROR);
				return;
			}
			for (int i = 0; i < m_list.GetItemCount(); i++) {
				MultiWorkThreadParam param = { this,m_list.GetItemCount(),{
							(m_list.GetItemText(i, 0) == "窗口" ? 0 : 1),
							m_list.GetItemText(i,1),
							m_list.GetItemText(i,2) }
				};
				isThreadReady = false;
				CreateThread(NULL, 0, KillProMultiWorkThread, &param, 0, 0);
				while (!isThreadReady)Sleep(5);
			}
		}
		SetDlgItemText(IDC_BUTTON5, _T("停止"));
		SetDlgItemText(IDC_BUTTON9, _T("暂停"));
		pause = false;
		((CButton*)GetDlgItem(IDC_BUTTON9))->EnableWindow(TRUE);
	}
}


void CKillProDlg::OnBnClickedButton9()//暂停按钮
{
	// TODO: 在此添加控件通知处理程序代码
	pause = !pause;
	if (pause) {
		SetDlgItemText(IDC_BUTTON9, _T("继续"));
	}
	else
		SetDlgItemText(IDC_BUTTON9, _T("暂停"));
	return;
}



//INT_PTR CKillProDlg::DoModal()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	ShowWindow(SW_HIDE);
//	return CDialogEx::DoModal();
//}


//int CKillProDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	// TODO:  在此添加您专用的创建代码
//	if (hidewindow)ShowWindow(SW_HIDE);
//
//	return 0;
//}

CString GetWorkDir()
{
	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	path.ReleaseBuffer();
	int pos = path.ReverseFind('\\');
	path = path.Left(pos);
	return path;
}

void CKillProDlg::OnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	autostartup = !autostartup;
	SetStartUp(autostartup);
	SetDlgItemText(IDC_BUTTON7, autostartup ? "取消开机自启" : "开机自启动");
	
	/******************************************旧版本 开机自启动
	if (!autostartup) {//如果非开机自启
		autostartup = true;//标记已开机自启
		USES_CONVERSION;
		CString path;
		CString command,runpath;
		path.Format("%s --nogui", AfxGetApp()->m_pszExeName);
		runpath.Format("cd /d \"%s\"", GetWorkDir());
		command.Format("start \"KillProAutoStart\" %s", path);
		std::ofstream ofs("AutoStartUpKillPro.bat");
		ofs << "@echo off" << std::endl;
		ofs << runpath << std::endl;
		ofs << command << std::endl;
		ofs.close();
		TCHAR Dir[_MAX_PATH];
		SHGetSpecialFolderPath(this->GetSafeHwnd(), Dir, CSIDL_STARTUP, 0);
		CString StartUpPath;
		StartUpPath.Format("%s\\StartUpKillPro.bat", Dir);//生成开机自启文件
		CopyFile("AutoStartUpKillPro.bat", StartUpPath, false);//复制开机自启文件到自启动文件夹
		SetDlgItemText(IDC_BUTTON7, "取消开机自启");
	}
	else {
		autostartup = false;
		TCHAR Dir[_MAX_PATH];
		SHGetSpecialFolderPath(this->GetSafeHwnd(), Dir, CSIDL_STARTUP, 0);
		CString StartUpPath;
		StartUpPath.Format("%s\\StartUpKillPro.bat", Dir);//生成开机自启文件
		DeleteFile(StartUpPath);
		SetDlgItemText(IDC_BUTTON7, "开机自启动");
	}
	*/
}


void CKillProDlg::On32771()
{
	// TODO: 在此添加命令处理程序代码
	int pos = (int)m_list.GetFirstSelectedItemPosition();
	if (pos == 0) {
		AfxMessageBox(_T("请先选中数据项"));
		return;
	}
	m_list.DeleteItem(pos - 1);
}


void CKillProDlg::On32772()
{
	// TODO: 在此添加命令处理程序代码
	m_list.DeleteAllItems();
	return;
}


void CKillProDlg::On32773()
{
	// TODO: 在此添加命令处理程序代码
	m_list.DeleteAllItems();
	ReadFromJsonFile(strDataPath);
	return;
}


void CKillProDlg::On32774()
{
	// TODO: 在此添加命令处理程序代码
	hidewindow = true;
	ShowWindow(SW_HIDE);
	return;
}

LRESULT CKillProDlg::OnAddNamePage(WPARAM wParam, LPARAM lParam)
{
	CString* p_Str = (CString*)wParam;
	m_list.InsertItem(m_list.GetItemCount(), _T("进程"));
	m_list.SetItemText(m_list.GetItemCount() - 1, 1, *p_Str);
	return LRESULT();
}


void CKillProDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_propage.ShowWindow(TRUE);
}


void CKillProDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	switch (m_combo.GetCurSel()) {
	case 0: {
		((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT1))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT3))->ShowWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT4))->ShowWindow(TRUE);
		break;
	}
	case 1: {
		((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT1))->ShowWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT3))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT4))->ShowWindow(FALSE);
		break;
	}
	}
	SetDlgItemText(IDC_EDIT1, "");
	SetDlgItemText(IDC_EDIT3, "");
	SetDlgItemText(IDC_EDIT4, "");
	return;
}


void CKillProDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	WriteToJsonFile(strDataPath);
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CKillProDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CAboutDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(0, NULL, _T("https://github.com/3363384229/KillPro"), NULL, NULL, SW_NORMAL);
}

void CKillProDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	isMTMode = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	WriteToJsonFile(strDataPath);
}


void CKillProDlg::OnInputData()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_MINIMIZE);
	CString strInputFile = BootFileDialog(TRUE, "Json Data File (*.json)|*.json||");
	if (strInputFile == "")return;
	m_list.DeleteAllItems();
	ReadFromJsonFile(((std::string)strInputFile).c_str());
	WriteToJsonFile(strDataPath);
	ShowWindow(SW_NORMAL);
}


void CKillProDlg::OnOutputData()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_MINIMIZE);
	CString strOutputFile = BootFileDialog(FALSE, "Json Data File (*.json)|*.json|All Files (*.*)|*.*||");
	if (strOutputFile == "")return;
	WriteToJsonFile(strOutputFile);
	ShowWindow(SW_NORMAL);
}

void CKillProDlg::OnNotifyIconExit()
{
	// TODO: 在此添加命令处理程序代码
	OnClose();
	PostQuitMessage(0);
}


void CKillProDlg::OnNotifyIconPauser()
{
	// TODO: 在此添加命令处理程序代码
	pause = !pause;
	if (pause) {
		SetDlgItemText(IDC_BUTTON9, _T("继续"));
	}
	else
		SetDlgItemText(IDC_BUTTON9, _T("暂停"));
}


void CKillProDlg::OnNotifyIconShowWindow()
{
	// TODO: 在此添加命令处理程序代码
	hidewindow = !hidewindow;
	ShowWindow((hidewindow ? SW_HIDE : SW_SHOW));
}


int CKillProDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}
