#pragma once
//Linking to debug versions of the libraries
#pragma comment(lib,"tagd.lib")
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avformat.lib")

/*For static linkage*/
#define ID3LIB_LINKOPTION 1
#define TAGLIB_STATIC 1

/*menu constants*/
#define ID_PLACES								2005
#define IDR_PLACES                              103
#define IDM_PLAYER                              40000
#define IDM_LIBRARY                             40001


/*Include files*/
#include <string>
#include <vector>
#include <stack>
#include<process.h>
#include<Windows.h>			//Windows header
#include<CommCtrl.h>		//Common control library
#include<WinUser.h>			//Deals with helpful codes
#include<shobjidl.h>		//Dialog header file
#include<tchar.h>
#include<strsafe.h>			//for safe versions, strcb*
#include<dwmapi.h>			//transparency,not used atm

#include "CppSQLite3.h"		//for integration to sqlite
#include "funcs.h"			//Libav and SDL functions
#include "include\sqlite3.h"							//SQLite header
#include "include\taglib-1.7\taglib\tag.h"				//Taglib header
#include "include\taglib-1.7\taglib\fileref.h"
extern "C"{
#include "include\ffmpeg\include\libavformat\avformat.h" //Libav header
}
/*Function Forward Declarations*/
	void wait_thread(BOOL play);
	unsigned int WINAPI avThread(PVOID filename);
	void SelChangePlayList(void);
	char* GetPath(const char* title);
	void SelChange(HWND hwnd,int index);
	//bool ListFiles(string path, string mask, vector<string>& files);
	void ClearLists()  ;
	BOOL SortLists(int SORT) ;
	BOOL AddToLibrary()	  ;
	void SelChangeListBox(HWND hWnd,WPARAM wParam)	;
	LRESULT WINAPI EditProc(HWND hWnd, UINT message, 
						  WPARAM wParam, LPARAM lParam)	 ;
	LRESULT WINAPI GrpProc(HWND hWnd, UINT message, 
						  WPARAM wParam, LPARAM lParam)	;
	LRESULT WINAPI ListProc(HWND hWnd, UINT message, 
						  WPARAM wParam, LPARAM lParam)	;
	BOOL CreateSearchBox(HWND hWnd);
/*Structures*/
//For playlist item
typedef struct Item
{
	BOOL selected;
	char* szPath;

}ListItem;
//Enum for listbox sorting
enum{
	SORT_ALL,
	SORT_ARTIST,
	SORT_ALBUM
};
//Current selection of listboxes
typedef struct sel_index{
  int Artist;
  int Album;
  int Song;
  int Playlist;
}sel_index;
//Current text of selected items in listboxes
typedef struct sel_listItems{
	char ARTIST[1000];
	char ALBUM[1000];
	char SONG[1000];
};

/*Global Variables*/
BOOL pause=FALSE;			//pause FLAG
char cur_play[1000];		//current file path for playback
char path[500][1000];		//to hold the paths of items in playlist
int path_index;				//current index to path
unsigned int th_playID;		//thread ID
HANDLE th_play;				//thread handle
sel_listItems ListItems;	
sel_index selIndex;

//Window procedures
WNDPROC 
	origListProc,
	origGrpProc,
	origEditProc;

//Handles
HWND 
	hPlayList,		//Playlist handle
	hAlbumList,		//Album viewer
	hArtistList,	//Artist viewer
	hSongList,		//Song viewer
	hBtnAdd,		//Button add
	hCurFile,
	hProgressBar ,  //Progressbar handle
	hEdit,         //Edit Handle
	hBtnRArtist,   //Artist checkbox
	hBtnRAlbum,     //Album checkbox
	hBtnRSong,		//Song checkbox
	hGrpBox,		//Searchbox
	hBtnPlay,		//Play Button
	hBtnNext,		//Next Button
	hBtnPrev,		//Prev Button
	hSeekBar		//Seekbar - to be implemented
	;


using namespace std;

