#pragma once
#include <string>
#include <d2d1.h>
#include <dwrite.h>

class DigitBoxBlueprint {
public:
	// Note : From text strings only number will be extracted, stored and shown, exception : placeholder text
	DigitBoxBlueprint(
		std::string text = "",
		std::string placeholderText = "Enter your value",
		int w = 40,
		int h = 20,
		int xRoundRadius = 4,
		int yRoundRadius = 4,
		D2D1::ColorF BackgroundCol = D2D1::ColorF::White,
		D2D1::ColorF BorderCol = D2D1::ColorF::Gray,
		D2D1::ColorF ActiveBorderCol = D2D1::ColorF::Blue,
		D2D1::ColorF TextCol = D2D1::ColorF::Black,
		float BorderWidth = 1.0f, 
		int xTxtPadding = 2,
		int yTxtPadding = 2
	);
	~DigitBoxBlueprint();

	// Note : From text strings only number will be extracted, stored and shown
	void SetText(std::string txt);
	// Note : From text strings only number will be extracted, stored and shown
	void SetPlaceHolderText(std::string txt);
	void SetWidth(int x);
	void SetHeight(int y);
	void SetXRoundRadius(int r);
	void SetYRoundRadius(int r);
	void SetBackgroundColor(D2D1::ColorF color);
	void SetBorderColor(D2D1::ColorF color);
	void SetActiveBorderColor(D2D1::ColorF color);
	void SetTextColor(D2D1::ColorF color);
	void SetYTextPadding(int y);
	void SetXTextPadding(int x);
	void SetBorderWidth(int w);

	std::string GetText();
	std::string GetPlaceHolderText();
	int GetWidth();
	int GetHeight();
	int GetXRoundRadius();
	int GetYRoundRadius();
	int GetYTextPadding();
	int GetXTextPadding();
	float GetBorderWidth();
	D2D1::ColorF GetBackgroundColor();
	D2D1::ColorF GetBorderColor();
	D2D1::ColorF GetActiveBorderColor();
	D2D1::ColorF GetTextColor();

private: //funcs 
	std::string GetNumFromStr(std::string str);

private:
	std::string mTxt;
	std::string mPlaceHolderTxt;
	int mWidth;
	int mHeight;
	int mXRoundRad;
	int mYRoundRad;
	int mYTxtPadding;
	int mXTxtPadding;
	float mBorderWidth;
	D2D1::ColorF mBackgroundCol;
	D2D1::ColorF mBorderCol;
	D2D1::ColorF mActiveBorderCol;
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
	void SetFocused(bool focused);
	DigitBoxBlueprint& GetBlueprint();
	D2D1_ROUNDED_RECT& GetBoxRect();
	bool GetFocused();

private:
	void RendText(
		D2D1_ROUNDED_RECT dbRect,
		ID2D1HwndRenderTarget* mpRend,
		IDWriteFactory* mpFactDWrite,
		ID2D1SolidColorBrush* mpBrOne,
		IDWriteTextFormat* mpTxtFmt,
		IDWriteTextLayout*& mpTxtLyout
	);
	std::wstring StringToWString(const std::string& str);
private:
	DigitBoxBlueprint mBlueprint;
	D2D1_ROUNDED_RECT mDigitBoxRect;
	bool mFocused = false;
};
