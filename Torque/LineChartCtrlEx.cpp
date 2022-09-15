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
#include "Torque.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define     CONT_XOFFSET        5       /* ��ʾ���֣���������ߵ�λ�� */
#define     CONT_YOFFSET        12      /* ��ʾ���֣�����ڵ��ߵ�λ�� */
#define     CONT_XSCALE         0.75    /* ���ֿ�ʼλ��(�����������) */
#define     CONT_XANTIRANGE     425     /* ���곬����λ��ʱ����Ҫ��������ʾ���� */
#define     CONT_XANTIOFFSET    80      /* �������ұ�ʱ����������� */
#define     CONT_XRIGHTRANGE    380     /* �յ�/ѡ�е���Ϣ���̫��������ʾ���� */
/////////////////////////////////////////////////////////////////////////////
// CLineChartCtrlEx

CLineChartCtrlEx::CLineChartCtrlEx()
{
    m_uLineType   = LINETYPE_MAIN;
    m_fOffset     = 1;
    m_iStaticX    = 0;
    m_iChartWidth = MAXLINEITEM;
    m_iChartHeight= 400;

    m_fUpperLimit = 2500;   /* ���Ť�� */
    m_fLowerLimit = 1800;   /* ��СŤ�� */
    m_fOptTorq    = 2000;   /* ���Ť�� */
    m_fSpeedDown  = 300;    /* ����Ť�� */
    m_fShow       = 100;    /* ��ʾŤ�� */
    m_fBear       = 1500;   /* �縺Ť�� */
    m_fControlCir = 2;      /* �������� */
    m_fUpperCir   = 4.5;    /* �������� */
    m_fLowerCir   = 0.1;    /* �������� */
    m_fMaxCir     = 5;      /* ������� */
    m_fWidthCir   = 5;
    m_fMaxLimit   = 5000;   /* ������� */
    m_fUpperTai   = 900;
    m_fLowerTai   = 200;
    m_bLastPoint  = FALSE;
    m_clrBk       = CTRLBKCOLOR;
    m_bBear       = FALSE;
    m_bBKLine     = TRUE;
    m_bIPLock     = FALSE;
    //m_nTorqNo     = 1;
    m_bIPSeled    = FALSE;
    m_nSelPntNum  = 0;
    m_nStartPoint = 0;
    memset(m_nSelPos, 0, sizeof(UINT)*MAX_SHOWSELPOINTNUM);
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

    wc.lpszClassName    = "LINE_TORQUE_CTRL"; // matches class name in client
    wc.hInstance        = hInstance;
    wc.lpfnWndProc      = ::DefWindowProc;
    wc.hCursor          = ::LoadCursor(NULL, IDC_ARROW);
    wc.hIcon            = 0;
    wc.lpszMenuName     = NULL;
    wc.hbrBackground    = (HBRUSH) ::GetStockObject(LTGRAY_BRUSH);
    wc.style            = CS_GLOBALCLASS; // To be modified
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;

    return (::RegisterClass(&wc) != 0);
}

void CLineChartCtrlEx::GetMemDC()
{
    CClientDC dc(this);

    COMP_BNE(m_MemDC.GetSafeHdc(), NULL);

    m_MemDC.CreateCompatibleDC(&dc);
    m_Bitmap.CreateCompatibleBitmap(&dc,m_rcClient.Width(),m_rcClient.Height());
    m_MemDC.SelectObject(m_Bitmap);
}

void CLineChartCtrlEx::InvalidateCtrl()
{
    if (m_MemDC.GetSafeHdc() == NULL)
    {
        GetMemDC();

        // draw scale
        DrawBkLine();
    }

    InvalidateRect(m_rcClient, FALSE);
}

UINT CLineChartCtrlEx::SetPos(double fPos, BOOL bLast)
{
    CHECK_VALUE_UP (fPos, m_tItem.m_fUpper);
    CHECK_VALUE_LOW(fPos, m_tItem.m_fLower);

    /* ����500�����ݣ�˵��ͼ�����һ�����������¿�ʼ */
    if(m_tItem.m_nPos >= MAXLINEITEM)
    {
        m_tItem.m_nPos = 0;

        return 0;
    }

    m_bLastPoint = bLast;
    m_tItem.m_fData[m_tItem.m_nPos++] = fPos;
    return m_tItem.m_nPos;
}

