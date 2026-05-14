#pragma once
#include <Windows.h>
#include <dwrite.h>
#include <graph.h>
#include "CheckBox.h"

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
	void RendWidgetsPanel(RECT rect);
	void RendConfigPanel(D2D1_RECT_F configPanel, int yStart, int xStart);
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
	const int widgetPanelPadding = 10;
	//Graph Widget Related
	int gf_xStart = 0;
	int gf_xEnd = 0;
	int gf_yStart = 0;
	int gf_yEnd = 0;
	int gf_xMid = 0;
	int gf_yMid = 0;
};
