// DlgStatSet.cpp : implementation file
//

#include "stdafx.h"
#include "Drill.h"
#include "DlgStatSet.h"
#include "afxdialogex.h"


// CDlgStatSet dialog

IMPLEMENT_DYNAMIC(CDlgStatSet, CDialogEx)

CDlgStatSet::CDlgStatSet(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DLGSTATPARA, pParent)
{

}

CDlgStatSet::~CDlgStatSet()
{
}

void CDlgStatSet::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDCTRLZERO, m_fCtrlZero);
    DDX_Text(pDX, IDC_EDCTRLUPPER, m_fCtrlMax);
    DDX_Text(pDX, IDC_EDCTRLMIN, m_fCtrlRange[0]);
    DDX_Text(pDX, IDC_EDCTRLRANG1, m_fCtrlRange[1]);
    DDX_Text(pDX, IDC_EDCTRLMAX, m_fCtrlRange[2]);
    //DDX_Text(pDX, IDC_EDSHOULDZERO, m_fShouldZero);
    //DDX_Text(pDX, IDC_EDSHOULDUPPER, m_fShouldMax);
    //DDX_Text(pDX, IDC_EDSHOULDMIN, m_fShouldRange[0]);
    //DDX_Text(pDX, IDC_EDSHOULDRANG1, m_fShouldRange[1]);
    //DDX_Text(pDX, IDC_EDSHOULDMAX, m_fShouldRange[2]);
    //DDX_Text(pDX, IDC_EDDELTAZERO, m_fDeltaZero);
    //DDX_Text(pDX, IDC_EDDELTAUPPER, m_fDeltaMax);
    //DDX_Text(pDX, IDC_EDDELTAMIN, m_fDeltaRange[0]);
    //DDX_Text(pDX, IDC_EDDELTARANG1, m_fDeltaRange[1]);
    //DDX_Text(pDX, IDC_EDDELTAMAX, m_fDeltaRange[2]);
}


BEGIN_MESSAGE_MAP(CDlgStatSet, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDlgStatSet::OnBnClickedOk)
    ON_EN_KILLFOCUS(IDC_EDCTRLMIN, &CDlgStatSet::OnEnKillfocusEdctrlmin)
    ON_EN_KILLFOCUS(IDC_EDCTRLRANG1, &CDlgStatSet::OnEnKillfocusEdctrlrang1)
    ON_EN_KILLFOCUS(IDC_EDCTRLMAX, &CDlgStatSet::OnEnKillfocusEdctrlmax)
    //ON_EN_KILLFOCUS(IDC_EDDELTAMIN, &CDlgStatSet::OnEnKillfocusEddeltamin)
    //ON_EN_KILLFOCUS(IDC_EDDELTARANG1, &CDlgStatSet::OnEnKillfocusEddeltarang1)
    //ON_EN_KILLFOCUS(IDC_EDDELTAMAX, &CDlgStatSet::OnEnKillfocusEddeltamax)
    //ON_EN_KILLFOCUS(IDC_EDSHOULDMIN, &CDlgStatSet::OnEnKillfocusEdshouldmin)
    //ON_EN_KILLFOCUS(IDC_EDSHOULDRANG1, &CDlgStatSet::OnEnKillfocusEdshouldrang1)
    //ON_EN_KILLFOCUS(IDC_EDSHOULDMAX, &CDlgStatSet::OnEnKillfocusEdshouldmax)
END_MESSAGE_MAP()


// CDlgStatSet message handlers
void CDlgStatSet::InitCtrl()
{
    int     i = 0;

    m_fCtrlZero = 0;

    for (i = 0; i < STATRANGENUM; i++)
    {
        m_fCtrlRange[i] = m_tempStat.fCtrlRange[i];
    }
}
#if 0
void CDlgStatSet::InitShould()
{
    int     i = 0;

    m_fShouldZero = 0;

    for (i = 0; i < STATRANGENUM; i++)
    {
        m_fShouldRange[i] = m_tempStat.fShouldRange[i];
    }
}

