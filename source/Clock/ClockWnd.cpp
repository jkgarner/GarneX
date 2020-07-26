// ClockWnd.cpp : implementation file
//
#include "stdafx.h"
#include "clock.h"
#include "ClockWnd.h"
#include "clockDlg.h"
#include "themedlg.h"
#include "AboutDlg.h"
#include <math.h>
#define PI 3.141592654

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TICKCOUNT 8

/////////////////////////////////////////////////////////////////////////////
// CClockWnd

IMPLEMENT_DYNCREATE(CClockWnd, CFrameWnd)

CClockWnd::CClockWnd()
{
	m_tick = 0;
	m_mouseDown = FALSE;	
	m_resizing  = FALSE;
	m_noTitle	= FALSE;
	CThemeDlg::LoadConfig();
	ReadConfig();
}

CClockWnd::~CClockWnd()
{
	WriteConfig();
	CThemeDlg::SaveConfig();
}

void CClockWnd::ReadConfig()
{
	m_rotate	= CThemeDlg::m_rotate;
	m_roman		= CThemeDlg::m_roman;
	m_quadOnly  = CThemeDlg::m_quadOnly;
	m_x	        = CThemeDlg::m_x;
	m_y	        = CThemeDlg::m_y;
	m_cx	    = CThemeDlg::m_cx;
	m_cy	    = CThemeDlg::m_cy;
	m_smooth    = CThemeDlg::m_smooth;
	m_sysColor  = CThemeDlg::m_sysColor;

	if (m_sysColor)
	{
		m_bkColor	 = GetSysColor(COLOR_3DFACE);
		m_handsColor = GetSysColor(COLOR_BTNTEXT);
		m_bdrColor   = GetSysColor(COLOR_3DSHADOW);
		m_tickColor  = GetSysColor(COLOR_3DSHADOW);
		m_tick5Color = GetSysColor(COLOR_BTNTEXT);
		m_fontColor  = GetSysColor(COLOR_BTNTEXT);
		m_scColor	 = GetSysColor(COLOR_3DSHADOW);
		m_numColor	 = GetSysColor(COLOR_BTNTEXT);
	}
	else
	{
		m_bkColor	 = CThemeDlg::m_bkColor;
		m_handsColor = CThemeDlg::m_handsColor;
		m_bdrColor   = CThemeDlg::m_bdrColor;
		m_tickColor  = CThemeDlg::m_tickColor;
		m_tick5Color = CThemeDlg::m_tick5Color;
		m_fontColor  = CThemeDlg::m_fontColor;
		m_scColor	 = CThemeDlg::m_scColor;
		m_numColor	 = CThemeDlg::m_numColor;
	}
}

void CClockWnd::WriteConfig()
{
	CThemeDlg::m_rotate		= m_rotate;
	CThemeDlg::m_roman		= m_roman;
	CThemeDlg::m_quadOnly	= m_quadOnly;
	CThemeDlg::m_x	        = m_x;
	CThemeDlg::m_y	        = m_y;
	CThemeDlg::m_cx			= m_cx;
	CThemeDlg::m_cy			= m_cy;
	CThemeDlg::m_smooth		= m_smooth;
	CThemeDlg::m_sysColor	= m_sysColor;

	if (!m_sysColor)
	{
		CThemeDlg::m_bkColor	 = m_bkColor;
		CThemeDlg::m_handsColor  = m_handsColor;
		CThemeDlg::m_bdrColor    = m_bdrColor;
		CThemeDlg::m_tickColor   = m_tickColor;
		CThemeDlg::m_tick5Color  = m_tick5Color;
		CThemeDlg::m_fontColor   = m_fontColor;
		CThemeDlg::m_scColor	 = m_scColor;
		CThemeDlg::m_numColor	 = m_numColor;
	}
}

BEGIN_MESSAGE_MAP(CClockWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CClockWnd)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDC_ABOUT, OnAbout)
	ON_COMMAND(IDC_CONFIGURE, OnConfigure)
	ON_COMMAND(IDC_EXIT, OnExit)
	ON_MESSAGE(IDC_MYICON, OnMyicon)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_QUERYENDSESSION()
	ON_WM_ENDSESSION()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClockWnd message handlers
BOOL CClockWnd::OnQueryEndSession()
{
	return TRUE;
}

void CClockWnd::OnEndSession(BOOL bEnding)
{
	if (bEnding)
	{
		KillTimer(m_timer);
	}
	CWnd::OnEndSession(bEnding);
}

void CClockWnd::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	if ( (nType != SIZE_MINIMIZED )&&
	     (nType != SIZE_MAXHIDE) )
	{
		m_midX = cx / 2;
		m_midY = cy / 2;
		int maxRadius = min(m_midX,m_midY);
		m_radiusHr = 63 * maxRadius/100; //5* maxRadius/10;
		m_radiusSc = 95 * maxRadius/100; //8* maxRadius/10;
		m_radiusMn = 95 * maxRadius/100; //8* maxRadius/10;
		CRect winRect;
		CRect client;
		GetWindowRect(winRect);
		GetClientRect(client);
		ClientToScreen(client);
		int offsetX = client.left - winRect.left;
		int offsetY = client.top - winRect.top;
		CRect clipRect(m_midX - maxRadius, m_midY - maxRadius,
			           m_midX + maxRadius, m_midY + maxRadius);
		clipRect.OffsetRect(offsetX, offsetY);		
		if (m_wndRgn.m_hObject != 0)
		{
			m_wndRgn.DeleteObject();
		}
		m_wndRgn.CreateEllipticRgn(clipRect.left, clipRect.top, clipRect.right, clipRect.bottom);
		SetWindowRgn(m_wndRgn,TRUE);
		Invalidate();
	}	
}

