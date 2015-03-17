// Camera.cpp: implementation of the CCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Camera.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCamera::CCamera()
{
	m_bAllocatedDIBs=FALSE;
	gbDisplayPreview= FALSE;
	giActions=0;

	if (InitCamera() != TRUE)
	{
		MessageBox(NULL, L"ERROR: Camera could not be initalized!",L"Error", MB_OK);
		
	}

	HDC hDC = GetWindowDC(NULL);
	if (hDC == NULL)
	{
		MessageBox(NULL, L"HDC IS NULL\r\n",L"Error", MB_OK);
		//FreeResources();
		return;
	}


	RETAILMSG(1, (TEXT("Allocating more memory!\r\n")));

	if ( !AllocateDIB (hDC, &m_bufPreview,gsizeX,gsizeY) ) 
	{
		RETAILMSG(1,(TEXT("ViewFinder: AllocateDIBs() failed\r\n")));
		MessageBox(NULL, L"ViewFinder: AllocateDIBs() failed\r\n",L"Error", MB_OK);
		//FreeResources();
		return;
	}

	// Allocate YUV image buffer(s)
	if ( !AllocateYUV(&m_bufCapture,CAM_CAPTURE_CX,CAM_CAPTURE_CY) ) 
	{
		RETAILMSG(1,(TEXT("ViewFinder: AllocateYUVs() failed\r\n")));
		MessageBox(NULL, L"ViewFinder: AllocateYUVs() failed\r\n",L"Error", MB_OK);
		//FreeResources();
		return;
	}

	

	
}

CCamera::~CCamera()
{

}


		/*case WM_TIMER:
			GetPreview();					// Get Preview image
			gbDisplayPreview = TRUE;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
			*/


/*		case WM_KEYDOWN:
			switch(wParam)
			{
			case VK_RETURN:
				//GetStill();
				//DestroyWindow(hWnd);
				PerformActions(hWnd);PerformActions(hWnd);PerformActions(hWnd);PerformActions(hWnd);
				break;
			case VK_SPACE:
				//GetPreview();					// Get Preview image
				//gbDisplayPreview = TRUE;
				//InvalidateRect(hWnd, NULL, TRUE);

				SetTimer(hWnd, 666 //(id timer)
				, 100, NULL);

				break;
			}
			
			*/
			//break;
		/*case WM_PAINT:
			RECT rt;
			hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rt);
		
			Paint(hdc,rt,0,0);
			if (giActions == 0)
				DrawText(hdc, szMess, _tcslen(szMess), &rt, 
					DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		
			EndPaint(hWnd, &ps);
			break;*/
		