void CLineChartCtrlEx::SetStartPoint(UINT nBegin)
{
    COMP_BGE(nBegin, MAXLINEITEM);
    if(nBegin >= MAXLINEITEM)
    {
        return;
    }

    m_iStaticX = nBegin;// int(nBegin / m_fOffset + 0.5);
    m_tItem.m_nPos = nBegin;

    m_nStartPoint = nBegin;
    
    return;
}

UINT CLineChartCtrlEx::UpdatePos(double fPos)
{
    CHECK_VALUE_UP (fPos, m_tItem.m_fUpper);
    CHECK_VALUE_LOW(fPos, m_tItem.m_fLower);

    COMP_BLE_R(m_tItem.m_nPos, 1, m_tItem.m_nPos);
    EraseLastOne();
    m_tItem.m_fData[m_tItem.m_nPos-1] = fPos;
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
    CPen*   pOldPen = m_MemDC.SelectObject(&pnLine);

    COMP_BL(m_iStaticX, 2);
    COMP_BL(m_tItem.m_nPos, 2);
    
    ptOld.x = int((m_iStaticX-1) * m_fOffset);
    ptNew.x = int(m_iStaticX * m_fOffset);
    
    fRange = m_tItem.m_fUpper - m_tItem.m_fLower;
    ptOld.y = (int)((((fRange - m_tItem.m_fData[m_tItem.m_nPos-2]+m_tItem.m_fLower))/fRange) * m_iChartHeight ) ;
    ptNew.y = (int)((((fRange - m_tItem.m_fData[m_tItem.m_nPos-1]+m_tItem.m_fLower))/fRange) * m_iChartHeight );

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
    CPen*   pOldPen = m_MemDC.SelectObject(&pnLine);

    // Minus one to make sure to draw inside the area
    if(bStop && m_iStaticX > 0)
        m_iStaticX --;
    ptOld.x = int(m_iStaticX * m_fOffset);
    m_iStaticX ++;
    ptNew.x = int(m_iStaticX * m_fOffset);

    COMP_BGE(m_iStaticX, MAXLINEITEM);
    
    fRange = m_tItem.m_fUpper - m_tItem.m_fLower;
    ptOld.y = m_iChartHeight;
    if(m_tItem.m_nPos >= 2)
    {
        ptOld.y = (int)((((fRange - m_tItem.m_fData[m_tItem.m_nPos-2]+m_tItem.m_fLower))/fRange) * m_iChartHeight ) ;
    }
    ptNew.y = (int)((((fRange - m_tItem.m_fData[m_tItem.m_nPos-1]+m_tItem.m_fLower))/fRange) * m_iChartHeight );

    //�������û�г�����ĻͼƬ���ұ߽��ߣ�ֱ������ĻͼƬ�������ڻ������ߣ�����ƽ��
/*
    if(ptOld.y == m_iChartHeight)
        m_MemDC.MoveTo(ptNew);
    else
*/
    m_MemDC.MoveTo(ptOld);
    m_MemDC.LineTo(ptNew);
    
    if(m_bLastPoint)
    {
        m_MemDC.MoveTo(ptNew);
        ptNew.y = m_iChartHeight;
        m_MemDC.LineTo(ptNew);
    }
    m_MemDC.SelectObject(pOldPen);
}

BOOL CLineChartCtrlEx::Add(COLORREF clrLine, double fUpper, double fLower, BYTE uType)
{
    memset(&m_tItem, 0, sizeof(LINECHARITEM));

    m_tItem.m_clrLine = clrLine;
    m_tItem.m_fLower  = fLower;
    m_tItem.m_fUpper  = fUpper;
    m_tItem.m_nPos    = 0;
    m_bLastPoint      = FALSE;
    m_uLineType       = uType;    

    return TRUE;
}

void CLineChartCtrlEx::FinishDraw()
{
    CPoint  ptNew;
    double  fRange = 0;
    CPen    pnLine(PS_SOLID, 1, m_tItem.m_clrLine);
    CPen*   pOldPen = m_MemDC.SelectObject(&pnLine);

    m_bLastPoint = TRUE;

    fRange = m_tItem.m_fUpper - m_tItem.m_fLower;
    ptNew.x = int(m_iStaticX * m_fOffset);
    ptNew.y = (int)((((fRange - m_tItem.m_fData[m_tItem.m_nPos-1]+m_tItem.m_fLower))/fRange) * m_iChartHeight );
    m_MemDC.MoveTo(ptNew);
    ptNew.y = m_iChartHeight;
    m_MemDC.LineTo(ptNew);
    m_MemDC.SelectObject(pOldPen);

    Invalidate(FALSE);
}

