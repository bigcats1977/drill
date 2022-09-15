// CDlgXlsStatSet.cpp: 实现文件
//

#include "stdafx.h"
#include "Torque.h"
#include "DlgXlsStatSet.h"
#include "afxdialogex.h"


// CDlgXlsStatSet 对话框


extern TORQUEDATA  m_tReadData;
TorqData::Torque   *g_ptLast = NULL;

IMPLEMENT_DYNAMIC(CDlgXlsStatSet, CDialogEx)

CDlgXlsStatSet::CDlgXlsStatSet(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DLGXLSSTATSET, pParent)
{

}

CDlgXlsStatSet::~CDlgXlsStatSet()
{
}

void CDlgXlsStatSet::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDXLSSNAME0, m_strShowName[0]);
    DDX_Text(pDX, IDC_EDXLSSNAME1, m_strShowName[1]);
    DDX_Text(pDX, IDC_EDXLSSNAME2, m_strShowName[2]);
    DDX_Text(pDX, IDC_EDXLSSNAME3, m_strShowName[3]);
    DDX_Text(pDX, IDC_EDXLSSNAME4, m_strShowName[4]);
    DDX_Text(pDX, IDC_EDXLSSNAME5, m_strShowName[5]);
    DDX_Text(pDX, IDC_EDXLSSNAME6, m_strShowName[6]);
    DDX_Text(pDX, IDC_EDXLSSNAME7, m_strShowName[7]);
    DDX_Text(pDX, IDC_EDXLSSNAME8, m_strShowName[8]);
    DDX_Text(pDX, IDC_EDXLSSNAME9, m_strShowName[9]);
    DDX_Text(pDX, IDC_EDXLSSNAME10, m_strShowName[10]);
    DDX_Text(pDX, IDC_EDXLSSNAME11, m_strShowName[11]);
    DDX_Text(pDX, IDC_EDXLSSNAME12, m_strShowName[12]);
    DDX_Text(pDX, IDC_EDXLSSNAME13, m_strShowName[13]);
    DDX_Text(pDX, IDC_EDXLSSNAME14, m_strShowName[14]);
    DDX_Check(pDX, IDC_CHECKSUMM0, m_bSummary[0]);
    DDX_Check(pDX, IDC_CHECKSUMM1, m_bSummary[1]);
    DDX_Check(pDX, IDC_CHECKSUMM2, m_bSummary[2]);
    DDX_Check(pDX, IDC_CHECKSUMM3, m_bSummary[3]);
    DDX_Check(pDX, IDC_CHECKSUMM4, m_bSummary[4]);
    DDX_Check(pDX, IDC_CHECKSUMM5, m_bSummary[5]);
    DDX_Check(pDX, IDC_CHECKSUMM6, m_bSummary[6]);
    DDX_Check(pDX, IDC_CHECKSUMM7, m_bSummary[7]);
    DDX_Check(pDX, IDC_CHECKSUMM8, m_bSummary[8]);
    DDX_Check(pDX, IDC_CHECKSUMM9, m_bSummary[9]);
    DDX_Check(pDX, IDC_CHECKSUMM10, m_bSummary[10]);
    DDX_Check(pDX, IDC_CHECKSUMM11, m_bSummary[11]);
    DDX_Check(pDX, IDC_CHECKSUMM12, m_bSummary[12]);
    DDX_Check(pDX, IDC_CHECKSUMM13, m_bSummary[13]);
    DDX_Check(pDX, IDC_CHECKSUMM14, m_bSummary[14]);
    DDX_Check(pDX, IDC_CHECKRPT0, m_bReport[0]);
    DDX_Check(pDX, IDC_CHECKRPT1, m_bReport[1]);
    DDX_Check(pDX, IDC_CHECKRPT2, m_bReport[2]);
    DDX_Check(pDX, IDC_CHECKRPT3, m_bReport[3]);
    DDX_Check(pDX, IDC_CHECKRPT4, m_bReport[4]);
    DDX_Check(pDX, IDC_CHECKRPT5, m_bReport[5]);
    DDX_Check(pDX, IDC_CHECKRPT6, m_bReport[6]);
    DDX_Check(pDX, IDC_CHECKRPT7, m_bReport[7]);
    DDX_Check(pDX, IDC_CHECKRPT8, m_bReport[8]);
    DDX_Check(pDX, IDC_CHECKRPT9, m_bReport[9]);
    DDX_Check(pDX, IDC_CHECKRPT10, m_bReport[10]);
    DDX_Check(pDX, IDC_CHECKRPT11, m_bReport[11]);
    DDX_Check(pDX, IDC_CHECKRPT12, m_bReport[12]);
    DDX_Check(pDX, IDC_CHECKRPT13, m_bReport[13]);
    DDX_Check(pDX, IDC_CHECKRPT14, m_bReport[14]);
    DDX_Radio(pDX, IDC_RADIOOPER0, m_iOperator);
    DDX_Control(pDX, IDC_RADIOOPER0, m_rdOperator[0]);
    DDX_Control(pDX, IDC_RADIOOPER1, m_rdOperator[1]);
    DDX_Control(pDX, IDC_RADIOOPER2, m_rdOperator[2]);
    DDX_Control(pDX, IDC_RADIOOPER3, m_rdOperator[3]);
    DDX_Control(pDX, IDC_RADIOOPER4, m_rdOperator[4]);
    DDX_Control(pDX, IDC_RADIOOPER5, m_rdOperator[5]);
    DDX_Control(pDX, IDC_RADIOOPER6, m_rdOperator[6]);
    DDX_Control(pDX, IDC_RADIOOPER7, m_rdOperator[7]);
    DDX_Control(pDX, IDC_RADIOOPER8, m_rdOperator[8]);
    DDX_Control(pDX, IDC_RADIOOPER9, m_rdOperator[9]);
    DDX_Control(pDX, IDC_RADIOOPER10, m_rdOperator[10]);
    DDX_Control(pDX, IDC_RADIOOPER11, m_rdOperator[11]);
    DDX_Control(pDX, IDC_RADIOOPER12, m_rdOperator[12]);
    DDX_Control(pDX, IDC_RADIOOPER13, m_rdOperator[13]);
    DDX_Control(pDX, IDC_RADIOOPER14, m_rdOperator[14]);
    DDX_Radio(pDX, IDC_RADIOTALLY0, m_iTally);
    DDX_Control(pDX, IDC_RADIOTALLY0, m_rdTally[0]);
    DDX_Control(pDX, IDC_RADIOTALLY1, m_rdTally[1]);
    DDX_Control(pDX, IDC_RADIOTALLY2, m_rdTally[2]);
    DDX_Control(pDX, IDC_RADIOTALLY3, m_rdTally[3]);
    DDX_Control(pDX, IDC_RADIOTALLY4, m_rdTally[4]);
    DDX_Control(pDX, IDC_RADIOTALLY5, m_rdTally[5]);
    DDX_Control(pDX, IDC_RADIOTALLY6, m_rdTally[6]);
    DDX_Control(pDX, IDC_RADIOTALLY7, m_rdTally[7]);
    DDX_Control(pDX, IDC_RADIOTALLY8, m_rdTally[8]);
    DDX_Control(pDX, IDC_RADIOTALLY9, m_rdTally[9]);
    DDX_Control(pDX, IDC_RADIOTALLY10, m_rdTally[10]);
    DDX_Control(pDX, IDC_RADIOTALLY11, m_rdTally[11]);
    DDX_Control(pDX, IDC_RADIOTALLY12, m_rdTally[12]);
    DDX_Control(pDX, IDC_RADIOTALLY13, m_rdTally[13]);
    DDX_Control(pDX, IDC_RADIOTALLY14, m_rdTally[14]);
    DDX_Control(pDX, IDC_CHECKSUMM0, m_clSummary[0]);
    DDX_Control(pDX, IDC_CHECKSUMM1, m_clSummary[1]);
    DDX_Control(pDX, IDC_CHECKSUMM2, m_clSummary[2]);
    DDX_Control(pDX, IDC_CHECKSUMM3, m_clSummary[3]);
    DDX_Control(pDX, IDC_CHECKSUMM4, m_clSummary[4]);
    DDX_Control(pDX, IDC_CHECKSUMM5, m_clSummary[5]);
    DDX_Control(pDX, IDC_CHECKSUMM6, m_clSummary[6]);
    DDX_Control(pDX, IDC_CHECKSUMM7, m_clSummary[7]);
    DDX_Control(pDX, IDC_CHECKSUMM8, m_clSummary[8]);
    DDX_Control(pDX, IDC_CHECKSUMM9, m_clSummary[9]);
    DDX_Control(pDX, IDC_CHECKSUMM10, m_clSummary[10]);
    DDX_Control(pDX, IDC_CHECKSUMM11, m_clSummary[11]);
    DDX_Control(pDX, IDC_CHECKSUMM12, m_clSummary[12]);
    DDX_Control(pDX, IDC_CHECKSUMM13, m_clSummary[13]);
    DDX_Control(pDX, IDC_CHECKSUMM14, m_clSummary[14]);
    DDX_Control(pDX, IDC_CHECKRPT0, m_clReport[0]);
    DDX_Control(pDX, IDC_CHECKRPT1, m_clReport[1]);
    DDX_Control(pDX, IDC_CHECKRPT2, m_clReport[2]);
    DDX_Control(pDX, IDC_CHECKRPT3, m_clReport[3]);
    DDX_Control(pDX, IDC_CHECKRPT4, m_clReport[4]);
    DDX_Control(pDX, IDC_CHECKRPT5, m_clReport[5]);
    DDX_Control(pDX, IDC_CHECKRPT6, m_clReport[6]);
    DDX_Control(pDX, IDC_CHECKRPT7, m_clReport[7]);
    DDX_Control(pDX, IDC_CHECKRPT8, m_clReport[8]);
    DDX_Control(pDX, IDC_CHECKRPT9, m_clReport[9]);
    DDX_Control(pDX, IDC_CHECKRPT10, m_clReport[10]);
    DDX_Control(pDX, IDC_CHECKRPT11, m_clReport[11]);
    DDX_Control(pDX, IDC_CHECKRPT12, m_clReport[12]);
    DDX_Control(pDX, IDC_CHECKRPT13, m_clReport[13]);
    DDX_Control(pDX, IDC_CHECKRPT14, m_clReport[14]);
}


