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

#include "../qcommon/qcommon.h"
#include "winquake.h"
#ifndef _WIN32_WCE
#include <errno.h>
#include <fcntl.h>
#endif
#include <stdio.h>
#ifndef _WIN32_WCE
#include <direct.h>
#include <io.h>
#include <conio.h>
#endif
//===============================================================================

int		hunkcount;


byte	*membase;
int		hunkmaxsize;
int		cursize;

#define	VIRTUAL_ALLOC

void *Hunk_Begin (int maxsize)
{
	// reserve a huge chunk of memory, but don't commit any yet
	cursize = 0;
	hunkmaxsize = maxsize;
#ifdef VIRTUAL_ALLOC
	membase = VirtualAlloc (NULL, maxsize, MEM_RESERVE, PAGE_NOACCESS);
#else
	membase = malloc (maxsize);
	memset (membase, 0, maxsize);
#endif
	if (!membase)
		Sys_Error ("VirtualAlloc reserve failed");
	return (void *)membase;
}

void *Hunk_Alloc (int size)
{
	void	*buf;

	// round to cacheline
	size = (size+31)&~31;

#ifdef VIRTUAL_ALLOC
	// commit pages as needed
//	buf = VirtualAlloc (membase+cursize, size, MEM_COMMIT, PAGE_READWRITE);
	buf = VirtualAlloc (membase, cursize+size, MEM_COMMIT, PAGE_READWRITE);
	if (!buf)
	{
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &buf, 0, NULL);
		Sys_Error ("VirtualAlloc commit failed.\n%s", buf);
	}
#endif
	cursize += size;
	if (cursize > hunkmaxsize)
		Sys_Error ("Hunk_Alloc overflow");

	return (void *)(membase+cursize-size);
}

int Hunk_End (void)
{

	// free the remaining unused virtual memory
#if 0
	void	*buf;

	// write protect it
	buf = VirtualAlloc (membase, cursize, MEM_COMMIT, PAGE_READONLY);
	if (!buf)
		Sys_Error ("VirtualAlloc commit failed");
#endif

	hunkcount++;
//Com_Printf ("hunkcount: %i\n", hunkcount);
	return cursize;
}

void Hunk_Free (void *base)
{
	if ( base )
#ifdef VIRTUAL_ALLOC
		VirtualFree (base, 0, MEM_RELEASE);
#else
		free (base);
#endif

	hunkcount--;
}

//===============================================================================


/*
================
Sys_Milliseconds
================
*/
int	curtime;
int Sys_Milliseconds (void)
{
	static int		base;
	static qboolean	initialized = false;

	if (!initialized)
	{	// let base retain 16 bits of effectively random data
#ifndef _WIN32_WCE
		base = timeGetTime() & 0xffff0000;
#else
		//Dan East: We'll use GetTickCount for Windows CE
		base = GetTickCount() & 0xffff0000;
#endif
		initialized = true;
	}
#ifndef _WIN32_WCE
	curtime = timeGetTime() - base;
#else
	curtime = GetTickCount() - base;
#endif

	return curtime;
}

void Sys_Mkdir (char *path)
{
#ifndef _WIN32_WCE
	_mkdir (path);
#else
	TCHAR path_w[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, path, -1, path_w, sizeof(path_w)/sizeof(TCHAR));
	CreateDirectory(path_w, NULL);
#endif
}

//============================================
#ifndef _WIN32_WCE
char	findbase[MAX_OSPATH];
char	findpath[MAX_OSPATH];
int		findhandle;

static qboolean CompareAttributes( unsigned found, unsigned musthave, unsigned canthave )
{
	if ( ( found & _A_RDONLY ) && ( canthave & SFF_RDONLY ) )
		return false;
	if ( ( found & _A_HIDDEN ) && ( canthave & SFF_HIDDEN ) )
		return false;
	if ( ( found & _A_SYSTEM ) && ( canthave & SFF_SYSTEM ) )
		return false;
	if ( ( found & _A_SUBDIR ) && ( canthave & SFF_SUBDIR ) )
		return false;
	if ( ( found & _A_ARCH ) && ( canthave & SFF_ARCH ) )
		return false;

	if ( ( musthave & SFF_RDONLY ) && !( found & _A_RDONLY ) )
		return false;
	if ( ( musthave & SFF_HIDDEN ) && !( found & _A_HIDDEN ) )
		return false;
	if ( ( musthave & SFF_SYSTEM ) && !( found & _A_SYSTEM ) )
		return false;
	if ( ( musthave & SFF_SUBDIR ) && !( found & _A_SUBDIR ) )
		return false;
	if ( ( musthave & SFF_ARCH ) && !( found & _A_ARCH ) )
		return false;

	return true;
}