void CLineChartCtrlEx::Go()
{
    DrawSpike();

    Invalidate(FALSE);
}

void CLineChartCtrlEx::Erase()
{
    CPoint  ptZero;

    ptZero.x = 0;
    ptZero.y = 0;

    m_MemDC.MoveTo(ptZero);
    m_MemDC.LineTo(ptZero);
    Invalidate(FALSE);
}

void CLineChartCtrlEx::DrawFinLine()
{
    double  fRange = 0;
    CPoint  ptOld;
    CPen    pnLine(PS_SOLID, 1, m_tItem.m_clrLine);
    CPen*   pOldPen = m_MemDC.SelectObject(&pnLine);

    // Minus one to make sure to draw inside the area
    fRange = m_tItem.m_fUpper - m_tItem.m_fLower;
    ptOld.x = int(m_iStaticX * m_fOffset);
    ptOld.y = (int)((((fRange - m_tItem.m_fData[m_tItem.m_nPos-1]+m_tItem.m_fLower))/fRange) * m_iChartHeight );
    
    m_MemDC.MoveTo(ptOld);
    ptOld.y = m_iChartHeight;
    m_MemDC.LineTo(ptOld);
    
    m_MemDC.SelectObject(pOldPen);
    Invalidate(FALSE);
}

BOOL CLineChartCtrlEx::RemoveAt()
{
    m_iStaticX = 0;
    m_nStartPoint = 0;
    m_rcInterPt.SetRectEmpty();

    return TRUE;
}

/* ��ӡʱ��ʾ��� */
void CLineChartCtrlEx::ShowTorqNo()
{
#if 0
    CPoint      ptBegin;
    CString     strTemp;
    COMP_BTRUE(m_bBKLine);
    strTemp.Format("%05d", m_nTorqNo);

    m_MemDC.SetTextColor(CTRLBLACKCOLOR);
    ptBegin.x = int((m_iChartWidth)*0.5-20);
    ptBegin.y = 10;
    m_MemDC.TextOut(ptBegin.x,ptBegin.y, strTemp);
#endif
}

/*������������*/
void CLineChartCtrlEx::DrawGridLine(COLORREF clrBk)
{
    int     i        = 0;
    CBrush  brBk(m_clrBk);
    CPen    penBk(PS_SOLID, 1, clrBk);
    CPen    *pOldPen = NULL;
    
    m_MemDC.SetBkColor(m_clrBk);
    m_MemDC.FillRect(m_rcClient,&brBk);

    COMP_BFALSE(m_bBKLine);

    /* ������ */
    pOldPen = m_MemDC.SelectObject(&penBk);
    for(i=0; i<=10; i++)
    {
        m_MemDC.MoveTo((m_iChartWidth)/10*i,0);
        m_MemDC.LineTo((m_iChartWidth)/10*i,m_iChartHeight);
    }
    for(i=0; i<=20 ;i++)
    {
        m_MemDC.MoveTo(0,(m_iChartHeight)/20*i);
        m_MemDC.LineTo(m_iChartWidth,(m_iChartHeight)/20*i);
    }
    m_MemDC.SelectObject(pOldPen);

    return;
}

/*���ݲ���������*/
/* ----- */
void CLineChartCtrlEx::DrawHLine(int y)
{
    CPoint  ptBegin;
    CPoint  ptEnd;

    ptBegin.x = 0;
    ptBegin.y = y;
    ptEnd.x   = m_iChartWidth;
    ptEnd.y   = y;
    m_MemDC.MoveTo(ptBegin);
    m_MemDC.LineTo(ptEnd);
}

/* ���ݲ��������� */
/* | */
void CLineChartCtrlEx::DrawVLine(int x)
{
    CPoint  ptBegin;
    CPoint  ptEnd;

    ptBegin.x = x;
    ptBegin.y = 0;
    ptEnd.x   = x;
    ptEnd.y   = m_iChartHeight;
    m_MemDC.MoveTo(ptBegin);
    m_MemDC.LineTo(ptEnd);
}

/* ��ӡ˵������ */
void  CLineChartCtrlEx::ShowContent(COLORREF clrText, int y, CString strContent, UINT nLeftOffset)
{
    CPoint  ptBegin;
    CRect   rectCont;
    CString strPrint;

    m_MemDC.SetTextColor(clrText);
    ptBegin.x = int((m_iChartWidth)*CONT_XSCALE);
    ptBegin.y = y;

    rectCont.left = 0;
    rectCont.right = m_rcClient.right - nLeftOffset;
    rectCont.top = y;
    rectCont.bottom = m_rcClient.bottom;

    strPrint = strContent + _T("          ");
    m_MemDC.DrawText(strPrint, -1, &rectCont, DT_SINGLELINE | DT_RIGHT | DT_TOP);
}

