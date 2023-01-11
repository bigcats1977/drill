// DlgZoomIn.cpp : implementation file
//

#include "stdafx.h"
#include "Drill.h"
#include "DlgZoomIn.h"
#include "afxdialogex.h"


// CDlgZoomIn dialog


/*********************�����************************************/
#define INSERTZOOMPNT(fTorque, iDrawed)         {       \
        if((iDrawed + 1) < MAXLINEITEM)                 \
        {                                               \
            m_wndLineZoom.SetPos(fTorque);              \
            m_wndLineZoom.DrawSpike();                  \
            iDrawed++;                                  \
        }                                               \
    }

IMPLEMENT_DYNAMIC(CDlgZoomIn, CDialogEx)

CDlgZoomIn::CDlgZoomIn(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DLGZOOMIN, pParent)
{
}

CDlgZoomIn::~CDlgZoomIn()
{
}

void CDlgZoomIn::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_XZOOMAXIS, m_xZoomAxis);
    DDX_Control(pDX, IDC_YZOOMAXIS, m_yZoomAxis);
}


BEGIN_MESSAGE_MAP(CDlgZoomIn, CDialogEx)
    //ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_MESSAGE(WM_UPDATE_SELPOS, SelPosChange)
END_MESSAGE_MAP()


// CDlgZoomIn message handlers
BOOL CDlgZoomIn::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_wndLineZoom.SubclassDlgItem(IDC_LINE_ZOOMTORQ, this);
    m_wndLineZoom.SetParent(this);
    m_xZoomAxis.SetRulerType(RT_HORIZONTAL);
    m_xZoomAxis.SetXAxisDecNum(3);
    m_yZoomAxis.SetRulerType(RT_VERTICAL);

    m_iZoomPos = m_nPos;
    m_nCurZoom = g_tGlbCfg.nZoomIn;

    DrawZoomPnt();

    m_bDragTitle = FALSE;

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgZoomIn::DrawZoomPnt()
{
    DRAWTORQDATA* ptDraw = NULL;
    UINT    nStartPnt = 0;

    COMP_BL(g_tReadData.nCur, 1);
    COMP_BG(g_tReadData.nCur, g_tReadData.nTotal);

    ptDraw = theApp.GetDrawDataFromTorq(g_tReadData.nCur - 1, m_nCurZoom);
    ASSERT_NULL(ptDraw);

    GetZoomRange(ptDraw);

    DrawZoomLine(ptDraw);
}

/* ��ȷ��m_wBegin/m_wEnd�����
   V3.22 begin/end�ǳ��˱����� */
void CDlgZoomIn::GetTorqueRange(DRAWTORQDATA* ptDraw)
{
    int     i = 0;
    double  fMin = 500000;
    double  fMax = 0;
    double  fDiff = 0;
    TorqData::Torque* ptTorq = NULL;

    ASSERT_NULL(ptDraw);
    ptTorq = ptDraw->ptOrgTorq;

    for (i = m_iBegin; i <= m_iEnd && i < ptDraw->wCount; i++)
    {
        if (fMin > ptDraw->fTorque[i])
            fMin = ptDraw->fTorque[i];
        if (fMax < ptDraw->fTorque[i])
            fMax = ptDraw->fTorque[i];
    }

    if (fMax - fMin < MINTORQRANGE)
    {
        fDiff = MINTORQRANGE - fMax + fMin;
        fMin -= fDiff / 2;
        fMax += fDiff / 2;

        if (fMin < 0)
        {
            fMax -= fMin;
            fMin = 0;
        }
    }

    m_fMinTorq = (int)fMin;
    m_fMaxTorq = (int)fMax;

}

