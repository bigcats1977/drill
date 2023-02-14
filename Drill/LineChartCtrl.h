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



#define     CONT_XOFFSET        5       /* ��ʾ���֣���������ߵ�λ�� */
#define     CONT_YOFFSET        12      /* ��ʾ���֣�����ڵ��ߵ�λ�� */
#define     CONT_XSCALE         0.75    /* ���ֿ�ʼλ��(�����������) */
#define     CONT_XANTIRANGE     425     /* ���곬����λ��ʱ����Ҫ��������ʾ���� */
#define     CONT_XANTIOFFSET    80      /* �������ұ�ʱ����������� */
#define     CONT_XRIGHTRANGE    380     /* �յ�/ѡ�е���Ϣ���̫���򷴷�����ʾ���� */

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
    double          m_fMaxCir;     /* ������� */
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
    BOOL    Add(COLORREF clrLine, double fUpper, double fLower, double fMaxCir);
    void    SetBkColor(COLORREF clrBk);
    void    FinishDraw();
    void    SetStartPoint(UINT nBegin);
    void    DrawFinLine();
    UINT    GetCurPoint();
    bool    UpdateMaxWidth(double fMaxCir);

    // Generated message map functions
protected:
    //{{AFX_MSG(CLineChartCtrl)
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void    DrawGridLine();

protected:
    void    ShowContent(COLORREF clrText, int y, string trContent, UINT nLeftOffset = 0);
    void    GetMemDC();

    CDC      m_MemDC;
    CBitmap  m_Bitmap;
    COLORREF m_clrBk;
    CRect    m_rcClient;
    int      m_iChartWidth;
    int      m_iChartHeight;
    BOOL     m_bLastPoint;
};

/////////////////////////////////////////////////////////////////////////////
#endif
