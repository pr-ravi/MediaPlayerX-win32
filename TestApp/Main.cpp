

/*Main header file*/
#include"glb_func.h"

/*Manifest for windows theming*/
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
enum {LIST_ID};



//Main window WNDPROC
LRESULT WINAPI WindowProc(HWND hWnd, UINT message, 
						  WPARAM wParam, LPARAM lParam)
{
	HDC hDC;                       // Display context handle
	PAINTSTRUCT PaintSt;           // Structure defining area to be drawn
	RECT aRect;                    // A working rectangle
	LPDRAWITEMSTRUCT lpdis; 
	const int BUFFER=1000;
	TCHAR tchBuffer[BUFFER];
	size_t pcch=BUFFER * sizeof(TCHAR);
	HRESULT hr;
	int y;
	TEXTMETRIC tm;
  switch(message)  
	  {
  
  case WM_CREATE:
	  {
		//Create the places menu and add it to the main window
		HMENU hmenu0 = CreateMenu();
		HMENU hmenu0_1 = CreatePopupMenu();
		AppendMenu(hmenu0_1, MF_STRING, IDM_PLAYER, ("Player"));
		AppendMenu(hmenu0_1, MF_STRING, IDM_LIBRARY, ("Library"));
		AppendMenu(hmenu0, MF_STRING | MF_POPUP, (UINT_PTR)hmenu0_1, ("Places"));
		SetMenu(hWnd,hmenu0);
		break;

	  }
  case WM_NOTIFY:
	  {
		  //For notificaiton of child controls,not used atm
		  if(wParam==(WPARAM)IDR_PLACES)
		  {
			  switch (((LPNMHDR)lParam)->code)
			  {
			  case BCN_DROPDOWN:
				  {
					  HMENU plMenu=LoadMenu(GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_PLACES));
					  RECT rc=((NMBCDROPDOWN *)lParam)->rcButton;
					  //TrackPopupMenuEx(plMenu,TPM_RIGHTBUTTON,rc.left,rc.bottom,((NMBCDROPDOWN *)lParam)->hdr,NULL);

				  }

			  }
		  }
		  return 0;
	  }
  case WM_CTLCOLORLISTBOX:
	  {
		  //Set the background for custom drawn controls
		  return (LRESULT) CreateSolidBrush(  RGB(0,0,0));
			  //RGB(241,245,251));

	  }
		  // Process selected messages
		  //Process WM_COMMAND messages
	case WM_COMMAND:
		  {
			  //Is it from one of the listbox?

			  if(lParam==(LPARAM)hPlayList || lParam==(LPARAM)hAlbumList || lParam==(LPARAM)hSongList || lParam==(LPARAM)hArtistList) 
			  {
				switch(HIWORD(wParam))
				//Double Click event
				  case LBN_DBLCLK:
				{
					SelChangeListBox((HWND) lParam,wParam);
					return 0;
				}
					
					break;;
			  }
			  //Is it from the add to library button?
			  else if(lParam==(LPARAM) hBtnAdd)
			  {
				  //Clicked event
				  switch(HIWORD(wParam))
					case BN_CLICKED:	
				  {
					  BOOL result=AddToLibrary();
					  if(result==FALSE)
						  MessageBox(hWnd,"Could not enter into the database..","Error",MB_ICONEXCLAMATION|MB_OK);
					  else
						   MessageBox(hWnd,"Added to database",":)",MB_OK);
				  }
				  
			  }
			  //Play Button?
			   else if(lParam==(LPARAM) hBtnPlay)
			  {
				  switch(HIWORD(wParam))
					case BN_CLICKED:	
				  {

					  if(!pause)
					  {
						  //Pause audio and suspend the SDL thread
						  pause=TRUE;
						  SetWindowText(hBtnPlay,"Play");
						  SDL_PauseAudio(1);
						  SuspendThread(th_play);
					  }
					  else
					  {
						  //Resume audio and thread
						  pause=FALSE;
						  SetWindowText(hBtnPlay,"Pause");
						  SDL_PauseAudio(0);
						  ResumeThread(th_play);
					  }
				  }
				  
			  }
			   else if(lParam==(LPARAM) hBtnPrev)
			   {
				  
					   if(selIndex.Playlist>0)
					   {
						   SendMessage(hPlayList,LB_SETCURSEL,(WPARAM)selIndex.Playlist-1,0);
						    SendMessage(hWnd,WM_COMMAND,MAKEWPARAM(LIST_ID,LBN_DBLCLK), (LPARAM) hPlayList);
					   }
						  
				    break;
			   }
			   else if(lParam=(LPARAM)hBtnNext)
			   {
				   int max_index=SendMessage(hPlayList,LB_GETCOUNT,0,0);
				   if(max_index!=LB_ERR)
				   {
					   if(selIndex.Playlist<max_index)
					   {
						   SendMessage(hPlayList,LB_SETCURSEL,(WPARAM)selIndex.Playlist+1,0);
						    SendMessage(hWnd,WM_COMMAND,MAKEWPARAM(LIST_ID,LBN_DBLCLK), (LPARAM) hPlayList);
					   }
						  
				   }
				  break;
			   }
			  switch(LOWORD(wParam))
			  {
				  //Hide and show controls on clicking appropriate menu item
				   case IDM_PLAYER:
				  {
					  ShowWindow(hArtistList,SW_HIDE);
					  ShowWindow(hAlbumList,SW_HIDE);
					  ShowWindow(hSongList,SW_HIDE);

					  break;
				  }
				  case IDM_LIBRARY:
				  {
					  ShowWindow(hArtistList,SW_SHOW);
					  ShowWindow(hAlbumList,SW_SHOW);
					  ShowWindow(hSongList,SW_SHOW);

					 break;
				  }
			  }

				return 0;
		  }
	  
	case WM_DROPFILES:
		{
			//Number of files dragged
			int numFiles=DragQueryFile((HDROP)wParam,-1,NULL,0);
			int path_len,slash_index;
			char num[10];
			char szFilePath[100];
			char szFileFormat[6];
			char szFileName[80];
			BOOL add=FALSE;
			sprintf(num,"%d",numFiles);
			//MessageBox(hWnd,num,"Drag",MB_OK);
			for(int i=0;i<numFiles;i++)
			{

				DragQueryFile((HDROP) wParam,i,szFilePath,99);
				path_len=strlen(szFilePath);
				//Get the last four characters
				szFileFormat[0]=szFilePath[path_len-1-3];
				szFileFormat[1]=szFilePath[path_len-1-2];
				szFileFormat[2]=szFilePath[path_len-1-1];
				szFileFormat[3]=szFilePath[path_len-1-0];
				szFileFormat[4]='\0';
				//Match it to accepted file extensions
				if(
				   strcmp(szFileFormat,".mp3")==0 ||
				  // strcmp(szFileFormat,".ogg")==0 ||
				   strcmp(szFileFormat,".mpg")==0 ||
				   //strcmp(szFileFormat,".flv")==0 ||
				   strcmp(szFileFormat,".avi")==0 ||
				   strcmp(szFileFormat,".wmv")==0 ||
				   strcmp(szFileFormat,".3gp")==0 ||
				   strcmp(szFileFormat,".mov")==0 
				  // strcmp(szFileFormat,".mp4")==0
				   )
				{
					add=TRUE;
				}
				if(add)
				{
					//Find th last '\' character
					for(int j=path_len-1;j>0;j--)
					{
						if (szFilePath[j]=='\\')
						{
							slash_index=j;
							break;
						}
					}
					//add the path to the path array
					strcpy(szFileName,szFilePath +slash_index+1);
					int playIndex=SendMessage(hPlayList,LB_ADDSTRING,0,(LPARAM) szFileName);
					if(path_index==500)
						path_index=0;
					strcpy(path[path_index],szFilePath);
					SendMessage(hPlayList,LB_SETITEMDATA,(WPARAM) playIndex,(LPARAM)path[path_index++]);
						  
				}

				}	
			//Release drag resources	
			DragFinish((HDROP) wParam);
			return 0;
		}
	case WM_DESTROY:   
		{
			// Window is being destroyed
				CoUninitialize();
				PostQuitMessage(0);
				return 0;
		}
	case WM_SIZE:
		{
				InvalidateRect(hWnd,NULL,TRUE);
				return 0;
		}
	case WM_MEASUREITEM: 
		{
			//Set height of an item in listbox
			LPMEASUREITEMSTRUCT lpmis = (LPMEASUREITEMSTRUCT) lParam; 
			lpmis->itemHeight = 20; 
			return TRUE;
		}
	case WM_CTLCOLORSTATIC:
		{
			//Set background for static controls
			HDC hdcStatic = (HDC) wParam;
			SetBkMode(hdcStatic, TRANSPARENT);
			return (LRESULT) (HBRUSH) GetStockObject(GRAY_BRUSH);
		}
	case WM_DRAWITEM: 
		{
			//Get the LPDRAWITEMSTRUCT structure
			lpdis = (LPDRAWITEMSTRUCT) lParam;
			//No items?
			if (lpdis->itemID == -1)			
				break; 
 
			switch (lpdis->itemAction) 
				{ 
					case ODA_SELECT: 
					case ODA_DRAWENTIRE: 
						{
		
							//Set background and foreground colors
							//Current Selected?
							if(lpdis->itemState & ODS_SELECTED)
							{
								SetBkColor(lpdis->hDC,    RGB(0,0,0)/*RGB(204,227,245*/);
								//GetSysColor(COLOR_WINDOWTEXT)
								SetTextColor(lpdis->hDC, RGB(44,61,90));
							}
							//Item which user double clicked or pressed Enter
							//ListItem lst;
							//lst.selected=TRUE
							else if((lpdis->hwndItem==hArtistList && lpdis->itemID==selIndex.Artist) ||
									(lpdis->hwndItem==hAlbumList  && lpdis->itemID==selIndex.Album)  ||
									(lpdis->hwndItem==hSongList   && lpdis->itemID==selIndex.Song)   ||
									(lpdis->hwndItem==hPlayList   && lpdis->itemID==selIndex.Playlist))
							{
								SetBkColor(lpdis->hDC,  RGB(255,0,0) /* RGB(192,220,255)*/);
								SetTextColor(lpdis->hDC, GetSysColor(COLOR_WINDOWTEXT));
								SetTextColor(lpdis->hDC, RGB(44,61,90));
								
							}
							//For all other items
							else
							{
								SetBkColor(lpdis->hDC,  RGB(0,0,0)/* RGB(241,245,251)*/);
								//SetTextColor(lpdis->hDC, GetSysColor(COLOR_WINDOWTEXT));
								//SetTextColor(lpdis->hDC, RGB(44,61,90));
								SetTextColor(lpdis->hDC, RGB(255,0,0));
							}
							//Drawing text..
							//Get the text
							SendMessage(lpdis->hwndItem, LB_GETTEXT, 
								lpdis->itemID, (LPARAM) tchBuffer); 
							//Get current text-size
							GetTextMetrics(lpdis->hDC, &tm); 
							//Get length of text
							hr = StringCchLength(tchBuffer, BUFFER, &pcch);
							//Display text
							ExtTextOut(lpdis->hDC,
										lpdis->rcItem.left + 5, lpdis->rcItem.top + 5,
										ETO_OPAQUE | ETO_CLIPPED, &lpdis->rcItem,
										tchBuffer, lstrlen(tchBuffer), NULL);
							if(lpdis->itemState & ODS_FOCUS);
							}
							break; 
					
							case ODA_FOCUS: 
								break; 			 
					return TRUE; 

				}



	default:                      
	 // default message processing
	  return DefWindowProc(hWnd, message, wParam, lParam);
  }
}
}



