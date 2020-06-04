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
// in_win.c -- windows 95 mouse and joystick code
// 02/21/97 JCB Added extended DirectInput code to support external controllers.

#include "../client/client.h"
#include "winquake.h"

#ifdef _WIN32_WCE
#include <gx.h>
#include <Aygshell.h>
#endif

extern	unsigned	sys_msg_time;
cvar_t  *sw_mode;

// joystick defines and variables
// where should defines be moved?
#define JOY_ABSOLUTE_AXIS	0x00000000		// control like a joystick
#define JOY_RELATIVE_AXIS	0x00000010		// control like a mouse, spinner, trackball
#define	JOY_MAX_AXES		6				// X, Y, Z, R, U, V
#define JOY_AXIS_X			0
#define JOY_AXIS_Y			1
#define JOY_AXIS_Z			2
#define JOY_AXIS_R			3
#define JOY_AXIS_U			4
#define JOY_AXIS_V			5

enum _ControlList
{
	AxisNada = 0, AxisForward, AxisLook, AxisSide, AxisTurn, AxisUp
};

#ifndef _WIN32_WCE
DWORD	dwAxisFlags[JOY_MAX_AXES] =
{
	JOY_RETURNX, JOY_RETURNY, JOY_RETURNZ, JOY_RETURNR, JOY_RETURNU, JOY_RETURNV
};
#endif

DWORD	dwAxisMap[JOY_MAX_AXES];
DWORD	dwControlMap[JOY_MAX_AXES];
PDWORD	pdwRawValue[JOY_MAX_AXES];

cvar_t	*in_mouse;
cvar_t	*in_joystick;
#ifdef _WIN32_WCE
//Dan East: Var addition for CE. This allows the yaw to be automatically
//incremented when the stylus reaches the very edges of the screen when
//mlooking.
cvar_t	*in_autoyawrate;
#endif

// none of these cvars are saved over a session
// this means that advanced controller configuration needs to be executed
// each time.  this avoids any problems with getting back to a default usage
// or when changing from one controller to another.  this way at least something
// works.
cvar_t	*joy_name;
cvar_t	*joy_advanced;
cvar_t	*joy_advaxisx;
cvar_t	*joy_advaxisy;
cvar_t	*joy_advaxisz;
cvar_t	*joy_advaxisr;
cvar_t	*joy_advaxisu;
cvar_t	*joy_advaxisv;
cvar_t	*joy_forwardthreshold;
cvar_t	*joy_sidethreshold;
cvar_t	*joy_pitchthreshold;
cvar_t	*joy_yawthreshold;
cvar_t	*joy_forwardsensitivity;
cvar_t	*joy_sidesensitivity;
cvar_t	*joy_pitchsensitivity;
cvar_t	*joy_yawsensitivity;
cvar_t	*joy_upthreshold;
cvar_t	*joy_upsensitivity;

qboolean	joy_avail, joy_advancedinit, joy_haspov;
DWORD		joy_oldbuttonstate, joy_oldpovstate;

int			joy_id;
DWORD		joy_flags;
DWORD		joy_numbuttons;

#ifndef _WIN32_WCE
static JOYINFOEX	ji;
#endif

qboolean	in_appactive;

// forward-referenced functions
void IN_StartupJoystick (void);
void Joy_AdvancedUpdate_f (void);
void IN_JoyMove (usercmd_t *cmd);

/*
============================================================

  MOUSE CONTROL

============================================================
*/

// mouse variables
cvar_t	*m_filter;

qboolean	mlooking;

void IN_MLookDown (void) { mlooking = true; }
void IN_MLookUp (void) {
mlooking = false;
if (!freelook->value && lookspring->value)
		IN_CenterView ();
}

int			mouse_buttons;
int			mouse_oldbuttonstate;
POINT		current_pos;
#ifdef _WIN32_WCE
POINT		start_pos;
#endif
int			mouse_x, mouse_y, old_mouse_x, old_mouse_y, mx_accum, my_accum;

int			old_x, old_y;

qboolean	mouseactive;	// false when not focus app

qboolean	restore_spi;
qboolean	mouseinitialized;
int		originalmouseparms[3], newmouseparms[3] = {0, 0, 1};
qboolean	mouseparmsvalid;

int			window_center_x, window_center_y;
RECT		window_rect;


