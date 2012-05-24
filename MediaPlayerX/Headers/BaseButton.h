#pragma once
#include "base_window.h"
extern ID2D1Factory* g_pDirect2dFactory;
extern IWICImagingFactory *g_pIWICFactory;
class BaseButton : public BaseWindow//<BaseButton>
{
public:
	~BaseButton(void);
	BaseButton(void);
	BOOL create_window(PCWSTR title,HWND parent,int xPos,int yPos,int Height,int Width);
	
	void SetHoverImage(LPCTSTR imgPath);
	void SetInactiveImage(LPCTSTR imgPath);
	void SetClickImage(LPCTSTR imgPath);
	ID2D1Bitmap * LoadBitmap(wchar_t* sz_path);
	//ID2D1Bitmap *p_bmp,
	HRESULT CreateD2DResources();
protected:
	LRESULT HandleMessage(UINT message,WPARAM wParam,LPARAM lParam);
	
	bool hit;

	ID2D1Bitmap    *m_pBmpHover;
	ID2D1Bitmap    *m_pBmpClick;
	ID2D1Bitmap    *m_pBmpInactive;
	wchar_t m_szBmpHoverPath[50];
	wchar_t m_szBmpClickPath[50];
	wchar_t m_szBmpInactivePath[50];
	
	
	//IWICBitmapDecoder *m_pDecoder;
	//LRESULT  BaseButton::BaseButton::HandleMessage(UINT message,WPARAM wParam,LPARAM lParam);
};

