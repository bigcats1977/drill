// LineChartCtrlStat.cpp : implementation file
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
#include "LineChartCtrlStat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define     CONT_XOFFSET        5       /* 显示文字，相对于竖线的位置 */
#define     CONT_YOFFSET        12      /* 显示文字，相对于底线的位置 */
#define     CONT_XSCALE         0.75    /* 文字开始位置(相对于整体宽度) */


/////////////////////////////////////////////////////////////////////////////
// CLineChartCtrlStat

CLineChartCtrlStat::CLineChartCtrlStat()
{
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
    m_fMaxLimit = 5000;   /* 最大上限 */
    //m_fUpperTai   = 900;
    //m_fLowerTai   = 200;
    //m_bBear       = FALSE;
    m_bScatter = FALSE;

    CLineChartCtrlStat::RegisterWndClass(AfxGetInstanceHandle());
}

CLineChartCtrlStat::~CLineChartCtrlStat()
{
}


BEGIN_MESSAGE_MAP(CLineChartCtrlStat, CWnd)
    //{{AFX_MSG_MAP(CLineChartCtrlStat)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CLineChartCtrlStat::RegisterWndClass(HINSTANCE hInstance)
{
    WNDCLASS wc;

    wc.lpszClassName = "MULTILINE_TORQUE_CTRL"; // matches class name in client
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

void CLineChartCtrlStat::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    GetMemDC();

    dc.BitBlt(0, 0, m_rcClient.Width(), m_rcClient.Height(), &m_MemDC, 0, 0, SRCCOPY);
}

BOOL CLineChartCtrlStat::Add(COLORREF clrLine, double fUpper, double fLower)
{
    memset(&m_tItem, 0, sizeof(LINECHARITEM));

    m_tItem.m_clrLine = clrLine;
    m_tItem.m_fLower = fLower;
    m_tItem.m_fUpper = fUpper;
    m_tItem.m_nPos = 0;
    m_bLastPoint = FALSE;

    return TRUE;
}

BOOL CLineChartCtrlStat::RemoveAt()
{
    m_iStaticX = 0;

    return TRUE;
}

/*画背景格子线*/
void CLineChartCtrlStat::DrawGridLine()
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
void CLineChartCtrlStat::DrawHLine(int y)
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

/* 根据参数画竖线 */
/* | */
void CLineChartCtrlStat::DrawVLine(int x)
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

/* 打印说明文字 */
void  CLineChartCtrlStat::ShowContent(COLORREF clrText, int y, string strContent, UINT nLeftOffset)
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

void CLineChartCtrlStat::DrawControlLine()
{
    int     x = 0;
    int     y = 0;
    CPen* pOldPen = NULL;
    CPen    penSafe(PS_DOT, 1, LC_SAFECOLOR);
    //CString strTemp;

    pOldPen = m_MemDC.SelectObject(&penSafe);

    /* | */
    x = int((m_iChartWidth)*m_fControlCir / m_fMaxCir);
    DrawVLine(x);
    /* ----- */
    y = int((m_iChartHeight) * (m_fMaxLimit - m_fOptTorq) / m_fMaxLimit);
    DrawHLine(y);
    /* 显示最佳扭矩值 */
    //strTemp.Format(IDS_STRLCXCONTROL, m_fOptTorq);
    //ShowContent(LC_SAFECOLOR, y-CONT_YOFFSET, strTemp);
    ShowContent(LC_SAFECOLOR, y - CONT_YOFFSET,
        string_format(theApp.LoadstringFromRes(IDS_STRLCXCONTROL).c_str(), (m_fOptTorq / SHOWTORQUEUNIT)));

    m_MemDC.SelectObject(pOldPen);
}

