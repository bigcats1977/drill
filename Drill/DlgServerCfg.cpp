// DlgServerCfg.cpp : implementation file
//

#include "stdafx.h"
#include "Drill.h"
#include "DlgServerCfg.h"


// CDlgServerCfg dialog

IMPLEMENT_DYNAMIC(CDlgServerCfg, CDialog)

CDlgServerCfg::CDlgServerCfg(CWnd* pParent /*=nullptr*/)
    : CDialog(IDD_DLGSERVCFG, pParent)
{
    m_nFTPPort = 22;
}

CDlgServerCfg::~CDlgServerCfg()
{
}

void CDlgServerCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDITFTPADDR, m_strFTPAddr);
    DDX_Text(pDX, IDC_EDITPORT, m_nFTPPort);
    DDX_Text(pDX, IDC_EDITFUSERNAME, m_strUsername);
    DDX_Text(pDX, IDC_EDITFPASSWORD, m_strPassword);
    DDX_Text(pDX, IDC_EDITROOTPATH, m_strTargetPath);
}


BEGIN_MESSAGE_MAP(CDlgServerCfg, CDialog)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CDlgServerCfg message handlers
BOOL CDlgServerCfg::OnInitDialog()
{
    int     i = 0;

    CDialog::OnInitDialog();

    m_clrNormal = RGB(0, 0, 0);
    m_clrChanged = RGB(255, 0, 0);

    /* 根据入参设置参数初始值 */
    SetParaValue(&theApp.m_tServCfg);

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

bool CDlgServerCfg::GetParaValue(SERVERCFG* ptCfg)
{
    CString     strTemp;

    ASSERT_NULL_R(ptCfg, false);

    //m_IPAddr.GetWindowText(strTemp);
    //ptCfg->strIPAddr = strTemp.GetBuffer(0);
    ptCfg->nFTPPort = m_nFTPPort;
    ptCfg->strFTPAddr = m_strFTPAddr.GetBuffer(0);
    ptCfg->strUserName = m_strUsername.GetBuffer(0);
    ptCfg->strPassword = m_strPassword.GetBuffer(0);
    ptCfg->strTargetPath = m_strTargetPath.GetBuffer(0);
    if (ptCfg->nFTPPort == 0 ||
        ptCfg->strFTPAddr.empty() ||
        ptCfg->strUserName.empty() ||
        ptCfg->strPassword.empty() ||
        ptCfg->strTargetPath.empty())
    {
        return false;
    }

    return true;
}

void CDlgServerCfg::SetParaValue(SERVERCFG* ptCfg)
{
    ASSERT_NULL(ptCfg);

    //m_IPAddr.SetWindowText(ptCfg->strIPAddr.c_str());
    m_nFTPPort = ptCfg->nFTPPort;
    m_strFTPAddr = ptCfg->strFTPAddr.c_str();
    m_strUsername = ptCfg->strUserName.c_str();
    m_strPassword = ptCfg->strPassword.c_str();
    m_strTargetPath = ptCfg->strTargetPath.c_str();
}

void CDlgServerCfg::OnOK()
{
    string     strInfo;

    UpdateData(TRUE);

    if (!GetParaValue(&theApp.m_tServCfg))
    {
        strInfo = theApp.LoadstringFromRes(IDS_STRINVALIDPARA);
        theApp.SaveShowMessage(strInfo.c_str(), MB_OK | MB_ICONINFORMATION);
        return;
    }

    theDB.UpdateServerCfg(&theApp.m_tServCfg);

    UpdateData(FALSE);

    CDialog::OnOK();
}

HBRUSH CDlgServerCfg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    COLORREF    clrCtrl;
    CString     strContent;

    GetDlgItemText(pWnd->GetDlgCtrlID(), strContent);
    if (strContent.IsEmpty())
        return hbr;

    // TODO:  在此更改 DC 的任何特性
    switch (pWnd->GetDlgCtrlID())//对某一个特定控件进行判断
    {
    case IDC_EDITPORT:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), theApp.m_tServCfg.nFTPPort);
        break;

    case IDC_IPADDR:
        JUDGE_STRPARA_CHANGE(strContent, theApp.m_tServCfg.strFTPAddr.c_str());
        break;
    case IDC_EDITFUSERNAME:
        JUDGE_STRPARA_CHANGE(strContent, theApp.m_tServCfg.strUserName.c_str());
        break;
    case IDC_EDITFPASSWORD:
        JUDGE_STRPARA_CHANGE(strContent, theApp.m_tServCfg.strPassword.c_str());
        break;
    case IDC_EDITROOTPATH:
        JUDGE_STRPARA_CHANGE(strContent, theApp.m_tServCfg.strTargetPath.c_str());
        break;

    default:
        hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
        break;
    }

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}
