// ShaderLabb.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ShaderLabb.h"
#include "RevSetupData.h"
#include "RevEngineMain.h"
#include "App.h"
#include <assert.h>


#define MAX_LOADSTRING 100

// Global Variables:
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	
	MSG windowsMessage;
	HWND windowHandler;

	RevSetupData setupData( "RevelationsEngine2.0", 800, 800, true );

	if( !RevEngineMain::Create( setupData, windowHandler, WndProc, hInstance ) )
	{
		assert( 0 && "FAILED TO INITLIZE ENGINE" );
		return 0;
	}

	App baseUpdateApp(  RevEngineMain::GetInstance() );

	while( true )
	{	
		if ( PeekMessage( &windowsMessage, NULL, 0, 0, PM_REMOVE ) ) 
		{
			// handle or dispatch messages
			if ( windowsMessage.message == WM_QUIT )
			{
				break;
			}
			TranslateMessage( &windowsMessage );
			DispatchMessage( &windowsMessage );
		} 
		else
		{
			if( !baseUpdateApp.Update( ) )
			{
				break; //Exits the application if it returns 0; 
				//Make sure that everything is deleted on main game thread before doing this
			}
		}

	}

	RevEngineMain::Destroy( );

	return 0;
}


LRESULT CALLBACK WndProc(	HWND hWnd, 
						 UINT message, 
						 WPARAM wParam, 
						 LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	};
	return 0;
}