BEGIN_MESSAGE_MAP(CDlgXlsStatSet, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDlgXlsStatSet::OnBnClickedOk)
    ON_BN_CLICKED(IDC_CHECKRPT0, &CDlgXlsStatSet::OnBnClickedCheckrpt0)
    ON_BN_CLICKED(IDC_CHECKRPT1, &CDlgXlsStatSet::OnBnClickedCheckrpt1)
    ON_BN_CLICKED(IDC_CHECKRPT2, &CDlgXlsStatSet::OnBnClickedCheckrpt2)
    ON_BN_CLICKED(IDC_CHECKRPT3, &CDlgXlsStatSet::OnBnClickedCheckrpt3)
    ON_BN_CLICKED(IDC_CHECKRPT4, &CDlgXlsStatSet::OnBnClickedCheckrpt4)
    ON_BN_CLICKED(IDC_CHECKRPT5, &CDlgXlsStatSet::OnBnClickedCheckrpt5)
    ON_BN_CLICKED(IDC_CHECKRPT6, &CDlgXlsStatSet::OnBnClickedCheckrpt6)
    ON_BN_CLICKED(IDC_CHECKRPT7, &CDlgXlsStatSet::OnBnClickedCheckrpt7)
    ON_BN_CLICKED(IDC_CHECKRPT8, &CDlgXlsStatSet::OnBnClickedCheckrpt8)
    ON_BN_CLICKED(IDC_CHECKRPT9, &CDlgXlsStatSet::OnBnClickedCheckrpt9)
    ON_BN_CLICKED(IDC_CHECKRPT10, &CDlgXlsStatSet::OnBnClickedCheckrpt10)
    ON_BN_CLICKED(IDC_CHECKRPT11, &CDlgXlsStatSet::OnBnClickedCheckrpt11)
    ON_BN_CLICKED(IDC_CHECKRPT12, &CDlgXlsStatSet::OnBnClickedCheckrpt12)
    ON_BN_CLICKED(IDC_CHECKRPT13, &CDlgXlsStatSet::OnBnClickedCheckrpt13)
    ON_BN_CLICKED(IDC_CHECKRPT14, &CDlgXlsStatSet::OnBnClickedCheckrpt14)
    ON_BN_CLICKED(IDC_CHECKSUMM0, &CDlgXlsStatSet::OnBnClickedChecksumm0)
    ON_BN_CLICKED(IDC_CHECKSUMM1, &CDlgXlsStatSet::OnBnClickedChecksumm1)
    ON_BN_CLICKED(IDC_CHECKSUMM2, &CDlgXlsStatSet::OnBnClickedChecksumm2)
    ON_BN_CLICKED(IDC_CHECKSUMM3, &CDlgXlsStatSet::OnBnClickedChecksumm3)
    ON_BN_CLICKED(IDC_CHECKSUMM4, &CDlgXlsStatSet::OnBnClickedChecksumm4)
    ON_BN_CLICKED(IDC_CHECKSUMM5, &CDlgXlsStatSet::OnBnClickedChecksumm5)
    ON_BN_CLICKED(IDC_CHECKSUMM6, &CDlgXlsStatSet::OnBnClickedChecksumm6)
    ON_BN_CLICKED(IDC_CHECKSUMM7, &CDlgXlsStatSet::OnBnClickedChecksumm7)
    ON_BN_CLICKED(IDC_CHECKSUMM8, &CDlgXlsStatSet::OnBnClickedChecksumm8)
    ON_BN_CLICKED(IDC_CHECKSUMM9, &CDlgXlsStatSet::OnBnClickedChecksumm9)
    ON_BN_CLICKED(IDC_CHECKSUMM10, &CDlgXlsStatSet::OnBnClickedChecksumm10)
    ON_BN_CLICKED(IDC_CHECKSUMM11, &CDlgXlsStatSet::OnBnClickedChecksumm11)
    ON_BN_CLICKED(IDC_CHECKSUMM12, &CDlgXlsStatSet::OnBnClickedChecksumm12)
    ON_BN_CLICKED(IDC_CHECKSUMM13, &CDlgXlsStatSet::OnBnClickedChecksumm13)
    ON_BN_CLICKED(IDC_CHECKSUMM14, &CDlgXlsStatSet::OnBnClickedChecksumm14)
    ON_BN_CLICKED(IDC_RADIOOPER0, &CDlgXlsStatSet::OnBnClickedRadiooper0)
    ON_BN_CLICKED(IDC_RADIOOPER1, &CDlgXlsStatSet::OnBnClickedRadiooper1)
    ON_BN_CLICKED(IDC_RADIOOPER2, &CDlgXlsStatSet::OnBnClickedRadiooper2)
    ON_BN_CLICKED(IDC_RADIOOPER3, &CDlgXlsStatSet::OnBnClickedRadiooper3)
    ON_BN_CLICKED(IDC_RADIOOPER4, &CDlgXlsStatSet::OnBnClickedRadiooper4)
    ON_BN_CLICKED(IDC_RADIOOPER5, &CDlgXlsStatSet::OnBnClickedRadiooper5)
    ON_BN_CLICKED(IDC_RADIOOPER6, &CDlgXlsStatSet::OnBnClickedRadiooper6)
    ON_BN_CLICKED(IDC_RADIOOPER7, &CDlgXlsStatSet::OnBnClickedRadiooper7)
    ON_BN_CLICKED(IDC_RADIOOPER8, &CDlgXlsStatSet::OnBnClickedRadiooper8)
    ON_BN_CLICKED(IDC_RADIOOPER9, &CDlgXlsStatSet::OnBnClickedRadiooper9)
    ON_BN_CLICKED(IDC_RADIOOPER10, &CDlgXlsStatSet::OnBnClickedRadiooper10)
    ON_BN_CLICKED(IDC_RADIOOPER11, &CDlgXlsStatSet::OnBnClickedRadiooper11)
    ON_BN_CLICKED(IDC_RADIOOPER12, &CDlgXlsStatSet::OnBnClickedRadiooper12)
    ON_BN_CLICKED(IDC_RADIOOPER13, &CDlgXlsStatSet::OnBnClickedRadiooper13)
    ON_BN_CLICKED(IDC_RADIOOPER14, &CDlgXlsStatSet::OnBnClickedRadiooper14)
    ON_BN_CLICKED(IDC_RADIOTALLY0, &CDlgXlsStatSet::OnBnClickedRadiotally0)
    ON_BN_CLICKED(IDC_RADIOTALLY1, &CDlgXlsStatSet::OnBnClickedRadiotally1)
    ON_BN_CLICKED(IDC_RADIOTALLY2, &CDlgXlsStatSet::OnBnClickedRadiotally2)
    ON_BN_CLICKED(IDC_RADIOTALLY3, &CDlgXlsStatSet::OnBnClickedRadiotally3)
    ON_BN_CLICKED(IDC_RADIOTALLY4, &CDlgXlsStatSet::OnBnClickedRadiotally4)
    ON_BN_CLICKED(IDC_RADIOTALLY5, &CDlgXlsStatSet::OnBnClickedRadiotally5)
    ON_BN_CLICKED(IDC_RADIOTALLY6, &CDlgXlsStatSet::OnBnClickedRadiotally6)
    ON_BN_CLICKED(IDC_RADIOTALLY7, &CDlgXlsStatSet::OnBnClickedRadiotally7)
    ON_BN_CLICKED(IDC_RADIOTALLY8, &CDlgXlsStatSet::OnBnClickedRadiotally8)
    ON_BN_CLICKED(IDC_RADIOTALLY9, &CDlgXlsStatSet::OnBnClickedRadiotally9)
    ON_BN_CLICKED(IDC_RADIOTALLY10, &CDlgXlsStatSet::OnBnClickedRadiotally10)
    ON_BN_CLICKED(IDC_RADIOTALLY11, &CDlgXlsStatSet::OnBnClickedRadiotally11)
    ON_BN_CLICKED(IDC_RADIOTALLY12, &CDlgXlsStatSet::OnBnClickedRadiotally12)
    ON_BN_CLICKED(IDC_RADIOTALLY13, &CDlgXlsStatSet::OnBnClickedRadiotally13)
    ON_BN_CLICKED(IDC_RADIOTALLY14, &CDlgXlsStatSet::OnBnClickedRadiotally14)
