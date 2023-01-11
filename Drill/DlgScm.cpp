// DlgScm.cpp : implementation file
//

#include "stdafx.h"
#include "Drill.h"
#include "DrillDlg.h"
#include "DlgScm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//CDrillDlg* g_ptScmParentDlg;
/////////////////////////////////////////////////////////////////////////////
// CDlgScm dialog


CDlgScm::CDlgScm(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgScm::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgScm)
    m_fMulti = 0.0;
    m_fControl = 0.0;
    m_fShow = 0.0;
    m_fSpeed = 0.0;
    //m_fLower    = 0.0;
    //m_fUpper    = 0.0;
    m_tDate = 0;
    m_tTime = 0;
    //}}AFX_DATA_INIT
}


void CDlgScm::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgScm)
    DDX_Control(pDX, IDC_SCMTIME, m_btnTime);
    DDX_Control(pDX, IDCANCEL, m_btnScmCancel);
    DDX_Control(pDX, IDC_SCMQIPI, m_btnQipi);
    DDX_Control(pDX, IDC_SCMINITIAL, m_btnInitial);
    DDX_Control(pDX, IDC_SCMMULTI, m_btnMulti);
    DDX_Text(pDX, IDC_EDITMULTI, m_fMulti);
    DDV_MinMaxDouble(pDX, m_fMulti, 0., 2.);
    DDX_Text(pDX, IDC_EDITSCMCONTROL, m_fControl);
    DDX_Text(pDX, IDC_EDITSCMSHOW, m_fShow);
    DDX_Text(pDX, IDC_EDITSCMSPEED, m_fSpeed);
    //DDX_Text(pDX, IDC_EDITSCMLOWER, m_fLower);
    //DDX_Text(pDX, IDC_EDITSCMUPPER, m_fUpper);
    DDX_DateTimeCtrl(pDX, IDC_DTPDATE, m_tDate);
    DDX_DateTimeCtrl(pDX, IDC_DTPTIME, m_tTime);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgScm, CDialog)
    //{{AFX_MSG_MAP(CDlgScm)
    ON_BN_CLICKED(IDC_SCMINITIAL, OnScminitial)
    ON_BN_CLICKED(IDC_SCMMULTI, OnScmmulti)
    ON_BN_CLICKED(IDC_SCMQIPI, OnScmqipi)
    ON_BN_CLICKED(IDC_SCMTIME, OnScmtime)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgScm message handlers

void CDlgScm::OnScminitial()
{
    BOOL        bChanged = FALSE;
    CONTROLPARA* ptCtrl = NULL;

    JUDGE_REG_STATUS();
    ptCtrl = &m_ptParaCfg->tCtrl;

    UpdateData(TRUE);

    BeginWaitCursor();
    CHECK_PARA_CHANGE(ptCtrl->fTorqConf[INDEX_TORQ_CONTROL], m_fControl, bChanged);
    //CHECK_PARA_CHANGE(ptCtrl->fTorqConf[INDEX_TORQ_SPEEDDOWN], m_fSpeed,   bChanged);
    CHECK_PARA_CHANGE(ptCtrl->fTorqConf[INDEX_TORQ_SHOW], m_fShow, bChanged);
    //CHECK_PARA_CHANGE(ptCtrl->fTorqConf[INDEX_TORQ_UPPERLIMIT],m_fUpper,   bChanged);
    //CHECK_PARA_CHANGE(ptCtrl->fTorqConf[INDEX_TORQ_LOWERLIMIT],m_fLower,   bChanged);

    if (bChanged)
    {
        /*theApp.WritePara(theApp.m_strParaFile,m_ptParaCfg);
        m_pParentDlg->ResetLineChart();*/
    }

    thepDlg->SendAllCommand();
    EndWaitCursor();
}

void CDlgScm::OnScmmulti()
{
    JUDGE_REG_STATUS();

    UpdateData(TRUE);

    BeginWaitCursor();
    if (g_tGlbCfg.fMulti != m_fMulti)
    {
        g_tGlbCfg.fMulti = m_fMulti;
        theDB.UpdateGlobalPara();
    }

    thepDlg->SendData(SCMADJUST);
    EndWaitCursor();
}

void CDlgScm::OnScmqipi()
{
    JUDGE_REG_STATUS();

    BeginWaitCursor();
    thepDlg->SendData(SCMQIPI);
    EndWaitCursor();
}

BOOL CDlgScm::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_btnInitial.SetIconAndText(IDI_INITIAL, IDS_STRSCMINIT);
    m_btnMulti.SetIconAndText(IDI_ADJUST, IDS_STRSCMMULTI);
    m_btnQipi.SetIconAndText(IDI_QIPI, IDS_STRSCMQIPI);
    m_btnScmCancel.SetIconAndText(IDI_POWER, IDS_STRRETURN);
    m_btnTime.SetIconAndText(IDI_TIMER, IDS_STRSCMTIME);

    m_ptParaCfg = &theApp.m_tParaCfg;

    m_fMulti = 1.0/*m_ptParaCfg->fMulti*/;
    m_fControl = m_ptParaCfg->tCtrl.fTorqConf[INDEX_TORQ_CONTROL];
    m_fSpeed = m_ptParaCfg->tCtrl.fTorqConf[INDEX_TORQ_OPTIMAL] * RATIO_OPTSHOULD;
    m_fShow = m_ptParaCfg->tCtrl.fTorqConf[INDEX_TORQ_SHOW];
    //m_fUpper     = m_ptParaCfg->tCtrl.fTorqConf[INDEX_TORQ_UPPERLIMIT];
    //m_fLower     = m_ptParaCfg->tCtrl.fTorqConf[INDEX_TORQ_LOWERLIMIT];

    m_tDate = CTime::GetCurrentTime();
    m_tTime = m_tDate;

    //g_ptScmParentDlg = (CDrillDlg*)GetParent();

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgScm::OnScmtime()
{
    JUDGE_REG_STATUS();

    UpdateData(TRUE);

    BeginWaitCursor();
    thepDlg->m_tSetTime = CTime(m_tDate.GetYear(), m_tDate.GetMonth(), m_tDate.GetDay(),
        m_tTime.GetHour(), m_tTime.GetMinute(), m_tTime.GetSecond());
    thepDlg->SendData(SCMTIME);
    EndWaitCursor();
}

