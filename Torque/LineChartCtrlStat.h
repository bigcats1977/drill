// LineChartCtrlStat.h : header file
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

#ifndef __LineChartCtrlStat_H__
#define __LineChartCtrlStat_H__

#include "LineChartCtrl.h"
#include "Torque.h"


/////////////////////////////////////////////////////////////////////////////
// CLineChartCtrlStat window

class CLineChartCtrlStat : public CWnd
{
// Construction
public:
    static BOOL RegisterWndClass(HINSTANCE hInstance);

    CLineChartCtrlStat();

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
    //{{AFX_VIRTUAL(CLineChartCtrlStat)
    public:
    virtual ~CLineChartCtrlStat();
    //}}AFX_VIRTUAL
// Implementation
public:
    //UINT    SetPos(double fPos, BOOL bLast = FALSE);
    //UINT    UpdatePos(double fPos);
    void    InvalidateCtrl();
    //void    DrawSpike();
    BOOL    RemoveAt();
    //void    Go();
    BOOL    Add(COLORREF clrLine, double fUpper, double fLower);
    void    DrawBkLine();
    void    SetBkColor(COLORREF clrBk);
    //void    FinishDraw();
    void    DrawMultiData(TORQUEDATA *ptTorqData, LISTINT liNO);
    void    DrawMultiScatter(TORQUEDATA *ptTorqData);

public:
    /* 扭矩显示相关参数 */
    double       m_fUpperLimit; /* 最大扭矩 */
    double       m_fLowerLimit; /* 最小扭矩 */
    double       m_fOptTorq;    /* 最佳扭矩 */
    double       m_fSpeedDown;  /* 减速扭矩 */
    double       m_fShow;       /* 显示扭矩 */
    double       m_fControlCir; /* 控制周数 */
    double       m_fUpperCir;   /* 上限周数 */
    double       m_fLowerCir;   /* 下限周数 */
    double       m_fMaxCir;     /* 最大周数 */
    double       m_fMaxLimit;   /* 最大上限 */
    double       m_fBear;       /* 肩负扭矩 */

    double       m_fUpperTai;   /* 最大台矩 */
    //double       m_fCtrlTai;  /* 最佳台矩 即为减速扭矩，直接使用减速扭矩 */
    double       m_fLowerTai;   /* 最小台矩 */

    BOOL         m_bBear;
    BOOL         m_bAlign;      /* 按扭矩对齐 */
    BOOL         m_bScatter;

    // Generated message map functions
protected:
    //{{AFX_MSG(CLineChartCtrlStat)
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
private:
    void    DrawGridLine();
    void    DrawHLine(int y);
    void    DrawVLine(int x);
    void    ShowContent(COLORREF clrText, int y, CString strContent, UINT nLeftOffset=0);
    void    DrawControlLine();
    void    DrawAlarmLine();
    void    DrawTaiLine();
    void    DrawShowLine();
    void    DrawBearLine();
    void    GetMemDC();
    void    DrawScatter(int x, int y, BOOL bAccept,BOOL bIP);
    
    CDC         m_MemDC;
    CBitmap     m_Bitmap;
    COLORREF    m_clrBk;
    
    int         m_iChartWidth;
    int         m_iChartHeight;
    CRect       m_rcClient;

    BOOL        m_bLastPoint;
};

/////////////////////////////////////////////////////////////////////////////
#endif
