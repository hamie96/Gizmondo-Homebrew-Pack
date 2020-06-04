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
// Main windowed and fullscreen graphics interface module. This module
// is used for both the software and OpenGL rendering versions of the
// Quake refresh engine.
#ifndef _WIN32_WCE
#include <assert.h>
#include <float.h>
#else
#include <windows.h>
#include <Aygshell.h>
#include "..\resource.h"
static HBITMAP ctrlBmp, titlebarBmp;
#define CONTROL_TOP (180+24)
#endif

#include "..\client\client.h"
#include "winquake.h"
//#include "zmouse.h"

// Structure containing functions exported from refresh DLL
refexport_t	re;

cvar_t *win_noalttab;

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL (WM_MOUSELAST+1)  // message that will be supported by the OS 
#endif

static UINT MSH_MOUSEWHEEL;

// Console variables that we need to access from this module
cvar_t		*vid_gamma;
cvar_t		*vid_ref;			// Name of Refresh DLL loaded
cvar_t		*vid_xpos;			// X coordinate of window position
cvar_t		*vid_ypos;			// Y coordinate of window position
cvar_t		*vid_fullscreen;
#ifdef _WIN32_WCE
cvar_t  *sw_mode;
#endif

// Global variables used internally by this module
viddef_t	viddef;				// global video state; used by other modules
HINSTANCE	reflib_library;		// Handle to refresh DLL 
qboolean	reflib_active = 0;

HWND        cl_hwnd;            // Main window handle for life of program

#define VID_NUM_MODES ( sizeof( vid_modes ) / sizeof( vid_modes[0] ) )

LONG WINAPI MainWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

static qboolean s_alttab_disabled;

extern	unsigned	sys_msg_time;

/*
** WIN32 helper functions
*/
extern qboolean s_win95;

static void WIN_DisableAltTab( void )
{
#ifndef _WIN32_WCE
	if ( s_alttab_disabled )
		return;

	if ( s_win95 )
	{
		BOOL old;

		SystemParametersInfo( SPI_SCREENSAVERRUNNING, 1, &old, 0 );
	}
	else
	{
		RegisterHotKey( 0, 0, MOD_ALT, VK_TAB );
		RegisterHotKey( 0, 1, MOD_ALT, VK_RETURN );
	}
	s_alttab_disabled = true;
#endif
}

static void WIN_EnableAltTab( void )
{
#ifndef _WIN32_WCE
	if ( s_alttab_disabled )
	{
		if ( s_win95 )
		{
			BOOL old;

			SystemParametersInfo( SPI_SCREENSAVERRUNNING, 0, &old, 0 );
		}
		else
		{
			UnregisterHotKey( 0, 0 );
			UnregisterHotKey( 0, 1 );
		}

		s_alttab_disabled = false;
	}
#endif
}

/*
==========================================================================

DLL GLUE

==========================================================================
*/

#define	MAXPRINTMSG	4096
void VID_Printf (int print_level, char *fmt, ...)
{
	va_list		argptr;
	char		msg[MAXPRINTMSG];
	static qboolean	inupdate;
	
	va_start (argptr,fmt);
	vsprintf (msg,fmt,argptr);
	va_end (argptr);

	if (print_level == PRINT_ALL)
	{
		Com_Printf ("%s", msg);
	}
	else if ( print_level == PRINT_DEVELOPER )
	{
		Com_DPrintf ("%s", msg);
	}
	else if ( print_level == PRINT_ALERT )
	{
#ifndef _WIN32_WCE
		MessageBox( 0, msg, "PRINT_ALERT", MB_ICONWARNING );
		OutputDebugString( msg );
#else
		TCHAR msg_t[MAXPRINTMSG];
		MultiByteToWideChar(CP_ACP, 0, msg, -1, msg_t, sizeof(msg_t)/sizeof(TCHAR));
		MessageBox( 0, msg_t, _T("PRINT_ALERT"), MB_ICONWARNING );
		OutputDebugString( msg_t );
#endif
	}
}

void VID_Error (int err_level, char *fmt, ...)
{
	va_list		argptr;
	char		msg[MAXPRINTMSG];
	static qboolean	inupdate;
	
	va_start (argptr,fmt);
	vsprintf (msg,fmt,argptr);
	va_end (argptr);

	Com_Error (err_level,"%s", msg);
}

