// MainInterfaceDrawable.h: interface for the MainInterfaceDrawable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAININTERFACEDRAWABLE_H__08CE88FC_58E0_43EB_BEA9_A88E9DA9EE3C__INCLUDED_)
#define AFX_MAININTERFACEDRAWABLE_H__08CE88FC_58E0_43EB_BEA9_A88E9DA9EE3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BackgroundInterfaceDrawable.h"
#include "FileList.h"

class MainInterface : public BackgroundInterface
{
public:
	MainInterface();
	virtual ~MainInterface();
	void Paint(HDC& hDC, RECT& rt);
private:
	FileList* m_curFileList;

};

#endif // !defined(AFX_MAININTERFACEDRAWABLE_H__08CE88FC_58E0_43EB_BEA9_A88E9DA9EE3C__INCLUDED_)
