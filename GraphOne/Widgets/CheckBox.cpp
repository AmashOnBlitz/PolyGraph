#include <pch.h>
#include "CheckBox.h"

#define ReleaseD2D1Item(x) if (x) x->Release(); x = NULL;
#define GuardD2D1Failure(x) if (x != S_OK) return false;
#define GuardD2D1FailureVoid(x) if (x != S_OK) return;

CheckBox::CheckBox(CheckboxBlueprint blueprint)
	: mBlueprint(blueprint),
	checkBoxRect({})
{
}

CheckBox::~CheckBox()
{
}

void CheckBox::Render(
	ID2D1HwndRenderTarget* mpRend,
	IDWriteFactory* mpFactDWrite,
	ID2D1SolidColorBrush* mpBrOne,
	IDWriteTextFormat* mpTxtFmt,
	IDWriteTextLayout*& mpTxtLyout,
	int x,
	int y
)
{
	CheckBox cbox = *this;
	const int textSpacing = 4;
	checkBoxRect = D2D1::RectF(
		x,
		y,
		x + cbox.GetBlueprint().GetWidth(),
		y + cbox.GetBlueprint().GetHeight()
	);

	mpBrOne->SetColor(D2D1::ColorF(
		(cbox.GetBlueprint().GetChecked()) ? cbox.GetBlueprint().GetCheckedCol() : cbox.GetBlueprint().GetUncheckedCol())
	);
	mpRend->FillRoundedRectangle(
		D2D1::RoundedRect(
			checkBoxRect,
			cbox.GetBlueprint().GetXRoundRadius(),
			cbox.GetBlueprint().GetYRoundRadius()
		),
		mpBrOne
	);

	if (cbox.GetBlueprint().GetChecked()) {
		mpBrOne->SetColor(D2D1::ColorF(cbox.GetBlueprint().GetTickCol()));
		float geometryRatio = 3.0f / 4.0f;
		float inverseGeometryRatio = 1.0f / 4.0f;
		int cBoxWidth = (checkBoxRect.right - checkBoxRect.left);
		int cBoxHeight = (checkBoxRect.bottom - checkBoxRect.top);
		mpRend->DrawLine(
			D2D1::Point2F(
				checkBoxRect.left + (cBoxWidth * inverseGeometryRatio),
				checkBoxRect.top + (cBoxHeight * 0.5f)
			),
			D2D1::Point2F(
				checkBoxRect.left + (cBoxWidth * 0.5f),
				checkBoxRect.top + (cBoxHeight * geometryRatio)
			),
			mpBrOne,
			2.0f
		);
		mpRend->DrawLine(
			D2D1::Point2F(
				checkBoxRect.left + (cBoxWidth * 0.5f),
				checkBoxRect.top + (cBoxHeight * geometryRatio)
			),
			D2D1::Point2F(
				checkBoxRect.left + (cBoxWidth * geometryRatio),
				checkBoxRect.top + (cBoxHeight * inverseGeometryRatio)
			),
			mpBrOne,
			2.0f
		);
	}

	mpBrOne->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	std::string s = cbox.GetBlueprint().GetText();
	std::wstring ws(s.begin(), s.end());
	TCHAR txt[2048];
	StringCchPrintf(txt, 2048, _TEXT("%s"), ws.c_str());
	ReleaseD2D1Item(mpTxtLyout);
	HRESULT Hr = mpFactDWrite->CreateTextLayout(
		txt,
		lstrlen(txt),
		mpTxtFmt,
		400,
		cbox.GetBlueprint().GetHeight(),
		&mpTxtLyout
	);
	GuardD2D1FailureVoid(Hr);
	DWRITE_TEXT_METRICS txtMetric;
	Hr = mpTxtLyout->GetMetrics(&txtMetric);
	GuardD2D1FailureVoid(Hr);
	mpRend->DrawTextLayout(
		D2D1::Point2F(
			checkBoxRect.right + textSpacing,
			checkBoxRect.top + (checkBoxRect.bottom - checkBoxRect.top - txtMetric.height) / 2.0f
		),
		mpTxtLyout,
		mpBrOne
	);
}

void CheckBox::PassMouseClick(int xPos, int yPos)
{
	if (!isPointInBound(xPos, yPos, checkBoxRect)) return;
	this->GetBlueprint().SetChecked(!this->GetBlueprint().GetChecked());
}

void CheckBox::SetBlueprint(CheckboxBlueprint blueprint)
{
	this->mBlueprint = blueprint;
}

CheckboxBlueprint& CheckBox::GetBlueprint()
{
	return this->mBlueprint;
}

D2D1_RECT_F& CheckBox::GetCheckBoxRect()
{
	return this->checkBoxRect;
}

bool CheckBox::isPointInBound(int x, int y, D2D1_RECT_F& rect)
{
	return ((x >= rect.left && x <= rect.right) && (y >= rect.top && y <= rect.bottom));
}
