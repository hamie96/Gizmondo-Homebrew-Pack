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
// sys_win.h

#include "../qcommon/qcommon.h"
#include "winquake.h"
#include "resource.h"
#ifndef _WIN32_WCE
#include <errno.h>
#include <float.h>
#include <fcntl.h>
#endif
#include <stdio.h>
#ifndef _WIN32_WCE
#include <direct.h>
#include <io.h>
#include <conio.h>
#endif
#include "../win32/conproc.h"

#ifndef _WIN32_WCE
#define MINIMUM_WIN_MEMORY	0x0a00000
#else
#define MINIMUM_WIN_MEMORY	0x0700000
#endif
#define MAXIMUM_WIN_MEMORY	0x1000000

//#define DEMO

qboolean s_win95;

int			starttime;
int			ActiveApp;
qboolean	Minimized;

static HANDLE		hinput, houtput;

unsigned	sys_msg_time;
unsigned	sys_frame_time;


static HANDLE		qwclsemaphore;

#define	MAX_NUM_ARGVS	128
int			argc;
char		*argv[MAX_NUM_ARGVS];


/*
===============================================================================

SYSTEM IO

===============================================================================
*/


void Sys_Error (char *error, ...)
{
	va_list		argptr;
	char		text[1024];

	CL_Shutdown ();
	Qcommon_Shutdown ();

	va_start (argptr, error);
	vsprintf (text, error, argptr);
	va_end (argptr);

#ifndef _WIN32_WCE
	MessageBox(NULL, text, "Error", 0 /* MB_OK */ );
#else
	{
		TCHAR text_t[1024];
		MSG msg;
		while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage (&msg);
   			DispatchMessage (&msg);
		}

		MultiByteToWideChar(CP_ACP, 0, text, -1, text_t, sizeof(text_t)/sizeof(TCHAR));
		MessageBox(NULL, text_t, _T("Error"), 0 /* MB_OK */ );
	}
#endif

	if (qwclsemaphore)
		CloseHandle (qwclsemaphore);

// shut down QHOST hooks if necessary
#ifndef _WIN32_WCE
	DeinitConProc ();
#endif

	exit (1);
}

void Sys_Quit (void)
{
#ifndef _WIN32_WCE
	//Dan East: Not needed for CE since we use GetTickCount for timing.
	timeEndPeriod( 1 );
#endif

	CL_Shutdown();
	Qcommon_Shutdown ();
	CloseHandle (qwclsemaphore);
#ifndef _WIN32_WCE
	if (dedicated && dedicated->value)
		FreeConsole ();
#endif

// shut down QHOST hooks if necessary
#ifndef _WIN32_WCE
	DeinitConProc ();
#endif

	exit (0);
}


void WinError (void)
{
	LPVOID lpMsgBuf;

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	// Display the string.
#ifndef _WIN32_WCE
	MessageBox( NULL, lpMsgBuf, "GetLastError", MB_OK|MB_ICONINFORMATION );
#else
	{
		TCHAR msgbuf_t[2048];
		MultiByteToWideChar(CP_ACP, 0, lpMsgBuf, -1, msgbuf_t, sizeof(msgbuf_t)/sizeof(TCHAR));
		MessageBox( NULL, msgbuf_t, _T("GetLastError"), MB_OK|MB_ICONINFORMATION );
	}
#endif

	// Free the buffer.
	LocalFree( lpMsgBuf );
}

//================================================================


/*
================
Sys_ScanForCD

================
*/
char *Sys_ScanForCD (void)
{
#ifndef _WIN32_WCE
	static char	cddir[MAX_OSPATH];
	static qboolean	done;
#ifndef DEMO
	char		drive[4];
	FILE		*f;
	char		test[MAX_QPATH];

	if (done)		// don't re-check
		return cddir;

	// no abort/retry/fail errors
	SetErrorMode (SEM_FAILCRITICALERRORS);

	drive[0] = 'c';
	drive[1] = ':';
	drive[2] = '\\';
	drive[3] = 0;

	done = true;

	// scan the drives
	for (drive[0] = 'c' ; drive[0] <= 'z' ; drive[0]++)
	{
		// where activision put the stuff...
		sprintf (cddir, "%sinstall\\data", drive);
		sprintf (test, "%sinstall\\data\\quake2.exe", drive);
		f = fopen(test, "r");
		if (f)
		{
			fclose (f);
			if (GetDriveType (drive) == DRIVE_CDROM)
				return cddir;
		}
	}
#endif

	cddir[0] = 0;
#endif
	return NULL;
}

/*
================
Sys_CopyProtect

================
*/
void	Sys_CopyProtect (void)
{
#ifndef DEMO
	char	*cddir;

	cddir = Sys_ScanForCD();
	if (!cddir[0])
		Com_Error (ERR_FATAL, "You must have the Quake2 CD in the drive to play.");
#endif
}