void CClockWnd::LoadTime()
{
	div_t q;
	CTime time = CTime::GetCurrentTime();

	m_pHr = m_hr;
	m_pMn = m_mn;
	m_pSc = m_sc;

	m_hr = time.GetHour();
	m_mn = time.GetMinute();
	m_sc = time.GetSecond();
	q = div(m_hr, 12);
	m_hr = q.rem;
	m_hr *= 60;
	m_hr *= 60;
	
	q = div(m_mn,60);
	m_mn = q.rem;
	m_mn *= 60;

	q = div(m_sc,60);
	m_sc = q.rem;
}

void CClockWnd::OnTimer(UINT nIDEvent) 
{
	CFrameWnd::OnTimer(nIDEvent);	
	m_tick++;
	if (m_tick == TICKCOUNT)
	{
		LoadTime();
		m_tick = 0;
		Invalidate();
		UpdateWindow();
	}
	else
	{
		if (m_smooth)
		{
			Invalidate();
			UpdateWindow();
		}
	}
}

void CClockWnd::Draw(CDC* dc)
{
    if ((m_mouseDown == TRUE) || (m_resizing == TRUE) )
    {
        //resizing window...
        DrawFace(dc);
    }
    else
    {
	    DrawFace(dc);
	    DrawHands(dc);
    }
}

void CClockWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC memDC;
	if (memDC.CreateCompatibleDC(&dc))
	{
		int maxRadius = min(m_midX,m_midY);
		CRect winRect;
		GetWindowRect(winRect);

		CBitmap memBmp;
		memBmp.CreateCompatibleBitmap(&dc,winRect.Width(), winRect.Height());
		CBitmap* oldMemBmp = memDC.SelectObject(&memBmp);
        Draw(&memDC);
		dc.BitBlt(0,0,winRect.Width(), winRect.Height(),
			      &memDC, 0,0, SRCCOPY);
		memDC.SelectObject(oldMemBmp);
	}
	else
	{
		Draw(&dc);
	}
}

CString CClockWnd::GetText(int number)
{
	UINT id = (m_roman)?(IDS_ROMAN):(IDS_ARABIC);
	CString allText;
	CString left;
	CString right;
	if (allText.LoadString(id) == 0)
	{
		allText = "|,|,|,|,|,|,|,|,|,|,|,|";
	}
	
	int i = 0;
	do
	{
		int charAt = allText.Find(',');
		left = allText.Left(charAt);
		right = allText.Right(allText.GetLength() - charAt - 1);
		allText = right;
		i++;
	} while (i <= number);
	return (number == 11) ? (right) : (left);	
}

void CClockWnd::DrawTicks(CDC* dc)
{
	//Draw Ticks
	POINT tick[2];
    int dif = min(m_midX, m_midY) - m_radiusSc;
    tick[0].x = 0;
    tick[0].y = -1* m_radiusSc;
    tick[0].y = -1* m_radiusSc;
    tick[1].x = 0;
    tick[1].y = -1* m_radiusSc + dif/2;

    //rom rot  q  r1&!r2 !q  r1&!r2&!q r1&!r2&!q|!q
    // 0   0   0    0     1     0            1
    // 0   0   1    0     0     0            0
    // 0   1   0    0     1     0            1
    // 0   1   1    0     0     0            0
    // 1   0   0    1     1     1            0
    // 1   0   1    1     0     0            0
    // 1   1   0    0     1     0            1
    // 1   1   1    0     0     0            0
    BOOL drawRing = ((this->m_quadOnly == false) &&
        !(!this->m_quadOnly && this->m_roman & !this->m_rotate));    

   if (drawRing)
    {
        CPen *oldPen = dc->SelectObject(&m_tickPen);
        CBrush *oldBrush = dc->SelectObject(&m_bkBrush);
        dc->Ellipse(this->m_midX-m_radiusSc,
                    this->m_midY-m_radiusSc,
                    this->m_midX+m_radiusSc,
                    this->m_midY+m_radiusSc);
        dc->Ellipse(this->m_midX-(m_radiusSc - dif/2),
                    this->m_midY-(m_radiusSc - dif/2),
                    this->m_midX+(m_radiusSc - dif/2),
                    this->m_midY+(m_radiusSc - dif/2));
        dc->SelectObject(oldBrush);
        dc->SelectObject(oldPen);
    }
    int i = 0;
    for (i = 0; i < 60; i++)
    {
        tick[0].x = 0;
        tick[0].y = -1* m_radiusSc;
        tick[0].y = -1* m_radiusSc;
        tick[1].x = 0;
        tick[1].y = -1* m_radiusSc + dif/2;
        double theta = (2.0 * PI * (double)i) / 60.0;
        RotateAndOffset(tick[0],theta);
        RotateAndOffset(tick[1],theta);
        div_t q2 = div(i,5);        
        CPen *oldPen = (drawRing)?
            (dc->SelectObject(&m_tickPen)):
            (  (q2.rem == 0)? 
	           (dc->SelectObject(&m_tick5Pen)):
	           (dc->SelectObject(&m_tickPen))
            );
        dc->MoveTo(tick[0]);
        dc->LineTo(tick[1]);
        dc->SelectObject(oldPen);
    }	        
}