/* ��ӡѡ���� / �Ŵ��Ķ�̬λ�õ���Ϣ */
void CLineChartCtrlEx::ShowVarPntText(COLORREF clrText, int x, int y, CString strContent)
{
    int nPos = int(x / m_fOffset);

    m_MemDC.SetTextColor(clrText);
    
    if(nPos < CONT_XANTIRANGE)
        x += 10;
    else
        x = x - CONT_XANTIOFFSET;

    m_MemDC.TextOut(x+2, y-CONT_YOFFSET-5, strContent);
}

/* ��ӡѡ���� / Ť�ص�������λ�� */
void CLineChartCtrlEx::ShowRightPntText(COLORREF clrText, int x, int y, CString strContent)
{
    int xx   = 0;
    int nPos = int(x / m_fOffset);

    xx=int(m_tItem.m_nPos * m_fOffset);

    m_MemDC.SetTextColor(clrText);
    
    if(xx < CONT_XRIGHTRANGE)
        x = xx;
    else
        x = x - CONT_XANTIOFFSET;

    m_MemDC.TextOut(x+2, y-CONT_YOFFSET-5, strContent);
}

void CLineChartCtrlEx::DrawControlLine()
{
    int     x        = 0;
    int     y        = 0;
    CPen    *pOldPen = NULL;
    CString strTemp;
    COLORREF clrCtrl = LC_SAFECOLOR;
    
    if(LINETYPE_HISG == m_uLineType)
    {
        clrCtrl = CTRLBKCOLOR;
    }
    
    CPen penSafe (PS_DOT, 1, clrCtrl);

    pOldPen = m_MemDC.SelectObject(&penSafe);

    /* | */
    x = int((m_iChartWidth)*m_fControlCir/m_fWidthCir);
    DrawVLine(x);
    /* ----- */
    y = int((m_iChartHeight)*(m_fMaxLimit-m_fOptTorq)/m_fMaxLimit);
    DrawHLine(y);    
    /* ��ʾ���Ť��ֵ */
    strTemp.Format(IDS_STRLCXCONTROL, m_fOptTorq);
    ShowContent(clrCtrl, y-CONT_YOFFSET, strTemp);

    m_MemDC.SelectObject(pOldPen);
}

void CLineChartCtrlEx::DrawAlarmLine()
{
    int     x        = 0;
    int     y        = 0;
    CPen    *pOldPen = NULL;
    CString strTemp;
    COLORREF clrAlarm = LC_ALARMCOLOR;
    
    if(LINETYPE_HISG == m_uLineType)
    {
        clrAlarm = CTRLBKCOLOR;
    }

    CPen    penAlarm(PS_DOT, 1, clrAlarm);
    pOldPen = m_MemDC.SelectObject(&penAlarm);
    
    /* | */
    x = int((m_iChartWidth)*m_fLowerCir/m_fWidthCir);
    DrawVLine(x);
    /* | ��ʷ���ݣ�������������� */
    if(LINETYPE_HISG != m_uLineType)
    {
        x = int((m_iChartWidth)*m_fUpperCir/m_fWidthCir);
        DrawVLine(x);
    }
    /* ----- */
    y = int((m_iChartHeight)*(m_fMaxLimit-m_fLowerLimit)/m_fMaxLimit);
    DrawHLine(y);
    /* ��ʾ��СŤ��ֵ */
    strTemp.Format(IDS_STRLCXLOWLIMIT, m_fLowerLimit);
    ShowContent(clrAlarm, y-CONT_YOFFSET, strTemp);    
    /* ----- */    
    y = int((m_iChartHeight)*(m_fMaxLimit-m_fUpperLimit)/m_fMaxLimit);
    DrawHLine(y);
    /* ��ʾ���Ť��ֵ */
    strTemp.Format(IDS_STRLCXUPLIMIT, m_fUpperLimit);
    ShowContent(clrAlarm, y-CONT_YOFFSET, strTemp);

    m_MemDC.SelectObject(pOldPen);
}