/*
===========
IN_ActivateMouse

Called when the window gains focus or changes in some way
===========
*/
void IN_ActivateMouse (void)
{
	int		width, height;

	if (!mouseinitialized)
		return;
#ifndef _WIN32_WCE
	if (!in_mouse->value)
	{
		mouseactive = false;
		return;
	}
	if (mouseactive)
		return;
#endif
	mouseactive = true;

	if (mouseparmsvalid)
		restore_spi = SystemParametersInfo (SPI_SETMOUSE, 0, newmouseparms, 0);

	width = GetSystemMetrics (SM_CXSCREEN);
	height = GetSystemMetrics (SM_CYSCREEN);

	GetWindowRect ( cl_hwnd, &window_rect);
	if (window_rect.left < 0)
		window_rect.left = 0;
	if (window_rect.top < 0)
		window_rect.top = 0;
	if (window_rect.right >= width)
		window_rect.right = width-1;
	if (window_rect.bottom >= height-1)
		window_rect.bottom = height-1;

	window_center_x = (window_rect.right + window_rect.left)/2;
	window_center_y = (window_rect.top + window_rect.bottom)/2;

	SetCursorPos (window_center_x, window_center_y);

	old_x = window_center_x;
	old_y = window_center_y;

	SetCapture ( cl_hwnd );
	ClipCursor (&window_rect);
#ifndef _WIN32_WCE
	while (ShowCursor (FALSE) >= 0)
		;
#endif
}


/*
===========
IN_DeactivateMouse

Called when the window loses focus
===========
*/
void IN_DeactivateMouse (void)
{
	if (!mouseinitialized)
		return;
	if (!mouseactive)
		return;

	if (restore_spi)
		SystemParametersInfo (SPI_SETMOUSE, 0, originalmouseparms, 0);

	mouseactive = false;

	ClipCursor (NULL);
	ReleaseCapture ();
#ifndef _WIN32_WCE
	while (ShowCursor (TRUE) < 0)
		;
#endif
}



/*
===========
IN_StartupMouse
===========
*/
void IN_StartupMouse (void)
{
	cvar_t		*cv;

	cv = Cvar_Get ("in_initmouse", "1", CVAR_NOSET);
	if ( !cv->value ) 
		return; 

	mouseinitialized = true;
	mouseparmsvalid = SystemParametersInfo (SPI_GETMOUSE, 0, originalmouseparms, 0);
	mouse_buttons = 3;
}

/*
===========
IN_MouseEvent
===========
*/
void IN_MouseEvent (int mstate)
{
	int		i;

	if (!mouseinitialized)
		return;

// perform button actions
	for (i=0 ; i<mouse_buttons ; i++)
	{
		if ( (mstate & (1<<i)) &&
			!(mouse_oldbuttonstate & (1<<i)) )
		{
			Key_Event (K_MOUSE1 + i, true, sys_msg_time);
		}

		if ( !(mstate & (1<<i)) &&
			(mouse_oldbuttonstate & (1<<i)) )
		{
				Key_Event (K_MOUSE1 + i, false, sys_msg_time);
		}
	}	
		
	mouse_oldbuttonstate = mstate;
}

#ifdef _WIN32_WCE
//Windows CE mouse vars
//extern int vid_modenum;
int start_yaw=0, start_pitch=0;
int mlook=0;
float yaw_modifier=0.0f;
int autoyaw_left, autoyaw_right;

//These are the mapping coordinates for the virtual on screen controls
//Note that with this scheme all vertical dividers must extend the extent of the
//mapping region.  In other words columns must extend from the top to the bottom
//and be of continuous width.  Rows may be discontinuous.

//define the 6 columns:
static int control_horz[10]={18, 37, 70, 78, 95, 120, 145, 169, 198, 0};
//define the rows for each of the columns:
static int control_vert[10][6]={
	{15, 35, 55, 75, 95, 0},
	{15, 35, 55, 75, 95, 0},
	{29, 94, 0},
	{29, 94, 0},
	{29, 94, 0},
	{29, 94, 0},
	{29, 94, 0},
	{29, 94, 0},
	{29, 94, 0},
	{29, 62, 95, 0}
};


#define CONTROL_TOP 180+24
#endif

