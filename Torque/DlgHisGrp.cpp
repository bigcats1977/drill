// DlgHisGrp.cpp : implementation file
//

#include "stdafx.h"
#include "torque.h"
#include "DlgHisGrp.h"
#include "DlgRemark.h"
#include "DlgZoomIn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHisGrp property page

IMPLEMENT_DYNCREATE(CDlgHisGrp, CPropertyPage)


/* 批量显示或者隐藏按钮 */
#define SHOW_HISGRP_BUTTON(bShow) {                         \
        GetDlgItem(IDC_BTNPRI)->ShowWindow((bShow));        \
        GetDlgItem(IDC_BTNNEXT)->ShowWindow((bShow));       \
        GetDlgItem(IDC_BTNPRNTGRP)->ShowWindow((bShow));    \
        GetDlgItem(IDC_BTNSAVEIMG)->ShowWindow((bShow));    \
    }

CDlgHisGrp::CDlgHisGrp() : CPropertyPage(CDlgHisGrp::IDD)
{
    //{{AFX_DATA_INIT(CDlgHisGrp)
    EmptyEdit();
    m_bToolBuck = FALSE;
    //m_bBreakOut = FALSE;
    //}}AFX_DATA_INIT
    m_nBeginPos = 0;
}

CDlgHisGrp::~CDlgHisGrp()
{
}

void CDlgHisGrp::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
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
    //DDX_Check(pDX, IDC_CHECKHISBREAKOUT, m_bBreakOut);
    //DDX_Text(pDX, IDC_STATIC_G8, m_strTorqType);
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
    DDX_Text(pDX, IDC_HISEDTOUTJOINT, m_strOutJoint);
    DDX_Text(pDX, IDC_HISEDTOUTWELL, m_strOutWellNO);
    DDX_Radio(pDX, IDC_RADIOSIGNLE, m_iSingleSTD);
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
    /*DDX_Text(pDX, IDC_HISEDIT16, m_strHisShowValue[15]);
    DDX_Text(pDX, IDC_HISEDIT17, m_strHisShowValue[16]);
    DDX_Text(pDX, IDC_HISEDIT18, m_strHisShowValue[17]);*/
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHisGrp, CPropertyPage)
    //{{AFX_MSG_MAP(CDlgHisGrp)
    ON_BN_CLICKED(IDC_BTNPRI, OnBtnpri)
    ON_BN_CLICKED(IDC_BTNNEXT, OnBtnnext)
    ON_BN_CLICKED(IDC_BTNPRNTGRP, OnBtnprntgrp)
    ON_BN_CLICKED(IDC_BTNSAVEIMG, OnBtnsaveimg)
    ON_WM_DESTROY()
    ON_MESSAGE(WM_INTERPT_ZOOMIN, InterPtZoomIn)
    ON_MESSAGE(WM_UPDATE_SELPOS, SelPosChange)
    ON_BN_CLICKED(IDC_CHECKTOOLBUCK, &CDlgHisGrp::OnBnClickedChecktoolbuck)
    ON_BN_CLICKED(IDC_BTNREMARK, OnModRemark)
    ON_EN_KILLFOCUS(IDC_HISMEMO, &CDlgHisGrp::OnEnKillfocusHismemo)
    ON_STN_CLICKED(IDC_PRIORSPLIT, &CDlgHisGrp::OnStnClickedPriorsplit)
    ON_STN_CLICKED(IDC_NEXTSPLIT, &CDlgHisGrp::OnStnClickedNextsplit)
    //ON_BN_CLICKED(IDC_CHECKHISBREAKOUT, &CDlgHisGrp::OnBnClickedCheckbreakout)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHisGrp message handlers

