// DlgMainShowSet.cpp : implementation file
//

#include "stdafx.h"
#include "Torque.h"
#include "DlgMainShowSet.h"
#include "afxdialogex.h"


//extern CTubeCfg    g_cTubing;
// CDlgMainShowSet dialog

IMPLEMENT_DYNAMIC(CDlgMainShowSet, CDialogEx)

CDlgMainShowSet::CDlgMainShowSet(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DLGMAINSHOW, pParent)
{
    //m_bFixedTube  = TRUE;
}

CDlgMainShowSet::~CDlgMainShowSet()
{
}

void CDlgMainShowSet::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    //DDX_Text(pDX, IDC_MAINPARANAME0, m_strMainShow[0]);
    DDX_Text(pDX, IDC_MAINPARANAME1, m_strMainShow[0]);
    DDX_Text(pDX, IDC_MAINPARANAME2, m_strMainShow[1]);
    DDX_Text(pDX, IDC_MAINPARANAME3, m_strMainShow[2]);
    DDX_Text(pDX, IDC_MAINPARANAME4, m_strMainShow[3]);
    DDX_Text(pDX, IDC_MAINPARANAME5, m_strMainShow[4]);
    DDX_Text(pDX, IDC_MAINPARANAME6, m_strMainShow[5]);
    DDX_Text(pDX, IDC_MAINPARANAME7, m_strMainShow[6]);
    DDX_Text(pDX, IDC_MAINPARANAME8, m_strMainShow[7]);
    //DDX_Control(pDX, IDC_CBMAINPARA0, m_cbMainOption[0]);
    DDX_Control(pDX, IDC_CBMAINPARA1, m_cbMainOption[0]);
    DDX_Control(pDX, IDC_CBMAINPARA2, m_cbMainOption[1]);
    DDX_Control(pDX, IDC_CBMAINPARA3, m_cbMainOption[2]);
    DDX_Control(pDX, IDC_CBMAINPARA4, m_cbMainOption[3]);
    DDX_Control(pDX, IDC_CBMAINPARA5, m_cbMainOption[4]);
    DDX_Control(pDX, IDC_CBMAINPARA6, m_cbMainOption[5]);
    DDX_Control(pDX, IDC_CBMAINPARA7, m_cbMainOption[6]);
    DDX_Control(pDX, IDC_CBMAINPARA8, m_cbMainOption[7]);
    //DDX_CBString(pDX, IDC_CBMAINPARA0, m_strMainOption[0]);
    DDX_CBString(pDX, IDC_CBMAINPARA1, m_strMainOption[0]);
    DDX_CBString(pDX, IDC_CBMAINPARA2, m_strMainOption[1]);
    DDX_CBString(pDX, IDC_CBMAINPARA3, m_strMainOption[2]);
    DDX_CBString(pDX, IDC_CBMAINPARA4, m_strMainOption[3]);
    DDX_CBString(pDX, IDC_CBMAINPARA5, m_strMainOption[4]);
    DDX_CBString(pDX, IDC_CBMAINPARA6, m_strMainOption[5]);
    DDX_CBString(pDX, IDC_CBMAINPARA7, m_strMainOption[6]);
    DDX_CBString(pDX, IDC_CBMAINPARA7, m_strMainOption[7]);
    /*DDX_Text(pDX, IDC_EDMAINPARA52, m_strTubeNO2);
    DDX_Check(pDX, IDC_CKMAINFIXED, m_bFixedTube);
    DDX_Control(pDX, IDC_CBMFIXPARA0, m_cbFixTubingOpt[0]);
    DDX_Control(pDX, IDC_CBMFIXPARA1, m_cbFixTubingOpt[1]);
    DDX_Control(pDX, IDC_CBMFIXPARA2, m_cbFixTubingOpt[2]);
    DDX_Control(pDX, IDC_CBMFIXPARA3, m_cbFixTubingOpt[3]);
    DDX_Control(pDX, IDC_CBMFIXPARA4, m_cbFixTubingOpt[4]);
    DDX_CBString(pDX, IDC_CBMFIXPARA0, m_strFixTubingOpt[0]);
    DDX_CBString(pDX, IDC_CBMFIXPARA1, m_strFixTubingOpt[1]);
    DDX_CBString(pDX, IDC_CBMFIXPARA2, m_strFixTubingOpt[2]);
    DDX_CBString(pDX, IDC_CBMFIXPARA3, m_strFixTubingOpt[3]);
    DDX_CBString(pDX, IDC_CBMFIXPARA4, m_strFixTubingOpt[4]);*/
}


