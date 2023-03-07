// DataModDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataMod.h"
#include "DataModDlg.h"
#include "DlgListMod.h"
#include "DlgHisGrp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // Dialog Data
        //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
        // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataModDlg dialog

CDataModDlg::CDataModDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CDataModDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDataModDlg)
    m_strDataName = _T("");
    m_strDataName2 = _T("");
    m_nSrc1 = 1;
    m_nDest = 1;
    m_nInsert = 1;
    m_nDestEnd = 1;
    m_iReplace = 0;
    m_nHisTotalRec = 0;
    m_nHisQualyRec = 0;
    m_nHisUnQualyRec = 0;
    m_nTotal2 = 0;
    m_nSrc2 = 1;
    m_bUpdWellNO = TRUE;

    m_bCanMod = FALSE;
    m_bHaveOtherSrc = FALSE;
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_ptStatTorq = NULL;
}

void CDataModDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDataModDlg)
    DDX_Control(pDX, IDC_BTNREPLACE, m_btnReplace);
    DDX_Control(pDX, IDC_BTNOPT, m_btnOpt);
    DDX_Control(pDX, IDC_BTNOPENDATA, m_btnOpen);
    DDX_Control(pDX, IDC_BTNINSERT, m_btnInsert);
    DDX_Control(pDX, IDC_BTNDEL, m_btnDel);
    DDX_Control(pDX, IDC_BTNPW, m_btnPW);
    DDX_Control(pDX, IDC_BTNMERGE, m_btnMerge);
    DDX_Control(pDX, IDC_LISTDATA, m_listData);
    DDX_Control(pDX, IDC_LISTDATA2, m_listData2);
    DDX_Text(pDX, IDC_EDITHISDATA, m_strDataName);
    DDX_Text(pDX, IDC_EDITSRC, m_nSrc1);
    DDX_Text(pDX, IDC_EDITDEST, m_nDest);
    DDX_Text(pDX, IDC_EDITINSERT, m_nInsert);
    DDX_Radio(pDX, IDC_RADIOSINGLE, m_iReplace);
    DDX_Text(pDX, IDC_EDITDEST2, m_nDestEnd);
    DDX_Text(pDX, IDC_EDTOTAL, m_nHisTotalRec);
    DDX_Text(pDX, IDC_EDQUALIFY, m_nHisQualyRec);
    DDX_Text(pDX, IDC_EDUNQUALIFY, m_nHisUnQualyRec);
    DDX_Check(pDX, IDC_CHECKOTHER, m_bHaveOtherSrc);
    DDX_Text(pDX, IDC_EDITHISDATA2, m_strDataName2);
    DDX_Text(pDX, IDC_EDTOTAL2, m_nTotal2);
    DDX_Text(pDX, IDC_EDITSRC2, m_nSrc2);
    DDX_Check(pDX, IDC_CHECKUPDWELLNO, m_bUpdWellNO);
    DDX_Text(pDX, IDC_EDITWELLNAME, theApp.m_strTallyName);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDataModDlg, CDialog)
    //{{AFX_MSG_MAP(CDataModDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTNOPENDATA, OnBtnopendata)
    ON_BN_CLICKED(IDC_BTNOPT, OnBtnopt)
    ON_BN_CLICKED(IDC_BTNDEL, OnBtndel)
    ON_BN_CLICKED(IDC_BTNREPLACE, OnBtnreplace)
    ON_BN_CLICKED(IDC_BTNINSERT, OnBtninsert)
    ON_BN_CLICKED(IDC_RADIOSINGLE, OnRadiosingle)
    ON_BN_CLICKED(IDC_RADIOMULTI, OnRadiomulti)
    ON_MESSAGE(WM_RE_READDATA, ReReadDataFun)
    ON_BN_CLICKED(IDC_CHECKUPDWELLNO, OnBnClickedCheckupdwellno)
    ON_EN_KILLFOCUS(IDC_EDITWELLNAME, OnEnKillfocusEditwellname)
    ON_BN_CLICKED(IDC_BTNPW, OnBnClickedBtnpw)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_CHECKOTHER, &CDataModDlg::OnBnClickedCheckother)
    ON_BN_CLICKED(IDC_BTNOPENDATA2, &CDataModDlg::OnBtnopendata2)
    ON_NOTIFY(NM_CLICK, IDC_LISTDATA, &CDataModDlg::OnNMClickListdata)
    ON_NOTIFY(NM_CLICK, IDC_LISTDATA2, &CDataModDlg::OnNMClickListdata2)
    ON_BN_CLICKED(IDC_BTNMERGE, &CDataModDlg::OnBnClickedBtnmerge)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataModDlg message handlers

