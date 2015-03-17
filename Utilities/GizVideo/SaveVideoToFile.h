// SaveVideoToFile.h: interface for the CSaveVideoToFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVEVIDEOTOFILE_H__BE6FF9AF_CD2E_49C7_A7F9_0422B19D19AA__INCLUDED_)
#define AFX_SAVEVIDEOTOFILE_H__BE6FF9AF_CD2E_49C7_A7F9_0422B19D19AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IObserver.h"

//#include "memory.h"
//#include "OEMINC.h"
//#include <winioctl.h>
#include "AviFile.h"

class CSaveVideoToFile   : public IObserver  
{
public:
	CSaveVideoToFile(CCamera* io, char* fName);
	virtual ~CSaveVideoToFile();
	void updateImage();
private:
	AviFile* m_cAviFile;
	
	void prepNewFrame();
	short* convertFrameToAppropritateAviFrame(short* frame);
	CCamera* m_cCurrentCamera;

protected:
	short RGB565toRGB555(short orgColour);
	
};

#endif // !defined(AFX_SAVEVIDEOTOFILE_H__BE6FF9AF_CD2E_49C7_A7F9_0422B19D19AA__INCLUDED_)