// MAIN entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	//set path index to 0
	path_index=0;

	/*Initialize COM*/
	HRESULT hr=CoInitializeEx(NULL,COINIT_APARTMENTTHREADED|COINIT_DISABLE_OLE1DDE);
	if(FAILED(hr))
		exit(0);
  
	WNDCLASSEX WindowClass;						// Structure to hold our window's attributes
  

  static LPCTSTR szAppName = TEXT("MPX");     // Define window class name
  HWND hWnd;									// Window handle
  MSG msg;										// Windows message structure
#pragma region Window
  WindowClass.cbSize = sizeof(WNDCLASSEX);		// Set structure size
  WindowClass.style   = CS_HREDRAW | CS_VREDRAW;
  WindowClass.lpfnWndProc =WindowProc;
  WindowClass.cbClsExtra = 0;					// No extra bytes after the window class
  WindowClass.cbWndExtra = 0;					// structure or the window instance
  WindowClass.hInstance = hInstance;			// Application instance handle
  WindowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
  WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
  WindowClass.hbrBackground = 	(HBRUSH) GetStockObject(BLACK_BRUSH);
	  //GetStockObject(GRAY_BRUSH);
  WindowClass.lpszMenuName = 0;					// No menu
  WindowClass.lpszClassName = szAppName;		// Set class name
  WindowClass.hIconSm = 0;						// Default small icon

  // Now register our window class
  RegisterClassEx(&WindowClass);

  // Now we can create the window
  hWnd = CreateWindow(
		  szAppName,                       // the window class name
		  TEXT("Mediaplayer X 0.1"),   // The window title
		  WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,             // Window style as overlapped
		  CW_USEDEFAULT,          // Default screen position of  upper left
		  CW_USEDEFAULT,          // corner of our window as x,y...
		  1000,          // Default window size
		  650,          // .... 
		  0,                      // No parent window
		  0,                      // No menu
		  hInstance,              // Program Instance handle
		  0                       // No window creation data
		);
  //Window can accept drag and drop of files
  DragAcceptFiles(hWnd,TRUE);
  MARGINS margins = { -1, -1, -1, -1 };
 DwmExtendFrameIntoClientArea(hWnd, &margins);
