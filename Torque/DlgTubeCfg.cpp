// DlgTubeCfg.cpp : implementation file
//

#include "stdafx.h"
#include "Torque.h"
#include "DlgTubeCfg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTubeCfg        g_cTubing;

/////////////////////////////////////////////////////////////////////////////
// CDlgTubeCfg dialog


CDlgTubeCfg::CDlgTubeCfg(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTubeCfg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgCollect)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    for (int i = 0; i < MAXTUBETORQNUM; i++)
    {
        m_strTorqVal[i] = NULLSTR;
    }

    m_strKindNORange = _T("(0-255)");
}

CDlgTubeCfg::~CDlgTubeCfg()
{
}

void CDlgTubeCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgTubeCfg)
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_CBTYPEKIND, m_cbTypeKind);
    DDX_Control(pDX, IDC_CBFACTORYNAME, m_cbFactoryName);
    DDX_Control(pDX, IDC_CBKINDNO, m_cbKindNO);
    DDX_Control(pDX, IDC_EDITKINDNAME1, m_eKindName[0]);
    DDX_Control(pDX, IDC_EDITKINDNAME2, m_eKindName[1]);
    DDX_Control(pDX, IDC_EDITKINDNAME3, m_eKindName[2]);
    DDX_Text(pDX, IDC_EDITKINDNAME1, m_strKindName[0]);
    DDX_Text(pDX, IDC_EDITKINDNAME2, m_strKindName[1]);
    DDX_Text(pDX, IDC_EDITKINDNAME3, m_strKindName[2]);
    DDX_Control(pDX, IDC_CBFACTORY, m_cbFixTubingOpt[0]);
    DDX_Control(pDX, IDC_CBOEM, m_cbFixTubingOpt[1]);
    DDX_Control(pDX, IDC_CBTUBINGSIZE, m_cbFixTubingOpt[2]);
    DDX_Control(pDX, IDC_CBMATERIAL, m_cbFixTubingOpt[3]);
    DDX_Control(pDX, IDC_CBCOUPLING, m_cbFixTubingOpt[4]);
    DDX_CBString(pDX, IDC_CBFACTORY, m_strFixTubingOpt[0]);
    DDX_CBString(pDX, IDC_CBOEM, m_strFixTubingOpt[1]);
    DDX_CBString(pDX, IDC_CBTUBINGSIZE, m_strFixTubingOpt[2]);
    DDX_CBString(pDX, IDC_CBMATERIAL, m_strFixTubingOpt[3]);
    DDX_CBString(pDX, IDC_CBCOUPLING, m_strFixTubingOpt[4]);
    DDX_Control(pDX, IDC_EDITTUBEMAXTORQ, m_neTorqVal[0]);
    DDX_Control(pDX, IDC_EDITTUBEOPTTORQ, m_neTorqVal[1]);
    DDX_Control(pDX, IDC_EDITTUBEMINTORQ, m_neTorqVal[2]);
    DDX_Control(pDX, IDC_EDITTUBEMAXTORQLB, m_neTorqVal[3]);
    DDX_Control(pDX, IDC_EDITTUBEOPTTORQLB, m_neTorqVal[4]);
    DDX_Control(pDX, IDC_EDITTUBEMINTORQLB, m_neTorqVal[5]);
    DDX_Text(pDX, IDC_EDITTUBEMAXTORQ, m_strTorqVal[0]);
    DDX_Text(pDX, IDC_EDITTUBEOPTTORQ, m_strTorqVal[1]);
    DDX_Text(pDX, IDC_EDITTUBEMINTORQ, m_strTorqVal[2]);
    DDX_Text(pDX, IDC_EDITTUBEMAXTORQLB, m_strTorqVal[3]);
    DDX_Text(pDX, IDC_EDITTUBEOPTTORQLB, m_strTorqVal[4]);
    DDX_Text(pDX, IDC_EDITTUBEMINTORQLB, m_strTorqVal[5]);
    DDX_Text(pDX, IDC_KINDNORANGE, m_strKindNORange);
}


