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

#define     CBINDEX_FACTORY     15
#define     CBINDEX_OEM         3
#define     CBINDEX_SIZE        4
#define     CBINDEX_MAT         5
#define     CBINDEX_COUPL       6

/////////////////////////////////////////////////////////////////////////////
// CDlgParaSet dialog

CDlgParaSet::CDlgParaSet(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgParaSet::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgParaSet)
    m_strParaFile   = _T("");
    m_strMemo       = _T("");
    m_nReset        = 0;
    m_fControlCir   = 0.0;
    m_fControl      = 0.0;
    m_fOptTorq      = 0.0;
    m_fLowerCir     = 0.0;
    m_fLowerLimit   = 0.0;
    m_fMaxCir       = 0.0;
    m_fMaxLimit     = 0.0;
    m_fMaxRPM       = 0.0;
    m_fShow         = 0.0;
    m_fSpeedDown    = 0.0;
    m_fUpperCir     = 0.0;
    m_fUpperLimit   = 0.0;
    m_fPlus         = 0.0;
    m_fLowerTai     = 0.0f;
    m_fUpperTai     = 0.0f;
    m_fMinShlSlope  = 0.0f;
    m_fMaxDeltaCir  = 0.0f;
    m_fMinDeltaCir  = 0.0f;
    m_bFixedTubing  = TRUE;
    //}}AFX_DATA_INIT
}


void CDlgParaSet::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgParaSet)
    DDX_Control(pDX, IDC_COMBOPORT, m_cbPort);
    DDX_Control(pDX, IDC_COMBOIMGNUM, m_cbImgNum);
    DDX_Text(pDX, IDC_EDITCONTROLCIR, m_fControlCir);
    DDX_Control(pDX, IDC_EDITCONTROLCIR, m_neControlCir);
    DDX_Text(pDX, IDC_EDITCONTROLTORQ, m_fControl);
    DDX_Control(pDX, IDC_EDITCONTROLTORQ, m_neControl);
    DDX_Text(pDX, IDC_EDITOPTTORQ, m_fOptTorq);
    DDX_Control(pDX, IDC_EDITOPTTORQ, m_neOptTorq);
    DDX_Text(pDX, IDC_EDITLOWERCIR, m_fLowerCir);
    DDX_Control(pDX, IDC_EDITLOWERCIR, m_neLowerCir);
    DDX_Text(pDX, IDC_EDITLOWERTORQ, m_fLowerLimit);
    DDX_Control(pDX, IDC_EDITLOWERTORQ, m_neLowerLimit);
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
    DDX_Text(pDX, IDC_EDITUPPERTORQ, m_fUpperLimit);
    DDX_Control(pDX, IDC_EDITUPPERTORQ, m_neUpperLimit);
    DDX_Text(pDX, IDC_EDITPARAFILE, m_strParaFile);
    DDX_Text(pDX, IDC_EDITRESET, m_nReset);
    DDX_Text(pDX, IDC_EDITPLUS, m_fPlus);
    DDX_Control(pDX, IDC_EDITPLUS, m_nePlus);
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
    DDX_Check(pDX, IDC_CKFIXEDTUBING, m_bFixedTubing);
    DDX_Text(pDX, IDC_CBFIXPARA04, m_strFixShowOption[3]);
    DDX_Text(pDX, IDC_CBFIXPARA05, m_strFixShowOption[4]);
    DDX_Text(pDX, IDC_CBFIXPARA06, m_strFixShowOption[5]);
    DDX_Text(pDX, IDC_CBFIXPARA07, m_strFixShowOption[6]);
    DDX_Text(pDX, IDC_CBFIXPARA16, m_strFixShowOption[15]);
    DDX_Control(pDX, IDC_CBFIXPARA04, m_cbFixShowOption[3]);
    DDX_Control(pDX, IDC_CBFIXPARA05, m_cbFixShowOption[4]);
    DDX_Control(pDX, IDC_CBFIXPARA06, m_cbFixShowOption[5]);
    DDX_Control(pDX, IDC_CBFIXPARA07, m_cbFixShowOption[6]);
    DDX_Control(pDX, IDC_CBFIXPARA16, m_cbFixShowOption[15]);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgParaSet, CDialog)
    //{{AFX_MSG_MAP(CDlgParaSet)
    ON_BN_CLICKED(IDC_PARAOPEN, OnParaopen)
    //ON_BN_CLICKED(IDC_PARANEW, OnParanew)
    ON_BN_CLICKED(IDC_PARASAVE, OnParasave)
    //ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
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
    ON_CBN_KILLFOCUS(IDC_CBFIXPARA04, &CDlgParaSet::OnCbnKillfocusCbfixpara04)
    ON_CBN_KILLFOCUS(IDC_CBFIXPARA05, &CDlgParaSet::OnCbnKillfocusCbfixpara05)
    ON_CBN_KILLFOCUS(IDC_CBFIXPARA06, &CDlgParaSet::OnCbnKillfocusCbfixpara06)
    ON_CBN_KILLFOCUS(IDC_CBFIXPARA07, &CDlgParaSet::OnCbnKillfocusCbfixpara07)
    ON_CBN_KILLFOCUS(IDC_CBFIXPARA16, &CDlgParaSet::OnCbnKillfocusCbfixpara16)
    ON_CBN_SELCHANGE(IDC_CBFIXPARA04, &CDlgParaSet::OnCbnSelchangeCbfixpara04)
    ON_CBN_SELCHANGE(IDC_CBFIXPARA05, &CDlgParaSet::OnCbnSelchangeCbfixpara05)
    ON_CBN_SELCHANGE(IDC_CBFIXPARA06, &CDlgParaSet::OnCbnSelchangeCbfixpara06)
    ON_CBN_SELCHANGE(IDC_CBFIXPARA07, &CDlgParaSet::OnCbnSelchangeCbfixpara07)
    ON_CBN_SELCHANGE(IDC_CBFIXPARA16, &CDlgParaSet::OnCbnSelchangeCbfixpara16)
    ON_BN_CLICKED(IDC_CKFIXEDTUBING, &CDlgParaSet::OnBnClickedCkfixedtubing)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgParaSet message handlers

