// DrawList.cpp: implementation of the CDrawList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawList::CDrawList()
{

}

CDrawList::~CDrawList()
{

}

void CDrawList::add(IDrawable *idraw)
{
	m_lDrawList.push_back(idraw);
}

void CDrawList::remove(IDrawable *idraw)
{

}

void CDrawList::Paint(HDC& hDC, RECT& rt)
{
	list<IDrawable*>::const_iterator it;

	for (it= m_lDrawList.begin(); it!= m_lDrawList.end(); ++it)
	{
	//	*it == *IDrawable
		(*it)->Paint(hDC, rt);
	}

}