/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
/*
** RW_IMP.C
**
** This file contains ALL Win32 specific stuff having to do with the
** software refresh.  When a port is being made the following functions
** must be implemented by the port:
**
** SWimp_EndFrame
** SWimp_Init
** SWimp_SetPalette
** SWimp_Shutdown

// Dan East: Thanks ID! This has been a whole lot easier to port to
// Pocket PC than Quake I. :)  I've used GAPI and blit the backbuffer out to the hardware,
// performing palette lookup for each pixel along the way.

*/
#include "..\ref_soft\r_local.h"
#include "rw_win.h"
#include "winquake.h"
 
#ifdef _WIN32_WCE
#ifdef _X86_

BITMAPINFOHEADER	vidppc_pDib;
HBITMAP				vidppc_hDibSect;
COLORREF			*vidppc_outbuf;
HDC					vidppc_hdc;
//Dan: Palette for windows DIB
char			vid_curpal[256][3];
#else

#include <gx.h>

//Dan: Palette for 565 / 555 RGB
unsigned short	vid_curpal[256];
unsigned short *pScreenBuf=NULL;		//Pointer to display RAM
int VID_GetScreenBuf(int calcParams);
void VID_ReleaseScreenBuf();
//GAPI vars
HINSTANCE dxDllLib;
typedef int (*GXOPENDISPLAY)(HWND, DWORD);
typedef struct GXDisplayProperties (*GXGETDISPLAYPROPERTIES)(void);
typedef void * (*GXBEGINDRAW)(void);
typedef struct GXKeyList (*GXGETDEFAULTKEYS)(int);

//functions
GXOPENDISPLAY pGXOpenDisplay=NULL;
FARPROC pGXCloseDisplay;
GXGETDISPLAYPROPERTIES pGXGetDisplayProperties;
GXBEGINDRAW pGXBeginDraw;
FARPROC pGXEndDraw;
FARPROC pGXOpenInput;
FARPROC pGXCloseInput;
GXGETDEFAULTKEYS pGXGetDefaultKeys;

typedef unsigned short (COLORFUNCTYPE) (unsigned char r, unsigned char g, unsigned char b);
COLORFUNCTYPE *ColorConvert;
int VID_GetScreenBuf(int calcParams);
void VID_ReleaseScreenBuf();
int cbxPitch;
int cbyPitch;
int cbNewRowPitch240;
int cbNewColPitch320;

struct GXDisplayProperties g_dispProps;
static short *pDrawStart=0;
static int nextRow=0;
static int xPitch=0;
static int yPitch=0;
static byte *colEnd;
int rowEnd;
static int nextSrcRow=0;

#ifdef _ARM_
//Workaround for iPaq models 38xx
int bIsiPaq38xx=0;
#endif

unsigned short RGB_24toRGB_565(unsigned char r, unsigned char g, unsigned char b) {
	//Dan: RGB 24 bit to RGB 565 mapping
	unsigned int gi=g>>2;
	unsigned int ri=r>>3;
	return b>>3|(gi<<5)|(ri<<11);
}

#endif

#endif

// Console variables that we need to access from this module

swwstate_t sww_state;

/*
** VID_CreateWindow
*/
#ifndef _WIN32_WCE
#define	WINDOW_CLASS_NAME "Quake 2"
#else
//If this is changed then it needs to be changed in sys_win.c as well
#define	WINDOW_CLASS_NAME _T("Quake 2")
#endif

void VID_CreateWindow( int width, int height, int stylebits )
{
	WNDCLASS		wc;
#ifndef _WIN32_WCE
	RECT			r;
	int				x, y, w, h;
#endif
	cvar_t			*vid_xpos, *vid_ypos, *vid_fullscreen;
	int				exstyle;

	vid_xpos = ri.Cvar_Get ("vid_xpos", "0", 0);
	vid_ypos = ri.Cvar_Get ("vid_ypos", "0", 0);
	vid_fullscreen = ri.Cvar_Get ("vid_fullscreen", "0", CVAR_ARCHIVE );

	if ( vid_fullscreen->value )
		exstyle = WS_EX_TOPMOST;
	else
		exstyle = 0;

	/* Register the frame class */
    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC)sww_state.wndproc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = sww_state.hInstance;
    wc.hIcon         = 0;