//==========================================================================
#ifndef _WIN32_WCE
byte        scantokey[128] = 
					{ 
//  0           1       2       3       4       5       6       7 
//  8           9       A       B       C       D       E       F 
	0  ,    27,     '1',    '2',    '3',    '4',    '5',    '6', 
	'7',    '8',    '9',    '0',    '-',    '=',    K_BACKSPACE, 9, // 0 
	'q',    'w',    'e',    'r',    't',    'y',    'u',    'i', 
	'o',    'p',    '[',    ']',    13 ,    K_CTRL,'a',  's',      // 1 
	'd',    'f',    'g',    'h',    'j',    'k',    'l',    ';', 
	'\'' ,    '`',    K_SHIFT,'\\',  'z',    'x',    'c',    'v',      // 2 
	'b',    'n',    'm',    ',',    '.',    '/',    K_SHIFT,'*', 
	K_ALT,' ',   0  ,    K_F1, K_F2, K_F3, K_F4, K_F5,   // 3 
	K_F6, K_F7, K_F8, K_F9, K_F10,  K_PAUSE,    0  , K_HOME, 
	K_UPARROW,K_PGUP,K_KP_MINUS,K_LEFTARROW,K_KP_5,K_RIGHTARROW, K_KP_PLUS,K_END, //4 
	K_DOWNARROW,K_PGDN,K_INS,K_DEL,0,0,             0,              K_F11, 
	K_F12,0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 5
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 6 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0         // 7 
}; 
#else
byte        scantokey[256] = {
	0,      '\x001', '\x002', '\x003', '\x004', '\x005', '\x006', '\x007', K_BACKSPACE, '\x009',
	'\x00a', '\x00b', '\x00c', K_ENTER, '\x00e', '\x00f', K_SHIFT, '\x011', '\x012', '\x013', 
	'\x014', '\x015', '\x016', '\x017', '\x018', '\x019', '\x01a', '\x01b', '\x01c', '\x01d', 
	'\x01e', '\x01f', '\x020', '\x021', '\x022', '\x023', '\x024', K_LEFTARROW, K_UPARROW, K_RIGHTARROW, 
	K_DOWNARROW, '\x029', '\x02a', '\x02b', '\x02c', '\x02d', '\x02e', '\x02f', ')', '!',
	'@', '#', '$', '%', '^', '&', '*', '(', '\x03a', '\x03b', 
	'\x03c', '\x03d', '\x03e', '\x03f', '\x040', '\x041', '\x042', '\x043', '\x044', '\x045', 
	'\x046', '\x047', '\x048', '\x049', '\x04a', '\x04b', '\x04c', '\x04d', '\x04e', '\x04f', 
	'\x050', '\x051', '\x052', '\x053', '\x054', '\x055', '\x056', '\x057', '\x058', '\x059', 	
	'\x05a', 0/*'\x05b'*/, '\x05c', '\x05d', '\x05e', '\x05f', '\x060', '\x061', '\x062', '\x063', 	
	'\x064', '\x065', '\x066', '\x067', '\x068', '\x069', '\x06a', '\x06b', '\x06c', '\x06d', 	
	'\x06e', '\x06f', '\x070', '\x071', '\x072', '\x073', '\x074', '\x075', '\x076', '\x077', 	
	'\x078', '\x079', '\x07a', '\x07b', '\x07c', '\x07d', '\x07e', '\x07f', '\x080', '\x081', 
	'\x082', '\x083', '\x084', '\x085', K_JOY1,  '\x087', '\x088', '\x089', '\x08a', '\x08b', 	
	'\x08c', '\x08d', '\x08e', '\x08f', '\x090', '\x091', '\x092', '\x093', '\x094', '\x095', 	
	'\x096', '\x097', '\x098', '\x099', '\x09a', '\x09b', '\x09c', '\x09d', '\x09e', '\x09f', 	
	'\x0a0', '\x0a1', '\x0a2', '\x0a3', '\x0a4', '\x0a5', '\x0a6', '\x0a7', '\x0a8', '\x0a9', 	
	'\x0aa', '\x0ab', '\x0ac', '\x0ad', '\x0ae', '\x0af', '\x0b0', '\x0b1', '\x0b2', '\x0b3', 	
	'\x0b4', '\x0b5', '\x0b6', '\x0b7', '\x0b8', '\x0b9', ':', '+', '<', '_', 
	'>', '?', '\x0c0', K_AUX1,  K_AUX2,  K_AUX3,  K_AUX4, K_AUX5, '\x0c6', '\x0c7', 
	'\x0c8', '\x0c9', '\x0ca', '\x0cb', '\x0cc', '\x0cd', '\x0ce', '\x0cf', '\x0d0', '\x0d1', 
	'\x0d2', '\x0d3', '\x0d4', '\x0d5', '\x0d6', '\x0d7', '\x0d8', '\x0d9', '\x0da', '{', 
	'|', '}', '"', '\x0df', '\x0e0', '\x0e1', '\x0e2', '\x0e3', '\x0e4', '\x0e5', 
	'\x0e6', '\x0e7', '\x0e8', '\x0e9', '\x0ea', '\x0eb', '\x0ec', '\x0ed', '\x0ee', '\x0ef', 
	'\x0f0', '\x0f1', '\x0f2', '\x0f3', '\x0f4', '\x0f5', '\x0f6', '\x0f7', '\x0f8', '\x0f9', 
	'\x0fa', '\x0fb', '\x0fc', '\x0fd', '\x0fe', '\x0ff'};