/*
===========
IN_MouseMove
===========
*/
void IN_MouseMove (usercmd_t *cmd)
{
#ifndef _WIN32_WCE
	int		mx, my;
#endif

	if (!mouseactive)
		return;
#ifndef _WIN32_WCE
	// find mouse movement
	if (!GetCursorPos (&current_pos))
		return;

	mx = current_pos.x - window_center_x;
	my = current_pos.y - window_center_y;

#if 0
	if (!mx && !my)
		return;
#endif

	if (m_filter->value)
	{
		mouse_x = (mx + old_mouse_x) * 0.5;
		mouse_y = (my + old_mouse_y) * 0.5;
	}
	else
	{
		mouse_x = mx;
		mouse_y = my;
	}

	old_mouse_x = mx;
	old_mouse_y = my;

	mouse_x *= sensitivity->value;
	mouse_y *= sensitivity->value;

// add mouse X/Y movement to cmd
	if ( (in_strafe.state & 1) || (lookstrafe->value && mlooking ))
		cmd->sidemove += m_side->value * mouse_x;
	else
		cl.viewangles[YAW] -= m_yaw->value * mouse_x;

	if ( (mlooking || freelook->value) && !(in_strafe.state & 1))
	{
		cl.viewangles[PITCH] += m_pitch->value * mouse_y;
	}
	else
	{
		cmd->forwardmove -= m_forward->value * mouse_y;
	}

	// force the mouse to the center, so there's room to move
	if (mx || my)
		SetCursorPos (window_center_x, window_center_y);
#else
	//Windows CE Stylus handling
	yaw_modifier=0;

	if (!mlook) return;

	//Dan: We force pitch drift off here.  For some reason it always wants to
	//center the view.
//	V_StopPitchDrift ();

	//Take into account the screen orientation:
	if (!sw_mode->value) {
		if (in_autoyawrate->value) {
			autoyaw_left=20;
			autoyaw_right=220;
			if (current_pos.x<autoyaw_left) 
				yaw_modifier=in_autoyawrate->value;
			else if (current_pos.x>autoyaw_right) 
				yaw_modifier=-in_autoyawrate->value;
			else 
				cl.viewangles[YAW]=start_yaw-(m_yaw->value*(current_pos.x-start_pos.x)*sensitivity->value*10);
		} else 
			cl.viewangles[YAW]=start_yaw-(m_yaw->value*(current_pos.x-start_pos.x)*sensitivity->value*10);
		cl.viewangles[PITCH]=start_pitch+(m_pitch->value*(current_pos.y-start_pos.y)*sensitivity->value*10);
	} else if (sw_mode->value==1) {
		if (in_autoyawrate->value) {
			autoyaw_left=40;
			autoyaw_right=280;
			if ((320-current_pos.y)<autoyaw_left) yaw_modifier=in_autoyawrate->value;
			else if ((320-current_pos.y)>autoyaw_right) yaw_modifier=-in_autoyawrate->value;
			else cl.viewangles[YAW]=start_yaw+(m_yaw->value*(current_pos.y-start_pos.y)*sensitivity->value*10);
		} else cl.viewangles[YAW]=start_yaw+(m_yaw->value*(current_pos.y-start_pos.y)*sensitivity->value*10);
		cl.viewangles[PITCH]=start_pitch+(m_pitch->value*(current_pos.x-start_pos.x)*sensitivity->value*10);
	} else {
		if (in_autoyawrate->value) {
			autoyaw_left=40;
			autoyaw_right=280;
			if (current_pos.y<autoyaw_left) yaw_modifier=in_autoyawrate->value;
			else if (current_pos.y>autoyaw_right) yaw_modifier=-in_autoyawrate->value;
			else cl.viewangles[YAW]=start_yaw-(m_yaw->value*(current_pos.y-start_pos.y)*sensitivity->value*10);
		} else cl.viewangles[YAW]=start_yaw-(m_yaw->value*(current_pos.y-start_pos.y)*sensitivity->value*10);
		cl.viewangles[PITCH]=start_pitch-(m_pitch->value*(current_pos.x-start_pos.x)*sensitivity->value*10);
	}
	if (cl.viewangles[PITCH] > 80)
		cl.viewangles[PITCH] = 80;
	else
	if (cl.viewangles[PITCH] < -70)
		cl.viewangles[PITCH] = -70;

	cl.viewangles[YAW]+=yaw_modifier;
	start_yaw+=yaw_modifier;

#endif
}

#ifdef _WIN32_WCE
//Windows CE Mouse (stylus) handling
int virt_key=0;

