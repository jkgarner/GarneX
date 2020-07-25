// ThemeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "clock.h"
#include "ThemeDlg.h"
#include "clockdlg.h"
#include "ClockWnd.h"

//THEME: display state varables
bool CThemeDlg::m_rotate     = true;
bool CThemeDlg::m_roman      = true;
bool CThemeDlg::m_quadOnly   = false;
bool CThemeDlg::m_noTitle    = true;
bool CThemeDlg::m_sysColor   = false;
bool CThemeDlg::m_smooth     = true;

//THEME: positioning variables
int CThemeDlg::m_x           = 100;
int CThemeDlg::m_y           = 100;
int CThemeDlg::m_cx          = 400;
int CThemeDlg::m_cy          = 400;

//THEME: color variables
COLORREF CThemeDlg::m_bkColor    = RGB(255,255,255);
COLORREF CThemeDlg::m_handsColor = RGB(0,0,0);
COLORREF CThemeDlg::m_bdrColor   = RGB(0,0,0);
COLORREF CThemeDlg::m_tickColor  = RGB(128,128,128);
COLORREF CThemeDlg::m_tick5Color = RGB(0,0,0);
COLORREF CThemeDlg::m_fontColor  = RGB(0,0,0);
COLORREF CThemeDlg::m_scColor    = RGB(128,0,0);
COLORREF CThemeDlg::m_numColor   = RGB(0,0,0);

CString CThemeDlg::m_name = CString("");

// CThemeDlg dialog

IMPLEMENT_DYNAMIC(CThemeDlg, CDialog)

CThemeDlg::CThemeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThemeDlg::IDD, pParent)
{
	//local display state varables
	l_rotate = false;
	l_roman = false;
	l_quadOnly = false;
	l_noTitle = false;
	l_sysColor = false;
	l_smooth = false;

	//local positioning variables
	l_x = 0;
	l_y = 0;
	l_cx = 0;
	l_cy = 0;

	//local color variables
	l_bkColor = 0;
	l_handsColor = 0;
	l_bdrColor = 0;
	l_tickColor = 0;
	l_tick5Color = 0;
	l_fontColor = 0;
	l_scColor = 0;
	l_numColor = 0;
}

CThemeDlg::~CThemeDlg()
{
}

void CThemeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_THEMES, m_nameList);
	DDX_Control(pDX, IDC_ADDNEW, m_btnAddNew);
	DDX_Control(pDX, IDC_DELETE, m_btnDelete);
	DDX_CBString(pDX, IDC_THEMES, m_name);
}