#ifndef _WIN32_WCE
    wc.hCursor       = LoadCursor (NULL,IDC_ARROW);
#else
    wc.hCursor       = 0;
#endif
	wc.hbrBackground = (void *)COLOR_GRAYTEXT;
    wc.lpszMenuName  = 0;
    wc.lpszClassName = WINDOW_CLASS_NAME;

    if (!RegisterClass (&wc) )
		ri.Sys_Error (ERR_FATAL, "Couldn't register window class");
#ifndef _WIN32_WCE
	r.left = 0;
	r.top = 0;
	r.right  = width;
	r.bottom = height;

	AdjustWindowRect (&r, stylebits, FALSE);

	w = r.right - r.left;
	h = r.bottom - r.top;
	x = vid_xpos->value;
	y = vid_ypos->value;

	sww_state.hWnd = CreateWindowEx (
		exstyle,
		 WINDOW_CLASS_NAME,
		 "Quake 2",
		 stylebits,
		 x, y, w, h,
		 NULL,
		 NULL,
		 sww_state.hInstance,
		 NULL);
#else
	sww_state.hWnd = CreateWindow(
		 WINDOW_CLASS_NAME,
		 _T("Quake 2"),
		 stylebits,
		 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		 NULL,
		 NULL,
		 sww_state.hInstance,
		 NULL);
/*
	sww_state.hWnd = CreateWindowEx (
		0,
		 WINDOW_CLASS_NAME,
		 _T("Quake 2"),
		 stylebits,
		 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		 NULL,
		 NULL,
		 sww_state.hInstance,
		 NULL);
*/
#endif
	if (!sww_state.hWnd)
		ri.Sys_Error (ERR_FATAL, "Couldn't create window");
	
	ShowWindow( sww_state.hWnd, SW_SHOWNORMAL );
	UpdateWindow( sww_state.hWnd );
	SetForegroundWindow( sww_state.hWnd );
	SetFocus( sww_state.hWnd );

	// let the sound and input subsystems know about the new window
	ri.Vid_NewWindow (width, height);
}

