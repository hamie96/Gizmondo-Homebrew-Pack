// BackgroundInterfaceDrawable.h: interface for the BackgroundInterfaceDrawable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKGROUNDINTERFACEDRAWABLE_H__6E7F08BC_EC37_443B_9E8F_47A0739CEDD8__INCLUDED_)
#define AFX_BACKGROUNDINTERFACEDRAWABLE_H__6E7F08BC_EC37_443B_9E8F_47A0739CEDD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ButtonInputDrawableTemplate.h"
#include "InputButtonState.h"
#include "IDrawable.h"

class BackgroundInterface : public IDrawable
{
public:
	BackgroundInterface(IDrawable* background, ButtonInputDrawableTemplate* buttonInputDraw, InputButtonState* currentInputButtonState);
	ButtonInputDrawableTemplate* getButtonInputDrawableTemplate();
	InputButtonState* getInputButtonState();
	IDrawable* getBackground();
	IDrawable* getExtraDrawable();
	BackgroundInterface();
	virtual ~BackgroundInterface();
	void Paint(HDC& hDC, RECT& rt);
protected:
	ButtonInputDrawableTemplate* m_bidtCurrentDrawableInputState;
	InputButtonState* m_ibsCurrentButtonState;
	IDrawable* m_drBackgroundDrawable;
	IDrawable* m_drExtraDrawableItems;
};

#endif // !defined(AFX_BACKGROUNDINTERFACEDRAWABLE_H__6E7F08BC_EC37_443B_9E8F_47A0739CEDD8__INCLUDED_)
