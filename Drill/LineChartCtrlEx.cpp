// LineChartCtrlEx.cpp : implementation file
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

#include "stdafx.h"
#include "LineChartCtrlEx.h"
#include "Drill.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineChartCtrlEx

CLineChartCtrlEx::CLineChartCtrlEx()
{
    m_uLineType = LINETYPE_MAIN;
    m_iChartHeight = 400;

    //m_fUpperLimit = 2500;   /* 最大扭矩 */
    //m_fLowerLimit = 1800;   /* 最小扭矩 */
    m_fOptTorq = 2000;   /* 最佳扭矩 */
    //m_fSpeedDown  = 300;    /* 减速扭矩 */
    m_fShow = 100;    /* 显示扭矩 */
    //m_fBear       = 1500;   /* 肩负扭矩 */
    m_fControlCir = 2;      /* 控制周数 */
    m_fUpperCir = 4.5;    /* 上限周数 */
    m_fLowerCir = 0.1;    /* 下限周数 */
    m_fMaxCir = 5;      /* 最大周数 */
    m_fWidthCir = 5;
    m_fMaxLimit = 5000;   /* 最大上限 */
    //m_bBear       = FALSE;
    //m_nTorqNo     = 1;
    m_nSelPntNum = 0;
    m_nStartPoint = 0;
    memset(m_nSelPos, 0, sizeof(UINT) * MAX_SHOWSELPOINTNUM);
    m_rcInterPt.SetRectEmpty();

    m_bpCross.LoadBitmap(IDB_CROSS);
    m_bpWCross.LoadBitmap(IDB_CROSSWHITE);
    m_bpCross.GetBitmap(&m_bmpInfo);

    CLineChartCtrlEx::RegisterWndClass(AfxGetInstanceHandle());
}

CLineChartCtrlEx::~CLineChartCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CLineChartCtrlEx, CWnd)
    //{{AFX_MSG_MAP(CLineChartCtrlEx)
    ON_WM_PAINT()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_MOUSEWHEEL()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CLineChartCtrlEx::RegisterWndClass(HINSTANCE hInstance)
{
    WNDCLASS wc;

    wc.lpszClassName = "LINE_TORQUE_CTRL"; // matches class name in client
    wc.hInstance = hInstance;
    wc.lpfnWndProc = ::DefWindowProc;
    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = 0;
    wc.lpszMenuName = NULL;
    wc.hbrBackground = (HBRUSH) ::GetStockObject(LTGRAY_BRUSH);
    wc.style = CS_GLOBALCLASS; // To be modified
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;

    return (::RegisterClass(&wc) != 0);
}

void CLineChartCtrlEx::SetStartPoint(UINT nBegin)
{
    COMP_BGE(nBegin, MAXLINEITEM);

    CLineChartCtrl::SetStartPoint(nBegin);
    m_tItem.m_nPos = nBegin;

    m_nStartPoint = nBegin;

    return;
}

UINT CLineChartCtrlEx::UpdatePos(double fPos)
{
    CHECK_VALUE_UP(fPos, m_tItem.m_fUpper);
    CHECK_VALUE_LOW(fPos, m_tItem.m_fLower);

    COMP_BLE_R(m_tItem.m_nPos, 1, m_tItem.m_nPos);
    EraseLastOne();
    m_tItem.m_fData[m_tItem.m_nPos - 1] = fPos;
    DrawSpike(TRUE);

    return m_tItem.m_nPos;
}

void CLineChartCtrlEx::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    GetMemDC();

    dc.BitBlt(0, 0, m_rcClient.Width(), m_rcClient.Height(), &m_MemDC, 0, 0, SRCCOPY);
}

void CLineChartCtrlEx::EraseLastOne()
{
    CPoint  ptOld;
    CPoint  ptNew;
    double  fRange = 0;
    CPen    pnLine(PS_SOLID, 1, LC_BKCOLOR);
    CPen* pOldPen = m_MemDC.SelectObject(&pnLine);

    COMP_BL(m_iStaticX, 2);
    COMP_BL(m_tItem.m_nPos, 2);

    ptOld.x = int((m_iStaticX - 1) * m_fOffset);
    ptNew.x = int(m_iStaticX * m_fOffset);

    fRange = m_tItem.m_fUpper - m_tItem.m_fLower;
    ptOld.y = (int)((((fRange - m_tItem.m_fData[m_tItem.m_nPos - 2] + m_tItem.m_fLower)) / fRange) * m_iChartHeight);
    ptNew.y = (int)((((fRange - m_tItem.m_fData[m_tItem.m_nPos - 1] + m_tItem.m_fLower)) / fRange) * m_iChartHeight);

    m_MemDC.MoveTo(ptOld);
    m_MemDC.LineTo(ptNew);
    m_MemDC.SelectObject(pOldPen);
}

