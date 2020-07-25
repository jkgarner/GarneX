// ClockWnd.cpp : implementation file
//
#include "stdafx.h"
#include "ClockWnd.h"
#include "ClockDlg.h"
#include <math.h>
#define PI 3.141592654

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CClockWnd

CClockWnd saver;
CClockDlg saverSlg;

CClockWnd::CClockWnd()
{
	m_tick      = 0;
    m_tickCount = 8;
    m_useBlack  = FALSE;
    m_posMode   = 1;//fixed
    m_speed     = 5;
   	srand(time(NULL));
    RestoreOptions();
}

CClockWnd::~CClockWnd()
{
    SaveOptions();
}

BOOL CClockWnd::DestroyWindow()
{
	m_bkBrush.DeleteObject();
    m_bdrBrush.DeleteObject();
	m_tickBrush.DeleteObject();
	m_tick5Brush.DeleteObject();
	m_fontBrush.DeleteObject();
	m_scBrush.DeleteObject();
	m_mnBrush.DeleteObject();
	m_hrBrush.DeleteObject();
	m_numBrush.DeleteObject();

	m_bkPen.DeleteObject();
	m_bdrPen.DeleteObject();
	m_tickPen.DeleteObject();
	m_tick5Pen.DeleteObject();
	m_fontPen.DeleteObject();
	m_numPen.DeleteObject();
	m_scPen.DeleteObject();
	m_mnPen.DeleteObject();
	m_hrPen.DeleteObject();
	m_scWtPen.DeleteObject();
	m_blackPen.DeleteObject();
	m_blackWtPen.DeleteObject();
    m_hrBrushHigh.DeleteObject();
    m_hrBrushDark.DeleteObject();
    m_mnBrushHigh.DeleteObject();
    m_mnBrushDark.DeleteObject();
    m_scBrushHigh.DeleteObject();
    m_scBrushDark.DeleteObject();
	
	KillTimer(m_timer);	

    CWnd* desktop = GetDesktopWindow();
    CDC* ddc = desktop->GetDC();
    CDC memDC;
    if (m_background.m_hObject)
    {
        m_background.DeleteObject();
    }

    return TRUE;
}


bool CClockWnd::WriteProfileInt(HKEY hkey, CString name, DWORD value)
{
    BYTE* lpData = (BYTE*)&value;
    if (ERROR_SUCCESS == RegSetValueExA(hkey, name, NULL, REG_DWORD, lpData, REG_DWORD))
    {
        return true;
    }
    else
    {
        return false;
    }
}

DWORD CClockWnd::ReadProfileInt(HKEY hkey, CString name, DWORD defaultValue)
{
    DWORD size;
    PVOID pvData;
    DWORD dataType;
    if (ERROR_SUCCESS == RegGetValueA(hkey, NULL, name, RRF_RT_ANY, 
                                      &dataType, &pvData, &size))
    {
        return (DWORD)pvData;
    }
    else
    {
        return defaultValue;
    }
}


void CClockWnd::RestoreOptions()
{
    HKEY hkey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER,
        TEXT("SOFTWARE\\Garnex\\Saver"),
        0,
        KEY_READ,
        &hkey) == ERROR_SUCCESS
        )
    {
        m_rotate = ReadProfileInt(hkey, "Rotate", 1);
        m_roman = ReadProfileInt(hkey, "Roman", 1);
        m_quadOnly = ReadProfileInt(hkey, "QuadOnly", 0);
        m_x = ReadProfileInt(hkey, "X", 0);
        m_y = ReadProfileInt(hkey, "Y", 0);
        m_cx = ReadProfileInt(hkey, "cx", 500);
        m_cy = ReadProfileInt(hkey, "cy", 500);
        m_smooth = ReadProfileInt(hkey, "Smooth", 0);
        m_bkColor = ReadProfileInt(hkey, "Background", RGB(0, 0, 48));
        m_faceColor = ReadProfileInt(hkey, "Face", RGB(255, 255, 255));
        m_bdrColor = ReadProfileInt(hkey, "Border", RGB(0, 0, 0));
        m_scColor = ReadProfileInt(hkey, "SecondHand", RGB(128, 0, 0));
        m_mnColor = ReadProfileInt(hkey, "MinuteHand", RGB(64, 64, 64));
        m_hrColor = ReadProfileInt(hkey, "HourHand", RGB(32, 32, 32));
        m_fontColor = ReadProfileInt(hkey, "Logo", RGB(96, 96, 96));
        m_tick5Color = ReadProfileInt(hkey, "5Minute", RGB(96, 96, 96));
        m_tickColor = ReadProfileInt(hkey, "1Minute", RGB(128, 128, 128));
        m_numColor = ReadProfileInt(hkey, "Numbers", RGB(96, 96, 96));
        m_tickCount = ReadProfileInt(hkey, "TickCount", 8);
        m_posMode = ReadProfileInt(hkey, "PosMode", 2);
        m_bkMode = ReadProfileInt(hkey, "BkMode", 0);
        m_speed = ReadProfileInt(hkey, "Speed", 5);
        m_wipes = ReadProfileInt(hkey, "Wipes", 0);
        m_shadows = ReadProfileInt(hkey, "Shadows", 1);
        RegCloseKey(hkey);
    }
}