//Experimental thread locking
void wait_thread(BOOL play)
{
	if(play)
	{
		SDL_LockAudio();
		SDL_CondWait(global_video_state->pictq_cond,global_video_state->pictq_mutex);
	}
	else
	{
		SDL_CondBroadcast(global_video_state->pictq_cond);
		SDL_mutexV(global_video_state->pictq_mutex);
	}
	

}

//Thread entry procedure to play media files
unsigned int WINAPI avThread(PVOID filename)
{
 SDL_Event       event;

  VideoState      *is;

  is = (VideoState *)av_mallocz(sizeof(VideoState));


  // Register all formats and codecs
  av_register_all();
  
  //Initialize SDL
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
	fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
	exit(1);
  }

  // Make a screen to put our video
	screen = SDL_SetVideoMode(640, 480, 24, 0);

  if(!screen) {
	fprintf(stderr, "SDL: could not set video mode - exiting\n");
	exit(1);
  }

  //Copy the file name from the argument 
  char* fl=(char *) filename;
  av_strlcpy(is->filename,fl,sizeof(is->filename));

  //Create mutex and conditional variables
  is->pictq_mutex = SDL_CreateMutex();
  is->pictq_cond = SDL_CreateCond();

  schedule_refresh(is, 40);

  //not sure if required..
  //is->av_sync_type = DEFAULT_AV_SYNC_TYPE;

  //Create SDL thread
  is->parse_tid = SDL_CreateThread(decode_thread, is);
  if(!is->parse_tid) {
	av_free(is);
	return -1;
  }
  //Main event loop
  for(;;) {

	SDL_WaitEvent(&event);
	switch(event.type) {
	case FF_QUIT_EVENT:
	case SDL_QUIT:
	  is->quit = 1;
	  SDL_Quit();
	  _endthreadex(0);
	  break;
	case FF_ALLOC_EVENT:
	  alloc_picture(event.user.data1);
	  break;
	case FF_REFRESH_EVENT:
	  video_refresh_timer(event.user.data1);
	  break;
	default:
	  break;
	}
  }
  return 0;;

}
 void SelChangePlayList(void)
 {
				  {
						//Get current selection
						 int cur_sel=SendMessage(hPlayList,LB_GETCURSEL,(WPARAM)0,(LPARAM)0);
						 if( cur_sel != LB_ERR)
						 {
							 selIndex.Playlist=cur_sel;
							 //MessageBox(hWnd,(LPCSTR)SendMessage(hPlayList,LB_GETITEMDATA,(WPARAM) SendMessage(hLst,LB_GETCURSEL,(WPARAM)0,(LPARAM)0),0),"Path",MB_OK);
							 //LPSTR path=NULL;
							 char path[1000];
							 
								strcpy(path,(LPCSTR)SendMessage(hPlayList,LB_GETITEMDATA,(WPARAM) cur_sel  ,0));
								if(strlen(path)>0)
								{
									BOOL try_terminate=TRUE;
									strcpy(cur_play,path);
									DWORD exitCode;
									if(th_play!=NULL)
										//Get the exit code of the SDL thread
									while(GetExitCodeThread(th_play,&exitCode))
									{
										//still active?
										if(exitCode==STILL_ACTIVE && try_terminate==TRUE)
										{

											//Set the cond variable to 1 and change text
											global_video_state->quit=1;
											SetWindowText(hBtnPlay,"Play");

										}
										if (exitCode!=STILL_ACTIVE)
										{
											//Try to create a new thread
											 th_play=(HANDLE)_beginthreadex(NULL,NULL,avThread,&cur_play,NULL,&th_playID);
											 SetWindowText(hBtnPlay,"Pause");
											 break;
										}
									}
									else
									{
										//Create a new thread
										th_play=(HANDLE)_beginthreadex(NULL,NULL,avThread,&cur_play,NULL,&th_playID);
										SetWindowText(hBtnPlay,"Pause");
									}

								}
						 }
	
								 }
 }
