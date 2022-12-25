// DlgParaSet.cpp : implementation file
//

#include "stdafx.h"
#include "Torque.h"
#include "DlgParaSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//extern CTubeCfg    g_cTubing;

/////////////////////////////////////////////////////////////////////////////
// CDlgParaSet dialog

CDlgParaSet::CDlgParaSet(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgParaSet::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgParaSet)
    m_strParaAlias  = _T("");
    m_strMemo       = _T("");
    m_fControlCir   = 0.0;
    m_fControl      = 0.0;
    m_fOptTorq      = 0.0;
    m_fLowerCir     = 0.0;
    //m_fLowerLimit   = 0.0;
    m_fMaxCir       = 0.0;
    m_fMaxLimit     = 0.0;
    m_fMaxRPM       = 0.0;
    m_fShow         = 0.0;
    //m_fSpeedDown    = 0.0;
    m_fUpperCir     = 0.0;
    //m_fUpperLimit   = 0.0;
    //m_fLowerTai     = 0.0f;
    //m_fUpperTai     = 0.0f;
    //m_fMinShlSlope  = 0.0f;
    //m_fMaxDeltaCir  = 0.0f;
    //m_fMinDeltaCir  = 0.0f;

    m_bParaChg = false;

    //}}AFX_DATA_INIT
}