BEGIN_MESSAGE_MAP(CDlgTubeCfg, CDialog)
    //{{AFX_MSG_MAP(CDlgTubeCfg)
    //}}AFX_MSG_MAP
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_ADDTUBEKIND, &CDlgTubeCfg::OnBnClickedAddtubekind)
    ON_BN_CLICKED(IDC_MODTUBEKIND, &CDlgTubeCfg::OnBnClickedModtubekind)
    //ON_BN_CLICKED(IDC_DELTUBEKIND, &CDlgTubeCfg::OnBnClickedDeltubekind)
    ON_BN_CLICKED(IDC_ADDTUBETORQ, &CDlgTubeCfg::OnBnClickedAddtubetorq)
    ON_BN_CLICKED(IDC_DELTUBETORQ, &CDlgTubeCfg::OnBnClickedDeltubetorq)
    ON_BN_CLICKED(IDC_MODTUBETORQ, &CDlgTubeCfg::OnBnClickedModtubetorq)
    ON_CBN_SELCHANGE(IDC_CBTYPEKIND, &CDlgTubeCfg::OnCbnSelchangeCbtypekind)
    ON_CBN_KILLFOCUS(IDC_CBTYPEKIND, &CDlgTubeCfg::OnCbnKillfocusCbtypekind)
    ON_CBN_SELCHANGE(IDC_CBFACTORYNAME, &CDlgTubeCfg::OnCbnSelchangeCbfactoryname)
    ON_CBN_KILLFOCUS(IDC_CBFACTORYNAME, &CDlgTubeCfg::OnCbnKillfocusCbfactoryname)
    ON_CBN_SELCHANGE(IDC_CBKINDNO, &CDlgTubeCfg::OnCbnSelchangeCbkindno)
    ON_CBN_KILLFOCUS(IDC_CBKINDNO, &CDlgTubeCfg::OnCbnKillfocusCbkindno)
    ON_CBN_SELCHANGE(IDC_CBFACTORY, &CDlgTubeCfg::OnCbnSelchangeCbfactory)
    ON_CBN_KILLFOCUS(IDC_CBFACTORY, &CDlgTubeCfg::OnCbnKillfocusCbfactory)
    ON_CBN_SELCHANGE(IDC_CBOEM, &CDlgTubeCfg::OnCbnSelchangeCboem)
    ON_CBN_KILLFOCUS(IDC_CBOEM, &CDlgTubeCfg::OnCbnKillfocusCboem)
    ON_CBN_SELCHANGE(IDC_CBTUBINGSIZE, &CDlgTubeCfg::OnCbnSelchangeCbtubingsize)
    ON_CBN_KILLFOCUS(IDC_CBTUBINGSIZE, &CDlgTubeCfg::OnCbnKillfocusCbtubingsize)
    ON_CBN_SELCHANGE(IDC_CBMATERIAL, &CDlgTubeCfg::OnCbnSelchangeCbmaterial)
    ON_CBN_KILLFOCUS(IDC_CBMATERIAL, &CDlgTubeCfg::OnCbnKillfocusCbmaterial)
    ON_CBN_SELCHANGE(IDC_CBCOUPLING, &CDlgTubeCfg::OnCbnSelchangeCbcoupling)
    ON_CBN_KILLFOCUS(IDC_CBCOUPLING, &CDlgTubeCfg::OnCbnKillfocusCbcoupling)
    ON_EN_KILLFOCUS(IDC_EDITTUBEMAXTORQ, &CDlgTubeCfg::OnEnKillfocusEdittubemaxtorq)
    ON_EN_KILLFOCUS(IDC_EDITTUBEOPTTORQ, &CDlgTubeCfg::OnEnKillfocusEdittubeopttorq)
    ON_EN_KILLFOCUS(IDC_EDITTUBEMINTORQ, &CDlgTubeCfg::OnEnKillfocusEdittubemintorq)
END_MESSAGE_MAP()


// CDlgTubeCfg message handlers

BOOL CDlgTubeCfg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_clrNormal = RGB(0, 0, 0);
    m_clrChanged = RGB(255, 0, 0);
    m_nCurLang = g_tGlbCfg.nLangType;

    // for 选项名称
    InitTubeVariable();
    InitFixTypeKind();
    InitFixFactoryName();
    InitFixKindNO();
    InitFixKindName();

    // for 扭矩配置
    //m_ptDBTubeCfg = &theApp.m_tDBTubeCfg;
    // ComboBox之间没有相对限制，没有对应的值可以增加扭矩配置
    InitTorqShow(&m_tSrcTub);
    m_tCurTub = m_tSrcTub;
    SetTorqValue(&m_tSrcTub);

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTubeCfg::ReportOperResult(UINT nBtnID, BOOL bSucc)
{
    string strInfo;
    CString strOper;

    GetDlgItem(nBtnID)->GetWindowText(strOper);
    if (bSucc)
        strInfo = theApp.LoadstringFromRes(IDS_STROPERATESUCC, strOper.GetBuffer());
    else
        strInfo = theApp.LoadstringFromRes(IDS_STROPERATEFAIL, strOper.GetBuffer());
    AfxMessageBox(strInfo.c_str());
}

