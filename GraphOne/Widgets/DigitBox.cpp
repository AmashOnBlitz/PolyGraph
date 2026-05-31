#include <pch.h>
#include "DigitBox.h"


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
		D2D1::RectF(x,
					y,
					x + this->GetBlueprint().GetWidth(),
					y + this->GetBlueprint().GetHeight()
		),
		this->GetBlueprint().GetXRoundRadius(),
		this->GetBlueprint().GetYRoundRadius()
	);

	mpBrOne->SetColor(D2D1::ColorF(this->GetBlueprint().GetBackgroundColor()));
	mpRend->FillRoundedRectangle(
		mDigitBoxRect,
		mpBrOne
	);
	mpBrOne->SetColor(D2D1::ColorF(this->GetBlueprint().GetBorderColor()));
	mpRend->DrawRoundedRectangle(
		mDigitBoxRect,
		mpBrOne
	);
}

void DigitBox::SetBlueprint(DigitBoxBlueprint blueprint)
{
	this->mBlueprint = blueprint;
}

DigitBoxBlueprint& DigitBox::GetBlueprint()
{
	return this->mBlueprint;
}

D2D1_ROUNDED_RECT& DigitBox::GetCheckBoxRect()
{
	return this->mDigitBoxRect;
}
