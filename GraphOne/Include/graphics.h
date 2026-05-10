#pragma once
#include <Windows.h>
#include <graph.h>

class Graphics
{
public:
	Graphics();
	~Graphics();
	bool Init(HWND hWnd);
	void Resize(UINT width, UINT height);
	void Render();

	void SetGraph(Graph* gf);
	Graph* GetGraph();

	void SetRColWidthRatio(int i10);
	int GetRColWidthRatio();

private:
	void RendGraph(RECT rect);
	int ConvRatioToInt(int ratio10, int Max);
private:
	HWND mHWnd;
	ID2D1Factory* mpFact;
	ID2D1HwndRenderTarget* mpRend;
	ID2D1SolidColorBrush* mpBrOne;
	Graph* mpGraph;
	int mRColWidthRatio10 = 2;
};