void CLineChartCtrlEx::DrawTaiLine()
{
    int     x        = 0;
    int     y        = 0;
    CPen    *pOldPen = NULL;
    CString strTemp;
    COLORREF clrTai = LC_TAICOLOR;
    
    if(LINETYPE_HISG == m_uLineType)
    {
        clrTai = CTRLBKCOLOR;
    }
    CPen    penTai  (PS_DOT, 1, clrTai);

    pOldPen = m_MemDC.SelectObject(&penTai);
    
    /*���̨��Ť��*/
    /* ----- */    
    y = int((m_iChartHeight)*(m_fMaxLimit-m_fUpperTai)/m_fMaxLimit);
    DrawHLine(y);
    /* ��ʾ���̨��Ť��ֵ */
    strTemp.Format(IDS_STRLCXUPTAI, m_fUpperTai);
    ShowContent(clrTai, y-CONT_YOFFSET, strTemp);
#if 0
    /* ��ʾ����Ť�� */
    /*���̨��Ť��(����Ť��) */
    /* ----- */
    y = int((m_iChartHeight)*(m_fMaxLimit-m_fSpeedDown)/m_fMaxLimit);
    DrawHLine(y);
    /* ��ʾ���̨��Ť��ֵ */
    strTemp.Format(IDS_STRLCXSPEED, m_fSpeedDown);
    ShowContent(LC_SPEEDCOLOR, y-CONT_YOFFSET, strTemp);
#endif
    /*��С̨��Ť��*/
    /* ----- */
    y = int((m_iChartHeight)*(m_fMaxLimit-m_fLowerTai)/m_fMaxLimit);
    DrawHLine(y);
    /* ��ʾ��С̨��Ť��ֵ */
    strTemp.Format(IDS_STRLCXLOWTAI, m_fLowerTai);
    ShowContent(clrTai, y-CONT_YOFFSET, strTemp);

    m_MemDC.SelectObject(pOldPen);
}

void CLineChartCtrlEx::DrawShowLine()
{
    int     x        = 0;
    int     y        = 0;
    CPen    *pOldPen = NULL;
    CString strTemp;

    COLORREF clrShow = LC_SHOWCOLOR;
    
    if(LINETYPE_HISG == m_uLineType)
    {
        clrShow = CTRLBKCOLOR;
    }
    
    CPen    penShow (PS_DOT, 1, clrShow);

    pOldPen = m_MemDC.SelectObject(&penShow);
    
    /* ----- */
    y = int((m_iChartHeight)*(m_fMaxLimit-m_fShow)/m_fMaxLimit);
    DrawHLine(y);
    /* ��ʾ��ʾŤ��ֵ */
    strTemp.Format(IDS_STRLCXSHOW, m_fShow);
    ShowContent(clrShow, y-CONT_YOFFSET, strTemp);

    m_MemDC.SelectObject(pOldPen);
}

void CLineChartCtrlEx::DrawBearLine()
{
    int     x        = 0;
    int     y        = 0;
    CPen    *pOldPen = NULL;
    CString strTemp;
    COLORREF clrBear = LC_BEARCOLOR;
    
    if(LINETYPE_HISG == m_uLineType)
    {
        clrBear = CTRLBKCOLOR;
    }
    CPen    penBear(PS_DOT,1,clrBear);

    pOldPen = m_MemDC.SelectObject(&penBear);

    COMP_BFALSE(m_bBear);

    /* ----- */
    y = int((m_iChartHeight)*(m_fMaxLimit-m_fBear)/m_fMaxLimit);
    DrawHLine(y);
    /* ��ʾ�縺Ť��ֵ */
    strTemp.Format(IDS_STRLCXBEAR, m_fBear);
    ShowContent(clrBear, y-CONT_YOFFSET, strTemp);

    m_MemDC.SelectObject(pOldPen);
}

void CLineChartCtrlEx::DrawBkLine()
{
    int         iOldMode  = 0;
    CString     strTemp;
    CFont       *pOldFont = NULL;
    CClientDC   dc(this);

    GetClientRect(&m_rcClient);

    /* ��ȡ��ͼ����Ŀ�͸� */
    m_iChartWidth = m_rcClient.Width()-1;
    m_iChartHeight= m_rcClient.Height()-1;
    m_fOffset     = (m_iChartWidth * 1.0) / MAXLINEITEM ;

    GetMemDC();


    /* ������������ */
    DrawGridLine();
    iOldMode= m_MemDC.SetBkMode(TRANSPARENT);
    if(LINETYPE_HISG == m_uLineType)
        pOldFont= m_MemDC.SelectObject(&theApp.m_tPntTextFont);
    else
        pOldFont= m_MemDC.SelectObject(&theApp.m_tLineTextFont);

    /* Ť����� */
    //ShowTorqNo();

    /* ������ */
    DrawControlLine();

    /* �澯�� */
    DrawAlarmLine();

    /*̨��Ť��*/
    DrawTaiLine();

    /*��ʾŤ��*/
    DrawShowLine();

    /*�縺Ť��*/
    DrawBearLine();

    m_MemDC.SetBkMode(iOldMode);
    m_MemDC.SelectObject(&pOldFont);
}

