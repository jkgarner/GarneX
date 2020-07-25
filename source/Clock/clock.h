// clock.h : main header file for the CLOCK application
//

#if !defined(AFX_CLOCK_H__433044E5_5C42_11D1_800A_0000C0491400__INCLUDED_)
#define AFX_CLOCK_H__433044E5_5C42_11D1_800A_0000C0491400__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "ClockWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CClockApp:
// See clock.cpp for the implementation of this class
//

class CClockApp : public CWinApp
{
public:
	CClockApp();
	CClockWnd *m_clock;
	HICON m_hIcon;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClockApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CClockApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLOCK_H__433044E5_5C42_11D1_800A_0000C0491400__INCLUDED_)
