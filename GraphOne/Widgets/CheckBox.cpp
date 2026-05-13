#include <pch.h>
#include "CheckBox.h"

CheckBox::CheckBox(CheckboxBlueprint blueprint)
	: mBlueprint(blueprint)
{
}

CheckBox::~CheckBox()
{
}

void CheckBox::SetBlueprint(CheckboxBlueprint blueprint)
{
	this->mBlueprint = blueprint;
}

CheckboxBlueprint CheckBox::GetBlueprint()
{
	return this->mBlueprint;
}