void CDlgTubeCfg::OnBnClickedAddtubekind()
{
    UINT i = 0;
    BOOL bRes = FALSE;
    vector<string> strKindName;

    GetCurTubeInfo();

    if (!CheckTubeInfo(FALSE, FALSE))
    {
        AfxMessageBox(theApp.LoadstringFromRes(IDS_STRINVALIDPARA).c_str());
        return;
    }

    for (i = 0; i < LANGUAGE_NUM; i++)
        strKindName.push_back(m_strKindName[i].GetBuffer(0));
    bRes = theDB.InsertTubeName(m_iTubeKind, m_iKindNO, strKindName);
    if (bRes)
    {
        if (INDEX_TUBE_FACTORY == m_iTubeKind)
        {
            InitFixFactoryName();
        }

        InitFixKindNO();

        m_cbKindNO.SetFocus();
        /* 修改焦点，触发edit控件修改颜色 */
        for (i = 0; i < LANGUAGE_NUM; i++)
            m_eKindName[i].SetFocus();

        // 更新扭矩侧的combobox的选项
        SetTorqShow(&m_tCurTub);

        UpdateData(FALSE);
    }

    ReportOperResult(IDC_ADDTUBEKIND, bRes);
}

void CDlgTubeCfg::OnBnClickedModtubekind()
{
    UINT i = 0;
    BOOL bRes = FALSE;
    vector<string> strKindName;

    GetCurTubeInfo();

    if (!CheckTubeInfo(TRUE, FALSE))
    {
        AfxMessageBox(theApp.LoadstringFromRes(IDS_STRINVALIDPARA).c_str());
        return;
    }

    for (i = 0; i < LANGUAGE_NUM; i++)
        strKindName.push_back(m_strKindName[i].GetBuffer(0));
    bRes = theDB.UpdateTubeName(m_iTubeKind, m_iKindNO, strKindName);
    if (bRes)
    {
        if (INDEX_TUBE_FACTORY == m_iTubeKind)
        {
            InitFixFactoryName();
        }

        /* 修改焦点，触发edit控件修改颜色 */
        for (i = 0; i < LANGUAGE_NUM; i++)
            m_eKindName[i].SetFocus();

        // 更新扭矩侧的combobox的选项
        SetTorqShow(&m_tCurTub);

        UpdateData(FALSE);
    }
    ReportOperResult(IDC_MODTUBEKIND, bRes);
}

/*
void CDlgTubeCfg::OnBnClickedDeltubekind()
{
    BOOL bRes = FALSE;

    GetCurTubeInfo();

    if (!CheckTubeInfo(TRUE, TRUE))
    {
        return;
    }

    bRes = m_ptDBTubeText->DeleteNO(m_iKindNO);
    if (bRes)
    {
        m_ptDBTubeText->Reload();
        theDB.LoadTubingInfo();

        if (INDEX_TUBE_FACTORY == m_iTubeKind)
        {
            InitFixFactoryName();
        }

        m_iKindNO = 0;

        InitFixKindNO();
        InitFixKindName();

        // 更新扭矩侧的combobox的选项
        SetTorqShow(&m_tCurTub);

        UpdateData(FALSE);
    }
    ReportOperResult(IDC_DELTUBEKIND, bRes);
}
*/
BOOL CDlgTubeCfg::CheckTubeInfo(BOOL bNeedExist, BOOL bDel)
{
    int i = 0;
    BOOL bExistNO = FALSE;

    bExistNO = theDB.CheckTubeNO(m_iTubeKind, m_iKindNO);

    if ((bNeedExist && !bExistNO) ||
        (!bNeedExist && bExistNO))
        return FALSE;

    if (bDel)
        return TRUE;

    if (m_iKindNO < m_iKindLow || m_iKindNO > m_iKindHigh)
        return FALSE;

    for (i = 0; i < LANGUAGE_NUM; i++)
    {
        if (m_strKindName[i].IsEmpty())
            return FALSE;
    }
    return TRUE;
}

void CDlgTubeCfg::EnableFactoryCtrl(BOOL bEnable)
{
    m_cbFactoryName.EnableWindow(bEnable);
    //GetDlgItem(IDC_DELTUBEKIND)->EnableWindow(bEnable);
}

void CDlgTubeCfg::EnableKindCtrl(BOOL bExist)
{
    GetDlgItem(IDC_ADDTUBEKIND)->EnableWindow(!bExist);
    GetDlgItem(IDC_MODTUBEKIND)->EnableWindow(bExist);
    //GetDlgItem(IDC_DELTUBEKIND)->EnableWindow(bExist);
}

