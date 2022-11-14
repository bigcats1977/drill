// LineChartCtrlEx.h : header file
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

#ifndef __LineChartCtrlEx_H__
#define __LineChartCtrlEx_H__

#include "LineChartCtrl.h"

#define  MAX_SHOWSELPOINTNUM    3

#define  LINETYPE_MAIN          0
#define  LINETYPE_HISG          1
#define  LINETYPE_ZOOM          2

/*typedef CTypedPtrArray <CObArray, CLineChartItem*> CItemArray;*/

/////////////////////////////////////////////////////////////////////////////
// CLineChartCtrlEx window

class CLineChartCtrlEx : public CLineChartCtrl
{
// Construction
public:
    static BOOL RegisterWndClass(HINSTANCE hInstance);

    CLineChartCtrlEx();

// Attributes
public:
    //LINECHARITEM    m_tItem;
    bool            m_bCheckIP;
    BYTE            m_uLineType;

    // Operations
public:
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CLineChartCtrlEx)
    public:
    virtual ~CLineChartCtrlEx();
    //}}AFX_VIRTUAL
// Implementation
public:
    UINT    UpdatePos(double fPos);
    //void    InvalidateCtrl();
    void    DrawSpike(BOOL bStop = FALSE);
    BOOL    RemoveAt();
    BOOL    Add(COLORREF clrLine, double fUpper, double fLower, BYTE uType = LINETYPE_MAIN);
    void    DrawBkLine();
    void    FinishDraw();
    void    DrawInflection(int iIPPos, double fDelIPCir, double fSetIPTorq = -1);//, COLORREF clrIP = IP_INFCOLOR);
    void    DrawZoomInfo(WORD wZoomPos, double fMinCir, double fSrcMaxCir, double fDeltaCir, double fSetZoomTorq = -1);
    void    DrawSelInfo(UINT nBeginPos, UINT nTotal);
    void    DrawZoomBkLine();
    void    ClearSelPnt();
    UINT    GetCurPoints();
    void    SetStartPoint(UINT nBegin);
    void    DrawFinLine();

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
    double       m_fWidthCir;   /* 画图的真实宽度的周数，一般等于m_fMaxCir，因为左边界为0
                                   对于放大的图, 左边m_fMinCir非0, 该参数小于m_fMaxCir */

    double       m_fUpperTai;   /* 最大台矩 */
    //double       m_fCtrlTai;  /* 最佳台矩 即为减速扭矩，直接使用减速扭矩 */
    double       m_fLowerTai;   /* 最小台矩 */

    BOOL         m_bBear;

    // Generated message map functions
protected:
    //{{AFX_MSG(CLineChartCtrlEx)
    afx_msg void OnPaint();
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
private:
    //void    GetMemDC();
    void    DrawGridLine(COLORREF clrBk = LC_BKCOLOR);
    void    DrawHLine(int y);
    void    DrawVLine(int x);
    void    DrawControlLine();
    void    DrawAlarmLine();
    void    DrawTaiLine();
    void    DrawShowLine();
    void    DrawBearLine();
    void    ShowTorqNo();
    void    EraseLastOne();
    void    ShowVarPntText(COLORREF clrText, int x, int y, CString strContent);
    void    ShowRightPntText(COLORREF clrText, int x, int y, CString strContent);

    CBitmap     m_bpCross;
    CBitmap     m_bpWCross;
    BITMAP      m_bmpInfo;
    CRect       m_rcInterPt;
    BOOL        m_bIPSeled;
    UINT        m_nSelPntNum;
    UINT        m_nSelPos[MAX_SHOWSELPOINTNUM];
        
    UINT        m_nStartPoint;
};

/////////////////////////////////////////////////////////////////////////////
#endif
