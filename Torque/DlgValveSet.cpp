// DlgValveSet.cpp : implementation file
//

#include "stdafx.h"
#include "Torque.h"
#include "DlgValveSet.h"
#include "afxdialogex.h"


// CDlgValveSet dialog

IMPLEMENT_DYNAMIC(CDlgValveSet, CDialogEx)

CDlgValveSet::CDlgValveSet(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DLGVALVESET, pParent)
    , m_ucValve1TorqInit(0)
    , m_ucValve1TorqDece1(30)
    , m_ucValve1TorqDece2(70)
    , m_ucValve1PressInit(90)
    , m_ucValve1PressDece1(70)
    , m_ucValve1PressDece2(10)
    , m_ucValve2TorqInit(0)
    , m_ucValve2TorqDece1(30)
    , m_ucValve2TorqDece2(70)
    , m_ucValve2FlowInit(50)
    , m_ucValve2FlowDece1(10)
    , m_ucValve2FlowDece2(10)
    //, m_ucZoomRatio(1)
{
}

CDlgValveSet::~CDlgValveSet()
{
}

void CDlgValveSet::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDITVALVE1TORQ1, m_ucValve1TorqInit);
    DDV_MinMaxByte(pDX, m_ucValve1TorqInit, 0, 100);
    DDX_Text(pDX, IDC_EDITVALVE1TORQ2, m_ucValve1TorqDece1);
    DDV_MinMaxByte(pDX, m_ucValve1TorqDece1, 0, 100);
    DDX_Text(pDX, IDC_EDITVALVE1TORQ3, m_ucValve1TorqDece2);
    DDV_MinMaxByte(pDX, m_ucValve1TorqDece2, 0, 100);
    DDX_Text(pDX, IDC_EDITVALVE1PRESS1, m_ucValve1PressInit);
    DDV_MinMaxByte(pDX, m_ucValve1PressInit, 0, 100);
    DDX_Text(pDX, IDC_EDITVALVE1PRESS2, m_ucValve1PressDece1);
    DDV_MinMaxByte(pDX, m_ucValve1PressDece1, 0, 100);
    DDX_Text(pDX, IDC_EDITVALVE1PRESS3, m_ucValve1PressDece2);
    DDV_MinMaxByte(pDX, m_ucValve1PressDece2, 0, 100);
    DDX_Text(pDX, IDC_EDITVALVE2TORQ1, m_ucValve2TorqInit);
    DDV_MinMaxByte(pDX, m_ucValve2TorqInit, 0, 100);
    DDX_Text(pDX, IDC_EDITVALVE2TORQ2, m_ucValve2TorqDece1);
    DDV_MinMaxByte(pDX, m_ucValve2TorqDece1, 0, 100);
    DDX_Text(pDX, IDC_EDITVALVE2TORQ3, m_ucValve2TorqDece2);
    DDV_MinMaxByte(pDX, m_ucValve2TorqDece2, 0, 100);
    DDX_Text(pDX, IDC_EDITVALVE2FLOW1, m_ucValve2FlowInit);
    DDV_MinMaxByte(pDX, m_ucValve2FlowInit, 0, 100);
    DDX_Text(pDX, IDC_EDITVALVE2FLOW2, m_ucValve2FlowDece1);
    DDV_MinMaxByte(pDX, m_ucValve2FlowDece1, 0, 100);
    DDX_Text(pDX, IDC_EDITVALVE2FLOW3, m_ucValve2FlowDece2);
    DDV_MinMaxByte(pDX, m_ucValve2FlowDece2, 0, 100);
    //DDX_Text(pDX, IDC_EDITZOOMRATIO, m_ucZoomRatio);
    //DDV_MinMaxByte(pDX, m_ucZoomRatio, 1, 30);
}


BEGIN_MESSAGE_MAP(CDlgValveSet, CDialogEx)
    ON_BN_CLICKED(IDC_BTNVALVE, &CDlgValveSet::OnBnClickedBtnvalve)
END_MESSAGE_MAP()


// CDlgValveSet message handlers


/* ONOK时发送命令 */
void CDlgValveSet::OnBnClickedBtnvalve()
{
    UpdateData(TRUE);

    m_tempValve.ucTorq[0][0] = m_ucValve1TorqInit;
    m_tempValve.ucTorq[0][1] = m_ucValve1TorqDece1;
    m_tempValve.ucTorq[0][2] = m_ucValve1TorqDece2;
    m_tempValve.ucTorq[1][0] = m_ucValve2TorqInit;
    m_tempValve.ucTorq[1][1] = m_ucValve2TorqDece1;
    m_tempValve.ucTorq[1][2] = m_ucValve2TorqDece2;

    m_tempValve.ucRatio[0][0] = m_ucValve1PressInit;
    m_tempValve.ucRatio[0][1] = m_ucValve1PressDece1;
    m_tempValve.ucRatio[0][2] = m_ucValve1PressDece2;
    m_tempValve.ucRatio[1][0] = m_ucValve2FlowInit;
    m_tempValve.ucRatio[1][1] = m_ucValve2FlowDece1;
    m_tempValve.ucRatio[1][2] = m_ucValve2FlowDece2;

    //m_tempValve.ucZoomRatio   = m_ucZoomRatio;

    if (!theDB.UpdateValvePara(&m_tempValve))
        return;

    CDialog::OnOK();
}


BOOL CDlgValveSet::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_ucValve1TorqInit = m_tempValve.ucTorq[0][0];
    m_ucValve1TorqDece1 = m_tempValve.ucTorq[0][1];
    m_ucValve1TorqDece2 = m_tempValve.ucTorq[0][2];
    m_ucValve2TorqInit = m_tempValve.ucTorq[1][0];
    m_ucValve2TorqDece1 = m_tempValve.ucTorq[1][1];
    m_ucValve2TorqDece2 = m_tempValve.ucTorq[1][2];

    m_ucValve1PressInit = m_tempValve.ucRatio[0][0];
    m_ucValve1PressDece1 = m_tempValve.ucRatio[0][1];
    m_ucValve1PressDece2 = m_tempValve.ucRatio[0][2];
    m_ucValve2FlowInit = m_tempValve.ucRatio[1][0];
    m_ucValve2FlowDece1 = m_tempValve.ucRatio[1][1];
    m_ucValve2FlowDece2 = m_tempValve.ucRatio[1][2];

    //m_ucZoomRatio        = m_tempValve.ucZoomRatio;

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}
