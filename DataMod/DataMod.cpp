// DataMod.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DataMod.h"
#include "DataModDlg.h"
#include "DlgPassword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataModApp

BEGIN_MESSAGE_MAP(CDataModApp, CWinApp)
    //{{AFX_MSG_MAP(CDataModApp)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


/*********************代码宏************************************/

/* 设置老版本显示参数 */
#define SET_SHOW_INFO(strName, strValue) {                      \
        pbShow = ptNew->add_tshow();                            \
        pbShow->set_strname(strName);                           \
        pbShow->set_strvalue(strValue);                         \
    }

/////////////////////////////////////////////////////////////////////////////
// CDataModApp construction

CDataModApp::CDataModApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDataModApp object

CDataModApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDataModApp initialization

BOOL CDataModApp::InitInstance()
{
    string  strFont;
    HDC     hdcScreen;

    AfxEnableControlContainer();

    hdcScreen = GetDC(NULL);   //获取屏幕的HDC  
    float ScaleX = (float)GetDeviceCaps(hdcScreen, DESKTOPHORZRES) / (float)GetDeviceCaps(hdcScreen, HORZRES);
    float ScaleY = (float)GetDeviceCaps(hdcScreen, DESKTOPVERTRES) / (float)GetDeviceCaps(hdcScreen, VERTRES);

    float dpiX = (float)GetDeviceCaps(hdcScreen, LOGPIXELSX) / 96;
    float dpiY = (float)GetDeviceCaps(hdcScreen, LOGPIXELSY) / 96;

    ScaleY = (ScaleY > dpiY) ? ScaleY : dpiY;
    m_ucDPILevel = (BYTE)(ScaleY * 4 + 0.5);
    m_nPBHead = htonl(PBHEAD);

    m_nScreenX = GetSystemMetrics(SM_CXFULLSCREEN);
    m_nScreenY = GetSystemMetrics(SM_CYFULLSCREEN);

    strFont = _T("Times New Roman");
    //int fontx = 5 * m_ucDPILevel/4 ;
    int fonty = -12 * m_ucDPILevel / 4;
    m_tLineTextFont.CreateFont(fonty, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,//GB2312_CHARSET,
        OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
        VARIABLE_PITCH | FF_ROMAN, strFont.c_str());
    m_tRuleHFont.CreateFont(fonty, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,//GB2312_CHARSET,
        OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
        VARIABLE_PITCH | FF_ROMAN, strFont.c_str());
    fonty = -12 * m_ucDPILevel / 4;
    m_tPntTextFont.CreateFont(fonty, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,//GB2312_CHARSET,
        OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
        VARIABLE_PITCH | FF_ROMAN, strFont.c_str());
    fonty = -10 * m_ucDPILevel / 4;
    m_tRuleVFont.CreateFont(fonty, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,//GB2312_CHARSET,
        OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
        VARIABLE_PITCH | FF_ROMAN, strFont.c_str());
    //strFont.Empty();
    m_strTallyName = DEFTALLYNAME;
    //m_strWellName.Empty();

    //GetCtrlManager().InstallHook();//启用CCoolControlsManager
    //CoInitialize(NULL);

    g_tReadData.nCur = 0;

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.
    CDataModDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    }
    else if (nResponse == IDCANCEL)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}

int CDataModApp::ExitInstance()
{
    DeleteObject(m_tLineTextFont);
    DeleteObject(m_tRuleHFont);
    DeleteObject(m_tRuleVFont);
    DeleteObject(m_tPntTextFont);

    google::protobuf::ShutdownProtobufLibrary();

    return CWinApp::ExitInstance();
}

int CDataModApp::GetMainIndexfromData(TorqData::Torque* ptTorq)
{
    int         i = 0;
    string      strName;

    ASSERT_NULL_R(ptTorq, -1);

    for (i = 0; i < ptTorq->tshow_size() && i < MAXPARANUM; i++)
    {
        strName = GetTorqShowName(ptTorq, i);
        if (strName == m_strTallyName.GetBuffer(0))
            return i;
    }
    return -1;
}

string  CDataModApp::GetQualityInfo(TorqData::Torque* ptTorq)
{
    int     i = 0;
    DWORD   dwFlag = 1;
    string  strQuality;
    //CString strInfo;
    DWORD   dwQuality = 0;

    ASSERT_NULL_R(ptTorq, strQuality);

    /* 测试版本，直接根据数据判断质量 */
    dwQuality = GetQuality(ptTorq);
    if (dwQuality & QUA_RESU_QUALITYBIT)
    {
        //strQuality.Format(IDS_STRMARKQUALITY);
        strQuality = LoadstringFromRes(IDS_STRMARKQUALITY);
    }
    else
    {
        strQuality = LoadstringFromRes(IDS_STRMARKDISQUAL);
        for (i = 1; i <= MAX_BAD_CAUSE; i++)
        {
            dwFlag *= 2;
            if (dwQuality & dwFlag)
            {
                strQuality = LoadstringFromRes(IDS_STRMARKDISQUAL + i);
                break;
            }
        }
    }

    return strQuality;
}

