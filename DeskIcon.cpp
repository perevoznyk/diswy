#include <stdafx.h>
#include "DeskIcon.h"

CDeskIcon::CDeskIcon()
{
}

CDeskIcon::~CDeskIcon()
{
}

BOOL CALLBACK CDeskIcon::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	HWND hChild = FindWindowEx( hwnd, NULL, _T("SHELLDLL_DefView"), NULL );
	if( hChild )
	{
		HWND hDesk = FindWindowEx( hChild, NULL, _T("SysListView32"), NULL );
		if( hDesk )
		{
			*(reinterpret_cast<HWND*>(lParam)) = hDesk;
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CDeskIcon::SetDeskIcon(DeskIconSize size)
{
	HWND hWnd = NULL;
	
	EnumWindows( EnumWindowsProc, (LPARAM)&hWnd );
	if( hWnd )
	{
		return SetDeskIcon( hWnd, size );
	}

	return FALSE;
}

BOOL CDeskIcon::IsVistaOrLater() 
{
   OSVERSIONINFOEX osvi;
   DWORDLONG dwlConditionMask = 0;
   int op=VER_GREATER_EQUAL;

   // Initialize the OSVERSIONINFOEX structure.
   ZeroMemory( &osvi, sizeof(OSVERSIONINFOEX) );
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
   osvi.dwMajorVersion = 6;
   osvi.dwMinorVersion = 0;
   osvi.wServicePackMajor = 0;
   osvi.wServicePackMinor = 0;

   // Initialize the condition mask.
   dwlConditionMask = VerSetConditionMask( dwlConditionMask, VER_MAJORVERSION, op );
   dwlConditionMask = VerSetConditionMask( dwlConditionMask, VER_MINORVERSION, op );
   dwlConditionMask = VerSetConditionMask( dwlConditionMask, VER_SERVICEPACKMAJOR, op );
   dwlConditionMask = VerSetConditionMask( dwlConditionMask, VER_SERVICEPACKMINOR, op );

   // Perform the test.
   return VerifyVersionInfo( &osvi, 
	  VER_MAJORVERSION | VER_MINORVERSION | 
	  VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR,
	  dwlConditionMask );	   
}

BOOL CDeskIcon::SetDeskIcon(HWND hWnd, DeskIconSize size)
{
	STYLESTRUCT Styles;

	if( IsVistaOrLater() )
	{
        // Resize the icons to smallest
		for( int i=0; i<100; i++ )
            SendMessage( hWnd, WM_MOUSEWHEEL, MAKEWPARAM(MK_CONTROL, -WHEEL_DELTA), MAKELPARAM(0, 0) );

        // Resize the icons to normal size
		if( size == DeskIconBig )
		{
			for( int i=0; i<6; i++ )
				SendMessage( hWnd, WM_MOUSEWHEEL, MAKEWPARAM(MK_CONTROL, WHEEL_DELTA), MAKELPARAM(0, 0) );
		}		
	}

    // Modify the style
	Styles.styleOld = GetWindowLong( hWnd, GWL_STYLE );
	Styles.styleNew = Styles.styleOld & (~LVS_TYPEMASK);	
	Styles.styleOld = Styles.styleNew;

	if ( size == DeskIconSmall ) 
	{
		Styles.styleNew |= LVS_SMALLICON;
		Styles.styleOld |= LVS_ICON;		
	}
	else
	{
		Styles.styleNew |= LVS_ICON;
		Styles.styleOld |= LVS_SMALLICON;
	}

    // Set the new style
	SetWindowLong( hWnd, GWL_STYLE, Styles.styleNew );
	SendMessage( hWnd, WM_STYLECHANGED, GWL_STYLE, (LONG)(&Styles) );
	SendMessage( hWnd, WM_KILLFOCUS, 0, 0 );
	
    // Refresh control
    UpdateWindow( hWnd );
	ShowWindow( hWnd, SW_HIDE );
	ShowWindow( hWnd, SW_SHOWNORMAL );

	return TRUE;
}
