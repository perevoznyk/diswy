// Diswy.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Diswy.h"
#include "DeskIcon.h"

#define CMD_SMALL  _T("/small")
#define CMD_BIG    _T("/big")

void ChangeVisibility()
{
  HWND H = FindWindowEx(0, 0, L"Progman", NULL);
  if (IsWindow(H)) 
  {
      if (IsWindowVisible(H)) 
        ShowWindow(H, SW_HIDE);
          else
            ShowWindow(H, SW_SHOW);
  }
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	CDeskIcon deskIcon;

    if( !_tcsicmp( lpCmdLine, CMD_SMALL ) )
    {
        deskIcon.SetDeskIcon( DeskIconSmall );
    }
    else if( !_tcsicmp( lpCmdLine, CMD_BIG ) )
    {
        deskIcon.SetDeskIcon( DeskIconBig ); 
    }
    else
    {
        ChangeVisibility();
    }
    
	return 0;

}

