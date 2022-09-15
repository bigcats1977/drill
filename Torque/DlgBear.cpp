// DlgBear.cpp : implementation file
//

#include "stdafx.h"
#include "torque.h"
#include "DlgBear.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBear dialog


CDlgBear::CDlgBear(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgBear::IDD, pParent)
    , m_bBigTorq(FALSE)
    , m_bShackle(FALSE)
{
    //{{AFX_DATA_INIT(CDlgBear)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgBear::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgBear)
    DDX_Check(pDX, IDC_CHECKBIGTORQ, m_bBigTorq);
    DDX_Check(pDX, IDC_CHECKSHACKLE, m_bShackle);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBear, CDialog)
    //{{AFX_MSG_MAP(CDlgBear)
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_CHECKBIGTORQ, OnBnClickedCheckbigtorq)
    ON_BN_CLICKED(IDC_CHECKSHACKLE, &CDlgBear::OnBnClickedCheckshackle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBear message handlers


BOOL CDlgBear::OnInitDialog()
{
    CDialog::OnInitDialog();

    SetTimer(BEARSHOW_TIMER,BEARSHOW_LEN,NULL);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBear::OnTimer(UINT_PTR nIDEvent)
{
    UpdateData(TRUE);
    
    KillTimer(BEARSHOW_TIMER);

    CDialog::OnOK();
    CDialog::OnTimer(nIDEvent);
}

void CDlgBear::OnBnClickedCheckbigtorq()
{
    CString strNum;

    UpdateData(TRUE);

    theApp.m_nTorqMulti = 1;
    theApp.m_bBigTorq = m_bBigTorq;
    if(m_bBigTorq)
        theApp.m_nTorqMulti = 10;

    strNum.Format("%d",m_bBigTorq);
    theApp.WriteConfigStr(IDS_STRPNADJUST,IDS_STRPIBIGTORQ,strNum,theApp.m_strParaFile);
}

void CDlgBear::OnBnClickedCheckshackle()
{
    CString strNum;

    UpdateData(TRUE);
}