void CLineChartCtrlStat::DrawAlarmLine()
{
    int     x = 0;
    int     y = 0;
    CPen    penAlarm(PS_DOT, 1, LC_ALARMCOLOR);
    CPen* pOldPen = NULL;
    //CString strTemp;

    pOldPen = m_MemDC.SelectObject(&penAlarm);

    /* | */
    x = int((m_iChartWidth)*m_fLowerCir / m_fMaxCir);
    DrawVLine(x);
    /* | */
    x = int((m_iChartWidth)*m_fUpperCir / m_fMaxCir);
    DrawVLine(x);
#if 0
    /* ----- */
    y = int((m_iChartHeight) * (m_fMaxLimit - m_fLowerLimit) / m_fMaxLimit);
    DrawHLine(y);
    /* 显示最小扭矩值 */
    //strTemp.Format(IDS_STRLCXLOWLIMIT, m_fLowerLimit);
    //ShowContent(LC_ALARMCOLOR, y-CONT_YOFFSET, strTemp);
    ShowContent(LC_ALARMCOLOR, y - CONT_YOFFSET, 
        string_format(theApp.LoadstringFromRes(IDS_STRLCXLOWLIMIT).c_str(), m_fLowerLimit));
    /* ----- */
    y = int((m_iChartHeight) * (m_fMaxLimit - m_fUpperLimit) / m_fMaxLimit);
    DrawHLine(y);
    /* 显示最大扭矩值 */
    //strTemp.Format(IDS_STRLCXUPLIMIT, m_fUpperLimit);
    //ShowContent(LC_ALARMCOLOR, y-CONT_YOFFSET, strTemp);
    ShowContent(LC_ALARMCOLOR, y - CONT_YOFFSET, 
        string_format(theApp.LoadstringFromRes(IDS_STRLCXUPLIMIT).c_str(), m_fUpperLimit));
#endif
    m_MemDC.SelectObject(pOldPen);
}

void CLineChartCtrlStat::DrawShowLine()
{
    int     x = 0;
    int     y = 0;
    CPen* pOldPen = NULL;
    CPen    penShow(PS_DOT, 1, LC_SHOWCOLOR);
    //CString strTemp;

    pOldPen = m_MemDC.SelectObject(&penShow);

    /* ----- */
    y = int((m_iChartHeight) * (m_fMaxLimit - m_fShow) / m_fMaxLimit);
    DrawHLine(y);
    /* 显示显示扭矩值 */
    //strTemp.Format(IDS_STRLCXSHOW, m_fShow);
    //ShowContent(LC_SHOWCOLOR, y-CONT_YOFFSET, strTemp);
    ShowContent(LC_SHOWCOLOR, y - CONT_YOFFSET,
        string_format(theApp.LoadstringFromRes(IDS_STRLCXSHOW).c_str(), (m_fShow / SHOWTORQUEUNIT)));

    m_MemDC.SelectObject(pOldPen);
}

void CLineChartCtrlStat::DrawBkLine()
{
    int         iOldMode = 0;
    //CString     strTemp;
    //LOGFONT     lf;
    //CFont       ftNew;
    CFont* pOldFont = NULL;
    CClientDC   dc(this);

    GetClientRect(&m_rcClient);
    /* 获取画图区域的宽和高 */
    m_iChartWidth = m_rcClient.Width() - 1;
    m_iChartHeight = m_rcClient.Height() - 1;
    m_fOffset = (m_iChartWidth * 1.0) / MAXLINEITEM;

    GetMemDC();

    /* 画背景格子线 */
    DrawGridLine();
    iOldMode = m_MemDC.SetBkMode(TRANSPARENT);
    pOldFont = m_MemDC.SelectObject(&theApp.m_tLineTextFont);

    /* 控制线 */
    DrawControlLine();

    /* 告警线 */
    DrawAlarmLine();

    /*台阶扭矩*/
    //DrawTaiLine();

    /*显示扭矩*/
    DrawShowLine();

    m_MemDC.SetBkMode(iOldMode);
    m_MemDC.SelectObject(&pOldFont);
}