void CDlgParaSet::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgParaSet)
    DDX_Text(pDX, IDC_EDITCONTROLCIR, m_fControlCir);
    DDX_Control(pDX, IDC_EDITCONTROLCIR, m_neControlCir);
    DDX_Text(pDX, IDC_EDITCONTROLTORQ, m_fControl);
    DDX_Control(pDX, IDC_EDITCONTROLTORQ, m_neControl);
    DDX_Text(pDX, IDC_EDITOPTTORQ, m_fOptTorq);
    DDX_Control(pDX, IDC_EDITOPTTORQ, m_neOptTorq);
    DDX_Text(pDX, IDC_EDITLOWERCIR, m_fLowerCir);
    DDX_Control(pDX, IDC_EDITLOWERCIR, m_neLowerCir);
    /*DDX_Text(pDX, IDC_EDITLOWERTORQ, m_fLowerLimit);
    DDX_Control(pDX, IDC_EDITLOWERTORQ, m_neLowerLimit);*/
    DDX_Text(pDX, IDC_EDITMAXCIR, m_fMaxCir);
    DDX_Control(pDX, IDC_EDITMAXCIR, m_neMaxCir);
    DDX_Text(pDX, IDC_EDITMAXTORQ, m_fMaxLimit);
    DDX_Control(pDX, IDC_EDITMAXTORQ, m_neMaxLimit);
    DDX_Text(pDX, IDC_EDITMAXRPM, m_fMaxRPM);
    DDX_Control(pDX, IDC_EDITMAXRPM, m_neMaxRPM);
    DDX_Text(pDX, IDC_EDITSHOWTORQ, m_fShow);
    DDX_Control(pDX, IDC_EDITSHOWTORQ, m_neShow);
    //DDX_Text(pDX, IDC_EDITSPEEDTORQ, m_fSpeedDown);
    //DDX_Control(pDX, IDC_EDITSPEEDTORQ, m_neSpeedDown);
    DDX_Text(pDX, IDC_EDITUPPERCIR, m_fUpperCir);
    DDX_Control(pDX, IDC_EDITUPPERCIR, m_neUpperCir);
    /*DDX_Text(pDX, IDC_EDITUPPERTORQ, m_fUpperLimit);
    DDX_Control(pDX, IDC_EDITUPPERTORQ, m_neUpperLimit);*/
    DDX_Control(pDX, IDC_CBALIAS, m_cbAlias);
    DDX_Text(pDX, IDC_CBALIAS, m_strParaAlias);
    //DDX_Text(pDX, IDC_EDITLOWERTAI, m_fLowerTai);
    //DDX_Control(pDX, IDC_EDITLOWERTAI, m_neLowerTai);
    //DDX_Text(pDX, IDC_EDITUPPERTAI, m_fUpperTai);
    //DDX_Control(pDX, IDC_EDITUPPERTAI, m_neUpperTai);
    //DDX_Text(pDX, IDC_EDITMINSHLSLOPE, m_fMinShlSlope);
    //DDX_Control(pDX, IDC_EDITMINSHLSLOPE, m_neMinShlSlope);
    //DDX_Text(pDX, IDC_EDITMAXDELTACIR, m_fMaxDeltaCir);
    //DDX_Control(pDX, IDC_EDITMAXDELTACIR, m_neMaxDeltaCir);
    //DDX_Text(pDX, IDC_EDITMINDELTACIR, m_fMinDeltaCir);
    //DDX_Control(pDX, IDC_EDITMINDELTACIR, m_neMinDeltaCir);
    DDX_Text(pDX, IDC_EDITMEMO, m_strMemo);
    DDX_Text(pDX, IDC_STATIC_M10, m_strLBM10);
    DDX_Radio(pDX, IDC_RADIOSETSINGLE, m_iSingleSTD);
    DDX_Control(pDX, IDC_RADIOSETSINGLE, m_rdSingle);
    DDX_Control(pDX, IDC_RADIOSETSTAND, m_rdStand);
    DDX_Text(pDX, IDC_SHOWSET01, m_strSetShowName[0]);
    DDX_Text(pDX, IDC_SHOWSET02, m_strSetShowName[1]);
    DDX_Text(pDX, IDC_SHOWSET03, m_strSetShowName[2]);
    DDX_Text(pDX, IDC_SHOWSET04, m_strSetShowName[3]);
    DDX_Text(pDX, IDC_SHOWSET05, m_strSetShowName[4]);
    DDX_Text(pDX, IDC_SHOWSET06, m_strSetShowName[5]);
    DDX_Text(pDX, IDC_SHOWSET07, m_strSetShowName[6]);
    DDX_Text(pDX, IDC_SHOWSET08, m_strSetShowName[7]);
    DDX_Text(pDX, IDC_SHOWSET09, m_strSetShowName[8]);
    DDX_Text(pDX, IDC_SHOWSET10, m_strSetShowName[9]);
    DDX_Text(pDX, IDC_SHOWSET11, m_strSetShowName[10]);
    DDX_Text(pDX, IDC_SHOWSET12, m_strSetShowName[11]);
    DDX_Text(pDX, IDC_SHOWSET13, m_strSetShowName[12]);
    DDX_Text(pDX, IDC_SHOWSET14, m_strSetShowName[13]);
    DDX_Text(pDX, IDC_SHOWSET15, m_strSetShowName[14]);
    DDX_Text(pDX, IDC_SHOWSET16, m_strSetShowName[15]);
    //DDX_Text(pDX, IDC_SHOWSET17, m_strSetShowName[16]);
    //DDX_Text(pDX, IDC_SHOWSET18, m_strSetShowName[17]);
    DDX_Text(pDX, IDC_CBPARA01, m_strSetShowOption[0]);
    DDX_Text(pDX, IDC_CBPARA02, m_strSetShowOption[1]);
    DDX_Text(pDX, IDC_CBPARA03, m_strSetShowOption[2]);
    DDX_Text(pDX, IDC_CBPARA04, m_strSetShowOption[3]);
    DDX_Text(pDX, IDC_CBPARA05, m_strSetShowOption[4]);
    DDX_Text(pDX, IDC_CBPARA06, m_strSetShowOption[5]);
    DDX_Text(pDX, IDC_CBPARA07, m_strSetShowOption[6]);
    DDX_Text(pDX, IDC_CBPARA08, m_strSetShowOption[7]);
    DDX_Text(pDX, IDC_CBPARA09, m_strSetShowOption[8]);
    DDX_Text(pDX, IDC_CBPARA10, m_strSetShowOption[9]);
    DDX_Text(pDX, IDC_CBPARA11, m_strSetShowOption[10]);
    DDX_Text(pDX, IDC_CBPARA12, m_strSetShowOption[11]);
    DDX_Text(pDX, IDC_CBPARA13, m_strSetShowOption[12]);
    DDX_Text(pDX, IDC_CBPARA14, m_strSetShowOption[13]);
    DDX_Text(pDX, IDC_CBPARA15, m_strSetShowOption[14]);
    DDX_Text(pDX, IDC_CBPARA16, m_strSetShowOption[15]);
  /*  DDX_Text(pDX, IDC_CBPARA17, m_strSetShowOption[16]);
    DDX_Text(pDX, IDC_CBPARA18, m_strSetShowOption[17]);*/
    DDX_Control(pDX, IDC_CBPARA01, m_cbSetShowOption[0]);
    DDX_Control(pDX, IDC_CBPARA02, m_cbSetShowOption[1]);
    DDX_Control(pDX, IDC_CBPARA03, m_cbSetShowOption[2]);
    DDX_Control(pDX, IDC_CBPARA04, m_cbSetShowOption[3]);
    DDX_Control(pDX, IDC_CBPARA05, m_cbSetShowOption[4]);
    DDX_Control(pDX, IDC_CBPARA06, m_cbSetShowOption[5]);
    DDX_Control(pDX, IDC_CBPARA07, m_cbSetShowOption[6]);
    DDX_Control(pDX, IDC_CBPARA08, m_cbSetShowOption[7]);
    DDX_Control(pDX, IDC_CBPARA09, m_cbSetShowOption[8]);
    DDX_Control(pDX, IDC_CBPARA10, m_cbSetShowOption[9]);
    DDX_Control(pDX, IDC_CBPARA11, m_cbSetShowOption[10]);
    DDX_Control(pDX, IDC_CBPARA12, m_cbSetShowOption[11]);
    DDX_Control(pDX, IDC_CBPARA13, m_cbSetShowOption[12]);
    DDX_Control(pDX, IDC_CBPARA14, m_cbSetShowOption[13]);
    DDX_Control(pDX, IDC_CBPARA15, m_cbSetShowOption[14]);
    DDX_Control(pDX, IDC_CBPARA16, m_cbSetShowOption[15]);
    /*DDX_Control(pDX, IDC_CBPARA17, m_cbSetShowOption[16]);
    DDX_Control(pDX, IDC_CBPARA18, m_cbSetShowOption[17]);*/
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgParaSet, CDialog)
    //{{AFX_MSG_MAP(CDlgParaSet)
    ON_BN_CLICKED(IDC_PARASAVE, OnParasave)
    ON_WM_CTLCOLOR()
    ON_CBN_KILLFOCUS(IDC_CBPARA01, &CDlgParaSet::OnCbnKillfocusCbpara01)
    ON_CBN_KILLFOCUS(IDC_CBPARA02, &CDlgParaSet::OnCbnKillfocusCbpara02)
    ON_CBN_KILLFOCUS(IDC_CBPARA03, &CDlgParaSet::OnCbnKillfocusCbpara03)
    ON_CBN_KILLFOCUS(IDC_CBPARA04, &CDlgParaSet::OnCbnKillfocusCbpara04)
    ON_CBN_KILLFOCUS(IDC_CBPARA05, &CDlgParaSet::OnCbnKillfocusCbpara05)
    ON_CBN_KILLFOCUS(IDC_CBPARA06, &CDlgParaSet::OnCbnKillfocusCbpara06)
    ON_CBN_KILLFOCUS(IDC_CBPARA07, &CDlgParaSet::OnCbnKillfocusCbpara07)
    ON_CBN_KILLFOCUS(IDC_CBPARA08, &CDlgParaSet::OnCbnKillfocusCbpara08)
    ON_CBN_KILLFOCUS(IDC_CBPARA09, &CDlgParaSet::OnCbnKillfocusCbpara09)
    ON_CBN_KILLFOCUS(IDC_CBPARA10, &CDlgParaSet::OnCbnKillfocusCbpara10)
    ON_CBN_KILLFOCUS(IDC_CBPARA11, &CDlgParaSet::OnCbnKillfocusCbpara11)
    ON_CBN_KILLFOCUS(IDC_CBPARA12, &CDlgParaSet::OnCbnKillfocusCbpara12)
    ON_CBN_KILLFOCUS(IDC_CBPARA13, &CDlgParaSet::OnCbnKillfocusCbpara13)
    ON_CBN_KILLFOCUS(IDC_CBPARA14, &CDlgParaSet::OnCbnKillfocusCbpara14)
    ON_CBN_KILLFOCUS(IDC_CBPARA15, &CDlgParaSet::OnCbnKillfocusCbpara15)
    ON_CBN_KILLFOCUS(IDC_CBPARA16, &CDlgParaSet::OnCbnKillfocusCbpara16)
 /*   ON_CBN_KILLFOCUS(IDC_CBPARA17, &CDlgParaSet::OnCbnKillfocusCbpara17)
    ON_CBN_KILLFOCUS(IDC_CBPARA18, &CDlgParaSet::OnCbnKillfocusCbpara18)*/
    ON_EN_KILLFOCUS(IDC_EDITOPTTORQ, &CDlgParaSet::OnKillfocusEditopttorq)
    ON_EN_KILLFOCUS(IDC_EDITMAXCIR, &CDlgParaSet::OnEnKillfocusEditmaxcir)
    ON_CBN_SELCHANGE(IDC_CBALIAS, &CDlgParaSet::OnCbnSelchangeCbalias)
    ON_CBN_KILLFOCUS(IDC_CBALIAS, &CDlgParaSet::OnCbnKillfocusCbalias)
    ON_BN_CLICKED(IDC_DELALIAS, &CDlgParaSet::OnBnClickedDelalias)
    ON_BN_CLICKED(IDC_RADIOSETSINGLE, &CDlgParaSet::OnBnClickedRadiosetsingle)
    ON_BN_CLICKED(IDC_RADIOSETSTAND, &CDlgParaSet::OnBnClickedRadiosetstand)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgParaSet message handlers

