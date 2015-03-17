// IObserver.h: interface for the IObserver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOBSERVER_H__7790836E_FCA4_48E0_A969_C664F8167BC7__INCLUDED_)
#define AFX_IOBSERVER_H__7790836E_FCA4_48E0_A969_C664F8167BC7__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Camera.h"
class CCamera;

class IObserver  
{
public:
	IObserver(CCamera* cc);
	virtual ~IObserver();
	virtual void updateImage() = 0 ;

//	

protected:
	
	CCamera* getCurrentCamera();
	
private:
	CCamera* currentCameraSubject;
};

#endif // !defined(AFX_IOBSERVER_H__7790836E_FCA4_48E0_A969_C664F8167BC7__INCLUDED_)