BOOL CDataModDlg::OnInitDialog()
{
    int     i = 0;
    string  strHead;
    CRect   rectView;

    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // TODO: Add extra initialization here

    m_listData.GetWindowRect(&rectView);
    m_iWidth = (int)(rectView.Width() / 12.17);
    m_listData.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT);
    /*m_strFixHead.Format("序号,%d;施工时间,%d;最小扭矩,%d;最佳扭矩,%d;最大扭矩,%d;实际扭矩,%d;实际周数,%d;拐点扭矩,%d;斜坡因子,%d;备注,%d;", // 夹紧扭矩,%d;
        int(0.8 * m_iWidth), int(1.7 * m_iWidth), int(0.9 * m_iWidth), int(0.9 * m_iWidth), int(0.9 * m_iWidth),
        int(0.9 * m_iWidth), int(0.9 * m_iWidth), int(0.9 * m_iWidth), int(0.9 * m_iWidth), int(2 * m_iWidth)); // int(0.9 * m_iWidth),
    strHead = m_strFixHead;*/
    strHead = string_format(theApp.LoadstringFromRes(IDS_STRHISLLISTHEAD).c_str(), int(0.8 * m_iWidth),
        int(2 * m_iWidth), int(2 * m_iWidth), int(0.9 * m_iWidth), int(0.9 * m_iWidth),
        int(0.9 * m_iWidth), int(0.9 * m_iWidth), int(0.9 * m_iWidth), int(0.9 * m_iWidth), int(2 * m_iWidth)); ;
    m_strFixHead = strHead;
    for (i = 0; i < MAXPARANUM; i++)
    {
        strHead += string_format("参数%d, %d;", i + 1, m_iWidth);
    }
    m_listData.SetHeadings(strHead.c_str());
    m_listData.LoadColumnInfo();
    m_listData.nShowNum = MAXPARANUM;

    m_listData2.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT);
    m_listData2.SetHeadings(strHead.c_str());
    m_listData2.LoadColumnInfo();
    m_listData2.nShowNum = MAXPARANUM;
    m_listData2.bSecondData = true;
#if 0
    m_bCanMod = TRUE;
#else
    m_bCanMod = theApp.CheckPassWord();
    if (m_bCanMod)
    {
        m_btnPW.EnableWindow(FALSE);
    }
#endif
    theApp.m_pdlgOpt = new CDlgHisGrp();
    theApp.m_pdlgOpt->Create(IDD_DLGHISGRP, this);
    theApp.m_pdlgOpt->ShowWindow(SW_HIDE);
    theApp.m_pdlgOpt->SetOwner((CWnd*)this);
    theApp.m_pdlgOpt->m_bCanMod = m_bCanMod;

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDataModDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDataModDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDataModDlg::OnQueryDragIcon()
{
    return (HCURSOR)m_hIcon;
}

/* 根据读取数据结果，enable/disable控件 */
void CDataModDlg::EnableCtrl()
{
    BOOL bEnable = FALSE;

    if (m_listData.GetItemCount() > 0)
    {
        bEnable = TRUE;
    }

    GetDlgItem(IDC_LISTDATA)->EnableWindow(bEnable);

    bEnable &= m_bCanMod;

    m_btnDel.EnableWindow(bEnable);
    m_btnInsert.EnableWindow(bEnable);
    m_btnReplace.EnableWindow(bEnable);
    m_btnOpt.EnableWindow(bEnable);

    GetDlgItem(IDC_EDITINSERT)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITDEST)->EnableWindow(bEnable);
    GetDlgItem(IDC_RADIOSINGLE)->EnableWindow(bEnable);
    GetDlgItem(IDC_RADIOMULTI)->EnableWindow(bEnable);
    GetDlgItem(IDC_CHECKUPDWELLNO)->EnableWindow(bEnable);

    GetDlgItem(IDC_EDITSRC)->EnableWindow(bEnable && !m_bHaveOtherSrc);
    m_btnMerge.EnableWindow(bEnable && m_listData2.GetItemCount() > 0);
}

/* 根据读取数据结果，enable/disable控件 */
void CDataModDlg::EnableCtrl2()
{
    BOOL bEnable = FALSE;

    if (m_listData2.GetItemCount() > 0)
    {
        bEnable = TRUE;
    }

    GetDlgItem(IDC_LISTDATA2)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITSRC2)->EnableWindow(bEnable);
}

void CDataModDlg::UpdateTorqNum()
{
    ASSERT_ZERO(g_tReadData.nTotal);

    m_nHisTotalRec = g_tReadData.nTotal;
    m_nHisQualyRec = g_tReadData.nQualy;
    m_nHisUnQualyRec = g_tReadData.nUnQualy;
}

void CDataModDlg::OnBtnopendata()
{
    int     i = 0;
    int     iShowListNum = 0;
    UINT    nMaxShowPlace = 0;
    CString strFilter;
    //BOOL    bReadRes = FALSE;
    CString strHead, strTemp;
    string  strName;
    TorqData::Torque* ptTorq = NULL;

    strFilter.Format(IDS_STRDATFILTER);

    CFileDialog fileDlg(TRUE, "pbd", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, NULL);

    COMP_BNE(fileDlg.DoModal(), IDOK);

    m_strDataName = fileDlg.GetPathName();
    theApp.ReadHisTorqFromFile(m_strDataName.GetBuffer(0));
    UpdateTorqNum();

    if (g_tReadData.nTotal <= 0)
    {
        UpdateData(FALSE);
        return;
    }

    m_listData.DeleteAllItems();

    /* update list head */
    strHead = m_strFixHead.c_str();
    nMaxShowPlace = 0;
    for (i = 0; i < (int)g_tReadData.nTotal; i++)
    {
        ptTorq = &g_tReadData.tData[i];
        if (ptTorq->tshow_size() > iShowListNum)
        {
            nMaxShowPlace = i;
            iShowListNum = ptTorq->tshow_size();
        }
    }
    ptTorq = &g_tReadData.tData[nMaxShowPlace];
    iShowListNum = MIN(iShowListNum, MAXPARANUM);
    m_listData.nShowNum = iShowListNum;

    for (i = 0; i < iShowListNum; i++)
    {
        strName = theApp.GetTorqShowName(ptTorq, i);
        if (strName.empty())
            strName = _T("NULL");
        strTemp.Format("%s, %d;", strName.c_str(), m_iWidth);
        strHead += strTemp;
    }
    m_listData.SetHeadings(strHead);
    m_listData.LoadColumnInfo();

    ShowHisTorqList();

    UpdateData(FALSE);
}

