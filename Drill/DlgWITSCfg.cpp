// DlgWITSCfg.cpp : implementation file
//
#include "stdafx.h"
#include "Drill.h"
#include "WITSEnc.h"
#include "DlgWITSCfg.h"

// CDlgWITSCfg dialog

IMPLEMENT_DYNAMIC(CDlgWITSCfg, CDialog)

CDlgWITSCfg::CDlgWITSCfg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLGWITSCFG, pParent)
{
}

CDlgWITSCfg::~CDlgWITSCfg()
{
}

void CDlgWITSCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_CHECKWITS01, m_bChecked[0]);
    DDX_Check(pDX, IDC_CHECKWITS02, m_bChecked[1]);
    DDX_Check(pDX, IDC_CHECKWITS03, m_bChecked[2]);
    DDX_Check(pDX, IDC_CHECKWITS04, m_bChecked[3]);
    DDX_Check(pDX, IDC_CHECKWITS05, m_bChecked[4]);
    DDX_Check(pDX, IDC_CHECKWITS06, m_bChecked[5]);
    DDX_Check(pDX, IDC_CHECKWITS07, m_bChecked[6]);
    DDX_Check(pDX, IDC_CHECKWITS08, m_bChecked[7]);
    DDX_Check(pDX, IDC_CHECKWITS09, m_bChecked[8]);
    DDX_Check(pDX, IDC_CHECKWITS10, m_bChecked[9]);
    DDX_Check(pDX, IDC_CHECKWITS11, m_bChecked[10]);
    DDX_Check(pDX, IDC_CHECKWITS12, m_bChecked[11]);
    DDX_Check(pDX, IDC_CHECKWITS13, m_bChecked[12]);
    DDX_Check(pDX, IDC_CHECKWITS14, m_bChecked[13]);
    DDX_Check(pDX, IDC_CHECKWITS15, m_bChecked[14]);
    DDX_Control(pDX, IDC_CHECKWITS01, m_ckShowPara[0]);
    DDX_Control(pDX, IDC_CHECKWITS02, m_ckShowPara[1]);
    DDX_Control(pDX, IDC_CHECKWITS03, m_ckShowPara[2]);
    DDX_Control(pDX, IDC_CHECKWITS04, m_ckShowPara[3]);
    DDX_Control(pDX, IDC_CHECKWITS05, m_ckShowPara[4]);
    DDX_Control(pDX, IDC_CHECKWITS06, m_ckShowPara[5]);
    DDX_Control(pDX, IDC_CHECKWITS07, m_ckShowPara[6]);
    DDX_Control(pDX, IDC_CHECKWITS08, m_ckShowPara[7]);
    DDX_Control(pDX, IDC_CHECKWITS09, m_ckShowPara[8]);
    DDX_Control(pDX, IDC_CHECKWITS10, m_ckShowPara[9]);
    DDX_Control(pDX, IDC_CHECKWITS11, m_ckShowPara[10]);
    DDX_Control(pDX, IDC_CHECKWITS12, m_ckShowPara[11]);
    DDX_Control(pDX, IDC_CHECKWITS13, m_ckShowPara[12]);
    DDX_Control(pDX, IDC_CHECKWITS14, m_ckShowPara[13]);
    DDX_Control(pDX, IDC_CHECKWITS15, m_ckShowPara[14]);
    DDX_Control(pDX, IDC_EDITWITSPARA01, m_edShowPara[0]);
    DDX_Control(pDX, IDC_EDITWITSPARA02, m_edShowPara[1]);
    DDX_Control(pDX, IDC_EDITWITSPARA03, m_edShowPara[2]);
    DDX_Control(pDX, IDC_EDITWITSPARA04, m_edShowPara[3]);
    DDX_Control(pDX, IDC_EDITWITSPARA05, m_edShowPara[4]);
    DDX_Control(pDX, IDC_EDITWITSPARA06, m_edShowPara[5]);
    DDX_Control(pDX, IDC_EDITWITSPARA07, m_edShowPara[6]);
    DDX_Control(pDX, IDC_EDITWITSPARA08, m_edShowPara[7]);
    DDX_Control(pDX, IDC_EDITWITSPARA09, m_edShowPara[8]);
    DDX_Control(pDX, IDC_EDITWITSPARA10, m_edShowPara[9]);
    DDX_Control(pDX, IDC_EDITWITSPARA11, m_edShowPara[10]);
    DDX_Control(pDX, IDC_EDITWITSPARA12, m_edShowPara[11]);
    DDX_Control(pDX, IDC_EDITWITSPARA13, m_edShowPara[12]);
    DDX_Control(pDX, IDC_EDITWITSPARA14, m_edShowPara[13]);
    DDX_Control(pDX, IDC_EDITWITSPARA15, m_edShowPara[14]);
    DDX_Text(pDX, IDC_EDITWITSDATE, m_nFixItems[0]);
    DDX_Text(pDX, IDC_EDITWITSTIME, m_nFixItems[1]);
    DDX_Text(pDX, IDC_EDITWITSSEQNO, m_nFixItems[2]);
    DDX_Text(pDX, IDC_EDITWITSTORQ, m_nRepeatItems[0]);
    DDX_Text(pDX, IDC_EDITWITSTURN, m_nRepeatItems[1]);
    DDX_Text(pDX, IDC_EDITWITSDURATION, m_nRepeatItems[2]);
    DDX_Text(pDX, IDC_EDITWITSCTRLTORQ, m_nCalItems[0]);
    DDX_Text(pDX, IDC_EDITWITSIPTORQ, m_nCalItems[1]);
    DDX_Text(pDX, IDC_EDITWITSDELTATORQ, m_nCalItems[2]);
    DDX_Text(pDX, IDC_EDITWITSIPRATIO, m_nCalItems[3]);
    DDX_Text(pDX, IDC_EDITWITSIPDURATION, m_nCalItems[4]);
    DDX_Text(pDX, IDC_EDITWITSPARA01, m_nShowItems[0]);
    DDX_Text(pDX, IDC_EDITWITSPARA02, m_nShowItems[1]);
    DDX_Text(pDX, IDC_EDITWITSPARA03, m_nShowItems[2]);
    DDX_Text(pDX, IDC_EDITWITSPARA04, m_nShowItems[3]);
    DDX_Text(pDX, IDC_EDITWITSPARA05, m_nShowItems[4]);
    DDX_Text(pDX, IDC_EDITWITSPARA06, m_nShowItems[5]);
    DDX_Text(pDX, IDC_EDITWITSPARA07, m_nShowItems[6]);
    DDX_Text(pDX, IDC_EDITWITSPARA08, m_nShowItems[7]);
    DDX_Text(pDX, IDC_EDITWITSPARA09, m_nShowItems[8]);
    DDX_Text(pDX, IDC_EDITWITSPARA10, m_nShowItems[9]);
    DDX_Text(pDX, IDC_EDITWITSPARA11, m_nShowItems[10]);
    DDX_Text(pDX, IDC_EDITWITSPARA12, m_nShowItems[11]);
    DDX_Text(pDX, IDC_EDITWITSPARA13, m_nShowItems[12]);
    DDX_Text(pDX, IDC_EDITWITSPARA14, m_nShowItems[13]);
    DDX_Text(pDX, IDC_EDITWITSPARA15, m_nShowItems[14]);
    DDX_Text(pDX, IDC_EDITWITSTORQTYPE, m_nConfigItems[0]);
    DDX_Text(pDX, IDC_EDITWITSMAXTORQ, m_nConfigItems[1]);
    DDX_Text(pDX, IDC_EDITWITSMINTORQ, m_nConfigItems[2]);
}

