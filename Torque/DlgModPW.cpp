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
    string strCompPW;
    string strSuppPW;

    UpdateData(TRUE);

    strSuppPW = theApp.LoadstringFromRes(IDS_STRSUPPORPW);
    strCompPW = g_tGlbCfg.strPassWord;
    if( 0 != m_strOldPW.Compare(strSuppPW.c_str()) &&
        0 != m_strOldPW.Compare(strCompPW.c_str()))
    {
        AfxMessageBox(theApp.LoadstringFromRes(IDS_STROLDPWERROR).c_str());
        return;
    }

    if(m_strNewPW.IsEmpty())
    {
        AfxMessageBox(theApp.LoadstringFromRes(IDS_STRNEWPWNULL).c_str());
        return;
    }

    g_tGlbCfg.strPassWord = m_strNewPW;
    //lstrcpyn(theApp.m_aucPassWord, m_strNewPW, MAXPWLEN);
    theDB.UpdateGlobalPara();

    AfxMessageBox(theApp.LoadstringFromRes(IDS_STRNEWPWOK).c_str());
    CDialog::OnOK();
}