BOOL CDataModDlg::DestroyWindow()
{
    int i = 0;
    if (theApp.m_pdlgOpt != NULL)
    {
        delete theApp.m_pdlgOpt;
        theApp.m_pdlgOpt = NULL;
    }
    m_listData.DeleteAllItems();
    m_listData2.DeleteAllItems();

    return CDialog::DestroyWindow();
}

void CDataModDlg::OnBtnopt()
{
    int         iItem = 0;
    POSITION    pos = m_listData.GetFirstSelectedItemPosition();

    iItem = m_listData.GetNextSelectedItem(pos);

    g_tReadData.nCur = 1;
    /* 有选中取选中值，否则取1 */
    if (iItem > 0)
        g_tReadData.nCur = (iItem + 1);

    theApp.m_pdlgOpt->m_ptTorData = &g_tReadData;
    theApp.m_pdlgOpt->ShowCurData(TRUE);
    theApp.m_pdlgOpt->ShowWindow(SW_SHOW);

    UpdateData(FALSE);
}

void CDataModDlg::OnBtndel()
{
    UINT        ntemNum = 0;
    CString     strTemp;
    CString     strPrint;

    ntemNum = GetSelectItem(&strPrint);
    ASSERT_ZERO(ntemNum);

    strTemp.Format("确定要删除以下(%d)条记录（最多%d条）。记录号为：\r\n", ntemNum, MAXWELLNUM);
    strPrint = strTemp + strPrint + "？";
    if (IDOK == AfxMessageBox(strPrint, MB_ICONEXCLAMATION | MB_OKCANCEL))
    {
        DelSelectData();
        ReReadTorqFile();

        UpdateData(FALSE);
    }
    return;
}

VOID CDataModDlg::ShowHisTorqList(bool bFirst)
{
    UINT        i = 0;
    int         j = 0;
    int         iItem = 0;
    WORD        wPara = 0;
    CString     strNo;
    CString     strTime, strBOTime;
    CString     strMakeTurn, strBreakTurn;
    CString     strOptTorq; // strCtrlTorq, 
    CString     strMakeTorq, strBreakTorq;
    CString     strOutWellNO, strMemo;
    CString     strShowPara[MAXPARANUM];
    CStringList slShow;
    DWORD       dwQuality = 0;
    CString     strNM = _T("N.m");
    //UINT        nCount;
    TORQUEDATA* ptAllData = NULL;
    TorqData::Torque* ptTorq = NULL;
    CMylistctrl* plsShow = NULL;

    if (bFirst)
    {
        ptAllData = &g_tReadData;
        plsShow = &m_listData;
        m_ptStatTorq = NULL;
    }
    else
    {
        ptAllData = &g_tReadData2;
        plsShow = &m_listData2;
    }

    ASSERT_ZERO(ptAllData->nTotal);

    plsShow->SetRedraw(0);

    BeginWaitCursor();
    if (bFirst)
        m_ptStatTorq = &ptAllData->tData[ptAllData->nTotal - 1];
    for (i = 0; i < ptAllData->nTotal; i++)
    {
        ptTorq = &ptAllData->tData[i];

        /* 20180102 序号删除后自动更新 */
        strNo.Format("%d", i + 1);
        strMemo = ptTorq->strmemo().c_str();
        if (theApp.HaveMakeUP(ptTorq))
        {
            strTime = theApp.GetTorqCollTime(ptTorq, false);
            strOptTorq.Format("%d", (int)theApp.GetOptTorq(ptTorq));
            strMakeTorq.Format("%d", (int)ptTorq->fmumaxtorq());
            strMakeTurn.Format("%.3f", theApp.GetCir(ptTorq));
        }
        else
        {
            strTime = _T("-");
            strOptTorq = _T("-");
            strMakeTorq = _T("-");
            strMakeTurn = _T("-");
        }
        //strCtrlTorq.Format("%d", (int)ptTorq->fcontrol());

        if (theApp.HaveBreakout(ptTorq))
        {
            strBOTime = theApp.GetTorqCollTime(ptTorq, true);
            strBreakTorq.Format("%d", (int)ptTorq->fbomaxtorq());
            strBreakTurn.Format("%.3f", theApp.GetCir(ptTorq, true));
            strOutWellNO.Format("%d", ptTorq->dwoutwellno());
        }
        else
        {
            strBOTime = _T("-");
            strBreakTorq = _T("-");
            strBreakTurn = _T("-");
            strOutWellNO = _T("-");
        }

        /*"序号,%d;施工时间,%d;实际扭矩,%d;实际周数,%d;拐点扭矩,%d;斜坡因子,%d; 备注,%d;*/
        /* "序号,%d;施工时间,%d;最小扭矩,%d;控制扭矩,%d;最佳扭矩,%d;最大扭矩,%d;实际扭矩,%d;实际周数,%d;拐点扭矩,%d;斜坡因子,%d;备注,%d;" */
        /* "序号,%d;施工时间,%d;最小扭矩,%d;最佳扭矩,%d;最大扭矩,%d;实际扭矩,%d;实际周数,%d;拐点扭矩,%d;斜坡因子,%d;备注,%d;" */
        /*"序号,%d;上扣时间,%d;卸扣时间,%d;最佳扭矩,%d;上扣扭矩,%d;上扣周数,%d;卸扣扭矩,%d;卸扣周数,%d;取出序号,%d;备注,%d;"*/
        slShow.AddTail(strNo);
        slShow.AddTail(strTime);
        slShow.AddTail(strBOTime);
        //slShow.AddTail(strCtrlTorq);
        slShow.AddTail(strOptTorq);
        slShow.AddTail(strMakeTorq);
        slShow.AddTail(strMakeTurn);
        slShow.AddTail(strBreakTorq);
        slShow.AddTail(strBreakTurn);
        slShow.AddTail(strOutWellNO);
        slShow.AddTail(strMemo);

        for (j = 0; j < ptTorq->tshow_size() && j < (int)plsShow->nShowNum; j++)
        {
            slShow.AddTail(theApp.GetTorqShowValue(ptTorq, j));
        }
        /* 补空 */
        for (; j < (int)plsShow->nShowNum; j++)
            slShow.AddTail(NULLSTR);
        iItem = plsShow->AddItemList(slShow);
        slShow.RemoveAll();

        dwQuality = theApp.GetQuality(ptTorq);
        if (!(dwQuality & QUA_RESU_QUALITYBIT))
        {
            plsShow->SetItemData(iItem, TORQ_BAD_QUALITY);
        }
        if (ptTorq->btoolbuck())
        {
            plsShow->SetItemData(iItem, TORQ_TOOLBUCKLE);
        }
        else if (bFirst)   // 非工具扣设置为统计序号
        {
            if (strMemo.IsEmpty())
                m_ptStatTorq = ptTorq;
        }
    }

    EndWaitCursor();
    plsShow->SetRedraw(1);

    EnableCtrl();
    EnableCtrl2();
}

