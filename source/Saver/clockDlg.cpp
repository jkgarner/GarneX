// clockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ClockWnd.h"
#include "clockDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClockDlg dialog

CClockDlg::CClockDlg()
{
	//{{AFX_DATA_INIT(CClockDlg)
	m_quad = FALSE;
	m_roman = FALSE;
	m_rotate = FALSE;
    m_smooth = FALSE;
	m_cx = 0;
	m_cy = 0;
	m_x = 0;
	m_y = 0;
	//}}AFX_DATA_INIT
}

void CClockDlg::DoDataExchange(CDataExchange* pDX)
{
    CClockWnd* pSaver = (CClockWnd*)AfxGetScreenSaverWnd();
	if (!pSaver)
		return;

    int tickVal = 0;
    int useBlackVal = 0;

	if (!pDX->m_bSaveAndValidate)
	{
		pSaver->RestoreOptions();

	    m_rotate	 = pSaver->m_rotate;	 
	    m_roman		 = pSaver->m_roman;		 
	    m_quad       = pSaver->m_quadOnly;   
	    m_smooth     = pSaver->m_smooth;          
	    m_x	         = pSaver->m_x;	         
	    m_y	         = pSaver->m_y;	         
	    m_cx	     = pSaver->m_cx;	     
	    m_cy	     = pSaver->m_cy;	     
        m_faceColor  = pSaver->m_faceColor;    
        m_bkColor    = pSaver->m_bkColor;    
	    m_bdrColor   = pSaver->m_bdrColor;   
	    m_scColor    = pSaver->m_scColor;    
	    m_mnColor    = pSaver->m_mnColor;    
	    m_hrColor    = pSaver->m_hrColor;    
	    m_fontColor  = pSaver->m_fontColor;
	    m_tick5Color = pSaver->m_tick5Color; 
	    m_tickColor  = pSaver->m_tickColor;  
	    m_numColor   = pSaver->m_numColor;
        m_tickCount  = pSaver->m_tickCount;
        m_posMode    = pSaver->m_posMode;
        m_bkMode     = pSaver->m_bkMode;
        m_speed      = pSaver->m_speed;
        m_wipes      = pSaver->m_wipes;
        m_shadows    = pSaver->m_shadows;

        switch (m_tickCount)
        {
        case 4 : tickVal = 0; break;
        case 6 : tickVal = 1; break;
        case 8 : tickVal = 2; break;
        case 12: tickVal = 3; break;
        case 16: tickVal = 4; break;
        case 24: tickVal = 5; break;
        default: tickVal = 2; break;
        }
    }

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClockDlg)
    DDX_Control(pDX, IDC_X, m_editX);
    DDX_Control(pDX, IDC_Y, m_editY);
    DDX_Control(pDX, IDC_CX, m_editCX);
    DDX_Control(pDX, IDC_CY, m_editCY);
    DDX_Control(pDX, IDC_SPEED, m_editSpeed);
	DDX_Control(pDX, IDC_COLORBOX1, m_colorBox1);
	DDX_Control(pDX, IDC_COLORBOX2, m_colorBox2);
	DDX_Control(pDX, IDC_COLORBOX3, m_colorBox3);
	DDX_Control(pDX, IDC_COLORBOX4, m_colorBox4);
	DDX_Control(pDX, IDC_COLORBOX5, m_colorBox5);
	DDX_Control(pDX, IDC_COLORBOX6, m_colorBox6);
	DDX_Control(pDX, IDC_COLORBOX7, m_colorBox7);
	DDX_Control(pDX, IDC_COLORBOX8, m_colorBox8);
	DDX_Control(pDX, IDC_COLORBOX9, m_colorBox9);
	DDX_Control(pDX, IDC_COLORBOX10, m_colorBox10);
    DDX_Control(pDX, IDC_WIPES, m_wipeWnd);
	DDX_Check(pDX, IDC_SMOOTH, m_smooth);
	DDX_Check(pDX, IDC_QUADONLY, m_quad);
	DDX_Check(pDX, IDC_ROMAN, m_roman);
	DDX_Check(pDX, IDC_Rotate, m_rotate);
    DDX_Check(pDX, IDC_WIPES, m_wipes);
    DDX_Check(pDX, IDC_SHADOWS, m_shadows);
    DDX_Radio(pDX, IDC_RADIO_CENTER, m_posMode);
    DDX_Radio(pDX, IDC_RADIO_0, tickVal);
    DDX_Radio(pDX, IDC_RADIO_SYSBKG, m_bkMode);
	DDX_Text(pDX, IDC_CX, m_cx);
	DDX_Text(pDX, IDC_CY, m_cy);
	DDX_Text(pDX, IDC_X, m_x);
	DDX_Text(pDX, IDC_Y, m_y);
	DDX_Text(pDX, IDC_SPEED, m_speed);
	//}}AFX_DATA_MAP
   
	if (pDX->m_bSaveAndValidate)
    {
        switch (tickVal)
        {
        case 0 : m_tickCount = 4;  break;
        case 1: m_tickCount  = 6;  break;
        case 2: m_tickCount  = 8;  break;
        case 3: m_tickCount  = 12; break;
        case 4: m_tickCount  = 16; break;
        case 5: m_tickCount  = 24; break;
        default: m_tickCount = 8;  break;
        }

	    pSaver->m_rotate	 = m_rotate;	 
	    pSaver->m_roman		 = m_roman;		 
	    pSaver->m_quadOnly   = m_quad;   
	    pSaver->m_smooth     = m_smooth;     
	    pSaver->m_x	         = m_x;	         
	    pSaver->m_y	         = m_y;	         
	    pSaver->m_cx	     = m_cx;	     
	    pSaver->m_cy	     = m_cy;	     
	    pSaver->m_faceColor  = m_faceColor;    
	    pSaver->m_bkColor    = m_bkColor;    
	    pSaver->m_bdrColor   = m_bdrColor;   
	    pSaver->m_scColor    = m_scColor;    
	    pSaver->m_mnColor    = m_mnColor;    
	    pSaver->m_hrColor    = m_hrColor;    
	    pSaver->m_fontColor  = m_fontColor;  
	    pSaver->m_tick5Color = m_tick5Color; 
	    pSaver->m_tickColor  = m_tickColor;  
	    pSaver->m_numColor   = m_numColor; 
        pSaver->m_tickCount  = m_tickCount;
        pSaver->m_posMode    = m_posMode;
        pSaver->m_bkMode     = m_bkMode;
        pSaver->m_speed      = m_speed;
        pSaver->m_wipes      = m_wipes;
        pSaver->m_shadows    = m_shadows;
		pSaver->SaveOptions();
    }
    else
    {
        EnablePosBoxes();
        EnableWipes();
    }
}