//================================================================


/*
================
Sys_Init
================
*/
void Sys_Init (void)
{
#ifndef _WIN32_WCE
	OSVERSIONINFO	vinfo;
#endif
#if 0
	// allocate a named semaphore on the client so the
	// front end can tell if it is alive

	// mutex will fail if semephore already exists
    qwclsemaphore = CreateMutex(
        NULL,         /* Security attributes */
        0,            /* owner       */
        "qwcl"); /* Semaphore name      */
	if (!qwclsemaphore)
		Sys_Error ("QWCL is already running on this system");
	CloseHandle (qwclsemaphore);

    qwclsemaphore = CreateSemaphore(
        NULL,         /* Security attributes */
        0,            /* Initial count       */
        1,            /* Maximum count       */
        "qwcl"); /* Semaphore name      */
#endif
#ifndef _WIN32_WCE
	//Dan East: Don't need for WinCE because we use GetTickCount for timing.
	timeBeginPeriod( 1 );
	vinfo.dwOSVersionInfoSize = sizeof(vinfo);

	if (!GetVersionEx (&vinfo))
		Sys_Error ("Couldn't get OS info");

	if (vinfo.dwMajorVersion < 4)
		Sys_Error ("Quake2 requires windows version 4 or greater");
	if (vinfo.dwPlatformId == VER_PLATFORM_WIN32s)
		Sys_Error ("Quake2 doesn't run on Win32s");
	else if ( vinfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
		s_win95 = true;
#endif

#ifndef _WIN32_WCE
	if (dedicated->value)
	{
		if (!AllocConsole ())
			Sys_Error ("Couldn't create dedicated server console");
		hinput = GetStdHandle (STD_INPUT_HANDLE);
		houtput = GetStdHandle (STD_OUTPUT_HANDLE);
	
		// let QHOST hook in
		InitConProc (argc, argv);
	}
#endif
}


static char	console_text[256];
static int	console_textlen;

/*
================
Sys_ConsoleInput
================
*/
char *Sys_ConsoleInput (void)
{
#ifndef _WIN32_WCE
	INPUT_RECORD	recs[1024];
	int		dummy;
	int		ch, numread, numevents;

	if (!dedicated || !dedicated->value)
		return NULL;


	for ( ;; )
	{
		if (!GetNumberOfConsoleInputEvents (hinput, &numevents))
			Sys_Error ("Error getting # of console events");

		if (numevents <= 0)
			break;

		if (!ReadConsoleInput(hinput, recs, 1, &numread))
			Sys_Error ("Error reading console input");

		if (numread != 1)
			Sys_Error ("Couldn't read console input");

		if (recs[0].EventType == KEY_EVENT)
		{
			if (!recs[0].Event.KeyEvent.bKeyDown)
			{
				ch = recs[0].Event.KeyEvent.uChar.AsciiChar;

				switch (ch)
				{
					case '\r':
						WriteFile(houtput, "\r\n", 2, &dummy, NULL);	

						if (console_textlen)
						{
							console_text[console_textlen] = 0;
							console_textlen = 0;
							return console_text;
						}
						break;

					case '\b':
						if (console_textlen)
						{
							console_textlen--;
							WriteFile(houtput, "\b \b", 3, &dummy, NULL);	
						}
						break;

					default:
						if (ch >= ' ')
						{
							if (console_textlen < sizeof(console_text)-2)
							{
								WriteFile(houtput, &ch, 1, &dummy, NULL);	
								console_text[console_textlen] = ch;
								console_textlen++;
							}
						}

						break;

				}
			}
		}
	}
#endif
	return NULL;
}


/*
================
Sys_ConsoleOutput

Print text to the dedicated console
================
*/
void Sys_ConsoleOutput (char *string)
{
#ifndef _WIN32_WCE
	int		dummy;
	char	text[256];

	if (!dedicated || !dedicated->value)
		return;

	if (console_textlen)
	{
		text[0] = '\r';
		memset(&text[1], ' ', console_textlen);
		text[console_textlen+1] = '\r';
		text[console_textlen+2] = 0;
		WriteFile(houtput, text, console_textlen+2, &dummy, NULL);
	}

	WriteFile(houtput, string, strlen(string), &dummy, NULL);

	if (console_textlen)
		WriteFile(houtput, console_text, console_textlen, &dummy, NULL);
#endif
}


/*
================
Sys_SendKeyEvents

Send Key_Event calls
================
*/
void Sys_SendKeyEvents (void)
{
    MSG        msg;
#ifndef _WIN32_WCE
	while (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if (!GetMessage (&msg, NULL, 0, 0))
			Sys_Quit ();
#else
	while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
	{
#endif
		sys_msg_time = msg.time;
      	TranslateMessage (&msg);
      	DispatchMessage (&msg);
#ifdef _WIN32_WCE
		if (msg.message==WM_QUIT)
			Sys_Quit();
#endif
	}

	// grab frame time 
#ifndef _WIN32_WCE
	sys_frame_time = timeGetTime();	// FIXME: should this be at start?
#else
	sys_frame_time = GetTickCount();	// FIXME: should this be at start?
#endif
}



/*
================
Sys_GetClipboardData

================
*/
char *Sys_GetClipboardData( void )
{
	char *data = NULL;
	char *cliptext;

	if ( OpenClipboard( NULL ) != 0 )
	{
		HANDLE hClipboardData;

		if ( ( hClipboardData = GetClipboardData( CF_TEXT ) ) != 0 )
		{
			if ( ( cliptext = GlobalLock( hClipboardData ) ) != 0 ) 
			{
				data = malloc( GlobalSize( hClipboardData ) + 1 );
				strcpy( data, cliptext );
				GlobalUnlock( hClipboardData );
			}
		}
		CloseClipboard();
	}
	return data;
}

/*
==============================================================================

 WINDOWS CRAP

==============================================================================
*/

/*
=================
Sys_AppActivate
=================
*/
void Sys_AppActivate (void)
{
	ShowWindow ( cl_hwnd, SW_RESTORE);
	SetForegroundWindow ( cl_hwnd );
}

/*
========================================================================

GAME DLL

========================================================================
*/

static HINSTANCE	game_library;

/*
=================
Sys_UnloadGame
=================
*/
void Sys_UnloadGame (void)
{
	if (!FreeLibrary (game_library))
		Com_Error (ERR_FATAL, "FreeLibrary failed for game library");
	game_library = NULL;
}

/*
=================
Sys_GetGameAPI

Loads the game dll
=================
*/
void *Sys_GetGameAPI (void *parms)
{
	void	*(*GetGameAPI) (void *);
	char	name[MAX_OSPATH];
	char	*path;
#ifndef _WIN32_WCE
	char	cwd[MAX_OSPATH];
#endif
#ifndef _WIN32_WCE
#if defined _M_IX86
	const char *gamename = "gamex86.dll";

#ifdef NDEBUG
	const char *debugdir = "release";
#else
	const char *debugdir = "debug";
#endif

#elif defined _M_ALPHA
	const char *gamename = "gameaxp.dll";

#ifdef NDEBUG
	const char *debugdir = "releaseaxp";
#else
	const char *debugdir = "debugaxp";
#endif
#endif
#else
	const char *gamename = "game.dll";
#endif

	if (game_library)
		Com_Error (ERR_FATAL, "Sys_GetGameAPI without Sys_UnloadingGame");
#ifndef _WIN32_WCE
	// check the current debug directory first for development purposes
	_getcwd (cwd, sizeof(cwd));
	Com_sprintf (name, sizeof(name), "%s/%s/%s", cwd, debugdir, gamename);
	game_library = LoadLibrary ( name );
#endif
	if (game_library)
	{
		Com_DPrintf ("LoadLibrary (%s)\n", name);
	}
	else
	{
		// check the current directory for other development purposes
#ifndef _WIN32_WCE
		Com_sprintf (name, sizeof(name), "%s/%s", cwd, gamename);
		game_library = LoadLibrary ( name );
#else
		{
			TCHAR name_t[MAX_PATH];
			MultiByteToWideChar(CP_ACP, 0, gamename, -1, name_t, sizeof(name_t)/sizeof(TCHAR));
			game_library = LoadLibrary ( name_t );
		}
#endif
		if (game_library)
		{
			Com_DPrintf ("LoadLibrary (%s)\n", name);
		}
		else
		{
			// now run through the search paths
			path = NULL;
			while (1)
			{
				path = FS_NextPath (path);
				if (!path)
					return NULL;		// couldn't find one anywhere
				Com_sprintf (name, sizeof(name), "%s/%s", path, gamename);
#ifndef _WIN32_WCE
				game_library = LoadLibrary (name);
#else
				{
					TCHAR name_t[MAX_PATH];
					MultiByteToWideChar(CP_ACP, 0, name, -1, name_t, sizeof(name_t)/sizeof(TCHAR));
					game_library = LoadLibrary ( name_t );
				}
#endif
				if (game_library)
				{
					Com_DPrintf ("LoadLibrary (%s)\n",name);
					break;
				}
			}
		}
	}
#ifndef _WIN32_WCE
	GetGameAPI = (void *)GetProcAddress (game_library, "GetGameAPI");
#else
#ifdef _X86_
	GetGameAPI = (void *)GetProcAddress (game_library, _T("_GetGameAPI@4"));
#else
	GetGameAPI = (void *)GetProcAddress (game_library, _T("GetGameAPI"));
#endif
#endif
	if (!GetGameAPI)
	{
		Sys_UnloadGame ();		
		return NULL;
	}

	return GetGameAPI (parms);
}

//=======================================================================


/*
==================
ParseCommandLine

==================
*/
void ParseCommandLine (LPSTR lpCmdLine)
{
	argc = 1;
	argv[0] = "exe";

	while (*lpCmdLine && (argc < MAX_NUM_ARGVS))
	{
		while (*lpCmdLine && ((*lpCmdLine <= 32) || (*lpCmdLine > 126)))
			lpCmdLine++;

		if (*lpCmdLine)
		{
			argv[argc] = lpCmdLine;
			argc++;

			while (*lpCmdLine && ((*lpCmdLine > 32) && (*lpCmdLine <= 126)))
				lpCmdLine++;

			if (*lpCmdLine)
			{
				*lpCmdLine = 0;
				lpCmdLine++;
			}
			
		}
	}

}

/*
==================
WinMain

==================
*/
HINSTANCE	global_hInstance;
#ifndef _WIN32_WCE
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
#endif
{
    MSG				msg;
	int				time, oldtime, newtime;
	char			*cddir;
#ifdef _WIN32_WCE
	HWND hWnd;
	TCHAR buf[1024];
#endif

    /* previous instances do not exist in Win32 */
    if (hPrevInstance)
        return 0;

#ifdef _WIN32_WCE
	//If it is already running, then focus on the window
	hWnd = FindWindow(_T("Quake 2"), NULL);
	if (hWnd) 
	{
		SetForegroundWindow ((HWND) (((DWORD)hWnd) | 0x01));    
		return 0;
	} 

	_stprintf(buf, _T("Pocket Quake 2 v%0.2f\nquake.pocketmatrix.com\n\nPocket PC Port: Dan East\nquake@dex-plor.com\n\nSkins: Randall Schleufer"), VERSION);
	MessageBox(NULL, buf, _T("Pocket Quake 2"), MB_OK);
	while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage (&msg);
   		DispatchMessage (&msg);
	}

#endif

	global_hInstance = hInstance;

#ifndef _WIN32_WCE
	ParseCommandLine (lpCmdLine);
#else
	{
		//Dan East: Assume 1024 chars is long enough for command line.  I think WinCE only
		//supports 256 in the first place (at least for shortcuts).
		char cmdLine_a[1024];
		WideCharToMultiByte(CP_ACP, 0, lpCmdLine, -1, cmdLine_a, sizeof(cmdLine_a), NULL, NULL);
		ParseCommandLine (cmdLine_a);
	}
#endif

	// if we find the CD, add a +set cddir xxx command line
	cddir = Sys_ScanForCD ();
	if (cddir && argc < MAX_NUM_ARGVS - 3)
	{
		int		i;

		// don't override a cddir on the command line
		for (i=0 ; i<argc ; i++)
			if (!strcmp(argv[i], "cddir"))
				break;
		if (i == argc)
		{
			argv[argc++] = "+set";
			argv[argc++] = "cddir";
			argv[argc++] = cddir;
		}
	}

	Qcommon_Init (argc, argv);
	oldtime = Sys_Milliseconds ();

    /* main window message loop */
	while (1)
	{
		// if at a full screen console, don't update unless needed
		if (Minimized || (dedicated && dedicated->value) )
		{
			Sleep (1);
		}
#ifndef _WIN32_WCE
		while (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage (&msg, NULL, 0, 0))
				Com_Quit ();
#else
		while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
		{
#endif
			sys_msg_time = msg.time;
			TranslateMessage (&msg);
   			DispatchMessage (&msg);
#ifdef _WIN32_WCE
			if (msg.message==WM_QUIT) {
				Com_Quit ();
				return FALSE;
			}
#endif
		}
#ifndef _WIN32_WCE
		do
		{
#endif
			newtime = Sys_Milliseconds ();
			time = newtime - oldtime;
#ifndef _WIN32_WCE
		} while (time < 1);
#endif
//			Con_Printf ("time:%5.2f - %5.2f = %5.2f\n", newtime, oldtime, time);

		//	_controlfp( ~( _EM_ZERODIVIDE /*| _EM_INVALID*/ ), _MCW_EM );
#ifndef _WIN32_WCE
		_controlfp( _PC_24, _MCW_PC );
#endif
		Qcommon_Frame (time);

		oldtime = newtime;
	}

	// never gets here
    return TRUE;
}
