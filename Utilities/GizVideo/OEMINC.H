///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2004 Gizmondo Ltd and
// Copyright (C) 2004 Intrinsyc Europe Ltd.
// All rights reserved.
// No use or disclosure of this information in any form without the
// written permission of the author Intrinsyc Europe Ltd.
// http://www.intrinsyc.com
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// FILE INFORMATION
//
//   Project: Gizmondo Terminal WinCE Port
//      Unit: 
// $Workfile: OEMINC.H $
// $Revision: 25 $
//     $Date: 22/11/04 14:54 $
//
// DESCRIPTION : OEM Specific Public Definitions
//
///////////////////////////////////////////////////////////////////////////////
//
// MODIFICATION HISTORY
//
// $Log: /Gizmondo/SWEDE/PLATFORM/SWEDE/INC/OEMINC.H $
// 
// 25    22/11/04 14:54 Martint
// Added MIDI state definitions.
// 
// 24    10/11/04 11:03 Martint
// Added SD/MMC card state events.
// 
// 23    9/11/04 8:31 Martint
// Increased DEF_SUSPEND_TIMEOUT to 5 days.
// 
// 22    3/11/04 10:40 Martint
// Added MIDI synthesiser control codes.
// 
// 21    22/10/04 15:14 Neilc
// Added IOCTL_OEM_GET_ENCRYPTION_KEY
// 
// 20    1/10/04 12:52 Martint
// Removed IOCTL_OEM_POWEROFF (was a temp method for powering the unit
// off).
// Added IOCTL_OEM_SET_POWEROFFFLAG, IOCTL_OEM_SET_OFFTIMEOUT,
// IOCTL_OEM_GET_OFFTIMEOUT and DEF_SUSPEND_TIMEOUT to support suspend to
// off timeout.
// 
// 19    29/09/04 10:45 Robn
// Added FTS Bluetooth enable define
// 
// 18    31/08/04 15:29 Neilc
// Added bluetooth test code
// 
// 17    25/08/04 14:32 Martint
// Added IOCTL_OEM_GET_CLOCKFREQ.
// 
// 16    24/08/04 16:21 Martint
// Added watchdog driver IOCTL definitions.
// 
// 15    5/08/04 11:21 Martint
// Added IOCTL_OEM_POWEROFF.
// 
// 14    2/08/04 15:39 Martint
// Renamed BTLOADER as BTHCNFG and moved defines out of camera section.
// 
// 13    30/07/04 15:00 Neilc
// Added IOCTL for GetModemPowerState
// 
// 12    27/07/04 13:03 Neilc
// Added BTLoader IOCTLs
// 
// 11    7/07/04 13:59 Martint
// Added DISPLAY_FRAME_EVENT - a manual reset event with this name is
// pulsed on each VSYNC by the display driver IST.
// 
// 10    7/07/04 9:30 Martint
// Removed redundant CAM_BUFFERS definition.
// 
// 9     21/06/04 18:06 Martint
// Added camera driver definitions.
// 
// $NoKeywords: $
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _OEMINC_H_
#define _OEMINC_H_

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////
// Public Registry Keys and Values
///////////////////////////////////////////////////////////////////////////

// Backlight registry keys and values (HKCU)
#define REGKEY_BACKLIGHT		TEXT("ControlPanel\\Backlight")
#define BACKLIGHT_MAX			31			// Maximum backlight level
#define BACKLIGHT_MIN			1			// Minimum backlight level
#define	BACKLIGHT_DEF			24			// Default backlight level
#define BACKLIGHT_DIM_DEF		8			// Default dim level
#define BACKLIGHT_BATT_TIMEOUT	15			// Default battery timeout
#define BACKLIGHT_AC_TIMEOUT	15			// Default AC timeout

#define REGVAL_LEVEL			TEXT("Level")
#define REGVAL_ON_LEVEL			TEXT("OnLevel")
#define REGVAL_DIM_LEVEL		TEXT("DimLevel")

#define EVENTNAME_BACKLIGHTCHANGEEVENT  TEXT("BackLightChangeEvent")

///////////////////////////////////////////////////////////////////////////
// Kernel I/O Control Codes
///////////////////////////////////////////////////////////////////////////

