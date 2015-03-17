// MainBackgroundDrawable.h: interface for the MainBackgroundDrawable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINBACKGROUNDDRAWABLE_H__E123C65F_E123_41B1_9282_BA206A57E9F2__INCLUDED_)
#define AFX_MAINBACKGROUNDDRAWABLE_H__E123C65F_E123_41B1_9282_BA206A57E9F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IDrawable.h"

class MainBackgroundDrawable : public IDrawable  
{
public:

	MainBackgroundDrawable();
	virtual ~MainBackgroundDrawable();
	void Paint(HDC& hDC, RECT& rt);

private:
		HBITMAP m_hBitmap;
};

#endif // !defined(AFX_MAINBACKGROUNDDRAWABLE_H__E123C65F_E123_41B1_9282_BA206A57E9F2__INCLUDED_)