///////////////////////////////////////////////////////////////////////////
//
// GetPreview() - capture a single preview frame from the camera driver
//
BOOL CCamera::GetPreview(void)
{
	VINFRAMEINFO vinFrame;
	DWORD cbRet;

//	MessageBox(NULL, TEXT("Test2.1.0.5"),TEXT("None"), MB_OK);

//	int x1, n1;

//	int tn, tn2, tr, tv, tb, b1=-1,
//		b2=-1;

	// Start grabbing frames
	if ( !DeviceIoControl(ghCam,
		IOCTL_OEM_CAM_START,NULL,0,NULL,0,&cbRet,NULL) ) 
	{
//		RETAILMSG(1,(TEXT("<CamTest>: IOCTL_OEM_CAM_START failed (%u)\r\n"), GetLastError()));
		return FALSE;
	}

//	MessageBox(NULL, TEXT("Test2.1.0.6"),TEXT("None"), MB_OK);

	// Set up frame information structure
	memset (&vinFrame, 0, sizeof(VINFRAMEINFO));
	vinFrame.dwTimeout = CAM_TIMEOUT;	// no timeout we want the image straight away
	vinFrame.dwWidth = m_bufPreview.cx;
	vinFrame.dwHeight = m_bufPreview.cy;

//	MessageBox(NULL, TEXT("Test2.1.0.7"),TEXT("None"), MB_OK);
/*
			if (m_bufPreview.pBitmap != NULL)
		{
			if (m_bufPreview.pBitmap[0] == NULL)
			{
				
				MessageBox(NULL, TEXT("PBitmapIsNull .1.0.8.0.9"),TEXT("None"), MB_OK);
			}
		}
		else
		{
			MessageBox(NULL, TEXT("PBitmapIsNull .1.0.8.0.9"),TEXT("None"), MB_OK);
		}

  */

	//ghCam vinFrame m_bufPreview pBitmap
	if (DeviceIoControl (ghCam, IOCTL_OEM_CAM_PREVIEW, &vinFrame, 
		sizeof(VINFRAMEINFO), m_bufPreview.pBitmap[0], 
		m_bufPreview.cbBuf, &cbRet, NULL)) 
	{
	//	MessageBox(NULL, L"Test2.1.0.7.5-GOOD",L"None", MB_OK);
		RETAILMSG(1,(TEXT("ViewFinder::GetPreview: Grabbed frame %u to pBitmap = %08X\r\n"), 
			vinFrame.dwFrameCount, m_bufPreview.pBitmap[0]));
	
		RETAILMSG(1,(TEXT("ViewFinder::GetPreview: dwDispBuf:%u, dwCurBuf:%u\r\n"), m_bufPreview.dwDispBuf, m_bufPreview.dwCurBuf));
	
	} else 
	{
		MessageBox(NULL, L"Test2.1.0.7.5-BAD",L"None", MB_OK);
	}

//	MessageBox(NULL, TEXT("Test2.1.0.8"),TEXT("None"), MB_OK);


/*	n1 = 0;
	for (x1=0;x1<38400-1;x1++)
	{
//		printf("2.1.0.8.[%d]-1 ", x1);

		b1 = m_bufPreview.pBitmap[0][x1*4];
		//b2 =m_bufPreview.pBitmap[0][x1*4+1];

//		printf("2 ");
		tr=(((b1+(b2<<8))&&0xF800))>>8;
		tv=(((b1+(b2<<8))&&0x07E0)<<5)>>8;
		tb=(((b1+(b2<<8))&&0x001F)<<11)>>8;

		tn= ((tr+tv+tb)/3)>>4;

//		printf("3 ");

		//b1 =m_bufPreview.pBitmap[0][x1*4+2];
		//b2 =m_bufPreview.pBitmap[0][x1*4+3];

		tr=(((b1+(b2<<8))&&0xF800))>>8;
		tv=(((b1+(b2<<8))&&0x07E0)<<5)>>8;
		tb=(((b1+(b2<<8))&&0x001F)<<11)>>8;

//		printf("4 ");
		tn2= ((tr+tv+tb)/3)>>4;
		
		m_nLastPreviewBuffer[x1]=tn+(tn2<<4);
//		printf("5\r\n");
	}
*/

//	MessageBox(NULL, TEXT("Test2.1.0.9"),TEXT("None"), MB_OK);

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////
//
// CreateBltDIB() - create DIB section for GDI rendering
//
HBITMAP CCamera::CreateBltDIB (HDC hdc, LONG cx, LONG cy, PVOID *ppvBits)
{
	PBITMAPINFO	pBMI;
	HBITMAP	hBitmap;
	DWORD	*pdwMasks;

	cy = 1 - cy - 1;

	pBMI = (PBITMAPINFO) LocalAlloc (LPTR, sizeof(BITMAPINFO) + 3*sizeof(DWORD) );
	pBMI->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pBMI->bmiHeader.biWidth = cx;
	pBMI->bmiHeader.biHeight = cy;
	pBMI->bmiHeader.biPlanes = 1;
	pBMI->bmiHeader.biBitCount = 16;
	pBMI->bmiHeader.biCompression = BI_BITFIELDS; 
	// Specify RGB 5:6:5 colour masks
	pdwMasks = (PDWORD)pBMI->bmiColors;
	*pdwMasks++ = 0xF800;			// red mask
	*pdwMasks++ = 0x07E0;			// green mask
	*pdwMasks++ = 0x001F;			// blue mask

	hBitmap = CreateDIBSection (hdc, pBMI, DIB_RGB_COLORS, ppvBits, NULL, 0);
	LocalFree (pBMI);

	return (hBitmap);
}

///////////////////////////////////////////////////////////////////////////
//
// AllocateYUVs() - allocates planar YUV 4:2;0 buffer(s)
//
BOOL CCamera::AllocateYUV(PYUVBUF pBuf, DWORD cx, DWORD cy)
{
	// Calculate required capture buffer sizes
	pBuf->cx = cx;
	pBuf->cy = cy;
	pBuf->cbBuf = (3 * pBuf->cx * pBuf->cy) >> 1;	// 4:2:0
//	RETAILMSG(1,(TEXT("<CamTest>: Allocating YUV buffers x (%ux%u)\r\n"), pBuf->cx, pBuf->cy));

	// Allocate the required number of YUV buffers
	pBuf->pBuf = (PBYTE) LocalAlloc (LPTR, pBuf->cbBuf);
	if (pBuf->pBuf == 0) 
	{
//		RETAILMSG(1,(TEXT("<CamTest>: Capture YUV alloc failed (%u)\r\n"), GetLastError()));
		
		return (FALSE);
	}
	
	// Start with first YUV buffer
	pBuf->dwCurBuf = 0;
	pBuf->dwDispBuf = 1;

	return (TRUE);
}



///////////////////////////////////////////////////////////////////////////
//
// AllocateDIBs() - allocates 16-bpp DIB buffer(s)
//
BOOL CCamera::AllocateDIB(HDC hDC, PDIBBUF pBuf, DWORD cx, DWORD cy)
{
	// get a compatible DC
	m_bufPreview.hImageCache = CreateCompatibleDC(hDC);

	
	// Calculate required capture buffer sizes
	pBuf->cx = cx;
	pBuf->cy = cy;
	pBuf->cbBuf = pBuf->cx * pBuf->cy * 2;	// 16bpp
	
//	RETAILMSG(1,(TEXT("<CamTest>: Allocating DIB buffer x (%ux%u)\r\n"), pBuf->cx, pBuf->cy));
//printf("<CamTest>: Allocating DIB buffer x (%u:%ux%u:%u)\r\n", pBuf->cx, cx, pBuf->cy, cy);
	DWORD inverseHeight = 0 - pBuf->cy;

	for(int loop=0;loop<NUM_BUFFERS;loop++)
	{
		// Allocate the required number of DIB buffers
		pBuf->hBitmap[loop] = CreateBltDIB (hDC, pBuf->cx, inverseHeight, (PVOID *)&pBuf->pBitmap[loop]);
		if (pBuf->hBitmap[loop] == 0) 
		{
			RETAILMSG(1,(TEXT("<CamTest>: Capture DIB CreateBltDIB failed (%u)\r\n"), GetLastError()));
//			printf("<CamTest>: Capture DIB CreateBltDIB failed (%u)\r\n", GetLastError());
			return (FALSE);
		}
	}
	
	// Start with first DIB buffer
	pBuf->dwCurBuf = 0;
	pBuf->dwDispBuf = 1;

	return (TRUE);
}


/*
void PerformActions(HWND hWnd)
{
	switch(giActions++)
	{
		case 0:
		case 1:
		case 2:
			GetPreview();					// Get Preview image
			gbDisplayPreview = TRUE;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 3:
			GetStill();
			break;
		case 4:   
			DestroyWindow(hWnd);
			break;
	}
}
*/
///////////////////////////////////////////////////////////////////////////
//
// GetStill() - get a single image capture frame
//
BOOL CCamera::GetStill()
{
	VINFRAMEINFO vinFrame;
	DWORD cbRet;
	BOOL bHaveFrame = FALSE;

	if ( !DeviceIoControl(ghCam,IOCTL_OEM_CAM_START,NULL,0,NULL,0,&cbRet,NULL) ) 
	{
		RETAILMSG(1,(TEXT("<CamTest>: IOCTL_OEM_CAM_START failed (%u)\r\n"), GetLastError()));
		return (FALSE);
	}

	// Set up frame information structure
	memset (&vinFrame, 0, sizeof(VINFRAMEINFO));
	vinFrame.dwTimeout = 10000;
	vinFrame.dwWidth = m_bufCapture.cx;
	vinFrame.dwHeight = m_bufCapture.cy;

	if (DeviceIoControl (ghCam, IOCTL_OEM_CAM_CAPTURE, &vinFrame, sizeof(VINFRAMEINFO), 
			m_bufCapture.pBuf, m_bufCapture.cbBuf, &cbRet, NULL)) 
	{
		bHaveFrame = TRUE;

//		RETAILMSG(1,(TEXT("<CamTest>: dwDispBuf:%u, dwCurBuf:%u\r\n"), m_bufCapture.dwDispBuf, m_bufCapture.dwCurBuf));
		notifyAllOfNewImage();

	}

	return (bHaveFrame);
}

BOOL CCamera::InitCamera()
{
	SIZE	imgSize[2];
	DWORD	cbRet;
	SIZE	szCapture,szPreview;

	// Zero the camera structures
	memset(&m_bufPreview, 0, sizeof(m_bufPreview));
	memset(&m_bufCapture, 0, sizeof(m_bufCapture));

	gsizeX = 320;
	gsizeY = 240;

	m_posX = 0;
	m_posY = 0;

	// Attempt to open the camera driver
	ghCam = CreateFile (TEXT("CAM1:"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (ghCam == INVALID_HANDLE_VALUE) 
	{
	    MessageBox(NULL,L"Failed to open camera driver",L"Error",MB_OK);
        return FALSE;
	}

	// First stop and set up camera 
	if ( !DeviceIoControl(ghCam,IOCTL_OEM_CAM_STOP,NULL,0,NULL,0,&cbRet,NULL) ) 
	{
//		RETAILMSG(1,(TEXT("<CamTest>: IOCTL_OEM_CAM_STOP failed (%u)\r\n"), GetLastError()));
		return (FALSE);
	}

	// Set capture image size
	szCapture.cx = CAM_CAPTURE_CX;
	szCapture.cy = CAM_CAPTURE_CY;

	// Set preview image size
	szPreview.cx = gsizeX;
	szPreview.cy = gsizeY;

	// Pass format as an array of capture and preview window sizes
	imgSize[0] = szCapture;
	imgSize[1] = szPreview;

	if ( !DeviceIoControl(ghCam,IOCTL_OEM_CAM_SETFORMAT,&imgSize,sizeof(imgSize),NULL,0,&cbRet,NULL) ) 
	{
//		RETAILMSG(1,(TEXT("<CamTest>: IOCTL_OEM_CAM_SETFORMAT failed (%u)\r\n"), GetLastError()));
//		printf("<CamTest>: IOCTL_OEM_CAM_SETFORMAT failed (%u)\r\n", GetLastError());
		Sleep(5000);
		return (FALSE);
	}


	return TRUE;
}

void CCamera::takePicture()
{
//	MessageBox(NULL, TEXT("Test2.1"),TEXT("None"), MB_OK);
	if (GetPreview() == TRUE)
	{
//		MessageBox(NULL, L"NotifyAll", L"None", MB_OK);
		notifyAllOfNewImage();
	}
}

PBYTE  CCamera::getLastPreviewBuffer()
{
	//bufferArray = m_nLastPreviewBuffer;

	return m_bufPreview.pBitmap[0];
}
