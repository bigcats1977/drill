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



extern CTubeCfg    g_cTubing;

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
    m_fSpeedDown    = 0.0;
    m_fUpperCir     = 0.0;
    //m_fUpperLimit   = 0.0;
    m_fLowerTai     = 0.0f;
    m_fUpperTai     = 0.0f;
    m_fMinShlSlope  = 0.0f;
    m_fMaxDeltaCir  = 0.0f;
    m_fMinDeltaCir  = 0.0f;
    m_bFixedTube  = TRUE;

    m_bParaChg = false;

    m_ptCurTub = &m_tempCfg.tTubeCfg;
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
    DDX_Text(pDX, IDC_EDITSPEEDTORQ, m_fSpeedDown);
    DDX_Control(pDX, IDC_EDITSPEEDTORQ, m_neSpeedDown);
    DDX_Text(pDX, IDC_EDITUPPERCIR, m_fUpperCir);
    DDX_Control(pDX, IDC_EDITUPPERCIR, m_neUpperCir);
    /*DDX_Text(pDX, IDC_EDITUPPERTORQ, m_fUpperLimit);
    DDX_Control(pDX, IDC_EDITUPPERTORQ, m_neUpperLimit);*/
    DDX_Control(pDX, IDC_CBALIAS, m_cbAlias);
    DDX_Text(pDX, IDC_CBALIAS, m_strParaAlias);
    DDX_Text(pDX, IDC_EDITLOWERTAI, m_fLowerTai);
    DDX_Control(pDX, IDC_EDITLOWERTAI, m_neLowerTai);
    DDX_Text(pDX, IDC_EDITUPPERTAI, m_fUpperTai);
    DDX_Control(pDX, IDC_EDITUPPERTAI, m_neUpperTai);
    DDX_Text(pDX, IDC_EDITMINSHLSLOPE, m_fMinShlSlope);
    DDX_Control(pDX, IDC_EDITMINSHLSLOPE, m_neMinShlSlope);
    DDX_Text(pDX, IDC_EDITMAXDELTACIR, m_fMaxDeltaCir);
    DDX_Control(pDX, IDC_EDITMAXDELTACIR, m_neMaxDeltaCir);
    DDX_Text(pDX, IDC_EDITMINDELTACIR, m_fMinDeltaCir);
    DDX_Control(pDX, IDC_EDITMINDELTACIR, m_neMinDeltaCir);
    DDX_Text(pDX, IDC_EDITMEMO, m_strMemo);
    DDX_Text(pDX, IDC_STATIC_M10, m_strLBM10);
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
    DDX_Check(pDX, IDC_CKFIXEDTUBING, m_bFixedTube);
    DDX_Text(pDX, IDC_CBFIXPARA01, m_strFixTubingOpt[0]);
    DDX_Text(pDX, IDC_CBFIXPARA05, m_strFixTubingOpt[1]);// 4567 9 10 14
    DDX_Text(pDX, IDC_CBFIXPARA06, m_strFixTubingOpt[2]);
    DDX_Text(pDX, IDC_CBFIXPARA07, m_strFixTubingOpt[3]);
    DDX_Text(pDX, IDC_CBFIXPARA08, m_strFixTubingOpt[4]);
    DDX_Control(pDX, IDC_CBFIXPARA01, m_cbFixTubingOpt[0]);
    DDX_Control(pDX, IDC_CBFIXPARA05, m_cbFixTubingOpt[1]);
    DDX_Control(pDX, IDC_CBFIXPARA06, m_cbFixTubingOpt[2]);
    DDX_Control(pDX, IDC_CBFIXPARA07, m_cbFixTubingOpt[3]);
    DDX_Control(pDX, IDC_CBFIXPARA08, m_cbFixTubingOpt[4]);
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
    ON_EN_KILLFOCUS(IDC_EDITOPTTORQ, &CDlgParaSet::OnKillfocusEditopttorq)
    ON_EN_KILLFOCUS(IDC_EDITMAXCIR, &CDlgParaSet::OnEnKillfocusEditmaxcir)
    ON_CBN_KILLFOCUS(IDC_CBFIXPARA01, &CDlgParaSet::OnCbnKillfocusCbfixpara01)
    ON_CBN_KILLFOCUS(IDC_CBFIXPARA05, &CDlgParaSet::OnCbnKillfocusCbfixpara05)
    ON_CBN_KILLFOCUS(IDC_CBFIXPARA06, &CDlgParaSet::OnCbnKillfocusCbfixpara06)
    ON_CBN_KILLFOCUS(IDC_CBFIXPARA07, &CDlgParaSet::OnCbnKillfocusCbfixpara07)
    ON_CBN_KILLFOCUS(IDC_CBFIXPARA08, &CDlgParaSet::OnCbnKillfocusCbfixpara08)
    ON_CBN_SELCHANGE(IDC_CBFIXPARA01, &CDlgParaSet::OnCbnSelchangeCbfixpara01)
    ON_CBN_SELCHANGE(IDC_CBFIXPARA05, &CDlgParaSet::OnCbnSelchangeCbfixpara05)
    ON_CBN_SELCHANGE(IDC_CBFIXPARA06, &CDlgParaSet::OnCbnSelchangeCbfixpara06)
    ON_CBN_SELCHANGE(IDC_CBFIXPARA07, &CDlgParaSet::OnCbnSelchangeCbfixpara07)
    ON_CBN_SELCHANGE(IDC_CBFIXPARA08, &CDlgParaSet::OnCbnSelchangeCbfixpara08)
    ON_BN_CLICKED(IDC_CKFIXEDTUBING, &CDlgParaSet::OnBnClickedCkfixedtubing)
    ON_CBN_SELCHANGE(IDC_CBALIAS, &CDlgParaSet::OnCbnSelchangeCbalias)
    ON_CBN_KILLFOCUS(IDC_CBALIAS, &CDlgParaSet::OnCbnKillfocusCbalias)
    ON_BN_CLICKED(IDC_DELALIAS, &CDlgParaSet::OnBnClickedDelalias)
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

    InitFixShowPara();

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
    //m_fUpperLimit = ptCtrl->fTorqConf[INDEX_TORQ_UPPERLIMIT];
    m_fControl = ptCtrl->fTorqConf[INDEX_TORQ_CONTROL];
    m_fOptTorq = ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL];
    //m_fLowerLimit = ptCtrl->fTorqConf[INDEX_TORQ_LOWERLIMIT];
    m_fSpeedDown = ptCtrl->fTorqConf[INDEX_TORQ_SPEEDDOWN];
    m_fShow = ptCtrl->fTorqConf[INDEX_TORQ_SHOW];
    m_fUpperTai = ptCtrl->fTorqConf[INDEX_TORQ_UPPERTAI];
    m_fLowerTai = ptCtrl->fTorqConf[INDEX_TORQ_LOWERTAI];

    m_fMaxCir = ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT];
    m_fUpperCir = ptCtrl->fTurnConf[INDEX_TURN_UPPERLIMIT];
    m_fControlCir = ptCtrl->fTurnConf[INDEX_TURN_CONTROL];
    m_fLowerCir = ptCtrl->fTurnConf[INDEX_TURN_LOWERLIMIT];
    m_fMaxDeltaCir = ptCtrl->fTurnConf[INDEX_TURN_MAXDELTA];
    m_fMinDeltaCir = ptCtrl->fTurnConf[INDEX_TURN_MINDELTA];

    m_fMaxRPM = ptCtrl->fFullRPM;
    m_fMinShlSlope = ptCtrl->fMinShlSlope;
    m_strMemo = ptCfg->strMemo.c_str();
    m_strParaAlias = ptCfg->strAlias.c_str();

    m_bFixedTube = theApp.CheckFixTube(ptCfg);
    
    lsShowIndex = theDB.ReadCurShowIndex();

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