//Get file path from database
char* GetPath(const char* title)
{
	CppSQLite3DB SQLdb;
	char szQuery[1200];
	char sqResult[1000];
	sprintf_s(szQuery,(size_t) 1000,"select * from music where TITLE like \'%s%%\'",title);
	try
	{
		SQLdb.open("music.db");
		CppSQLite3Query sqQuery=SQLdb.execQuery(szQuery);
		if(!sqQuery.eof())
			strcpy(sqResult,sqQuery.fieldValue(3));
		else
			strcpy(sqResult,"");
		sqQuery.finalize();
		SQLdb.close();
	}
	catch(CppSQLite3Exception excp)
	{
		MessageBox(hPlayList,excp.errorMessage(),":)",MB_OK);;
	}
	return sqResult;

}
//On selection change(listbox) set the selected item  global variable
void SelChange(HWND hwnd,int index)
{
	/*Get the text from item and copy to global variable*/
	char value[500];
	if(hwnd==hArtistList)
	{
		 selIndex.Artist=index;
		 if(SendMessage(hArtistList,LB_GETTEXT,(WPARAM) index,(LPARAM)value)!=LB_ERR)
			strcpy(ListItems.ARTIST,value);
	}
	else if(hwnd==hAlbumList)
	{
		selIndex.Album=index;
		if(SendMessage(hAlbumList,LB_GETTEXT,(WPARAM) index,(LPARAM)value)!=LB_ERR)
			strcpy(ListItems.ALBUM,value);
	}
	else if(hwnd==hSongList)
	{
		selIndex.Song=index;
		if(SendMessage(hSongList,LB_GETTEXT,(WPARAM) index,(LPARAM)value)!=LB_ERR)
			strcpy(ListItems.SONG,value);
	}
						  
}

