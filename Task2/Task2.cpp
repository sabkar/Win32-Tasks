#include "stdafx.h"
#include "Window.h"
#include "WinApp.h"
#include "WndProc.h"

HWND CreateMainWindow();
HWND CreateChildWindow(Window* parent);
HWND CreatePopupWindow(Window* parent);

// --- ���������� ���������� ����������
TCHAR* ClassName = _T("WindowClass"); // �������� ������ ����
TCHAR* PopupClassName = _T("PopupWindow");
TCHAR* ChildClassName = _T("ChildWwindow");

TCHAR* AppTitle = _T("������� ����"); // ��������� �����. ����

HINSTANCE hInstance;

// --- ������� ������� ���������� WinMain
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// ��������� �� ������ ������ Window - ������� ���� ����������
	Window* pMainWindow;
	Window* pPopupWindow;
	Window* pChildWindow;

	HWND hMainWnd;
	// --- ��������, ���� �� ���������� �������� �����
	if((hMainWnd = FindWindow(ClassName, NULL)) != NULL) {
		// ���� ������. ���� �������� �����, ��������������
		// � ��������� �� �������� ���� ��� ������� ����
		if(IsIconic(hMainWnd))
			ShowWindow(hMainWnd, SW_RESTORE);
		SetForegroundWindow(hMainWnd);
		// ������ ����� ����� ������������
		return FALSE;
	}

	// ������� ������ ������ WinApp - ���� ����������
	WinApp App(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	// ������������ ����
	App.RegisterWndClass(ClassName, MainWndProc);
	App.RegisterWndClass(PopupClassName, PopupWndProc);
	App.RegisterWndClass(ChildClassName, ChildWndProc);
	if(App.Error()) return App.Error();
	// ������� ����
	pMainWindow = new Window(CreateMainWindow());
	pPopupWindow = new Window(CreatePopupWindow(pMainWindow));
	pChildWindow = new Window(CreateChildWindow(pMainWindow));		
	// ��������� ������, ������� ����� ���������� ��� �������� ����
	if(pMainWindow->Error()) pMainWindow->Error();
	if(pChildWindow->Error()) pChildWindow->Error();
	if(pPopupWindow->Error()) pPopupWindow->Error();
	// ���������� ����
	pMainWindow->Show(SW_SHOWMAXIMIZED);
	pChildWindow->Show(nCmdShow);
	pPopupWindow->Show(nCmdShow);
	// �������� � ���� ���������
	pMainWindow->Update();
	pChildWindow->Update();
	pPopupWindow->Update();
	// ��������� ����������
	App.Go();
	// ��������� ������
	return App.Error();
}

HWND CreateMainWindow() {
	return CreateWindow(
		ClassName,         // ��� ������ ����
		AppTitle,       // ��������� ����
		WS_OVERLAPPEDWINDOW, // ����� ����
		CW_USEDEFAULT,       // ������ ������� � ������������
		CW_USEDEFAULT,       // ����, �������� �� ��������� 
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,                   // ������������� ������������� ����
		0,                   // ������������� ����
		hInstance,           // ������������� ����������
		NULL);     
}

HWND CreateChildWindow(Window* parent) {
	return CreateWindow(
		ChildClassName,         // ��� ������ ����
		L"�������� ����",       // ��������� ����
		WS_CHILDWINDOW|WS_CAPTION|WS_VISIBLE, // ����� ����
		150,       // ������ ������� � ������������
		150,       // ����, �������� �� ��������� 
		250,
		250,
		parent->getHWND(),                   // ������������� ������������� ����
		0,                   // ������������� ����
		hInstance,           // ������������� ����������
		NULL);    
}