void CDataModDlg::ReReadTorqFile()
{
    theApp.ReadHisTorqFromFile(m_strDataName.GetBuffer(0));

    UpdateTorqNum();

    m_listData.DeleteAllItems();
    ShowHisTorqList();
}

/* 获取LIST选择的记录信息，有下面三种形式供选择
   //m_bSelItem: 对应位置为TRUE，说明记录被选择，默认为FALSE
   m_iSelItem: 记录选择的序号，最大MAXWELLNUM,从1开始
   */
UINT CDataModDlg::GetSelectItem(CString* pstrItem)
{
    int         iItem = 0;
    UINT        nSelNum = 0;
    UINT        i = 0;
    CString     strItemNo;
    POSITION    pos = m_listData.GetFirstSelectedItemPosition();

    //memset(m_bSelItem, false, sizeof(UINT) * MAXWELLNUM);
    memset(m_nSelItem, 0, sizeof(UINT) * MAXWELLNUM);

    if (0)//m_bExpAllImg
    {
        nSelNum = m_listData.GetItemCount();
        if (nSelNum >= MAXWELLNUM)
        {
            nSelNum = MAXWELLNUM;
        }
        for (i = 0; i < nSelNum; i++)
        {
            //m_bSelItem[i] = true;
            m_nSelItem[i] = i + 1;
            strItemNo = strItemNo + m_listData.GetItemText(i, 0) + " ";
        }
        if (pstrItem != NULL)
            *pstrItem = strItemNo;
        return nSelNum;
    }

    iItem = m_listData.GetNextSelectedItem(pos);
    COMP_BE_R(iItem, -1, 0);

    /* 收集需要处理的记录 */
    while (iItem != -1)
    {
        //m_bSelItem[nSelNum] = true;
        m_nSelItem[nSelNum++] = iItem + 1;
        strItemNo = strItemNo + m_listData.GetItemText(iItem, 0) + " ";

        /* 最大处理MAXWELLNUM个记录 */
        if (nSelNum >= MAXWELLNUM)
        {
            nSelNum = MAXWELLNUM;
            break;
        }

        iItem = m_listData.GetNextSelectedItem(pos);
    }

    if (pstrItem != NULL)
        *pstrItem = strItemNo;
    return nSelNum;
}

/* 删除选定的记录信息
   根据bSelItem信息处理，先以清空文件的形式打开，然后判断bSelItem，没有选择才保存到文件 */
BOOL CDataModDlg::DelSelectData()
{
    CFile   file;
    UINT    nNewNum = 0;
    int     i = 0;
    int     k = 0;
    TorqData::Torque* ptData = NULL;
    //string  strTorq;
    //UINT    nLeng    = 0;
    size_t  nDataLen = 0;

    /* open the file */
    if (!file.Open(m_strDataName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, NULL))
    {
        file.Close();
        return FALSE;
    }

    /* 跳过总记录数 */
    file.Seek(sizeof(UINT), CFile::current);

    for (i = 0; i < (int)g_tReadData.nTotal; i++)
    {
        /* 本记录需要删除，跳过，不保存 */
        if (m_nSelItem[k] - 1 == i)
        {
            k++;
            continue;
        }

        /* 记录没有删除，写入文件 */
        nDataLen = g_tReadData.tData[i].ByteSizeLong();
        if (nDataLen == 0 || nDataLen >= MAXPROBUFF)
            continue;
        memset(theApp.m_cProtoBuf, 0, MAXPROBUFF);
        if (!g_tReadData.tData[i].SerializeToArray(theApp.m_cProtoBuf, nDataLen))
        {
            continue;
        }

        //g_tReadData.tData[i].SerializeToString(&strTorq);
        //nLeng = strTorq.size();
        file.Write(&theApp.m_nPBHead, PBHEADLEN);
        file.Write(&nDataLen, sizeof(UINT));
        file.Write(theApp.m_cProtoBuf, nDataLen);
        nNewNum++;
    }

    /* 最后更新记录数*/
    file.SeekToBegin();
    file.Write(&nNewNum, sizeof(UINT));
    file.Close();
    return TRUE;
}

