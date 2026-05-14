#include "pch.h"
#include "CheckBox.h"

#define FLAG_CHECKBOX_DATA_CHANGED this->SetDataChangedFlag();

CheckboxBlueprint::CheckboxBlueprint(
	std::string text,
	bool checked,
	int w,
	int h,
	D2D1::ColorF defaultCol,
	D2D1::ColorF checkedCol,
	D2D1::ColorF tickCol
) 
	: mText(text),
	mChecked(checked),
	mWidth(w),
	mHeight((h < 0)? w : h),
	mUncheckedCol(defaultCol),
	mCheckedCol(checkedCol),
	mTickCol(tickCol)
{
}

CheckboxBlueprint::~CheckboxBlueprint()
{
}

void CheckboxBlueprint::SetText(std::string t)
{
	this->mText = t;
	FLAG_CHECKBOX_DATA_CHANGED;
}

void CheckboxBlueprint::SetChecked(bool checked)
{
	this->mChecked = checked;
	FLAG_CHECKBOX_DATA_CHANGED;
}

void CheckboxBlueprint::SetHeight(int h)
{
	this->mHeight = h;
	FLAG_CHECKBOX_DATA_CHANGED;
}

void CheckboxBlueprint::SetWidth(int w)
{
	this->mWidth = w;
	FLAG_CHECKBOX_DATA_CHANGED;
}

void CheckboxBlueprint::SetUncheckedCol(D2D1::ColorF col)
{
	this->mUncheckedCol = D2D1::ColorF(col);
	FLAG_CHECKBOX_DATA_CHANGED;
}

void CheckboxBlueprint::SetCheckedCol(D2D1::ColorF col)
{
	this->mCheckedCol = D2D1::ColorF(col);
	FLAG_CHECKBOX_DATA_CHANGED;
}

void CheckboxBlueprint::SetTickCol(D2D1::ColorF col)
{
	this->mTickCol = D2D1::ColorF(col);
	FLAG_CHECKBOX_DATA_CHANGED;
}

int CheckboxBlueprint::GetHeight()
{
	return this->mHeight;
}

int CheckboxBlueprint::GetWidth()
{
	return this->mWidth;
}

std::string CheckboxBlueprint::GetText()
{
	return this->mText;
}

bool CheckboxBlueprint::GetChecked()
{
	return this->mChecked;
}

bool CheckboxBlueprint::PollDataChangedFlag()
{
	if (!mDataChanged) return false;
	bool orig = this->mDataChanged;
	this->mDataChanged = false;
	return orig;
}

D2D1::ColorF CheckboxBlueprint::GetUncheckedCol()
{
	return this->mUncheckedCol;
}

D2D1::ColorF CheckboxBlueprint::GetCheckedCol()
{
	return this->mCheckedCol;
}

D2D1::ColorF CheckboxBlueprint::GetTickCol()
{
	return this->mTickCol;
}

void CheckboxBlueprint::SetDataChangedFlag(bool b)
{
	this->mDataChanged = b;
}
