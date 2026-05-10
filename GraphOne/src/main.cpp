#include "pch.h"
#include "app.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	APP App(hInstance,szCmdLine,iCmdShow);
	App.Init();
	return App.Run();
}