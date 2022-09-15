// DlgDataStat.cpp : implementation file
//

#include "stdafx.h"
#include "Torque.h"
#include "DlgDataStat.h"
#include "afxdialogex.h"
#include "DlgStatSet.h"

#include ".\TeeChart\series.h"
#include ".\TeeChart\page.h"
#include ".\TeeChart\Axes.h" 
#include ".\TeeChart\Axis.h"


// CDlgDataStat dialog

extern TORQUEDATA  m_tReadData;

IMPLEMENT_DYNAMIC(CDlgDataStat, CDialogEx)

CDlgDataStat::CDlgDataStat(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DLGDATASTAT, pParent)
    , m_strStatFile(_T(""))
    , m_nQuali(0)
    , m_nUnQuali(0)
    , m_strBadNO(_T(""))
    , m_strToolNO(_T(""))
    , m_fOptTorq(0)
    , m_fMaxTorq(0)
    , m_fMinTorq(0)
    , m_nBuckNum(0)
    , m_bAlignShow(TRUE)
{
    m_nStatType = 0;
}

CDlgDataStat::~CDlgDataStat()
{
}

void CDlgDataStat::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_YSTATAXIS, m_yStatAxis);
    DDX_Control(pDX, IDC_XSTATAXIS, m_xStatAxis);
    DDX_Control(pDX, IDC_COMBOBUCKTYPE, m_cbBuckType);
    DDX_Control(pDX, IDC_COMBOSTATTYPE, m_cbStatType);
    DDX_Text(pDX, IDC_STAT_DATAPATH, m_strStatFile);
    DDX_Text(pDX, IDC_EDSTATQUALIFY, m_nQuali);
    DDX_Text(pDX, IDC_EDSTATUNQUALIFY, m_nUnQuali);
    DDX_Text(pDX, IDC_EDBADQUANO, m_strBadNO);
    DDX_Text(pDX, IDC_EDTOOLBACKNO, m_strToolNO);
    DDX_Text(pDX, IDC_EDBESTTORQ, m_fOptTorq);
    DDX_Text(pDX, IDC_EDMAXTORQ, m_fMaxTorq);
    DDX_Text(pDX, IDC_EDMINTORQ, m_fMinTorq);
    DDX_Text(pDX, IDC_EDBUCKNUM, m_nBuckNum);
    DDX_Control(pDX, IDC_TCHARTCTRL, m_tccCtrl);
    DDX_Control(pDX, IDC_TCHARTSHOULD, m_tccShould);
    DDX_Control(pDX, IDC_TCHARTDELTA, m_tccDelta);
    DDX_Check(pDX, IDC_CHECKALIGN, m_bAlignShow);
    DDX_Text(pDX, IDC_STATIC_STAT3, m_strLBS3);
}


BEGIN_MESSAGE_MAP(CDlgDataStat, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_OPENDATA, &CDlgDataStat::OnBnClickedBtnOpendata)
    ON_BN_CLICKED(IDC_BTNSETSTAT, &CDlgDataStat::OnBnClickedBtnsetstat)
    ON_CBN_SELCHANGE(IDC_COMBOBUCKTYPE, &CDlgDataStat::OnCbnSelchangeCombobucktype)
    ON_CBN_SELCHANGE(IDC_COMBOSTATTYPE, &CDlgDataStat::OnCbnSelchangeCombostattype)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BTNSAVESTAT, &CDlgDataStat::OnBnClickedBtnsavestat)
    ON_BN_CLICKED(IDC_CHECKALIGN, &CDlgDataStat::OnBnClickedCheckalign)
END_MESSAGE_MAP()


// CDlgDataStat message handlers


BOOL CDlgDataStat::DestroyWindow()
{
    ClearFileInfo();

    return CDialogEx::DestroyWindow();
}

void CDlgDataStat::InitChart(CTChart *ptTChart, CString strCaption, CString strLabel[])
{
    int     i = 0;
    ASSERT_NULL(ptTChart);
    
    ptTChart->Series(0).Clear();
    ptTChart->GetPage().SetMaxPointsPerPage(12);

    ptTChart->Series(0).SetColor(RGB(255, 0, 0));
    ptTChart->Series(0).SetTitle(strCaption);

    for(i=0; i<=STATRANGENUM; i++)
        ptTChart->Series(0).AddXY(0, i, strLabel[i], m_clrLabel[i]);
}

