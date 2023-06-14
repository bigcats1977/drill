// DlgHisGrp.cpp : implementation file
//

#include "stdafx.h"
#include "datamod.h"
#include "DlgHisGrp.h"
#include "DlgListMod.h"
#include "DlgRemark.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHisGrp dialog

/* 确认修改保存 */
#define MODIFIED_SAVE()            {                                    \
        if(m_bModified)                                                 \
        {                                                               \
            if(IDYES == AfxMessageBox("当前数据已修改，是否需要保存？",    \
                            MB_YESNO|MB_ICONINFORMATION))               \
            {                                                           \
                SaveCurData();                                          \
                m_bModified  = FALSE;                                   \
            }                                                           \
        }                                                               \
    }

#define  CANMODIFY      (m_bCanMod && !m_bSecond)
#define  READONLY       (!m_bCanMod || m_bSecond)

CDlgHisGrp::CDlgHisGrp(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgHisGrp::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgHisGrp)
    EmptyEdit();
    m_bToolBuck = FALSE;
    //m_bBreakOut = FALSE;
    //}}AFX_DATA_INIT
    m_nBeginPos = 0;
    m_iGrpType = 0;
    m_bCanMod = FALSE;
    m_bSecond = FALSE;
}

void CDlgHisGrp::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgHisGrp)
    DDX_Control(pDX, IDC_XHISAXIS1, m_xHisAxis1);
    DDX_Control(pDX, IDC_YHISAXIS1, m_yHisAxis1);
    DDX_Control(pDX, IDC_XHISAXIS2, m_xHisAxis2);
    DDX_Control(pDX, IDC_YHISAXIS2, m_yHisAxis2);
    DDX_Text(pDX, IDC_HISMEMO, m_strMemo);
    DDX_Text(pDX, IDC_HISCONTROL, m_strControl);
    DDX_Text(pDX, IDC_HISBREAKOUT, m_strBOTorq);
    DDX_Text(pDX, IDC_HISQUALITY, m_strQuality);
    DDX_Text(pDX, IDC_HISTIME, m_strTime);
    DDX_Text(pDX, IDC_HISBOTIME, m_strBOTime);
    DDX_Text(pDX, IDC_HISNO, m_strNo);
    DDX_Text(pDX, IDC_HISCIR, m_strCir);
    DDX_Text(pDX, IDC_HISBOCIR, m_strBOCir);
    DDX_Check(pDX, IDC_CHECKTOOLBUCK, m_bToolBuck);
    //DDX_Check(pDX, IDC_CHECKHISSHACKLE, m_bBreakOut);
    DDX_Text(pDX, IDC_STATIC_G1, m_strLBG1);
    DDX_Text(pDX, IDC_STATIC_G10, m_strLBG10);
    DDX_Text(pDX, IDC_STATIC_G16, m_strLBG16);
    DDX_Text(pDX, IDC_HISSHOW01, m_strHisShowName[0]);
    DDX_Text(pDX, IDC_HISSHOW02, m_strHisShowName[1]);
    DDX_Text(pDX, IDC_HISSHOW03, m_strHisShowName[2]);
    DDX_Text(pDX, IDC_HISSHOW04, m_strHisShowName[3]);
    DDX_Text(pDX, IDC_HISSHOW05, m_strHisShowName[4]);
    DDX_Text(pDX, IDC_HISSHOW06, m_strHisShowName[5]);
    DDX_Text(pDX, IDC_HISSHOW07, m_strHisShowName[6]);
    DDX_Text(pDX, IDC_HISSHOW08, m_strHisShowName[7]);
    DDX_Text(pDX, IDC_HISSHOW09, m_strHisShowName[8]);
    DDX_Text(pDX, IDC_HISSHOW10, m_strHisShowName[9]);
    DDX_Text(pDX, IDC_HISSHOW11, m_strHisShowName[10]);
    DDX_Text(pDX, IDC_HISSHOW12, m_strHisShowName[11]);
    DDX_Text(pDX, IDC_HISSHOW13, m_strHisShowName[12]);
    DDX_Text(pDX, IDC_HISSHOW14, m_strHisShowName[13]);
    DDX_Text(pDX, IDC_HISSHOW15, m_strHisShowName[14]);
    //DDX_Text(pDX, IDC_HISSHOW16, m_strHisShowName[15]);
    DDX_Text(pDX, IDC_HISEDTOUTJOINT, m_strOutJoint);
    DDX_Text(pDX, IDC_HISEDTOUTWELL, m_strOutWellNO);
    DDX_Radio(pDX, IDC_RADIOONE, m_iSingleSTD);
    DDX_Radio(pDX, IDC_RADIOGRPBOTH, m_iGrpType);
    DDX_Text(pDX, IDC_HISEDIT01, m_strHisShowValue[0]);
    DDX_Text(pDX, IDC_HISEDIT02, m_strHisShowValue[1]);
    DDX_Text(pDX, IDC_HISEDIT03, m_strHisShowValue[2]);
    DDX_Text(pDX, IDC_HISEDIT04, m_strHisShowValue[3]);
    DDX_Text(pDX, IDC_HISEDIT05, m_strHisShowValue[4]);
    DDX_Text(pDX, IDC_HISEDIT06, m_strHisShowValue[5]);
    DDX_Text(pDX, IDC_HISEDIT07, m_strHisShowValue[6]);
    DDX_Text(pDX, IDC_HISEDIT08, m_strHisShowValue[7]);
    DDX_Text(pDX, IDC_HISEDIT09, m_strHisShowValue[8]);
    DDX_Text(pDX, IDC_HISEDIT10, m_strHisShowValue[9]);
    DDX_Text(pDX, IDC_HISEDIT11, m_strHisShowValue[10]);
    DDX_Text(pDX, IDC_HISEDIT12, m_strHisShowValue[11]);
    DDX_Text(pDX, IDC_HISEDIT13, m_strHisShowValue[12]);
    DDX_Text(pDX, IDC_HISEDIT14, m_strHisShowValue[13]);
    DDX_Text(pDX, IDC_HISEDIT15, m_strHisShowValue[14]);
    //DDX_Text(pDX, IDC_HISEDIT16, m_strHisShowValue[15]);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgHisGrp, CDialog)
    //{{AFX_MSG_MAP(CDlgHisGrp)
    ON_BN_CLICKED(IDC_BTNPRI, OnBtnpri)
    ON_BN_CLICKED(IDC_BTNNEXT, OnBtnnext)
    ON_BN_CLICKED(IDC_BTNMOD, OnBtnmod)
    ON_BN_CLICKED(IDC_BTNMODPARA, OnBtnmodpara)
    ON_BN_CLICKED(IDC_BTNREMARK, OnBtnremark)
    //ON_BN_CLICKED(IDC_BTNSAVE, OnBtnsave)
    ON_MESSAGE(WM_UPDATE_SELPOS, SelPosChange)
    ON_BN_CLICKED(IDC_CHECKTOOLBUCK, OnBnClickedChecktoolbuck)
    ON_BN_CLICKED(IDC_RADIOONE, OnBnClickedRadioone)
    ON_BN_CLICKED(IDC_RADIOTWO, OnBnClickedRadiotwo)
    ON_BN_CLICKED(IDC_RADIOTHREE, OnBnClickedRadiothree)
    ON_BN_CLICKED(IDC_RADIOGRPBOTH, OnBnClickedRadiogrpboth)
    ON_BN_CLICKED(IDC_RADIOGRPMU, OnBnClickedRadiogrpmu)
    ON_BN_CLICKED(IDC_RADIOGRPBO, OnBnClickedRadiogrpbo)
    //ON_BN_CLICKED(IDC_CHECKHISSHACKLE, OnBnClickedCheckhisshackle)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHisGrp message handlers