//Get all the files from the folder and push it to the stack
bool ListFiles(string path, string mask, vector<string>& files)
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA fdata;
	string fullpath;
	stack<string> folders;
	folders.push(path);
	files.clear();
	
	while (!folders.empty())
	{
		path = folders.top();
		fullpath = path + "\\" + mask;
		folders.pop();

		hFind = FindFirstFile(fullpath.c_str(), &fdata);
		//Invalid handle?
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				// . & .. paths?
				if (strcmp(fdata.cFileName, ".") != 0 &&
					strcmp(fdata.cFileName, "..") != 0)
				{
					/*push onto stack*/
					if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						folders.push(path + "\\" + fdata.cFileName);
					}
					else
					{
						files.push_back(path + "\\" + fdata.cFileName);
					}
				}
			}
			while (FindNextFile(hFind, &fdata) != 0);
		}
		/*Close file handle*/
		if (GetLastError() != ERROR_NO_MORE_FILES)
		{
			FindClose(hFind);

			return false;
		}

		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
	}

	return true;
}
//Clear Listboxes
void ClearLists()
{
	SendMessage(hAlbumList,LB_RESETCONTENT,0,0);
	SendMessage(hArtistList,LB_RESETCONTENT,0,0);
	SendMessage(hSongList,LB_RESETCONTENT,0,0);
}
//Procedure to sort the llistboxes
BOOL SortLists(int SORT)
{

	char szQuery[1200];
	CppSQLite3Statement stmt;
	char szText[1000];
	char szSelectField[15];
	CppSQLite3DB SQLdb;
	SQLdb.open("music.db");
	char szField[30];
	//Sort everything
	if(SORT==SORT_ALL)
	{
		ClearLists();
	
	/*which radio button is checked?*/
	if(SendMessage(hBtnRArtist,BM_GETCHECK,0,0)==BST_CHECKED)
		strcpy(szField,"ARTIST");
	else if(SendMessage(hBtnRAlbum,BM_GETCHECK,0,0)==BST_CHECKED)
		strcpy(szField,"ALBUM");
	else if(SendMessage(hBtnRSong,BM_GETCHECK,0,0)==BST_CHECKED)
		strcpy(szField,"TITLE");
	else 
		return FALSE;

		GetWindowText(hEdit,szText,1000);
		//Query string
		sprintf_s(szQuery,(size_t) 1000,"select * from music where %s like \'%s%%\'",szField,szText);
		//stmt=SQLdb.compileStatement("select * from music where ? like \'?%%\';");
		char str[1000];
		//sprintf(str,"\'%s%%\'",szText);
		//stmt.bind(1,szField);
//		stmt.bind(2,str);
		//stmt.bind(2,szText);
		//stmt.
	}
	//Message from artist listbox?
	else if(SORT==SORT_ARTIST)
	{
		SendMessage(hArtistList,LB_RESETCONTENT,0,0);
		SendMessage(hSongList,LB_RESETCONTENT,0,0);
		if(strcmp(ListItems.ALBUM,"")==0)
		{
			//stmt=SQLdb.compileStatement("select * from music where ARTIST=?;");
			//stmt.bind(1,ListItems.ARTIST);
			//strcpy(szQuery,	"select * from music where ARTIST=?");
			sprintf_s(szQuery,(size_t) 1000,"select * from music where ARTIST=\'%s\';",ListItems.ARTIST);
		}
		else
		{
			//stmt=SQLdb.compileStatement("select * from music where ALBUM=? and ARTIST=?;");
			//stmt.bind(1,ListItems.ALBUM);
			//stmt.bind(2,ListItems.ARTIST);
			sprintf_s(szQuery,(size_t) 1000,"select * from music where ALBUM=\'%s\' and ARTIST=\'%s\';",ListItems.ALBUM,ListItems.ARTIST);
		}
	}
	//or album?
	else if(SORT==SORT_ALBUM)
	{
			//stmt=SQLdb.compileStatement("select * from music where ALBUM like ?");
			 //stmt.bind(1,ListItems.ALBUM);
			sprintf_s(szQuery,(size_t) 1000,"select * from music where ALBUM like \'%s%%\';",ListItems.ALBUM);
			SendMessage(hArtistList,LB_RESETCONTENT,0,0);
			SendMessage(hSongList,LB_RESETCONTENT,0,0);
	}
	else  {
		SQLdb.close();
		return FALSE;
	}
		
	try
			{
				
				CppSQLite3Query sqQuery= SQLdb.execQuery(szQuery);
					//stmt.execQuery();
				//stmt.reset();
					//SQLdb.execQuery(szQuery);
				//stmt.execQuery();
				while(!sqQuery.eof())
				{
					//Add text to the listboxes
					SendMessage(hArtistList,LB_ADDSTRING,0,(LPARAM) sqQuery.fieldValue(0));
					SendMessage(hAlbumList,LB_ADDSTRING,0,(LPARAM) sqQuery.fieldValue(1));
					SendMessage(hSongList,LB_ADDSTRING,0,(LPARAM) sqQuery.fieldValue(2));
					sqQuery.nextRow();
				}
				sqQuery.finalize();
				stmt.finalize();
				SQLdb.close();


			}
			//Exception?
			catch(CppSQLite3Exception excp)
			{
				MessageBox(hPlayList,excp.errorMessage(),":)",MB_OK);;
			}
	
		SQLdb.close();
			
	return TRUE;
}