void CDlgZoomIn::AdjustShowCir(DRAWTORQDATA* ptDraw, double  fSrcMaxCir)
{
    int     i = 0;
    double  fBeginCir = 0;
    double  fDelCir = 0;
    int     iDelCnt = 0;
    int     iAdjDel = 1;

    ASSERT_NULL(ptDraw);

    /* delcir��Ҫ���ٱ��� for 3.22 */
    iAdjDel = m_nCurZoom;

    /* �Ŵ�ͼ������Ϊ�����������һ���пգ���begin������ */
    if (m_iBegin < 0)
    {
        fDelCir = m_iBegin * fSrcMaxCir / iAdjDel / MAXLINEITEM;
        m_fMinCir -= fDelCir;
        m_fMaxCir -= fDelCir;
        m_iEnd -= m_iBegin;
        m_iBegin = 0;
    }
    /* ���һ���Ŵ�ʱ�����End ���� */
    if (m_iEnd >= ptDraw->wCount)
    {
        iDelCnt = m_iEnd - ptDraw->wCount;
        fDelCir = iDelCnt * fSrcMaxCir / iAdjDel / MAXLINEITEM;
        m_fMinCir -= fDelCir;
        m_fMaxCir -= fDelCir;
        m_iBegin -= iDelCnt;
        m_iEnd = ptDraw->wCount - 1;

        if (m_iBegin < 0)  /* ��խͼ��,�Ŵ�ͼ��������У�ֻ��ͼ�ηŴ󣬷�Χû�б仯 */
        {
            m_fMinCir = 0;
            m_fMaxCir = fSrcMaxCir / m_nCurZoom;
            m_iBegin = 0;
        }
    }

    /* ����ֱ�ӷ��� */
    COMP_BLE(m_tSplit.iSplitNum, 1);

    /* �������¼���Ȧ����Χ */
    /* ���㵱ǰŤ�ص�ǰ����Ȧ����Χ */

    fBeginCir = m_tSplit.iEnd[0] * fSrcMaxCir / MAXLINEITEM - fSrcMaxCir
        + fSrcMaxCir * (m_tSplit.iCur - 1);

    m_fMinCir += fBeginCir;
    m_fMaxCir += fBeginCir;
    return;
}

void CDlgZoomIn::GetZoomRange(DRAWTORQDATA* ptDraw)
{
    double  fZoomCir = 0;      // �����λ��
    double  fSrcMaxCir = 0;
    int     iBegin, iEnd;       // �Ŵ�ǰ��λ�ã�m_iBegin/m_iEnd: �Ŵ���λ��
    UINT    nStartPoint = 0;
    TorqData::Torque* ptTorq = NULL;

    SPLITPOINT  tCurSplit;      /* ��ǰ������Ϣ */

    ASSERT_NULL(ptDraw);
    ptTorq = ptDraw->ptOrgTorq;
    tCurSplit = g_tReadData.tSplit[g_tReadData.nCur - 1];

    if (m_nCurZoom <= 0)
        m_nCurZoom = 1;

    fSrcMaxCir = theApp.GetMaxCir(ptTorq);

    fZoomCir = fSrcMaxCir * m_nPos / MAXLINEITEM;
    m_fMinCir = fZoomCir - fSrcMaxCir / m_nCurZoom / 2;
    CHECK_VALUE_LOW(m_fMinCir, 0);
    m_fMaxCir = m_fMinCir + fSrcMaxCir / m_nCurZoom;
    if (m_fMaxCir > fSrcMaxCir)
    {
        m_fMinCir -= m_fMaxCir - fSrcMaxCir;
        m_fMaxCir = fSrcMaxCir;
    }

    theApp.GetShowDataRange(ptDraw, iBegin, iEnd, &m_tSplit, m_nCurZoom);

    /* �����ĵ�һ��ʱ�����һ�ͼ */
    m_iZoomPos = m_nPos + iBegin;
    if (m_tSplit.iSplitNum > 1)
    {
        if (iBegin == 0)
        {
            nStartPoint = MAXLINEITEM - iEnd;
            m_iZoomPos = m_nPos - nStartPoint;
        }
        else
        {
            //nStartPoint = 0;
        }
    }

    m_iBegin = (UINT)(m_fMinCir / fSrcMaxCir * MAXLINEITEM) - nStartPoint + iBegin;
    /* 20200312 ����Ŵ�ʱ���Ŵ�LineCtrlEx���ĵ㳬��500��m_nPosΪ0���Ŵ�ͼ���޷������� */
    m_iEnd = (UINT)(m_fMaxCir / fSrcMaxCir * MAXLINEITEM) - nStartPoint + iBegin - 1;

    m_iZoomPos *= m_nCurZoom;
    m_iBegin *= m_nCurZoom;
    m_iEnd *= m_nCurZoom;
    m_iEnd += m_nCurZoom - 1;

    AdjustShowCir(ptDraw, fSrcMaxCir);

    GetTorqueRange(ptDraw);
    m_fMaxTorq += 500;

    m_xZoomAxis.SetTickPara(10, m_fMaxCir, m_fMinCir);
    m_yZoomAxis.SetTickPara(20, m_fMaxTorq, m_fMinTorq);

    m_wndLineZoom.m_fMaxCir = m_fMaxCir;
    m_wndLineZoom.m_fWidthCir = m_fMaxCir - m_fMinCir;

    return;
}

