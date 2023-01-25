// Ruler.cpp : implementation file
//

#include "stdafx.h"
#include "Ruler.h"
#include "Drill.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRuler

CRuler::CRuler()
{
    //CString str;

    m_nRulerType = RT_HORIZONTAL;
    m_iTickNum = 10;
    m_fMinTickVal = 0;
    m_fTickValue = 5;
    m_nXDecNum = 2;
#if 0
    str.Format(IDS_STRFONTDEAAULT);
    m_hFont.CreateFont(12, 6, 000, 000, FW_NORMAL, 0, 0, 0, GB2312_CHARSET,
        OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
        VARIABLE_PITCH | FF_ROMAN, str);
    m_vFont.CreateFont(10, 5, 000, 000, FW_NORMAL, 0, 0, 0, GB2312_CHARSET,
        OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
        VARIABLE_PITCH | FF_ROMAN, str);
#endif
}

CRuler::~CRuler()
{
    //DeleteObject(m_hFont);
    //DeleteObject(m_vFont);
}


BEGIN_MESSAGE_MAP(CRuler, CStatic)
    //{{AFX_MSG_MAP(CRuler)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRuler message handlers


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CRuler::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    CRect    rcClient;

    GetClientRect(&rcClient);

    // draw scale
    COMP_BE(m_MemDC.GetSafeHdc(), NULL);

    dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &m_MemDC, 0, 0, SRCCOPY);
}


void CRuler::SetRulerType(UINT nRulerType)
{
    m_nRulerType = nRulerType;
}

void CRuler::SetTickPara(int iTickNum, double fMaxVal, double fMinVal)
{
    m_iTickNum = iTickNum;
    if (m_iTickNum <= 0)
        m_iTickNum = 10;
    m_fMinTickVal = fMinVal;
    m_fTickValue = fMaxVal;
    InvalidateCtrl();
}

void CRuler::SetXAxisDecNum(UINT nNum)
{
    m_nXDecNum = nNum;
}

void CRuler::InvalidateCtrl()
{
    CClientDC   dc(this);
    CRect       rcClient;

    GetClientRect(&rcClient);

    if (m_MemDC.GetSafeHdc() == NULL)
    {
        m_MemDC.CreateCompatibleDC(&dc);
        m_Bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
        m_MemDC.SelectObject(m_Bitmap);
    }

    if (m_nRulerType == RT_VERTICAL)
    {
        DrawVerRuler();
    }
    else
    {
        DrawHorRuler();
    }

    InvalidateRect(rcClient, FALSE);
}