/*
** SWimp_Init
**
** This routine is responsible for initializing the implementation
** specific stuff in a software rendering subsystem.
*/
int SWimp_Init( void *hInstance, void *wndProc )
{
	sww_state.hInstance = ( HINSTANCE ) hInstance;
	sww_state.wndproc = wndProc;

#ifdef _WIN32_WCE
#ifdef _ARM_
	{
		TCHAR pdaname[100];
		SystemParametersInfo(SPI_GETOEMINFO, 100, pdaname, SPIF_SENDCHANGE || SPIF_UPDATEINIFILE);
		bIsiPaq38xx=!_tcscmp(pdaname,_T("Compaq iPAQ H3800"));
	}
#endif
#ifndef _X86_
	{
	int rc;

	dxDllLib=LoadLibrary(_T("gx.dll"));
	if (!dxDllLib) {
		MessageBox(sww_state.hWnd, _T("Unable to find the file gx.dll. This file needs to be copied into your \\Windows\\ directory before Pocket Quake 2 can run."), _T("Error"), MB_OK);
		//TODO: exit app
		return false;
	}

	pGXOpenDisplay=(GXOPENDISPLAY)GetProcAddress(dxDllLib, _T("?GXOpenDisplay@@YAHPAUHWND__@@K@Z"));
	pGXCloseDisplay=(FARPROC)GetProcAddress(dxDllLib, _T("?GXCloseDisplay@@YAHXZ"));
	pGXGetDisplayProperties=(GXGETDISPLAYPROPERTIES)GetProcAddress(dxDllLib, _T("?GXGetDisplayProperties@@YA?AUGXDisplayProperties@@XZ"));
	pGXBeginDraw=(GXBEGINDRAW)GetProcAddress(dxDllLib, _T("?GXBeginDraw@@YAPAXXZ"));
	pGXEndDraw=(FARPROC)GetProcAddress(dxDllLib, _T("?GXEndDraw@@YAHXZ"));
	pGXOpenInput=(FARPROC)GetProcAddress(dxDllLib, _T("?GXOpenInput@@YAHXZ"));
	pGXCloseInput=(FARPROC)GetProcAddress(dxDllLib, _T("?GXCloseInput@@YAHXZ"));
	pGXGetDefaultKeys=(GXGETDEFAULTKEYS)GetProcAddress(dxDllLib, _T("?GXGetDefaultKeys@@YA?AUGXKeyList@@H@Z"));

	rc=pGXOpenDisplay(sww_state.hWnd, GX_FULLSCREEN);
	if (rc) {
#ifdef _ARM_
		if (bIsiPaq38xx) {
			//If running on an iPaq 3835 manually set the graphics settings
			//ADDED BY SCOTT RAMSBY
			g_dispProps.cxWidth = 240;
			g_dispProps.cyHeight = 320;
			g_dispProps.cBPP = 16;
			g_dispProps.ffFormat = 168;
			g_dispProps.cbyPitch = 2;
			g_dispProps.cbxPitch = -640;
		} else
#endif
		g_dispProps=pGXGetDisplayProperties();
		cbxPitch=g_dispProps.cbxPitch >> 1;	//We divide these values by two since we deal with only shorts
		cbyPitch=g_dispProps.cbyPitch >> 1;
		cbNewRowPitch240=cbyPitch-(240*cbxPitch);
		cbNewColPitch320=cbyPitch-(240*cbyPitch);
	}
	if (pGXOpenInput) rc=pGXOpenInput();
	if (pGXGetDefaultKeys) pGXGetDefaultKeys(GX_NORMALKEYS);
	ColorConvert=RGB_24toRGB_565;
	}
#endif
#endif
	return true;
}

/*
** SWimp_InitGraphics
**
** This initializes the software refresh's implementation specific
** graphics subsystem.  In the case of Windows it creates DIB or
** DDRAW surfaces.
**
** The necessary width and height parameters are grabbed from
** vid.width and vid.height.
*/
static qboolean SWimp_InitGraphics( qboolean fullscreen )
{
	// free resources in use
	SWimp_Shutdown ();

	// create a new window
	VID_CreateWindow (vid.width, vid.height, WINDOW_STYLE);

#ifndef _WIN32_WCE
	// initialize the appropriate subsystem
	if ( !fullscreen )
	{
		if ( !DIB_Init( &vid.buffer, &vid.rowbytes ) )
		{
			vid.buffer = 0;
			vid.rowbytes = 0;

			return false;
		}
	}
	else
	{
		if ( !DDRAW_Init( &vid.buffer, &vid.rowbytes ) )
		{
			vid.buffer = 0;
			vid.rowbytes = 0;

			return false;
		}
	}
#else
	vid.buffer = malloc (vid.width * vid.height);
	vid.rowbytes=vid.width;
#ifndef _X86_
	VID_GetScreenBuf(1);
	VID_ReleaseScreenBuf();
#endif
#endif
	return true;
}

