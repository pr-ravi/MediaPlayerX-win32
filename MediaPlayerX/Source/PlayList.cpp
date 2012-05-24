#include "../Headers/PlayList.h"
WNDPROC BaseWindow::m_orig_WNDPROC;
LRESULT PlayList::HandleMessage(UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
	case WM_SIZE:
		{
			RECT Client;
			GetClientRect(GetParent(m_hwnd),&Client);
			MoveWindow(m_hwnd,Client.right-305,20,300,Client.bottom-90,TRUE);
			//SendMessage(m_hwnd,WM_ENABLE,(WPARAM)FALSE,NULL);
			//RedrawWindow(m_hwnd,NULL,NULL,RDW_ERASE|RDW_FRAME|RDW_INVALIDATE|RDW_ALLCHILDREN);
			break;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}

	}
		return CallWindowProc(m_orig_WNDPROC,m_hwnd,message,wParam,lParam);
}
PlayList::PlayList() : BaseWindow()
{

}
PlayList::~PlayList()
{

}
BOOL PlayList::create_window(PCWSTR title,HWND parent,int xPos,int yPos,int Height,int Width)
{
	SetClassName(WC_LISTBOX);
	SetWindowClassCreate(FALSE);
	SetWindowTitle(L"PlayList");
	//SetPos(PLAY_POS_X,PLAY_POS_Y);
	SetPos(xPos,yPos);
	SetSize(Width,Height); 
	//(HBRUSH) GetStockObject(WHITE_BRUSH)
	//CreateWndClass(sizeof(WNDCLASSEX),CS_HREDRAW|CS_VREDRAW,NULL,NULL,NULL,NULL,NULL,NULL,L"Listbox",NULL);
	SetStyle( LBS_STANDARD | LBS_SORT | WS_VSCROLL | WS_CHILD | WS_TABSTOP | WS_VISIBLE|LBS_NOTIFY);
	SetHParent(parent);
	SetStyleEx(WS_EX_CLIENTEDGE);
	BOOL created=Create();
	SetWNDPROC();
	
	if(created)
	{
		BaseWindow* ptr;
		//SetWindowLong(m_hwnd, GWL_WNDPROC,(long) HandleMessage);
	}
	return created;
	//ShowWindow(m_hwnd,1);
}
