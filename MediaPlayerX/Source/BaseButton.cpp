#include "../Headers/BaseButton.h"
//#include "../Resource/Images/play.jpg"

//WNDPROC BaseWindow::m_orig_WNDPROC;


BaseButton::BaseButton() :BaseWindow()
{
	m_szBmpInactivePath[0]=L'\0';
	m_szBmpHoverPath[0]=L'\0';
	m_szBmpClickPath[0]=L'\0';
	m_pBmpHover=NULL;
	m_pBmpInactive=NULL;
	m_pBmpClick=NULL;
	hit=false;
	
}
BaseButton::~BaseButton(void)
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pBmpClick);
	SafeRelease(&m_pBmpHover);
	SafeRelease(&m_pBmpInactive);
}
void BaseButton::SetHoverImage(LPCTSTR imgPath)
{
	wcscpy_s(m_szBmpHoverPath,imgPath);
	m_pBmpHover=LoadBitmap(m_szBmpHoverPath);
	int i=0;
}
void BaseButton::SetClickImage(LPCTSTR imgPath)
{
	wcscpy_s(m_szBmpClickPath,imgPath);
	m_pBmpClick=LoadBitmap(m_szBmpClickPath);
}
void BaseButton::SetInactiveImage(LPCTSTR imgPath)
{
	wcscpy_s(m_szBmpInactivePath,imgPath);
	m_pBmpInactive=LoadBitmap(m_szBmpInactivePath);
}
ID2D1Bitmap * BaseButton::LoadBitmap(wchar_t* sz_path)
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
		hr = pScaler->Initialize(pSource,50,30,WICBitmapInterpolationModeCubic);
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

HRESULT BaseButton::CreateD2DResources()
{
	HRESULT hr=S_OK;
	//const wchar_t uriOpen[]=L"D:\play4.jpg";
	if(!m_pRenderTarget)
		hr=CreateRenderTarget();

	return hr;
}
LRESULT BaseButton::HandleMessage(UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
	case WM_MOUSEFIRST:
		TRACKMOUSEEVENT lp;
		lp.cbSize=sizeof(TRACKMOUSEEVENT);
		lp.dwFlags=TME_LEAVE;
		lp.hwndTrack=m_hwnd;
		lp.dwHoverTime=500;
		TrackMouseEvent(&lp);
		if(!hit)
		{
			CreateD2DResources();
			m_pRenderTarget->BeginDraw();
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			//m_pRenderTarget->FillRectangle(D2D1::RectF(0,0,GetWidth(),GetHeight()),m_pBrush);
			if(m_pBmpHover!=NULL)
				m_pRenderTarget->DrawBitmap(m_pBmpHover,D2D1::Rect<float>(0.f,0.f,GetWidth(),GetHeight()));
			else
			m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Firebrick));
			m_pRenderTarget->EndDraw();
			hit=true;
		}
		return 0;
	case WM_PAINT:
		{
			if(!hit)
			{
			CreateD2DResources();
			m_pRenderTarget->BeginDraw();
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

			//
			if(m_pBmpInactive!=NULL)
			m_pRenderTarget->DrawBitmap(m_pBmpInactive,D2D1::Rect<float>(0.f,0.f,GetWidth(),GetHeight()));
			else
			m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

			m_pRenderTarget->EndDraw();
			}
			ValidateRect(m_hwnd,NULL);
			
			return 0;
		}
	case WM_MOUSELEAVE:
		InvalidateRect(m_hwnd,NULL,TRUE);
			hit=false;
			return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		return 0;
	case WM_LBUTTONDOWN:
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		if(m_pBmpClick!=NULL)
			m_pRenderTarget->DrawBitmap(m_pBmpClick,D2D1::Rect<float>(0.f,0.f,GetWidth(),GetHeight()));
		else
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Aquamarine));
		m_pRenderTarget->EndDraw();
		return 0;
	case WM_LBUTTONUP:
		hit=false;
		InvalidateRect(m_hwnd,NULL,TRUE);
		PostMessage(m_hwnd,WM_MOUSEFIRST,NULL,FALSE);
		return 0;
	default:
		return DefWindowProc(m_hwnd,message,wParam,lParam);
	}
	//return true;
	//return DefWindowProc(m_hwnd,message,wParam,lParam);
	return CallWindowProc(m_orig_WNDPROC,m_hwnd,message,wParam,lParam);

}
BOOL BaseButton::create_window(PCWSTR title,HWND parent,int xPos,int yPos,int Height,int Width)
{
	SetClassName(WC_BUTTON);
	SetPos(xPos,yPos);
	SetSize(Width,Height);
	SetWindowClassCreate(FALSE);
	
	//CreateWndClass(sizeof(WNDCLASSEX),CS_HREDRAW|CS_VREDRAW,NULL,NULL,NULL,NULL,NULL,NULL,L"Button",NULL);
	//return Create(title,WS_CHILD|WS_VISIBLE|BS_MULTILINE|WS_TABSTOP|WS_GROUP,parent,WS_EX_OVERLAPPEDWINDOW,NULL);
	SetWindowTitle((wchar_t *)(title));
	SetStyle(WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON&~WS_BORDER);
	SetStyleEx(NULL);
	SetHParent(parent);
	//SetMenu(NULL);
	BOOL created= Create();
	//SetWNDPROC();
	return created;

	//return TRUE;
}
