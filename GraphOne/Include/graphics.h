#pragma once
#include <Windows.h>
#include <dwrite.h>
#include <graph.h>

class Graphics
{
public:
	Graphics();
	~Graphics();
	bool Init(HWND hWnd);
	void Resize(UINT width, UINT height);
	void MouseMov(int x, int y);
	void Render();

	void SetGraph(Graph* gf);
	Graph* GetGraph();

	void SetRColWidthRatio(int i10);
	int GetRColWidthRatio();

private:
	void RendGraph(RECT rect);
	void RendCoordHint(RECT rect);
	int ConvRatioToInt(int ratio10, int Max);
	float Sharp(int value);

private:
	HWND mHWnd;
	ID2D1Factory* mpFact;
	IDWriteFactory* mpFactDWrite;
	IDWriteTextFormat* mpTxtFmt;
	ID2D1HwndRenderTarget* mpRend;
	ID2D1SolidColorBrush* mpBrOne;
	Graph* mpGraph;
	int mRColWidthRatio10 = 2;
	int MousePosX = -1;
	int MousePosY = -1;
};