//Proedure to add files to database
BOOL AddToLibrary()
{
	/* Get the folder*/
	/*COM component calls using IFileDialog*/				
					wchar_t * wfolder_path=NULL;
					 IFileDialog *pfd;
						if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
						{
							DWORD dwOptions;
							if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
							{
								pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
							}
							if (SUCCEEDED(pfd->Show(NULL)))
							{
								IShellItem *psi;
								if (SUCCEEDED(pfd->GetResult(&psi)))
								{
									
									if(SUCCEEDED(psi->GetDisplayName(SIGDN_FILESYSPATH,&wfolder_path )))
										
									{

										MessageBoxW(NULL, wfolder_path, L"Selected Folder", NULL);
									}
									psi->Release();
								}
							}
							pfd->Release();
						}
						/* If no folder is selected, break!!*/
						if(wfolder_path==NULL)
							return FALSE;
						else
						{
						try{
								
								CppSQLite3DB SQLdb;
								SQLdb.open("music.db");
								//Query for insertion
								CppSQLite3Statement stmt=SQLdb.compileStatement("insert into music values(?,?,?,?);");
								//Query to find out if already present in database
								CppSQLite3Statement stmt_query=SQLdb.compileStatement("select * from music where ARTIST=? AND ALBUM=? AND TITLE=?;");
							
								char query[500];
								if (SQLdb.tableExists("music"))
								{
									/* intialise STL vector,sqlite and taglib resources*/
									vector<string> files;
									char folder_path[1000];
									const char * flPath=NULL;
									const char * fieldData=NULL;
									TagLib::FileRef fRef;
									TagLib::String field;
									size_t mb_size=(size_t) (wcslen(wfolder_path)+1);
									wcstombs(folder_path,wfolder_path, mb_size);

									char szAlbum[100];
									char szArtist[100];
									char szSong[100];
									
									
									//Any files in the folder?
									if (!ListFiles(folder_path, "*", files))
									return FALSE;
									
									//Set progressbar range
									int count=files.size();
									ShowWindow(hProgressBar,SW_SHOW);
									SendMessage(hProgressBar,PBM_SETRANGE,(WPARAM)0,MAKELPARAM(0,count));
									SendMessage(hProgressBar,PBM_SETSTEP,(WPARAM) 1,0);
									//SendMessage(hProgressBar,PBM_SETMARQUEE,(WPARAM) 1,NULL);
									int cError=0;
									//Iterate through each file in vector
									for (vector<string>::iterator iter = files.begin(); iter != files.end(); ++iter)
									{
									
										
										flPath=iter->c_str();
										TagLib::FileRef fRef(iter->c_str());
										//Can taglib read the tag?
										if(fRef.isNull())
										{
											cError++;
											SendMessage(hProgressBar,PBM_SETRANGE,(WPARAM)0,MAKELPARAM(0,count-cError));
											continue;
										}
										else
										{
											/*note: only ASCII tags are supported now, TODO:change to wchar*/
											/*Go through each tag of ARTIST,ALBUM,TITLE and if not present, use Unknown*/
											 field=fRef.tag()->artist();
											 if(field.isAscii())
											 {
												 
												 if(strcmp(field.toCString(),"")==0)
													 strcpy(szArtist,"Unknown");
												 else
													 strcpy(szArtist,field.toCString());
											 }
											 else
												 continue;
											 
											 field=fRef.tag()->album();
											 if(field.isAscii())
											 {
											 
												if(strcmp(field.toCString(),"")==0)
													 strcpy(szAlbum,"Unknown");
												 else
													 strcpy(szAlbum,field.toCString());
											 }
											 else
												 continue;
											 
											 field=fRef.tag()->title();
											 if(field.isAscii())
											 {
												 if(strcmp(field.toCString(),"")==0)
													 strcpy(szSong,"Unknown");
												 else
													 strcpy(szSong,field.toCString());

											 }
											 else
												 continue;

											 //Bind the values to query
											 stmt_query.bind(1,szArtist);
											 stmt_query.bind(2,szAlbum);
											 stmt_query.bind(3,szSong);
											 
											 CppSQLite3Query result= stmt_query.execQuery();
											 
											 stmt_query.reset();
											 //No record exists, if so insert data
											 if(result.eof()){
											 //Bind values to query
											 stmt.bind(1,szArtist);
											 stmt.bind(2,szAlbum);
											 stmt.bind(3,szSong);
											 stmt.bind(4,iter->c_str());
											 //Execute query
											 stmt.execDML();
											 stmt.reset();
											 }

											 //Update progressbar
											 SendMessage(hProgressBar,PBM_STEPIT,0,0);

											
										   }
										
									}
									stmt.finalize();
									stmt_query.finalize();
									//SendMessage(hProgressBar,PBM_SETMARQUEE,(WPARAM) 0,NULL);
									SendMessage(hProgressBar,PBM_SETPOS,(WPARAM)SendMessage(hProgressBar,PBM_GETRANGE,(WPARAM) FALSE,NULL),NULL);
								}
								
								else
									return FALSE;

								SQLdb.close();
								ShowWindow(hProgressBar,SW_HIDE);
								}
								//Error handling
								catch(CppSQLite3Exception excp)
								{
									MessageBox(hPlayList,excp.errorMessage(),":)",MB_OK);;
								}
							
								return TRUE;
						}
				}
   void SelChangeListBox(HWND hWnd,WPARAM wParam)
	{
					  //ID3_Tag 
					  //Create hwnd for listbox
					  HWND hLst=hWnd;	
					  //Get current selected index
					  int index=SendMessage(hLst,LB_GETCURSEL,(WPARAM)0,(LPARAM)0);	
					  //Find the length of text
					  int textLen = (int) SendMessage(hLst, LB_GETTEXTLEN, (WPARAM) index, 0);	
					  //Create Local Buffer
					  TCHAR * textBuffer = new TCHAR[textLen + 1];			
					  //Get the text
					  int a=SendMessage(hLst,LB_GETTEXT,(WPARAM)index,(LPARAM)textBuffer);		
					  //MessageBox(hLst,textBuffer,"...",MB_OK);
					  //Get item data
					  a=SendMessage(hLst,LB_GETITEMDATA,(WPARAM)index,0);	
					  //Get count of items
					  int maxItems=SendMessage(hLst,LB_GETCOUNT,(WPARAM)0,(LPARAM)0);			
						
					  //Return focus to listbox
						SetFocus(hLst);		
						InvalidateRect(hWnd,NULL,FALSE);
						SelChange(hLst,index);
						//Sort Listboxes based on handle
						if(hLst==hAlbumList)
						{
							SortLists(SORT_ALBUM);
							SetFocus(hArtistList);
						}
						else if(hLst==hArtistList)
						{
							SortLists(SORT_ARTIST);
							SetFocus(hSongList);
						}
						else if(hLst==hSongList)
					  {
						  char str[1000];
						  int playIndex;
						  //Get the text of current selected item
						  SendMessage(hSongList,LB_GETTEXT,index,(LPARAM) str);
						  playIndex=SendMessage(hPlayList,LB_ADDSTRING,0,(LPARAM) str);
							//Is the path array filled?
						  if(path_index==500)
							  path_index=0;
						  strcpy(path[path_index],GetPath(str));
						  //Set the path as ITEMDATA
						  SendMessage(hPlayList,LB_SETITEMDATA,(WPARAM) playIndex,(LPARAM)path[path_index++]);
						  
					  }
						else if(hLst==hPlayList)
					 {
						 if(HIWORD(wParam)==LBN_DBLCLK || wParam==VK_RETURN)
									 SelChangePlayList();
						 SetFocus(hPlayList);
						
					 }


						
					}
