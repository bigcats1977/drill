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


/*********************�����************************************/

/* �����ϰ汾��ʾ���� */
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

void CDataModApp::InitVariant()
{
    // test user name
    g_tReadData.nCur = 0;
    m_nPBHead = htonl(PBHEAD);
    m_strTallyName = DEFTALLYNAME;

    InitGlobalPara();
}

BOOL CDataModApp::InitInstance()
{
    string  strFont;
    HDC     hdcScreen;

    AfxEnableControlContainer();

    hdcScreen = GetDC(NULL);   //��ȡ��Ļ��HDC  
    float ScaleX = (float)GetDeviceCaps(hdcScreen, DESKTOPHORZRES) / (float)GetDeviceCaps(hdcScreen, HORZRES);
    float ScaleY = (float)GetDeviceCaps(hdcScreen, DESKTOPVERTRES) / (float)GetDeviceCaps(hdcScreen, VERTRES);

    float dpiX = (float)GetDeviceCaps(hdcScreen, LOGPIXELSX) / 96;
    float dpiY = (float)GetDeviceCaps(hdcScreen, LOGPIXELSY) / 96;

    ScaleY = (ScaleY > dpiY) ? ScaleY : dpiY;
    m_ucDPILevel = (BYTE)ceil(ScaleY * 4);// + 0.5);
    m_nPBHead = htonl(PBHEAD);

    m_nScreenX = GetSystemMetrics(SM_CXFULLSCREEN);
    m_nScreenY = GetSystemMetrics(SM_CYFULLSCREEN);

    strFont = _T("Times New Roman");
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

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

    /* ��ʼ�����顢���� */
    InitVariant();

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

void CDataModApp::InitGlobalPara()
{
    g_tGlbCfg.nLangType = LANGUAGE_CHINESE;
    g_tGlbCfg.nPortNO = 1;
    g_tGlbCfg.nPlusPerTurn = 4500;
    g_tGlbCfg.nTorqUnit = 0;
    g_tGlbCfg.nCollectDur = 100;
    g_tGlbCfg.nResetDur = 10000;
    g_tGlbCfg.nSaveDur = 30000;
    g_tGlbCfg.nZoomIn = 5;
    g_tGlbCfg.nImgNum = 8;
    g_tGlbCfg.nTest = 0;

    g_tGlbCfg.fDiscount = 1;
    g_tGlbCfg.fMulti = 1;
    g_tGlbCfg.fRpmAdj = 3.5;

    g_tGlbCfg.bBigTorq = false;
    g_tGlbCfg.bDateBehind = false;

    g_tGlbCfg.strPassWord = _T("Ktwh888");
    g_tGlbCfg.strDataPath = NULLSTR;

    g_tGlbCfg.strUnit = string_format("N%sm", BIGPOINT);
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

    /* ���԰汾��ֱ�Ӹ��������ж����� */
    dwQuality = GetQuality(ptTorq);
    if (dwQuality & QUA_RESU_QUALITYBIT)
    {
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

    /* ���԰汾��ֱ�Ӹ��������ж����� */
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

/* �����ж� */
/* 20180804 �� API��ͨ�۲���Ҫ�յ㣬�ϸ��ж��Կ���Ť��Ϊ׼���������Ť�غ͵�����СŤ��Ϊ���ϸ� == ж�۰汾*/
/*
  ���ϸ�-�����Ť�� ���ϸ�-Ť�ع�С ���ϸ�-ͼ�β���׼ ���ϸ�-�޹յ� ���ϸ�-�� ���ϸ�-��ʼŤ�ش� ���ϸ�-ͼ��ƽ��
���ϸ�-б�ʣ�5
���ϸ�-ж�ۼ��
���ϸ�-��ѹ���ܷ�
���ϸ�-������й©
���ϸ�-������λ��
***-*****λ�ò���"-"���Բ�Ҫ
*/
DWORD CDataModApp::JudgeQuality(TorqData::Torque* ptTorq, int iShackle)
{
    DWORD       dwQuality = QUA_RESU_BAD; /* Ĭ������NOK */
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
    fCircle = GetCir(ptTorq, TYPE_MAKEUP);

    //SET_QUALITY_BIT(fMaxTorq < ptTorq->flowerlimit(), QUA_TORQ_LESS_LIMIT, dwQuality);
    //SET_QUALITY_BIT(fMaxTorq > ptTorq->fupperlimit(), QUA_TORQ_MORE_LIMIT, dwQuality);
    /* ʵ�ʵ���ʼŤ�ش������Ť�ص�15% */
    SET_QUALITY_BIT(ptTorq->ftorque(0) > (GetOptTorq(ptTorq) * 0.15), QUA_TORQ_MORE_START, dwQuality);

    /* ����̨����ƽ����������0.2 */
    SET_QUALITY_BIT(JudgeTranslate(ptTorq), QUA_TRANSLATE, dwQuality);

    /* ͼ������С��0.20�������� */
    SET_QUALITY_BIT(fCircle < ptTorq->flowercir(), QUA_CIRC_LESS_LIMIT, dwQuality);

    if (dwQuality == 0)
    {
        return QUA_RESU_GOOD;
    }

    return dwQuality;
}

/* ��������Ť�ص�15%��ƽ����������0.25 */
bool CDataModApp::JudgeTranslate(TorqData::Torque* ptTorq)
{
    int     i = 0;
    int     iTranCount = 1;
    double  fCtrlTorq = 1;
    double  fTemp;
    int     iCount = 0;

    ASSERT_NULL_R(ptTorq, true);
    ASSERT_ZERO_R(ptTorq->dwmucount(), true);

    iTranCount = (int)ceil((0.25 / ptTorq->fmaxcir()) * MAXLINEITEM);
    fCtrlTorq = ptTorq->fmumaxtorq();

    for (i = ptTorq->dwmucount() - 1; i > iTranCount; i--)
    {
        //if (ptTorq->ftorque(i) < ptTorq->fcontrol()*0.15) // С�ڿ���Ť�ص�15%
        //if (ptTorq->ftorque(i) < ptTorq->flowertai()) // С����С̨��Ť��
        //    break;

        fTemp = (ptTorq->ftorque(i) - ptTorq->ftorque(i - iTranCount)) / fCtrlTorq;
        if (fTemp < 0.005)
            iCount++;
        else
            iCount = 0;

        COMP_BGE_R(iCount, iTranCount, true);
    }

    return false;
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

    for (i = 0; i < MAXWELLNUM; i++)
        pAllData->tData[i].Clear();
    pAllData->strFileName.clear();
}

/* ���ļ���ǰ��ȡλ�û�ȡ���ݵĳ��ȣ��ļ�λ��Ϊ��ȡ��Ч���ļ�����֮�� */
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

    /* 20221114 �ļ����м����PBHEADLEN���������� */
    /*file.Read(cTmpRead, 4);
    if (memcmp(&cTmpRead[i], &m_nPBHead, PBHEADLEN) != 0)
        file.Seek(iFilePos, CFile::begin);*/

        /* ����ʵ�ʵĳ��ȣ��������ܵ�β�� */
    file.Read(&nLeng, sizeof(UINT));
    iFirstPos = (int)file.GetPosition();
    if ((iFirstPos + (int)nLeng > iFileLen) || (nLeng > MAXPROBUFF))
    {
        /* 20190616 �����ٶ�MAXSKIPLEN�Σ��������������쳣
           1���ֽ�һ�� */
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

/* �ӵ�һ����¼��ʼ������ָ���ļ�¼λ��
   ˵��:�ļ��ĵ�һ��UINT��¼�����Ѿ�����
   forѭ���ĵ���Ϊint i
   nLeng ΪUNIT����,��ʾ�������������� */
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

    // ��������head����
    //COMP_BFALSE_R(g_tReadData.bHaveHead, 0);

    iFileLen = (int)file.GetLength();
    if ((iCurPos + MIN_TORQDATALEN) >= iFileLen)
        return -1;

    file.Read(cTmpRead, MAXSKIPLEN);

    /* ���������ļ�ͷ */
    for (i = 0; i < MAXSKIPLEN - PBHEADLEN; i++)
    {
        if (memcmp(&cTmpRead[i], &m_nPBHead, PBHEADLEN) == 0)
        {
            file.Seek(iCurPos + i + PBHEADLEN, CFile::begin);
            return 0;
        }
    }

    return -1;
}

BOOL CDataModApp::ReadHisTorqFromFile(string strDataName, TORQUEDATA* pAllData)
{
    COMP_BFALSE_R(GetTorqDataFromFile(strDataName, pAllData), FALSE);

    /* û�и����뾮��ţ�ֱ�ӷ�����ȷ���������¶�һ�� */
    COMP_BFALSE_R(ReCalTallyNO(strDataName, pAllData), TRUE);

    return GetTorqDataFromFile(strDataName, pAllData);
}

/* ��ȡ�°汾��ʷ��Ť�������ļ�
   һ������ȡMAXWELLNUM 5000�� */
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

    ASSERT_ZERO_R(file.Open(strDataName.c_str(), CFile::modeReadWrite | CFile::shareDenyNone), FALSE);

    if (NULL == pAllData)
        pAllData = &g_tReadData;
    ClearReadTorq(pAllData);
    pAllData->strFileName = strDataName;

    file.SeekToBegin();
    file.Read(&nNum, sizeof(UINT));
    if (nNum > MAXWELLNUM)
    {
        strInfo.Format("�����ļ���¼��(%d)����%d���������¼�����룡", nNum, MAXWELLNUM);
        AfxMessageBox(strInfo);
        nNum = MAXWELLNUM;
    }

    BeginWaitCursor();

    for (i = 0; i < (int)nNum; i++)
    {
        iFilePos = (int)file.GetPosition();
        strInfo.Format("<%s>�ļ����������ܼ�¼%d,%d����Ч����ǰλ��%d��", strDataName.c_str(), nNum, i-1, file.GetPosition());

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

        bRes = pAllData->tData[nValid].ParseFromArray(m_cProtoBuf, iDataLen);
        if (!bRes)
            continue;

        // 20230606 �ϰ汾��������ֵ��bsinglestd�У���Ҫ���õ�columns�У��Ա��������ͨ��columns��ʾ������
        if (ptTorq->bsinglestd() && ptTorq->dwcolumns() == 0)
            ptTorq->set_dwcolumns(1);

        /* ���ݴ���1��ʱ���÷�����Ϣ */
        /* 20190609���һ���������������������������� */
        /* 20190916 ������ݴ��ڿ�������������Ҫ���������һ���ڿ��������� */
        ptTorq = &pAllData->tData[nValid];
        //pSplit = &g_tReadData.tSplit[nValid];

        pAllData->nTotalPlus[nValid] = 0;
        if (HaveMakeUP(ptTorq))
            pAllData->nTotalPlus[nValid] += ptTorq->dwmuplus();
        if (HaveBreakout(ptTorq))
            pAllData->nTotalPlus[nValid] += ptTorq->dwboplus();
        if (ptTorq->fplus() > 0 && ptTorq->fmaxcir() > 0)
        {
            double maxcir = pAllData->nTotalPlus[nValid] / ptTorq->fplus();
            // 20230503 ���������ʵ������С(û�ж�̬�����������߸���ʧ��)ʱ���������������������֤һ������ʾȫ������
            if (maxcir > ptTorq->fmaxcir() * AUTOUPDTURNRATIO)
            {
                maxcir = (int)ceil(maxcir / AUTOUPDTURNRATIO);
                ptTorq->set_fmaxcir(maxcir);
            }
            iTotalPnt = (int)ceil(pAllData->nTotalPlus[nValid] / ptTorq->fplus() / ptTorq->fmaxcir() * MAXLINEITEM);
        }

        pAllData->nTotal++;
        dwQuality = GetQuality(ptTorq);
        if (dwQuality & QUA_RESU_QUALITYBIT)
        {
            pAllData->nQualy++;
        }
        else
        {
            pAllData->nUnQualy++;
        }
        nValid++;
    }

    file.Close();

    EndWaitCursor();

    return TRUE;
}

bool  CDataModApp::GetMakeupDrawData(TorqData::Torque* ptOrg, DRAWTORQDATA* ptDraw, UINT nMulti)
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
    //double fPreRpm = 0;
    double fCurRpm = 0;
    double fPlusPerPnt = 1.0;

    ASSERT_NULL_R(ptOrg, false);
    ASSERT_NULL_R(ptDraw, false);
    COMP_BFALSE_R(HaveMakeUP(ptOrg), false);

    fPlusPerPnt = ptOrg->fplus() * ptOrg->fmaxcir() / nMulti / MAXLINEITEM;
    iDrawPnt = (int)ceil(ptOrg->dwmuplus() / fPlusPerPnt);
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
    iDrawIndex++;
    iDataIndex++;

    for (; iDataIndex < ptOrg->dwmucount() - 1; iDataIndex++)
    {
        fCurTorq = MAX(fCurTorq, ptOrg->ftorque(iDataIndex));
        /* ����delta����Ϊ0����� */
        if (ptOrg->dwdelplus(iDataIndex) == 0)
            continue;
        fCurRpm = ptOrg->frpm(iDataIndex);

        iDataPlus += ptOrg->dwdelplus(iDataIndex);

        /* data�ֱ��ʴ���draw�ֱ���ʱ, ֻ���µ�ǰŤ�� */
        if (iDataPlus <= iDrawPlus)
        {
            iInsCnt = 0;
        }
        else
        {
            for (; iDrawIndex < iDrawPnt; iDrawIndex++)
            {
                iDrawPlus = int(ceil(iDrawIndex * fPlusPerPnt));
                // ��ǰ��iDrawIndex�����ã�ȡiDrawIndex-1
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
            // iDrawIndex == iPriorDrawIndex
            ptDraw->fTorque[iDrawIndex] = MAX(ptDraw->fTorque[iDrawIndex], fCurTorq);
            ptDraw->fRpm[iDrawIndex] = fCurRpm;
        }
        else if (iInsCnt == 1)
        {
            ptDraw->fTorque[iDrawIndex] = fCurTorq;
            ptDraw->fRpm[iDrawIndex] = fCurRpm;
        }
        else
        {
            fPreTorq = ptOrg->ftorque(iPriorDataIndex);
            //fPreRpm = ptOrg->frpm(iPriorDataIndex);
            for (i = 1; i <= iInsCnt; i++)
            {
                ptDraw->fTorque[i + iPriorDrawIndex] = (fCurTorq - fPreTorq) * i / iInsCnt + fPreTorq;
                //ptDraw->fRpm[i + iPriorDrawIndex] = (fCurRpm - fPreRpm) * i / iInsCnt + fPreRpm;
                ptDraw->fRpm[i + iPriorDrawIndex] = fCurRpm;
            }
        }
        iPriorDrawIndex = iDrawIndex;
        iPriorDataIndex = iDataIndex;
        fCurTorq = 0;
    }

    // last point
    iInsCnt = iDrawPnt - 1 - iDrawIndex;
    if (iInsCnt <= 1)
    {
        ptDraw->fTorque[iDrawIndex] = MAX(ptOrg->fmumaxtorq(), ptDraw->fTorque[iDrawIndex]);
        ptDraw->fRpm[iDrawIndex] = MAX(ptOrg->frpm(ptOrg->dwmucount() - 1), ptDraw->fRpm[iDrawIndex]);
        if (iInsCnt < 0)
            iDrawPnt = iDrawIndex + 1;
    }
    else
    {
        fCurTorq = ptOrg->ftorque(ptOrg->dwmucount() - 1);
        fCurRpm = ptOrg->frpm(ptOrg->dwmucount() - 1);
        fPreTorq = ptOrg->ftorque(iPriorDataIndex);
        //fPreRpm = ptOrg->frpm(iPriorDataIndex);
        for (i = 1; i <= iInsCnt; i++)
        {
            ptDraw->fTorque[i + iDrawIndex] = (fCurTorq - fPreTorq) * i / iInsCnt + fPreTorq;
            //ptDraw->fRpm[i + iDrawIndex] = (fCurRpm - fPreRpm) * i / iInsCnt + fPreRpm;
            ptDraw->fRpm[i + iDrawIndex] = fCurRpm;
        }
    }

    ptDraw->wCount = iDrawPnt;
    ptDraw->wMUEndPos = iDrawPnt;
    return true;
}

bool CDataModApp::GetBreakoutDrawData(TorqData::Torque* ptOrg, DRAWTORQDATA* ptDraw, UINT nMulti)
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
    //double fPreRpm = 0;
    double fCurRpm = 0;
    double fPlusPerPnt = 1.0;

    ASSERT_NULL_R(ptOrg, false);
    ASSERT_NULL_R(ptDraw, false);
    COMP_BFALSE_R(HaveBreakout(ptOrg), false);

    fPlusPerPnt = ptOrg->fplus() * ptOrg->fmaxcir() / nMulti / MAXLINEITEM;
    iDrawPnt = (int)ceil(ptOrg->dwboplus() / fPlusPerPnt);
    if (iDrawPnt < 2)
        iDrawPnt = 2;

    if (ptDraw->wCount > 0)
    {
        ptDraw->fTorque[ptDraw->wCount] = 0;
        ptDraw->fRpm[ptDraw->wCount] = ptDraw->fRpm[ptDraw->wCount - 1];
        ptDraw->wCount++;

        // �Ͽ�ж��֮������20��������
        for (i = 0; i < SPLITPOSNUM; i++)
        {
            ptDraw->fTorque[ptDraw->wCount] = 0;
            ptDraw->fRpm[ptDraw->wCount] = 0;
            ptDraw->wCount++;
        }
    }

    iDrawPnt += ptDraw->wCount;
    iDrawIndex = ptDraw->wCount;
    iDataIndex = ptOrg->dwmucount();
    iDataPlus = ptOrg->dwdelplus(iDataIndex);
    if (iDrawIndex > 0)
        iDataPlus += ptOrg->dwmuplus();
    iDrawPlus = int(ceil(iDrawIndex * fPlusPerPnt));

    ptDraw->fTorque[iDrawIndex] = ptOrg->ftorque(ptOrg->dwmucount());
    ptDraw->fRpm[iDrawIndex] = ptOrg->frpm(ptOrg->dwmucount());

    iPriorDataIndex = iDataIndex;
    iPriorDrawIndex = iDrawIndex;
    iDrawIndex++;
    iDataIndex++;

    int totalCount = ptOrg->dwmucount() + ptOrg->dwbocount();
    for (; iDataIndex < totalCount - 1; iDataIndex++)
    {
        fCurTorq = MAX(fCurTorq, ptOrg->ftorque(iDataIndex));
        /* ����delta����Ϊ0����� */
        if (ptOrg->dwdelplus(iDataIndex) == 0)
            continue;
        fCurRpm = ptOrg->frpm(iDataIndex);

        iDataPlus += ptOrg->dwdelplus(iDataIndex);

        /* data�ֱ��ʴ���draw�ֱ���ʱ, ֻ���µ�ǰŤ�� */
        if (iDataPlus <= iDrawPlus)
        {
            iInsCnt = 0;
        }
        else
        {
            for (; iDrawIndex < iDrawPnt; iDrawIndex++)
            {
                iDrawPlus = int(ceil(iDrawIndex * fPlusPerPnt));
                // ��ǰ��iDrawIndex�����ã�ȡiDrawIndex-1
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
            // iDrawIndex == iPriorDrawIndex
            ptDraw->fTorque[iDrawIndex] = MAX(ptDraw->fTorque[iDrawIndex], fCurTorq);
            ptDraw->fRpm[iDrawIndex] = fCurRpm;
        }
        else if (iInsCnt == 1)
        {
            ptDraw->fTorque[iDrawIndex] = fCurTorq;
            ptDraw->fRpm[iDrawIndex] = fCurRpm;
        }
        else
        {
            fPreTorq = ptOrg->ftorque(iPriorDataIndex);
            //fPreRpm = ptOrg->frpm(iPriorDataIndex);
            for (i = 1; i <= iInsCnt; i++)
            {
                ptDraw->fTorque[i + iPriorDrawIndex] = (fCurTorq - fPreTorq) * i / iInsCnt + fPreTorq;
                //ptDraw->fRpm[i + iPriorDrawIndex] = (fCurRpm - fPreRpm) * i / iInsCnt + fPreRpm;
                ptDraw->fRpm[i + iPriorDrawIndex] = fCurRpm;
            }
        }
        iPriorDrawIndex = iDrawIndex;
        iPriorDataIndex = iDataIndex;
        fCurTorq = 0;
    }

    // last point
    iInsCnt = iDrawPnt - 1 - iDrawIndex;
    if (iInsCnt <= 1)
    {
        ptDraw->fTorque[iDrawIndex] = MAX(ptOrg->ftorque(totalCount - 1), ptDraw->fTorque[iDrawIndex]);
        ptDraw->fRpm[iDrawIndex] = ptOrg->frpm(totalCount - 1);
        if (iInsCnt < 0)
            iDrawPnt = iDrawIndex + 1;
    }
    else
    {
        fCurTorq = ptOrg->ftorque(totalCount - 1);
        fCurRpm = ptOrg->frpm(totalCount - 1);
        fPreTorq = ptOrg->ftorque(iPriorDataIndex);
        //fPreRpm = ptOrg->frpm(iPriorDataIndex);
        for (i = 1; i <= iInsCnt; i++)
        {
            ptDraw->fTorque[i + iDrawIndex] = (fCurTorq - fPreTorq) * i / iInsCnt + fPreTorq;
            //ptDraw->fRpm[i + iDrawIndex] = (fCurRpm - fPreRpm) * i / iInsCnt + fPreRpm;
            ptDraw->fRpm[i + iDrawIndex] = fCurRpm;
        }
    }

    ptDraw->wCount = iDrawPnt;
    return true;
}

/*  nNO ��0��ʼ����
    iMulti�ڷŴ�ʱʹ�� */
DRAWTORQDATA* CDataModApp::GetDrawDataFromTorq(TorqData::Torque* ptOrg, UINT nMulti, UINT nType)
{
    DRAWTORQDATA* ptDraw = NULL;

    ASSERT_NULL_R(ptOrg, NULL);
    ASSERT_ZERO_R(ptOrg->ftorque_size(), NULL);

    ptDraw = &m_tCurDrawTorq;
    if (nMulti > 1)
        ptDraw = &m_tCurZoomTorq;

    memset(ptDraw, 0, sizeof(DRAWTORQDATA));
    ptDraw->ptOrgTorq = ptOrg;

    if (ptOrg->dwmucount() > 0 && (nType & 0x01))
    {
        GetMakeupDrawData(ptOrg, ptDraw, nMulti);
    }
    if (ptOrg->dwbocount() > 0 && (nType & 0x02))
    {
        GetBreakoutDrawData(ptOrg, ptDraw, nMulti);
    }
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
    string          strSupPW;
    CDlgPassword    dlgPW;

    if (IDOK != dlgPW.DoModal())
    {
        return FALSE;
    }

    strSupPW = LoadstringFromRes(IDS_STRSUPPORPW);
    if (0 != dlgPW.m_strPassword.Compare(g_tGlbCfg.strPassWord.c_str()) &&
        0 != dlgPW.m_strPassword.Compare(strSupPW.c_str()))
    {
        AfxMessageBox(LoadstringFromRes(IDS_STRERRORPW).c_str());
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

//string CDataModApp::LoadstringFromRes(unsigned string_ID, int val)
//{
//    char buffer[MAX_LOADSTRING];
//    LoadString(NULL, string_ID, buffer, MAX_LOADSTRING);
//    snprintf(buffer, MAX_LOADSTRING, LoadstringFromRes(string_ID).c_str(), val);
//
//    return string(buffer);
//}
//
//string CDataModApp::LoadstringFromRes(unsigned string_ID, double val)
//{
//    char buffer[MAX_LOADSTRING];
//    LoadString(NULL, string_ID, buffer, MAX_LOADSTRING);
//    snprintf(buffer, MAX_LOADSTRING, LoadstringFromRes(string_ID).c_str(), val);
//
//    return string(buffer);
//}
//
//string CDataModApp::LoadstringFromRes(unsigned string_ID, string val)
//{
//    string buffer1;
//    char buffer2[MAX_LOADSTRING];
//    buffer1 = LoadstringFromRes(string_ID);
//    //LoadString(m_hLangDLL[g_tGlbCfg.nLangType], string_ID, buffer, MAX_LOADSTRING);
//    snprintf(buffer2, MAX_LOADSTRING, buffer1.c_str(), val.c_str());
//
//    return string(buffer2);
//}

/*
    nDataPlace: form 1 ��ʼ
    -1��ʾ���һ������
*/
void CDataModApp::UpdateHisData(string strName, int iDataPlace, TorqData::Torque* ptTorq)
{
    int     i = 0;
    UINT    nCurPos = 0;        /* ��ǰ����λ�� */
    UINT    nNextPos = 0;       /* ��һ�����ݵ�λ�� */
    UINT    nLastPos = 0;
    int     iLeft = 0;          /* ��ǰ���ݺ�����ݴ�С */
    size_t  iCurLen = 0;        /* ��ǰ���ݵ��ܳ��� */
    UINT    iDataLen = 0;       /* ���ݵ��ܳ��� */
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
    ASSERT_ZERO(file.Open(strName.c_str(), CFile::modeReadWrite | CFile::shareDenyNone, NULL));

    /* �����ļ������������� */
    file.Read(&nTotal, sizeof(UINT));
    if (iDataPlace == -1)
        iDataPlace = nTotal;

    /* ����֮ǰ��¼ */
    if (0 != SeekTorque(file, iDataPlace - 1))
    {
        file.Close();
        return;
    }
    //SEEK_TORQUE(((int)g_tReadData.nCur-1), nLeng);

    /* ��õ�ǰ���ݵ�λ�� */
    iDataLen = SeekFileLen(file);
    if (iDataLen < 0)
    {
        file.Close();
        return;
    }

    /* ��õ�ǰ���ݳ��ȵ�λ�ã�����PBHEADLEN,������4���ֽ� */
    nCurPos = (UINT)file.GetPosition() - sizeof(UINT);
    file.Seek(iDataLen, CFile::current);

    /* ������ǰ���ݺ�������� */
    nNextPos = (UINT)file.GetPosition();
    iLeft = (int)(file.GetLength() - nNextPos);
    if (iLeft > 0)
    {
        pBuffer = new BYTE[iLeft];
        ASSERT_NULL(pBuffer);
        memset(pBuffer, 0, iLeft);
        file.Read(pBuffer, iLeft);
    }

    /* ������ǰ���ݿ�ʼλ�ã�д����µ�������Ϣ */
    file.Seek(nCurPos, CFile::begin);

    file.Write(&theApp.m_nPBHead, PBHEADLEN);
    file.Write(&iCurLen, sizeof(UINT));
    file.Write(pcBuff, iCurLen);

    /* ��ԭ��ǰ���ݺ�������� */
    if (pBuffer != NULL)
    {
        file.Write(pBuffer, iLeft);
        delete pBuffer;
        pBuffer = NULL;
    }
    /* �ص��ļ�β���ಿ������ */
    nLastPos = (UINT)file.GetPosition();
    if (nLastPos < file.GetLength())
        file.SetLength(nLastPos);
    file.Close();

    delete pcBuff;
    return;
}

double CDataModApp::GetCir(TorqData::Torque* ptTorq, UINT nType)
{
    double fCir = 0;

    ASSERT_NULL_R(ptTorq, 0);

    if(nType & TYPE_MAKEUP)
        fCir += THOUSANDTH(ptTorq->dwmuplus() / ptTorq->fplus());
    if(nType & TYPE_BREAKOUT)
        fCir += THOUSANDTH(ptTorq->dwboplus() / ptTorq->fplus());

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

/* �����Ƿ����޸�
   20200302 ÿ�ζ����ʼ���¼��� */
BOOL CDataModApp::ReCalTallyNO(string strDataName, TORQUEDATA* pAllData)
{
    int     i = 0;
    int     iTallyIndex = -1;
    UINT    nRunningNO = 1;
    BOOL    bModified = FALSE;
    CString strRunningNO;
    CString strOldNO;
    TorqData::Torque* ptTorq = NULL;
    TorqData::ShowInfo* ptRunningShow = NULL;

    if (NULL == pAllData)
        pAllData = &g_tReadData;

    iTallyIndex = GetMainIndexfromData(&pAllData->tData[0]);
    COMP_BL_R(iTallyIndex, 0, FALSE);

    BeginWaitCursor();
    /* ��ӵ�ǰ��ſ�ʼ(��ʼ��ֵΪ1), ˳����º������ݵ��뾮��� */
    for (i = 1; i <= (int)pAllData->nTotal; i++)
    {
        ptTorq = &pAllData->tData[i - 1];

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

    SaveAllData(strDataName, pAllData);

    return TRUE;
}

void CDataModApp::SaveAllData(string strDataName, TORQUEDATA* pAllData)
{
    UINT    i = 0;
    size_t  nDataLen = 0;
    CFile   file;

    if (NULL == pAllData)
        pAllData = &g_tReadData;

    /* write to file */
    if (file.Open(strDataName.c_str(), CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone, NULL))
    {
        /*���¼�¼��*/
        file.Write(&pAllData->nTotal, sizeof(UINT));

        for (i = 0; i < pAllData->nTotal; i++)
        {
            nDataLen = pAllData->tData[i].ByteSizeLong();
            if (nDataLen == 0 || nDataLen >= MAXPROBUFF)
                continue;
            memset(m_cProtoBuf, 0, MAXPROBUFF);
            if (!pAllData->tData[i].SerializeToArray(m_cProtoBuf, nDataLen))
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

    // cur version iIndex ��1��ʼ, 0ΪFactory
    // 20220928 ��listNO�洢��NO��1~15����show��Ŷ�Ӧ������Ҫ--
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

    // cur version iIndex ��1��ʼ, 0ΪFactory
    // 20220928 ��listNO�洢��NO��1~15����show��Ŷ�Ӧ������Ҫ--
    /*if (ptTorq->dwver() < 2 && iIndex > 0)
        iIndex--;*/
    return ptTorq->tshow(iIndex).strvalue().c_str();
}
