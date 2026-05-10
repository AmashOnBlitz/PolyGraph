#include "pch.h"
#include "graph.h"

Graph::Graph()
{
}

Graph::~Graph()
{
}

void Graph::SetSpacingWidth(int x)
{
	this->SpacingWidth = x;
}

void Graph::SetSpacingHeight(int x)
{
	this->SpacingHeight = x;
}

void Graph::SetScaleX(int x)
{
	this->scaleX = x;
}

void Graph::SetScaleY(int x)
{
	this->scaleY = x;
}

void Graph::SetXAxisColor(D2D1::ColorF col)
{
	this->XAxisCol = D2D1::ColorF(col);
}

void Graph::SetYAxisColor(D2D1::ColorF col)
{
	this->YAxisCol = D2D1::ColorF(col);
}

void Graph::SetXRefLinesColor(D2D1::ColorF col)
{
	this->XRefLinesCol = D2D1::ColorF(col);
}

void Graph::SetYRefLinesColor(D2D1::ColorF col)
{
	this->YRefLinesCol = D2D1::ColorF(col);
}

void Graph::SetInnerRefLinesColor(const D2D1::ColorF col)
{
	this->InnerRefLinesCol = D2D1::ColorF(col);
}

void Graph::SetGridSpace(int x)
{
	this->GridSpace = x;
}

int Graph::GetSpacingWidth()
{
	return this->SpacingWidth;
}

int Graph::GetSpacingHeight()
{
	return this->SpacingHeight;
}

int Graph::GetScaleX()
{
	return this->scaleX;
}

int Graph::GetScaleY()
{
	return this->scaleY;
}

int Graph::GetGridSpace()
{
	return this->GridSpace;
}

D2D1::ColorF Graph::GetXAxisColor() const
{
	return this->XAxisCol;
}

D2D1::ColorF Graph::GetYAxisColor() const
{
	return this->YAxisCol;
}

D2D1::ColorF Graph::GetXRefLinesColor() const
{
	return this->XRefLinesCol;
}

D2D1::ColorF Graph::GetYRefLinesColor() const
{
	return this->YRefLinesCol;
}

D2D1::ColorF Graph::GetInnerRefLinesColor() const
{
	return this->InnerRefLinesCol;
}