BOOL CDlgParaSet::OnInitDialog()
{
    int     i = 0;

    CDialog::OnInitDialog();

    m_clrNormal  = RGB(0, 0, 0);
    m_clrChanged = RGB(255, 0, 0);
    m_nCurLang  = g_tGlbCfg.nLangType;

    /* 根据入参设置参数初始值 */
    SetParaValue(&m_tempCfg, &m_tempShow);

    UpdateDlgLabel();

    UpdateData(FALSE);

    InitAliasShow();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgParaSet::UpdateDlgLabel()
{
    m_strLBM10 = theApp.LoadstringFromRes(IDS_STRTORQPARA, g_tGlbCfg.strUnit).c_str();
}

void CDlgParaSet::SetParaValue(PARACFG *ptCfg, SHOWCFG *ptShow)
{
    CONTROLPARA *ptCtrl = NULL;
    WORD        i = 0;
    WORD        j = 0;
    vector<int> lsShowIndex;
    vector<string> lsOption;

    ASSERT_NULL(ptCfg);
    ASSERT_NULL(ptShow);

    ptCtrl = &ptCfg->tCtrl;

    m_fMaxLimit = ptCtrl->fTorqConf[INDEX_TORQ_MAXLIMIT];
    m_fControl = ptCtrl->fTorqConf[INDEX_TORQ_CONTROL];
    m_fOptTorq = ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL];
    m_fShow = ptCtrl->fTorqConf[INDEX_TORQ_SHOW];

    m_fMaxCir = ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT];
    m_fUpperCir = ptCtrl->fTurnConf[INDEX_TURN_UPPERLIMIT];
    m_fControlCir = ptCtrl->fTurnConf[INDEX_TURN_CONTROL];
    m_fLowerCir = ptCtrl->fTurnConf[INDEX_TURN_LOWERLIMIT];

    m_fMaxRPM = ptCtrl->fFullRPM;
    m_strMemo = ptCfg->strMemo.c_str();
    m_strParaAlias = ptCfg->strAlias.c_str();
    m_iSingleSTD = ptCtrl->iSingleSTD;
    
    lsShowIndex = theDB.ReadCurOptionIndex();

    for(i=0; i<MAXPARANUM; i++)   //  i<ptShow->wParaNum+1 && 
    {
        m_strSetShowName[i] = ptShow->strShow[i].c_str();

        m_cbSetShowOption[i].ResetContent();
        if (i >= lsShowIndex.size())
            continue;

        lsOption.clear();
        
        lsOption = theDB.ReadOptionsByShowIndex(lsShowIndex[i]);
        for (j = 0; j < lsOption.size(); j++)
        {
            m_cbSetShowOption[i].AddString(lsOption[j].c_str());
            if (ptCfg->strValue[i] == lsOption[j])
            {
                m_cbSetShowOption[i].SetCurSel(j);
                m_strSetShowOption[i] = lsOption[j].c_str();
            }
        }
    }
}