void CLineChartCtrlEx::DrawSpike(BOOL bStop)
{
    CPoint  ptOld;
    CPoint  ptNew;
    double  fRange = 0;
    CPen    pnLine(PS_SOLID, 1, m_tItem.m_clrLine);
    CPen* pOldPen = m_MemDC.SelectObject(&pnLine);

    // Minus one to make sure to draw inside the area
    if (bStop && m_iStaticX > 0)
        m_iStaticX--;
    ptOld.x = int(m_iStaticX * m_fOffset);
    m_iStaticX++;
    ptNew.x = int(m_iStaticX * m_fOffset);

    COMP_BGE(m_iStaticX, MAXLINEITEM);

    fRange = m_tItem.m_fUpper - m_tItem.m_fLower;
    ptOld.y = m_iChartHeight;
    if (m_tItem.m_nPos >= 2)
    {
        ptOld.y = (int)((((fRange - m_tItem.m_fData[m_tItem.m_nPos - 2] + m_tItem.m_fLower)) / fRange) * m_iChartHeight);
    }
    ptNew.y = (int)((((fRange - m_tItem.m_fData[m_tItem.m_nPos - 1] + m_tItem.m_fLower)) / fRange) * m_iChartHeight);

    //如果曲线没有超出屏幕图片框右边界线，直接在屏幕图片框区域内绘制曲线，不用平移
/*
    if(ptOld.y == m_iChartHeight)
        m_MemDC.MoveTo(ptNew);
    else
*/
    m_MemDC.MoveTo(ptOld);
    m_MemDC.LineTo(ptNew);

    if (m_bLastPoint)
    {
        m_MemDC.MoveTo(ptNew);
        ptNew.y = m_iChartHeight;
        m_MemDC.LineTo(ptNew);
    }
    m_MemDC.SelectObject(pOldPen);
}

BOOL CLineChartCtrlEx::Add(COLORREF clrLine, double fUpper, double fLower, BYTE uType)
{
    CLineChartCtrl::Add(clrLine, fUpper, fLower);
    m_uLineType = uType;

    return TRUE;
}


void CLineChartCtrlEx::EraseLine()
{
    CPoint  ptOld;
    CPoint  ptNew;
    double  fRange = 0;

    if (m_tItem.m_nPos == 0)
        return;

    CPen    pnLine(PS_SOLID, 1, CTRLBKCOLOR);
    CPen* pOldPen = m_MemDC.SelectObject(&pnLine);

    fRange = m_tItem.m_fUpper - m_tItem.m_fLower;
    ptOld.y = m_iChartHeight;

    for (UINT i = 1; i <= m_tItem.m_nPos; i++)
    {
        ptOld.x = int(i - 1 * m_fOffset);
        ptNew.x = int(i * m_fOffset);

        if (i > 1)
        {
            ptOld.y = (int)((((fRange - m_tItem.m_fData[i - 2] + m_tItem.m_fLower)) / fRange) * m_iChartHeight);
        }
        ptNew.y = (int)((((fRange - m_tItem.m_fData[i - 1] + m_tItem.m_fLower)) / fRange) * m_iChartHeight);

        m_MemDC.MoveTo(ptOld);
        m_MemDC.LineTo(ptNew);
    }

    m_MemDC.SelectObject(pOldPen);
    Invalidate(TRUE);
    return;
}