void IN_CEMouseButton(int x, int y, int pressed) {
	//We have a skin that we need to map against first.
	//If the main game area is clicked, we do not trigger a mouse button event
	//in Q2, but simply begin capture and mlook mode.
	int mlook_button=0;

	if (pressed) {
		int row=0, col=0;
		int idx;

		start_pos.x=x;
		start_pos.y=y;
		current_pos.x=x;
		current_pos.y=y;

		//Signal the stylus is down and get stylus capture
		SetCapture(cl_hwnd);

		if (sw_mode->value==0) {
			if (start_pos.y<24) {//The user clicked on the title bar
				if (start_pos.x<30) {
					//Menu button
					virt_key=K_ESCAPE;
					//Start button
					//vid_paused=1;
					//SHFullScreen(cl_hwnd, SHFS_SHOWTASKBAR);
					//ShowWindow(cl_hwnd, SW_MINIMIZE);
				} else if (start_pos.x<54) {
					//Mute button
//					if (snd_blocked) snd_blocked=0;
//					else snd_blocked=1;
				} else if (start_pos.x<170) {
					//Nothing - pocket Quake 2 logo
				} else if (start_pos.x<216) {
					//Console button
					virt_key='~';
				} else if (start_pos.x>216) {
					//Close button
					PostMessage(cl_hwnd, WM_CLOSE, 0, 0);
					ReleaseCapture();
					return;
				}
			} else
			if (start_pos.y<CONTROL_TOP) //The user clicked in the game area
			{
				//Start mlook
				//Store starting player view angles
				start_yaw=cl.viewangles[YAW];
				start_pitch=cl.viewangles[PITCH];
				mlook=1;
				return; //TODO: handle as mouse click
			} else {
				//Offset so that the top of the control area is y==0
				start_pos.y-=CONTROL_TOP;

				//Determine which column of buttons was pressed:
				while (control_horz[col]) {
					if (start_pos.x<=control_horz[col]) break;
					col++;
				}
				//Now for the row
				while (control_vert[col][row]) {
					if (start_pos.y<=control_vert[col][row]) break;
					row++;
				}

				//Act on which col and row were selected.
				idx=col<<4|row;
				switch (idx) {
					case 0x00: virt_key='1'; break;
					case 0x01: virt_key='2'; break;
					case 0x02: virt_key='4'; break;
					case 0x03: virt_key='6'; break;
					case 0x04: virt_key='8'; break;
					case 0x05: virt_key='0'; break;

					case 0x10: virt_key='0'; break;
					case 0x11: virt_key='3'; break;
					case 0x12: virt_key='5'; break;
					case 0x13: virt_key='7'; break;
					case 0x14: virt_key='9'; break;
					case 0x15: virt_key='g'; break;

					case 0x20: virt_key=K_AUX20; break;	//A
					case 0x21: mlook_button=1; break;
					case 0x22: virt_key='q'; break;		//QD

					case 0x30: virt_key=K_AUX20; break;	//A
					case 0x31: mlook_button=1; break;
					case 0x32: virt_key='i'; break;		//IN

					case 0x40: virt_key=K_CTRL; break;	//Attack
					case 0x41: mlook_button=1; break;
					case 0x42: virt_key='i'; break;		//IN

					case 0x50: virt_key=K_CTRL; break;	//Attack
					case 0x51: mlook_button=1; break;
					case 0x52: virt_key='b'; break;		//RB

					case 0x60: virt_key=K_CTRL; break;	//Attack
					case 0x61: mlook_button=1; break;
					case 0x62: virt_key='e'; break;		//ES

					case 0x70: virt_key=K_CTRL; break;	//Attack
					case 0x71: mlook_button=1; break;
					case 0x72: virt_key='p'; break;		//AS

					case 0x80: virt_key=K_CTRL; break;	//Attack
					case 0x81: mlook_button=1; break;
					case 0x82: virt_key='s'; break;		//SI

					case 0x90: virt_key=K_AUX21; break;	//B
					case 0x91: virt_key=' '; break;		//Up
					case 0x92: virt_key='c'; break;		//Down
					case 0x93: virt_key=K_F1; break;	//F1

					default:
						//Error - we don't have a handler for this region
						ASSERT(0);
						virt_key=0;
						break;
				}
			}
			if (mlook_button) {
				//Restore real mouse y pos (remember we offset so that y==top of control area)
				start_pos.y=y;

				//Store starting player view angles
				start_yaw=cl.viewangles[YAW];
				start_pitch=cl.viewangles[PITCH];
				mlook=1;
				virt_key=0;
			}
		} else {
			//We are in landscape rotated video mode. We have some virtual areas defined
			//which trigger events around the border of the full-screen display.
			int x, y;
			if (sw_mode->value==1) {
				x=320-start_pos.y;
				y=start_pos.x;//-192;
			} else {
				x=start_pos.y;
				y=240-start_pos.x;
			}
			if (x<45) {
				if (y<160) virt_key=K_AUX10;
				else virt_key=K_AUX11;
			} else if (x>320-45) {
				if (y<160) virt_key=K_AUX12;
				else virt_key=K_AUX13;
			} else {
				if (y<35) virt_key=K_AUX14;
				else if (y>192) virt_key=K_AUX15;
				else {
					//mlook Area
					//Store starting player view angles
					start_yaw=cl.viewangles[YAW];
					start_pitch=cl.viewangles[PITCH];
					mlook=1;
				}
			}
		}
		if (virt_key) Key_Event (virt_key, true, 0);
	} else {
		//Stylus lifted
		if (virt_key) {
			//Signal quake that the key was released
			Key_Event (virt_key, false, 0);
			virt_key=0;
		}
		/*mouse_down=*/mlook=0;
		yaw_modifier=0.0f;
		if (sw_mode->value) ReleaseCapture();
	}
}