void CClockWnd::DrawNumbers(CDC* dc)
{
	//draw numbers...
    int i;
	for (i = 0; i < 12; i++)
	{
		div_t q = div(i,(m_quadOnly)?(3):(1));
		if (q.rem == 0)
		{
			CFont tempFont;
			CString text = GetText(i);

			//int dif = min(m_midX, m_midY) - m_radiusSc;
            int dif = min(m_midX, m_midY) - m_radiusHr;
			double theta = (2.0 * PI * (double)i) / 12.0;

            if (m_rotate)
			{
                int rot = 3600 - 300*i;
                if (i>3&&i<9)
                {
                    rot += 1800;
                }
				//tempFont.CreateFont(dif,0, rot,
				//tempFont.CreateFont(dif/2,0, rot,
                tempFont.CreateFont(2*dif/3,0, rot,
									0,400,0,0,0,0,0,0,0,0,
									_T("Times New Roman"));
			}
			else
			{
				//tempFont.CreateFont(dif,0, 0,
				//tempFont.CreateFont(dif/2,0, 0,
                tempFont.CreateFont(2*dif/3,0, 0,
									0,400,0,0,0,0,0,0,0,0,
									_T("Times New Roman"));
			}
			CFont* oldF = dc->SelectObject(&tempFont);
			CSize extent = dc->GetTextExtent(text);
            
			POINT ctr;
			ctr.x = 0;
			//ctr.y = -1 * m_radiusSc - dif/2;
			//ctr.y = -1 * m_radiusHr - 2*dif/4 - dif/8;
            ctr.y = -1 * m_radiusHr - 2*dif/4 + dif/8;

			POINT ctrRot;
			ctrRot.x = ctr.x;
			ctrRot.y = ctr.y;
            ctrRot.x -= (extent.cx / 2);
			ctrRot.y -= (extent.cy / 2);
            if (i>3&&i<9)
            {
                ctrRot.x += extent.cx;
			    ctrRot.y += extent.cy;
            }
			RotateAndOffset(ctr,theta);

			POINT ctrNRot;
			ctrNRot.x = ctr.x;
			ctrNRot.y = ctr.y;
			RotateAndOffset(ctrRot,theta);                        
            ctrNRot.x -= (extent.cx / 2);
			ctrNRot.y -= (extent.cy / 2);
            
			int xPos = (m_rotate)?(ctrRot.x):(ctrNRot.x);
			int yPos = (m_rotate)?(ctrRot.y):(ctrNRot.y);
			dc->TextOut(xPos,yPos,text);

			dc->SelectObject(oldF);
		}
	}
}

void CClockWnd::DrawFace(CDC* dc)
{	
	CRgn faceRgn;
	int hold;
	COLORREF oldC;

	hold = dc->SetBkMode(TRANSPARENT);
	oldC = dc->SetTextColor(m_numColor);
	CRect client;
	GetClientRect(client);
	dc->FillRect(client,&m_bdrBrush);
	
	int maxRadius = min(m_midX,m_midY);
	CRect faceRect(m_midX - maxRadius, m_midY - maxRadius,
		           m_midX + maxRadius, m_midY + maxRadius);
	faceRgn.CreateEllipticRgn(faceRect.left, faceRect.top, faceRect.right, faceRect.bottom);
	dc->FillRgn(&faceRgn, &m_bkBrush);
	dc->FrameRgn(&faceRgn,&m_bdrBrush, 1,1);
    
    DrawTicks(dc);
    DrawNumbers(dc);

	WriteLogo(dc);

	dc->SetBkMode(hold);
	dc->SetTextColor(oldC);
	faceRgn.DeleteObject();
}

void CClockWnd::DrawHands(CDC* dc)
{
	CRgn newHr;
	CreateHrHandRgn(newHr, TRUE);
	dc->FillRgn(&newHr, & m_handsBrush);
	newHr.DeleteObject();

	CRgn newMn;
	CreateMnHandRgn(newMn, TRUE);	
	dc->FillRgn(&newMn, & m_handsBrush);
	newMn.DeleteObject();

	DrawScHand(dc);

}

void CClockWnd::CreateHrHandRgn(CRgn& rgn, bool current)
{
	POINT hand[4];
	hand[0].x = 0;
	hand[0].y = m_radiusHr / 8;//5;
	hand[1].x = m_radiusHr / 10;
	hand[1].y = m_radiusHr / 10;//6;
	hand[2].x = 0;
	hand[2].y = -1* m_radiusHr;
	hand[3].x = -1* m_radiusHr / 10;
	hand[3].y = m_radiusHr / 10;//6;
	
	double theta;
	double hour = (current)?((double)m_hr):((double)m_pHr);
	double minute = (current)?((double)m_mn):((double)m_pMn);
	double second = (current)?((double)m_sc):((double)m_pSc);
	theta = (double)(hour + minute + second) * 2.0 * (double)PI / (12.0 * 60.0 * 60.0);
	
	RotateAndOffset(hand[0],theta);
	RotateAndOffset(hand[1],theta);
	RotateAndOffset(hand[2],theta);
	RotateAndOffset(hand[3],theta);

	rgn.CreatePolygonRgn(hand,4, WINDING);
}