void CLineChartCtrlEx::DrawZoomBkLine()
{
    int         iOldMode  = 0;
    CString     strTemp;
    
    CFont       *pOldFont = NULL;
    CClientDC   dc(this);
    GetClientRect(&m_rcClient);
    
    /* ��ȡ��ͼ����Ŀ�͸� */
    m_iChartWidth = m_rcClient.Width()-1;
    m_iChartHeight= m_rcClient.Height()-1;
    m_fOffset     = (m_iChartWidth * 1.0) / MAXLINEITEM ;

    GetMemDC();

    /* ������������ */
    DrawGridLine(LC_ZOOMCOLOR);
    pOldFont= m_MemDC.SelectObject(&theApp.m_tLineTextFont);

    m_MemDC.SetBkMode(iOldMode);
    m_MemDC.SelectObject(&pOldFont);
}

void CLineChartCtrlEx::SetBkColor(COLORREF clrBk)
{
    m_clrBk = clrBk;
}

void CLineChartCtrlEx::DrawInflection(int iIPPos, double fDelIPCir, double fSetIPTorq)//, COLORREF clrIP)
{
    double      fRange;
    int         x = 0, y = 0;
    double      fIPTorq = 0;//      fIPTorq = 0;
    int         i = 0;
    CString     strTemp;
    CBitmap*    pOldBitmap;
    CDC         dcMemory;
    CPen*       pOldPen = NULL;
    int         iRealIPPos = 0;
    COLORREF    clrIP = LC_SAFECOLOR;
    
    if(LINETYPE_HISG == m_uLineType)
    {
        clrIP = CTRLBKCOLOR;
    }
    
    CPen        pnLine(PS_DOT, 1, clrIP);

    COMP_BLE(iIPPos, 0);

    iRealIPPos = iIPPos + m_nStartPoint;

    fIPTorq = fSetIPTorq;
    if(fIPTorq == -1)
        fIPTorq = (UINT)m_tItem.m_fData[iRealIPPos];
    
    fRange  = m_tItem.m_fUpper - m_tItem.m_fLower;
    x       = int(iRealIPPos * m_fOffset);
    y       = (int)(((fRange - fIPTorq + m_tItem.m_fLower)/fRange) * m_iChartHeight ) ;


    /*���յ�Ť�ص���*/
    pOldPen = m_MemDC.SelectObject(&pnLine);
    /*�յ�Ť��*/
    /* | */
    DrawVLine(x);
    
    /* ----- */ 
    DrawHLine(y);
    /* �յ�Ť�� */
    if(0 == fDelIPCir)
        fDelIPCir  = ((int)m_tItem.m_nPos - iRealIPPos)*m_fWidthCir/MAXLINEITEM;
    strTemp.Format("(%.3f \xA1\xF1 %.f)", fDelIPCir, fIPTorq );
    ShowRightPntText(clrIP, x, y, strTemp);

    m_MemDC.SelectObject(pOldPen);

    dcMemory.CreateCompatibleDC(&m_MemDC);

    // Select the bitmap into the in-memory DC
    if(m_uLineType == LINETYPE_MAIN)
        pOldBitmap = dcMemory.SelectObject(&m_bpCross);
    else
        pOldBitmap = dcMemory.SelectObject(&m_bpWCross);

    m_MemDC.BitBlt(x-5, y-5, m_bmpInfo.bmWidth, m_bmpInfo.bmHeight, &dcMemory,0, 0, SRCCOPY);
    dcMemory.SelectObject(pOldBitmap);
    dcMemory.DeleteDC();

    m_rcInterPt = CRect(x-7, y-7, x-3 + m_bmpInfo.bmWidth, y-3+m_bmpInfo.bmHeight);

    return;
}

