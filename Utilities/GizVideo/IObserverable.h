// IObserverable.h: interface for the IObserverable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOBSERVERABLE_H__D575C1E6_95E5_4683_AF39_F0171BA8B5D7__INCLUDED_)
#define AFX_IOBSERVERABLE_H__D575C1E6_95E5_4683_AF39_F0171BA8B5D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NUM_OBSERVERS_MAX 10
#include "IObserver.h"
class IObserver; 

class IObserverable  
{
public:
	IObserverable();
	virtual  ~IObserverable();

	void addObserver(IObserver* io);
	void removeObserver(IObserver * io);
	void notifyAllOfNewImage();
private: 
	int m_nCountCurrentObservers;
	IObserver* currentOberserverables[NUM_OBSERVERS_MAX];
	int findObserverInIndex(IObserver * io);
	
};

#endif // !defined(AFX_IOBSERVERABLE_H__D575C1E6_95E5_4683_AF39_F0171BA8B5D7__INCLUDED_)