byte        scantolower[256] = {
	0,      '\x001', '\x002', '\x003', '\x004', '\x005', '\x006', '\x007', K_BACKSPACE, '\x009',
	'\x00a', '\x00b', '\x00c', K_ENTER, '\x00e', '\x00f', K_SHIFT, '\x011', '\x012', '\x013', 
	'\x014', '\x015', '\x016', '\x017', '\x018', '\x019', '\x01a', '\x01b', '\x01c', '\x01d', 
	'\x01e', '\x01f', '\x020', '\x021', '\x022', '\x023', '\x024', K_LEFTARROW, K_UPARROW, K_RIGHTARROW, 
	K_DOWNARROW, '\x029', '\x02a', '\x02b', '\x02c', '\x02d', '\x02e', '\x02f', '\x030', '\x031',
	'\x032', '\x033', '\x034', '\x035', '\x036', '\x037', '\x038', '\x039', '\x03a', '\x03b', 
	'\x03c', '\x03d', '\x03e', '\x03f', '\x040', 'a', 'b', 'c', 'd', 'e', 
	'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 
	'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 	
	'z', 0, '\x05c', '\x05d', '\x05e', '\x05f', '\x060', '\x061', '\x062', '\x063', 	
	'\x064', '\x065', '\x066', '\x067', '\x068', '\x069', '\x06a', '\x06b', '\x06c', '\x06d', 	
	'\x06e', '\x06f', '\x070', '\x071', '\x072', '\x073', '\x074', '\x075', '\x076', '\x077', 	
	'\x078', '\x079', '\x07a', '\x07b', '\x07c', '\x07d', '\x07e', '\x07f', '\x080', '\x081', 
	'\x082', '\x083', '\x084', '\x085', K_JOY1,  '\x087', '\x088', '\x089', '\x08a', '\x08b', 	
	'\x08c', '\x08d', '\x08e', '\x08f', '\x090', '\x091', '\x092', '\x093', '\x094', '\x095', 	
	'\x096', '\x097', '\x098', '\x099', '\x09a', '\x09b', '\x09c', '\x09d', '\x09e', '\x09f', 	
	'\x0a0', '\x0a1', '\x0a2', '\x0a3', '\x0a4', '\x0a5', '\x0a6', '\x0a7', '\x0a8', '\x0a9', 	
	'\x0aa', '\x0ab', '\x0ac', '\x0ad', '\x0ae', '\x0af', '\x0b0', '\x0b1', '\x0b2', '\x0b3', 	
	'\x0b4', '\x0b5', '\x0b6', '\x0b7', '\x0b8', '\x0b9', ';', '=', ',', '-', 
	'.', '/', '\x0c0', K_AUX1,  K_AUX2,  K_AUX3,  K_AUX4, K_AUX5, '\x0c6', '\x0c7', 
	'\x0c8', '\x0c9', '\x0ca', '\x0cb', '\x0cc', '\x0cd', '\x0ce', '\x0cf', '\x0d0', '\x0d1', 
	'\x0d2', '\x0d3', '\x0d4', '\x0d5', '\x0d6', '\x0d7', '\x0d8', '\x0d9', '\x0da', '[', 
	'\\', ']', '\'', '\x0df', '\x0e0', '\x0e1', '\x0e2', '\x0e3', '\x0e4', '\x0e5', 
	'\x0e6', '\x0e7', '\x0e8', '\x0e9', '\x0ea', '\x0eb', '\x0ec', '\x0ed', '\x0ee', '\x0ef', 
	'\x0f0', '\x0f1', '\x0f2', '\x0f3', '\x0f4', '\x0f5', '\x0f6', '\x0f7', '\x0f8', '\x0f9', 
	'\x0fa', '\x0fb', '\x0fc', '\x0fd', '\x0fe', '\x0ff'};
