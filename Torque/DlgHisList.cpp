// DlgHisList.cpp : implementation file
//

#include "stdafx.h"
#include "torque.h"
#include "DlgHisList.h"
#include "DlgHisGrp.h"
#include "DlgXlsStatSet.h"
/* for excel */
/*#include "CApplication.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "CChart.h"
#include "CChartObjects.h"
#include "CChartObject.h"
#include "CShapes.h"
#include "CShape.h"
#include "CFont0.h"*/

#include "DlgScatter.h"
#include "DlgDataStat.h"

#define SHEET_COVER     _T("Cover")
#define SHEET_SUMMARY   _T("Page1")
#define SHEET_QUALITY   _T("Page2")
#define SHEET_SCATTER   _T("Page3")
#define SHEET_REPORT    _T("Page4")

#define TEMPLATE_RPTCHN _T("RptTmpChn.xlsx")
#define TEMPLATE_RPTENG _T("RptTmpEng.xlsx")
#define TEMPLATE_GRAPHY _T("GrpTmp.xlsx")

#define SHOWPARA_WELLNAME       0
#define SHOWPARA_COMPANY        2
#define SHOWPARA_TUBINGOEM      3
#define SHOWPARA_TUBINGTYPE     4

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define HAVINGPRNIMG        // 打印图像时便于测试，不用每次都打印

/////////////////////////////////////////////////////////////////////////////
// CDlgHisList property page
extern TORQUEDATA  m_tReadData;
SAVELOGDATA g_tOrgData;         /* 存储扭矩原始数据暂时内存 */
//CWorksheet  *g_ptCurSheet = NULL;

#define     MAXIMGINFO          17
#define     IMG_HOSTNO          0
#define     IMG_ENDTIME         1
#define     IMG_CTRLTORQ        2
#define     IMG_CTRLTURN        3
#define     IMG_SHLDTORQ        4
#define     IMG_DELTATORQ       5
#define     IMG_DELTATURN       6
#define     IMG_SLOPEFAC        7
#define     IMG_QUALITY         8
#define     IMG_COMMENT         9
#define     IMG_BHA             10
#define     IMG_BREAKOUT        11
#define     IMG_YES             12
#define     IMG_NO              13
#define     IMG_NM              14
#define     IMG_LBFT            15
#define     IMG_TURN            16
SHOWPARANAME g_tImgInfo[MAXIMGINFO] = {
    /* 0  */    { 0,    "电脑编号",     "Host No.",         "Номер ком."},
    /* 1  */    { 1,    "完成时间",     "End Time",         "Вр. св."},
    /* 2  */    { 2,    "实际扭矩",     "Ctrl Torq",        "Фак.кру.мон."},
    /* 3  */    { 3,    "实际周数",     "Ctrl Turn",        "Ч. об. св."},
    /* 4  */    { 4,    "拐点扭矩",     "ShoulderTorq",     "плечо мом."},
    /* 5  */    { 5,    "扭矩差值",     "DeltaTorq",        "дельта мом."},
    /* 6  */    { 6,    "周数差值",     "DeltaTurn",        "Дельта-поворот"},
    /* 7  */    { 7,    "斜坡因子",     "SlopeFac",         "фактор скат"},
    /* 8  */    { 8,    "质量说明",     "Quality",          "Оц. Кач-ва"},
    /* 9  */    { 9,    "备    注",     "Comment",          "Приме."},
    /* 10 */    {10,    "工具扣",       "BHA",              "пряжка"},
    /* 11 */    {11,    "卸扣",         "BreakOut",         "мочка"},
    /* 12 */    {12,    "是",           "Yes",              "да"},
    /* 13 */    {13,    "否",           "No",               "нет"},
    /* 14 */    {14,    "N.m",          "N.m",              "N.m"},
    /* 15 */    {15,    "lb.ft",        "lb.ft",            "lb.ft"},
    /* 16 */    {16,    "周",           "Turn",             "об."}
};

#if 0
COleVariant
    covTrue((short)TRUE),
    covFalse((short)FALSE),
    covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
#endif
CDlgHisGrp      *pdlgPrint = NULL;
//CWorkbook       g_GlbBook;
IMPLEMENT_DYNCREATE(CDlgHisList, CPropertyPage)

CDlgHisList::CDlgHisList() : CPropertyPage(CDlgHisList::IDD)
{
    //{{AFX_DATA_INIT(CDlgHisList)
    m_strHisName = _T("");
    m_bExpAllImg = TRUE;
    m_nHisTotalRec = 0;
    m_nHisQualyRec = 0;
    m_nHisUnQualyRec = 0;
    m_ptStatTorq     = NULL;
    //}}AFX_DATA_INIT
}

CDlgHisList::~CDlgHisList()
{
}

void CDlgHisList::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgHisList)
    DDX_Control(pDX, IDC_LISTHIST, m_listHis);
    DDX_Check(pDX, IDC_CHECKALLIMG, m_bExpAllImg);
    DDX_Text(pDX, IDC_EDITHISTORY, m_strHisName);
    DDX_Text(pDX, IDC_EDTOTAL, m_nHisTotalRec);
    DDX_Text(pDX, IDC_EDQUALIFY, m_nHisQualyRec);
    DDX_Text(pDX, IDC_EDUNQUALIFY, m_nHisUnQualyRec);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHisList, CPropertyPage)
    //{{AFX_MSG_MAP(CDlgHisList)
    ON_BN_CLICKED(IDC_BTNHIS, &CDlgHisList::OnBtnhis)
    ON_BN_CLICKED(IDC_BTNEXPORT, &CDlgHisList::OnBnClickedBtnexport)
    ON_BN_CLICKED(IDC_BTNIMGEXP, &CDlgHisList::OnBnClickedBtnimgexp)
    ON_BN_CLICKED(IDC_BTNORGDATA, &CDlgHisList::OnBnClickedBtnOrgdata)
    ON_BN_CLICKED(IDC_BTNGRAPHEXP, &CDlgHisList::OnBnClickedBtngraphexp)
    ON_BN_CLICKED(IDC_BTNSTATLIST, &CDlgHisList::OnBnClickedBtnstatlist)
    ON_BN_CLICKED(IDC_BTNSTATSET, &CDlgHisList::OnBnClickedBtnstatset)
    ON_WM_DESTROY()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHisList message handlers

BOOL CDlgHisList::OnInitDialog()
{
    WORD    i        = 0;
    CString strHead;
    CString strTemp;
    CRect   rcView;

    CPropertyPage::OnInitDialog();

    theApp.AdaptDlgCtrlSize(this,2);

    m_nCurLang = theApp.m_nLangType;

    m_listHis.GetWindowRect(&rcView);
    m_iWidth = (int)(rcView.Width()/12.17);
    m_listHis.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_UNDERLINEHOT);
    m_strFixHead.Format(IDS_STRHISLLISTHEAD,
                   int(0.8*m_iWidth),int(1.7*m_iWidth),int(0.9*m_iWidth),int(0.9*m_iWidth),int(0.9*m_iWidth),
                   int(0.9*m_iWidth),int(0.9*m_iWidth),int(0.9*m_iWidth),int(0.9*m_iWidth),int(0.9*m_iWidth),
                   int(2*m_iWidth));
    strHead = m_strFixHead;
    m_wShowListNum = 0;
    for(i=0; i<theApp.m_ptCurShow->wListNum && i<MAXPARANUM; i++)
    {
        strTemp.Format("%s, %d;", theApp.m_ptCurShow->strList[i].c_str(), m_iWidth);
        strHead += strTemp;
        m_wShowListNum++;
    }
    m_listHis.SetHeadings(strHead);
    m_listHis.LoadColumnInfo();
    
    //GetDlgItem(IDC_BTNORGDATA)->ShowWindow(TRUE);
    //GetDlgItem(IDC_BTNSTATSET)->ShowWindow(TRUE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDlgHisList::GetPrintDlg()
{
    CPropertySheet *pSheet;

    if (pdlgPrint != NULL && pdlgPrint->m_hWnd != NULL)
    {
        return;
    }

    pSheet = (CPropertySheet*)GetOwner();
    pSheet->SetActivePage(1);
    pdlgPrint = (CDlgHisGrp *)(pSheet->GetPage(1));
    pSheet->SetActivePage(0);
    return;
}

