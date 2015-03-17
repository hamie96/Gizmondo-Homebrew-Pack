// GizVideo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GizVideo.h"
#include <commctrl.h>
#include "SaveVideoToFile.h"
#include "CDIB.h"
#include <shellapi.h>
#include "IDrawable.h" 
#include "MainBackgroundDrawable.h"
#include "DrawList.h"
#include "ButtonDrawable.h"
#include "BackgroundInterfaceDrawable.h"
#include "MainInterfaceDrawable.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE			hInst;			// The current instance
HWND				hwndCB;			// The command bar handle
IDrawable *	m_iDrawable;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass	(HINSTANCE, LPTSTR);
BOOL				InitInstance	(HINSTANCE, int);
LRESULT CALLBACK	WndProc			(HWND, UINT, WPARAM, LPARAM);

int handleKeyPress(WPARAM wKeyPress);

BackgroundInterface * m_biCurrentBackground;

int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPTSTR    lpCmdLine,
					int       nCmdShow)
{


	MSG msg;
	HACCEL hAccelTable;
	CDrawList* cd = new CDrawList();
	m_iDrawable= cd;
	m_biCurrentBackground= new 
		MainInterface();


	//cd->add();

	//cd->add(new ButtonDrawable(100, 44, TEXT("Exit"), TEXT("\\SD CARD\\GizVideo\\playbutton.bmp")));

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_GIZVIDEO);

	
/*
	CCamera * cCam= new CCamera();
	CSaveVideoToFile* csvf= new CSaveVideoToFile(cCam, 
		"\\SD Card\\aaa.avi");
*/
	int maxpictures=3;

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

/*			if (maxpictures-- > 0) 
			{
				cCam->takePicture();
			}
*/
		}
	}


//	delete csvf;
//	delete cCam;

	return msg.wParam; //*/
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    It is important to call this function so that the application 
//    will get 'well formed' small icons associated with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS	wc;

    wc.style			= CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc		= (WNDPROC) WndProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= hInstance;
    wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GIZVIDEO));
    wc.hCursor			= 0;
    wc.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName		= 0;
    wc.lpszClassName	= szWindowClass;

	return RegisterClass(&wc);
}

//
//  FUNCTION: InitInstance(HANDLE, int)
//
//  PURPOSE: Saves instance handle and creates main window
//
//  COMMENTS:
//
//    In this function, we save the instance handle in a global variable and
//    create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND	hWnd;
	TCHAR	szTitle[MAX_LOADSTRING];			// The title bar text
	TCHAR	szWindowClass[MAX_LOADSTRING];		// The window class name

	hInst = hInstance;		// Store instance handle in our global variable
	// Initialize global strings
	LoadString(hInstance, IDC_GIZVIDEO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance, szWindowClass);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{	
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	if (hwndCB)
		CommandBar_Show(hwndCB, TRUE);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	int wmId, wmEvent;
	PAINTSTRUCT ps;

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
		//	switch (wmId)
		//	{
		//		default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
		//	}
			break;
		case WM_KEYDOWN:
			if (handleKeyPress(wParam) == -1)
			{
				PostQuitMessage(0);
			}

			break;

		case WM_CREATE:
			break;
		case WM_PAINT:
			RECT rt;
			hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rt);

			m_biCurrentBackground->Paint(hdc, rt);
			m_iDrawable->Paint(hdc, rt);
			
			
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			//CommandBar_Destroy(hwndCB);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}


int handleKeyPress(WPARAM wKeyPress)
{
	InputButtonState* curInputState= 
		m_biCurrentBackground->getInputButtonState();
	switch(wKeyPress)
	{
	
	case VK_UP: //up button
		curInputState->pressUp();
		break;

	case VK_RIGHT: //right button
		curInputState->pressRight();
		break;

	case VK_DOWN: //down button
		curInputState->pressDown();
		break;

	case VK_LEFT: //left button
		curInputState->pressLeft();
		break;

	case VK_CONTROL: //Stop Button
		curInputState->pressStop();
		break;
	
	case VK_SPACE: //action east
		curInputState->pressRewind();
		break;

	case VK_RETURN: //Play Button
		curInputState->pressPlay();
		break;

	case VK_LSHIFT: //Fast Forward Button
		curInputState->pressFastForward();
		break;


	default:
		break;
	}

	return 0;
}