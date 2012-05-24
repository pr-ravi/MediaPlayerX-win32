#include "../Headers/MainWindow.h"
#include <dwmapi.h>
 //Direct2D
//extern WNDPROC m_orig_WNDPROC;
MainWindow::MainWindow() :BaseWindow()
{
	g_pDirect2dFactory=NULL;
	g_pIWICFactory=NULL;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pDirect2dFactory);
	CoCreateInstance(CLSID_WICImagingFactory,NULL,CLSCTX_INPROC_SERVER,IID_IWICImagingFactory,reinterpret_cast<void **>(&g_pIWICFactory));

}
MainWindow::~MainWindow(void)
{
	SafeRelease(&g_pDirect2dFactory);
	SafeRelease(&g_pIWICFactory);
	SafeRelease(&m_pRenderTarget);

 }


 LRESULT  MainWindow::HandleMessage(UINT message,WPARAM wParam,LPARAM lParam) 
{
	switch(message)
	{
	case WM_CREATE:
		{
			CreateD2DResources(); 
			//SendMessage(control.Window(),WM_PAINT,wParam,lParam);
			return 0;
		}
	case WM_ERASEBKGND:
		{
			//MARGINS margins = { -1, -1, -1, -1 };

			//DwmExtendFrameIntoClientArea(m_hwnd, &margins);

			RECT rc;
			GetClientRect(m_hwnd, &rc);
			HBRUSH brush = CreateSolidBrush(RGB(0,0,0));
			FillRect((HDC)wParam, &rc, brush);
			DeleteObject(brush);
			InvalidateRect(m_hwnd,NULL,TRUE);


			//SendMessage(control.Window(),WM_SIZE,wParam,lParam);
			return TRUE;
		}
	case WM_PAINT:
		{
			CreateD2DResources();
			//SendMessage(control.Window(),WM_PAINT,NULL,NULL);
// 			m_pRenderTarget->BeginDraw();
// 			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
// // 			m_pRender Target->FillRectangle(
// // 				D2D1::RectF(0,0,GetWidth(),GetHeight()),
// // 				m_pLightBrownBrush);
// 			m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
// 			m_pRenderTarget->EndDraw();
			ValidateRect(m_hwnd,NULL);
			return 0;
		}
		case WM_COMMAND:
	  {
		 /* switch(LOWORD(wParam))
		  {
		  case LIST_ID:
			  switch(HIWORD(wParam))
			  {
			  case LBN_DBLCLK:
				  MessageBox(hWnd,"...","...",MB_OK);
			  }
		  }*/
		  if(lParam==(LPARAM)plList.Window())
				switch(HIWORD(wParam))
			  {
			  case LBN_DBLCLK:
				  {
				  int index=SendMessage(plList.Window(),LB_GETCURSEL,(WPARAM)0,(LPARAM)0);
				  int textLen = (int) SendMessage(plList.Window(), LB_GETTEXTLEN, (WPARAM) index, 0);
				  TCHAR * textBuffer = new TCHAR[textLen + 1];
				  int a=SendMessage(plList.Window(),LB_GETTEXT,(WPARAM)index,(LPARAM)textBuffer);
				  MessageBox(m_hwnd,textBuffer,L"...",MB_OK);
				  a=0;
			  }
		  }
		  return 0;
	  }
	case WM_SIZE:
		{
				if (m_pRenderTarget)
				{
					m_pRenderTarget->Resize(D2D1::SizeU(GetWidth(), GetHeight()));
				}
			
// 			HWND CBOX=
// 				FindWindowEx(m_hwnd,NULL,NULL,L"Control Box");
			InvalidateRect(m_hwnd,NULL,TRUE);
			//SendMessage(m_hwnd,WM_ERASEBKGND,wParam,lParam);
			//SendMessage(m_hwnd,WM_PAINT,wParam,lParam);
			//SendMessage(control.Window(),WM_PAINT,wParam,lParam);
			SendMessage(plList.Window(),WM_SIZE,wParam,lParam);
			PostMessage(control.Window(),WM_SIZE,wParam,lParam);	
			return 0;
		}
		
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	
	default:
	return DefWindowProc(m_hwnd,message,wParam,lParam);
	}
	return true;

}
 HRESULT MainWindow::CreateD2DResources()
 {
	HRESULT hr=CreateRenderTarget();
	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::BlueViolet),&m_pLightBrownBrush);
	return hr;
 }
 
 BOOL MainWindow::create_window(PCWSTR title)
 {
//	 SetIconSmall(LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDFAVDEF)));
	 BOOL wndCreated=FALSE;
	 SetWindowTitle(L"Main Window");
	 SetStyle(WS_CLIPCHILDREN|WS_TILEDWINDOW);
	 SetStyleEx(WS_EX_ACCEPTFILES|WS_EX_APPWINDOW);
	   if (wndCreated=Create())
	   {
		   MARGINS margins = { -1, -1, -1, -1 };
		   DwmExtendFrameIntoClientArea(m_hwnd, &margins);
		   _DWM_BLURBEHIND DwmInfo;
		   DwmInfo.fEnable=TRUE;
		   DwmInfo.dwFlags=DWM_BB_ENABLE|DWM_BB_TRANSITIONONMAXIMIZED ;
		   DwmInfo.hRgnBlur=NULL;
		   DwmInfo.fTransitionOnMaximized=TRUE;
		   //DwmEnableBlurBehindWindow(m_hwnd,&DwmInfo);
		   control.create_window(L"Control Box",m_hwnd,0,this->GetHeight()-60,60,this->GetWidth());
		   plList.create_window(L"PlayList",m_hwnd,this->GetWidth()-305,20,this->GetHeight()-90,400);

				TCHAR pszDest[30]; 
				size_t cbDest = 30 * sizeof(TCHAR);

				LPCTSTR pszFormat = TEXT("%d");
				TCHAR* pszTxt = TEXT("The answer is");

					

// The resultant string at pszDest is "The answer is 1 + 2 = 3."
		   	for(int i=0;i<1000;i++)
			{
				StringCbPrintf(pszDest, cbDest, pszFormat, i);
				SendMessage(plList.Window(),LB_ADDSTRING,0,(LPARAM) pszDest );
			}
		   // control.CreateLayout();
		  // SendMessage(control.Window(),WM_SIZE,NULL,NULL);
	   }
	   //SetLayeredWindowAttributes(m_hwnd,RGB(0,0,0),0,LWA_COLORKEY);
	   //UpdateWindow(m_hwnd);
	   //ShowWindow(m_hwnd,);
	return wndCreated;
 }


