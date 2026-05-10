#pragma once
#include <d2d1.h>

class Graph
{
public:
	Graph();
	~Graph();

	void SetSpacingWidth(int x);
	void SetSpacingHeight(int x);
	void SetScaleX(int x);
	void SetScaleY(int x);
	void SetXAxisColor(const D2D1::ColorF col);
	void SetYAxisColor(const D2D1::ColorF col);
	void SetXRefLinesColor(const D2D1::ColorF col);
	void SetYRefLinesColor(const D2D1::ColorF col);
	void SetGridSpace(int x);

	int GetSpacingWidth();
	int GetSpacingHeight();
	int GetScaleX();
	int GetScaleY();
	int GetGridSpace();

	D2D1::ColorF GetXAxisColor() const; 
	D2D1::ColorF GetYAxisColor() const; 
	D2D1::ColorF GetXRefLinesColor() const;
	D2D1::ColorF GetYRefLinesColor() const;

private:

	int scaleX = 10;
	int scaleY = scaleX;
	int SpacingWidth = 10;
	int SpacingHeight = SpacingWidth;
	int GridSpace = 20;

	D2D1::ColorF XAxisCol = D2D1::ColorF(D2D1::ColorF::Red);
	D2D1::ColorF YAxisCol = D2D1::ColorF(XAxisCol);

	D2D1::ColorF XRefLinesCol = D2D1::ColorF(D2D1::ColorF::Gray);
	D2D1::ColorF YRefLinesCol = D2D1::ColorF(XRefLinesCol);
};
