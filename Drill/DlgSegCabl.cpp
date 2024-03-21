// CDlgSegCabl.cpp: implementation file
//

#include "stdafx.h"
#include "Drill.h"
#include "DlgSegCabl.h"
#include "DrillDlg.h"
#include "afxdialogex.h"

//CDrillDlg* g_ptCalParentDlg = NULL;
// CDlgSegCabl dialog

IMPLEMENT_DYNAMIC(CDlgSegCabl, CDialogEx)

CDlgSegCabl::CDlgSegCabl(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DLGSEGCALIB, pParent)
    , m_nCalibTorq(1)
    , m_nSCMTorq(0)
    , m_nLowTorq(0)
    , m_nHighTorq(1)
    , m_iType(0)
{
    m_ucCurSeg = 0;
}

CDlgSegCabl::~CDlgSegCabl()
{
}

BOOL CDlgSegCabl::OnInitDialog()
{
    int         i = 0;
    //string      strInfo;
    string      strHead;
    CRect       rcView;
    int         iWidth = 0;
    //char buffer[MAX_LOADSTRING];

    CDialogEx::OnInitDialog();

    memset(&theApp.m_tCalibCtrl, 0, sizeof(CALIBCTRL));
    memset(&m_tCalibInfo[0], 0, MAXSEGNUM * sizeof(CALIBINFO));

    m_cbSegment.Clear();

    for (i = 0; i < MAXSEGNUM; i++)
    {
        //strInfo.Format("%d", i+1);
        m_cbSegment.AddString(to_string(i + 1).c_str());
    }
    m_cbSegment.SetCurSel(0);

    m_lsCalibInfo.GetWindowRect(&rcView);

    iWidth = (int)(rcView.Width() / 4.2);
    m_lsCalibInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT);
    // "类型,%d;段号,%d;校准扭矩,%d;A/D扭矩,%d;最小扭矩,%d;最大扭矩,%d;"
    //strHead.Format(IDS_STRCALIBLISTHEAD,int(iWidth*0.4),int(iWidth*0.4),int(iWidth*0.8),int(iWidth*0.8),int(iWidth*0.8),int(iWidth*0.8));
    /*snprintf(buffer, MAX_LOADSTRING, theApp.LoadstringFromRes(IDS_STRCALIBLISTHEAD).c_str(),
        int(iWidth * 0.4), int(iWidth * 0.4), int(iWidth * 0.8), int(iWidth * 0.8), int(iWidth * 0.8), int(iWidth * 0.8));*/
    strHead = string_format(theApp.LoadstringFromRes(IDS_STRCALIBLISTHEAD).c_str(),
        int(iWidth * 0.4), int(iWidth * 0.4), int(iWidth * 0.8), int(iWidth * 0.8), int(iWidth * 0.8), int(iWidth * 0.8));
    m_lsCalibInfo.SetHeadings(strHead.c_str());
    m_lsCalibInfo.LoadColumnInfo();

    //g_ptCalParentDlg = (CDrillDlg*)GetParent();

    //InitTestData();
    RegisterHotKey(GetSafeHwnd(), 1, (MOD_CONTROL | MOD_ALT), UINT('I'));

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CDlgSegCabl::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDITCALIBTORQ, m_nCalibTorq);
    DDX_Text(pDX, IDC_EDITSCMTORQ, m_nSCMTorq);
    DDX_Text(pDX, IDC_EDITLOWTORQ, m_nLowTorq);
    DDX_Text(pDX, IDC_EDITHIGHTORQ, m_nHighTorq);
    DDX_Control(pDX, IDC_LISTCALIBINFO, m_lsCalibInfo);
    DDX_Control(pDX, IDC_CBSEGMENT, m_cbSegment);
    DDX_Radio(pDX, IDC_RADIOCABMAKEUP, m_iType);
}