int CDataModApp::GetQualityIndex(TorqData::Torque* ptTorq)
{
    int     i = 0;
    DWORD   dwFlag = 1;
    DWORD   dwQuality = 0;

    ASSERT_NULL_R(ptTorq, -1);

    /* 测试版本，直接根据数据判断质量 */
    dwQuality = GetQuality(ptTorq);

    if (dwQuality & QUA_RESU_QUALITYBIT)
    {
        return QUA_RESU_GOOD;
    }

    for (i = 1; i <= MAX_BAD_CAUSE; i++)
    {
        dwFlag *= 2;
        if (dwQuality & dwFlag)
        {
            return i;
        }
    }

    return QUA_RESU_BAD;
}

DWORD CDataModApp::GetQuality(TorqData::Torque* ptTorq)
{
    ASSERT_NULL_R(ptTorq, QUA_RESU_BAD);

    if (HaveBreakout(ptTorq) && !HaveMakeUP(ptTorq))
        return QUA_RESU_GOOD;

#ifndef TEST_QUALITY
    return ptTorq->dwquality();
#else
    return JudgeQuality(ptTorq, ptTorq->bbreakout());
#endif
}

/* 质量判定 */
/* 20180804 喻 API普通扣不需要拐点，合格判定以控制扭矩为准，超过最大扭矩和低于最小扭矩为不合格 == 卸扣版本*/
/*
  不合格-超最大扭矩 不合格-扭矩过小 不合格-图形不标准 不合格-无拐点 不合格-打滑 不合格-起始扭矩大 不合格-图形平移
不合格-斜率＜5
不合格-卸扣检查
不合格-试压不密封
不合格-工厂端泄漏
不合格-工厂端位移
***-*****位置不够"-"可以不要
*/
DWORD CDataModApp::JudgeQuality(TorqData::Torque* ptTorq, int iShackle)
{
    DWORD       dwQuality = QUA_RESU_BAD; /* 默认质量NOK */
    WORD        wIPPos = 0;
    WORD        wSchPos = 0;
    double      fMaxTorq = 0;
    double      fCircle = 0;
    double      fDeltaCir = 0;
    double      fSlopeFactor = 0;
    double      fIPTorq = 0;
    double      fMaxDelCir = 0;
    double      fMinDelCir = 0;
    double      fMinSlope = 0;

    ASSERT_NULL_R(ptTorq, QUA_RESU_BAD);

    dwQuality = QUA_RESU_GOOD;

    GET_CTRL_TORQ(fMaxTorq, ptTorq);
    fCircle = GetCir(ptTorq);

    //SET_QUALITY_BIT(fMaxTorq < ptTorq->flowerlimit(), QUA_TORQ_LESS_LIMIT, dwQuality);
    //SET_QUALITY_BIT(fMaxTorq > ptTorq->fupperlimit(), QUA_TORQ_MORE_LIMIT, dwQuality);
    /* 实际的起始扭矩大于最佳扭矩的15% */
    SET_QUALITY_BIT(ptTorq->ftorque(0) > (GetOptTorq(ptTorq) * 0.15), QUA_TORQ_MORE_START, dwQuality);

    /* 超过台阶再平移周数超过0.2 */
    SET_QUALITY_BIT(JudgeTranslate(ptTorq), QUA_TRANSLATE, dwQuality);

    /* 图形周数小于0.20或者贴边 */
    SET_QUALITY_BIT(fCircle < ptTorq->flowercir(), QUA_CIRC_LESS_LIMIT, dwQuality);

    if (dwQuality == 0)
    {
        return QUA_RESU_GOOD;
    }

    return dwQuality;
}

/* 超过控制扭矩的15%再平移周数超过0.2 */
BOOL CDataModApp::JudgeTranslate(TorqData::Torque* ptTorq)
{
    int     i = 0;
    int     iTranCount = 1;
    double  fCtrlTorq = 1;
    double  fTemp;
    int     iCount = 0;

    ASSERT_NULL_R(ptTorq, TRUE);
    ASSERT_ZERO_R(ptTorq->dwmucount(), TRUE);

    iTranCount = (int)ceil((0.2 / ptTorq->fmaxcir()) * 500);
    fCtrlTorq = ptTorq->fmumaxtorq();

    for (i = ptTorq->dwmucount() - 1; i > iTranCount; i--)
    {
        //if (ptTorq->ftorque(i) < ptTorq->fcontrol()*0.15) // 小于控制扭矩的15%
        //if (ptTorq->ftorque(i) < ptTorq->flowertai()) // 小于最小台阶扭矩
        //    break;

        fTemp = (ptTorq->ftorque(i) - ptTorq->ftorque(i - iTranCount)) / fCtrlTorq;
        if (fTemp < 0.005)
            iCount++;
        else
            iCount = 0;

        COMP_BGE_R(iCount, iTranCount, TRUE);
    }

    return FALSE;
}

CString CDataModApp::GetTorqCollTime(TorqData::Torque* ptTorq, bool bBreakout)
{
    __time64_t  colTime;

    ASSERT_NULL_R(ptTorq, _T(""));

    colTime = ptTorq->mucoltime();
    if (bBreakout)
        colTime = ptTorq->bocoltime();

    if (colTime == 0)
        return "";
    COleDateTime olett(colTime);
    return olett.Format(_T("%Y-%m-%d %H:%M:%S"));
}

void CDataModApp::ClearReadTorq(TORQUEDATA* pAllData)
{
    int i = 0;

    if (pAllData == NULL)
        pAllData = &g_tReadData;

    pAllData->nCur = 0;
    pAllData->nTotal = 0;
    pAllData->nQualy = 0;
    pAllData->nUnQualy = 0;

    memset(&pAllData->tSplit, 0, MAXWELLNUM * sizeof(SPLITPOINT));

    for (i = 0; i < MAXWELLNUM; i++)
        pAllData->tData[i].Clear();
    pAllData->strFileName.clear();
}