void IN_CEMouseMove(int x, int y) {
	current_pos.x=x;
	current_pos.y=y;
}
#endif

/*
=========================================================================

VIEW CENTERING

=========================================================================
*/

cvar_t	*v_centermove;
cvar_t	*v_centerspeed;


/*
===========
IN_Init
===========
*/
void IN_Init (void)
{
	// mouse variables
	m_filter				= Cvar_Get ("m_filter",					"0",		0);
    in_mouse				= Cvar_Get ("in_mouse",					"1",		CVAR_ARCHIVE);
#ifdef _WIN32_WCE
	in_autoyawrate			= Cvar_Get ("in_autoyawrate",			"15",		0);
#endif
	// joystick variables
	in_joystick				= Cvar_Get ("in_joystick",				"0",		CVAR_ARCHIVE);
	joy_name				= Cvar_Get ("joy_name",					"joystick",	0);
	joy_advanced			= Cvar_Get ("joy_advanced",				"0",		0);
	joy_advaxisx			= Cvar_Get ("joy_advaxisx",				"0",		0);
	joy_advaxisy			= Cvar_Get ("joy_advaxisy",				"0",		0);
	joy_advaxisz			= Cvar_Get ("joy_advaxisz",				"0",		0);
	joy_advaxisr			= Cvar_Get ("joy_advaxisr",				"0",		0);
	joy_advaxisu			= Cvar_Get ("joy_advaxisu",				"0",		0);
	joy_advaxisv			= Cvar_Get ("joy_advaxisv",				"0",		0);
	joy_forwardthreshold	= Cvar_Get ("joy_forwardthreshold",		"0.15",		0);
	joy_sidethreshold		= Cvar_Get ("joy_sidethreshold",		"0.15",		0);
	joy_upthreshold  		= Cvar_Get ("joy_upthreshold",			"0.15",		0);
	joy_pitchthreshold		= Cvar_Get ("joy_pitchthreshold",		"0.15",		0);
	joy_yawthreshold		= Cvar_Get ("joy_yawthreshold",			"0.15",		0);
	joy_forwardsensitivity	= Cvar_Get ("joy_forwardsensitivity",	"-1",		0);
	joy_sidesensitivity		= Cvar_Get ("joy_sidesensitivity",		"-1",		0);
	joy_upsensitivity		= Cvar_Get ("joy_upsensitivity",		"-1",		0);
	joy_pitchsensitivity	= Cvar_Get ("joy_pitchsensitivity",		"1",		0);
	joy_yawsensitivity		= Cvar_Get ("joy_yawsensitivity",		"-1",		0);

	// centering
	v_centermove			= Cvar_Get ("v_centermove",				"0.15",		0);
	v_centerspeed			= Cvar_Get ("v_centerspeed",			"500",		0);

	Cmd_AddCommand ("+mlook", IN_MLookDown);
	Cmd_AddCommand ("-mlook", IN_MLookUp);

	Cmd_AddCommand ("joy_advancedupdate", Joy_AdvancedUpdate_f);

	IN_StartupMouse ();
	IN_StartupJoystick ();

#ifdef _WIN32_WCE
	sw_mode = Cvar_Get( "sw_mode", "0", CVAR_ARCHIVE );
	mouseactive = true;
#endif
}

/*
===========
IN_Shutdown
===========
*/
void IN_Shutdown (void)
{
	IN_DeactivateMouse ();
}


/*
===========
IN_Activate

Called when the main window gains or loses focus.
The window may have been destroyed and recreated
between a deactivate and an activate.
===========
*/
void IN_Activate (qboolean active)
{
	in_appactive = active;
#ifndef _WIN32_WCE
	mouseactive = !active;		// force a new window check or turn off
#else
	mouseactive = 1;	//Mouse is always active for Windows CE
#endif
}


