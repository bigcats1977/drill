// DlgShowSet.cpp : implementation file
//

#include "stdafx.h"
#include "DlgShowSet.h"
#include "afxdialogex.h"


// CDlgShowSet dialog

IMPLEMENT_DYNAMIC(CDlgShowSet, CDialog)

CDlgShowSet::CDlgShowSet(CWnd* pParent /*=NULL*/)
    : CDialog(IDD_DLGSHOWPARA, pParent)
{
}

CDlgShowSet::~CDlgShowSet()
{
}

void CDlgShowSet::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDSHOWNAME0, m_strShowName[0]);
    DDX_Text(pDX, IDC_EDSHOWNAME1, m_strShowName[1]);
    DDX_Text(pDX, IDC_EDSHOWNAME2, m_strShowName[2]);
    DDX_Text(pDX, IDC_EDSHOWNAME3, m_strShowName[3]);
    DDX_Text(pDX, IDC_EDSHOWNAME4, m_strShowName[4]);
    DDX_Text(pDX, IDC_EDSHOWNAME5, m_strShowName[5]);
    DDX_Text(pDX, IDC_EDSHOWNAME6, m_strShowName[6]);
    DDX_Text(pDX, IDC_EDSHOWNAME7, m_strShowName[7]);
    DDX_Text(pDX, IDC_EDSHOWNAME8, m_strShowName[8]);
    DDX_Text(pDX, IDC_EDSHOWNAME9, m_strShowName[9]);
    DDX_Text(pDX, IDC_EDSHOWNAME10, m_strShowName[10]);
    DDX_Text(pDX, IDC_EDSHOWNAME11, m_strShowName[11]);
    DDX_Text(pDX, IDC_EDSHOWNAME12, m_strShowName[12]);
    DDX_Text(pDX, IDC_EDSHOWNAME13, m_strShowName[13]);
    DDX_Text(pDX, IDC_EDSHOWNAME14, m_strShowName[14]);
    DDX_Check(pDX, IDC_CHECKLIST0, m_bList[0]);
    DDX_Check(pDX, IDC_CHECKLIST1, m_bList[1]);
    DDX_Check(pDX, IDC_CHECKLIST2, m_bList[2]);
    DDX_Check(pDX, IDC_CHECKLIST3, m_bList[3]);
    DDX_Check(pDX, IDC_CHECKLIST4, m_bList[4]);
    DDX_Check(pDX, IDC_CHECKLIST5, m_bList[5]);
    DDX_Check(pDX, IDC_CHECKLIST6, m_bList[6]);
    DDX_Check(pDX, IDC_CHECKLIST7, m_bList[7]);
    DDX_Check(pDX, IDC_CHECKLIST8, m_bList[8]);
    DDX_Check(pDX, IDC_CHECKLIST9, m_bList[9]);
    DDX_Check(pDX, IDC_CHECKLIST10, m_bList[10]);
    DDX_Check(pDX, IDC_CHECKLIST11, m_bList[11]);
    DDX_Check(pDX, IDC_CHECKLIST12, m_bList[12]);
    DDX_Check(pDX, IDC_CHECKLIST13, m_bList[13]);
    DDX_Check(pDX, IDC_CHECKLIST14, m_bList[14]);
    DDX_Check(pDX, IDC_CHECKMAIN0, m_bMain[0]);
    DDX_Check(pDX, IDC_CHECKMAIN1, m_bMain[1]);
    DDX_Check(pDX, IDC_CHECKMAIN2, m_bMain[2]);
    DDX_Check(pDX, IDC_CHECKMAIN3, m_bMain[3]);
    DDX_Check(pDX, IDC_CHECKMAIN4, m_bMain[4]);
    DDX_Check(pDX, IDC_CHECKMAIN5, m_bMain[5]);
    DDX_Check(pDX, IDC_CHECKMAIN6, m_bMain[6]);
    DDX_Check(pDX, IDC_CHECKMAIN7, m_bMain[7]);
    DDX_Check(pDX, IDC_CHECKMAIN8, m_bMain[8]);
    DDX_Check(pDX, IDC_CHECKMAIN9, m_bMain[9]);
    DDX_Check(pDX, IDC_CHECKMAIN10, m_bMain[10]);
    DDX_Check(pDX, IDC_CHECKMAIN11, m_bMain[11]);
    DDX_Check(pDX, IDC_CHECKMAIN12, m_bMain[12]);
    DDX_Check(pDX, IDC_CHECKMAIN13, m_bMain[13]);
    DDX_Check(pDX, IDC_CHECKMAIN14, m_bMain[14]);
    DDX_Radio(pDX, IDC_RADIOSHOWFILE0, m_iFileName);
    DDX_Radio(pDX, IDC_RADIOSHOWSTAT0, m_iShowStat);
    DDX_Control(pDX, IDC_RADIOSHOWFILE0, m_rdShowName[0]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE1, m_rdShowName[1]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE2, m_rdShowName[2]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE3, m_rdShowName[3]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE4, m_rdShowName[4]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE5, m_rdShowName[5]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE6, m_rdShowName[6]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE7, m_rdShowName[7]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE8, m_rdShowName[8]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE9, m_rdShowName[9]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE10, m_rdShowName[10]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE11, m_rdShowName[11]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE12, m_rdShowName[12]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE13, m_rdShowName[13]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE14, m_rdShowName[14]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT0, m_rdStatName[0]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT1, m_rdStatName[1]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT2, m_rdStatName[2]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT3, m_rdStatName[3]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT4, m_rdStatName[4]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT5, m_rdStatName[5]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT6, m_rdStatName[6]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT7, m_rdStatName[7]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT8, m_rdStatName[8]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT9, m_rdStatName[9]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT10, m_rdStatName[10]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT11, m_rdStatName[11]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT12, m_rdStatName[12]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT13, m_rdStatName[13]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT14, m_rdStatName[14]);
    DDX_Control(pDX, IDC_CHECKLIST0, m_clListName[0]);
    DDX_Control(pDX, IDC_CHECKLIST1, m_clListName[1]);
    DDX_Control(pDX, IDC_CHECKLIST2, m_clListName[2]);
    DDX_Control(pDX, IDC_CHECKLIST3, m_clListName[3]);
    DDX_Control(pDX, IDC_CHECKLIST4, m_clListName[4]);
    DDX_Control(pDX, IDC_CHECKLIST5, m_clListName[5]);
    DDX_Control(pDX, IDC_CHECKLIST6, m_clListName[6]);
    DDX_Control(pDX, IDC_CHECKLIST7, m_clListName[7]);
    DDX_Control(pDX, IDC_CHECKLIST8, m_clListName[8]);
    DDX_Control(pDX, IDC_CHECKLIST9, m_clListName[9]);
    DDX_Control(pDX, IDC_CHECKLIST10, m_clListName[10]);
    DDX_Control(pDX, IDC_CHECKLIST11, m_clListName[11]);
    DDX_Control(pDX, IDC_CHECKLIST12, m_clListName[12]);
    DDX_Control(pDX, IDC_CHECKLIST13, m_clListName[13]);
    DDX_Control(pDX, IDC_CHECKLIST14, m_clListName[14]);
    DDX_Control(pDX, IDC_CHECKMAIN0, m_clMainName[0]);
    DDX_Control(pDX, IDC_CHECKMAIN1, m_clMainName[1]);
    DDX_Control(pDX, IDC_CHECKMAIN2, m_clMainName[2]);
    DDX_Control(pDX, IDC_CHECKMAIN3, m_clMainName[3]);
    DDX_Control(pDX, IDC_CHECKMAIN4, m_clMainName[4]);
    DDX_Control(pDX, IDC_CHECKMAIN5, m_clMainName[5]);
    DDX_Control(pDX, IDC_CHECKMAIN6, m_clMainName[6]);
    DDX_Control(pDX, IDC_CHECKMAIN7, m_clMainName[7]);
    DDX_Control(pDX, IDC_CHECKMAIN8, m_clMainName[8]);
    DDX_Control(pDX, IDC_CHECKMAIN9, m_clMainName[9]);
    DDX_Control(pDX, IDC_CHECKMAIN10, m_clMainName[10]);
    DDX_Control(pDX, IDC_CHECKMAIN11, m_clMainName[11]);
    DDX_Control(pDX, IDC_CHECKMAIN12, m_clMainName[12]);
    DDX_Control(pDX, IDC_CHECKMAIN13, m_clMainName[13]);
    DDX_Control(pDX, IDC_CHECKMAIN14, m_clMainName[14]);
    
}