void CLineChartCtrlStat::DrawMultiData(TORQUEDATA* ptTorqData, vector<int> liNO)
{
    int     i = 0;
    int     j = 0;
    int     count = 0;
    CPoint  ptOld;
    CPoint  ptNew;
    double  fRange = 0;
    CPen    pnLine(PS_SOLID, 1, m_tItem.m_clrLine);
    CPen* pOldPen = m_MemDC.SelectObject(&pnLine);
    double  fMaxTorq = 0;
    double  fStep = 1.0;
    double  fCurX = 0;
    TorqData::Torque* ptTorq = NULL;

    ASSERT_NULL(ptTorqData);

    count = (int)liNO.size();
    fRange = m_tItem.m_fUpper - m_tItem.m_fLower;

    if (!m_bAlign)
    {
        for (j = 0; j < count; j++)
        {
            m_tItem.m_nPos = 0;
            fCurX = 0;
            ptTorq = &ptTorqData->tData[liNO[j]];

            ptOld.x = 0;
            ptOld.y = m_iChartHeight;

            fStep = theApp.GetMaxCir(ptTorq) / m_fMaxCir;
            for (i = 0; i < (int)ptTorq->ftorque_size(); i++)
            {
                fCurX += fStep;
                ptNew.x = int(fCurX * m_fOffset);
                ptNew.y = (int)((((fRange - ptTorq->ftorque(i))) / fRange) * m_iChartHeight);

                m_MemDC.MoveTo(ptOld);
                m_MemDC.LineTo(ptNew);

                ptOld = ptNew;
            }
        }
    }
    /* 最后一根线按控制周数对齐 m_fControlCir */
    else
    {
        for (j = 0; j < count; j++)
        {
            m_tItem.m_nPos = 0;
            fCurX = int(MAXLINEITEM * m_fControlCir / m_fMaxCir);
            ptTorq = &ptTorqData->tData[liNO[j]];

            GET_CTRL_TORQ(fMaxTorq, ptTorq);

            ptOld.x = int(fCurX * m_fOffset);
            ptOld.y = int((m_iChartHeight) * (m_fMaxLimit - fMaxTorq) / m_fMaxLimit);

            fStep = theApp.GetMaxCir(ptTorq) / m_fMaxCir;
            for (i = (int)(ptTorq->ftorque_size() - 2); i >= 0 && fCurX > 0; i--)
            {
                fCurX -= fStep;
                ptNew.x = int(fCurX * m_fOffset);
                ptNew.y = (int)((((fRange - ptTorq->ftorque(i))) / fRange) * m_iChartHeight);

                m_MemDC.MoveTo(ptOld);
                m_MemDC.LineTo(ptNew);

                ptOld = ptNew;
            }
        }
    }
    m_MemDC.SelectObject(pOldPen);

    Invalidate(FALSE);
}

void CLineChartCtrlStat::DrawScatter(int x, int y, BOOL bAccept, BOOL bIP)
{
    COLORREF    clrPnt = SCATTER_ACC_CTRL;
    CRgn        rgnOut, rgnIn;
    CBrush      brBg, brFront;

    if (bAccept)
    {
        if (bIP)
            clrPnt = SCATTER_ACC_IP;
    }
    else
    {
        clrPnt = SCATTER_REJ_CTRL;
        if (bIP)
            clrPnt = SCATTER_REJ_IP;
    }

    rgnOut.CreateEllipticRgn(x - 6, y - 6, x + 6, y + 6);
    brFront.CreateSolidBrush(clrPnt);
    m_MemDC.FillRgn(&rgnOut, &brFront);

    rgnIn.CreateEllipticRgn(x - 4, y - 4, x + 4, y + 4);
    brBg.CreateSolidBrush(SCATTER_BG);
    m_MemDC.FillRgn(&rgnIn, &brBg);
}

void CLineChartCtrlStat::DrawMultiScatter(TORQUEDATA* ptTorqData)
{
    int     i = 0;
    int     x, y;
    DWORD   dwQuality = 0;
    BOOL    bAccepted = FALSE;
    double  fRange = 0;
    WORD    wIPPos = 0;
    WORD    wSchPos = 0;
    WORD    wCtrlPos = 0;
    double  fCtrlTorq = 0;
    //UINT    nIPTorq     = 0;
    TorqData::Torque* ptTorq = NULL;

    ASSERT_NULL(ptTorqData);

    fRange = m_tItem.m_fUpper - m_tItem.m_fLower;

    for (i = ptTorqData->nTotal - 1; i >= 0; i--)
    {
        ptTorq = &ptTorqData->tData[i];

        dwQuality = theApp.GetQuality(ptTorq);
        bAccepted = FALSE;
        if (dwQuality & QUA_RESU_QUALITYBIT)
        {
            bAccepted = TRUE;
        }

        GET_CTRL_TORQ(fCtrlTorq, ptTorq);
        wCtrlPos = ptTorq->ftorque_size();
        //nIPTorq = theApp.GetIPTorq(ptTorq, wIPPos, wSchPos);
        x = (int)(wCtrlPos * theApp.GetMaxCir(ptTorq) / m_fMaxCir * m_fOffset);
        y = (int)((((fRange - fCtrlTorq)) / fRange) * m_iChartHeight);
        DrawScatter(x, y, bAccepted, FALSE);
#if 0
        if (wIPPos > 0 && nIPTorq > 0)
        {
            x = (int)(wIPPos * theApp.GetMaxCir(ptTorq) / m_fMaxCir * m_fOffset);
            y = (int)((((fRange - nIPTorq)) / fRange) * m_iChartHeight);
            DrawScatter(x, y, bAccepted, TRUE);
        }
#endif
    }
}
