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
    m_nPlus = 0;
    m_nReset = 0;
    m_nCollect = 0;
    m_nSave = 0;
    m_bDateBehind = FALSE;

    m_Baud[0] = 4800;
    m_Baud[1] = 9600;
    m_Baud[2] = 14400;
    m_Baud[3] = 19200;
    m_Baud[4] = 38400;
    m_Baud[5] = 56000;
    m_Baud[6] = 57600;
    m_Baud[7] = 115200;

    m_ImgNum[0] = 1;
    m_ImgNum[1] = 2;
    m_ImgNum[2] = 3;
    m_ImgNum[3] = 8;
}

CDlgGlbCfg::~CDlgGlbCfg()
{
}

void CDlgGlbCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBOPORT, m_cbPort);
    DDX_Control(pDX, IDC_COMBOBAUD, m_cbBaud);
    DDX_Control(pDX, IDC_COMBOIMGNUM, m_cbImgNum);
    DDX_Text(pDX, IDC_EDITRESET, m_nReset);
    DDX_Text(pDX, IDC_EDITCOLLECT, m_nCollect);
    DDX_Text(pDX, IDC_EDITSAVEDUR, m_nSave);
    DDX_Text(pDX, IDC_EDITPLUS, m_nPlus);
    DDX_Check(pDX, IDC_CHECKBEHIND, m_bDateBehind);
}


BEGIN_MESSAGE_MAP(CDlgGlbCfg, CDialog)
    ON_WM_CTLCOLOR()
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

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgGlbCfg::SelComboItem(CComboBox* ptCCB, map<int, int> Items, int val)
{
    map<int, int>::iterator it;

    ASSERT_NULL(ptCCB);

    it = find_if(Items.begin(), Items.end(), map_value_finder(val));
    if (it == Items.end() ||
        it->first >= ptCCB->GetCount())
    {
        ptCCB->SetCurSel(0);
    }
    else
    {
        ptCCB->SetCurSel(it->first);
    }
}

BOOL CDlgGlbCfg::GetParaValue(GLBCFG *ptCfg)
{
    CString     strTemp;

    ASSERT_NULL_R(ptCfg, FALSE);

    ptCfg->nPortNO = m_cbPort.GetCurSel() + 1;
    ptCfg->nBaudRate = m_Baud[m_cbBaud.GetCurSel()];
    ptCfg->nImgNum = m_ImgNum[m_cbImgNum.GetCurSel()];

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

    m_cbPort.SetCurSel(ptCfg->nPortNO - 1);
    SelComboItem(&m_cbBaud, m_Baud, ptCfg->nBaudRate);
    SelComboItem(&m_cbImgNum, m_ImgNum, ptCfg->nImgNum);

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
    case IDC_EDITRESET:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), g_tGlbCfg.nResetDur);
        break;
    case IDC_EDITCOLLECT:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), g_tGlbCfg.nCollectDur);
        break;
    case IDC_EDITSAVEDUR:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), g_tGlbCfg.nSaveDur);
        break;

        // 系统配置
    case IDC_COMBOPORT:
        JUDGE_NUMBERPARA_CHANGE((m_cbPort.GetCurSel() + 1), g_tGlbCfg.nPortNO);
        break;
    case IDC_COMBOBAUD:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), g_tGlbCfg.nBaudRate);
        break;
    case IDC_COMBOIMGNUM:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), g_tGlbCfg.nImgNum);
        break;

    default:
        hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
        break;
    }

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}