BOOL CDlgParaSet::OnInitDialog()
{
    int     i = 0;

    CDialog::OnInitDialog();

    m_strParaFile = theApp.m_strParaFile.c_str();

    m_clrNormal  = RGB(0, 0, 0);
    m_clrChanged = RGB(255, 0, 0);
    m_nCurLang  = theApp.m_nLangType;

    /* 根据入参设置参数初始值 */
    SetParaValue(&m_tempCfg, &m_tempShow);

    InitFixShowPara();

    UpdateDlgLabel();

    UpdateData(FALSE);
    m_cbPort.SetCurSel(m_ucPort-1);
    SetImgNum();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgParaSet::SetImgNum()
{
    switch(theApp.m_nImgNum)
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

void CDlgParaSet::UpdateDlgLabel()
{
    switch(m_nCurLang)
    {
        case LANGUAGE_CHINESE:
            m_strLBM10.Format("扭矩参数(单位: %s)", theApp.m_strUnit);
            break;
        case LANGUAGE_ENGLISH:
            m_strLBM10.Format("Torque Parameter(%s)", theApp.m_strUnit);
            break;
        case LANGUAGE_RUSSIAN:
            m_strLBM10.Format("Пара. крут. мом.(ед: %s)", theApp.m_strUnit);
            break;
    }
}

void CDlgParaSet::SetParaValue(PARACFG *ptCfg, SHOWCFG *ptShow)
{
    CONTROLPARA *ptCtrl = NULL;
    COMMONCFG   *ptComm = NULL;
    WORD        i = 0;
    WORD        j = 0;

    ASSERT_NULL(ptCfg);
    ASSERT_NULL(ptShow);

    ptCtrl = &ptCfg->tCtrl;
    ptComm = &ptCfg->tComm;

    for(i=0; i<MAXPARANUM+1; i++)   //  i<ptShow->wParaNum+1 && 
    {
        m_strSetShowName[i] = ptShow->tShow[i].strName.c_str();

        m_cbSetShowOption[i].ResetContent();
        for(j=0; j<ptShow->tOption[i].wOptNum && j<MAXOPTIONNUM; j++)
        {
            m_cbSetShowOption[i].AddString(ptShow->tOption[i].strOpt[j].c_str());
        }

        m_strSetShowOption[i] = ptShow->tShow[i].strValue.c_str();
        if(m_strSetShowOption[i].IsEmpty() && ptShow->tOption[i].wOptNum > 0)
            m_strSetShowOption[i] = ptShow->tOption[i].strOpt[0].c_str();
    }

    m_fMaxLimit     = ptCtrl->fMaxLimit;
    m_fUpperLimit   = ptCtrl->fUpperLimit;
    m_fControl      = ptCtrl->fControl;
    m_fOptTorq      = ptCtrl->fOptTorq;
    m_fLowerLimit   = ptCtrl->fLowerLimit;
    m_fSpeedDown    = ptCtrl->fSpeedDown;
    m_fShow         = ptCtrl->fShow;

    m_fMaxCir       = ptCtrl->fMaxCir;
    m_fUpperCir     = ptCtrl->fUpperCir;
    m_fControlCir   = ptCtrl->fControlCir;
    m_fLowerCir     = ptCtrl->fLowerCir;

    m_fMaxRPM       = ptCtrl->fMaxRPM;
    m_fPlus         = ptCtrl->fPlus;

    m_fUpperTai     = ptComm->fUpperTai;
    m_fLowerTai     = ptComm->fLowerTai;
    m_fMinShlSlope  = ptComm->fMinShlSlope;
    m_fMaxDeltaCir  = ptComm->fMaxDeltaCir;
    m_fMinDeltaCir  = ptComm->fMinDeltaCir;
    m_strMemo       = ptComm->aucMemo;

    m_nReset        = theApp.m_nReset;
    m_bFixedTubing  = ptShow->bFixTub;
}

void CDlgParaSet::InitFixShowPara()
{
    m_cTubing.SetFactoryBox(&m_cbFixShowOption[CBINDEX_FACTORY], &m_tCurTub, m_nCurLang);
    m_cTubing.SetOEMBox(&m_cbFixShowOption[CBINDEX_OEM], &m_tCurTub, m_nCurLang);
    m_cTubing.SetSizeBox(&m_cbFixShowOption[CBINDEX_SIZE], &m_tCurTub, m_nCurLang);
    m_cTubing.SetMatBox(&m_cbFixShowOption[CBINDEX_MAT], &m_tCurTub, m_nCurLang);
    m_cTubing.SetCouplBox(&m_cbFixShowOption[CBINDEX_COUPL], &m_tCurTub, m_nCurLang);
    
    ShowTubingCtrl(true);
}

void CDlgParaSet::ShowTubingCtrl(bool bInit)
{
    m_cbSetShowOption[CBINDEX_FACTORY].ShowWindow(!m_bFixedTubing);
    m_cbSetShowOption[CBINDEX_OEM].ShowWindow(!m_bFixedTubing);
    m_cbSetShowOption[CBINDEX_SIZE].ShowWindow(!m_bFixedTubing);
    m_cbSetShowOption[CBINDEX_MAT].ShowWindow(!m_bFixedTubing);
    m_cbSetShowOption[CBINDEX_COUPL].ShowWindow(!m_bFixedTubing);
    
    m_cbFixShowOption[CBINDEX_FACTORY].ShowWindow(m_bFixedTubing);
    m_cbFixShowOption[CBINDEX_OEM].ShowWindow(m_bFixedTubing);
    m_cbFixShowOption[CBINDEX_SIZE].ShowWindow(m_bFixedTubing);
    m_cbFixShowOption[CBINDEX_MAT].ShowWindow(m_bFixedTubing);
    m_cbFixShowOption[CBINDEX_COUPL].ShowWindow(m_bFixedTubing);

    /* 由其他管材改为设定管材，修改默认扭矩值配置 */
    if(m_bFixedTubing)
        ChgTorqbyShowPara(bInit);
}

void CDlgParaSet::OnParaopen()
{
    int     iLen = 0;
    CString strFilter;

    strFilter.Format(IDS_STRINIFILTER);
    CFileDialog fileDlg(TRUE,"ini",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,strFilter,NULL);
    COMP_BNE(fileDlg.DoModal(), IDOK);

    m_strParaFile = fileDlg.GetPathName();
    SetDlgItemText(IDC_EDITPARAFILE,m_strParaFile);
    iLen =  ((CEdit*)GetDlgItem(IDC_EDITPARAFILE))->LineLength();
    ((CEdit*)GetDlgItem(IDC_EDITPARAFILE))->SetSel(iLen,iLen);

    theApp.ReadPara(m_strParaFile.GetBuffer(0), &m_tempCfg);
    theApp.ReadShowPara(&m_tempShow);
    m_ucPort = 1;
    SetParaValue(&m_tempCfg, &m_tempShow);
    m_strParaFile.ReleaseBuffer();
    UpdateData(FALSE);

    m_cbPort.SetCurSel(m_ucPort-1);
    SetImgNum();
}
#if 0
void CDlgParaSet::OnParanew()
{
    CString strTemp;

    strTemp.Format(IDS_STRTMPINI);  // "temp.ini"
    strTemp = theApp.m_strAppPath.c_str() + strTemp;
    theApp.ReadPara(strTemp.GetBuffer(0), &m_tempCfg, FALSE);
    theApp.ReadShowPara(m_nCurLang, &m_tempShow);
    m_strParaFile.Empty();
    m_ucPort = 1;
    SetParaValue(&m_tempCfg, &m_tempShow);
    strTemp.ReleaseBuffer();
    UpdateData(FALSE);
}
#endif
void CDlgParaSet::OnParasave()
{
    CString strInfo;

    UpdateData(TRUE);

    if(m_strParaFile.IsEmpty())
    {
        strInfo.Format(IDS_STRINFPARAFILE);
        theApp.SaveShowMessage(strInfo, MB_OK|MB_ICONINFORMATION);
        return;
    }

    if(!GetParaValue(&m_tempCfg, &m_tempShow))
    {
        theApp.SaveShowMessage(strInfo, MB_OK|MB_ICONINFORMATION);
        return;
    }

    /* 保存前先校验下参数的有效范围 */
    theApp.AdjustParaValue(&m_tempCfg);
    theApp.WritePara(m_strParaFile.GetBuffer(0), &m_tempCfg);
    WriteOtherPara(m_strParaFile.GetBuffer(0));
    m_strParaFile.ReleaseBuffer();

    CheckShowOption(&m_tempShow);
    theApp.WriteShowPara();

    UpdateData(FALSE);

    strInfo.Format(IDS_STRINFSAVEPARAFILE, m_strParaFile);
    theApp.SaveShowMessage(strInfo, MB_OK|MB_ICONEXCLAMATION);
}
#if 0
void CDlgParaSet::OnDefault()
{
    CString strTemp;

    UpdateData(TRUE);
    strTemp.Format(IDS_STRTMPINI);
    strTemp = theApp.m_strAppPath.c_str() + strTemp;
    theApp.ReadPara(strTemp.GetBuffer(0), &m_tempCfg, FALSE);
    theApp.ReadShowPara(m_nCurLang, &m_tempShow);
    m_ucPort = 1;
    SetParaValue(&m_tempCfg, &m_tempShow);
    strTemp.ReleaseBuffer();
    UpdateData(FALSE);
}
#endif
BOOL CDlgParaSet::GetParaValue(PARACFG *ptCfg, SHOWCFG *ptShow)
{
    CString     strTemp;
    CONTROLPARA *ptCtrl = NULL;
    COMMONCFG   *ptComm = NULL;
    WORD        i = 0;

    ASSERT_NULL_R(ptCfg, FALSE);
    ASSERT_NULL_R(ptShow, FALSE);

    ptCtrl = &ptCfg->tCtrl;
    ptComm = &ptCfg->tComm;

    for(i=0; i<MAXPARANUM+1; i++)   //  i<ptShow->wParaNum+1 && 
    {
        ptShow->tShow[i].strValue = m_strSetShowOption[i];
    }

    ptCtrl->fMaxLimit    = m_fMaxLimit;
    ptCtrl->fUpperLimit  = m_fUpperLimit;
    ptCtrl->fControl     = m_fControl;
    ptCtrl->fOptTorq     = m_fOptTorq;
    ptCtrl->fLowerLimit  = m_fLowerLimit;
    ptCtrl->fSpeedDown   = m_fSpeedDown;
    ptCtrl->fShow        = m_fShow;

    ptCtrl->fMaxCir      = m_fMaxCir;
    ptCtrl->fUpperCir    = m_fUpperCir;
    ptCtrl->fControlCir  = m_fControlCir;
    ptCtrl->fLowerCir    = m_fLowerCir;

    ptCtrl->fMaxRPM      = m_fMaxRPM;
    ptCtrl->fPlus        = m_fPlus;

    m_ucPort = m_cbPort.GetCurSel() + 1;

    ptComm->fUpperTai    = m_fUpperTai;
    ptComm->fLowerTai    = m_fLowerTai;
    ptComm->fMinShlSlope = m_fMinShlSlope;
    ptComm->fMaxDeltaCir = m_fMaxDeltaCir;
    ptComm->fMinDeltaCir = m_fMinDeltaCir;
    lstrcpyn(ptComm->aucMemo, m_strMemo, MAXMEMOLEN);

    theApp.m_nReset      = m_nReset;
    ptShow->bFixTub      = m_bFixedTubing;
    m_cbImgNum.GetWindowText(strTemp);
    theApp.m_nImgNum     = atoi(strTemp);

    return TRUE;
}

void CDlgParaSet::OnOK()
{
    CString     strInfo;

    UpdateData(TRUE);

    if(m_fMaxLimit < m_fUpperLimit)
    {
        AfxMessageBox(IDS_STRSMAXTORQ);
        return;
    }
    if(m_fMaxCir < m_fUpperCir)
    {
        AfxMessageBox(IDS_STRSMAXCIR);
        return;
    }
    if(theApp.FindNotFileChar(m_strSetShowOption[m_tempShow.wFileName]))
    {
        strInfo.Format(IDS_STRSNCFILENAME, 
            m_tempShow.tShow[m_tempShow.wFileName].strName.c_str());
        AfxMessageBox(strInfo);
        return;
    }

    GetParaValue(&m_tempCfg, &m_tempShow);

    CheckShowOption(&m_tempShow);
    WriteOtherPara(m_strParaFile.GetBuffer(0));

    UpdateData(FALSE);

    CDialog::OnOK();
}

void CDlgParaSet::CheckShowOption(SHOWCFG *ptShow)
{
    WORD    i          = 0;
    WORD    j          = 0;
    BOOL    bFindStr   = FALSE;
    CString strTemp;

    ASSERT_NULL(ptShow);

    for(i=0; i<MAXPARANUM+1; i++)   // i<ptShow->wParaNum+1 && 
    {
        bFindStr = FALSE;
        if(m_strSetShowOption[i].IsEmpty())
            continue;
        for(j=0; j<ptShow->tOption[i].wOptNum && j<MAXOPTIONNUM; j++)
        {
            strTemp = ptShow->tOption[i].strOpt[j].c_str();
            if(m_strSetShowOption[i].CompareNoCase(strTemp) == 0)
            {
                bFindStr = TRUE;
                break;
            }
        }

        if(!bFindStr)
        {
            theApp.InsertShowOption(ptShow, i, m_strSetShowOption[i], &m_cbSetShowOption[i]);
        }
    }
    return;
}

void CDlgParaSet::WriteOtherPara(string strParaName)
{
    CString     strValue;

    strValue.Format("%d",m_nReset);
    theApp.WriteConfigStr(IDS_STRPNOTHER,IDS_STRPIRESET,strValue,strParaName);

    m_cbImgNum.GetWindowText(strValue);
    theApp.WriteConfigStr(IDS_STRPNOTHER,IDS_STRPIEXPORTIMGNUM,strValue,strParaName);
}

HBRUSH CDlgParaSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    COLORREF    clrCtrl;
    CString     strContent;

    CONTROLPARA *ptCtrl = &theApp.m_tParaCfg.tCtrl;
    COMMONCFG   *ptComm = &theApp.m_tParaCfg.tComm;
    SHOWCFG     *ptShow = theApp.m_ptCurShow;

    GetDlgItemText(pWnd->GetDlgCtrlID(), strContent);
    if (strContent.IsEmpty())
        return hbr;

    // TODO:  在此更改 DC 的任何特性
    switch (pWnd->GetDlgCtrlID())//对某一个特定控件进行判断
    {
    // 扭矩参数
    case IDC_EDITSHOWTORQ:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fShow);
        break;
    case IDC_EDITUPPERTORQ:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fUpperLimit);
        break;
    case IDC_EDITCONTROLTORQ:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fControl);
        break;
    case IDC_EDITOPTTORQ:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fOptTorq);
        break;
    case IDC_EDITLOWERTORQ:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fLowerLimit);
        break;
    case IDC_EDITMAXTORQ:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fMaxLimit);
        break;
    case IDC_EDITUPPERTAI:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptComm->fUpperTai);
        break;
    case IDC_EDITSPEEDTORQ:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fSpeedDown);
        break;
    case IDC_EDITLOWERTAI:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptComm->fLowerTai);
        break;
    case IDC_EDITMINSHLSLOPE:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptComm->fMinShlSlope);
        break;
    case IDC_EDITMAXDELTACIR:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptComm->fMaxDeltaCir);
        break;
    case IDC_EDITMINDELTACIR:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptComm->fMinDeltaCir);
        break;

    // 扭拧周数
    case IDC_EDITUPPERCIR:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fUpperCir);
        break;
    case IDC_EDITCONTROLCIR:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fControlCir);
        break;
    case IDC_EDITMAXRPM:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fMaxRPM);
        break;
    case IDC_EDITLOWERCIR:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fLowerCir);
        break;
    case IDC_EDITMAXCIR:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fMaxCir);
        break;
    case IDC_EDITPLUS:
        JUDGE_NUMBERPARA_CHANGE(atof(strContent), ptCtrl->fPlus);
        break;

    // 系统配置
    case IDC_COMBOPORT:
        JUDGE_NUMBERPARA_CHANGE((m_cbPort.GetCurSel() + 1), theApp.m_tPortCfg.ucPortNo);
        break;
    case IDC_COMBOIMGNUM:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), theApp.m_nImgNum);
        break;
    case IDC_EDITMEMO:
        JUDGE_STRPARA_CHANGE(strContent, ptComm->aucMemo);
        break;
    case IDC_EDITRESET:
        JUDGE_NUMBERPARA_CHANGE(atoi(strContent), theApp.m_nReset);
        break;
    case IDC_EDITPARAFILE:
        JUDGE_STRPARA_CHANGE(strContent, theApp.m_strParaFile.c_str());
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

    COMP_BGE(nIndex, (MAXPARANUM+1));

    UpdateData(TRUE);
    clrCtrl = m_clrNormal;
    if (m_strSetShowOption[nIndex].Compare(theApp.m_ptCurShow->tShow[nIndex].strValue.c_str()) != 0)
        clrCtrl = m_clrChanged;
    m_cbSetShowOption[nIndex].m_ColorText = clrCtrl;
    m_cbSetShowOption[nIndex].Invalidate();
}