BOOL CDlgHisGrp::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    m_wndLineHis.SubclassDlgItem(IDC_LINE_HISTORQUE, this);
    m_wndLineHis.SetParent(this);
    m_xHisAxis1.SetRulerType(RT_HORIZONTAL);
    m_yHisAxis1.SetRulerType(RT_VERTICAL);
    m_wndRpmHis.SubclassDlgItem(IDC_LINE_HISRPM, this);
    m_xHisAxis2.SetRulerType(RT_HORIZONTAL);
    m_yHisAxis2.SetRulerType(RT_VERTICAL);

    theApp.AdaptDlgCtrlSize(this, 2);
    m_ptCurDraw = NULL;
    m_ptCurTorq = NULL;

    ResetHisLineByCfg(&theApp.m_tParaCfg);

    UpdateData(FALSE);

    //RegisterHotKey(GetSafeHwnd(),1,(MOD_CONTROL|MOD_SHIFT),UINT('M'));
    RegisterHotKey(GetSafeHwnd(), 1, (MOD_CONTROL | MOD_SHIFT), UINT('R'));

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

/* 清空EDIT控件信息 */
void CDlgHisGrp::EmptyEdit()
{
    int  i = 0;

    /* 显示参数 */
    for (i = 0; i < 15; i++)
        m_strHisShowValue[i].Empty();
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

void CDlgHisGrp::UpdateDlgLabel()
{
    m_strLBG1 = theApp.LoadstringFromRes(IDS_STRLINELABEL, g_tGlbCfg.strUnit).c_str();
    m_strLBG10 = g_tGlbCfg.strUnit.c_str();
    m_strLBG16 = g_tGlbCfg.strUnit.c_str();
    /*if (m_bBreakOut)
        m_strTorqType = theApp.LoadstringFromRes(IDS_STRBREAKOUT).c_str();
    else
        m_strTorqType = theApp.LoadstringFromRes(IDS_STRMAKEUP).c_str();*/
}

/* 设置EDIT控件内容 */
void CDlgHisGrp::SetCurEdit()
{
    double      fTorq = 0;
    WORD        i = 0;

    ASSERT_NULL(m_ptCurTorq);

    m_strNo.Format("%d", g_tReadData.nCur);
    m_strTime = theApp.GetTorqCollTime(m_ptCurTorq);
    m_bToolBuck = m_ptCurTorq->btoolbuck();
    //m_bBreakOut = m_ptCurTorq->bbreakout();
    m_strMemo = m_ptCurTorq->strmemo().c_str();
    /* 显示参数 */
    for (i = 0; i <= m_ptCurTorq->tshow_size() && i < MAXPARANUM; i++)
    {
        m_strHisShowName[i] = theApp.GetTorqShowName(m_ptCurTorq, i);
        m_strHisShowValue[i] = theApp.GetTorqShowValue(m_ptCurTorq, i);
    }
    m_strQuality = theApp.GetQualityInfo(m_ptCurTorq).c_str();
    m_strCir.Format("%.3f", theApp.GetCir(m_ptCurTorq));

    GET_CTRL_TORQ(fTorq, m_ptCurTorq);
    m_iSingleSTD = (int)m_ptCurTorq->bsinglestd();
    m_strControl.Format("%d", (int)fTorq);
    m_strBOTime = NULLSTR;
    m_strBOTorq = NULLSTR;
    m_strBOCir = NULLSTR;
    m_strOutWellNO = NULLSTR;
    m_strOutJoint = NULLSTR;
    if (m_ptCurTorq->bbreakout())
    {
        m_strBOTime = theApp.GetTorqCollTime(m_ptCurTorq, true);
        m_strBOTorq.Format("%d", (int)m_ptCurTorq->fbomaxtorq());
        m_strBOCir.Format("%.3f", theApp.GetCir(m_ptCurTorq, true));
        m_strOutWellNO.Format("%d", m_ptCurTorq->dwoutwellno());
        m_strOutJoint = m_ptCurTorq->strbojoint().c_str();
    }
}

BOOL CDlgHisGrp::GetCirRange(double* fMin, double* fMax)
{
    int    i = 0;
    double fBeginCir = 0;
    double fMaxCir = m_wndLineHis.m_fMaxCir; /* 满屏的周数 */

    ASSERT_NULL_R(fMin, FALSE);
    ASSERT_NULL_R(fMax, FALSE);

    /* 单屏采用默认值，在外面已经初始化 */
    COMP_BLE_R(m_tCurSplit.iSplitNum, 1, TRUE);

    /* 多屏重新计算圈数范围 */
    /* 计算当前扭矩当前屏的圈数范围 */
    *fMin = m_tCurSplit.iEnd[0] * fMaxCir / MAXLINEITEM - fMaxCir
        + fMaxCir * (m_tCurSplit.iCur - 1);
    *fMax = *fMin + fMaxCir;

    return TRUE;
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
    GetCirRange(&fMinCir, &fMaxCir);

    /* 重新设置刻度 */
    m_xHisAxis1.SetTickPara(10, fMaxCir, fMinCir);
    m_yHisAxis1.SetTickPara(20, m_ptCurTorq->fmaxlimit());
    m_wndLineHis.DrawBkLine();

    m_wndRpmHis.SetBkColor(RGB(255, 255, 255));
    m_wndRpmHis.m_bBKLine = FALSE;
    m_wndRpmHis.Add(RGB(0, 0, 0), m_ptCurTorq->fmaxrpm(), 0.0);

    /* 重新设置刻度 */
    m_xHisAxis2.SetTickPara(10, fMaxCir, fMinCir);
    m_yHisAxis2.SetTickPara(3, m_ptCurTorq->fmaxrpm());
    m_wndRpmHis.DrawBkLine();

    UpdateDlgLabel();
}

void CDlgHisGrp::ResetHisLineByCfg(PARACFG* ptCfg)
{
    CONTROLPARA* ptCtrl = NULL;
    COMMONCFG* ptComm = NULL;

    ASSERT_NULL(ptCfg);

    m_wndLineHis.RemoveAt();
    m_wndRpmHis.RemoveAt();

    ptCtrl = &ptCfg->tCtrl;
    ptComm = &ptCfg->tComm;

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
    m_wndLineHis.DrawBkLine();
    m_wndRpmHis.Add(RGB(255, 255, 255), ptCtrl->fFullRPM, 0.0);
    m_xHisAxis2.SetTickPara(10, ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT]);
    m_yHisAxis2.SetTickPara(3, ptCtrl->fFullRPM);
    m_wndRpmHis.DrawBkLine();

    UpdateDlgLabel();
}

