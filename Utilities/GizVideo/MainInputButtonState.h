// MainInputButtonState.h: interface for the MainInputButtonState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAININPUTBUTTONSTATE_H__EAB56452_9A2E_4720_9F3A_AC32DEC335C9__INCLUDED_)
#define AFX_MAININPUTBUTTONSTATE_H__EAB56452_9A2E_4720_9F3A_AC32DEC335C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InputButtonState.h"

class MainInputButtonState : public InputButtonState  
{
public:
	MainInputButtonState();
	virtual ~MainInputButtonState();

	void pressPlay();
	void pressFastForward();
	void pressRewind();
	void pressStop();
	void pressRight();
	void pressLeft();
	void pressDown();
	void pressUp();
};

#endif // !defined(AFX_MAININPUTBUTTONSTATE_H__EAB56452_9A2E_4720_9F3A_AC32DEC335C9__INCLUDED_)