void CDlgParaSet::InitFixShowPara()
{
    g_cTubing.SetFactoryBox(&m_cbFixTubingOpt[INDEX_TUBE_FACTORY], m_ptCurTub, m_nCurLang);
    g_cTubing.SetOEMBox(&m_cbFixTubingOpt[INDEX_TUBE_OEM], m_ptCurTub, m_nCurLang);
    g_cTubing.SetSizeBox(&m_cbFixTubingOpt[INDEX_TUBE_SIZE], m_ptCurTub, m_nCurLang);
    g_cTubing.SetMatBox(&m_cbFixTubingOpt[INDEX_TUBE_MATER], m_ptCurTub, m_nCurLang);
    g_cTubing.SetCouplBox(&m_cbFixTubingOpt[INDEX_TUBE_COUPL], m_ptCurTub, m_nCurLang);
    
    ShowTubingCtrl(true);
}

void CDlgParaSet::ShowTubingCtrl(bool bInit)
{
    m_cbSetShowOption[FIXINDEX_FACTORY].ShowWindow(!m_bFixedTube);
    m_cbSetShowOption[FIXINDEX_OEM].ShowWindow(!m_bFixedTube);
    m_cbSetShowOption[FIXINDEX_SIZE].ShowWindow(!m_bFixedTube);
    m_cbSetShowOption[FIXINDEX_MATER].ShowWindow(!m_bFixedTube);
    m_cbSetShowOption[FIXINDEX_COUPL].ShowWindow(!m_bFixedTube);
    
    m_cbFixTubingOpt[INDEX_TUBE_FACTORY].ShowWindow(m_bFixedTube);
    m_cbFixTubingOpt[INDEX_TUBE_OEM].ShowWindow(m_bFixedTube);
    m_cbFixTubingOpt[INDEX_TUBE_SIZE].ShowWindow(m_bFixedTube);
    m_cbFixTubingOpt[INDEX_TUBE_MATER].ShowWindow(m_bFixedTube);
    m_cbFixTubingOpt[INDEX_TUBE_COUPL].ShowWindow(m_bFixedTube);

    /* 由其他管材改为设定管材，修改默认扭矩值配置 */
    if(m_bFixedTube)
        ChgTorqbyShowPara(bInit);
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
    //COMMONCFG   *ptComm = NULL;
    int         i = 0;
    int         iFactory = 0;

    ASSERT_NULL_R(ptCfg, FALSE);

    ptCtrl = &ptCfg->tCtrl;
    //ptComm = &ptCfg->tComm;

    for(i=0; i<MAXPARANUM; i++)   //  i<ptShow->wParaNum+1 && 
    {
        ptCfg->strValue[i] = m_strSetShowOption[i];
    }

    //ptShow->bFixTube = m_bFixedTube;
    if (m_bFixedTube)
    {
        for (i = 0; i < MAXTUBECFGNUM; i++)
        {
            ptCfg->tTubeCfg.nFixTube[i] = g_cTubing.GetTubeNOByName(i, m_ptCurTub->nFixTube[INDEX_TUBE_FACTORY], &m_cbFixTubingOpt[i], m_nCurLang);
        }
    }
    else
    {
        memset(&ptCfg->tTubeCfg, 0, sizeof(TUBECFG));
    }

    ptCtrl->fTorqConf[INDEX_TORQ_MAXLIMIT]    = m_fMaxLimit;
    //ptCtrl->fTorqConf[INDEX_TORQ_UPPERLIMIT]  = m_fUpperLimit;
    ptCtrl->fTorqConf[INDEX_TORQ_CONTROL]     = m_fControl;
    ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL]     = m_fOptTorq;
    //ptCtrl->fTorqConf[INDEX_TORQ_LOWERLIMIT] = m_fLowerLimit;
    ptCtrl->fTorqConf[INDEX_TORQ_SPEEDDOWN]   = m_fSpeedDown;
    ptCtrl->fTorqConf[INDEX_TORQ_SHOW]        = m_fShow;
    ptCtrl->fTorqConf[INDEX_TORQ_UPPERTAI]    = m_fUpperTai;
    ptCtrl->fTorqConf[INDEX_TORQ_LOWERTAI]    = m_fLowerTai;

    ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT]      = m_fMaxCir;
    ptCtrl->fTurnConf[INDEX_TURN_UPPERLIMIT]    = m_fUpperCir;
    ptCtrl->fTurnConf[INDEX_TURN_CONTROL]  = m_fControlCir;
    ptCtrl->fTurnConf[INDEX_TURN_LOWERLIMIT]    = m_fLowerCir;
    ptCtrl->fTurnConf[INDEX_TURN_MAXDELTA] = m_fMaxDeltaCir;
    ptCtrl->fTurnConf[INDEX_TURN_MINDELTA] = m_fMinDeltaCir;

    ptCtrl->fFullRPM      = m_fMaxRPM;

    ptCtrl->fMinShlSlope = m_fMinShlSlope;
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
    case IDC_EDITUPPERTAI:
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
        break;

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