BEGIN_MESSAGE_MAP(CThemeDlg, CDialog)
	ON_BN_CLICKED(IDC_ADDNEW, &CThemeDlg::OnBnClickedAddNew)
	ON_BN_CLICKED(IDC_DELETE, &CThemeDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDOK, &CThemeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CThemeDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_THEMES, &CThemeDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


BOOL CThemeDlg::OnInitDialog()
{
	if (CDialog::OnInitDialog())
	{
		m_name = GetCurrent();
		bool current = false;

        //get list of other stheemes...
        HKEY hKey;

        if( RegOpenKeyEx( HKEY_CURRENT_USER,
            TEXT("SOFTWARE\\Garnex\\Clock"),
            0,
            KEY_READ,
            &hKey) == ERROR_SUCCESS
          )
        {
            TCHAR    achKey[255];   // buffer for subkey name
            DWORD    cbName;                   // size of name string 
            TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
            DWORD    cchClassName = MAX_PATH;  // size of class string 
            DWORD    cSubKeys=0;               // number of subkeys 
            DWORD    cbMaxSubKey;              // longest subkey size 
            DWORD    cchMaxClass;              // longest class string 
            DWORD    cValues;              // number of values for key 
            DWORD    cchMaxValue;          // longest value name 
            DWORD    cbMaxValueData;       // longest value data 
            DWORD    cbSecurityDescriptor; // size of security descriptor 
            FILETIME ftLastWriteTime;      // last write time 

            DWORD i, retCode; 

            //TCHAR  achValue[255]; 
            DWORD cchValue = 255; 

            // Get the class name and the value count. 
            retCode = RegQueryInfoKey(
                hKey,                    // key handle 
                achClass,                // buffer for class name 
                &cchClassName,           // size of class string 
                NULL,                    // reserved 
                &cSubKeys,               // number of subkeys 
                &cbMaxSubKey,            // longest subkey size 
                &cchMaxClass,            // longest class string 
                &cValues,                // number of values for this key 
                &cchMaxValue,            // longest value name 
                &cbMaxValueData,         // longest value data 
                &cbSecurityDescriptor,   // security descriptor 
                &ftLastWriteTime);       // last write time 

            // Enumerate the subkeys, until RegEnumKeyEx fails.

            if (cSubKeys)
            {
                for (i=0; i<cSubKeys; i++) 
                { 
                    cbName = 255;
                    retCode = RegEnumKeyEx(hKey, i,
                             achKey, 
                             &cbName, 
                             NULL, 
                             NULL, 
                             NULL, 
                             &ftLastWriteTime); 
                    if (retCode == ERROR_SUCCESS) 
                    {
		                CString name = achKey;
                        if (name == m_name)
                        {
                            name += "(current)";
							current = true;
						}
						m_nameList.AddString(name);
                    }
                }
            } 
        }

		if (current)
		{
			m_btnAddNew.EnableWindow(current);
			m_btnDelete.EnableWindow(FALSE);
		}
		else
		{
			m_btnAddNew.EnableWindow(TRUE);
			m_btnDelete.EnableWindow(TRUE);
		}

		l_rotate = CThemeDlg::m_rotate;
		l_roman = CThemeDlg::m_roman;
		l_quadOnly = CThemeDlg::m_quadOnly;
		l_x = CThemeDlg::m_x;
		l_y = CThemeDlg::m_y;
		l_cx = CThemeDlg::m_cx;
		l_cy = CThemeDlg::m_cy;
		l_smooth = CThemeDlg::m_smooth;
		l_sysColor = CThemeDlg::m_sysColor;
		l_bkColor = CThemeDlg::m_bkColor;
		l_bdrColor = CThemeDlg::m_bdrColor;
		l_handsColor = CThemeDlg::m_handsColor;
		l_scColor = CThemeDlg::m_scColor;
		l_numColor = CThemeDlg::m_numColor;
		l_tick5Color = CThemeDlg::m_tick5Color;
		l_tickColor = CThemeDlg::m_tickColor;
		l_fontColor = CThemeDlg::m_fontColor;
		UpdateData(FALSE);

	}
	return TRUE;
}

void CThemeDlg::OnOK()
{
	UpdateData(TRUE);

	//THEME: display state varables
	m_rotate = l_rotate;
	m_roman = l_roman;
	m_quadOnly = l_quadOnly;
	m_noTitle = l_noTitle;
	m_sysColor = l_sysColor;
	m_smooth = l_smooth;

	//THEME: positi//local positioning variablesoning variables
	m_x = l_x;
	m_y = l_y;
	m_cx = l_cx;
	m_cy = l_cy;

	//THEME: color //local color variablesvariables
	m_bkColor = l_bkColor;
	m_handsColor = l_handsColor;
	m_bdrColor = l_bdrColor;
	m_tickColor = l_tickColor;
	m_tick5Color = l_tick5Color;
	m_fontColor = l_fontColor;
	m_scColor = l_scColor;
	m_numColor = l_numColor;

	CString name = m_name;
	if (m_name.Find(_T("(current)")) > -1)
	{
		name = m_name.Left(m_name.GetLength() - 9);
	}
	SaveLocalConfig(name);
	SaveCurrent(name);
	CDialog::OnOK();
}

void CThemeDlg::SaveCurrent(CString name)
{
	CRegKey *key =  new CRegKey(OpenKey(true));
	if (ERROR_SUCCESS != key->SetStringValue(_T("Current"), name ))
	{
		MessageBox(_T("Unable to save the current settings to the Registry."), _T("Error"),
			MB_OK|MB_ICONSTOP);
	}
	delete key;
}

// CThemeDlg message handlers

void CThemeDlg::OnBnClickedAddNew()
{
	UpdateData(TRUE);
	bool changed = false;
    bool current = false;
	CClockDlg dlg;
	CString name;

    if (m_name.Find(_T("(current)")) > -1)
    {
        name = m_name.Left(m_name.GetLength() - 9);
        current = true;
    }
	else
	{
		name = m_name;
	}

    LoadLocalConfig(name);
	dlg.m_name       = name;
	dlg.m_rotate	 = l_rotate;	
	dlg.m_roman		 = l_roman;		
	dlg.m_quad       = l_quadOnly;		
	dlg.m_x	         = l_x;	        
	dlg.m_y	         = l_y;	        
	dlg.m_cx	     = l_cx;	    
	dlg.m_cy	     = l_cy;	    
	dlg.m_smooth     = l_smooth;    
	dlg.m_sysColor   = l_sysColor;  
	dlg.m_bkColor    = l_bkColor;    
	dlg.m_bdrColor   = l_bdrColor;   
	dlg.m_handsColor = l_handsColor; 
	dlg.m_scColor    = l_scColor;    
	dlg.m_numColor   = l_numColor;   
	dlg.m_tick5Color = l_tick5Color; 
	dlg.m_tickColor  = l_tickColor;  
	dlg.m_fontColor  = l_fontColor;  

	dlg.DoModal();

	if (l_rotate != (dlg.m_rotate == TRUE) ? true : false
		|| l_roman != (dlg.m_roman == TRUE) ? true : false
		|| l_quadOnly != (dlg.m_quad == TRUE) ? true : false
		|| l_x != dlg.m_x
		|| l_y != dlg.m_y
		|| l_cx != dlg.m_cx
		|| l_cy != dlg.m_cy
		|| l_smooth != (dlg.m_smooth == TRUE) ? true : false
		|| l_sysColor != (dlg.m_sysColor == TRUE) ? true : false
		|| l_bkColor != dlg.m_bkColor
		|| l_bdrColor != dlg.m_bdrColor
		|| l_handsColor != dlg.m_handsColor
		|| l_scColor != dlg.m_scColor
		|| l_numColor != dlg.m_numColor
		|| l_tick5Color != dlg.m_tick5Color
		|| l_tickColor != dlg.m_tickColor
		|| l_fontColor != dlg.m_fontColor
		)
	{
		l_rotate = dlg.m_rotate;
		l_roman = dlg.m_roman;
		l_quadOnly = dlg.m_quad;
		l_x = dlg.m_x;
		l_y = dlg.m_y;
		l_cx = dlg.m_cx;
		l_cy = dlg.m_cy;
		l_smooth = dlg.m_smooth;
		l_sysColor = dlg.m_sysColor;
		l_bkColor = dlg.m_bkColor;
		l_bdrColor = dlg.m_bdrColor;
		l_handsColor = dlg.m_handsColor;
		l_scColor = dlg.m_scColor;
		l_numColor = dlg.m_numColor;
		l_tick5Color = dlg.m_tick5Color;
		l_tickColor = dlg.m_tickColor;
		l_fontColor = dlg.m_fontColor;
		name = dlg.m_name;

		bool newSize = (m_x != l_x || m_y != l_y || m_cx != l_cx || m_cy != l_cy);

		m_name = name;
		m_rotate = l_rotate;
		m_roman = l_roman;
		m_quadOnly = l_quadOnly;
		m_noTitle = l_noTitle;
		m_sysColor = l_sysColor;
		m_smooth = l_smooth;
		m_x = l_x;
		m_y = l_y;
		m_cx = l_cx;
		m_cy = l_cy;
		m_bkColor = l_bkColor;
		m_handsColor = l_handsColor;
		m_bdrColor = l_bdrColor;
		m_tickColor = l_tickColor;
		m_tick5Color = l_tick5Color;
		m_fontColor = l_fontColor;
		m_scColor = l_scColor;
		m_numColor = l_numColor;

		UpdateData(FALSE);

		//update name list...
		CString oldCurrent = GetCurrent();
		int oldSel = m_nameList.FindString(0, oldCurrent + "(current)");
		m_nameList.DeleteString(oldSel);
		m_nameList.AddString(oldCurrent);
		m_nameList.AddString(m_name + "(current)");
		int newSel = m_nameList.FindString(0, m_name + "(current)");
		m_nameList.SetCurSel(newSel);

		SaveCurrent(m_name);
		SaveConfig();

		//update window...
		CClockWnd* clock = (CClockWnd*)(AfxGetApp()->m_pMainWnd);
		if (newSize)
		{
			clock->MoveWindow(m_x, m_y, m_cx, m_cy);
		}
		clock->ReadConfig();
		clock->Invalidate();		
	}
}

void CThemeDlg::OnBnClickedDelete()
{
	UpdateData(TRUE);
	if (IDOK == MessageBox(_T("Are you sure you wish to delete the theme" + m_name + "? "), _T("Confirm Delete"),
		MB_ICONQUESTION | MB_OKCANCEL))
	{
		//saving a modified theme... delete existing
		HKEY key = AfxGetApp()->GetAppRegistryKey();
		AfxGetApp()->DelRegTree(key,m_name);
		int sel = m_nameList.FindString(0,m_name);
		m_nameList.DeleteString(sel);
		m_name = GetCurrent();
		CString name = m_name += "(current)";
		sel = m_nameList.FindString(0, name);
		m_nameList.SetCurSel(-1);
		UpdateData(FALSE);
	}
}

void CThemeDlg::OnBnClickedOk()
{
	OnOK();
}

void CThemeDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CThemeDlg::OnCbnSelchangeCombo1()
{
	UpdateData(TRUE);
	CString name = m_name;
	bool current = (m_name.Find(_T("(current)")) > -1);

	if (current)
		name = m_name.Left(m_name.GetLength() - 9);
	else
		name = m_name;

	m_btnAddNew.EnableWindow(TRUE);
	m_btnDelete.EnableWindow(current ? FALSE : TRUE);
	LoadLocalConfig(name);
}

HKEY CThemeDlg::OpenKey(bool write)
{
	HKEY hkey;
	if (write)
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER,
			TEXT("SOFTWARE\\Garnex\\Clock"),
			0,
			KEY_ALL_ACCESS,
			&hkey) == ERROR_SUCCESS
			)
			return hkey;
	}
	else
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER,
			TEXT("SOFTWARE\\Garnex\\Clock"),
			0,
			KEY_READ,
			&hkey) == ERROR_SUCCESS
			)
			return hkey;
	}
	return NULL;
}

