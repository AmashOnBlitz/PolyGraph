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
	void SetRColWidthRatio(int i10);

	int GetRColWidthRatio();

	Graph* GetGraph();

private:
	void RendGraph(RECT rect);
	void RendCoordHint(RECT rect);
	int ConvRatioToInt(int ratio10, int Max);
	float Sharp(int value);
	bool IsPointInBound(int x, int y, int yStart, int yEnd, int xStart, int xEnd);
private:
	HWND mHWnd;
	ID2D1Factory* mpFact;
	IDWriteFactory* mpFactDWrite;
	IDWriteTextFormat* mpTxtFmt;
	IDWriteTextLayout* mpTxtLyout;
	ID2D1HwndRenderTarget* mpRend;
	ID2D1SolidColorBrush* mpBrOne;
	Graph* mpGraph;
	int mRColWidthRatio10 = 2;
	int MousePosX = -1;
	int MousePosY = -1;

	//Graph Widget Related
	int xStart = 0;
	int xEnd = 0;
	int yStart = 0;
	int yEnd = 0;
	int xMid = 0;
	int yMid = 0;
};