void CDlgParaSet::InitAliasShow()
{
    size_t i = 0;
    int iCur = 0;
    vector<string> lsAlias;
    m_cbAlias.ResetContent();

    lsAlias = theDB.ReadAllAlias();
    if (lsAlias.size() <= 0)
        return;

    for (i = 0; i < lsAlias.size(); i++)
    {
        m_cbAlias.AddString(lsAlias[i].c_str());
        if(m_tempCfg.strAlias == lsAlias[i])
            iCur = m_cbAlias.GetCount() - 1;
    }
    m_cbAlias.SetCurSel(iCur);
}

void CDlgParaSet::OnParasave()
{
    string strInfo;
    int AliasID = DB_INVALID_VAL;

    UpdateData(TRUE);

    if(m_strParaAlias.IsEmpty())
    {
        strInfo = theApp.LoadstringFromRes(IDS_STRINFPARAALIAS, m_strParaAlias.GetBuffer(0));
        theApp.SaveShowMessage(strInfo.c_str(), MB_OK|MB_ICONINFORMATION);
        return;
    }

    GetParaValue(&m_tempCfg);

    strInfo = theApp.LoadstringFromRes(IDS_STRINFSAVEALIASFAIL, m_strParaAlias.GetBuffer(0));
    /* 保存前先校验下参数的有效范围 */
    theApp.AdjustParaValue(&m_tempCfg);
    // 更新扭矩参数配置
    AliasID = theDB.UpdateTorqCfgPara(&m_tempCfg, &m_tempShow);
    if (AliasID <= 0)
    {
        theApp.SaveShowMessage(strInfo.c_str(), MB_OK | MB_ICONINFORMATION);
        return;
    }

    m_tempShow.nAlias = AliasID;
    if (!theDB.UpdateShowAlias(theApp.m_ptCurShow, AliasID))
    {
        theApp.SaveShowMessage(strInfo.c_str(), MB_OK | MB_ICONINFORMATION);
        return;
    }

    UpdateData(FALSE);

    InitAliasShow();

    m_bParaChg = true;

    strInfo = theApp.LoadstringFromRes(IDS_STRINFSAVEALIAS, m_strParaAlias.GetBuffer(0));
    theApp.SaveShowMessage(strInfo.c_str(), MB_OK|MB_ICONEXCLAMATION);
}