// 卸扣时动态更新图形高度
bool CLineChartCtrlEx::UpdateMaxHeight(double fUpper)
{
    CPoint  ptOld;
    CPoint  ptNew;
    double  fRange = 0;

    m_tItem.m_fUpper = fUpper;
    if (m_tItem.m_nPos == 0)
        return true;

    DrawBkLine(true);
    Invalidate(TRUE);

    CPen  pnLine(PS_SOLID, 1, m_tItem.m_clrLine);
    CPen* pOldPen = m_MemDC.SelectObject(&pnLine);

    fRange = m_tItem.m_fUpper - m_tItem.m_fLower;
    ptOld.y = m_iChartHeight;
    ptOld.x = 0;
    for (UINT i = 1; i <= m_tItem.m_nPos; i++)
    {
        ptNew.x = int(i * m_fOffset);
        ptNew.y = (int)((((fRange - m_tItem.m_fData[i - 1] + m_tItem.m_fLower)) / fRange) * m_iChartHeight);

        m_MemDC.MoveTo(ptOld);
        m_MemDC.LineTo(ptNew);
        ptOld = ptNew;
    }

    m_MemDC.SelectObject(pOldPen);
    Invalidate(TRUE);
    return true;
}

bool CLineChartCtrlEx::ReDrawLine(double* pData, UINT nCount,  bool bBreakOut)
{
    CPoint  ptOld;
    CPoint  ptNew;
    double  fRange = 0;

    ASSERT_NULL_R(pData, false);
    ASSERT_ZERO_R(m_tItem.m_nPos, true);

    DrawBkLine(bBreakOut);
    Invalidate(TRUE);

    memset(m_tItem.m_fData, 0, m_tItem.m_nPos * sizeof(double));
    m_tItem.m_nPos = nCount;
    m_iStaticX = nCount;

    CPen  pnLine(PS_SOLID, 1, m_tItem.m_clrLine);
    CPen* pOldPen = m_MemDC.SelectObject(&pnLine);

    /* 第一个数据直接拷贝 */
    m_tItem.m_fData[0] = pData[0];
    fRange = m_tItem.m_fUpper - m_tItem.m_fLower;
    ptOld.y = m_iChartHeight;
    ptOld.x = 0;
    for (int i = 1; i < (int)(nCount - 1); i++)
    {
        m_tItem.m_fData[i] = pData[i];

        ptNew.x = int(i * m_fOffset);
        ptNew.y = (int)((((fRange - m_tItem.m_fData[i - 1] + m_tItem.m_fLower)) / fRange) * m_iChartHeight);

        m_MemDC.MoveTo(ptOld);
        m_MemDC.LineTo(ptNew);
        ptOld = ptNew;
    }
    /* 最后一个数据直接拷贝 */
    m_tItem.m_fData[nCount - 1] = pData[nCount - 1];
    ptNew.x = int(nCount * m_fOffset);
    ptNew.y = (int)((((fRange - m_tItem.m_fData[nCount - 1] + m_tItem.m_fLower)) / fRange) * m_iChartHeight);
    m_MemDC.MoveTo(ptOld);
    m_MemDC.LineTo(ptNew);

    m_MemDC.SelectObject(pOldPen);
    Invalidate(TRUE);
    return true;
}


void CLineChartCtrlEx::FinishDraw()
{
    CPoint  ptNew;
    double  fRange = 0;
    CPen    pnLine(PS_SOLID, 1, m_tItem.m_clrLine);
    CPen* pOldPen = m_MemDC.SelectObject(&pnLine);

    m_bLastPoint = TRUE;

    fRange = m_tItem.m_fUpper - m_tItem.m_fLower;
    ptNew.x = int(m_iStaticX * m_fOffset);
    ptNew.y = (int)((((fRange - m_tItem.m_fData[m_tItem.m_nPos - 1] + m_tItem.m_fLower)) / fRange) * m_iChartHeight);
    m_MemDC.MoveTo(ptNew);
    ptNew.y = m_iChartHeight;
    m_MemDC.LineTo(ptNew);
    m_MemDC.SelectObject(pOldPen);

    Invalidate(FALSE);
}

void CLineChartCtrlEx::DrawFinLine()
{
    double  fRange = 0;
    CPoint  ptOld;
    CPen    pnLine(PS_SOLID, 1, m_tItem.m_clrLine);
    CPen* pOldPen = m_MemDC.SelectObject(&pnLine);

    // Minus one to make sure to draw inside the area
    fRange = m_tItem.m_fUpper - m_tItem.m_fLower;
    ptOld.x = int(m_iStaticX * m_fOffset);
    ptOld.y = (int)((((fRange - m_tItem.m_fData[m_tItem.m_nPos - 1] + m_tItem.m_fLower)) / fRange) * m_iChartHeight);

    m_MemDC.MoveTo(ptOld);
    ptOld.y = m_iChartHeight;
    m_MemDC.LineTo(ptOld);

    m_MemDC.SelectObject(pOldPen);
    Invalidate(FALSE);
}