HKEY CThemeDlg::OpenKey(CString themeName, bool write)
{
	if (themeName == "")
		return OpenKey(write);

	CStringW base = "SOFTWARE\\Garnex\\Clock";

	HKEY child;
	HKEY  parent = OpenKey(write);
	if (parent != NULL)
	{
		if (RegCreateKeyEx(parent, themeName, 0, 
			NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 
			NULL, &child, NULL) == ERROR_SUCCESS)
			return child;
	}
	return NULL;
}

void CThemeDlg::LoadConfig()
{
	bool loaded = false;	
	if (m_name == "")
	{
		CRegKey *key =  new CRegKey(OpenKey());
		if (key != NULL)
		{
			TCHAR curBuff[100];
			ULONG szCurBuff = 99;
			if (ERROR_SUCCESS != key->QueryStringValue(_T("Current"), curBuff, &szCurBuff))
			{
				m_name = _T("Default");
				m_rotate = 0;
				m_roman = 0;
				m_quadOnly = 1;
				m_x = 100;
				m_y = 100;
				m_cx = 400;
				m_cy = 400;
				m_smooth = 1;
				m_sysColor = 0;
				m_bkColor = RGB(255, 255, 255);
				m_bdrColor = RGB(0, 0, 0);
				m_handsColor = RGB(0, 0, 0);
				m_scColor = RGB(128, 0, 0);
				m_fontColor = RGB(192, 192, 192);
				m_tick5Color = RGB(128, 128, 128);
				m_tickColor = RGB(192, 192, 192);
				m_numColor = RGB(128, 128, 128);
				key->SetStringValue(_T("Current"), _T("Default"));
				loaded = true;
			}
			else
			{
				m_name = curBuff;
			}
			delete key;
		}
	}
	if (!loaded)
	{	
		CRegKey *key = new CRegKey(OpenKey(m_name));
		if (key != NULL)
		{
			//m_name = name;
			m_rotate = ReadProfileInt(key, _T("Rotate"), 0);
			m_roman = ReadProfileInt(key, _T("Roman"), 0);
			m_quadOnly = ReadProfileInt(key, _T("QuadOnly"), 1);
			m_x = ReadProfileInt(key, _T("X"), 100);
			m_y = ReadProfileInt(key, _T("Y"), 100);
			m_cx = ReadProfileInt(key, _T("cx"), 400);
			m_cy = ReadProfileInt(key, _T("cy"), 400);
			m_smooth = ReadProfileInt(key, _T("Smooth"), 1);
			m_sysColor = ReadProfileInt(key, _T("sysColor"), 0);

			if (m_sysColor)
			{
				m_bkColor = GetSysColor(COLOR_3DFACE);
				m_handsColor = GetSysColor(COLOR_BTNTEXT);
				m_bdrColor = GetSysColor(COLOR_3DSHADOW);
				m_tickColor = GetSysColor(COLOR_3DSHADOW);
				m_tick5Color = GetSysColor(COLOR_BTNTEXT);
				m_fontColor = GetSysColor(COLOR_BTNTEXT);
				m_scColor = GetSysColor(COLOR_3DSHADOW);
				m_numColor = GetSysColor(COLOR_BTNTEXT);
			}
			else
			{
				m_bkColor = ReadProfileInt(key, _T("Face"), RGB(255, 255, 255));
				m_bdrColor = ReadProfileInt(key, _T("Border"), RGB(0, 0, 0));
				m_handsColor = ReadProfileInt(key, _T("Hands"), RGB(0, 0, 0));
				m_scColor = ReadProfileInt(key, _T("SecondHand"), RGB(128, 0, 0));
				m_fontColor = ReadProfileInt(key, _T("Logo"), RGB(192, 192, 192));
				m_tick5Color = ReadProfileInt(key, _T("5Minute"), RGB(128, 128, 128));
				m_tickColor = ReadProfileInt(key, _T("1Minute"), RGB(192, 192, 192));
				m_numColor = ReadProfileInt(key, _T("Numbers"), RGB(128, 128, 128));
			}
			key->Close();
			delete key;
		}
	}
}

