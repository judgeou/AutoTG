// periphery_m.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include <tlhelp32.h>
#include <commctrl.h>
#include <shellapi.h>
#include <string>
#include <fstream>
#include <iostream>
#include "periphery_m.h"
#include "initialize_ui.h"
#include "constant.h"
#include "set_autorun.h"

using namespace std;

#define MAX_LOADSTRING 100


//#pragma comment ( lib, "comctl32.lib")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	//设置库的搜索路径
	SetDllDirectory(L"lib");


	static WCHAR szAppName[] = TEXT("gpatm");

	HWND		hwnd;
	MSG			msg;
	WNDCLASS	wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_LEFT));
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = CreateSolidBrush(RGB(236, 233, 216));
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("窗口类注册失败!"), TEXT("错误"), MB_OK | MB_ICONERROR);
		return 0;
	}

	InitCommonControls();

	hwnd = CreateWindow(
		szAppName, TEXT(""),
		WS_CAPTION,
		CW_USEDEFAULT, CW_USEDEFAULT,
		300, 250,
		NULL, NULL, hInstance, NULL
	);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;

}



//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hBtn[7];
	LOGFONT		lf;
	HDC			hdc;
	PAINTSTRUCT ps;
	WCHAR		btnLab[10];
	HFONT		hFont;

	wcscpy_s(lf.lfFaceName, TEXT("Arial"));
	lf.lfWidth = 6;
	lf.lfHeight = 12;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = 0;
	lf.lfUnderline = 0;
	lf.lfStrikeOut = 0;
	lf.lfCharSet = GB2312_CHARSET;

	switch (message)
	{
	case WM_CREATE:						//创建创建时的相关初始化
		regVirtualMouseHotKey(hWnd);
		wcscpy_s(lf.lfFaceName, TEXT("Times New Roman"));
		lf.lfHeight = 16;
		lf.lfWidth = 8;
		lf.lfWeight = FW_NORMAL;
		hFont = CreateFontIndirect(&lf);
		InitWndUI(hWnd, ((LPCREATESTRUCT)lParam)->hInstance, hBtn, hFont, lf);

		if (checkAutorun())
			SendMessage(hBtn[3], BM_SETCHECK, 1, 0);

		return 0;

	case WM_COMMAND:					//处理按钮消息
		dealWithBtnMsg(hWnd, wParam, lParam, hBtn);	return 0;

	case WM_HOTKEY:						//处理热键消息
		dealWithHotKey(hWnd, wParam);			return 0;

	case WM_PAINT:						//处理重绘消息
		hdc = BeginPaint(hWnd, &ps);
		hFont = CreateFontIndirect(&lf);
		SelectObject(hdc, hFont);
		SetBkColor(hdc, RGB(236, 233, 216));
		drawTipText(hdc);
		DeleteObject(hFont);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_NOTIFY:						//处理超链接消息
		switch (((LPNMHDR)lParam)->code)
		{
		case NM_CLICK:
		case NM_RETURN:
			//ShellExecute(NULL, TEXT("open"), TEXT("http://www.dtcoming.com/"), NULL, NULL, SW_SHOW);
			break;
		}
		return 0;

	case WM_DESTROY:					//退出程序
		GetWindowText(hBtn[0], btnLab, 10);
		if (wcscmp(btnLab, TEXT("停止")) == 0)			//当热键没有被注销时
		{
			destroyRegedHotKey(hWnd);					//注销热键

		}
		PostQuitMessage(0);		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}




//注册热键
void regVirtualMouseHotKey(HWND hwnd)
{

	RegisterHotKey(hwnd, ID_HOT_WND_HIDE, MOD_CONTROL, VK_F12);			//注册 Ctrl + f12 , 显示窗口
	RegisterHotKey(hwnd, ID_HOT_STOP, MOD_CONTROL, VK_F10);			//注册 Ctrl + f10 , 启动\停止热键
	RegisterHotKey(hwnd, ID_HOT_QUICK_STOP, MOD_CONTROL, VK_F11);			//注册 Ctrl + f11 , 快速启动\停止热键
	RegisterHotKey(hwnd, ID_HOT_PAUSE, MOD_CONTROL, VK_F9);			//注册 Ctrl + f9 , 运行\暂停热键
}

//////////////////////////////////////////////////////////////////////////

//撤销注册的热键
void destroyRegedHotKey(HWND hwnd)
{
	int hotID = ID_HOT_WND_HIDE;
	for (hotID; hotID <= ID_HOT_QUICK_STOP; hotID++)
		UnregisterHotKey(hwnd, hotID);
}

//////////////////////////////////////////////////////////////////////////

