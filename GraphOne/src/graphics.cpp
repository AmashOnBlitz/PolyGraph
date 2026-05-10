#include "pch.h"
#include "graphics.h"

#define ReleaseD2D1Item(x) if (x) x->Release();
#define GuardD2D1Failure(x) if (x != S_OK) return false;
Graphics::Graphics() :
	mHWnd(NULL),
	mpFact(nullptr),
	mpRend(nullptr),
	mpBrOne(nullptr),
	mpGraph(nullptr)
{
}

Graphics::~Graphics()
{
	ReleaseD2D1Item(mpFact);
	ReleaseD2D1Item(mpRend);
	ReleaseD2D1Item(mpBrOne);
}

bool Graphics::Init(HWND hWnd) 
{
	this->mHWnd = hWnd;

	HRESULT hRes = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &mpFact);
	GuardD2D1Failure(hRes);

	RECT rect;
	GetClientRect(mHWnd, &rect);
	hRes = mpFact->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			mHWnd,
			D2D1::SizeU(
				rect.right - rect.left,
				rect.bottom - rect.top
			)
		),
		&mpRend
	);
	GuardD2D1Failure(hRes);

	hRes = mpRend->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &mpBrOne);
	GuardD2D1Failure(hRes);

	return true;
}

void Graphics::Resize(UINT width, UINT height)
{
	if (mpRend) mpRend->Resize(D2D1::SizeU(width, height));
}

void Graphics::Render()
{
	RECT rect;
	GetClientRect(mHWnd, &rect);
	mpRend->BeginDraw();
	mpRend->Clear(D2D1::ColorF(D2D1::ColorF::White));
	RendGraph(rect);
	mpRend->EndDraw();
}
void Graphics::RendGraph(RECT rect)
{
	const int PADDING = 10;
	int xStart = rect.left + ConvRatioToInt(
		mRColWidthRatio10,
		rect.right - rect.left
	) + PADDING;
	int xEnd = ((rect.right - rect.left) - PADDING);
	int yStart = (rect.top + PADDING);
	int yEnd = ((rect.bottom - rect.top) - PADDING);
	int xMid = ((xStart + xEnd) / 2);
	int yMid = ((yStart + yEnd) / 2);

	D2D1::ColorF BgCol = D2D1::ColorF::WhiteSmoke;
	if (mpGraph == nullptr) BgCol = D2D1::ColorF::Black;

	mpBrOne->SetColor(D2D1::ColorF(BgCol));
	mpRend->FillRectangle(
		D2D1::RectF(
			xStart,
			yStart,
			xEnd,
			yEnd
		),
		mpBrOne
	);

	if (mpGraph == nullptr) {
		mpBrOne->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
		mpRend->DrawLine(
			D2D1::Point2F(
				xStart,
				yStart
			),
			D2D1::Point2F(
				xEnd,
				yEnd
			),
			mpBrOne
		);
		mpRend->DrawLine(
			D2D1::Point2F(
				xStart,
				yEnd
			),
			D2D1::Point2F(
				xEnd,
				yStart
			),
			mpBrOne
		);
	}
	else {
		mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetXRefLinesColor()));
		//Left Side X
		int currentPos = xMid;
		while (currentPos > xStart)
		{
			mpRend->DrawLine(
				D2D1::Point2F(
					currentPos,
					yStart
				),
				D2D1::Point2F(
					currentPos,
					yEnd
				),
				mpBrOne
			);
			currentPos -= mpGraph->GetGridSpace();
		}
		//Right Side X
		currentPos = xMid + mpGraph->GetGridSpace();
		while (currentPos < xEnd) 
		{
			mpRend->DrawLine(
				D2D1::Point2F(
					currentPos,
					yStart
				),
				D2D1::Point2F(
					currentPos,
					yEnd
				),
				mpBrOne
			);
			currentPos += mpGraph->GetGridSpace();
		}

		mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetYRefLinesColor()));

		//Top Side Y
		currentPos = yMid - mpGraph->GetGridSpace();
		while (currentPos > yStart)
		{
			mpRend->DrawLine(
				D2D1::Point2F(
					xStart,
					currentPos
				),
				D2D1::Point2F(
					xEnd,
					currentPos
				),
				mpBrOne
			);
			currentPos -= mpGraph->GetGridSpace();
		}

		//Bottom Side Y
		currentPos = yMid + mpGraph->GetGridSpace();
		while (currentPos < yEnd)
		{
			mpRend->DrawLine(
				D2D1::Point2F(
					xStart,
					currentPos
				),
				D2D1::Point2F(
					xEnd,
					currentPos
				),
				mpBrOne
			);
			currentPos += mpGraph->GetGridSpace();
		}

		//X axis
		mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetXAxisColor()));
		mpRend->DrawLine(
			D2D1::Point2F(
				xStart,
				yMid
			),
			D2D1::Point2F(
				xEnd,
				yMid
				),
			mpBrOne
		);

		//Y axis
		mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetYAxisColor()));
		mpRend->DrawLine(
			D2D1::Point2F(
				xMid,
				yStart
			),
			D2D1::Point2F(
				xMid,
				yEnd
			),
			mpBrOne
		);
	}

}

int Graphics::ConvRatioToInt(int ratio10, int Max)
{
	return ((ratio10 * Max)/10);
}

void Graphics::SetGraph(Graph* gf)
{
	this->mpGraph = gf;
}

Graph* Graphics::GetGraph()
{
	return this->mpGraph;
}

void Graphics::SetRColWidthRatio(int i10)
{
	this->mRColWidthRatio10 = i10;
}

int Graphics::GetRColWidthRatio()
{
	return this->mRColWidthRatio10;
}

