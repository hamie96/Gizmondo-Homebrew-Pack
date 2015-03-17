// FileList.h: interface for the FileList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILELIST_H__D97576A1_7647_4D0C_8012_75442F4B6482__INCLUDED_)
#define AFX_FILELIST_H__D97576A1_7647_4D0C_8012_75442F4B6482__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "FileListDataItem.h"
#include <list>
using namespace std;

class FileList  
{
public:
	void movePrevious();
	void moveNext();
	
	FileListDataItem* getCurrentFilesShown();
	FileList();
	virtual ~FileList();
private: 
	void findFiles();
	
	int m_nCurrentPosition;
	list<FileListDataItem*> m_lDrawList;
};

#endif // !defined(AFX_FILELIST_H__D97576A1_7647_4D0C_8012_75442F4B6482__INCLUDED_)