/* 从文件当前读取位置获取数据的长度，文件位置为读取有效的文件长度之后 */
int CDataModApp::SeekFileLen(CFile& file)
{
    int     i = 0;
    int     iFileLen = 0;
    int     iFirstPos = 0;
    int     iFilePos = 0;
    UINT    nLeng = 0;
    UINT    nNextLen = 0;
    char    cTmpRead[4] = { 0 };

    iFileLen = (int)file.GetLength();
    iFilePos = (int)file.GetPosition();

    if (SeekPBDataPos(file, iFilePos) == -1)
    {
        return -1;
    }

    /* 20221114 文件在中间出现PBHEADLEN，尽量跳过 */
    /*file.Read(cTmpRead, 4);
    if (memcmp(&cTmpRead[i], &m_nPBHead, PBHEADLEN) != 0)
        file.Seek(iFilePos, CFile::begin);*/

        /* 跳过实际的长度，包括可能的尾巴 */
    file.Read(&nLeng, sizeof(UINT));
    iFirstPos = (int)file.GetPosition();
    if ((iFirstPos + (int)nLeng > iFileLen) || (nLeng > MAXPROBUFF))
    {
        /* 20190616 往后再读MAXSKIPLEN次，尝试跳过数据异常
           1个字节一跳 */
        for (i = -3; i < MAXSKIPLEN; i++)
        {
            file.Seek((iFirstPos + i), CFile::begin);
            file.Read(&nNextLen, sizeof(UINT));
            iFilePos = (int)file.GetPosition();
            if ((iFilePos + (int)nNextLen <= iFileLen) && (nNextLen < MAXPROBUFF))
            {
                nLeng = nNextLen;
                break;
            }
        }
        if (i >= MAXSKIPLEN)
        {
            return -2;
        }
    }

    if (nLeng > MAXPROBUFF)
    {
        return -3;
    }

    return nLeng;
}

/* 从第一条记录开始，跳到指定的记录位置
   说明:文件的第一个UINT记录总数已经读出
   for循环的递增为int i
   nLeng 为UNIT类型,表示该数据完整长度 */
int CDataModApp::SeekTorque(CFile& file, int iDataNum)
{
    int     i = 0;
    int     iDataLen = 0;

    file.Seek(sizeof(UINT), CFile::begin);

    for (i = 0; i < iDataNum; i++)
    {
        iDataLen = SeekFileLen(file);
        if (iDataLen < 0)
            return -1;

        file.Seek(iDataLen, CFile::current);
    }

    return 0;
}

int  CDataModApp::SeekPBDataPos(CFile& file, int iCurPos)
{
    int     i = 0;
    int     iFileLen = 0;
    char    cTmpRead[MAXSKIPLEN + 1] = { 0 };

    // 无论有无head都找
    //COMP_BFALSE_R(g_tReadData.bHaveHead, 0);

    iFileLen = (int)file.GetLength();
    if ((iCurPos + MIN_TORQDATALEN) >= iFileLen)
        return -1;

    file.Read(cTmpRead, MAXSKIPLEN);

    /* 查找数据文件头 */
    for (i = 0; i < MAXSKIPLEN - PBHEADLEN; i++)
    {
        if (memcmp(&cTmpRead[i], &m_nPBHead, PBHEADLEN) == 0)
        {
            file.Seek(iCurPos + i + PBHEADLEN, CFile::begin);
            return 0;
        }
    }

    if (g_tReadData.bHaveHead)
        return -1;
    else // 没有pbhead
    {
        file.Seek(iCurPos, CFile::begin);
        return 0;
    }
}

BOOL CDataModApp::ReadHisTorqFromFile(string strDataName, TORQUEDATA* pAllData)
{
    COMP_BFALSE_R(GetTorqDataFromFile(strDataName, pAllData), FALSE);

    /* 没有更新入井序号，直接返回正确，否则重新读一次 */
    COMP_BFALSE_R(ReCalTallyNO(strDataName), TRUE);

    return GetTorqDataFromFile(strDataName, pAllData);
}

/* 读取新版本历史的扭矩数据文件
   一次最多读取MAXWELLNUM 2000条 */
