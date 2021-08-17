// CoverDlg.cpp: 实现文件
//

#include "pch.h"
#include "KillPro.h"
#include "CoverDlg.h"
#include "afxdialogex.h"


// CoverDlg 对话框

IMPLEMENT_DYNAMIC(CoverDlg, CDialogEx)

CoverDlg::CoverDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COVER_DLG, pParent)
{

}

CoverDlg::~CoverDlg()
{
}

void CoverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CoverDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CoverDlg 消息处理程序


bool CoverDlg::OnSetPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), // 结构的大小  
		1, // 结构的版本  
		PFD_DRAW_TO_WINDOW | // 在窗口(而不是位图)中绘图  
		PFD_SUPPORT_OPENGL | // 支持在窗口中进行OpenGL调用  
		PFD_DOUBLEBUFFER, // 双缓冲模式  
		PFD_TYPE_RGBA, // RGBA颜色模式  
		32, // 需要32位颜色  
		0, 0, 0, 0, 0, 0, // 不用于选择模式  
		0, 0, // 不用于选择模式  
		0, 0, 0, 0, 0, // 不用于选择模式  
		16, // 深度缓冲区的大小  
		0, // 在此不使用  
		0, // 在此不使用  
		0, // 在此不使用  
		0, // 在此不使用  
		0, 0, 0 // 在此不使用  
	};
	// 选择一种与pfd所描述的最匹配的像素格式  
	// 为设备环境设置像素格式  
	int pixelformat;
	pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
	if (0 == pixelformat) return false;
	// 为设备环境设置像素格式  
	return SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd);
}

BOOL CoverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LONG para = GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE);
	para |= WS_EX_LAYERED;
	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, para);

	SetLayeredWindowAttributes(RGB(0, 0, 0), 127, LWA_ALPHA);

	GetClientRect(&m_rect);

	//	glClearColor(1, 1, 1, 1);
	
	/*
	int nWidth = GetSystemMetrics(SM_CXSCREEN);  //屏幕宽度	
	int nHeight = GetSystemMetrics(SM_CYSCREEN); //屏幕高度
	MoveWindow(0, 0, nWidth, nHeight);
	*/

	::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CoverDlg::ChangeSize(UINT nID, int x, int y)
{
	CWnd* pWnd;
	pWnd = GetDlgItem(nID);
	if (pWnd != NULL) //判断是否为空，因为在窗口创建的时候也会调用OnSize函数，但是此时各个控件还没有创建，Pwnd为空
	{
		CRect rec;
		pWnd->GetWindowRect(&rec); //获取控件变化前的大小
		ScreenToClient(&rec); //将控件大小装换位在对话框中的区域坐标
		rec.left = rec.left * x / m_rect.Width(); //按照比例调整空间的新位置
		rec.top = rec.top * y / m_rect.Height();
		rec.bottom = rec.bottom * y / m_rect.Height();
		rec.right = rec.right * x / m_rect.Width();
		pWnd->MoveWindow(rec); //伸缩控件
	}
}


void CoverDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	RECT rect;
	GetClientRect(&rect);
	dc.FillSolidRect(&rect, RGB(0, 0, 0));
}


HBRUSH CoverDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	CFont m_font;
	// TODO:  在此更改 DC 的任何特性
	LANGID langID = GetSystemDefaultLangID();
	m_font.CreatePointFont(170, (langID == 0x0804 ? "微软雅黑" : "Microsoft YaHei"));
	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
	{
		pDC->SetTextColor(RGB(255, 255, 255));	//设置控件字体颜色
		pDC->SetBkMode(TRANSPARENT);   //设置透明属性
		pDC->SelectObject(&m_font);	//控件字体&大小
		return (HBRUSH)GetStockObject(NULL_BRUSH);   //返回空画刷
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CoverDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (nType != SIZE_MINIMIZED) //判断窗口是不是最小化了，因为窗口最小化之后 ，窗口的长和宽会变成0，当前一次变化的时就会出现除以0的错误操作
	{
		GetClientRect(&m_rect); //最后要更新对话框的大小，当做下一次变化的旧坐标
		CWnd* pWnd = GetDlgItem(IDC_STATIC);
		if (pWnd != NULL)
			pWnd->MoveWindow(m_rect);
	}
}