/* Initialize Common Control Library*/
  INITCOMMONCONTROLSEX comm_init;
  comm_init.dwSize=sizeof(INITCOMMONCONTROLSEX);
  comm_init.dwICC=ICC_STANDARD_CLASSES;
  BOOL init=InitCommonControlsEx(&comm_init);

#pragma endregion Window
 
#pragma region ListBox
  //Initialize initial listbox position
	POINT lbsPos;
	
	lbsPos.x=20;
	lbsPos.y=70;

	/*album list*/
  hAlbumList = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTBOX, NULL,
				   LBS_OWNERDRAWFIXED    | WS_VSCROLL | WS_HSCROLL |WS_CHILD | WS_TABSTOP | WS_VISIBLE|LBS_NOTIFY|LBS_HASSTRINGS,
					lbsPos.x, lbsPos.y,200, 400, hWnd, (HMENU) LIST_ID, GetModuleHandle(NULL), NULL);
	lbsPos.x+=200;;
	
	/*artist list*/
  hArtistList = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTBOX, NULL,
				   LBS_OWNERDRAWFIXED    | WS_VSCROLL | WS_HSCROLL |WS_CHILD | WS_TABSTOP | WS_VISIBLE|LBS_NOTIFY|LBS_HASSTRINGS,
					lbsPos.x, lbsPos.y,200, 400, hWnd, (HMENU) LIST_ID, GetModuleHandle(NULL), NULL);
	lbsPos.x+=200;
	
	/*song list*/
  hSongList = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTBOX, NULL,
				   LBS_OWNERDRAWFIXED    | WS_VSCROLL | WS_HSCROLL |WS_CHILD | WS_TABSTOP | WS_VISIBLE|LBS_NOTIFY|LBS_HASSTRINGS,
					lbsPos.x, lbsPos.y,250, 400, hWnd, (HMENU) LIST_ID, GetModuleHandle(NULL), NULL);
	lbsPos.x+=300;
	
	/*play list*/
  hPlayList = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTBOX, NULL,
				   LBS_OWNERDRAWFIXED  |WS_VSCROLL   | WS_HSCROLL |WS_CHILD | WS_TABSTOP | WS_VISIBLE|LBS_NOTIFY|LBS_HASSTRINGS,
					lbsPos.x, lbsPos.y,250, 400, hWnd, (HMENU) LIST_ID, GetModuleHandle(NULL), NULL);

  //Create progress bar

   hProgressBar=CreateWindowEx(NULL,PROGRESS_CLASS,NULL,WS_CHILD & ~WS_VISIBLE,580,25,150,30,hWnd,NULL,GetModuleHandle(NULL),NULL);

   //Add to Library Button

   hBtnAdd=CreateWindowEx(NULL, WC_BUTTON, "Add to library",
				   WS_CHILD | WS_TABSTOP | WS_VISIBLE,
					450, 25,120, 30, hWnd, (HMENU) LIST_ID, GetModuleHandle(NULL), NULL);
   //Play Button

   
  hBtnPlay=CreateWindowEx(NULL, WC_BUTTON, "Play",
				   WS_CHILD | WS_TABSTOP | WS_VISIBLE,
					350, 500,50, 30, hWnd, (HMENU) LIST_ID, GetModuleHandle(NULL), NULL);
  //Prev Button

  hBtnPrev=CreateWindowEx(NULL, WC_BUTTON, "Prev",
				   WS_CHILD | WS_TABSTOP | WS_VISIBLE,
					250, 500,50, 30, hWnd, (HMENU) LIST_ID, GetModuleHandle(NULL), NULL);
  //Next Button

  hBtnNext=CreateWindowEx(NULL, WC_BUTTON, "Next",
				   WS_CHILD | WS_TABSTOP | WS_VISIBLE,
					450, 500,50, 30, hWnd, (HMENU) LIST_ID, GetModuleHandle(NULL), NULL);



 //Create the searchbox
   CreateSearchBox(hWnd);