BOOL CDataModApp::GetTorqDataFromFile(string strDataName, TORQUEDATA* pAllData)
{
    CFile   file;
    int     i = 0;
    int     j = 0;
    int     iCtrlCount = 0;
    UINT    nNum = 0;
    UINT    nValid = 0;
    int     iFilePos = 0;
    int     iDataLen = 0;
    int     iTotalPnt = 0;
    CString strInfo;
    //CString strTitle;
    bool    bRes;
    TorqData::Torque* ptTorq = NULL;
    //SPLITPOINT* pSplit = NULL;
    //int     iSplitPos = 0;
    char    cPBHead[PBHEADLEN + 1] = { 0 };
    DWORD   dwQuality = 0;
    double  fRatio = NM2LBFT;
    //string strData;

    COMP_BTRUE_R(strDataName.empty(), FALSE);

    ASSERT_ZERO_R(file.Open(strDataName.c_str(), CFile::modeRead | CFile::shareDenyNone), FALSE);

    m_strReadFile = strDataName;
    //strTitle = file.GetFileTitle();
    //strTitle.Delete(strTitle.GetLength() - 4, 4);
    //m_strFileTitle = strTitle;

    ClearReadTorq();

    file.SeekToBegin();
    file.Read(&nNum, sizeof(UINT));
    if (nNum > MAXWELLNUM)
    {
        strInfo.Format("数据文件记录数(%d)大于%d条，多余记录不读入！", nNum, MAXWELLNUM);
        AfxMessageBox(strInfo);
        nNum = MAXWELLNUM;
    }

    /* 检查文件是否有头 */
#if 0
    g_tReadData.bHaveHead = FALSE;
    file.Read(cPBHead, PBHEADLEN);
    file.Seek(sizeof(UINT), CFile::begin);
    if (memcmp(cPBHead, &m_nPBHead, PBHEADLEN) == 0)
    {
        g_tReadData.bHaveHead = TRUE;
    }
#endif
    BeginWaitCursor();

    for (i = 0; i < (int)nNum; i++)
    {
        iFilePos = (int)file.GetPosition();
        strInfo.Format("<%s>文件数据有误，总记录%d,%d条有效，当前位置%d！", strDataName, nNum, i, file.GetPosition());

        iDataLen = SeekFileLen(file);
        if (iDataLen < 0)
        {
            AfxMessageBox(strInfo);
            break;
        }

        memset(m_cProtoBuf, 0, MAXPROBUFF);
        file.Read(m_cProtoBuf, iDataLen);

        if (iDataLen < MIN_TORQDATALEN)
        {
            continue;
        }

        bRes = g_tReadData.tData[nValid].ParseFromArray(m_cProtoBuf, iDataLen);
        if (!bRes)
            continue;

        /* 数据大于1屏时设置分屏信息 */
        /* 20190609最后一屏按控制周数，其他按满屏计算 */
        /* 20190916 如果数据大于控制周数，则需要分屏，最后一周在控制周数上 */
        ptTorq = &g_tReadData.tData[nValid];
        //pSplit = &g_tReadData.tSplit[nValid];

        g_tReadData.nTotalPlus[nValid] = 0;
        if (HaveMakeUP(ptTorq))
            g_tReadData.nTotalPlus[nValid] += ptTorq->dwmuplus();
        if (HaveBreakout(ptTorq))
            g_tReadData.nTotalPlus[nValid] += ptTorq->dwboplus();
        if (ptTorq->fplus() > 0 && ptTorq->fmaxcir() > 0)
        {
            iTotalPnt = (int)ceil(g_tReadData.nTotalPlus[nValid] / ptTorq->fplus() / ptTorq->fmaxcir() * MAXLINEITEM);
        }
#if 0
        if (HaveBreakout(ptTorq))   /* 从前往后分屏 */
        {
            if (iTotalPnt > MAXLINEITEM)
            {
                // 按 MAXLINEITEM 直接分屏
                pSplit->iCtrlPnt = MAXLINEITEM;
                pSplit->iSplitNum = (int)ceil(iTotalPnt * 1.0 / MAXLINEITEM);
                iSplitPos = 0;
                pSplit->iCur = 1;
                for (j = 0; j < pSplit->iSplitNum && j < MAXSPLIITNUM; j++)
                {
                    pSplit->iBegin[j] = iSplitPos;
                    pSplit->iEnd[j] = MIN(iSplitPos + MAXLINEITEM, iTotalPnt);
                    iSplitPos += MAXLINEITEM;
                    if (iSplitPos >= iTotalPnt)
                        break;
                }
            }
        }
        else    /* 从后往前分屏 */
        {
            iCtrlCount = (int)ceil(GetCtrlCir(ptTorq) * MAXLINEITEM / GetMaxCir(ptTorq));
            if (iCtrlCount < 0)
            {
                break;
            }

            pSplit->iCtrlPnt = iCtrlCount;

            if (iTotalPnt > iCtrlCount)
            {
                pSplit->iSplitNum = 1 + (int)ceil((iTotalPnt - iCtrlCount) * 1.0 / MAXLINEITEM);
                iSplitPos = iTotalPnt;
                j = MIN(pSplit->iSplitNum, MAXSPLIITNUM);
                pSplit->iCur = j;

                // 第一屏到iCtrlCount, 其他满屏
                pSplit->iEnd[j - 1] = iSplitPos;
                pSplit->iBegin[j - 1] = MAX(iSplitPos - iCtrlCount, 0);
                iSplitPos -= iCtrlCount;
                j--;
                for (; j >= 0; j--)
                {
                    pSplit->iEnd[j - 1] = iSplitPos;
                    pSplit->iBegin[j - 1] = MAX(iSplitPos - MAXLINEITEM, 0);
                    iSplitPos -= MAXLINEITEM;
                    if (iSplitPos <= 0)
                        break;
                }
            }
        }
#endif
        /* NM  < ---- > lbft (* ratio) */
        if (g_tGlbCfg.nTorqUnit != ptTorq->dwtorqunit())
        {
            /* 0 (N.M) lb.ft --> N.m  */
            fRatio = LBFT2NM;
            if (g_tGlbCfg.nTorqUnit == 1) // N.m --> lb.ft
                fRatio = NM2LBFT;

            ptTorq->set_fmumaxtorq(fRatio * ptTorq->fmumaxtorq());
            ptTorq->set_fbomaxtorq(fRatio * ptTorq->fbomaxtorq());
            ptTorq->set_fmaxlimit(fRatio * ptTorq->fmaxlimit());
            ptTorq->set_fcontrol(fRatio * ptTorq->fcontrol());
            ptTorq->set_fopttorq(fRatio * ptTorq->fopttorq());
            ptTorq->set_fshow(fRatio * ptTorq->fshow());

            for (j = 0; j < ptTorq->ftorque_size(); j++)
            {
                ptTorq->set_ftorque(j, fRatio * ptTorq->ftorque(j));
            }
        }

        g_tReadData.nTotal++;
        dwQuality = GetQuality(ptTorq);
        if (dwQuality & QUA_RESU_QUALITYBIT)
        {
            g_tReadData.nQualy++;
        }
        else
        {
            g_tReadData.nUnQualy++;
        }
        nValid++;
    }

    file.Close();

    EndWaitCursor();

    return TRUE;
}