//Custom WNDPROC for edit control

LRESULT WINAPI EditProc(HWND hWnd, UINT message, 
						  WPARAM wParam, LPARAM lParam)
{
	//For insta search functionality
	switch(message)
	{

		case WM_KEYDOWN:
			{
				switch(wParam)
				{
					case VK_RETURN:
						{
							
							SortLists(SORT_ALL);
							break;
						}
				
				}
				break;
			}
		
	
	}
	//call original WNDPROC
	return CallWindowProc(origEditProc,hWnd,message,wParam,lParam);;
}

//Custom WNDPROC for groupbox
LRESULT WINAPI GrpProc(HWND hWnd, UINT message, 
						  WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_CTLCOLORSTATIC:
		{
			HDC hdcStatic = (HDC) wParam;
			SetBkMode(hdcStatic, OPAQUE);
			SetTextColor(hdcStatic,RGB(255,0,0));
			return (LRESULT) (HBRUSH) GetStockObject(GRAY_BRUSH);
		}
		case WM_CTLCOLOREDIT:
			{
				SetTextColor((HDC) wParam,RGB(255,0,0));
				break;
			}
		/*case WM_NOTIFY:
			{
				switch(((NMHDR *)lParam)->code)
				{
				case NM_CUSTOMDRAW:
					{
						
						HDC hd=GetDC(((NMHDR *)lParam)->hwndFrom);
						SetTextColor(hd,RGB(255,0,0));
						ReleaseDC(	((NMHDR *)lParam)->hwndFrom,hd);
						SetWindowTheme(((NMHDR *)lParam)->hwndFrom,L"",NULL);
						return CDRF_DODEFAULT;
					}
				}
			}*/
		case WM_COMMAND:
		{
			//Redundant, TODO:remove or modify
			if(HIWORD(wParam)==EN_CHANGE)
			{
				SortLists(SORT_ALL);
				break;
			}
			break;
		}

		default:
			CallWindowProc(origGrpProc,hWnd,message,wParam,lParam);
	}
	return CallWindowProc(origGrpProc,hWnd,message,wParam,lParam);;
}

