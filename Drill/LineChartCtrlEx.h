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
    BOOL    Add(COLORREF clrLine, double fUpper, double fLower, BYTE uType);
    bool    UpdateMaxHeight(double fUpper);
    void    DrawBkLine(bool bBreakOut);
    void    FinishDraw();
    void    DrawZoomInfo(WORD wZoomPos, double fMinCir, double fSrcMaxCir, double fDeltaCir, double fSetZoomTorq = -1);
    void    DrawSelInfo(UINT nBeginPos, UINT nTotal);
    void    DrawZoomBkLine();
    void    ClearSelPnt();
    UINT    GetCurPoints();
    void    SetStartPoint(UINT nBegin);
    void    DrawFinLine();
    void    EraseLine();
    void    DrawMakeupLine(double fTorq, int begin, int end);
    void    DrawBreakoutLine(double fTorq, int begin, int end);
    bool    ReDrawLine(double* pData, UINT nCount, bool bBreakOut);

public:
    /* Ť����ʾ��ز��� */
    //double       m_fUpperLimit; /* ���Ť�� */
    //double       m_fLowerLimit; /* ��СŤ�� */
    double       m_fOptTorq;    /* ���Ť�� */
    //double       m_fSpeedDown;  /* ����Ť�� */
    double       m_fShow;       /* ��ʾŤ�� */
    double       m_fControlCir; /* �������� */
    double       m_fUpperCir;   /* �������� */
    double       m_fLowerCir;   /* �������� */
    double       m_fMaxCir;     /* ������� */
    double       m_fMaxLimit;   /* ������� */
    //double       m_fBear;       /* �縺Ť�� */
    double       m_fWidthCir;   /* ��ͼ����ʵ��ȵ�������һ�����m_fMaxCir����Ϊ��߽�Ϊ0
                                   ���ڷŴ��ͼ, ���m_fMinCir��0, �ò���С��m_fMaxCir */

                                   //BOOL         m_bBear;

                                   // Generated message map functions
protected:
    //{{AFX_MSG(CLineChartCtrlEx)
    afx_msg void OnPaint();
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    //void    GetMemDC();
    void    DrawGridLine(COLORREF clrBk = LC_BKCOLOR);
    void    DrawHLine(int y);
    void    DrawVLine(int x);
    void    DrawHLine(int begin, int end, int height);
    void    DrawControlLine();
    void    DrawAlarmLine();
    void    DrawShowLine();
    void    ShowTorqNo();
    void    EraseLastOne();
    void    ShowVarPntText(COLORREF clrText, int x, int y, string strContent);
    void    ShowRightPntText(COLORREF clrText, int x, int y, string strContent);
    void    ShowContent(COLORREF clrText, int y, string trContent, UINT nLeftOffset = 0);
    void    ShowContent(COLORREF clrText, int x, int y, string trContent);

    CBitmap     m_bpCross;
    CBitmap     m_bpWCross;
    BITMAP      m_bmpInfo;
    CRect       m_rcInterPt;
    UINT        m_nSelPntNum;
    UINT        m_nSelPos[MAX_SHOWSELPOINTNUM];

    UINT        m_nStartPoint;
};

/////////////////////////////////////////////////////////////////////////////
#endif
