#include "pch.h"
#include "graphics.h"

#define ReleaseD2D1Item(x) if (x) x->Release(); x = NULL;
#define GuardD2D1Failure(x) if (x != S_OK) return false;
#define GuardD2D1FailureVoid(x) if (x != S_OK) return;
#define MousePosValid(x) ((x) > 0)

Graphics::Graphics() :
	mHWnd(NULL),
	mpFact(nullptr),
	mpFactDWrite(nullptr),
	mpRend(nullptr),
	mpBrOne(nullptr),
	mpGraph(nullptr),
	mpTxtFmt(nullptr),
	mpTxtLyout(nullptr)
{
}

Graphics::~Graphics()
{
	ReleaseD2D1Item(mpFact);
	ReleaseD2D1Item(mpFactDWrite);
	ReleaseD2D1Item(mpRend);
	ReleaseD2D1Item(mpBrOne);
	ReleaseD2D1Item(mpTxtFmt);
}

bool Graphics::Init(HWND hWnd) 
{
	this->mHWnd = hWnd;

	HRESULT hRes = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &mpFact);
	GuardD2D1Failure(hRes);

	hRes = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)(&mpFactDWrite));
	GuardD2D1Failure(hRes);

	hRes = mpFactDWrite->CreateTextFormat(
		L"Bahnschrift",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		16.0f,
		L"en-us",
		&mpTxtFmt
	);
	mpTxtFmt->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	mpTxtFmt->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

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

void Graphics::MouseMov(int x, int y)
{
	this->MousePosX = -1;
	this->MousePosY = -1;
	if (MousePosValid(x)) {
		this->MousePosX = x;
	}
	if (MousePosValid(y)) {
		this->MousePosY = y;
	}
	InvalidateRect(mHWnd, NULL, FALSE);
}

void Graphics::Render()
{
	RECT rect;
	GetClientRect(mHWnd, &rect);
	mpRend->BeginDraw();
	mpRend->Clear(D2D1::ColorF(D2D1::ColorF::White)); 
	mpRend->SetAntialiasMode(
		D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
	);
	mpRend->SetTextAntialiasMode(
		D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE
	);
	RendGraph(rect);
	RendCoordHint(rect);
	mpRend->EndDraw();
}