BEGIN_MESSAGE_MAP(CDlgMainShowSet, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDlgMainShowSet::OnBnClickedOk)
    ON_WM_CTLCOLOR()
    //ON_CBN_KILLFOCUS(IDC_CBMAINPARA0, &CDlgMainShowSet::OnCbnKillfocusCbmainpara0)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA1, &CDlgMainShowSet::OnCbnKillfocusCbmainpara1)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA2, &CDlgMainShowSet::OnCbnKillfocusCbmainpara2)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA3, &CDlgMainShowSet::OnCbnKillfocusCbmainpara3)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA4, &CDlgMainShowSet::OnCbnKillfocusCbmainpara4)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA5, &CDlgMainShowSet::OnCbnKillfocusCbmainpara5)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA6, &CDlgMainShowSet::OnCbnKillfocusCbmainpara6)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA7, &CDlgMainShowSet::OnCbnKillfocusCbmainpara7)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA7, &CDlgMainShowSet::OnCbnKillfocusCbmainpara8)
    /*ON_BN_CLICKED(IDC_CKMAINFIXED, &CDlgMainShowSet::OnBnClickedCkmainfixed)
    ON_CBN_KILLFOCUS(IDC_CBMFIXPARA0, &CDlgMainShowSet::OnCbnKillfocusCbmfixpara0)
    ON_CBN_KILLFOCUS(IDC_CBMFIXPARA1, &CDlgMainShowSet::OnCbnKillfocusCbmfixpara1)
    ON_CBN_KILLFOCUS(IDC_CBMFIXPARA2, &CDlgMainShowSet::OnCbnKillfocusCbmfixpara2)
    ON_CBN_KILLFOCUS(IDC_CBMFIXPARA3, &CDlgMainShowSet::OnCbnKillfocusCbmfixpara3)
    ON_CBN_KILLFOCUS(IDC_CBMFIXPARA4, &CDlgMainShowSet::OnCbnKillfocusCbmfixpara4)
    ON_CBN_SELCHANGE(IDC_CBMFIXPARA0, &CDlgMainShowSet::OnCbnSelchangeCbmfixpara0)
    ON_CBN_SELCHANGE(IDC_CBMFIXPARA1, &CDlgMainShowSet::OnCbnSelchangeCbmfixpara1)
    ON_CBN_SELCHANGE(IDC_CBMFIXPARA2, &CDlgMainShowSet::OnCbnSelchangeCbmfixpara2)
    ON_CBN_SELCHANGE(IDC_CBMFIXPARA3, &CDlgMainShowSet::OnCbnSelchangeCbmfixpara3)
    ON_CBN_SELCHANGE(IDC_CBMFIXPARA4, &CDlgMainShowSet::OnCbnSelchangeCbmfixpara4)*/
END_MESSAGE_MAP()


// CDlgMainShowSet message handlers

BOOL CDlgMainShowSet::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_nCurLang = g_tGlbCfg.nLangType;

    InitMainShowPara();

    //InitFixShowPara();

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CDlgMainShowSet::InitMainShowPara()
{
    WORD    i = 0;
    WORD    j = 0;
    WORD    k = 0;
    vector<string> lsOption;

    m_clrNormal  = RGB(0, 0, 0); 
    m_clrChanged = RGB(255, 0, 0);

    //m_bFixedTube = theApp.CheckFixTube(&m_tempCfg);

    for(i=0; i< m_ptShow->nMainNum && i<MAXMAINPARA; i++)
    {
        m_strMainShow[i] = theApp.GetMainShowName(theApp.m_ptCurShow, i).c_str();
        if (i == MAINSHOWWELL)  //入井序号跳过
            continue;

        m_cbMainOption[i].ResetContent();

        lsOption = theDB.GetOptionsByIndex(m_ptShow->nMain[i]);
        for (j = 0; j < lsOption.size(); j++)
        {
            m_cbMainOption[i].AddString(lsOption[j].c_str());
            if (theApp.m_tParaCfg.strValue[m_ptShow->nMain[i]].c_str() == lsOption[j])
            {
                m_cbMainOption[i].SetCurSel(j);
                m_strMainOption[i] = lsOption[j].c_str();
            }
        }
        lsOption.clear();
    }
}

