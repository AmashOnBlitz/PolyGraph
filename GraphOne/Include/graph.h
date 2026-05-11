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
	void SetInnerRefLinesColor(const D2D1::ColorF col);
	void SetUsrIndicatorLineColor(const D2D1::ColorF col);
	void SetGridSpace(int x);
	void SetShowXUsrIndicator(bool b);
	void SetShowYUsrIndicator(bool b);
	// Complement to ShowProcessedValues, Will change it to !b
	void SetShowRawCoords(bool b);
	// Complement to ShowRawCoords, Will change it to !b
	void SetShowProcessedValues(bool b);

	int GetSpacingWidth();
	int GetSpacingHeight();
	int GetScaleX();
	int GetScaleY();
	int GetGridSpace();

	bool GetShowXUsrIndicator();
	bool GetShowYUsrIndicator();
	bool GetShowRawCoords();
	bool GetShowProcessedValues();

	D2D1::ColorF GetXAxisColor() const; 
	D2D1::ColorF GetYAxisColor() const; 
	D2D1::ColorF GetXRefLinesColor() const;
	D2D1::ColorF GetYRefLinesColor() const;
	D2D1::ColorF GetInnerRefLinesColor() const;
	D2D1::ColorF GetUsrIndicatorLineColor() const;

private:

	int scaleX = 10;
	int scaleY = scaleX;
	int SpacingWidth = 10;
	int SpacingHeight = SpacingWidth;
	int GridSpace = 50;

	bool showXUsrIndicator = false;
	bool showYUsrIndicator = false;
	bool showRawCoords = false;
	bool showProcessedValues = true;

	D2D1::ColorF XAxisCol = D2D1::ColorF(D2D1::ColorF::Red);
	D2D1::ColorF YAxisCol = D2D1::ColorF(XAxisCol);

	D2D1::ColorF XRefLinesCol = D2D1::ColorF(D2D1::ColorF::Gray);
	D2D1::ColorF YRefLinesCol = D2D1::ColorF(XRefLinesCol);

	D2D1::ColorF InnerRefLinesCol = D2D1::ColorF(D2D1::ColorF(0.85f, 0.85f, 0.85f, 1.0f));

	D2D1::ColorF UsrIndicatorLineCol = D2D1::ColorF(D2D1::ColorF::Green);
};