//处理热键消息
void dealWithHotKey(HWND hwnd, WPARAM wParam)
{

	switch (wParam)
	{

		case ID_HOT_WND_HIDE:		//呼出主界面
		{
			ShowWindow(hwnd, SW_SHOWNORMAL);
			SetForegroundWindow(hwnd);
			break;
		}
		case ID_HOT_STOP:			//停止\启动热键
		{
			SendMessage(hwnd, WM_COMMAND, ID_BTN_STOP | BN_CLICKED, 0);
			break;
		}
		case ID_HOT_PAUSE:			//暂停\运行热键
		{
			SendMessage(hwnd, WM_COMMAND, ID_BTN_PAUSE | BN_CLICKED, 0);
			break;
		}
		case ID_HOT_QUICK_STOP:			//停止\快速启动热键
		{
			SendMessage(hwnd, WM_COMMAND, ID_BTN_STOP | BN_CLICKED, 1);
			break;
		}
	}

}

//////////////////////////////////////////////////////////////////////////



bool PauseInterpreter(HANDLE &hwt, DWORD &dwThreadId, bool &runState)
{
	DWORD  dwExitCode = 0;
	WaitForSingleObject(hwt, 100);
	GetExitCodeThread(hwt, &dwExitCode);
	if (dwExitCode == STILL_ACTIVE)
	{
		SuspendThread(hwt);
	}
	return true;
}


bool RunInterpreter(HANDLE &hwt, DWORD &dwThreadId, bool &runState)
{
	DWORD  dwExitCode = 0;
	WaitForSingleObject(hwt, 100);
	GetExitCodeThread(hwt, &dwExitCode);
	if (dwExitCode == STILL_ACTIVE)
	{
		ResumeThread(hwt);
	}
	return true;
}


bool StopInterpreter(HINSTANCE &m_hdllInst, HANDLE &hfile, HANDLE &hwt, DWORD &dwThreadId, bool &runState)
{
	DWORD  dwExitCode = 0;
	if (hwt != INVALID_HANDLE_VALUE)
	{
		WaitForSingleObject(hwt, 100);
		GetExitCodeThread(hwt, &dwExitCode);
		if (dwExitCode == STILL_ACTIVE)
		{
			TerminateThread(hwt, dwExitCode);
		}
		CloseHandle(hwt);
		runState = false;
		dwThreadId = 0;
		hwt = INVALID_HANDLE_VALUE;
	}
#if 0
	if (m_hdllInst)
	{
		FreeLibrary(m_hdllInst);
		m_hdllInst = nullptr;
	}

	if (hfile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hfile);
		FreeConsole();
	}
#endif
	return true;
}



int StartInterpreter(HINSTANCE &m_hdllInst, HANDLE &hfile, HANDLE &hwt, DWORD &dwThreadId, bool &runState, string &role, bool quick)
{
	// 加载动态库 kmd.dll
	typedef DWORD(WINAPI *gp_thr)(LPVOID lpParameter);
	typedef DWORD(WINAPI* gp_thr_q)(LPVOID lpParameter);
	typedef DWORD(WINAPI* gp_thr_no_enc)(LPVOID lpParameter);
	//DWORD WINAPI gp_thr(LPVOID lpParameter)
	static gp_thr gp_thr_v = nullptr;
	static gp_thr_q gp_thr_q_v = nullptr;
	static gp_thr_no_enc gp_thr_no_enc_v = nullptr;
	if (!m_hdllInst)
	{
		m_hdllInst = LoadLibrary(L"gpatc.dll");
		if (m_hdllInst)
		{
			//C++ name mangling or export "C"
			//gp_thr_v = (gp_thr)GetProcAddress(m_hdllInst, "_gp_thr@4");
			//gp_thr_q_v = (gp_thr_q)GetProcAddress(m_hdllInst, "_gp_thr_quick@4");
			gp_thr_no_enc_v = (gp_thr_q)GetProcAddress(m_hdllInst, "_gp_thr_no_enc@4");
		}
	}

#if 0
	if (hfile == INVALID_HANDLE_VALUE)
	{
		//AllocConsole();
		SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES),NULL,TRUE };
		hfile = CreateFile(TEXT("run.log"),
			GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			&sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		SetStdHandle(STD_OUTPUT_HANDLE, hfile);
		SetStdHandle(STD_ERROR_HANDLE, hfile);


	}
#endif


	//if (gp_thr_v && gp_thr_q_v && gp_thr_no_enc_v)
	if (gp_thr_no_enc_v)
	{
		hwt = CreateThread(NULL, 0, gp_thr_no_enc_v, (LPVOID)role.c_str(), CREATE_SUSPENDED, &dwThreadId);//创建工作线程

#if 0
		if (quick)
		{
			hwt = CreateThread(NULL, 0, gp_thr_q_v, (LPVOID)role.c_str(), CREATE_SUSPENDED, &dwThreadId);//创建工作线程
		}
		else
		{
			hwt = CreateThread(NULL, 0, gp_thr_v, (LPVOID)role.c_str(), CREATE_SUSPENDED, &dwThreadId);//创建工作线程
		}
#endif
		runState = true;
	}
	else
	{
		runState = false;
		dwThreadId = 0;
		hwt = INVALID_HANDLE_VALUE;
	}


	return 0;
}

