// LineChartCtrl.cpp : implementation file
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
#include "LineChartCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineChartCtrl

CLineChartCtrl::CLineChartCtrl()
{
    m_iChartWidth = MAXLINEITEM;
    m_iChartHeight = 100;
    m_fOffset = 1;
    m_iStaticX = 0;
    m_clrBk = CTRLBKCOLOR;
    m_bLastPoint = FALSE;
    m_bBKLine = TRUE;
    CLineChartCtrl::RegisterWndClass(AfxGetInstanceHandle());
}

CLineChartCtrl::~CLineChartCtrl()
{
}


BEGIN_MESSAGE_MAP(CLineChartCtrl, CWnd)
    //{{AFX_MSG_MAP(CLineChartCtrl)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CLineChartCtrl::RegisterWndClass(HINSTANCE hInstance)
{
    WNDCLASS wc;

    wc.lpszClassName = "LINE_RPM_CTRL"; // matches class name in client
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

void CLineChartCtrl::GetMemDC()
{
    CClientDC dc(this);

    COMP_BNE(m_MemDC.GetSafeHdc(), NULL);

    m_MemDC.CreateCompatibleDC(&dc);
    m_Bitmap.CreateCompatibleBitmap(&dc, m_rcClient.Width(), m_rcClient.Height());
    m_MemDC.SelectObject(m_Bitmap);
}

void CLineChartCtrl::InvalidateCtrl()
{
    if (m_MemDC.GetSafeHdc() == NULL)
    {
        GetMemDC();

        // draw scale
        DrawBkLine();
    }

    InvalidateRect(m_rcClient, FALSE);
}

UINT CLineChartCtrl::SetPos(double fPos, BOOL bLast)
{
    CHECK_VALUE_UP(fPos, m_tItem.m_fUpper);
    CHECK_VALUE_LOW(fPos, m_tItem.m_fLower);

    /* 超过500个数据，说明图像过了一屏，清屏重新开始 */
    if (m_tItem.m_nPos >= MAXLINEITEM)
    {
        m_tItem.m_nPos = 0;

        return 0;
    }

    m_bLastPoint = bLast;
    m_tItem.m_fData[m_tItem.m_nPos++] = fPos;
    return m_tItem.m_nPos;
}

void CLineChartCtrl::SetStartPoint(UINT nBegin)
{
    COMP_BGE(nBegin, MAXLINEITEM);

    m_iStaticX = nBegin;//int(nBegin / m_fOffset + 0.5);
    return;
}

UINT CLineChartCtrl::UpdatePos(double fPos)
{
    CHECK_VALUE_UP(fPos, m_tItem.m_fUpper);
    CHECK_VALUE_LOW(fPos, m_tItem.m_fLower);

    m_tItem.m_fData[m_tItem.m_nPos] = fPos;
    return m_tItem.m_nPos;
}

void CLineChartCtrl::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    // draw scale
    GetMemDC();

    dc.BitBlt(0, 0, m_rcClient.Width(), m_rcClient.Height(), &m_MemDC, 0, 0, SRCCOPY);
}