char *Sys_FindFirst (char *path, unsigned musthave, unsigned canthave )
{
	struct _finddata_t findinfo;

	if (findhandle)
		Sys_Error ("Sys_BeginFind without close");
	findhandle = 0;

	COM_FilePath (path, findbase);
	findhandle = _findfirst (path, &findinfo);
	if (findhandle == -1)
		return NULL;
	if ( !CompareAttributes( findinfo.attrib, musthave, canthave ) )
		return NULL;
	Com_sprintf (findpath, sizeof(findpath), "%s/%s", findbase, findinfo.name);
	return findpath;
}

char *Sys_FindNext ( unsigned musthave, unsigned canthave )
{
	struct _finddata_t findinfo;

	if (findhandle == -1)
		return NULL;
	if (_findnext (findhandle, &findinfo) == -1)
		return NULL;
	if ( !CompareAttributes( findinfo.attrib, musthave, canthave ) )
		return NULL;

	Com_sprintf (findpath, sizeof(findpath), "%s/%s", findbase, findinfo.name);
	return findpath;
}

void Sys_FindClose (void)
{
	if (findhandle != -1)
		_findclose (findhandle);
	findhandle = 0;
}
#else
//Dan East: We use the FindFirstFile windows API for file searching with CE,
//plus obligatory UNICODE conversions and Win32 file attributes.
char	findbase[MAX_OSPATH];
char	findpath[MAX_OSPATH];
HANDLE	findhandle=INVALID_HANDLE_VALUE;

static qboolean CompareAttributes( unsigned found, unsigned musthave, unsigned canthave )
{
	if ( ( found & FILE_ATTRIBUTE_READONLY ) && ( canthave & SFF_RDONLY ) )
		return false;
	if ( ( found & FILE_ATTRIBUTE_HIDDEN ) && ( canthave & SFF_HIDDEN ) )
		return false;
	if ( ( found & FILE_ATTRIBUTE_SYSTEM ) && ( canthave & SFF_SYSTEM ) )
		return false;
	if ( ( found & FILE_ATTRIBUTE_DIRECTORY ) && ( canthave & SFF_SUBDIR ) )
		return false;
	if ( ( found & FILE_ATTRIBUTE_ARCHIVE ) && ( canthave & SFF_ARCH ) )
		return false;

	if ( ( musthave & SFF_RDONLY ) && !( found & FILE_ATTRIBUTE_READONLY ) )
		return false;
	if ( ( musthave & SFF_HIDDEN ) && !( found & FILE_ATTRIBUTE_HIDDEN ) )
		return false;
	if ( ( musthave & SFF_SYSTEM ) && !( found & FILE_ATTRIBUTE_SYSTEM ) )
		return false;
	if ( ( musthave & SFF_SUBDIR ) && !( found & FILE_ATTRIBUTE_DIRECTORY ) )
		return false;
	if ( ( musthave & SFF_ARCH ) && !( found & FILE_ATTRIBUTE_ARCHIVE ) )
		return false;

	return true;
}

char *Sys_FindFirst (char *path, unsigned musthave, unsigned canthave )
{
	WIN32_FIND_DATA ffd;
	TCHAR path_w[MAX_PATH];
	char fname[MAX_PATH];

	MultiByteToWideChar(CP_ACP, 0, path, -1, path_w, sizeof(path_w)/sizeof(TCHAR));

	if (findhandle!=INVALID_HANDLE_VALUE)
		Sys_Error ("Sys_BeginFind without close");

	COM_FilePath (path, findbase);
	
	findhandle=FindFirstFile(path_w, &ffd);
	
	if (findhandle == INVALID_HANDLE_VALUE)
		return NULL;

	if ( !CompareAttributes( ffd.dwFileAttributes, musthave, canthave ) )
		return NULL;

	WideCharToMultiByte(CP_ACP, 0, ffd.cFileName, -1, fname, sizeof(fname), NULL, NULL);

	Com_sprintf (findpath, sizeof(findpath), "%s/%s", findbase, fname);
	return findpath;
}

char *Sys_FindNext ( unsigned musthave, unsigned canthave )
{
	WIN32_FIND_DATA ffd;
	char fname[MAX_PATH];

	if (findhandle == INVALID_HANDLE_VALUE)
		return NULL;
	if (!FindNextFile(findhandle, &ffd))
		return NULL;
	if ( !CompareAttributes( ffd.dwFileAttributes, musthave, canthave ) )
		return NULL;

	WideCharToMultiByte(CP_ACP, 0, ffd.cFileName, -1, fname, sizeof(fname), NULL, NULL);

	Com_sprintf (findpath, sizeof(findpath), "%s/%s", findbase, fname);
	return findpath;
}

void Sys_FindClose (void)
{
	if (findhandle != INVALID_HANDLE_VALUE)
		FindClose(findhandle);
	findhandle = INVALID_HANDLE_VALUE;
}


#endif

//============================================