void CDlgStatSet::InitDelta()
{
    int     i = 0;

    m_fDeltaZero = 0;

    for (i = 0; i < STATRANGENUM; i++)
    {
        m_fDeltaRange[i] = m_tempStat.fDeltaRange[i];
    }
}
#endif
BOOL CDlgStatSet::OnInitDialog()
{
    CDialog::OnInitDialog();

    InitCtrl();
    //InitShould();
    //InitDelta();

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CDlgStatSet::OnBnClickedOk()
{
    int     i = 0;

    UpdateData(TRUE);

    for (i = 0; i < STATRANGENUM; i++)
    {
        m_tempStat.fCtrlRange[i] = m_fCtrlRange[i];
        //m_tempStat.fShouldRange[i] = m_fShouldRange[i];
        //m_tempStat.fDeltaRange[i]  = m_fDeltaRange[i];
    }

    CDialog::OnOK();
}

void CDlgStatSet::OnEnKillfocusEdctrlmin()
{
    double fOld = m_fCtrlRange[0];

    UpdateData(TRUE);
    if (m_fCtrlRange[0] < m_fCtrlZero || m_fCtrlRange[0] > m_fCtrlRange[1])
    {
        m_fCtrlRange[0] = fOld;
        UpdateData(FALSE);
        return;
    }
}

void CDlgStatSet::OnEnKillfocusEdctrlrang1()
{
    double fOld = m_fCtrlRange[1];

    UpdateData(TRUE);
    if (m_fCtrlRange[1] < m_fCtrlRange[0] || m_fCtrlRange[1] > m_fCtrlRange[2])
    {
        m_fCtrlRange[1] = fOld;
        UpdateData(FALSE);
        return;
    }
}

void CDlgStatSet::OnEnKillfocusEdctrlmax()
{
    double fOld = m_fCtrlRange[2];

    UpdateData(TRUE);
    if (m_fCtrlRange[2] < m_fCtrlRange[1] || m_fCtrlRange[2] > m_fCtrlMax)
    {
        m_fCtrlRange[2] = fOld;
        UpdateData(FALSE);
        return;
    }
}
#if 0
void CDlgStatSet::OnEnKillfocusEdshouldmin()
{
    double fOld = m_fShouldRange[0];

    UpdateData(TRUE);
    if (m_fShouldRange[0] < m_fShouldZero || m_fShouldRange[0] > m_fShouldRange[1])
    {
        m_fShouldRange[0] = fOld;
        UpdateData(FALSE);
        return;
    }
}

void CDlgStatSet::OnEnKillfocusEdshouldrang1()
{
    double fOld = m_fShouldRange[1];

    UpdateData(TRUE);
    if (m_fShouldRange[1] < m_fShouldRange[0] || m_fShouldRange[1] > m_fShouldRange[2])
    {
        m_fShouldRange[1] = fOld;
        UpdateData(FALSE);
        return;
    }
}

void CDlgStatSet::OnEnKillfocusEdshouldmax()
{
    double fOld = m_fShouldRange[2];

    UpdateData(TRUE);
    if (m_fShouldRange[2] < m_fShouldRange[1] || m_fShouldRange[2] > m_fShouldMax)
    {
        m_fShouldRange[2] = fOld;
        UpdateData(FALSE);
        return;
    }
}

void CDlgStatSet::OnEnKillfocusEddeltamin()
{
    double fOld = m_fDeltaRange[0];

    UpdateData(TRUE);
    if (m_fDeltaRange[0] < m_fDeltaZero || m_fDeltaRange[0] > m_fDeltaRange[1])
    {
        m_fDeltaRange[0] = fOld;
        UpdateData(FALSE);
        return;
    }
}

void CDlgStatSet::OnEnKillfocusEddeltarang1()
{
    double fOld = m_fDeltaRange[1];

    UpdateData(TRUE);
    if (m_fDeltaRange[1] < m_fDeltaRange[0] || m_fDeltaRange[1] > m_fDeltaRange[2])
    {
        m_fDeltaRange[1] = fOld;
        UpdateData(FALSE);
        return;
    }
}

void CDlgStatSet::OnEnKillfocusEddeltamax()
{
    double fOld = m_fDeltaRange[2];

    UpdateData(TRUE);
    if (m_fDeltaRange[2] < m_fDeltaRange[1] || m_fDeltaRange[2] > m_fDeltaMax)
    {
        m_fDeltaRange[2] = fOld;
        UpdateData(FALSE);
        return;
    }
}
#endif