/*  nNO 从0开始计数
    iMulti在放大时使用 */
DRAWTORQDATA* CDataModApp::GetDrawDataFromTorq(TorqData::Torque* ptOrg, UINT nTotalPlus, int iMulti)
{
    int i = 0;
    int iDrawPnt = 0;
    int iDataPlus = 0;
    int iDrawPlus = 0;
    int iDrawIndex = 0;
    int iDataIndex = 0;
    int iInsCnt = 0;
    int iPriorDataIndex = 0;
    int iPriorDrawIndex = 0;
    double fPreTorq = 0;
    double fCurTorq = 0;
    double fPreRpm = 0;
    double fCurRpm = 0;
    double fPlusPerPnt = 1.0;
    DRAWTORQDATA* ptDraw = NULL;

    ASSERT_NULL_R(ptOrg, NULL);
    ASSERT_ZERO_R(ptOrg->ftorque_size(), NULL);

    ptDraw = &m_tCurDrawTorq;
    if (iMulti > 1)
        ptDraw = &m_tCurZoomTorq;

    memset(ptDraw, 0, sizeof(DRAWTORQDATA));
    ptDraw->ptOrgTorq = ptOrg;
    if (!VERSION_RECPLUS(ptOrg))   // 3.21及之前版本
    {
        for (i = 0; i < ptOrg->ftorque_size(); i++)
        {
            ptDraw->fTorque[i] = ptOrg->ftorque(i);
            ptDraw->fRpm[i] = ptOrg->frpm(i);
        }

        ptDraw->wCount = ptOrg->ftorque_size();

        return ptDraw;
    }

    fPlusPerPnt = ptOrg->fplus() * ptOrg->fmaxcir() / iMulti / MAXLINEITEM;

    iDrawPnt = (int)ceil(nTotalPlus / fPlusPerPnt);
    if (iDrawPnt < 2)
        iDrawPnt = 2;

    iDrawIndex = 0;
    iDataIndex = 0;
    iDataPlus = ptOrg->dwdelplus(0);
    iDrawPlus = int(ceil(iDrawIndex * fPlusPerPnt));
    ptDraw->fTorque[iDrawIndex] = ptOrg->ftorque(0);
    ptDraw->fRpm[iDrawIndex] = ptOrg->frpm(0);

    iPriorDataIndex = iDataIndex;
    iPriorDrawIndex = iDrawIndex;
    iDrawIndex = 1;
    for (iDataIndex = 1; iDataIndex < ptOrg->ftorque_size() - 1; iDataIndex++)
    {
        fCurTorq = ptOrg->ftorque(iDataIndex);
        /* 跳过delta脉冲为0的情况 */
        while (ptOrg->dwdelplus(iDataIndex) == 0 && iDataIndex < ptOrg->ftorque_size() - 1)
        {
            iDataIndex++;
            fCurTorq = MAX(fCurTorq, ptOrg->ftorque(iDataIndex));
        }
        fCurRpm = ptOrg->frpm(iDataIndex);

        iDataPlus += ptOrg->dwdelplus(iDataIndex);

        /* data分辨率大于draw分辨率时, 只更新当前扭矩 */
        if (iDataPlus <= iDrawPlus)
        {
            iInsCnt = 0;
        }
        else
        {
            for (; iDrawIndex < iDrawPnt; iDrawIndex++)
            {
                iDrawPlus = int(ceil(iDrawIndex * fPlusPerPnt));
                // 当前的iDrawIndex不设置，取iDrawIndex-1
                if (iDrawPlus > iDataPlus)
                    break;
            }

            /* (priorData+1) -- Data
               (priorDraw+1) -- Draw-1 */
            iInsCnt = iDrawIndex - iPriorDrawIndex;
        }
        if (iInsCnt == 0)
        {
            //ptDraw->fTorque[iDrawIndex - 1] = fCurTorq;
            ptDraw->fTorque[iDrawIndex - 1] = MAX(ptDraw->fTorque[iDrawIndex - 1], fCurTorq);
            ptDraw->fRpm[iDrawIndex - 1] = fCurRpm;
        }
        else if (iInsCnt == 1)
        {
            ptDraw->fTorque[iDrawIndex - 1] = fCurTorq;
            ptDraw->fRpm[iDrawIndex - 1] = fCurRpm;
        }
        else
        {
            fPreTorq = ptOrg->ftorque(iPriorDataIndex);
            fPreRpm = ptOrg->frpm(iPriorDataIndex);
            for (i = 0; i < iInsCnt; i++)
            {
                ptDraw->fTorque[i + iPriorDrawIndex + 1] = (fCurTorq - fPreTorq) * (i + 1) / iInsCnt + fPreTorq;
                ptDraw->fRpm[i + iPriorDrawIndex + 1] = (fCurRpm - fPreRpm) * (i + 1) / iInsCnt + fPreRpm;
            }
        }
        iPriorDrawIndex = iDrawIndex;
        iPriorDataIndex = iDataIndex;
    }

    iInsCnt = iDrawPnt - 1 - iDrawIndex;
    if (iInsCnt <= 1)
    {
        ptDraw->fTorque[iDrawPnt - 1] = ptOrg->ftorque(ptOrg->ftorque_size() - 1);
        ptDraw->fRpm[iDrawPnt - 1] = ptOrg->frpm(ptOrg->ftorque_size() - 1);
    }
    else
    {
        fCurTorq = ptOrg->ftorque(ptOrg->ftorque_size() - 1);
        fCurRpm = ptOrg->frpm(ptOrg->ftorque_size() - 1);
        fPreTorq = ptOrg->ftorque(iPriorDataIndex);
        fPreRpm = ptOrg->frpm(iPriorDataIndex);
        for (i = 0; i < iInsCnt; i++)
        {
            ptDraw->fTorque[i + iDrawIndex + 1] = (fCurTorq - fPreTorq) * (i + 1) / iInsCnt + fPreTorq;
            ptDraw->fRpm[i + iDrawIndex + 1] = (fCurRpm - fPreRpm) * (i + 1) / iInsCnt + fPreRpm;
        }
    }

    ptDraw->wCount = iDrawPnt;
    return ptDraw;
}