// just for hisgrip
void CLineChartCtrlEx::DrawMakeupLine(double fTorq, int begin, int end)
{
    int     x = 0;
    int     y = 0;
    string  strVal;
    CPen* pOldPen = NULL;
    COLORREF clrCtrl = CTRLBKCOLOR;

    CPen penSafe(PS_DOT, 1, clrCtrl);

    pOldPen = m_MemDC.SelectObject(&penSafe);

    /* ----- */
    begin = int(begin * m_fOffset);
    end = int(end * m_fOffset);
    x = MAX(0, end - 100);
    // 上扣扭矩位置
    y = int((m_iChartHeight) * (m_fMaxLimit - fTorq) / m_fMaxLimit);
    DrawHLine(begin, end, y);
    /* 显示上扣扭矩值 */
    strVal = string_format(theApp.LoadstringFromRes(IDS_STRLCMAKETORQ).c_str(), fTorq);
    ShowContent(clrCtrl, x, y - CONT_YOFFSET - 10, strVal);

    m_MemDC.SelectObject(pOldPen);
}

void CLineChartCtrlEx::DrawBreakoutLine(double fTorq, int begin, int end)
{
    int     x = 0;
    int     y = 0;
    string  strVal;
    CPen* pOldPen = NULL;
    COLORREF clrCtrl = CTRLBKCOLOR;

    CPen penSafe(PS_DOT, 1, clrCtrl);

    pOldPen = m_MemDC.SelectObject(&penSafe);

    /* ----- */
    begin = int(begin * m_fOffset);
    end = int(end * m_fOffset);
    x = begin;
    // 卸扣扭矩位置
    y = int((m_iChartHeight) * (m_fMaxLimit - fTorq) / m_fMaxLimit);
    DrawHLine(begin, end, y);
    /* 显示卸扣扭矩值 */
    strVal = string_format(theApp.LoadstringFromRes(IDS_STRLCBREAKTORQ).c_str(), fTorq);
    ShowContent(clrCtrl, x, y - CONT_YOFFSET - 10, strVal);

    m_MemDC.SelectObject(pOldPen);
}

BOOL CLineChartCtrlEx::RemoveAt()
{
    m_iStaticX = 0;
    m_nStartPoint = 0;
    m_rcInterPt.SetRectEmpty();

    return TRUE;
}

/* 打印时显示序号 */
void CLineChartCtrlEx::ShowTorqNo()
{
#if 0
    CPoint      ptBegin;
    string     strTemp;
    COMP_BTRUE(m_bBKLine);
    strTemp = string_format("%05d", m_nTorqNo);

    m_MemDC.SetTextColor(CTRLBLACKCOLOR);
    ptBegin.x = int((m_iChartWidth) * 0.5 - 20);
    ptBegin.y = 10;
    m_MemDC.TextOut(ptBegin.x, ptBegin.y, strTemp.c_str());
#endif
}

/*画背景格子线*/
void CLineChartCtrlEx::DrawGridLine(COLORREF clrBk)
{
    int     i = 0;
    CBrush  brBk(m_clrBk);
    CPen    penBk(PS_SOLID, 1, clrBk);
    CPen* pOldPen = NULL;

    m_MemDC.SetBkColor(m_clrBk);
    m_MemDC.FillRect(m_rcClient, &brBk);

    COMP_BFALSE(m_bBKLine);

    /* 背景线 */
    pOldPen = m_MemDC.SelectObject(&penBk);
    for (i = 0; i <= 10; i++)
    {
        m_MemDC.MoveTo((m_iChartWidth) / 10 * i, 0);
        m_MemDC.LineTo((m_iChartWidth) / 10 * i, m_iChartHeight);
    }
    for (i = 0; i <= 20; i++)
    {
        m_MemDC.MoveTo(0, (m_iChartHeight) / 20 * i);
        m_MemDC.LineTo(m_iChartWidth, (m_iChartHeight) / 20 * i);
    }
    m_MemDC.SelectObject(pOldPen);

    return;
}