void CDlgParaSet::JudgeFixShowParaChanged(UINT nIndex, TUBECFG* ptTubing)
{
    COLORREF    clrCtrl;
    TUBECFG   *ptOldTubing = &theApp.m_tParaCfg.tTubeCfg;

    COMP_BL(nIndex, INDEX_TUBE_FACTORY);
    COMP_BG(nIndex, INDEX_TUBE_COUPL);
    ASSERT_NULL(ptTubing);

    clrCtrl = m_clrNormal;
    if(theApp.IsFixTube() != m_bFixedTube)
        clrCtrl = m_clrChanged;
    else if (ptOldTubing->nFixTube[nIndex] != ptTubing->nFixTube[nIndex])
        clrCtrl = m_clrChanged;

    m_cbFixTubingOpt[nIndex].m_ColorText = clrCtrl;
    m_cbFixTubingOpt[nIndex].Invalidate();
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

void CDlgParaSet::OnCbnKillfocusCbfixpara01()
{
    JudgeFixShowParaChanged(INDEX_TUBE_FACTORY, m_ptCurTub);
}
void CDlgParaSet::OnCbnKillfocusCbfixpara05()
{
    JudgeFixShowParaChanged(INDEX_TUBE_OEM, m_ptCurTub);
}
void CDlgParaSet::OnCbnKillfocusCbfixpara06()
{
    JudgeFixShowParaChanged(INDEX_TUBE_SIZE, m_ptCurTub);
}
void CDlgParaSet::OnCbnKillfocusCbfixpara07()
{
    JudgeFixShowParaChanged(INDEX_TUBE_MATER, m_ptCurTub);
}
void CDlgParaSet::OnCbnKillfocusCbfixpara08()
{
    JudgeFixShowParaChanged(INDEX_TUBE_COUPL, m_ptCurTub);
}

/* 20210419 最大，最小是最佳的1.1    0.9倍，满屏是1.3倍，修改最佳扭矩后默认值 */
void CDlgParaSet::OnKillfocusEditopttorq()
{
    double oldOptTorq = m_fOptTorq;
    UpdateData(TRUE);

    /* 值没有修改则直接返回 */
    COMP_BE(oldOptTorq, m_fOptTorq);

    m_fControl      = m_fOptTorq;
    m_fMaxLimit     = HAND_CEIL (m_fOptTorq * 1.3);
    /*m_fUpperLimit   = HAND_CEIL (m_fOptTorq * 1.1);
    m_fLowerLimit   = HAND_FLOOR(m_fOptTorq * 0.9);*/
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

void CDlgParaSet::ChgTorqbyShowPara(bool bInit)
{
    UINT i = 0;
    TUBECFG   *ptCfg = g_cTubing.m_tTubCfg.ptCfg;

    for(i=0; i<g_cTubing.m_tTubCfg.nNum; i++)
    {
        if( ptCfg[i].nFixTube[INDEX_TUBE_OEM]   == m_ptCurTub->nFixTube[INDEX_TUBE_OEM] &&
            ptCfg[i].nFixTube[INDEX_TUBE_SIZE]  == m_ptCurTub->nFixTube[INDEX_TUBE_SIZE] &&
            ptCfg[i].nFixTube[INDEX_TUBE_MATER] == m_ptCurTub->nFixTube[INDEX_TUBE_MATER] &&
            ptCfg[i].nFixTube[INDEX_TUBE_COUPL] == m_ptCurTub->nFixTube[INDEX_TUBE_COUPL])
        {
            //m_fUpperLimit   = ptCfg[i].nTorqVal[INDEX_TUBE_MAXTORQ];
            m_fOptTorq      = ptCfg[i].nTorqVal[INDEX_TUBE_OPTTORQ];
            //m_fLowerLimit   = ptCfg[i].nTorqVal[INDEX_TUBE_MINTORQ];
            /* lb.ft */
            if(g_tGlbCfg.nTorqUnit != 0)
            {
                //m_fUpperLimit   = ptCfg[i].nTorqVal[INDEX_TUBE_LBMAXTORQ];
                m_fOptTorq      = ptCfg[i].nTorqVal[INDEX_TUBE_LBOPTTORQ];
                //m_fLowerLimit   = ptCfg[i].nTorqVal[INDEX_TUBE_LBMINTORQ];
            }

            m_fLowerTai     = HAND_FLOOR(m_fOptTorq * 0.1);
            m_fSpeedDown    = HAND_CEIL(m_fOptTorq * 0.3);
            m_fUpperTai     = HAND_CEIL(m_fOptTorq * 0.7);
            //m_fMaxLimit     = (ceil(ceil(m_fUpperLimit * 1.15) / 100)) * 100;
            m_fMaxLimit = HAND_CEIL(m_fOptTorq * 1.3);

            if(!bInit)
                m_fControl = m_fOptTorq;

            UpdateData(FALSE);
            return;
        }
    }
}

void CDlgParaSet::ChangeTubingShowBox(UINT nIndex)
{
    TUBECFG tTmpTub = { 0 };
    UINT    nCurNO = 0;
    int     i = 0;

    COMP_BL(nIndex, INDEX_TUBE_FACTORY);
    COMP_BG(nIndex, INDEX_TUBE_COUPL);

    // 获取和设置油管5个参数值
    nCurNO = g_cTubing.GetTubeNOByName(nIndex, m_ptCurTub->nFixTube[INDEX_TUBE_FACTORY], &m_cbFixTubingOpt[nIndex], m_nCurLang);
    if (!g_cTubing.CheckTubeNOChanged(nIndex, nCurNO, m_ptCurTub, &tTmpTub))
        return;
    /* 后续序号从m_ptCurTub获取 */
    for (i = nIndex - 1; i >= 0; i--)
    {
        tTmpTub.nFixTube[i] = m_ptCurTub->nFixTube[i];
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

    *m_ptCurTub = tTmpTub;
}

void CDlgParaSet::OnCbnSelchangeCbfixpara01()
{
    ChangeTubingShowBox(INDEX_TUBE_FACTORY);
    ChgTorqbyShowPara();
}

void CDlgParaSet::OnCbnSelchangeCbfixpara05()
{
    ChangeTubingShowBox(INDEX_TUBE_OEM);
    ChgTorqbyShowPara();
}

void CDlgParaSet::OnCbnSelchangeCbfixpara06()
{
    ChangeTubingShowBox(INDEX_TUBE_SIZE);
    ChgTorqbyShowPara();
}

void CDlgParaSet::OnCbnSelchangeCbfixpara07()
{
    ChangeTubingShowBox(INDEX_TUBE_MATER);
    ChgTorqbyShowPara();
}

void CDlgParaSet::OnCbnSelchangeCbfixpara08()
{
    ChangeTubingShowBox(INDEX_TUBE_COUPL);
    ChgTorqbyShowPara();
}

void CDlgParaSet::OnBnClickedCkfixedtubing()
{
    int     i = 0;
    UpdateData(TRUE);
    ShowTubingCtrl();

    /* 自选管材转固定管材 */
    if(m_bFixedTube && !theApp.IsFixTube())
    {
        ChangeTubingShowBox(INDEX_TUBE_FACTORY);
    }

    /* 固定管材转自选管材 */
    if(!m_bFixedTube && theApp.IsFixTube())
    {
        m_cbSetShowOption[0].m_ColorText = m_clrChanged;
        m_cbSetShowOption[0].Invalidate();
        for(i= FIXINDEX_OEM; i<= FIXINDEX_COUPL; i++)
        {
            m_cbSetShowOption[i].m_ColorText = m_clrChanged;
            m_cbSetShowOption[i].Invalidate();
        }
    }
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

    if(m_tempCfg.tTubeCfg.nIndex > 0)
        m_bFixedTube = TRUE;

    SetParaValue(&m_tempCfg, &m_tempShow);

    InitFixShowPara();

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
