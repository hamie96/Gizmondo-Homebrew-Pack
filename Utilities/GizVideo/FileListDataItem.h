// FileListDataItem.h: interface for the FileListDataItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILELISTDATAITEM_H__69E5D67A_BD47_4BF8_BB06_CA1B3A9EE9A7__INCLUDED_)
#define AFX_FILELISTDATAITEM_H__69E5D67A_BD47_4BF8_BB06_CA1B3A9EE9A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class FileListDataItem  
{
public:
	char* getFullPath();
	char* getShortName();
	BOOL getIsItemSelected();

	FileListDataItem(char* fullName, char* shortName, BOOL isSelected);
	virtual ~FileListDataItem();

private:
	BOOL m_isSelected;
	char* m_strShortName;
	char* m_strFullPath;
};

#endif // !defined(AFX_FILELISTDATAITEM_H__69E5D67A_BD47_4BF8_BB06_CA1B3A9EE9A7__INCLUDED_)