/* ���������ʾ��ǰŤ�غ����� */
void CLineChartCtrlEx::DrawSelInfo(UINT nBeginPos, UINT nTotal)
{
    double      fRange = 0;
    int         x = 0, y = 0;
    double      fSelTorq = 0;
    double      fSelCir  = 0;
    UINT        i = 0;
    CString     strTemp;
    CDC         dcMemory;
    CPen        pnLine(PS_DOT, 1, IP_SELCOLOR);
    CPen*       pOldPen = NULL;

    ASSERT_ZERO(m_nSelPntNum);
    for(i=0; i<m_nSelPntNum && i<MAX_SHOWSELPOINTNUM; i++)
    {
        if(m_nSelPos[i] >= m_tItem.m_nPos)
            continue;

        fSelTorq = (UINT)m_tItem.m_fData[m_nSelPos[i]];
        fSelCir  = (nTotal-nBeginPos-m_nSelPos[i] + m_nStartPoint)*m_fWidthCir/MAXLINEITEM;
        
        fRange  = m_tItem.m_fUpper - m_tItem.m_fLower;
        x       = int(m_nSelPos[i] * m_fOffset);
        y       = (int)(((fRange - fSelTorq)/fRange) * m_iChartHeight ) ;


        /*��ѡ��ǰλ�õ�Ť�ص���*/
        pOldPen = m_MemDC.SelectObject(&pnLine);
        /* | */
        DrawVLine(x);
        
        /* ----- */ 
        DrawHLine(y);

        /* ��ʾ��ǰŤ�غ�������Ϣ */
        strTemp.Format("(%.3f \xA1\xF1 %.f)", fSelCir, fSelTorq );
        ShowRightPntText(IP_SELCOLOR, x, y, strTemp);
    }
    m_MemDC.SelectObject(pOldPen);
}

void CLineChartCtrlEx::OnRButtonDown(UINT nFlags, CPoint point)
{
    UINT    nCurPos = 0;
    HWND hParent = ::GetParent(m_hWnd);
    
    /* ֻ����hisgraph����Ŵ����ƶ��յ��¼� */
    if(m_uLineType == LINETYPE_HISG)
    {
        COMP_BTRUE(m_bIPLock); /* ������IP�����������޸ģ�ֻ�ܿ� */
        if(m_rcInterPt.PtInRect(point))
        {
            m_bIPSeled = TRUE;
        }
    }
    else if(m_uLineType == LINETYPE_ZOOM)
    {
        ::PostMessage(hParent, WM_RBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
    }

    CWnd::OnRButtonDown(nFlags, point);
}

BOOL CLineChartCtrlEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
    UINT    nCurPos = 0;
    HWND hParent = ::GetParent(m_hWnd);

    /* ֻ����hisgraph����Ŵ����Ŵ��¼� */
    COMP_BFALSE_R(m_uLineType == LINETYPE_HISG, FALSE); 

    ScreenToClient(&point);
    nCurPos = UINT(point.x/m_fOffset);

    ::SendMessage(hParent, WM_INTERPT_ZOOMIN, 0, nCurPos);

    return CWnd::OnMouseWheel(nFlags, zDelta, point);
}

void CLineChartCtrlEx::OnRButtonUp(UINT nFlags, CPoint point)
{
    UINT        nIPPos = 0;
    HWND hParent = ::GetParent(m_hWnd);

    COMP_BFALSE(m_uLineType == LINETYPE_HISG);
    COMP_BFALSE(m_bIPSeled);
    m_bIPSeled = FALSE;

    nIPPos = UINT(point.x/m_fOffset);

    ::SendMessage(hParent, WM_INTERPT_CHANGE,WPARAM(nIPPos), 1);//m_tItem.m_fData[nIPPos]

    CWnd::OnRButtonUp(nFlags, point);
}

void CLineChartCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{

    UINT        nIPPos = 0;
    HWND hParent = ::GetParent(m_hWnd);
    
    SetFocus();

    COMP_BFALSE(m_uLineType == LINETYPE_HISG);
    COMP_BFALSE(m_bIPSeled);

    nIPPos = UINT(point.x/m_fOffset);

    ::SendMessage(hParent, WM_INTERPT_CHANGE,WPARAM(nIPPos), 0);//

    CWnd::OnMouseMove(nFlags, point);
}

void CLineChartCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{ 
    UINT    nCurPos = 0;
    HWND hParent = ::GetParent(m_hWnd);

    /* ֻ����hisgraph����Ŵ������������ʾ��ǰŤ�غ����� */
    if(m_uLineType == LINETYPE_HISG || m_uLineType == LINETYPE_ZOOM)
    {
        /* �ﵽָ��ѡ����Ŀ�ĵ�����ȫ����� */
        /* ȥ����ǰŤ�غ���������ʾ */
        if(m_nSelPntNum >= MAX_SHOWSELPOINTNUM)
        {
            m_nSelPntNum = 0;
            memset(m_nSelPos, 0, sizeof(UINT)*MAX_SHOWSELPOINTNUM);
            ::SendMessage(hParent, WM_UPDATE_SELPOS, 0, 0);
            return;
        }

        nCurPos = UINT(point.x/m_fOffset);
        if( m_uLineType == LINETYPE_HISG)
        {
            COMP_BGE(nCurPos, m_tItem.m_nPos);
            COMP_BL(nCurPos, m_nStartPoint);
        }
        if( m_uLineType == LINETYPE_ZOOM)
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
    fDeltaCir: > 0 ������ǰ���������ݵ�DelCir���㣬���ں�������Cir���������� - CurCir 
               < 0 �����������һ������ֵΪ��ControlCir��DelCir = ControlCir-CurCir
*/
void CLineChartCtrlEx::DrawZoomInfo(WORD wZoomPos, double fMinCir, double fSrcMaxCir, double fDeltaCir, double fSetZoomTorq)
{
    double      fRange   = 0;
    double      fSelTorq = 0;
    double      fSelCir  = 0;
    int         x = 0, y = 0;
    UINT        i = 0;
    CString     strTemp;
    CBitmap*    pOldBitmap;
    CDC         dcMemory;
    CPen        pnLine(PS_DOT, 1, ZOOM_SELCOLOR);
    CPen*       pOldPen = NULL;
    UINT        nSelPos = 0;
    
    CPoint  ptBegin;
    CPoint  ptEnd;

    if(fMinCir < 0)
        fMinCir = 0;

    if(fSetZoomTorq == -1)
        fSetZoomTorq = (UINT)m_tItem.m_fData[wZoomPos];
    
    fRange  = m_tItem.m_fUpper - m_tItem.m_fLower;
    pOldPen = m_MemDC.SelectObject(&pnLine);

    /* ��ѡ���ĵ� */
    for(i=0; i<m_nSelPntNum && i<MAX_SHOWSELPOINTNUM; i++)
    {
        if (m_nSelPos[i] >= MAXLINEITEM)
            continue;

        fSelTorq = (UINT)(m_tItem.m_fData[m_nSelPos[i]]);
        fSelCir = fMinCir + m_nSelPos[i]*m_fWidthCir/MAXLINEITEM; // ��ǰ���ڷŴ�ͼ���е�����

        fSelCir = fDeltaCir - fSelCir;

        x        = int(m_nSelPos[i] * m_fOffset);
        y        = (int)(((fRange - fSelTorq + m_tItem.m_fLower)/fRange) * m_iChartHeight ) ;

        /* | */
        DrawVLine(x);
        
        /* ----- */ 
        DrawHLine(y);

        strTemp.Format("(%.4f \xA1\xF1 %.f)", fSelCir, fSelTorq );
        ShowVarPntText(ZOOM_SELCOLOR, x, y, strTemp);
    }
    
    /*���Ŵ��Ť�ص���*/
    /* ���Ŵ������ */
    x = int(wZoomPos * m_fOffset);
    y = (int)(((fRange - fSetZoomTorq + m_tItem.m_fLower) / fRange) * m_iChartHeight);

    /*�Ŵ��Ť��*/
    /* | */
    DrawVLine(x);

    /* ----- */
    DrawHLine(y);

    /* �Ŵ��Ť�� */
    fSelCir = fMinCir + wZoomPos*m_fWidthCir/MAXLINEITEM; // ��ǰ���ڷŴ�ͼ���е�����
    fSelCir = fDeltaCir - fSelCir;
    strTemp.Format("(%.4f \xA1\xF1 %.f)", fSelCir, fSetZoomTorq );
    ShowVarPntText(ZOOM_SELCOLOR, x, y, strTemp);

    m_MemDC.SelectObject(pOldPen);

    dcMemory.CreateCompatibleDC(&m_MemDC);

    // Select the bitmap into the in-memory DC
    pOldBitmap = dcMemory.SelectObject(&m_bpWCross);

    m_MemDC.BitBlt(x-5, y-5, m_bmpInfo.bmWidth, m_bmpInfo.bmHeight, &dcMemory,0, 0, SRCCOPY);
    dcMemory.SelectObject(pOldBitmap);
    dcMemory.DeleteDC();
    return;
}