void CDlgTubeCfg::InitTubeVariable()
{
    m_iTubeKind = 0;
    m_iTubeFactory = 0;
    m_iKindLow = 0;
    m_iKindHigh = 255;
    m_iKindNO = 0;
    m_iNOIndex = 0;

    m_ptTubeInfo = &g_cTubing.m_tTubInfo[0];
    //m_ptDBTubeText = &theApp.m_tDBTubeText[0];
}

void CDlgTubeCfg::GetTubeKindNO()
{
    UINT i = 0;
    CString strTemp;
    
    strTemp = GetCCBString(&m_cbKindNO);
    m_iKindNO = atoi(strTemp);

    m_iNOIndex = -1;
    for (i = 0; i < m_ptTubeInfo->nNum; i++)
    {
        if (m_iKindNO == m_ptTubeInfo->ptPara[i].nNO)
        {
            m_iNOIndex = i;
            break;
        }
    }
}

void CDlgTubeCfg::GetCurTubeInfo()
{
    CString strTemp;

    UpdateData(TRUE);

    m_iTubeKind = m_cbTypeKind.GetCurSel();
    if (INDEX_TUBE_FACTORY == m_iTubeKind)
        m_iTubeFactory = 0;
    else
        m_iTubeFactory = m_cbFactoryName.GetCurSel();
    m_ptTubeInfo = &g_cTubing.m_tTubInfo[m_iTubeKind];
    //m_ptDBTubeText = &theApp.m_tDBTubeText[m_iTubeKind];

    m_iKindLow = 256 * m_iTubeFactory;
    m_iKindHigh = m_iKindLow + 255;
    m_strKindNORange.Format("(%d-%d)", m_iKindLow, m_iKindHigh);

    GetTubeKindNO();
}

void CDlgTubeCfg::InitFixTypeKind()
{
    m_cbTypeKind.ResetContent();
    m_cbTypeKind.AddString(theApp.m_ptCurShow->strShow[INDEX_SHOW_FACTORY].c_str());
    m_cbTypeKind.AddString(theApp.m_ptCurShow->strShow[INDEX_SHOW_OEM].c_str());
    m_cbTypeKind.AddString(theApp.m_ptCurShow->strShow[INDEX_SHOW_SIZE].c_str());
    m_cbTypeKind.AddString(theApp.m_ptCurShow->strShow[INDEX_SHOW_MATER].c_str());
    m_cbTypeKind.AddString(theApp.m_ptCurShow->strShow[INDEX_SHOW_COUPL].c_str());
    m_cbTypeKind.SetCurSel(0);
    m_iTubeKind = 0;
    EnableFactoryCtrl(FALSE);

    UpdateData(FALSE);
}

void CDlgTubeCfg::InitFixFactoryName()
{
    UINT i = 0;
    FIXTUBEINFO* pFixTub = &g_cTubing.m_tTubInfo[INDEX_TUBE_FACTORY];

    m_cbFactoryName.ResetContent();
    for (i = 0; i < pFixTub->nNum; i++)
    {
        //m_cbFactoryName.AddString(pFixTub->ptPara[i].aucName[m_nCurLang]);
        m_cbFactoryName.AddString(pFixTub->ptPara[i].strName[m_nCurLang].c_str());
    }
    m_cbFactoryName.SetCurSel(0);
    m_iTubeFactory = 0;
    m_cbFactoryName.EnableWindow(FALSE);
}

void CDlgTubeCfg::InitFixKindNO()
{
    UINT i = 0;
    int  iNO = 0;
    int  iCurSel = 0;

    m_cbKindNO.ResetContent();
    for (i = 0; i < m_ptTubeInfo->nNum; i++)
    {
        iNO = m_ptTubeInfo->ptPara[i].nNO;
        if (iNO < m_iKindLow || iNO > m_iKindHigh)
            continue;
        if (iNO == m_iKindNO)
            iCurSel = m_cbKindNO.GetCount();
        m_cbKindNO.AddString(to_string(iNO).c_str());
    }
    m_cbKindNO.SetCurSel(iCurSel);

    GetTubeKindNO();
}

void CDlgTubeCfg::SetKindName(int iIndex)
{
    int i = 0;
    FIXTUBEPARA* ptPara = NULL;

    if (iIndex >= 0)
        ptPara = &m_ptTubeInfo->ptPara[iIndex];

    if (ptPara == NULL)
    {
        for (i = 0; i < LANGUAGE_NUM; i++)
            m_strKindName[i].Empty();

        EnableKindCtrl(FALSE);
        return;
    }

    EnableKindCtrl(TRUE);
    for (i = 0; i < LANGUAGE_NUM; i++)
        m_strKindName[i] = ptPara->strName[i].c_str();
}

