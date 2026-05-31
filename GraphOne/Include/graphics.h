#pragma once
#include <Windows.h>
#include <dwrite.h>
#include <graph.h>
#include "CheckBox.h"
#include <DigitBox.h>
#include <vector>

class Graphics
{
public:
	Graphics();
	~Graphics();
	bool Init(HWND hWnd);

	void Resize(UINT width, UINT height);
	void MouseMov(int x, int y);
	void MouseUp(int x, int y);
	void Render();
	void SetGraph(Graph* gf);
	void SetRColWidthRatio(int i10);
	void SetShowCoordHint(bool show);
	void DrawCenteredText(const std::wstring& text, float centerX, float centerY, float maxWidth = 100.0f, float maxHeight = 30.0f);

	int GetRColWidthRatio();
	bool GetShowCoordHint();
	Graph* GetGraph();

private:
	void InitCheckBoxes();
	void InitdigitBoxes();
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
	bool mShowCoordHint;
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
	float gf_x_Indicator_Spacing = 0;
	float gf_y_Indicator_Spacing = 0;
	//Config Related
	CheckBox CBoxShowXIndLine;
	CheckBox CBoxShowYIndLine;
	CheckBox CBoxShowCoordHint;
	CheckboxBlueprint checkBoxBprint;
	int CBoxHeight = 0;
	int CBoxYSpacing = 0;
	DigitBoxBlueprint digitBoxBlueprint;
	DigitBox digitBox;
};