#if 0
void CDlgMainShowSet::InitFixShowPara()
{
    TUBECFG *ptTube = &m_tempCfg.tTubeCfg;
    g_cTubing.SetFactoryBox(&m_cbFixTubingOpt[INDEX_TUBE_FACTORY], ptTube, m_nCurLang);
    g_cTubing.SetOEMBox(&m_cbFixTubingOpt[INDEX_TUBE_OEM], ptTube, m_nCurLang);
    g_cTubing.SetSizeBox(&m_cbFixTubingOpt[INDEX_TUBE_SIZE], ptTube, m_nCurLang);
    g_cTubing.SetMatBox(&m_cbFixTubingOpt[INDEX_TUBE_MATER], ptTube, m_nCurLang);
    g_cTubing.SetCouplBox(&m_cbFixTubingOpt[INDEX_TUBE_COUPL], ptTube, m_nCurLang);

    ShowTubingCtrl();
}

void CDlgMainShowSet::ShowTubingCtrl()
{
    //m_cbMainFactory.ShowWindow(!m_bFixedTubing);
    m_cbMainOption[INDEX_TUBE_FACTORY].ShowWindow(!m_bFixedTube);
    m_cbMainOption[INDEX_TUBE_OEM].ShowWindow(!m_bFixedTube);
    m_cbMainOption[INDEX_TUBE_SIZE].ShowWindow(!m_bFixedTube);
    m_cbMainOption[INDEX_TUBE_MATER].ShowWindow(!m_bFixedTube);
    m_cbMainOption[INDEX_TUBE_COUPL].ShowWindow(!m_bFixedTube);

    //m_cbMFixFactory.ShowWindow(m_bFixedTubing);
    m_cbFixTubingOpt[INDEX_TUBE_FACTORY].ShowWindow(m_bFixedTube);
    m_cbFixTubingOpt[INDEX_TUBE_OEM].ShowWindow(m_bFixedTube);
    m_cbFixTubingOpt[INDEX_TUBE_SIZE].ShowWindow(m_bFixedTube);
    m_cbFixTubingOpt[INDEX_TUBE_MATER].ShowWindow(m_bFixedTube);
    m_cbFixTubingOpt[INDEX_TUBE_COUPL].ShowWindow(m_bFixedTube);
}
#endif
BOOL CDlgMainShowSet::GetParaValue()
{
    UINT        i = 0;
    UINT        j = 0;
    UINT        nFactory = 0;
        
    for (i = 0; i<= m_ptShow->nMainNum && i<MAXMAINPARA; i++)
    {
        if(i == MAINSHOWWELL)  // 第6个为入井序号，不需要使用combobox
            continue;
        m_tempCfg.strValue[m_ptShow->nMain[i]] = m_strMainOption[i];
    }
#if 0
    if (m_bFixedTube)
    {
        nFactory = m_cbFixTubingOpt[INDEX_TUBE_FACTORY].GetCurSel();
        for (i = 0; i < MAXTUBECFGNUM; i++)
        {
            m_tempCfg.tTubeCfg.nFixTube[i] = g_cTubing.GetTubeNOByName(i, nFactory, &m_cbFixTubingOpt[i], m_nCurLang);
        }
        theDB.ReadTubeInfo(&m_tempCfg.tTubeCfg, FALSE);
    }
    else
    {
        memset(&m_tempCfg.tTubeCfg, 0, sizeof(TUBECFG));
    }
#endif
    return TRUE;
}

void CDlgMainShowSet::OnBnClickedOk()
{
    UpdateData(TRUE);

    GetParaValue();

    theDB.UpdateTorqCfgPara(&m_tempCfg, theApp.m_ptCurShow);

    UpdateData(FALSE);

    CDialogEx::OnOK();
}

