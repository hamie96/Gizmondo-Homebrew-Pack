// IDrawable.cpp: implementation of the IDrawable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IDrawable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IDrawable::IDrawable()
{
	m_nX= 0;
	m_nY= 0;
}

IDrawable::~IDrawable()
{

}


int IDrawable::getX()
{
	return m_nX;

}

int IDrawable::getY()
{
return m_nY;
}

void IDrawable::setX(int x)
{
	m_nX= x;

}

void IDrawable::setY(int y)
{
	m_nY= y;
}