/*
==================
IN_Frame

Called every frame, even if not generating commands
==================
*/
void IN_Frame (void)
{
	if (!mouseinitialized)
		return;

	if (!in_mouse 
#ifndef _WIN32_WCE
		|| !in_appactive)
#else
		)
#endif
	{
		IN_DeactivateMouse ();
		return;
	}

	if ( !cl.refresh_prepped
		|| cls.key_dest == key_console
		|| cls.key_dest == key_menu)
	{
		// temporarily deactivate if in fullscreen
		if (Cvar_VariableValue ("vid_fullscreen") == 0)
		{
			IN_DeactivateMouse ();
			return;
		}
	}
#ifndef _WIN32_WCE
	IN_ActivateMouse ();
#endif
}

/*
===========
IN_Move
===========
*/
void IN_Move (usercmd_t *cmd)
{
	IN_MouseMove (cmd);

	if (ActiveApp)
		IN_JoyMove (cmd);
}


/*
===================
IN_ClearStates
===================
*/
void IN_ClearStates (void)
{
	mx_accum = 0;
	my_accum = 0;
	mouse_oldbuttonstate = 0;
}


/*
=========================================================================

JOYSTICK

=========================================================================
*/

/* 
=============== 
IN_StartupJoystick 
=============== 
*/  
void IN_StartupJoystick (void) 
{ 
#ifndef _WIN32_WCE
	int			numdevs;
	JOYCAPS		jc;
	MMRESULT	mmr;
	cvar_t		*cv;

 	// assume no joystick
	joy_avail = false; 

	// abort startup if user requests no joystick
	cv = Cvar_Get ("in_initjoy", "1", CVAR_NOSET);
	if ( !cv->value ) 
		return; 
 
	// verify joystick driver is present
	if ((numdevs = joyGetNumDevs ()) == 0)
	{
//		Com_Printf ("\njoystick not found -- driver not present\n\n");
		return;
	}

	// cycle through the joystick ids for the first valid one
	for (joy_id=0 ; joy_id<numdevs ; joy_id++)
	{
		memset (&ji, 0, sizeof(ji));
		ji.dwSize = sizeof(ji);
		ji.dwFlags = JOY_RETURNCENTERED;

		if ((mmr = joyGetPosEx (joy_id, &ji)) == JOYERR_NOERROR)
			break;
	} 

	// abort startup if we didn't find a valid joystick
	if (mmr != JOYERR_NOERROR)
	{
		Com_Printf ("\njoystick not found -- no valid joysticks (%x)\n\n", mmr);
		return;
	}

	// get the capabilities of the selected joystick
	// abort startup if command fails
	memset (&jc, 0, sizeof(jc));
	if ((mmr = joyGetDevCaps (joy_id, &jc, sizeof(jc))) != JOYERR_NOERROR)
	{
		Com_Printf ("\njoystick not found -- invalid joystick capabilities (%x)\n\n", mmr); 
		return;
	}

	// save the joystick's number of buttons and POV status
	joy_numbuttons = jc.wNumButtons;
	joy_haspov = jc.wCaps & JOYCAPS_HASPOV;

	// old button and POV states default to no buttons pressed
	joy_oldbuttonstate = joy_oldpovstate = 0;

	// mark the joystick as available and advanced initialization not completed
	// this is needed as cvars are not available during initialization

	joy_avail = true; 
	joy_advancedinit = false;

	Com_Printf ("\njoystick detected\n\n"); 
#endif
}


/*
===========
RawValuePointer
===========
*/
#ifndef _WIN32_WCE
PDWORD RawValuePointer (int axis)
{
	switch (axis)
	{
	case JOY_AXIS_X:
		return &ji.dwXpos;
	case JOY_AXIS_Y:
		return &ji.dwYpos;
	case JOY_AXIS_Z:
		return &ji.dwZpos;
	case JOY_AXIS_R:
		return &ji.dwRpos;
	case JOY_AXIS_U:
		return &ji.dwUpos;
	case JOY_AXIS_V:
		return &ji.dwVpos;
	}
}
#endif

