#ifndef f_VIDEOWINDOW_H
#define f_VIDEOWINDOW_H

#include <windows.h>

extern const char g_szVideoWindowClass[];

#define VIDEOWINDOWCLASS (g_szVideoWindowClass)

ATOM RegisterVideoWindow();

class IVDVideoDisplay;

class VDINTERFACE IVDVideoWindow {
public:
	virtual void SetSourceSize(int w, int h) = 0;
	virtual void GetFrameSize(int& w, int& h) = 0;
	virtual void Resize() = 0;
	virtual void SetChild(HWND hwnd) = 0;
	virtual void SetDisplay(IVDVideoDisplay *) = 0;
};

IVDVideoWindow *VDGetIVideoWindow(HWND hwnd);

enum {
	VWN_RESIZED		= 16,				// WM_NOTIFY: window rect was changed
	VWN_REQUPDATE	= 17
};

#endif
