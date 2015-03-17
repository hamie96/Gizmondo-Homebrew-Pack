// IObserverable.cpp: implementation of the IObserverable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IObserverable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IObserverable::IObserverable()
{
	m_nCountCurrentObservers=0;
}

IObserverable::~IObserverable()
{

}


void IObserverable::addObserver(IObserver *io)
{
	if (m_nCountCurrentObservers < NUM_OBSERVERS_MAX)
	{
		currentOberserverables[m_nCountCurrentObservers++]= 
			io;
	}
}

void IObserverable::removeObserver(IObserver * io)
{
	int nPosition= findObserverInIndex(io);

	if (nPosition != -1)
	{
		m_nCountCurrentObservers--;

		currentOberserverables[nPosition] = 
			currentOberserverables[m_nCountCurrentObservers];
		currentOberserverables[m_nCountCurrentObservers] = NULL;
		
	}
}
	
	
void IObserverable::notifyAllOfNewImage()
{
	for (int i=0; i < m_nCountCurrentObservers; i++)
	{
		IObserver * io = currentOberserverables[i];
		io->updateImage();
	}
}

int IObserverable::findObserverInIndex(IObserver * io)
	{
		int nResult = -1; 

		for (int i=0; i < m_nCountCurrentObservers && nResult != -1; i++)
		{
			if (currentOberserverables[i] == io)
			{
				nResult = i;
			}
		}

		return nResult;
	}