HBRUSH CDlgMainShowSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    //COLORREF    clrCtrl;
    CString     strContent;

    GetDlgItemText(pWnd->GetDlgCtrlID(), strContent);
    if (strContent.IsEmpty())
        return hbr;

    // TODO:  在此更改 DC 的任何特性
    //switch (pWnd->GetDlgCtrlID())//对某一个特定控件进行判断  
    //{
    ///*case IDC_EDMAINPARA52:
    //    JUDGE_STRPARA_CHANGE(strContent, m_strTubeNO2);
    //    break;*/
    //default:
    //    hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    //    break;
    //}
    hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void CDlgMainShowSet::JudgeMainShowParaChanged(UINT nIndex, CString strContent)
{
    COLORREF    clrCtrl;

    COMP_BGE(nIndex, MAXMAINPARA);
    COMP_BGE(nIndex, m_ptShow->nMainNum);

    clrCtrl = m_clrNormal;
    if (strContent.Compare(m_strMainOption[nIndex]) != 0)
        clrCtrl = m_clrChanged;
    m_cbMainOption[nIndex].m_ColorText = clrCtrl;
    m_cbMainOption[nIndex].Invalidate();
}

//void CDlgMainShowSet::OnCbnKillfocusCbmainpara0()
//{
//    CString     strContent;
//    GetDlgItemText(IDC_CBMAINPARA0, strContent);
//    JudgeMainShowParaChanged(0, strContent);
//}

void CDlgMainShowSet::OnCbnKillfocusCbmainpara1()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA1, strContent);
    JudgeMainShowParaChanged(0, strContent);
}
void CDlgMainShowSet::OnCbnKillfocusCbmainpara2()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA2, strContent);
    JudgeMainShowParaChanged(1, strContent);
}
void CDlgMainShowSet::OnCbnKillfocusCbmainpara3()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA3, strContent);
    JudgeMainShowParaChanged(2, strContent);
}
void CDlgMainShowSet::OnCbnKillfocusCbmainpara4()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA4, strContent);
    JudgeMainShowParaChanged(3, strContent);
}
void CDlgMainShowSet::OnCbnKillfocusCbmainpara5()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA5, strContent);
    JudgeMainShowParaChanged(4, strContent);
}
void CDlgMainShowSet::OnCbnKillfocusCbmainpara6()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA6, strContent);
    JudgeMainShowParaChanged(5, strContent);
}
void CDlgMainShowSet::OnCbnKillfocusCbmainpara7()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA7, strContent);
    JudgeMainShowParaChanged(6, strContent);
}
void CDlgMainShowSet::OnCbnKillfocusCbmainpara8()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA8, strContent);
    JudgeMainShowParaChanged(7, strContent);
}
#if 0
void CDlgMainShowSet::OnCbnKillfocusCbmfixpara0()
{
    JudgeFixShowParaChanged(INDEX_TUBE_FACTORY, &m_tempCfg.tTubeCfg);
}

void CDlgMainShowSet::OnCbnKillfocusCbmfixpara1()
{
    JudgeFixShowParaChanged(INDEX_TUBE_OEM, &m_tempCfg.tTubeCfg);
}

void CDlgMainShowSet::OnCbnKillfocusCbmfixpara2()
{
    JudgeFixShowParaChanged(INDEX_TUBE_SIZE, &m_tempCfg.tTubeCfg);
}

void CDlgMainShowSet::OnCbnKillfocusCbmfixpara3()
{
    JudgeFixShowParaChanged(INDEX_TUBE_MATER, &m_tempCfg.tTubeCfg);
}

void CDlgMainShowSet::OnCbnKillfocusCbmfixpara4()
{
    JudgeFixShowParaChanged(INDEX_TUBE_COUPL, &m_tempCfg.tTubeCfg);
}

void CDlgMainShowSet::OnCbnSelchangeCbmfixpara0()
{
    ChangeTubingShowBox(INDEX_TUBE_FACTORY);
}

void CDlgMainShowSet::OnCbnSelchangeCbmfixpara1()
{
    ChangeTubingShowBox(INDEX_TUBE_OEM);
}

void CDlgMainShowSet::OnCbnSelchangeCbmfixpara2()
{
    ChangeTubingShowBox(INDEX_TUBE_SIZE);
}

void CDlgMainShowSet::OnCbnSelchangeCbmfixpara3()
{
    ChangeTubingShowBox(INDEX_TUBE_MATER);
}

void CDlgMainShowSet::OnCbnSelchangeCbmfixpara4()
{
    ChangeTubingShowBox(INDEX_TUBE_COUPL);
}

