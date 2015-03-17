// MainBackgroundDrawable.cpp: implementation of the MainBackgroundDrawable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainBackgroundDrawable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MainBackgroundDrawable::MainBackgroundDrawable()
{
	m_hBitmap= 
		SHLoadDIBitmap(TEXT("\\SD CARD\\GizVideo\\background.bmp"));
}

MainBackgroundDrawable::~MainBackgroundDrawable()
{

	DeleteObject(m_hBitmap);
}

void MainBackgroundDrawable::Paint(HDC& hDC, RECT &rt)
{

	
	
	HDC mem= CreateCompatibleDC(hDC);

	SelectObject(mem, m_hBitmap);

	BITMAP bmp;

	GetObject(m_hBitmap, sizeof(BITMAP), &bmp);

	BitBlt(hDC, getX(),getY(),bmp.bmWidth,bmp.bmHeight, mem, 0,0, SRCCOPY);
	DeleteDC(mem);
	

}