//Set inital select items in listboxes to -1
  selIndex.Artist=-1;
  selIndex.Album=-1;
  selIndex.Song=-1;
  selIndex.Playlist=-1;

  //Get the original listbox WNDPROC
 origListProc=(WNDPROC) GetWindowLong(hPlayList,GWL_WNDPROC);
 //Set to our custom WNDPROC
 SetWindowLong(hPlayList,GWL_WNDPROC,(long) ListProc);
 SetWindowLong(hArtistList,GWL_WNDPROC,(long) ListProc);
 SetWindowLong(hSongList,GWL_WNDPROC,(long) ListProc);
 SetWindowLong(hAlbumList,GWL_WNDPROC,(long) ListProc);

 //To set horizontal scrollbar
 SendMessage(hArtistList,LB_SETHORIZONTALEXTENT,(WPARAM)250,NULL);
 SendMessage(hAlbumList,LB_SETHORIZONTALEXTENT,(WPARAM)250,NULL);
 SendMessage(hSongList,LB_SETHORIZONTALEXTENT,(WPARAM)650,NULL);
  SendMessage(hPlayList,LB_SETHORIZONTALEXTENT,(WPARAM)800,NULL);

#pragma endregion ListBox
  
  ShowWindow(hWnd, nCmdShow);         // Display the window
  UpdateWindow(hWnd);                 // Cause window client area to be drawn
  ShowWindow(hPlayList, nCmdShow);     // Display the window
  UpdateWindow(hPlayList);             // Cause window client area to be drawn

  //Set default list values
  strcpy(ListItems.ALBUM,"");
  strcpy(ListItems.ARTIST,"");
  strcpy(ListItems.SONG,"");

  // The message loop
  while(GetMessage(&msg, 0, 0, 0) == TRUE)  // Get any messages
  {
	TranslateMessage(&msg);                 // Translate the message
	DispatchMessage(&msg);                  // Dispatch the message
  }

 

  return static_cast<int>(msg.wParam);      // End, so return to Windows
}

 

