#include "pch.h"
#include "App.h"

#define ERR_CHAR UniversalErrChar
#define ERR_CHAR_LIMIT 1028
#define RESERVE_ERR_CHAR static TCHAR ERR_CHAR[ERR_CHAR_LIMIT];

#define MAIN_EXIT_FAILURE 1
#define MAIN_EXIT_SUCCESS 0

APP::APP(HINSTANCE hInstance, LPSTR szCmdLine, int iCmdShow) :
	gfx(nullptr),
	mHInstance(hInstance),
	mSzCmdLine(szCmdLine),
	mICmdShow(iCmdShow),
	mHWnd(NULL),
	mWndClass({}),
	mMsg({}),
	mInitFailure(false),
	mpGraph(nullptr)
{

}

APP::~APP()
{
}

void APP::Init()
{
	TCHAR szClassName[] = _T("Graph App Window Class");
	TCHAR szAppName[] = _T("Poly Graph");
	mWndClass.cbClsExtra = 0;
	mWndClass.cbWndExtra = 0;
	mWndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	mWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	mWndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	mWndClass.hInstance = mHInstance;
	mWndClass.lpfnWndProc = fnWinProc;
	mWndClass.lpszClassName = szClassName;
	mWndClass.lpszMenuName = NULL;
	mWndClass.style = CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClass(&mWndClass)) {
		RESERVE_ERR_CHAR;
		StringCchPrintf(
			ERR_CHAR,
			ERR_CHAR_LIMIT,
			_T("RegisterClass Failed!\nCannot Register Window of Class Name : %s"),
			mWndClass.lpszClassName
		);
		MessageBox(
			NULL,
			UniversalErrChar,
			_T("Failure in Register Class"),
			MB_ICONERROR | MB_OK
		);
		mInitFailure = true;
		return;
	}

	mHWnd = CreateWindow(
		szClassName,
		szAppName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		mHInstance,
		this
	);
	if (mHWnd == NULL || mHWnd == INVALID_HANDLE_VALUE) {
		RESERVE_ERR_CHAR;
		StringCchPrintf(
			ERR_CHAR,
			ERR_CHAR_LIMIT,
			_T("CreateWindow Failed!\nCannot Create Window of Class Name : %s\nWindow Name : %s"),
			mWndClass.lpszClassName,
			szAppName
		);
		MessageBox(
			NULL,
			UniversalErrChar,
			_T("Failure in Create Window"),
			MB_ICONERROR | MB_OK
		);
		mInitFailure = true;
		return;
	}
	mpGraph = new Graph();
	mInitFailure = false;
}

LRESULT APP::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_SIZE:
	{
		if (gfx)
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			gfx->Resize(width, height);
		}
		return 0;
	}
	case WM_PAINT: {
		BeginPaint(hWnd, &ps);
		if (gfx) gfx->Render();
		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int APP::Run()
{	
	if (mInitFailure) return MAIN_EXIT_FAILURE;
	UpdateWindow(mHWnd);
	ShowWindow(mHWnd, mICmdShow);
	if (!gfx) gfx = new Graphics();
	if (mpGraph) gfx->SetGraph(mpGraph);
	gfx->Init(mHWnd);
	while (GetMessageA(&mMsg, NULL, 0, 0)) {
		TranslateMessage(&mMsg);
		DispatchMessage(&mMsg);
	}
	return MAIN_EXIT_SUCCESS;
}

LRESULT fnWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	APP* pApp = nullptr;

	if (msg == WM_NCCREATE) {
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		pApp = reinterpret_cast<APP*>(cs->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp));
	}
	else {
        pApp = reinterpret_cast<APP*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }
	if (pApp) {
		return pApp->WinProc(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
