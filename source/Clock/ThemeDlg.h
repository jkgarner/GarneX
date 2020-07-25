#pragma once
#include "afxwin.h"
#include "atlbase.h"

// CThemeDlg dialog

class CThemeDlg : public CDialog
{
	DECLARE_DYNAMIC(CThemeDlg)

public:
	CThemeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CThemeDlg();

// Dialog Data
	enum { IDD = IDD_THEMEDLG };


protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();


private:
	//local display state varables
	bool l_rotate;
	bool l_roman;
	bool l_quadOnly;
	bool l_noTitle;
	bool l_sysColor;
	bool l_smooth;

	//local positioning variables
	int l_x;
	int l_y;
	int l_cx;
	int l_cy;

	//local color variables
	COLORREF l_bkColor;
	COLORREF l_handsColor;
	COLORREF l_bdrColor;
	COLORREF l_tickColor;
	COLORREF l_tick5Color;
	COLORREF l_fontColor;
	COLORREF l_scColor;
	COLORREF l_numColor;

public:
	static void LoadConfig();
	static void SaveConfig();
	
	void SaveCurrent(CString name);
	void LoadLocalConfig(CString name);
	void SaveLocalConfig(CString name);

	static bool CanUse(CString name);
	static CString GetCurrent();

	static HKEY OpenKey(bool write = false);
	static HKEY OpenKey(CString themeName, bool write = false);
	static bool WriteProfileInt(CRegKey *key, CString name, DWORD value);
	static DWORD ReadProfileInt(CRegKey *key, CString name, DWORD defaultValue);

public:
	//THEME: display state varables
	static bool m_rotate;
	static bool m_roman;
	static bool m_quadOnly;
	static bool m_noTitle;
	static bool m_sysColor;
	static bool m_smooth;

	//THEME: positioning variables
	static int m_x;
	static int m_y;
	static int m_cx;
	static int m_cy;

	//THEME: color variables
	static COLORREF m_bkColor;
	static COLORREF m_handsColor;
	static COLORREF m_bdrColor;
	static COLORREF m_tickColor;
	static COLORREF m_tick5Color;
	static COLORREF m_fontColor;
	static COLORREF m_scColor;
	static COLORREF m_numColor;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddNew();
public:
	afx_msg void OnBnClickedDelete();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	CComboBox m_nameList;
	CButton m_btnDelete;
	CButton m_btnAddNew;
	
public:
	static CString m_name;
public:
	afx_msg void OnCbnSelchangeCombo1();
};
