// FileListDataItem.cpp: implementation of the FileListDataItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileListDataItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FileListDataItem::FileListDataItem(char* fullName, char* shortName, BOOL isSelected)
{
printf("ditemStart\n");
	m_strFullPath= fullName;
	m_strShortName= shortName;
	m_isSelected= isSelected;
	printf("ditemStop\n");
}

FileListDataItem::~FileListDataItem()
{
	delete m_strFullPath;
	delete m_strShortName;

}

char* FileListDataItem::getFullPath()
{
	return m_strFullPath;
}


char* FileListDataItem::getShortName()
{
	return m_strShortName;
}

BOOL FileListDataItem::getIsItemSelected()
{
	return m_isSelected;
}