/*
** SWimp_EndFrame
**
** This does an implementation specific copy from the backbuffer to the
** front buffer.  In the Win32 case it uses BitBlt or BltFast depending
** on whether we're using DIB sections/GDI or DDRAW.
*/
void SWimp_EndFrame (void)
{
#ifndef _WIN32_WCE
	if ( !sw_state.fullscreen )
	{
		if ( sww_state.palettized )
		{
//			holdpal = SelectPalette(hdcScreen, hpalDIB, FALSE);
//			RealizePalette(hdcScreen);
		}

	    
		BitBlt( sww_state.hDC,
			    0, 0,
				vid.width,
				vid.height,
				sww_state.hdcDIBSection,
				0, 0,
				SRCCOPY );

		if ( sww_state.palettized )
		{
//			SelectPalette(hdcScreen, holdpal, FALSE);
		}
	}
	else
	{
		RECT r;
		HRESULT rval;
		DDSURFACEDESC ddsd;

		r.left = 0;
		r.top = 0;
		r.right = vid.width;
		r.bottom = vid.height;

		sww_state.lpddsOffScreenBuffer->lpVtbl->Unlock( sww_state.lpddsOffScreenBuffer, vid.buffer );

		if ( sww_state.modex )
		{
			if ( ( rval = sww_state.lpddsBackBuffer->lpVtbl->BltFast( sww_state.lpddsBackBuffer,
																	0, 0,
																	sww_state.lpddsOffScreenBuffer, 
																	&r, 
																	DDBLTFAST_WAIT ) ) == DDERR_SURFACELOST )
			{
				sww_state.lpddsBackBuffer->lpVtbl->Restore( sww_state.lpddsBackBuffer );
				sww_state.lpddsBackBuffer->lpVtbl->BltFast( sww_state.lpddsBackBuffer,
															0, 0,
															sww_state.lpddsOffScreenBuffer, 
															&r, 
															DDBLTFAST_WAIT );
			}

			if ( ( rval = sww_state.lpddsFrontBuffer->lpVtbl->Flip( sww_state.lpddsFrontBuffer,
															 NULL, DDFLIP_WAIT ) ) == DDERR_SURFACELOST )
			{
				sww_state.lpddsFrontBuffer->lpVtbl->Restore( sww_state.lpddsFrontBuffer );
				sww_state.lpddsFrontBuffer->lpVtbl->Flip( sww_state.lpddsFrontBuffer, NULL, DDFLIP_WAIT );
			}
		}
		else
		{
			if ( ( rval = sww_state.lpddsBackBuffer->lpVtbl->BltFast( sww_state.lpddsFrontBuffer,
																	0, 0,
																	sww_state.lpddsOffScreenBuffer, 
																	&r, 
																	DDBLTFAST_WAIT ) ) == DDERR_SURFACELOST )
			{
				sww_state.lpddsBackBuffer->lpVtbl->Restore( sww_state.lpddsFrontBuffer );
				sww_state.lpddsBackBuffer->lpVtbl->BltFast( sww_state.lpddsFrontBuffer,
															0, 0,
															sww_state.lpddsOffScreenBuffer, 
															&r, 
															DDBLTFAST_WAIT );
			}
		}

		memset( &ddsd, 0, sizeof( ddsd ) );
		ddsd.dwSize = sizeof( ddsd );
	
		sww_state.lpddsOffScreenBuffer->lpVtbl->Lock( sww_state.lpddsOffScreenBuffer, NULL, &ddsd, DDLOCK_WAIT, NULL );

		vid.buffer = ddsd.lpSurface;
		vid.rowbytes = ddsd.lPitch;
	}
#else
#ifndef _X86_
	//Pocket PC GAPI rendering
	register unsigned short *cdst;
	register byte *csrc, *rowEnd2;
	register int xPitch2=xPitch, nextRow2=nextRow, nextSrcRow2=nextSrcRow;

#if !defined(MIPS)&&!defined(SH3)
	//We need to get the buffer each frame for MIPS (SH3 too)
	if (!pScreenBuf) 
#endif
		if (!VID_GetScreenBuf(0)) return;

	//Blit our drawing buffer out to the display
	cdst=pDrawStart;
	csrc=vid.buffer;

	while (csrc<colEnd) {
		//Calculate the ending row pixel
		rowEnd2=csrc+rowEnd;
		while (csrc<rowEnd2) {
			//The next line is what actually sets the color value of the display pixel
			*cdst=(short)vid_curpal[*csrc];
			//Increment our source and display buffer pointers one pixel to the right
			cdst+=xPitch2;
			csrc++;
		}
		//Move down to the next row and all the way to the left
		cdst+=nextRow2;
		csrc+=nextSrcRow2;
	}

#if defined(MIPS)||defined(SH3)
	//We release the buffer each frame for MIPS (SH3 too)
	VID_ReleaseScreenBuf();
#endif
#else

	//This routine is for non-Game API support, such as the emulator (or non-CE 3.0 devices).
	//This can be optimized quite a bit, but the emulator is far faster than any CE device to begin with.

	register byte *csrc;
	HDC hdc;
	int x,y;
	char *pos=(char *)vidppc_outbuf;

	csrc=vid.buffer;
	for (y=0; y<180; y++) {
		for (x=0; x<240; x++ ) {
			*pos++=vid_curpal[*csrc][0];
			*pos++=vid_curpal[*csrc][1];
			*pos++=vid_curpal[*csrc++][2];
		}
		//csrc+=320-240;
	}
	hdc=GetDC(sww_state.hWnd );
	BitBlt(hdc, 0, 24, 240, 180, vidppc_hdc, 0, 0, SRCCOPY);
	ReleaseDC(sww_state.hWnd , hdc);

#endif
#endif
}

