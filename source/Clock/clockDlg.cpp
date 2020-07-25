// clockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "clock.h"
#include "clockDlg.h"
#include "ThemeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClockDlg dialog

CClockDlg::CClockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClockDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClockDlg)
	m_quad = FALSE;
	m_roman = FALSE;
	m_rotate = FALSE;
	m_cx = 0;
	m_cy = 0;
	m_x = 0;
	m_y = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClockDlg)
	DDX_Control(pDX, IDC_RADIOSYSCOLORS, m_btnRadioSys);
	DDX_Control(pDX, IDC_RADIOOWNCOLORS, m_btnRadioOwn);
	DDX_Control(pDX, IDC_COLORBOX1, m_colorBox1);
	DDX_Control(pDX, IDC_COLORBOX2, m_colorBox2);
	DDX_Control(pDX, IDC_COLORBOX3, m_colorBox3);
	DDX_Control(pDX, IDC_COLORBOX4, m_colorBox4);
	DDX_Control(pDX, IDC_COLORBOX5, m_colorBox5);
	DDX_Control(pDX, IDC_COLORBOX6, m_colorBox6);
	DDX_Control(pDX, IDC_COLORBOX7, m_colorBox7);
	DDX_Control(pDX, IDC_COLORBOX8, m_colorBox8);
	DDX_Check(pDX, IDC_SMOOTH, m_smooth);
	DDX_Check(pDX, IDC_QUADONLY, m_quad);
	DDX_Check(pDX, IDC_ROMAN, m_roman);
	DDX_Check(pDX, IDC_Rotate, m_rotate);
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Text(pDX, IDC_CX, m_cx);
	DDX_Text(pDX, IDC_CY, m_cy);
	DDX_Text(pDX, IDC_X, m_x);
	DDX_Text(pDX, IDC_Y, m_y);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClockDlg, CDialog)
	//{{AFX_MSG_MAP(CClockDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIOSYSCOLORS, &CClockDlg::OnBnClickedRadiosyscolors)
	ON_BN_CLICKED(IDC_RADIOOWNCOLORS, &CClockDlg::OnBnClickedRadioowncolors)
	ON_STN_CLICKED(IDC_COLORBOX1, &CClockDlg::OnStnClickedColorbox1)
	ON_STN_CLICKED(IDC_COLORBOX2, &CClockDlg::OnStnClickedColorbox2)
	ON_STN_CLICKED(IDC_COLORBOX3, &CClockDlg::OnStnClickedColorbox3)
	ON_STN_CLICKED(IDC_COLORBOX4, &CClockDlg::OnStnClickedColorbox4)
	ON_STN_CLICKED(IDC_COLORBOX5, &CClockDlg::OnStnClickedColorbox5)
	ON_STN_CLICKED(IDC_COLORBOX6, &CClockDlg::OnStnClickedColorbox6)
	ON_STN_CLICKED(IDC_COLORBOX7, &CClockDlg::OnStnClickedColorbox7)
	ON_STN_CLICKED(IDC_COLORBOX8, &CClockDlg::OnStnClickedColorbox8)
	ON_STN_CLICKED(IDCANCEL, &CClockDlg::OnBnClickedCancel)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClockDlg message handlers

BOOL CClockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnRadioSys.SetCheck(m_sysColor);
	m_btnRadioOwn.SetCheck(!m_sysColor);

	UpdateData(FALSE);

	InvalidateColorBox(m_colorBox1);
	InvalidateColorBox(m_colorBox2);
	InvalidateColorBox(m_colorBox3);
	InvalidateColorBox(m_colorBox4);
	InvalidateColorBox(m_colorBox5);
	InvalidateColorBox(m_colorBox6);
	InvalidateColorBox(m_colorBox7);
	InvalidateColorBox(m_colorBox8);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CClockDlg::OnOK() 
{
	//write out configuration
	UpdateData(TRUE);
	if (false == CThemeDlg::CanUse(m_name))
	{	
		MessageBox(_T("A new theme can not have the same name\n")
			_T("as an existing theme.  Please try again \n")
			_T("with a different name."),
			_T("User Error"), MB_OK | MB_ICONHAND);
	}
	else
	{
		CDialog::OnOK();
	}
}

void CClockDlg::InvalidateColorBox(CWnd& box)
{
	CRect colorRect;
	box.GetClientRect(colorRect);
	box.ClientToScreen(colorRect);
	this->ScreenToClient(colorRect);
	this->InvalidateRect(colorRect);
}

void CClockDlg::PaintColorBox(CDC& dc, CWnd& box, COLORREF color)
{
	CRect colorRect;
	box.GetClientRect(colorRect);
	box.ClientToScreen(colorRect);
	this->ScreenToClient(colorRect);
	CBrush brush;
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, color);
	brush.CreateSolidBrush(color);
	CPen *oldP = dc.SelectObject(&pen);
	CBrush *oldB = dc.SelectObject(&brush);
	dc.Rectangle(colorRect);
	dc.SelectObject(oldB);
	dc.SelectObject(oldP);
}

COLORREF CClockDlg::SelectColor(COLORREF color)
{
	CColorDialog *cdlg = new CColorDialog(color);
	if (IDOK == cdlg->DoModal())
	{
		return cdlg->GetColor();
	}
	return color;
}

void CClockDlg::OnStnClickedColorbox1()
{
	m_bkColor = SelectColor(m_bkColor);
	InvalidateColorBox(m_colorBox1);
}

void CClockDlg::OnStnClickedColorbox2()
{
	m_bdrColor = SelectColor(m_bdrColor);
	InvalidateColorBox(m_colorBox2);
}

void CClockDlg::OnStnClickedColorbox3()
{
	m_handsColor = SelectColor(m_handsColor);
	InvalidateColorBox(m_colorBox3);
}

void CClockDlg::OnStnClickedColorbox4()
{
	m_scColor = SelectColor(m_scColor);
	InvalidateColorBox(m_colorBox4);
}

void CClockDlg::OnStnClickedColorbox5()
{
	m_numColor = SelectColor(m_numColor);
	InvalidateColorBox(m_colorBox5);
}

void CClockDlg::OnStnClickedColorbox6()
{
	m_tick5Color = SelectColor(m_tick5Color);
	InvalidateColorBox(m_colorBox6);
}

void CClockDlg::OnStnClickedColorbox7()
{
	m_tickColor = SelectColor(m_tickColor);
	InvalidateColorBox(m_colorBox7);
}

void CClockDlg::OnStnClickedColorbox8()
{
	m_fontColor = SelectColor(m_fontColor);
	InvalidateColorBox(m_colorBox8);
}

void CClockDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CClockDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	PaintColorBox(dc, m_colorBox1, m_bkColor   );
	PaintColorBox(dc, m_colorBox2, m_bdrColor  );
	PaintColorBox(dc, m_colorBox3, m_handsColor);
	PaintColorBox(dc, m_colorBox4, m_scColor   );
	PaintColorBox(dc, m_colorBox5, m_numColor  );
	PaintColorBox(dc, m_colorBox6, m_tick5Color);
	PaintColorBox(dc, m_colorBox7, m_tickColor );
	PaintColorBox(dc, m_colorBox8, m_fontColor  );
}

void CClockDlg::OnBnClickedRadiosyscolors()
{
	m_sysColor = true;
	m_btnRadioSys.SetCheck(true);
	m_btnRadioOwn.SetCheck(false);
}

void CClockDlg::OnBnClickedRadioowncolors()
{
	m_sysColor = false;
	m_btnRadioSys.SetCheck(false);
	m_btnRadioOwn.SetCheck(true);
}

