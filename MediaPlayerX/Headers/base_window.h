#pragma  once
#include "../Headers/common_include.h"

extern ID2D1Factory* g_pDirect2dFactory;
extern IWICImagingFactory* g_pIWICFactory;
//Child parameter structure
struct ch_params
{
	HWND hwnd;
	SIZE size;
	POINT pos;
	wchar_t name[20];
};
//template <class DERIVED_TYPE> 
class BaseWindow
{

public:
	static WNDPROC BaseWindow::m_orig_WNDPROC;
	//Base procedure to delegate window events
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		BaseWindow *pThis = NULL;
		wchar_t  clsName[20];
		if (uMsg == WM_NCCREATE || uMsg==WM_CREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (BaseWindow*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
			wcscpy(clsName	,pCreate->lpszClass);
			
//clsName=pCreate->lpszClass;
			pThis->m_hwnd = hwnd;
		}
		else
		{
			pThis = (BaseWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}
		if(pThis  )
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else if(wcscmp(clsName,WC_LISTBOX)==0 )
		{
		  return CallWindowProc((WNDPROC)BaseWindow::m_orig_WNDPROC,hwnd,uMsg,wParam,lParam);
		}
		else
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
//	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, 
//                                  WPARAM wParam, 
//                                  LPARAM lParam)
//{
//  LRESULT lResult = 0;
//
//  //-- look for the creation message
//  if (msg == WM_NCCREATE)
//  {
//    CREATESTRUCT *pCreateData = (CREATESTRUCT*)lParam;
//
//    //-- get the window object passed in
//    BaseWindow *pWindow = (BaseWindow *)pCreateData->lpCreateParams;
//
//    if (pWindow)
//    {
//      //-- attach the window object to the hwnd
//      SetWindowLong(hwnd, GWLP_USERDATA,(LONG_PTR)pWindow);
//
//      //-- let the window object dispatch the message
//      pWindow->HandleMessage(msg, wParam, lParam);
//    }
//    else
//    {
//      //-- leave the message to windows
//      lResult = DefWindowProc(hwnd, msg, wParam, lParam);
//    }
//  }
//  else if (hwnd)
//  {
//    //-- get the object attached to the hwnd
//    BaseWindow *pWindow = (BaseWindow *)GetWindowLong(hwnd,GWLP_USERDATA);
//
//    //-- check to see if we have an object window attached to the handle
//    if (pWindow)
//    {
//      //-- let the window object dispatch the message
//      pWindow->HandleMessage(msg, wParam, lParam);
//    }
//
//    else
//    {
//      //-- leave the message to windows
//      lResult = ::DefWindowProc(hwnd, msg, wParam, lParam);
//    }
//	return lResult;
//  }
//	}
	~BaseWindow()
	{
		
	}
	BaseWindow()  
	{
		m_hwnd=NULL;
		created_class=false;
		m_pos.x=CW_USEDEFAULT;
		m_pos.y=CW_USEDEFAULT;
		m_size.cx=CW_USEDEFAULT;
		m_size.cy=CW_USEDEFAULT;
		m_Parent=NULL;
		m_Menu=NULL;
		m_icon=NULL;
		m_pRenderTarget=NULL;
		m_createWNDCLASS=TRUE;
		//m_WindowName=NULL;
	}
	
	//Create window class
	void CreateWndClass(
		UINT      cbSize,		UINT      style,int		cbClsExtra,	
		int       cbWndExtra,	HICON     hIcon,		HCURSOR   hCursor,		
		HBRUSH    hbrBackground,LPCTSTR   lpszMenuName,	LPCTSTR   lpszClassName,
		HICON     hIconSm		
		)
	{
		
		m_class.lpfnWndProc   = WindowProc;
		m_class.hInstance     = GetModuleHandle(NULL);
		m_class.lpszClassName = GetClassName();
		m_class.cbSize=cbSize;
		m_class.style=style;
		m_class.cbClsExtra=cbClsExtra;
		m_class.cbWndExtra=cbWndExtra;
		m_class.hIcon=hIcon;
		m_class.hCursor=hCursor;
		m_class.hbrBackground=hbrBackground;
		m_class.lpszClassName=lpszClassName;
		m_class.lpszMenuName=lpszMenuName;
		m_class.hIconSm=hIconSm;
		created_class=true;
	}
	//Create default window class
	void CreateWndClass(void)
	{
		HICON icon;
		if(m_icon==NULL)
			icon=LoadIcon(NULL,IDI_APPLICATION);
		else
			icon=m_icon;
		//(HBRUSH) GetStockObject(WHITE_BRUSH)
		CreateWndClass(sizeof(WNDCLASSEX),CS_HREDRAW|CS_VREDRAW|CS_DROPSHADOW,
			NULL,NULL,icon,
			LoadCursor(NULL,IDC_ARROW),NULL,
			NULL,m_classname,NULL);
		created_class=true;
	}
	//Set the window classname
	void SetClassName(PCWSTR name){ wcscpy_s(m_classname,name);}
	//Set if window class should be created or not
	void SetWindowClassCreate(BOOL value)
	{
		m_createWNDCLASS=value;
	}
	//Retrieve handle to window
	HWND Window() const { return m_hwnd; }
	//Get window classname
	PCWSTR  GetClassName() const{ return m_classname;}
	//Set Position
	void SetPos(int x,int y)
	{
		m_pos.x=x;
		m_pos.y=y;
	}
	POINT GetPos(void)
	{
		return m_pos;
	}
	void SetSize(int width,int height)
	{
		m_size.cx=width;
		m_size.cy=height;
	}
	int GetWidth()
	{
		GetClientRect(m_hwnd,&m_rect);
		return m_size.cx=(m_rect.right-m_rect.left);
	}
	int GetHeight()
	{
		GetClientRect(m_hwnd,&m_rect);
		return m_size.cy=(m_rect.bottom-m_rect.top);
	}
	WNDPROC GetWNDPROC()
	{
		return  (WNDPROC) GetWindowLong(m_hwnd,GWL_WNDPROC);
	}
	void SetWNDPROC()
	{
		  m_orig_WNDPROC=(WNDPROC) GetWindowLong(m_hwnd, GWL_WNDPROC);

		  //BaseWindow::m_orig_WNDPROC= originalPROC;
		SetWindowLong(m_hwnd,GWL_WNDPROC,(long) WindowProc);
		  //return originalPROC;
	}
	std::vector<ch_params> m_children;

	void AddChild(BaseWindow *child)
	{
				child->SetHParent(m_hwnd);
				ch_params cParams;
				cParams.hwnd=child->Window();
				cParams.pos=child->m_pos;
				cParams.size=child->m_size;
				wcscpy_s(cParams.name,wcslen(cParams.name),child->m_WindowName);
				m_children.push_back(cParams);
	}
		  //static WNDPROC m_orig_WNDPROC;

	protected:
		

		HWND m_hwnd;				//Window handle
		WNDCLASSEX m_class;			//Window Class Structure
		wchar_t m_classname[20];	//Window Class Name
		bool created_class;			//Bool to see if class was created
		POINT m_pos;				//Position on screen
		SIZE  m_size;				//Size of window
		HWND m_Parent;
		RECT m_rect;
		HICON m_icon;
		BOOL m_createWNDCLASS;
		
		wchar_t m_WindowName[20];	
		DWORD m_Style;			
		DWORD m_ExStyle;
		HMENU m_Menu;
		ID2D1HwndRenderTarget* m_pRenderTarget;
		//Create D2D Render Target
		HRESULT CreateRenderTarget()
		{
			if(!m_pRenderTarget)
			{

				RECT rc;
				GetClientRect(m_hwnd, &rc);

				D2D1_SIZE_U size = D2D1::SizeU(
					rc.right - rc.left,
					rc.bottom - rc.top
					);

				// Create a Direct2D render target.
				HRESULT hr=g_pDirect2dFactory->CreateHwndRenderTarget(
					D2D1::RenderTargetProperties(),
					D2D1::HwndRenderTargetProperties(m_hwnd, size),
					&m_pRenderTarget);

			
				return hr;
			}
			return -1;
		}
		virtual HRESULT CreateD2DResources()
		{
			return -1;
		}
		//Set Window Title
		void SetWindowTitle(wchar_t title[])
		{
			wcscpy_s(m_WindowName,wcslen(m_WindowName),title);
		}
		//Create window
		//Set Style
		void SetStyle(DWORD ws_style)
		{
			m_Style=ws_style;
		}
		//Set ExStyle
		void SetStyleEx(DWORD ws_ex_style)
		{
			m_ExStyle=ws_ex_style;
		}
		//Set Menu
		void SetMenu(HMENU menu)
		{
			m_Menu=menu;
		}
		void SetIconSmall(HICON icon)
		{
			m_icon=icon;
		}
		virtual void SetDefaultWndParam()
		{
		
		}
		void SetHParent(HWND parent)
		{
			m_Parent=parent;
		}
		BOOL Create()
		{
			if(m_createWNDCLASS)
			{
				if(!created_class)
				{
					CreateWndClass();
				}
			 RegisterClassEx(&m_class);
			
			}
			
			
			 m_hwnd = CreateWindowEx(
				m_ExStyle,GetClassName(), m_WindowName, m_Style, m_pos.x, m_pos.y,
				m_size.cx, m_size.cy, m_Parent, m_Menu, GetModuleHandle(NULL), this
				);
			
			  //GetClassName()
			
			
			//m_Parent=hWndParent;
			return (m_hwnd ? TRUE : FALSE);
		}
		
	//Handle Window Events
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
		{return 0;};

	

};