BOOL CDlgHisGrp::OnSetActive()
{
    SendMessageToDescendants(WM_SETFONT, (WPARAM)theApp.m_tRuleHFont.GetSafeHandle(), TRUE);

    ShowCurData(TRUE);

    return CPropertyPage::OnSetActive();
}

/* 查看前一个数据 */
void CDlgHisGrp::OnBtnpri()
{
    if (g_tReadData.nCur > 1)
    {
        g_tReadData.nCur--;
        ShowCurData(TRUE);
        return;
    }

    ShowCurData(FALSE);

    return;
}

/* 查看后一个数据 */
void CDlgHisGrp::OnBtnnext()
{
    if (g_tReadData.nCur < g_tReadData.nTotal)
    {
        g_tReadData.nCur++;
        ShowCurData(TRUE);
        return;
    }

    ShowCurData(FALSE);

    return;
}

/* 检查nCur，并enable/disable按钮 */
BOOL CDlgHisGrp::CheckCurData(UINT* pnCur, UINT nMax)
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
    if (g_tReadData.nCur > 1)
    {
        GetDlgItem(IDC_BTNPRI)->EnableWindow(TRUE);
    }
    GetDlgItem(IDC_BTNNEXT)->EnableWindow(TRUE);
    if (*pnCur >= nMax)
    {
        *pnCur = nMax;
        GetDlgItem(IDC_BTNNEXT)->EnableWindow(FALSE);
    }

    return TRUE;
}

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

