// ClockWnd.cpp : implementation file
//
#include "stdafx.h"
#include "clock.h"
#include "ClockWnd.h"
#include "clockDlg.h"
#include "AboutDlg.h"
#include <math.h>
#define PI 3.141592654

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClockWnd

IMPLEMENT_DYNCREATE(CClockWnd, CFrameWnd)

CClockWnd::CClockWnd()
{
	m_mouseDown = FALSE;	
	m_rotate	= (0 < AfxGetApp()->GetProfileInt(_T("Init"),_T("Rotate"),0));
	m_roman		= (0 < AfxGetApp()->GetProfileInt(_T("Init"), _T("Roman"),0));
	m_quadOnly	= (0 < AfxGetApp()->GetProfileInt(_T("Init"), _T("QuadOnly"), 1));
	m_period	= AfxGetApp()->GetProfileInt(_T("Init"), _T("Period"), 1000);
	m_noTitle	= (0 < AfxGetApp()->GetProfileInt(_T("Init"), _T("NoTitle"), 0));
	m_x	        = AfxGetApp()->GetProfileInt(_T("Init"), _T("X"),100);
	m_y	        = AfxGetApp()->GetProfileInt(_T("Init"), _T("Y"),100);
	m_cx	    = AfxGetApp()->GetProfileInt(_T("Init"), _T("cx"),400);
	m_cy	    = AfxGetApp()->GetProfileInt(_T("Init"), _T("cy"),400);
}