BOOL CDlgHisGrp::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_wndLineHis.SubclassDlgItem(IDC_LINE_HISTORQUE, this);
    m_wndLineHis.SetParent(this);
    m_xHisAxis1.SetRulerType(RT_HORIZONTAL);
    m_yHisAxis1.SetRulerType(RT_VERTICAL);
    m_wndRpmHis.SubclassDlgItem(IDC_LINE_HISRPM, this);
    m_xHisAxis2.SetRulerType(RT_HORIZONTAL);
    m_yHisAxis2.SetRulerType(RT_VERTICAL);

    m_ptCurDraw = NULL;
    m_ptCurTorq = NULL;
    ResetHisLineByCfg(&theApp.m_tParaCfg);
    UpdateData(FALSE);

    m_bModified = FALSE;

    CenterWindow();

    //RegisterHotKey(GetSafeHwnd(), 1, (MOD_CONTROL | MOD_SHIFT), UINT('M'));

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

/* 清空EDIT控件信息 */
void CDlgHisGrp::EmptyEdit()
{
    int  i = 0;

    /* 显示参数 */
    for (i = 0; i < MAXPARANUM; i++)
    {
        //m_strHisShowName[i].Empty();
        m_strHisShowValue[i].Empty();
    }
    m_strMemo = NULLSTR;
    m_strControl = NULLSTR;
    m_strBOTorq = NULLSTR;
    m_strQuality = NULLSTR;
    m_strTime = NULLSTR;
    m_strBOTime = NULLSTR;
    m_strNo = NULLSTR;
    m_strCir = NULLSTR;
    m_strBOCir = NULLSTR;
}

void CDlgHisGrp::UpdateDlgLabel(UINT nUnit)
{
    CString strUnit;
    strUnit.Format("N%sm", BIGPOINT);
    if (nUnit != 0)
        strUnit.Format("lb%sft", BIGPOINT);

    m_strLBG1 = string_format(theApp.LoadstringFromRes(IDS_STRLINELABEL).c_str(), strUnit).c_str();

    m_strLBG10 = strUnit;
    m_strLBG16 = strUnit;
    //m_strLBG20 = strUnit;
}