string WStringToString(wstring ws)
{
	// 获取缓冲区大小
	int len = WideCharToMultiByte(CP_ACP, 0, ws.c_str(), ws.size(), NULL, 0, NULL, NULL);
	// 申请空间，缓冲区大小按字节计算
	char* buf = new char[len + 1];
	// 宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP, 0, ws.c_str(), ws.size(), buf, len, NULL, NULL);
	// 添加字符串结尾
	buf[len] = '\0';
	string s;
	s.append(buf);
	// 删除缓冲区
	if (buf) delete[] buf;

	return s;
}//处理按钮消息
void dealWithBtnMsg(HWND hwnd, WPARAM wParam, LPARAM lParam, HWND *hBtn)
{
	WCHAR btnLab[10];
	WCHAR roleNum[10];
	static DWORD dwThreadId;
	static HINSTANCE m_hdllInst = nullptr;
	static HANDLE hwt = INVALID_HANDLE_VALUE;
	static HANDLE hfile = INVALID_HANDLE_VALUE;
	static bool  runSate = false;
	static string role;
	switch (LOWORD(wParam))
	{
	case ID_BTN_HIDE:			//处理隐藏按钮消息
		ShowWindow(hwnd, SW_MINIMIZE);		//先最小化
		ShowWindow(hwnd, SW_HIDE);			//再隐藏
		return;

	case ID_BTN_STOP:			//处理停止\启动
		GetWindowText(hBtn[0], btnLab, 10);
		if (wcscmp(btnLab, TEXT("停止")) == 0)
		{
			if (StopInterpreter(m_hdllInst, hfile, hwt, dwThreadId, runSate))
			{
				SetWindowText(hBtn[0], TEXT("启动"));
				SetWindowText(hBtn[6], TEXT("运行"));
			}
		}
		else
		{
			GetWindowText(hBtn[5], roleNum, 10);
			role = WStringToString(roleNum);
			//MessageBox(hwnd, roleNum, L"提示", MB_OK | MB_ICONINFORMATION);
			if (lParam == 1)
			{
				StartInterpreter(m_hdllInst, hfile, hwt, dwThreadId, runSate, role, true);
			}
			else
			{
				StartInterpreter(m_hdllInst, hfile, hwt, dwThreadId, runSate, role, false);
			}
			SetWindowText(hBtn[0], TEXT("停止"));

		}
		return;
	case ID_BTN_PAUSE:			//处理暂停\运行
		if (runSate)
		{
			GetWindowText(hBtn[6], btnLab, 10);
			if (wcscmp(btnLab, TEXT("暂停")) == 0)
			{
				if (PauseInterpreter(hwt, dwThreadId, runSate))
				{
					SetWindowText(hBtn[6], TEXT("运行"));
				}
			}
			else
			{
				if (RunInterpreter(hwt, dwThreadId, runSate))
				{
					SetWindowText(hBtn[6], TEXT("暂停"));
					SendMessage(hwnd, WM_COMMAND, ID_BTN_HIDE | BN_CLICKED, 0);
				}
			}
		}
		return;
	case ID_BTN_EXIT:			//处理退出消息
		if (runSate)
		{
			StopInterpreter(m_hdllInst, hfile, hwt, dwThreadId, runSate);
		}
		SendMessage(hwnd, WM_DESTROY, 0, 0);
		return;

	case ID_ID_AUTORUN:			//处理开机自启动消息
		VirtualMouseAutorun(hBtn[3], runSate);
		return;
	case ID_EDIT_ROLE:			//第几个角色
		return;
	}
}

//////////////////////////////////////////////////////////////////////////

//绘制提示信息
void drawTipText(HDC hdc)
{
	int i = 1, x = 10, y = 30;

	TCHAR szTip[][128] = {
		TEXT(" 选择角色:"),
		TEXT("________________________"),
		TEXT(" 运行/暂停: Ctrl + F9"),
		TEXT(" 启动/停止: Ctrl + F10"),
		TEXT(" 快启/停止: Ctrl + F11"),
		TEXT(" 唤出 窗口: Ctrl + F12"),
		TEXT("________________________"),
	};
	TextOut(hdc, 10, 13, szTip[0], lstrlen(szTip[0]));
	for (i; i < sizeof(szTip) / sizeof(szTip[0]); i++)
	{
		TextOut(hdc, x, y, szTip[i], lstrlen(szTip[i]));
		y += 20;
	}
}
