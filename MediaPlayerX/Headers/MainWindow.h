#pragma once
#include "../Headers/base_window.h"
#include "../Headers/ControlBar.h"
#include "../Headers/common_include.h"
#include "../Headers/PlayList.h"
// extern ID2D1Factory* g_pDirect2dFactory;
// extern IWICImagingFactory *g_pIWICFactory;
class MainWindow : public BaseWindow 
{
public:
	~MainWindow(void);
	MainWindow();
	
	BOOL MainWindow::create_window(PCWSTR title);

protected:

	PlayList plList;
	ControlBar control;
	ID2D1SolidColorBrush* m_pLightBrownBrush;
	//ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	LRESULT HandleMessage(UINT message,WPARAM wParam,LPARAM lParam);
	HRESULT CreateD2DResources();
};
