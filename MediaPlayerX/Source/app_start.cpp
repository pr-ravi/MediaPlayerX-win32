#pragma once
#include "../Headers/MainWindow.h"


ID2D1Factory* g_pDirect2dFactory;
IWICImagingFactory *g_pIWICFactory;
//Window Entry Point
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	InitCommonControls();
	HRESULT hr=CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
	if(SUCCEEDED(hr))
	{
		CoCreateInstance(CLSID_WICImagingFactory,NULL,CLSCTX_INPROC_SERVER,IID_IWICImagingFactory,(LPVOID*)&g_pIWICFactory);
		if(SUCCEEDED(hr))
		{
			MainWindow win;

			win.SetClassName(L"Main");
			//win.SetWindowClassCreate(FALSE);
			win.create_window(L"Main Window");
			AnimateWindow(win.Window(),1000,AW_BLEND);
			MSG msg = { };
			while (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		
		}
		CoUninitialize();
		
	}
	
	
	return 0;
}