void CDlgMainShowSet::JudgeFixShowParaChanged(UINT nIndex, TUBECFG* ptTubing)
{
    COLORREF    clrCtrl;
    TUBECFG   *ptOldTubing = &theApp.m_tParaCfg.tTubeCfg;

    COMP_BL(nIndex, INDEX_TUBE_FACTORY);
    COMP_BG(nIndex, INDEX_TUBE_COUPL);
    ASSERT_NULL(ptTubing);

    clrCtrl = m_clrNormal;

    if (theApp.IsFixTube() != m_bFixedTube)
        clrCtrl = m_clrChanged;
    else if(ptOldTubing->nFixTube[nIndex] != ptTubing->nFixTube[nIndex])
        clrCtrl = m_clrChanged;

    m_cbFixTubingOpt[nIndex].m_ColorText = clrCtrl;
    m_cbFixTubingOpt[nIndex].Invalidate();
}

void CDlgMainShowSet::ChangeTubingShowBox(UINT nIndex)
{
    TUBECFG tTmpTub = { 0 };
    UINT    nCurNO = 0;
    int    i = 0;

    COMP_BL(nIndex, INDEX_TUBE_FACTORY);
    COMP_BG(nIndex, INDEX_TUBE_COUPL);

    // 获取和设置油管5个参数值
    nCurNO = g_cTubing.GetTubeNOByName(nIndex, m_tempCfg.tTubeCfg.nFixTube[INDEX_TUBE_FACTORY], &m_cbFixTubingOpt[nIndex], m_nCurLang);
    if (!g_cTubing.CheckTubeNOChanged(nIndex, nCurNO, &m_tempCfg.tTubeCfg, &tTmpTub))
        return;
    /* 后续序号从m_tCurTub获取 */
    for (i = nIndex - 1; i >= 0; i--)
    {
        tTmpTub.nFixTube[i] = m_tempCfg.tTubeCfg.nFixTube[i];
    }

    // 初始化COMBOBOX内容，有变化则显示红色
    switch (nIndex)
    {
    case INDEX_TUBE_FACTORY:
        g_cTubing.InitOEMValue(&tTmpTub);
        g_cTubing.SetFactoryBox(&m_cbFixTubingOpt[INDEX_TUBE_FACTORY], &tTmpTub, m_nCurLang);
        JudgeFixShowParaChanged(INDEX_TUBE_FACTORY, &tTmpTub);

    case INDEX_TUBE_OEM:
        g_cTubing.InitSizeValue(&tTmpTub);
        g_cTubing.SetOEMBox(&m_cbFixTubingOpt[INDEX_TUBE_OEM], &tTmpTub, m_nCurLang);
        JudgeFixShowParaChanged(INDEX_TUBE_OEM, &tTmpTub);

    case INDEX_TUBE_SIZE:
        g_cTubing.InitMatValue(&tTmpTub);
        g_cTubing.SetSizeBox(&m_cbFixTubingOpt[INDEX_TUBE_SIZE], &tTmpTub, m_nCurLang);
        JudgeFixShowParaChanged(INDEX_TUBE_SIZE, &tTmpTub);

    case INDEX_TUBE_MATER:
        g_cTubing.InitCouplValue(&tTmpTub);
        g_cTubing.SetMatBox(&m_cbFixTubingOpt[INDEX_TUBE_MATER], &tTmpTub, m_nCurLang);
        JudgeFixShowParaChanged(INDEX_TUBE_MATER, &tTmpTub);

    case INDEX_TUBE_COUPL:
        g_cTubing.SetCouplBox(&m_cbFixTubingOpt[INDEX_TUBE_COUPL], &tTmpTub, m_nCurLang);
        JudgeFixShowParaChanged(INDEX_TUBE_COUPL, &tTmpTub);
    default:
        break;
    }

    ShowTubingCtrl();

    m_tempCfg.tTubeCfg = tTmpTub;
}

void CDlgMainShowSet::OnBnClickedCkmainfixed()
{
    int i = 0;

    UpdateData(TRUE);
    ShowTubingCtrl();

    /* 自选管材转固定管材 */
    if (m_bFixedTube && !theApp.IsFixTube())
    {
        ChangeTubingShowBox(INDEX_TUBE_FACTORY);
    }
    
    /* 固定管材转自选管材 */
    if(!m_bFixedTube && theApp.IsFixTube())
    {
        for(i=MAINSHOWBEGIN; i<=MAINSHOWEND; i++)
        {
            m_cbMainOption[i].m_ColorText = m_clrChanged;
            m_cbMainOption[i].Invalidate();
        }
    }
    UpdateData(FALSE);
}
#endif
