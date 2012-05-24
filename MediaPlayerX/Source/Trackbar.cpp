#include "../Headers/Trackbar.h"


Trackbar::Trackbar(void)
{
	xCaretPos=0.005;
	m_pBkgrnd=NULL;

}

ID2D1Bitmap * Trackbar::LoadBitmap(wchar_t* sz_path)
{
	ID2D1Bitmap *p_bmp=NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;
	IWICFormatConverter *pConverter = NULL;
	IWICBitmapScaler *pScaler = NULL;
	HRESULT hr;
	hr = g_pIWICFactory->CreateDecoderFromFilename(sz_path,NULL,GENERIC_READ,WICDecodeMetadataCacheOnLoad,&pDecoder);
	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{
		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = g_pIWICFactory->CreateFormatConverter(&pConverter);
	}
	if (SUCCEEDED(hr))
	{
		// If a new width or height was specified, create an
		// IWICBitmapScaler and use it to resize the image.
		UINT originalWidth, originalHeight;
		hr = pSource->GetSize(&originalWidth, &originalHeight);
	}
	if (SUCCEEDED(hr))
	{

		hr =g_pIWICFactory->CreateBitmapScaler(&pScaler);
	}
	if (SUCCEEDED(hr))
	{
		hr = pScaler->Initialize(pSource,GetWidth(),GetHeight(),WICBitmapInterpolationModeCubic);
	}
	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(pScaler,GUID_WICPixelFormat32bppPBGRA,WICBitmapDitherTypeNone,NULL,0.f,WICBitmapPaletteTypeMedianCut);
	}

	if (SUCCEEDED(hr))
	{
		// Create a Direct2D bitmap from the WIC bitmap.
		hr =m_pRenderTarget->CreateBitmapFromWicBitmap(pConverter,NULL,&p_bmp);
	}


	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);
	// 	if (SUCCEEDED(hr))
	// 	{
	// 		return TRUE;
	// 	}
	// 	else
	// 		return FALSE;
	return p_bmp;
}
Trackbar::~Trackbar(void)
{
	SafeRelease(&m_pBkgrnd);
}
// HRESULT Trackbar::CreateD2DResources()
// {
// 	HRESULT hr=S_OK;
// 	//const wchar_t uriOpen[]=L"D:\play4.jpg";
// 	if(!m_pRenderTarget)
// 		hr=CreateRenderTarget();
// 
// 	return hr;
// }
BOOL Trackbar::create_window(PCWSTR title,HWND parent,int xPos,int yPos,int Height,int Width)
{
	SetClassName(L"Trackbar");
	SetPos(xPos,yPos);
	SetSize(Width,Height);
	CreateWndClass(sizeof(WNDCLASSEX),CS_HREDRAW|CS_VREDRAW,NULL,NULL,NULL,NULL,(HBRUSH) RGB(0,0,0),NULL,L"Trackbar",NULL);
	//return Create(title,WS_CHILD|WS_VISIBLE|BS_MULTILINE|WS_TABSTOP|WS_GROUP,parent,WS_EX_OVERLAPPEDWINDOW,NULL);
	SetWindowTitle((wchar_t *)(title));
	SetStyle(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN);
	SetStyleEx(NULL);
	SetHParent(parent);
	//SetMenu(NULL);
	BOOL result=Create();
	//CreateButtons();
	//CreateLayout();
	return result;
}
HRESULT Trackbar::CreateD2DResources()
{
	HRESULT hr=CreateRenderTarget();
	if(SUCCEEDED(hr))
	{ 
		m_pBkgrnd=LoadBitmap(L"blackBkgrnd.jpg");
	}
	return hr;
}
void Trackbar::DrawBar()
{
	//UpdateWindow(m_hwnd);
	CreateD2DResources();
	ID2D1SolidColorBrush *brBkgrnd;
	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AntiqueWhite),&brBkgrnd);
	
	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	//if(m_pBkgrnd!=NULL)m_pRenderTarget->DrawBitmap(m_pBkgrnd,D2D1::Rect<float>(0.f,0.f,GetWidth(),GetHeight()));
	//m_pRenderTarget->Clear(D2D1::ColorF(0,0,0,0.0f))	 ;
	//m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	m_pRenderTarget->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(0.f,GetHeight()/3,GetWidth(),GetHeight()/3),5.0f,5.0f),brBkgrnd,1.0f);
	m_pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(xCaretPos*GetWidth(),GetHeight()/3),5.0f,5.0f),brBkgrnd)   ;
	//DrawFilledEllipse(D2D1::Ellipse(D2D1::Point2F(5,GetHeight()/2),5.0f,50.f),brBkgrnd);

	//m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Coral));
	m_pRenderTarget->EndDraw();
	SafeRelease(&brBkgrnd);
}
LRESULT Trackbar::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_PAINT:
		{
			
			DrawBar();
			ValidateRect(m_hwnd,NULL);
			return 0;
		}
	//case WM_ERASEBKGND:
		{
// 			RECT rc;
// 			LPPAINTSTRUCT *ps;
// 			ps->hdc((HDC) wParam);
// 			ps->fErase=FALSE;
// 			ps->
// 			BeginPaint(m_hwnd)
// 			GetClientRect(m_hwnd, &rc);
// 			HBRUSH brush = CreateSolidBrush(RGB(0,0,0));
// 			//(HDC)wParam
// 			//HDC hc=GetDC(m_hwnd);
// 			FillRect((HDC) wParam, &rc, brush)
// 			//MessageBox(m_hwnd,L"Filled",L"...",MB_OK);
// 			//ReleaseDC(m_hwnd,hc);
// 			DeleteObject(brush);
		//	return 1;
		}
	
	case WM_SIZE:
		{
			if (m_pRenderTarget)
			{
				m_pRenderTarget->Resize(D2D1::SizeU(GetWidth(),GetHeight()));
			}
		//	RedrawWindow(m_hwnd,NULL,NULL,RDW_ERASE|RDW_FRAME|RDW_INVALIDATE|RDW_ALLCHILDREN);
			InvalidateRect(m_hwnd,NULL,TRUE);
			return 0;
		}
	case WM_LBUTTONDOWN:
		{
			//SendMessage(m_hwnd,WM_ENTERSIZEMOVE,(WPARAM)GetDC(m_hwnd),NULL);
			//MoveWindow(m_hwnd,0,0,GetWidth(),GetHeight(),FALSE);
			//ShowWindow(m_hwnd,SW_HIDE);
			//ShowWindow(m_hwnd,SW_SHOW);
			
			xCaretPos=((float)LOWORD(lParam)/(float)GetWidth());
			//UpdateWindow(m_hwnd);
			//InvalidateRect(m_hwnd,NULL,TRUE);
			SendMessage(m_hwnd,WM_ERASEBKGND,(WPARAM) GetDC(m_hwnd),lParam);
			DrawBar();
			//InvalidateRect(m_hwnd,NULL,TRUE);
			return 0;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	default:
		return DefWindowProc(m_hwnd,message,wParam,lParam);
	}
}
