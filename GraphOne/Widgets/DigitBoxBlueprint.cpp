#include <pch.h>
#include <DigitBox.h>

DigitBoxBlueprint::DigitBoxBlueprint(
	std::string text,
	std::string placeholderText,
	int w,
	int h,
	int xRoundRadius,
	int yRoundRadius,
	D2D1::ColorF BackgroundCol,
	D2D1::ColorF BorderCol,
	D2D1::ColorF ActiveBorderCol,
	D2D1::ColorF TextCol,
	float BorderWidth,
	int xTxtPadding,
	int yTxtPadding
) : 
	mTxt(GetNumFromStr(text)),
	mPlaceHolderTxt(placeholderText),
	mWidth(w),
	mHeight(h),
	mXRoundRad(xRoundRadius),
	mYRoundRad(yRoundRadius),
	mBackgroundCol(BackgroundCol),
	mBorderCol(BorderCol),
	mActiveBorderCol(ActiveBorderCol),
	mTxtCol(TextCol),
	mXTxtPadding(xTxtPadding),
	mYTxtPadding(yTxtPadding),
	mBorderWidth(BorderWidth)
{
}

DigitBoxBlueprint::~DigitBoxBlueprint()
{
}

void DigitBoxBlueprint::SetText(std::string txt)
{
	this->mTxt = GetNumFromStr(txt);
}

void DigitBoxBlueprint::SetPlaceHolderText(std::string txt)
{
	this->mPlaceHolderTxt = txt;
}

void DigitBoxBlueprint::SetWidth(int x)
{
	this->mWidth = x;
}

void DigitBoxBlueprint::SetHeight(int y)
{
	this->mHeight = y;
}

void DigitBoxBlueprint::SetXRoundRadius(int r)
{
	this->mXRoundRad = r;
}

void DigitBoxBlueprint::SetYRoundRadius(int r)
{
	this->mYRoundRad = r;
}

void DigitBoxBlueprint::SetBackgroundColor(D2D1::ColorF color)
{
	this->mBackgroundCol = color;
}

void DigitBoxBlueprint::SetBorderColor(D2D1::ColorF color)
{
	this->mBorderCol = color;
}

void DigitBoxBlueprint::SetActiveBorderColor(D2D1::ColorF color)
{
	this->mActiveBorderCol = color;
}

void DigitBoxBlueprint::SetTextColor(D2D1::ColorF color)
{
	this->mTxtCol = color;
}

void DigitBoxBlueprint::SetYTextPadding(int y)
{
	this->mYTxtPadding = y;
}

void DigitBoxBlueprint::SetXTextPadding(int x)
{
	this->mXTxtPadding = x;
}

void DigitBoxBlueprint::SetBorderWidth(int w)
{
	this->mBorderWidth = w;
}

std::string DigitBoxBlueprint::GetText()
{
	return this->mTxt;
}

std::string DigitBoxBlueprint::GetPlaceHolderText()
{
	return this->mPlaceHolderTxt;
}

int DigitBoxBlueprint::GetWidth()
{
	return this->mWidth;
}

int DigitBoxBlueprint::GetHeight()
{
	return this->mHeight;
}

int DigitBoxBlueprint::GetXRoundRadius()
{
	return this->mXRoundRad;
}

int DigitBoxBlueprint::GetYRoundRadius()
{
	return this->mYRoundRad;
}

int DigitBoxBlueprint::GetYTextPadding()
{
	return this->mYTxtPadding;
}

int DigitBoxBlueprint::GetXTextPadding()
{
	return this->mXTxtPadding;
}

float DigitBoxBlueprint::GetBorderWidth()
{
	return this->mBorderWidth;
}

D2D1::ColorF DigitBoxBlueprint::GetBackgroundColor()
{
	return this->mBackgroundCol;
}

D2D1::ColorF DigitBoxBlueprint::GetBorderColor()
{
	return this->mBorderCol;
}

D2D1::ColorF DigitBoxBlueprint::GetActiveBorderColor()
{
	return this->mActiveBorderCol;
}

D2D1::ColorF DigitBoxBlueprint::GetTextColor()
{
	return this->mTxtCol;
}

std::string DigitBoxBlueprint::GetNumFromStr(std::string str)
{
	std::string filterStr = "";
	for (int i{ 0 }; i < str.length(); i++) {
		if (std::isdigit(str[i])) filterStr += str[i];
	}
	return filterStr;
}