#endif
/*
=======
MapKey

Map from windows to quake keynums
=======
*/
#ifndef _WIN32_WCE
int MapKey (int key)
{
	int result;
	int modified = ( key >> 16 ) & 255;
	qboolean is_extended = false;

	if ( modified > 127)
		return 0;

	if ( key & ( 1 << 24 ) )
		is_extended = true;

	result = scantokey[modified];

	if ( !is_extended )
	{
		switch ( result )
		{
		case K_HOME:
			return K_KP_HOME;
		case K_UPARROW:
			return K_KP_UPARROW;
		case K_PGUP:
			return K_KP_PGUP;
		case K_LEFTARROW:
			return K_KP_LEFTARROW;
		case K_RIGHTARROW:
			return K_KP_RIGHTARROW;
		case K_END:
			return K_KP_END;
		case K_DOWNARROW:
			return K_KP_DOWNARROW;
		case K_PGDN:
			return K_KP_PGDN;
		case K_INS:
			return K_KP_INS;
		case K_DEL:
			return K_KP_DEL;
		default:
			return result;
		}
	}
	else
	{
		switch ( result )
		{
		case 0x0D:
			return K_KP_ENTER;
		case 0x2F:
			return K_KP_SLASH;
		case 0xAF:
			return K_KP_PLUS;
		}
		return result;
	}
}
#else
int ignoreNextKey=0;
//int vid_modenum=0;
int MapKey (int key)
{
	if (ignoreNextKey==-key) {
		return ignoreNextKey=0;
	} 
	if (ignoreNextKey==key) {
		ignoreNextKey=-key;
		return 0;
	}

	if (key==134) ignoreNextKey=13;

	//Here we take into account screen orientation:
	if (!sw_mode->value) return scantokey[key];
	if (sw_mode->value==1) {
		int k=scantokey[key];
		switch (k) {
			case K_LEFTARROW:	return K_UPARROW;
			case K_RIGHTARROW:	return K_DOWNARROW;
			case K_UPARROW:		return K_RIGHTARROW;
			case K_DOWNARROW:	return K_LEFTARROW;
			default:			return k;
		}
	}
	if (sw_mode->value==2) {
		int k=scantokey[key];
		switch (k) {
			case K_LEFTARROW:	return K_DOWNARROW;
			case K_RIGHTARROW:	return K_UPARROW;
			case K_UPARROW:		return K_LEFTARROW;
			case K_DOWNARROW:	return K_RIGHTARROW;
			default:			return k;
		}
	}
	return scantokey[key];
}

int MapToCap (int key)
{
	if (ignoreNextKey==-key) {
		return ignoreNextKey=0;
	} 
	if (ignoreNextKey==key) {
		ignoreNextKey=-key;
		return 0;
	}

	if (key==134) ignoreNextKey=13;

	//Here we take into account screen orientation:
	if (!sw_mode->value) return scantokey[key];
	if (sw_mode->value==1) {
		int k=scantokey[key];
		switch (k) {
			case K_LEFTARROW:	return K_UPARROW;
			case K_RIGHTARROW:	return K_DOWNARROW;
			case K_UPARROW:		return K_RIGHTARROW;
			case K_DOWNARROW:	return K_LEFTARROW;
			default:			return k;
		}
	}
	if (sw_mode->value==2) {
		int k=scantokey[key];
		switch (k) {
			case K_LEFTARROW:	return K_DOWNARROW;
			case K_RIGHTARROW:	return K_UPARROW;
			case K_UPARROW:		return K_LEFTARROW;
			case K_DOWNARROW:	return K_RIGHTARROW;
			default:			return k;
		}
	}
	return scantokey[key];
}

int MapToLower (int key)
{
	if (ignoreNextKey==-key) {
		return ignoreNextKey=0;
	} 
	if (ignoreNextKey==key) {
		ignoreNextKey=-key;
		return 0;
	}

	if (key==134) ignoreNextKey=13;

	//Here we take into account screen orientation:
	if (!sw_mode->value) return scantolower[key];
	if (sw_mode->value==1) {
		int k=scantolower[key];
		switch (k) {
			case K_LEFTARROW:	return K_UPARROW;
			case K_RIGHTARROW:	return K_DOWNARROW;
			case K_UPARROW:		return K_RIGHTARROW;
			case K_DOWNARROW:	return K_LEFTARROW;
			default:			return k;
		}
	}
	if (sw_mode->value==2) {
		int k=scantolower[key];
		switch (k) {
			case K_LEFTARROW:	return K_DOWNARROW;
			case K_RIGHTARROW:	return K_UPARROW;
			case K_UPARROW:		return K_LEFTARROW;
			case K_DOWNARROW:	return K_RIGHTARROW;
			default:			return k;
		}
	}
	return scantolower[key];
}
#endif

void AppActivate(BOOL fActive, BOOL minimize)
{
	Minimized = minimize;

	Key_ClearStates();

	// we don't want to act like we're active if we're minimized
	if (fActive && !Minimized)
		ActiveApp = true;
	else
		ActiveApp = false;

	// minimize/restore mouse-capture on demand
	if (!ActiveApp)
	{
		IN_Activate (false);
		CDAudio_Activate (false);
		S_Activate (false);

		if ( win_noalttab->value )
		{
			WIN_EnableAltTab();
		}
	}
	else
	{
		IN_Activate (true);
		CDAudio_Activate (true);
		S_Activate (true);
		if ( win_noalttab->value )
		{
			WIN_DisableAltTab();
		}
	}
}

