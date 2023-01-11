// DlgPassword.cpp : implementation file
//

#include "stdafx.h"
#include "Drill.h"
#include "DlgPassword.h"


// CDlgPassword dialog

IMPLEMENT_DYNAMIC(CDlgPassword, CDialog)

CDlgPassword::CDlgPassword(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgPassword::IDD, pParent)
    , m_strPassword(_T(""))
{

}

CDlgPassword::~CDlgPassword()
{
}

void CDlgPassword::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDITPASSWORD, m_strPassword);
}


BEGIN_MESSAGE_MAP(CDlgPassword, CDialog)
END_MESSAGE_MAP()


// CDlgPassword message handlers