void CThemeDlg::SaveConfig()
{
	CString name = m_name;
    if (m_name.Find(_T("(current)")) > -1)
    {
        name = m_name.Left(m_name.GetLength() - 9);
    }

	CRegKey* key = new CRegKey(OpenKey(name,true));
	if (key != NULL)
	{
		WriteProfileInt(key, _T("NoTitle"), (m_noTitle) ? (1) : (0));
		WriteProfileInt(key, _T("Rotate"), (m_rotate) ? (1) : (0));
		WriteProfileInt(key, _T("Roman"), (m_roman) ? (1) : (0));
		WriteProfileInt(key, _T("QuadOnly"), (m_quadOnly) ? (1) : (0));
		WriteProfileInt(key, _T("X"), m_x);
		WriteProfileInt(key, _T("Y"), m_y);
		WriteProfileInt(key, _T("cx"), m_cx);
		WriteProfileInt(key, _T("cy"), m_cy);
		WriteProfileInt(key, _T("Smooth"), (m_smooth) ? (1) : (0));
		WriteProfileInt(key, _T("sysColor"), (m_sysColor) ? (1) : (0));

		if (!m_sysColor)
		{
			WriteProfileInt(key, _T("Face"), m_bkColor);
			WriteProfileInt(key, _T("Border"), m_bdrColor);
			WriteProfileInt(key, _T("Hands"), m_handsColor);
			WriteProfileInt(key, _T("SecondHand"), m_scColor);
			WriteProfileInt(key, _T("Logo"), m_fontColor);
			WriteProfileInt(key, _T("5Minute"), m_tick5Color);
			WriteProfileInt(key, _T("1Minute"), m_tickColor);
			WriteProfileInt(key, _T("Numbers"), m_numColor);
		}
		key->Close();
		delete key;
	}
}

