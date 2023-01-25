// DlgCollect.cpp : implementation file
//

#include "stdafx.h"
#include "Drill.h"
#include "DlgCollect.h"
#include "DrillDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ONEMAXTORQUE 10

/////////////////////////////////////////////////////////////////////////////
// CDlgCollect dialog


//CDrillDlg* g_ptParentDlg = NULL;


CDlgCollect::CDlgCollect(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgCollect::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgCollect)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

#define INVALID_COLLECTDATA(strInfo)    {                       \
        m_nErrorNum ++;                                         \
        m_strLastError = (strInfo);                             \
        theApp.SaveCollectErrorData((strInfo),                  \
                                    thepDlg->m_ucRcvByte, \
                                    thepDlg->m_wRcvLen);  \
        return FALSE;                                           \
    }

void CDlgCollect::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgCollect)
    DDX_Control(pDX, IDC_BTNCOLLECT, m_btnCollect);
    DDX_Control(pDX, IDCANCEL, m_btnCollCancel);
    DDX_Control(pDX, IDC_BTNXLSSAVE, m_btnXls);
    DDX_Control(pDX, IDC_LISTDATA, m_listData);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCollect, CDialog)
    //{{AFX_MSG_MAP(CDlgCollect)
    ON_BN_CLICKED(IDC_BTNXLSSAVE, OnBtnxlssave)
    ON_BN_CLICKED(IDC_BTNCOLLECT, OnBtncollect)
    ON_MESSAGE(WM_COLLECT_DATA, ProcessCollData)
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDCANCEL, &CDlgCollect::OnBnClickedCancel)
    ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCollect message handlers

BOOL CDlgCollect::OnInitDialog()
{
    string      strHead;
    CRect       rcView;
    //char        buffer[MAX_LOADSTRING];
    int         iWidth = 0;

    CDialog::OnInitDialog();

    m_btnCollect.SetIconAndText(IDI_LOAD, IDS_STRHISDATA);
    m_btnXls.SetIconAndText(IDI_EXCEL, IDS_STRSAVEXLS);
    m_btnCollCancel.SetIconAndText(IDI_POWER, IDS_STRRETURN);

    m_listData.GetWindowRect(&rcView);

    iWidth = (int)(rcView.Width() / 4);
    m_listData.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT);
    //strHead.Format("上扣时间,%d;扭拧周数,%d;扭拧扭矩,%d;",nWidth*2,nWidth,nWidth);
    //strHead.Format(IDS_STRCOLLISTHEAD,int(iWidth*1.8),(iWidth),(iWidth));
    /*snprintf(buffer, MAX_LOADSTRING, theApp.LoadstringFromRes(IDS_STRCOLLISTHEAD),
        int(iWidth * 1.8), (iWidth), (iWidth));*/

    strHead = string_format(theApp.LoadstringFromRes(IDS_STRCOLLISTHEAD).c_str(),
        int(iWidth * 1.8), (iWidth), (iWidth));
    //strHead = buffer;
    m_listData.SetHeadings(strHead.c_str());
    m_listData.LoadColumnInfo();

    //g_ptParentDlg = (CDrillDlg*)GetParent();

    m_nCollectStatus = NODATA;
    m_nErrorNum = 0;

    /* 收集状态对应的命令 */
    m_nCollCmdType[NODATA] = SCMCOLLECT;
    m_nCollCmdType[VALIDDATA] = SCMCOLLECTOK;
    m_nCollCmdType[LASTDATA] = SCMCOLLECTOK;
    m_nCollCmdType[INVALIDDATA] = SCMCOLLECTNOK;

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCollect::OnBtnxlssave()
{
    BeginWaitCursor();
    theApp.SaveList2XlsFile(NULLSTR, &m_listData);
    EndWaitCursor();
}

void CDlgCollect::OnBtncollect()
{
    theApp.SaveAppStatus(STATUS_COLLECT, __FUNCTION__);

    ResetCollHisData();

    SetTimer(COLLECT_TIMER, COLLECT_TLEN, NULL);

    if (g_tGlbCfg.nTest == COLL_HISTORY)
    {
        thepDlg->RunIniAutoFile();
    }

    m_btnCollect.EnableWindow(FALSE);
}

void CDlgCollect::ResetCollHisData()
{
    memset(&m_tHisData, 0, sizeof(HISDATA));
    m_nErrorNum = 0;
    m_nCollectStatus = NODATA;
}

BOOL CDlgCollect::CheckDataValid(int& iCollNum)
{
    CString strInfo;

    //ASSERT_NULL_R(g_ptParentDlg, FALSE);

    if (thepDlg->m_ucRcvByte[0] != PORT485)
    {
        strInfo.Format(IDS_STRINFPORTERR,
            PORT485,
            thepDlg->m_ucRcvByte[0]);
        INVALID_COLLECTDATA(strInfo.GetBuffer(0));
    }

    if (thepDlg->m_ucRcvByte[1] != 0x10 &&
        thepDlg->m_ucRcvByte[1] != 0x88)
    {
        strInfo.Format(IDS_STRINFRCVCMDERR,
            thepDlg->m_ucRcvByte[1]);
        INVALID_COLLECTDATA(strInfo.GetBuffer(0));
    }

    iCollNum = thepDlg->m_ucRcvByte[2];
    if (iCollNum > ONEMAXTORQUE)
    {
        strInfo.Format(IDS_STRINFCOLOVERFLOW,
            iCollNum,
            ONEMAXTORQUE);
        INVALID_COLLECTDATA(strInfo.GetBuffer(0));
    }

    return TRUE;
}