/*
** SWimp_SetMode
*/
rserr_t SWimp_SetMode( int *pwidth, int *pheight, int mode, qboolean fullscreen )
{
	const char *win_fs[] = { "W", "FS" };
	rserr_t retval = rserr_ok;

	ri.Con_Printf (PRINT_ALL, "setting mode %d:", mode );

	if ( !ri.Vid_GetModeInfo( pwidth, pheight, mode ) )
	{
		ri.Con_Printf( PRINT_ALL, " invalid mode\n" );
		return rserr_invalid_mode;
	}

	ri.Con_Printf( PRINT_ALL, " %d %d %s\n", *pwidth, *pheight, win_fs[fullscreen] );

	sww_state.initializing = true;
	if ( fullscreen )
	{
		if ( !SWimp_InitGraphics( 1 ) )
		{
			if ( SWimp_InitGraphics( 0 ) )
			{
				// mode is legal but not as fullscreen
				fullscreen = 0;
				retval = rserr_invalid_fullscreen;
			}
			else
			{
				// failed to set a valid mode in windowed mode
				retval = rserr_unknown;
			}
		}
	}
	else
	{
		// failure to set a valid mode in windowed mode
		if ( !SWimp_InitGraphics( fullscreen ) )
		{
			sww_state.initializing = true;
			return rserr_unknown;
		}
	}

	sw_state.fullscreen = fullscreen;
#if 0
	if ( retval != rserr_unknown )
	{
		if ( retval == rserr_invalid_fullscreen ||
			 ( retval == rserr_ok && !fullscreen ) )
		{
			SetWindowLong( sww_state.hWnd, GWL_STYLE, WINDOW_STYLE );
		}
	}
#endif
	R_GammaCorrectAndSetPalette( ( const unsigned char * ) d_8to24table );
	sww_state.initializing = true;

#ifdef _WIN32_WCE
//	vid_modenum=mode;
#ifndef _X86_
	VID_GetScreenBuf(1);
	VID_ReleaseScreenBuf();
#else
	{
		//Non Game-API support
		HDC hdc=GetDC(sww_state.hWnd);
		vidppc_hdc=CreateCompatibleDC(hdc);
		ReleaseDC(sww_state.hWnd, hdc);

		memset( &vidppc_pDib, 0, sizeof( BITMAPINFOHEADER ) );
		vidppc_pDib.biSize = sizeof( BITMAPINFOHEADER );
		vidppc_pDib.biWidth = 240;
		vidppc_pDib.biHeight = -320; // top down
		vidppc_pDib.biPlanes = 1;
		vidppc_pDib.biBitCount = 24; // 16 bit color
		vidppc_pDib.biCompression = BI_RGB;

		vidppc_hDibSect = CreateDIBSection( vidppc_hdc, (BITMAPINFO*)&vidppc_pDib,
			DIB_RGB_COLORS, &vidppc_outbuf, NULL, 0);
		SelectObject(vidppc_hdc, vidppc_hDibSect);
	}
#endif
#endif
	return retval;
}

