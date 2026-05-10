#include <graphics.h>
#ifndef APP_HEADER
#define APP_HEADER 

class APP;

LRESULT CALLBACK fnWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class APP
{

public:
	APP(HINSTANCE hInstance, LPSTR szCmdLine, int iCmdShow);
	~APP();

	int Run();
	void Init();
	
	LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	Graphics* gfx;
	Graph* mpGraph;
	HINSTANCE mHInstance;
	LPSTR mSzCmdLine;
	int mICmdShow;

	HWND mHWnd;
	WNDCLASS mWndClass;
	MSG mMsg;

	bool mInitFailure;
};


#endif // !APP_HEADER
