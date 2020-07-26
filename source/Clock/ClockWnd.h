#if !defined(AFX_CLOCKWND_H__433044EF_5C42_11D1_800A_0000C0491400__INCLUDED_)
#define AFX_CLOCKWND_H__433044EF_5C42_11D1_800A_0000C0491400__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ClockWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClockWnd frame

class CClockWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CClockWnd)
public:
	CClockWnd();
	virtual ~CClockWnd();
	void ReadConfig();
	void WriteConfig();

// Attributes
private:
	//THEME: display state varables
	bool m_rotate;
	bool m_roman;
	bool m_quadOnly;
	bool m_noTitle;
	bool m_sysColor;
	bool m_smooth;
	HICON m_hIcon;

	//THEME: positioning variables
	int m_x;
	int m_y;
	int m_cx;
	int m_cy;
	//THEME: color variables
	COLORREF m_bkColor;
	COLORREF m_handsColor;
	COLORREF m_bdrColor;
	COLORREF m_tickColor;
	COLORREF m_tick5Color;
	COLORREF m_fontColor;
	COLORREF m_scColor;
	COLORREF m_numColor;

	//computed positioning variables
	int m_midX;
	int m_midY;
	int m_radiusHr;
	int m_radiusSc;
	int m_radiusMn;

	//window pos variables
	int m_winCx;
	int m_winCy;
	int m_winX;
	int m_winY;

	//time variables
	int m_tick;
	int m_hr;
	int m_mn;
	int m_sc;
	int m_pHr;
	int m_pMn;
	int m_pSc;

	//brush variables
	CBrush m_bkBrush;
	CBrush m_handsBrush;
	CBrush m_bdrBrush;
	CBrush m_tickBrush;
	CBrush m_tick5Brush;
	CBrush m_fontBrush;
	CBrush m_scBrush;
	CBrush m_numBrush;

	//pen variables
	CPen m_bkPen;
	CPen m_handsPen;
	CPen m_bdrPen;
	CPen m_tickPen;
	CPen m_tick5Pen;
	CPen m_fontPen;
	CPen m_numPen;
	CPen m_scPen;
	CPen m_scWtPen;
	
	//other
	int m_timer;
	bool m_mouseDown;
	bool m_resizing;
	CPoint m_startDrag;
	CPoint m_dragPoint;
	CRect m_dragRect;
	CRgn m_wndRgn;
	
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClockWnd)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CClockWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAbout();
	afx_msg void OnConfigure();
	afx_msg void OnExit();
	afx_msg long OnMyicon(UINT wParam, LONG lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnQueryEndSession();
	afx_msg void OnEndSession(BOOL);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	inline void CreateScHandRgn(CRgn &rgn, bool current);
	inline void CreateHrHandRgn(CRgn &rgn, bool current);
	inline void CreateMnHandRgn(CRgn &rgn, bool current);
    inline void Draw(CDC* dc);
    inline void DrawTicks(CDC* dc);
    inline void DrawNumbers(CDC* dc);
    inline void DrawFace(CDC* dc);
	inline void DrawHands(CDC* dc);
	inline void DrawScHand(CDC* dc);
	inline CString GetText(int number);
	inline void LoadTime();
	inline void RotateAndOffset(POINT& pt,double theta);
	inline void RunPopupMenu(CPoint& point);
	inline void CClockWnd::WriteLogo(CDC* dc);
    inline bool TestMoveRegion(CPoint point);
    inline bool TestResizeRegion(CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLOCKWND_H__433044EF_5C42_11D1_800A_0000C0491400__INCLUDED_)
