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
    m_bModified = FALSE;
}

CDlgShowSet::~CDlgShowSet()
{
}

void CDlgShowSet::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDSHOWNAME1, m_strShowName[0]);
    DDX_Text(pDX, IDC_EDSHOWNAME2, m_strShowName[1]);
    DDX_Text(pDX, IDC_EDSHOWNAME3, m_strShowName[2]);
    DDX_Text(pDX, IDC_EDSHOWNAME4, m_strShowName[3]);
    DDX_Text(pDX, IDC_EDSHOWNAME5, m_strShowName[4]);
    DDX_Text(pDX, IDC_EDSHOWNAME6, m_strShowName[5]);
    DDX_Text(pDX, IDC_EDSHOWNAME7, m_strShowName[6]);
    DDX_Text(pDX, IDC_EDSHOWNAME8, m_strShowName[7]);
    DDX_Text(pDX, IDC_EDSHOWNAME9, m_strShowName[8]);
    DDX_Text(pDX, IDC_EDSHOWNAME10, m_strShowName[9]);
    DDX_Text(pDX, IDC_EDSHOWNAME11, m_strShowName[10]);
    DDX_Text(pDX, IDC_EDSHOWNAME12, m_strShowName[11]);
    DDX_Text(pDX, IDC_EDSHOWNAME13, m_strShowName[12]);
    DDX_Text(pDX, IDC_EDSHOWNAME14, m_strShowName[13]);
    DDX_Text(pDX, IDC_EDSHOWNAME15, m_strShowName[14]);
    DDX_Text(pDX, IDC_EDSHOWNAME16, m_strShowName[15]);
    DDX_Text(pDX, IDC_EDSHOWNAME17, m_strShowName[16]);
    DDX_Text(pDX, IDC_EDSHOWNAME18, m_strShowName[17]);
    DDX_Check(pDX, IDC_CHECKLIST1, m_bList[0]);
    DDX_Check(pDX, IDC_CHECKLIST2, m_bList[1]);
    DDX_Check(pDX, IDC_CHECKLIST3, m_bList[2]);
    DDX_Check(pDX, IDC_CHECKLIST4, m_bList[3]);
    DDX_Check(pDX, IDC_CHECKLIST5, m_bList[4]);
    DDX_Check(pDX, IDC_CHECKLIST6, m_bList[5]);
    DDX_Check(pDX, IDC_CHECKLIST7, m_bList[6]);
    DDX_Check(pDX, IDC_CHECKLIST8, m_bList[7]);
    DDX_Check(pDX, IDC_CHECKLIST9, m_bList[8]);
    DDX_Check(pDX, IDC_CHECKLIST10, m_bList[9]);
    DDX_Check(pDX, IDC_CHECKLIST11, m_bList[10]);
    DDX_Check(pDX, IDC_CHECKLIST12, m_bList[11]);
    DDX_Check(pDX, IDC_CHECKLIST13, m_bList[12]);
    DDX_Check(pDX, IDC_CHECKLIST14, m_bList[13]);
    DDX_Check(pDX, IDC_CHECKLIST15, m_bList[14]);
    DDX_Check(pDX, IDC_CHECKLIST16, m_bList[15]);
    DDX_Check(pDX, IDC_CHECKLIST17, m_bList[16]);
    DDX_Check(pDX, IDC_CHECKLIST18, m_bList[17]);
    DDX_Check(pDX, IDC_CHECKMAIN1, m_bMain[0]);
    DDX_Check(pDX, IDC_CHECKMAIN2, m_bMain[1]);
    DDX_Check(pDX, IDC_CHECKMAIN3, m_bMain[2]);
    DDX_Check(pDX, IDC_CHECKMAIN4, m_bMain[3]);
    DDX_Check(pDX, IDC_CHECKMAIN5, m_bMain[4]);
    DDX_Check(pDX, IDC_CHECKMAIN6, m_bMain[5]);
    DDX_Check(pDX, IDC_CHECKMAIN7, m_bMain[6]);
    DDX_Check(pDX, IDC_CHECKMAIN8, m_bMain[7]);
    DDX_Check(pDX, IDC_CHECKMAIN9, m_bMain[8]);
    DDX_Check(pDX, IDC_CHECKMAIN10, m_bMain[9]);
    DDX_Check(pDX, IDC_CHECKMAIN11, m_bMain[10]);
    DDX_Check(pDX, IDC_CHECKMAIN12, m_bMain[11]);
    DDX_Check(pDX, IDC_CHECKMAIN13, m_bMain[12]);
    DDX_Check(pDX, IDC_CHECKMAIN14, m_bMain[13]);
    DDX_Check(pDX, IDC_CHECKMAIN15, m_bMain[14]);
    DDX_Check(pDX, IDC_CHECKMAIN16, m_bMain[15]);
    DDX_Check(pDX, IDC_CHECKMAIN17, m_bMain[16]);
    DDX_Check(pDX, IDC_CHECKMAIN18, m_bMain[17]);
    DDX_Radio(pDX, IDC_RADIOSHOWFILE0, m_iFileName);
    DDX_Radio(pDX, IDC_RADIOSHOWSTAT0, m_iShowStat);
    DDX_Control(pDX, IDC_RADIOSHOWFILE1, m_rdShowName[0]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE2, m_rdShowName[1]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE3, m_rdShowName[2]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE4, m_rdShowName[3]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE5, m_rdShowName[4]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE6, m_rdShowName[5]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE7, m_rdShowName[6]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE8, m_rdShowName[7]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE9, m_rdShowName[8]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE10, m_rdShowName[9]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE11, m_rdShowName[10]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE12, m_rdShowName[11]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE13, m_rdShowName[12]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE14, m_rdShowName[13]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE15, m_rdShowName[14]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE16, m_rdShowName[15]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE17, m_rdShowName[16]);
    DDX_Control(pDX, IDC_RADIOSHOWFILE18, m_rdShowName[17]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT1, m_rdStatName[0]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT2, m_rdStatName[1]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT3, m_rdStatName[2]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT4, m_rdStatName[3]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT5, m_rdStatName[4]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT6, m_rdStatName[5]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT7, m_rdStatName[6]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT8, m_rdStatName[7]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT9, m_rdStatName[8]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT10, m_rdStatName[9]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT11, m_rdStatName[10]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT12, m_rdStatName[11]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT13, m_rdStatName[12]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT14, m_rdStatName[13]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT15, m_rdStatName[14]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT16, m_rdStatName[15]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT17, m_rdStatName[16]);
    DDX_Control(pDX, IDC_RADIOSHOWSTAT18, m_rdStatName[17]);
    DDX_Control(pDX, IDC_CHECKLIST1, m_clListName[0]);
    DDX_Control(pDX, IDC_CHECKLIST2, m_clListName[1]);
    DDX_Control(pDX, IDC_CHECKLIST3, m_clListName[2]);
    DDX_Control(pDX, IDC_CHECKLIST4, m_clListName[3]);
    DDX_Control(pDX, IDC_CHECKLIST5, m_clListName[4]);
    DDX_Control(pDX, IDC_CHECKLIST6, m_clListName[5]);
    DDX_Control(pDX, IDC_CHECKLIST7, m_clListName[6]);
    DDX_Control(pDX, IDC_CHECKLIST8, m_clListName[7]);
    DDX_Control(pDX, IDC_CHECKLIST9, m_clListName[8]);
    DDX_Control(pDX, IDC_CHECKLIST10, m_clListName[9]);
    DDX_Control(pDX, IDC_CHECKLIST11, m_clListName[10]);
    DDX_Control(pDX, IDC_CHECKLIST12, m_clListName[11]);
    DDX_Control(pDX, IDC_CHECKLIST13, m_clListName[12]);
    DDX_Control(pDX, IDC_CHECKLIST14, m_clListName[13]);
    DDX_Control(pDX, IDC_CHECKLIST15, m_clListName[14]);
    DDX_Control(pDX, IDC_CHECKLIST16, m_clListName[15]);
    DDX_Control(pDX, IDC_CHECKLIST17, m_clListName[16]);
    DDX_Control(pDX, IDC_CHECKLIST18, m_clListName[17]);
    DDX_Control(pDX, IDC_CHECKMAIN1, m_clMainName[0]);
    DDX_Control(pDX, IDC_CHECKMAIN2, m_clMainName[1]);
    DDX_Control(pDX, IDC_CHECKMAIN3, m_clMainName[2]);
    DDX_Control(pDX, IDC_CHECKMAIN4, m_clMainName[3]);
    DDX_Control(pDX, IDC_CHECKMAIN5, m_clMainName[4]);
    DDX_Control(pDX, IDC_CHECKMAIN6, m_clMainName[5]);
    DDX_Control(pDX, IDC_CHECKMAIN7, m_clMainName[6]);
    DDX_Control(pDX, IDC_CHECKMAIN8, m_clMainName[7]);
    DDX_Control(pDX, IDC_CHECKMAIN9, m_clMainName[8]);
    DDX_Control(pDX, IDC_CHECKMAIN10, m_clMainName[9]);
    DDX_Control(pDX, IDC_CHECKMAIN11, m_clMainName[10]);
    DDX_Control(pDX, IDC_CHECKMAIN12, m_clMainName[11]);
    DDX_Control(pDX, IDC_CHECKMAIN13, m_clMainName[12]);
    DDX_Control(pDX, IDC_CHECKMAIN14, m_clMainName[13]);
    DDX_Control(pDX, IDC_CHECKMAIN15, m_clMainName[14]);
    DDX_Control(pDX, IDC_CHECKMAIN16, m_clMainName[15]);
    DDX_Control(pDX, IDC_CHECKMAIN17, m_clMainName[16]);
    DDX_Control(pDX, IDC_CHECKMAIN18, m_clMainName[17]);
}

