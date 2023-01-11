// DlgZero.cpp : implementation file
//

#include "stdafx.h"
#include "Drill.h"
#include "DlgZero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgZero dialog


CDlgZero::CDlgZero(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgZero::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgZero)
    m_fZero = 0.0;
    //}}AFX_DATA_INIT
}


void CDlgZero::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgZero)
    DDX_Control(pDX, IDOK, m_ZeroOK);
    DDX_Control(pDX, IDCANCEL, m_ZeroCancel);
    DDX_Text(pDX, IDC_ZERO, m_fZero);
    DDV_MinMaxDouble(pDX, m_fZero, 0., 2.);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgZero, CDialog)
    //{{AFX_MSG_MAP(CDlgZero)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgZero message handlers

BOOL CDlgZero::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_ZeroOK.SetIconAndText(IDI_APPLY, IDS_STRZEROCORRECT);
    m_ZeroCancel.SetIconAndText(IDI_NO, IDS_STRRETURN);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

