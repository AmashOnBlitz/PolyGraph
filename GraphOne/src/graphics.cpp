#include "pch.h"
#include "graphics.h"
#include "CheckBox.h"
#include <string>

#define ReleaseD2D1Item(x) if (x) x->Release(); x = NULL;
#define GuardD2D1Failure(x) if (x != S_OK) return false;
#define GuardD2D1FailureVoid(x) if (x != S_OK) return;
#define MousePosValid(x) ((x) > 0)
#define TEXT_FONT_PRIMARY L"SF Pro Text"
#define TEXT_FONT_SECONDARY L"Bahnschrift"
#define CORNER_ROUNDING_X 4.0f
#define CORNER_ROUNDING_Y CORNER_ROUNDING_X 
//#define TEXT_FONT L"SF Pro Display"

Graphics::Graphics() :
	mHWnd(NULL),
	mpFact(nullptr),
	mpFactDWrite(nullptr),
	mpRend(nullptr),
	mpBrOne(nullptr),
	mpGraph(nullptr),
	mpTxtFmt(nullptr),
	mpTxtLyout(nullptr),
	mShowCoordHint(true),
	checkBoxBprint("NULL"),
	CBoxShowXIndLine(checkBoxBprint),
	CBoxShowYIndLine(checkBoxBprint),
	CBoxShowCoordHint(checkBoxBprint),
	digitBox(digitBoxBlueprint)
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
		TEXT_FONT_PRIMARY,
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		16.0f,
		L"en-us",
		&mpTxtFmt
	);
	if (FAILED(hRes))
	{
		hRes = mpFactDWrite->CreateTextFormat(
			TEXT_FONT_SECONDARY,
			NULL,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			16.0f,
			L"en-us",
			&mpTxtFmt
		);
	}
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

	InitCheckBoxes();

	return true;
}

void Graphics::InitCheckBoxes()
{
	checkBoxBprint = CheckboxBlueprint(
		"Show X Indicator Line",
		false,
		20, -1,
		CORNER_ROUNDING_X,
		CORNER_ROUNDING_Y
	);

	CBoxShowXIndLine = CheckBox(checkBoxBprint);
	CBoxShowXIndLine.GetBlueprint().SetChecked(mpGraph->GetShowXUsrIndicator());

	checkBoxBprint.SetText("Show Y Indicator Line");
	CBoxShowYIndLine = CheckBox(checkBoxBprint);
	CBoxShowYIndLine.GetBlueprint().SetChecked(mpGraph->GetShowYUsrIndicator());

	checkBoxBprint.SetText("Show Coordinate Hint");
	CBoxShowCoordHint = CheckBox(checkBoxBprint);
	CBoxShowCoordHint.GetBlueprint().SetChecked(mShowCoordHint);

}

