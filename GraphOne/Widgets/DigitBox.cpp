#include <pch.h>
#include "DigitBox.h"

#define DB_SHARP_COORDS(value) static_cast<float>(value) + 0.5f

DigitBox::DigitBox(DigitBoxBlueprint blueprint) :
	mBlueprint(blueprint),
	mDigitBoxRect({})
{
}

DigitBox::~DigitBox()
{
}

void DigitBox::Render(
	ID2D1HwndRenderTarget* mpRend,
	IDWriteFactory* mpFactDWrite,
	ID2D1SolidColorBrush* mpBrOne,
	IDWriteTextFormat* mpTxtFmt,
	IDWriteTextLayout*& mpTxtLyout,
	int x,
	int y
)
{
	mDigitBoxRect = D2D1::RoundedRect(
		D2D1::RectF(
			DB_SHARP_COORDS(x),
			DB_SHARP_COORDS(y),
			DB_SHARP_COORDS(x + this->GetBlueprint().GetWidth()),
			DB_SHARP_COORDS(y + this->GetBlueprint().GetHeight())
		),
		this->GetBlueprint().GetXRoundRadius(),
		this->GetBlueprint().GetYRoundRadius()
	);

	mpBrOne->SetColor(D2D1::ColorF(this->GetBlueprint().GetBackgroundColor()));
	mpRend->FillRoundedRectangle(
		mDigitBoxRect,
		mpBrOne
	);
	mpBrOne->SetColor(
		D2D1::ColorF(
			(mFocused) ? 
			this->GetBlueprint().GetActiveBorderColor() : 
			this->GetBlueprint().GetBorderColor()
		)
	);
	mpRend->DrawRoundedRectangle(
		mDigitBoxRect,
		mpBrOne,
		this->GetBlueprint().GetBorderWidth()
	);
	RendText(
		mDigitBoxRect,
		mpRend,
		mpFactDWrite,
		mpBrOne,
		mpTxtFmt,
		mpTxtLyout
	);
}

void DigitBox::SetBlueprint(DigitBoxBlueprint blueprint)
{
	this->mBlueprint = blueprint;
}

void DigitBox::SetFocused(bool focused)
{
	this->mFocused = focused;
}

DigitBoxBlueprint& DigitBox::GetBlueprint()
{
	return this->mBlueprint;
}

D2D1_ROUNDED_RECT& DigitBox::GetBoxRect()
{
	return this->mDigitBoxRect;
}

bool DigitBox::GetFocused()
{
	return this->mFocused;
}

void DigitBox::RendText(
	D2D1_ROUNDED_RECT dbRect,
	ID2D1HwndRenderTarget* mpRend,
	IDWriteFactory* mpFactDWrite,
	ID2D1SolidColorBrush* mpBrOne,
	IDWriteTextFormat* mpTxtFmt,
	IDWriteTextLayout*& mpTxtLyout
)
{
	D2D1_RECT_F flatDbRect = dbRect.rect;
	D2D1_RECT_F rectTxt = D2D1::RectF(
		flatDbRect.left + this->GetBlueprint().GetBorderWidth() + this->GetBlueprint().GetXTextPadding(),
		flatDbRect.top + this->GetBlueprint().GetBorderWidth() + this->GetBlueprint().GetYTextPadding(),
		flatDbRect.right - this->GetBlueprint().GetBorderWidth() - this->GetBlueprint().GetXTextPadding(),
		flatDbRect.bottom - this->GetBlueprint().GetBorderWidth() - this->GetBlueprint().GetYTextPadding()
	);

	std::wstring rendTxt; 
	(this->GetBlueprint().GetText().empty()) ?
		rendTxt = this->StringToWString(this->GetBlueprint().GetPlaceHolderText()) :
		rendTxt = this->StringToWString(this->GetBlueprint().GetText());

	FLOAT width = rectTxt.right - rectTxt.left;
	FLOAT height = rectTxt.bottom - rectTxt.top;
	HRESULT hr = mpFactDWrite->CreateTextLayout(
		rendTxt.c_str(),
		(UINT32)rendTxt.length(),
		mpTxtFmt,
		width,
		height,
		&mpTxtLyout
	);
	if (!SUCCEEDED(hr)) return;

	DWRITE_TRIMMING trimming = {};
	IDWriteInlineObject* pEllipsis = nullptr;
	trimming.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;

	mpFactDWrite->CreateEllipsisTrimmingSign(
		mpTxtFmt,
		&pEllipsis
	);
	mpTxtLyout->SetTrimming(
		&trimming,
		pEllipsis
	);
	if (pEllipsis) pEllipsis->Release();

	mpTxtLyout->SetWordWrapping(
		DWRITE_WORD_WRAPPING_NO_WRAP
	);

	mpBrOne->SetColor(D2D1::ColorF(this->GetBlueprint().GetTextColor()));
	mpRend->DrawTextLayout(
		D2D1::Point2F(rectTxt.left, rectTxt.top),
		mpTxtLyout,
		mpBrOne
	);
}

std::wstring DigitBox::StringToWString(const std::string& str)
{
	int txtSize = MultiByteToWideChar(
		CP_UTF8,
		0,
		str.c_str(),
		(int)str.size(),
		NULL,
		0
	);
	std::wstring wstr(txtSize, 0);

	MultiByteToWideChar(
		CP_UTF8,
		0,
		str.c_str(),
		(int)str.size(),
		&wstr[0],
		txtSize
	);

	return wstr;
}