BOOL CDlgParaSet::GetParaValue(PARACFG *ptCfg)
{
    CString     strTemp;
    CONTROLPARA *ptCtrl = NULL;
    int         i = 0;
    int         iFactory = 0;

    ASSERT_NULL_R(ptCfg, FALSE);

    ptCtrl = &ptCfg->tCtrl;

    for(i=0; i<MAXPARANUM; i++)
    {
        ptCfg->strValue[i] = m_strSetShowOption[i];
    }

    ptCtrl->fTorqConf[INDEX_TORQ_MAXLIMIT]    = m_fMaxLimit;
    //ptCtrl->fTorqConf[INDEX_TORQ_UPPERLIMIT]  = HAND_CEIL(m_fOptTorq * RATIO_UPPERLIMIT);
    ptCtrl->fTorqConf[INDEX_TORQ_CONTROL]     = m_fControl;
    ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL]     = m_fOptTorq;
    //ptCtrl->fTorqConf[INDEX_TORQ_LOWERLIMIT]  = HAND_FLOOR(m_fOptTorq * RATIO_LOWERLIMIT);
    //ptCtrl->fTorqConf[INDEX_TORQ_SPEEDDOWN]   = m_fSpeedDown;
    ptCtrl->fTorqConf[INDEX_TORQ_SHOW]        = m_fShow;
    //ptCtrl->fTorqConf[INDEX_TORQ_UPPERTAI]    = m_fUpperTai;
    //ptCtrl->fTorqConf[INDEX_TORQ_LOWERTAI]    = m_fLowerTai;

    ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT]      = m_fMaxCir;
    ptCtrl->fTurnConf[INDEX_TURN_UPPERLIMIT]    = m_fUpperCir;
    ptCtrl->fTurnConf[INDEX_TURN_CONTROL]       = m_fControlCir;
    ptCtrl->fTurnConf[INDEX_TURN_LOWERLIMIT]    = m_fLowerCir;
  /*  ptCtrl->fTurnConf[INDEX_TURN_MAXDELTA]      = m_fMaxDeltaCir;
    ptCtrl->fTurnConf[INDEX_TURN_MINDELTA]      = m_fMinDeltaCir;*/

    ptCtrl->fFullRPM      = m_fMaxRPM;

    //ptCtrl->fMinShlSlope = m_fMinShlSlope;
    ptCtrl->iSingleSTD = m_iSingleSTD;
    ptCfg->strMemo = m_strMemo.GetBuffer(0);
    //lstrcpyn(ptComm->aucMemo, m_strMemo, MAXMEMOLEN);

    ptCfg->strAlias = m_strParaAlias.GetBuffer(0);

    return TRUE;
}

