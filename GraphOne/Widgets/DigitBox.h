#pragma once
#include <string>
#include <d2d1.h>

class DigitBoxBlueprint {
public:
	// Note : From text strings only number will be extracted, stored and shown
	DigitBoxBlueprint(
		std::string intialText = "",
		std::string placeholderText = "",
		int w = 40,
		int h = 20,
		int xRoundRadius = 4,
		int yRoundRadius = -1,
		D2D1::ColorF BackgroundCol = D2D1::ColorF::White,
		D2D1::ColorF BorderCol = D2D1::ColorF::Gray,
		D2D1::ColorF TextCol = D2D1::ColorF::Black,
		int xTxtPadding = 5,
		int yTxtPadding = 5
	);
	~DigitBoxBlueprint();

	// Note : From text strings only number will be extracted, stored and shown
	void SetInitialText(std::string txt);
	// Note : From text strings only number will be extracted, stored and shown
	void SetPlaceHolderText(std::string txt);
	void SetWidth(int x);
	void SetHeight(int y);
	void SetXRoundRadius(int r);
	void SetYRoundRadius(int r);
	void SetBackgroundColor(D2D1_COLOR_F color);
	void SetBorderColor(D2D1_COLOR_F color);
	void SetTextColor(D2D1_COLOR_F color);
	void SetYTextPadding(int y);
	void SetXTextPadding(int x);

	std::string GetInitialText();
	std::string GetPlaceHolderText();
	int GetWidth();
	int GetHeight();
	int GetXRoundRadius();
	int GetYRoundRadius();
	int GetYTextPadding();
	int GetXTextPadding();
	D2D1_COLOR_F GetBackgroundColor();
	D2D1_COLOR_F GetBorderColor();
	D2D1_COLOR_F GetTextColor();

private: //funcs 
	std::string GetNumFromStr(std::string str);

private:
	std::string mInitialTxt;
	std::string mPlaceHolderTxt;
	int mWidth;
	int mHeight;
	int mXRoundRad;
	int mYRoundRad;
	int mYTxtPadding;
	int mXTxtPadding;
	D2D1_COLOR_F mBackgroundCol;
	D2D1_COLOR_F mBorderCol;
	D2D1_COLOR_F mTxtCol;
};