// ListItem.h: interface for the ListItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTITEM_H__B2058715_70A2_41C4_8B08_421DCBD3EB68__INCLUDED_)
#define AFX_LISTITEM_H__B2058715_70A2_41C4_8B08_421DCBD3EB68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "IDrawable.h"
#include "FileListDataItem.h"

class ListItem : public IDrawable  
{
public:
	ListItem(FileListDataItem* fld);
	virtual ~ListItem();
	void Paint(HDC& hDC, RECT &rt);

private:
	FileListDataItem* m_flDataItem;
};

#endif // !defined(AFX_LISTITEM_H__B2058715_70A2_41C4_8B08_421DCBD3EB68__INCLUDED_)
