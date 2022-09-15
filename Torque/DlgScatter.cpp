// DlgScatter.cpp: 实现文件
//

#include "stdafx.h"
#include "Torque.h"
#include "DlgScatter.h"
#include "afxdialogex.h"


extern TORQUEDATA  m_tReadData; 
// CDlgScatter 对话框

IMPLEMENT_DYNAMIC(CDlgScatter, CDialogEx)

CDlgScatter::CDlgScatter(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DLGSCATTER, pParent)
{

}

CDlgScatter::~CDlgScatter()
{
}

void CDlgScatter::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_YSCATAXIS, m_yScatAxis);
    DDX_Control(pDX, IDC_XSCATAXIS, m_xScatAxis);
}


BEGIN_MESSAGE_MAP(CDlgScatter, CDialogEx)
END_MESSAGE_MAP()


// CDlgScatter 消息处理程序

BOOL CDlgScatter::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_wndLineScat.m_bScatter = TRUE;
    m_wndLineScat.SubclassDlgItem(IDC_LINE_SCATTER, this);
    m_wndLineScat.SetParent(this);
    m_xScatAxis.SetRulerType(RT_HORIZONTAL);
    m_yScatAxis.SetRulerType(RT_VERTICAL);

    m_fFullCir = 0;
    m_fFullTorq= 0;
    
    DrawScatterPlot();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CDlgScatter::ResetScatLineByData(TorqData::Torque *ptTorq)
{
    ASSERT_NULL(ptTorq);
    
    CHECK_VALUE_LOW(m_fFullTorq, ptTorq->fmaxlimit());
    CHECK_VALUE_LOW(m_fFullCir, ptTorq->fmaxcir());

    m_wndLineScat.RemoveAt();
    m_wndLineScat.m_fUpperLimit = ptTorq->fupperlimit();    /* 最大扭矩 */
    m_wndLineScat.m_fLowerLimit = ptTorq->flowerlimit();    /* 最小扭矩 */
    m_wndLineScat.m_fOptTorq    = theApp.GetOptTorq(ptTorq);/* 最佳扭矩 */
    m_wndLineScat.m_fSpeedDown  = ptTorq->fspeeddown();     /* 减速扭矩 */
    m_wndLineScat.m_fShow = ptTorq->fshow();                /* 显示扭矩 */
    m_wndLineScat.m_fBear = ptTorq->fbear();                /* 肩负扭矩 */
    m_wndLineScat.m_fControlCir = ptTorq->fcontrolcir();    /* 控制周数 */
    m_wndLineScat.m_fUpperCir   = ptTorq->fuppercir();      /* 上限周数 */
    m_wndLineScat.m_fLowerCir   = ptTorq->flowercir();      /* 下限周数 */
    m_wndLineScat.m_fMaxCir     = m_fFullCir;               /* 最大周数 */
    m_wndLineScat.m_fMaxLimit   = m_fFullTorq;              /* 最大上限 */
    m_wndLineScat.m_fUpperTai   = ptTorq->fuppertai();      /* 最大台阶 */
    m_wndLineScat.m_fLowerTai   = ptTorq->flowertai();      /* 最小台阶 */

    m_wndLineScat.SetBkColor(RGB(255, 255, 255));
    m_wndLineScat.m_bBKLine = FALSE;
    m_wndLineScat.Add(RGB(0, 0, 0), m_fFullTorq, 0.0);

    /* 重新设置刻度 */
    m_xScatAxis.SetTickPara(10, m_fFullCir);
    m_yScatAxis.SetTickPara(20, m_fFullTorq);
    m_wndLineScat.DrawBkLine();
}