/*根据参数画横线*/
/* ----- */
void CLineChartCtrlEx::DrawHLine(int y)
{
    CPoint  ptBegin;
    CPoint  ptEnd;

    ptBegin.x = 0;
    ptBegin.y = y;
    ptEnd.x = m_iChartWidth;
    ptEnd.y = y;
    m_MemDC.MoveTo(ptBegin);
    m_MemDC.LineTo(ptEnd);
}

/* ----- */
void  CLineChartCtrlEx::DrawHLine(int begin, int end, int height)
{
    CPoint  ptBegin;
    CPoint  ptEnd;

    ptBegin.x = begin;
    ptBegin.y = height;
    ptEnd.x = end;
    ptEnd.y = height;
    m_MemDC.MoveTo(ptBegin);
    m_MemDC.LineTo(ptEnd);
}

/* 根据参数画竖线 */
/* | */
void CLineChartCtrlEx::DrawVLine(int x)
{
    CPoint  ptBegin;
    CPoint  ptEnd;

    ptBegin.x = x;
    ptBegin.y = 0;
    ptEnd.x = x;
    ptEnd.y = m_iChartHeight;
    m_MemDC.MoveTo(ptBegin);
    m_MemDC.LineTo(ptEnd);
}

/* 打印选定点 / 放大点的动态位置的信息 */
void CLineChartCtrlEx::ShowVarPntText(COLORREF clrText, int x, int y, string strContent)
{
    int nPos = int(x / m_fOffset);

    m_MemDC.SetTextColor(clrText);

    if (nPos < CONT_XANTIRANGE)
        x += 10;
    else
        x = x - CONT_XANTIOFFSET;

    m_MemDC.TextOut(x + 2, y - CONT_YOFFSET - 5, strContent.c_str());
}

/* 打印选定点 / 扭矩到竖线右位置 */
void CLineChartCtrlEx::ShowRightPntText(COLORREF clrText, int x, int y, string strContent)
{
    int xx = 0;
    int nPos = int(x / m_fOffset);

    xx = int(m_tItem.m_nPos * m_fOffset);

    m_MemDC.SetTextColor(clrText);

    if (xx < CONT_XRIGHTRANGE)
        x = xx;
    else
        x = x - CONT_XANTIOFFSET;

    m_MemDC.TextOut(x + 2, y - CONT_YOFFSET - 5, strContent.c_str());
}

void CLineChartCtrlEx::DrawControlLine()
{
    int     x = 0;
    int     y = 0;
    CPen* pOldPen = NULL;
    COLORREF clrCtrl = LC_SAFECOLOR;

    if (LINETYPE_HISG == m_uLineType)
    {
        clrCtrl = CTRLBKCOLOR;
    }

    CPen penSafe(PS_DOT, 1, clrCtrl);

    pOldPen = m_MemDC.SelectObject(&penSafe);

    /* | */
    //x = int((m_iChartWidth)*m_fControlCir / m_fWidthCir);
    x = int(m_iChartWidth*AUTOUPDTURNRATIO);
    DrawVLine(x);
    /* ----- */
    y = int((m_iChartHeight) * (m_fMaxLimit - m_fOptTorq) / m_fMaxLimit);
    DrawHLine(y);
    /* 显示最佳扭矩值 */
    //strTemp.Format(IDS_STRLCXCONTROL, m_fOptTorq);
    ShowContent(clrCtrl, y - CONT_YOFFSET,
        string_format(theApp.LoadstringFromRes(IDS_STRLCXCONTROL).c_str(), m_fOptTorq));

    m_MemDC.SelectObject(pOldPen);
}