void CDlgCollect::ReadCollData(int iCollNum)
{
    int     i = 0;
    int     iYear = 0;
    int     iMonth = 0;
    int     iDate = 0;
    int     iHour = 0;
    int     iMinute = 0;
    UINT    nCur = 0;
    double  fTorque = 0;
    double  fPlus = 0;
    BYTE* pucPos = NULL;
    BYTE    ucYM = 0;

    pucPos = &thepDlg->m_ucRcvByte[3];
    nCur = m_tHisData.nReadCount;
    for (i = 0; i < iCollNum; i++)
    {
        /* 年和月在同一个字节内，节省空间 */
        ucYM = (*pucPos++);
        if (ucYM % 12 == 0)
        {
            iMonth = 12;
            iYear = ucYM / 12 - 1 + 2000;
        }
        else
        {
            iYear = ucYM / 12 + 2000;
            iMonth = ucYM % 12;
        }
        iDate = (*pucPos++);
        iHour = (*pucPos++);
        iMinute = (*pucPos++);

        /* 3.17和4.17合一，扭矩需要乘倍率 */
        fTorque = *(pucPos++);
        fTorque = fTorque * 256 + *(pucPos++);
        fTorque *= theApp.m_nTorqMulti;

        fPlus = *(pucPos++);
        fPlus = fPlus * 256.0 + *(pucPos++);

        m_tHisData.tOneData[nCur].tTime = CTime(iYear, iMonth, iDate, iHour, iMinute, 0, 0);
        m_tHisData.tOneData[nCur].fTorque = fTorque;
        m_tHisData.tOneData[nCur].fCir = fPlus / g_tGlbCfg.nPlusPerTurn;

        nCur++;
        if (nCur >= MAXHISDATANUM)
            break;
    }
    m_tHisData.nReadCount = nCur;
    m_nErrorNum = 0;

    m_nCollectStatus = VALIDDATA;
    theApp.SaveCollectOrgData(thepDlg->m_ucRcvByte,
        thepDlg->m_wRcvLen);
}

LRESULT CDlgCollect::ProcessCollData(WPARAM wParam, LPARAM lParam)
{
    int     iCollNum = 0;
    BOOL    bLastData = FALSE;
    BOOL    bValid = FALSE;
    CString strInfo;

    //ASSERT_NULL_R(g_ptParentDlg, 0);

    COMP_BFALSE_R(CheckDataValid(iCollNum), 0);

    if (thepDlg->m_ucRcvByte[1] == 0x88)
    {
        bLastData = TRUE;
    }

    /* 读取有效数据 */
    ReadCollData(iCollNum);

    if (bLastData)
    {
        KillTimer(COLLECT_TIMER);
        m_nCollectStatus = LASTDATA;
        thepDlg->SendData(SCMCOLLECTOK);
        m_btnCollect.EnableWindow(TRUE);

        Sleep(100);
        thepDlg->StartGetValveStatus();
    }

    return 1;
}

void CDlgCollect::CheckErrorTimes()
{
    CString strInfo;

    COMP_BL(m_nErrorNum, 10);

    KillTimer(COLLECT_TIMER);
    m_nCollectStatus = NODATA;
    m_nErrorNum = 0;
    m_btnCollect.EnableWindow(TRUE);

    strInfo.Format(IDS_STRINFREADDATAERR,
        m_nErrorNum,
        m_strLastError);
    theApp.SaveShowMessage(strInfo.GetBuffer(0));

    //ASSERT_NULL(g_ptParentDlg);
    thepDlg->StartGetValveStatus();
}

void CDlgCollect::OnTimer(UINT_PTR nIDEvent)
{
    BOOL    bReadRes = FALSE;
    UINT    nCmdType = SCMCOLLECT;

    COMP_BNE(nIDEvent, COLLECT_TIMER);

    nCmdType = m_nCollCmdType[m_nCollectStatus];

    m_nCollectStatus = INVALIDDATA;

    /* 读取autosave文件中的#COL 开头的Collect数据 */
    if (g_tGlbCfg.nTest == COLL_HISTORY)
    {
        bReadRes = thepDlg->CollectHisData();
    }
    else
    {
        bReadRes = thepDlg->SendData(nCmdType);
    }

    if (bReadRes)
    {
        //ProcessCollData();
    }
    else
    {
        m_strLastError.Format(IDS_STRINFSNDCMDERR, nCmdType);
        m_nErrorNum++;
    }

    /* 检查错误次数，超过一定次数停止收集，杀定时器 */
    CheckErrorTimes();

    ShowCollHisData();

    return;
}

void CDlgCollect::ShowCollHisData()
{
    int     i = 0;
    CString strTime;
    CString strTorque;
    CString strCir;

    m_listData.DeleteAllItems();
    m_listData.SetRedraw(0);

    for (i = m_tHisData.nReadCount - 1; i >= 0; i--)
    {
        strTime = m_tHisData.tOneData[i].tTime.Format(IDS_STRDATETIME);
        strCir.Format("%.3f", m_tHisData.tOneData[i].fCir);
        strTorque.Format("%d", (int)m_tHisData.tOneData[i].fTorque);

        m_listData.AddItem(strTime, strCir, strTorque);
    }

    m_listData.SetRedraw(1);

    GetDlgItem(IDC_BTNXLSSAVE)->EnableWindow(m_listData.GetItemCount() > 0);

    UpdateData(FALSE);
}

void CDlgCollect::OnBnClickedCancel()
{
    KillTimer(COLLECT_TIMER);

    //if (NULL != g_ptParentDlg)
    thepDlg->StartGetValveStatus();
    CDialog::OnCancel();
}


void CDlgCollect::OnDestroy()
{
    CDialog::OnDestroy();

    m_listData.DeleteAllItems();
}