HBRUSH CDlgParaSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    COLORREF    clrCtrl;
    CString     strContent;

    CONTROLPARA *ptCtrl = &theApp.m_tParaCfg.tCtrl;
    //COMMONCFG   *ptComm = &theApp.m_tParaCfg.tComm;
    SHOWCFG     *ptShow = theApp.m_ptCurShow;

    GetDlgItemText(pWnd->GetDlgCtrlID(), strContent);
    if (strContent.IsEmpty())
        return hbr;

    // TODO:  在此更改 DC 的任何特性
    switch (pWnd->GetDlgCtrlID())//对某一个特定控件进行判断
    {
    // 扭矩参数
    case IDC_EDITSHOWTORQ:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fTorqConf[INDEX_TORQ_SHOW]);
        break;
    /*case IDC_EDITUPPERTORQ:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fTorqConf[INDEX_TORQ_UPPERLIMIT]);
        break;*/
    case IDC_EDITCONTROLTORQ:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fTorqConf[INDEX_TORQ_CONTROL]);
        break;
    case IDC_EDITOPTTORQ:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL]);
        break;
    /*case IDC_EDITLOWERTORQ:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fTorqConf[INDEX_TORQ_LOWERLIMIT]);
        break;*/
    case IDC_EDITMAXTORQ:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fTorqConf[INDEX_TORQ_MAXLIMIT]);
        break;
    /*case IDC_EDITUPPERTAI:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fTorqConf[INDEX_TORQ_UPPERTAI]);
        break;
    case IDC_EDITSPEEDTORQ:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fTorqConf[INDEX_TORQ_SPEEDDOWN]);
        break;
    case IDC_EDITLOWERTAI:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fTorqConf[INDEX_TORQ_LOWERTAI]);
        break;
    case IDC_EDITMINSHLSLOPE:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fMinShlSlope);
        break;
    case IDC_EDITMAXDELTACIR:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fTurnConf[INDEX_TURN_MAXDELTA]);
        break;
    case IDC_EDITMINDELTACIR:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fTurnConf[INDEX_TURN_MINDELTA]);
        break;*/

    // 扭拧周数
    case IDC_EDITUPPERCIR:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fTurnConf[INDEX_TURN_UPPERLIMIT]);
        break;
    case IDC_EDITCONTROLCIR:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fTurnConf[INDEX_TURN_CONTROL]);
        break;
    case IDC_EDITMAXRPM:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fFullRPM);
        break;
    case IDC_EDITLOWERCIR:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fTurnConf[INDEX_TURN_LOWERLIMIT]);
        break;
    case IDC_EDITMAXCIR:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT]);
        break;

    case IDC_EDITMEMO:
        JUDGE_STRPARA_CHANGE(strContent, theApp.m_tParaCfg.strMemo.c_str());
        break;

    default:
        hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
        break;
    }

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void CDlgParaSet::JudgeShowParaChanged(UINT nIndex)
{
    COLORREF    clrCtrl;

    COMP_BGE(nIndex, MAXPARANUM);

    UpdateData(TRUE);
    clrCtrl = m_clrNormal;
    if (m_strSetShowOption[nIndex].Compare(theApp.m_tParaCfg.strValue[nIndex].c_str()) != 0)
        clrCtrl = m_clrChanged;
    m_cbSetShowOption[nIndex].m_ColorText = clrCtrl;
    m_cbSetShowOption[nIndex].Invalidate();
}

