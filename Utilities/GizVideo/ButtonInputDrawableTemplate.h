// ButtonInputDrawableTemplate.h: interface for the ButtonInputDrawableTemplate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUTTONINPUTDRAWABLETEMPLATE_H__3F041FCC_59EA_406C_8BCE_C1BE73BE28A8__INCLUDED_)
#define AFX_BUTTONINPUTDRAWABLETEMPLATE_H__3F041FCC_59EA_406C_8BCE_C1BE73BE28A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IDrawable.h"
#include "DrawList.h"

class ButtonInputDrawableTemplate : public IDrawable
{
public:
	CDrawList* m_dlListOfInputVisuals;
	ButtonInputDrawableTemplate();
	ButtonInputDrawableTemplate(int nx, int ny);

	virtual ~ButtonInputDrawableTemplate();
	void Paint(HDC& hDC, RECT& rt);
	void add(IDrawable* newSprite, int nXOffset, int nYOffset);
	void remove(IDrawable* item);

};

#endif // !defined(AFX_BUTTONINPUTDRAWABLETEMPLATE_H__3F041FCC_59EA_406C_8BCE_C1BE73BE28A8__INCLUDED_)