/* 设置EDIT控件内容 */
void CDlgHisGrp::SetCurEdit()
{
    ASSERT_NULL(m_ptCurTorq);

    m_strNo.Format("%d", m_ptTorData->nCur);
    m_iSingleSTD = m_ptCurTorq->dwcolumns();
    m_bToolBuck = m_ptCurTorq->btoolbuck();
    m_strMemo = m_ptCurTorq->strmemo().c_str();
    /* 显示参数 */
    for (int i = 0; i < m_ptCurTorq->tshow_size() && i < MAXPARANUM; i++)
    {
        m_strHisShowName[i] = theApp.GetTorqShowName(m_ptCurTorq, i);
        m_strHisShowValue[i] = theApp.GetTorqShowValue(m_ptCurTorq, i);
    }
    m_strQuality = theApp.GetQualityInfo(m_ptCurTorq).c_str();

    if (theApp.HaveMakeUP(m_ptCurTorq))
    {
        m_strTime = theApp.GetTorqCollTime(m_ptCurTorq);
        m_strCir.Format("%.3f", theApp.GetCir(m_ptCurTorq, TYPE_MAKEUP));
        m_strControl.Format("%d", (int)m_ptCurTorq->fmumaxtorq());
        ((CEdit*)GetDlgItem(IDC_HISTIME))->SetReadOnly(READONLY);
    }
    else
    {
        m_strTime = NULLSTR;
        m_strCir = NULLSTR;
        m_strControl = NULLSTR;
        ((CEdit*)GetDlgItem(IDC_HISTIME))->SetReadOnly(TRUE);
    }
    if (theApp.HaveBreakout(m_ptCurTorq))
    {
        m_strBOTime = theApp.GetTorqCollTime(m_ptCurTorq, true);
        m_strBOTorq.Format("%d", (int)m_ptCurTorq->fbomaxtorq());
        m_strBOCir.Format("%.3f", theApp.GetCir(m_ptCurTorq, TYPE_BREAKOUT));
        m_strOutWellNO.Format("%d", m_ptCurTorq->dwoutwellno());
        m_strOutJoint = m_ptCurTorq->strbojoint().c_str();
        ((CEdit*)GetDlgItem(IDC_HISBOTIME))->SetReadOnly(READONLY);
        ((CEdit*)GetDlgItem(IDC_HISEDTOUTJOINT))->SetReadOnly(READONLY);
        ((CEdit*)GetDlgItem(IDC_HISEDTOUTWELL))->SetReadOnly(READONLY);
    }
    else
    {
        m_strBOTime = NULLSTR;
        m_strBOTorq = NULLSTR;
        m_strBOCir = NULLSTR;
        m_strOutWellNO = NULLSTR;
        m_strOutJoint = NULLSTR;
        ((CEdit*)GetDlgItem(IDC_HISBOTIME))->SetReadOnly(TRUE);
        ((CEdit*)GetDlgItem(IDC_HISEDTOUTJOINT))->SetReadOnly(TRUE);
        ((CEdit*)GetDlgItem(IDC_HISEDTOUTWELL))->SetReadOnly(TRUE);
    }
}

void CDlgHisGrp::ResetHisLineByCurData()
{
    int    i = 0;
    double fMinCir = 0;
    double fMaxCir = 0;

    ASSERT_NULL(m_ptCurTorq);

    m_wndLineHis.RemoveAt();
    m_wndRpmHis.RemoveAt();
    m_wndLineHis.m_fOptTorq = theApp.GetOptTorq(m_ptCurTorq);   /* 最佳扭矩 */
    //m_wndLineHis.m_fSpeedDown   = m_ptCurTorq->fspeeddown();        /* 减速扭矩 */
    m_wndLineHis.m_fShow = m_ptCurTorq->fshow();             /* 显示扭矩 */
    //m_wndLineHis.m_fBear        = m_ptCurTorq->fbear();             /* 肩负扭矩 */
    m_wndLineHis.m_fControlCir = theApp.GetCtrlCir(m_ptCurTorq);   /* 控制时间 */
    m_wndLineHis.m_fUpperCir = theApp.GetUpperCir(m_ptCurTorq);  /* 上限时间 */
    m_wndLineHis.m_fLowerCir = theApp.GetLowerCir(m_ptCurTorq);  /* 下限时间 */
    m_wndLineHis.m_fMaxCir = theApp.GetMaxCir(m_ptCurTorq);    /* 最大时间 */
    m_wndLineHis.m_fWidthCir = theApp.GetMaxCir(m_ptCurTorq);
    m_wndLineHis.m_fMaxLimit = m_ptCurTorq->fmaxlimit();         /* 最大上限 */

    m_wndLineHis.SetBkColor(RGB(255, 255, 255));
    m_wndLineHis.m_bBKLine = FALSE;
    m_wndLineHis.Add(RGB(0, 0, 0), m_ptCurTorq->fmaxlimit(), 0.0, LINETYPE_HISG);

    fMinCir = 0;
    fMaxCir = m_wndLineHis.m_fMaxCir;
    //GetCirRange(&fMinCir, &fMaxCir);

    /* 重新设置刻度 */
    m_xHisAxis1.SetTickPara(10, fMaxCir, fMinCir);
    m_yHisAxis1.SetTickPara(20, m_ptCurTorq->fmaxlimit());
    m_wndLineHis.DrawBkLine(theApp.HaveBreakout(m_ptCurTorq));

    m_wndRpmHis.SetBkColor(RGB(255, 255, 255));
    m_wndRpmHis.m_bBKLine = FALSE;
    m_wndRpmHis.Add(RGB(0, 0, 0), m_ptCurTorq->fmaxrpm(), 0.0);

    /* 重新设置刻度 */
    m_xHisAxis2.SetTickPara(10, fMaxCir, fMinCir);
    m_yHisAxis2.SetTickPara(3, m_ptCurTorq->fmaxrpm());
    m_wndRpmHis.DrawBkLine();

    UpdateDlgLabel(m_ptCurTorq->dwtorqunit());
}

