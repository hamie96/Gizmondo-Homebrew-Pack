// Camera.h: interface for the CCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMERA_H__A92D57EB_A55F_4B3B_84BF_0CB479697AD2__INCLUDED_)
#define AFX_CAMERA_H__A92D57EB_A55F_4B3B_84BF_0CB479697AD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NUM_BUFFERS 2
#define MAX_LOADSTRING 100

#include "memory.h"
#include "OEMINC.h"
#include <winioctl.h>
#include "IObserverable.h"



class CCamera : public IObserverable
{
public:
	PBYTE getLastPreviewBuffer();
	void takePicture();
	CCamera();
	virtual ~CCamera();

private:
	BOOL GetStill();
	BOOL GetPreview();
	
	BOOL InitCamera();
	HBITMAP CreateBltDIB (HDC hdc, LONG cx, LONG cy, PVOID *ppvBits);
	// Preview buffer structure
	
typedef struct {
	DWORD	nBuf;					// number of buffer sets
	DWORD	cx;						// buffer width
	DWORD	cy;						// buffer height
	DWORD	cbBuf;					// buffer size (bytes)
	HBITMAP	hBitmap[NUM_BUFFERS];	// DIB handle
	HDC		hImageCache;			// HDC
	PBYTE	pBitmap[NUM_BUFFERS];	// pointer to DIB bitmap data
	DWORD	dwCurBuf;				// index of current preview DIB
	DWORD	dwDispBuf;				// index of current display DIB
} DIBBUF, * PDIBBUF;
// Captured image buffe

typedef struct {
	DWORD	cx;						// buffer width
	DWORD	cy;						// buffer height
	DWORD	cbBuf;					// buffer size (bytes)
	PBYTE	pBuf;					// pointers to YUV image data
	DWORD	dwCurBuf;				// index of current image
	DWORD	dwDispBuf;				// index of current image
} YUVBUF, * PYUVBUF;

HANDLE fichier;

	HANDLE ghCam;
	DIBBUF m_bufPreview;			// Preview buffers
YUVBUF m_bufCapture;			// Captured image buffers

 int gsizeX;
int gsizeY;
BOOL m_bAllocatedDIBs; //=FALSE;
BOOL gbDisplayPreview; // = FALSE;
int giActions; // =0;
int m_posX;
int m_posY;
int AllocateYUV(PYUVBUF pBuf, DWORD cx, DWORD cy);	
int AllocateDIB(HDC hDC, PDIBBUF pBuf, DWORD cx, DWORD cy);

};

#endif // !defined(AFX_CAMERA_H__A92D57EB_A55F_4B3B_84BF_0CB479697AD2__INCLUDED_)