void CLineChartCtrlEx::DrawAlarmLine()
{
    int     x = 0;
    int     y = 0;
    CPen* pOldPen = NULL;
    //CString strTemp;
    COLORREF clrAlarm = LC_ALARMCOLOR;

    if (LINETYPE_HISG == m_uLineType)
    {
        clrAlarm = CTRLBKCOLOR;
    }

    CPen    penAlarm(PS_DOT, 1, clrAlarm);
    pOldPen = m_MemDC.SelectObject(&penAlarm);

    /* | */
    x = int((m_iChartWidth)*m_fLowerCir / m_fWidthCir);
    DrawVLine(x);
    /* | 历史数据，不画最大周数线 */
    if (LINETYPE_HISG != m_uLineType)
    {
        x = int((m_iChartWidth)*m_fUpperCir / m_fWidthCir);
        DrawVLine(x);
    }
    /* 20220922 钻杆数据无最大/最小扭矩 */
#if 0
    /* ----- */
    y = int((m_iChartHeight) * (m_fMaxLimit - m_fLowerLimit) / m_fMaxLimit);
    DrawHLine(y);
    /* 显示最小扭矩值 */
    //strTemp.Format(IDS_STRLCXLOWLIMIT, m_fLowerLimit);
    //ShowContent(clrAlarm, y-CONT_YOFFSET, strTemp);
    ShowContent(clrAlarm, y - CONT_YOFFSET,
        string_format(theApp.LoadstringFromRes(IDS_STRLCXLOWLIMIT).c_str(), m_fLowerLimit));
    /* ----- */
    y = int((m_iChartHeight) * (m_fMaxLimit - m_fUpperLimit) / m_fMaxLimit);
    DrawHLine(y);
    /* 显示最大扭矩值 */
    //strTemp.Format(IDS_STRLCXUPLIMIT, m_fUpperLimit);
    //ShowContent(clrAlarm, y-CONT_YOFFSET, strTemp);
    ShowContent(clrAlarm, y - CONT_YOFFSET,
        string_format(theApp.LoadstringFromRes(IDS_STRLCXUPLIMIT).c_str(), m_fUpperLimit));
#endif
    m_MemDC.SelectObject(pOldPen);
}

void CLineChartCtrlEx::DrawShowLine()
{
    int     x = 0;
    int     y = 0;
    CPen* pOldPen = NULL;
    //CString strTemp;

    COLORREF clrShow = LC_SHOWCOLOR;

    if (LINETYPE_HISG == m_uLineType)
    {
        clrShow = CTRLBKCOLOR;
    }

    CPen    penShow(PS_DOT, 1, clrShow);

    pOldPen = m_MemDC.SelectObject(&penShow);

    /* ----- */
    y = int((m_iChartHeight) * (m_fMaxLimit - m_fShow) / m_fMaxLimit);
    DrawHLine(y);
    /* 显示显示扭矩值 */
    //strTemp.Format(IDS_STRLCXSHOW, m_fShow);
    //ShowContent(clrShow, y-CONT_YOFFSET, strTemp);
    ShowContent(clrShow, y - CONT_YOFFSET,
        string_format(theApp.LoadstringFromRes(IDS_STRLCXSHOW).c_str(), m_fShow));

    m_MemDC.SelectObject(pOldPen);
}

void CLineChartCtrlEx::DrawBkLine(bool bBreakOut)
{
    int         iOldMode = 0;
    //CString     strTemp;
    CFont* pOldFont = NULL;
    CClientDC   dc(this);

    GetMemDC();

    /* 画背景格子线 */
    DrawGridLine();
    iOldMode = m_MemDC.SetBkMode(TRANSPARENT);
    if (LINETYPE_HISG == m_uLineType)
        pOldFont = m_MemDC.SelectObject(&theApp.m_tPntTextFont);
    else
        pOldFont = m_MemDC.SelectObject(&theApp.m_tLineTextFont);

    /* 扭矩序号 */
    //ShowTorqNo();

    // 卸扣时只保留最大扭矩，不画其他控制线
    if (!bBreakOut)
    {
        /* 控制线 */
        DrawControlLine();

        /* 告警线 */
        DrawAlarmLine();
    }

    /*显示扭矩*/
    DrawShowLine();

    m_MemDC.SetBkMode(iOldMode);
    m_MemDC.SelectObject(&pOldFont);
}

void CLineChartCtrlEx::DrawZoomBkLine()
{
    int         iOldMode = 0;
    //CString     strTemp;

    CFont* pOldFont = NULL;

    GetMemDC();

    /* 画背景格子线 */
    DrawGridLine(LC_ZOOMCOLOR);
    pOldFont = m_MemDC.SelectObject(&theApp.m_tLineTextFont);

    m_MemDC.SetBkMode(iOldMode);
    m_MemDC.SelectObject(&pOldFont);
}

