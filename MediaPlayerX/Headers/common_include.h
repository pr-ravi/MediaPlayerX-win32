#pragma once
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <windows.h>
#include<strsafe.h>
#include <WinDef.h>
#include <vector>
#include <d2d1.h>
#include <d2d1helper.h>
#include <wincodec.h>
#include <CommCtrl.h>


#define PLAY_POS_X 400;
#define PLAY_POS_Y 50;


template<class Interface>
inline void SafeRelease(
	Interface **ppInterfaceToRelease
	)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}