void CDlgTubeCfg::InitFixKindName()
{
    if (m_cbKindNO.GetCount() < 1)
    {
        SetKindName(-1);
        return;
    }

    SetKindName(m_iNOIndex);
}

void CDlgTubeCfg::OnCbnSelchangeCbtypekind()
{
    GetCurTubeInfo();
    EnableFactoryCtrl(FALSE);
    if (INDEX_TUBE_FACTORY != m_cbTypeKind.GetCurSel())
    {
        EnableFactoryCtrl(TRUE);
    }
    InitFixKindNO();
    InitFixKindName();

    UpdateData(FALSE);
}

void CDlgTubeCfg::OnCbnSelchangeCbfactoryname()
{
    GetCurTubeInfo();
    InitFixKindNO();
    InitFixKindName();

    UpdateData(FALSE);
}

void CDlgTubeCfg::OnCbnSelchangeCbkindno()
{
    GetCurTubeInfo();

    InitFixKindName();

    UpdateData(FALSE);
}

void CDlgTubeCfg::OnCbnKillfocusCbkindno()
{
    COLORREF    clrCtrl;
    GetCurTubeInfo();

    InitFixKindName();

    // 不存在的NO，设置颜色为红色
    clrCtrl = m_clrNormal;
    if (!theDB.CheckTubeNO(m_iTubeKind, m_iKindNO))
    {
        clrCtrl = m_clrChanged;
    }
    m_cbKindNO.m_ColorText = clrCtrl;
    m_cbKindNO.Invalidate();

    UpdateData(FALSE);
}

HBRUSH CDlgTubeCfg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    COLORREF    clrCtrl;
    CString     strContent;
    CString     strOld = "";

    GetDlgItemText(pWnd->GetDlgCtrlID(), strContent);
    if (strContent.IsEmpty())
        return hbr;

    switch (pWnd->GetDlgCtrlID())//对某一个特定控件进行判断
    {
    case IDC_EDITKINDNAME1:
        if (m_iNOIndex >= 0)
            strOld = m_ptTubeInfo->ptPara[m_iNOIndex].strName[0].c_str();
        JUDGE_STRPARA_CHANGE(strContent, strOld);
        break;
    case IDC_EDITKINDNAME2:
        if (m_iNOIndex >= 0)
            strOld = m_ptTubeInfo->ptPara[m_iNOIndex].strName[1].c_str();
        JUDGE_STRPARA_CHANGE(strContent, strOld);
        break;
    case IDC_EDITKINDNAME3:
        if (m_iNOIndex >= 0)
            strOld = m_ptTubeInfo->ptPara[m_iNOIndex].strName[2].c_str();
        JUDGE_STRPARA_CHANGE(strContent, strOld);
        break;

    case IDC_EDITTUBEMAXTORQ:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_tSrcTub.nTorqVal[INDEX_TUBE_MAXTORQ]);
        break;
    case IDC_EDITTUBEOPTTORQ:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_tSrcTub.nTorqVal[INDEX_TUBE_OPTTORQ]);
        break;
    case IDC_EDITTUBEMINTORQ:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_tSrcTub.nTorqVal[INDEX_TUBE_MINTORQ]);
        break;
    case IDC_EDITTUBEMAXTORQLB:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_tSrcTub.nTorqVal[INDEX_TUBE_LBMAXTORQ]);
        break;
    case IDC_EDITTUBEOPTTORQLB:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_tSrcTub.nTorqVal[INDEX_TUBE_LBOPTTORQ]);
        break;
    case IDC_EDITTUBEMINTORQLB:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), m_tSrcTub.nTorqVal[INDEX_TUBE_LBMINTORQ]);
        break;

    default:
        hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
        break;
    }
    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void CDlgTubeCfg::SelchangeComboBox(UINT nIndex)
{
    COMP_BFALSE(g_cTubing.CheckIndex(nIndex));

    ChangeTubingShowBox(nIndex);

    GetCurTorqInfo();

    SetTorqValue(&m_tCurTub);
}