void Graphics::InitdigitBoxes()
{
	digitBoxBlueprint = DigitBoxBlueprint("90d9");

	digitBox = DigitBox(digitBoxBlueprint);
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

void Graphics::MouseUp(int x, int y)
{
	D2D1_RECT_F rect;
	auto PassClickIfInBound = [this, &rect, &x, &y](CheckBox* cBox, std::function<void(bool)> callback) {
		rect = cBox->GetCheckBoxRect();
		if (IsPointInBound(x, y, rect.top, rect.bottom, rect.left, rect.right)) {
			cBox->PassMouseClick(x, y);
			callback(cBox->GetBlueprint().GetChecked());
		}
	};
	PassClickIfInBound(&CBoxShowXIndLine, [this](bool checked) {
		this->mpGraph->SetShowXUsrIndicator(checked);
	});
	PassClickIfInBound(&CBoxShowYIndLine, [this](bool checked) {
		this->mpGraph->SetShowYUsrIndicator(checked);
	});
	PassClickIfInBound(&CBoxShowCoordHint, [this](bool checked) {
		this->mShowCoordHint = checked;
	});
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
	RendWidgetsPanel(rect);
	mpRend->EndDraw();
}

void Graphics::RendGraph(RECT rect)
{
	gf_x_Indicator_Spacing = 10.0f;
	gf_y_Indicator_Spacing = 15.0f;
	const int PADDING = 10;
	const float INDICATORLINEWIDTH = 1.0f;
	const float AXISLINEWIDTH = 1.5f;
	const float INNER_IND_L_WIDTH =  1.0f;

	gf_xStart = rect.left + ConvRatioToInt(
		mRColWidthRatio10,
		rect.right - rect.left
	) + PADDING;
	gf_xEnd = ((rect.right - rect.left) - PADDING - 3*gf_y_Indicator_Spacing);
	gf_yStart = (rect.top + PADDING + gf_x_Indicator_Spacing);
	gf_yEnd = ((rect.bottom - rect.top) - PADDING);
	gf_xMid = ((gf_xStart + gf_xEnd) / 2);
	gf_yMid = ((gf_yStart + gf_yEnd) / 2);

	D2D1::ColorF BgCol = D2D1::ColorF::WhiteSmoke;
	if (mpGraph == nullptr) BgCol = D2D1::ColorF::Black;

	mpBrOne->SetColor(D2D1::ColorF(BgCol));
	mpRend->FillRectangle(
		D2D1::RectF(
			Sharp(gf_xStart),
			Sharp(gf_yStart),
			Sharp(gf_xEnd),
			Sharp(gf_yEnd)
		),
		mpBrOne
	);

	if (mpGraph == nullptr) {
		mpBrOne->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
		mpRend->DrawLine(
			D2D1::Point2F(
				gf_xStart,
				gf_yStart
			),
			D2D1::Point2F(
				gf_xEnd,
				gf_yEnd
			),
			mpBrOne
		);
		mpRend->DrawLine(
			D2D1::Point2F(
				gf_xStart,
				gf_yEnd
			),
			D2D1::Point2F(
				gf_xEnd,
				gf_yStart
			),
			mpBrOne
		);
	}
	else {
		mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetXRefLinesColor()));

		//Left Side X
		int currentPos = gf_xMid;
		int leftXBoxIteration = 0;
		while (currentPos > gf_xStart)
		{
			if (currentPos != gf_xMid) {
				mpRend->DrawLine(
					D2D1::Point2F(
						Sharp(currentPos),
						gf_yStart
					),
					D2D1::Point2F(
						Sharp(currentPos),
						gf_yEnd
					),
					mpBrOne,
					INDICATORLINEWIDTH
				);
				leftXBoxIteration++;
				int value = -(leftXBoxIteration * mpGraph->GetScaleX());

				DrawCenteredText(
					std::to_wstring(value),
					static_cast<float>(currentPos),
					gf_yStart - gf_x_Indicator_Spacing - 5
				);
			}

			int step = mpGraph->GetGridSpace() / 10;
			int xPos = 0;
			mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetInnerRefLinesColor()));
			for (int i = 1; i <= 9; i++) {
				xPos = currentPos - (i * step);
				if (xPos < gf_xStart) break;
				mpRend->DrawLine(
					D2D1::Point2F(
						Sharp(xPos),
						gf_yStart
					),
					D2D1::Point2F(
						Sharp(xPos),
						gf_yEnd
					),
					mpBrOne,
					INNER_IND_L_WIDTH
				);
			}
			mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetXRefLinesColor()));
			currentPos -= mpGraph->GetGridSpace();
		}
		//Right Side X
		currentPos = gf_xMid;
		int rightXBoxIteration = 0;

		while (currentPos < gf_xEnd)
		{
			if (currentPos != gf_xMid)
			{
				mpRend->DrawLine(
					D2D1::Point2F(
						Sharp(currentPos),
						gf_yStart
					),
					D2D1::Point2F(
						Sharp(currentPos),
						gf_yEnd
					),
					mpBrOne,
					INDICATORLINEWIDTH
				);
				rightXBoxIteration++;
				int value = rightXBoxIteration * mpGraph->GetScaleX();

				DrawCenteredText(
					std::to_wstring(value),
					static_cast<float>(currentPos),
					gf_yStart - gf_x_Indicator_Spacing - 5
				);
			}
			int step = mpGraph->GetGridSpace() / 10;
			int xPos = 0;
			mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetInnerRefLinesColor()));
			for (int i = 1; i <= 9; i++) {
				xPos = currentPos + (i * step);
				if (xPos > gf_xEnd) break;
				mpRend->DrawLine(
					D2D1::Point2F(
						Sharp(xPos),
						gf_yStart
					),
					D2D1::Point2F(
						Sharp(xPos),
						gf_yEnd
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
		currentPos = gf_yMid;
		int topYBoxIteration = 0;
		while (currentPos > gf_yStart)
		{
			if (currentPos != gf_yMid) {
				mpRend->DrawLine(
					D2D1::Point2F(
						gf_xStart,
						Sharp(currentPos)
					),
					D2D1::Point2F(
						gf_xEnd,
						Sharp(currentPos)
					),
					mpBrOne,
					INDICATORLINEWIDTH
				);
				topYBoxIteration++;
				int value = topYBoxIteration * mpGraph->GetScaleY();

				DrawCenteredText(
					std::to_wstring(value),
					gf_xEnd + gf_y_Indicator_Spacing,
					static_cast<float>(currentPos)
				);
			}
			int step = mpGraph->GetGridSpace() / 10;
			int yPos = 0;
			mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetInnerRefLinesColor()));
			for (int i = 1; i <= 9; i++) {
				yPos = currentPos - (i * step);
				if (yPos < gf_yStart) break;
				mpRend->DrawLine(
					D2D1::Point2F(
						gf_xStart,
						Sharp(yPos)
					),
					D2D1::Point2F(
						gf_xEnd,
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
		currentPos = gf_yMid;
		int bottomYBoxIteration = 0;
		while (currentPos < gf_yEnd)
		{
			if (currentPos != gf_yMid) {
				mpRend->DrawLine(
					D2D1::Point2F(
						gf_xStart,
						Sharp(currentPos)
					),
					D2D1::Point2F(
						gf_xEnd,
						Sharp(currentPos)
					),
					mpBrOne,
					INDICATORLINEWIDTH
				); 
				
				bottomYBoxIteration++;
				int value = -(bottomYBoxIteration * mpGraph->GetScaleY());

				DrawCenteredText(
					std::to_wstring(value),
					gf_xEnd + gf_y_Indicator_Spacing,
					static_cast<float>(currentPos)
				);
			}
			int step = mpGraph->GetGridSpace() / 10;
			int yPos = 0;
			mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetInnerRefLinesColor()));
			for (int i = 1; i <= 9; i++) {
				yPos = currentPos + (i * step);
				if (yPos > gf_yEnd) break;
				mpRend->DrawLine(
					D2D1::Point2F(
						gf_xStart,
						Sharp(yPos)
					),
					D2D1::Point2F(
						gf_xEnd,
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
				gf_xStart,
				Sharp(gf_yMid)
			),
			D2D1::Point2F(
				gf_xEnd,
				Sharp(gf_yMid)
			),
			mpBrOne,
			AXISLINEWIDTH
		);

		//Y axis
		mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetYAxisColor()));
		mpRend->DrawLine(
			D2D1::Point2F(
				Sharp(gf_xMid),
				gf_yStart
			),
			D2D1::Point2F(
				Sharp(gf_xMid),
				gf_yEnd
			),
			mpBrOne,
			AXISLINEWIDTH
		);
		if (
			IsPointInBound(
				MousePosX,
				MousePosY,
				gf_yStart,
				gf_yEnd,
				gf_xStart,
				gf_xEnd
			)
			) 
		{
			//Usr Indicator X
			mpBrOne->SetColor(D2D1::ColorF(mpGraph->GetUsrIndicatorLineColor()));
			if (mpGraph->GetShowXUsrIndicator() && MousePosValid(MousePosX)) {
				mpRend->DrawLine(
					D2D1::Point2F(
						MousePosX,
						gf_yStart
					),
					D2D1::Point2F(
						MousePosX,
						gf_yEnd
					),
					mpBrOne
				);
			}

			//Usr Indicator Y
			if (mpGraph->GetShowYUsrIndicator() && MousePosValid(MousePosY)) {
				mpRend->DrawLine(
					D2D1::Point2F(
						gf_xStart,
						MousePosY
					),
					D2D1::Point2F(
						gf_xEnd,
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
		gf_yStart,
		gf_yEnd,
		gf_xStart,
		gf_xEnd) ||
		!mShowCoordHint
		) 
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
		StringCchPrintf(txt, 256, _TEXT("(%d, %d)"), MousePosX-gf_xStart, MousePosY-gf_yStart);
	}
	else
	{
		double graphX = (double)(MousePosX - gf_xMid) * mpGraph->GetScaleX() / mpGraph->GetGridSpace();
		double graphY = (double)(gf_yMid - MousePosY) *mpGraph->GetScaleY() / mpGraph->GetGridSpace();

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
	if (hintRect.right > gf_xEnd / dpiScaleX) {
		hintRect.left = (MousePosX - MouseCoordHintPadding) / dpiScaleX - width;
		hintRect.right = hintRect.left + width;
	}
	//Left guard
	if (hintRect.left < gf_xStart / dpiScaleX) {
		hintRect.left = gf_xStart / dpiScaleX;
		hintRect.right = hintRect.left + width;
	}
	//Top guard
	if (hintRect.top < gf_yStart / dpiScaleY) {
		hintRect.top = gf_yStart / dpiScaleY;
		hintRect.bottom = hintRect.top + height;
	}
	//Bottom guard
	if (hintRect.bottom > gf_yEnd / dpiScaleY) {
		hintRect.top = (MousePosY - MouseCoordHintPadding) / dpiScaleY - height;
		hintRect.bottom = hintRect.top + height;
	}

	D2D1_ROUNDED_RECT roundedHintRect = D2D1::RoundedRect(
		hintRect,
		CORNER_ROUNDING_X,
		CORNER_ROUNDING_Y
	);

	mpBrOne->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
	mpRend->FillRoundedRectangle(roundedHintRect, mpBrOne);

	mpBrOne->SetColor(D2D1::ColorF(D2D1::ColorF::Black));

	D2D1_POINT_2F textPos = D2D1::Point2F(
		hintRect.left + MouseCoordHintInnerPadding,
		hintRect.top + MouseCoordHintInnerPadding
	);
	mpRend->DrawTextLayout(textPos, mpTxtLyout, mpBrOne);
	ReleaseD2D1Item(mpTxtLyout);
}

void Graphics::RendWidgetsPanel(RECT rect)
{
	D2D1_RECT_F widgetPanelAr = {};
	widgetPanelAr.left = rect.left + widgetPanelPadding;
	widgetPanelAr.right = gf_xStart - widgetPanelPadding;
	widgetPanelAr.top = rect.top + widgetPanelPadding;
	widgetPanelAr.bottom = rect.bottom - widgetPanelPadding;

	int widgetPanelWidth = (widgetPanelAr.right - widgetPanelAr.left);
	int widgetPanelHeight = (widgetPanelAr.bottom - widgetPanelAr.top);

	mpBrOne->SetColor(D2D1::ColorF(D2D1::ColorF::FloralWhite));
	mpRend->FillRectangle(widgetPanelAr, mpBrOne);

	D2D1_RECT_F configArea = widgetPanelAr;
	configArea.top = widgetPanelAr.top + widgetPanelHeight / 2.0f;
	const float layoutHeight = 25.0f;

	TCHAR txt[]= _TEXT("Configurations");
	ReleaseD2D1Item(mpTxtLyout);
	HRESULT Hr = mpFactDWrite->CreateTextLayout(
		txt,
		lstrlen(txt),
		mpTxtFmt,
		400,
		layoutHeight,
		&mpTxtLyout
	);

	GuardD2D1FailureVoid(Hr);
	DWRITE_TEXT_METRICS txtMetric;
	Hr = mpTxtLyout->GetMetrics(&txtMetric);
	GuardD2D1FailureVoid(Hr);

	float configAreaHeight = configArea.bottom - configArea.top;
	float textWidth = txtMetric.width;
	float textHeight = txtMetric.height;
	float textStartX = configArea.left + 5;
	float textStartY = configArea.top;
	float lineCenterY = textStartY + layoutHeight / 2.0f + 1.0f;

	mpBrOne->SetColor(D2D1::ColorF(D2D1::ColorF::Gray));

	mpRend->DrawTextLayout(
		D2D1::Point2F(Sharp(textStartX), Sharp(textStartY)),
		mpTxtLyout,
		mpBrOne
	);

	mpRend->DrawLine(
		D2D1::Point2F(
			Sharp(textStartX + textWidth + 10),
			Sharp(lineCenterY)
		),
		D2D1::Point2F(
			Sharp(configArea.right - 5),
			Sharp(lineCenterY)
		),
		mpBrOne
	);

	RendConfigPanel(configArea, (textStartY + textHeight + 10), textStartX);
}

void Graphics::RendConfigPanel(D2D1_RECT_F configPanel, int yStart, int xStart)
{
	D2D1_RECT_F checkBoxesArea = configPanel;
	checkBoxesArea.top = yStart;
	checkBoxesArea.left = xStart;
	CBoxHeight = checkBoxBprint.GetHeight();
	CBoxYSpacing = CBoxHeight + 5;
	int i = 0;
	CBoxShowXIndLine.Render(mpRend,mpFactDWrite,mpBrOne,mpTxtFmt,mpTxtLyout, checkBoxesArea.left, checkBoxesArea.top + (i++*CBoxYSpacing));
	CBoxShowYIndLine.Render(mpRend,mpFactDWrite,mpBrOne,mpTxtFmt,mpTxtLyout, checkBoxesArea.left, checkBoxesArea.top + (i++*CBoxYSpacing));
	CBoxShowCoordHint.Render(mpRend,mpFactDWrite,mpBrOne,mpTxtFmt,mpTxtLyout, checkBoxesArea.left, checkBoxesArea.top + (i++*CBoxYSpacing));


	digitBox.Render(mpRend,mpFactDWrite,mpBrOne,mpTxtFmt,mpTxtLyout, checkBoxesArea.left, checkBoxesArea.top + 0);
	
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

void Graphics::SetShowCoordHint(bool show)
{
	this->mShowCoordHint = show;
}

void Graphics::DrawCenteredText(
	const std::wstring& text,
	float centerX,
	float centerY,
	float maxWidth,
	float maxHeight
)
{
	ReleaseD2D1Item(mpTxtLyout);

	HRESULT hr = mpFactDWrite->CreateTextLayout(
		text.c_str(),
		static_cast<UINT32>(text.length()),
		mpTxtFmt,
		maxWidth,
		maxHeight,
		&mpTxtLyout
	);
	GuardD2D1FailureVoid(hr);

	hr = mpTxtLyout->SetFontWeight(
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_TEXT_RANGE{
			0,
			static_cast<UINT32>(text.length())
		}
	);
	GuardD2D1FailureVoid(hr);

	DWRITE_TEXT_METRICS metrics;
	hr = mpTxtLyout->GetMetrics(&metrics);
	GuardD2D1FailureVoid(hr);

	float x = centerX - (metrics.width / 2.0f);
	float y = centerY - (metrics.height / 2.0f);

	mpRend->DrawTextLayout(
		D2D1::Point2F(x, y),
		mpTxtLyout,
		mpBrOne
	);

	ReleaseD2D1Item(mpTxtLyout);
}

int Graphics::GetRColWidthRatio()
{
	return this->mRColWidthRatio10;
}

bool Graphics::GetShowCoordHint()
{
	return this->mShowCoordHint;
}

