#pragma once
//此功能已弃用
#ifndef UAC_H
#define UAC_H

#include <ShlObj_core.h>

// for IsUserAnAdmin()
#pragma comment (lib, "Shell32.lib")

class UAC
{
public:
	// 以管理员权限启动一个新实例
	// true-启动了新实例
	// false-未启动新实例
	static bool runAsAdmin()
	{
		if (IsUserAnAdmin())
			return false; // 当前程序正以管理员权限运行

		// 获取应用程序可执行文件的路径
		CString strFilePath = ::GetCommandLine();
		AfxMessageBox(strFilePath);
		// 以管理员权限，执行exe程序
		HINSTANCE ins = ShellExecuteA(nullptr, "runas", strFilePath,
			"runas", nullptr, SW_SHOWNORMAL);
		if (ins > (HINSTANCE)32)
			return true; // 程序新实例启动成功
		return false;
	}
};

#endif // UAC_H