void CDlgTubeCfg::OnCbnSelchangeCbfactory()
{
    UpdateData(TRUE);

    SelchangeComboBox(INDEX_TUBE_FACTORY);

    UpdateData(FALSE);
}
void CDlgTubeCfg::OnCbnSelchangeCboem()
{
    UpdateData(TRUE);

    SelchangeComboBox(INDEX_TUBE_OEM);

    UpdateData(FALSE);
}
void CDlgTubeCfg::OnCbnSelchangeCbtubingsize()
{
    UpdateData(TRUE);

    SelchangeComboBox(INDEX_TUBE_SIZE);

    UpdateData(FALSE);
}
void CDlgTubeCfg::OnCbnSelchangeCbmaterial()
{
    UpdateData(TRUE);

    SelchangeComboBox(INDEX_TUBE_MATER);

    UpdateData(FALSE);
}
void CDlgTubeCfg::OnCbnSelchangeCbcoupling()
{
    UpdateData(TRUE);

    SelchangeComboBox(INDEX_TUBE_COUPL);

    UpdateData(FALSE);
}

void CDlgTubeCfg::UpdateLBFTTorq(UINT nIndex)
{
    COMP_BGE(nIndex, HALFTUBETORQNUM);

    int oldTorq = atoi(m_strTorqVal[nIndex]);
    int newTorq = 0;
    UpdateData(TRUE);

    newTorq = atoi(m_strTorqVal[nIndex]);
    /* 值没有修改则直接返回 */
    COMP_BE(oldTorq, newTorq);

    SET_STRING_VALUE(m_strTorqVal[nIndex+ HALFTUBETORQNUM], int(newTorq * NM2LBFT));
    UpdateData(FALSE);
}

void CDlgTubeCfg::OnEnKillfocusEdittubemaxtorq()
{
    UpdateLBFTTorq(0);
}

void CDlgTubeCfg::OnEnKillfocusEdittubeopttorq()
{
    UpdateLBFTTorq(1);
}

void CDlgTubeCfg::OnEnKillfocusEdittubemintorq()
{
    UpdateLBFTTorq(2);
}

void CDlgTubeCfg::OnCbnKillfocusCbtypekind()
{
    // TODO: Add your control notification handler code here
}

void CDlgTubeCfg::OnCbnKillfocusCbfactoryname()
{
    // TODO: Add your control notification handler code here
}

void CDlgTubeCfg::OnCbnKillfocusCbfactory()
{
    JudgeTorqCBParaChanged(INDEX_TUBE_FACTORY, &m_tCurTub);
}

void CDlgTubeCfg::OnCbnKillfocusCboem()
{
    JudgeTorqCBParaChanged(INDEX_TUBE_OEM, &m_tCurTub);
}

void CDlgTubeCfg::OnCbnKillfocusCbtubingsize()
{
    JudgeTorqCBParaChanged(INDEX_TUBE_SIZE, &m_tCurTub);
}

void CDlgTubeCfg::OnCbnKillfocusCbmaterial()
{
    JudgeTorqCBParaChanged(INDEX_TUBE_MATER, &m_tCurTub);
}

void CDlgTubeCfg::OnCbnKillfocusCbcoupling()
{
    JudgeTorqCBParaChanged(INDEX_TUBE_COUPL, &m_tCurTub);
}

void CDlgTubeCfg::OnBnClickedAddtubetorq()
{
    UINT i = 0;
    BOOL bRes = FALSE;

    UpdateData(TRUE);

    GetCurTorqInfo();

    if (!GetTorqValue(&m_tCurTub))
    {
        AfxMessageBox(theApp.LoadstringFromRes(IDS_STRINVALIDTORQ).c_str());
        return;
    }

    if(theDB.InsertTubeCfg(&m_tCurTub)> 0)
    {
        m_tSrcTub = m_tCurTub;
        SetTorqValue(&m_tCurTub);

        /* 改变combobox颜色 */
        for (i = 0; i < MAXTUBECFGNUM; i++)
            JudgeTorqCBParaChanged(i, &m_tCurTub);

        /* 修改焦点，触发edit控件修改颜色 */
        for (i = 0; i < MAXTUBETORQNUM; i++)
            m_neTorqVal[i].SetFocus();
        UpdateData(FALSE);
    }
    ReportOperResult(IDC_ADDTUBETORQ, bRes);
}


void CDlgTubeCfg::OnBnClickedDeltubetorq()
{
    UINT i = 0;
    BOOL bRes = FALSE;

    UpdateData(TRUE);

    bRes = theDB.DeleteTubeCfg(&m_tCurTub);
    if (bRes)
    {
        ClearTorqValue(&m_tCurTub);
        //SetTorqValue(&m_tCurTub);

        m_tSrcTub = m_tCurTub;
        SetTorqValue(&m_tCurTub);

        /* 改变combobox颜色 */
        for (i = 0; i < MAXTUBECFGNUM; i++)
            JudgeTorqCBParaChanged(i, &m_tCurTub);

        UpdateData(FALSE);
    }

    ReportOperResult(IDC_DELTUBETORQ, bRes);
}