void CDlgHisList::OnBtnhis()
{
    UINT    i = 0;
    UINT    nMaxShowPlace = 0;
    CString strFilter;
    CString strInfo;
    CString strHead, strTemp;
    CString strName;
    TorqData::Torque  *ptTorq = NULL;
    
    strFilter.Format(IDS_STRDATFILTER);

    CFileDialog fileDlg(TRUE,"pbd",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,strFilter,NULL);

    COMP_BNE(fileDlg.DoModal(), IDOK);

    m_strHisName = fileDlg.GetPathName();

    if(!theApp.ReadHisTorqFromFile(m_strHisName))
    {
        strInfo.Format(IDS_STRINFFILEERR);
        theApp.SaveShowMessage(strInfo);
        return;
    }

    m_nHisTotalRec   = m_tReadData.nTotal;
    m_nHisQualyRec   = m_tReadData.nQualy;
    m_nHisUnQualyRec = m_tReadData.nUnQualy;

    SetDataPlace(1);
    
    m_listHis.DeleteAllItems();

    /* update list head */
    strHead = m_strFixHead;
    nMaxShowPlace = 0;
    ptTorq = &m_tReadData.tData[0];
    m_wShowListNum = ptTorq->tshow_size();

    for(i=1; i<m_tReadData.nTotal; i++)
    {
        ptTorq = &m_tReadData.tData[i];
        if(ptTorq->tshow_size() > m_wShowListNum)
        {
            nMaxShowPlace = i;
            m_wShowListNum = ptTorq->tshow_size();
        }
    }
    ptTorq = &m_tReadData.tData[nMaxShowPlace];
    for(i=0; i<MAXPARANUM && i< m_wShowListNum; i++)
    {
        strName = theApp.GetTorqShowName(ptTorq, i);
        if (strName.IsEmpty())
            strName = _T("NULL");
        strTemp.Format("%s, %d;", strName, m_iWidth);
        strHead += strTemp;
    }                
    m_listHis.SetHeadings(strHead);
    m_listHis.LoadColumnInfo();
    
    ShowHisTorqList();

    UpdateData(FALSE);

    return;
}