CClockWnd::~CClockWnd()
{
	AfxGetApp()->WriteProfileInt(_T("Init"), _T("Rotate"),(m_rotate)?(1):(0) );
	AfxGetApp()->WriteProfileInt(_T("Init"), _T("Roman"),(m_roman)?(1):(0) );
	AfxGetApp()->WriteProfileInt(_T("Init"), _T("QuadOnly"), (m_quadOnly)?(1):(0) );
	AfxGetApp()->WriteProfileInt(_T("Init"), _T("Period"), m_period);
	AfxGetApp()->WriteProfileInt(_T("Init"), _T("NoTitle"), (m_noTitle)?(1):(0) );
	AfxGetApp()->WriteProfileInt(_T("Init"), _T("X"), m_x);
	AfxGetApp()->WriteProfileInt(_T("Init"), _T("Y"), m_y);
	AfxGetApp()->WriteProfileInt(_T("Init"), _T("cx"), m_cx);
	AfxGetApp()->WriteProfileInt(_T("Init"), _T("cy"), m_cy);
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClockWnd message handlers

void CClockWnd::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	if ( (nType != SIZE_MINIMIZED )&&
	     (nType != SIZE_MAXHIDE) )
	{
		m_midX = cx / 2;
		m_midY = cy / 2;
		int maxRadius = min(m_midX,m_midY);
		m_radiusHr = 5* maxRadius/10;
		m_radiusSc = 8* maxRadius/10;
		m_radiusMn = 8* maxRadius/10;
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
		CRgn wndRgn;
		wndRgn.CreateEllipticRgn(clipRect.left, clipRect.top, clipRect.right, clipRect.bottom);
		SetWindowRgn(wndRgn,TRUE);
		//Invalidate();
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
	LoadTime();
	CDC* myDC = GetDC();
	DrawHands(myDC);
	ReleaseDC(myDC);
}

void CClockWnd::OnPaint() 
{
	m_bkColor	= GetSysColor(COLOR_3DFACE);
	m_handsColor = GetSysColor(COLOR_BTNTEXT);
	m_scColor	= GetSysColor(COLOR_3DSHADOW);
	m_numColor	= GetSysColor(COLOR_BTNTEXT);
	CPaintDC dc(this); // device context for painting
	DrawFace(&dc);
	DrawHands(&dc);
}

CString CClockWnd::GetText(int number)
{
	UINT id = (m_roman)?(IDS_ROMAN):(IDS_ARABIC);
	CString allText;
	CString left;
	CString right;
	allText.LoadString(id);
	int i = 0;
	do
	{
		int charAt = allText.Find(',');
		left = allText.Left(charAt);
		right = allText.Right(allText.GetLength() - charAt -1);
		allText = right;
		i++;
	}
	while (i <= number);
	return (number == 11)?(right):(left);
}

void CClockWnd::DrawFace(CDC* dc)
{	
	CRgn faceRgn;
	int hold;
	COLORREF oldC;

	m_bkBrush.CreateSolidBrush(m_bkColor);
	m_scBrush.CreateSolidBrush(m_scColor);
	hold = dc->SetBkMode(TRANSPARENT);
	oldC = dc->SetTextColor(m_numColor);
	CRect client;
	GetClientRect(client);
	dc->FillRect(client,&m_bkBrush);
	
	int maxRadius = min(m_midX,m_midY);
	CRect faceRect(m_midX - maxRadius, m_midY - maxRadius,
		           m_midX + maxRadius, m_midY + maxRadius);
	faceRgn.CreateEllipticRgn(faceRect.left, faceRect.top, faceRect.right, faceRect.bottom);
	dc->FrameRgn(&faceRgn,&m_scBrush, 1,1);

	int i = 0;
	for (i = 0; i < 12; i++)
	{
		div_t q = div(i,(m_quadOnly)?(3):(1));
		if (q.rem == 0)
		{
			int dif = min(m_midX, m_midY) - m_radiusSc;
			double theta = (2.0 * PI * (double)i) / 12.0;
			POINT ctr;
			ctr.x = 0;
			ctr.y = -1 * m_radiusSc - dif/2;
			POINT ctrOrig;
			ctrOrig.x = 0;
			ctrOrig.y = -1 * m_radiusSc - dif/2;
			CFont tempFont;
			CString text = GetText(i);
			POINT temp;
			temp.x = ctr.x;
			temp.y = ctr.y;
			RotateAndOffset(ctr,theta);
			if (m_rotate)
			{
				tempFont.CreateFont(dif,0, 3600 - 300*i,
									0,400,0,0,0,0,0,0,0,0,
									"Times New Roman");
			}
			else
			{
				tempFont.CreateFont(dif,0, 0,
									0,400,0,0,0,0,0,0,0,0,
									"Times New Roman");
			}
			CFont* oldF = dc->SelectObject(&tempFont);
			CSize extent = dc->GetTextExtent(text);
			ctrOrig.x -= (extent.cx / 2);
			ctrOrig.y -= (extent.cy / 2);
			RotateAndOffset(ctrOrig,theta);
			int xPos = (m_rotate)?(ctrOrig.x):(ctr.x - extent.cx / 2);
			int yPos = (m_rotate)?(ctrOrig.y):(ctr.y - extent.cy / 2);
			dc->TextOut(xPos,yPos,text);

			dc->SelectObject(oldF);
		}
	}
	dc->SetBkMode(hold);
	dc->SetTextColor(oldC);
	m_bkBrush.DeleteObject();
	m_scBrush.DeleteObject();
	faceRgn.DeleteObject();
}

void CClockWnd::DrawHands(CDC* dc)
{

	m_bkBrush.CreateSolidBrush(m_bkColor);
	m_handsBrush.CreateSolidBrush(m_handsColor);
	m_scBrush.CreateSolidBrush(m_scColor);

	CPen bkPen;
	CPen scPen;
	CPen pen;
	bkPen.CreatePen(PS_SOLID,1,m_bkColor);
	scPen.CreatePen(PS_SOLID,1,m_scColor);
	pen.CreatePen(PS_SOLID,2,m_numColor);

	CRgn oldHr;
	CRgn oldMn;
	CRgn newHr;
	CRgn newMn;
	CRgn newHandsRgn;
	CRgn oldHandsRgn;
	CRgn difRgn;
	CreateHrHandRgn(oldHr, FALSE);
	CreateMnHandRgn(oldMn, FALSE);
	CreateHrHandRgn(newHr, TRUE);
	CreateMnHandRgn(newMn, TRUE);
	
	oldHandsRgn.CreateRectRgn(0,0,1,1);
	newHandsRgn.CreateRectRgn(0,0,1,1);
	difRgn.CreateRectRgn(0,0,1,1);
	oldHandsRgn.CombineRgn(&oldHr,&oldMn,RGN_OR);
	newHandsRgn.CombineRgn(&newHr,&newMn,RGN_OR);
	difRgn.CombineRgn(&oldHandsRgn, &newHandsRgn, RGN_DIFF);

	DrawScHand(dc, &bkPen, FALSE);
	dc->FillRgn(&difRgn, &m_bkBrush);

	//Draw Ticks
	POINT tick[2];
	int i = 0;
	for (i = 0; i < 60; i++)
	{
		int dif = min(m_midX, m_midY) - m_radiusSc;
		double theta = (2.0 * PI * (double)i) / 60.0;
		tick[0].x = 0;
		tick[0].y = -1* m_radiusSc;// + dif/3;
		tick[1].x = 0;
		tick[1].y = -1* m_radiusSc + dif/4;
		RotateAndOffset(tick[0],theta);
		RotateAndOffset(tick[1],theta);
		div_t q = div(i,5);
		CPen *oldPen = (q.rem == 0)? 
			(dc->SelectObject(&pen)):
			(dc->SelectObject(&scPen));
		dc->MoveTo(tick[0]);
		dc->LineTo(tick[1]);
		dc->SelectObject(oldPen);
	}
	
	WriteLogo(dc);

	dc->FillRgn(&newHandsRgn, & m_handsBrush);
	DrawScHand(dc, &scPen, TRUE);
	bkPen.DeleteObject();
	scPen.DeleteObject();
	pen.DeleteObject();
	
	newHandsRgn.DeleteObject();
	oldHandsRgn.DeleteObject();
	difRgn.DeleteObject();
	newHr.DeleteObject();
	oldHr.DeleteObject();
	newMn.DeleteObject();
	oldMn.DeleteObject();

	m_bkBrush.DeleteObject();
	m_handsBrush.DeleteObject();
	m_scBrush.DeleteObject();
}

void CClockWnd::CreateHrHandRgn(CRgn& rgn, bool current)
{
	POINT hand[4];
	hand[0].x = 0;
	hand[0].y = m_radiusHr / 5;
	hand[1].x = m_radiusHr / 10;
	hand[1].y = m_radiusHr / 6;
	hand[2].x = 0;
	hand[2].y = -1* m_radiusHr;
	hand[3].x = -1* m_radiusHr / 10;
	hand[3].y = m_radiusHr / 6;
	
	double theta;
	int hour = (current)?(m_hr):(m_pHr);
	int minute = (current)?(m_mn):(m_pMn);
	int second = (current)?(m_sc):(m_pSc);
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
	int minute = (current)?(m_mn):(m_pMn);
	int second = (current)?(m_sc):(m_pSc);
	theta = (double)(minute + second) * 2.0 * (double)PI / (60.0 * 60.0);

	RotateAndOffset(hand[0],theta);
	RotateAndOffset(hand[1],theta);
	RotateAndOffset(hand[2],theta);
	RotateAndOffset(hand[3],theta);

	rgn.CreatePolygonRgn(hand,4,WINDING);
}

void CClockWnd::DrawScHand(CDC* dc, CPen* pen, bool current)
{
	POINT hand[2];
	hand[0].x = 0;
	hand[0].y = 0;
	hand[1].x = 0;
	hand[1].y = -1* m_radiusSc;

	double theta;
	int second = (current)?(m_sc):(m_pSc);
	theta = (2.0 * PI * (double)second ) / (double)60.0;

	RotateAndOffset(hand[0],theta);
	RotateAndOffset(hand[1],theta);

	CPen* oldPen = dc->SelectObject(pen);
	dc->MoveTo(hand[0]);
	dc->LineTo(hand[1]);
	dc->SelectObject(oldPen);
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

	m_hr = 0;
	m_mn = 0;
	m_sc = 0;
	LoadTime();
	m_timer = SetTimer(1000,1000,NULL);	
	return 0;
}

void CClockWnd::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	KillTimer(m_timer);
	m_bkBrush.DeleteObject();
	m_handsBrush.DeleteObject();
	m_scBrush.DeleteObject();
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
		popup.AppendMenu(MF_STRING,IDC_ABOUT,"&About");
		popup.AppendMenu(MF_STRING,IDC_CONFIGURE,"&Configure");
		popup.AppendMenu(MF_STRING,IDC_EXIT,"E&xit");
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

	CClockDlg dlg;
	dlg.DoModal();

	//read in configuration
	m_rotate	= (0 < AfxGetApp()->GetProfileInt("Init","Rotate",0));
	m_roman		= (0 < AfxGetApp()->GetProfileInt("Init","Roman",0));
	m_quadOnly	= (0 < AfxGetApp()->GetProfileInt("Init","QuadOnly", 1));
	m_period	= AfxGetApp()->GetProfileInt("Init","Period", 1000);
	m_noTitle	= (0 < AfxGetApp()->GetProfileInt("Init","NoTitle", 0));
	m_x	        = AfxGetApp()->GetProfileInt("Init","X",100);
	m_y	        = AfxGetApp()->GetProfileInt("Init","Y",100);
	m_cx	    = AfxGetApp()->GetProfileInt("Init","cx",400);
	m_cy	    = AfxGetApp()->GetProfileInt("Init","cy",400);
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
		UpdateWindow();
	}
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
	COLORREF oldC = dc->SetTextColor(m_numColor);
	CRect client;
	GetClientRect(client);
	int dif = min(m_midX, m_midY) - m_radiusSc;
	POINT ctr;
	ctr.x = m_midX;
	ctr.y = m_midY + 1 * m_radiusSc - dif;
	CFont tempFont;
	CString text("GarneX");
	tempFont.CreateFont(2*dif/3,0, 0,
						0,400,0,0,0,0,0,0,0,0,
						"Times New Roman");
	CFont* oldF = dc->SelectObject(&tempFont);
	CSize extent = dc->GetTextExtent(text);
	int xPos = ctr.x - extent.cx / 2;
	int yPos = ctr.y - extent.cy / 2;
	dc->TextOut(xPos,yPos,text);
	dc->SelectObject(oldF);
	dc->SetBkMode(hold);
	dc->SetTextColor(oldC);
}

void CClockWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_mouseDown = TRUE;
	m_startDrag = point;
	m_dragPoint = point;
	GetWindowRect(m_dragRect);
	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CClockWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_mouseDown)
	{
		ReleaseCapture();
		m_mouseDown = FALSE;	
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
	}
	CFrameWnd::OnLButtonUp(nFlags, point);
}

void CClockWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_mouseDown)
	{
		int deltaX = point.x - m_dragPoint.x;
		int deltaY = point.y - m_dragPoint.y;
		m_dragPoint = point;
		m_dragRect.OffsetRect(deltaX, deltaY);
		m_x = m_dragRect.left;
		m_y = m_dragRect.top;
		m_cx = m_dragRect.Width();
		m_cy = m_dragRect.Height();
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
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