/*
====================
MainWndProc

main window procedure
====================
*/
LONG WINAPI MainWndProc (
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam)
{
	LONG			lRet = 0;

	if ( uMsg == MSH_MOUSEWHEEL )
	{
		if ( ( ( int ) wParam ) > 0 )
		{
			Key_Event( K_MWHEELUP, true, sys_msg_time );
			Key_Event( K_MWHEELUP, false, sys_msg_time );
		}
		else
		{
			Key_Event( K_MWHEELDOWN, true, sys_msg_time );
			Key_Event( K_MWHEELDOWN, false, sys_msg_time );
		}
        return DefWindowProc (hWnd, uMsg, wParam, lParam);
	}

	switch (uMsg)
	{
	case WM_MOUSEWHEEL:
		/*
		** this chunk of code theoretically only works under NT4 and Win98
		** since this message doesn't exist under Win95
		*/
		if ( ( short ) HIWORD( wParam ) > 0 )
		{
			Key_Event( K_MWHEELUP, true, sys_msg_time );
			Key_Event( K_MWHEELUP, false, sys_msg_time );
		}
		else
		{
			Key_Event( K_MWHEELDOWN, true, sys_msg_time );
			Key_Event( K_MWHEELDOWN, false, sys_msg_time );
		}
		break;

	case WM_HOTKEY:
		return 0;

	case WM_CREATE: {
		cl_hwnd = hWnd;
#ifndef _WIN32_WCE
		MSH_MOUSEWHEEL = RegisterWindowMessage("MSWHEEL_ROLLMSG"); 
#else
		//Dan East: Don't even need this for CE, but there may be
		//ramifications of the MSH_MOUSEWHEEL message ID not being created.
		MSH_MOUSEWHEEL = RegisterWindowMessage(_T("MSWHEEL_ROLLMSG")); 

		//Load the bitmap skins
		//TODO: allow external skins
		ctrlBmp=LoadBitmap(global_hInstance, MAKEINTRESOURCE(IDB_CONTROLS));
		titlebarBmp=LoadBitmap(global_hInstance, MAKEINTRESOURCE(IDB_TITLEBAR));
#endif
        return DefWindowProc (hWnd, uMsg, wParam, lParam);
#ifdef _WIN32_WCE
		//Dan East: There was no handler for WM_CLOSE. We need to shutdown
		//gracefully so the main game loop will break. Otherwise the main window
		//will close, input will cease, but the game will keep on drawing out to the
		//display.
	}
	case WM_CLOSE:
		Sys_Quit();
		break;
#endif
	case WM_PAINT: {
		cvar_t  *sw_mode;
		sw_mode = Cvar_Get( "sw_mode", "0", CVAR_ARCHIVE );

		SCR_DirtyScreen ();	// force entire screen to update next frame
		if (sw_mode->value==0/*||vid_modenum==NO_MODE*/) {
			HBITMAP oBmp;
			PAINTSTRUCT		ps;
			HDC hdc=BeginPaint(hWnd, &ps);
			HDC bmpDC;

			//TODO: Create  DC for the skin bitmaps once when the bitmaps are loaded.
			bmpDC=CreateCompatibleDC(hdc);
			oBmp=SelectObject(bmpDC, titlebarBmp);
			//Blit the titlebar bitmap
			BitBlt(hdc, 0, 0, 240, 24, bmpDC, 0, 0, SRCCOPY);
/*
			//Update the clock
			vid_clockrect.left=181;
			vid_clockrect.top=3;
			vid_clockrect.right=211;
			vid_clockrect.bottom=21;
			PaintCapTime(hdc, &vid_clockrect);
*/
			SelectObject(bmpDC, ctrlBmp);
			//Blit the control bitmap
			BitBlt(hdc, 0, CONTROL_TOP, 240, 320-CONTROL_TOP, bmpDC, 0, 0, SRCCOPY);

			SelectObject(bmpDC, oBmp);
			DeleteDC(bmpDC);

			//Tell Quake to force an update of the game portion of the screen
/*			if (!in_mode_set && host_initialized)
				SCR_UpdateWholeScreen ();
*/
			EndPaint(hWnd, &ps);
			break;
		} else
			return DefWindowProc (hWnd, uMsg, wParam, lParam);
	}
	case WM_DESTROY:
		// let sound and input know about this?
		cl_hwnd = NULL;
        return DefWindowProc (hWnd, uMsg, wParam, lParam);

	case WM_ACTIVATE:
		{
			int	fActive, fMinimized;

			// KJB: Watch this for problems in fullscreen modes with Alt-tabbing.
			fActive = LOWORD(wParam);
			fMinimized = (BOOL) HIWORD(wParam);
#ifndef _WIN32_WCE
			AppActivate( fActive != WA_INACTIVE, fMinimized);

			if ( reflib_active )
				re.AppActivate( !( fActive == WA_INACTIVE ) );
#else
			if (fActive==WA_INACTIVE) { 
			} else {
				HWND wnd;
				//sndPlaySound(_T("Alarm1.wav"), SND_ASYNC);
				//if (vid_inactivating) return;
				//if ((lParam&&GetParent((HWND)lParam)!=hWnd))
				//	MessageBox(hWnd, _T("Activate"), NULL, MB_OK);
				SHFullScreen(hWnd, SHFS_HIDETASKBAR);
				//The following is to add GigaBar support.  We hide GigaBar if we are full screen
				wnd=FindWindow(_T("GigaBar"), NULL);
				if (wnd) ShowWindow(wnd, SW_HIDE);
				MoveWindow(hWnd, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), TRUE);
			}
#endif
		}
        return DefWindowProc (hWnd, uMsg, wParam, lParam);
#ifndef _WIN32_WCE
	//Dan East: Pocket PC App windows can't be moved
	case WM_MOVE:
		{
			int		xPos, yPos;
			RECT r;
			int		style;

			if (!vid_fullscreen->value)
			{
				xPos = (short) LOWORD(lParam);    // horizontal position 
				yPos = (short) HIWORD(lParam);    // vertical position 

				r.left   = 0;
				r.top    = 0;
				r.right  = 1;
				r.bottom = 1;

				style = GetWindowLong( hWnd, GWL_STYLE );
				AdjustWindowRect( &r, style, FALSE );

				Cvar_SetValue( "vid_xpos", xPos + r.left);
				Cvar_SetValue( "vid_ypos", yPos + r.top);
				vid_xpos->modified = false;
				vid_ypos->modified = false;
				if (ActiveApp)
					IN_Activate (true);
			}
		}
        return DefWindowProc (hWnd, uMsg, wParam, lParam);
#endif
// this is complicated because Win32 seems to pack multiple mouse events into
// one update sometimes, so we always check all states and look for events
	case WM_LBUTTONDOWN:
#ifdef _WIN32_WCE
		IN_CEMouseButton(LOWORD(lParam), HIWORD(lParam), true);
		return 0;
#endif
	case WM_LBUTTONUP:
#ifdef _WIN32_WCE
		IN_CEMouseButton(LOWORD(lParam), HIWORD(lParam), false);
		return 0;
#endif
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
#ifdef _WIN32_WCE
		return DefWindowProc (hWnd, uMsg, wParam, lParam);
#endif
	case WM_MOUSEMOVE:
#ifndef _WIN32_WCE
		{
			int	temp;

			temp = 0;

			if (wParam & MK_LBUTTON)
				temp |= 1;

			if (wParam & MK_RBUTTON)
				temp |= 2;

			if (wParam & MK_MBUTTON)
				temp |= 4;

			IN_MouseEvent (temp);
		}
#else
		IN_CEMouseMove(LOWORD(lParam), HIWORD(lParam));
#endif
		break;
#ifndef _WIN32_WCE
	//Dan East: No screensavers on WinCE
	case WM_SYSCOMMAND:
		if ( wParam == SC_SCREENSAVE )
			return 0;
        return DefWindowProc (hWnd, uMsg, wParam, lParam);
#endif
	case WM_SYSKEYDOWN:
		if ( wParam == 13 )
		{
			if ( vid_fullscreen )
			{
				Cvar_SetValue( "vid_fullscreen", !vid_fullscreen->value );
			}
			return 0;
		}
		// fall through
	case WM_KEYDOWN:
#ifndef _WIN32_WCE
		Key_Event( MapKey( lParam ), true, sys_msg_time);
#else
		//if (!in_mode_set&&!vid_paused) {
			if (GetKeyState(VK_SHIFT)&0x8000||GetKeyState(VK_CAPITAL)&0x0001) {
				if (MapToCap(wParam)) Key_Event (MapToCap(wParam), true, sys_msg_time);
			} else 
				if (MapToLower(wParam)) Key_Event (MapToLower(wParam), true, sys_msg_time);
		//}

#endif
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
#ifndef _WIN32_WCE
		Key_Event( MapKey( lParam ), false, sys_msg_time);
#else
		//if (!in_mode_set&&!vid_paused) {
			if (GetKeyState(VK_SHIFT)&0x8000||GetKeyState(VK_CAPITAL)&0x0001) {
				if (MapToCap(wParam)) Key_Event (MapToCap(wParam), false, sys_msg_time);
			} else 
				if (MapToLower(wParam)) Key_Event (MapToLower(wParam), false, sys_msg_time);
		//}

#endif
		break;
#ifndef _WIN32_WCE
	//Dan East: No CD-Roms on WinCE devices.
	case MM_MCINOTIFY:
		{
			LONG CDAudio_MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			lRet = CDAudio_MessageHandler (hWnd, uMsg, wParam, lParam);
		}
		break;
#endif
	default:	// pass all unhandled messages to DefWindowProc
        return DefWindowProc (hWnd, uMsg, wParam, lParam);
    }

    /* return 0 if handled message, 1 if not */
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