void CDlgDataStat::InitStatRange(double fMin, double fCtrl, double fMax)
{
    if(fMin >= fCtrl || fCtrl >= fMax)
        return;
    
    m_tStatCfg.fCtrlRange[0] = fMin;
    m_tStatCfg.fCtrlRange[1] = fCtrl;
    m_tStatCfg.fCtrlRange[2] = fMax;
    
    m_tStatCfg.fShouldRange[0] = fCtrl*0.1;
    m_tStatCfg.fShouldRange[1] = fCtrl*0.3;
    m_tStatCfg.fShouldRange[2] = fCtrl*0.7;
    
    m_tStatCfg.fDeltaRange[0]  = 0.01;
    m_tStatCfg.fDeltaRange[1]  = 0.1;
    m_tStatCfg.fDeltaRange[2]  = 0.2;
}

void CDlgDataStat::InitLabelInfo()
{
    STATCFG     *ptCfg = NULL;
    CString     strName;

    m_clrLabel[0] = RGB(255, 0, 0);
    m_clrLabel[1] = RGB(0, 255, 0);
    m_clrLabel[2] = RGB(0, 255, 0);
    m_clrLabel[3] = RGB(255, 0, 0);
    
    m_strCtrlLabel[0].Format("<%d",  (int)m_tStatCfg.fCtrlRange[0]);
    m_strCtrlLabel[1].Format("%d~%d", (int)m_tStatCfg.fCtrlRange[0],(int)m_tStatCfg.fCtrlRange[1]-1);
    m_strCtrlLabel[2].Format("%d~%d", (int)m_tStatCfg.fCtrlRange[1],(int)m_tStatCfg.fCtrlRange[2]-1);
    m_strCtrlLabel[3].Format(">=%d", (int)m_tStatCfg.fCtrlRange[2]);
    
    m_strShouldLabel[0].Format("<%d",  (int)m_tStatCfg.fShouldRange[0]);
    m_strShouldLabel[1].Format("%d~%d", (int)m_tStatCfg.fShouldRange[0],(int)m_tStatCfg.fShouldRange[1]-1);
    m_strShouldLabel[2].Format("%d~%d", (int)m_tStatCfg.fShouldRange[1],(int)m_tStatCfg.fShouldRange[2]-1);
    m_strShouldLabel[3].Format(">=%d", (int)m_tStatCfg.fShouldRange[2]);
    
    m_strDeltaLabel[0].Format("<%.2f",  m_tStatCfg.fDeltaRange[0]);
    m_strDeltaLabel[1].Format("%.2f~%.2f", m_tStatCfg.fDeltaRange[0],m_tStatCfg.fDeltaRange[1]);
    m_strDeltaLabel[2].Format("%.2f~%.2f", m_tStatCfg.fDeltaRange[1],m_tStatCfg.fDeltaRange[2]);
    m_strDeltaLabel[3].Format(">=%.2f", m_tStatCfg.fDeltaRange[2]);

    strName.Format(IDS_STRSTATCTRLTORQ);
    InitChart(&m_tccCtrl, strName, m_strCtrlLabel);
    strName.Format(IDS_STRSTATSHOULDER);
    InitChart(&m_tccShould, strName, m_strShouldLabel);
    strName.Format(IDS_STRSTATDELTA);
    InitChart(&m_tccDelta, strName, m_strDeltaLabel);
}