void CDlgHisGrp::ResetHisLineByCfg(PARACFG* ptCfg)
{
    CONTROLPARA* ptCtrl = NULL;
    //COMMONCFG* ptComm = NULL;

    ASSERT_NULL(ptCfg);

    m_wndLineHis.RemoveAt();
    m_wndRpmHis.RemoveAt();

    ptCtrl = &ptCfg->tCtrl;
    //ptComm = &ptCfg->tComm;

    //m_wndLineHis.m_fUpperLimit = ptCtrl->fTorqConf[INDEX_TORQ_UPPERLIMIT];     /* 最大扭矩 */
    //m_wndLineHis.m_fLowerLimit = ptCtrl->fTorqConf[INDEX_TORQ_LOWERLIMIT];     /* 最小扭矩 */
    m_wndLineHis.m_fOptTorq = ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL];        /* 最佳扭矩 */
    //m_wndLineHis.m_fSpeedDown  = ptCtrl->fTorqConf[INDEX_TORQ_SPEEDDOWN];      /* 减速扭矩 */
    m_wndLineHis.m_fShow = ptCtrl->fTorqConf[INDEX_TORQ_SHOW];           /* 显示扭矩 */
    //m_wndLineHis.m_fBear       = ptCtrl->fTorqConf[INDEX_TORQ_BEAR];           /* 肩负扭矩 */
    m_wndLineHis.m_fControlCir = ptCtrl->fTurnConf[INDEX_TURN_CONTROL];         /* 控制周数 */
    m_wndLineHis.m_fUpperCir = ptCtrl->fTurnConf[INDEX_TURN_UPPERLIMIT];       /* 上限周数 */
    m_wndLineHis.m_fLowerCir = ptCtrl->fTurnConf[INDEX_TURN_LOWERLIMIT];       /* 下限周数 */
    m_wndLineHis.m_fMaxCir = ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT];         /* 最大周数 */
    m_wndLineHis.m_fWidthCir = ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT];
    m_wndLineHis.m_fMaxLimit = ptCtrl->fTorqConf[INDEX_TORQ_MAXLIMIT];       /* 最大上限 */
    //m_wndLineHis.m_fBear       = ptCtrl->fTorqConf[INDEX_TORQ_BEAR];           /* 肩负扭矩 */

    m_wndLineHis.Add(RGB(255, 255, 255), ptCtrl->fTorqConf[INDEX_TORQ_MAXLIMIT], 0.0, LINETYPE_HISG);
    m_xHisAxis1.SetTickPara(10, ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT]);
    m_yHisAxis1.SetTickPara(20, ptCtrl->fTorqConf[INDEX_TORQ_MAXLIMIT]);
    m_wndLineHis.DrawBkLine(false);
    m_wndRpmHis.Add(RGB(255, 255, 255), ptCtrl->fFullRPM, 0.0);
    m_xHisAxis2.SetTickPara(10, ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT]);
    m_yHisAxis2.SetTickPara(3, ptCtrl->fFullRPM);
    m_wndRpmHis.DrawBkLine();

    UpdateDlgLabel(0);
}

/* 查看前一个数据 */
void CDlgHisGrp::OnBtnpri()
{
    MODIFIED_SAVE();

    /* 只有当前数据不是第一个，游标更新，显示新的数据 */
    if (m_ptTorData->nCur > 1)
    {
        m_ptTorData->nCur--;
        ShowCurData(true);
        /* 如果不需要保存，且游标变化，不再保存前一次数据，修改标记清除 */
        m_bModified = FALSE;
    }
}

/* 查看后一个数据 */
void CDlgHisGrp::OnBtnnext()
{
    MODIFIED_SAVE();

    /* 只有当前数据不是最后一个，游标更新，显示新的数据 */
    if (m_ptTorData->nCur < m_ptTorData->nTotal)
    {
        m_ptTorData->nCur++;
        ShowCurData(true);
        /* 如果不需要保存，且游标变化，不再保存前一次数据，修改标记清除 */
        m_bModified = FALSE;
    }
}

