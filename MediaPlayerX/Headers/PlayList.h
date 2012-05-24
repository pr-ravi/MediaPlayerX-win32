#pragma once
#include "../Headers/base_window.h"
//long BaseWindow::m_orig_WNDPROC;
class PlayList : public BaseWindow 
{
public:
	PlayList();
  ~PlayList();
	BOOL create_window(PCWSTR title,HWND parent,int xPos,int yPos,int Height,int Width);
	//BaseWindow::m_orig_WNDPROC;
protected:
	 LRESULT HandleMessage(UINT message,WPARAM wParam,LPARAM lParam);
	 //WNDPROC m_orig_WNDPROC;
	 
};