#pragma once
//�˹���������
#ifndef UAC_H
#define UAC_H

#include <ShlObj_core.h>

// for IsUserAnAdmin()
#pragma comment (lib, "Shell32.lib")

class UAC
{
public:
	// �Թ���ԱȨ������һ����ʵ��
	// true-��������ʵ��
	// false-δ������ʵ��
	static bool runAsAdmin()
	{
		if (IsUserAnAdmin())
			return false; // ��ǰ�������Թ���ԱȨ������

		// ��ȡӦ�ó����ִ���ļ���·��
		CString strFilePath = ::GetCommandLine();
		AfxMessageBox(strFilePath);
		// �Թ���ԱȨ�ޣ�ִ��exe����
		HINSTANCE ins = ShellExecuteA(nullptr, "runas", strFilePath,
			"runas", nullptr, SW_SHOWNORMAL);
		if (ins > (HINSTANCE)32)
			return true; // ������ʵ�������ɹ�
		return false;
	}
};

#endif // UAC_H