/*
============
VID_Restart_f

Console command to re-start the video mode and refresh DLL. We do this
simply by setting the modified flag for the vid_ref variable, which will
cause the entire video mode and refresh DLL to be reset on the next frame.
============
*/
void VID_Restart_f (void)
{
	vid_ref->modified = true;
}

void VID_Front_f( void )
{
	SetWindowLong( cl_hwnd, GWL_EXSTYLE, WS_EX_TOPMOST );
	SetForegroundWindow( cl_hwnd );
}

/*
** VID_GetModeInfo
*/
typedef struct vidmode_s
{
	const char *description;
	int         width, height;
	int         mode;
} vidmode_t;

vidmode_t vid_modes[] =
{
#ifndef _WIN32_WCE
	{ "Mode 0: 320x240",   320, 240,   0 },
	{ "Mode 1: 400x300",   400, 300,   1 },
	{ "Mode 2: 512x384",   512, 384,   2 },
	{ "Mode 3: 640x480",   640, 480,   3 },
	{ "Mode 4: 800x600",   800, 600,   4 },
	{ "Mode 5: 960x720",   960, 720,   5 },
	{ "Mode 6: 1024x768",  1024, 768,  6 },
	{ "Mode 7: 1152x864",  1152, 864,  7 },
	{ "Mode 8: 1280x960",  1280, 960, 8 },
	{ "Mode 9: 1600x1200", 1600, 1200, 9 }
#else
	{ "Mode 0: 240x180",   240, 180,   0 },
	{ "Mode 1: 320x240 R", 320, 240,   1 },
	{ "Mode 2: 320x240 L", 320, 240,   2 },
#endif
};