void CThemeDlg::LoadLocalConfig(CString name)
{
	CRegKey* key = new CRegKey(OpenKey(name));
	if (key != NULL)
	{
		//m_name = name;
		l_rotate = ReadProfileInt(key, _T("Rotate"), 0);
		l_roman = ReadProfileInt(key, _T("Roman"), 0);
		l_quadOnly = ReadProfileInt(key, _T("QuadOnly"), 1);
		l_x = ReadProfileInt(key, _T("X"), 100);
		l_y = ReadProfileInt(key, _T("Y"), 100);
		l_cx = ReadProfileInt(key, _T("cx"), 400);
		l_cy = ReadProfileInt(key, _T("cy"), 400);
		l_smooth = ReadProfileInt(key, _T("Smooth"), 1);
		l_sysColor = ReadProfileInt(key, _T("sysColor"), 0);

		if (l_sysColor)
		{
			l_bkColor = GetSysColor(COLOR_3DFACE);
			l_handsColor = GetSysColor(COLOR_BTNTEXT);
			l_bdrColor = GetSysColor(COLOR_3DSHADOW);
			l_tickColor = GetSysColor(COLOR_3DSHADOW);
			l_tick5Color = GetSysColor(COLOR_BTNTEXT);
			l_fontColor = GetSysColor(COLOR_BTNTEXT);
			l_scColor = GetSysColor(COLOR_3DSHADOW);
			l_numColor = GetSysColor(COLOR_BTNTEXT);
		}
		else
		{
			l_bkColor = ReadProfileInt(key, _T("Face"), RGB(255, 255, 255));
			l_bdrColor = ReadProfileInt(key, _T("Border"), RGB(0, 0, 0));
			l_handsColor = ReadProfileInt(key, _T("Hands"), RGB(0, 0, 0));
			l_scColor = ReadProfileInt(key, _T("SecondHand"), RGB(128, 0, 0));
			l_fontColor = ReadProfileInt(key, _T("Logo"), RGB(192, 192, 192));
			l_tick5Color = ReadProfileInt(key, _T("5Minute"), RGB(128, 128, 128));
			l_tickColor = ReadProfileInt(key, _T("1Minute"), RGB(192, 192, 192));
			l_numColor = ReadProfileInt(key, _T("Numbers"), RGB(128, 128, 128));
		}
		key->Close();
		delete key;
	}
}