END_MESSAGE_MAP()

// CDlgXlsStatSet 消息处理程序
BOOL CDlgXlsStatSet::OnInitDialog()
{
    WORD    i = 0;

    CDialog::OnInitDialog();

    g_ptLast   = &m_tReadData.tData[m_tReadData.nTotal-1];
    m_ptStat   = &theApp.m_tXlsStatCfg;
    m_iShowNum = g_ptLast->tshow_size();

    m_clrNormal  = RGB(0, 0, 0); 
    m_clrChanged = RGB(255, 0, 0);
    memset(&m_bSummary[0], FALSE, sizeof(BOOL)*MAXPARANUM);
    memset(&m_bReport[0],  FALSE, sizeof(BOOL)*MAXPARANUM);

    for(i=0; i< m_iShowNum && i<MAXPARANUM; i++)
    {
        m_strShowName[i] = theApp.GetTorqShowName(g_ptLast, i);
    }
    m_iPriorOperID  = m_iOperator = m_ptStat->ucOperator;
    m_iPriorTallyID = m_iTally    = m_ptStat->ucTally;
    memcpy(&m_bSummary[0],  &m_ptStat->bSummary[0], sizeof(BOOL)*MAXPARANUM);
    memcpy(&m_bReport[0],   &m_ptStat->bReport[0],  sizeof(BOOL)*MAXPARANUM);

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CDlgXlsStatSet::OnBnClickedOk()
{
    int     i       = 0;
    int     iSummary= 0;
    int     iReport = 0;

    UpdateData(TRUE);
    
    memset(&m_ptStat->bSummary[0],  FALSE, sizeof(BOOL)*MAXPARANUM);
    memset(&m_ptStat->bReport[0],   FALSE, sizeof(BOOL)*MAXPARANUM);

    for (i = 0; i < MAXPARANUM; i++)
    {
        if (m_strShowName[i].IsEmpty())
            continue;

        if(m_iOperator == i)
            m_ptStat->ucOperator = (BYTE)i;
        if (m_iTally == i)
            m_ptStat->ucTally = (BYTE)i;
        
        if(m_bSummary[i] && iSummary< MAXSUMMARYPARA)
        {
            iSummary++;
            m_ptStat->bSummary[i] = TRUE;
        }
        if(m_bReport[i] && iReport < MAXREPORTPARA)
        {
            iReport++;
            m_ptStat->bReport[i] = TRUE;
        }
    }

    theApp.WriteXlsStatPara(theApp.m_strParaFile, m_ptStat);

    CDialog::OnOK();
}

void CDlgXlsStatSet::JudgeReportCheck(int iCtrlIdx)
{
    COLORREF    clrCtrl;

    COMP_BL(iCtrlIdx, 0);
    COMP_BG(iCtrlIdx, MAXPARANUM);
    
    clrCtrl = m_clrNormal;
    if (m_ptStat->bReport[iCtrlIdx] != m_bReport[iCtrlIdx])
    {
        clrCtrl = m_clrChanged;
    }

    m_clReport[iCtrlIdx].SetForeColor(clrCtrl);
    m_clReport[iCtrlIdx].Invalidate();
}
void CDlgXlsStatSet::OnBnClickedCheckrpt0()
{
    UpdateData(TRUE);
    JudgeReportCheck(0);
}
void CDlgXlsStatSet::OnBnClickedCheckrpt1()
{
    UpdateData(TRUE);
    JudgeReportCheck(1);
}
void CDlgXlsStatSet::OnBnClickedCheckrpt2()
{
    UpdateData(TRUE);
    JudgeReportCheck(2);
}
void CDlgXlsStatSet::OnBnClickedCheckrpt3()
{
    UpdateData(TRUE);
    JudgeReportCheck(3);
}
void CDlgXlsStatSet::OnBnClickedCheckrpt4()
{
    UpdateData(TRUE);
    JudgeReportCheck(4);
}
void CDlgXlsStatSet::OnBnClickedCheckrpt5()
{
    UpdateData(TRUE);
    JudgeReportCheck(5);
}
void CDlgXlsStatSet::OnBnClickedCheckrpt6()
{
    UpdateData(TRUE);
    JudgeReportCheck(6);
}
void CDlgXlsStatSet::OnBnClickedCheckrpt7()
{
    UpdateData(TRUE);
    JudgeReportCheck(7);
}
void CDlgXlsStatSet::OnBnClickedCheckrpt8()
{
    UpdateData(TRUE);
    JudgeReportCheck(8);
}
void CDlgXlsStatSet::OnBnClickedCheckrpt9()
{
    UpdateData(TRUE);
    JudgeReportCheck(9);
}
void CDlgXlsStatSet::OnBnClickedCheckrpt10()
{
    UpdateData(TRUE);
    JudgeReportCheck(10);
}
void CDlgXlsStatSet::OnBnClickedCheckrpt11()
{
    UpdateData(TRUE);
    JudgeReportCheck(11);
}
void CDlgXlsStatSet::OnBnClickedCheckrpt12()
{
    UpdateData(TRUE);
    JudgeReportCheck(12);
}
void CDlgXlsStatSet::OnBnClickedCheckrpt13()
{
    UpdateData(TRUE);
    JudgeReportCheck(13);
}
void CDlgXlsStatSet::OnBnClickedCheckrpt14()
{
    UpdateData(TRUE);
    JudgeReportCheck(14);
}

void CDlgXlsStatSet::JudgeSummaryCheck(int iCtrlIdx)
{
    COLORREF    clrCtrl;

    COMP_BL(iCtrlIdx, 0);
    COMP_BG(iCtrlIdx, MAXPARANUM);
    
    clrCtrl = m_clrNormal;
    if (m_ptStat->bSummary[iCtrlIdx] != m_bSummary[iCtrlIdx])
    {
        clrCtrl = m_clrChanged;
    }

    m_clSummary[iCtrlIdx].SetForeColor(clrCtrl);
    m_clSummary[iCtrlIdx].Invalidate();
}
void CDlgXlsStatSet::OnBnClickedChecksumm0()
{
    UpdateData(TRUE);
    JudgeSummaryCheck(0);
}
void CDlgXlsStatSet::OnBnClickedChecksumm1()
{
    UpdateData(TRUE);
    JudgeSummaryCheck(1);
}
void CDlgXlsStatSet::OnBnClickedChecksumm2()
{
    UpdateData(TRUE);
    JudgeSummaryCheck(2);
}
void CDlgXlsStatSet::OnBnClickedChecksumm3()
{
    UpdateData(TRUE);
    JudgeSummaryCheck(3);
}
void CDlgXlsStatSet::OnBnClickedChecksumm4()
{
    UpdateData(TRUE);
    JudgeSummaryCheck(4);
}
void CDlgXlsStatSet::OnBnClickedChecksumm5()
{
    UpdateData(TRUE);
    JudgeSummaryCheck(5);
}
void CDlgXlsStatSet::OnBnClickedChecksumm6()
{
    UpdateData(TRUE);
    JudgeSummaryCheck(6);
}
void CDlgXlsStatSet::OnBnClickedChecksumm7()
{
    UpdateData(TRUE);
    JudgeSummaryCheck(7);
}
void CDlgXlsStatSet::OnBnClickedChecksumm8()
{
    UpdateData(TRUE);
    JudgeSummaryCheck(8);
}
void CDlgXlsStatSet::OnBnClickedChecksumm9()
{
    UpdateData(TRUE);
    JudgeSummaryCheck(9);
}
void CDlgXlsStatSet::OnBnClickedChecksumm10()
{
    UpdateData(TRUE);
    JudgeSummaryCheck(10);
}
void CDlgXlsStatSet::OnBnClickedChecksumm11()
{
    UpdateData(TRUE);
    JudgeSummaryCheck(11);
}
void CDlgXlsStatSet::OnBnClickedChecksumm12()
{
    UpdateData(TRUE);
    JudgeSummaryCheck(12);
}
void CDlgXlsStatSet::OnBnClickedChecksumm13()
{
    UpdateData(TRUE);
    JudgeSummaryCheck(13);
}
void CDlgXlsStatSet::OnBnClickedChecksumm14()
{
    UpdateData(TRUE);
    JudgeSummaryCheck(14);
}

void CDlgXlsStatSet::JudgeOperRadio(int iCtrlIdx)
{
    COLORREF    clrCtrl;
    BOOL        bSeled = FALSE;

    COMP_BL(iCtrlIdx, 0);
    COMP_BG(iCtrlIdx, MAXPARANUM);
    
    clrCtrl = m_clrNormal;
    bSeled = (m_iOperator == iCtrlIdx);
    if ((bSeled  && iCtrlIdx != m_ptStat->ucOperator) ||
        (!bSeled && iCtrlIdx == m_ptStat->ucOperator))
    {
        if (iCtrlIdx != m_ptStat->ucOperator)
        {
            clrCtrl = m_clrChanged;
        }
    }

    m_rdOperator[iCtrlIdx].SetForeColor(clrCtrl);
    m_rdOperator[iCtrlIdx].Invalidate();
}
void CDlgXlsStatSet::OnBnClickedRadiooper0()
{
    UpdateData(TRUE);
    JudgeOperRadio(0);
    JudgeOperRadio(m_iPriorOperID);
    m_iPriorOperID = 0;
}
void CDlgXlsStatSet::OnBnClickedRadiooper1()
{
    UpdateData(TRUE);
    JudgeOperRadio(1);
    JudgeOperRadio(m_iPriorOperID);
    m_iPriorOperID = 1;
}
void CDlgXlsStatSet::OnBnClickedRadiooper2()
{
    UpdateData(TRUE);
    JudgeOperRadio(2);
    JudgeOperRadio(m_iPriorOperID);
    m_iPriorOperID = 2;
}
void CDlgXlsStatSet::OnBnClickedRadiooper3()
{
    UpdateData(TRUE);
    JudgeOperRadio(3);
    JudgeOperRadio(m_iPriorOperID);
    m_iPriorOperID = 3;
}
void CDlgXlsStatSet::OnBnClickedRadiooper4()
{
    UpdateData(TRUE);
    JudgeOperRadio(4);
    JudgeOperRadio(m_iPriorOperID);
    m_iPriorOperID = 4;
}
void CDlgXlsStatSet::OnBnClickedRadiooper5()
{
    UpdateData(TRUE);
    JudgeOperRadio(5);
    JudgeOperRadio(m_iPriorOperID);
    m_iPriorOperID = 5;
}
void CDlgXlsStatSet::OnBnClickedRadiooper6()
{
    UpdateData(TRUE);
    JudgeOperRadio(6);
    JudgeOperRadio(m_iPriorOperID);
    m_iPriorOperID = 6;
}
void CDlgXlsStatSet::OnBnClickedRadiooper7()
{
    UpdateData(TRUE);
    JudgeOperRadio(7);
    JudgeOperRadio(m_iPriorOperID);
    m_iPriorOperID = 7;
}
void CDlgXlsStatSet::OnBnClickedRadiooper8()
{
    UpdateData(TRUE);
    JudgeOperRadio(8);
    JudgeOperRadio(m_iPriorOperID);
    m_iPriorOperID = 8;
}
void CDlgXlsStatSet::OnBnClickedRadiooper9()
{
    UpdateData(TRUE);
    JudgeOperRadio(9);
    JudgeOperRadio(m_iPriorOperID);
    m_iPriorOperID = 9;
}
void CDlgXlsStatSet::OnBnClickedRadiooper10()
{
    UpdateData(TRUE);
    JudgeOperRadio(10);
    JudgeOperRadio(m_iPriorOperID);
    m_iPriorOperID = 10;
}
void CDlgXlsStatSet::OnBnClickedRadiooper11()
{
    UpdateData(TRUE);
    JudgeOperRadio(11);
    JudgeOperRadio(m_iPriorOperID);
    m_iPriorOperID = 11;
}
void CDlgXlsStatSet::OnBnClickedRadiooper12()
{
    UpdateData(TRUE);
    JudgeOperRadio(12);
    JudgeOperRadio(m_iPriorOperID);
    m_iPriorOperID = 12;
}
void CDlgXlsStatSet::OnBnClickedRadiooper13()
{
    UpdateData(TRUE);
    JudgeOperRadio(13);
    JudgeOperRadio(m_iPriorOperID);
    m_iPriorOperID = 13;
}
void CDlgXlsStatSet::OnBnClickedRadiooper14()
{
    UpdateData(TRUE);
    JudgeOperRadio(14);
    JudgeOperRadio(m_iPriorOperID);
    m_iPriorOperID = 14;
}

void CDlgXlsStatSet::JudgeTallyRadio(int iCtrlIdx)
{
    COLORREF    clrCtrl;
    BOOL        bSeled = FALSE;

    COMP_BL(iCtrlIdx, 0);
    COMP_BG(iCtrlIdx, MAXPARANUM);
    
    clrCtrl = m_clrNormal;
    bSeled = (m_iTally== iCtrlIdx);
    if ((bSeled  && iCtrlIdx != m_ptStat->ucTally) ||
        (!bSeled && iCtrlIdx == m_ptStat->ucTally))
    {
        if (iCtrlIdx != m_ptStat->ucTally)
        {
            clrCtrl = m_clrChanged;
        }
    }

    m_rdTally[iCtrlIdx].SetForeColor(clrCtrl);
    m_rdTally[iCtrlIdx].Invalidate();
}
void CDlgXlsStatSet::OnBnClickedRadiotally0()
{
    UpdateData(TRUE);
    JudgeTallyRadio(0);
    JudgeTallyRadio(m_iPriorTallyID);
    m_iPriorTallyID = 0;
}
void CDlgXlsStatSet::OnBnClickedRadiotally1()
{
    UpdateData(TRUE);
    JudgeTallyRadio(1);
    JudgeTallyRadio(m_iPriorTallyID);
    m_iPriorTallyID = 1;
}
void CDlgXlsStatSet::OnBnClickedRadiotally2()
{
    UpdateData(TRUE);
    JudgeTallyRadio(2);
    JudgeTallyRadio(m_iPriorTallyID);
    m_iPriorTallyID = 2;
}
void CDlgXlsStatSet::OnBnClickedRadiotally3()
{
    UpdateData(TRUE);
    JudgeTallyRadio(3);
    JudgeTallyRadio(m_iPriorTallyID);
    m_iPriorTallyID = 3;
}
void CDlgXlsStatSet::OnBnClickedRadiotally4()
{
    UpdateData(TRUE);
    JudgeTallyRadio(4);
    JudgeTallyRadio(m_iPriorTallyID);
    m_iPriorTallyID = 4;
}
void CDlgXlsStatSet::OnBnClickedRadiotally5()
{
    UpdateData(TRUE);
    JudgeTallyRadio(5);
    JudgeTallyRadio(m_iPriorTallyID);
    m_iPriorTallyID = 5;
}
void CDlgXlsStatSet::OnBnClickedRadiotally6()
{
    UpdateData(TRUE);
    JudgeTallyRadio(6);
    JudgeTallyRadio(m_iPriorTallyID);
    m_iPriorTallyID = 6;
}
void CDlgXlsStatSet::OnBnClickedRadiotally7()
{
    UpdateData(TRUE);
    JudgeTallyRadio(7);
    JudgeTallyRadio(m_iPriorTallyID);
    m_iPriorTallyID = 7;
}
void CDlgXlsStatSet::OnBnClickedRadiotally8()
{
    UpdateData(TRUE);
    JudgeTallyRadio(8);
    JudgeTallyRadio(m_iPriorTallyID);
    m_iPriorTallyID = 8;
}
void CDlgXlsStatSet::OnBnClickedRadiotally9()
{
    UpdateData(TRUE);
    JudgeTallyRadio(9);
    JudgeTallyRadio(m_iPriorTallyID);
    m_iPriorTallyID = 9;
}
void CDlgXlsStatSet::OnBnClickedRadiotally10()
{
    UpdateData(TRUE);
    JudgeTallyRadio(10);
    JudgeTallyRadio(m_iPriorTallyID);
    m_iPriorTallyID = 10;
}
void CDlgXlsStatSet::OnBnClickedRadiotally11()
{
    UpdateData(TRUE);
    JudgeTallyRadio(11);
    JudgeTallyRadio(m_iPriorTallyID);
    m_iPriorTallyID = 11;
}
void CDlgXlsStatSet::OnBnClickedRadiotally12()
{
    UpdateData(TRUE);
    JudgeTallyRadio(12);
    JudgeTallyRadio(m_iPriorTallyID);
    m_iPriorTallyID = 12;
}void CDlgXlsStatSet::OnBnClickedRadiotally13()
{
    UpdateData(TRUE);
    JudgeTallyRadio(13);
    JudgeTallyRadio(m_iPriorTallyID);
    m_iPriorTallyID = 13;
}
void CDlgXlsStatSet::OnBnClickedRadiotally14()
{
    UpdateData(TRUE);
    JudgeTallyRadio(14);
    JudgeTallyRadio(m_iPriorTallyID);
    m_iPriorTallyID = 14;
}