void Graphics::RendGraph(RECT rect)
{
	const int PADDING = 10;
	const float INDICATORLINEWIDTH = 1.0f;
	const float AXISLINEWIDTH = 1.5f;
	const float INNER_IND_L_WIDTH =  1.0f;

	xStart = rect.left + ConvRatioToInt(
		mRColWidthRatio10,
		rect.right - rect.left
	) + PADDING;
	xEnd = ((rect.right - rect.left) - PADDING);
	yStart = (rect.top + PADDING);
	yEnd = ((rect.bottom - rect.top) - PADDING);
	xMid = ((xStart + xEnd) / 2);
	yMid = ((yStart + yEnd) / 2);

	D2D1::ColorF BgCol = D2D1::ColorF::WhiteSmoke;
	if (mpGraph == nullptr) BgCol = D2D1::ColorF::Black;

	mpBrOne->SetColor(D2D1::ColorF(BgCol));
	mpRend->FillRectangle(
		D2D1::RectF(
			Sharp(xStart),
			Sharp(yStart),
			Sharp(xEnd),
			Sharp(yEnd)
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
			if (currentPos != xMid) {
				mpRend->DrawLine(
					D2D1::Point2F(
						Sharp(currentPos),
						yStart
					),
					D2D1::Point2F(
						Sharp(currentPos),
						yEnd
					),
					mpBrOne,
					INDICATORLINEWIDTH
				);
			}
			int step = mpGraph->GetGridSpace() / 10;
			int xPos = 0;
			mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetInnerRefLinesColor()));
			for (int i = 1; i <= 9; i++) {
				xPos = currentPos - (i * step);
				if (xPos < xStart) break;
				mpRend->DrawLine(
					D2D1::Point2F(
						Sharp(xPos),
						yStart
					),
					D2D1::Point2F(
						Sharp(xPos),
						yEnd
					),
					mpBrOne,
					INNER_IND_L_WIDTH
				);
			}
			mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetXRefLinesColor()));
			currentPos -= mpGraph->GetGridSpace();
		}
		//Right Side X
		currentPos = xMid;
		while (currentPos < xEnd)
		{
			if (currentPos != xMid) {
				mpRend->DrawLine(
					D2D1::Point2F(
						Sharp(currentPos),
						yStart
					),
					D2D1::Point2F(
						Sharp(currentPos),
						yEnd
					),
					mpBrOne,
					INDICATORLINEWIDTH
				);
			}
			int step = mpGraph->GetGridSpace() / 10;
			int xPos = 0;
			mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetInnerRefLinesColor()));
			for (int i = 1; i <= 9; i++) {
				xPos = currentPos + (i * step);
				if (xPos > xEnd) break;
				mpRend->DrawLine(
					D2D1::Point2F(
						Sharp(xPos),
						yStart
					),
					D2D1::Point2F(
						Sharp(xPos),
						yEnd
					),
					mpBrOne,
					INNER_IND_L_WIDTH
				);
			}
			mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetXRefLinesColor()));
			currentPos += mpGraph->GetGridSpace();
		}

		mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetYRefLinesColor()));

		//Top Side Y
		currentPos = yMid;
		while (currentPos > yStart)
		{
			if (currentPos != yMid) {
				mpRend->DrawLine(
					D2D1::Point2F(
						xStart,
						Sharp(currentPos)
					),
					D2D1::Point2F(
						xEnd,
						Sharp(currentPos)
					),
					mpBrOne,
					INDICATORLINEWIDTH
				);
			}
			int step = mpGraph->GetGridSpace() / 10;
			int yPos = 0;
			mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetInnerRefLinesColor()));
			for (int i = 1; i <= 9; i++) {
				yPos = currentPos - (i * step);
				if (yPos < yStart) break;
				mpRend->DrawLine(
					D2D1::Point2F(
						xStart,
						Sharp(yPos)
					),
					D2D1::Point2F(
						xEnd,
						Sharp(yPos)
					),
					mpBrOne,
					INNER_IND_L_WIDTH
				);
			}
			mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetYRefLinesColor()));
			currentPos -= mpGraph->GetGridSpace();
		}

		//Bottom Side Y
		currentPos = yMid;
		while (currentPos < yEnd)
		{
			if (currentPos != yMid) {
				mpRend->DrawLine(
					D2D1::Point2F(
						xStart,
						Sharp(currentPos)
					),
					D2D1::Point2F(
						xEnd,
						Sharp(currentPos)
					),
					mpBrOne,
					INDICATORLINEWIDTH
				);
			}
			int step = mpGraph->GetGridSpace() / 10;
			int yPos = 0;
			mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetInnerRefLinesColor()));
			for (int i = 1; i <= 9; i++) {
				yPos = currentPos + (i * step);
				if (yPos > yEnd) break;
				mpRend->DrawLine(
					D2D1::Point2F(
						xStart,
						Sharp(yPos)
					),
					D2D1::Point2F(
						xEnd,
						Sharp(yPos)
					),
					mpBrOne,
					INNER_IND_L_WIDTH
				);
			}
			mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetYRefLinesColor()));
			currentPos += mpGraph->GetGridSpace();
		}

		//X axis
		mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetXAxisColor()));
		mpRend->DrawLine(
			D2D1::Point2F(
				xStart,
				Sharp(yMid)
			),
			D2D1::Point2F(
				xEnd,
				Sharp(yMid)
			),
			mpBrOne,
			AXISLINEWIDTH
		);

		//Y axis
		mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetYAxisColor()));
		mpRend->DrawLine(
			D2D1::Point2F(
				Sharp(xMid),
				yStart
			),
			D2D1::Point2F(
				Sharp(xMid),
				yEnd
			),
			mpBrOne,
			AXISLINEWIDTH
		);
		if (
			IsPointInBound(
				MousePosX,
				MousePosY,
				yStart,
				yEnd,
				xStart,
				xEnd
			)
			) 
		{
			//Usr Indicator X
			mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetUsrIndicatorLineColor()));
			if (mpGraph->GetShowXUsrIndicator() && MousePosValid(MousePosX)) {
				mpRend->DrawLine(
					D2D1::Point2F(
						MousePosX,
						yStart
					),
					D2D1::Point2F(
						MousePosX,
						yEnd
					),
					mpBrOne
				);
			}

			//Usr Indicator Y
			if (mpGraph->GetShowYUsrIndicator() && MousePosValid(MousePosY)) {
				mpRend->DrawLine(
					D2D1::Point2F(
						xStart,
						MousePosY
					),
					D2D1::Point2F(
						xEnd,
						MousePosY
					),
					mpBrOne
				);
			}
		}
	}

}

