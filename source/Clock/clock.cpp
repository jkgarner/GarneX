// clock.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "clock.h"
#include "clockDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClockApp

BEGIN_MESSAGE_MAP(CClockApp, CWinApp)
	//{{AFX_MSG_MAP(CClockApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClockApp construction

CClockApp::CClockApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CClockApp object

CClockApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CClockApp initialization

BOOL CClockApp::InitInstance()
{
	SetRegistryKey(_T("Garnex"));	

	m_clock = new CClockWnd;
	CRect tempRect(0,0,100,100);
	if (!m_clock->Create(AfxRegisterWndClass(0), _T("Garnex"),
						 WS_VISIBLE,
						 tempRect, NULL, NULL,
						 WS_EX_TOOLWINDOW |
						 WS_EX_DLGMODALFRAME,
						 NULL))
	{
		delete m_clock;
		m_clock = 0;
		return FALSE;
	}

	m_pMainWnd = m_clock;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CClockApp::ExitInstance() 
{
	return CWinApp::ExitInstance();
}
