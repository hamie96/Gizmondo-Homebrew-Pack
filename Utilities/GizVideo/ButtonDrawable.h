// ButtonDrawable.h: interface for the ButtonDrawable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUTTONDRAWABLE_H__D280BACB_AD64_480C_B178_2172850FDDA3__INCLUDED_)
#define AFX_BUTTONDRAWABLE_H__D280BACB_AD64_480C_B178_2172850FDDA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IDrawable.h"
#include <string.h>

class ButtonDrawable : public IDrawable
{
public:
	ButtonDrawable(int x, int y, LPCTSTR text, LPCTSTR url);
	virtual ~ButtonDrawable();
	void Paint(HDC& hDC, RECT &rt);
private:
	LPCTSTR m_strText;
	HBITMAP m_hBitmap;
};

#endif // !defined(AFX_BUTTONDRAWABLE_H__D280BACB_AD64_480C_B178_2172850FDDA3__INCLUDED_)
