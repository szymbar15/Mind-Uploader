// Project3.cpp : Defines the entry point for the application.
//

#include "header.h"
#include "MindUploader.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR errorName[MAX_LOADSTRING];				// debug

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_STRING134, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PROJECT3, szWindowClass, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_STRING104, errorName, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT3));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT3));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
	wcex.hbrBackground = /*brush*/(HBRUSH)(COLOR_WINDOWTEXT + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PROJECT3);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	HWND hWnd;
	int HorzRes = GetDeviceCaps(GetDC(NULL), HORZRES);
	int VertRes = GetDeviceCaps(GetDC(NULL), VERTRES);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			hWnd = CreateWindow(szWindowClass, 0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, i * HorzRes / 3, j * VertRes / 3, HorzRes / 3, VertRes / 3, NULL, NULL, hInstance, NULL);
			SetMenu(hWnd, NULL);
			::SetWindowLong(hWnd, GWL_STYLE, ::GetWindowLong(hWnd, GWL_STYLE) & ~(WS_BORDER | WS_DLGFRAME | WS_THICKFRAME));
			::SetWindowLong(hWnd, GWL_EXSTYLE, ::GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_DLGMODALFRAME);
		}
	}

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int AnimationFrame = 0;
	static int WindowSync = 0;
	switch (message)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 300, NULL);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		TCHAR s[] = _T("Hello world!");
		HBITMAP bitmap;
		WCHAR a[MAX_LOADSTRING];
		switch (AnimationFrame) {
		case 0:
			bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
			break;
		case 1:
			bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
			break;
		case 2:
			bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
			break;
		}
		HDC memDC = CreateCompatibleDC(hdc);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, bitmap);
		swprintf_s(a, L"%d", AnimationFrame);
		StretchBlt(hdc, 0, 0, GetDeviceCaps(GetDC(NULL), HORZRES) / 3, GetDeviceCaps(GetDC(NULL), VERTRES) / 3, memDC, 0, 0, 153, 108, SRCCOPY);

		SelectObject(memDC, oldBitmap);
		DeleteObject(bitmap);
		DeleteDC(memDC);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		KillTimer(hWnd, WM_TIMER);
		PostQuitMessage(0);
		break;

	case WM_TIMER:
	{
		WindowSync++;
		if (WindowSync == 9) {
			if (AnimationFrame >= 2) AnimationFrame = 0;
			else AnimationFrame++;
			WindowSync = 0;
		}
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);

		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