void CDlgTubeCfg::OnBnClickedModtubetorq()
{
    UINT i = 0;
    BOOL bRes = FALSE;

    UpdateData(TRUE);

    GetCurTorqInfo();

    if (!GetTorqValue(&m_tCurTub))
    {
        AfxMessageBox(theApp.LoadstringFromRes(IDS_STRINVALIDTORQ).c_str());
        return;
    }

    bRes = theDB.UpdateTubeCfg(&m_tCurTub);
    if (bRes)
    {
        m_tSrcTub = m_tCurTub;
        SetTorqValue(&m_tCurTub);

        /* 改变combobox颜色 */
        for (i = 0; i < MAXTUBECFGNUM; i++)
            JudgeTorqCBParaChanged(i, &m_tCurTub);

        /* 修改焦点，触发edit控件修改颜色 */
        for (i = 0; i < MAXTUBETORQNUM; i++)
            m_neTorqVal[i].SetFocus();

        UpdateData(FALSE);
    }

    ReportOperResult(IDC_MODTUBETORQ, bRes);
}

void CDlgTubeCfg::InitTorqShow(TUBECFG* ptTube)
{
    ASSERT_NULL(ptTube);

    theDB.GetInitTube(ptTube);

    SetTorqShow(ptTube);
}

void  CDlgTubeCfg::SetTorqShow(TUBECFG *ptTube)
{
    ASSERT_NULL(ptTube);

    g_cTubing.SetFactoryBox(&m_cbFixTubingOpt[INDEX_TUBE_FACTORY], ptTube, m_nCurLang);
    g_cTubing.SetAllComboBox(INDEX_TUBE_OEM, &m_cbFixTubingOpt[INDEX_TUBE_OEM], ptTube, m_nCurLang);
    g_cTubing.SetAllComboBox(INDEX_TUBE_SIZE, &m_cbFixTubingOpt[INDEX_TUBE_SIZE], ptTube, m_nCurLang);
    g_cTubing.SetAllComboBox(INDEX_TUBE_MATER, &m_cbFixTubingOpt[INDEX_TUBE_MATER], ptTube, m_nCurLang);
    g_cTubing.SetAllComboBox(INDEX_TUBE_COUPL, &m_cbFixTubingOpt[INDEX_TUBE_COUPL], ptTube, m_nCurLang);
}

void CDlgTubeCfg::EnableTorqCtrl(BOOL bExist)
{
    GetDlgItem(IDC_ADDTUBETORQ)->EnableWindow(!bExist);
    GetDlgItem(IDC_DELTUBETORQ)->EnableWindow(bExist);
    GetDlgItem(IDC_MODTUBETORQ)->EnableWindow(bExist);
}

void CDlgTubeCfg::SetTorqValue(TUBECFG* ptTube)
{
    UINT i = 0;
    int index = 0;

    ASSERT_NULL(ptTube);
    index = theDB.QueryIndexByInfo(ptTube);
    ASSERT_ZERO(index);

    if (index > 0)
    {
        EnableTorqCtrl(TRUE);
        for (i = 0; i < MAXTUBETORQNUM; i++)
        {
            SET_STRING_VALUE(m_strTorqVal[i], ptTube->nTorqVal[i]);
        }
    }
    else
    {
        EnableTorqCtrl(FALSE);
        for (i = 0; i < MAXTUBETORQNUM; i++)
        {
            SET_STRING_VALUE(m_strTorqVal[i], 0);
        }
    }
}

void CDlgTubeCfg::ClearTorqValue(TUBECFG* ptTube)
{
    for (int i = 0; i < MAXTUBETORQNUM; i++)
    {
        ptTube->nTorqVal[i] = 0;
    }
}

BOOL CDlgTubeCfg::GetTorqValue(TUBECFG* ptTube)
{
    UINT i = 0;
    int  iTorq = 0;

    ASSERT_NULL_R(ptTube, FALSE);
    for (i = 0; i < MAXTUBETORQNUM; i++)
    {
        if (m_strTorqVal[i].IsEmpty())
        {
            return FALSE;
        }
        iTorq = atoi(m_strTorqVal[i]);
        if (iTorq <= 0)
        {
            return FALSE;
        }
        ptTube->nTorqVal[i] = iTorq;
    }

    return TRUE;
}