void CDlgZoomIn::DrawZoomLine(DRAWTORQDATA* ptDraw)
{
    int     i = 0;
    UINT    j = 0;
    UINT    k = 0;
    double  fPreTorq = 0;
    double  fCurTorq = 0;
    double  fInsTorq = 0;
    double  fDeltaCir = 0;
    double  fmaxcir = 0;
    COLORREF clrZoom = RGB(0, 0, 255);
    double  fMinDiff = m_nZoomTorq;
    int     iZoomPos = 0; // ��ǰ��ͼ��ʵ��λ��
    UINT    iDrawed = 0;
    int     iBegin = 0, iEnd = 0;

    ASSERT_NULL(ptDraw);

    m_wndLineZoom.RemoveAt();
    m_wndLineZoom.SetBkColor(RGB(255, 255, 255));
    m_wndLineZoom.m_bBKLine = TRUE;
    m_wndLineZoom.Add(clrZoom, m_fMaxTorq, m_fMinTorq, LINETYPE_ZOOM);

    m_wndLineZoom.DrawZoomBkLine();

    /* ����һ���� */
    fPreTorq = ptDraw->fTorque[m_iBegin];
    m_wndLineZoom.SetPos(fPreTorq);
    m_wndLineZoom.DrawSpike();

    for (i = m_iBegin + 1; i < m_iEnd && i < ptDraw->wCount; i++)
    {
        fCurTorq = ptDraw->fTorque[i];
        m_wndLineZoom.SetPos(fCurTorq);
        m_wndLineZoom.DrawSpike();
        if (i == m_iZoomPos)
        {
            iZoomPos = m_wndLineZoom.GetCurPoints();
        }
    }

    /* �Ŵ�ͼ��������Ť�أ������� */
    /*���һ������������GO���������˫�ߵ����*/
    if (m_iEnd >= ptDraw->wCount - 1)
    {
        m_wndLineZoom.SetPos(ptDraw->fTorque[ptDraw->wCount - 1], TRUE);
        m_wndLineZoom.Go();
    }
    else
    {
        m_wndLineZoom.SetPos(ptDraw->fTorque[m_iEnd]);
        m_wndLineZoom.Go();
    }

    theApp.GetShowDataRange(ptDraw, iBegin, iEnd, &m_tSplit);
    fmaxcir = theApp.GetMaxCir(ptDraw->ptOrgTorq);
    fDeltaCir = theApp.GetCir(ptDraw->ptOrgTorq);

    m_nZoomTorq = (UINT)ptDraw->fTorque[m_iZoomPos];
    m_wndLineZoom.DrawZoomInfo(iZoomPos, m_fMinCir, fmaxcir, fDeltaCir, m_nZoomTorq);
}

/* ������� wParam :True ��ʾ/ false: ����ʾ */
LRESULT CDlgZoomIn::SelPosChange(WPARAM wParam, LPARAM lParam)
{
    DrawZoomPnt();

    Invalidate(TRUE);

    return 0;
}

void CDlgZoomIn::OnRButtonDown(UINT nFlags, CPoint point)
{
    m_bDragTitle = TRUE;
    m_pointDown = point;
    SetCapture();
}
void CDlgZoomIn::OnRButtonUp(UINT nFlags, CPoint point)
{
    if (!m_bDragTitle)
    {
        return;
    }

    m_bDragTitle = FALSE;
    ReleaseCapture();
}
void CDlgZoomIn::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bDragTitle)
    {
        return;
    }

    int offleft = point.x - m_pointDown.x;
    int offtop = point.y - m_pointDown.y;

    CRect wrc;
    GetWindowRect(&wrc);

    wrc.OffsetRect(offleft, offtop);
    MoveWindow(&wrc, TRUE);
}