BOOL CDlgHisList::OnSetActive()
{
    UINT    nCurSel = m_tReadData.nCur;
    CHECK_VALUE_LOW(nCurSel, 1);
    SendMessageToDescendants(WM_SETFONT, (WPARAM)theApp.m_tRuleHFont.GetSafeHandle(), TRUE);
    if(theApp.ReadHisTorqFromFile(m_strHisName))
    {
        m_nHisTotalRec   = m_tReadData.nTotal;
        m_nHisQualyRec   = m_tReadData.nQualy;
        m_nHisUnQualyRec = m_tReadData.nUnQualy;
        /* 20210317 从图形切回列表时，保持当前的选择记录 */
        SetDataPlace(nCurSel);
        
        m_listHis.DeleteAllItems();
        ShowHisTorqList();

        //选中时第二个参数值为1，取消选中时第二个参数为0
        m_listHis.SetItemState(nCurSel-1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
        m_listHis.EnsureVisible(nCurSel-1, TRUE);
    }

    UpdateData(FALSE);

    return CPropertyPage::OnSetActive();
}

void CDlgHisList::OnDestroy()
{
    m_listHis.DeleteAllItems();

    pdlgPrint = NULL;

    CPropertyPage::OnDestroy();
}

void CDlgHisList::OnBnClickedBtnexport()
{
    CString strList;
    CString strSheetName;

    strList = theApp.GetSaveDataPath();
    strList += theApp.m_strFileTitle.c_str();
    strList += "_List.xls";

    strSheetName.Format(IDS_STRSHEETHISDATA);
    theApp.SaveList2XlsFile(strList, strSheetName, &m_listHis);
}

/*"序号,%d;施工时间,%d;实际扭矩,%d;实际周数,%d;拐点扭矩,%d;斜坡因子,%d;
   管件名称,%d;上扣类型,%d;重量,%d;接箍规格,%d;备注,%d;"*/
/* "序号,%d;施工时间,%d;最小扭矩,%d;控制扭矩,%d;最佳扭矩,%d;最大扭矩,%d;
    实际扭矩,%d;实际周数,%d;拐点扭矩,%d;斜坡因子,%d;备注,%d;" */
VOID CDlgHisList::ShowHisTorqList()
{
    UINT        i       = 0;
    int         j       = 0;
    int         iItem   = 0;
    double      fTorque = 0;
    WORD        wPara   = 0;
    CString     strNo;
    CString     strTime;
    CString     strCir;
    CString     strMinTorq, strMaxTorq;
    CString     strCtrlTorq, strOptTorq;
    CString     strTorq;
    CString     strShoulder;
    CString     strSlope;
    CString     strShowPara[MAXPARANUM];
    double      fSlope;
    CString     strMin,strMax;
    CString     strMemo;
    WORD        wIPPos = 0;
    WORD        wSchPos= 0;
    int         iIPTorq= 0;
    CStringList slShow;
    //UINT        nCount;
    DWORD       dwQuality = 0;
    TorqData::Torque *ptTorq = NULL;

    m_ptStatTorq = NULL;

    ASSERT_ZERO(m_tReadData.nTotal);

    m_listHis.SetRedraw(0);

    m_ptStatTorq = &m_tReadData.tData[m_tReadData.nTotal - 1];
    for(i=0;i<m_tReadData.nTotal;i++)
    {
        ptTorq = theApp.GetOrgTorqFromTorq(i);
        if(NULL == ptTorq)
            continue;
        strTime = theApp.GetTorqCollTime(ptTorq);

        /* 20180102 序号删除后自动更新 */
        strNo.Format("%d",i+1);
        strMinTorq.Format("%d", (int)ptTorq->flowerlimit());
        strMaxTorq.Format("%d", (int)ptTorq->fupperlimit());
        strCtrlTorq.Format("%d", (int)ptTorq->fcontrol());
        strOptTorq.Format("%d", (int)theApp.GetOptTorq(ptTorq));
        strMemo = ptTorq->strmemo().c_str();

        strCir.Format ("%.3f", theApp.GetCir(ptTorq));

        GET_CTRL_TORQ(fTorque, ptTorq);
        strTorq.Format("%d", (int)fTorque);

        /* 获得拐点扭矩 */
        iIPTorq = theApp.GetIPTorq(ptTorq, wIPPos, wSchPos);
        if(wIPPos == 0)
        {
            strShoulder.Empty();
            strSlope.Empty();
        }
        else
        {
            strShoulder.Format("%d", iIPTorq);
            fSlope = theApp.GetFlopeFactor(ptTorq, wIPPos, iIPTorq);
            if( fSlope < 0)
                strSlope.Empty();
            else
                strSlope.Format("%.3f", fSlope);
        }

        strMin.Format("%d", (int)ptTorq->flowerlimit());
        strMax.Format("%d", (int)ptTorq->fupperlimit());

        /*"序号,%d;施工时间,%d;实际扭矩,%d;实际周数,%d;拐点扭矩,%d;斜坡因子,%d; 备注,%d;*/
        /* "序号,%d;施工时间,%d;最小扭矩,%d;最大扭矩,%d;实际扭矩,%d;实际周数,%d;拐点扭矩,%d;斜坡因子,%d;备注,%d;" */
        /* "序号,%d;施工时间,%d;最小扭矩,%d;控制扭矩,%d;最佳扭矩,%d;最大扭矩,%d;实际扭矩,%d;实际周数,%d;拐点扭矩,%d;斜坡因子,%d;备注,%d;" */
        slShow.AddTail(strNo);
        slShow.AddTail(strTime);
        slShow.AddTail(strMinTorq);
        slShow.AddTail(strCtrlTorq);
        slShow.AddTail(strOptTorq);
        slShow.AddTail(strMaxTorq);
        slShow.AddTail(strTorq);
        slShow.AddTail(strCir);
        slShow.AddTail(strShoulder);
        slShow.AddTail(strSlope);
        slShow.AddTail(strMemo);

        for(j = 0; j<ptTorq->tshow_size() && j<m_wShowListNum ; j++)
            slShow.AddTail(theApp.GetTorqShowValue(ptTorq, j));
        /* 补空 */
        for(; j<m_wShowListNum; j++)
            slShow.AddTail(NULLSTR);
        iItem    = m_listHis.AddItemList(slShow);
        slShow.RemoveAll();

        dwQuality = theApp.GetQuality(ptTorq);
        if(!(dwQuality & QUA_RESU_QUALITYBIT))
        {
            m_listHis.SetItemData(iItem,TORQ_BAD_QUALITY);
        }
        if(ptTorq->btoolbuck())
        {
            m_listHis.SetItemData(iItem,TORQ_TOOLBUCKLE);
        }
        else    // 非工具扣设置为统计序号
        {
            if(strMemo.IsEmpty())
                m_ptStatTorq = ptTorq;
        }
    }

    GetDlgItem(IDC_BTNEXPORT)->EnableWindow(m_listHis.GetItemCount() > 0);
    GetDlgItem(IDC_BTNIMGEXP)->EnableWindow(m_listHis.GetItemCount() > 0);
    GetDlgItem(IDC_BTNGRAPHEXP)->EnableWindow(m_listHis.GetItemCount() > 0);
    GetDlgItem(IDC_BTNORGDATA)->EnableWindow(m_listHis.GetItemCount() > 0);
    GetDlgItem(IDC_BTNSTATLIST)->EnableWindow(m_listHis.GetItemCount() > 0);
    GetDlgItem(IDC_BTNSTATSET)->EnableWindow(m_listHis.GetItemCount() > 0);

    m_listHis.SetRedraw(1);
}

void CDlgHisList::SetDataPlace(UINT nCur)
{
    ASSERT_ZERO(nCur);
    COMP_BG(nCur, m_tReadData.nTotal);

    m_tReadData.nCur = nCur;
}

/* 获取LIST选择的记录信息，有下面三种形式供选择
   //m_bSelItem: 对应位置为TRUE，说明记录被选择，默认为FALSE
   m_nSelItem: 记录选择的序号，最大MAXDELRECORD
   */
UINT CDlgHisList::GetSelectItem()
{
    int         iItem   = 0;
    UINT        nSelNum = 0;
    UINT        i       = 0;
    CString     strItemNo;
    POSITION    pos = m_listHis.GetFirstSelectedItemPosition();

    memset(m_nSelItem, 0, sizeof(UINT)*MAXWELLNUM);

    if(m_bExpAllImg)
    {
        nSelNum = m_listHis.GetItemCount();
        if(nSelNum >= MAXWELLNUM)
        {
            nSelNum = MAXWELLNUM;
        }
        for(i=0; i< nSelNum; i++)
            m_nSelItem[i] = i+1;
        return nSelNum;
    }

    iItem = m_listHis.GetNextSelectedItem(pos);
    COMP_BE_R(iItem, -1, 0);

    /* 收集需要处理的记录 */
    nSelNum = 0;
    while(iItem != -1)
    {
        m_nSelItem[nSelNum++] = iItem+1;

        /* 最大处理MAXWELLNUM个记录 */
        if(nSelNum >= MAXWELLNUM)
        {
            nSelNum = MAXWELLNUM;
            break;
        }
        
        iItem = m_listHis.GetNextSelectedItem(pos);
    }

    return nSelNum;
}

void CDlgHisList::OnBnClickedBtnimgexp()
{
    UINT    nSelCount = 0;
    UINT    i  = 0;

    UpdateData(TRUE);

    nSelCount = GetSelectItem();
    ASSERT_ZERO(nSelCount);

    GetPrintDlg();
    pdlgPrint->ShowWindow(SW_SHOW);
    for(i=0; i<nSelCount; i=i+theApp.m_nImgNum)
    {
        pdlgPrint->PrintOneImage(&m_nSelItem[i], 0, theApp.m_nImgNum);
    }
    
    pdlgPrint->ShowWindow(SW_HIDE);
}

void CDlgHisList::OnBnClickedBtnOrgdata()
{
    UINT        i = 0;
    int         j = 0;
    UINT        index = 0;
    int         iLen    = 0;
    WORD        wIPPos  = 0;
    WORD        wSchPos = 0;
    UINT        nIPTorq = 0;
    double      fCtrlTorq = 0;
    CFile       file;
    CString     strOrgData;
    CString     strInfo;
    CString     strTime;
    CString     strSecHead;
    char        aucHead[SPRINTFLEN+1];
    int         iHeadLen= 0;
    char        *pPrnData = NULL;
    TorqData::Torque    *ptTorq = NULL;
    UINT    nSelCount = 0;

    UpdateData(TRUE);

    nSelCount = GetSelectItem();
    ASSERT_ZERO(nSelCount);

    strOrgData  = theApp.GetSaveDataPath();
    strOrgData += theApp.m_strFileTitle.c_str();
    strOrgData += ".txt";

    if(!file.Open(strOrgData, CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone))
    {
        file.Close();
        return;
    }

    /* construct NULL head */
    ptTorq   = &m_tReadData.tData[0];
    strTime = theApp.GetTorqCollTime(ptTorq);
    nIPTorq = theApp.GetIPTorq(ptTorq, wIPPos, wSchPos);
    iHeadLen = sprintf_s(aucHead, SPRINTFLEN, "%5d; %s; %3d; %5d; %5.0f; Torq: ", 
                     (1), 
                     (LPSTR)(LPCTSTR)strTime,
                     wIPPos,
                     nIPTorq,
                     fCtrlTorq);

    memset(aucHead, 0, SPRINTFLEN+1);
    memset(aucHead, ' ', iHeadLen);
    strSecHead = " DelPlus: ";
    memcpy(&aucHead[iHeadLen - strSecHead.GetLength()], strSecHead, strSecHead.GetLength());

    for(i=0; i<nSelCount && i < m_tReadData.nTotal; i++)
    {
        memset(&g_tOrgData, 0, sizeof(SAVELOGDATA));
        pPrnData = &g_tOrgData.aucLog[0];
        if(m_nSelItem[i] == 0)
            continue;
        index = m_nSelItem[i] - 1;
        ptTorq   = &m_tReadData.tData[index];

        strTime = theApp.GetTorqCollTime(ptTorq);
        nIPTorq = theApp.GetIPTorq(ptTorq, wIPPos, wSchPos);
        GET_CTRL_TORQ(fCtrlTorq, ptTorq);

        iLen = sprintf_s(pPrnData, SPRINTFLEN, "%5d; %s; %3d; %5d; %5.0f; Torq: ", 
                         (index+1),
                         (LPSTR)(LPCTSTR)strTime,
                         wIPPos,
                         nIPTorq,
                         fCtrlTorq);
        g_tOrgData.iCur += iLen;
        pPrnData += iLen;
        for(j=0; j<ptTorq->ftorque_size(); j++)
        {
            iLen = sprintf_s(pPrnData, SPRINTFLEN, "%6.0f,", ptTorq->ftorque(j));
            g_tOrgData.iCur += iLen;
            pPrnData += iLen; 
        }

        iLen = sprintf_s(pPrnData, SPRINTFLEN, "\r\n");
        g_tOrgData.iCur += iLen;
        pPrnData += iLen; 

        /* 3.22 版本打印delplus */
        if(VERSION_RECPLUS(ptTorq))
        {
            sprintf_s(pPrnData, SPRINTFLEN, "%s", aucHead);
            g_tOrgData.iCur += iHeadLen;
            pPrnData += iHeadLen; 
            for(j=0; j<ptTorq->ftorque_size(); j++)
            {
                iLen = sprintf_s(pPrnData, SPRINTFLEN, "%6d,", ptTorq->dwdelplus(j));
                g_tOrgData.iCur += iLen;
                pPrnData += iLen; 
            }

            iLen = sprintf_s(pPrnData, SPRINTFLEN, "\r\n");
            g_tOrgData.iCur += iLen;
            pPrnData += iLen; 

        }
        
        file.Write(g_tOrgData.aucLog, g_tOrgData.iCur);
    }

    file.Close();
    
    strInfo.Format(IDS_STRINFSAVEXLSUCC,strOrgData);
    theApp.SaveShowMessage(strInfo);
}

void CDlgHisList::Export1Img(UINT *pnSel, UINT nSelCount)
{
    UINT    i       = 0;
    float   fHeight = (float)470.25;    //excel 2010
    float   X       = 0;
    float   Y       = 0;
    int     iRow    = 0;
    int     iCol    = 0;
    CString strNo;
    CString strTempName;
    CFile   TempFile;
    
    ASSERT_NULL(pnSel);
    ASSERT_ZERO(nSelCount);

    for(i=0; i<nSelCount; i++)
    {
        strNo.Format(IDS_STRPNGNAME, pdlgPrint->m_strFileName, pnSel[i]);
        strTempName = theApp.GetSaveDataPath() + strNo;

        X = 55;
        Y = 5 + i*fHeight;
        iRow = i * 19 + 1;
        iCol = 5;

        //m_oShapes.AddPicture(strTempName, false, true, X, Y, 710, 450);
        m_tSaveExc.addCellPicture(strTempName, X, Y, 710, 450);
#ifndef HAVINGPRNIMG
        TempFile.Remove(strTempName);
#endif
    }
}

void CDlgHisList::Export2Img(UINT *pnSel, UINT nSelCount)
{
    UINT    i      = 0;
    float   fHeight = 351;  //excel 2010； 一页 = 2*nHeight
    float   X       = 0;
    float   Y       = 0;
    int     iRow    = 0;
    int     iCol    = 0;
    CString strNo;
    CString strTempName;
    CFile   TempFile;
    
    ASSERT_NULL(pnSel);
    ASSERT_ZERO(nSelCount);

    for(i=0; i<nSelCount; i++)
    {
        strNo.Format(IDS_STRPNGNAME, pdlgPrint->m_strFileName, pnSel[i]);
        strTempName = theApp.GetSaveDataPath() + strNo;

        X = 31;
        Y = 5 + i*fHeight;
        iRow = i * 18 + 2;
        iCol = 4;

        //m_oShapes.AddPicture(strTempName, false, true, (float)X, (float)Y, 538, 341);
        m_tSaveExc.addCellPicture(strTempName, X, Y, 538, 341);

#ifndef HAVINGPRNIMG
        TempFile.Remove(strTempName);
#endif
    }
}

void CDlgHisList::Export3Img(UINT *pnSel, UINT nSelCount)
{
    UINT    i       = 0;
    int     iPages  = 0;
    float   fWidth  = 286;
    float   fHeight = (float)471.75;    //excel 2010
    float   X       = 0;
    float   Y       = 0;
    int     iRow    = 0;
    int     iCol    = 0;
    CString strNo;
    CString strTempName;
    CFile   TempFile;
    CShape  tCurShape;
    VARIANT var;
    
    ASSERT_NULL(pnSel);
    ASSERT_ZERO(nSelCount);

    var.vt   = VT_I2;
    var.lVal = 1;

    for(i=0; i<nSelCount; i++)
    {
        strNo.Format(IDS_STRPNGNAME, pdlgPrint->m_strFileName, pnSel[i]);
        strTempName = theApp.GetSaveDataPath() + strNo;

        iPages = i / 3;
        switch(i%3)
        {
            case 0:
                X = (float)(1.0 + 2 * fWidth);
                Y = 1 + iPages*fHeight;
                iRow = 23 + iPages * 37;
                iCol = 1;
                break;
            case 1:
                X = (float)(1.0 + fWidth);
                Y = 1 + iPages*fHeight;
                iRow = 23 + iPages * 37;
                iCol = 7;
                break;
            case 2:
                X = (float)1.0;
                Y = 1 + iPages*fHeight;
                iRow = 23 + iPages * 37;
                iCol = 13;
                break;
        }

        //m_oShape.AttachDispatch(m_oShapes.AddPicture(strTempName, false, true, X, Y, 441, 280), TRUE);
        tCurShape = m_tSaveExc.addCellPicture(strTempName, X, Y, 441, 280);
        tCurShape.IncrementRotation(90);
        tCurShape.IncrementLeft(-78);
        if(i < 3)
        {
            tCurShape.ScaleWidth ((float)0.62, -1, var); // msoTrue -1 ; msoScaleFromMiddle 1
            tCurShape.ScaleHeight((float)0.62, -1, var); // msoTrue -1
            tCurShape.IncrementTop(10);
        }
        else
            tCurShape.IncrementTop(90);
        tCurShape.ReleaseDispatch();
#ifndef HAVINGPRNIMG
        TempFile.Remove(strTempName);
#endif
    }
}

void CDlgHisList::Export8Img(UINT *pnSel, UINT nSelCount)
{
    UINT    i       = 0;
    int     iPages  = 0;
    float   fWidth  = 303;
    float   fHeight = 178;    //excel 2010
    float   X       = 0;
    float   Y       = 0;
    int     iRow    = 0;
    int     iCol    = 0;
    int     iPageHeight = -19;
    CString strNo;
    CString strTempName;
    CFile   TempFile;
    
    ASSERT_NULL(pnSel);
    ASSERT_ZERO(nSelCount);

    for(i=0; i<nSelCount; i++)
    {
        strNo.Format(IDS_STRPNGNAME, pdlgPrint->m_strFileName, pnSel[i]);
        strTempName = theApp.GetSaveDataPath() + strNo;

        iPages = i / 8;
        if(i%2 == 0)
        {
            X = 11;
            Y = 1 + (i/2)*fHeight + iPages* iPageHeight;
            iRow = (i/2) * 17 + 1 - iPages*2;
            iCol = 4;
        }
        else
        {
            X = 21 + fWidth;
            Y = 1 + (i/2)*fHeight + iPages * iPageHeight;
            iRow = (i/2) * 17 + 1 - iPages*2;
            iCol = 13;
        }

        //m_oShapes.AddPicture(strTempName, false, true, (float)X, (float)Y, 270, 170);
        m_tSaveExc.addCellPicture(strTempName, X, Y, 270, 170);

#ifndef HAVINGPRNIMG
        TempFile.Remove(strTempName);
#endif
    }
}

void CDlgHisList::OnBnClickedBtngraphexp()
{
    CString     strTmpFile;
    CString     strXlsFile;
    CString     strItem;
    CString     strInfo;
    CString     strModel;
    CString     strSheet;
    CString     strsuffix;
    UINT        nSelCount = 0;
    int         i = 0;
    
    BeginWaitCursor();
    if(!m_tSaveExc.initExcel())
    {
        EndWaitCursor();
        strInfo.Format(IDS_STRINFNODRIVE);
        theApp.SaveShowMessage(strInfo);
        return;
    }

    strsuffix.Format("_Img(%d).xlsx", theApp.m_nImgNum);
    strSheet.Format("Pic%d", theApp.m_nImgNum);

    strModel = theApp.m_strAppPath.c_str();
    strModel += TEMPLATE_GRAPHY;
   
    if(!m_tSaveExc.open(strModel))
    {
        m_tSaveExc.close();
        m_tSaveExc.release();
        EndWaitCursor();
        strInfo.Format(IDS_STRINFOPENXLFAIL,TEMPLATE_GRAPHY);
        theApp.SaveShowMessage(strInfo); 
        return;
    }
    
    m_tSaveExc.delNOTNameSheet(strSheet);

    if(!m_tSaveExc.loadSheet(strSheet))
    {
        m_tSaveExc.close();
        m_tSaveExc.release();
        EndWaitCursor();
        return;
    }
    
    UpdateData(TRUE);
    nSelCount = GetSelectItem();
    ASSERT_ZERO(nSelCount);
    GetPrintDlg();
#ifndef HAVINGPRNIMG
    pdlgPrint->ShowWindow(SW_SHOW);
    pdlgPrint->PrintLineImg(&m_nSelItem[0], nSelCount);
    pdlgPrint->ShowWindow(SW_HIDE);
#endif
    switch(theApp.m_nImgNum)
    {
        case 1:
            Export1Img(&m_nSelItem[0], nSelCount);
            break;
        case 2:
            Export2Img(&m_nSelItem[0], nSelCount);
            break;
        case 3:
            Export3Img(&m_nSelItem[0], nSelCount);
            break;
        case 8:
        default:
            Export8Img(&m_nSelItem[0], nSelCount);
            break;
    }

    strXlsFile = theApp.GetSaveDataPath();
    strXlsFile += theApp.m_strFileTitle.c_str();
    strXlsFile += strsuffix;
    
    m_tSaveExc.saveAsXLSFile(strXlsFile);
    m_tSaveExc.close();
    m_tSaveExc.release();

    EndWaitCursor();
    
    strInfo.Format(IDS_STRINFSAVEXLSUCC,strXlsFile);
    theApp.SaveShowMessage(strInfo);

}
#if 0
void CDlgHisList::AddExcelChart(double top, CString strRange)
{
    double left = 100, width = 350, height = 220;   // 图表位置，top由入参传入
    // 图表(chart)    
    CChart chart;
    CChartObjects chartobjects;
    CChartObject chartobject;
    VARIANT var;
    LPDISPATCH  lpDisp = NULL;
    
    ASSERT_NULL(g_ptCurSheet);

    lpDisp = g_ptCurSheet->ChartObjects(covOptional);
    ASSERT(lpDisp);

    chartobjects.AttachDispatch(lpDisp);
    chartobject = chartobjects.Add(left, top, width, height);
    chart.AttachDispatch(chartobject.get_Chart());
    chart.put_ChartType(5); // 饼图 xlPie
    //chart.put_ChartType(-4102); // 饼图 xl3DPie
    
    lpDisp = g_ptCurSheet->get_Range(COleVariant("A1"), COleVariant(strRange));
    ASSERT(lpDisp);    
    
    var.vt = VT_DISPATCH;
    var.pdispVal = lpDisp;

    chart.ChartWizard(  var,
                        covOptional,
                        covOptional,
                        covOptional,
                        covOptional,
                        covOptional,
                        covOptional,
                        covOptional,
                        covOptional,
                        covOptional,
                        covOptional
    );

    chart.ApplyDataLabels(2,    // xlDataLabelsShowValue
                        covOptional,
                        covOptional,
                        covOptional,
                        covOptional,
                        covOptional,
                        COleVariant((short)TRUE),   // showvalue
                        COleVariant((short)TRUE),   // showpercent
                        covOptional,
                        covOptional);

}
#endif
void CDlgHisList::SetCell(long irow, long icolumn, CString new_string)
{ 
#if 0
    COleVariant new_value(new_string);

    ASSERT_NULL(g_ptCurSheet);
    
    CRange start_range = g_ptCurSheet->get_Range(COleVariant(_T("A1")),covOptional);
    CRange write_range = start_range.get_Offset(COleVariant((long)irow -1),COleVariant((long)icolumn -1) );
    write_range.put_Value2(new_value);
    start_range.ReleaseDispatch();
    write_range.ReleaseDispatch();
#else
    m_tSaveExc.setCellString(irow, icolumn, new_string);
#endif
}

void CDlgHisList::SetCell(long irow, long icolumn, double fValue)
{ 
    CString     strValue;
    strValue.Format("%.3f", fValue);

    SetCell(irow, icolumn, strValue);
}

void CDlgHisList::SetCell(long irow, long icolumn, int iValue)
{ 
    CString     strValue;
    strValue.Format("%d", iValue);

    SetCell(irow, icolumn, strValue);
}

void CDlgHisList::SetCellFont(int iRow, int iCol, int iNum, UINT dwQuality)
{
    long    lClr = 0;   // 1-black;2-white;3-red;4-green;5-blue; 6-yellow; 7-pink;8-dark blue;

    COMP_BL(iRow, 1);
    COMP_BL(iCol, 1);
    COMP_BL(iNum, 0);

    if (!(dwQuality & QUA_RESU_QUALITYBIT))
    {
        lClr = 3;
    }
        
    m_tSaveExc.setMultiColFont(iRow, iCol, iNum, _T("Times New Roman"), 8, lClr);
}

/*
    iIndex  : 总结中填写显示参数的序号，范围1~6(MAXSUMMARYPARA)
    iSeq    : 参数在结构中的顺序号，范围0~14
    iBegin  : excel表格的开始行号，大于0
    iMaxVal : 这个参数列举的最大值个数，超过它将覆盖其他显示参数的值
*/
BOOL CDlgHisList::SetMultiValue(int iIndex, int iSeq, int iBegin, int iMaxNum)
{
    int     i       = 0;
    int     iValNum = 0;
    CString strValue;
    CStringList slValue;
    TorqData::Torque *ptTorq = NULL;

    COMP_BLE_R(iIndex, 0, FALSE);
    COMP_BG_R(iIndex, MAXSUMMARYPARA, FALSE);
    COMP_BL_R(iSeq, 0, FALSE);
    COMP_BGE_R(iSeq, MAXPARANUM, FALSE);
    COMP_BLE_R(iBegin, 0, FALSE);
    COMP_BLE_R(iMaxNum, 0, FALSE);
    
    strValue.Format("%d.%s", iIndex, theApp.GetTorqShowName(m_ptStatTorq, iSeq));
    SetCell(iBegin, 2,   strValue);

    for(i=0;i<(int)m_tReadData.nTotal;i++)
    {
        ptTorq = &m_tReadData.tData[i];

        /* 调过工具扣和备注非空的记录 */
        if(ptTorq->btoolbuck())
            continue;
        if(!ptTorq->strmemo().empty())
            continue;

        strValue = theApp.GetTorqShowValue(ptTorq, iSeq);
        if(strValue.IsEmpty())
            continue;
        if (NULL == slValue.Find(strValue))
        {
            slValue.AddTail(strValue);
            SetCell(iBegin, 4, strValue);
            iBegin ++;
            iValNum ++;
            if(iValNum >= iMaxNum)
                break;
        }
    }

    return TRUE;
}

/*
    iSeq    : 参数在结构中的顺序号，范围0~14
*/
int CDlgHisList::GetParaValueInfo(int iSeq, LISTINT &listNo)
{
    int     i       = 0;
    int     iValNum = 0;
    CString strValue;
    CStringList slValue;
    TorqData::Torque *ptTorq = NULL;

    COMP_BL_R(iSeq, 0, 0);
    COMP_BGE_R(iSeq, MAXPARANUM, 0);

    for(i=0; i<(int)m_tReadData.nTotal; i++)
    {
        ptTorq = &m_tReadData.tData[i];

        /* 调过工具扣和备注非空的记录 */
        if(ptTorq->btoolbuck())
            continue;

        strValue = theApp.GetTorqShowValue(ptTorq, iSeq);
        if(strValue.IsEmpty())
            continue;
        if (NULL == slValue.Find(strValue))
        {
            slValue.AddTail(strValue);
            listNo.push_back(i);
            iValNum++;
        }
    }

    return iValNum;
}

CString CDlgHisList::GetOperInfo(CStringList &slOper)
{
    int i = 0;
    int j = 0;
    CString strName;
    CString strOper;
    CStringList slCurOper;
    POSITION    tPos;
    TorqData::Torque *ptTorq = NULL;

    for(i=0;i<(int)m_tReadData.nTotal;i++)
    {
        ptTorq = &m_tReadData.tData[i];

        slCurOper.RemoveAll();

        /* 当班班长 */
        strOper = theApp.GetTorqShowValue(ptTorq, theApp.m_tXlsStatCfg.ucOperator);

        theApp.SplitString(strOper, slCurOper);

        tPos = slCurOper.GetHeadPosition();
        while(tPos != NULL)
        {
            strOper = slCurOper.GetNext(tPos);

            if (NULL == slOper.Find(strOper))
                slOper.AddTail(strOper);
        }
    }

    if (ptTorq != NULL)
        strName = theApp.GetTorqShowName(ptTorq, theApp.m_tXlsStatCfg.ucOperator);
    
    return strName;
}

CString CDlgHisList::GetWellName(BOOL bSuffix)
{
    CString strWellName;

    ASSERT_NULL_R(m_ptStatTorq, strWellName)

    strWellName.Format("%s", theApp.GetTorqShowValue(m_ptStatTorq, SHOWPARA_WELLNAME));

    COMP_BFALSE_R(bSuffix, strWellName);
    
    if(LANGUAGE_CHINESE == m_nCurLang)
        strWellName += _T("井");
    else
        strWellName += _T(" Well");

    return strWellName;

}

/* (该井于2019年 12 月16日开始作业，期间共入井
    2-7/8″*6.45EUE(管件规格,参数5)  油管20根，
    2-7/8″*7.34BGT2油管270根
    3-1/2″*6.45BGT3油管160根，
    4-1/2″*6.45BGT2油管120根
    工具扣20根。
    合计入井数370根。
    于2019年12月25号试压合格。)
    
    该井于2020年2月17日开始作业，期间共入井
    88.9×6.45mm 361根，
    73.02X5.51mm 1根，
    88.9×7.34mm 211根，
    工具扣10根。
    合计下油管583根，其中合格入井551根，不合格32根。
    于2020年2月19号试压合格。

IDS_STRINFCONSBEGIN    "该井于%s开始作业，期间共上扣"
IDS_STRINFCONSTUBING    "%s %d根，"
IDS_STRINFCONSBHA       "工具扣%d根。"
//IDS_STRINFCONSTOTAL     "合计入井数%d根。"
IDS_STRINFCONSTOTAL     "合计下油管%d根，其中合格入井%d根，不合格%d根。"
IDS_STRINFCONSEND       "于%s试压合格。"
*/
CString CDlgHisList::GetConsSummaryInfo()
{
    int         i = 0;
    int         j = 0;
    int         iTubing[100] = {0};
    int         iBHA        = 0;
    int         iTubingIndex= 0;
    CString     strSumm;
    CString     strTmp;
    TorqData::Torque *ptTorq = NULL;
    CString     strTubing;
    CStringList slTubing;
    CString     strHaved;
    POSITION    pos;

    ASSERT_ZERO_R(m_tReadData.nTotal, strSumm);

    strTmp.Format(IDS_STRINFCONSBEGIN, theApp.GetTorqFullDate(&m_tReadData.tData[0]));
    strSumm = strTmp;

    for(i=0;i<(int)m_tReadData.nTotal;i++)
    {
        ptTorq = &m_tReadData.tData[i];

        if(ptTorq->btoolbuck())
        {
            iBHA++;
            continue;
        }

        /* 管件规格 */
        strTubing = theApp.GetTorqShowValue(ptTorq, SHOWPARA_TUBINGTYPE);
        for(j=0; j<slTubing.GetCount(); j++)
        {
            pos = slTubing.FindIndex(j);
            if(pos == NULL)
                break;
            strHaved = slTubing.GetAt(pos);
            if(strHaved.CompareNoCase(strTubing) == 0)
            {
                iTubing[j]++;
                break;
            }
        }
        /* 没有找到 */
        if(j == slTubing.GetCount())
        {
            slTubing.AddTail(strTubing);
            iTubing[j]++;
        }
    }

    /* 2-7/8″*7.34BGT2油管270根 */
    strTmp.Empty();
    for(j=0; j<slTubing.GetCount(); j++)
    {
        pos = slTubing.FindIndex(j);
        if(pos == NULL)
            continue;
        strTubing.Format(IDS_STRINFCONSTUBING, slTubing.GetAt(pos), iTubing[j]);
        strTmp += strTubing;
    }
    strSumm += strTmp;

    /* 工具扣 */
    strTmp.Format(IDS_STRINFCONSBHA, iBHA);
    strSumm += strTmp;

    /* 总数 */
    strTmp.Format(IDS_STRINFCONSTOTAL, m_tReadData.nTotal, m_tReadData.nQualy, m_tReadData.nUnQualy);
    strSumm += strTmp;

    strTmp.Format(IDS_STRINFCONSEND, theApp.GetTorqFullDate(&m_tReadData.tData[m_tReadData.nTotal - 1]));

    strSumm += strTmp;

    return strSumm;
}

void CDlgHisList::WriteCoverSheet()
{
    CString     strTime;

    COMP_BFALSE(m_tSaveExc.loadSheet(SHEET_COVER));

    /* A4 / E11 井号 */
    SetCell(4,   1, GetWellName(FALSE));
    SetCell(11,  5, GetWellName(FALSE));

    /* 27 行 D 列 D27 : 开始日期 */
    strTime.Format("'%s", theApp.GetTorqFullDate(&m_tReadData.tData[0]));
    SetCell(27,  4, strTime);

    /* 27 行 G 列 G27 : 结束日期 */
    strTime.Format("'%s", theApp.GetTorqFullDate(&m_tReadData.tData[m_tReadData.nTotal-1]));
    SetCell(27,  7, strTime);
}

void CDlgHisList::WriteSummarySheet()
{
    int         i = 0;
    int         iIndex = 0;
    CStringList slOper;
    int         iRow   = 0;
    CString     strItem;
    POSITION    tPos;

    ASSERT_NULL(m_ptStatTorq);

    COMP_BFALSE(m_tSaveExc.loadSheet(SHEET_SUMMARY));

    /* B2 井名 */
    SetCell(2, 2, GetWellName(FALSE));

    /* 5 行 D 列 D5 : 文件名*/
    //SetCell(5,  4, theApp.m_strFileTitle.c_str());

    /* 4 行 D 列 D4 : 施工井号*/
    SetCell(4, 4, GetWellName(FALSE));
    /* H4:  开始日期         */
    SetCell(4, 8, theApp.GetTorqSimpDate(&m_tReadData.tData[0]));

    /* B7: 6个显示参数，可选, 3/4为多个值 */
    iRow = 7;
    iIndex = 0;
    for(i=0; i<MAXPARANUM; i++)
    {
        if(!theApp.m_tXlsStatCfg.bSummary[i])
            continue;
        iIndex++;
        /* 最多6个显示参数 */
        if(iIndex > MAXSUMMARYPARA)
            break;
        switch(iIndex)
        {
            case 3: // 最多显示4个值
                /* B11:         显示参数名称4  */
                SetMultiValue(SHOWPARA_TUBINGOEM, i, 11, MAX3VALUES);
                iRow = 15;
                break;
            case 4: // 最多显示8个值
                /* B15:         显示参数名称5  */
                SetMultiValue(SHOWPARA_TUBINGTYPE, i, 15, MAX4VALUES);
                iRow = 23;
                break;
            default:
                /* B7:         显示参数名称1  */
                strItem.Format("%d.%s", iIndex, theApp.GetTorqShowName(m_ptStatTorq, i));
                SetCell(iRow, 2,   strItem);
                /* D7:         显示参数值1  */
                SetCell(iRow, 4, theApp.GetTorqShowValue(m_ptStatTorq, i));
                iRow += 2;
                break;
        }
    }

    /* 当班班长和操作员: 第一个是当班班长1个，后面的为操作员4个 */
    GetOperInfo(slOper);
    iRow = 29;
    SetCell(iRow, 2, GetOperInfo(slOper));
    tPos = slOper.GetHeadPosition();
    while(tPos != NULL )
    {
        strItem = slOper.GetNext(tPos);
        SetCell(iRow, 4, strItem);
        iRow ++;
        if(iRow > 33)
            break;
    }

    /* 施工小结 B47 */
    SetCell(47, 2, GetConsSummaryInfo());
}

void CDlgHisList::WriteQualitySheet()
{
    int         i = 0;
    int         j = 0;
    int         iRow = 1;
    /* total/accepted/reject */
    int         iQual[3] = {0};
    int         iUnQuaDetai[MAX_BAD_CAUSE+1] = {0};
    int         iNoDetai = 0;
    /* final/shoulder/delta */
    double      fAveTorq[3] = {0};
    double      fAveTurn[3] = {0};
    int         nShlNum  = 0;
    DWORD       dwQuality = 0;
    DWORD       dwFlag = 1;
    double      fTorque = 0;
    double      fCir    = 0;
    double      fShlTorq=0;
    double      fShlCir = 0;
    WORD        wIPPos  = 0;
    WORD        wSchPos = 0;
    TorqData::Torque *ptTorq = NULL;

    COMP_BFALSE(m_tSaveExc.loadSheet(SHEET_QUALITY));

    iQual[0] = m_tReadData.nTotal;
    for(i=0;i<(int)m_tReadData.nTotal;i++)
    {
        ptTorq = theApp.GetOrgTorqFromTorq(i);
        if(NULL== ptTorq)
            continue;

        dwQuality = theApp.GetQuality(ptTorq);
        if (!(dwQuality & QUA_RESU_QUALITYBIT))
        {
            iQual[2] ++;
            dwFlag = 1;
            iNoDetai++;
            for(j=1; j<=MAX_BAD_CAUSE; j++)
            {
                dwFlag *= 2;
                if(dwQuality & dwFlag)
                {
                    iUnQuaDetai[j]++;
                    iNoDetai--;
                    break;
                }
            }
        }
        else
        {
            iQual[1] ++;
        }

        /* final */
        GET_CTRL_TORQ(fTorque, ptTorq);
        fCir = theApp.GetCir(ptTorq);
        fAveTorq[0] += fTorque;
        fAveTurn[0] += fCir;
        
        /* shoulder */
        fShlTorq = theApp.GetIPTorq(ptTorq,wIPPos,wSchPos); // IP Torq
        fShlCir  = wIPPos * ptTorq->fmaxcir()/MAXLINEITEM;
        if(wIPPos != 0)
        {
            nShlNum++;
            fAveTorq[1] += fShlTorq;
            fAveTurn[1] += fShlCir;
            fAveTorq[2] += (fTorque - fShlTorq);
            fAveTurn[2] += (fCir    - fShlCir);
        }
    }

    if(0 != m_tReadData.nTotal)
    {
        fAveTorq[0] /= m_tReadData.nTotal;
        fAveTurn[0] /= m_tReadData.nTotal;
    }
    if(0 != nShlNum)
    {
        fAveTorq[1] /= nShlNum;
        fAveTurn[1] /= nShlNum;
        fAveTorq[2] /= nShlNum;
        fAveTurn[2] /= nShlNum;
    }

    /* A2 井名 */
    SetCell(2,  1, GetWellName(FALSE));

    /* 5 行 C 列 C5 : 总数 */
    iRow = 5;
    SetCell(iRow++,  3, iQual[0]);
    /* :  合格总数  */
    SetCell(iRow++,  3, iQual[1]);
    /* :  不合格总数  */
    SetCell(iRow++,  3, iQual[2]);

    iRow = 21;
    /* C :  上扣扭矩过小   QUA_TORQ_LESS_LIMIT */
    SetCell(iRow++,  3, iUnQuaDetai[1]);
    /* :  超最大扭矩     QUA_TORQ_MORE_LIMIT */
    SetCell(iRow++,  3, iUnQuaDetai[2]);
    /* :  夹持打滑       QUA_TRANSLATE */
    SetCell(iRow++,  3, iUnQuaDetai[7]);
    /* :  总周数过小     QUA_CIRC_LESS_LIMIT */
    SetCell(iRow++,  3, iUnQuaDetai[5]);
    /* :  起始扭矩过大   QUA_TORQ_MORE_START */
    SetCell(iRow++,  3, iUnQuaDetai[3]);
    /* :  拐点斜率小     QUA_LOW_SLOPE */
    SetCell(iRow++,  3, iUnQuaDetai[13]);
    /* :  超拐点扭矩     QUA_HIGHT_DELTATURN */
    SetCell(iRow++,  3, iUnQuaDetai[10]);
    /* :  拐点扭矩小     QUA_LOW_SHOULD */
    SetCell(iRow++,  3, iUnQuaDetai[9]);
    /* :  超周数差值     QUA_HIGHT_DELTATURN */
    SetCell(iRow++,  3, iUnQuaDetai[12]);
    /* :  卸扣检查       QUA_SHACK_INSPECT */
    SetCell(iRow++,  3, iUnQuaDetai[15]);
    /* :  粘扣           QUA_GALLING */
    SetCell(iRow++,  3, iUnQuaDetai[16]);
    /* :  液压钳打滑           QUA_HYDTONGSLIP */
    //SetCell(iRow++,  3, iUnQuaDetai[17]);
    /* :  丝扣清洗不干净           QUA_THREADNOTCLEAN */
    SetCell(iRow++,  3, iUnQuaDetai[17]);
    /* :  气检不合格           QUA_GASSEALINSPECT */
    SetCell(iRow++,  3, iUnQuaDetai[18]);
    /* :  Low Shoulder Slope  QUA_OTHER_CAUSE */
    SetCell(iRow++,  3, (iUnQuaDetai[4]  +  // QUA_TORQ_MORE_CTRL
                     iUnQuaDetai[6]  +  // QUA_CIRC_MORE_LIMIT
                     iUnQuaDetai[8]  +  // QUA_NOIPPOINT
                     iUnQuaDetai[11] +  // QUA_LOW_DELTATURN
                     iUnQuaDetai[14] +  // QUA_OTHER_CAUSE
                     iNoDetai));        // 没有详细错误信息

    iRow = 53;
    /* B :  平均扭矩  */
    SetCell(iRow,  2, fAveTorq[0]);
    /* C :  平均周数  */
    SetCell(iRow++,  3, fAveTurn[0]);
    /* B:  平均拐点扭矩  */
    SetCell(iRow,  2, fAveTorq[1]);
    /* C++:  平均拐点周数  */
    SetCell(iRow++,  3, fAveTurn[1]);
    /* B:  DELTA拐点扭矩  */
    SetCell(iRow,  2, fAveTorq[2]);
    /* C++:  DELTA拐点周数  */
    SetCell(iRow++,  3, fAveTurn[2]);
}

void CDlgHisList::WriteScatterSheet()
{
    CFile       TempFile;
    CDlgScatter *pDlgScatter = NULL;
    CDlgDataStat *pDlgStat   = NULL;
    CString     strScat;
    CString     strStat;

    COMP_BFALSE(m_tSaveExc.loadSheet(SHEET_SCATTER));

    pDlgScatter = new CDlgScatter();
    pDlgScatter->Create(IDD_DLGSCATTER,this);
    pDlgScatter->DrawScatterPlot();
    pDlgScatter->ShowWindow(SW_SHOW);
    strScat = pDlgScatter->SaveScatterImg();
    delete pDlgScatter;

    pDlgStat = new CDlgDataStat();
    pDlgStat->Create(IDD_DLGDATASTAT,this);
    pDlgStat->ShowCurStat(m_strHisName);
    pDlgStat->ShowWindow(SW_SHOW);
    strStat = pDlgStat->SaveStatImg();
    delete pDlgStat;    

    /* B1井名 */
    SetCell(1,  2, GetWellName(FALSE));

    m_tSaveExc.addCellPicture(strScat, 5, 63, 465, 288);
    TempFile.Remove(strScat);

    m_tSaveExc.addCellPicture(strStat, 5, 440, 465, 288);
    TempFile.Remove(strStat);
}

void  CDlgHisList::FillReportHead(int &iRow, TorqData::Torque *ptHeadTorq)
{
    int         iCol = 0;

    COMP_BL(iRow, 4);
    ASSERT_NULL(ptHeadTorq);

    /* B4 / E4 3 甲方 */
    SetCell(iRow, 2,  theApp.GetTorqShowName(ptHeadTorq,  SHOWPARA_COMPANY));
    SetCell(iRow, 5,  theApp.GetTorqShowValue(ptHeadTorq, SHOWPARA_COMPANY));
    /* H4 / K4 1 井号 */
    SetCell(iRow, 8,  theApp.GetTorqShowName(ptHeadTorq,  SHOWPARA_WELLNAME));
    SetCell(iRow, 11, theApp.GetTorqShowValue(ptHeadTorq, SHOWPARA_WELLNAME));
    iRow++;
    
    /* B5 / E5 4 管件名称 */
    SetCell(iRow, 2,  theApp.GetTorqShowName(ptHeadTorq,  SHOWPARA_TUBINGOEM));
    SetCell(iRow, 5,  theApp.GetTorqShowValue(ptHeadTorq, SHOWPARA_TUBINGOEM));
    /* H5 / K5 5 规格扣型 */
    SetCell(iRow, 8,  theApp.GetTorqShowName(ptHeadTorq,  SHOWPARA_TUBINGTYPE));
    SetCell(iRow, 11, theApp.GetTorqShowValue(ptHeadTorq, SHOWPARA_TUBINGTYPE));
    iRow++;

    // 跳过显示扭矩的名称行
    iRow++;
    iCol = 3;
    /* 7 行 C 列 C7 : 显示扭矩 */
    SetCell(iRow, iCol++, ptHeadTorq->fshow());
    /* :  最小扭矩  */
    SetCell(iRow, iCol++, ptHeadTorq->flowerlimit());
    /* :  最佳扭矩  */
    SetCell(iRow, iCol++, theApp.GetOptTorq(ptHeadTorq));
    /* :  最大扭矩  */
    SetCell(iRow, iCol++, ptHeadTorq->fupperlimit());
    /* :  最小拐点  */
    SetCell(iRow, iCol++, ptHeadTorq->flowertai());
    /* :  最大拐点  */
    SetCell(iRow, iCol++, ptHeadTorq->fuppertai());
    /* :  最小周数  */
    SetCell(iRow, iCol++, ptHeadTorq->flowercir());
    /* :  最大周数  */
    SetCell(iRow, iCol++, ptHeadTorq->fuppercir());
    /* :  最小周数差*/
    SetCell(iRow, iCol++, ptHeadTorq->fmindeltacir());
    /* :  最大周数差*/
    SetCell(iRow, iCol++, ptHeadTorq->fmaxdeltacir());
    /* :  最小斜度  */
    SetCell(iRow, iCol++, ptHeadTorq->fminshlslope());
    iRow++;
}

void CDlgHisList::FillReportData(int &iRow, TorqData::Torque *ptHeadTorq)
{
    int         i           = 0;
    int         iCol        = 0;
    double      fTorque     = 0;
    double      fCir        = 0;
    double      fShlTorq    = 0;
    double      fDelCir     = 0;
    WORD        wIPPos      = 0;
    WORD        wSchPos     = 0;
    DWORD       dwQuality   = 0;
    CStringList slDateTime;
    CString     strQual;
    CString     strCause;
    CString     strRow;
    CString     strType;
    TorqData::Torque *ptTorq = NULL;

    COMP_BL(iRow, 4);
    ASSERT_NULL(ptHeadTorq);

    strType = theApp.GetTorqShowValue(ptHeadTorq, SHOWPARA_TUBINGTYPE);

    // 跳过序号/日期行
    iRow++;
    /* 9 行开始 */
    for(i=0; i<(int)m_tReadData.nTotal; i++)
    {
        ptTorq = theApp.GetOrgTorqFromTorq(i);
        if(NULL== ptTorq)
            continue;

        if(0 != strType.CompareNoCase(theApp.GetTorqShowValue(ptTorq, SHOWPARA_TUBINGTYPE)))
            continue;

        iCol = 2;
        strRow.Format("%d", iRow);
        /* B 序号 */
        SetCell(iRow, iCol++, (i+1));
        /* C 日期/时间 */
        theApp.SplitString(theApp.GetTorqCollTime(ptTorq), slDateTime);
        SetCell(iRow, iCol++, slDateTime.GetHead());
        SetCell(iRow, iCol++, slDateTime.GetTail());
        slDateTime.RemoveAll();
        /* E final Torq */
        GET_CTRL_TORQ(fTorque, ptTorq);
        SetCell(iRow, iCol++, int(fTorque));
        /* F final turn */
        fCir = theApp.GetCir(ptTorq);
        SetCell(iRow, iCol++, fCir);
        
        fShlTorq = theApp.GetIPTorq(ptTorq, wIPPos, wSchPos); // IP Torq
        fDelCir  = theApp.GetIPDelCir(ptTorq, wIPPos);
        if(wIPPos != 0)
        {
            /* G should Torq */
            SetCell(iRow, iCol++, (int)fShlTorq);
            /* H Delta Turn */
            SetCell(iRow, iCol++, fDelCir);
        }
        else
            iCol += 2;

        /* I 入井序号 显示参数10 */
        SetCell(iRow, iCol++, theApp.GetTorqShowValue(ptTorq, theApp.m_tXlsStatCfg.ucTally));
        
        /* J 质量属性 */
        dwQuality = theApp.GetQuality(ptTorq);
        if(dwQuality & QUA_RESU_QUALITYBIT)
        {
            strQual.Format(IDS_STRMARKQUALITY);
            SetCell(iRow, iCol++, strQual);
            iCol++;
        }
        else
        {
            strQual.Format(IDS_STRMARKDISQUAL);
            SetCell(iRow, iCol++, strQual);

            strCause = theApp.GetQualityInfo(ptTorq);
            SetCell(iRow, iCol++, strCause);
        }

        /* 操作人, 显示参数14 占1格 不合并 */
        /* 合并单元格 */
        //range.AttachDispatch(sheet.get_Range(COleVariant(_T("K")+strRow),COleVariant(_T("L")+strRow)),TRUE);
        //range.Merge(COleVariant((long)0));
        SetCell(iRow, iCol++, theApp.GetTorqShowValue(ptTorq, theApp.m_tXlsStatCfg.ucOperator));
        //range.ReleaseDispatch();
        //iCol++;

        /* M 备注  占1格 不合并 */
        //range.AttachDispatch(sheet.get_Range(COleVariant(_T("M")+strRow),COleVariant(_T("N")+strRow)),TRUE);   
        //range.Merge(COleVariant((long)0));
        SetCell(iRow, iCol++, ptTorq->strmemo().c_str());
        //range.ReleaseDispatch();

        /* 设置单元格格式 B~M */
        SetCellFont(iRow, 2, 12, dwQuality);
        iRow++;
    }

}

void CDlgHisList::WriteReportSheet()
{
    int         iBeginRow = 4;
    int         iCurRow   = 0;
    LISTINT     listNo;
    TorqData::Torque *ptHead = NULL;

    ASSERT_NULL(m_ptStatTorq);
    COMP_BFALSE(m_tSaveExc.loadSheet(SHEET_REPORT));

    /* B2 井名 TITLE */
    SetCell(2,  2, GetWellName(FALSE));
    iCurRow  = 4;

    /* 获取 5 规格扣型 的值的名称 的最新序号 */
    GetParaValueInfo(SHOWPARA_TUBINGTYPE, listNo);
    list<int>::iterator it = listNo.begin();

    do {
        if (4 == iCurRow)
        {
            ptHead = m_ptStatTorq;
            if (0 != listNo.size())
            {
                ptHead = &m_tReadData.tData[*it];
            }
        }
        else
        {
            ptHead = &m_tReadData.tData[*it];

            /* 拷贝目的位置 */
            m_tSaveExc.copyMultiRow(iBeginRow, iCurRow, 4);
            m_tSaveExc.setMultiRowHeight(iCurRow+1, 2);
        }

        FillReportHead(iCurRow, ptHead);

        FillReportData(iCurRow, ptHead);

        if (0 != listNo.size())
        {
            /* 跳空3行 */
            iCurRow += 3;
            it++;
        }
        else  // listNo为空，循环只执行一次,it为无效值
        {
            break;
        }
    }while (it != listNo.end());
}

/*  先创建一个_Application类，用_Application来创建一个Excel应用程序接口。
    Excel接口类中层次如下所示：
        -_Application
            -Workbooks 工作薄集合
                -_Workbook 工作薄
                    -Worksheets 工作表集合
                        -_Worksheet 工作表
                            -Range单元格区域
    故要操作表，必须先逐步获取Workbooks—>Workbook —>Worksheets —>Worksheet —>Range */
void CDlgHisList::OnBnClickedBtnstatlist()
{
    CString strTemplate;
    CString strTmpFile;
    CString strXlsFile;
    CString strItem;
    CString strInfo;

    COMP_BFALSE(theApp.CheckPassWord());
    
    BeginWaitCursor();
    if(!m_tSaveExc.initExcel())
    {
        EndWaitCursor();
        strInfo.Format(IDS_STRINFNODRIVE);
        theApp.SaveShowMessage(strInfo);
        return;
    }

    strTmpFile = theApp.m_strAppPath.c_str();
    if(LANGUAGE_CHINESE == m_nCurLang)
        strTemplate = TEMPLATE_RPTCHN;
    else
        strTemplate = TEMPLATE_RPTENG;
    strTmpFile += strTemplate;

    /* 打开模板文件 */
    
    if(!m_tSaveExc.open(strTmpFile))
    {
        m_tSaveExc.close();
        m_tSaveExc.release();
        EndWaitCursor();
        strInfo.Format(IDS_STRINFOPENXLFAIL,strTmpFile);
        theApp.SaveShowMessage(strInfo); 
        return;
    }

    WriteCoverSheet();
    WriteSummarySheet();
    WriteQualitySheet();
    WriteScatterSheet();
    WriteReportSheet();

    strXlsFile = theApp.GetSaveDataPath();
    strXlsFile += theApp.m_strFileTitle.c_str();
    strXlsFile += "_Stat.xlsx";
    
    // 56: xlExcel8    xls 
    // 51: xlWorkbookDefault   xlsx   
    m_tSaveExc.saveAsXLSFile(strXlsFile);
    m_tSaveExc.close();
    m_tSaveExc.release();

    EndWaitCursor();
    
    strInfo.Format(IDS_STRINFSAVEXLSUCC,strXlsFile);
    theApp.SaveShowMessage(strInfo);

    return;
}

void CDlgHisList::OnBnClickedBtnstatset()
{
    CDlgXlsStatSet  dlgXlsStatSet;
    dlgXlsStatSet.DoModal();
}