/* 左键单击显示当前扭矩和周数 */
void CLineChartCtrlEx::DrawSelInfo(UINT nBeginPos, UINT nTotal)
{
    double      fRange = 0;
    int         x = 0, y = 0;
    double      fSelTorq = 0;
    double      fSelCir = 0;
    UINT        i = 0;
    string      strTemp;
    CDC         dcMemory;
    CPen        pnLine(PS_DOT, 1, IP_SELCOLOR);
    CPen* pOldPen = NULL;

    ASSERT_ZERO(m_nSelPntNum);
    for (i = 0; i < m_nSelPntNum && i < MAX_SHOWSELPOINTNUM; i++)
    {
        if (m_nSelPos[i] >= m_tItem.m_nPos)
            continue;

        fSelTorq = (UINT)m_tItem.m_fData[m_nSelPos[i]];
        fSelCir = (nTotal - nBeginPos - m_nSelPos[i] + m_nStartPoint) * m_fWidthCir / MAXLINEITEM;

        fRange = m_tItem.m_fUpper - m_tItem.m_fLower;
        x = int(m_nSelPos[i] * m_fOffset);
        y = (int)(((fRange - fSelTorq) / fRange) * m_iChartHeight);


        /*画选择当前位置的扭矩的线*/
        pOldPen = m_MemDC.SelectObject(&pnLine);
        /* | */
        DrawVLine(x);

        /* ----- */
        DrawHLine(y);

        /* 显示当前扭矩和周数信息 */
        strTemp = string_format("(%.3f \xA1\xF1 %.f)", fSelCir, fSelTorq);
        ShowRightPntText(IP_SELCOLOR, x, y, strTemp.c_str());
    }
    m_MemDC.SelectObject(pOldPen);
}

void CLineChartCtrlEx::OnRButtonDown(UINT nFlags, CPoint point)
{
    UINT    nCurPos = 0;
    HWND hParent = ::GetParent(m_hWnd);

    /* 只是在hisgraph里面才触发移动拐点事件 */
    if (m_uLineType == LINETYPE_ZOOM)
    {
        ::PostMessage(hParent, WM_RBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
    }

    CWnd::OnRButtonDown(nFlags, point);
}

BOOL CLineChartCtrlEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
    UINT    nCurPos = 0;
    HWND hParent = ::GetParent(m_hWnd);

    /* 只是在hisgraph里面才触发放大事件 */
    COMP_BFALSE_R(m_uLineType == LINETYPE_HISG, FALSE);

    ScreenToClient(&point);
    nCurPos = UINT(point.x / m_fOffset);

    ::SendMessage(hParent, WM_INTERPT_ZOOMIN, 0, nCurPos);

    return CWnd::OnMouseWheel(nFlags, zDelta, point);
}

void CLineChartCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
    UINT    nCurPos = 0;
    HWND hParent = ::GetParent(m_hWnd);

    /* 只是在hisgraph里面才触发左键单击显示当前扭矩和周数 */
    if (m_uLineType == LINETYPE_HISG || m_uLineType == LINETYPE_ZOOM)
    {
        /* 达到指定选定数目的点数，全部清除 */
        /* 去掉当前扭矩和周数的显示 */
        if (m_nSelPntNum >= MAX_SHOWSELPOINTNUM)
        {
            m_nSelPntNum = 0;
            memset(m_nSelPos, 0, sizeof(UINT) * MAX_SHOWSELPOINTNUM);
            ::SendMessage(hParent, WM_UPDATE_SELPOS, 0, 0);
            return;
        }

        nCurPos = UINT(point.x / m_fOffset);
        if (m_uLineType == LINETYPE_HISG)
        {
            COMP_BGE(nCurPos, m_tItem.m_nPos);
            COMP_BL(nCurPos, m_nStartPoint);
        }
        if (m_uLineType == LINETYPE_ZOOM)
        {
            COMP_BGE(nCurPos, m_tItem.m_nPos);
        }
        m_nSelPos[m_nSelPntNum] = nCurPos;

        m_nSelPntNum++;
        ::SendMessage(hParent, WM_UPDATE_SELPOS, m_nSelPntNum, 0);
    }

    CWnd::OnLButtonDown(nFlags, point);
}

void CLineChartCtrlEx::ClearSelPnt()
{
    m_nSelPntNum = 0;
}

UINT CLineChartCtrlEx::GetCurPoints()
{
    return m_tItem.m_nPos;
}

