// MainInterfaceDrawable.cpp: implementation of the MainInterfaceDrawable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainInterfaceDrawable.h"
#include "MainBackgroundDrawable.h"
#include "MainInputButtonState.h"
#include "MainInputButtonDrawable.h"
#include "ButtonDrawable.h"
#include "ListItem.h" 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MainInterface::MainInterface() : BackgroundInterface(new MainBackgroundDrawable(), 
				new MainInputButtonDrawable(170, 44), new MainInputButtonState())
{

    if (m_bidtCurrentDrawableInputState != NULL)
	{

		m_bidtCurrentDrawableInputState->add(
			new ButtonDrawable(-1, -1, TEXT("Exit"), TEXT("\\SD CARD\\GizVideo\\stopbutton.bmp")),
			45, 0);

		m_bidtCurrentDrawableInputState->add(
			new ButtonDrawable(-1, -1, TEXT("Select"), TEXT("\\SD CARD\\GizVideo\\playbutton.bmp")),
			45, 60);

		
		m_bidtCurrentDrawableInputState->add(
			new ButtonDrawable(-1, -1, TEXT("Up List"), TEXT("\\SD CARD\\GizVideo\\forwardbutton.bmp")),
			80, 30);


		m_bidtCurrentDrawableInputState->add(
			new ButtonDrawable(-1, -1, TEXT("Down List"), TEXT("\\SD CARD\\GizVideo\\rewindbutton.bmp")),
			0, 30);
	}
	else
	{ printf("Error: Input Button drawable is null\n"); }

	
	m_curFileList= new FileList();
	
}

MainInterface::~MainInterface()
{
	delete m_curFileList;
}


void MainInterface::Paint(HDC& hDC, RECT& rt)
{
	if (m_drExtraDrawableItems != NULL)
	{
		delete m_drExtraDrawableItems;
	}

	
	//m_drExtraDrawableItems= new CDrawList();

	m_drExtraDrawableItems= new ListItem(new FileListDataItem("FileListingWill Be available later", "FileListingWill Be available later", TRUE));

	m_drExtraDrawableItems->setY(200);
	printf("PreAdd\n");
	
	/*
	FileListDataItem* fileList= 
		m_curFileList->getCurrentFilesShown();

	while (fileList != NULL)
	{
		((CDrawList*)m_drExtraDrawableItems)->add(new ListItem(&*fileList));

		fileList++;
		printf("getExtraDrawable: Loop should be 3\n");
	}
*/
	
//	((CDrawList*)m_drExtraDrawableItems)->add(new ListItem(new FileListDataItem("Testing1","T1",TRUE)));
//	((CDrawList*)m_drExtraDrawableItems)->add(new ListItem(new FileListDataItem("Testing1","T2",FALSE)));

	printf("Afteradd\n");
	BackgroundInterface::Paint(hDC, rt);
}