BEGIN_MESSAGE_MAP(CDlgSegCabl, CDialogEx)
    ON_BN_CLICKED(IDC_BTNCALIBBEGIN, &CDlgSegCabl::OnBnClickedBtncalibbegin)
    ON_BN_CLICKED(IDC_BTNCALIBGO, &CDlgSegCabl::OnBnClickedBtncalibgo)
    ON_BN_CLICKED(IDC_BTNCALIBREAD, &CDlgSegCabl::OnBnClickedBtncalibread)
    ON_BN_CLICKED(IDC_BTNCALIBFIN, &CDlgSegCabl::OnBnClickedBtncalibfin)
    ON_BN_CLICKED(IDC_BTNCALIBEXP, &CDlgSegCabl::OnBnClickedBtncalibexp)
    ON_MESSAGE(WM_CALIB_DATA, ProcReadCalib)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BTNCALIBIMP, &CDlgSegCabl::OnBnClickedBtncalibimp)
    ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CDlgSegCabl::PreTranslateMessage(MSG* pMsg)
{
    ASSERT_NULL_R(pMsg, FALSE);

    if (pMsg->message == WM_HOTKEY)
    {
        if (pMsg->wParam == 1 &&
            LOWORD(pMsg->lParam) == (MOD_CONTROL | MOD_ALT))
        {
            if (HIWORD(pMsg->lParam) == UINT('I'))
            {
                OnBnClickedBtncalibimp();
                return TRUE;
            }
        }
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgSegCabl::InitTestData()
{
    int         i = 0;
    CALIBINFO* ptCurCabli = NULL;

    for (i = 1; i <= MAXSEGNUM; i++)
    {
        ptCurCabli = &m_tCalibInfo[i - 1];
        ptCurCabli->ucSegNO = i;
        ptCurCabli->iCalibTorq = 500 * i;
        ptCurCabli->iSCMTorq = 497 * i;
        ptCurCabli->iLowTorq = 500 * (i - 1);
        ptCurCabli->iHighTorq = 500 * (i + 1) - 1;
    }
    UpdateCalibList();
}

// CDlgSegCabl message handlers
void CDlgSegCabl::OnDestroy()
{
    CDialogEx::OnDestroy();

    UnregisterHotKey(GetSafeHwnd(), 1);
    m_lsCalibInfo.DeleteAllItems();
}

BOOL CDlgSegCabl::CheckCalib(CALIBINFO* ptInfo)
{
    ASSERT_NULL_R(ptInfo, 0);
    COMP_BLE_R(ptInfo->ucSegNO, 0, FALSE);
    COMP_BLE_R(ptInfo->iCalibTorq, 0, FALSE);
    COMP_BL_R(ptInfo->iSCMTorq, 0, FALSE);
    COMP_BL_R(ptInfo->iLowTorq, 0, FALSE);
    COMP_BLE_R(ptInfo->iHighTorq, 0, FALSE);
    COMP_BLE_R(ptInfo->iHighTorq, ptInfo->iLowTorq, FALSE);
    //COMP_BL_R(ptInfo->iCalibTorq, ptInfo->iLowTorq, FALSE);
    //COMP_BG_R(ptInfo->iCalibTorq, ptInfo->iHighTorq, FALSE);

    return TRUE;
}

BOOL CDlgSegCabl::CheckTorqValue()
{
    ASSERT_ZERO_R(m_nCalibTorq, FALSE);
    ASSERT_ZERO_R(m_nHighTorq, FALSE);
    COMP_BLE_R(m_nHighTorq, m_nLowTorq, FALSE);
    //COMP_BL_R(m_nCalibTorq, m_nLowTorq, FALSE);
    //COMP_BG_R(m_nCalibTorq, m_nHighTorq, FALSE);
    COMP_BG_R(m_ucCurSeg, MAXSEGNUM, FALSE);

    return TRUE;
}

void CDlgSegCabl::GetCalibInfo()
{
    BYTE        ucSegNO = 0;
    CALIBINFO* ptCabli = NULL;
    CALIBINFO* ptGlab = NULL;

    COMP_BG(m_ucCurSeg, MAXSEGNUM);

    ptCabli = &m_tCalibInfo[m_ucCurSeg - 1];
    ptGlab = &theApp.m_tCalibCtrl.tInfo;

    ptGlab->ucSegNO = ptCabli->ucSegNO = m_ucCurSeg;
    ptGlab->iCalibTorq = ptCabli->iCalibTorq = m_nCalibTorq;
    ptGlab->iSCMTorq = ptCabli->iSCMTorq = m_nSCMTorq;
    ptGlab->iHighTorq = ptCabli->iHighTorq = m_nHighTorq;
    ptGlab->iLowTorq = ptCabli->iLowTorq = m_nLowTorq;
}

void CDlgSegCabl::UpdateCalibList()
{
    int         i = 0;
    CALIBINFO* ptCurCabli = NULL;
    CString     strType;
    CString     strNO;
    CString     strCalib;
    CString     strSCM;
    CString     strMin;
    CString     strMax;

    m_lsCalibInfo.DeleteAllItems();
    m_lsCalibInfo.SetRedraw(0);

#if 0
    if (m_iType == 0)
        strType.Format("%d", m_iType);
    else
        strType.Format(IDS_STRBREAKOUT);
#else
    strType.Format("%d", m_iType);
#endif
    for (i = 0; i < MAXSEGNUM; i++)
    {
        ptCurCabli = &m_tCalibInfo[i];
        if (ptCurCabli->ucSegNO <= 0)
            continue;

        strNO.Format("%d", ptCurCabli->ucSegNO);
        strCalib.Format("%d", ptCurCabli->iCalibTorq);
        strSCM.Format("%d", ptCurCabli->iSCMTorq);
        strMin.Format("%d", ptCurCabli->iLowTorq);
        strMax.Format("%d", ptCurCabli->iHighTorq);

        // "类型,%d;段号,%d;校准扭矩,%d;A/D扭矩,%d;最小扭矩,%d;最大扭矩,%d;"
        m_lsCalibInfo.AddItem(strType, strNO, strCalib, strSCM, strMin, strMax);
    }

    m_lsCalibInfo.SetRedraw(1);

    GetDlgItem(IDC_BTNCALIBEXP)->EnableWindow(m_lsCalibInfo.GetItemCount() > 0);

    UpdateData(FALSE);
}

void CDlgSegCabl::EnableCtrl(BOOL bBegin)
{
    GetDlgItem(IDC_BTNCALIBBEGIN)->EnableWindow(!bBegin);
    GetDlgItem(IDC_RADIOCABMAKEUP)->EnableWindow(!bBegin);
    GetDlgItem(IDC_RADIOCABBREAKOUT)->EnableWindow(!bBegin);

    GetDlgItem(IDC_BTNCALIBGO)->EnableWindow(bBegin);
    GetDlgItem(IDC_BTNCALIBREAD)->EnableWindow(bBegin);
    GetDlgItem(IDC_BTNCALIBFIN)->EnableWindow(bBegin);
}

void CDlgSegCabl::OnBnClickedBtncalibbegin()
{
    //ASSERT_NULL(g_ptCalParentDlg);
    UpdateData(TRUE);

    m_lsCalibInfo.DeleteAllItems();
    memset(&m_tCalibInfo[0], 0, MAXSEGNUM * sizeof(CALIBINFO));

    theApp.m_tCalibCtrl.ucProc = 0x00;
    theApp.m_tCalibCtrl.ucType = m_iType;

    if (!thepDlg->SendData(SCMCTRLCALIB))
    {
        AfxMessageBox(theApp.LoadstringFromRes(IDS_STRCALIBSENDFAIL).c_str());
        return;
    }

    EnableCtrl(TRUE);
}

void CDlgSegCabl::OnBnClickedBtncalibfin()
{
    //ASSERT_NULL(g_ptCalParentDlg);
    UpdateData(TRUE);

    UpdateCalibList();

    theApp.m_tCalibCtrl.ucProc = 0x01;
    theApp.m_tCalibCtrl.ucType = m_iType;

    if (!thepDlg->SendData(SCMCTRLCALIB))
    {
        AfxMessageBox(theApp.LoadstringFromRes(IDS_STRCALIBSENDFAIL).c_str());
        return;
    }

    EnableCtrl(FALSE);
}

void CDlgSegCabl::OnBnClickedBtncalibgo()
{
    //ASSERT_NULL(g_ptCalParentDlg);
    UpdateData(TRUE);
    m_ucCurSeg = m_cbSegment.GetCurSel() + 1;

    if (!CheckTorqValue())
    {
        AfxMessageBox(theApp.LoadstringFromRes(IDS_STRCALIBERRORINPUT).c_str());
        return;
    }

    GetCalibInfo();

    UpdateCalibList();

    /* 20201119: 校准时A/D */
    theApp.m_tCalibCtrl.tInfo.iSCMTorq = 0;
    if (!thepDlg->SendData(SCMWRITECALIB))
    {
        AfxMessageBox(theApp.LoadstringFromRes(IDS_STRCALIBSENDFAIL).c_str());
    }
}

void CDlgSegCabl::OnBnClickedBtncalibread()
{
    //ASSERT_NULL(g_ptCalParentDlg);
    UpdateData(TRUE);
    m_ucCurSeg = m_cbSegment.GetCurSel() + 1;

    GetCalibInfo();

    if (!thepDlg->SendData(SCMREADCALIB))
    {
        AfxMessageBox(theApp.LoadstringFromRes(IDS_STRCALIBSENDFAIL).c_str());
    }
    Sleep(200);
#if 0
    BYTE* pByte = &thepDlg->m_ucRcvByte[3];
    *pByte++ = 0x19;
    *pByte++ = 0x00;
    *pByte++ = 0x02;
    *pByte++ = 0x07;
    *pByte++ = 0xd0;
    *pByte++ = 0x00;
    *pByte++ = 0x06;
    *pByte++ = 0x77;
    *pByte++ = 0x08;


    ProcReadCalib(WM_CALIB_DATA, 1);
#endif
}

BOOL CDlgSegCabl::CheckReadCalib(CALIBCTRL* ptRead, string& strError)
{
    stringstream ss;
    ASSERT_NULL_R(ptRead, FALSE);

    if (ptRead->ucType != m_iType)
    {
        ss << "CurType:" << m_iType << "RcvType:" << ptRead->ucType;
        strError = ss.str();
        //strError.Format("CurType:%d, RcvType:%d", m_iType, ptRead->ucType);
        return FALSE;
    }

    if (ptRead->tInfo.ucSegNO != m_ucCurSeg)
    {
        ss << "CurSeg:" << m_ucCurSeg << "RcvSeg:" << ptRead->tInfo.ucSegNO;
        strError = ss.str();
        //strError.Format("CurSeg:%d, RcvSeg:%d", m_ucCurSeg, ptRead->tInfo.ucSegNO);
        return FALSE;
    }

    return TRUE;
}

LRESULT CDlgSegCabl::ProcReadCalib(WPARAM wParam, LPARAM lParam)
{
    BYTE* pucPos = NULL;
    CALIBCTRL tRead = { 0 };
    UINT    nCalibTorq = 0;
    UINT    nSCMTorq = 0;
    string  strError;
    string  strInfo;
    CALIBINFO* ptCurCabli = NULL;

    //ASSERT_NULL_R(g_ptCalParentDlg, 0);
    UpdateData(TRUE);

    pucPos = &thepDlg->m_ucRcvByte[4];
    tRead.ucType = (*pucPos++);
    tRead.tInfo.ucSegNO = (*pucPos++);
    nCalibTorq = (*pucPos++);
    nCalibTorq = nCalibTorq * 256 + *(pucPos++);
    pucPos++;
    nSCMTorq = (*pucPos++);
    nSCMTorq = nSCMTorq * 256 + *(pucPos++);

    m_ucCurSeg = m_cbSegment.GetCurSel() + 1;

    if (!CheckReadCalib(&tRead, strError))
    {
        //strInfo.Format(IDS_STRCALIBREADERROR, strError);
        //AfxMessageBox(strInfo);
        strInfo = string_format(theApp.LoadstringFromRes(IDS_STRCALIBREADERROR).c_str(), strError.c_str());
        AfxMessageBox(strInfo.c_str());
        return 1;
    }

    ptCurCabli = &m_tCalibInfo[m_ucCurSeg - 1];
    ptCurCabli->iCalibTorq = m_nCalibTorq = nCalibTorq;
    ptCurCabli->iSCMTorq = m_nSCMTorq = nSCMTorq;

    UpdateCalibList();
    UpdateData(FALSE);

    return 0;
}

void CDlgSegCabl::OnBnClickedBtncalibexp()
{
    BeginWaitCursor();
    theApp.SaveList2XlsFile(NULLSTR, &m_lsCalibInfo);
    EndWaitCursor();
}

BOOL CDlgSegCabl::GetCalibInfoFromExcel(CString strName)
{
    int         i = 0, j = 0;
    int         iRow = 0, iCol = 0;
    int         iSegNO = 0;
    int         iFirstType = -1;
    int         iCurType = -1;
    CALIBINFO   tCalib[MAXSEGNUM];
    CALIBINFO* ptCur = NULL;

    COMP_BFALSE_R(m_impExc.open(strName), FALSE);
    COMP_BFALSE_R(m_impExc.loadSheet(1), FALSE);

    iRow = m_impExc.getRowCount();//获取sheet中行数
    iCol = m_impExc.getColumnCount();//获取sheet中列数
    if (iCol < 6 || iRow < 2)
    {
        return FALSE;
    }

    ptCur = &tCalib[0];
    iRow = MIN(iRow, (MAXSEGNUM + 1));
    for (i = 2; i <= iRow; i++)
    {
        // 1 type
        iCurType = atoi(m_impExc.getCellString(i, 1));
        if (iCurType != 0 && iCurType != 1)
            return FALSE;

        if (-1 == iFirstType)
            iFirstType = iCurType;
        else if (iCurType != iFirstType)
            return FALSE;

        // 2 segno
        iSegNO = atoi(m_impExc.getCellString(i, 2));
        if (iSegNO <= 0)
            return FALSE;
        ptCur->ucSegNO = iSegNO;
        // 3 CalibTorq
        ptCur->iCalibTorq = atoi(m_impExc.getCellString(i, 3));
        // 4 A/D Torq
        ptCur->iSCMTorq = atoi(m_impExc.getCellString(i, 4));
        // 5 Min Torq
        ptCur->iLowTorq = atoi(m_impExc.getCellString(i, 5));
        // 6 Max Torq
        ptCur->iHighTorq = atoi(m_impExc.getCellString(i, 6));

        if (!CheckCalib(ptCur))
            return FALSE;

        ptCur++;
    }

    m_iType = iFirstType;
    m_ucCurSeg = 1;
    m_cbSegment.SetCurSel(0);
    memset(&m_tCalibInfo[0], 0, sizeof(CALIBINFO) * MAXSEGNUM);
    memcpy(&m_tCalibInfo[0], &tCalib[0], sizeof(CALIBINFO) * (iRow - 1));
    //UpdateCalibList();

    return TRUE;
}

void CDlgSegCabl::OnBnClickedBtncalibimp()
{
    CString     strFilter;
    CString     stImpFile;
    CString     strInfo;

    strFilter.Format(IDS_STRXLSFILTER);

    CFileDialog fileDlg(TRUE, "xls", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, NULL);

    COMP_BNE(fileDlg.DoModal(), IDOK);
    stImpFile = fileDlg.GetPathName();

    BeginWaitCursor();
    if (!m_impExc.initExcel())
    {
        EndWaitCursor();
        strInfo.Format(IDS_STRINFNODRIVE);
        theApp.SaveShowMessage(strInfo.GetBuffer(0));
        return;
    }
    if (!GetCalibInfoFromExcel(stImpFile))
    {
        m_impExc.close();
        m_impExc.release();
        EndWaitCursor();
        strInfo.Format(IDS_STRCALIBFORMARTERR, stImpFile);
        theApp.SaveShowMessage(strInfo.GetBuffer(0));
        return;
    }
    m_impExc.close();
    m_impExc.release();

    SetTimer(SETCALIB_TIMER, SETCALIB_LEN, NULL);

    EndWaitCursor();
}

// 定时发送设置消息
void CDlgSegCabl::OnTimer(UINT_PTR nIDEvent)
{
    int     i = 0;
    CALIBINFO* ptCabli = NULL;
    CALIBINFO* ptGlab = NULL;

    COMP_BNE(nIDEvent, SETCALIB_TIMER);

    //if (NULL == g_ptCalParentDlg)
    {
        KillTimer(SETCALIB_TIMER);
        return;
    }

    COMP_BG(m_ucCurSeg, MAXSEGNUM);

    ptGlab = &theApp.m_tCalibCtrl.tInfo;
    for (i = m_ucCurSeg - 1; i < MAXSEGNUM; i++)
    {
        ptCabli = &m_tCalibInfo[m_ucCurSeg - 1];
        if (0 == ptCabli->ucSegNO)
        {
            m_ucCurSeg++;
            continue;
        }
        ptGlab->ucSegNO = ptCabli->ucSegNO;
        ptGlab->iCalibTorq = ptCabli->iCalibTorq;
        ptGlab->iSCMTorq = ptCabli->iSCMTorq;
        ptGlab->iHighTorq = ptCabli->iHighTorq;
        ptGlab->iLowTorq = ptCabli->iLowTorq;

        if (!thepDlg->SendData(SCMWRITECALIB))
        {
            KillTimer(SETCALIB_TIMER);
            AfxMessageBox(theApp.LoadstringFromRes(IDS_STRCALIBSENDFAIL).c_str());
            return;
        }
    }

    m_ucCurSeg++;

    if (m_ucCurSeg > MAXSEGNUM)
    {
        UpdateCalibList();
        KillTimer(SETCALIB_TIMER);
        AfxMessageBox(theApp.LoadstringFromRes(IDS_STRCALIBIMPSUCC).c_str());
    }

    CDialogEx::OnTimer(nIDEvent);
}
