// ButtonInputDrawableTemplate.cpp: implementation of the ButtonInputDrawableTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ButtonInputDrawableTemplate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ButtonInputDrawableTemplate::ButtonInputDrawableTemplate()
{
	m_nX= 0;
	m_nY= 0;
	m_dlListOfInputVisuals= new CDrawList();
}

ButtonInputDrawableTemplate::ButtonInputDrawableTemplate(int nx, int ny)
{
	m_nX= nx;
	m_nY= ny;
	m_dlListOfInputVisuals= new CDrawList();
}

ButtonInputDrawableTemplate::~ButtonInputDrawableTemplate()
{
	delete m_dlListOfInputVisuals;
}

void ButtonInputDrawableTemplate::Paint(HDC& hDC, RECT& rt)
{
	m_dlListOfInputVisuals->Paint(hDC, rt);
}

void ButtonInputDrawableTemplate::add(IDrawable* newSprite, int nXOffset, int nYOffset)
{
	newSprite->setX(nXOffset+ m_nX);
	newSprite->setY(m_nY+ nYOffset);

	m_dlListOfInputVisuals->add(newSprite);
}

void ButtonInputDrawableTemplate::remove(IDrawable* item)
{
	m_dlListOfInputVisuals->remove(item);
}