/* 记录替换，分为单个替换和多个替换
   m_iReplace = 0:单个替换，将m_iSrc记录替换m_iDest记录
   m_iReplace = 1:多个替换，将m_iSrc记录替换从m_iDest到m_iDestEnd的记录
   替换时，目的数据的时间和配置不变*/
void CDataModDlg::OnBtnreplace()
{
    UINT  i = 0;
    TorqData::Torque* ptInsTorq = NULL;

    UpdateData(TRUE);

    /* 入参有效性判断 */
    if (!GetSrcTorq(&ptInsTorq))
    {
        AfxMessageBox("Invalid Source NO., pls re-input!");
        return;
    }
    //JUDGE_RANGE_MESS(m_nSrc1, g_tReadData.nTotal, "源序号无效，请重新输入！");
    if (m_iReplace == 0)
    {
        JUDGE_RANGE_MESS(m_nDest, g_tReadData.nTotal, "目的序号无效，请重新输入！");
        ReplaceTorque(m_nDest, ptInsTorq);

        ReReadTorqFile();
        UpdateData(FALSE);
        return;
    }

    /* 多条记录的替换 */
    JUDGE_RANGE_MESS(m_nDest, g_tReadData.nTotal, "目的起始序号无效，请重新输入！");
    if (m_nDest > m_nDestEnd)
    {
        AfxMessageBox("起始序号大于结束序号，请重新输入！");
        return;
    }

    for (i = m_nDest; i <= m_nDestEnd; i++)
    {
        ReplaceTorque(i, ptInsTorq);
    }

    ReReadTorqFile();
    UpdateData(FALSE);
}

/*  将nSrc记录替换nDest记录
    替换时，目的数据的时间和配置不变*/
void CDataModDlg::ReplaceTorque(UINT nDest, UINT nSrc)
{
    TorqData::Torque* ptSrc = NULL;

    JUDGE_RANGE(nSrc, g_tReadData.nTotal);
    ptSrc = &g_tReadData.tData[nSrc - 1];

    ReplaceTorque(nDest, ptSrc);
}

void CDataModDlg::ReplaceTorque(UINT nDest, TorqData::Torque* ptSrc)
{
    int i = 0, index = 0;
    int  muCount = 0;
    double fMaxCir = 0;
    double fMaxLimit = 0;
    TorqData::Torque  tTempData;
    TorqData::Torque* ptDest = NULL;

    ASSERT_NULL(ptSrc);
    JUDGE_RANGE(nDest, g_tReadData.nTotal);

    tTempData = g_tReadData.tData[nDest - 1];
    ptDest = &g_tReadData.tData[nDest - 1];

    tTempData.set_dwquality(ptSrc->dwquality());
    tTempData.set_btoolbuck(ptSrc->btoolbuck());
    tTempData.set_bsinglestd(ptSrc->bsinglestd());

    tTempData.clear_ftorque();
    tTempData.clear_frpm();
    tTempData.clear_dwdelplus();
    if (theApp.HaveMakeUP(ptSrc))
    {
        if (!theApp.HaveMakeUP(&tTempData))
        {
            tTempData.set_mucoltime(ptSrc->mucoltime());
        }
        tTempData.set_dwmucount(ptSrc->dwmucount());
        tTempData.set_dwmuplus(ptSrc->dwmuplus());
        tTempData.set_fmumaxtorq(ptSrc->fmumaxtorq());
        tTempData.set_fmuduration(ptSrc->fmuduration());
        muCount = ptSrc->dwmucount();
        fMaxLimit = ptSrc->fmaxlimit();

        for (i = 0; i < ptSrc->dwmucount(); i++)
        {
            theApp.UpdateTorqRpm(&tTempData, i, ptSrc->ftorque(i), ptSrc->frpm(i));
            theApp.UpdateDelplus(&tTempData, i, ptSrc->dwdelplus(i));
        }
    }
    else
    {
        muCount = ptDest->dwmucount();
        fMaxLimit = ptDest->fmaxlimit();
        for (i = 0; i < ptDest->dwmucount(); i++)
        {
            theApp.UpdateTorqRpm(&tTempData, i, ptDest->ftorque(i), ptDest->frpm(i));
            theApp.UpdateDelplus(&tTempData, i, ptDest->dwdelplus(i));
        }
    }
    if (theApp.HaveBreakout(ptSrc))
    {
        if (!theApp.HaveBreakout(&tTempData))
        {
            tTempData.set_bocoltime(ptSrc->bocoltime());
            tTempData.set_dwoutwellno(ptSrc->dwoutwellno());
        }
        tTempData.set_dwbocount(ptSrc->dwbocount());
        tTempData.set_dwboplus(ptSrc->dwboplus());
        tTempData.set_fbomaxtorq(ptSrc->fbomaxtorq());
        tTempData.set_fboduration(ptSrc->fboduration());
        fMaxLimit = fMaxLimit > ptSrc->fmaxlimit() ? fMaxLimit : ptSrc->fmaxlimit();

        for (i = 0; i < ptSrc->dwbocount(); i++)
        {
            index = i + ptSrc->dwmucount();
            theApp.UpdateTorqRpm(&tTempData, i + muCount, ptSrc->ftorque(index), ptSrc->frpm(index));
            theApp.UpdateDelplus(&tTempData, i + muCount, ptSrc->dwdelplus(index));
        }
    }
    else
    {
        fMaxLimit = fMaxLimit > ptDest->fmaxlimit() ? fMaxLimit : ptDest->fmaxlimit();
        for (i = 0; i < ptDest->dwbocount(); i++)
        {
            index = i + ptDest->dwmucount();
            theApp.UpdateTorqRpm(&tTempData, i + muCount, ptDest->ftorque(index), ptDest->frpm(index));
            theApp.UpdateDelplus(&tTempData, i + muCount, ptDest->dwdelplus(index));
        }
    }

    if (fMaxLimit > tTempData.fmaxlimit())
    {
        tTempData.set_fmaxlimit(fMaxLimit);
    }

    fMaxCir = theApp.GetCir(&tTempData) + theApp.GetCir(&tTempData, true);
    if (fMaxCir > tTempData.fmaxcir())
    {
        tTempData.set_fmaxcir(ceil(fMaxCir + 0.5));
    }

    theApp.UpdateHisData(m_strDataName, nDest, &tTempData);

    return;
}