/* 检查nCur，并enable/disable按钮 */
BOOL CDlgHisGrp::CheckCurData(UINT* pnCur, UINT nMax)
{
    ASSERT_NULL_R(pnCur, FALSE);

    EnableCtrlforMod();

    /* 按钮先置灰 */
    GetDlgItem(IDC_BTNPRI)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTNNEXT)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTNMOD)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTNMODPARA)->EnableWindow(FALSE);

    /* 没有有效数据，字符串清空 */
    if (*pnCur == 0)
    {
        EmptyEdit();
        UpdateData(FALSE);
        return FALSE;
    }

    /* 游标保护 */
    if (*pnCur > 1)
    {
        GetDlgItem(IDC_BTNPRI)->EnableWindow(TRUE);
    }
    GetDlgItem(IDC_BTNNEXT)->EnableWindow(TRUE);
    if (*pnCur >= nMax)
    {
        *pnCur = nMax;
        GetDlgItem(IDC_BTNNEXT)->EnableWindow(FALSE);
    }
    GetDlgItem(IDC_BTNMOD)->EnableWindow(CANMODIFY);
    GetDlgItem(IDC_BTNMODPARA)->EnableWindow(CANMODIFY);

    return TRUE;
}

void CDlgHisGrp::CheckGrpType()
{
    GetDlgItem(IDC_RADIOGRPMU)->EnableWindow(FALSE);
    GetDlgItem(IDC_RADIOGRPBO)->EnableWindow(FALSE);
    if (!theApp.HaveMakeUP(m_ptCurTorq) || !theApp.HaveBreakout(m_ptCurTorq))
    {
        m_iGrpType = 0;
        return;
    }

    GetDlgItem(IDC_RADIOGRPMU)->EnableWindow(TRUE);
    GetDlgItem(IDC_RADIOGRPBO)->EnableWindow(TRUE);
}
#if 0
void CDlgHisGrp::CheckCurSplit()
{
    /* 按钮先置灰 */
    GetDlgItem(IDC_PRIORSPLIT)->EnableWindow(FALSE);
    GetDlgItem(IDC_NEXTSPLIT)->EnableWindow(FALSE);
    GetDlgItem(IDC_PRIORSPLIT)->ShowWindow(FALSE);
    GetDlgItem(IDC_NEXTSPLIT)->ShowWindow(FALSE);

    if (m_tCurSplit.iSplitNum <= 0)
        return;
    GetDlgItem(IDC_PRIORSPLIT)->ShowWindow(TRUE);
    GetDlgItem(IDC_NEXTSPLIT)->ShowWindow(TRUE);

    /* 游标保护 */
    if (m_tCurSplit.iCur > 1)
    {
        GetDlgItem(IDC_PRIORSPLIT)->EnableWindow(TRUE);
    }
    GetDlgItem(IDC_NEXTSPLIT)->EnableWindow(TRUE);
    if (m_tCurSplit.iCur >= m_tCurSplit.iSplitNum)
    {
        m_tCurSplit.iCur = m_tCurSplit.iSplitNum;
        GetDlgItem(IDC_NEXTSPLIT)->EnableWindow(FALSE);
    }

    return;
}
#endif
void CDlgHisGrp::DrawCurTorque()
{
    int     i = 0;
    int     iBegin = 0, iEnd = 0;
    int     iLineB = 0, iLineE = 0;
    int     interval = 0;
    UINT    nType = TYPE_TOTAL;
    UINT    nBeginPos = 0;

    ASSERT_NULL(m_ptCurDraw);
    ASSERT_NULL(m_ptCurTorq);

    ResetHisLineByCurData();

    ASSERT_ZERO(m_ptCurDraw->wCount);

    /* 如果记录数据大于一圈，显示最后一圈的数据，留空10 */
    i = 0;
    iBegin = 0;
    iEnd = m_ptCurDraw->wCount;
    //theApp.GetShowDataRange(m_ptCurDraw, iBegin, iEnd, &m_tCurSplit);
    /* 多屏的第一屏时，贴右画图 */
    //if (iBegin == 0 && m_tCurSplit.iSplitNum > 1)
    //{
    //    nBeginPos = MAXLINEITEM - iEnd;
    //    // SetStartPoint 需要在m_fOffset初始化后再设置，一般在DrawZoomBkLine之后即可
    //    m_wndLineHis.SetStartPoint(nBeginPos);
    //    m_wndRpmHis.SetStartPoint(nBeginPos);
    //}

    for (i = iBegin; i < iEnd - 1; i++)
    {
        m_wndLineHis.SetPos(m_ptCurDraw->fTorque[i]);
        m_wndLineHis.DrawSpike();
        m_wndRpmHis.SetPos(m_ptCurDraw->fRpm[i]);
        m_wndRpmHis.DrawSpike();
    }

    /*最后一个数据在外面GO，以免出现双线的情况*/
    m_wndLineHis.SetPos(m_ptCurDraw->fTorque[iEnd - 1], TRUE);
    m_wndLineHis.Go();
    m_wndRpmHis.SetPos(m_ptCurDraw->fRpm[iEnd - 1], TRUE);
    m_wndRpmHis.Go();

    m_nBeginPos = iBegin;

    // Draw makeup/breakout line
    if (m_iGrpType > 0)
        nType = m_iGrpType;

    if (m_ptCurTorq->dwmucount() > 0 && (nType & TYPE_MAKEUP))
    {
        if (iBegin < m_ptCurDraw->wMUEndPos)
        {
            iLineB = 0;
            iLineE = MIN(500, m_ptCurDraw->wMUEndPos - iBegin);
            m_wndLineHis.DrawMakeupLine(m_ptCurTorq->fmumaxtorq(), iLineB, iLineE);
            interval = SPLITPOSNUM;
        }
    }
    if (m_ptCurTorq->dwbocount() > 0 && (nType & TYPE_BREAKOUT))
    {
        if (iEnd > m_ptCurDraw->wMUEndPos + interval)
        {
            iLineB = MAX(m_ptCurDraw->wMUEndPos + interval - iBegin, 0);
            iLineE = 500;
            m_wndLineHis.DrawBreakoutLine(m_ptCurTorq->fbomaxtorq(), iLineB, iLineE);
        }
    }

    return;
}