void CDlgParaSet::OnCbnKillfocusCbpara01()
{
    JudgeShowParaChanged(0);
}
void CDlgParaSet::OnCbnKillfocusCbpara02()
{
    JudgeShowParaChanged(1);
}
void CDlgParaSet::OnCbnKillfocusCbpara03()
{
    JudgeShowParaChanged(2);
}
void CDlgParaSet::OnCbnKillfocusCbpara04()
{
    JudgeShowParaChanged(3);
}
void CDlgParaSet::OnCbnKillfocusCbpara05()
{
    JudgeShowParaChanged(4);
}
void CDlgParaSet::OnCbnKillfocusCbpara06()
{
    JudgeShowParaChanged(5);
}
void CDlgParaSet::OnCbnKillfocusCbpara07()
{
    JudgeShowParaChanged(6);
}
void CDlgParaSet::OnCbnKillfocusCbpara08()
{
    JudgeShowParaChanged(7);
}
void CDlgParaSet::OnCbnKillfocusCbpara09()
{
    JudgeShowParaChanged(8);
}
void CDlgParaSet::OnCbnKillfocusCbpara10()
{
    JudgeShowParaChanged(9);
}
void CDlgParaSet::OnCbnKillfocusCbpara11()
{
    JudgeShowParaChanged(10);
}
void CDlgParaSet::OnCbnKillfocusCbpara12()
{
    JudgeShowParaChanged(11);
}
void CDlgParaSet::OnCbnKillfocusCbpara13()
{
    JudgeShowParaChanged(12);
}
void CDlgParaSet::OnCbnKillfocusCbpara14()
{
    JudgeShowParaChanged(13);
}
void CDlgParaSet::OnCbnKillfocusCbpara15()
{
    JudgeShowParaChanged(14);
}
void CDlgParaSet::OnCbnKillfocusCbpara16()
{
    JudgeShowParaChanged(15);
}
//void CDlgParaSet::OnCbnKillfocusCbpara17()
//{
//    JudgeShowParaChanged(16);
//}
//void CDlgParaSet::OnCbnKillfocusCbpara18()
//{
//    JudgeShowParaChanged(17);
//}

/* 20210419 最大，最小是最佳的1.1    0.9倍，满屏是1.3倍，修改最佳扭矩后默认值 */
void CDlgParaSet::OnKillfocusEditopttorq()
{
    double oldOptTorq = m_fOptTorq;
    UpdateData(TRUE);

    /* 值没有修改则直接返回 */
    COMP_BE(oldOptTorq, m_fOptTorq);

    m_fControl      = m_fOptTorq;
    m_fMaxLimit     = HAND_CEIL (m_fOptTorq * RATIO_MAXLIMIT);
    UpdateData(FALSE);
}