void CClockWnd::CreateMnHandRgn(CRgn &rgn, bool current)
{
	POINT hand[4];
	hand[0].x = 0;
	hand[0].y = m_radiusMn / 8;
	hand[1].x = m_radiusMn / 15;
	hand[1].y = m_radiusMn / 10;
	hand[2].x = 0;
	hand[2].y = -1* m_radiusMn;
	hand[3].x = -1* m_radiusMn / 15;
	hand[3].y = m_radiusMn / 10;
	
	double theta;
	double minute = (current) ? ((double)m_mn) : ((double)m_pMn);
	double second = (current) ? ((double)m_sc) : ((double)m_pSc);
	theta = (double)(minute + second) * 2.0 * (double)PI / (60.0 * 60.0);

	RotateAndOffset(hand[0],theta);
	RotateAndOffset(hand[1],theta);
	RotateAndOffset(hand[2],theta);
	RotateAndOffset(hand[3],theta);

	rgn.CreatePolygonRgn(hand,4,WINDING);
}

void CClockWnd::DrawScHand(CDC* dc)
{
    if (m_cx < 500)
    {
        POINT hand[10];
	    hand[0].x = 0;
	    hand[0].y = m_radiusSc/6;

	    hand[1].x = 0;
	    hand[1].y = -1* m_radiusSc;
	    hand[2].x = 1;
	    hand[2].y = m_radiusSc/6;
	    hand[3].x = 1;
	    hand[3].y = 0;
	    hand[4].x = -1;
	    hand[4].y = m_radiusSc/6;
	    hand[5].x = -1;;
	    hand[5].y = 0;
	    hand[6].x = 2;
	    hand[6].y = m_radiusSc/6;
	    hand[7].x = 2;
	    hand[7].y = 0;
	    hand[8].x = -2;
	    hand[8].y = m_radiusSc/6;
	    hand[9].x = -2;
	    hand[9].y = 0;


	    double theta;
	    int second = m_sc;
	    int maxTick = (m_smooth)?(TICKCOUNT):(1);
	    if (m_tick == maxTick)
		    second = m_sc;

	    int numTicks = second * (maxTick) + m_tick;
	    theta = (2.0 * PI * (double)numTicks) / (60.0 * (double)maxTick);
	    RotateAndOffset(hand[0],theta);
	    RotateAndOffset(hand[1],theta);
	    RotateAndOffset(hand[2],theta);
	    RotateAndOffset(hand[3],theta);
	    RotateAndOffset(hand[4],theta);
	    RotateAndOffset(hand[5],theta);
	    RotateAndOffset(hand[6],theta);
	    RotateAndOffset(hand[7],theta);
	    RotateAndOffset(hand[8],theta);
	    RotateAndOffset(hand[9],theta);

        CPen* oldPen;
        if (m_cx < 250)
        {
	        oldPen = dc->SelectObject(&m_scPen);
        }
        else
        {
	        oldPen = dc->SelectObject(&m_scWtPen);
        }
	    dc->MoveTo(hand[1]);
	    dc->LineTo(hand[0]);
	    dc->SelectObject(m_scWtPen);
	    dc->LineTo(hand[2]);
	    dc->LineTo(hand[3]);
	    dc->LineTo(hand[5]);
	    dc->LineTo(hand[4]);
	    dc->LineTo(hand[6]);
	    dc->LineTo(hand[7]);
	    dc->LineTo(hand[9]);
	    dc->LineTo(hand[8]);
	    dc->SelectObject(oldPen);
    }
    else
    {
	    //create sec hand region...    
	    POINT hand[10];
        hand[0].x = -1 * m_radiusSc/50;
        hand[0].y = 1 * m_radiusSc/5;
        hand[1].x = 0;
        hand[1].y = (1 * m_radiusSc/5 + m_radiusSc/150);
        hand[2].x = 1 * m_radiusSc/50;
        hand[2].y = 1 * m_radiusSc/5;
        hand[3].x = 0;
        hand[3].y = -1 * m_radiusSc;

        double theta;
	    int second = m_sc;
	    int maxTick = (m_smooth)?(TICKCOUNT):(1);
	    if (m_tick == maxTick)
		    second = m_sc;

	    int numTicks = second * (maxTick) + m_tick;
	    theta = (2.0 * PI * (double)numTicks) / (60.0 * (double)maxTick);
	    RotateAndOffset(hand[0],theta);
	    RotateAndOffset(hand[1],theta);
	    RotateAndOffset(hand[2],theta);
	    RotateAndOffset(hand[3],theta);

	    CRgn newSec;
        newSec.CreatePolygonRgn(hand,4,WINDING);
	    dc->FillRgn(&newSec, & m_scBrush);       
	    dc->FrameRgn(&newSec, & m_scBrush, 2, 2);       
	    newSec.DeleteObject();
    }
}

void CClockWnd::RotateAndOffset(POINT& pt,double theta)
{
	//Rotate
	double X = (double)pt.x;
	double Y = (double)pt.y;
	//double R = sqrt( (X*X) + (Y*Y) );
    double x1 = X * (float)cos(theta) 
              - Y * (float)sin(theta);
    double y1 = X * (float)sin(theta) 
              + Y * (float)cos(theta);

	pt.x = (long)x1;
	pt.y = (long)y1;
	//Offset
	pt.x += m_midX;
	pt.y += m_midY;
}

