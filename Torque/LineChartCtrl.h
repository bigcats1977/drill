// LineChartCtrl.h : header file
//
// Written by Yuheng Zhao (yuheng@ministars.com)
// http://www.ministars.com
// The original idea and part of the code from Ken C. Len's CHistogramCtrl
// http://www.codeguru.com/controls/histogram_control.shtml
//
// Copyright (c) 1998.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name is included. If
// the source code in  this file is used in any commercial application
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage whatsoever.
// It's free - so you get what you pay for.
//

#ifndef __LINECHARTCTRL_H__
#define __LINECHARTCTRL_H__

#include <afxtempl.h>


#define MAXLINEITEM      500
typedef struct tagLINECHARITEM
{
    COLORREF    m_clrLine;
    double      m_fLower;       // lower bounds
    double      m_fUpper;       // upper bounds
    double      m_fData[MAXLINEITEM];
    UINT        m_nPos;         // current position within bounds
}LINECHARITEM;
/////////////////////////////////////////////////////////////////////////////
// CLineChartCtrl window

#define CTRLBKCOLOR     RGB(0,0,0)
#define CTRLWHITECOLOR  RGB(255,255,255)
#define CTRLBLACKCOLOR  RGB(0,0,0)
#define LC_BKCOLOR      RGB(50,50,50)
#define LC_ZOOMCOLOR    RGB(218,218,218)
#define LC_SAFECOLOR    RGB(0,255,0)
#define LC_ALARMCOLOR   RGB(255,0,0)
#define LC_SPEEDCOLOR   RGB(0,0,255)
#define LC_SHOWCOLOR    RGB(0,255,255)
#define LC_BEARCOLOR    RGB(255,0,0)
#define LC_TAICOLOR     RGB(0,0,255)
#define IP_INFCOLOR     RGB(0,255,0)
#define IP_SELCOLOR     RGB(255,0,255)
#define ZOOM_SELCOLOR   RGB(255,0,255)
//#define IP_INFCOLOR     RGB(160,32,240)
#define LC_HISSAFECLR   RGB(136,0, 21)
#define LC_HISSHOWCLR   RGB(164,82,164)

class CLineChartCtrl : public CWnd
{
// Construction
public:
    static BOOL RegisterWndClass(HINSTANCE hInstance);

    CLineChartCtrl();

// Attributes
public:
    LINECHARITEM    m_tItem;
    int             m_iStaticX;
    double          m_fOffset;
    BOOL            m_bBKLine;
    // Operations
public:
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CLineChartCtrl)
    public:
    virtual ~CLineChartCtrl();
    //}}AFX_VIRTUAL

// Implementation
public:
    UINT    SetPos(double fPos, BOOL bLast = FALSE);
    UINT    UpdatePos(double fPos);
    void    InvalidateCtrl();
    void    DrawSpike();
    void    DrawBkLine();
    BOOL    RemoveAt();
    void    Go();
    void    Erase();
    BOOL    Add(COLORREF clrLine, double fUpper, double fLower );
    void    SetBkColor(COLORREF clrBk);
    void    FinishDraw();
    void    SetStartPoint(UINT nBegin);
    void    DrawFinLine();

    // Generated message map functions
protected:
    //{{AFX_MSG(CLineChartCtrl)
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void    DrawGridLine();
    void    GetMemDC();

    int      m_iRpmWidth;
    int      m_iRpmHeight;
    CRect    m_rcClient;
    CDC      m_MemDC;
    CBitmap  m_Bitmap;
    COLORREF m_clrBk;
    BOOL     m_bLastPoint;
};

/////////////////////////////////////////////////////////////////////////////
#endif