void CDlgTubeCfg::GetCurTorqInfo()
{
    UINT i = 0;

    for (i = 0; i < MAXTUBECFGNUM; i++)
    {
        m_tCurTub.nFixTube[i] = g_cTubing.GetTubeNOByName(i, m_tCurTub.nFixTube[INDEX_TUBE_FACTORY], &m_cbFixTubingOpt[i], m_nCurLang);
    }
}

void CDlgTubeCfg::ChangeTubingShowBox(UINT nIndex)
{
    TUBECFG tTmpTub = { 0 };
    UINT    nCurNO = 0;
    int     i = 0;

    COMP_BFALSE(g_cTubing.CheckIndex(nIndex));

    // 获取和设置油管5个参数值
    nCurNO = g_cTubing.GetTubeNOByName(nIndex, m_tCurTub.nFixTube[INDEX_TUBE_FACTORY], &m_cbFixTubingOpt[nIndex], m_nCurLang);
    if (!g_cTubing.CheckTubeNOChanged(nIndex, nCurNO, &m_tCurTub, &tTmpTub))
        return;
    /* 后续序号从m_tCurTub获取 */
    for (i = nIndex - 1; i >= 0; i--)
    {
        tTmpTub.nFixTube[i] = m_tCurTub.nFixTube[i];
    }

    // 初始化COMBOBOX内容，有变化则显示红色
    switch (nIndex)
    {
    case INDEX_TUBE_FACTORY:
        g_cTubing.InitOEMValue(&tTmpTub);
        g_cTubing.SetFactoryBox(&m_cbFixTubingOpt[INDEX_TUBE_FACTORY], &tTmpTub, m_nCurLang);
        JudgeTorqCBParaChanged(INDEX_TUBE_FACTORY, &tTmpTub);

    case INDEX_TUBE_OEM:
        g_cTubing.InitSizeValue(&tTmpTub);
        //g_cTubing.SetOEMBox(&m_cbFixTubingOpt[INDEX_OEM], &tTmpTub, m_nCurLang);
        g_cTubing.SetAllComboBox(INDEX_TUBE_OEM, &m_cbFixTubingOpt[INDEX_TUBE_OEM], &tTmpTub, m_nCurLang);
        JudgeTorqCBParaChanged(INDEX_TUBE_OEM, &tTmpTub);

    case INDEX_TUBE_SIZE:
        g_cTubing.InitMatValue(&tTmpTub);
        //g_cTubing.SetSizeBox(&m_cbFixTubingOpt[INDEX_SIZE], &tTmpTub, m_nCurLang);
        g_cTubing.SetAllComboBox(INDEX_TUBE_SIZE, &m_cbFixTubingOpt[INDEX_TUBE_SIZE], &tTmpTub, m_nCurLang);
        JudgeTorqCBParaChanged(INDEX_TUBE_SIZE, &tTmpTub);

    case INDEX_TUBE_MATER:
        g_cTubing.InitCouplValue(&tTmpTub);
        //g_cTubing.SetMatBox(&m_cbFixTubingOpt[INDEX_MATER], &tTmpTub, m_nCurLang);
        g_cTubing.SetAllComboBox(INDEX_TUBE_MATER, &m_cbFixTubingOpt[INDEX_TUBE_MATER], &tTmpTub, m_nCurLang);
        JudgeTorqCBParaChanged(INDEX_TUBE_MATER, &tTmpTub);

    case INDEX_TUBE_COUPL:
        //g_cTubing.SetCouplBox(&m_cbFixTubingOpt[INDEX_COUPL], &tTmpTub, m_nCurLang);
        g_cTubing.SetAllComboBox(INDEX_TUBE_COUPL, &m_cbFixTubingOpt[INDEX_TUBE_COUPL], &tTmpTub, m_nCurLang);
        JudgeTorqCBParaChanged(INDEX_TUBE_COUPL, &tTmpTub);
    default:
        break;
    }
}

void CDlgTubeCfg::JudgeTorqCBParaChanged(UINT nIndex, TUBECFG* ptTube)
{
    COLORREF    clrCtrl;

    COMP_BFALSE(g_cTubing.CheckIndex(nIndex));
    ASSERT_NULL(ptTube);

    clrCtrl = m_clrNormal;
    if (m_tSrcTub.nFixTube[nIndex] != ptTube->nFixTube[nIndex])
        clrCtrl = m_clrChanged;

    m_cbFixTubingOpt[nIndex].m_ColorText = clrCtrl;
    m_cbFixTubingOpt[nIndex].Invalidate();
}