void CDataModApp::UpdateTorqRpm(TorqData::Torque* ptTorq, int index, double fTorq, double fRpm)
{
    ASSERT_NULL(ptTorq);

    if (index < ptTorq->ftorque_size())
    {
        ptTorq->set_ftorque(index, fTorq);
        ptTorq->set_frpm(index, fRpm);
        return;
    }

    ptTorq->add_ftorque(fTorq);
    ptTorq->add_frpm(fRpm);
}

void CDataModApp::UpdateDelplus(TorqData::Torque* ptTorq, int index, UINT nDelplus)
{
    ASSERT_NULL(ptTorq);

    if (index < ptTorq->dwdelplus_size())
    {
        ptTorq->set_dwdelplus(index, nDelplus);
        return;
    }

    ptTorq->add_dwdelplus(nDelplus);
}

BOOL CDataModApp::CheckPassWord()
{
    CString         strCompPW;
    CString         strSupPW;
    CDlgPassword    dlgPW;

    if (IDOK != dlgPW.DoModal())
    {
        return FALSE;
    }

    strCompPW.Format(IDS_STRDEFAULTPW);
    strSupPW.Format(IDS_STRSUPPORPW);
    if (0 != strCompPW.Compare(dlgPW.m_strPassword) &&
        0 != strSupPW.Compare(dlgPW.m_strPassword))
    {
        AfxMessageBox(IDS_STRERRORPW);
        return FALSE;
    }

    return TRUE;
}

string CDataModApp::LoadstringFromRes(unsigned string_ID)
{
    char buffer[MAX_LOADSTRING];

    unsigned bytes_copied = LoadString(NULL, string_ID, buffer, MAX_LOADSTRING);
    if (!bytes_copied)
        throw std::runtime_error("Resource not found!");

    return string(buffer, bytes_copied);
}

string CDataModApp::LoadstringFromRes(unsigned string_ID, int val)
{
    char buffer[MAX_LOADSTRING];
    LoadString(NULL, string_ID, buffer, MAX_LOADSTRING);
    snprintf(buffer, MAX_LOADSTRING, LoadstringFromRes(string_ID).c_str(), val);

    return string(buffer);
}

string CDataModApp::LoadstringFromRes(unsigned string_ID, double val)
{
    char buffer[MAX_LOADSTRING];
    LoadString(NULL, string_ID, buffer, MAX_LOADSTRING);
    snprintf(buffer, MAX_LOADSTRING, LoadstringFromRes(string_ID).c_str(), val);

    return string(buffer);
}

string CDataModApp::LoadstringFromRes(unsigned string_ID, string val)
{
    string buffer1;
    char buffer2[MAX_LOADSTRING];
    buffer1 = LoadstringFromRes(string_ID);
    //LoadString(m_hLangDLL[g_tGlbCfg.nLangType], string_ID, buffer, MAX_LOADSTRING);
    snprintf(buffer2, MAX_LOADSTRING, buffer1.c_str(), val.c_str());

    return string(buffer2);
}