/* 检查nCur，并enable/disable按钮 */
BOOL CDlgHisGrp::CheckCursor(UINT* pnCur, UINT nMax)
{
    ASSERT_NULL_R(pnCur, FALSE);

    /* 按钮先置灰 */
    GetDlgItem(IDC_BTNPRI)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTNNEXT)->EnableWindow(FALSE);

    /* 没有有效数据，字符串清空 */
    if (*pnCur == 0)
    {
        EmptyEdit();
        UpdateData(FALSE);
        return FALSE;
    }

    /* 游标保护 */
    if (*pnCur > 1)
    {
        GetDlgItem(IDC_BTNPRI)->EnableWindow(TRUE);
    }
    GetDlgItem(IDC_BTNNEXT)->EnableWindow(TRUE);
    if (*pnCur >= nMax)
    {
        GetDlgItem(IDC_BTNNEXT)->EnableWindow(FALSE);
        *pnCur = nMax;
    }

    return TRUE;
}

void CDlgHisGrp::ShowCurData(bool bNew)
{
    //char    aucTemp[250];
    UINT    nType = 3;  // 1:MakeUP, 2: BreakOut, 3: All

    /* 检查nCur并设置控件，如果nCur为0，直接返回 */
    COMP_BFALSE(CheckCurData(&m_ptTorData->nCur, m_ptTorData->nTotal));

    /* 检查nCur并设置控件，如果nCur为0，直接返回 */
    if (m_iGrpType > 0)
        nType = m_iGrpType;

    m_ptCurDraw = theApp.GetDrawDataFromTorq(&m_ptTorData->tData[m_ptTorData->nCur - 1], 1, nType);
    ASSERT_NULL(m_ptCurDraw);
    m_ptCurTorq = m_ptCurDraw->ptOrgTorq;
    ASSERT_NULL(m_ptCurTorq);
    // 设置显示图形按钮
    CheckGrpType();

    if (bNew)
    {
        //m_tCurSplit = m_ptTorData->tSplit[m_ptTorData->nCur - 1];
        m_wndLineHis.ClearSelPnt();
    }

    //m_wndLineHis.ClearSelPnt();

    //CheckCurSplit();

    /* 设置显示参数 */
    SetCurEdit();

    /* 画图 */
    DrawCurTorque();

    UpdateData(FALSE);
}

void CDlgHisGrp::OnBtnmod()
{
    CDlgListMod dlgListMod;

    ASSERT_NULL(m_ptCurTorq);

    dlgListMod.m_ptModTorq = m_ptCurTorq;
    dlgListMod.DoModal();
    SaveCurData();
    ShowCurData(true);

    return;
}

__time64_t CDlgHisGrp::GetTimeFromStr(CString strTime)
{
    struct tm tmTime = { 0 };

    if (strTime.IsEmpty())
        return 0;

    sscanf(strTime, "%4d-%2d-%2d %2d:%2d:%2d",
        &tmTime.tm_year,
        &tmTime.tm_mon,
        &tmTime.tm_mday,
        &tmTime.tm_hour,
        &tmTime.tm_min,
        &tmTime.tm_sec);
    tmTime.tm_year -= 1900;
    tmTime.tm_mon--;
    tmTime.tm_isdst = -1;

    return mktime(&tmTime);
}

void CDlgHisGrp::OnBtnmodpara()
{
    int             i = 0;
    TorqData::ShowInfo* ptShow = NULL;

    UpdateData(TRUE);

    ASSERT_NULL(m_ptCurTorq);

    m_ptCurTorq->set_strmemo(m_strMemo);

    if (theApp.HaveBreakout(m_ptCurTorq)) 
    {
        m_ptCurTorq->set_mucoltime(GetTimeFromStr(m_strTime));
    }
    if (theApp.HaveBreakout(m_ptCurTorq))
    {
        m_ptCurTorq->set_bocoltime(GetTimeFromStr(m_strBOTime));
        m_ptCurTorq->set_dwoutwellno(atoi(m_strOutWellNO));
        m_ptCurTorq->set_strbojoint(m_strOutJoint);
    }

    /* 修改数据的环境参数 */
    for (i = 0; i < MAXPARANUM && i < m_ptCurTorq->tshow_size(); i++)
    {
        ptShow = (TorqData::ShowInfo*)&m_ptCurTorq->tshow(i);
        ptShow->set_strvalue(m_strHisShowValue[i].GetBuffer(0));
    }


    SetModified();

    SaveCurData();

    ShowCurData(TRUE);
}