BOOL CClockWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	cs.lpszClass = AfxRegisterWndClass(0,AfxGetApp()->LoadStandardCursor(IDC_ARROW),0, m_hIcon);
	cs.cx = m_cx;
	cs.cy = m_cy;
	cs.x = m_x;
	cs.y = m_y;
	return CFrameWnd::PreCreateWindow(cs);
}

int CClockWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	NOTIFYICONDATA data;
	data.cbSize = sizeof(NOTIFYICONDATA); 
    data.hWnd = m_hWnd; 
    data.uID = IDR_MAINFRAME; 
    data.uFlags = NIF_ICON | NIF_MESSAGE; 
	data.uCallbackMessage = IDC_MYICON;
	data.hIcon =  m_hIcon;
	Shell_NotifyIcon(NIM_ADD, &data);

	//create GDI objects for use...
	m_bkBrush.CreateSolidBrush(m_bkColor);
	m_handsBrush.CreateSolidBrush(m_handsColor);
	m_bdrBrush.CreateSolidBrush(m_bdrColor);
	m_tickBrush.CreateSolidBrush(m_tickColor);
	m_tick5Brush.CreateSolidBrush(m_tick5Color);
	m_fontBrush.CreateSolidBrush(m_fontColor);
	m_scBrush.CreateSolidBrush(m_scColor);
	m_numBrush.CreateSolidBrush(m_numColor);

	m_bkPen.CreatePen   (PS_SOLID,1,m_bkColor);
	m_handsPen.CreatePen(PS_SOLID,1,m_handsColor);
	m_bdrPen.CreatePen  (PS_SOLID,1,m_bdrColor);
	m_tickPen.CreatePen (PS_SOLID,1,m_tickColor);
	m_tick5Pen.CreatePen(PS_SOLID,2,m_tick5Color);
	m_fontPen.CreatePen (PS_SOLID,1,m_fontColor);
	m_numPen.CreatePen  (PS_SOLID,1,m_numColor);
	m_scPen.CreatePen   (PS_SOLID,1,m_scColor);
	m_scWtPen.CreatePen (PS_SOLID,2,m_scColor);


	m_hr = 0;
	m_mn = 0;
	m_sc = 0;
	LoadTime();
	m_timer = SetTimer( 1000, (1000/TICKCOUNT),NULL);	
	return 0;
}

void CClockWnd::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	KillTimer(m_timer);

	m_bkBrush.DeleteObject();
	m_handsBrush.DeleteObject();
	m_bdrBrush.DeleteObject();
	m_tickBrush.DeleteObject();
	m_tick5Brush.DeleteObject();
	m_fontBrush.DeleteObject();
	m_scBrush.DeleteObject();
	m_numBrush.DeleteObject();

	m_bkPen.DeleteObject();
	m_handsPen.DeleteObject();
	m_bdrPen.DeleteObject();
	m_tickPen.DeleteObject();
	m_tick5Pen.DeleteObject();
	m_fontPen.DeleteObject();
	m_scPen.DeleteObject();
	m_numPen.DeleteObject();
	m_scWtPen.DeleteObject();
}

void CClockWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	RunPopupMenu(point);
	CFrameWnd::OnRButtonDown(nFlags, point);
}

void CClockWnd::RunPopupMenu(CPoint& point)
{
	CMenu popup;
	POINT pt;
	pt.x = point.x;
	pt.y = point.y;
	ClientToScreen(&pt);
	//if(popup.LoadMenu(IDR_POPUP))
	popup.CreatePopupMenu();
	{
		popup.AppendMenu(MF_STRING,IDC_ABOUT,_T("&About"));
		popup.AppendMenu(MF_STRING,IDC_CONFIGURE, _T("&Configure"));
		popup.AppendMenu(MF_STRING,IDC_EXIT, _T("E&xit"));
		popup.TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
	}
}