/*
    nDataPlace: form 1 开始
    -1表示最后一个数据
*/
void CDataModApp::UpdateHisData(CString filename, int iDataPlace, TorqData::Torque* ptTorq)
{
    int     i = 0;
    UINT    nCurPos = 0;    /* 当前数据位置 */
    UINT    nNextPos = 0;    /* 下一个数据的位置 */
    UINT    nLastPos = 0;
    int     iLeft = 0;    /* 当前数据后的数据大小 */
    size_t  iCurLen = 0;    /* 当前数据的总长度 */
    UINT    iDataLen = 0;    /* 数据的总长度 */
    UINT    nTotal = 0;
    CFile   file;
    BYTE* pBuffer = NULL;
    char* pcBuff = NULL;

    ASSERT_NULL(ptTorq);
    ASSERT_ZERO(iDataPlace);

    iCurLen = ptTorq->ByteSizeLong();
    ASSERT_ZERO(iCurLen);
    COMP_BGE(iCurLen, MAXPROBUFF);
    pcBuff = new char[iCurLen];
    ASSERT_NULL(pcBuff);
    memset(pcBuff, 0, iCurLen);
    if (!ptTorq->SerializeToArray(pcBuff, iCurLen))
    {
        delete pcBuff;
        return;
    }

    /* write to file */
    ASSERT_ZERO(file.Open(filename, CFile::modeReadWrite | CFile::shareDenyNone, NULL));

    /* 跳过文件的数据总条数 */
    file.Read(&nTotal, sizeof(UINT));
    if (iDataPlace == -1)
        iDataPlace = nTotal;

    /* 跳过之前记录 */
    if (0 != SeekTorque(file, iDataPlace - 1))
    {
        file.Close();
        return;
    }
    //SEEK_TORQUE(((int)g_tReadData.nCur-1), nLeng);

    /* 获得当前数据的位置 */
    iDataLen = SeekFileLen(file);
    if (iDataLen < 0)
    {
        file.Close();
        return;
    }

    /* 获得当前数据长度的位置，跳过PBHEADLEN,往回跳4个字节 */
    nCurPos = (UINT)file.GetPosition() - sizeof(UINT);
    file.Seek(iDataLen, CFile::current);

    /* 保留当前数据后面的数据 */
    nNextPos = (UINT)file.GetPosition();
    iLeft = (int)(file.GetLength() - nNextPos);
    if (iLeft > 0)
    {
        pBuffer = new BYTE[iLeft];
        ASSERT_NULL(pBuffer);
        memset(pBuffer, 0, iLeft);
        file.Read(pBuffer, iLeft);
    }

    /* 跳到当前数据开始位置，写入更新的数据信息 */
    file.Seek(nCurPos, CFile::begin);

    file.Write(&iCurLen, sizeof(UINT));
    file.Write(pcBuff, iCurLen);

    /* 还原当前数据后面的数据 */
    if (pBuffer != NULL)
    {
        file.Write(pBuffer, iLeft);
        delete pBuffer;
        pBuffer = NULL;
    }
    /* 截掉文件尾多余部分内容 */
    nLastPos = (UINT)file.GetPosition();
    if (nLastPos < file.GetLength())
        file.SetLength(nLastPos);
    file.Close();

    delete pcBuff;
    return;
}

void CDataModApp::GetShowDataRange(DRAWTORQDATA* ptDraw, int& iBegin, int& iEnd, SPLITPOINT* ptSplit, UINT nMulti)
{
    ASSERT_NULL(ptDraw);

    iBegin = 0;
    iEnd = ptDraw->wCount;
    if (nMulti != 1 && VERSION_RECPLUS(ptDraw->ptOrgTorq))  // for 3.22 放大数据
        iEnd = (int)ceil(ptDraw->wCount * 1.0 / nMulti);

    ASSERT_NULL(ptSplit);
    ASSERT_ZERO(ptSplit->iSplitNum);

    /* 默认第一屏 */
    iBegin = ptSplit->iBegin[0];
    iEnd = ptSplit->iEnd[0];

    COMP_BG(ptSplit->iCur, ptSplit->iSplitNum);
    COMP_BL(ptSplit->iCur, 1);

    iBegin = ptSplit->iBegin[ptSplit->iCur - 1];
    iEnd = ptSplit->iEnd[ptSplit->iCur - 1];

    return;
}

double CDataModApp::GetCir(TorqData::Torque* ptTorq, bool bBreakout)
{
    double fCir = 0;

    ASSERT_NULL_R(ptTorq, 0);

    if (!bBreakout)
        fCir = THOUSANDTH(ptTorq->dwmuplus() / ptTorq->fplus());
    else
        fCir = THOUSANDTH(ptTorq->dwboplus() / ptTorq->fplus());

    return fCir;
}

double CDataModApp::GetMaxCir(TorqData::Torque* ptTorq)
{
    double fMaxCir = 1;

    ASSERT_NULL_R(ptTorq, fMaxCir);

    fMaxCir = ptTorq->fmaxcir();

    return fMaxCir;
}

double CDataModApp::GetCtrlCir(TorqData::Torque* ptTorq)
{
    double fCtrlCir = 0.8;

    ASSERT_NULL_R(ptTorq, fCtrlCir);

    fCtrlCir = ptTorq->fcontrolcir();

    return fCtrlCir;
}

double CDataModApp::GetUpperCir(TorqData::Torque* ptTorq)
{
    double fUpperCir = 0.9;

    ASSERT_NULL_R(ptTorq, fUpperCir);

    fUpperCir = ptTorq->fuppercir();

    return fUpperCir;
}

double CDataModApp::GetLowerCir(TorqData::Torque* ptTorq)
{
    double fLowerCir = 0.1;

    ASSERT_NULL_R(ptTorq, fLowerCir);

    fLowerCir = ptTorq->flowercir();

    return fLowerCir;
}

double CDataModApp::GetOptTorq(TorqData::Torque* ptTorq)
{
    double fOptTorq = 2000;

    ASSERT_NULL_R(ptTorq, fOptTorq);

    fOptTorq = ptTorq->fopttorq();
    if (0 == fOptTorq)
        fOptTorq = ptTorq->fcontrol();

    return fOptTorq;
}