/* 直接对数据的质量标记进行备注 */
void CDlgHisGrp::OnBtnremark()
{
    int         iQuality = 0;
    CDlgRemark  dlgRemark;
    BOOL        bModified = FALSE;
    int         i = 0;
    WORD        wIPPos = 0;
    WORD        wSchPos = 0;
    UINT        nShoulder = 0;
    int         iCause = 0;

    ASSERT_NULL(m_ptCurTorq);

    iQuality = dlgRemark.m_iQuality = m_ptCurTorq->dwquality() & QUA_RESU_QUALITYBIT;
    iCause = dlgRemark.m_iCause = theApp.GetQualityIndex(m_ptCurTorq);

    //nShoulder = dlgRemark.m_nShoulder = theApp.GetIPTorq(m_ptCurTorq, wIPPos, wSchPos);

    COMP_BNE(dlgRemark.DoModal(), IDOK);

    /* 修改了质量属性 */
    if (iQuality != dlgRemark.m_iQuality || iCause != dlgRemark.m_iCause)
    {
        bModified = TRUE;

        if (dlgRemark.m_iQuality == QUA_RESU_GOOD)
        {
            m_ptCurTorq->set_dwquality(QUA_RESU_GOOD);
        }
        else
        {
            m_ptCurTorq->set_dwquality(2 << (dlgRemark.m_iCause - 1));
        }
    }
    m_strQuality = theApp.GetQualityInfo(m_ptCurTorq).c_str();

    if (bModified)
    {
        SetModified();
        //DrawTorque(m_ptCurData);
        ShowCurData(false);

        /* 判断入井序号有变动，没有变化直接返回，否则保存重新读取和计算入井序号 */
        if (iQuality != dlgRemark.m_iQuality)
            UpdateTallyNO();
    }
}

void CDlgHisGrp::UpdateTallyNO()
{
    int         iTallyIndex = -1;

    iTallyIndex = theApp.GetMainIndexfromData(m_ptCurTorq);
    COMP_BE(iTallyIndex, -1);

    SaveCurData();

    ShowCurData(TRUE);
}

void CDlgHisGrp::SetModified()
{
    m_bModified = TRUE;
}

void CDlgHisGrp::SaveCurData()
{
    COMP_BTRUE(!m_bModified);
    ASSERT_NULL(m_ptCurTorq);

    theApp.UpdateHisData(m_ptTorData->strFileName.c_str(), m_ptTorData->nCur, m_ptCurTorq);
    m_bModified = FALSE;

    ::SendMessage(GetParent()->GetSafeHwnd(), WM_RE_READDATA, m_ptTorData->nCur, 0);
}

/* 左键单击 wParam :True 显示/ false: 不显示 */
LRESULT CDlgHisGrp::SelPosChange(WPARAM wParam, LPARAM lParam)
{
    ASSERT_NULL_R(m_ptCurDraw, 0);

    DrawCurTorque();
    if (wParam > 0)
        m_wndLineHis.DrawSelInfo(m_nBeginPos, m_ptCurDraw->wCount);

    UpdateData(FALSE);

    return 0;
}

/* wParam= nPos
lParam 1: 右键up */
//LRESULT CDlgHisGrp::InterPtChange(WPARAM wParam, LPARAM lParam)
//{
//    WORD    wIPPos = 0;
//    double  fIPTorq = 0;
//
//    ASSERT_NULL_R(m_ptCurDraw, 0);
//    ASSERT_NULL_R(m_ptCurTorq, 0);
//
//    wIPPos = wParam + m_nBeginPos;
//    fIPTorq = m_ptCurDraw->fTorque[wIPPos];
//
//    theApp.SetIPInfo(m_ptCurTorq, fIPTorq);
//
//    m_nIPTorq = (UINT)fIPTorq;
//    m_strIPTorq.Format("%d", m_nIPTorq);
//
//    DrawCurTorque();
//
//    ASSERT_ZERO_R(lParam, 0);
//
//    theApp.UpdateHisData(m_ptTorData->strFileName.c_str(), m_ptTorData->nCur, m_ptCurTorq);
//    UpdateData(FALSE);
//
//    return 0;
//}

/*
    多屏的有效性控制：第1屏：500-End ~ 500；
                      第2～n-1屏：0～500；
                      最后1屏：0～(iEnd[n] - iEnd[n-1])
*/
BOOL CDlgHisGrp::JudgeValidPosition(int iPos)
{
    //int  iCur = m_tCurSplit.iCur;

    ASSERT_NULL_R(m_ptCurDraw, FALSE);
    COMP_BLE_R(iPos, 0, FALSE);
    COMP_BGE_R(iPos, MAXLINEITEM, FALSE);
    //COMP_BG_R(m_tCurSplit.iSplitNum, MAXSPLIITNUM, FALSE);
    if (iPos >= m_ptCurDraw->wCount)
        return FALSE;
#if 0
    /* 单屏数据 */
    if (m_tCurSplit.iSplitNum <= 1)
    {
        if (iPos >= m_ptCurDraw->wCount)
            return FALSE;
    }
    else    /* 多屏数据 */
    {
        /* 第1屏 */
        if (iCur == 1)
        {
            if (iPos < MAXLINEITEM - m_tCurSplit.iEnd[0])
                return FALSE;
        }
        /* 最后1屏 */
        if (iCur == m_tCurSplit.iSplitNum)
        {
            if (iPos >= m_tCurSplit.iEnd[iCur - 1] - m_tCurSplit.iEnd[iCur - 2])
                return FALSE;
        }
    }
#endif
    return TRUE;
}