BEGIN_MESSAGE_MAP(CDlgWITSCfg, CDialog)
    ON_WM_CTLCOLOR()
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_CHECKWITS01, &CDlgWITSCfg::OnBnClickedCheckwits01)
    ON_BN_CLICKED(IDC_CHECKWITS02, &CDlgWITSCfg::OnBnClickedCheckwits02)
    ON_BN_CLICKED(IDC_CHECKWITS03, &CDlgWITSCfg::OnBnClickedCheckwits03)
    ON_BN_CLICKED(IDC_CHECKWITS04, &CDlgWITSCfg::OnBnClickedCheckwits04)
    ON_BN_CLICKED(IDC_CHECKWITS05, &CDlgWITSCfg::OnBnClickedCheckwits05)
    ON_BN_CLICKED(IDC_CHECKWITS06, &CDlgWITSCfg::OnBnClickedCheckwits06)
    ON_BN_CLICKED(IDC_CHECKWITS07, &CDlgWITSCfg::OnBnClickedCheckwits07)
    ON_BN_CLICKED(IDC_CHECKWITS08, &CDlgWITSCfg::OnBnClickedCheckwits08)
    ON_BN_CLICKED(IDC_CHECKWITS09, &CDlgWITSCfg::OnBnClickedCheckwits09)
    ON_BN_CLICKED(IDC_CHECKWITS10, &CDlgWITSCfg::OnBnClickedCheckwits10)
    ON_BN_CLICKED(IDC_CHECKWITS11, &CDlgWITSCfg::OnBnClickedCheckwits11)
    ON_BN_CLICKED(IDC_CHECKWITS12, &CDlgWITSCfg::OnBnClickedCheckwits12)
    ON_BN_CLICKED(IDC_CHECKWITS13, &CDlgWITSCfg::OnBnClickedCheckwits13)
    ON_BN_CLICKED(IDC_CHECKWITS14, &CDlgWITSCfg::OnBnClickedCheckwits14)
    ON_BN_CLICKED(IDC_CHECKWITS15, &CDlgWITSCfg::OnBnClickedCheckwits15)
