// DrawList.h: interface for the CDrawList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWLIST_H__612E74C3_9E5E_49CB_AF83_3665D17830B2__INCLUDED_)
#define AFX_DRAWLIST_H__612E74C3_9E5E_49CB_AF83_3665D17830B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IDrawable.h"
#include <list>
using namespace std;

class CDrawList: public IDrawable
{
public:
	void remove(IDrawable* idraw);
	void add(IDrawable * idraw);
	CDrawList();
	virtual ~CDrawList();
	void Paint(HDC& hDC, RECT& rt);

private: 
	list<IDrawable*> m_lDrawList;
};

#endif // !defined(AFX_DRAWLIST_H__612E74C3_9E5E_49CB_AF83_3665D17830B2__INCLUDED_)
