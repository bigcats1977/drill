// DlgFuncTest.cpp : implementation file
//

#include "stdafx.h"
#include "Torque.h"
#include "DlgFuncTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFuncTest dialog


CDlgFuncTest::CDlgFuncTest(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgFuncTest::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgFuncTest)
    //}}AFX_DATA_INIT
}


void CDlgFuncTest::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgFuncTest)
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Control(pDX, IDC_BTNUNLOADEND, m_btnUnloadE);
    DDX_Control(pDX, IDC_BTNUNLOADBEGIN, m_btnUnloadB);
    DDX_Control(pDX, IDC_BTNSPEEDEND, m_btnSpeedE);
    DDX_Control(pDX, IDC_BTNSPEEDBEGIN, m_btnSpeedB);
    DDX_Control(pDX, IDC_BTNALARMEND, m_btnAlarmE);
    DDX_Control(pDX, IDC_BTNALARMBEGIN, m_btnAlarmB);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFuncTest, CDialog)
    //{{AFX_MSG_MAP(CDlgFuncTest)
    ON_BN_CLICKED(IDC_BTNSPEEDBEGIN, OnBtnspeedbegin)
    ON_BN_CLICKED(IDC_BTNSPEEDEND, OnBtnspeedend)
    ON_BN_CLICKED(IDC_BTNUNLOADBEGIN, OnBtnunloadbegin)
    ON_BN_CLICKED(IDC_BTNUNLOADEND, OnBtnunloadend)
    ON_BN_CLICKED(IDC_BTNALARMBEGIN, OnBtnalarmbegin)
    ON_BN_CLICKED(IDC_BTNALARMEND, OnBtnalarmend)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFuncTest message handlers

BOOL CDlgFuncTest::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    m_btnUnloadE.SetIcon(IDI_UNLOADE);
    m_btnUnloadB.SetIcon(IDI_UNLOADB);
    m_btnSpeedE.SetIcon(IDI_SPEEDE);
    m_btnSpeedB.SetIcon(IDI_SPEEDB);
    m_btnAlarmE.SetIcon(IDI_ALARME);
    m_btnAlarmB.SetIcon(IDI_ALARMB);

    m_btnOK.SetIconAndText(IDI_NO, IDS_STRRETURN);

    m_pParentDlg = (CTorqueDlg*)GetParent();

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFuncTest::OnBtnspeedbegin()
{
    m_btnSpeedB.EnableWindow(FALSE);
    m_btnSpeedE.EnableWindow(TRUE);

    m_pParentDlg->TestSpeedLed(IDI_LEDON);
    m_pParentDlg->SendData(SCMBSPEED);
}

void CDlgFuncTest::OnBtnspeedend()
{
    m_btnSpeedB.EnableWindow(TRUE);
    m_btnSpeedE.EnableWindow(FALSE);

    m_pParentDlg->TestSpeedLed(IDI_LEDREDON);
    m_pParentDlg->SendData(SCMESPEED);
}

void CDlgFuncTest::OnBtnunloadbegin()
{
    m_btnUnloadB.EnableWindow(FALSE);
    m_btnUnloadE.EnableWindow(TRUE);

    m_pParentDlg->TestUnloadLed(IDI_LEDON);
    m_pParentDlg->SendData(SCMBUNLOAD);
}

void CDlgFuncTest::OnBtnunloadend()
{
    m_btnUnloadB.EnableWindow(TRUE);
    m_btnUnloadE.EnableWindow(FALSE);

    m_pParentDlg->TestUnloadLed(IDI_LEDREDON);
    m_pParentDlg->SendData(SCMEUNLOAD);
}

void CDlgFuncTest::OnBtnalarmbegin()
{
    m_btnAlarmB.EnableWindow(FALSE);
    m_btnAlarmE.EnableWindow(TRUE);

    m_pParentDlg->TestAlarmLed(IDI_LEDREDON);

    theApp.PlayAlarmSound();
}

void CDlgFuncTest::OnBtnalarmend()
{
    m_btnAlarmB.EnableWindow(TRUE);
    m_btnAlarmE.EnableWindow(FALSE);

    m_pParentDlg->TestAlarmLed(IDI_LEDREDOFF);

    theApp.StopAlarmSound();
}