/*
===========
Joy_AdvancedUpdate_f
===========
*/
void Joy_AdvancedUpdate_f (void)
{
#ifndef _WIN32_WCE
	// called once by IN_ReadJoystick and by user whenever an update is needed
	// cvars are now available
	int	i;
	DWORD dwTemp;

	// initialize all the maps
	for (i = 0; i < JOY_MAX_AXES; i++)
	{
		dwAxisMap[i] = AxisNada;
		dwControlMap[i] = JOY_ABSOLUTE_AXIS;
		pdwRawValue[i] = RawValuePointer(i);
	}

	if( joy_advanced->value == 0.0)
	{
		// default joystick initialization
		// 2 axes only with joystick control
		dwAxisMap[JOY_AXIS_X] = AxisTurn;
		// dwControlMap[JOY_AXIS_X] = JOY_ABSOLUTE_AXIS;
		dwAxisMap[JOY_AXIS_Y] = AxisForward;
		// dwControlMap[JOY_AXIS_Y] = JOY_ABSOLUTE_AXIS;
	}
	else
	{
		if (strcmp (joy_name->string, "joystick") != 0)
		{
			// notify user of advanced controller
			Com_Printf ("\n%s configured\n\n", joy_name->string);
		}

		// advanced initialization here
		// data supplied by user via joy_axisn cvars
		dwTemp = (DWORD) joy_advaxisx->value;
		dwAxisMap[JOY_AXIS_X] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_X] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD) joy_advaxisy->value;
		dwAxisMap[JOY_AXIS_Y] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_Y] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD) joy_advaxisz->value;
		dwAxisMap[JOY_AXIS_Z] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_Z] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD) joy_advaxisr->value;
		dwAxisMap[JOY_AXIS_R] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_R] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD) joy_advaxisu->value;
		dwAxisMap[JOY_AXIS_U] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_U] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD) joy_advaxisv->value;
		dwAxisMap[JOY_AXIS_V] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_V] = dwTemp & JOY_RELATIVE_AXIS;
	}

	// compute the axes to collect from DirectInput
	joy_flags = JOY_RETURNCENTERED | JOY_RETURNBUTTONS | JOY_RETURNPOV;
	for (i = 0; i < JOY_MAX_AXES; i++)
	{
		if (dwAxisMap[i] != AxisNada)
		{
			joy_flags |= dwAxisFlags[i];
		}
	}
#endif
}


/*
===========
IN_Commands
===========
*/
void IN_Commands (void)
{
#ifndef _WIN32_WCE
	int		i, key_index;
	DWORD	buttonstate, povstate;

	if (!joy_avail)
	{
		return;
	}

	
	// loop through the joystick buttons
	// key a joystick event or auxillary event for higher number buttons for each state change
	buttonstate = ji.dwButtons;
	for (i=0 ; i < joy_numbuttons ; i++)
	{
		if ( (buttonstate & (1<<i)) && !(joy_oldbuttonstate & (1<<i)) )
		{
			key_index = (i < 4) ? K_JOY1 : K_AUX1;
			Key_Event (key_index + i, true, 0);
		}

		if ( !(buttonstate & (1<<i)) && (joy_oldbuttonstate & (1<<i)) )
		{
			key_index = (i < 4) ? K_JOY1 : K_AUX1;
			Key_Event (key_index + i, false, 0);
		}
	}
	joy_oldbuttonstate = buttonstate;

	if (joy_haspov)
	{
		// convert POV information into 4 bits of state information
		// this avoids any potential problems related to moving from one
		// direction to another without going through the center position
		povstate = 0;
		if(ji.dwPOV != JOY_POVCENTERED)
		{
			if (ji.dwPOV == JOY_POVFORWARD)
				povstate |= 0x01;
			if (ji.dwPOV == JOY_POVRIGHT)
				povstate |= 0x02;
			if (ji.dwPOV == JOY_POVBACKWARD)
				povstate |= 0x04;
			if (ji.dwPOV == JOY_POVLEFT)
				povstate |= 0x08;
		}
		// determine which bits have changed and key an auxillary event for each change
		for (i=0 ; i < 4 ; i++)
		{
			if ( (povstate & (1<<i)) && !(joy_oldpovstate & (1<<i)) )
			{
				Key_Event (K_AUX29 + i, true, 0);
			}

			if ( !(povstate & (1<<i)) && (joy_oldpovstate & (1<<i)) )
			{
				Key_Event (K_AUX29 + i, false, 0);
			}
		}
		joy_oldpovstate = povstate;
	}
#endif
}


/* 
=============== 
IN_ReadJoystick
=============== 
*/  
qboolean IN_ReadJoystick (void)
{
#ifndef _WIN32_WCE
	memset (&ji, 0, sizeof(ji));
	ji.dwSize = sizeof(ji);
	ji.dwFlags = joy_flags;

	if (joyGetPosEx (joy_id, &ji) == JOYERR_NOERROR)
	{
		return true;
	}
	else
	{
		// read error occurred
		// turning off the joystick seems too harsh for 1 read error,\
		// but what should be done?
		// Com_Printf ("IN_ReadJoystick: no response\n");
		// joy_avail = false;
		return false;
	}
#else
	return false;
#endif
}


