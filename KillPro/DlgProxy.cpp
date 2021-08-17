
// DlgProxy.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "KillPro.h"
#include "DlgProxy.h"
#include "KillProDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKillProDlgAutoProxy

IMPLEMENT_DYNCREATE(CKillProDlgAutoProxy, CCmdTarget)

CKillProDlgAutoProxy::CKillProDlgAutoProxy()
{
	EnableAutomation();

	// 为使应用程序在自动化对象处于活动状态时一直保持
	//	运行，构造函数调用 AfxOleLockApp。
	AfxOleLockApp();

	// 通过应用程序的主窗口指针
	//  来访问对话框。  设置代理的内部指针
	//  指向对话框，并设置对话框的后向指针指向
	//  该代理。
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CKillProDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CKillProDlg)))
		{
			m_pDialog = reinterpret_cast<CKillProDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CKillProDlgAutoProxy::~CKillProDlgAutoProxy()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	//	析构函数调用 AfxOleUnlockApp。
	//  除了做其他事情外，这还将销毁主对话框
	if (m_pDialog != nullptr)
		m_pDialog->m_pAutoProxy = nullptr;
	AfxOleUnlockApp();
}

void CKillProDlgAutoProxy::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CKillProDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CKillProDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_IKillPro 的支持来支持类型安全绑定
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {9f2c5c0d-e5fb-45d2-a011-f1ba402854ef}
static const IID IID_IKillPro =
{0x9f2c5c0d,0xe5fb,0x45d2,{0xa0,0x11,0xf1,0xba,0x40,0x28,0x54,0xef}};

BEGIN_INTERFACE_MAP(CKillProDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CKillProDlgAutoProxy, IID_IKillPro, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 宏是在此项目的 pch.h 中定义的
// {a33eff31-ddbc-490e-848d-aaa811bdaf27}
IMPLEMENT_OLECREATE2(CKillProDlgAutoProxy, "KillPro.Application", 0xa33eff31,0xddbc,0x490e,0x84,0x8d,0xaa,0xa8,0x11,0xbd,0xaf,0x27)


// CKillProDlgAutoProxy 消息处理程序