HWND CreatePopupWindow(Window* parent) {
	return CreateWindow(
		PopupClassName,         // ��� ������ ����
		L"��������� ����",       // ��������� ����
		WS_POPUPWINDOW|WS_CAPTION|WS_VISIBLE, // ����� ����
		300,       // ������ ������� � ������������
		100,       // ����, �������� �� ��������� 
		300,
		300,
		parent->getHWND(),                   // ������������� ������������� ����
		0,                   // ������������� ����
		hInstance,           // ������������� ����������
		NULL);    
}

//VOID InitMainWnd(HINSTANCE hInstance, int nCmdShow) {
//	WNDCLASS wc;
//
//	ZeroMemory(&wc, sizeof(wc)); // ������� ����� ���������
//	// ���������� ����� ���-�� WNDCLASS
//	wc.lpszClassName = ClassName; // ��� ������ ����
//	//����� ������� �-���
//	wc.lpfnWndProc = (WNDPROC) MainWndProc;
//	wc.style = CS_HREDRAW|CS_VREDRAW; // ����� ������ ����
//	wc.hInstance = hInstance; // ��������� ����������
//	// ����������� ��� ����
//	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//	// ������ ���� ��� ����
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//	// ����� ��� ����
//	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
//	wc.lpszMenuName = NULL; // ������ ���� ����
//	wc.cbClsExtra = 0; // �������������� ������ ������
//	wc.cbWndExtra = 0; // �������������� ������ ����
//	// P���������� ������ ����.
//	BOOL k = RegisterClass(&wc);
//	if (!k) {
//		MessageBox(NULL, _T("Window create error"), AppTitle, MB_OK|MB_ICONSTOP);
//	}
//	// �������� �������� ���� ����������.
//	hMainWnd = CreateWindow(
//		ClassName, // ��� ������ ����
//		AppTitle, // ��������� ����
//		WS_OVERLAPPEDWINDOW, // ����� ����
//		CW_USEDEFAULT, // X-����������
//		CW_USEDEFAULT, // Y-����������
//		CW_USEDEFAULT, // ������ ����
//		CW_USEDEFAULT, // ������ ����
//		NULL, // ���������� ����-��������
//		NULL, // ���������� ���� ����
//		hInstance, // ���������� ���. ����������
//		NULL); // �������������� ����������
//
//	if(!hMainWnd) {
//		// ���� �� �������, �������� ���������.
//		MessageBox(NULL, _T("Window create error"), AppTitle, MB_OK|MB_ICONSTOP);
//		return;
//	}
//	// ����������� ���� � ���������� ���
//	ShowWindow(hMainWnd, nCmdShow);
//	UpdateWindow(hMainWnd);
//}
//
//VOID InitPopupWnd(HINSTANCE hInstance, int nCmdShow) {
//	WNDCLASS wc;
//	HWND hWndPopup;
//
//	// ���������� ��������� WNDCLASS ��� ����������� ������ ������������ ����.
//	memset(&wc, 0, sizeof(wc));
//	wc.lpszClassName = PopupClassName;			// ��� ������ ����
//	wc.lpfnWndProc = (WNDPROC) PopupWndProc;			// ����� ������� �������
//	wc.style = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;			// ����� ������ ����
//	wc.hInstance = hInstance;				// ��������� ����������
//	wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);		// ����������� ��� ����
//	wc.hCursor = LoadCursor(NULL,IDC_ARROW);			// ������ ���� ��� ����
//	wc.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE);	// ����� ��� ����
//	wc.lpszMenuName = NULL;					// ������ ���� ����
//	wc.cbClsExtra = 0;					// �������������� ������
//	wc.cbWndExtra = 0;					// �������������� ������
//
//	// P���������� ������ ����.
//	BOOL k = RegisterClass(&wc);
//
//	if (!k) {
//		MessageBox(NULL, _T("Window create error"), AppTitle, MB_OK|MB_ICONSTOP);
//	}
//
//	int rx = 1 + (rand() % (GetSystemMetrics(SM_CXSCREEN) - 300) - 1);
//	int ry = rand() % (GetSystemMetrics(SM_CYSCREEN) - 300) + 1;
//
//	// �������� ���������� ����
//	hWndPopup = CreateWindow(
//		PopupClassName,					// ��� ������ ����
//		_T("POPUPWINDOW"),						// ��������� ����
//		WS_POPUPWINDOW|WS_CAPTION|WS_VISIBLE,// ����� ����
//		rx, // X-����������
//		ry,// Y-����������
//		200,								// ������ ����
//		GetSystemMetrics(SM_CYCAPTION) + 50,	// ������ ����   
//		hMainWnd,							// ���������� ����-��������
//		NULL,								// ���������� ���� ����
//		hInstance,							// ���������� ���������� ����������
//		NULL);								// �������������� ����������
//	if(hWndPopup == 0) { 
//		DestroyWindow(hMainWnd); 
//		return; 
//	}
//	ShowWindow(hWndPopup, nCmdShow); 
//	UpdateWindow(hWndPopup);
//}
//
//VOID InitChildWnd(HINSTANCE hInstance, int nCmdShow) {
//	WNDCLASS wc;
//	HWND hWndChild;
//
//	// ���������� ��������� WNDCLASS ��� ����������� ������ ��������� ����.
//	memset(&wc, 0, sizeof(wc));
//	wc.lpszClassName = ChildClassName;			// ��� ������ ����
//	wc.lpfnWndProc = (WNDPROC)ChildWndProc;			// ����� ������� �������
//	wc.style = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;			// ����� ������ ����
//	wc.hInstance = hInstance;				// ��������� ����������
//	wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);		// ����������� ��� ����
//	wc.hCursor = LoadCursor(NULL,IDC_ARROW);			// ������ ���� ��� ����
//	wc.hbrBackground = (HBRUSH)(COLOR_HIGHLIGHT);		// ����� ��� ����
//	wc.lpszMenuName = NULL;					// ������ ���� ����
//	wc.cbClsExtra = 0;					// �������������� ������
//	wc.cbWndExtra = 0;					// �������������� ������
//
//	// P���������� ������ ����.
//	RegisterClass(&wc);
//
//	int width = GetSystemMetrics(SM_CXSCREEN)/16;
//	int height = GetSystemMetrics(SM_CYSCREEN)/16;
//
//	// �������� ��������� ����
//	hWndChild = CreateWindow(
//		ChildClassName,
//		L"CHILDWINDOW",
//		WS_CHILDWINDOW|WS_CAPTION|WS_VISIBLE,
//		GetSystemMetrics(SM_CXSCREEN)/2 - width/2,// X-����������
//		GetSystemMetrics(SM_CYSCREEN)/2 - height/2 ,// Y-����������
//		width,
//		height, 
//		hMainWnd, 
//		NULL, 
//		hInstance,
//		NULL);
//
//	if(!hWndChild) { 
//		//MessageBox(hMainWnd, L"�� �����", L"������", MB_HELP);
//		DestroyWindow(hMainWnd); 
//		return; 
//	} 
//	ShowWindow(hWndChild, nCmdShow); 
//	UpdateWindow(hWndChild);
//}
//
//// --- ������� ������� �������� ����
//LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//	switch(uMsg) {
//		// ������ ����� ������ ���� � ���������� ���. ����.
//	case WM_LBUTTONDOWN:
//		// ����� ���������� � ���������� � �������
//		// ����������� ���� ��������� MessageBox()
//		FLASHWINFO  fi;
//		fi.cbSize = sizeof(FLASHWINFO);
//		fi.hwnd = hWnd;
//		fi.dwFlags = FLASHW_ALL;
//		fi.uCount = 4;
//		fi.dwTimeout = 0;
//		FlashWindowEx(&fi);			
//		ShowWindow(hWnd, SW_MINIMIZE);
//		//MessageBox(hWnd, _T("Windows Aplication"), _T("MainWindowMessage"), MB_OK|MB_ICONINFORMATION);
//		break;
//		// ������������ ������ ����
//	case WM_RBUTTONDOWN:
//		Beep(2500, 1000);
//		//MessageBox(hWnd, _T("Windows Aplication"), _T("MainWindowMessage"), MB_OK|MB_ICONINFORMATION);
//		break;
//	case WM_DESTROY:
//		// ���� ��� ������� ������� �������� ����, �� � �������
//		// ��������� ���������� ��������� WM_QUIT
//		PostQuitMessage(0);
//		break;
//		// �������������� ��������� ������������ �
//		// �� � ������� ������� DefWindowProc()
//	default:
//		return DefWindowProc(hWnd, uMsg, wParam, lParam);
//	}
//	return 0;
//}
//
//WORD GetWindowWidth(HWND hwnd) {
//	RECT rect;
//	GetWindowRect(hwnd, &rect);
//	return rect.right - rect.left;
//}
//
//WORD GetWindowHeight(HWND hwnd) {
//	RECT rect;
//	GetWindowRect(hwnd, &rect);
//	return rect.bottom - rect.top;
//}
//
//LRESULT CALLBACK PopupWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//	switch(uMsg) {
//		// ������ ����� ������ ���� � ���������� ���. ����.
//	case WM_LBUTTONDBLCLK:
//		MessageBox(hMainWnd, L"������ ���� �������� �����������. ��� ������� ������� ������ ������� ���� ������������ �� ������� �������", L"����������", MB_OK | MB_ICONINFORMATION );
//		break;
//		// ������������ ������ ����
//	case WM_RBUTTONDBLCLK:
//		WORD xPos, yPos, newX, newY, width, height;
//		// ��������� ���������� ������� ����
//		POINT pt;
//		GetCursorPos(&pt);
//		xPos = pt.x;
//		yPos = pt.y;
//		width = GetWindowWidth(hWnd);
//		height = GetWindowHeight(hWnd);
//		newX = xPos - width / 2;
//		newY = yPos - height / 2 - GetSystemMetrics(SM_CYCAPTION) / 2;
//
//		MoveWindow(hWnd, newX, newY, width, height, FALSE);
//		break;
//	case WM_DESTROY:
//		// ���� ��� ������� ������� �������� ����, �� � �������
//		// ��������� ���������� ��������� WM_QUIT
//		//PostQuitMessage(0);
//		break;
//		// �������������� ��������� ������������ �
//		// �� � ������� ������� DefWindowProc()
//	default:
//		return DefWindowProc(hWnd, uMsg, wParam, lParam);
//	}
//	return 0;
//}
//
//
//
//LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//	switch(uMsg) {
//		// ������ ����� ������ ���� � ���������� ���. ����.
//	case WM_LBUTTONDOWN:
//		WORD MWH, width, height, newY, newX;
//		MWH = GetWindowHeight(hMainWnd);
//		width = GetWindowWidth(hWnd);
//		height = GetWindowHeight(hWnd);
//		newY = MWH - width;
//		MoveWindow(hWnd, 0, newY, width, height, true);
//		break;
//		// ������������ ������ ����
//	case WM_RBUTTONDOWN:
//		width = GetWindowWidth(hWnd);
//		height = GetWindowHeight(hWnd);
//		newX = GetSystemMetrics(SM_CXSCREEN)/2 - width/2,// X-����������
//			newY = GetSystemMetrics(SM_CYSCREEN)/2 - height/2 ,// Y-����������
//			MoveWindow(hWnd, newX, newY, width, height, true);			
//		break;
//	case WM_DESTROY:
//		// ���� ��� ������� ������� �������� ����, �� � �������
//		// ��������� ���������� ��������� WM_QUIT
//		PostQuitMessage(0);
//		break;
//		// �������������� ��������� ������������ �
//		// �� � ������� ������� DefWindowProc()
//	default:
//		return DefWindowProc(hWnd, uMsg, wParam, lParam);
//	}
//}