qboolean VID_GetModeInfo( int *width, int *height, int mode )
{
	if ( mode < 0 || mode >= VID_NUM_MODES )
		return false;

	*width  = vid_modes[mode].width;
	*height = vid_modes[mode].height;

	return true;
}

/*
** VID_UpdateWindowPosAndSize
*/
void VID_UpdateWindowPosAndSize( int x, int y )
{
#ifndef _WIN32_WCE
	RECT r;
	int		style;
	int		w, h;

	r.left   = 0;
	r.top    = 0;
	r.right  = viddef.width;
	r.bottom = viddef.height;

	style = GetWindowLong( cl_hwnd, GWL_STYLE );
	AdjustWindowRect( &r, style, FALSE );

	w = r.right - r.left;
	h = r.bottom - r.top;

	MoveWindow( cl_hwnd, vid_xpos->value, vid_ypos->value, w, h, TRUE );
#endif
}

/*
** VID_NewWindow
*/
void VID_NewWindow ( int width, int height)
{
	viddef.width  = width;
	viddef.height = height;

	cl.force_refdef = true;		// can't use a paused refdef
#ifdef _WIN32_WCE
	//Dan East:
	//This needs to be called explicitly so the console can adjust to our non-standard
	//display widths, which can be smaller than normal.
	Con_CheckResize();
#endif
}

void VID_FreeReflib (void)
{
	if ( !FreeLibrary( reflib_library ) )
		Com_Error( ERR_FATAL, "Reflib FreeLibrary failed" );
	memset (&re, 0, sizeof(re));
	reflib_library = NULL;
	reflib_active  = false;
}

/*
==============
VID_LoadRefresh
==============
*/
qboolean VID_LoadRefresh( char *name )
{
	refimport_t	ri;
	GetRefAPI_t	GetRefAPI;
#ifdef _WIN32_WCE
	TCHAR name_t[MAX_PATH];
#endif
	
	if ( reflib_active )
	{
		re.Shutdown();
		VID_FreeReflib ();
	}

	Com_Printf( "------- Loading %s -------\n", name );
#ifndef _WIN32_WCE
	if ( ( reflib_library = LoadLibrary( name ) ) == 0 )
#else
	MultiByteToWideChar(CP_ACP, 0, name, -1, name_t, sizeof(name_t)/sizeof(TCHAR));
	if ( ( reflib_library = LoadLibrary( name_t ) ) == 0 )
#endif
	{
		Com_Printf( "LoadLibrary(\"%s\") failed\n", name );

		return false;
	}

	ri.Cmd_AddCommand = Cmd_AddCommand;
	ri.Cmd_RemoveCommand = Cmd_RemoveCommand;
	ri.Cmd_Argc = Cmd_Argc;
	ri.Cmd_Argv = Cmd_Argv;
	ri.Cmd_ExecuteText = Cbuf_ExecuteText;
	ri.Con_Printf = VID_Printf;
	ri.Sys_Error = VID_Error;
	ri.FS_LoadFile = FS_LoadFile;
	ri.FS_FreeFile = FS_FreeFile;
	ri.FS_Gamedir = FS_Gamedir;
	ri.Cvar_Get = Cvar_Get;
	ri.Cvar_Set = Cvar_Set;
	ri.Cvar_SetValue = Cvar_SetValue;
	ri.Vid_GetModeInfo = VID_GetModeInfo;
	ri.Vid_MenuInit = VID_MenuInit;
	ri.Vid_NewWindow = VID_NewWindow;

#ifndef _WIN32_WCE
	if ( ( GetRefAPI = (void *) GetProcAddress( reflib_library, "GetRefAPI" ) ) == 0 )
#else
#ifndef _X86_
	if ( ( GetRefAPI = (void *) GetProcAddress( reflib_library, _T("GetRefAPI") ) ) == 0 )
#else
	if ( ( GetRefAPI = (void *) GetProcAddress( reflib_library, _T("_GetRefAPI@64") ) ) == 0 )
#endif
#endif
		Com_Error( ERR_FATAL, "GetProcAddress failed on %s", name );

	re = GetRefAPI( ri );

	if (re.api_version != API_VERSION)
	{
		VID_FreeReflib ();
		Com_Error (ERR_FATAL, "%s has incompatible api_version", name);
	}

	if ( re.Init( global_hInstance, MainWndProc ) == -1 )
	{
		re.Shutdown();
		VID_FreeReflib ();
		return false;
	}

#ifndef _WIN32_WCE
	Com_Printf( "------------------------------------\n");
#else
	//A few too many hypens for the portrait PQ2 display mode.
	Com_Printf("----------------------------\n");
#endif

	reflib_active = true;

//======
//PGM
	vidref_val = VIDREF_OTHER;
	if(vid_ref)
	{
		if(!strcmp (vid_ref->string, "gl"))
			vidref_val = VIDREF_GL;
		else if(!strcmp(vid_ref->string, "soft"))
			vidref_val = VIDREF_SOFT;
	}
//PGM
//======

	return true;
}