/*lint -e414*/
/* 画竖尺子 */
void CRuler::DrawVerRuler()
{
    int     x = 0, y = 0;
    int     i = 0;
    int     iOldAlign = 0;
    int     iOldBkMode = 0;
    string  str;
    CRect   rcRuler;
    // drawing tools
    CFont* pOldFont = NULL;
    CPen    wtPen(PS_SOLID, 1, GetSysColor(COLOR_WINDOWTEXT));
    CPen    wbkPen(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
    CPen* pOldPen = m_MemDC.SelectObject(&wbkPen);
    CBrush  wbkBr(GetSysColor(COLOR_BTNFACE));
    CBrush* pOldBrush = m_MemDC.SelectObject(&wbkBr);

    iOldAlign = m_MemDC.SetTextAlign(TA_RIGHT | TA_TOP);
    iOldBkMode = m_MemDC.SetBkMode(TRANSPARENT);

    GetClientRect(&rcRuler);

    m_MemDC.Rectangle(rcRuler);

    // draw the ruler margin
    m_MemDC.SelectObject(&wtPen);
    pOldFont = m_MemDC.SelectObject(&theApp.m_tRuleVFont);

    for (i = 0; i <= m_iTickNum; i++)
    {
        str = string_format("%d", (int)((m_fTickValue - m_fMinTickVal) * i / m_iTickNum + m_fMinTickVal));
        y = rcRuler.Height() * (m_iTickNum - i) / m_iTickNum;
        if (i == 0)
        {
            /* 画大刻度 */
            m_MemDC.MoveTo(rcRuler.right, y - 1);
            m_MemDC.LineTo(rcRuler.right - MAX_SCALE_LEN, y - 1);
            str = string_format("%d", (int)m_fMinTickVal);
            m_MemDC.TextOut(rcRuler.right - 6, y - 9, str.c_str());
            DrawVerScale(y, rcRuler);
            continue;
        }
        if (i == m_iTickNum)
        {
            /* 画大刻度 */
            m_MemDC.MoveTo(rcRuler.right, y);
            m_MemDC.LineTo(rcRuler.right - MAX_SCALE_LEN, y);
            m_MemDC.TextOut(rcRuler.right - 6, y, str.c_str());
            continue;
        }

        /* else */

        /* 画大刻度 */
        m_MemDC.MoveTo(rcRuler.right, y);
        m_MemDC.LineTo(rcRuler.right - MAX_SCALE_LEN, y);
        m_MemDC.TextOut(rcRuler.right - 6, y - 5, str.c_str());
        DrawVerScale(y, rcRuler);
    }

    m_MemDC.SelectObject(pOldPen);
    m_MemDC.SelectObject(pOldBrush);
    m_MemDC.SelectObject(pOldFont);
    m_MemDC.SetTextAlign(iOldAlign);
    m_MemDC.SetBkMode(iOldBkMode);
}

/* 画尺子的小竖刻度 */
void CRuler::DrawVerScale(int iBeginY, CRect rcRuler)
{
    int     i = 0;
    double  fScaleHeight = 0;
    int     iScaleY = 0;

    fScaleHeight = rcRuler.Height() / (m_iTickNum * SCALE_NUM * 1.0);

    for (i = 1; i <= SCALE_NUM; i++)
    {
        iScaleY = int(iBeginY - i * fScaleHeight);

        /* 画小刻度 */
        m_MemDC.MoveTo(rcRuler.right, iScaleY);
        m_MemDC.LineTo(rcRuler.right - Min_SCALE_LEN, iScaleY);
    }
}

/* 画横尺子 */
void CRuler::DrawHorRuler()
{
    int     x = 0, y = 0;
    int     i = 0;
    int     j = 0;
    int     iOldAlign = 0;
    int     iOldBkMode = 0;
    int     iZeroIndex = -1;
    int     iZeroPlace = 0;
    double  fXValue = 0;
    double  fStep = 0;
    int     iStrPlace = 0;
    string  str;
    CRect   rcRuler;
    // drawing tools
    CFont* pOldFont = NULL;
    CPen    wtPen(PS_SOLID, 1, GetSysColor(COLOR_WINDOWTEXT));
    CPen    wbkPen(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
    CPen* pOldPen = m_MemDC.SelectObject(&wbkPen);
    CBrush  wbkBr(GetSysColor(COLOR_BTNFACE));
    CBrush* pOldBrush = m_MemDC.SelectObject(&wbkBr);
    string  strHFormat = "%.2f";

    iOldAlign = m_MemDC.SetTextAlign(TA_RIGHT | TA_TOP);
    iOldBkMode = m_MemDC.SetBkMode(TRANSPARENT);

    GetClientRect(&rcRuler);

    m_MemDC.Rectangle(rcRuler);

    // draw the ruler margin
    m_MemDC.SelectObject(&wtPen);
    pOldFont = m_MemDC.SelectObject(&theApp.m_tRuleHFont);

    strHFormat = string_format("%%.%df", m_nXDecNum);

    fStep = (m_fTickValue - m_fMinTickVal) / m_iTickNum;
    if (m_fMinTickVal < 0)
    {
        iZeroIndex = (int)(fabs(m_fMinTickVal / fStep));
        double fPrior = iZeroIndex * fStep + m_fMinTickVal;
        iZeroPlace = (int)((iZeroIndex + fabs(fPrior) / fStep) * rcRuler.Width() / m_iTickNum);
        fXValue = fPrior + fStep;
        if (fabs(fPrior) > fXValue)
        {
            iZeroIndex += 1;
        }
    }

    for (i = 0; i <= m_iTickNum; i++)
    {
        fXValue = i * fStep + m_fMinTickVal;
        str = string_format(strHFormat.c_str(), fXValue);
        x = i * rcRuler.Width() / m_iTickNum;
        iStrPlace = x + 10;
        if (i == 0)
        {
            //str.Format(strHFormat, m_fMinTickVal);
            if (m_fMinTickVal < 0)
                iStrPlace = x + 25;
            else
                iStrPlace = x + 22;
        }
        if (i == m_iTickNum)
            iStrPlace = x;

        /* 起始点为负值时，iZeroIndex大于0，重新标定0点 */
        if (i == iZeroIndex)
        {
            str = _T("0");
            iStrPlace = iZeroPlace + 5;
        }
        m_MemDC.TextOut(iStrPlace, rcRuler.top + 2, "      ");
        m_MemDC.TextOut(iStrPlace, rcRuler.top + 2, str.c_str());

        /* 最后一个刻度不需要画小刻度 */
        if (i == m_iTickNum)
        {
            m_MemDC.MoveTo(x - 1, rcRuler.top);
            m_MemDC.LineTo(x - 1, rcRuler.top + MAX_SCALE_LEN);
            continue;
        }

        /* 画大刻度 */
        m_MemDC.MoveTo(x, rcRuler.top);
        m_MemDC.LineTo(x, rcRuler.top + MAX_SCALE_LEN);
        /* 画带小刻度的坐标线 */
        DrawHorScale(x, rcRuler);
    }

    m_MemDC.SelectObject(pOldPen);
    m_MemDC.SelectObject(pOldBrush);
    m_MemDC.SelectObject(pOldFont);
    m_MemDC.SetTextAlign(iOldAlign);
    m_MemDC.SetBkMode(iOldBkMode);
}/*lint +e414*/

/* 画尺子的小横刻度 */
void CRuler::DrawHorScale(int iBeginX, CRect rcRuler)
{
    int     i = 0;
    double  fScaleWidth = 0;
    int     iScaleX = 0;

    fScaleWidth = rcRuler.Width() / (m_iTickNum * SCALE_NUM * 1.0);

    /* 画大刻度 */
    m_MemDC.MoveTo(iBeginX, rcRuler.top);
    m_MemDC.LineTo(iBeginX, rcRuler.top + MAX_SCALE_LEN);

    for (i = 1; i <= SCALE_NUM; i++)
    {
        iScaleX = int(iBeginX + i * fScaleWidth);

        /* 画小刻度 */
        m_MemDC.MoveTo(iScaleX, rcRuler.top);
        m_MemDC.LineTo(iScaleX, rcRuler.top + Min_SCALE_LEN);
    }
}