void CDlgHisGrp::DrawCurTorque()
{
    int     i = 0;
    double  fTorq = 0;
    int     iBegin = 0;
    int     iEnd = 0;
    UINT    nBeginPos = 0;

    ASSERT_NULL(m_ptCurDraw);
    ASSERT_NULL(m_ptCurTorq);

    ResetHisLineByCurData();

    GET_CTRL_TORQ(fTorq, m_ptCurTorq);
    ASSERT_ZERO(m_ptCurDraw->wCount);

    /* 如果记录数据大于一圈，显示最后一圈的数据，留空10 */
    i = 0;
    theApp.GetShowDataRange(m_ptCurDraw, iBegin, iEnd, &m_tCurSplit);
    /* 多屏的第一屏时，贴右画图 */
    if (iBegin == 0 && m_tCurSplit.iSplitNum > 1)
    {
        nBeginPos = MAXLINEITEM - iEnd;
        // SetStartPoint 需要在m_fOffset初始化后再设置，一般在DrawZoomBkLine之后即可
        m_wndLineHis.SetStartPoint(nBeginPos);
        m_wndRpmHis.SetStartPoint(nBeginPos);
    }

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

void CDlgHisGrp::ShowCurData(BOOL bNew)
{
    char    aucTemp[250];

    /* 检查nCur并设置控件，如果nCur为0，直接返回 */
    COMP_BFALSE(CheckCurData(&g_tReadData.nCur, g_tReadData.nTotal));

    /* 检查nCur并设置控件，如果nCur为0，直接返回 */
    m_ptCurDraw = theApp.GetDrawDataFromTorq(g_tReadData.nCur - 1);
    ASSERT_NULL(m_ptCurDraw);
    m_ptCurTorq = m_ptCurDraw->ptOrgTorq;
    ASSERT_NULL(m_ptCurTorq);

    if (bNew)
    {
        m_tCurSplit = g_tReadData.tSplit[g_tReadData.nCur - 1];
        m_wndLineHis.ClearSelPnt();
    }

    CheckCurSplit();

    /* 设置参数 */
    SetCurEdit();

    /* 画图 */
    DrawCurTorque();

    GetFileTitle(theApp.m_strReadFile.c_str(), aucTemp, 250);

    m_strFileName = aucTemp;

    m_strFileName = m_strFileName.Left(m_strFileName.GetLength() - 4);

    UpdateData(FALSE);
}

void CDlgHisGrp::OnDestroy()
{
    m_strFileName.Empty();
    CPropertyPage::OnDestroy();

    UnregisterHotKey(GetSafeHwnd(), 1);
}

BOOL CDlgHisGrp::PreTranslateMessage(MSG* pMsg)
{
    ASSERT_NULL_R(pMsg, FALSE);

    if (pMsg->message == WM_HOTKEY)
    {
        if (pMsg->wParam == 1 &&
            LOWORD(pMsg->lParam) == (MOD_CONTROL | MOD_SHIFT))
        {
            if (HIWORD(pMsg->lParam) == UINT('R'))
            {
                OnModRemark();
                return TRUE;
            }
        }
    }

    return CPropertyPage::PreTranslateMessage(pMsg);
}

/* 打印当前的数据 */
void CDlgHisGrp::OnBtnprntgrp()
{
    CRect       rcClt;
    CString     strFile;    // "C:\\temp.bmp";
    CString     strTitle;
    CPaintDC    ImgDC(this);
    HBITMAP     hBitmap;

    // Get the application title ?
    strTitle.Format(IDS_STRHISGPRINTTITLE);
    strFile.Format(IDS_STRTMPBMP);
    strFile = theApp.m_strDataPath.c_str() + strFile;

    /* 打印前隐藏按钮 */
    SHOW_HISGRP_BUTTON(FALSE);

    GetClientRect(&rcClt);
    hBitmap = theApp.CopyDCToBitmap(ImgDC.m_hDC, &rcClt);
    theApp.SaveBmp(hBitmap, strFile);

    ShellExecute(0, "print", strFile, NULL, NULL, SW_SHOWNORMAL);

    SHOW_HISGRP_BUTTON(TRUE);
}

void CDlgHisGrp::OnBtnsaveimg()
{
    CPaintDC    dc(this);
    HDC         hdc;
    CString     strNo;
    HBITMAP     hbm;
    CRect       rcClt;
    CString     strFileName;

    SHOW_HISGRP_BUTTON(FALSE);

    GetClientRect(&rcClt);

    hdc = dc.m_hDC;
    hbm = theApp.CopyDCToBitmap(hdc, &rcClt);
    strNo.Format(IDS_STRPNGNAME, m_strFileName, g_tReadData.nCur);

    strFileName = theApp.GetSaveDataPath();
    strFileName += strNo;

    theApp.SavePNG(hbm, strFileName);

    SHOW_HISGRP_BUTTON(TRUE);
    return;
}

void CDlgHisGrp::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
    HBITMAP hBitmap;
    BITMAP  bm;
    HDC     hDesDC;
    HDC     hSrcDC;
    CString strFile;    // "C:\\temp.bmp";

    ASSERT_NULL(pDC);

    strFile.Format(IDS_STRTMPBMP);
    strFile = theApp.m_strDataPath.c_str() + strFile;

    hBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(), strFile, IMAGE_BITMAP, 0, 0,
        LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    hDesDC = pDC->m_hDC;
    hSrcDC = CreateCompatibleDC(hDesDC);

    GetObject(hBitmap, sizeof(BITMAP), &bm);
    SelectObject(hSrcDC, hBitmap);

    ::StretchBlt(hDesDC, 0, 0, bm.bmWidth * 10, bm.bmHeight * 10,
        hSrcDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    return;
}

/* 直接对数据的质量标记进行备注 */
void CDlgHisGrp::OnModRemark()
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
        theApp.UpdateHisData(theApp.m_strReadFile.c_str(), g_tReadData.nCur, m_ptCurTorq);
        ShowCurData(FALSE);

        /* 判断入井序号有变动，没有变化直接返回，否则保存重新读取和计算入井序号 */
        if (iQuality != dlgRemark.m_iQuality)
            UpdateTallyNO();
    }
}