/*
============
VID_CheckChanges

This function gets called once just before drawing each frame, and it's sole purpose in life
is to check to see if any of the video mode parameters have changed, and if they have to 
update the rendering DLL and/or video mode to match.
============
*/
void VID_CheckChanges (void)
{
	char name[100];

	if ( win_noalttab->modified )
	{
		if ( win_noalttab->value )
		{
			WIN_DisableAltTab();
		}
		else
		{
			WIN_EnableAltTab();
		}
		win_noalttab->modified = false;
	}

	if ( vid_ref->modified )
	{
		cl.force_refdef = true;		// can't use a paused refdef
		S_StopAllSounds();
	}
	while (vid_ref->modified)
	{
		/*
		** refresh has changed
		*/
		vid_ref->modified = false;
		vid_fullscreen->modified = true;
		cl.refresh_prepped = false;
		cls.disable_screen = true;

		Com_sprintf( name, sizeof(name), "ref_%s.dll", vid_ref->string );
		if ( !VID_LoadRefresh( name ) )
		{
			if ( strcmp (vid_ref->string, "soft") == 0 )
				Com_Error (ERR_FATAL, "Couldn't fall back to software refresh!");
			Cvar_Set( "vid_ref", "soft" );

			/*
			** drop the console if we fail to load a refresh
			*/
			if ( cls.key_dest != key_console )
			{
				Con_ToggleConsole_f();
			}
		}
		cls.disable_screen = false;
	}

	/*
	** update our window position
	*/
	if ( vid_xpos->modified || vid_ypos->modified )
	{
		if (!vid_fullscreen->value)
			VID_UpdateWindowPosAndSize( vid_xpos->value, vid_ypos->value );

		vid_xpos->modified = false;
		vid_ypos->modified = false;
	}
}

/*
============
VID_Init
============
*/
void VID_Init (void)
{
	/* Create the video variables so we know how to start the graphics drivers */
	vid_ref = Cvar_Get ("vid_ref", "soft", CVAR_ARCHIVE);
	vid_xpos = Cvar_Get ("vid_xpos", "3", CVAR_ARCHIVE);
	vid_ypos = Cvar_Get ("vid_ypos", "22", CVAR_ARCHIVE);
	vid_fullscreen = Cvar_Get ("vid_fullscreen", "0", CVAR_ARCHIVE);
	vid_gamma = Cvar_Get( "vid_gamma", "1", CVAR_ARCHIVE );
	win_noalttab = Cvar_Get( "win_noalttab", "0", CVAR_ARCHIVE );
#ifdef _WIN32_WCE
	sw_mode = Cvar_Get( "sw_mode", "0", CVAR_ARCHIVE );
#endif

	/* Add some console commands that we want to handle */
	Cmd_AddCommand ("vid_restart", VID_Restart_f);
	Cmd_AddCommand ("vid_front", VID_Front_f);

	/*
	** this is a gross hack but necessary to clamp the mode for 3Dfx
	*/
#if 0
	{
		cvar_t *gl_driver = Cvar_Get( "gl_driver", "opengl32", 0 );
		cvar_t *gl_mode = Cvar_Get( "gl_mode", "3", 0 );

		if ( stricmp( gl_driver->string, "3dfxgl" ) == 0 )
		{
			Cvar_SetValue( "gl_mode", 3 );
			viddef.width  = 640;
			viddef.height = 480;
		}
	}
#endif

#ifndef _WIN32_WCE
	/* Disable the 3Dfx splash screen */
	putenv("FX_GLIDE_NO_SPLASH=0");
#endif
	
	/* Start the graphics mode and load refresh DLL */
	VID_CheckChanges();
}

/*
============
VID_Shutdown
============
*/
void VID_Shutdown (void)
{
	if ( reflib_active )
	{
		re.Shutdown ();
		VID_FreeReflib ();
	}
}