BEGIN_MESSAGE_MAP(CClockDlg, CDialog)
	//{{AFX_MSG_MAP(CClockDlg)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_COLORBOX1, &CClockDlg::OnStnClickedColorbox1)
	ON_STN_CLICKED(IDC_COLORBOX2, &CClockDlg::OnStnClickedColorbox2)
	ON_STN_CLICKED(IDC_COLORBOX3, &CClockDlg::OnStnClickedColorbox3)
	ON_STN_CLICKED(IDC_COLORBOX4, &CClockDlg::OnStnClickedColorbox4)
	ON_STN_CLICKED(IDC_COLORBOX5, &CClockDlg::OnStnClickedColorbox5)
	ON_STN_CLICKED(IDC_COLORBOX6, &CClockDlg::OnStnClickedColorbox6)
	ON_STN_CLICKED(IDC_COLORBOX7, &CClockDlg::OnStnClickedColorbox7)
	ON_STN_CLICKED(IDC_COLORBOX8, &CClockDlg::OnStnClickedColorbox8)
	ON_STN_CLICKED(IDC_COLORBOX9, &CClockDlg::OnStnClickedColorbox9)
	ON_STN_CLICKED(IDC_COLORBOX10, &CClockDlg::OnStnClickedColorbox10)
	ON_STN_CLICKED(IDCANCEL, &CClockDlg::OnBnClickedCancel)
	ON_WM_PAINT()
    ON_BN_CLICKED(IDC_RADIO_CENTER, &CClockDlg::OnBnClickedRadioCenter)
    ON_BN_CLICKED(IDC_RADIO_FIXED, &CClockDlg::OnBnClickedRadioFixed)
    ON_BN_CLICKED(IDC_RADIO_FLOATING, &CClockDlg::OnBnClickedRadioFloating)
    ON_BN_CLICKED(IDC_RADIO_DESKTOP, &CClockDlg::OnBnClickedRadioDesktop)
    ON_BN_CLICKED(IDC_RADIO_BLACKBKG, &CClockDlg::OnBnClickedRadioBlackbkg)
    ON_BN_CLICKED(IDC_RADIO_SYSBKG, &CClockDlg::OnBnClickedRadioSysbkg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClockDlg message handlers

BOOL CClockDlg::OnInitDialog()
{
	CScreenSaverDlg::OnInitDialog();

	InvalidateColorBox(m_colorBox1);
	InvalidateColorBox(m_colorBox2);
	InvalidateColorBox(m_colorBox3);
	InvalidateColorBox(m_colorBox4);
	InvalidateColorBox(m_colorBox5);
	InvalidateColorBox(m_colorBox6);
	InvalidateColorBox(m_colorBox7);
	InvalidateColorBox(m_colorBox8);
	InvalidateColorBox(m_colorBox9);
	InvalidateColorBox(m_colorBox10);

	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
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
	m_faceColor = SelectColor(m_faceColor);
	InvalidateColorBox(m_colorBox1);
}

void CClockDlg::OnStnClickedColorbox2()
{
	m_bdrColor = SelectColor(m_bdrColor);
	InvalidateColorBox(m_colorBox2);
}

void CClockDlg::OnStnClickedColorbox3()
{
	m_hrColor = SelectColor(m_hrColor);
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

void CClockDlg::OnStnClickedColorbox9()
{
	m_mnColor = SelectColor(m_mnColor);
	InvalidateColorBox(m_colorBox9);
}

void CClockDlg::OnStnClickedColorbox10()
{
	m_bkColor = SelectColor(m_bkColor);
	InvalidateColorBox(m_colorBox10);
}


void CClockDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CClockDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	PaintColorBox(dc, m_colorBox1, m_faceColor);
	PaintColorBox(dc, m_colorBox2, m_bdrColor  );
	PaintColorBox(dc, m_colorBox3, m_hrColor   );
	PaintColorBox(dc, m_colorBox4, m_scColor   );
	PaintColorBox(dc, m_colorBox5, m_numColor  );
	PaintColorBox(dc, m_colorBox6, m_tick5Color);
	PaintColorBox(dc, m_colorBox7, m_tickColor );
	PaintColorBox(dc, m_colorBox8, m_fontColor );
	PaintColorBox(dc, m_colorBox9, m_mnColor   );
	PaintColorBox(dc, m_colorBox10, m_bkColor  );
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


void CClockDlg::OnBnClickedRadioCenter()
{
    UpdateData(TRUE);
    EnablePosBoxes();
}

void CClockDlg::OnBnClickedRadioFixed()
{
    UpdateData(TRUE);
    EnablePosBoxes();
}

void CClockDlg::OnBnClickedRadioFloating()
{
    UpdateData(TRUE);
    EnablePosBoxes();
}

void CClockDlg::EnablePosBoxes()
{
    m_editX.EnableWindow(FALSE);
    m_editY.EnableWindow(FALSE);
    m_editCX.EnableWindow(FALSE);
    m_editCY.EnableWindow(FALSE);
    m_editSpeed.EnableWindow(FALSE);
    switch (m_posMode)
    {
    case 2: //floating
        m_editCX.EnableWindow(TRUE);
        m_editCY.EnableWindow(TRUE);
        m_editSpeed.EnableWindow(TRUE);
        break;
    case 0: //center;
        m_editCX.EnableWindow(TRUE);
        m_editCY.EnableWindow(TRUE);
        break;
    default:
    case 1: //fixed
        m_editX.EnableWindow(TRUE);
        m_editY.EnableWindow(TRUE);
        m_editCX.EnableWindow(TRUE);
        m_editCY.EnableWindow(TRUE);
        break;
    }
}

void CClockDlg::OnBnClickedRadioDesktop()
{
    UpdateData(TRUE);
    EnableWipes();
}

void CClockDlg::OnBnClickedRadioBlackbkg()
{
    UpdateData(TRUE);
    EnableWipes();    
}

void CClockDlg::OnBnClickedRadioSysbkg()
{
    UpdateData(TRUE);
    EnableWipes();
}

void CClockDlg::EnableWipes()
{
    m_wipeWnd.EnableWindow(TRUE);//m_bkMode == 2);
}