//Custom WNDPROC for listboxes
LRESULT WINAPI ListProc(HWND hWnd, UINT message, 
						  WPARAM wParam, LPARAM lParam)
{
  HDC hDC;                       // Display context handle
  PAINTSTRUCT PaintSt;           // Structure defining area to be drawn
  RECT aRect;                    // A working rectangle

  switch(message)                // Process selected messages
  {
 
	  case WM_NCPAINT:
		  {
			  break;
		  }
		//Keypress event
		 case WM_KEYDOWN:
	{
		  switch(wParam)
		  {
			  //Enter key?
			case VK_RETURN:
				SelChangeListBox(hWnd,wParam);
				break;

			case VK_DELETE:
				{
					if(hWnd==hPlayList)
					{
					int cur_focusIndex=SendMessage(hWnd,LB_GETCURSEL,0,0);
					SendMessage(hWnd,LB_DELETESTRING,(WPARAM)cur_focusIndex,0);
					return 0;
					}
					else 
						break;
				}
			default:
				break;
				  

		  }
		  break;
	  }

		 case WM_KILLFOCUS:
			 {
				 SendMessage(hWnd,LB_SETCURSEL,(WPARAM)-1,0);
				 InvalidateRect(hWnd,NULL,TRUE);
				 return 0;
			 }
		 case LB_ADDSTRING:
			 {
				 //Don't duplicate, entries
				 if(SendMessage(hWnd,LB_FINDSTRINGEXACT,(WPARAM) -1,lParam)==LB_ERR)
					 break;
				 else
					 return 0;
			 }

	 
		case WM_DESTROY:              // Window is being destroyed
		  PostQuitMessage(0);
		  return 0;
								  
  }	
  // default message processing
  return CallWindowProc(origListProc,hWnd,message,wParam,lParam);
}


//Procedure to create the searchbox
BOOL CreateSearchBox(HWND hWnd)
{
  HWND hGrpBox=CreateWindowEx(NULL, WC_BUTTON, "Search",
				   WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_LEFT|BS_GROUPBOX,
					20, 10,400, 50, hWnd,NULL, GetModuleHandle(NULL), NULL);
   hEdit=CreateWindowEx(NULL, WC_EDIT, NULL,
				   WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_LEFT,
					10, 20,120, 20, hGrpBox,NULL, GetModuleHandle(NULL), NULL);
   hBtnRArtist=CreateWindowEx(NULL, WC_BUTTON , "Artist",
				   WS_CHILD | WS_TABSTOP | WS_VISIBLE|BS_AUTORADIOBUTTON  ,
				   140, 15,70, 30, hGrpBox,NULL, GetModuleHandle(NULL), NULL);
   hBtnRAlbum=CreateWindowEx(NULL, WC_BUTTON , "Album",
				   WS_CHILD | WS_TABSTOP | WS_VISIBLE|BS_AUTORADIOBUTTON  ,
					220, 15,70, 30, hGrpBox,NULL, GetModuleHandle(NULL), NULL);
   hBtnRSong=CreateWindowEx(NULL, WC_BUTTON , "Song",
				   WS_CHILD | WS_TABSTOP | WS_VISIBLE|BS_AUTORADIOBUTTON  ,
					300, 15,70, 30, hGrpBox, NULL, GetModuleHandle(NULL), NULL);

   //Set new WNDPROC's
	origGrpProc=(WNDPROC) GetWindowLong(hGrpBox,GWL_WNDPROC);
   SetWindowLong(hGrpBox,GWL_WNDPROC,(long) GrpProc);
   origEditProc=(WNDPROC) GetWindowLong(hEdit,GWL_WNDPROC);
   SetWindowLong(hEdit,GWL_WNDPROC,(long) EditProc);

   if(hGrpBox && hEdit && hBtnRArtist && hBtnRAlbum && hBtnRSong)
	   return TRUE;
   else
	   return FALSE;
}