UINT CDataModApp::SerialTorq(TorqData::Torque* ptTorq)
{
    UINT nLen = 0;

    ASSERT_NULL_R(ptTorq, 0);

    nLen = ptTorq->ByteSizeLong();

    if (nLen == 0 || nLen >= MAXPROBUFF)
        return 0;

    memset(theApp.m_cProtoBuf, 0, MAXPROBUFF);

    if (!ptTorq->SerializeToArray(theApp.m_cProtoBuf, nLen))
    {
        return 0;
    }

    return nLen;
}

bool CDataModApp::HaveMakeUP(TorqData::Torque* ptTorq)
{
    ASSERT_NULL_R(ptTorq, false);
    if (ptTorq->dwmucount() > 0)
        return true;
    return false;
}

bool CDataModApp::HaveBreakout(TorqData::Torque* ptTorq)
{
    ASSERT_NULL_R(ptTorq, false);
    if (ptTorq->dwbocount() > 0)
        return true;
    return false;
}

bool CDataModApp::HaveTallyNO(TorqData::Torque* ptTorq)
{
    ASSERT_NULL_R(ptTorq, false);

    if (ptTorq->dwmucount() > 0 && QUA_RESU_GOOD == ptTorq->dwquality())
        return true;
    return false;
}

/* 返回是否有修改
   20200302 每次都从最开始重新计算 */
BOOL CDataModApp::ReCalTallyNO(string strDataName)
{
    int     i = 0;
    int     iTallyIndex = -1;
    UINT    nRunningNO = 1;
    BOOL    bModified = FALSE;
    CString strRunningNO;
    CString strOldNO;
    TorqData::Torque* ptTorq = NULL;
    TorqData::ShowInfo* ptRunningShow = NULL;

    iTallyIndex = GetMainIndexfromData(&g_tReadData.tData[0]);
    COMP_BL_R(iTallyIndex, 0, FALSE);

    BeginWaitCursor();
    /* 会从当前序号开始(开始数值为1), 顺序更新后续数据的入井序号 */
    for (i = 1; i <= (int)g_tReadData.nTotal; i++)
    {
        ptTorq = &g_tReadData.tData[i - 1];

        strOldNO = GetTorqShowValue(ptTorq, iTallyIndex);
        ptRunningShow = ptTorq->mutable_tshow(iTallyIndex);
        if (HaveTallyNO(ptTorq))
        {
            strRunningNO.Format("%d", nRunningNO++);
        }
        else
        {
            strRunningNO = NULLSTR;
        }

        if (0 == strOldNO.CompareNoCase(strRunningNO))
            continue;

        bModified = TRUE;
        ptRunningShow->set_strvalue(strRunningNO.GetBuffer());
    }

    EndWaitCursor();

    COMP_BFALSE_R(bModified, FALSE);

    SaveAllData(strDataName);

    return TRUE;
}

void CDataModApp::SaveAllData(string strDataName)
{
    UINT    i = 0;
    size_t  nDataLen = 0;
    CFile   file;

    /* write to file */
    if (file.Open(strDataName.c_str(), CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone, NULL))
    {
        /*更新记录数*/
        file.Write(&g_tReadData.nTotal, sizeof(UINT));

        for (i = 0; i < g_tReadData.nTotal; i++)
        {
            nDataLen = g_tReadData.tData[i].ByteSizeLong();
            if (nDataLen == 0 || nDataLen >= MAXPROBUFF)
                continue;
            memset(m_cProtoBuf, 0, MAXPROBUFF);
            if (!g_tReadData.tData[i].SerializeToArray(m_cProtoBuf, nDataLen))
            {
                continue;
            }

            file.Write(&m_nPBHead, PBHEADLEN);
            file.Write(&nDataLen, sizeof(UINT));
            file.Write(m_cProtoBuf, nDataLen);
        }
    }

    file.Close();
}

CString CDataModApp::GetTorqShowName(TorqData::Torque* ptTorq, int iIndex)
{
    ASSERT_NULL_R(ptTorq, NULLSTR);
    COMP_BL_R(iIndex, 0, NULLSTR);
    COMP_BGE_R(iIndex, MAXPARANUM, NULLSTR);

    if (iIndex >= ptTorq->tshow_size())
        return NULLSTR;

    // cur version iIndex 从1开始, 0为Factory
    // 20220928 按listNO存储，NO从1~15，和show序号对应，不需要--
    /*int ver = ptTorq->dwver();
    if (ptTorq->dwver() < 2 && iIndex > 0)
        iIndex--;*/
    return ptTorq->tshow(iIndex).strname().c_str();
}

CString CDataModApp::GetTorqShowValue(TorqData::Torque* ptTorq, int iIndex)
{
    ASSERT_NULL_R(ptTorq, NULLSTR);
    COMP_BL_R(iIndex, 0, NULLSTR);
    COMP_BGE_R(iIndex, MAXPARANUM, NULLSTR);

    if (iIndex >= ptTorq->tshow_size())
        return NULLSTR;

    // cur version iIndex 从1开始, 0为Factory
    // 20220928 按listNO存储，NO从1~15，和show序号对应，不需要--
    /*if (ptTorq->dwver() < 2 && iIndex > 0)
        iIndex--;*/
    return ptTorq->tshow(iIndex).strvalue().c_str();
}