BOOL CDlgDataStat::OnInitDialog()
{
    CONTROLPARA *ptCtrl = NULL;
    CString     strType;
    int         i  = 0;

    CDialogEx::OnInitDialog();

    m_wndLineStat.SubclassDlgItem(IDC_LINE_STATTORQUE, this);
    m_wndLineStat.SetParent(this);
    m_xStatAxis.SetRulerType(RT_HORIZONTAL);
    m_yStatAxis.SetRulerType(RT_VERTICAL);
    theApp.AdaptDlgCtrlSize(this, 0);

    ptCtrl = &theApp.m_tParaCfg.tCtrl;
    InitStatRange(ptCtrl->fLowerLimit, ptCtrl->fOptTorq, ptCtrl->fUpperLimit);
    InitLabelInfo();

    ResetStatLine(&theApp.m_tParaCfg);
    ClearFileInfo();

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CDlgDataStat::UpdateDlgLabel()
{
    switch(theApp.m_nLangType)
    {
        case LANGUAGE_CHINESE:
            m_strLBS3.Format("扭矩-周数曲线图        （单位:纵轴=%s, 横轴=周）", theApp.m_strUnit);
            break;
        case LANGUAGE_ENGLISH:
            m_strLBS3.Format("Torque-Turn Graph:    (Vert=%s, Hori=Turn)",  theApp.m_strUnit);
            break;
        /*case LANGUAGE_RUSSIAN:
            m_strLBS3.Format("граф. КМЧО(Ед.:  В. ось=%s, О. ось=об.)",  theApp.m_strUnit);
            break;*/
    }
}

void CDlgDataStat::ShowCurStat(CString strStatFile)
{
    m_strStatFile = strStatFile;
    
    ASSERT_ZERO(m_tReadData.nTotal);

    ClearFileInfo();

    m_nStatType = theApp.m_ptCurShow->wStatType;

    GetStatType();

    GetBasicStatInfo();

    BeginCalStat();
}

void CDlgDataStat::OnBnClickedBtnOpendata()
{
    CString strFilter;
    CString strInfo;

    strFilter.Format(IDS_STRDATFILTER);

    CFileDialog fileDlg(TRUE, "pbd", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, NULL);

    COMP_BNE(fileDlg.DoModal(), IDOK);

    m_strStatFile = fileDlg.GetPathName();

    if (!theApp.ReadHisTorqFromFile(m_strStatFile))
    {
        strInfo.Format(IDS_STRINFFILEERR);
        theApp.SaveShowMessage(strInfo);
        return;
    }

    ShowCurStat(m_strStatFile);
}

void CDlgDataStat::ResetStatLineByData(TorqData::Torque *ptTorq)
{
    ASSERT_NULL(ptTorq);

    m_wndLineStat.RemoveAt();
    m_wndLineStat.m_fUpperLimit = ptTorq->fupperlimit();     /* 最大扭矩 */
    m_wndLineStat.m_fLowerLimit = ptTorq->flowerlimit();     /* 最小扭矩 */
    m_wndLineStat.m_fOptTorq    = theApp.GetOptTorq(ptTorq); /* 最佳扭矩 */
    m_wndLineStat.m_fSpeedDown  = ptTorq->fspeeddown();      /* 减速扭矩 */
    m_wndLineStat.m_fShow       = ptTorq->fshow();           /* 显示扭矩 */
    m_wndLineStat.m_fBear       = ptTorq->fbear();           /* 肩负扭矩 */
    m_wndLineStat.m_fControlCir = theApp.GetCtrlCir(ptTorq); /* 控制周数 */
    m_wndLineStat.m_fUpperCir   = theApp.GetUpperCir(ptTorq);/* 上限周数 */
    m_wndLineStat.m_fLowerCir   = theApp.GetLowerCir(ptTorq);/* 下限周数 */
    m_wndLineStat.m_fMaxCir     = theApp.GetMaxCir(ptTorq);  /* 最大周数 */
    m_wndLineStat.m_fMaxLimit   = ptTorq->fmaxlimit();       /* 最大上限 */
    m_wndLineStat.m_fUpperTai   = ptTorq->fuppertai();       /* 最大台阶 */
    m_wndLineStat.m_fLowerTai   = ptTorq->flowertai();       /* 最小台阶 */

    m_wndLineStat.SetBkColor(RGB(255, 255, 255));
    m_wndLineStat.m_bBKLine = FALSE;
    m_wndLineStat.Add(RGB(0, 0, 0), ptTorq->fmaxlimit(), 0.0);

    /* 重新设置刻度 */
    m_xStatAxis.SetTickPara(10, m_wndLineStat.m_fMaxCir);
    m_yStatAxis.SetTickPara(20, ptTorq->fmaxlimit());
    m_wndLineStat.DrawBkLine();

    UpdateDlgLabel();
}

void CDlgDataStat::ResetStatLine(PARACFG *ptCfg)
{
    CONTROLPARA *ptCtrl = NULL;
    COMMONCFG   *ptComm = NULL;

    ASSERT_NULL(ptCfg);

    ptCtrl = &ptCfg->tCtrl;
    ptComm = &ptCfg->tComm;

    m_wndLineStat.RemoveAt();
    m_wndLineStat.m_fUpperLimit = ptCtrl->fUpperLimit;     /* 最大扭矩 */
    m_wndLineStat.m_fLowerLimit = ptCtrl->fLowerLimit;     /* 最小扭矩 */
    m_wndLineStat.m_fOptTorq    = ptCtrl->fOptTorq;        /* 最佳扭矩 */
    m_wndLineStat.m_fSpeedDown  = ptCtrl->fSpeedDown;      /* 减速扭矩 */
    m_wndLineStat.m_fShow       = ptCtrl->fShow;           /* 显示扭矩 */
    m_wndLineStat.m_fBear       = ptCtrl->fBear;           /* 肩负扭矩 */
    m_wndLineStat.m_fControlCir = ptCtrl->fControlCir;     /* 控制周数 */
    m_wndLineStat.m_fUpperCir   = ptCtrl->fUpperCir;       /* 上限周数 */
    m_wndLineStat.m_fLowerCir   = ptCtrl->fLowerCir;       /* 下限周数 */
    m_wndLineStat.m_fMaxCir     = ptCtrl->fMaxCir;         /* 最大周数 */
    m_wndLineStat.m_fMaxLimit   = ptCtrl->fMaxLimit;       /* 最大上限 */
    m_wndLineStat.m_fUpperTai   = ptComm->fUpperTai;       /* 最大台阶 */
    m_wndLineStat.m_fLowerTai   = ptComm->fLowerTai;       /* 最小台阶 */

    m_wndLineStat.SetBkColor(RGB(255, 255, 255));
    m_wndLineStat.m_bBKLine = FALSE;
    m_wndLineStat.Add(RGB(0, 0, 0), ptCtrl->fMaxLimit, 0.0);

    /* 重新设置刻度 */
    m_xStatAxis.SetTickPara(10, ptCtrl->fMaxCir);
    m_yStatAxis.SetTickPara(20, ptCtrl->fMaxLimit);
    m_wndLineStat.DrawBkLine();

    UpdateDlgLabel();
}

void CDlgDataStat::GetStatType()
{
    int    i  = 0;
    TorqData::Torque  *ptTorq = NULL;

    ASSERT_ZERO(m_tReadData.nTotal);

    m_cbStatType.ResetContent();
    ptTorq = &m_tReadData.tData[m_tReadData.nTotal-1];    
    for(i=0; i<ptTorq->tshow_size(); i++)
    {
        m_cbStatType.AddString(theApp.GetTorqShowName(ptTorq, i));
    }
    
    m_cbStatType.SetCurSel(m_nStatType);
}

void CDlgDataStat::GetBasicStatInfo()
{
    UINT    i  = 0;
    POSITION pos;
    TorqData::Torque  *ptTorq = NULL;
    CString strType;

    ASSERT_ZERO(m_tReadData.nTotal);
    
    strType.Format(IDS_STRALLSTAT);
    m_cbBuckType.AddString(strType);

    for(i=0; i<m_tReadData.nTotal; i++)
    {
        ptTorq = &m_tReadData.tData[i];
        strType = theApp.GetStatType(ptTorq, m_nStatType);

        if(!strType.IsEmpty())
        {
             if ((pos = m_slStatType.Find(strType)) == NULL)
             {
                m_slStatType.AddTail(strType);
                m_cbBuckType.AddString(strType);
             }    
        }
    }

    strType.Format(IDS_STRBUCKLE);
    m_cbBuckType.AddString(strType);
    m_cbBuckType.SetCurSel(0);
}

void CDlgDataStat::ClearFileInfo()
{
    UINT    i = 0;
    m_cbBuckType.ResetContent();
    m_cbStatType.ResetContent();
    m_slStatType.RemoveAll();

    m_nBuckNum = 0;
    m_nQuali   = 0;
    m_nUnQuali = 0;
    m_fOptTorq = (int)theApp.m_tParaCfg.tCtrl.fOptTorq;
    m_fMaxTorq = (int)theApp.m_tParaCfg.tCtrl.fUpperLimit;
    m_fMinTorq = 0;

    m_ptDrawData = NULL;
    
    m_strBadNO.Empty();
    m_strToolNO.Empty();
    
    m_tccCtrl.Series(0).Clear();
    m_tccShould.Series(0).Clear();
    m_tccDelta.Series(0).Clear();

    for(i=0; i<MAXSTATLEVEL; i++)
    {
        m_strCtrlRatio[i].Empty();
        m_strInflRatio[i].Empty();
        m_strDeltRatio[i].Empty();
    }
}

BYTE CDlgDataStat::GetValueRange(double *fRange, double fValue)
{
    ASSERT_NULL_R(fRange, 0);

    if(fValue < fRange[0])
        return 1;

    if(fValue >= fRange[0] && fValue < fRange[1])
        return 2;
    
    if(fValue >= fRange[1] && fValue < fRange[2])
        return 3;

     return 4;
}

/* 是否是选中的类型 */
BOOL CDlgDataStat::IsSelType(TorqData::Torque *ptTorq)
{
    CString strCurType;
    CString strType;
    CString strBuckle;

    ASSERT_NULL_R(ptTorq, FALSE);

    m_cbBuckType.GetWindowText(strCurType);
    strType.Format(IDS_STRALLSTAT);
    strBuckle.Format(IDS_STRBUCKLE);

    /* 所有数据时，不比较类型 */
    if(strCurType.CompareNoCase(strType) == 0)
        return TRUE;
    
    strType = theApp.GetStatType(ptTorq, m_nStatType);

    /* 工具扣 */
    if(strCurType.CompareNoCase(strBuckle) == 0)
    {
        /* 选中工具扣，当前数据不是工具扣 */
        if(!ptTorq->btoolbuck())
        {
            return FALSE;
            
        }
    }
    else if(strCurType.CompareNoCase(strType) != 0)
    {
        return FALSE;
    }
    /* 但是没有选中工具扣，当前数据是工具扣 */
    else if (ptTorq->btoolbuck())
    {
        return FALSE;
    }

    return TRUE;
}
    
void CDlgDataStat::BeginCalStat(BOOL bSetRange)
{
    int     i           = 0;
    BYTE    ucLevel     = 0;
    UINT    nCtrl[MAXSTATLEVEL] = {0};
    UINT    nInfl[MAXSTATLEVEL] = {0};
    UINT    nDelt[MAXSTATLEVEL] = {0};
    WORD    wIPPos      = 0;
    WORD    wSchPos     = 0;
    double  fCtrlTorq   = 0;
    double  fInflTorq   = 0;
    double  fDeltCir    = 0;
    DWORD   dwQuality   = 0;
    int     iShowIndex  = m_tReadData.nTotal-1;
    double  fFullTorq   = 0;
    CString strNo;
    LISTINT listNo;
    TorqData::Torque *ptTorq = NULL;

    m_nBuckNum = 0;
    m_nQuali   = 0;
    m_nUnQuali = 0;
    
    m_strBadNO.Empty();
    m_strToolNO.Empty();
    
    m_fMaxTorq  = 0;
    m_fMinTorq  = 0xFFFFFFFF;
    m_ptDrawData = NULL;

    for(i=m_tReadData.nTotal-1; i>=0; i--)
    {
        ptTorq = &m_tReadData.tData[i];
        if(!IsSelType(ptTorq))
            continue;
        
        if(ptTorq->fmaxlimit() > fFullTorq)
        {
            iShowIndex = i;
            fFullTorq  = ptTorq->fmaxlimit();
        }
    }
    /* 选择满屏扭矩最大的数据,或者没有选中，取最后的值 */
    {
        m_ptDrawData = &m_tReadData.tData[iShowIndex];
        if(!bSetRange)
        {
            InitStatRange(m_ptDrawData->flowerlimit(), 
                          theApp.GetOptTorq(m_ptDrawData), 
                          m_ptDrawData->fupperlimit());
            InitLabelInfo();
        }
        ResetStatLineByData(m_ptDrawData);
        m_fOptTorq = (int)theApp.GetOptTorq(m_ptDrawData);
    }

    for(i=m_tReadData.nTotal-1; i>=0; i--)
    {
        ptTorq = &m_tReadData.tData[i];
        if(!IsSelType(ptTorq))
            continue;

        m_nBuckNum++;
        strNo.Format("%d,", i + 1);

        dwQuality = theApp.GetQuality(ptTorq);
        if (!(dwQuality & QUA_RESU_QUALITYBIT))
        {
            m_nUnQuali ++;
            m_strBadNO = strNo + m_strBadNO;
        }
        else
        {
            m_nQuali ++;
        }

        if(ptTorq->btoolbuck())
        {
            m_strToolNO = strNo + m_strToolNO;
        }

        listNo.push_back(i);

        GET_CTRL_TORQ(fCtrlTorq, ptTorq);
        if(fCtrlTorq > m_fMaxTorq)
            m_fMaxTorq = (int)fCtrlTorq;
        if(fCtrlTorq < m_fMinTorq)
            m_fMinTorq = (int)fCtrlTorq;

        ucLevel = GetValueRange(m_tStatCfg.fCtrlRange, fCtrlTorq);
        if(ucLevel > 0 && ucLevel <= MAXSTATLEVEL)
        {
            nCtrl[ucLevel-1]++;
        }
        
        fInflTorq = theApp.GetIPTorq(ptTorq, wIPPos, wSchPos);
        /* 有拐点扭矩才求拐点百分比和delta */
        if(fInflTorq > 0)
        {
            ucLevel = GetValueRange(m_tStatCfg.fShouldRange, fInflTorq);
            if(ucLevel > 0 && ucLevel <= MAXSTATLEVEL)
            {
                nInfl[ucLevel-1]++;
            }

            fDeltCir  = theApp.GetIPDelCir(ptTorq, wIPPos);
            ucLevel = GetValueRange(m_tStatCfg.fDeltaRange, fDeltCir);
            if(ucLevel > 0 && ucLevel <= MAXSTATLEVEL)
            {
                nDelt[ucLevel-1]++;
            }
        }
    }

    DrawStatFlow(&m_tccCtrl,   m_strCtrlRatio,  m_strCtrlLabel,   &nCtrl[0]);
    DrawStatFlow(&m_tccShould, m_strInflRatio,  m_strShouldLabel, &nInfl[0]);
    DrawStatFlow(&m_tccDelta,  m_strDeltRatio,  m_strDeltaLabel,  &nDelt[0]);

    m_wndLineStat.m_bAlign = m_bAlignShow;

    DrawStatTorq(listNo);

    UpdateData(FALSE);
}

void CDlgDataStat::DrawStatFlow(CTChart *ptChart, CString strRatio[], CString strLabel[], UINT pnNum[])
{
    int     i       = 0;
    UINT    nTotal  = 0;

    ASSERT_NULL(ptChart);
    ASSERT_NULL(strRatio);
    ASSERT_NULL(pnNum);
    
    ptChart->Series(0).Clear();
    ASSERT_ZERO(m_nBuckNum);
    
    for(i=0; i<MAXSTATLEVEL; i++)
    {
        strRatio[i].Empty();
        nTotal += pnNum[i];
    }
    ASSERT_ZERO(nTotal);
    
    for(i=0; i<MAXSTATLEVEL; i++)
    {
        strRatio[i].Format("%.2f%%", 100.0*pnNum[i]/m_nBuckNum);
        ptChart->Series(0).AddXY(100.0*pnNum[i] / m_nBuckNum, i, strLabel[i], m_clrLabel[i]);
    }
    
}

void CDlgDataStat::DrawStatTorq(LISTINT listNo)
{
    if(m_nBuckNum != 0)
    {
        m_wndLineStat.DrawMultiData(&m_tReadData, listNo);
        return;
    }

    m_wndLineStat.DrawMultiData(&m_tReadData, listNo);
    m_fMaxTorq = 0;
    m_fMinTorq = 0;
}
void CDlgDataStat::OnBnClickedBtnsetstat()
{
    CDlgStatSet dlgStatSet;
    TorqData::Torque *ptTorq = m_ptDrawData;

    if(m_tReadData.nTotal != 0)
    {
        if(ptTorq == NULL)
            ptTorq = &m_tReadData.tData[m_tReadData.nTotal-1];
        dlgStatSet.m_fCtrlMax   = ptTorq->fmaxlimit();
        dlgStatSet.m_fShouldMax = theApp.GetOptTorq(ptTorq);
        dlgStatSet.m_fDeltaMax  = ptTorq->fmaxcir();
    }
    else
    {
        dlgStatSet.m_fCtrlMax   = theApp.m_tParaCfg.tCtrl.fMaxLimit;
        dlgStatSet.m_fShouldMax = theApp.m_tParaCfg.tCtrl.fOptTorq;
        dlgStatSet.m_fDeltaMax  = theApp.m_tParaCfg.tCtrl.fMaxCir;
    }
    
    dlgStatSet.m_tempStat     = m_tStatCfg;
    if(IDOK == dlgStatSet.DoModal())
    {
        m_tStatCfg = dlgStatSet.m_tempStat;
        InitLabelInfo();
        BeginCalStat(TRUE);
    }
}

void CDlgDataStat::OnCbnSelchangeCombobucktype()
{
    BeginCalStat();
}

void CDlgDataStat::OnCbnSelchangeCombostattype()
{
    m_nStatType = m_cbStatType.GetCurSel();
    m_cbBuckType.ResetContent();
    m_slStatType.RemoveAll();
    
    m_strBadNO.Empty();
    m_strToolNO.Empty();

    GetBasicStatInfo();
    
    BeginCalStat();
}

void CDlgDataStat::OnDestroy()
{
    theApp.ClearReadTorq();
    CDialogEx::OnDestroy();
}

void CDlgDataStat::OnBnClickedBtnsavestat()
{
    CPaintDC    dc(this);
    HDC         hdc;
    CString     strName;
    HBITMAP     hbm;
    CRect       rcClt;
    CString     strFilter;
    CString     strCurType;
    CString     strFilePath;
    CString     strInfo;

    GetDlgItem(IDC_BTNSAVESTAT)->ShowWindow(FALSE);

    GetClientRect(&rcClt);

    hdc = dc.m_hDC;
    hbm = theApp.CopyDCToBitmap(hdc, &rcClt);

    
    strFilePath = theApp.GetSaveDataPath();
    strFilePath += theApp.m_strFileTitle.c_str();

    m_cbBuckType.GetWindowText(strCurType);
    strName.Format(IDS_STRSTATNAME, strFilePath, strCurType);
    strName.Replace('*', 'X');
    theApp.SavePNG(hbm,strName);

    strInfo.Format(IDS_STRINFSAVEXLSUCC,strName);
    theApp.SaveShowMessage(strInfo);

    GetDlgItem(IDC_BTNSAVESTAT)->ShowWindow(TRUE);
    return;
}


void CDlgDataStat::OnBnClickedCheckalign()
{
    m_bAlignShow = !m_bAlignShow;
    COMP_BE(m_tReadData.nTotal, 0);
    BeginCalStat();
}

CString CDlgDataStat::SaveStatImg(void)
{
    CPaintDC    dc(this);
    HDC         hdc;
    HDC         hMemDC      = NULL; // 屏幕和内存设备描述表
    HBITMAP     hBitmap     = NULL; // 位图句柄
    HBITMAP     hOldBitmap  = NULL; 
    CRect       rcClt;
    CString     strFileName;
    int         iWidth      = 0;
    int         iHeight     = 0;
    
    strFileName  = theApp.GetSaveDataPath();
    strFileName += "DataStat.png";

    GetClientRect(&rcClt); 
    iWidth  = rcClt.Width();
    iHeight = rcClt.Height();
    hdc = dc.m_hDC;

    // 为屏幕设备描述表创建兼容的内存设备描述表
    hMemDC  = CreateCompatibleDC(hdc);
    // 创建一个与屏幕设备描述表兼容的位图
    hBitmap = CreateCompatibleBitmap(hdc, iWidth, iHeight);

    hOldBitmap =(HBITMAP) ::SelectObject(hMemDC,hBitmap);
    RedrawWindow();

    // 把新位图选到内存设备描述表中
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    StretchBlt( hMemDC, 0, 0, iWidth, iHeight,
                hdc,    0, 0, iWidth, iHeight, SRCCOPY );

    // 得到位图的句柄
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
    
    theApp.SavePNG(hBitmap,strFileName);

    return strFileName;
}

