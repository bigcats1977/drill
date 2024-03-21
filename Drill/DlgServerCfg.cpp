// DlgServerCfg.cpp : implementation file
//

#include "stdafx.h"
#include "Drill.h"
#include "WITSEnc.h"
#include "DlgServerCfg.h"

// CDlgServerCfg dialog

IMPLEMENT_DYNAMIC(CDlgServerCfg, CDialog)

CDlgServerCfg::CDlgServerCfg(CWnd* pParent /*=nullptr*/)
    : CDialog(IDD_DLGSERVCFG, pParent)
{
    m_nFTPPort = 22;
    m_nTCPPort = 9600;
}

CDlgServerCfg::~CDlgServerCfg()
{
}

void CDlgServerCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDITFTPADDR, m_strFTPAddr);
    DDX_Text(pDX, IDC_EDITPORT, m_nFTPPort);
    DDV_MinMaxUInt(pDX, m_nFTPPort, 1, 65535);
    DDX_Text(pDX, IDC_EDITFUSERNAME, m_strUsername);
    DDX_Text(pDX, IDC_EDITFPASSWORD, m_strPassword);
    DDX_Text(pDX, IDC_EDITROOTPATH, m_strTargetPath);
    DDX_Control(pDX, IDC_HOSTIP, m_hostIP);
    DDX_Text(pDX, IDC_EDITTCPPORT, m_nTCPPort);
    DDV_MinMaxUInt(pDX, m_nTCPPort, 1, 65535);
}


BEGIN_MESSAGE_MAP(CDlgServerCfg, CDialog)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDTCPSTART, &CDlgServerCfg::OnBnClickedTcpstart)
    ON_BN_CLICKED(IDTCPSTOP, &CDlgServerCfg::OnBnClickedTcpstop)
    //ON_BN_CLICKED(IDTCPTEST, &CDlgServerCfg::OnBnClickedTcptest)
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

    //GetDlgItem(IDTCPTEST)->ShowWindow(FALSE);

    EnableTCPButton(theApp.isTCPServer());

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgServerCfg::EnableTCPButton(bool Started)
{
    GetDlgItem(IDTCPSTART)->EnableWindow(!Started);
    GetDlgItem(IDTCPSTOP)->EnableWindow(Started);
    //GetDlgItem(IDTCPTEST)->EnableWindow(Started);
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
    ptCfg->nTCPPort = m_nTCPPort;
    if (ptCfg->strFTPAddr.empty() ||
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

    // Address & port
    GetIPAddr();
    m_nTCPPort = ptCfg->nTCPPort;
}

void CDlgServerCfg::GetIPAddr()
{
    WSADATA WSAData;
    char hostname[256] = { 0 };

    struct addrinfo hints;
    struct addrinfo* res;// , * cur;
    struct sockaddr_in* addr;

    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
    {
        return;
    }
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        WSACleanup();
        return;
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;     /* Allow IPv4 */
    hints.ai_flags = AI_PASSIVE;/* For wildcard IP address */
    hints.ai_protocol = 0;         /* Any protocol */
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, NULL, &hints, &res) == 0)
    {
        addr = (struct sockaddr_in*)res->ai_addr;
        m_hostIP.SetAddress(addr->sin_addr.S_un.S_un_b.s_b1,
            addr->sin_addr.S_un.S_un_b.s_b2,
            addr->sin_addr.S_un.S_un_b.s_b3,
            addr->sin_addr.S_un.S_un_b.s_b4);

        freeaddrinfo(res);
    }
}

void CDlgServerCfg::OnOK()
{
    string     strInfo;
    UINT       nOldPort = theApp.m_tServCfg.nTCPPort;

    UpdateData(TRUE);

    if (!GetParaValue(&theApp.m_tServCfg))
    {
        strInfo = theApp.LoadstringFromRes(IDS_STRINVALIDPARA);
        theApp.SaveShowMessage(strInfo.c_str(), MB_OK | MB_ICONINFORMATION);
        return;
    }

    theDB.UpdateServerCfg(&theApp.m_tServCfg);

    if (nOldPort != m_nTCPPort || !theApp.isTCPServer())
    {
        theApp.InitTCPServer();
    }

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
    case IDC_EDITTCPPORT:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), theApp.m_tServCfg.nTCPPort);
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

void CDlgServerCfg::OnBnClickedTcpstart()
{
    UpdateData(TRUE);

    theApp.m_tServCfg.nTCPPort = m_nTCPPort;
    theDB.UpdateServerCfg(&theApp.m_tServCfg);

    theApp.InitTCPServer();

    EnableTCPButton(true);
}

void CDlgServerCfg::OnBnClickedTcpstop()
{
    theApp.CloseSockets();

    EnableTCPButton(false);
}
