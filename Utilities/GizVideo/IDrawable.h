// IDrawable.h: interface for the IDrawable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDRAWABLE_H__678F67ED_53BF_4A67_BB8B_203CA18951B7__INCLUDED_)
#define AFX_IDRAWABLE_H__678F67ED_53BF_4A67_BB8B_203CA18951B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IDrawable  
{
public:
	void setY(int y);
	void setX(int x);
	int getY();
	int getX();
	
	IDrawable();
	virtual ~IDrawable();
	virtual void Paint(HDC& hDC, RECT& rt) =0;


protected:
	int m_nX;
	int m_nY;
};

#endif // !defined(AFX_IDRAWABLE_H__678F67ED_53BF_4A67_BB8B_203CA18951B7__INCLUDED_)