BEGIN_MESSAGE_MAP(CDlgShowSet, CDialog)
    ON_BN_CLICKED(IDOK, &CDlgShowSet::OnBnClickedOk)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_RADIOSHOWFILE0, &CDlgShowSet::OnBnClickedRadioshowfile0)
    ON_BN_CLICKED(IDC_RADIOSHOWFILE1, &CDlgShowSet::OnBnClickedRadioshowfile1)
    ON_BN_CLICKED(IDC_RADIOSHOWFILE2, &CDlgShowSet::OnBnClickedRadioshowfile2)
    ON_BN_CLICKED(IDC_RADIOSHOWFILE3, &CDlgShowSet::OnBnClickedRadioshowfile3)
    ON_BN_CLICKED(IDC_RADIOSHOWFILE4, &CDlgShowSet::OnBnClickedRadioshowfile4)
    ON_BN_CLICKED(IDC_RADIOSHOWFILE5, &CDlgShowSet::OnBnClickedRadioshowfile5)
    ON_BN_CLICKED(IDC_RADIOSHOWFILE6, &CDlgShowSet::OnBnClickedRadioshowfile6)
    ON_BN_CLICKED(IDC_RADIOSHOWFILE7, &CDlgShowSet::OnBnClickedRadioshowfile7)
    ON_BN_CLICKED(IDC_RADIOSHOWFILE8, &CDlgShowSet::OnBnClickedRadioshowfile8)
    ON_BN_CLICKED(IDC_RADIOSHOWFILE9, &CDlgShowSet::OnBnClickedRadioshowfile9)
    ON_BN_CLICKED(IDC_RADIOSHOWFILE10, &CDlgShowSet::OnBnClickedRadioshowfile10)
    ON_BN_CLICKED(IDC_RADIOSHOWFILE11, &CDlgShowSet::OnBnClickedRadioshowfile11)
    ON_BN_CLICKED(IDC_RADIOSHOWFILE12, &CDlgShowSet::OnBnClickedRadioshowfile12)
    ON_BN_CLICKED(IDC_RADIOSHOWFILE13, &CDlgShowSet::OnBnClickedRadioshowfile13)
    ON_BN_CLICKED(IDC_RADIOSHOWFILE14, &CDlgShowSet::OnBnClickedRadioshowfile14)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT0, &CDlgShowSet::OnBnClickedRadioshowstat0)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT1, &CDlgShowSet::OnBnClickedRadioshowstat1)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT2, &CDlgShowSet::OnBnClickedRadioshowstat2)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT3, &CDlgShowSet::OnBnClickedRadioshowstat3)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT4, &CDlgShowSet::OnBnClickedRadioshowstat4)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT5, &CDlgShowSet::OnBnClickedRadioshowstat5)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT6, &CDlgShowSet::OnBnClickedRadioshowstat6)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT7, &CDlgShowSet::OnBnClickedRadioshowstat7)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT8, &CDlgShowSet::OnBnClickedRadioshowstat8)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT9, &CDlgShowSet::OnBnClickedRadioshowstat9)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT10, &CDlgShowSet::OnBnClickedRadioshowstat10)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT11, &CDlgShowSet::OnBnClickedRadioshowstat11)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT12, &CDlgShowSet::OnBnClickedRadioshowstat12)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT13, &CDlgShowSet::OnBnClickedRadioshowstat13)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT14, &CDlgShowSet::OnBnClickedRadioshowstat14)
    ON_BN_CLICKED(IDC_CHECKLIST0, &CDlgShowSet::OnBnClickedChecklist0)
    ON_BN_CLICKED(IDC_CHECKLIST1, &CDlgShowSet::OnBnClickedChecklist1)
    ON_BN_CLICKED(IDC_CHECKLIST2, &CDlgShowSet::OnBnClickedChecklist2)
    ON_BN_CLICKED(IDC_CHECKLIST3, &CDlgShowSet::OnBnClickedChecklist3)
    ON_BN_CLICKED(IDC_CHECKLIST4, &CDlgShowSet::OnBnClickedChecklist4)
    ON_BN_CLICKED(IDC_CHECKLIST5, &CDlgShowSet::OnBnClickedChecklist5)
    ON_BN_CLICKED(IDC_CHECKLIST6, &CDlgShowSet::OnBnClickedChecklist6)
    ON_BN_CLICKED(IDC_CHECKLIST7, &CDlgShowSet::OnBnClickedChecklist7)
    ON_BN_CLICKED(IDC_CHECKLIST8, &CDlgShowSet::OnBnClickedChecklist8)
    ON_BN_CLICKED(IDC_CHECKLIST9, &CDlgShowSet::OnBnClickedChecklist9)
    ON_BN_CLICKED(IDC_CHECKLIST10, &CDlgShowSet::OnBnClickedChecklist10)
    ON_BN_CLICKED(IDC_CHECKLIST11, &CDlgShowSet::OnBnClickedChecklist11)
    ON_BN_CLICKED(IDC_CHECKLIST12, &CDlgShowSet::OnBnClickedChecklist12)
    ON_BN_CLICKED(IDC_CHECKLIST13, &CDlgShowSet::OnBnClickedChecklist13)
    ON_BN_CLICKED(IDC_CHECKLIST14, &CDlgShowSet::OnBnClickedChecklist14)
    ON_BN_CLICKED(IDC_CHECKMAIN0, &CDlgShowSet::OnBnClickedCheckmain0)
    ON_BN_CLICKED(IDC_CHECKMAIN1, &CDlgShowSet::OnBnClickedCheckmain1)
    ON_BN_CLICKED(IDC_CHECKMAIN2, &CDlgShowSet::OnBnClickedCheckmain2)
    ON_BN_CLICKED(IDC_CHECKMAIN3, &CDlgShowSet::OnBnClickedCheckmain3)
    ON_BN_CLICKED(IDC_CHECKMAIN4, &CDlgShowSet::OnBnClickedCheckmain4)
    ON_BN_CLICKED(IDC_CHECKMAIN5, &CDlgShowSet::OnBnClickedCheckmain5)
    ON_BN_CLICKED(IDC_CHECKMAIN6, &CDlgShowSet::OnBnClickedCheckmain6)
    ON_BN_CLICKED(IDC_CHECKMAIN7, &CDlgShowSet::OnBnClickedCheckmain7)
    ON_BN_CLICKED(IDC_CHECKMAIN8, &CDlgShowSet::OnBnClickedCheckmain8)
    ON_BN_CLICKED(IDC_CHECKMAIN9, &CDlgShowSet::OnBnClickedCheckmain9)
    ON_BN_CLICKED(IDC_CHECKMAIN10, &CDlgShowSet::OnBnClickedCheckmain10)
    ON_BN_CLICKED(IDC_CHECKMAIN11, &CDlgShowSet::OnBnClickedCheckmain11)
    ON_BN_CLICKED(IDC_CHECKMAIN12, &CDlgShowSet::OnBnClickedCheckmain12)
    ON_BN_CLICKED(IDC_CHECKMAIN13, &CDlgShowSet::OnBnClickedCheckmain13)
    ON_BN_CLICKED(IDC_CHECKMAIN14, &CDlgShowSet::OnBnClickedCheckmain14)
