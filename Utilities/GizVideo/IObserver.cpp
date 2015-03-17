// IObserver.cpp: implementation of the IObserver class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IObserver.h"
#include "Camera.h" 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IObserver::IObserver(CCamera* cc)
{
	
	cc->addObserver(this);
	currentCameraSubject= cc;
}

IObserver::~IObserver()
{
	currentCameraSubject->removeObserver(this);
}


CCamera* IObserver::getCurrentCamera()
{
	return currentCameraSubject;
}


