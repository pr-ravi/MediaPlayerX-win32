#pragma once
#include "../Headers/base_window.h"
#include "../Headers/BaseButton.h"
#include "../Headers/Trackbar.h"
#define CONTROL_BOX_HEIGHT 50
#define CBB_DFHEIGHT 30
#define CBB_DFWIDTH 50
#define CBB_DFSPACE 20

class ControlBar : public BaseWindow
{
public:
	ControlBar(void);
	~ControlBar(void);
	void CreateLayout(void);
	void CreateButtons(void);
	BOOL create_window(PCWSTR title,HWND parent,int xPos,int yPos,int Height,int Width);
protected:
	LRESULT HandleMessage(UINT message,WPARAM wParam,LPARAM lParam);
	HRESULT CreateD2DResources();
	Trackbar tracker;
	BaseButton btnNext;
	BaseButton btnPrev;
	BaseButton btnShuffle;
	BaseButton btnRepeat;
	BaseButton btnMute;
	BaseButton btnPlaces;
	BaseButton btnPlay;
	bool hit;
	

};