END_MESSAGE_MAP()


// CDlgWITSCfg message handlers
BOOL CDlgWITSCfg::OnInitDialog()
{
    int     i = 0;

    CDialog::OnInitDialog();

    m_pCurCfg = &theApp.m_tWITSCfg;

    m_clrNormal = RGB(0, 0, 0);
    m_clrChanged = RGB(255, 0, 0);

    SetParaValue(m_pCurCfg);

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgWITSCfg::OnDestroy()
{
    CDialog::OnDestroy();

    WSACleanup();
}

bool CDlgWITSCfg::GetParaValue(WITSCFG* ptCfg)
{
    int i = 0;
    CString     strTemp;
    vector<int> items;

    ASSERT_NULL_R(ptCfg, false);

    // fix Items
    ptCfg->FixItems.clear();
    for (i = 0; i < WITSRPT_FIXHEADNUM; i++) {
        if (m_nFixItems[i] == 0)
            return false;
        ptCfg->FixItems.push_back(m_nFixItems[i]);
    }

    // Repeat Items
    ptCfg->RepeatItems.clear();
    for (i = 0; i < WITSRPT_REPEATNUM; i++) {
        if (m_nRepeatItems[i] == 0)
            return false;
        ptCfg->RepeatItems.push_back(m_nRepeatItems[i]);
    }

    // calculator Items
    ptCfg->CalItems.clear();
    for (i = 0; i < WITSRPT_CALPARANUM; i++) {
        if (m_nCalItems[i] == 0)
            return false;
        ptCfg->CalItems.push_back(m_nCalItems[i]);
    }

    // show params
    ptCfg->ShowParas.clear();
    ptCfg->ShowItems.clear();
    for (i = 0; i < (int)m_tempShow.nParaNum && i < WITSRPT_SHOWPARANUM; i++)
    {
        if (m_bChecked[i])
        {
            if (m_nShowItems[i] == 0)
                return false;
            ptCfg->ShowParas.push_back(i+1);
            ptCfg->ShowItems.push_back(m_nShowItems[i]);
        }
    }

    return true;
}

int CDlgWITSCfg::GetItemIndexByPara(WITSCFG* ptCfg, int para)
{
    vector<int>::iterator it;

    ASSERT_NULL_R(ptCfg, -1);

    it = find(ptCfg->ShowParas.begin(), ptCfg->ShowParas.end(), para);
    if (it == ptCfg->ShowParas.end())
        return -1;

    return it - ptCfg->ShowParas.begin();
}

void CDlgWITSCfg::SetParaValue(WITSCFG* ptCfg)
{
    size_t i = 0;
    int index = -1;

    ASSERT_NULL(ptCfg);

    // fix Items
    memset(m_nFixItems, 0, sizeof(UINT) * WITSRPT_FIXHEADNUM);
    for (i = 0; i < WITSRPT_FIXHEADNUM && i < ptCfg->FixItems.size(); i++) {
        m_nFixItems[i] = ptCfg->FixItems[i];
    }

    // Repeat Items
    memset(m_nRepeatItems, 0, sizeof(UINT) * WITSRPT_REPEATNUM);
    for (i = 0; i < WITSRPT_REPEATNUM && i < ptCfg->RepeatItems.size(); i++) {
        m_nRepeatItems[i] = ptCfg->RepeatItems[i];
    }

    // calculator Items
    memset(m_nCalItems, 0, sizeof(UINT)* WITSRPT_CALPARANUM);
    for (i = 0; i < WITSRPT_CALPARANUM && i < ptCfg->CalItems.size(); i++) {
        m_nCalItems[i] = ptCfg->CalItems[i];
    }

    // config Items
    memset(m_nConfigItems, 0, sizeof(UINT)* WITSRPT_CONFIGNUM);
    for (i = 0; i < WITSRPT_CONFIGNUM && i < ptCfg->ConfigItems.size(); i++) {
        m_nConfigItems[i] = ptCfg->ConfigItems[i];
    }

    // show params
    vector<int> items = ptCfg->ShowParas;
    memset(m_bChecked, 0, sizeof(BOOL) * (WITSRPT_SHOWPARANUM));
    memset(m_nShowItems, 0, sizeof(UINT) * WITSRPT_SHOWPARANUM);
    for (i = 0; i < (int)m_tempShow.nParaNum && i < WITSRPT_SHOWPARANUM; i++)
    {
        m_ckShowPara[i].SetWindowText(m_tempShow.strShow[i+1].c_str());
        index = GetItemIndexByPara(ptCfg, i + 1);
        if (index >= 0)
        {
            m_bChecked[i] = TRUE;
            m_edShowPara[i].EnableWindow(TRUE);
            m_nShowItems[i] = ptCfg->ShowItems[index];
        }
        else
            m_edShowPara[i].EnableWindow(FALSE);
    }
}

void CDlgWITSCfg::OnOK()
{
    string     strInfo;
    WITSCFG    tempCfg;

    UpdateData(TRUE);

    if (!GetParaValue(&tempCfg))
    {
        strInfo = theApp.LoadstringFromRes(IDS_STRINVALIDPARA);
        theApp.SaveShowMessage(strInfo.c_str(), MB_OK | MB_ICONINFORMATION);
        return;
    }
    *m_pCurCfg = tempCfg;
    theDB.UpdateWITSCfg(m_pCurCfg);

    UpdateData(FALSE);

    CDialog::OnOK();
}

HBRUSH CDlgWITSCfg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    COLORREF    clrCtrl;
    CString     strContent;
    int         CtrlID = pWnd->GetDlgCtrlID();
    int         iShowITem = 0;
    int         index = -1;

    GetDlgItemText(pWnd->GetDlgCtrlID(), strContent);
    if (strContent.IsEmpty())
        return hbr;

    // TODO:  在此更改 DC 的任何特性
    switch (CtrlID)//对某一个特定控件进行判断
    {

    // fix items
    case IDC_EDITWITSDATE:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_pCurCfg->FixItems[0]);
        break;
    case IDC_EDITWITSTIME:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_pCurCfg->FixItems[1]);
        break;
    case IDC_EDITWITSSEQNO:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_pCurCfg->FixItems[2]);
        break;

    // repeat torque items
    case IDC_EDITWITSTORQ:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_pCurCfg->RepeatItems[0]);
        break;
    case IDC_EDITWITSTURN:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_pCurCfg->RepeatItems[1]);
        break;
    case IDC_EDITWITSDURATION:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_pCurCfg->RepeatItems[2]);
        break;

    // cal items
    case IDC_EDITWITSCTRLTORQ:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_pCurCfg->CalItems[0]);
        break;
    case IDC_EDITWITSIPTORQ:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_pCurCfg->CalItems[1]);
        break;
    case IDC_EDITWITSDELTATORQ:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_pCurCfg->CalItems[2]);
        break;
    case IDC_EDITWITSIPRATIO:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_pCurCfg->CalItems[3]);
        break;
    case IDC_EDITWITSIPDURATION:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_pCurCfg->CalItems[4]);
        break;

        // cal items
    case IDC_EDITWITSTORQTYPE:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_pCurCfg->ConfigItems[0]);
        break;
    case IDC_EDITWITSMAXTORQ:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_pCurCfg->ConfigItems[1]);
        break;
    case IDC_EDITWITSMINTORQ:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_pCurCfg->ConfigItems[2]);
        break;

    case IDC_EDITWITSPARA01:
    case IDC_EDITWITSPARA02:
    case IDC_EDITWITSPARA03:
    case IDC_EDITWITSPARA04:
    case IDC_EDITWITSPARA05:
    case IDC_EDITWITSPARA06:
    case IDC_EDITWITSPARA07:
    case IDC_EDITWITSPARA08:
    case IDC_EDITWITSPARA09:
    case IDC_EDITWITSPARA10:
    case IDC_EDITWITSPARA11:
    case IDC_EDITWITSPARA12:
    case IDC_EDITWITSPARA13:
    case IDC_EDITWITSPARA14:
    case IDC_EDITWITSPARA15:
        index = GetItemIndexByPara(m_pCurCfg, CtrlID - IDC_EDITWITSPARA01 + 1);
        if (index >= 0)
            iShowITem = m_pCurCfg->ShowItems[index];
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), iShowITem);
        break;

    default:
        hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
        break;
    }

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void CDlgWITSCfg::JudgeShowParaCheck(int iCtrlIdx)
{
    COLORREF    clrCtrl;
    BOOL bcheck = FALSE;
    int index = iCtrlIdx - 1;

    COMP_BL(iCtrlIdx, 1);
    COMP_BG(iCtrlIdx, WITSRPT_SHOWPARANUM);

    if (GetItemIndexByPara(m_pCurCfg, iCtrlIdx) >= 0)
    {
        bcheck = TRUE;
    }
    clrCtrl = m_clrNormal;
    if (bcheck != m_bChecked[index])
    {
        clrCtrl = m_clrChanged;
    }
    m_edShowPara[index].EnableWindow(FALSE);
    if (m_bChecked[index])
    {
        m_edShowPara[index].EnableWindow(TRUE);
    }

    m_ckShowPara[index].SetForeColor(clrCtrl);
    m_ckShowPara[index].Invalidate();
}
void CDlgWITSCfg::OnBnClickedCheckwits01()
{
    UpdateData(TRUE);
    JudgeShowParaCheck(1);
}
void CDlgWITSCfg::OnBnClickedCheckwits02()
{
    UpdateData(TRUE);
    JudgeShowParaCheck(2);
}
void CDlgWITSCfg::OnBnClickedCheckwits03()
{
    UpdateData(TRUE);
    JudgeShowParaCheck(3);
}
void CDlgWITSCfg::OnBnClickedCheckwits04()
{
    UpdateData(TRUE);
    JudgeShowParaCheck(4);
}
void CDlgWITSCfg::OnBnClickedCheckwits05()
{
    UpdateData(TRUE);
    JudgeShowParaCheck(5);
}
void CDlgWITSCfg::OnBnClickedCheckwits06()
{
    UpdateData(TRUE);
    JudgeShowParaCheck(6);
}
void CDlgWITSCfg::OnBnClickedCheckwits07()
{
    UpdateData(TRUE);
    JudgeShowParaCheck(7);
}
void CDlgWITSCfg::OnBnClickedCheckwits08()
{
    UpdateData(TRUE);
    JudgeShowParaCheck(8);
}
void CDlgWITSCfg::OnBnClickedCheckwits09()
{
    UpdateData(TRUE);
    JudgeShowParaCheck(9);
}
void CDlgWITSCfg::OnBnClickedCheckwits10()
{
    UpdateData(TRUE);
    JudgeShowParaCheck(10);
}
void CDlgWITSCfg::OnBnClickedCheckwits11()
{
    UpdateData(TRUE);
    JudgeShowParaCheck(11);
}
void CDlgWITSCfg::OnBnClickedCheckwits12()
{
    UpdateData(TRUE);
    JudgeShowParaCheck(12);
}
void CDlgWITSCfg::OnBnClickedCheckwits13()
{
    UpdateData(TRUE);
    JudgeShowParaCheck(13);
}
void CDlgWITSCfg::OnBnClickedCheckwits14()
{
    UpdateData(TRUE);
    JudgeShowParaCheck(14);
}
void CDlgWITSCfg::OnBnClickedCheckwits15()
{
    UpdateData(TRUE);
    JudgeShowParaCheck(15);
}
