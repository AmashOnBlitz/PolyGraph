#pragma once
#include <d2d1.h>

class CheckboxBlueprint
{
public:
	// -1 means same as width
	CheckboxBlueprint(
		std::string text,
		bool checked = false,
		int w = 20,
		int h = -1,
		D2D1::ColorF uncheckedCol = D2D1::ColorF::Gray,
		D2D1::ColorF checkedCol = D2D1::ColorF::Green,
		D2D1::ColorF tickCol = D2D1::ColorF::White
	);
	~CheckboxBlueprint();

	void SetText(std::string t);
	void SetChecked(bool checked);
	void SetHeight(int h);
	void SetWidth(int w);
	void SetUncheckedCol(D2D1::ColorF col);
	void SetCheckedCol(D2D1::ColorF col);
	void SetTickCol(D2D1::ColorF col);

	int GetHeight();
	int GetWidth();
	std::string GetText();
	bool GetChecked();
	bool PollDataChangedFlag();

	D2D1::ColorF GetUncheckedCol();
	D2D1::ColorF GetCheckedCol();
	D2D1::ColorF GetTickCol();

private:
	void SetDataChangedFlag(bool b = true);

private:
	std::string mText;
	bool mChecked;
	bool mDataChanged = false;
	int mWidth; 
	int mHeight;

	D2D1::ColorF mUncheckedCol = D2D1::ColorF::GhostWhite;
	D2D1::ColorF mCheckedCol = D2D1::ColorF::Green;
	D2D1::ColorF mTickCol = D2D1::ColorF::White;
};


class CheckBox
{
public:
	CheckBox() = delete;
	CheckBox(CheckboxBlueprint blueprint);
	~CheckBox();
	void SetBlueprint(CheckboxBlueprint blueprint);
	CheckboxBlueprint GetBlueprint();

private:
	CheckboxBlueprint mBlueprint;
};