END_MESSAGE_MAP()

// CDlgShowSet message handlers


BOOL CDlgShowSet::OnInitDialog()
{
    WORD    i = 0;
    WORD    j = 0;
    CString strList;
    CString strMain;

    CDialog::OnInitDialog();

    m_clrNormal  = RGB(0, 0, 0); 
    m_clrChanged = RGB(255, 0, 0);
    memset(&m_bHisList[0], FALSE, sizeof(BOOL)*MAXPARANUM);
    memset(&m_bHisMain[0], FALSE, sizeof(BOOL)*MAXPARANUM);

    for(i=0; i<m_tempShow.wParaNum && i<MAXPARANUM; i++)
    {
        m_strShowName[i] = m_tempShow.tShow[i].strName.c_str();
    }
    m_iPriorShowCtrlID = m_iFileName = m_tempShow.wFileName;
    m_iPriorStatCtrlID = m_iShowStat = m_tempShow.wStatType;
    for(i=0; i<m_tempShow.wListNum; i++)
    {
        strList = m_tempShow.strList[i].c_str();
        for(j=0; j<m_tempShow.wParaNum && j<MAXPARANUM; j++)
        {
            if(strList.CompareNoCase(m_tempShow.tShow[j].strName.c_str()) == 0)
            {
                m_bHisList[j] = m_bList[j] = TRUE;
                break;
            }
        }
    }
    
    for(i=0; i<m_tempShow.wMainNum; i++)
    {
        strMain = m_tempShow.strMain[i].c_str();
        for(j=0; j<m_tempShow.wParaNum && j<MAXPARANUM; j++)
        {
            if(strMain.CompareNoCase(m_tempShow.tShow[j].strName.c_str()) == 0)
            {
                m_bHisMain[j] = m_bMain[j] = TRUE;
                break;
            }
        }
    }

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CDlgShowSet::OnBnClickedOk()
{
    WORD    i = 0;
    int     iShow = 0;
    int     iList = 0;
    int     iMain = 0;

    UpdateData(TRUE);

    for (i = 0; i < MAXPARANUM; i++)
    {
        if (m_strShowName[i].IsEmpty())
            continue;

        m_tempShow.tShow[iShow].strName = m_strShowName[i];
        if(m_iFileName == i)
            m_tempShow.wFileName = iShow;
        if (m_iShowStat == i)
            m_tempShow.wStatType = iShow;
        iShow++;

        if (m_bList[i])
        {
            m_tempShow.strList[iList++] = m_strShowName[i];
        }
        if (m_bMain[i])
        {
            if(iMain >= MAXMAINPARA)
            {
                continue;
            }
            m_tempShow.strMain[iMain++] = m_strShowName[i];
        }
    }
    
    m_tempShow.wParaNum = iShow;
    m_tempShow.wListNum = iList;
    m_tempShow.wMainNum = iMain;
    CDialog::OnOK();
}

HBRUSH CDlgShowSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    
    COLORREF    clrCtrl;
    CString     strContent;
    int         iIndex = 0;
    int         iCtrlID;
    BOOL        bSeled = FALSE;
    
    SHOWCFG     *ptShow = theApp.m_ptCurShow;

    ASSERT_NULL_R(pDC, hbr);
    ASSERT_NULL_R(pWnd, hbr);

    iCtrlID = pWnd->GetDlgCtrlID();

    GetDlgItemText(iCtrlID, strContent);
    if (strContent.IsEmpty())
        return hbr;

    // TODO:  在此更改 DC 的任何特性
    switch (iCtrlID)//对某一个特定控件进行判断  
    {
    case IDC_EDSHOWNAME0:
    case IDC_EDSHOWNAME1:
    case IDC_EDSHOWNAME2:
    case IDC_EDSHOWNAME3:
    case IDC_EDSHOWNAME4:
    case IDC_EDSHOWNAME5:
    case IDC_EDSHOWNAME6:
    case IDC_EDSHOWNAME7:
    case IDC_EDSHOWNAME8:
    case IDC_EDSHOWNAME9:
    case IDC_EDSHOWNAME10:
    case IDC_EDSHOWNAME11:
    case IDC_EDSHOWNAME12:
    case IDC_EDSHOWNAME13:
    case IDC_EDSHOWNAME14:
        //UpdateData(TRUE);
        iIndex = iCtrlID - IDC_EDSHOWNAME0;
        //JUDGE_STRPARA_CHANGE(m_strShowName[iIndex], ptShow->tShow[iIndex].strName.c_str());
        JUDGE_STRPARA_CHANGE(strContent, ptShow->tShow[iIndex].strName.c_str());
        break;
    default:
        hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
        break;
    }

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void CDlgShowSet::JudgeShowRadio(int iCtrlIdx)
{
    COLORREF    clrCtrl;
    BOOL        bSeled = FALSE;

    COMP_BL(iCtrlIdx, 0);
    COMP_BG(iCtrlIdx, MAXPARANUM);
    
    clrCtrl = m_clrNormal;
    bSeled = (m_iFileName == iCtrlIdx);
    if ((bSeled  && iCtrlIdx != theApp.m_ptCurShow->wFileName) ||
        (!bSeled && iCtrlIdx == theApp.m_ptCurShow->wFileName))
    {
        if (iCtrlIdx != theApp.m_ptCurShow->wFileName)
        {
            clrCtrl = m_clrChanged;
        }
    }

    m_rdShowName[iCtrlIdx].SetForeColor(clrCtrl);
    m_rdShowName[iCtrlIdx].Invalidate();
}

void CDlgShowSet::OnBnClickedRadioshowfile0()
{
    UpdateData(TRUE);
    JudgeShowRadio(0);
    JudgeShowRadio(m_iPriorShowCtrlID);
    m_iPriorShowCtrlID = 0;
}
void CDlgShowSet::OnBnClickedRadioshowfile1()
{
    UpdateData(TRUE);
    JudgeShowRadio(1);
    JudgeShowRadio(m_iPriorShowCtrlID);
    m_iPriorShowCtrlID = 1;
}
void CDlgShowSet::OnBnClickedRadioshowfile2()
{
    UpdateData(TRUE);
    JudgeShowRadio(2);
    JudgeShowRadio(m_iPriorShowCtrlID);
    m_iPriorShowCtrlID = 2;
}
void CDlgShowSet::OnBnClickedRadioshowfile3()
{
    UpdateData(TRUE);
    JudgeShowRadio(3);
    JudgeShowRadio(m_iPriorShowCtrlID);
    m_iPriorShowCtrlID = 3;
}
void CDlgShowSet::OnBnClickedRadioshowfile4()
{
    UpdateData(TRUE);
    JudgeShowRadio(4);
    JudgeShowRadio(m_iPriorShowCtrlID);
    m_iPriorShowCtrlID = 4;
}
void CDlgShowSet::OnBnClickedRadioshowfile5()
{
    UpdateData(TRUE);
    JudgeShowRadio(5);
    JudgeShowRadio(m_iPriorShowCtrlID);
    m_iPriorShowCtrlID = 5;
}
void CDlgShowSet::OnBnClickedRadioshowfile6()
{
    UpdateData(TRUE);
    JudgeShowRadio(6);
    JudgeShowRadio(m_iPriorShowCtrlID);
    m_iPriorShowCtrlID = 6;
}
void CDlgShowSet::OnBnClickedRadioshowfile7()
{
    UpdateData(TRUE);
    JudgeShowRadio(7);
    JudgeShowRadio(m_iPriorShowCtrlID);
    m_iPriorShowCtrlID = 7;
}
void CDlgShowSet::OnBnClickedRadioshowfile8()
{
    UpdateData(TRUE);
    JudgeShowRadio(8);
    JudgeShowRadio(m_iPriorShowCtrlID);
    m_iPriorShowCtrlID = 8;
}
void CDlgShowSet::OnBnClickedRadioshowfile9()
{
    UpdateData(TRUE);
    JudgeShowRadio(9);
    JudgeShowRadio(m_iPriorShowCtrlID);
    m_iPriorShowCtrlID = 9;
}
void CDlgShowSet::OnBnClickedRadioshowfile10()
{
    UpdateData(TRUE);
    JudgeShowRadio(10);
    JudgeShowRadio(m_iPriorShowCtrlID);
    m_iPriorShowCtrlID = 10;
}
void CDlgShowSet::OnBnClickedRadioshowfile11()
{
    UpdateData(TRUE);
    JudgeShowRadio(11);
    JudgeShowRadio(m_iPriorShowCtrlID);
    m_iPriorShowCtrlID = 11;
}
void CDlgShowSet::OnBnClickedRadioshowfile12()
{
    UpdateData(TRUE);
    JudgeShowRadio(12);
    JudgeShowRadio(m_iPriorShowCtrlID);
    m_iPriorShowCtrlID = 12;
}
void CDlgShowSet::OnBnClickedRadioshowfile13()
{
    UpdateData(TRUE);
    JudgeShowRadio(13);
    JudgeShowRadio(m_iPriorShowCtrlID);
    m_iPriorShowCtrlID = 13;
}
void CDlgShowSet::OnBnClickedRadioshowfile14()
{
    UpdateData(TRUE);
    JudgeShowRadio(14);
    JudgeShowRadio(m_iPriorShowCtrlID);
    m_iPriorShowCtrlID = 14;
}

void CDlgShowSet::JudgeStatRadio(int iCtrlIdx)
{
    COLORREF    clrCtrl;
    BOOL        bSeled = FALSE;

    COMP_BL(iCtrlIdx, 0);
    COMP_BG(iCtrlIdx, MAXPARANUM);
    
    clrCtrl = m_clrNormal;
    bSeled = (m_iShowStat == iCtrlIdx);
    if ((bSeled  && iCtrlIdx != theApp.m_ptCurShow->wStatType) ||
        (!bSeled && iCtrlIdx == theApp.m_ptCurShow->wStatType))
    if (iCtrlIdx != theApp.m_ptCurShow->wStatType)
    {
        clrCtrl = m_clrChanged;
    }

    m_rdStatName[iCtrlIdx].SetForeColor(clrCtrl);
    m_rdStatName[iCtrlIdx].Invalidate();
}

void CDlgShowSet::OnBnClickedRadioshowstat0()
{
    UpdateData(TRUE);
    JudgeStatRadio(0);
    JudgeStatRadio(m_iPriorStatCtrlID);
    m_iPriorStatCtrlID = 0;
}
void CDlgShowSet::OnBnClickedRadioshowstat1()
{
    UpdateData(TRUE);
    JudgeStatRadio(1);
    JudgeStatRadio(m_iPriorStatCtrlID);
    m_iPriorStatCtrlID = 1;
}
void CDlgShowSet::OnBnClickedRadioshowstat2()
{
    UpdateData(TRUE);
    JudgeStatRadio(2);
    JudgeStatRadio(m_iPriorStatCtrlID);
    m_iPriorStatCtrlID = 2;
}
void CDlgShowSet::OnBnClickedRadioshowstat3()
{
    UpdateData(TRUE);
    JudgeStatRadio(3);
    JudgeStatRadio(m_iPriorStatCtrlID);
    m_iPriorStatCtrlID = 3;
}
void CDlgShowSet::OnBnClickedRadioshowstat4()
{
    UpdateData(TRUE);
    JudgeStatRadio(4);
    JudgeStatRadio(m_iPriorStatCtrlID);
    m_iPriorStatCtrlID = 4;
}
void CDlgShowSet::OnBnClickedRadioshowstat5()
{
    UpdateData(TRUE);
    JudgeStatRadio(5);
    JudgeStatRadio(m_iPriorStatCtrlID);
    m_iPriorStatCtrlID = 5;
}
void CDlgShowSet::OnBnClickedRadioshowstat6()
{
    UpdateData(TRUE);
    JudgeStatRadio(6);
    JudgeStatRadio(m_iPriorStatCtrlID);
    m_iPriorStatCtrlID = 6;
}
void CDlgShowSet::OnBnClickedRadioshowstat7()
{
    UpdateData(TRUE);
    JudgeStatRadio(7);
    JudgeStatRadio(m_iPriorStatCtrlID);
    m_iPriorStatCtrlID = 7;
}
void CDlgShowSet::OnBnClickedRadioshowstat8()
{
    UpdateData(TRUE);
    JudgeStatRadio(8);
    JudgeStatRadio(m_iPriorStatCtrlID);
    m_iPriorStatCtrlID = 8;
}
void CDlgShowSet::OnBnClickedRadioshowstat9()
{
    UpdateData(TRUE);
    JudgeStatRadio(9);
    JudgeStatRadio(m_iPriorStatCtrlID);
    m_iPriorStatCtrlID = 9;
}
void CDlgShowSet::OnBnClickedRadioshowstat10()
{
    UpdateData(TRUE);
    JudgeStatRadio(10);
    JudgeStatRadio(m_iPriorStatCtrlID);
    m_iPriorStatCtrlID = 10;
}

void CDlgShowSet::OnBnClickedRadioshowstat11()
{
    UpdateData(TRUE);
    JudgeStatRadio(11);
    JudgeStatRadio(m_iPriorStatCtrlID);
    m_iPriorStatCtrlID = 11;
}

void CDlgShowSet::OnBnClickedRadioshowstat12()
{
    UpdateData(TRUE);
    JudgeStatRadio(12);
    JudgeStatRadio(m_iPriorStatCtrlID);
    m_iPriorStatCtrlID = 12;
}

void CDlgShowSet::OnBnClickedRadioshowstat13()
{
    UpdateData(TRUE);
    JudgeStatRadio(13);
    JudgeStatRadio(m_iPriorStatCtrlID);
    m_iPriorStatCtrlID = 13;
}
void CDlgShowSet::OnBnClickedRadioshowstat14()
{
    UpdateData(TRUE);
    JudgeStatRadio(14);
    JudgeStatRadio(m_iPriorStatCtrlID);
    m_iPriorStatCtrlID = 14;
}


void CDlgShowSet::JudgeListCheck(int iCtrlIdx)
{
    COLORREF    clrCtrl;

    COMP_BL(iCtrlIdx, 0);
    COMP_BG(iCtrlIdx, MAXPARANUM);
    
    clrCtrl = m_clrNormal;
    if (m_bHisList[iCtrlIdx] != m_bList[iCtrlIdx])
    {
        clrCtrl = m_clrChanged;
    }

    m_clListName[iCtrlIdx].SetForeColor(clrCtrl);
    m_clListName[iCtrlIdx].Invalidate();
}
void CDlgShowSet::OnBnClickedChecklist0()
{
    UpdateData(TRUE);
    JudgeListCheck(0);
}
void CDlgShowSet::OnBnClickedChecklist1()
{
    UpdateData(TRUE);
    JudgeListCheck(1);
}
void CDlgShowSet::OnBnClickedChecklist2()
{
    UpdateData(TRUE);
    JudgeListCheck(2);
}
void CDlgShowSet::OnBnClickedChecklist3()
{
    UpdateData(TRUE);
    JudgeListCheck(3);
}
void CDlgShowSet::OnBnClickedChecklist4()
{
    UpdateData(TRUE);
    JudgeListCheck(4);
}
void CDlgShowSet::OnBnClickedChecklist5()
{
    UpdateData(TRUE);
    JudgeListCheck(5);
}
void CDlgShowSet::OnBnClickedChecklist6()
{
    UpdateData(TRUE);
    JudgeListCheck(6);
}
void CDlgShowSet::OnBnClickedChecklist7()
{
    UpdateData(TRUE);
    JudgeListCheck(7);
}
void CDlgShowSet::OnBnClickedChecklist8()
{
    UpdateData(TRUE);
    JudgeListCheck(8);
}
void CDlgShowSet::OnBnClickedChecklist9()
{
    UpdateData(TRUE);
    JudgeListCheck(9);
}

void CDlgShowSet::OnBnClickedChecklist10()
{
    UpdateData(TRUE);
    JudgeListCheck(10);
}
void CDlgShowSet::OnBnClickedChecklist11()
{
    UpdateData(TRUE);
    JudgeListCheck(11);
}
void CDlgShowSet::OnBnClickedChecklist12()
{
    UpdateData(TRUE);
    JudgeListCheck(12);
}
void CDlgShowSet::OnBnClickedChecklist13()
{
    UpdateData(TRUE);
    JudgeListCheck(13);
}
void CDlgShowSet::OnBnClickedChecklist14()
{
    UpdateData(TRUE);
    JudgeListCheck(14);
}

void CDlgShowSet::JudgeMainCheck(int iCtrlIdx)
{
    COLORREF    clrCtrl;

    COMP_BL(iCtrlIdx, 0);
    COMP_BG(iCtrlIdx, MAXPARANUM);
    
    clrCtrl = m_clrNormal;
    if (m_bHisMain[iCtrlIdx] != m_bMain[iCtrlIdx])
    {
        clrCtrl = m_clrChanged;
    }

    m_clMainName[iCtrlIdx].SetForeColor(clrCtrl);
    m_clMainName[iCtrlIdx].Invalidate();
}
void CDlgShowSet::OnBnClickedCheckmain0()
{
    UpdateData(TRUE);
    JudgeMainCheck(0);
}
void CDlgShowSet::OnBnClickedCheckmain1()
{
    UpdateData(TRUE);
    JudgeMainCheck(1);
}
void CDlgShowSet::OnBnClickedCheckmain2()
{
    UpdateData(TRUE);
    JudgeMainCheck(2);
}
void CDlgShowSet::OnBnClickedCheckmain3()
{
    UpdateData(TRUE);
    JudgeMainCheck(3);
}
void CDlgShowSet::OnBnClickedCheckmain4()
{
    UpdateData(TRUE);
    JudgeMainCheck(4);
}
void CDlgShowSet::OnBnClickedCheckmain5()
{
    UpdateData(TRUE);
    JudgeMainCheck(5);
}
void CDlgShowSet::OnBnClickedCheckmain6()
{
    UpdateData(TRUE);
    JudgeMainCheck(6);
}
void CDlgShowSet::OnBnClickedCheckmain7()
{
    UpdateData(TRUE);
    JudgeMainCheck(7);
}
void CDlgShowSet::OnBnClickedCheckmain8()
{
    UpdateData(TRUE);
    JudgeMainCheck(8);
}
void CDlgShowSet::OnBnClickedCheckmain9()
{
    UpdateData(TRUE);
    JudgeMainCheck(9);
}

void CDlgShowSet::OnBnClickedCheckmain10()
{
    UpdateData(TRUE);
    JudgeMainCheck(10);
}
void CDlgShowSet::OnBnClickedCheckmain11()
{
    UpdateData(TRUE);
    JudgeMainCheck(11);
}
void CDlgShowSet::OnBnClickedCheckmain12()
{
    UpdateData(TRUE);
    JudgeMainCheck(12);
}
void CDlgShowSet::OnBnClickedCheckmain13()
{
    UpdateData(TRUE);
    JudgeMainCheck(13);
}
void CDlgShowSet::OnBnClickedCheckmain14()
{
    UpdateData(TRUE);
    JudgeMainCheck(14);
}