void CClockWnd::SaveOptions()
{
    HKEY hkey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER,
        TEXT("SOFTWARE\\Garnex\\Saver"),
        0,
        KEY_ALL_ACCESS,
        &hkey) == ERROR_SUCCESS
        )
    {
        WriteProfileInt(hkey, "Rotate", m_rotate);
        WriteProfileInt(hkey, "Roman", m_roman);
        WriteProfileInt(hkey, "QuadOnly", m_quadOnly);
        WriteProfileInt(hkey, "X", m_x);
        WriteProfileInt(hkey, "Y", m_y);
        WriteProfileInt(hkey, "cx", m_cx);
        WriteProfileInt(hkey, "cy", m_cy);
        WriteProfileInt(hkey, "Smooth", m_smooth);
        WriteProfileInt(hkey, "Background", m_bkColor);
        WriteProfileInt(hkey, "Face", m_faceColor);
        WriteProfileInt(hkey, "Border", m_bdrColor);
        WriteProfileInt(hkey, "SecondHand", m_scColor);
        WriteProfileInt(hkey, "MinuteHand", m_mnColor);
        WriteProfileInt(hkey, "HourHand", m_hrColor);
        WriteProfileInt(hkey, "Logo", m_fontColor);
        WriteProfileInt(hkey, "5Minute", m_tick5Color);
        WriteProfileInt(hkey, "1Minute", m_tickColor);
        WriteProfileInt(hkey, "Numbers", m_numColor);
        WriteProfileInt(hkey, "TickCount", m_tickCount);
        WriteProfileInt(hkey, "PosMode", m_posMode);
        WriteProfileInt(hkey, "BkMode", m_bkMode);
        WriteProfileInt(hkey, "Speed", m_speed);
        WriteProfileInt(hkey, "Wipes", m_wipes);
        WriteProfileInt(hkey, "Shadows", m_shadows);
        RegCloseKey(hkey);
    }
}


BEGIN_MESSAGE_MAP(CClockWnd, CScreenSaverWnd)
	//{{AFX_MSG_MAP(CClockWnd)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

COLORREF Lighten( COLORREF old)
{
    int r = min(255, GetRValue(old)*11/10);
    int g = min(255, GetGValue(old)*11/10);
    int b = min(255, GetBValue(old)*11/10);
    return RGB( r,g,b);
}

COLORREF Darken( COLORREF old)
{
    int r = max(0, GetRValue(old)*9/10);                             
    int g = max(0, GetGValue(old)*9/10);
    int b = max(0, GetBValue(old)*9/10);
    return RGB( r,g,b);
}