#define IOCTL_OEM_GET_DEVICEVER		CTL_CODE(FILE_DEVICE_HAL, 2001, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_INTERRUPTS_OFF	CTL_CODE(FILE_DEVICE_HAL, 2004, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_INTERRUPTS_ON		CTL_CODE(FILE_DEVICE_HAL, 2005, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_OEM_DEBUG_SERIAL		CTL_CODE(FILE_DEVICE_HAL, 2008, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_GET_ALARMTIME		CTL_CODE(FILE_DEVICE_HAL, 2009, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_RESTART			CTL_CODE(FILE_DEVICE_HAL, 2010, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_POWEROFF			CTL_CODE(FILE_DEVICE_HAL, 2011, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_OEM_MODIFYREGISTER	CTL_CODE(FILE_DEVICE_HAL, 2020, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_GET_CLOCKFREQ		CTL_CODE(FILE_DEVICE_HAL, 2021, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_GET_CONFIG_RECORD	CTL_CODE(FILE_DEVICE_HAL, 2022, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_OEM_SET_POWEROFFFLAG	CTL_CODE(FILE_DEVICE_HAL, 2025, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_SET_OFFTIMEOUT	CTL_CODE(FILE_DEVICE_HAL, 2026, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_GET_OFFTIMEOUT	CTL_CODE(FILE_DEVICE_HAL, 2027, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_OEM_GET_ENCRYPTION_KEY	CTL_CODE(FILE_DEVICE_HAL, 2030, METHOD_BUFFERED, FILE_ANY_ACCESS)


// Default suspend to power off timeout (minutes)
#define DEF_SUSPEND_TIMEOUT		(5 * 24 * 60)

//////////////////////////////////////////////////////////////////////////
// Version identifiers for IOCTL_OEM_GET_DEVICEVER
//////////////////////////////////////////////////////////////////////////

#define DEVICEVER_OAL			1		// Kernel OAL
#define DEVICEVER_BOOT			2		// Bootloader
#define DEVICEVER_FPGA			3		// FPGA
#define DEVICEVER_PCB			4		// PCB
#define DEVICEVER_CPLD			5		// CPLD

//////////////////////////////////////////////////////////////////////////
// IOCTL_OEM_MODIFYREGISTER input parameter structure
//////////////////////////////////////////////////////////////////////////

typedef struct {
	ULONG	ulRegSize;				// sizeof(reg)
	ULONG	ulRegFlags;				// Special handling flag(s), see below
	ULONG	ulRegAddr;				// Virtual address of register
	ULONG	ulAndMask;				// Value to AND with existing contents
	ULONG	ulXorMask;				// Value to XOR with after AND operation
									//		AND=0 XOR=0 => clear
									//		AND=0 XOR=1 => set
									//		AND=1 XOR=0 => leave
									//		AND=1 XOR=1 => toggle
} MODIFY_REGISTER_STRUCT, *PMODIFY_REGISTER_STRUCT;

// ulRegFlags
#define MODIFY_REGISTER_FLAG_NONE		0x00000000
#define MODIFY_REGISTER_FLAG_VOLATILE	0x00000001

//////////////////////////////////////////////////////////////////////////
// OEM-Specific Virtual Key Codes
///////////////////////////////////////////////////////////////////////////

#define VK_VOL_DN	(VK_F1)		// Volume down key
#define VK_VOL_UP	(VK_F2)		// Volume up key
#define VK_BACKLIGHT (VK_F3)	// Backlight key
#define VK_PANIC	(VK_F4)		// Panic key

#define VK_SUSPEND	(VK_F11)	// Power key - suspend
#define VK_PWROFF	(VK_F12)	// Power key - power off

///////////////////////////////////////////////////////////////////////////
// Display Driver
///////////////////////////////////////////////////////////////////////////

// Lock out GDI from accessing physical frame buffer
#define DRVESC_EXCLUSIVEMODE	10000

// Allow GDI to access frame buffer as normal
#define DRVESC_NORMALMODE		10001

// Named events
#define DISPLAY_FRAME_EVENT		TEXT("DISPLAY_FRAME_EVENT")

///////////////////////////////////////////////////////////////////////////
// Serial Driver Control Codes
///////////////////////////////////////////////////////////////////////////

#define	IOCTL_OEM_GSM_ON			CTL_CODE(FILE_DEVICE_SERIAL_PORT , 1001, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define	IOCTL_OEM_GSM_OFF			CTL_CODE(FILE_DEVICE_SERIAL_PORT , 1002, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_GSM_RESET			CTL_CODE(FILE_DEVICE_SERIAL_PORT , 1003, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_GSM_TOGGLE		CTL_CODE(FILE_DEVICE_SERIAL_PORT , 1004, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_GSM_GETPOWERSTATE	CTL_CODE(FILE_DEVICE_SERIAL_PORT , 1005, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_OEM_GPS_ON			CTL_CODE(FILE_DEVICE_SERIAL_PORT , 2001, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_GPS_OFF			CTL_CODE(FILE_DEVICE_SERIAL_PORT , 2002, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_GPS_BOOTSEL		CTL_CODE(FILE_DEVICE_SERIAL_PORT , 2003, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_GPS_WAKEUP		CTL_CODE(FILE_DEVICE_SERIAL_PORT , 2004, METHOD_BUFFERED, FILE_ANY_ACCESS)

///////////////////////////////////////////////////////////////////////////
// Camera Driver I/O Control Codes
///////////////////////////////////////////////////////////////////////////