/*
    fDeltaCir: > 0 多屏的前面屏的数据的DelCir计算，等于后面屏的Cir，加上满屏 - CurCir
               < 0 单屏或者最后一屏，赋值为负ControlCir，DelCir = ControlCir-CurCir
*/
void CLineChartCtrlEx::DrawZoomInfo(WORD wZoomPos, double fMinCir, double fSrcMaxCir, double fDeltaCir, double fSetZoomTorq)
{
    double      fRange = 0;
    double      fSelTorq = 0;
    double      fSelCir = 0;
    int         x = 0, y = 0;
    UINT        i = 0;
    string      strTemp;
    CBitmap* pOldBitmap;
    CDC         dcMemory;
    CPen        pnLine(PS_DOT, 1, ZOOM_SELCOLOR);
    CPen* pOldPen = NULL;
    UINT        nSelPos = 0;

    CPoint  ptBegin;
    CPoint  ptEnd;

    if (fMinCir < 0)
        fMinCir = 0;

    if (fSetZoomTorq == -1)
        fSetZoomTorq = (UINT)m_tItem.m_fData[wZoomPos];

    fRange = m_tItem.m_fUpper - m_tItem.m_fLower;
    pOldPen = m_MemDC.SelectObject(&pnLine);

    /* 画选定的点 */
    for (i = 0; i < m_nSelPntNum && i < MAX_SHOWSELPOINTNUM; i++)
    {
        if (m_nSelPos[i] >= MAXLINEITEM)
            continue;

        fSelTorq = (UINT)(m_tItem.m_fData[m_nSelPos[i]]);
        fSelCir = fMinCir + m_nSelPos[i] * m_fWidthCir / MAXLINEITEM; // 当前点在放大图像中的周数

        fSelCir = fDeltaCir - fSelCir;

        x = int(m_nSelPos[i] * m_fOffset);
        y = (int)(((fRange - fSelTorq + m_tItem.m_fLower) / fRange) * m_iChartHeight);

        /* | */
        DrawVLine(x);

        /* ----- */
        DrawHLine(y);

        strTemp = string_format("(%.4f \xA1\xF1 %.f)", fSelCir, fSelTorq);
        ShowVarPntText(ZOOM_SELCOLOR, x, y, strTemp);
    }

    /*画放大点扭矩的线*/
    /* 画放大点坐标 */
    x = int(wZoomPos * m_fOffset);
    y = (int)(((fRange - fSetZoomTorq + m_tItem.m_fLower) / fRange) * m_iChartHeight);

    /*放大点扭矩*/
    /* | */
    DrawVLine(x);

    /* ----- */
    DrawHLine(y);

    /* 放大点扭矩 */
    fSelCir = fMinCir + wZoomPos * m_fWidthCir / MAXLINEITEM; // 当前点在放大图像中的周数
    fSelCir = fDeltaCir - fSelCir;
    strTemp = string_format("(%.4f \xA1\xF1 %.f)", fSelCir, fSetZoomTorq);
    ShowVarPntText(ZOOM_SELCOLOR, x, y, strTemp);

    m_MemDC.SelectObject(pOldPen);

    dcMemory.CreateCompatibleDC(&m_MemDC);

    // Select the bitmap into the in-memory DC
    pOldBitmap = dcMemory.SelectObject(&m_bpWCross);

    m_MemDC.BitBlt(x - 5, y - 5, m_bmpInfo.bmWidth, m_bmpInfo.bmHeight, &dcMemory, 0, 0, SRCCOPY);
    dcMemory.SelectObject(pOldBitmap);
    dcMemory.DeleteDC();
    return;
}


/* 打印说明文字 */
void  CLineChartCtrlEx::ShowContent(COLORREF clrText, int y, string strContent, UINT nLeftOffset)
{
    CRect   rectCont;
    char    strPrint[MAX_LOADSTRING];

    m_MemDC.SetTextColor(clrText);

    rectCont.left = 0;
    rectCont.right = m_rcClient.right - nLeftOffset;
    rectCont.top = y;
    rectCont.bottom = m_rcClient.bottom;

    snprintf(strPrint, MAX_LOADSTRING, "%s          ", strContent.c_str());
    //strPrint = strContent + _T("          ");
    m_MemDC.DrawText(strPrint, -1, &rectCont, DT_SINGLELINE | DT_RIGHT | DT_TOP);
}

void CLineChartCtrlEx::ShowContent(COLORREF clrText, int x, int y, string strContent)
{
    m_MemDC.SetTextColor(clrText);

    if (x < CONT_XANTIRANGE)
        x += 10;
    else
        x = x - CONT_XANTIOFFSET;

    m_MemDC.TextOut(x, y, strContent.c_str());
}