void Graphics::RendCoordHint(RECT rect)
{
	if (!IsPointInBound(
		MousePosX,
		MousePosY,
		yStart,
		yEnd,
		xStart,
		xEnd
	)) 
	{
		return;
	}
	float dpi = (float)(GetDpiForWindow(mHWnd));
	float dpiX = dpi;
	float dpiY = dpi;
	float dpiScaleX = dpiX / 96.0f;
	float dpiScaleY = dpiY / 96.0f;
	const int MouseCoordHintPadding = 10;
	const int MouseCoordHintInnerPadding = 5;
	TCHAR txt[256];

	if (mpGraph->GetShowRawCoords()) {
		StringCchPrintf(txt, 256, _TEXT("(%d, %d)"), MousePosX-xStart, MousePosY-yStart);
	}
	else
	{
		double graphX = (double)(MousePosX - xMid) * mpGraph->GetScaleX() / mpGraph->GetGridSpace();
		double graphY = (double)(yMid - MousePosY) *mpGraph->GetScaleY() / mpGraph->GetGridSpace();

		int roundedX = (int)round(graphX);
		int roundedY = (int)round(graphY);

		StringCchPrintf(
			txt,
			256,
			_TEXT("(%d, %d)"),
			roundedX,
			roundedY
		);
	}

	ReleaseD2D1Item(mpTxtLyout);
	HRESULT Hr = mpFactDWrite->CreateTextLayout(
		txt,
		lstrlen(txt),
		mpTxtFmt,
		400,
		25,
		&mpTxtLyout
	);
	GuardD2D1FailureVoid(Hr);

	DWRITE_TEXT_METRICS txtMetrics;
	Hr = mpTxtLyout->GetMetrics(&txtMetrics);
	GuardD2D1FailureVoid(Hr);

	D2D1_RECT_F hintRect = D2D1::RectF(
		(MousePosX + MouseCoordHintPadding) / dpiScaleX,
		(MousePosY + MouseCoordHintPadding) / dpiScaleY,
		(MousePosX + MouseCoordHintPadding) / dpiScaleX + txtMetrics.width + (MouseCoordHintInnerPadding*2),
		(MousePosY + MouseCoordHintPadding) / dpiScaleY + (txtMetrics.height) + (MouseCoordHintInnerPadding * 2)
	);
	float width = hintRect.right - hintRect.left;
	float height = hintRect.bottom - hintRect.top;
	
	//Right guard
	if (hintRect.right > xEnd / dpiScaleX) {
		hintRect.left = (MousePosX - MouseCoordHintPadding) / dpiScaleX - width;
		hintRect.right = hintRect.left + width;
	}
	//Left guard
	if (hintRect.left < xStart / dpiScaleX) {
		hintRect.left = xStart / dpiScaleX;
		hintRect.right = hintRect.left + width;
	}
	//Top guard
	if (hintRect.top < yStart / dpiScaleY) {
		hintRect.top = yStart / dpiScaleY;
		hintRect.bottom = hintRect.top + height;
	}
	//Bottom guard
	if (hintRect.bottom > yEnd / dpiScaleY) {
		hintRect.top = (MousePosY - MouseCoordHintPadding) / dpiScaleY - height;
		hintRect.bottom = hintRect.top + height;
	}

	mpBrOne->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
	mpRend->FillRectangle(hintRect, mpBrOne);

	mpBrOne->SetColor(D2D1::ColorF(D2D1::ColorF::Black));

	D2D1_POINT_2F textPos = D2D1::Point2F(
		hintRect.left + MouseCoordHintInnerPadding,
		hintRect.top + MouseCoordHintInnerPadding
	);
	mpRend->DrawTextLayout(textPos, mpTxtLyout, mpBrOne);
}

int Graphics::ConvRatioToInt(int ratio10, int Max)
{
	return ((ratio10 * Max)/10);
}

inline float Graphics::Sharp(int value)
{
	return static_cast<float>(value) + 0.5f;
}

bool Graphics::IsPointInBound(int x, int y, int yStart, int yEnd, int xStart, int xEnd)
{
	return ((y >= yStart && y <= yEnd) && (x >= xStart && x <= xEnd));
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

