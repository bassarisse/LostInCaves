#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"

#include "resource.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    EGLView* eglView = EGLView::sharedOpenGLView();
    eglView->setViewName("LostInCaves");
    eglView->setFrameSize(160, 144);
	eglView->setFrameZoomFactor(2.0f);

	HICON hIcon = LoadIcon( GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1) );
	if( hIcon )
	{
	   SendMessage( eglView->getHWnd(), WM_SETICON, ICON_BIG, (LPARAM)hIcon );
	   DestroyIcon( hIcon );
	}

    return Application::sharedApplication()->run();
}