/////////////////////////////////////////////////////////////////////////////
// CClockWnd message handlers
void CClockWnd::OnInitialUpdate()
{
	//create GDI objects for use...
	m_bkBrush.CreateSolidBrush(m_faceColor);
	m_bdrBrush.CreateSolidBrush(m_bdrColor);    
	m_tickBrush.CreateSolidBrush(m_tickColor);
	m_tick5Brush.CreateSolidBrush(m_tick5Color);
	m_fontBrush.CreateSolidBrush(m_fontColor);
	m_scBrush.CreateSolidBrush(m_scColor);
	m_mnBrush.CreateSolidBrush(m_mnColor);
	m_hrBrush.CreateSolidBrush(m_hrColor);
	m_numBrush.CreateSolidBrush(m_numColor);

	m_bkPen.CreatePen   (PS_SOLID,1,m_faceColor);
	m_bdrPen.CreatePen  (PS_SOLID,1,m_bdrColor);
	m_tickPen.CreatePen (PS_SOLID,1,m_tickColor);
	m_tick5Pen.CreatePen(PS_SOLID,2,m_tick5Color);
	m_fontPen.CreatePen (PS_SOLID,1,m_fontColor);
	m_numPen.CreatePen  (PS_SOLID,1,m_numColor);
	m_scPen.CreatePen   (PS_SOLID,1,m_scColor);
	m_mnPen.CreatePen   (PS_SOLID,1,m_mnColor);
	m_hrPen.CreatePen   (PS_SOLID,1,m_hrColor);
	m_scWtPen.CreatePen (PS_SOLID,2,m_scColor);
	m_blackPen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	m_blackWtPen.CreatePen(PS_SOLID,2,RGB(0,0,0));

    //handshading brushes...
    m_hrBrushHigh.CreateSolidBrush( Lighten(m_hrColor) );
    m_hrBrushDark.CreateSolidBrush( Darken(m_hrColor) );
    m_mnBrushHigh.CreateSolidBrush( Lighten(m_mnColor) );
    m_mnBrushDark.CreateSolidBrush( Darken(m_mnColor) );
    m_scBrushHigh.CreateSolidBrush( Lighten(m_scColor) );
    m_scBrushDark.CreateSolidBrush( Darken(m_scColor) );

	m_hr = 0;
	m_mn = 0;
	m_sc = 0;
	LoadTime();
    if ((m_posMode < 1) && (m_smooth == FALSE))
    {
        m_timer = SetTimer( 1000,1000,NULL);	
    }
    else
    {
	    m_timer = SetTimer( 1000, (1000/m_tickCount),NULL);	
    }

    //constrain the clock display to the client window!
    if (m_x < 0) m_x = 0;
    if (m_y < 0) m_y = 0;
    if (m_cx < 0) m_cx = 400;
    if (m_cy < 0) m_cy = 400;
    GetClientRect(m_bounds);

    CRect deskRect;
    CWnd* desktop = GetDesktopWindow();
    ::GetClientRect(desktop->m_hWnd,deskRect);

    BOOL preview = TRUE;
    if (m_bounds.bottom != deskRect.bottom ||
        m_bounds.right != deskRect.right)
    {
        preview = TRUE;
        m_x  = max(m_bounds.Width()/3, m_x  * m_bounds.Width() / deskRect.Width());
        m_y  = max(m_bounds.Height()/2, m_y  * m_bounds.Height() / deskRect.Height());
        m_cx = max(m_bounds.Width()/3, m_cx * m_bounds.Width() / deskRect.Width());
        m_cy = max(m_bounds.Height()/2, m_cy * m_bounds.Height() / deskRect.Height());
    }
    if (m_cx >= m_bounds.Width())
    {
        m_cx = m_bounds.Width() -1;
    }
    if (m_cy >= m_bounds.Height())
    {
        m_cy = m_bounds.Height() -1;
    }
    if ((m_x + m_cx) >= m_bounds.Width())
    {
        m_x = m_bounds.Width() - m_cx -1;
    }
    if ((m_y + m_cy) >= m_bounds.Height())
    {
        m_y = m_bounds.Height() - m_cy -1;
    }
  
    int radiusX = m_cx / 2;
	int radiusY = m_cy / 2;
	m_maxRadius = min(radiusX,radiusY);

    if      (m_posMode == 0) // center
    {
        m_midX = m_bounds.Width() / 2;
        m_midY = m_bounds.Height() / 2;
    }
    else if (m_posMode == 1) // fixed
    {
	    m_midX = m_x + radiusX;
        m_midY = m_y + radiusY;
    }
    else  // m_posMode == 2  // floating
    {       
        //randomly set angle!
        m_angle = (rand() % 160 - 80);//+80 to -80
        if ((rand() % 2) == 0) m_angle+= 180;

        //randomly set start position
        m_midX  = rand() % (m_bounds.Width()  - m_cx) + m_maxRadius;
        m_midY  = rand() % (m_bounds.Height() - m_cy) + m_maxRadius;
        m_x    = m_midX - m_maxRadius;
        m_y    = m_midY - m_maxRadius;
        m_cx   = m_maxRadius * 2;
        m_cy   = m_maxRadius * 2;
    }

    m_radiusHr = 63 * m_maxRadius/100;//5* m_maxRadius/10;
	m_radiusSc = 95 * m_maxRadius/100;//8* m_maxRadius/10;
	m_radiusMn = 95 * m_maxRadius/100;//8* m_maxRadius/10;

    CDC* ddc = desktop->GetDC();
    CDC memDC;
    if (memDC.CreateCompatibleDC(ddc))
    {
	    CRect winRect;
	    GetWindowRect(winRect);

        m_background.CreateCompatibleBitmap(ddc,winRect.Width(), winRect.Height());
        CBitmap* oldMemBmp = memDC.SelectObject(&m_background);

	    if (m_bkMode == 0)   // color
	    { 
            //would clear to color but I disabled...
		    CRect rcClient;
		    GetClientRect(&rcClient);
            memDC.FillSolidRect(&rcClient, m_bkColor);
	    }
	    else if (m_bkMode == 1) // black
	    {
            //clears to black 
		    CRect rcClient;
		    GetClientRect(&rcClient);
		    memDC.FillSolidRect(&rcClient, RGB(0, 0, 0));
	    }
        else 
        {
            //need to store the oringal desktop image for use later...   		    
            memDC.StretchBlt(0,0,winRect.Width(),winRect.Height(),ddc,
                             0,0,deskRect.Width(),deskRect.Height(),SRCCOPY);
	    }
	    memDC.SelectObject(oldMemBmp);
        memDC.DeleteDC();
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

void CClockWnd::SetHandPolygons()
{
    m_hrHand[0].x = 0;
    m_hrHand[0].y = m_radiusHr / 8; //5;
    m_hrHand[1].x = m_radiusHr / 10;
    m_hrHand[1].y = m_radiusHr / 10; //6;
    m_hrHand[2].x = 0;
    m_hrHand[2].y = -1* m_radiusHr;
    m_hrHand[3].x = -1* m_radiusHr / 10;
    m_hrHand[3].y = m_radiusHr / 10; //6;

	m_mnHand[0].x = 0;
	m_mnHand[0].y = m_radiusMn / 8;
	m_mnHand[1].x = m_radiusMn / 15;
	m_mnHand[1].y = m_radiusMn / 10;
	m_mnHand[2].x = 0;
	m_mnHand[2].y = -1* m_radiusMn;
	m_mnHand[3].x = -1* m_radiusMn / 15;
	m_mnHand[3].y = m_radiusMn / 10;

    m_scHand[0].x = 0;
    m_scHand[0].y = (m_radiusSc/5 + m_radiusSc/150);
    m_scHand[1].x = m_radiusSc/50;
    m_scHand[1].y = m_radiusSc/5;
    m_scHand[2].x = 0;
    m_scHand[2].y = -1 * m_radiusSc;
    m_scHand[3].x = -1 * m_radiusSc/50;
    m_scHand[3].y = m_radiusSc/5;
}

void CClockWnd::MoveClockFace()
{
    double trueSpeed = max(2.0,((double)m_speed/10.0)* (48.0 / (double)m_tickCount));
    double dX = trueSpeed * cos(m_angle*PI/180.0);
    double dY = trueSpeed * sin(m_angle*PI/180.0);
    int newX = m_x + dX;
    int newY = m_y + dY;
    if ( (newX < 0) || (newX + m_cx > m_bounds.Width()) )
    {
        //bounce off side!
        m_angle = 180 - m_angle;
        m_angle = (m_angle + (rand() % 15 - 7));
        while (m_angle > 180)  m_angle -= 360;             
        while (m_angle < -180) m_angle += 360;             
        if ( abs(m_angle) < 5)
        {
            if (abs(m_angle) == m_angle)
                m_angle = 5;
            else
                m_angle = -5;
        }
        else if ( ( abs(m_angle) > 85) && ( abs(m_angle) < 90) )
        {
            if (abs(m_angle) == m_angle)
                m_angle = 85;
            else
                m_angle = -85;
        }
        else if ( ( abs(m_angle) > 90) && ( abs(m_angle) < 95) )
        {
            if (abs(m_angle) == m_angle)
                m_angle = 95;
            else
                m_angle = -95;
        }
        else if ( abs(m_angle) > 175) 
        {
            if (abs(m_angle) == m_angle)
                m_angle = 175;
            else
                m_angle = -175;
        }
        if (newX < 0)  m_x = 1;
        else           m_x = m_bounds.Width() - m_cx;
    }
    else
    {
        //good x value!
        m_x = newX;
    }

    if ( (newY < 0) || (newY + m_cy > m_bounds.Height()) )
    {
        //bounce off top/bottom!
        m_angle = -1* m_angle;
        m_angle = (m_angle + (rand() % 15 - 8));
        while (m_angle > 180)  m_angle -= 360;             
        while (m_angle < -180) m_angle += 360;             
        if ( abs(m_angle) < 5)
        {
            if (abs(m_angle) == m_angle)
                m_angle = 5;
            else
                m_angle = -5;
        }
        else if ( ( abs(m_angle) > 85) && ( abs(m_angle) < 90) )
        {
            if (abs(m_angle) == m_angle)
                m_angle = 85;
            else
                m_angle = -85;
        }
        else if ( ( abs(m_angle) > 90) && ( abs(m_angle) < 95) )
        {
            if (abs(m_angle) == m_angle)
                m_angle = 95;
            else
                m_angle = -95;
        }
        else if ( abs(m_angle) > 175) 
        {
            if (abs(m_angle) == m_angle)
                m_angle = 175;
            else
                m_angle = -175;
        }

        if (newY < 0)  m_y = 1;
        else           m_y = m_bounds.Height() - m_cy;
    }
    else
    {
        //good y value!
        m_y = newY;
    }
    m_midX = m_x + m_maxRadius;
    m_midY = m_y + m_maxRadius;
}

void CClockWnd::OnTimer(UINT nIDEvent) 
{
	CScreenSaverWnd::OnTimer(nIDEvent);
    if (m_posMode > 1) // floating
    {
        MoveClockFace();
    }

    if (m_smooth)
    {
    	m_tick++;
	    if (m_tick == m_tickCount)
	    {
		    LoadTime();
		    m_tick = 0;
	    }
        SetAngles(TRUE);
        SetHandPolygons();
        Invalidate();
        UpdateWindow();
    }
    else
    {        
        LoadTime();
        if ((m_sc != m_pSc) || (m_posMode > 1))
        {
            SetAngles(TRUE);
            SetHandPolygons();
            Invalidate();
            UpdateWindow();
        }
    }
}

void CClockWnd::SetAngles(BOOL current)
{
	int hour = (current)?(m_hr):(m_pHr);
	int minute = (current)?(m_mn):(m_pMn);
    int second = m_sc;

    int maxTick = (m_smooth)?(m_tickCount):(1);
    if (m_tick == maxTick)
	    second = m_sc;
    int numTicks = second * (maxTick) + m_tick;

    m_thetaHr = (double)(hour + minute + second) * 2.0 * (double)PI / (12.0 * 60.0 * 60.0);
    while (m_thetaHr > 2 * PI) m_thetaHr -= (2 * PI);
    while (m_thetaHr < 0) m_thetaHr += (2 * PI);

	m_thetaMn = (double)(minute + second) * 2.0 * (double)PI / (60.0 * 60.0);
    while (m_thetaMn > 2 * PI) m_thetaMn -= (2 * PI);
    while (m_thetaMn < 0) m_thetaMn += (2 * PI);

    m_thetaSc = (2.0 * PI * (double)numTicks) / (60.0 * (double)maxTick);
    while (m_thetaSc > 2 * PI) m_thetaSc -= (2 * PI);
    while (m_thetaSc < 0) m_thetaSc += (2 * PI);
}

void CClockWnd::DrawBackground(CDC* dc)
{
	CRect winRect;
	GetWindowRect(winRect);
    CDC memDC;
    if (memDC.CreateCompatibleDC(dc))
    {
        CBitmap* oldMemBmp = memDC.SelectObject(&m_background);
        dc->BitBlt(0,0,winRect.Width(), winRect.Height(),
	              &memDC, 0,0, SRCCOPY);
        memDC.SelectObject(oldMemBmp);
        memDC.DeleteDC();
    }
}

void CClockWnd::Draw(CDC* dc)
{
    DrawBackground(dc);
    DrawFace(dc);
    DrawHands(dc);
}

void CClockWnd::OnDraw(CDC* pDC) 
{
	CDC memDC;
	if (memDC.CreateCompatibleDC(pDC))
	{
		CRect winRect;
		GetWindowRect(winRect);

		CBitmap memBmp;
		memBmp.CreateCompatibleBitmap(pDC,winRect.Width(), winRect.Height());
		CBitmap* oldMemBmp = memDC.SelectObject(&memBmp);
        Draw(&memDC);
		pDC->BitBlt(0,0,winRect.Width(), winRect.Height(),
			      &memDC, 0,0, SRCCOPY);
		memDC.SelectObject(oldMemBmp);
	}
	else
	{
		Draw(pDC);
	}
    if (m_wipes)
    {
        //save background!
	    CRect winRect;
	    GetWindowRect(winRect);
        CDC memDC;
        if (memDC.CreateCompatibleDC(pDC))
        {
	        CBitmap* oldMemBmp = memDC.SelectObject(&m_background);
            memDC.BitBlt(0,0,winRect.Width(), winRect.Height(),
	                  pDC, 0,0, SRCCOPY);
            memDC.SelectObject(oldMemBmp);
            memDC.DeleteDC();
        }
    }
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

void CClockWnd::DrawTicks(CDC* dc)
{
	//Draw Ticks
	POINT tick[2];
    int dif = m_maxRadius - m_radiusSc;
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

			//int dif = m_maxRadius - m_radiusSc;
            int dif = m_maxRadius - m_radiusHr;
			double theta = (2.0 * PI * (double)i) / 12.0;

            if (m_rotate)
			{
                int rot = 3600 - 300*i;
                if (i>3&&i<9)
                {
                    rot += 1800;
                    //if (rot > 3600) rot -= 3600;
                }
				//tempFont.CreateFont(dif,0, rot,
				//tempFont.CreateFont(dif/2,0, rot,
                tempFont.CreateFont(2*dif/3,0, rot,
									0,400,0,0,0,0,0,0,0,0,
									"Times New Roman");
			}
			else
			{
				//tempFont.CreateFont(dif,0, 0,
				//tempFont.CreateFont(dif/2,0, 0,
                tempFont.CreateFont(2*dif/3,0, 0,
									0,400,0,0,0,0,0,0,0,0,
									"Times New Roman");
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
	
	CRect faceRect(m_midX - m_maxRadius, m_midY - m_maxRadius,
		           m_midX + m_maxRadius, m_midY + m_maxRadius);
	faceRgn.CreateEllipticRgn(faceRect.left, faceRect.top, faceRect.right, faceRect.bottom);
	dc->FillRgn(&faceRgn, &m_bkBrush);
	dc->FrameRgn(&faceRgn,&m_bdrBrush, m_maxRadius/75,m_maxRadius/75);
    
    DrawTicks(dc);
    DrawNumbers(dc);

	WriteLogo(dc);

	dc->SetBkMode(hold);
	dc->SetTextColor(oldC);
    //DrawDebugData(dc);
	faceRgn.DeleteObject();
}

void CClockWnd::ShadeHand(CDC* dc, double theta, CRgn *handRgn, CRgn *leftRgn, CRgn *rightRgn, CBrush* regB, CBrush *lightB, CBrush * darkB)
{    
    theta *= -1;
    theta += PI/2;
    while (theta > 2 * PI) theta -= (2 * PI);
    while (theta < 0) theta += (2 * PI);
    if      (theta < (PI/2))
    {
        dc->FillRgn(leftRgn,lightB);
        dc->FillRgn(rightRgn,darkB);
    }
    else if (theta < (5*PI/8))
    {
        dc->FillRgn(leftRgn,lightB);
        dc->FillRgn(rightRgn,regB);
    }
    else if (theta < (7*PI/8))
    {
        dc->FillRgn(leftRgn,lightB);
        dc->FillRgn(rightRgn,lightB);
    }
    else if (theta < PI)
    {
        dc->FillRgn(leftRgn,regB);
        dc->FillRgn(rightRgn,lightB);
    }
    else if (theta < 3 * PI / 2)
    {
        dc->FillRgn(leftRgn,darkB);
        dc->FillRgn(rightRgn,lightB);
    }
    else if (theta < (13*PI/8))
    {
        dc->FillRgn(leftRgn,darkB);
        dc->FillRgn(rightRgn,regB);
    }
    else if (theta < (15*PI/8))
    {
        dc->FillRgn(leftRgn,darkB);
        dc->FillRgn(rightRgn,darkB);
    }
    else
    {
        dc->FillRgn(leftRgn,darkB);
        dc->FillRgn(rightRgn,regB);
    }
    //dc->FillRgn(leftRgn,darkB);
    //dc->FillRgn(rightRgn,lightB);
}

void CClockWnd::DrawHands(CDC* dc)
{
	CRgn newMn;
	CRgn newMn1;
	CRgn newMn2;

    CRgn newHr;
    CRgn newHr1;
    CRgn newHr2;

    CRgn newSc;
    CRgn newSc1;
    CRgn newSc2;

	CreateScHandRgn(newSc);
	CreateScHand1Rgn(newSc1);
	CreateScHand2Rgn(newSc2);

    CreateMnHandRgn(newMn);	
    CreateMnHand1Rgn(newMn1);	
    CreateMnHand2Rgn(newMn2);	

    CreateHrHandRgn(newHr);
    CreateHrHand1Rgn(newHr1);
    CreateHrHand2Rgn(newHr2);

    //shadows
    if (m_shadows)
    {
        CRgn shadow;    
        shadow.CreateRectRgn(0,0,1,1);
        shadow.CombineRgn(&newHr,&newMn,RGN_OR);
        if (m_maxRadius >= 250)
        {            
            shadow.CombineRgn(&shadow,&newSc,RGN_OR);
        }
        DrawShadow(dc, &shadow);
        shadow.DeleteObject();
    }

    if (m_shadows)
    {
        dc->FrameRgn(&newHr, & m_hrBrush, 1, 1);       
        ShadeHand(dc, m_thetaHr, &newHr, &newHr1, &newHr2,
            &m_hrBrush, &m_hrBrushHigh, &m_hrBrushDark);
        dc->FrameRgn(&newMn, & m_mnBrush, 1, 1);       
        ShadeHand(dc, m_thetaMn, &newMn, &newMn1, &newMn2,
            &m_mnBrush, &m_mnBrushHigh, &m_mnBrushDark);
    }
    else
    {
        dc->FrameRgn(&newHr, & m_hrBrush, 1, 1);       
    	dc->FillRgn(&newHr, & m_hrBrush);
        dc->FrameRgn(&newMn, & m_mnBrush, 1, 1);       
    	dc->FillRgn(&newMn, & m_mnBrush);
    }


    if (m_maxRadius < 250)
    {
    	DrawScHand(dc, FALSE);
    }
    else
    {
        if (m_shadows)
        {
	        dc->FrameRgn(&newSc, & m_scBrush, 1, 1);       
            ShadeHand(dc, m_thetaSc, &newSc, &newSc1, &newSc2,
                &m_scBrush, &m_scBrushHigh, &m_scBrushDark);
        }
        else
        {
	        dc->FrameRgn(&newSc, & m_scBrush, 1, 1);       
	        dc->FillRgn(&newSc, & m_scBrush);       
        }
    }

    //cleanup regions
    newHr.DeleteObject();
    newHr1.DeleteObject();
    newHr2.DeleteObject();

    newMn.DeleteObject();
    newMn1.DeleteObject();
    newMn2.DeleteObject();

    newSc.DeleteObject();
    newSc1.DeleteObject();
    newSc2.DeleteObject();
}

//must be called after draw face, after creation of hand regions and before drawing of hands...
void CClockWnd::DrawShadow(CDC *dc, CRgn *shadow)
{
    CDC memDC;
    if (memDC.CreateCompatibleDC(dc))
    {
	    CRect winRect;
	    GetWindowRect(winRect);
        int winCx = winRect.Width();
        int winCy = winRect.Height();

		CBitmap memBmp;
        CBrush blackB;

        BLENDFUNCTION blend;
        blend.BlendOp = AC_SRC_OVER;
        blend.BlendFlags = 0;
        blend.SourceConstantAlpha = 0x7f;
        blend.AlphaFormat = 0;

		//copy clock face to bitmap...
        memBmp.CreateCompatibleBitmap(dc, winCx, winCy);
		CBitmap* oldMemBmp = memDC.SelectObject(&memBmp);
        memDC.BitBlt(0,0, winCx, winCy,dc,0,0,SRCCOPY);  
        shadow->OffsetRgn(m_maxRadius/50,m_maxRadius/50);
        blackB.CreateSolidBrush(RGB(0,0,0));
        memDC.FrameRgn(shadow,&blackB,1,1);
        memDC.FillRgn(shadow, &blackB);
        if (m_maxRadius < 250)
        {
            DrawScHand(&memDC,TRUE);
        }
        dc->AlphaBlend(0,0, winCx, winCy, &memDC,0,0, winCx, winCy,blend);
		memDC.SelectObject(oldMemBmp);
    }
}

void CClockWnd::CreateHrHandRgn(CRgn& rgn)
{
	POINT hand[4];
	hand[0] = m_hrHand[0];
	hand[1] = m_hrHand[1];
	hand[2] = m_hrHand[2];
	hand[3] = m_hrHand[3];
	RotateAndOffset(hand[0],m_thetaHr);
	RotateAndOffset(hand[1],m_thetaHr);
	RotateAndOffset(hand[2],m_thetaHr);
	RotateAndOffset(hand[3],m_thetaHr);
	rgn.CreatePolygonRgn(hand,4, WINDING);
}

void CClockWnd::CreateHrHand1Rgn(CRgn& rgn)
{
	POINT hand[4];
	hand[0] = m_hrHand[2];
	hand[1] = m_hrHand[3];
	hand[2] = m_hrHand[0];	
	RotateAndOffset(hand[0],m_thetaHr);
	RotateAndOffset(hand[1],m_thetaHr);
	RotateAndOffset(hand[2],m_thetaHr);
	rgn.CreatePolygonRgn(hand,3, WINDING);
}

void CClockWnd::CreateHrHand2Rgn(CRgn& rgn)
{
	POINT hand[3];
	hand[0] = m_hrHand[0];
	hand[1] = m_hrHand[1];
	hand[2] = m_hrHand[2];
	RotateAndOffset(hand[0],m_thetaHr);
	RotateAndOffset(hand[1],m_thetaHr);
	RotateAndOffset(hand[2],m_thetaHr);
	rgn.CreatePolygonRgn(hand,3, WINDING);
}

void CClockWnd::CreateMnHandRgn(CRgn &rgn)
{
	POINT hand[4];
	hand[0] = m_mnHand[0];
	hand[1] = m_mnHand[1];
	hand[2] = m_mnHand[2];
	hand[3] = m_mnHand[3];
	RotateAndOffset(hand[0],m_thetaMn);
	RotateAndOffset(hand[1],m_thetaMn);
	RotateAndOffset(hand[2],m_thetaMn);
	RotateAndOffset(hand[3],m_thetaMn);
	rgn.CreatePolygonRgn(hand,4,WINDING);
}

void CClockWnd::CreateMnHand1Rgn(CRgn &rgn)
{
	POINT hand[3];
	hand[0] = m_mnHand[2];
	hand[1] = m_mnHand[3];
	hand[2] = m_mnHand[0];
	RotateAndOffset(hand[0],m_thetaMn);
	RotateAndOffset(hand[1],m_thetaMn);
	RotateAndOffset(hand[2],m_thetaMn);
	rgn.CreatePolygonRgn(hand,3,WINDING);
}

void CClockWnd::CreateMnHand2Rgn(CRgn &rgn)
{
	POINT hand[3];
	hand[0] = m_mnHand[0];
	hand[1] = m_mnHand[1];
	hand[2] = m_mnHand[2];
	RotateAndOffset(hand[0],m_thetaMn);
	RotateAndOffset(hand[1],m_thetaMn);
	RotateAndOffset(hand[2],m_thetaMn);
	rgn.CreatePolygonRgn(hand,3,WINDING);
}

void CClockWnd::CreateScHandRgn(CRgn &rgn)
{
    //create sec hand region...    
    POINT hand[4];
    hand[0] = m_scHand[0];
    hand[1] = m_scHand[1];
    hand[2] = m_scHand[2];
    hand[3] = m_scHand[3];
    RotateAndOffset(hand[0],m_thetaSc);
    RotateAndOffset(hand[1],m_thetaSc);
    RotateAndOffset(hand[2],m_thetaSc);
    RotateAndOffset(hand[3],m_thetaSc);
    rgn.CreatePolygonRgn(hand,4,WINDING);
}

void CClockWnd::CreateScHand1Rgn(CRgn &rgn)
{
    //create sec hand region...    
    POINT hand[4];
    hand[0] = m_scHand[2];
    hand[1] = m_scHand[3];
    hand[2] = m_scHand[0];
    RotateAndOffset(hand[0],m_thetaSc);
    RotateAndOffset(hand[1],m_thetaSc);
    RotateAndOffset(hand[2],m_thetaSc);
    rgn.CreatePolygonRgn(hand,3,WINDING);
}

void CClockWnd::CreateScHand2Rgn(CRgn &rgn)
{
    //create sec hand region...    
    POINT hand[4];
    hand[0] = m_scHand[0];
    hand[1] = m_scHand[1];
    hand[2] = m_scHand[2];
    RotateAndOffset(hand[0],m_thetaSc);
    RotateAndOffset(hand[1],m_thetaSc);
    RotateAndOffset(hand[2],m_thetaSc);
    rgn.CreatePolygonRgn(hand,3,WINDING);
}

void CClockWnd::DrawScHand(CDC* dc, BOOL shadow)
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
    int maxTick = (m_smooth)?(m_tickCount):(1);
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
    CPen *scPen   = &m_scPen;
    CPen *scWtPen = &m_scWtPen;
    if (shadow)
    {
        hand[0].x += m_maxRadius/50;
        hand[0].y += m_maxRadius/50;
        hand[1].x += m_maxRadius/50;
        hand[1].y += m_maxRadius/50;
        hand[2].x += m_maxRadius/50;
        hand[2].y += m_maxRadius/50;
        hand[3].x += m_maxRadius/50;
        hand[3].y += m_maxRadius/50;
        hand[4].x += m_maxRadius/50;
        hand[4].y += m_maxRadius/50;
        hand[5].x += m_maxRadius/50;
        hand[5].y += m_maxRadius/50;
        hand[6].x += m_maxRadius/50;
        hand[6].y += m_maxRadius/50;
        hand[7].x += m_maxRadius/50;
        hand[7].y += m_maxRadius/50;
        hand[8].x += m_maxRadius/50;
        hand[8].y += m_maxRadius/50;
        hand[9].x += m_maxRadius/50;
        hand[9].y += m_maxRadius/50;        
        scPen   = &m_blackPen;
        scWtPen = &m_blackWtPen;
    }

    CPen* oldPen;
    if (m_cx < 250)
    {
        oldPen = dc->SelectObject(scPen);
    }
    else
    {
        oldPen = dc->SelectObject(scWtPen);
    }

    dc->MoveTo(hand[1]);
    dc->LineTo(hand[0]);
    dc->SelectObject(scWtPen);
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


void CClockWnd::WriteLogo(CDC* dc)
{
	int hold = dc->SetBkMode(TRANSPARENT);
	COLORREF oldC = dc->SetTextColor(m_fontColor);
	CRect client;
	GetClientRect(client);
	//int dif = min(m_midX, m_midY) - m_radiusSc;
    int dif = m_maxRadius - m_radiusHr;
	POINT ctr;
	ctr.x = m_midX;
	//ctr.y = m_midY + 1 * m_radiusSc - dif;
    ctr.y = m_midY + 1 * m_radiusHr - dif/4;
	CFont tempFont;
	CString text("GarneX");
	//tempFont.CreateFont(2*dif/3,0, 0,
    tempFont.CreateFont(9*dif/24,0, 0,
						0,400,0,0,0,0,0,0,0,0,
						//"Times New Roman");
                        "Georgia");
	CFont* oldF = dc->SelectObject(&tempFont);
	CSize extent = dc->GetTextExtent(text);
	int xPos = ctr.x - extent.cx / 2;
	int yPos = ctr.y - extent.cy / 2;
	dc->TextOut(xPos,yPos,text);
	dc->SelectObject(oldF);
	dc->SetBkMode(hold);
	dc->SetTextColor(oldC);
}