void CDlgHisGrp::UpdateTallyNO()
{
    int         iTallyIndex = -1;
    CPropertySheet* pSheet = NULL;

    iTallyIndex = theApp.GetMainIndexfromData(MAINSHOWTALLY, m_ptCurTorq);
    COMP_BE(iTallyIndex, -1);

    // Reload file and recal Tally NO
    pSheet = (CPropertySheet*)GetOwner();
    pSheet->SetActivePage(0);
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

/*
    多屏的有效性控制：第1屏：500-End ~ 500；
                      第2～n-1屏：0～500；
                      最后1屏：0～(iEnd[n] - iEnd[n-1])
*/
BOOL CDlgHisGrp::JudgeValidPosition(int iPos)
{
    int  iCur = m_tCurSplit.iCur;

    ASSERT_NULL_R(m_ptCurDraw, FALSE);
    COMP_BLE_R(iPos, 0, FALSE);
    COMP_BGE_R(iPos, MAXLINEITEM, FALSE);
    COMP_BG_R(m_tCurSplit.iSplitNum, MAXSPLIITNUM, FALSE);

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

    return TRUE;
}

/* 拐点位置放大5倍 */
LRESULT CDlgHisGrp::InterPtZoomIn(WPARAM wParam, LPARAM lParam)
{
    CDlgZoomIn   dlgZoom;

    COMP_BFALSE_R(JudgeValidPosition(lParam), 1);

    dlgZoom.m_nPos = lParam;
    dlgZoom.m_tSplit = m_tCurSplit;

    dlgZoom.DoModal();

    return 0;
}

/* 打印当前的数据 */
void CDlgHisGrp::PrintOneImage(UINT* pnCur, UINT nIndex, UINT nMax, int iTmpNo)
{
    CPaintDC    dc(this);
    HDC         hSrcDC;
    CString     strNo;
    CString     strTempName;
    CRect       rcClt;
    CString     strFilter;
    CString     strSavePath;
    int         iWidth, iHeight;
    int         iDestX, iDestY;
    int         iTabW = 20, iTabH = 270;
    UINT        i;
    UINT        nLast = 0;

    HDC         hMemDC = NULL; // 屏幕和内存设备描述表
    HBITMAP     hBitmap = NULL; // 位图句柄
    HBITMAP     hOldBitmap = NULL;

    ASSERT_NULL(pnCur);
    nLast = pnCur[0];
    ASSERT_ZERO(nLast);

    GetClientRect(&rcClt);
    iWidth = rcClt.Width();
    iHeight = rcClt.Height() - 18;
    hSrcDC = dc.m_hDC;

    strSavePath = theApp.GetSaveDataPath();

    switch (nMax)
    {
    case 2:
        iDestX = iWidth;
        iDestY = iHeight * 2 + iTabH;
        break;
    case 4:
        iDestX = iWidth * 2 + iTabW;
        iDestY = iHeight * 2 + iTabH;
        break;
    case 6:
        iDestX = iWidth * 2 + iTabW;
        iDestY = iHeight * 3 + iTabH * 2;
        break;
    case 8:
        iDestX = iWidth * 2 + iTabW;
        iDestY = iHeight * 4 + iTabH * 3;
        break;
    case 1:
    default:
        iDestX = iWidth;
        iDestY = iHeight;
        break;
    }

    SHOW_HISGRP_BUTTON(FALSE);

    // 为屏幕设备描述表创建兼容的内存设备描述表
    hMemDC = CreateCompatibleDC(hSrcDC);
    // 创建一个与屏幕设备描述表兼容的位图
    hBitmap = CreateCompatibleBitmap(hSrcDC, iDestX, iDestY);

    hOldBitmap = (HBITMAP) ::SelectObject(hMemDC, hBitmap);
    PatBlt(hMemDC, 0, 0, iDestX, iDestY, RGB(255, 255, 255));
    for (i = nIndex; i < nMax; i++)
    {
        if (pnCur[i] == 0)
            break;
        g_tReadData.nCur = pnCur[i];
        ShowCurData(TRUE);
        RedrawWindow();

        nLast = pnCur[i];

        // 把新位图选到内存设备描述表中
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
        // 把屏幕设备描述表拷贝到内存设备描述表中
        if (nMax == 2)
        {
            iDestX = 0;
            iDestY = i % 2;
        }
        else
        {
            iDestX = i % 2;
            iDestY = i / 2;
        }
        StretchBlt(hMemDC, iDestX * (iWidth + iTabW), iDestY * (iHeight + iTabH), iWidth, iHeight,
            hSrcDC, 0, 0, iWidth, iHeight, SRCCOPY);

        if (pnCur[i] >= g_tReadData.nTotal || pnCur[i + 1] == 0)
            break;
    }

    SHOW_HISGRP_BUTTON(TRUE);

    // 得到位图的句柄
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

    if (iTmpNo > 0)
    {
        strNo.Format(IDS_STRTEMPPNGNAME, iTmpNo);
    }
    else if (nMax == 1)
        strNo.Format(IDS_STRPNGNAME, m_strFileName, pnCur[0]);
    else
        strNo.Format(IDS_STRMULTIPNGNAME, m_strFileName, pnCur[0], nLast);

    strTempName = strSavePath + strNo;
    theApp.SavePNG(hBitmap, strTempName);

    return;
}

void CDlgHisGrp::PrintLineImg(UINT* pnSel, UINT nSelCount)
{
    UINT        i = 0;
    CPaintDC    dc(this);
    HDC         hdc;
    HDC         hMemDC = NULL; // 屏幕和内存设备描述表
    HBITMAP     hBitmap = NULL; // 位图句柄
    CRect       rcClt;
    CString     strNo;
    CString     strTempName;

    ASSERT_NULL(pnSel);
    ASSERT_ZERO(nSelCount);

    SHOW_HISGRP_BUTTON(FALSE);

    GetClientRect(&rcClt);
    /* 20200423 显示图像直接显示全部截图，不需要只显示图像 */
    //rcClt.right = (LONG)(rcClt.right*0.55);


    hdc = dc.m_hDC;
    hMemDC = CreateCompatibleDC(hdc);
    hBitmap = CreateCompatibleBitmap(hdc, rcClt.Width(), rcClt.Height());

    for (i = 0; i < nSelCount; i++)
    {
        g_tReadData.nCur = pnSel[i];
        ShowCurData(TRUE);
        RedrawWindow();

        strNo.Format(IDS_STRPNGNAME, m_strFileName, g_tReadData.nCur);
        strTempName = theApp.GetSaveDataPath() + strNo;

        theApp.CopyDCToPNGFile(hdc, pnSel[i], strTempName, &rcClt, hMemDC, hBitmap);

        Sleep(400);
    }
    DeleteDC(hMemDC);
    DeleteObject(hBitmap);

    SHOW_HISGRP_BUTTON(TRUE);

    return;
}

void CDlgHisGrp::OnBnClickedChecktoolbuck()
{
    ASSERT_NULL(m_ptCurTorq);

    UpdateData(TRUE);
    m_ptCurTorq->set_btoolbuck(0);
    if (m_bToolBuck)
        m_ptCurTorq->set_btoolbuck(1);

    theApp.UpdateHisData(theApp.m_strReadFile.c_str(), g_tReadData.nCur, m_ptCurTorq);
}
#if 0
void CDlgHisGrp::OnBnClickedCheckbreakout()
{
    ASSERT_NULL(m_ptCurTorq);

    UpdateData(TRUE);
    m_ptCurTorq->set_bbreakout(0);
    m_strTorqType = theApp.LoadstringFromRes(IDS_STRMAKEUP).c_str();
    if (m_bBreakOut) {
        m_ptCurTorq->set_bbreakout(1);
        m_strTorqType = theApp.LoadstringFromRes(IDS_STRBREAKOUT).c_str();
    }

    /* 质量重新判定 */
    DWORD dwQuality = theApp.JudgeQuality(m_ptCurTorq, m_bBreakOut);
    m_ptCurTorq->set_dwquality(dwQuality);

    m_strQuality = theApp.GetQualityInfo(m_ptCurTorq).c_str();

    theApp.UpdateHisData(theApp.m_strReadFile.c_str(), g_tReadData.nCur, m_ptCurTorq);

    DrawCurTorque();

    UpdateData(FALSE);

    /* 判断入井序号有变动，没有变化直接返回，否则保存重新读取和计算入井序号 */
    UpdateTallyNO();
}
#endif
void CDlgHisGrp::OnEnKillfocusHismemo()
{
    ASSERT_NULL(m_ptCurTorq);

    UpdateData(TRUE);

    m_ptCurTorq->set_strmemo(m_strMemo);

    UpdateData(FALSE);
    theApp.UpdateHisData(theApp.m_strReadFile.c_str(), g_tReadData.nCur, m_ptCurTorq);
}

void CDlgHisGrp::OnStnClickedPriorsplit()
{
    if (m_tCurSplit.iCur > 1)
    {
        m_tCurSplit.iCur--;

        m_wndLineHis.ClearSelPnt();
    }

    ShowCurData(FALSE);

    return;
}

void CDlgHisGrp::OnStnClickedNextsplit()
{
    if (m_tCurSplit.iCur < m_tCurSplit.iSplitNum)
    {
        m_tCurSplit.iCur++;

        m_wndLineHis.ClearSelPnt();
    }

    ShowCurData(FALSE);

    return;
}

