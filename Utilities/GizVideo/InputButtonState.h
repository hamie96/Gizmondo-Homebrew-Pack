// InputButtonState.h: interface for the InputButtonState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPUTBUTTONSTATE_H__901C1F51_7E48_4139_895E_253DFA57804E__INCLUDED_)
#define AFX_INPUTBUTTONSTATE_H__901C1F51_7E48_4139_895E_253DFA57804E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class InputButtonState  
{
public:
	virtual void pressPlay()= 0;
	virtual void pressFastForward()= 0;
	virtual void pressRewind()= 0;
	virtual void pressStop()= 0;
	virtual void pressRight()= 0;
	virtual void pressLeft()= 0;
	virtual void pressDown()= 0;
	virtual void pressUp()= 0;
	InputButtonState();
	virtual ~InputButtonState();

};

#endif // !defined(AFX_INPUTBUTTONSTATE_H__901C1F51_7E48_4139_895E_253DFA57804E__INCLUDED_)