void CThemeDlg::SaveLocalConfig(CString name)
{
	CRegKey* key = new CRegKey(OpenKey(name, true));
	if (key != NULL)
	{
		WriteProfileInt(key, _T("NoTitle"), (l_noTitle) ? (1) : (0));
		WriteProfileInt(key, _T("Rotate"), (l_rotate) ? (1) : (0));
		WriteProfileInt(key, _T("Roman"), (l_roman) ? (1) : (0));
		WriteProfileInt(key, _T("QuadOnly"), (l_quadOnly) ? (1) : (0));
		WriteProfileInt(key, _T("X"), l_x);
		WriteProfileInt(key, _T("Y"), l_y);
		WriteProfileInt(key, _T("cx"), l_cx);
		WriteProfileInt(key, _T("cy"), l_cy);
		WriteProfileInt(key, _T("Smooth"), (l_smooth) ? (1) : (0));
		WriteProfileInt(key, _T("sysColor"), (l_sysColor) ? (1) : (0));

		if (!l_sysColor)
		{
			WriteProfileInt(key, _T("Face"), l_bkColor);
			WriteProfileInt(key, _T("Border"), l_bdrColor);
			WriteProfileInt(key, _T("Hands"), l_handsColor);
			WriteProfileInt(key, _T("SecondHand"), l_scColor);
			WriteProfileInt(key, _T("Logo"), l_fontColor);
			WriteProfileInt(key, _T("5Minute"), l_tick5Color);
			WriteProfileInt(key, _T("1Minute"), l_tickColor);
			WriteProfileInt(key, _T("Numbers"), l_numColor);
		}
		key->Close();
		delete key;
	}
}