bool CDataModDlg::GetSrcTorq(TorqData::Torque** ptSrc)
{
    if (!m_bHaveOtherSrc)
    {
        if (m_nSrc1 == 0 || m_nSrc1 > g_tReadData.nTotal)
            return false;
        //m_nSrc = m_nSrc1;
        *ptSrc = &g_tReadData.tData[m_nSrc1 - 1];
    }
    else
    {
        if (m_nSrc2 == 0 || m_nSrc2 > g_tReadData2.nTotal)
            return false;
        //m_nSrc = m_nSrc2;
        *ptSrc = &g_tReadData2.tData[m_nSrc2 - 1];
    }

    return true;
}

void CDataModDlg::OnBtninsert()
{
    UINT    i = 0;
    int     iLeft = 0;
    UINT    nItemNum = 0;
    UINT    nCurPos = 0;
    UINT    nLeng = 0;
    size_t  nDataLen = 0;
    UINT    nNewCount = 0;
    UINT    nIstCount = 0;
    BYTE* pBuffer = NULL;
    TorqData::Torque* ptInsTorq = NULL;
    CFile   file;
    CString strInfo;
    string  strTorq;

    UpdateData(TRUE);

    /* 入参有效性判断 */
    JUDGE_RANGE_MESS(m_nInsert, g_tReadData.nTotal, "插入序号无效，请重新输入！");
    if (!GetSrcTorq(&ptInsTorq))
    {
        AfxMessageBox("Invalid Source NO., pls re-input!");
        return;
    }

    // 只能单个插入
    /*nItemNum = GetSelectItem(NULL);
    ASSERT_ZERO(nItemNum);*/
    nItemNum = 1;

    nNewCount = g_tReadData.nTotal + nItemNum;
    if (nNewCount > MAXWELLNUM)
    {
        strInfo.Format("当前记录数(%d)与插入记录数(%d)之和超过文件最大记录数(%d)",
            g_tReadData.nTotal, nItemNum, MAXWELLNUM);
        AfxMessageBox(strInfo);
        return;
    }

    /* write to file */
    if (file.Open(m_strDataName, CFile::modeReadWrite | CFile::shareDenyNone, NULL))
    {
        /*更新记录数*/
        file.Seek(sizeof(UINT), CFile::begin);

        /* 跳过之前记录 */
        //SEEK_TORQUE((m_nInsert - 1), nLeng);
        if (0 != theApp.SeekTorque(file, m_nInsert - 1))
        {
            file.Close();
            return;
        }

        /*获取插入位置，将插入位置后的数据缓存*/
        nCurPos = (UINT)file.GetPosition();
        iLeft = (int)(file.GetLength() - nCurPos);
        if (iLeft > 0)
        {
            pBuffer = new BYTE[iLeft];
            memset(pBuffer, 0, iLeft);
            file.Read(pBuffer, iLeft);
        }

        /*将插入的数据写入到文件中*/
        file.Seek(nCurPos, CFile::begin);
        // for (i = 0; i < nItemNum; i++)
        {
            //nDataLen = g_tReadData.tData[m_nSelItem[i]-1].ByteSizeLong();
            nDataLen = ptInsTorq->ByteSizeLong();

            if (nDataLen == 0 || nDataLen >= MAXPROBUFF)
                return;
            memset(theApp.m_cProtoBuf, 0, MAXPROBUFF);
            //if (!g_tReadData.tData[m_nSelItem[i]-1].SerializeToArray(theApp.m_cProtoBuf, nDataLen))
            if (!ptInsTorq->SerializeToArray(theApp.m_cProtoBuf, nDataLen))
            {
                return;
            }

            //g_tReadData.tData[m_iSelItem[i]].SerializeToString(&strTorq);
            //nLeng = strTorq.size();
            file.Write(&theApp.m_nPBHead, PBHEADLEN);
            file.Write(&nDataLen, sizeof(UINT));
            file.Write(theApp.m_cProtoBuf, nDataLen);

            nIstCount++;
        }
        if (pBuffer != NULL)
        {
            file.Write(pBuffer, iLeft);
            delete pBuffer;
            pBuffer = NULL;
        }

        file.SeekToBegin();
        nNewCount = g_tReadData.nTotal + nIstCount;
        file.Write(&nNewCount, sizeof(UINT));

        file.Close();
    }

    ReReadTorqFile();
    UpdateData(FALSE);

    return;
}

