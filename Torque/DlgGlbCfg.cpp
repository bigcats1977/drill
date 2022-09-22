// DlgGlbCfg.cpp : implementation file
//

#include "stdafx.h"
#include "Torque.h"
#include "DlgGlbCfg.h"

// CDlgGlbCfg dialog

IMPLEMENT_DYNAMIC(CDlgGlbCfg, CDialog)

CDlgGlbCfg::CDlgGlbCfg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLGGLBCFG, pParent)
{
    m_nPort = 1;
    m_nPlus = 0;
    m_nReset = 0;
    m_nCollect = 0;
    m_nSave = 0;
    m_bDateBehind = FALSE;
}

CDlgGlbCfg::~CDlgGlbCfg()
{
}

void CDlgGlbCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBOPORT, m_cbPort);
    DDX_Control(pDX, IDC_COMBOIMGNUM, m_cbImgNum);
    DDX_Text(pDX, IDC_EDITRESET, m_nReset);
    DDX_Text(pDX, IDC_EDITCOLLECT, m_nCollect);
    DDX_Text(pDX, IDC_EDITSAVEDUR, m_nSave);
    DDX_Text(pDX, IDC_EDITPLUS, m_nPlus);
    DDX_Control(pDX, IDC_EDITPLUS, m_nePlus);
    DDX_Check(pDX, IDC_CHECKBEHIND, m_bDateBehind);
}


BEGIN_MESSAGE_MAP(CDlgGlbCfg, CDialog)
END_MESSAGE_MAP()


// CDlgGlbCfg message handlers

BOOL CDlgGlbCfg::OnInitDialog()
{
    int     i = 0;

    CDialog::OnInitDialog();

    m_clrNormal = RGB(0, 0, 0);
    m_clrChanged = RGB(255, 0, 0);
    m_nCurLang = g_tGlbCfg.nLangType;

    /* 根据入参设置参数初始值 */
    SetParaValue(&g_tGlbCfg);

    UpdateData(FALSE);

    m_cbPort.SetCurSel(m_nPort - 1);
    SetImgNum();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgGlbCfg::GetParaValue(GLBCFG *ptCfg)
{
    CString     strTemp;

    ASSERT_NULL_R(ptCfg, FALSE);

    m_nPort = m_cbPort.GetCurSel() + 1;
    ptCfg->nPortNO = m_nPort;

    ptCfg->nPlusPerTurn = m_nPlus;

    ptCfg->nResetDur = m_nReset;
    ptCfg->nCollectDur = m_nCollect;
    ptCfg->nSaveDur = m_nSave;
    ptCfg->bDateBehind = m_bDateBehind;
    m_cbImgNum.GetWindowText(strTemp);
    ptCfg->nImgNum = atoi(strTemp);

    return TRUE;
}

void CDlgGlbCfg::SetParaValue(GLBCFG *ptCfg)
{
    ASSERT_NULL(ptCfg);

    m_nPort = ptCfg->nPortNO;

    m_nPlus = ptCfg->nPlusPerTurn;

    m_nReset = ptCfg->nResetDur;
    m_nCollect = ptCfg->nCollectDur;
    m_nSave = ptCfg->nSaveDur;
    m_bDateBehind = ptCfg->bDateBehind;
}

void CDlgGlbCfg::OnOK()
{
    string     strInfo;

    UpdateData(TRUE);

    GetParaValue(&g_tGlbCfg);

    theDB.UpdateGlobalPara();

    UpdateData(FALSE);

    CDialog::OnOK();
}

void CDlgGlbCfg::SetImgNum()
{
    switch (g_tGlbCfg.nImgNum)
    {
    case 1:
        m_cbImgNum.SetCurSel(0);
        break;
    case 2:
        m_cbImgNum.SetCurSel(1);
        break;
    case 3:
        m_cbImgNum.SetCurSel(2);
        break;
    case 8:
    default:
        m_cbImgNum.SetCurSel(3);
        break;
    }
}

HBRUSH CDlgGlbCfg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
    case IDC_EDITPLUS:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), g_tGlbCfg.nPlusPerTurn);
        break;

        // 系统配置
    case IDC_COMBOPORT:
        JUDGE_NUMBERPARA_CHANGE((m_cbPort.GetCurSel() + 1), g_tGlbCfg.nPortNO);
        break;
    case IDC_COMBOIMGNUM:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), g_tGlbCfg.nImgNum);
        break;
    case IDC_EDITRESET:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), g_tGlbCfg.nResetDur);
        break;
    case IDC_EDITCOLLECT:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), g_tGlbCfg.nCollectDur);
        break;

    default:
        hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
        break;
    }

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}