bool CThemeDlg::CanUse(CString name)
{
	//get current value...
	CRegKey* key1 = new CRegKey();
	key1->Attach(OpenKey());

	TCHAR curBuff[100];
	ULONG szCurBuff = 99;
	if (ERROR_SUCCESS == key1->QueryStringValue(_T("Current"), curBuff, &szCurBuff))
	{
		CString current = curBuff;
		if (current == name)
			return false;
	}

	//get list of other themes...
	HKEY hKey = OpenKey();
	if (hKey != NULL)
	{
		TCHAR    achKey[255];   // buffer for subkey name
		DWORD    cbName;                   // size of name string 
		TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
		DWORD    cchClassName = MAX_PATH;  // size of class string 
		DWORD    cSubKeys = 0;               // number of subkeys 
		DWORD    cbMaxSubKey;              // longest subkey size 
		DWORD    cchMaxClass;              // longest class string 
		DWORD    cValues;              // number of values for key 
		DWORD    cchMaxValue;          // longest value name 
		DWORD    cbMaxValueData;       // longest value data 
		DWORD    cbSecurityDescriptor; // size of security descriptor 
		FILETIME ftLastWriteTime;      // last write time 

		DWORD i, retCode;

		//TCHAR  achValue[255]; 
		DWORD cchValue = 255;

		// Get the class name and the value count. 
		retCode = RegQueryInfoKey(
			hKey,                    // key handle 
			achClass,                // buffer for class name 
			&cchClassName,           // size of class string 
			NULL,                    // reserved 
			&cSubKeys,               // number of subkeys 
			&cbMaxSubKey,            // longest subkey size 
			&cchMaxClass,            // longest class string 
			&cValues,                // number of values for this key 
			&cchMaxValue,            // longest value name 
			&cbMaxValueData,         // longest value data 
			&cbSecurityDescriptor,   // security descriptor 
			&ftLastWriteTime);       // last write time 

		// Enumerate the subkeys, until RegEnumKeyEx fails.
		//or we fiunc a matching key...
		if (cSubKeys)
		{
			for (i = 0; i < cSubKeys; i++)
			{
				cbName = 255;
				retCode = RegEnumKeyEx(hKey, i,
					achKey,
					&cbName,
					NULL,
					NULL,
					NULL,
					&ftLastWriteTime);
				if (retCode == ERROR_SUCCESS)
				{
					CString test = achKey;
					if (name == test)
					{
						return false;
					}
				}
			}
		}
	}
	return true;
}

CString CThemeDlg::GetCurrent()
{
	//get current value...
	CRegKey* key1 = new CRegKey();
	key1->Attach(OpenKey());

	TCHAR curBuff[100];
	ULONG szCurBuff = 99;
	if (ERROR_SUCCESS == key1->QueryStringValue(_T("Current"), curBuff, &szCurBuff))
	{
		return CString(curBuff);
	}
	else
	{
		return CString("");
	}
}

bool CThemeDlg::WriteProfileInt(CRegKey *key, CString name, DWORD value)
{
	if (ERROR_SUCCESS == key->SetDWORDValue(name, value))
	{
		return true;
	}
	else
	{
		return false;
	}
}

DWORD CThemeDlg::ReadProfileInt(CRegKey *key, CString name, DWORD defaultValue)
{
	DWORD data;
	if (ERROR_SUCCESS ==key->QueryDWORDValue(name, data))
	{
		return data;
	}
	else
	{
		return defaultValue;
	}
}
