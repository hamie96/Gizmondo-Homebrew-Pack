// BackgroundInterfaceDrawable.cpp: implementation of the BackgroundInterfaceDrawable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BackgroundInterfaceDrawable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BackgroundInterface::BackgroundInterface()
{
	m_bidtCurrentDrawableInputState= NULL;
	m_drBackgroundDrawable= NULL;
	m_ibsCurrentButtonState= NULL;
}

BackgroundInterface::~BackgroundInterface(){
	delete m_drBackgroundDrawable;
	delete m_ibsCurrentButtonState;
	delete m_bidtCurrentDrawableInputState;
}

IDrawable* BackgroundInterface::getBackground()
{
	return m_drBackgroundDrawable;
}

InputButtonState* BackgroundInterface::getInputButtonState()
{
	return m_ibsCurrentButtonState;
}

ButtonInputDrawableTemplate* BackgroundInterface::getButtonInputDrawableTemplate()
{
	return m_bidtCurrentDrawableInputState;
}

BackgroundInterface::BackgroundInterface(IDrawable *background, ButtonInputDrawableTemplate *buttonInputDraw, InputButtonState *currentInputButtonState)
{
	m_bidtCurrentDrawableInputState= buttonInputDraw;
	m_drBackgroundDrawable= background;
	m_ibsCurrentButtonState= currentInputButtonState;

}

void BackgroundInterface::Paint(HDC& hDC, RECT& rt)
{
	if (m_drBackgroundDrawable != NULL) {
		m_drBackgroundDrawable->Paint(hDC, rt);
	}

	if (m_bidtCurrentDrawableInputState != NULL)
	{
		m_bidtCurrentDrawableInputState->Paint(hDC, rt);
	}

	if (m_drExtraDrawableItems != NULL)
	{
		m_drExtraDrawableItems->Paint(hDC, rt);
	}
	else { printf("ExtraDrawablesIsEmpty\n"); }
}

IDrawable* BackgroundInterface::getExtraDrawable()
{
	return m_drExtraDrawableItems;
}