/*
===========
IN_JoyMove
===========
*/
void IN_JoyMove (usercmd_t *cmd)
{
#ifndef _WIN32_WCE
	float	speed, aspeed;
	float	fAxisValue;
	int		i;

	// complete initialization if first time in
	// this is needed as cvars are not available at initialization time
	if( joy_advancedinit != true )
	{
		Joy_AdvancedUpdate_f();
		joy_advancedinit = true;
	}

	// verify joystick is available and that the user wants to use it
	if (!joy_avail || !in_joystick->value)
	{
		return; 
	}
 
	// collect the joystick data, if possible
	if (IN_ReadJoystick () != true)
	{
		return;
	}

	if ( (in_speed.state & 1) ^ (int)cl_run->value)
		speed = 2;
	else
		speed = 1;
	aspeed = speed * cls.frametime;

	// loop through the axes
	for (i = 0; i < JOY_MAX_AXES; i++)
	{
		// get the floating point zero-centered, potentially-inverted data for the current axis
		fAxisValue = (float) *pdwRawValue[i];
		// move centerpoint to zero
		fAxisValue -= 32768.0;

		// convert range from -32768..32767 to -1..1 
		fAxisValue /= 32768.0;

		switch (dwAxisMap[i])
		{
		case AxisForward:
			if ((joy_advanced->value == 0.0) && mlooking)
			{
				// user wants forward control to become look control
				if (fabs(fAxisValue) > joy_pitchthreshold->value)
				{		
					// if mouse invert is on, invert the joystick pitch value
					// only absolute control support here (joy_advanced is false)
					if (m_pitch->value < 0.0)
					{
						cl.viewangles[PITCH] -= (fAxisValue * joy_pitchsensitivity->value) * aspeed * cl_pitchspeed->value;
					}
					else
					{
						cl.viewangles[PITCH] += (fAxisValue * joy_pitchsensitivity->value) * aspeed * cl_pitchspeed->value;
					}
				}
			}
			else
			{
				// user wants forward control to be forward control
				if (fabs(fAxisValue) > joy_forwardthreshold->value)
				{
					cmd->forwardmove += (fAxisValue * joy_forwardsensitivity->value) * speed * cl_forwardspeed->value;
				}
			}
			break;

		case AxisSide:
			if (fabs(fAxisValue) > joy_sidethreshold->value)
			{
				cmd->sidemove += (fAxisValue * joy_sidesensitivity->value) * speed * cl_sidespeed->value;
			}
			break;

		case AxisUp:
			if (fabs(fAxisValue) > joy_upthreshold->value)
			{
				cmd->upmove += (fAxisValue * joy_upsensitivity->value) * speed * cl_upspeed->value;
			}
			break;

		case AxisTurn:
			if ((in_strafe.state & 1) || (lookstrafe->value && mlooking))
			{
				// user wants turn control to become side control
				if (fabs(fAxisValue) > joy_sidethreshold->value)
				{
					cmd->sidemove -= (fAxisValue * joy_sidesensitivity->value) * speed * cl_sidespeed->value;
				}
			}
			else
			{
				// user wants turn control to be turn control
				if (fabs(fAxisValue) > joy_yawthreshold->value)
				{
					if(dwControlMap[i] == JOY_ABSOLUTE_AXIS)
					{
						cl.viewangles[YAW] += (fAxisValue * joy_yawsensitivity->value) * aspeed * cl_yawspeed->value;
					}
					else
					{
						cl.viewangles[YAW] += (fAxisValue * joy_yawsensitivity->value) * speed * 180.0;
					}

				}
			}
			break;

		case AxisLook:
			if (mlooking)
			{
				if (fabs(fAxisValue) > joy_pitchthreshold->value)
				{
					// pitch movement detected and pitch movement desired by user
					if(dwControlMap[i] == JOY_ABSOLUTE_AXIS)
					{
						cl.viewangles[PITCH] += (fAxisValue * joy_pitchsensitivity->value) * aspeed * cl_pitchspeed->value;
					}
					else
					{
						cl.viewangles[PITCH] += (fAxisValue * joy_pitchsensitivity->value) * speed * 180.0;
					}
				}
			}
			break;

		default:
			break;
		}
	}
#endif
}

