// DlgModPW.cpp : implementation file
//

#include "stdafx.h"
#include "Torque.h"
#include "DlgModPW.h"
#include "afxdialogex.h"


// CDlgModPW dialog

IMPLEMENT_DYNAMIC(CDlgModPW, CDialogEx)

CDlgModPW::CDlgModPW(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DLGMODPW, pParent)
    , m_strOldPW(_T(""))
    , m_strNewPW(_T(""))
{

}

CDlgModPW::~CDlgModPW()
{
}

void CDlgModPW::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDITOLDPW, m_strOldPW);
    DDX_Text(pDX, IDC_EDITNEWPW, m_strNewPW);
    DDV_MaxChars(pDX, m_strOldPW, 30);
    DDV_MaxChars(pDX, m_strNewPW, 30);
}


BEGIN_MESSAGE_MAP(CDlgModPW, CDialogEx)
    ON_BN_CLICKED(IDC_BTNMODPW, &CDlgModPW::OnBnClickedBtnmodpw)
END_MESSAGE_MAP()

// CDlgModPW message handlers
void CDlgModPW::OnBnClickedBtnmodpw()
{
    CString strCompPW;
    CString strSuppPW;

    UpdateData(TRUE);

    strSuppPW.Format(IDS_STRSUPPORPW);
    
    strCompPW = theApp.m_aucPassWord;
    if( 0 != strCompPW.Compare(m_strOldPW) &&
        0 != strSuppPW.Compare(m_strOldPW))
    {
        AfxMessageBox(IDS_STROLDPWERROR);
        return;
    }

    if(m_strNewPW.IsEmpty())
    {
        AfxMessageBox(IDS_STRNEWPWNULL);
        return;
    }

    lstrcpyn(theApp.m_aucPassWord, m_strNewPW, MAXPWLEN);
    theApp.WriteShowPara();

    AfxMessageBox(IDS_STRNEWPWOK);
    CDialog::OnOK();
}