void CLineChartCtrl::DrawSpike()
{
    CPoint  ptOld;
    CPoint  ptNew;
    double  fRange = 0;
    CPen    pen(PS_SOLID, 1, m_tItem.m_clrLine);
    CPen* pOldPen = m_MemDC.SelectObject(&pen);

    // Minus one to make sure to draw inside the area
    ptOld.x = int(m_iStaticX * m_fOffset);
    m_iStaticX++;
    ptNew.x = int(m_iStaticX * m_fOffset);
    COMP_BGE(m_iStaticX, MAXLINEITEM);

    fRange = m_tItem.m_fUpper - m_tItem.m_fLower;

    ptOld.y = m_iChartHeight;
    if (m_tItem.m_nPos >= 2)
    {
        ptOld.y = (int)((((fRange - m_tItem.m_fData[m_tItem.m_nPos - 2])) / fRange) * m_iChartHeight);
    }
    ptNew.y = (int)((((fRange - m_tItem.m_fData[m_tItem.m_nPos - 1])) / fRange) * m_iChartHeight);

    //如果曲线没有超出屏幕图片框右边界线，直接在屏幕图片框区域内绘制曲线，不用平移
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

BOOL CLineChartCtrl::Add(COLORREF clrLine, double fUpper, double fLower)
{
    GetClientRect(&m_rcClient);

    /* 获取画图区域的宽和高 */
    m_iChartWidth = m_rcClient.Width() - 1;
    m_iChartHeight = m_rcClient.Height() - 1;

    memset(&m_tItem, 0, sizeof(LINECHARITEM));
    m_tItem.m_clrLine = clrLine;
    m_tItem.m_fLower = fLower;
    m_tItem.m_fUpper = fUpper;
    m_tItem.m_nPos = 0;
    m_bLastPoint = FALSE;
    m_fOffset = (m_iChartWidth * 1.0) / MAXLINEITEM;

    return TRUE;
}

void CLineChartCtrl::FinishDraw()
{
    m_bLastPoint = TRUE;

    DrawSpike();

    Invalidate(FALSE);
}

void CLineChartCtrl::Go()
{
    DrawSpike();

    Invalidate(FALSE);
}

void CLineChartCtrl::Erase()
{
    CPoint  ptZero;

    ptZero.x = 0;
    ptZero.y = 0;

    m_MemDC.MoveTo(ptZero);
    m_MemDC.LineTo(ptZero);
    Invalidate(FALSE);
}

void CLineChartCtrl::DrawFinLine()
{
    double  fRange = 0;
    CPoint  ptOld;
    CPen    pen(PS_SOLID, 1, m_tItem.m_clrLine);
    CPen* pOldPen = m_MemDC.SelectObject(&pen);

    // Minus one to make sure to draw inside the area
    fRange = m_tItem.m_fUpper - m_tItem.m_fLower;
    ptOld.x = int(m_iStaticX * m_fOffset);
    ptOld.y = (int)((((fRange - m_tItem.m_fData[m_tItem.m_nPos - 2])) / fRange) * m_iChartHeight);

    m_MemDC.MoveTo(ptOld);
    ptOld.y = m_iChartHeight;
    m_MemDC.LineTo(ptOld);

    m_MemDC.SelectObject(pOldPen);
    Invalidate(FALSE);
}

BOOL CLineChartCtrl::RemoveAt()
{
    m_iStaticX = 0;

    return TRUE;
}

/*画背景格子线*/
void CLineChartCtrl::DrawGridLine()
{
    int     i = 0;
    CBrush  brBk(m_clrBk);
    CPen    penBk(PS_SOLID, 1, LC_BKCOLOR);
    CPen* pOldPen = NULL;

    m_MemDC.SetBkColor(m_clrBk);
    m_MemDC.FillRect(m_rcClient, &brBk);

    COMP_BFALSE(m_bBKLine);

    /* 背景线 */
    pOldPen = m_MemDC.SelectObject(&penBk);
    for (i = 0; i <= 10; i++)
    {
        m_MemDC.MoveTo(m_iChartWidth / 10 * i, 0);
        m_MemDC.LineTo(m_iChartWidth / 10 * i, m_iChartHeight);
    }
    for (i = 0; i <= 3; i++)
    {
        m_MemDC.MoveTo(0, m_iChartHeight / 3 * i);
        m_MemDC.LineTo(m_iChartWidth, m_iChartHeight / 3 * i);
    }
    m_MemDC.SelectObject(pOldPen);

    return;
}

void CLineChartCtrl::DrawBkLine()
{
    GetMemDC();

    /* 画背景格子线 */
    DrawGridLine();
}

void CLineChartCtrl::SetBkColor(COLORREF clrBk)
{
    m_clrBk = clrBk;
}


/* 打印说明文字 */
void  CLineChartCtrl::ShowContent(COLORREF clrText, int y, string strContent, UINT nLeftOffset)
{
    CPoint  ptBegin;
    CRect   rectCont;
    char    strPrint[MAX_LOADSTRING];

    m_MemDC.SetTextColor(clrText);
    ptBegin.x = int((m_iChartWidth)*CONT_XSCALE);
    ptBegin.y = y;

    rectCont.left = 0;
    rectCont.right = m_rcClient.right - nLeftOffset;
    rectCont.top = y;
    rectCont.bottom = m_rcClient.bottom;

    snprintf(strPrint, MAX_LOADSTRING, "%s          ", strContent.c_str());
    //strPrint = strContent + _T("          ");
    m_MemDC.DrawText(strPrint, -1, &rectCont, DT_SINGLELINE | DT_RIGHT | DT_TOP);
}

UINT CLineChartCtrl::GetCurPoint()
{
    //return (UINT)ceil(m_tItem.m_nPos * m_fOffset);
    return m_tItem.m_nPos;
}

bool CLineChartCtrl::ReDrawLine(double* pData, UINT nCount)
{
    CPoint  ptOld;
    CPoint  ptNew;
    double  fRange = 0;

    ASSERT_NULL_R(pData, false);
    ASSERT_ZERO_R(m_tItem.m_nPos, true);

    DrawBkLine();
    Invalidate(TRUE);

    memset(m_tItem.m_fData, 0, m_tItem.m_nPos * sizeof(double));
    m_tItem.m_nPos = nCount;

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