/* 20210317 最佳周数自动设置为满屏周数的3/4 */
void CDlgParaSet::OnEnKillfocusEditmaxcir()
{
    double oldMaxCir = m_fMaxCir;
    UpdateData(TRUE);

    /* 值没有修改则直接返回 */
    COMP_BE(oldMaxCir, m_fMaxCir);
    
    m_fControlCir = m_fMaxCir * 3 / 4;
    UpdateData(FALSE);
}

void CDlgParaSet::OnCbnKillfocusCbalias()
{
    COLORREF    clrCtrl;

    UpdateData(TRUE);
    clrCtrl = m_clrNormal;
    if (m_strParaAlias.Compare(theApp.m_tParaCfg.strAlias.c_str()) != 0)
        clrCtrl = m_clrChanged;
    m_cbAlias.m_ColorText = clrCtrl;
    m_cbAlias.Invalidate();
}

void CDlgParaSet::OnCbnSelchangeCbalias()
{
    int iAliasID = 0;
    BOOL bRes = TRUE;
    TORQCFGID tCfgID = { 0 };
    CString strAlias;

    UpdateData(TRUE);

    strAlias = GetCCBString(&m_cbAlias);

    iAliasID = theDB.ReadTorqCfgPara(strAlias.GetBuffer(0), &m_tempCfg);
    COMP_BLE(iAliasID, 0);

    SetParaValue(&m_tempCfg, &m_tempShow);

    UpdateData(FALSE);
}

CString CDlgParaSet::DelCurAliasContent()
{
    int curSel = 0;
    CString strAlias;

    UpdateData(TRUE);

    curSel = m_cbAlias.GetCurSel();
    if (curSel >= 0)
    {
        m_cbAlias.GetLBText(curSel, strAlias);
        m_cbAlias.DeleteString(curSel);
    }
    
    if (m_cbAlias.GetCount() <= 0)
    {
        m_strParaAlias.Empty();
    }
    else
    {
        m_cbAlias.SetCurSel(0);
    }
    UpdateData(FALSE);
    return strAlias;
}

void CDlgParaSet::OnBnClickedDelalias()
{
    CString strAlias;
    int curSel = 0;
    BOOL bRes = FALSE;
    string strInfo;

    strAlias = DelCurAliasContent();
    if (strAlias.IsEmpty())
        return;

    COMP_BFALSE(theDB.DeleteAlias(strAlias.GetBuffer(0)));

    m_tempShow.nAlias = 0;
    theDB.UpdateShowAlias(theApp.m_ptCurShow, 0);

    theDB.ReadTorqCfgPara(theApp.m_ptCurShow->nAlias, &theApp.m_tParaCfg);

    UpdateData(FALSE);

    InitAliasShow();

    strInfo = theApp.LoadstringFromRes(IDS_STRINFDELETEALIAS, strAlias.GetBuffer(0));
    theApp.SaveShowMessage(strInfo.c_str(), MB_OK | MB_ICONEXCLAMATION);
}


void CDlgParaSet::OnBnClickedRadiosetsingle()
{
    COLORREF    clrCtrl = m_clrNormal;
    if (0 != theApp.m_tParaCfg.tCtrl.iSingleSTD)
    {
        clrCtrl = m_clrChanged;
    }

    m_rdSingle.SetForeColor(clrCtrl);
    m_rdSingle.Invalidate();
}

void CDlgParaSet::OnBnClickedRadiosetstand()
{
    COLORREF    clrCtrl = m_clrNormal;
    if (1 != theApp.m_tParaCfg.tCtrl.iSingleSTD)
    {
        clrCtrl = m_clrChanged;
    }

    m_rdStand.SetForeColor(clrCtrl);
    m_rdStand.Invalidate();
}
