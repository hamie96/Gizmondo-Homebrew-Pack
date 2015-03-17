// ButtonDrawable.cpp: implementation of the ButtonDrawable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ButtonDrawable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ButtonDrawable::ButtonDrawable(int x, int y, LPCTSTR text, LPCTSTR url)
{
	
	setX(x);
	setY(y);
	m_strText= text;
	m_hBitmap= 
		SHLoadDIBitmap(url);
}

ButtonDrawable::~ButtonDrawable()
{
	DeleteObject(m_hBitmap);
}


void ButtonDrawable::Paint(HDC& hDC, RECT &rt)
{		
	HDC mem= CreateCompatibleDC(hDC);

	SelectObject(mem, m_hBitmap);

	BITMAP bmp;

	GetObject(m_hBitmap, sizeof(BITMAP), &bmp);

	BitBlt(hDC, getX(),getY(),bmp.bmWidth,bmp.bmHeight, mem, 0,0, SRCCOPY);
	DeleteDC(mem);
	

	SetTextColor(hDC, RGB(0,0,0));

	RECT textPos= 
	{
		getX()+ bmp.bmWidth, getY(), 
			getX()+ bmp.bmWidth + (int)(5.3* lstrlen(m_strText)), 
			getY()+ bmp.bmHeight

	};
	
	DrawText(hDC, m_strText, 
		lstrlen(m_strText), &textPos, 
		DT_SINGLELINE | DT_VCENTER | DT_LEFT);
}
