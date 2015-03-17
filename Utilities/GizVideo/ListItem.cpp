// ListItem.cpp: implementation of the ListItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ListItem.h"
#include <string>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ListItem::ListItem(FileListDataItem* fld)
{
	m_flDataItem= fld;
}

ListItem::~ListItem()
{

}

void ListItem::Paint(HDC &hDC, RECT &rt)
{
	RECT rtc;
	rtc.top= getY();
	rtc.left= getX();
	rtc.bottom= getY()+20;
	rtc.right= 320;

	printf("ListItem: %s\n", m_flDataItem->getShortName());
	
	if (m_flDataItem->getIsItemSelected())
	{
		FillRect(hDC, &rtc, CreateSolidBrush(RGB(0,0,255)));
	}

	LPCTSTR test=(LPCTSTR)malloc(sizeof(LPCTSTR)*300);

	int nSize= wcstombs(m_flDataItem->getShortName(), test, 300);

	printf("%s %s\n", m_flDataItem->getShortName(), test);

	DrawText(hDC, test, nSize, 
		&rtc, 0);


	
}