void CDataModDlg::OnRadiosingle()
{
    GetDlgItem(IDC_EDITDEST2)->EnableWindow(FALSE);
    m_iReplace = 0;
}

void CDataModDlg::OnRadiomulti()
{
    GetDlgItem(IDC_EDITDEST2)->EnableWindow(TRUE);
    m_iReplace = 1;
}

LRESULT CDataModDlg::ReReadDataFun(WPARAM wParam, LPARAM lParam)
{
    ReReadTorqFile();
    g_tReadData.nCur = (UINT)wParam;
    UpdateData(FALSE);

    return TRUE;
}

void CDataModDlg::OnBnClickedCheckupdwellno()
{
    UpdateData(TRUE);
    GetDlgItem(IDC_EDITWELLNAME)->EnableWindow(m_bUpdWellNO);
}

void CDataModDlg::OnEnKillfocusEditwellname()
{
    UpdateData(TRUE);
}

void CDataModDlg::OnBnClickedBtnpw()
{
    m_bCanMod = theApp.CheckPassWord();
    if (m_bCanMod)
    {
        m_btnPW.EnableWindow(FALSE);
    }

    theApp.m_pdlgOpt->m_bCanMod = m_bCanMod;

    EnableCtrl();
}

void CDataModDlg::OnNMClickListdata(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    /*if (m_bHaveOtherSrc)
    {
        m_nSrc1 = 0;
        UpdateData(FALSE);
        return;
    }*/
    //获取单击所在的行号   
    //找出鼠标位置   
    DWORD dwPos = GetMessagePos();
    CPoint point(LOWORD(dwPos), HIWORD(dwPos));
    m_listData.ScreenToClient(&point);

    //定义结构体   
    LVHITTESTINFO lvinfo;
    lvinfo.pt = point;

    //获取行号信息   
    int nItem = m_listData.HitTest(&lvinfo);
    if (nItem != -1)
    {
        m_nSrc1 = atoi(m_listData.GetItemText(lvinfo.iItem, 0));//当前行号
        UpdateData(FALSE);
    }

    *pResult = 0;
}

void CDataModDlg::OnNMClickListdata2(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    /*if (!m_bHaveOtherSrc)
    {
        m_nSrc2 = 2;
        UpdateData(FALSE);
        return;
    }*/

    //获取单击所在的行号   
    //找出鼠标位置   
    DWORD dwPos = GetMessagePos();
    CPoint point(LOWORD(dwPos), HIWORD(dwPos));
    m_listData2.ScreenToClient(&point);

    //定义结构体   
    LVHITTESTINFO lvinfo;
    lvinfo.pt = point;

    //获取行号信息   
    int nItem = m_listData2.HitTest(&lvinfo);
    if (nItem != -1)
    {
        m_nSrc2 = atoi(m_listData2.GetItemText(lvinfo.iItem, 0));//当前行号
        UpdateData(FALSE);
    }

    *pResult = 0;
}

void CDataModDlg::OnBnClickedCheckother()
{
    UpdateData(TRUE);

    if (!m_bHaveOtherSrc)
    {
        m_strDataName2.Empty();
        m_nTotal2 = 0;
        m_nSrc2 = 0;
        m_listData2.DeleteAllItems();
    }
    else
    {
        m_nSrc1 = 0;
    }

    GetDlgItem(IDC_LISTDATA2)->EnableWindow(m_bHaveOtherSrc);
    GetDlgItem(IDC_BTNOPENDATA2)->EnableWindow(m_bHaveOtherSrc);
    GetDlgItem(IDC_EDITSRC)->EnableWindow(!m_bHaveOtherSrc);
    GetDlgItem(IDC_EDITSRC2)->EnableWindow(m_bHaveOtherSrc && g_tReadData2.nTotal > 0);

    UpdateData(FALSE);
}