/*
** SWimp_SetPalette
**
** System specific palette setting routine.  A NULL palette means
** to use the existing palette.  The palette is expected to be in
** a padded 4-byte xRGB format.
*/
void SWimp_SetPalette( const unsigned char *palette )
{
	//Dan East: You are cool if you place profanities in your comments:
	// MGL - what the fuck was kendall doing here?!
	// clear screen to black and change palette
	//	for (i=0 ; i<vid.height ; i++)
	//		memset (vid.buffer + i*vid.rowbytes, 0, vid.width);

	if ( !palette )
		palette = ( const unsigned char * ) sw_state.currentpalette;

#ifndef _WIN32_WCE
	if ( !sw_state.fullscreen )
	{
		DIB_SetPalette( ( const unsigned char * ) palette );
	}
	else
	{
		DDRAW_SetPalette( ( const unsigned char * ) palette );
	}
#else
#ifndef _X86_
	if (palette) {
		int i;
		for (i = 0; i < 256; i++)
			vid_curpal[i]=ColorConvert(palette[i*4],palette[i*4+1],palette[i*4+2]);
	}
#else
	if (palette) {
		int i;
		for (i = 0; i < 256; i++)
		{
			//if (palette[i*3]==255&&palette[i*3+1]==0&&palette[i*3+2]==0)
			//	i=i;

			vid_curpal[i][2]=palette[i*4+0];
			vid_curpal[i][1]=palette[i*4+1];
			vid_curpal[i][0]=palette[i*4+2];
		}
	}
#endif
#endif
}

/*
** SWimp_Shutdown
**
** System specific graphics subsystem shutdown routine.  Destroys
** DIBs or DDRAW surfaces as appropriate.
*/
void SWimp_Shutdown( void )
{
	ri.Con_Printf( PRINT_ALL, "Shutting down SW imp\n" );
#ifndef _WIN32_WCE
	DIB_Shutdown();
	DDRAW_Shutdown();
#endif
	if ( sww_state.hWnd )
	{
		ri.Con_Printf( PRINT_ALL, "...destroying window\n" );
		ShowWindow( sww_state.hWnd, SW_SHOWNORMAL );	// prevents leaving empty slots in the taskbar
		DestroyWindow (sww_state.hWnd);
		sww_state.hWnd = NULL;
		UnregisterClass (WINDOW_CLASS_NAME, sww_state.hInstance);
	}
}

/*
** SWimp_AppActivate
*/
void SWimp_AppActivate( qboolean active )
{
	if ( active )
	{
		if ( sww_state.hWnd )
		{
			SetForegroundWindow( sww_state.hWnd );
			ShowWindow( sww_state.hWnd, SW_RESTORE );
		}
	}
	else
	{
		if ( sww_state.hWnd )
		{
			if ( sww_state.initializing )
				return;
			if ( vid_fullscreen->value )
				ShowWindow( sww_state.hWnd, SW_MINIMIZE );
		}
	}
}

//===============================================================================


/*
================
Sys_MakeCodeWriteable
================
*/
void Sys_MakeCodeWriteable (unsigned long startaddr, unsigned long length)
{
	DWORD  flOldProtect;

	if (!VirtualProtect((LPVOID)startaddr, length, PAGE_READWRITE, &flOldProtect))
 		ri.Sys_Error(ERR_FATAL, "Protection change failed\n");
}

/*
** Sys_SetFPCW
**
** For reference:
**
** 1
** 5               0
** xxxxRRPP.xxxxxxxx
**
** PP = 00 = 24-bit single precision
** PP = 01 = reserved
** PP = 10 = 53-bit double precision
** PP = 11 = 64-bit extended precision
**
** RR = 00 = round to nearest
** RR = 01 = round down (towards -inf, floor)
** RR = 10 = round up (towards +inf, ceil)
** RR = 11 = round to zero (truncate/towards 0)
**
*/
#if !id386
void Sys_SetFPCW (void)
{
}
#else
unsigned fpu_ceil_cw, fpu_chop_cw, fpu_full_cw, fpu_cw, fpu_pushed_cw;
unsigned fpu_sp24_cw, fpu_sp24_ceil_cw;