void CClockWnd::OnAbout() 
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CClockWnd::OnConfigure() 
{
	int oldx = m_x;
	int oldy = m_y;
	int oldcx = m_cx;
	int oldcy = m_cy;

    //write out the current config...
    WriteConfig();

	CThemeDlg dlg;
	dlg.DoModal();

	//read in configuration
	ReadConfig();

	//stop teh timer... this will prevent accessing the GDI objects while they are out of scope...
	KillTimer(m_timer);

	//recreate the GDI Objects!
	m_bkBrush.DeleteObject();
	m_handsBrush.DeleteObject();
	m_bdrBrush.DeleteObject();
	m_tickBrush.DeleteObject();
	m_tick5Brush.DeleteObject();
	m_fontBrush.DeleteObject();
	m_scBrush.DeleteObject();
	m_numBrush.DeleteObject();

	m_bkPen.DeleteObject();
	m_handsPen.DeleteObject();
	m_bdrPen.DeleteObject();
	m_tickPen.DeleteObject();
	m_tick5Pen.DeleteObject();
	m_fontPen.DeleteObject();
	m_scPen.DeleteObject();
	m_numPen.DeleteObject();
	m_scWtPen.DeleteObject();

	//create GDI objects for use...
	m_bkBrush.CreateSolidBrush(m_bkColor);
	m_handsBrush.CreateSolidBrush(m_handsColor);
	m_bdrBrush.CreateSolidBrush(m_bdrColor);
	m_tickBrush.CreateSolidBrush(m_tickColor);
	m_tick5Brush.CreateSolidBrush(m_tick5Color);
	m_fontBrush.CreateSolidBrush(m_fontColor);
	m_scBrush.CreateSolidBrush(m_scColor);
	m_numBrush.CreateSolidBrush(m_numColor);

	m_bkPen.CreatePen   (PS_SOLID,1,m_bkColor);
	m_handsPen.CreatePen(PS_SOLID,1,m_handsColor);
	m_bdrPen.CreatePen  (PS_SOLID,1,m_bdrColor);
	m_tickPen.CreatePen (PS_SOLID,1,m_tickColor);
	m_tick5Pen.CreatePen(PS_SOLID,2,m_tick5Color);
	m_fontPen.CreatePen (PS_SOLID,1,m_fontColor);
	m_numPen.CreatePen  (PS_SOLID,1,m_numColor);
	m_scPen.CreatePen   (PS_SOLID,1,m_scColor);
	m_scWtPen.CreatePen (PS_SOLID,2,m_scColor);


	if (  (oldx != m_x)
		||(oldy != m_y)
		||(oldcx != m_cx)
		||(oldcy != m_cy))
	{
		Invalidate();
		MoveWindow(m_x, m_y,m_cx, m_cy,TRUE);
	}
	else
	{
		Invalidate();
	}
	//reload the time... and start clock again
	m_hr = 0;
	m_mn = 0;
	m_sc = 0;
	LoadTime();
	UpdateWindow();
	m_timer = SetTimer( 1000, (1000/TICKCOUNT),NULL);	
}

void CClockWnd::OnExit() 
{
	PostMessage(WM_CLOSE,0,0);	
}

BOOL CClockWnd::DestroyWindow() 
{
	NOTIFYICONDATA data;
	data.cbSize = sizeof(NOTIFYICONDATA); 
    data.hWnd = m_hWnd; 
    data.uID = IDR_MAINFRAME; 
    data.uFlags = NIF_ICON; 
 	data.hIcon =  m_hIcon;
	Shell_NotifyIcon(NIM_DELETE, &data);

	return CFrameWnd::DestroyWindow();
}

long CClockWnd::OnMyicon(UINT wParam, LONG lParam) 
{
    UINT uID; 
    UINT uMouseMsg; 
 
    uID = (UINT) wParam; 
    uMouseMsg = (UINT) lParam; 
 
    switch (uMouseMsg)
	{
	case WM_LBUTTONDOWN: 
		SetWindowPos(&wndTop, 0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
		UpdateWindow();
		break;
	case WM_RBUTTONDOWN:
		CPoint pt(GetSystemMetrics(SM_CXMAXIMIZED),
				  GetSystemMetrics(SM_CYMAXIMIZED) - 100);
		RunPopupMenu(pt);				
		break;
	}
    return 0; 
}

void CClockWnd::WriteLogo(CDC* dc)
{
	int hold = dc->SetBkMode(TRANSPARENT);
	COLORREF oldC = dc->SetTextColor(m_fontColor);
	CRect client;
	GetClientRect(client);
	//int dif = min(m_midX, m_midY) - m_radiusSc;
    int dif = min(m_midX, m_midY) - m_radiusHr;
	POINT ctr;
	ctr.x = m_midX;
	//ctr.y = m_midY + 1 * m_radiusSc - dif;
    ctr.y = m_midY + 1 * m_radiusHr - dif/4;
	CFont tempFont;
	CString text("GarneX");
	//tempFont.CreateFont(2*dif/3,0, 0,
    tempFont.CreateFont(7*dif/24,0, 0,
						0,400,0,0,0,0,0,0,0,0,
						//"Times New Roman");
                        _T("Georgia"));
	CFont* oldF = dc->SelectObject(&tempFont);
	CSize extent = dc->GetTextExtent(text);
	int xPos = ctr.x - extent.cx / 2;
	int yPos = ctr.y - extent.cy / 2;
	dc->TextOut(xPos,yPos,text);
	dc->SelectObject(oldF);
	dc->SetBkMode(hold);
	dc->SetTextColor(oldC);
}

bool CClockWnd::TestResizeRegion(CPoint point)
{
	//if so, prepare to grow window
	double dX = (double)point.x - (double)m_midX;
	double dY =(double) point.y - (double)m_midY;
	double dist = sqrt( (double)(dX*dX) + (double)(dY*dY));
	int maxRadius = min(m_midX,m_midY);
	return (dist > (double)maxRadius - 10.0);
}

bool CClockWnd::TestMoveRegion(CPoint point)
{
	CRect testRect(m_midX - 4 * GetSystemMetrics(SM_CXDOUBLECLK),
				   m_midY - 4 * GetSystemMetrics(SM_CYDOUBLECLK),
				   m_midX + 4 * GetSystemMetrics(SM_CXDOUBLECLK),
				   m_midY + 4 * GetSystemMetrics(SM_CYDOUBLECLK));
	return (testRect.PtInRect(point));
}

void CClockWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//did click have control key pressed...
	//if did, cycle settings
	if (nFlags & MK_CONTROL)
	{
		short bit1  = (m_roman)?(1):(0);
		short bit2  = (m_rotate)?(1):(0);
		short bit3  = (m_quadOnly)?(1):(0);
		short all   = (bit1<<2) | (bit2<<1) | (bit3);
		all+=1;
		if (all == 8)
			all = 0;
		m_roman     = (0 < (0x0004 & all));
		m_rotate    = (0 < (0x0002 & all));
		m_quadOnly  = (0 < (0x0001 & all));
		CThemeDlg::m_roman     = (0 < (0x0004 & all));
		CThemeDlg::m_rotate    = (0 < (0x0002 & all));
		CThemeDlg::m_quadOnly  = (0 < (0x0001 & all));
		CThemeDlg::SaveConfig();

		Invalidate();
		UpdateWindow();
		CFrameWnd::OnLButtonDown(nFlags, point);
		return;
	}

	//was click in center...if was prepare to move window
	if (TestMoveRegion(point))
	{
		//clicked in center
		m_mouseDown = TRUE;
		SetCapture();
        ClientToScreen(&point);
		m_startDrag = point;
		m_dragPoint = point;
		GetWindowRect(m_dragRect);
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
		CFrameWnd::OnLButtonDown(nFlags, point);
		return;
	}

	//was click near edge?
    if (TestResizeRegion(point))
	{
		//clicked near edge, prepare to grow window
	    int dX = point.x - m_midX;
    	int dY = point.y - m_midY;
		m_resizing = TRUE;
		SetCapture();
        ClientToScreen(&point);
        m_startDrag = point;
		m_dragPoint = point;
		GetWindowRect(m_dragRect);
		double phi = atan( -1.0 * (double)dY / (double)dX );
		if (phi > 3.0 * PI / 8.0)
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		else if (phi > PI / 8.0)
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
		else if (phi > -1 * PI / 8.0)
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		else if (phi > -3 * PI / 8.0)
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
		else
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
	}
	else
	{
		//on face but not near edge
		m_smooth = (m_smooth)?(FALSE):(TRUE);
		Invalidate();
		UpdateWindow();
	}
	CFrameWnd::OnLButtonDown(nFlags, point);	
}

void CClockWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_mouseDown)
	{
		ReleaseCapture();
		m_mouseDown = FALSE;	
        ClientToScreen(&point);
		int deltaX = point.x - m_dragPoint.x;
		int deltaY = point.y - m_dragPoint.y;
		m_dragPoint = point;
		m_dragRect.OffsetRect(deltaX, deltaY);
		m_x = m_dragRect.left;
		m_y = m_dragRect.top;
		m_cx = m_dragRect.Width();
		m_cy = m_dragRect.Height();
		SetWindowPos(NULL,m_x,m_y,m_cx,m_cy,SWP_NOZORDER);
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		CThemeDlg::m_x = m_dragRect.left;
		CThemeDlg::m_y = m_dragRect.top;
		CThemeDlg::m_cx = m_dragRect.Width();
		CThemeDlg::m_cy = m_dragRect.Height();
		CThemeDlg::SaveConfig();
	}
	else
	{
		if (m_resizing)
		{
			ShowWindow(SW_HIDE);
			UpdateWindow();
			double dX = (double)point.x - (double)m_midX;
			double dY = (double)point.y - (double)m_midY;
            double dist = sqrt( (dX*dX) + (dY*dY));
			CRect winRect;
			CRect client;
			GetWindowRect(winRect);
			GetClientRect(client);
			ClientToScreen(client);
			int difTop    = client.top - winRect.top;
			int difLeft   = client.left - winRect.left;
			int difRight  = winRect.right - client.right;
			int difBottom = winRect.bottom - client.bottom;
            
            int maxRadius = min(m_midX,m_midY);
			int newMaxRadius = (int)dist;
			CRect newClient(m_midX - newMaxRadius,
							m_midY - newMaxRadius,
							m_midX + newMaxRadius,
							m_midY + newMaxRadius);
			ClientToScreen(newClient);
			m_x = newClient.left;
			m_y = newClient.top;
			m_cx = newClient.Width();
			m_cy = newClient.Height();
            CRect newWindow(newClient.left   - difLeft,
                            newClient.top    - difTop,
                            newClient.right  + difRight,
                            newClient.bottom + difBottom);
            SetWindowPos(NULL,newWindow.left,newWindow.top,newWindow.Width(),newWindow.Height(),SWP_NOZORDER | SWP_NOREDRAW);          
            Invalidate();
			ShowWindow(SW_SHOW);
			UpdateWindow();
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			CThemeDlg::m_x = newWindow.left;
			CThemeDlg::m_y = newWindow.top;
			CThemeDlg::m_cx = newWindow.Width();
			CThemeDlg::m_cy = newWindow.Height();
			CThemeDlg::SaveConfig();
			m_resizing = FALSE;
			ReleaseCapture();
		}
	}
	CFrameWnd::OnLButtonUp(nFlags, point);
}

void CClockWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_mouseDown)
	{
        ClientToScreen(&point);
		int deltaX = point.x - m_dragPoint.x;
		int deltaY = point.y - m_dragPoint.y;
		m_dragPoint = point;
		m_dragRect.OffsetRect(deltaX, deltaY);
		m_x = m_dragRect.left;
		m_y = m_dragRect.top;
		m_cx = m_dragRect.Width();
		m_cy = m_dragRect.Height();
		SetWindowPos(NULL,m_x,m_y,m_cx,m_cy,SWP_NOZORDER);
        Invalidate();
		UpdateWindow();
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
	}
	else
	{
		if (m_resizing)
		{
			double dX = (double)point.x - (double)m_midX;
			double dY = (double)point.y - (double)m_midY;
			double phi = atan( -1.0 * dY / dX );
			if (phi > 3.0 * PI / 8.0)
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
			else if (phi > PI / 8.0)
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
			else if (phi > -1 * PI / 8.0)
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
			else if (phi > -3 * PI / 8.0)
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
			else
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
           
            double dist = sqrt( (dX*dX) + (dY*dY));

			CRect winRect;
			CRect client;
			GetWindowRect(winRect);
			GetClientRect(client);
			ClientToScreen(client);
			int difTop    = client.top - winRect.top;
			int difLeft   = client.left - winRect.left;
			int difRight  = winRect.right - client.right;
			int difBottom = winRect.bottom - client.bottom;
            
            int maxRadius = min(m_midX,m_midY);
			int newMaxRadius = (int)dist;
			CRect newClient(m_midX - newMaxRadius,
							m_midY - newMaxRadius,
							m_midX + newMaxRadius,
							m_midY + newMaxRadius);
			ClientToScreen(newClient);
            CRect newWindow(newClient.left   - difLeft,
                            newClient.top    - difTop,
                            newClient.right  + difRight,
                            newClient.bottom + difBottom);
			m_x  = newWindow.left;
			m_y  = newWindow.top;
			m_cx = newWindow.Width();
			m_cy = newWindow.Height();
            SetWindowPos(NULL,newWindow.left,newWindow.top,newWindow.Width(),newWindow.Height(),SWP_NOZORDER | SWP_NOREDRAW);          
            ScreenToClient(winRect);
            Invalidate();
	    	UpdateWindow();
   		}
        else
        {
	        if (TestMoveRegion(point))
	        {
		        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
		        return;
	        }

	        //was mouse near edge?
            if (TestResizeRegion(point))
	        {
		        //clicked near edge, prepare to grow window
	            int dX = point.x - m_midX;
    	        int dY = point.y - m_midY;
		        double phi = atan( -1.0 * (double)dY / (double)dX );
		        if (phi > 3.0 * PI / 8.0)
			        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		        else if (phi > PI / 8.0)
			        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
		        else if (phi > -1 * PI / 8.0)
			        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		        else if (phi > -3 * PI / 8.0)
			        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
		        else
			        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
	        }
        }
	}
	CFrameWnd::OnMouseMove(nFlags, point);
}

BOOL CClockWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_mouseDown)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
		return TRUE;
	}
	return CFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CClockWnd::OnSysColorChange()
{
	CWnd::OnSysColorChange();

	if (m_sysColor)
	{
		KillTimer(m_timer);

		//recreate the GDI Objects!
		m_bkBrush.DeleteObject();
		m_handsBrush.DeleteObject();
		m_bdrBrush.DeleteObject();
		m_tickBrush.DeleteObject();
		m_tick5Brush.DeleteObject();
		m_fontBrush.DeleteObject();
		m_scBrush.DeleteObject();
		m_numBrush.DeleteObject();

		m_bkPen.DeleteObject();
		m_handsPen.DeleteObject();
		m_bdrPen.DeleteObject();
		m_tickPen.DeleteObject();
		m_tick5Pen.DeleteObject();
		m_fontPen.DeleteObject();
		m_scPen.DeleteObject();
		m_numPen.DeleteObject();
		m_scWtPen.DeleteObject();

		//create GDI objects for use...
		m_bkBrush.CreateSolidBrush(m_bkColor);
		m_handsBrush.CreateSolidBrush(m_handsColor);
		m_bdrBrush.CreateSolidBrush(m_bdrColor);
		m_tickBrush.CreateSolidBrush(m_tickColor);
		m_tick5Brush.CreateSolidBrush(m_tick5Color);
		m_fontBrush.CreateSolidBrush(m_fontColor);
		m_scBrush.CreateSolidBrush(m_scColor);
		m_numBrush.CreateSolidBrush(m_numColor);

		m_bkPen.CreatePen   (PS_SOLID,1,m_bkColor);
		m_handsPen.CreatePen(PS_SOLID,1,m_handsColor);
		m_bdrPen.CreatePen  (PS_SOLID,1,m_bdrColor);
		m_tickPen.CreatePen (PS_SOLID,1,m_tickColor);
		m_tick5Pen.CreatePen(PS_SOLID,2,m_tick5Color);
		m_fontPen.CreatePen (PS_SOLID,1,m_fontColor);
		m_numPen.CreatePen  (PS_SOLID,1,m_numColor);
		m_scPen.CreatePen   (PS_SOLID,1,m_scColor);
		m_scWtPen.CreatePen (PS_SOLID,2,m_scColor);

		//reload the time... and start clock again
		m_hr = 0;
		m_mn = 0;
		m_sc = 0;
		LoadTime();
		UpdateWindow();
		m_timer = SetTimer( 1000, (1000/TICKCOUNT),NULL);	
	}
}