void CDlgParaSet::JudgeFixShowParaChanged(UINT nIndex)
{
    COLORREF    clrCtrl;
    BOOL        bChanged = FALSE;
    TUBINGCFG   *ptOldTubing = &theApp.m_ptCurShow->tTubingCfg;

    clrCtrl = m_clrNormal;

    if(theApp.m_ptCurShow->bFixTub != m_bFixedTubing)
        bChanged = TRUE;
    else
    {
        switch(nIndex)
        {
            case 3:
                if(m_tCurTub.nOEM != ptOldTubing->nOEM)
                    bChanged = TRUE;
                break;
            case 4:
                if(m_tCurTub.nSize != ptOldTubing->nSize)
                    bChanged = TRUE;
                break;
            case 5:
                if(m_tCurTub.nMat != ptOldTubing->nMat)
                    bChanged = TRUE;
                break;
            case 6:
                if(m_tCurTub.nCoupling != ptOldTubing->nCoupling)
                    bChanged = TRUE;
                break;
            case 15:
                if(m_tCurTub.nFactory != ptOldTubing->nFactory)
                    bChanged = TRUE;
                break;
            default:
                return;
        }
    }

    if (bChanged)
        clrCtrl = m_clrChanged;
    m_cbFixShowOption[nIndex].m_ColorText = clrCtrl;
    m_cbFixShowOption[nIndex].Invalidate();
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

void CDlgParaSet::OnCbnKillfocusCbfixpara04()
{
    JudgeFixShowParaChanged(3);
}
void CDlgParaSet::OnCbnKillfocusCbfixpara05()
{
    JudgeFixShowParaChanged(4);
}
void CDlgParaSet::OnCbnKillfocusCbfixpara06()
{
    JudgeFixShowParaChanged(5);
}
void CDlgParaSet::OnCbnKillfocusCbfixpara07()
{
    JudgeFixShowParaChanged(6);
}
void CDlgParaSet::OnCbnKillfocusCbfixpara16()
{
    JudgeFixShowParaChanged(15);
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
    m_fUpperLimit   = HAND_CEIL (m_fOptTorq * 1.1);
    m_fLowerLimit   = HAND_FLOOR(m_fOptTorq * 0.9);
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
    TUBINGCFG   *ptCfg = m_cTubing.m_tAllTub.ptCfg;

    for(i=0; i<m_cTubing.m_tAllTub.nNum; i++)
    {
        if( ptCfg[i].nOEM     == m_tCurTub.nOEM     &&
            ptCfg[i].nSize    == m_tCurTub.nSize    &&
            ptCfg[i].nMat     == m_tCurTub.nMat     &&
            ptCfg[i].nCoupling== m_tCurTub.nCoupling)
        {
            m_fUpperLimit   = ptCfg[i].nMaxTorq;
            m_fOptTorq      = ptCfg[i].nOptTorq;
            m_fLowerLimit   = ptCfg[i].nMinTorq;
            /* lb.ft */
            if(theApp.m_nTorqUnit != 0)
            {
                m_fUpperLimit   = ptCfg[i].lMaxTorq;
                m_fOptTorq      = ptCfg[i].lOptTorq;
                m_fLowerLimit   = ptCfg[i].lMinTorq;
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

/*
    5: factory
    4: OEM
    3: Size
    2: Mat
    1: Coupling
*/
void CDlgParaSet::ChangeTubingShowBox(UINT nNum)
{
    switch(nNum)
    {
        case 5:
            JudgeFixShowParaChanged(15);
        case 4:
            JudgeFixShowParaChanged(3);
        case 3:
            JudgeFixShowParaChanged(4);
        case 2:
            JudgeFixShowParaChanged(5);
        case 1:
            JudgeFixShowParaChanged(6);
        default:
            break;
    }
}

void CDlgParaSet::OnCbnSelchangeCbfixpara16()
{
    UINT    i = 0;
    UINT    nOldFactory = m_tCurTub.nFactory;
    CString strFactory;
    FIXTUBPARA  *ptTubPara = m_cTubing.m_tFactory.ptPara;

    ASSERT_NULL(ptTubPara);

    m_cbFixShowOption[CBINDEX_FACTORY].GetWindowText(strFactory);

    for (i = 0; i < m_cTubing.m_tFactory.nNum; i++)
    {
        if (0 == strFactory.CompareNoCase(ptTubPara[i].strName[m_nCurLang].c_str()))
        {
            m_tCurTub.nFactory = ptTubPara[i].nNO;
            break;
        }
    }

    if(nOldFactory == m_tCurTub.nFactory)
        return;

    m_cTubing.InitOEMValue(&m_tCurTub);
    m_cTubing.InitSizeValue(&m_tCurTub);
    m_cTubing.InitMatValue(&m_tCurTub);
    m_cTubing.InitCouplValue(&m_tCurTub);

    InitFixShowPara();
    ChgTorqbyShowPara();
    ChangeTubingShowBox(5);
}

void CDlgParaSet::OnCbnSelchangeCbfixpara04()
{
    UINT    i = 0;
    UINT    nOldOEM = m_tCurTub.nOEM;
    CString strOEM;
    FIXTUBPARA  *ptTubPara = m_cTubing.m_tOEM.ptPara;

    ASSERT_NULL(ptTubPara);

    m_cbFixShowOption[CBINDEX_OEM].GetWindowText(strOEM);

    for (i = 0; i < m_cTubing.m_tOEM.nNum; i++)
    {
        /* OEM有厂家名称，不用比较厂家的值 */
        if (0 == strOEM.CompareNoCase(ptTubPara[i].strName[m_nCurLang].c_str()))
        {
            m_tCurTub.nOEM = ptTubPara[i].nNO;
            break;
        }
    }

    if(nOldOEM == m_tCurTub.nOEM)
        return;

    m_cTubing.InitSizeValue(&m_tCurTub);
    m_cTubing.InitMatValue(&m_tCurTub);
    m_cTubing.InitCouplValue(&m_tCurTub);

    InitFixShowPara();
    ChgTorqbyShowPara();
    ChangeTubingShowBox(4);
}

void CDlgParaSet::OnCbnSelchangeCbfixpara05()
{
    UINT    i = 0;
    UINT    nOldSize = m_tCurTub.nSize;
    CString strSize;
    FIXTUBPARA  *ptTubPara = m_cTubing.m_tSize.ptPara;

    ASSERT_NULL(ptTubPara);

    m_cbFixShowOption[CBINDEX_SIZE].GetWindowText(strSize);

    for (i = 0; i < m_cTubing.m_tSize.nNum; i++)
    {
        /* 厂家ID不同，跳过 */
        if((ptTubPara[i].nNO & 0xFF00) != (nOldSize & 0xFF00))
            continue;
        if (0 == strSize.CompareNoCase(ptTubPara[i].strName[m_nCurLang].c_str()))
        {
            m_tCurTub.nSize = ptTubPara[i].nNO;
            break;
        }
    }

    if(nOldSize == m_tCurTub.nSize)
        return;

    m_cTubing.InitMatValue(&m_tCurTub);
    m_cTubing.InitCouplValue(&m_tCurTub);

    InitFixShowPara();
    ChgTorqbyShowPara();
    ChangeTubingShowBox(3);
}

void CDlgParaSet::OnCbnSelchangeCbfixpara06()
{
    UINT    i = 0;
    UINT    nOldMat = m_tCurTub.nMat;
    CString strMat;
    FIXTUBPARA  *ptTubPara = m_cTubing.m_tMater.ptPara;

    ASSERT_NULL(ptTubPara);

    m_cbFixShowOption[CBINDEX_MAT].GetWindowText(strMat);

    for (i = 0; i < m_cTubing.m_tMater.nNum; i++)
    {
        /* 厂家ID不同，跳过 */
        if((ptTubPara[i].nNO & 0xFF00) != (nOldMat & 0xFF00))
            continue;
        if (0 == strMat.CompareNoCase(ptTubPara[i].strName[m_nCurLang].c_str()))
        {
            m_tCurTub.nMat = ptTubPara[i].nNO;
            break;
        }
    }

    if(nOldMat == m_tCurTub.nMat)
        return;

    m_cTubing.InitCouplValue(&m_tCurTub);

    InitFixShowPara();
    ChgTorqbyShowPara();
    ChangeTubingShowBox(2);
}

void CDlgParaSet::OnCbnSelchangeCbfixpara07()
{
    UINT    i = 0;
    UINT    nOldCoupl = m_tCurTub.nCoupling;
    CString strCoupl;
    FIXTUBPARA  *ptTubPara = m_cTubing.m_tCoupl.ptPara;

    ASSERT_NULL(ptTubPara);

    m_cbFixShowOption[CBINDEX_COUPL].GetWindowText(strCoupl);

    for (i = 0; i < m_cTubing.m_tCoupl.nNum; i++)
    {
        /* 厂家ID不同，跳过 */
        if((ptTubPara[i].nNO & 0xFF00) != (nOldCoupl & 0xFF00))
            continue;
        if (0 == strCoupl.CompareNoCase(ptTubPara[i].strName[m_nCurLang].c_str()))
        {
            m_tCurTub.nCoupling = ptTubPara[i].nNO;
            break;
        }
    }

    if(nOldCoupl == m_tCurTub.nCoupling)
        return;

    ChgTorqbyShowPara();
    ChangeTubingShowBox(1);
}

void CDlgParaSet::OnBnClickedCkfixedtubing()
{
    int     i = 0;
    UpdateData(TRUE);
    ShowTubingCtrl();

    /* 自选管材转固定管材 */
    if(m_bFixedTubing && !theApp.m_ptCurShow->bFixTub)
    {
        ChangeTubingShowBox(5);
    }

    /* 固定管材转自选管材 */
    if(!m_bFixedTubing && theApp.m_ptCurShow->bFixTub)
    {
        for(i=3; i<=6; i++)
        {
            m_cbSetShowOption[i].m_ColorText = m_clrChanged;
            m_cbSetShowOption[i].Invalidate();
        }
        m_cbSetShowOption[15].m_ColorText = m_clrChanged;
        m_cbSetShowOption[15].Invalidate();
    }
    UpdateData(FALSE);
}

