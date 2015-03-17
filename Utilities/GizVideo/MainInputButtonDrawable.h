// MainInputButtonDrawable.h: interface for the MainInputButtonDrawable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAININPUTBUTTONDRAWABLE_H__7035A8F2_E45C_4911_9181_570159E20E34__INCLUDED_)
#define AFX_MAININPUTBUTTONDRAWABLE_H__7035A8F2_E45C_4911_9181_570159E20E34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ButtonInputDrawableTemplate.h"

class MainInputButtonDrawable : public ButtonInputDrawableTemplate  
{
public:
	MainInputButtonDrawable();
	MainInputButtonDrawable(int nx, int ny);
	virtual ~MainInputButtonDrawable();
//	void Paint(HDC& hDC, RECT& rt);
};

#endif // !defined(AFX_MAININPUTBUTTONDRAWABLE_H__7035A8F2_E45C_4911_9181_570159E20E34__INCLUDED_)
