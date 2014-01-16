#pragma once
#include <windows.h>
#include <CommCtrl.h>

typedef enum
{
	DeskIconSmall,
	DeskIconBig
} DeskIconSize;

class CDeskIcon
{
public:
    CDeskIcon();
    ~CDeskIcon();

    BOOL SetDeskIcon(DeskIconSize size);

private:
    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

    BOOL IsVistaOrLater();
    BOOL SetDeskIcon(HWND hWnd, DeskIconSize size);

};