BEGIN_MESSAGE_MAP(CDlgShowSet, CDialog)
    ON_WM_CTLCOLOR()
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
    ON_BN_CLICKED(IDC_RADIOSHOWFILE15, &CDlgShowSet::OnBnClickedRadioshowfile15)
    ON_BN_CLICKED(IDC_RADIOSHOWFILE16, &CDlgShowSet::OnBnClickedRadioshowfile16)
    ON_BN_CLICKED(IDC_RADIOSHOWFILE17, &CDlgShowSet::OnBnClickedRadioshowfile17)
    ON_BN_CLICKED(IDC_RADIOSHOWFILE18, &CDlgShowSet::OnBnClickedRadioshowfile18)
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
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT15, &CDlgShowSet::OnBnClickedRadioshowstat15)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT16, &CDlgShowSet::OnBnClickedRadioshowstat16)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT17, &CDlgShowSet::OnBnClickedRadioshowstat17)
    ON_BN_CLICKED(IDC_RADIOSHOWSTAT18, &CDlgShowSet::OnBnClickedRadioshowstat18)
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
    ON_BN_CLICKED(IDC_CHECKLIST15, &CDlgShowSet::OnBnClickedChecklist15)
    ON_BN_CLICKED(IDC_CHECKLIST16, &CDlgShowSet::OnBnClickedChecklist16)
    ON_BN_CLICKED(IDC_CHECKLIST17, &CDlgShowSet::OnBnClickedChecklist17)
    ON_BN_CLICKED(IDC_CHECKLIST18, &CDlgShowSet::OnBnClickedChecklist18)
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
    ON_BN_CLICKED(IDC_CHECKMAIN15, &CDlgShowSet::OnBnClickedCheckmain15)
    ON_BN_CLICKED(IDC_CHECKMAIN16, &CDlgShowSet::OnBnClickedCheckmain16)
    ON_BN_CLICKED(IDC_CHECKMAIN17, &CDlgShowSet::OnBnClickedCheckmain17)
    ON_BN_CLICKED(IDC_CHECKMAIN18, &CDlgShowSet::OnBnClickedCheckmain18)
    ON_BN_CLICKED(IDC_UPDATESHOW, &CDlgShowSet::OnBnClickedUpdateshow)
