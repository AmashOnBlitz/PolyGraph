#pragma once
#include <string>
#include <d2d1.h>
#include <dwrite.h>

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
	void SetBackgroundColor(D2D1::ColorF color);
	void SetBorderColor(D2D1::ColorF color);
	void SetTextColor(D2D1::ColorF color);
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
	D2D1::ColorF GetBackgroundColor();
	D2D1::ColorF GetBorderColor();
	D2D1::ColorF GetTextColor();

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
	D2D1::ColorF mBackgroundCol;
	D2D1::ColorF mBorderCol;
	D2D1::ColorF mTxtCol;
};


class DigitBox
{
public:
	DigitBox() = delete;
	DigitBox(DigitBoxBlueprint blueprint);
	~DigitBox();

	void Render(
		ID2D1HwndRenderTarget* mpRend,
		IDWriteFactory* mpFactDWrite,
		ID2D1SolidColorBrush* mpBrOne,
		IDWriteTextFormat* mpTxtFmt,
		IDWriteTextLayout*& mpTxtLyout,
		int x,
		int y
	);
	void SetBlueprint(DigitBoxBlueprint blueprint); 
	DigitBoxBlueprint& GetBlueprint();
	D2D1_ROUNDED_RECT& GetCheckBoxRect();

private:
	DigitBoxBlueprint mBlueprint;
	D2D1_ROUNDED_RECT mDigitBoxRect;
};
