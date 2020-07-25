// clockDlg.h : header file
//

#if !defined(AFX_CLOCKDLG_H__433044E7_5C42_11D1_800A_0000C0491400__INCLUDED_)
#define AFX_CLOCKDLG_H__433044E7_5C42_11D1_800A_0000C0491400__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CClockDlg dialog

class CClockDlg : public CDialog
{
// Construction
public:
	CClockDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CClockDlg)
	enum { IDD = IDD_CLOCK_DIALOG };
	CString m_name;
	BOOL	m_quad;
	BOOL	m_roman;
	BOOL	m_rotate;
	int		m_cx;
	int		m_cy;
	int		m_x;
	int		m_y;
	BOOL    m_smooth;
	BOOL    m_sysColor;
	//}}AFX_DATA

	COLORREF m_bkColor;
	COLORREF m_handsColor;
	COLORREF m_bdrColor;
	COLORREF m_tickColor;
	COLORREF m_tick5Color;
	COLORREF m_fontColor;
	COLORREF m_scColor;
	COLORREF m_numColor;

	CWnd m_colorBox1;
	CWnd m_colorBox2;
	CWnd m_colorBox3;
	CWnd m_colorBox4;
	CWnd m_colorBox5;
	CWnd m_colorBox6;
	CWnd m_colorBox7;
	CWnd m_colorBox8;

	CButton m_btnRadioSys;
	CButton m_btnRadioOwn;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CClockDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void InvalidateColorBox(CWnd& box);
	void PaintColorBox(CDC& dc, CWnd& box, COLORREF color);
	COLORREF SelectColor(COLORREF color);
	afx_msg void OnStnClickedColorbox1();
	afx_msg void OnStnClickedColorbox2();
	afx_msg void OnStnClickedColorbox3();
	afx_msg void OnStnClickedColorbox4();
	afx_msg void OnStnClickedColorbox5();
	afx_msg void OnStnClickedColorbox6();
	afx_msg void OnStnClickedColorbox7();
	afx_msg void OnStnClickedColorbox8();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedRadiosyscolors();
	afx_msg void OnBnClickedRadioowncolors();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLOCKDLG_H__433044E7_5C42_11D1_800A_0000C0491400__INCLUDED_)