END_MESSAGE_MAP()

// CDlgShowSet message handlers
BOOL CDlgShowSet::OnInitDialog()
{
    WORD    i = 0;
    //WORD    j = 0;
    //string  strList;
    //string  strMain;

    CDialog::OnInitDialog();

    m_clrNormal  = RGB(0, 0, 0); 
    m_clrChanged = RGB(255, 0, 0);
    memset(&m_bList[0], FALSE, sizeof(BOOL) * (MAXPARANUM));
    memset(&m_bMain[0], FALSE, sizeof(BOOL) * (MAXPARANUM));
    memset(&m_bHisList[0], FALSE, sizeof(BOOL) * (MAXPARANUM));
    memset(&m_bHisMain[0], FALSE, sizeof(BOOL) * (MAXPARANUM));

    for(i=0; i<m_tempShow.nParaNum && i<MAXPARANUM; i++)
    {
        m_strShowName[i] = m_tempShow.strShow[i].c_str();
    }
    m_iPriorShowCtrlID = m_iFileName = m_tempShow.nFileName;
    m_iPriorStatCtrlID = m_iShowStat = m_tempShow.nStatType;
    for(i=0; i<m_tempShow.nListNum; i++)
    {
        m_bHisList[m_tempShow.nList[i]] = m_bList[m_tempShow.nList[i]] = TRUE;
    }
    
    for(i=0; i< m_tempShow.nMainNum; i++)
    {
        m_bHisMain[m_tempShow.nMain[i]] = m_bMain[m_tempShow.nMain[i]] = TRUE;
    }

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CDlgShowSet::OnBnClickedUpdateshow()
{
    WORD    i = 0;
    int     iList = 0;
    int     iMain = 0;

    UpdateData(TRUE);

    for (i = 0; i < MAXPARANUM; i++)
    {
        if (m_strShowName[i].IsEmpty())
        {
            GetDlgItem(IDC_EDSHOWNAME1 + i)->SetFocus();
            AfxMessageBox(theApp.LoadstringFromRes(IDS_STRNULLSHOWNAME).c_str());
            return;
        }
    }

    memset(&m_tempShow.nList[0], 0, sizeof(UINT) * MAXPARANUM);
    memset(&m_tempShow.nMain[0], 0, sizeof(UINT) * MAXMAINPARA);
    m_tempShow.nFileName = m_iFileName;
    m_tempShow.nStatType = m_iShowStat;
    for (i = 0; i < MAXPARANUM; i++)
    {
        m_tempShow.strShow[i] = m_strShowName[i];

        if (m_bList[i])
        {
            m_tempShow.nList[iList++] = i;
        }
        if (m_bMain[i])
        {
            if (iMain >= MAXMAINPARA)
            {
                continue;
            }
            m_tempShow.nMain[iMain++] = i;
        }
    }

    m_tempShow.nParaNum = MAXPARANUM;
    m_tempShow.nListNum = iList;
    m_tempShow.nMainNum = iMain;

    SaveShowCfgInotDB();
}

BOOL CDlgShowSet::CheckShowCfgChanged()
{
    int i = 0;
    SHOWCFG* ptCurShow = theApp.m_ptCurShow;

    if (m_tempShow.nListNum != ptCurShow->nListNum ||
        m_tempShow.nMainNum != ptCurShow->nMainNum ||
        m_tempShow.nFileName != ptCurShow->nFileName ||
        m_tempShow.nStatType != ptCurShow->nStatType)
        return TRUE;

    if (0 != memcmp(&m_tempShow.nShow[0], &ptCurShow->nShow[0], sizeof(UINT) * MAXPARANUM) ||
        0 != memcmp(&m_bHisMain[0], &m_bMain[0], sizeof(BOOL) * MAXPARANUM) ||
        0 != memcmp(&m_bHisList[0], &m_bList[0], sizeof(BOOL) * MAXPARANUM))
    {
        return TRUE;
    }

    for (i = 0; i < MAXPARANUM; i++)
    {
        if (m_tempShow.strShow[i] != ptCurShow->strShow[i])
            return TRUE;
    }

    return FALSE;
}

BOOL CDlgShowSet::SaveShowCfgInotDB()
{
    if (!theDB.UpdateShowName(&m_tempShow))
        return FALSE;

    // update showcfg
    m_bModified = CheckShowCfgChanged();
    if (m_bModified)
    {
        return theDB.UpdateShowPara(&m_tempShow);
    }

    return TRUE;
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
    case IDC_EDSHOWNAME15:
    case IDC_EDSHOWNAME16:
    case IDC_EDSHOWNAME17:
    case IDC_EDSHOWNAME18:
        //UpdateData(TRUE);
        iIndex = iCtrlID - IDC_EDSHOWNAME1;
        //JUDGE_STRPARA_CHANGE(m_strShowName[iIndex], ptShow->tShow[iIndex].strName.c_str());
        JUDGE_STRPARA_CHANGE(strContent, ptShow->strShow[iIndex].c_str());
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
    COMP_BGE(iCtrlIdx, MAXPARANUM);
    
    clrCtrl = m_clrNormal;
    bSeled = (m_iFileName == iCtrlIdx);
    if ((bSeled  && iCtrlIdx != theApp.m_ptCurShow->nFileName) ||
        (!bSeled && iCtrlIdx == theApp.m_ptCurShow->nFileName))
    {
        if (iCtrlIdx != theApp.m_ptCurShow->nFileName)
        {
            clrCtrl = m_clrChanged;
        }
    }

    m_rdShowName[iCtrlIdx].SetForeColor(clrCtrl);
    m_rdShowName[iCtrlIdx].Invalidate();
}
void CDlgShowSet::UpdateShowFile(int iCtrlIdx)
{
    COMP_BL(iCtrlIdx, 0);
    COMP_BGE(iCtrlIdx, MAXPARANUM);

    UpdateData(TRUE);
    JudgeShowRadio(iCtrlIdx);
    JudgeShowRadio(m_iPriorShowCtrlID);
    m_iPriorShowCtrlID = iCtrlIdx;
}
void CDlgShowSet::OnBnClickedRadioshowfile1()
{
    UpdateShowFile(0);
}
void CDlgShowSet::OnBnClickedRadioshowfile2()
{
    UpdateShowFile(1);
}
void CDlgShowSet::OnBnClickedRadioshowfile3()
{
    UpdateShowFile(2);
}
void CDlgShowSet::OnBnClickedRadioshowfile4()
{
    UpdateShowFile(3);
}
void CDlgShowSet::OnBnClickedRadioshowfile5()
{
    UpdateShowFile(4);
}
void CDlgShowSet::OnBnClickedRadioshowfile6()
{
    UpdateShowFile(5);
}
void CDlgShowSet::OnBnClickedRadioshowfile7()
{
    UpdateShowFile(6);
}
void CDlgShowSet::OnBnClickedRadioshowfile8()
{
    UpdateShowFile(7);
}
void CDlgShowSet::OnBnClickedRadioshowfile9()
{
    UpdateShowFile(8);
}
void CDlgShowSet::OnBnClickedRadioshowfile10()
{
    UpdateShowFile(9);
}
void CDlgShowSet::OnBnClickedRadioshowfile11()
{
    UpdateShowFile(10);
}
void CDlgShowSet::OnBnClickedRadioshowfile12()
{
    UpdateShowFile(11);
}
void CDlgShowSet::OnBnClickedRadioshowfile13()
{
    UpdateShowFile(12);
}
void CDlgShowSet::OnBnClickedRadioshowfile14()
{
    UpdateShowFile(13);
}
void CDlgShowSet::OnBnClickedRadioshowfile15()
{
    UpdateShowFile(14);
}
void CDlgShowSet::OnBnClickedRadioshowfile16()
{
    UpdateShowFile(15);
}
void CDlgShowSet::OnBnClickedRadioshowfile17()
{
    UpdateShowFile(16);
}
void CDlgShowSet::OnBnClickedRadioshowfile18()
{
    UpdateShowFile(17);
}

void CDlgShowSet::JudgeStatRadio(int iCtrlIdx)
{
    COLORREF    clrCtrl;
    BOOL        bSeled = FALSE;

    COMP_BL(iCtrlIdx, 0);
    COMP_BGE(iCtrlIdx, MAXPARANUM);
    
    clrCtrl = m_clrNormal;
    bSeled = (m_iShowStat == iCtrlIdx);
    if ((bSeled  && iCtrlIdx != theApp.m_ptCurShow->nStatType) ||
        (!bSeled && iCtrlIdx == theApp.m_ptCurShow->nStatType))
    if (iCtrlIdx != theApp.m_ptCurShow->nStatType)
    {
        clrCtrl = m_clrChanged;
    }

    m_rdStatName[iCtrlIdx].SetForeColor(clrCtrl);
    m_rdStatName[iCtrlIdx].Invalidate();
}
void CDlgShowSet::UpdateShowStat(int iCtrlIdx)
{
    COMP_BL(iCtrlIdx, 0);
    COMP_BGE(iCtrlIdx, MAXPARANUM);

    UpdateData(TRUE);
    JudgeStatRadio(iCtrlIdx);
    JudgeStatRadio(m_iPriorStatCtrlID);
    m_iPriorStatCtrlID = iCtrlIdx;
}
void CDlgShowSet::OnBnClickedRadioshowstat1()
{
    UpdateShowStat(0);
}
void CDlgShowSet::OnBnClickedRadioshowstat2()
{
    UpdateShowStat(1);
}
void CDlgShowSet::OnBnClickedRadioshowstat3()
{
    UpdateShowStat(2);
}
void CDlgShowSet::OnBnClickedRadioshowstat4()
{
    UpdateShowStat(3);
}
void CDlgShowSet::OnBnClickedRadioshowstat5()
{
    UpdateShowStat(4);
}
void CDlgShowSet::OnBnClickedRadioshowstat6()
{
    UpdateShowStat(5);
}
void CDlgShowSet::OnBnClickedRadioshowstat7()
{
    UpdateShowStat(6);
}
void CDlgShowSet::OnBnClickedRadioshowstat8()
{
    UpdateShowStat(7);
}
void CDlgShowSet::OnBnClickedRadioshowstat9()
{
    UpdateShowStat(8);
}
void CDlgShowSet::OnBnClickedRadioshowstat10()
{
    UpdateShowStat(9);
}
void CDlgShowSet::OnBnClickedRadioshowstat11()
{
    UpdateShowStat(10);
}
void CDlgShowSet::OnBnClickedRadioshowstat12()
{
    UpdateShowStat(11);
}
void CDlgShowSet::OnBnClickedRadioshowstat13()
{
    UpdateShowStat(12);
}
void CDlgShowSet::OnBnClickedRadioshowstat14()
{
    UpdateShowStat(13);
}
void CDlgShowSet::OnBnClickedRadioshowstat15()
{
    UpdateShowStat(14);
}
void CDlgShowSet::OnBnClickedRadioshowstat16()
{
    UpdateShowStat(15);
}
void CDlgShowSet::OnBnClickedRadioshowstat17()
{
    UpdateShowStat(16);
}
void CDlgShowSet::OnBnClickedRadioshowstat18()
{
    UpdateShowStat(17);
}

void CDlgShowSet::JudgeListCheck(int iCtrlIdx)
{
    COLORREF    clrCtrl;

    COMP_BL(iCtrlIdx, 0);
    COMP_BGE(iCtrlIdx, MAXPARANUM);
    UpdateData(TRUE);
    
    clrCtrl = m_clrNormal;
    if (m_bHisList[iCtrlIdx] != m_bList[iCtrlIdx])
    {
        clrCtrl = m_clrChanged;
    }

    m_clListName[iCtrlIdx].SetForeColor(clrCtrl);
    m_clListName[iCtrlIdx].Invalidate();
}
void CDlgShowSet::OnBnClickedChecklist1()
{
    JudgeListCheck(0);
}
void CDlgShowSet::OnBnClickedChecklist2()
{
    JudgeListCheck(1);
}
void CDlgShowSet::OnBnClickedChecklist3()
{
    JudgeListCheck(2);
}
void CDlgShowSet::OnBnClickedChecklist4()
{
    JudgeListCheck(3);
}
void CDlgShowSet::OnBnClickedChecklist5()
{
    JudgeListCheck(4);
}
void CDlgShowSet::OnBnClickedChecklist6()
{
    JudgeListCheck(5);
}
void CDlgShowSet::OnBnClickedChecklist7()
{
    JudgeListCheck(6);
}
void CDlgShowSet::OnBnClickedChecklist8()
{
    JudgeListCheck(7);
}
void CDlgShowSet::OnBnClickedChecklist9()
{
    JudgeListCheck(8);
}
void CDlgShowSet::OnBnClickedChecklist10()
{
    JudgeListCheck(9);
}
void CDlgShowSet::OnBnClickedChecklist11()
{
    JudgeListCheck(10);
}
void CDlgShowSet::OnBnClickedChecklist12()
{
    JudgeListCheck(11);
}
void CDlgShowSet::OnBnClickedChecklist13()
{
    JudgeListCheck(12);
}
void CDlgShowSet::OnBnClickedChecklist14()
{
    JudgeListCheck(13);
}
void CDlgShowSet::OnBnClickedChecklist15()
{
    JudgeListCheck(14);
}
void CDlgShowSet::OnBnClickedChecklist16()
{
    JudgeListCheck(15);
}
void CDlgShowSet::OnBnClickedChecklist17()
{
    JudgeListCheck(16);
}
void CDlgShowSet::OnBnClickedChecklist18()
{
    JudgeListCheck(17);
}

void CDlgShowSet::JudgeMainCheck(int iCtrlIdx)
{
    COLORREF    clrCtrl;

    COMP_BL(iCtrlIdx, 0);
    COMP_BGE(iCtrlIdx, MAXPARANUM);

    UpdateData(TRUE);
    
    clrCtrl = m_clrNormal;
    if (m_bHisMain[iCtrlIdx] != m_bMain[iCtrlIdx])
    {
        clrCtrl = m_clrChanged;
    }

    m_clMainName[iCtrlIdx].SetForeColor(clrCtrl);
    m_clMainName[iCtrlIdx].Invalidate();
}
void CDlgShowSet::OnBnClickedCheckmain1()
{
    JudgeMainCheck(0);
}
void CDlgShowSet::OnBnClickedCheckmain2()
{
    JudgeMainCheck(1);
}
void CDlgShowSet::OnBnClickedCheckmain3()
{
    JudgeMainCheck(2);
}
void CDlgShowSet::OnBnClickedCheckmain4()
{
    JudgeMainCheck(3);
}
void CDlgShowSet::OnBnClickedCheckmain5()
{
    JudgeMainCheck(4);
}
void CDlgShowSet::OnBnClickedCheckmain6()
{
    JudgeMainCheck(5);
}
void CDlgShowSet::OnBnClickedCheckmain7()
{
    JudgeMainCheck(6);
}
void CDlgShowSet::OnBnClickedCheckmain8()
{
    JudgeMainCheck(7);
}
void CDlgShowSet::OnBnClickedCheckmain9()
{
    JudgeMainCheck(8);
}
void CDlgShowSet::OnBnClickedCheckmain10()
{
    JudgeMainCheck(9);
}
void CDlgShowSet::OnBnClickedCheckmain11()
{
    JudgeMainCheck(10);
}
void CDlgShowSet::OnBnClickedCheckmain12()
{
    JudgeMainCheck(11);
}
void CDlgShowSet::OnBnClickedCheckmain13()
{
    JudgeMainCheck(12);
}
void CDlgShowSet::OnBnClickedCheckmain14()
{
    JudgeMainCheck(13);
}
void CDlgShowSet::OnBnClickedCheckmain15()
{
    JudgeMainCheck(14);
}
void CDlgShowSet::OnBnClickedCheckmain16()
{
    JudgeMainCheck(15);
}
void CDlgShowSet::OnBnClickedCheckmain17()
{
    JudgeMainCheck(16);
}
void CDlgShowSet::OnBnClickedCheckmain18()
{
    JudgeMainCheck(17);
}