#define IOCTL_OEM_CAM_SETFORMAT		CTL_CODE(FILE_DEVICE_HAL, 2101, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_CAM_GETFORMAT		CTL_CODE(FILE_DEVICE_HAL, 2102, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_CAM_START			CTL_CODE(FILE_DEVICE_HAL, 2103, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_CAM_STOP			CTL_CODE(FILE_DEVICE_HAL, 2104, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_CAM_PREVIEW		CTL_CODE(FILE_DEVICE_HAL, 2105, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_CAM_CAPTURE		CTL_CODE(FILE_DEVICE_HAL, 2106, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_CAM_GETTHREADS	CTL_CODE(FILE_DEVICE_HAL, 2107, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_CAM_WRITE			CTL_CODE(FILE_DEVICE_HAL, 2108, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_CAM_READ			CTL_CODE(FILE_DEVICE_HAL, 2109, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Capture buffer sizes
#define CAM_CAPTURE_CX		640
#define CAM_CAPTURE_CY		480
#define CAM_PREVIEW_CX		320
#define CAM_PREVIEW_CY		240

// Default frame capture timeout (ms)
#define CAM_TIMEOUT			1000

// Capture frame information structure
typedef struct {
	DWORD	dwWidth;				// capture buffer width, pixels (IN)
	DWORD	dwHeight;				// capture buffer height, lines (IN)
	DWORD	dwFrameCount;			// number of captured frame (IN/OUT)
	DWORD	dwTimeout;				// frame grabber timeout (IN)
} VINFRAMEINFO, *PVINFRAMEINFO;

///////////////////////////////////////////////////////////////////////////
// Bluetooth Control Driver I/O Control Codes
///////////////////////////////////////////////////////////////////////////

#define IOCTL_OEM_BLUETOOTH_ON		CTL_CODE(FILE_DEVICE_HAL, 2120, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_BLUETOOTH_OFF		CTL_CODE(FILE_DEVICE_HAL, 2121, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define	ENABLE_BLUETOOTH_DUT		0xFF
#define	ENABLE_BLUETOOTH_FTS		0xFE

///////////////////////////////////////////////////////////////////////////
// Watchdog Driver I/O Control Codes
///////////////////////////////////////////////////////////////////////////

#define IOCTL_OEM_WDOG_KICK			CTL_CODE(FILE_DEVICE_HAL, 2130, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_WDOG_DISABLE		CTL_CODE(FILE_DEVICE_HAL, 2131, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_WDOG_PERIOD		CTL_CODE(FILE_DEVICE_HAL, 2132, METHOD_BUFFERED, FILE_ANY_ACCESS)

///////////////////////////////////////////////////////////////////////////
// Wave Driver I/O Control Codes
///////////////////////////////////////////////////////////////////////////

#define IOCTL_OEM_SYNTH_ENABLE		CTL_CODE(FILE_DEVICE_SOUND, 1010, METHOD_BUFFERED, FILE_ANY_ACCESS)

///////////////////////////////////////////////////////////////////////////
// MIDI Driver I/O Control Codes
///////////////////////////////////////////////////////////////////////////

#define IOCTL_OEM_MIDI_PLAY			CTL_CODE(FILE_DEVICE_SOUND, 1020, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_MIDI_STOP			CTL_CODE(FILE_DEVICE_SOUND, 1022, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_MIDI_PAUSE		CTL_CODE(FILE_DEVICE_SOUND, 1024, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_MIDI_RESUME		CTL_CODE(FILE_DEVICE_SOUND, 1026, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OEM_MIDI_STATE		CTL_CODE(FILE_DEVICE_SOUND, 1028, METHOD_BUFFERED, FILE_ANY_ACCESS)

// MIDI hardware states
typedef enum {
	MIDI_UNKNOWN = -1, 				// unknown state
	MIDI_OFF = 0, 					// hardware is off
	MIDI_DSPON,						// clocks and DSP are on
	MIDI_RUN, 						// DSP is running
	MIDI_STARTING,					// playback is starting
	MIDI_PLAYING, 					// playback is in progress
	MIDI_WAITING, 					// waiting for playback completion
	MIDI_DONE,						// playback is complete
} MIDI_STATE, *PMIDI_STATE;

///////////////////////////////////////////////////////////////////////////
// SD/MMC Driver
///////////////////////////////////////////////////////////////////////////

// Named events
#define SDMMC_INSERT_EVENT		TEXT("SDMMC_INSERT_EVENT")
#define SDMMC_REMOVE_EVENT		TEXT("SDMMC_REMOVE_EVENT")

//////////////////////////////////////////////////////////////////////////
// Interrupt Control macros
///////////////////////////////////////////////////////////////////////////

#define	OEM_INTERRUPTS_OFF()	{ \
	DWORD cbRet; \
	KernelIoControl (IOCTL_OEM_INTERRUPTS_OFF,NULL,0,NULL,0,&cbRet); \
}
#define	OEM_INTERRUPTS_ON()	{ \
	DWORD cbRet; \
	KernelIoControl (IOCTL_OEM_INTERRUPTS_ON,NULL,0,NULL,0,&cbRet); \
}

///////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif	// _OEMINC_H_