void Sys_SetFPCW( void )
{
	__asm xor eax, eax

	__asm fnstcw  word ptr fpu_cw
	__asm mov ax, word ptr fpu_cw

	__asm and ah, 0f0h
	__asm or  ah, 003h          ; round to nearest mode, extended precision
	__asm mov fpu_full_cw, eax

	__asm and ah, 0f0h
	__asm or  ah, 00fh          ; RTZ/truncate/chop mode, extended precision
	__asm mov fpu_chop_cw, eax

	__asm and ah, 0f0h
	__asm or  ah, 00bh          ; ceil mode, extended precision
	__asm mov fpu_ceil_cw, eax

	__asm and ah, 0f0h          ; round to nearest, 24-bit single precision
	__asm mov fpu_sp24_cw, eax

	__asm and ah, 0f0h          ; ceil mode, 24-bit single precision
	__asm or  ah, 008h          ; 
	__asm mov fpu_sp24_ceil_cw, eax
}
#endif

#if defined(_WIN32_WCE)&&!defined(_X86_)
int VID_GetScreenBuf(int calcParams) {
	//Return codes:
	// 0: failure
	// 1: success
//	MessageBox(sww_state.hWnd, _T("Enter VID_GetScreenBuf"), _T("Dan"), MB_OK);
	if (!pScreenBuf) {
		//We don't have a direct screen access buffer, try to get it.
#ifdef _ARM_
		if (bIsiPaq38xx)
			pScreenBuf=(unsigned short *) 0xac0755a0;
		else {
#endif
		if (!pGXBeginDraw) return 0;	//gx.dll has not been loaded
		pScreenBuf=(unsigned short *)pGXBeginDraw();
		if (!pScreenBuf) return 0;	//Can't access screen buffer
#ifdef _ARM_
		}
#endif
	}

	//Calculate 
	if (calcParams||!pDrawStart)
		if (!sw_mode->value) {
			//Move down to the bottom of taskbar
			pDrawStart=pScreenBuf+(cbyPitch*24);	//24 - harcoded height of taskbar

			//This value represents how we have to offset to go from right side of the screen
			//to the left side of the screen and down one pixel:
			//(0, y+1) = (240, y)+nextRow
			//In the case of devices whose screen has (0,0) at the top left this will be zero
			//For devices whose (0,0) is the bottom left (iPaq), this will be a large negative number.
			nextRow=cbyPitch-(240*cbxPitch);
			xPitch=cbxPitch;
			yPitch=cbyPitch;
			//Calculate the ending column pixel
			colEnd=vid.buffer+240*180;
			rowEnd=240;
			nextSrcRow=0;	
		} else if (sw_mode->value==1) {
			//Landscape Right
			//Move to bottom of screen
			//Dan: TODO: won't work for screens that aren't physically rotated like the iPaq's
//			if (cbyPitch<0)
			//pDrawStart=backBuffer;
			pDrawStart=pScreenBuf+(cbyPitch*320);

			nextRow=cbxPitch+(320*cbyPitch);
			xPitch=-cbyPitch;
			yPitch=cbxPitch;
			//Calculate the ending column pixel
			colEnd=vid.buffer+320*240;
			rowEnd=320;
			nextSrcRow=0;
		} else {
			//Assume Landscape rotated Left
			pDrawStart=pScreenBuf+(cbxPitch*240);
			nextRow=-(cbxPitch+(320*cbyPitch));
			xPitch=cbyPitch;
			yPitch=-cbxPitch;
			//Calculate the ending column pixel
			colEnd=vid.buffer+320*240;
			rowEnd=320;
			nextSrcRow=0;
		}

//	MessageBox(sww_state.hWnd, _T("Leave VID_GetScreenBuf"), _T("Dan"), MB_OK);
	return 1;
}

void VID_ReleaseScreenBuf() {
#ifdef _ARM_
	if (!bIsiPaq38xx)
#endif
	if (pScreenBuf&&pGXEndDraw) {
		pGXEndDraw();
//		MessageBox(sww_state.hWnd, _T("EndDraw"), _T("Dan"), MB_OK);
	}
	pScreenBuf=NULL;
}
#endif