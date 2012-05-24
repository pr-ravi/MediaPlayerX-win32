#pragma once
#include "../Headers/base_window.h"
class Trackbar :
	public BaseWindow
{
public:
	Trackbar(void);
	~Trackbar(void);
BOOL	create_window(PCWSTR title,HWND parent,int xPos,int yPos,int Height,int Width)	;
HRESULT CreateD2DResources();
void DrawBar();
protected:
	LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) ;
ID2D1Bitmap    *m_pBkgrnd;
ID2D1Bitmap * LoadBitmap(wchar_t* sz_path);
float xCaretPos;
	//HRESULT CreateD2DResources();
	
};

