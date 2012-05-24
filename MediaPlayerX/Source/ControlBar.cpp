#include "../Headers/ControlBar.h"
#include <dwmapi.h>

ControlBar::ControlBar(void)
{
}


ControlBar::~ControlBar(void)
{
	SafeRelease(&m_pRenderTarget);
}
HRESULT ControlBar::CreateD2DResources()
{
 HRESULT hr=CreateRenderTarget();
 if(SUCCEEDED(hr))
 { 
 }
 return hr;
}

void ControlBar::CreateLayout()
{
	int CBWidth=GetWidth();

	MoveWindow(tracker.Window(),0,5,GetWidth(),15,TRUE);
	//Mute Button
	MoveWindow(btnMute.Window(),CBWidth-CBB_DFWIDTH-CBB_DFSPACE,25,CBB_DFWIDTH,CBB_DFHEIGHT,TRUE);
	//Repeat Button
	MoveWindow(btnRepeat.Window(),CBWidth-2*CBB_DFWIDTH-2*CBB_DFSPACE,25,CBB_DFWIDTH,CBB_DFHEIGHT,TRUE);
	//Shuffle Button
	MoveWindow(btnShuffle.Window(),CBWidth-3*CBB_DFWIDTH-3*CBB_DFSPACE,25,CBB_DFWIDTH,CBB_DFHEIGHT,TRUE);
	//Next Button
	MoveWindow(btnNext.Window(),CBWidth-7*CBB_DFWIDTH-7*CBB_DFSPACE,25,CBB_DFWIDTH,CBB_DFHEIGHT,TRUE);
	//Play Button
	MoveWindow(btnPlay.Window(),CBWidth-8*CBB_DFWIDTH-8*CBB_DFSPACE,25,CBB_DFWIDTH,CBB_DFHEIGHT,TRUE);
	//Prev Button
	MoveWindow(btnPrev.Window(),CBWidth-9*CBB_DFWIDTH-9*CBB_DFSPACE,25,CBB_DFWIDTH,CBB_DFHEIGHT,TRUE);
}
void ControlBar::CreateButtons(void)
{
	tracker.create_window(L"Trackbar",m_hwnd,0,5,10,GetWidth());
	btnPlaces.create_window(L"Places",m_hwnd,10,20,CBB_DFHEIGHT,CBB_DFWIDTH);
	btnPlaces.CreateD2DResources();
	btnPlaces.SetInactiveImage(L"play2.jpg");
	btnPlaces.SetClickImage(L"play4.jpg");
	btnPlaces.SetHoverImage(L"play1.jpg");
	btnMute.create_window(L"Mute",m_hwnd,10,20,CBB_DFHEIGHT,CBB_DFWIDTH);
	btnRepeat.create_window(L"Repeat",m_hwnd,10,20,CBB_DFHEIGHT,CBB_DFWIDTH);
	btnShuffle.create_window(L"Shuffle",m_hwnd,10,20,CBB_DFHEIGHT,CBB_DFWIDTH);
	btnNext.create_window(L"Next",m_hwnd,10,20,CBB_DFHEIGHT,CBB_DFWIDTH);
	btnPlay.create_window(L"Play",m_hwnd,10,20,CBB_DFHEIGHT,CBB_DFWIDTH);
	btnPrev.create_window(L"Prev",m_hwnd,10,20,CBB_DFHEIGHT,CBB_DFWIDTH);
	
}

LRESULT ControlBar::HandleMessage(UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
	case WM_CREATE:
		{
			InvalidateRect(m_hwnd,NULL,TRUE);
			return 0;
		}

	case WM_PAINT:
		{
			CreateD2DResources();
			RECT Client;
			GetClientRect(GetParent(m_hwnd),&Client);
			MoveWindow(m_hwnd,0,Client.bottom-60,Client.right-Client.left,60,TRUE);
			CreateLayout();
			GetClientRect(m_hwnd,&Client);
			SetSize(Client.right,Client.bottom);
			PostMessage(btnRepeat.Window(),WM_PAINT,wParam,lParam);
			ValidateRect(m_hwnd,NULL);
			return 0;
		}
	
	case WM_SIZE:
		{
		if (m_pRenderTarget)
		{
			m_pRenderTarget->Resize(D2D1::SizeU(GetWidth(), GetHeight()));
		}
		RedrawWindow(m_hwnd,NULL,NULL,RDW_ERASE|RDW_FRAME|RDW_INVALIDATE|RDW_ALLCHILDREN);
		InvalidateRect(m_hwnd,NULL,TRUE);
		//SendMessage(m_hwnd,WM_ERASEBKGND,wParam,lParam);
		//SendMessage(m_hwnd,WM_PAINT,wParam,lParam);
		return 0;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(m_hwnd,message,wParam,lParam);
	}
	//return true;
	return DefWindowProc(m_hwnd,message,wParam,lParam);

}
BOOL ControlBar::create_window(PCWSTR title,HWND parent,int xPos,int yPos,int Height,int Width)
{
	SetClassName(L"Control Box");
	SetPos(xPos,yPos);
	SetSize(Width,Height);
	CreateWndClass(sizeof(WNDCLASSEX),CS_HREDRAW|CS_VREDRAW,NULL,NULL,NULL,NULL,(HBRUSH) GetStockObject(BLACK_BRUSH),NULL,L"Control Box",NULL);
	//return Create(title,WS_CHILD|WS_VISIBLE|BS_MULTILINE|WS_TABSTOP|WS_GROUP,parent,WS_EX_OVERLAPPEDWINDOW,NULL);
	SetWindowTitle((wchar_t *)(title));
	SetStyle(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN);
	
	SetStyleEx(NULL);
	SetHParent(parent);
	//SetMenu(NULL);
	BOOL result=Create();
	//SetLayeredWindowAttributes(m_hwnd,RGB(0,0,0),255,LWA_COLORKEY);
	ShowWindow(m_hwnd,1);
	CreateButtons();
	CreateLayout();
	return result;
}

