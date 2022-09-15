// DlgScatter.cpp: ʵ���ļ�
//

#include "stdafx.h"
#include "Torque.h"
#include "DlgScatter.h"
#include "afxdialogex.h"


extern TORQUEDATA  m_tReadData; 
// CDlgScatter �Ի���

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


// CDlgScatter ��Ϣ�������

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
                  // �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgScatter::ResetScatLineByData(TorqData::Torque *ptTorq)
{
    ASSERT_NULL(ptTorq);
    
    CHECK_VALUE_LOW(m_fFullTorq, ptTorq->fmaxlimit());
    CHECK_VALUE_LOW(m_fFullCir, ptTorq->fmaxcir());

    m_wndLineScat.RemoveAt();
    m_wndLineScat.m_fUpperLimit = ptTorq->fupperlimit();    /* ���Ť�� */
    m_wndLineScat.m_fLowerLimit = ptTorq->flowerlimit();    /* ��СŤ�� */
    m_wndLineScat.m_fOptTorq    = theApp.GetOptTorq(ptTorq);/* ���Ť�� */
    m_wndLineScat.m_fSpeedDown  = ptTorq->fspeeddown();     /* ����Ť�� */
    m_wndLineScat.m_fShow = ptTorq->fshow();                /* ��ʾŤ�� */
    m_wndLineScat.m_fBear = ptTorq->fbear();                /* �縺Ť�� */
    m_wndLineScat.m_fControlCir = ptTorq->fcontrolcir();    /* �������� */
    m_wndLineScat.m_fUpperCir   = ptTorq->fuppercir();      /* �������� */
    m_wndLineScat.m_fLowerCir   = ptTorq->flowercir();      /* �������� */
    m_wndLineScat.m_fMaxCir     = m_fFullCir;               /* ������� */
    m_wndLineScat.m_fMaxLimit   = m_fFullTorq;              /* ������� */
    m_wndLineScat.m_fUpperTai   = ptTorq->fuppertai();      /* ���̨�� */
    m_wndLineScat.m_fLowerTai   = ptTorq->flowertai();      /* ��С̨�� */

    m_wndLineScat.SetBkColor(RGB(255, 255, 255));
    m_wndLineScat.m_bBKLine = FALSE;
    m_wndLineScat.Add(RGB(0, 0, 0), m_fFullTorq, 0.0);

    /* �������ÿ̶� */
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
    m_wndLineScat.m_fUpperLimit = ptCtrl->fUpperLimit;     /* ���Ť�� */
    m_wndLineScat.m_fLowerLimit = ptCtrl->fLowerLimit;     /* ��СŤ�� */
    m_wndLineScat.m_fOptTorq    = ptCtrl->fOptTorq;        /* ���Ť�� */
    m_wndLineScat.m_fSpeedDown  = ptCtrl->fSpeedDown;      /* ����Ť�� */
    m_wndLineScat.m_fShow       = ptCtrl->fShow;           /* ��ʾŤ�� */
    m_wndLineScat.m_fBear       = ptCtrl->fBear;           /* �縺Ť�� */
    m_wndLineScat.m_fControlCir = ptCtrl->fControlCir;     /* �������� */
    m_wndLineScat.m_fUpperCir   = ptCtrl->fUpperCir;       /* �������� */
    m_wndLineScat.m_fLowerCir   = ptCtrl->fLowerCir;       /* �������� */
    m_wndLineScat.m_fMaxCir     = ptCtrl->fMaxCir;         /* ������� */
    m_wndLineScat.m_fMaxLimit   = ptCtrl->fMaxLimit;       /* ������� */
    m_wndLineScat.m_fUpperTai   = ptComm->fUpperTai;       /* ���̨�� */
    m_wndLineScat.m_fLowerTai   = ptComm->fLowerTai;       /* ��С̨�� */

    m_wndLineScat.SetBkColor(RGB(255, 255, 255));
    m_wndLineScat.m_bBKLine = FALSE;
    m_wndLineScat.Add(RGB(0, 0, 0), ptCtrl->fMaxLimit, 0.0);

    /* �������ÿ̶� */
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
    HDC         hMemDC      = NULL; // ��Ļ���ڴ��豸������
    HBITMAP     hBitmap     = NULL; // λͼ���
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

    // Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
    hMemDC  = CreateCompatibleDC(hdc);
    // ����һ������Ļ�豸��������ݵ�λͼ
    hBitmap = CreateCompatibleBitmap(hdc, iWidth, iHeight);

    hOldBitmap =(HBITMAP) ::SelectObject(hMemDC,hBitmap);
    RedrawWindow();

    // ����λͼѡ���ڴ��豸��������
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    StretchBlt( hMemDC, 0, 0, iWidth, iHeight,
                hdc,    0, 0, iWidth, iHeight, SRCCOPY );

    // �õ�λͼ�ľ��
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
    
    theApp.SavePNG(hBitmap,strFileName);

    return strFileName;
}