void CDlgHisGrp::OnBnClickedChecktoolbuck()
{
    ASSERT_NULL(m_ptCurTorq);

    UpdateData(TRUE);
    m_ptCurTorq->set_btoolbuck(false);
    if (m_bToolBuck)
        m_ptCurTorq->set_btoolbuck(true);

    m_bModified = TRUE;
}
void CDlgHisGrp::OnBnClickedRadioone()
{
    ASSERT_NULL(m_ptCurTorq);

    UpdateData(TRUE);
    m_ptCurTorq->set_dwcolumns(0);

    m_bModified = TRUE;
}
void CDlgHisGrp::OnBnClickedRadiotwo()
{
    ASSERT_NULL(m_ptCurTorq);

    UpdateData(TRUE);
    m_ptCurTorq->set_dwcolumns(1);

    m_bModified = TRUE;
}
void CDlgHisGrp::OnBnClickedRadiothree()
{
    ASSERT_NULL(m_ptCurTorq);

    UpdateData(TRUE);
    m_ptCurTorq->set_dwcolumns(2);

    m_bModified = TRUE;
}

void CDlgHisGrp::OnBnClickedRadiogrpboth()
{
    UpdateData(TRUE);
    m_wndLineHis.ClearSelPnt();
    ShowCurData(false);
}

void CDlgHisGrp::OnBnClickedRadiogrpmu()
{
    UpdateData(TRUE);
    m_wndLineHis.ClearSelPnt();
    ShowCurData(false);
}

void CDlgHisGrp::OnBnClickedRadiogrpbo()
{
    UpdateData(TRUE);
    m_wndLineHis.ClearSelPnt();
    ShowCurData(false);
}

//void CDlgHisGrp::OnBnClickedCheckhisshackle()
//{
//    ASSERT_NULL(m_ptCurTorq);
//
//    UpdateData(TRUE);
//    m_ptCurTorq->set_bbreakout(0);
//    if (m_bBreakOut)
//        m_ptCurTorq->set_bbreakout(1);
//
//    m_bModified = TRUE;
//}

void CDlgHisGrp::EnableCtrlforMod()
{
    GetDlgItem(IDC_BTNREMARK)->EnableWindow(CANMODIFY);
    GetDlgItem(IDC_CHECKTOOLBUCK)->EnableWindow(CANMODIFY);
    GetDlgItem(IDC_RADIOONE)->EnableWindow(CANMODIFY);
    GetDlgItem(IDC_RADIOTWO)->EnableWindow(CANMODIFY);
    GetDlgItem(IDC_RADIOTHREE)->EnableWindow(CANMODIFY);
    //GetDlgItem(IDC_CHECKHISSHACKLE)->EnableWindow(CANMODIFY);
    ((CEdit*)GetDlgItem(IDC_HISTIME))->SetReadOnly(READONLY);
    ((CEdit*)GetDlgItem(IDC_HISMEMO))->SetReadOnly(READONLY);
    ((CEdit*)GetDlgItem(IDC_HISEDIT01))->SetReadOnly(READONLY);
    ((CEdit*)GetDlgItem(IDC_HISEDIT02))->SetReadOnly(READONLY);
    ((CEdit*)GetDlgItem(IDC_HISEDIT03))->SetReadOnly(READONLY);
    ((CEdit*)GetDlgItem(IDC_HISEDIT04))->SetReadOnly(READONLY);
    ((CEdit*)GetDlgItem(IDC_HISEDIT05))->SetReadOnly(READONLY);
    ((CEdit*)GetDlgItem(IDC_HISEDIT06))->SetReadOnly(READONLY);
    ((CEdit*)GetDlgItem(IDC_HISEDIT07))->SetReadOnly(READONLY);
    ((CEdit*)GetDlgItem(IDC_HISEDIT08))->SetReadOnly(READONLY);
    ((CEdit*)GetDlgItem(IDC_HISEDIT09))->SetReadOnly(READONLY);
    ((CEdit*)GetDlgItem(IDC_HISEDIT10))->SetReadOnly(READONLY);
    ((CEdit*)GetDlgItem(IDC_HISEDIT11))->SetReadOnly(READONLY);
    ((CEdit*)GetDlgItem(IDC_HISEDIT12))->SetReadOnly(READONLY);
    ((CEdit*)GetDlgItem(IDC_HISEDIT13))->SetReadOnly(READONLY);
    ((CEdit*)GetDlgItem(IDC_HISEDIT14))->SetReadOnly(READONLY);
    ((CEdit*)GetDlgItem(IDC_HISEDIT15))->SetReadOnly(READONLY);
}