void CDlgScatter::ResetScatLine(PARACFG *ptCfg)
{
    CONTROLPARA *ptCtrl = NULL;
    COMMONCFG   *ptComm = NULL;

    ASSERT_NULL(ptCfg);

    ptCtrl = &ptCfg->tCtrl;
    ptComm = &ptCfg->tComm;

    m_wndLineScat.RemoveAt();
    m_wndLineScat.m_fUpperLimit = ptCtrl->fUpperLimit;     /* 最大扭矩 */
    m_wndLineScat.m_fLowerLimit = ptCtrl->fLowerLimit;     /* 最小扭矩 */
    m_wndLineScat.m_fOptTorq    = ptCtrl->fOptTorq;        /* 最佳扭矩 */
    m_wndLineScat.m_fSpeedDown  = ptCtrl->fSpeedDown;      /* 减速扭矩 */
    m_wndLineScat.m_fShow       = ptCtrl->fShow;           /* 显示扭矩 */
    m_wndLineScat.m_fBear       = ptCtrl->fBear;           /* 肩负扭矩 */
    m_wndLineScat.m_fControlCir = ptCtrl->fControlCir;     /* 控制周数 */
    m_wndLineScat.m_fUpperCir   = ptCtrl->fUpperCir;       /* 上限周数 */
    m_wndLineScat.m_fLowerCir   = ptCtrl->fLowerCir;       /* 下限周数 */
    m_wndLineScat.m_fMaxCir     = ptCtrl->fMaxCir;         /* 最大周数 */
    m_wndLineScat.m_fMaxLimit   = ptCtrl->fMaxLimit;       /* 最大上限 */
    m_wndLineScat.m_fUpperTai   = ptComm->fUpperTai;       /* 最大台阶 */
    m_wndLineScat.m_fLowerTai   = ptComm->fLowerTai;       /* 最小台阶 */

    m_wndLineScat.SetBkColor(RGB(255, 255, 255));
    m_wndLineScat.m_bBKLine = FALSE;
    m_wndLineScat.Add(RGB(0, 0, 0), ptCtrl->fMaxLimit, 0.0);

    /* 重新设置刻度 */
    m_xScatAxis.SetTickPara(10, ptCtrl->fMaxCir);
    m_yScatAxis.SetTickPara(20, ptCtrl->fMaxLimit);
    m_wndLineScat.DrawBkLine();
}

void CDlgScatter::DrawScatterPlot(void)
{
    UINT    i       = 0;
    double  fTorque = 0;
    double  fCir    = 0;
    TorqData::Torque  *ptTorq = NULL;
    
    for(i=0; i<m_tReadData.nTotal; i++)
    {
        ptTorq = theApp.GetOrgTorqFromTorq(i);
        if(NULL== ptTorq)
            continue;

        GET_CTRL_TORQ(fTorque, ptTorq);
        if(fTorque > OVERFLOWTORQ)
            continue;
        CHECK_VALUE_LOW(m_fFullTorq, fTorque);

        fCir = theApp.GetCir(ptTorq);
        CHECK_VALUE_LOW(m_fFullCir, fCir);
    }
    ResetScatLineByData(&m_tReadData.tData[m_tReadData.nTotal-1]);

    m_wndLineScat.DrawMultiScatter(&m_tReadData);

    UpdateData(FALSE);
}

CString CDlgScatter::SaveScatterImg(void)
{
    CPaintDC    dc(this);
    HDC         hdc;
    HDC         hMemDC      = NULL; // 屏幕和内存设备描述表
    HBITMAP     hBitmap     = NULL; // 位图句柄
    HBITMAP     hOldBitmap  = NULL; 
    CRect       rcClt;
    CString     strFileName;
    int         iWidth      = 0;
    int         iHeight     = 0;
    
    strFileName  = theApp.GetSaveDataPath();
    strFileName += "Scatter.png";

    GetClientRect(&rcClt); 
    iWidth  = rcClt.Width();
    iHeight = rcClt.Height();
    hdc = dc.m_hDC;

    // 为屏幕设备描述表创建兼容的内存设备描述表
    hMemDC  = CreateCompatibleDC(hdc);
    // 创建一个与屏幕设备描述表兼容的位图
    hBitmap = CreateCompatibleBitmap(hdc, iWidth, iHeight);

    hOldBitmap =(HBITMAP) ::SelectObject(hMemDC,hBitmap);
    RedrawWindow();

    // 把新位图选到内存设备描述表中
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    StretchBlt( hMemDC, 0, 0, iWidth, iHeight,
                hdc,    0, 0, iWidth, iHeight, SRCCOPY );

    // 得到位图的句柄
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
    
    theApp.SavePNG(hBitmap,strFileName);

    return strFileName;
}
