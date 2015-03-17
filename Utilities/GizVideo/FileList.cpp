// FileList.cpp: implementation of the FileList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileList.h"
#include "FileListDataItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FileList::FileList()
{

	m_lDrawList.push_back(new FileListDataItem("[...]", "Record New File", TRUE));
	findFiles();
}

FileList::~FileList()
{
//TODO: research if garbage collection is needed
//	delete m_lDrawList;
}

FileListDataItem* FileList::getCurrentFilesShown()
{
	FileListDataItem* result;
	
	list<FileListDataItem*>::const_iterator it;

	//set current selection
		it= m_lDrawList.begin();
		for (int run=1; run < m_nCurrentPosition; run++)
		{
			++it;
		}
		

	//If the file list is past 3 max 
	if (m_nCurrentPosition + 3 >= m_lDrawList.size())
	{
		//TODO: Can be simplified
		int nItemsAtTheEnd= m_lDrawList.size()- m_nCurrentPosition;

		

		//handle size-m_nCurrrentPosition
		for (int i=0; i < nItemsAtTheEnd; i++)
		{
			result= *it;
			result++;
			it++;

			
		}

		it= m_lDrawList.begin() ;

		//handle size-m_nCurrrentPosition
		for (int d=0; d < (3-nItemsAtTheEnd); d++)
		{
			result= *it;
			result++;
			it++;
		}

		//handle 3- previous amount
	}
	else
	{
		//get current list
		for (int i = 0; i < 3 && i < m_lDrawList.size(); i++)
		{
		    result= *it;
			result++;
			it++;
		}	
	}

	return result;
}

void FileList::moveNext()
{
	m_nCurrentPosition= ++m_nCurrentPosition % m_lDrawList.size();
}

void FileList::movePrevious()
{
	m_nCurrentPosition= --m_nCurrentPosition % m_lDrawList.size();
}

void FileList::findFiles()
{
	WIN32_FIND_DATA data;
	HANDLE hFiles= FindFirstFileEx(L"Videos\\*.avi", FindExInfoStandard, &data, FindExSearchNameMatch, NULL, 0);

	if (hFiles != INVALID_HANDLE_VALUE)
	{
		do {
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				char* convert = "";
				sprintf(convert, "%s", data.cFileName);

				m_lDrawList.push_back(new FileListDataItem(convert, convert, FALSE));
			}
		} while ( FindNextFile(hFiles, &data) );

		FindClose(hFiles);
	}
}