void CDataModDlg::OnBtnopendata2()
{
    int     i = 0;
    UINT    nMaxShowPlace = 0;
    int     iShowListNum = 0;
    CString strHead, strTemp;
    string  strName;
    TorqData::Torque* ptTorq = NULL;
    CString strFilter;

    strFilter.Format(IDS_STRDATFILTER);

    CFileDialog fileDlg(TRUE, "pbd", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, NULL);

    COMP_BNE(fileDlg.DoModal(), IDOK);

    m_strDataName2 = fileDlg.GetPathName();
    theApp.ReadHisTorqFromFile(m_strDataName2.GetBuffer(0), &g_tReadData2);
    m_nTotal2 = g_tReadData2.nTotal;
    m_nSrc2 = g_tReadData2.nTotal == 0 ? 0 : 1;
    GetDlgItem(IDC_EDITSRC2)->EnableWindow(m_bHaveOtherSrc && g_tReadData2.nTotal > 0);

    if (g_tReadData2.nTotal <= 0)
    {
        UpdateData(FALSE);
        return;
    }

    m_listData2.DeleteAllItems();

    /* update list head */
    strHead = m_strFixHead.c_str();
    nMaxShowPlace = 0;
    for (i = 0; i < (int)g_tReadData2.nTotal; i++)
    {
        ptTorq = &g_tReadData2.tData[i];
        if (ptTorq->tshow_size() > iShowListNum)
        {
            nMaxShowPlace = i;
            iShowListNum = ptTorq->tshow_size();
        }
    }
    ptTorq = &g_tReadData2.tData[nMaxShowPlace];
    iShowListNum = MIN(iShowListNum, MAXPARANUM);
    m_listData2.nShowNum = iShowListNum;
    for (i = 0; i < iShowListNum; i++)
    {
        strName = theApp.GetTorqShowName(ptTorq, i);
        if (strName.empty())
            strName = _T("NULL");
        strTemp.Format("%s, %d;", strName.c_str(), m_iWidth);
        //strTemp.Format("%s, %d;", ptTorq->tshow(i).strname().c_str(), m_iWidth);
        strHead += strTemp;
    }

    m_listData2.SetHeadings(strHead);
    m_listData2.LoadColumnInfo();

    ShowHisTorqList(false);

    UpdateData(FALSE);
}

CString CDataModDlg::GetFileName(CString path)
{
    return path.Mid(path.ReverseFind('\\') + 1);
}

void CDataModDlg::OnBnClickedBtnmerge()
{
    UINT i = 0, j = 0, nTotal = 0;
    CString strInfo;
    int     iLeft = 0;
    UINT    nItemNum = 0;
    UINT    nCurPos = 0;
    UINT    nLeng = 0;
    UINT    nDataLen = 0;
    TorqData::Torque* ptTorq = NULL;
    CFile   file;

    //if (m_strDataName.IsEmpty() || m_strDataName2.IsEmpty())
    //{
    //    AfxMessageBox("Invalid merge file, pls re-input!");
    //    return;
    //}

    //if (g_tReadData.nTotal == 0 || g_tReadData2.nTotal == 0)
    //{
    //    AfxMessageBox("Invalid merge file, pls re-input!");
    //    return;
    //}

    ASSERT_ZERO(file.Open(m_strDataName, CFile::modeReadWrite | CFile::shareDenyNone));

    nTotal = g_tReadData.nTotal + g_tReadData2.nTotal;
    if (nTotal > MAXWELLNUM)
    {
        AfxMessageBox("The number of total data(%d) exceeds %d and cannot be merged!", g_tReadData.nTotal + g_tReadData2.nTotal, MAXWELLNUM);
        return;
    }

    /*最后更新记录数*/
    file.Seek(sizeof(UINT), CFile::begin);
    nTotal = 0;
    uint64_t mu1, bo1, mu2, bo2;
    while (i < g_tReadData.nTotal && j < g_tReadData2.nTotal)
    {
        mu1 = g_tReadData.tData[i].mucoltime();
        bo1 = g_tReadData.tData[i].bocoltime();
        if (mu1 == 0)
            mu1 = bo1;
        mu2 = g_tReadData2.tData[j].mucoltime();
        bo2 = g_tReadData2.tData[j].bocoltime();
        if (mu2 == 0)
            mu2 = bo2;
        // 1 和 2 重复
        if (mu1 == mu2)
        {
            ptTorq = &g_tReadData.tData[i];
            i++, j++;
        }
        else if (mu1 < mu2)
        {
            ptTorq = &g_tReadData.tData[i];
            i++;
        }
        else
        {
            ptTorq = &g_tReadData2.tData[j];
            j++;
        }

        nDataLen = theApp.SerialTorq(ptTorq);

        if (nDataLen == 0 || nDataLen >= MAXPROBUFF)
            continue;

        file.Write(&theApp.m_nPBHead, PBHEADLEN);
        file.Write(&nDataLen, sizeof(UINT));
        file.Write(theApp.m_cProtoBuf, nDataLen);

        nTotal++;
    }
    while (i < g_tReadData.nTotal)
    {
        ptTorq = &g_tReadData.tData[i];
        i++;

        nDataLen = theApp.SerialTorq(ptTorq);

        if (nDataLen == 0 || nDataLen >= MAXPROBUFF)
            continue;

        file.Write(&theApp.m_nPBHead, PBHEADLEN);
        file.Write(&nDataLen, sizeof(UINT));
        file.Write(theApp.m_cProtoBuf, nDataLen);

        nTotal++;
    }
    while (j < g_tReadData2.nTotal)
    {
        ptTorq = &g_tReadData2.tData[j];
        j++;

        nDataLen = theApp.SerialTorq(ptTorq);

        if (nDataLen == 0 || nDataLen >= MAXPROBUFF)
            continue;

        file.Write(&theApp.m_nPBHead, PBHEADLEN);
        file.Write(&nDataLen, sizeof(UINT));
        file.Write(theApp.m_cProtoBuf, nDataLen);

        nTotal++;
    }

    file.SeekToBegin();
    file.Write(&nTotal, sizeof(UINT));

    file.Close();

    ReReadTorqFile();
    UpdateData(FALSE);

    strInfo.Format("'%s' merged into '%s' successfully!", GetFileName(m_strDataName2), GetFileName(m_strDataName));
    AfxMessageBox(strInfo, MB_OK | MB_ICONINFORMATION);
    return;
}
