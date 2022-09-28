// Torque.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Torque.h"
#include "TorqueDlg.h"
//#include "CoolControlsManager.h"
#include "Winver.h"
#include "Windows.h"
#include "math.h"
#include "lodepng.h"
#include "CrashHandler.h"
#include "DlgPassword.h"
#include "TubeCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define     TEST_QUALITY
extern CTubeCfg    g_cTubing;

/////////////////////////////////////////////////////////////////////////////
// CTorqueApp

BEGIN_MESSAGE_MAP(CTorqueApp, CWinApp)
    //{{AFX_MSG_MAP(CTorqueApp)
    //}}AFX_MSG_MAP
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()



/*********************代码宏************************************/


/* 打印DEBUG信息时，指针自动往后移动
   变量pData和iLen都约定好，减少入参 */
#define INC_DBG_INFO()                  {                       \
        m_tSaveLog.iCur += iLen;                                \
        pData += iLen;                                          \
        if(m_tSaveLog.iCur >= MAXSAVELEN)   {                   \
            SaveLogInfo();                                      \
            pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];        \
        }                                                       \
    }

/////////////////////////////////////////////////////////////////////////////
// CTorqueApp construction

CTorqueApp::CTorqueApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTorqueApp object

CTorqueApp theApp;
CDBAccess  theDB;

/////////////////////////////////////////////////////////////////////////////
// CTorqueApp initialization

BOOL CALLBACK EnumWndProc(HWND hwnd,LPARAM lParam)
{
    HANDLE h = GetProp(hwnd,g_strPropName.c_str());

    COMP_BNE_R(h, g_hValue, TRUE);

    *(HWND*)lParam = hwnd;
    return FALSE;
}

void CTorqueApp::InitVariant()
{
    m_bShowCRC      = FALSE;
    m_nPBHead       = htonl(PBHEAD);
    m_strReadFile.clear();
}

void CTorqueApp::InitLanguage()
{
    int     i = 0;

    for(i=0; i<LANGUAGE_NUM; i++)
    {
        m_hLangDLL[i] = NULL;
    }

    m_hLangDLL[LANGUAGE_CHINESE] = ::LoadLibrary(CHNDLLNAME);
    m_hLangDLL[LANGUAGE_ENGLISH] = ::LoadLibrary(ENGDLLNAME);
    //m_hLangDLL[LANGUAGE_RUSSIAN] = ::LoadLibrary(RUSDLLNAME);
}

void CTorqueApp::ClearTorqCfgPara(PARACFG* ptCfg)
{
    int i = 0;

    memset(&m_tParaCfg.tCtrl, 0, sizeof(CONTROLPARA));
    memset(&m_tParaCfg.tComm, 0, sizeof(COMMONCFG));
    memset(&ptCfg->tTubeCfg, 0, sizeof(TUBECFG));
    m_tParaCfg.strAlias.clear();
    m_tParaCfg.strMemo.clear();
    for (i = 0; i < MAXPARANUM; i++)
        ptCfg->strValue[i].clear();
    m_tParaCfg.tCtrl.ucVer = 2;
}

void CTorqueApp::InitTorqCfgPara(PARACFG* ptCfg)
{
    CONTROLPARA* ptCtrl = NULL;

    ASSERT_NULL(ptCfg);

    ClearTorqCfgPara(ptCfg);

    ptCtrl = &ptCfg->tCtrl;

    ptCtrl->fTorqConf[INDEX_TORQ_MAXLIMIT] = stod(LoadstringFromRes(IDS_STRPVMAXBOUND));
    ptCtrl->fTorqConf[INDEX_TORQ_UPPERLIMIT] = stod(LoadstringFromRes(IDS_STRPVUPLIMIT));
    ptCtrl->fTorqConf[INDEX_TORQ_CONTROL] = stod(LoadstringFromRes(IDS_STRPVCONTROL));
    ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL] = stod(LoadstringFromRes(IDS_STRPVOPTTORQ));
    ptCtrl->fTorqConf[INDEX_TORQ_LOWERLIMIT] = stod(LoadstringFromRes(IDS_STRPVLOWLIMIT));
    ptCtrl->fTorqConf[INDEX_TORQ_SPEEDDOWN] = stod(LoadstringFromRes(IDS_STRPVSPEEDDOWN));
    ptCtrl->fTorqConf[INDEX_TORQ_SHOW] = stod(LoadstringFromRes(IDS_STRPVSHOW));
    ptCtrl->fTorqConf[INDEX_TORQ_BEAR] = stod(LoadstringFromRes(IDS_STRPVBEAR));
    ptCtrl->fTorqConf[INDEX_TORQ_UPPERTAI] = stod(LoadstringFromRes(IDS_STRPVUPTAI));
    ptCtrl->fTorqConf[INDEX_TORQ_LOWERTAI] = stod(LoadstringFromRes(IDS_STRPVLOWTAI));

    ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT] = stod(LoadstringFromRes(IDS_STRPVMAXCIR));
    ptCtrl->fTurnConf[INDEX_TURN_UPPERLIMIT] = stod(LoadstringFromRes(IDS_STRPVUPCIR));
    ptCtrl->fTurnConf[INDEX_TURN_CONTROL] = stod(LoadstringFromRes(IDS_STRPVCTRLCIR));
    ptCtrl->fTurnConf[INDEX_TURN_LOWERLIMIT] = stod(LoadstringFromRes(IDS_STRPVLOWCIR));
    ptCtrl->fTurnConf[INDEX_TURN_MAXDELTA] = stod(LoadstringFromRes(IDS_STRPVMAXDELTACIR));
    ptCtrl->fTurnConf[INDEX_TURN_MINDELTA] = stod(LoadstringFromRes(IDS_STRPVMINDELTACIR));

    ptCtrl->fFullRPM = stod(LoadstringFromRes(IDS_STRPVMAXRPM));
    ptCtrl->fMinShlSlope = stod(LoadstringFromRes(IDS_STRPVMINSHLSLOPE));
}

void CTorqueApp::InitValvePara(VALVECFG* ptCfg)
{
    ASSERT_NULL(ptCfg);

    ptCfg->ucTorq[0][0] = 0;
    ptCfg->ucTorq[0][1] = 30;
    ptCfg->ucTorq[0][2] = 70;

    ptCfg->ucTorq[1][0] = 0;
    ptCfg->ucTorq[1][1] = 30;
    ptCfg->ucTorq[1][2] = 70;

    ptCfg->ucRatio[0][0] = 90;
    ptCfg->ucRatio[0][1] = 70;
    ptCfg->ucRatio[0][2] = 10;

    ptCfg->ucRatio[1][0] = 50;
    ptCfg->ucRatio[1][1] = 10;
    ptCfg->ucRatio[1][2] = 10;
}

BOOL CTorqueApp::LoadLanguageDll(UINT nLang, BOOL bUpdate)
{
    CString strValue;

    CheckLanguage(nLang);

    ASSERT_NULL_R(m_hLangDLL[nLang], FALSE);

    g_tGlbCfg.nLangType = nLang;
    AfxSetResourceHandle(m_hLangDLL[nLang]);

    COMP_BFALSE_R(bUpdate, TRUE);

    theDB.UpdateGlobalPara();

    return TRUE;
}

void CTorqueApp::InitArray()
{
    m_ucDefaultMac[0]   = 0x12;
    m_ucDefaultMac[1]   = 0x79;
    m_ucDefaultMac[2]   = 0x56;
    m_ucDefaultMac[3]   = 0x58;
    m_ucDefaultMac[4]   = 0x11;

    m_strDbgHead[0]     = _T("####");
    m_strDbgHead[1]     = _T("****");
    m_strDbgHead[2]     = _T("#COL");
    m_strDbgHead[3]     = _T("#INF");
    m_strDbgHead[4]     = _T("#SND");
}

BOOL CTorqueApp::InitInstance()
{
    CTime   time=CTime::GetCurrentTime();//得到当前时间
    string  strFont;
    HDC     hdcScreen;
    int     i = 0;
    BOOL    bModified = FALSE;
    string  strDbFile;

    CCrashHandler ch;
    ch.SetProcessExceptionHandlers();
    ch.SetThreadExceptionHandlers();

    //_CrtSetBreakAlloc(9450);

    AfxEnableControlContainer();

    AfxInitRichEdit2();


    /* 避免弹出“由于另一个程序正在运行... */
    AfxOleInit();
    COleMessageFilter* ptFilter = AfxOleGetMessageFilter();
    ptFilter->EnableBusyDialog(FALSE);
    /* AfxOleGetMessageFilter()->EnableBusyDialog(FALSE);
     AfxOleGetMessageFilter()->SetBusyReply(SERVERCALL_RETRYLATER);
     AfxOleGetMessageFilter()->EnableNotRespondingDialog(TRUE);
     AfxOleGetMessageFilter()->SetMessagePendingDelay(-1);*/
    
    /* 让程序退出时自动调用内存泄漏检测函数 */
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    hdcScreen = GetDC(NULL);   //获取屏幕的HDC  
    float ScaleX = (float)GetDeviceCaps(hdcScreen, DESKTOPHORZRES) / (float)GetDeviceCaps(hdcScreen, HORZRES);
    float ScaleY = (float)GetDeviceCaps(hdcScreen, DESKTOPVERTRES) / (float)GetDeviceCaps(hdcScreen, VERTRES);

    float dpiX = (float)GetDeviceCaps(hdcScreen, LOGPIXELSX) / 96;
    float dpiY = (float)GetDeviceCaps(hdcScreen, LOGPIXELSY)/ 96;

    ScaleY = (ScaleY > dpiY) ? ScaleY : dpiY;
    m_ucDPILevel = (BYTE)(ScaleY * 4 + 0.5);
    
    m_nScreenX = GetSystemMetrics(SM_CXFULLSCREEN);
    m_nScreenY = GetSystemMetrics(SM_CYFULLSCREEN);

    strFont= LoadstringFromRes(IDS_STRFONTTIMESNR);
    //int fontx = 5 * m_ucDPILevel/4 ;
    int fonty = -12 * m_ucDPILevel/4 ;
    m_tLineTextFont.CreateFont(fonty, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,//GB2312_CHARSET,
                     OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
                     VARIABLE_PITCH|FF_ROMAN, strFont.c_str());
    m_tRuleHFont.CreateFont(fonty, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,//GB2312_CHARSET,
                     OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
                     VARIABLE_PITCH|FF_ROMAN, strFont.c_str());
#if 0
    fonty = -18 * m_ucDPILevel/4 ;
    m_tPntTextFont.CreateFont(fonty, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET,//GB2312_CHARSET,
                     OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
                     VARIABLE_PITCH|FF_ROMAN, strFont);
#else
    fonty = -12 * m_ucDPILevel/4 ;
    m_tPntTextFont.CreateFont(fonty, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,//GB2312_CHARSET,
                     OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
                     VARIABLE_PITCH|FF_ROMAN, strFont.c_str());
#endif

    fonty = -10 * m_ucDPILevel/4 ;
    m_tRuleVFont.CreateFont(fonty, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,//GB2312_CHARSET,
                     OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
                     VARIABLE_PITCH|FF_ROMAN, strFont.c_str());
    //strFont.Empty();

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

    /*  测试生成dump文件 */
    /* symbols路径设置
       SRV*http://msdl.microsoft.com/download/symbols;C:\Windows\symbols;D:\Project\Torque\3.18\Release */
    /*  符号分析命令
        !analyze -v */

    //这里得到的是程序当前路径
    char buffer[MAX_PATH + 1] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    (_tcsrchr(buffer, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
    m_strAppPath = buffer;

    /* 创建数据路径 */
    m_strDataPath  = m_strAppPath + "Data\\";
    CreateDirectory(m_strDataPath.c_str(), NULL);

    /* 创建操作记录路径 */
    m_strLogPath  = m_strAppPath + "Log\\";
    CreateDirectory(m_strLogPath.c_str(), NULL);

    /*注册文件路径*/
    m_strRegFile   = m_strAppPath + REGNAME;

    /* 动态链接库路径 */
    m_strDllFile   = m_strAppPath + CHNDLLNAME;

    /* 获取数据库文件 */
    int initStep = 0;
    if(!theDB.InitConfigFromDB(initStep))
    {
        InitDefaultConfig(initStep);
    }

    /* 初始化数组、变量 */
    InitVariant();
    InitLanguage();
    InitArray();

    /*构造保存正常Log数据的文件路径*/
    m_strLogFile = m_strLogPath;
    m_strLogFile += time.Format(IDS_STRDATEFORM);
    m_strLogFile += _T(".dbg");

    /* 打开OrgFile*/
    m_tSaveLog.iCur = 0;
    m_SaveLogFile.Open(m_strLogFile.c_str(), CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::shareDenyNone,NULL);

    SetRegistryKey(_T("zsg Applications"));

    LoadLanguageDll(g_tGlbCfg.nLangType, FALSE);
    m_ptCurShow = &m_tShowCfg[g_tGlbCfg.nLangType];
    /* 获取当前数据序号 */
    GetCurNum();

    //启用CCoolControlsManager
    //GetCtrlManager().InstallHook();
    //CoInitialize(NULL);

    m_dwTotalTorqNum = 0;
    /*检查注册情况*/
    CheckAppReg();
    /* 检查产品有效日期 */
    if(!CheckProductDate())
    {
        m_tdbReg.bReged = 0;
    }
    //m_tdbReg.bReged = 1;

    if(0 == m_dwTotalTorqNum)
        m_dwTotalTorqNum = m_nCurNO;

#if 0
    HWND oldHWnd = NULL;
    EnumWindows(EnumWndProc,(LPARAM)&oldHWnd);    //枚举所有运行的窗口
    if(oldHWnd != NULL)
    {
        /*AfxMessageBox("本程序已经在运行了");*/
        ::ShowWindow(oldHWnd,SW_SHOWNORMAL);          //激活找到的前一个程序
        ::SetForegroundWindow(oldHWnd);                //把它设为前景窗口
        return false;                                  //退出本次运行
    }
#endif

    CTorqueDlg dlg;
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

int CTorqueApp::ExitInstance()
{
    int i = 0;

    m_SaveLogFile.Close();

    DeleteObject(m_tLineTextFont);
    DeleteObject(m_tRuleHFont);
    DeleteObject(m_tRuleVFont);
    DeleteObject(m_tPntTextFont);
    for(i=0; i<DBG_MAXNUM; i++)
        m_strDbgHead[i].Empty();
    for (i = 0; i<LANGUAGE_NUM; i++)
    {
        if (m_hLangDLL[i])
        {
            // for NO preprocess _AFXDLL
            //  FreeLibrary(m_hLangDLL[i]);
            AfxFreeLibrary(m_hLangDLL[i]);
        }
    }

    google::protobuf::ShutdownProtobufLibrary();

    //_CrtDumpMemoryLeaks();

    return CWinApp::ExitInstance();
}

/* 获取注册信息和校验 */
void  CTorqueApp::CheckAppReg()
{
    CFile           file;
    CFileFind       find;
    ULONGLONG       iLen = 0;
    CString         strRegCode;
    CString         strReg[REGCODESEGNUM];

    memset(&m_tdbReg,0, sizeof(DBREG));
    m_tdbReg.bRsv1 = 6;
    m_tdbReg.bRsv2 = 12;

    COMP_BFALSE(find.FindFile(m_strRegFile.c_str(), 0));

    file.Open(m_strRegFile.c_str(),CFile::modeRead|CFile::shareDenyNone,NULL);
    iLen = file.GetLength();
    if(iLen == 0)
    {
        file.Close();
        return;
    }
    file.Read(&m_tdbReg,sizeof(DBREG));
    if(iLen > sizeof(DBREG))
    {
        file.Read(&m_dwTotalTorqNum,sizeof(DWORD));
    }
    file.Close();

    /*校验注册码是否正常*/
    strRegCode = m_tdbReg.strRegCode;
    StringSubtract(strRegCode,  REGCODEVALUE);

    SplitRegString(strReg, strRegCode);

    /* 校验注册码 */
    /* 免注册版，直接注释掉Check */
    m_tdbReg.bReged = 0;
    if(CheckReg(strReg))
    {
        m_tdbReg.bReged = 1;
    }

    return;
}

void CTorqueApp::ClearShowPara(SHOWCFG *ptShow)
{
    int     i = 0;
    int     j = 0;
    
    ASSERT_NULL(ptShow);
    
    ptShow->nParaNum = 0;
    ptShow->nListNum = 0;
    ptShow->nMainNum = 0;
    ptShow->nFileName = 0;
    ptShow->nStatType = 0;
    memset(ptShow->nList, 0, sizeof(UINT) * MAXPARANUM);
    memset(ptShow->nMain, 0, sizeof(UINT) * MAXMAINPARA);
    for(i = 0; i<MAXPARANUM; i++)
    {
        ptShow->strShow[i].clear();
    }
}

void CTorqueApp::InitShowPara(SHOWCFG *ptShow, UINT nLang)
{
    int      i  = 0;
    string   strPW;

    ASSERT_NULL(ptShow);
    CheckLanguage(nLang);

    for(i=0; i< MAXPARANUM; i++)
    {
        ptShow->strShow[i] = g_tNameInfo[i].strName[nLang];
        ptShow->nList[i] = i;
    }

    for(i=0;i< MAXMAINPARA;i++)
    {
        ptShow->nMain[i] = g_nMainNameNO[i];
    }

    ptShow->nParaNum = MAXPARANUM;
    ptShow->nListNum = MAXPARANUM-1;
    ptShow->nMainNum = MAXMAINPARA;
    ptShow->nFileName = 1;
    ptShow->nStatType = 5;
}

void CTorqueApp::InitDefaultConfig(int initStep)
{
    int i = 0;

    // global parameter
    if(initStep < 1)
        InitGlobalPara();

    // Show parameter
    if (initStep < 2)
    {
        for (i = 0; i < LANGUAGE_NUM; i++)
        {
            InitShowPara(&m_tShowCfg[i], i);
            InitXlsStatPara(&m_tXlsStatCfg[i]);
        }
    }

    if (initStep < 3)
    {
        InitTorqCfgPara(&m_tParaCfg);
    }
    if (initStep < 4)
    {
        g_cTubing.InitConfig();
    }
    if (initStep < 5)
    {
        InitValvePara(&m_tValveCfg);
    }

    return;
}

void CTorqueApp::InitGlobalPara()
{
    g_tGlbCfg.nLangType = LANGUAGE_CHINESE;
    g_tGlbCfg.nPortNO = 1;
    g_tGlbCfg.nPlusPerTurn = stoi(LoadstringFromRes(IDS_STRPVPLUS));
    g_tGlbCfg.nTorqUnit = 0;
    g_tGlbCfg.nCollectDur = 100;
    g_tGlbCfg.nResetDur = 10000;
    g_tGlbCfg.nSaveDur = 30000;
    g_tGlbCfg.nIPShowMode = 1;
    g_tGlbCfg.nZoomIn = 5;
    g_tGlbCfg.nImgNum = 8;
    g_tGlbCfg.nTest = 0;

    g_tGlbCfg.fDiscount = 1;
    g_tGlbCfg.fMulti = 1;
    g_tGlbCfg.fRpmAdj = 3.5;
    g_tGlbCfg.fIPDeltaVal = 0.1;

    g_tGlbCfg.bCheckIP = 1;
    g_tGlbCfg.bBigTorq = 0;
    g_tGlbCfg.bShackle = 0;
    g_tGlbCfg.bDateBehind = 0;

    g_tGlbCfg.strPassWord = LoadstringFromRes(IDS_STRPVPASSWORD);
    g_tGlbCfg.strDataPath = NULLSTR;

    g_tGlbCfg.strUnit = LoadstringFromRes(IDS_STRTORQNMUNIT, BIGPOINT);
}

BOOL CTorqueApp::SetShowNameFromID(string lsID,SHOWCFG* ptShow, UINT nLang)
{
    UINT i;
    UINT iParaNum = 0;
    vector<string> lsName;
    vector<int> lsIndex;
    string* plsName = NULL;

    ASSERT_NULL_R(ptShow, FALSE);
    if (lsID.empty())
        return FALSE;
    CheckLanguage(nLang);

    iParaNum = ptShow->nParaNum;
    lsName = theDB.GetNamesByIndexs(lsID);
    lsIndex = GetIDFromList(lsID);
    if (lsName.size() < iParaNum)
    {
        return FALSE;
    }
    for (i = 0; i < iParaNum; i++)
    {
        ptShow->strShow[i] = lsName[i];
        ptShow->nShow[i] = lsIndex[i];
    }
    return TRUE;
}

BOOL CTorqueApp::SetShowNOFromID(int iType, string lsID, SHOWCFG* ptShow)
{
    UINT i;
    UINT iParaNum = 0;
    vector<int> lsNO;
    UINT* plsNO = NULL;

    ASSERT_NULL_R(ptShow, FALSE);
    if (lsID.empty())
        return FALSE;

    iParaNum = ptShow->nListNum;
    if (0 == iType)     //当前list
    {
        iParaNum = ptShow->nListNum;
        plsNO = &ptShow->nList[0];
        lsNO = GetIDFromList(lsID);
    }
    else                //当前main
    {
        iParaNum = ptShow->nMainNum;
        plsNO = &ptShow->nMain[0];
        lsNO = GetIDFromList(lsID);
    }

    if (lsNO.size() < iParaNum)
    {
        return FALSE;
    }
    for (i = 0; i < iParaNum; i++)
    {
        plsNO[i] = lsNO[i];
    }
    return TRUE;
}

int CTorqueApp::GetOptionIDbyValue(string name, string value, UINT nLang)
{
    if(name.empty() || value.empty())
        return -1;
    CheckLanguage(nLang);

    return 0;
}
BOOL CTorqueApp::UpdateCurOptions(WORD wNum, string value[], UINT nLang)
{
    CheckLanguage(nLang);
    return FALSE;
}

void CTorqueApp::InitXlsStatPara(XLSSTATCFG* ptStat)
{
    ASSERT_NULL(ptStat);
    
    memset(ptStat, 0, sizeof(XLSSTATCFG));

    ptStat->ucTally    = 10;         /* 10 入井序号 */
    ptStat->ucOperator = 14;         /* 14 队长 */
    
    ptStat->bSummary[2]  = TRUE;     /*  2 作业目的  */
    ptStat->bSummary[3]  = TRUE;     /*  3 甲方      */
    ptStat->bSummary[4]  = TRUE;     /*  4 管件名称  */
    ptStat->bSummary[5]  = TRUE;     /*  5 规格扣型  */
    ptStat->bSummary[12] = TRUE;     /* 12 悬吊工具  */
    ptStat->bSummary[15] = TRUE;     /* 15 液压钳    */
        
    ptStat->bReport[1]  = TRUE;      /* 1 施工井号 */
    ptStat->bReport[3]  = TRUE;      /* 3 甲方     */
    ptStat->bReport[8]  = TRUE;      /* 8 合同号   */
}

/* 扭矩参数 相差 DIFF_TORQUE 10 */
void CTorqueApp::AdjustTorquePara(CONTROLPARA *ptCtrl)
{
    ASSERT_NULL(ptCtrl);

    CHECK_PARA_UP(ptCtrl->fTorqConf[INDEX_TORQ_UPPERLIMIT], ptCtrl->fTorqConf[INDEX_TORQ_MAXLIMIT],   DIFF_TORQUE);
    CHECK_PARA_UP(ptCtrl->fTorqConf[INDEX_TORQ_CONTROL],    ptCtrl->fTorqConf[INDEX_TORQ_UPPERLIMIT], DIFF_TORQUE);
    CHECK_PARA_UP(ptCtrl->fTorqConf[INDEX_TORQ_LOWERLIMIT], ptCtrl->fTorqConf[INDEX_TORQ_CONTROL],    DIFF_TORQUE);
    CHECK_PARA_UP(ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL],    ptCtrl->fTorqConf[INDEX_TORQ_UPPERLIMIT], DIFF_TORQUE);
    CHECK_PARA_UP(ptCtrl->fTorqConf[INDEX_TORQ_LOWERLIMIT], ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL],    DIFF_TORQUE);
    CHECK_PARA_UP(ptCtrl->fTorqConf[INDEX_TORQ_SPEEDDOWN],  ptCtrl->fTorqConf[INDEX_TORQ_LOWERLIMIT], DIFF_TORQUE);
    CHECK_PARA_UP(ptCtrl->fTorqConf[INDEX_TORQ_SHOW],       ptCtrl->fTorqConf[INDEX_TORQ_SPEEDDOWN],  DIFF_TORQUE);
    CHECK_PARA_LOW(ptCtrl->fTorqConf[INDEX_TORQ_BEAR],      ptCtrl->fTorqConf[INDEX_TORQ_SPEEDDOWN],  DIFF_TORQUE);
}

/* 周参数 相差 DIFF_CIRCUIT 0.1 */
void CTorqueApp::AdjustCircuitPara(CONTROLPARA *ptCtrl)
{
    ASSERT_NULL(ptCtrl);

    CHECK_PARA_UP(ptCtrl->fTurnConf[INDEX_TURN_UPPERLIMIT],   ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT],     DIFF_CIRCUIT);
    CHECK_PARA_UP(ptCtrl->fTurnConf[INDEX_TURN_CONTROL], ptCtrl->fTurnConf[INDEX_TURN_UPPERLIMIT],   DIFF_CIRCUIT);
    CHECK_PARA_UP(ptCtrl->fTurnConf[INDEX_TURN_LOWERLIMIT],   ptCtrl->fTurnConf[INDEX_TURN_CONTROL], DIFF_CIRCUIT);
}

void CTorqueApp::AdjustParaValue(PARACFG *ptCfg)
{
    int         i       = 0;
    CONTROLPARA *ptCtrl = NULL;

    ASSERT_NULL(ptCfg);
    ptCtrl = &ptCfg->tCtrl;

    /* 扭矩参数 相差10 */
    AdjustTorquePara(ptCtrl);

    /* 扭拧周数 相差0.1 */
    AdjustCircuitPara(ptCtrl);

    /* 其他参数 */
    //AdjustOtherPara(ptCfg);
}

/* 从原始注册码解密运算 */
BOOL CTorqueApp::GetVolMacFromRegStr(CString strReg[], DWORD pdwVol[], DWORD pdwMac[], int& iYear, int& iMonth, int& iDay)
{
    ASSERT_NULL_R(strReg, FALSE);
    ASSERT_NULL_R(pdwVol,  FALSE);
    ASSERT_NULL_R(pdwMac,  FALSE);

    pdwVol[0] = strtoul(strReg[4] + strReg[5], NULL, 16);
    pdwVol[1] = strtoul(strReg[0] + strReg[1], NULL, 16);
    pdwMac[0] = strtoul(strReg[3], NULL, 16);
    pdwMac[1] = strtoul(strReg[2], NULL, 16);

    pdwVol[0]  = ~pdwVol[0];    ///////// 解密运算
    pdwVol[0] ^= VOL0DEC;       ///////// 解密运算
    pdwVol[0] ^= VOL0XOR;

    pdwVol[1]  = ~pdwVol[1];    ///////// 解密运算
    pdwVol[1] ^= VOL1DEC;       ///////// 解密运算
    pdwVol[1] ^= VOL1XOR;

    pdwMac[0]  = ~pdwMac[0];
    pdwMac[0] ^= MAC0DEC;
    pdwMac[0] ^= MAC0XOR;
    pdwMac[0] &= 0xFFFFFF;

    pdwMac[1]  = ~pdwMac[1];
    pdwMac[1] ^= MAC1DEC;
    pdwMac[1] ^= MAC1XOR;
    pdwMac[1] &= 0xFFFF;

    iYear  = (pdwVol[1] & 0x00FFFF00) >> 8;
    iMonth = (pdwVol[1] & 0x000000FF);
    iDay   = (pdwVol[1] & 0xFF000000) >> 24;

    return TRUE;
}

BOOL CTorqueApp::GetVolMacInfo(DWORD pdwVol[], DWORD pdwMac[], int iYear, int iMonth, int iDay)
{
    UCHAR ucMac[5];

    ASSERT_NULL_R(pdwVol, FALSE);
    ASSERT_NULL_R(pdwMac, FALSE);
    
    GetVolumeInformation("C:\\", NULL, NULL, &pdwVol[0], NULL, NULL, NULL, 0);
    pdwVol[1] = (iDay << 24) + (iYear << 8) + iMonth;

    GetMACAddr(ucMac);
    pdwMac[0] = ((ucMac[0]*256) + ucMac[1])*256 + ucMac[2];
    pdwMac[1] =  (ucMac[3]*256) + ucMac[4];

    return TRUE;
}

/* 验证注册码是否有效 */
BOOL CTorqueApp::CheckReg(CString strReg[])
{
    DWORD adwRegVol[2];
    DWORD adwRegMac[2];
    DWORD adwPCVol[2];
    DWORD adwPCMac[2];
    int   iYear, iMonth, iDay;
    CTime   tNowDate;
    CString strRegDate;
    CString strNowDate;

    ASSERT_NULL_R(strReg, FALSE);

    /* 获取注册码中的卷和MAC信息 */
    GetVolMacFromRegStr(strReg, adwRegVol, adwRegMac, iYear, iMonth, iDay);

    /* 获取机器上的卷和MAC信息 */
    GetVolMacInfo(adwPCVol, adwPCMac, iYear, iMonth, iDay);

    if( memcmp(adwRegVol, adwPCVol, 2*sizeof(DWORD)) != 0 ||
        memcmp(adwRegMac, adwPCMac, 2*sizeof(DWORD)) != 0)
    {
        return FALSE;
    }

    // 检查日期是否超过5年
    tNowDate = CTime::GetCurrentTime();
    tNowDate = CTime(tNowDate.GetYear() - VALID_YEAR,
                     tNowDate.GetMonth(),
                     tNowDate.GetDay(),
                     0,0,0);
    strNowDate = tNowDate.Format( "%Y%m%d");
    strRegDate.Format("%4d%02d%02d", iYear, iMonth, iDay);

    if(strNowDate > strRegDate)
    {
        SaveMessage(strRegDate);
        return FALSE;
    }

    return TRUE;
}

BOOL CTorqueApp::GetProductVersion(CString &strVersion)
{
    DWORD   dwSize      = 0;
    UINT    nSize       = 0;
    LPVOID  pBlock      = NULL;
    char    *pVerValue  = NULL;
    int     iPlace      = -1;
    CString strTemp;
    
    dwSize = GetFileVersionInfoSize(m_strDllFile.c_str(),NULL);
    COMP_BLE_R(dwSize, 0, FALSE);
    pBlock = malloc(dwSize+1);
    ASSERT_NULL_R(pBlock, FALSE);
    GetFileVersionInfo(m_strDllFile.c_str(), 0, dwSize, pBlock);
    ASSERT_ZERO_R(dwSize, FALSE);

    VerQueryValue(pBlock, "\\StringFileInfo\\080404b0\\ProductVersion", (LPVOID*)&pVerValue, &nSize);
    ASSERT_ZERO_R(nSize, FALSE);

    strTemp.Format("%s", pVerValue);
    iPlace = strTemp.ReverseFind(',');
    COMP_BE_R(iPlace, -1, FALSE);

    strVersion = strTemp.Right(strTemp.GetLength() - iPlace - 1);

    free(pBlock);

    return TRUE;
}

BOOL CTorqueApp::CheckProductDate()
{
    BOOL    bRet        = FALSE;
    CTime   tNowDate;
    int     iRandDay    = 0;
    CString strProductDate;
    CString strNowDate;

    bRet = GetProductVersion(strProductDate);
    COMP_BE_R(bRet, FALSE, FALSE);

    /* 有效期5年+(31以内)随机天数 */
    tNowDate  = CTime::GetCurrentTime();

    iRandDay  = rand() % 31;
    tNowDate += CTimeSpan(iRandDay, 0, 0, 0);

    tNowDate = CTime(tNowDate.GetYear() - VALID_YEAR,
                     tNowDate.GetMonth(),
                     tNowDate.GetDay(),
                     0,0,0);
    strNowDate = tNowDate.Format( "%Y%m%d");

    if(strNowDate > strProductDate)
    {
        SaveMessage(strProductDate);
        return FALSE;
    }

    return TRUE;
}

/* 获取MAC地址 */
void CTorqueApp::GetMACAddr(UCHAR *pcMac)
{
    NCB     ncb;
    UCHAR   uRetCode;
    ASTAT   Adapter;
    int     i = 0;

    ASSERT_NULL(pcMac);

    memset( &ncb, 0, sizeof(ncb) );
    ncb.ncb_command = NCBRESET;
    ncb.ncb_lana_num = 0; // 指定网卡号
    // 首先对选定的网卡发送一个NCBRESET命令,以便进行初始化
    Netbios( &ncb );

    memset( &ncb, 0, sizeof(ncb) );
    ncb.ncb_command = NCBASTAT;
    ncb.ncb_lana_num = 0; // 指定网卡号
    ncb.ncb_callname[0] = '*';
    ncb.ncb_callname[1] = '\0';
    ncb.ncb_buffer = (unsigned char *) &Adapter; // 指定返回的信息存放的变量
    ncb.ncb_length = sizeof(Adapter);
    // 接着,可以发送NCBASTAT命令以获取网卡的信息
    uRetCode = Netbios( &ncb );
    // 强制设定为获取网卡失败，使用默认Mac
    uRetCode = 1;
    if(uRetCode == 0)
    {
        for(i=0; i<5; i++)
            *(pcMac+i) = Adapter.adapt.adapter_address[i+1];
    }
    else
    {
        memcpy(pcMac, m_ucDefaultMac, 5);
    }

    return;
}

void CTorqueApp::CreateNewWellFile()
{
    CString strTemp;
    CTime   time=CTime::GetCurrentTime();//得到当前时间

    /* 以井号保存文件 */
    strTemp = m_tParaCfg.strValue[m_ptCurShow->nFileName].c_str();
    m_strDataFile = m_strDataPath;
    if(!g_tGlbCfg.bDateBehind)
    {
        m_strDataFile += time.Format(IDS_STRDATEFORM);
        m_strDataFile += _T("_");
        m_strDataFile += strTemp;
    }
    else
    {
        m_strDataFile += strTemp;
        m_strDataFile += _T("_");
        m_strDataFile += time.Format(IDS_STRDATEFORM);
    }
    m_strDataFile += _T(".pbd");

    m_nCurNO = 0;

    return;
}

int CTorqueApp::GetMainWellIndex()
{
    UINT        i = 0;
    string      strWellName;
    
    strWellName = GetMainShowName(m_ptCurShow, MAINSHOWWELL);

    for(i=1; i< m_ptCurShow->nParaNum && i<MAXPARANUM; i++)
    {
        if(strWellName == m_tShowCfg[g_tGlbCfg.nLangType].strShow[i])
        {
            return i;
        }
    }
    return -1;
}

int CTorqueApp::GetMainWellIndexfromData(UINT nWellNO, TorqData::Torque *ptTorq)
{
    int         i = 0;
    string      strWellName;
    string      strName;

    ASSERT_NULL_R(ptTorq, -1);

    strWellName = theApp.GetListShowName(m_ptCurShow, nWellNO);
    if(strWellName.empty())
        return -1;

    for(i=0; i< ptTorq->tshow_size() && i<MAXPARANUM; i++)
    {
        strName = GetTorqShowName(ptTorq, i);
        if (strName == strWellName)
            return i;
    }
    return -1;
}

int CTorqueApp::GetMainTubeIndex()
{
    UINT        i = 0;
    string      strTubeName;
    
    strTubeName = m_tShowCfg[g_tGlbCfg.nLangType].nMain[MAINSHOWTUBE];

    for(i=1; i<m_tShowCfg[g_tGlbCfg.nLangType].nParaNum && i<MAXPARANUM; i++)
    {
        if(0 == strTubeName.compare(m_tShowCfg[g_tGlbCfg.nLangType].strShow[i]))
        {
            return i;
        }
    }
    return -1;
}

/* 获取当前扭矩的序号 */
void CTorqueApp::GetCurNum()
{
    CFile   file;
    int     iWellIndex = -1;
    TorqData::Torque *ptTorq = NULL;
    int     iWellNO = 0;
    int     i       = 0;
    CString strNumInfo;

    m_nCurNO = 0;

    /* 获取当前的记录文件，超过60天生成新的文件，序号归零 */
    GetCurWellFile();
    file.Open(m_strDataFile.c_str(), CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::shareDenyNone,NULL);
    if(file.GetLength() != 0)
    {
        file.Read(&m_nCurNO,sizeof(UINT));
    }
    file.Close();

    /* 文件记录超量，生成新的文件，序号归零 */
    if(m_nCurNO >= MAXWELLNUM)
        // || m_nCurNO == 0)  为什么每天生成一个数据文件？？的源头 20190927
    {
        CreateNewWellFile();
    }

    if(m_nCurNO > 0 && ReadHisTorqFromFile(m_strDataFile.c_str()))
    {
        iWellIndex = GetMainWellIndex();
        if(iWellIndex >= 0)
        {
            /* 从后往前找最新的入井序号 */
            for(i=g_tReadData.nTotal - 1; i>=0; i--)
            {
                ptTorq = &g_tReadData.tData[i];
                if(iWellIndex >= ptTorq->tshow_size())
                    continue;

                iWellNO = atoi(GetTorqShowValue(ptTorq, iWellIndex));
                if(iWellNO > 0)
                    break;
            }
            /* else iWellNO is 0*/
        }
    }

    /* 记录当前文件名及序号 */
    strNumInfo.Format("%s--%d", m_strDataFile.c_str(), m_nCurNO);
    SaveMessage(strNumInfo);

    m_nCurRunningNO = m_nCurNO + 1;
    if(iWellNO >= 0)
        m_nCurRunningNO = iWellNO + 1;

    return;
}

void CTorqueApp::SaveTorqNum()
{
    CFile           file;
    CFileFind       find;

    COMP_BFALSE(find.FindFile(m_strRegFile.c_str(), 0));

    file.Open(m_strRegFile.c_str(),CFile::modeRead|CFile::modeWrite|CFile::shareDenyNone,NULL);
    if(file.GetLength() == 0)
    {
        file.Close();
        return;
    }
    file.Seek(sizeof(DBREG),CFile::current);
    file.Write(&m_dwTotalTorqNum, sizeof(DWORD));
    file.Close();
}

/* IDS_STRDATEFORM         "2014-11-18_" */
BOOL CTorqueApp::TimeValidWell(CString strFileName)
{
    CString     strDate;
    int         iYear   = 0;
    int         iMonth  = 0;
    int         iDay    = 0;
    CTime       time=CTime::GetCurrentTime();//得到当前时间
    CTime       oldtime;
    CTimeSpan   tSpan;

    if(!g_tGlbCfg.bDateBehind) //  日期在前
    {
        strDate = strFileName;
    }
    else    /* "2014-11-18.pbd" */
    {
        strDate = strFileName.Right(14 );
    }
    iYear   = atoi(strDate.Left(4));
    strDate = strDate.Right(strDate.GetLength() - 5);
    iMonth  = atoi(strDate.Left(2));
    strDate = strDate.Right(strDate.GetLength() - 3);
    iDay    = atoi(strDate.Left(2));

    oldtime = CTime(iYear,iMonth,iDay,0,0,0,0);
    tSpan = time - oldtime;
    COMP_BLE_R(tSpan.GetDays(), 60, TRUE);

    /* else */
    return FALSE;
}

void  CTorqueApp::GetCurWellFile()
{
    WIN32_FIND_DATA findData;
    TCHAR       tcsExename[MAX_PATH];
    CString     strSearch;
    CString     strWell;
    HANDLE      hFindHandle;

    SetCurrentDirectory(m_strDataPath.c_str());

    /* 以井号保存文件 */
    strWell = m_tParaCfg.strValue[m_ptCurShow->nFileName].c_str();
    if (strWell.IsEmpty())
    {
        CreateNewWellFile();
        return;
    }

    if(!g_tGlbCfg.bDateBehind)
        strSearch = _T("*_") + strWell + _T(".pbd");
    else
        strSearch = strWell + _T("_*") + _T(".pbd");

    hFindHandle = FindFirstFile(strSearch, &findData);

    if ((hFindHandle != INVALID_HANDLE_VALUE))
    {
        GetLongPathName(findData.cFileName, tcsExename, MAX_PATH);
        /* 没有超过两个月 */
        if(TimeValidWell(tcsExename))
        {
            m_strDataFile = m_strDataPath + tcsExename;
            return;
        }
        
        /* else */
        while (FindNextFile(hFindHandle, &findData) != 0)
        {
            /* 没有超过两个月 */
            GetLongPathName(findData.cFileName, tcsExename, MAX_PATH);
            if(TimeValidWell(tcsExename))
            {
                m_strDataFile = m_strDataPath + tcsExename;
                return;
            }
        }
    }

    /*超过两个月，或者没有找到文件，生成新文件*/
    CreateNewWellFile();

    return;
}

string  CTorqueApp::GetQualityInfo(TorqData::Torque *ptTorq)
{
    int     i       = 0;
    DWORD   dwFlag  = 1;
    string  strQuality;
    //CString strInfo;
    DWORD   dwQuality = 0;

    ASSERT_NULL_R(ptTorq, strQuality);

    /* 测试版本，直接根据数据判断质量 */
    dwQuality = GetQuality(ptTorq);
    if(dwQuality & QUA_RESU_QUALITYBIT)
    {
        //strQuality.Format(IDS_STRMARKQUALITY);
        strQuality = LoadstringFromRes(IDS_STRMARKQUALITY);
    }
    else
    {
        strQuality = LoadstringFromRes(IDS_STRMARKDISQUAL);
        for(i=1; i<=MAX_BAD_CAUSE; i++)
        {
            dwFlag *= 2;
            if(dwQuality & dwFlag)
            {
                strQuality = LoadstringFromRes(IDS_STRMARKDISQUAL+i);
                break;
            }
        }
    }
    
    return strQuality;
}

int CTorqueApp::GetQualityIndex(TorqData::Torque *ptTorq)
{
    int     i       = 0;
    DWORD   dwFlag  = 1;
    DWORD   dwQuality = 0;

    ASSERT_NULL_R(ptTorq, -1);

    /* 测试版本，直接根据数据判断质量 */
    dwQuality = GetQuality(ptTorq);

    if(dwQuality & QUA_RESU_QUALITYBIT)
    {
        return QUA_RESU_GOOD;
    }
    
    for(i=1; i<=MAX_BAD_CAUSE; i++)
    {
        dwFlag *= 2;
        if(dwQuality & dwFlag)
        {
            return i;
        }
    }
    
    return QUA_RESU_BAD;
}

///////////////////////////////////////////////////////////////////////////////
//  void ExportListToExcel(CListCtrl* pList, CDatabase* ptDb, CString strTable)
//  参数：
//      pList       需要导出的List控件指针
//      ptDb        CDatabase指针
//      strTitle    导出的数据表标题
//  说明:
//      导出CListCtrl控件的全部数据到Excel文件。Excel文件名由用户通过“另存为”
//      对话框输入指定。创建名为strTitle的工作表，将List控件内的所有数据（包括
//      列名和数据项）以文本的形式保存到Excel工作表中。保持行列关系。
//
//  edit by [r]@dotlive.cnblogs.com
///////////////////////////////////////////////////////////////////////////////
void CTorqueApp::ExportListToExcel(CString strSheetName, CDatabase* ptDb, CListCtrl *ptlistData)
{
    // 创建表结构
    int         i           = 0;
    int         iItemIndex  = 0;
    int         iColNum     = 0;
    LVCOLUMN    tColData;
    CString     strColName;
    CString     strSql      = "";
    CString     strH        = "";
    CString     strV        = "";
    CString     strTemp;

    ASSERT_NULL(ptDb);
    ASSERT_NULL(ptlistData);

    tColData.mask = LVCF_TEXT;
    tColData.cchTextMax =100;
    tColData.pszText = strColName.GetBuffer (100);
    for(i=0; ptlistData->GetColumn(i,&tColData); i++)
    {
        if (i!=0)
        {
            strSql = strSql + ", " ;
            strH   = strH + ", " ;
        }
        strSql = strSql + " [" + tColData.pszText +"] TEXT";
        strH   = strH   + " [" + tColData.pszText +"] ";
    }
    strColName.ReleaseBuffer();
    iColNum = i;

    strTemp.Format(IDS_STRCREATETABLE, strSheetName);
    strSql = strTemp + strSql +  " ) ";
    ptDb->ExecuteSQL(strSql);

    // 插入数据项
    for (iItemIndex=0; iItemIndex<ptlistData->GetItemCount(); iItemIndex++)
    {
        strV = "";
        for(i=0; i<iColNum; i++)
        {
            if (i!=0)
            {
                strV = strV + ", " ;
            }
            strV = strV + " '" + ptlistData->GetItemText(iItemIndex,i) +"' ";
        }

        strTemp.Format(IDS_STRINSERTTABLE, strSheetName);
        strSql =  strTemp   + strH + ")" + 
                 " VALUES(" + strV + ")";
        ptDb->ExecuteSQL(strSql);
    }

    return;
}

//////////////////////////////////////////////////////////////////////////////
//名称：CheckExcelDriver
//功能：检查ODBC中Excel驱动是否有效
//作者：徐景周(jingzhou_xu@163.net)
//组织：未来工作室(Future Studio)
//日期：2002.9.1
/////////////////////////////////////////////////////////////////////////////
BOOL CTorqueApp::CheckExcelDriver(CString &strDriver)
{
    char    aucBuf[2001];
    WORD    wBufMax = 2000;
    WORD    wBufOut = 0;
    char    *pszBuf = aucBuf;
    CString strWarn;

    // 获取已安装驱动的名称(涵数在odbcinst.h里)
    COMP_BFALSE_R(SQLGetInstalledDrivers(aucBuf, wBufMax, &wBufOut), FALSE);

    // 检索已安装的驱动是否有Excel...
    do
    {
        if (strstr(pszBuf, "Excel") != 0)
        {
            //发现 !
            strDriver = CString(pszBuf);
            break;
        }
        pszBuf = strchr(pszBuf, '\0') + 1;
    }while (pszBuf[1] != '\0');

    if (strDriver.IsEmpty())
    {
        // 没有发现Excel驱动
        strWarn.Format(IDS_STRINFNODRIVE);
        SaveShowMessage(strWarn);
        return FALSE;
    }

    return TRUE;
}

//获得默认的文件名
//默认文件名：yyyymmddhhmmss.xls
BOOL CTorqueApp::GetDefaultXlsFileName(CString sDefTitle, CString& sExcelFile)
{
    CString strTime;
    CString strAllFilter;
    CString strTitle;
    CString strFilter;
    CTime   tDay = CTime::GetCurrentTime();

    strTime = sDefTitle;
    if(sDefTitle.IsEmpty())
    {
        strTime.Format( "%04d%02d%02d%02d%02d%02d",
                        tDay.GetYear(),      //yyyy年
                        tDay.GetMonth(),     //mm月份
                        tDay.GetDay(),       //dd日
                        tDay.GetHour(),      //hh小时
                        tDay.GetMinute(),    //mm分钟
                        tDay.GetSecond());   //ss秒
    }

    sExcelFile =  strTime + ".xls";

    // prompt the user (with all document templates)
    CFileDialog dlgFile(FALSE, ".xls", sExcelFile);

    strTitle.Format(IDS_STREXPORT);
    strFilter = "Excel File(*.xls)";
    strFilter += (TCHAR)'\0';   // next string please
    strFilter += _T("*.xls");
    strFilter += (TCHAR)'\0';   // last string
    dlgFile.m_ofn.nMaxCustFilter++;
    dlgFile.m_ofn.nFilterIndex = 1;

    // append the "*.*" all files filter
    VERIFY(strAllFilter.LoadString(AFX_IDS_ALLFILTER));
    strFilter += strAllFilter;
    strFilter += (TCHAR)'\0';   // next string please
    strFilter += _T("*.*");
    strFilter += (TCHAR)'\0';   // last string
    dlgFile.m_ofn.nMaxCustFilter++;

    dlgFile.m_ofn.lpstrFilter = strFilter;
    dlgFile.m_ofn.lpstrTitle  = strTitle;

    // open cancelled
    COMP_BE_R(dlgFile.DoModal(), IDCANCEL, FALSE);

    sExcelFile.ReleaseBuffer();

    sExcelFile = dlgFile.GetPathName();
    if (MakeSurePathExists(sExcelFile,true))
    {
        // delete the file
        if(!DeleteFile(sExcelFile))
        {
            AfxMessageBox(LoadstringFromRes(IDS_STRINFOVEREXLERR).c_str());
            return FALSE;
        }
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//  BOOL MakeSurePathExists( CString &Path,bool FilenameIncluded)
//  参数：
//      Path                路径
//      FilenameIncluded    路径是否包含文件名
//  返回值:
//      文件是否存在
//  说明:
//      判断Path文件(FilenameIncluded=true)是否存在,存在返回TURE，不存在返回FALSE
//      自动创建目录
//
///////////////////////////////////////////////////////////////////////////////
BOOL CTorqueApp::MakeSurePathExists( CString &Path,
                                     bool FilenameIncluded)
{
    int iPos = 0;
    
    while((iPos=Path.Find('\\',iPos+1))!=-1)
        CreateDirectory(Path.Left(iPos),NULL);

    if(!FilenameIncluded)
        CreateDirectory(Path,NULL);

    return !_access(Path,0);
}

BOOL CTorqueApp::SaveList2XlsFile(CString strFileName, CString strSheetName, CListCtrl *ptlistData)
{
    CDatabase   database;
    CString     strDriver;
    CString     strWarn;
    CString     strExcelFile;
    CString     strSql;

    ASSERT_NULL_R(ptlistData, FALSE);

    // 检索是否安装有Excel驱动 "Microsoft Excel Driver (*.xls)"
    COMP_BFALSE_R(CheckExcelDriver(strDriver), FALSE);

    // 如果strFileName为空，取默认文件名，按当前时间
    if (strFileName.IsEmpty())
    {
        COMP_BFALSE_R(GetDefaultXlsFileName(_T(""), strExcelFile), FALSE);
    }
    else
    {
        strExcelFile = strFileName;
        if (MakeSurePathExists(strFileName,true))
        {
            DeleteFile(strFileName);
        }
    }

    // 创建进行存取的字符串
    strSql.Format(IDS_STRCRTEXCDB, strDriver, strExcelFile, strExcelFile);

    // 创建数据库 (Excel表格文件)
    if(! database.OpenEx(strSql,CDatabase::noOdbcDialog) )
    {
        strWarn.Format(IDS_STRINFCRTXLSFAIL);
        SaveShowMessage(strWarn);
        // 关闭数据库
        database.Close();
        return FALSE;
    }

    ExportListToExcel(strSheetName, &database, ptlistData);

    // 关闭数据库
    database.Close();

    strWarn.Format(IDS_STRINFSAVEXLSUCC,strExcelFile);
    SaveShowMessage(strWarn);

    return TRUE;
}

void CTorqueApp::ShowMainTitle()
{
    string strAppName;
    CWnd    *m_pCWnd = AfxGetMainWnd();

    if(m_tdbReg.bReged)
    {
        strAppName = LoadstringFromRes(IDS_STRTITLE);
        //strAppName.Format(IDS_STRTITLE);
        if(g_tGlbCfg.bBigTorq)
            strAppName = LoadstringFromRes(IDS_STRBIGTITLE);
            //strAppName.Format(IDS_STRBIGTITLE);
        ::SetWindowText(*m_pCWnd,strAppName.c_str());
        return;
    }
    
    /* else */
    strAppName = LoadstringFromRes(IDS_STRTRYOUT);
    //strAppName.Format(IDS_STRTRYOUT);
    if(g_tGlbCfg.bBigTorq)
        strAppName = LoadstringFromRes(IDS_STRBIGTRYOUT);
        //strAppName.Format(IDS_STRBIGTRYOUT);

    ::SetWindowText(*m_pCWnd,strAppName.c_str());
    return;
}

/* 无效数据直接跳过，节省读取时间
   无效数据现在都以"####"开头
   CRC数据也不计数了 */
BOOL CTorqueApp::IsDebugInfo(CString strContent)
{
    int     i       = 0;
    int     iPlace  = -1;

    COMP_BTRUE_R(strContent.IsEmpty(), FALSE);
    
    for(i=0; i<DBG_MAXNUM; i++)
    {
        iPlace = strContent.Find(m_strDbgHead[i]);

        COMP_BNE_R(iPlace, -1, TRUE);
    }

    return FALSE;
}

void CTorqueApp::SaveCurTime()
{
    char        *pData = NULL;
    int         iLen   = 0;
    SYSTEMTIME  ts;

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    GetLocalTime(&ts);

    iLen  = sprintf_s(pData, SPRINTFLEN, "%02d:%02d:%02d.%03d : ",
                           ts.wHour,ts.wMinute, ts.wSecond, ts.wMilliseconds);
    INC_DBG_INFO();

    return;
}

void CTorqueApp::SaveDbgHead(UINT nType)
{
    char    *pData  = NULL;
    int     iLen    = DBG_HEADLEN;

    COMP_BGE(nType, DBG_MAXNUM);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    memcpy(pData, m_strDbgHead[nType], DBG_HEADLEN);
    INC_DBG_INFO();

    return;
}

void CTorqueApp::SaveCurTimeAndHead(UINT nType)
{
    /* Time */
    SaveCurTime();

    /* debug head */
    SaveDbgHead(nType);
}

void CTorqueApp::SaveStreamData(CString strStream)
{
    char    *pData  = NULL;
    int     iLen    = 0;
    
    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];
    iLen = strStream.GetLength();
    
    iLen = sprintf_s(pData, SPRINTFLEN, "%s", strStream.GetBuffer());
    INC_DBG_INFO();
}

void CTorqueApp::SaveSerialData(BYTE *pucRcvByte, WORD wLen)
{
    int     i       = 0;
    int     iLen    = 0;
    char    *pData  = NULL;

    ASSERT_NULL(pucRcvByte);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    /* source serial data */
    for(i=0; i<wLen; i++)
    {
        iLen = sprintf_s(pData, SPRINTFLEN, "%02x ", pucRcvByte[i]);
        INC_DBG_INFO();
    }

    iLen = 2;
    memcpy(pData, "\r\n", iLen);
    INC_DBG_INFO();
}

/* 保存正常的原始数据，测试和定位使用
   正常有效数据，不需要DEBUG HEAD
   在CTRL+SHIFT+T 显示CRC 框后生效 */
void CTorqueApp::SaveOrdData(ORGDATA *ptOrgData,BYTE *pucRcvByte, WORD wLen)
{
    char    *pData = NULL;
    int     iLen   = 0;

    ASSERT_NULL(ptOrgData);

    COMP_BFALSE(m_bShowCRC);

    /* Time */
    SaveCurTime();

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    /* 原始串口的扭矩和脉冲数据 */
    if(wLen == PORT_VLDVLVLEN)  // 打印阀门的2个状态字节
    {
        iLen  = sprintf_s(pData, SPRINTFLEN, "%02x %02x %02x %02x %02x %02x %02x %02x %02x ", 
                                            ptOrgData->ucRcvByte[0],
                                            ptOrgData->ucRcvByte[1],
                                            ptOrgData->ucRcvByte[2],
                                            ptOrgData->ucRcvByte[3],
                                            ptOrgData->ucRcvByte[4],
                                            ptOrgData->ucRcvByte[5],
                                            ptOrgData->ucRcvByte[6],
                                            ptOrgData->ucRcvByte[7],
                                            ptOrgData->ucRcvByte[8]);
    }
    else
    {
        iLen  = sprintf_s(pData, SPRINTFLEN, "%02x %02x %02x %02x %02x %02x %02x ", 
                                            ptOrgData->ucRcvByte[0],
                                            ptOrgData->ucRcvByte[1],
                                            ptOrgData->ucRcvByte[2],
                                            ptOrgData->ucRcvByte[3],
                                            ptOrgData->ucRcvByte[4],
                                            ptOrgData->ucRcvByte[5],
                                            ptOrgData->ucRcvByte[6]);
    }
    INC_DBG_INFO();

    /* 计算的扭矩等信息 */
    iLen  = sprintf_s(pData, 60, "T%10d, P%10d, S%4d, N%4d, R%6.2f\r\n",
                            ptOrgData->nTorque,
                            ptOrgData->nPlus,
                            ptOrgData->ucStatus,
                            ptOrgData->ucPointNum,
                            ptOrgData->fRpm);
    INC_DBG_INFO();

    /* 有效数据的长度应该大于12，有长度为24的数据，也可以尽可能取出一条来，
       但是前12个数据通过CRC校验，则当做普通数据处理，调试开关打开时也将原始数据保存下来 */
    if(wLen != PORT_VALIDLEN && wLen != PORT_VLDVLVLEN)
    {
        /* Time */
        SaveCurTimeAndHead(DBG_HASH);

        pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

        /* data len */
        iLen = sprintf_s(pData,SPRINTFLEN, " ORG DATA(len%2d) ", wLen);
        INC_DBG_INFO();

        /* source serial data */
        SaveSerialData(pucRcvByte, wLen);
    }
    return;
}

/* 保存正常的原始数据，测试和定位使用
   正常有效数据，不需要DEBUG HEAD
   在CTRL+SHIFT+T 显示CRC 框后生效 */
void CTorqueApp::SaveMultiData(ORGDATA *ptOrgData,BYTE *pucRcvByte, WORD wLen)
{
    char    *pData = NULL;
    int     iLen   = 0;
    int     i      = 0;
    WORD    tmpTorq= 0;

    ASSERT_NULL(ptOrgData);

    COMP_BFALSE(m_bShowCRC);

    /* Time */
    SaveCurTime();

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];
    
    iLen  = sprintf_s(pData, SPRINTFLEN, "%02x %02x ", 0xFF, 0x15);
    INC_DBG_INFO();

    for(i=4; i<wLen; i++)
    {
        iLen  = sprintf_s(pData, SPRINTFLEN, "%02x ", pucRcvByte[i]);
        INC_DBG_INFO();
    }

    iLen = 2;
    memcpy(pData, "\r\n", iLen);
    INC_DBG_INFO();
    
    return;
}

BOOL CTorqueApp::MsgLenIsZero(WORD wLen, UINT nType)
{
    char    *pData = NULL;
    int     iLen   = 0;

    COMP_BNE_R(wLen, 0, FALSE);

    /* 只有m_bShowCRC打开，才记录长度为0的数据 */
    COMP_BFALSE_R(m_bShowCRC, TRUE);

    SaveCurTimeAndHead(nType);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    /* Save Info */
    iLen = sprintf_s(pData, SPRINTFLEN, " Recv Len 0 Data!\r\n");
    INC_DBG_INFO();

    return TRUE;
}

/* 保存CRC错误的原始数据，测试和定位使用
   有CRC错误都会自动保存
   CRC或者串口错误、长度不对数据 */
void CTorqueApp::SaveCrcErrorData(BYTE *pucRcvByte, WORD wLen, UINT &nCRCErr)
{
    char    *pData = NULL;
    int     iLen   = 0;
    CTorqueDlg* pdlgMain = (CTorqueDlg*)m_pMainWnd;

    ASSERT_NULL(pucRcvByte);

    COMP_BNE(g_tGlbCfg.nTest, COLL_PORT);

    /* 消息长度为0，处理后直接返回 */
    COMP_BTRUE(MsgLenIsZero(wLen, DBG_HASH));

    /* 对话框中的CRC错误数目+1 */
    nCRCErr ++;

    SaveCurTimeAndHead(DBG_HASH);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];
    /* data len */
    iLen = sprintf_s(pData, SPRINTFLEN, " CRC  Error(len%2d) ", wLen);
    INC_DBG_INFO();

    /* source serial data */
    SaveSerialData(pucRcvByte, wLen);

    return;
}

/* 采集数据时出错信息保存 */
void CTorqueApp::SaveCollectErrorData(CString strError, BYTE *pucRcvByte, WORD wLen)
{
    char    *pData = NULL;
    int     iLen   = 0;
    int     i      = 0;

    ASSERT_NULL(pucRcvByte);//m_wRcvLen

    COMP_BNE(g_tGlbCfg.nTest, COLL_PORT);

    /* 消息长度为0，处理后直接返回 */
    COMP_BTRUE(MsgLenIsZero(wLen, DBG_COLLECT));

    SaveCurTimeAndHead(DBG_COLLECT);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];
    /* Info and data len */
    iLen = sprintf_s(pData, SPRINTFLEN, " %s (len%2d) ", (LPCTSTR)strError, wLen);
    INC_DBG_INFO();

    /* source serial data */
    SaveSerialData(pucRcvByte, wLen);

    return;
}

/* 保存采集数据普通信息 */
void CTorqueApp::SaveCollectOrgData(BYTE *pucRcvByte, WORD wLen)
{
    char    *pData = NULL;
    int     iLen   = 0;
    int     i      = 0;

    ASSERT_NULL(pucRcvByte);
    /* COMP_BFALSE(m_bShowCRC); */

    SaveCurTimeAndHead(DBG_COLLECT);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];
    /* data len */
    iLen = sprintf_s(pData, SPRINTFLEN, " ORG  Data(len%2d) ", wLen);
    INC_DBG_INFO();

    /* source serial data */
    SaveSerialData(pucRcvByte, wLen);

    return;
}

//保存复位时的串口数据
void CTorqueApp::SaveResetData(BYTE *pucRcvByte, WORD wLen)
{
    char    *pData = NULL;
    int     iLen   = 0;
    int     i      = 0;

    ASSERT_NULL(pucRcvByte);

    COMP_BFALSE(m_bShowCRC);
    COMP_BNE(g_tGlbCfg.nTest, COLL_PORT);

    SaveCurTimeAndHead(DBG_START);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];
    /* data len */
    iLen = sprintf_s(pData, SPRINTFLEN, " Reset Data(len%2d) ", wLen);
    INC_DBG_INFO();

    /* source serial data */
    SaveSerialData(pucRcvByte, wLen);

    return;
}

//保存上位机发送到串口数据
void CTorqueApp::SaveSendData(CString strCmd, BYTE *pucRcvByte, WORD wLen)
{
    char    *pData = NULL;
    int     iLen   = 0;
    int     i      = 0;

    ASSERT_NULL(pucRcvByte);

    /* COMP_BFALSE(m_bShowCRC); */

    SaveCurTimeAndHead(DBG_SNDCMD);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    /* data len */
    iLen = sprintf_s(pData, SPRINTFLEN, " %s(len%2d)\r\n", (LPCTSTR)strCmd, wLen);
    INC_DBG_INFO();
    
    /* 对外不展现modbus编码 */
    return;

    /* source serial data */
    SaveSerialData(pucRcvByte, wLen);
}

//保存串口发送错误数据
void CTorqueApp::SaveSendFailure(UINT nCmdType)
{
    char    *pData = NULL;
    int     iLen   = 0;
    string  strCmd;

    COMP_BNE(g_tGlbCfg.nTest, COLL_PORT);

    SaveCurTimeAndHead(DBG_HASH);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];
    strCmd = g_strCmdName[nCmdType-SCMREAD];

    /* Save Info */
    iLen = sprintf_s(pData, SPRINTFLEN, " Send %s Command Failure!\r\n", strCmd.c_str());
    INC_DBG_INFO();
    return;
}

void CTorqueApp::SaveLogInfo()
{
    COMP_BL(m_tSaveLog.iCur, 1);

    m_SaveLogFile.SeekToEnd();
    m_SaveLogFile.Write(m_tSaveLog.aucLog, m_tSaveLog.iCur);
    m_tSaveLog.iCur = 0;

    return;
}

//保存程序运行状态
void CTorqueApp::SaveAppStatus(UINT nStatus, CString strInfo)
{
    char    *pData = NULL;
    int     iLen   = 0;

    COMP_BGE(nStatus, STATUS_MAXNUM);

    /* Send app status Time */
    SaveCurTimeAndHead(DBG_HASH);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    /* Save Info */
    iLen = sprintf_s(pData, 100, " Application is %s Status(%s)!\r\n", (LPCTSTR)g_strStatus[nStatus].c_str(), (LPSTR)(LPCTSTR)strInfo);
    INC_DBG_INFO();
    return;
}

//保存端口操作记录 打开/关闭
void CTorqueApp::SavePortOper(UINT nPortOpr)
{
    char    *pData = NULL;
    int     iLen   = 0;
    
    COMP_BGE(nPortOpr, PORTOPR_MAXNUM);

    /* Send port operation Time */
    SaveCurTimeAndHead(DBG_HASH);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    /* Save Info */
    iLen = sprintf_s(pData, SPRINTFLEN, " Port %d is %s!\r\n", g_tGlbCfg.nPortNO, (LPCTSTR)g_strPortOpr[nPortOpr].c_str());
    INC_DBG_INFO();
    return;
}

//保存发送消息时的异常的串口缓冲数据
void CTorqueApp::SavePortBufData(BYTE *pucRcvByte, WORD wLen, UINT nClashSta)
{
    char    *pData = NULL;
    int     iLen   = 0;
    int     i      = 0;

    ASSERT_NULL(pucRcvByte);
    ASSERT_ZERO(wLen);

    /* data Time */
    SaveCurTimeAndHead(DBG_HASH);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    /* data len */
    if(nClashSta == RS_READCLASH)
        iLen = sprintf_s(pData, SPRINTFLEN, " ReadCls  Data(len%2d) ", wLen);
    else if(nClashSta == RS_RESETCLASH)
        iLen = sprintf_s(pData, SPRINTFLEN, " ResetCls Data(len%2d) ", wLen);
    INC_DBG_INFO();

    /* source serial data */
    SaveSerialData(pucRcvByte, wLen);

    return;
}

/* 保存MessageBox显示的信息到文件 */
void CTorqueApp::SaveMessage(CString strMessage)
{
    char    *pData = NULL;
    int     iLen   = 0;

    /* Send MessageBox Time */
    SaveCurTimeAndHead(DBG_MESSAGE);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    /* Save Info */
    iLen = sprintf_s(pData, 200, " %s\r\n", (LPCTSTR)strMessage);
    INC_DBG_INFO();
    return;
}

void CTorqueApp::SaveShowMessage(CString strMessage, UINT nType)
{
    SaveMessage(strMessage);
    AfxMessageBox(strMessage, nType);
}

void CTorqueApp::StringSubtract(CString& strValue, BYTE ucChar)
{
    int  i = 0;

    for(i=0; i<strValue.GetLength(); i++)
    {
        strValue.SetAt(i,strValue.GetAt(i) - ucChar);
    }
}

void CTorqueApp::SplitRegString(CString strReg[], CString strRegCode)
{
    int  i = 0;

    ASSERT_NULL(strReg);

    for(i=0; i<REGCODESEGNUM; i++)
    {
        strReg[i] = strRegCode.Left(g_nValidLen[i]);
        strRegCode.Delete(0, g_nValidLen[i]);
    }
}

void CTorqueApp::MergeRegString(CString strReg[], CString& strRegCode)
{
    int  i = 0;

    ASSERT_NULL(strReg);

    for(i=0; i<REGCODESEGNUM; i++)
    {
        strRegCode += strReg[i];
    }
}

void CTorqueApp::AdaptDlgCtrlSize(CDialog *pdlgAdapt, UINT nSheetType)
{
    double      fRatiox = 1,fRatioy = 1,fRatio= 1;
    int         woc;
    POINT       Newp;
    CRect       oldRect;
    CRect       newRect;
    CRect       Rect;
    //左右上角的数据  
    CPoint      OldTLPoint, NewTLPint;  
    CPoint      OldBRPoint, NewBRPint;
    CFont       *pFont = NULL;
    UINT        nPageHeight = 0;

    ASSERT_NULL(pdlgAdapt);

    if(nSheetType == 2)
    {
        //nPageWidth = 20 * m_ucDPILevel / 4;
        nPageHeight = 120 * m_ucDPILevel / 4;
    }

    pdlgAdapt->GetClientRect(&oldRect);                        /* 对话框大小 */

    COMP_BGE(m_nScreenY, (oldRect.Height()+nPageHeight));

    fRatiox = 1.0 * m_nScreenX / oldRect.Width();
    fRatioy = 1.0 * m_nScreenY / (oldRect.Height() + nPageHeight);
    fRatio = (fRatiox < fRatioy) ? fRatiox : fRatioy;
    if(nSheetType == 1)
    {
        fRatio = m_fSheetRatio+0.05;
    }
    else if(nSheetType == 2)
    { 
        if(m_fSheetRatio != 1.0)
        {
            fRatio = m_fSheetRatio;
        }
        else
        {
            m_fSheetRatio = fRatio;
        }
    }

    Newp.x = int(oldRect.Width()*fRatio);
    Newp.y = int(oldRect.Height()*fRatio);

    /* 对话框先缩小 */
    pdlgAdapt->MoveWindow(0, 0, Newp.x, Newp.y);
    pdlgAdapt->CenterWindow();

    //获取现在对话框的大小 
    pdlgAdapt->GetClientRect(&newRect);     //取客户区大小  
    HWND  hwndChild = ::GetWindow(pdlgAdapt->m_hWnd,GW_CHILD);  
  
    while(hwndChild)  
    {  
        //取得ID  
        woc  = ::GetDlgCtrlID(hwndChild);  
        pdlgAdapt->GetDlgItem(woc)->GetWindowRect(Rect);  
        pdlgAdapt->ScreenToClient(Rect);  

        Rect.left  = Rect.left * newRect.Width() / oldRect.Width();
        Rect.right = Rect.right * newRect.Width() / oldRect.Width();
        Rect.top = Rect.top * newRect.Height() / oldRect.Height();
        Rect.bottom = Rect.bottom * newRect.Height() / oldRect.Height();
        pdlgAdapt->GetDlgItem(woc)->MoveWindow(Rect, TRUE);

        hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);  
    }
}

void CTorqueApp::ReOpenWindow()
{
    CTorqueDlg dlgMain;

    m_pMainWnd = &dlgMain;
    dlgMain.DoModal();
}

/* 播放3s告警，放一次，停300ms，播放10次 */
void CTorqueApp::PlayAlarmSound()
{
    PlaySound((LPCTSTR)SND_ALIAS_SYSTEMEXCLAMATION, NULL, SND_ALIAS_ID|SND_ASYNC|SND_LOOP);
}

void CTorqueApp::StopAlarmSound()
{
    PlaySound(NULL, 0, 0);
}

/* 当前位置往前找iInterval个点，找到<=0的位置 */
WORD CTorqueApp::GetIPPlace(int iCurPnt, int iInterval)
{
    int     i       = 0;

    for(i = iCurPnt -1; i>(iCurPnt-1-iInterval) && i>0; i--)
    {
        if(m_fAdjInfPnt[i] <= 0)
            return i;
    }

    return 0;
}

/* 获取或者查找拐点的位置 */
/* 在离控制扭矩0.1周内查找拐点，如果找不到则不找
   daqing: 无论如何找到一个拐点，后续根据拐点位置判别是否合格 */
/* 20200306拐点扭矩选择范围: 最佳扭矩的10~~70%, 圈数差值 0.01~0.25 */
WORD CTorqueApp::SearchIPPoint(TorqData::Torque *ptTorq, BOOL bCheckIP)
{
    int     i         = 0;
    int     j         = 0;
    int     iIPBegin  = 0;
    int     iIPEnd    = 0;
    WORD    wIPPos    = 0;
    int     iInterval = 5;
    double  fCtrlTor  = 0;
    WORD    wTmpPos   = 0;
    double  fTmpAdjInfPnt = 0;
    double  fStandValue = 0.01;
    double  fBgnTorqPct  = 0.3; // 40%
    double  fChkIntSlope = 0.04;
    int     iSearchPnt = 0;

    ASSERT_NULL_R(ptTorq, 0);
    ASSERT_ZERO_R(ptTorq->ftorque_size(), 0);
    COMP_BTRUE_R(ptTorq->btimeline(), 0);
    /* 斜率比为0，不需要找拐点，直接返回 */

    GET_CTRL_TORQ(fCtrlTor, ptTorq);
    ASSERT_ZERO_R(fCtrlTor, 0);
    COMP_BFALSE_R(bCheckIP, 0);

    /* 获得拐点扭矩 */
    wIPPos = ptTorq->dwippos();
    if(wIPPos > 0 && wIPPos < ptTorq->ftorque_size())
    {
        return wIPPos;
    }

    /* 在扭矩曲线上取任意相邻两点连线斜率与之前相隔若干点的相邻两点连线斜率的比值达到一定的倍数时，
       确定所取两点之一为拐点。 */
    /**  斜率的斜率/控制扭矩 >1%  / iInterval(5) 
        往回找相邻点斜率的斜率的最大值 
        如果最大相邻拐点 大于 斜率的斜率的50%，则该最大值为拐点；否则斜率的斜率前一个点为拐点*/
    memset(m_fAdjSlope, 0, sizeof(double)*COLLECTPOINTS);   //相邻点斜率
    memset(m_fAdjInfPnt, 0, sizeof(double)*COLLECTPOINTS);  //相邻点拐点
    memset(m_fIntSlope, 0, sizeof(double)*COLLECTPOINTS);   //间隔点斜率
    memset(m_fIntInfPnt, 0, sizeof(double)*COLLECTPOINTS);  //间隔点拐点
    
    fStandValue = 0.009;
    if(ptTorq->fmaxcir() != 5)
        fStandValue = fStandValue * atan(ptTorq->fmaxcir()) / atan(5.0) *0.95;

    //iInterval = (UINT)(iInterval * 5 / ptTorq->fmaxcir());
    /* 扭矩点数小于间隔，直接返回0 */
    COMP_BLE_R(ptTorq->ftorque_size(), iInterval, 0);

    iSearchPnt = iInterval;
    m_fAdjSlope[iInterval] = (ptTorq->ftorque(iInterval) - ptTorq->ftorque(iInterval-1))/fCtrlTor;
    m_fIntSlope[iInterval] = (ptTorq->ftorque(iInterval) - ptTorq->ftorque(1))/fCtrlTor;
    for(i=iInterval+1; i<int(ptTorq->ftorque_size()-1); i++)
    {
        if(iSearchPnt == iInterval && ptTorq->ftorque(i) > fBgnTorqPct * fCtrlTor)
            iSearchPnt = i;
        m_fAdjSlope[i]  = (ptTorq->ftorque(i) - ptTorq->ftorque(i-1))/fCtrlTor;
        //m_fAdjSlope[i]  = (ptTorq->ftorque(i) - ptTorq->ftorque(i-1));
        m_fAdjInfPnt[i] = m_fAdjSlope[i] - m_fAdjSlope[i-1];

        m_fIntSlope[i]  = (ptTorq->ftorque(i) - ptTorq->ftorque(i-iInterval+1))/fCtrlTor;
        //m_fIntSlope[i]  = (ptTorq->ftorque(i) - ptTorq->ftorque(i-iInterval+1))/(iInterval-1);
        m_fIntInfPnt[i] = m_fIntSlope[i] - m_fIntSlope[i-1];

        if(fabs(m_fAdjInfPnt[i]) < 0.00001) m_fAdjInfPnt[i] = 0;
        if(fabs(m_fIntInfPnt[i]) < 0.00001) m_fIntInfPnt[i] = 0;
    }
#if 0    
    /* 从控制扭矩往回退0.1周 */
    nIPBegin = (UINT)(ptTorq->ftorque_size() - 0.15 * MAXLINEITEM / ptTorq->fmaxcir());
    nIPEnd   = (UINT)(ptTorq->ftorque_size() - 0.03 * MAXLINEITEM / ptTorq->fmaxcir());
    if (nIPBegin < iInterval)
        nIPBegin = iInterval;
#else
    //nIPBegin = iInterval;
#endif

#if 1
    /*  new test argorithm */
    /* 圈数差值 0.01~0.25 */
    iIPBegin = (int)(ptTorq->ftorque_size() - 0.25 * MAXLINEITEM / ptTorq->fmaxcir());

    /* 先从40%控制扭矩位置往后找 */
    if(iIPBegin < iSearchPnt)
        iIPBegin = iSearchPnt;
    if(iIPBegin < iInterval)
        iIPBegin = iInterval;
    for(i= iIPBegin; i<ptTorq->ftorque_size()-1; i++)
    {
        if (m_fIntSlope[i] < fChkIntSlope)
            continue;

        wTmpPos = GetIPPlace(i, iInterval);
        if(wTmpPos > 0)
        {
            wIPPos = wTmpPos;
            break;
        }
    }
    COMP_BG_R(wIPPos, 0, wIPPos);

    /* 找不到，再从40%控制扭矩位置往前找 */
    iIPBegin = (UINT)(ptTorq->ftorque_size() - 0.25 * MAXLINEITEM / ptTorq->fmaxcir());
    if(iIPBegin < iInterval)
        iIPBegin = iInterval;
    
    for(i= iSearchPnt-1; i>iIPBegin; i--)
    {
        if (m_fIntSlope[i] < fChkIntSlope)
            continue;

        wTmpPos = GetIPPlace(i, iInterval);
        if(wTmpPos > 0)
        {
            wIPPos = wTmpPos;
            break;
        }
    }
#else
    /*  old argorithm */
    /* 圈数差值 0.01~0.25 */
    iIPBegin = (UINT)(ptTorq->ftorque_size() - 0.25 * MAXLINEITEM / ptTorq->fmaxcir());
    if(iIPBegin < iInterval)
        iIPBegin = iInterval;
    for(i= iIPBegin +1; i<int(ptTorq->ftorque_size()-1); i++)
    {
        if (m_fIntInfPnt[i] < fStandValue)
            continue;

        wTmpPos = i - 1;
        fTmpAdjInfPnt = 0;
        for (j = i - 1; j >= int(i - iInterval + 1); j--)
        {
            if (fTmpAdjInfPnt < m_fAdjInfPnt[j])
            {
                fTmpAdjInfPnt = m_fAdjInfPnt[j];
                wTmpPos = j;
            }
        }

        if(m_fAdjInfPnt[wTmpPos] < m_fAdjInfPnt[i]*0.5)
            wTmpPos = i - 1;
        /* 最佳扭矩的10~~70% */
        if (ptTorq->ftorque(wTmpPos) < (0.1*fCtrlTor))
            continue;
        if (ptTorq->ftorque(wTmpPos) > (0.7*fCtrlTor))
            return 0;

        wIPPos = wTmpPos;
        break;
    }
#endif
    return wIPPos;
}

/* Tn+2 - Tn > Delta 110 
   Diff / CtrlTorq > 0.03 两个0.07 */
/* 20200311 最佳扭矩的5~70%, 圈数差值 0.01~0.25 */
WORD CTorqueApp::SearchDeltaIP(TorqData::Torque *ptTorq, BOOL bCheckIP)
{
    int     i         = 0;
    int     iIPBegin  = 0;
    WORD    wIPPos    = 0;
    double  fBestTorq = 0;
    int     iInterval = 2;
    WORD    wTmpPos   = 0;
    //double  fLowValue = 0.07;//0.03;
    //double  fHighValue = 0.35;//0.05;
    //double  fDeltaVal   = 0.1;//0.07;
    int     iFoundNum   = 0;
    int     iMinIPPnt   = 10;
    
    ASSERT_NULL_R(ptTorq, 0);
    ASSERT_ZERO_R(ptTorq->ftorque_size(), 0);
    COMP_BTRUE_R(ptTorq->btimeline(), 0);
    
    COMP_BFALSE_R(bCheckIP, 0);

    if(VERSION_RECPLUS(ptTorq))
        return SearchIP4RECPLUS(ptTorq);

#if 0
    /* 获得拐点扭矩 */
    wIPPos = ptTorq->dwippos();
    if(wIPPos > 0 && wIPPos < ptTorq->ftorque_size())
    {
        return wIPPos;
    }
#endif

    COMP_BLE_R(ptTorq->ftorque_size(), iMinIPPnt, 0)
    fBestTorq = GetOptTorq(ptTorq);
    ASSERT_ZERO_R(fBestTorq, 0);

    /* 相差两个点的扭矩差/ 控制扭矩 > 0.03 两次；从第6个数据开始，找到往回走一步 */
    memset(m_fAdjSlope, 0, sizeof(double)*COLLECTPOINTS);   //相邻点斜率


    m_fAdjSlope[iInterval] = (ptTorq->ftorque(iInterval) - ptTorq->ftorque(0))/fBestTorq;
    for(i=iInterval+1; i<int(ptTorq->ftorque_size()-1); i++)
    {
        m_fAdjSlope[i]  = (ptTorq->ftorque(i) - ptTorq->ftorque(i-iInterval))/fBestTorq;
    }

    /* 圈数差值 0.01~0.25 */
    iIPBegin = (int)(ptTorq->ftorque_size() - 0.25 * MAXLINEITEM / ptTorq->fmaxcir());
    if(iIPBegin < iMinIPPnt)
        iIPBegin = iMinIPPnt;
    for(i= iIPBegin; i<ptTorq->ftorque_size()-1; i++)
    {
        /*if (m_fAdjSlope[i] < fLowValue)
        {
            iFoundNum = 0;
            continue;
        }
        iFoundNum ++;*/

        //if(iFoundNum >= 2 || m_fAdjSlope[i] > fHighValue)
        //if((m_fAdjSlope[i] + m_fAdjSlope[i+1]) > 2*fLowValue || m_fAdjSlope[i] > fHighValue)
        if((m_fAdjSlope[i] + m_fAdjSlope[i+1]) >= g_tGlbCfg.fIPDeltaVal)
        {
            wTmpPos = i - 1;
            //wTmpPos = i;

            /* 最佳扭矩的5~70% */
            /* 小于5%，往后找 */
            if (ptTorq->ftorque(wTmpPos) < (0.05*fBestTorq))
            {
                wIPPos = wTmpPos - 1;
                continue;
            }
            /* 大于70%，往前一个,跳出 */
            if (ptTorq->ftorque(wTmpPos) > (0.7*fBestTorq))
                wIPPos = wTmpPos - 1;

            /* 找到 */
            wIPPos = wTmpPos;
            break;
        }
    }

    return wIPPos;
}

/*  ΔTorq / ΔCir > ??
    0.01圈的扭矩值差值,和控制扭矩进行比值，然后进行比较，即ΔCir=0.01时的扭矩差值
*/
WORD  CTorqueApp::SearchIP4RECPLUS(TorqData::Torque *ptTorq)
{
    int     i           = 0;
    int     j           = 0;
    int     iIPBegin    = 0;
    WORD    wIPPos      = 0;
    double  fBestTorq   = 0;
    WORD    wTmpPos     = 0;
    int     iInterval   = 5;
    double  fDelTorq    = 0;
    double  fDelta      = 0;
    UINT    nBeginPlus  = 0;
    UINT    nCurPlus    = 0;
    double  fCompPlus   = 0;
    int     iDelPlus    = 0;
    
    ASSERT_NULL_R(ptTorq, 0);
    ASSERT_ZERO_R(ptTorq->ftorque_size(), 0);
    COMP_BTRUE_R(ptTorq->btimeline(), 0);
    COMP_BFALSE_R(VERSION_RECPLUS(ptTorq), 0);

    COMP_BLE_R(ptTorq->ftorque_size(), iInterval, 0);
    fBestTorq = GetOptTorq(ptTorq);
    ASSERT_ZERO_R(fBestTorq, 0);

    /* 圈数差值 0.01~0.25 */
    nCurPlus   = ptTorq->dwtotalplus();
    fCompPlus  = ptTorq->fplus() / 100; /* 0.01圈 */
    nBeginPlus = (UINT)(ptTorq->dwtotalplus() - 0.25 * ptTorq->fplus());
    for(i=ptTorq->ftorque_size()-1; i>0; i--)
    {
        nCurPlus -= ptTorq->dwdelplus(i);
        if(nCurPlus < nBeginPlus)
        {
            iIPBegin = i - 1;
            break;
        }
    }
    if(iIPBegin < iInterval)
        iIPBegin = iInterval;
    for(i= iIPBegin; i<ptTorq->ftorque_size()-1; i++)
    {
        if (0 == ptTorq->dwdelplus(i))
            continue;
        iDelPlus = 0;
        for(j=i; j>0; j--)
        {
            iDelPlus += ptTorq->dwdelplus(i);
            if(iDelPlus > fCompPlus)
                break;
        }
        if(j == 0)
            continue;

        fDelTorq = ptTorq->ftorque(i) - ptTorq->ftorque(j);
        fDelta   = fDelTorq / fBestTorq;
        if(fDelta >= g_tGlbCfg.fIPDeltaVal)
        {
            wTmpPos = i - 1;

            /* 最佳扭矩的5~70% */
            /* 小于5%，往后找 */
            if (ptTorq->ftorque(wTmpPos) < (0.05*fBestTorq))
            {
                wIPPos = wTmpPos - 1;
                continue;
            }
            /* 大于70%，往前一个,跳出 */
            if (ptTorq->ftorque(wTmpPos) > (0.7*fBestTorq))
                wIPPos = wTmpPos - 1;

            /* 找到 */
            wIPPos = wTmpPos;
            break;
        }
    }

    return wIPPos;
}

int CTorqueApp::GetIPPlus(TorqData::Torque *ptTorq, WORD wIPPos)
{
    int     i       = 0;
    int     iIPPlus = 0;

    ASSERT_NULL_R(ptTorq, iIPPlus);
    ASSERT_ZERO_R(wIPPos, iIPPlus);
    ASSERT_ZERO_R(ptTorq->dwdelplus_size(), iIPPlus);
    COMP_BGE_R(wIPPos, (ptTorq->dwdelplus_size()-1), iIPPlus);
    
    iIPPlus = ptTorq->dwtotalplus();

    for(i=ptTorq->dwdelplus_size()-1; i>wIPPos; i--)
    {
        iIPPlus -= ptTorq->dwdelplus(i);
    }
    COMP_BLE_R(iIPPlus, 0, 0);

    return iIPPlus;
}

BOOL CTorqueApp::SetIPInfo(TorqData::Torque *ptTorq, double fIPTorq)
{
    int     i       = 0;
    WORD    wIPPos  = 0;
    WORD    wIPPlus = 0;

    ASSERT_NULL_R(ptTorq, FALSE);
    ASSERT_ZERO_R(ptTorq->ftorque_size(), FALSE);
    COMP_BGE_R(fIPTorq, ptTorq->ftorque(ptTorq->ftorque_size()-1), FALSE);

    // ipTorq为0或者wIPPos为0
    if(fIPTorq <= 0 || fIPTorq == ptTorq->ftorque(0)) 
    {
        ptTorq->set_dwippos(0);
        ptTorq->set_dwiptorq(0);
        ptTorq->set_dwipplus(0);
        return TRUE;
    }

    for(i=0; i<ptTorq->ftorque_size(); i++)
    {
        if(ptTorq->ftorque(i) >= fIPTorq)
        {
            wIPPos = i;
            fIPTorq = ptTorq->ftorque(i);
            break;
        }
    }

    ptTorq->set_dwippos(wIPPos);
    if(VERSION_RECPLUS(ptTorq))
    {
        ptTorq->set_dwiptorq((int)fIPTorq);
        wIPPlus = GetIPPlus(ptTorq, wIPPos);
        ptTorq->set_dwipplus(wIPPlus);
    }

    return TRUE;
}

/* 斜坡因子=(控制扭矩-拐点扭矩)/((控制周数-拐点时周数)*最佳扭矩) */
double CTorqueApp::GetFlopeFactor(TorqData::Torque *ptTorq, WORD wIPPos, UINT nIPTorq)
{
    double fMaxTorq   = 0;
    double fDeltaTorq = 0;
    double fDeltaTurn = 0;
    double fSlope     = 0;
    int    nCount     = 0;

    ASSERT_NULL_R(ptTorq, -1);
    ASSERT_ZERO_R(wIPPos, -1);
    COMP_BLE_R(GetOptTorq(ptTorq), 0, -1);
    nCount = ptTorq->ftorque_size();
    ASSERT_ZERO_R(nCount, -1);
    COMP_BGE_R(wIPPos, nCount, -1);

    GET_CTRL_TORQ(fMaxTorq, ptTorq);
    fDeltaTorq = fMaxTorq - nIPTorq;
    fDeltaTurn = GetIPDelCir(ptTorq, wIPPos);

    fSlope = THOUSANDTH(fDeltaTorq / (fDeltaTurn * GetOptTorq(ptTorq)));

    return fSlope;
}

/* 20200412 GetIPTorq可能返回两个拐点: 
   1. 数据中的dwippos
   2. SearchDeltaIP根据数据计算 */
UINT CTorqueApp::GetIPTorq(TorqData::Torque *ptTorq, WORD &wIPPos, WORD &wSchPos)
{
    int     i       = 0;
    UINT    nIPTorq = 0;

    wIPPos = 0;
    ASSERT_NULL_R(ptTorq, 0);
    
    if(!VERSION_RECPLUS(ptTorq))
    {
        wIPPos = ptTorq->dwippos();
        if(wIPPos >= ptTorq->ftorque_size())
            wIPPos = 0;

        if(wIPPos > 0)
            nIPTorq = (UINT)ptTorq->ftorque(wIPPos);
    }
    else
    {
        nIPTorq = ptTorq->dwiptorq();
        for (i = ptTorq->ftorque_size()-1; i > 0; i--)
        {
            if ((UINT)ptTorq->ftorque(i) <= nIPTorq)
            {
                wIPPos = i;
                break;
            }
        }
    }

    /* 实验版本，如果wIPPos 为0 则按数据中找拐点的方法处理或者直接获取 */
    wSchPos = SearchDeltaIP(ptTorq, !(ptTorq->bncheckip()));
    if(wIPPos == 0 && wSchPos != 0)
    {
        wIPPos = wSchPos;
        theApp.SetIPInfo(ptTorq, ptTorq->ftorque(wIPPos));
        nIPTorq = (UINT)ptTorq->ftorque(wSchPos);
    }

    return nIPTorq;
}

double CTorqueApp::GetIPDelCir(TorqData::Torque *ptTorq, WORD wIPPos)
{
    double fDelCir = 0;

    ASSERT_NULL_R(ptTorq, 0);
    ASSERT_ZERO_R(wIPPos, 0);
    COMP_BGE_R(wIPPos, ptTorq->ftorque_size(), 0);
    
    fDelCir    = THOUSANDTH((ptTorq->ftorque_size() - wIPPos)*ptTorq->fmaxcir()/MAXLINEITEM);
    if(VERSION_RECPLUS(ptTorq))   // for 3.22
    {
        fDelCir  = THOUSANDTH((ptTorq->dwtotalplus() - ptTorq->dwipplus()) / ptTorq->fplus());
    }

    return fDelCir;
}

DWORD CTorqueApp::GetQuality(TorqData::Torque *ptTorq)
{
    ASSERT_NULL_R(ptTorq, QUA_RESU_BAD);

    if(ptTorq->bshackle())
        return QUA_RESU_GOOD;
    
#ifndef TEST_QUALITY
    return ptTorq->dwquality();
#else
    return JudgeQuality(ptTorq, ptTorq->bshackle());
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
DWORD CTorqueApp::JudgeQuality(TorqData::Torque *ptTorq, int iShackle)
{
    DWORD       dwQuality   = QUA_RESU_BAD; /* 默认质量NOK */
    WORD        wIPPos      = 0;
    WORD        wSchPos     = 0;
    double      fMaxTorq    = 0;
    double      fCircle     = 0;
    double      fDeltaCir   = 0;
    double      fSlopeFactor= 0;
    double      fIPTorq     = 0;
    double      fMaxDelCir  = 0;
    double      fMinDelCir  = 0;
    double      fMinSlope   = 0;

    ASSERT_NULL_R(ptTorq, QUA_RESU_BAD);

    if(iShackle > 0)
    {
        dwQuality = QUA_RESU_GOOD;
        goto ENDJUDGE;
    }

    GET_CTRL_TORQ(fMaxTorq, ptTorq);
    fCircle  = theApp.GetCir(ptTorq);

    SET_QUALITY_BIT(fMaxTorq < ptTorq->flowerlimit(), QUA_TORQ_LESS_LIMIT, dwQuality);
    SET_QUALITY_BIT(fMaxTorq > ptTorq->fupperlimit(), QUA_TORQ_MORE_LIMIT, dwQuality);
    /* 实际的起始扭矩大于最佳扭矩的15% */
    SET_QUALITY_BIT(ptTorq->ftorque(0) > (GetOptTorq(ptTorq) * 0.15), QUA_TORQ_MORE_START, dwQuality);

    /* 超过台阶再平移周数超过0.2 */
    SET_QUALITY_BIT(JudgeTranslate(ptTorq), QUA_TRANSLATE, dwQuality);
    
    /* 图形周数小于0.20或者贴边 */
    SET_QUALITY_BIT(fCircle < ptTorq->flowercir(), QUA_CIRC_LESS_LIMIT, dwQuality);

    if(ptTorq->bncheckip())
    {
        goto ENDJUDGE;
    }

    fIPTorq = theApp.GetIPTorq(ptTorq, wIPPos, wSchPos);
    /* 没有找到拐点，且需要找到拐点时，其他错误 */
    if(wIPPos == 0) 
    {
        SET_QUALITY_BIT(TRUE, QUA_OTHER_CAUSE, dwQuality);
        goto ENDJUDGE;
    }

    /* 拐点扭矩范围 */
    /* 超拐点扭矩 / 拐点扭矩小 */
    SET_QUALITY_BIT(fIPTorq < ptTorq->flowertai(), QUA_LOW_SHOULD, dwQuality);
    SET_QUALITY_BIT(fIPTorq > ptTorq->fuppertai(), QUA_HIGHT_SHOULD, dwQuality);
    
    fMinDelCir = (ptTorq->fmindeltacir() > 0) ? ptTorq->fmindeltacir() : 0.01;
    fMaxDelCir = (ptTorq->fmaxdeltacir() > 0) ? ptTorq->fmaxdeltacir() : 0.1;
    fMinSlope  = (ptTorq->fminshlslope() > 0) ? ptTorq->fminshlslope() : 5.0;
    
    fDeltaCir  = theApp.GetIPDelCir(ptTorq, wIPPos);
    /*  拐点斜率小    （实际拐点斜率数值小于设置的拐点斜率数值时 */
    fSlopeFactor = theApp.GetFlopeFactor(ptTorq, wIPPos, (UINT)fIPTorq);
    SET_QUALITY_BIT(fSlopeFactor < fMinSlope, QUA_LOW_SLOPE, dwQuality);

    /* 超周数差值  周数差值超过设置的最大周数差值时----判废 */
    SET_QUALITY_BIT(fDeltaCir < fMinDelCir, QUA_OTHER_CAUSE, dwQuality);
    SET_QUALITY_BIT(fDeltaCir > fMaxDelCir, QUA_HIGHT_DELTATURN, dwQuality);

ENDJUDGE:
    if(dwQuality == 0)
    {
        return QUA_RESU_GOOD;
    }

    return dwQuality;
}

/* 超过控制扭矩的15%再平移周数超过0.2 */
BOOL CTorqueApp::JudgeTranslate(TorqData::Torque *ptTorq)
{
    int     i           = 0;
    int     iTranCount  = 1;
    double  fCtrlTorq   = 1;
    double  fTemp;
    int     iCount      = 0;

    ASSERT_NULL_R(ptTorq, TRUE);
    ASSERT_ZERO_R(ptTorq->ftorque_size(), TRUE);

    iTranCount = (int)((0.2 / ptTorq->fmaxcir())*500 +0.5);
    GET_CTRL_TORQ(fCtrlTorq, ptTorq);

    for(i= ptTorq->ftorque_size() -1; i>iTranCount; i--)
    {
        //if(ptTorq->ftorque(i) < ptTorq->fcontrol()*0.15) // 小于控制扭矩的15%
        if(ptTorq->ftorque(i) < ptTorq->flowertai()) // 小于最小台阶扭矩
            break;

        fTemp = (ptTorq->ftorque(i) - ptTorq->ftorque(i- iTranCount)) / fCtrlTorq;
        if (fTemp < 0.005)
            iCount++;
        else
            iCount = 0;
        
        COMP_BGE_R(iCount, iTranCount, TRUE);
    }

    return FALSE;
}

CString CTorqueApp::GetTorqCollTime(TorqData::Torque *ptTorq)
{
    __time64_t  colTime;

    ASSERT_NULL_R(ptTorq, _T(""));
    
    colTime = ptTorq->coltime();

    COleDateTime olett(colTime);
    return olett.Format(_T("%Y-%m-%d %H:%M:%S"));
}

CString CTorqueApp::GetTorqFullDate(TorqData::Torque *ptTorq)
{
    __time64_t  colTime;

    ASSERT_NULL_R(ptTorq, _T(""));

    colTime = ptTorq->coltime();
    COleDateTime olett(colTime);

    if(LANGUAGE_CHINESE == g_tGlbCfg.nLangType)
    {
        return olett.Format(_T("%Y年%m月%d日"));
    }

    /* February 17, 2020 */
    return olett.Format(_T("%b %d, %Y"));
}

CString CTorqueApp::GetTorqSimpDate(TorqData::Torque *ptTorq)
{
    __time64_t  colTime;

    ASSERT_NULL_R(ptTorq, _T(""));
    
    colTime = ptTorq->coltime();

    COleDateTime olett(colTime);
    return olett.Format(_T("%Y-%m-%d"));
}

/* 拷贝指定区域到DC的位图中 */
HBITMAP CTorqueApp::CopyDCToBitmap(HDC hScrDC,   LPRECT lprcScr)
{
    HDC         hMemDC      = NULL; // 屏幕和内存设备描述表
    HBITMAP     hBitmap     = NULL; // 位图句柄
    HBITMAP     hOldBitmap  = NULL;
    int         iX          = 0;    // 选定区域坐标
    int         iY          = 0;
    int         iWidth      = 0;    // 位图宽度和高度
    int         iHeight     = 0;

    // 确保选定区域不为空矩形
    COMP_BTRUE_R(IsRectEmpty(lprcScr), NULL);

    // 获得选定区域坐标
    iX      = lprcScr->left;
    iY      = lprcScr->top;
    iWidth  = lprcScr->right - lprcScr->left;
    iHeight = lprcScr->bottom - lprcScr->top;

    // 为屏幕设备描述表创建兼容的内存设备描述表
    hMemDC  = CreateCompatibleDC(hScrDC);

    // 创建一个与屏幕设备描述表兼容的位图
    hBitmap = CreateCompatibleBitmap(hScrDC, iWidth, iHeight);
    /*while(!hBitmap)
    {
        fFactor  -= 0.05f;
        hBitmap  = CreateCompatibleBitmap(hScrDC, (int)(iWidth*fFactor), (int)(iHeight*fFactor));
    }*/

    // 把新位图选到内存设备描述表中
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    // 把屏幕设备描述表拷贝到内存设备描述表中
    StretchBlt(hMemDC,0,0,iWidth,iHeight,hScrDC,iX,iY,iWidth,iHeight,SRCCOPY );
    
    // 得到位图的句柄
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

    // 清除
    DeleteDC(hMemDC);
    DeleteObject(hOldBitmap);

    // 返回位图句柄
    return   hBitmap;
}

int CTorqueApp::CopyDCToPNGFile(HDC hScrDC, UINT nNO, CString strFile, LPRECT lprcScr, HDC hMemDC, HBITMAP hBitmap)
{
    HBITMAP     hOldBitmap  = NULL;
    int         iX          = 0;    // 选定区域坐标
    int         iY          = 0;
    int         iWidth      = 0;    // 位图宽度和高度
    int         iHeight     = 0;
    //double      fFactor     = 1.0f;
    BOOL        bMemDC      = FALSE;
    BOOL        bBitmap     = FALSE;

    // 确保选定区域不为空矩形
    COMP_BTRUE_R(IsRectEmpty(lprcScr), -1);
    ASSERT_NULL_R(hMemDC, -1);
    ASSERT_NULL_R(hBitmap, -1);

    // 获得选定区域坐标
    iX      = lprcScr->left;
    iY      = lprcScr->top;
    iWidth  = lprcScr->right - lprcScr->left;
    iHeight = lprcScr->bottom - lprcScr->top;

    // 把新位图选到内存设备描述表中
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    // 把屏幕设备描述表拷贝到内存设备描述表中
    StretchBlt(hMemDC,0,0,iWidth,iHeight,hScrDC,iX,iY,iWidth,iHeight,SRCCOPY );
    
    // 得到位图的句柄
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
    theApp.SavePNG(hBitmap, strFile);

    return   0;
}

/* 根据HBITMAP获取图像数据，供后面保存文件使用，保存后需要释放HANDLE */
HANDLE CTorqueApp::GetImgData(HBITMAP hBitmap, LPBITMAPINFOHEADER& lpbi, DWORD& dwBmBitsSize)
{
    WORD                wBitCount   = 24;   //位图中每象素所占字节数
    DWORD               dwLineSize  = 0;
    BITMAP              Bitmap      = {0};  //位图属性结构
    BITMAPINFOHEADER    bi          = {0};  //位图信息头结构
    HDC                 hDC         = NULL;
    HANDLE              hDib        = NULL;
    HANDLE              hPal        = NULL;
    HANDLE              hOldPal     = NULL;  

    ASSERT_NULL_R(hBitmap, NULL);

    GetObject(hBitmap,   sizeof(Bitmap),   (LPSTR)&Bitmap);
    bi.biSize           =   sizeof(BITMAPINFOHEADER);
    bi.biWidth          =   Bitmap.bmWidth;
    bi.biHeight         =   Bitmap.bmHeight;
    bi.biPlanes         =   1;
    bi.biBitCount       =   wBitCount;
    bi.biCompression    =   BI_RGB;
    bi.biSizeImage      =   0;
    bi.biXPelsPerMeter  =   0;
    bi.biYPelsPerMeter  =   0;
    bi.biClrImportant   =   0;
    bi.biClrUsed        =   0;

    dwLineSize   = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4;
    dwBmBitsSize = dwLineSize * Bitmap.bmHeight;

    // 为位图内容分配内存,24位位图没有调色板
    hDib  = GlobalAlloc(GHND,dwBmBitsSize + sizeof(BITMAPINFOHEADER));
    ASSERT_NULL_R(hDib, NULL);
    
    lpbi  = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    *lpbi = bi;

    // 处理调色板
    hPal  = GetStockObject(DEFAULT_PALETTE);
    if(hPal)
    {
        hDC     = ::GetDC(NULL);
        hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
        RealizePalette(hDC);
    }

    // 获取该调色板下新的像素值
    GetDIBits(hDC, hBitmap, 0, Bitmap.bmHeight,
              (LPSTR)lpbi + sizeof(BITMAPINFOHEADER),
              (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

    // 恢复调色板
    if(hOldPal)
    {
        ::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
        RealizePalette(hDC);
        ::ReleaseDC(NULL, hDC);
    }

    return hDib;
}


/* 保存Bmp图像 */
BOOL CTorqueApp::SaveBmp(HBITMAP hBitmap, CString FileName)
{
    // 定义 位图中像素字节大小，位图文件大小，写入文件字节数
    DWORD               dwBmBitsSize= 0;
    DWORD               dwDIBSize   = 0;
    DWORD               dwWritten   = 0;
    // 定义文件，分配内存句柄，调色板句柄
    HANDLE              fh          = 0; 
    BITMAPFILEHEADER    bmfHdr;
    LPBITMAPINFOHEADER  lpbi        = NULL; //指向位图信息头结构
    HANDLE              hDib        = NULL;

    hDib = GetImgData(hBitmap, lpbi, dwBmBitsSize);
    ASSERT_NULL_R(hDib, FALSE);
    ASSERT_NULL_R(lpbi, FALSE);

    //创建位图文件
    fh  = CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                     FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    COMP_BE_R(fh, INVALID_HANDLE_VALUE, FALSE);

    //设置位图文件头
    bmfHdr.bfType       = BFT_BITMAP;   //   "BM "
    dwDIBSize           = sizeof(BITMAPFILEHEADER) +  sizeof(BITMAPINFOHEADER) + dwBmBitsSize;
    bmfHdr.bfSize       = dwDIBSize;
    bmfHdr.bfReserved1  = 0;
    bmfHdr.bfReserved2  = 0;
    bmfHdr.bfOffBits    = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

    // 写入位图文件头
    WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
    // 写入位图文件其余内容
    WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
    // 清除
    CloseHandle(fh);
    GlobalUnlock(hDib);
    GlobalFree(hDib);

    return TRUE;
}

//returns 0 if all went ok, non-0 if error
//output image is always given in RGBA (with alpha channel), even if it's a BMP without alpha channel
unsigned CTorqueApp::decodeBMP(std::vector<unsigned char>& image, unsigned& w, unsigned& h, unsigned char * bmp)
{
    static const unsigned MINHEADER = 54; //minimum BMP header size

    //if (bmp.size() < MINHEADER) return -1;
    if (bmp[0] != 'B' || bmp[1] != 'M') return 1; //It's not a BMP file if it doesn't start with marker 'BM'
    unsigned pixeloffset = bmp[10] + 256 * bmp[11]; //where the pixel data starts
    //read width and height from BMP header
    w = bmp[18] + bmp[19] * 256;
    h = bmp[22] + bmp[23] * 256;
    //read number of channels from BMP header
    if (bmp[28] != 24 && bmp[28] != 32) return 2; //only 24-bit and 32-bit BMPs are supported.
    unsigned numChannels = bmp[28] / 8;

    //The amount of scanline bytes is width of image times channels, with extra bytes added if needed
    //to make it a multiple of 4 bytes.
    unsigned scanlineBytes = w * numChannels;
    if (scanlineBytes % 4 != 0) scanlineBytes = (scanlineBytes / 4) * 4 + 4;

    unsigned dataSize = scanlineBytes * h;
    //if (bmp.size() < dataSize + pixeloffset) return 3; //BMP file too small to contain all pixels

    image.resize(w * h * 4);

    /*
    There are 3 differences between BMP and the raw image buffer for LodePNG:
    -it's upside down
    -it's in BGR instead of RGB format (or BRGA instead of RGBA)
    -each scanline has padding bytes to make it a multiple of 4 if needed
    The 2D for loop below does all these 3 conversions at once.
    */
    for (unsigned y = 0; y < h; y++)
        for (unsigned x = 0; x < w; x++) {
            //pixel start byte position in the BMP
            unsigned bmpos = pixeloffset + (h - y - 1) * scanlineBytes + numChannels * x;
            //pixel start byte position in the new raw image
            unsigned newpos = 4 * y * w + 4 * x;
            if (numChannels == 3) {
                image[newpos + 0] = bmp[bmpos + 2]; //R
                image[newpos + 1] = bmp[bmpos + 1]; //G
                image[newpos + 2] = bmp[bmpos + 0]; //B
                image[newpos + 3] = 255;            //A
            }
            else {
                image[newpos + 0] = bmp[bmpos + 2]; //R
                image[newpos + 1] = bmp[bmpos + 1]; //G
                image[newpos + 2] = bmp[bmpos + 0]; //B
                image[newpos + 3] = bmp[bmpos + 3]; //A
            }
        }
    return 0;
}

BOOL CTorqueApp::SavePNG(HBITMAP hBitmap, CString FileName)
{
    // 定义 位图中像素字节大小，位图文件大小，写入文件字节数
    DWORD               dwBmBitsSize= 0;
    DWORD               dwDIBSize   = 0;
    BITMAPFILEHEADER    bmfHdr;
    LPBITMAPINFOHEADER  lpbi        = NULL; //指向位图信息头结构
    HANDLE              hDib        = NULL;
    BYTE                *pBuf       = NULL;
    BYTE                *pTmp       = NULL;
    std::vector<unsigned char> image;
    std::vector<unsigned char> png;
    unsigned            w, h;

    hDib = GetImgData(hBitmap, lpbi, dwBmBitsSize);
    ASSERT_NULL_R(hDib, FALSE);
    ASSERT_NULL_R(lpbi, FALSE);

    pBuf = new BYTE[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwBmBitsSize];
    
    //设置位图文件头
    bmfHdr.bfType       = BFT_BITMAP;   //   "BM "
    dwDIBSize           = sizeof(BITMAPFILEHEADER) +  sizeof(BITMAPINFOHEADER) + dwBmBitsSize;
    bmfHdr.bfSize       = dwDIBSize;
    bmfHdr.bfReserved1  = 0;
    bmfHdr.bfReserved2  = 0;
    bmfHdr.bfOffBits    = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

    memcpy(pBuf, &bmfHdr, sizeof(BITMAPFILEHEADER));
    pTmp = pBuf + sizeof(BITMAPFILEHEADER);
    memcpy(pTmp, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER) + dwBmBitsSize);

    decodeBMP(image, w, h, pBuf);

    lodepng::encode(png, image, lpbi->biWidth, lpbi->biHeight);
    lodepng::save_file(png, FileName.GetString());

    delete [] pBuf;
    GlobalUnlock(hDib);
    GlobalFree(hDib);
    return TRUE;
}

void CTorqueApp::ClearReadTorq()
{
    int i = 0;

    g_tReadData.nCur = 0;
    g_tReadData.nTotal = 0;
    g_tReadData.nQualy = 0;
    g_tReadData.nUnQualy = 0;

    memset(&g_tReadData.tSplit, 0, MAXWELLNUM*sizeof(SPLITPOINT));

    for(i=0; i<MAXWELLNUM; i++)
        g_tReadData.tData[i].Clear();
}

/* 从文件当前读取位置获取数据的长度，文件位置为读取有效的文件长度之后 */
int CTorqueApp::SeekFileLen(CFile &file)
{
    int     i = 0;
    int     iFileLen = 0;
    int     iFirstPos= 0;
    int     iFilePos = 0;
    UINT    nLeng    = 0;
    UINT    nNextLen = 0;
    
    iFileLen = (int)file.GetLength();
    iFilePos = (int)file.GetPosition();

    if(SeekPBDataPos(file, iFilePos) == -1)
    {
        return -1;
    }
    
    /* 跳过实际的长度，包括可能的尾巴 */
    file.Read(&nLeng,sizeof(UINT));
    iFirstPos = (int)file.GetPosition();
    if ((iFirstPos + (int)nLeng > iFileLen) || (nLeng > MAXPROBUFF))
    {
        /* 20190616 往后再读MAXSKIPLEN次，尝试跳过数据异常 
           1个字节一跳 */
        for(i=-3; i<MAXSKIPLEN; i++)
        {
            file.Seek((iFirstPos + i), CFile::begin);
            file.Read(&nNextLen,sizeof(UINT));
            iFilePos = (int)file.GetPosition();
            if ((iFilePos + (int)nNextLen <= iFileLen) && (nNextLen < MAXPROBUFF))
            {
                nLeng = nNextLen;
                break;
            }
        }
        if(i >= MAXSKIPLEN)
        {
            return -2;
        }
    }
    
    if(nLeng > MAXPROBUFF)
    {
        return -3;
    }

    return nLeng;
}

/* 从第一条记录开始，跳到指定的记录位置
   说明:文件的第一个UINT记录总数已经读出
   for循环的递增为int i 
   nLeng 为UNIT类型,表示该数据完整长度 */
int CTorqueApp::SeekTorque(CFile &file, int iDataNum)
{
    int     i = 0;
    int     iDataLen = 0;

    file.Seek(sizeof(UINT), CFile::begin);
    
    for(i=0; i< iDataNum; i++)
    {
        iDataLen = SeekFileLen(file);
        if(iDataLen < 0)
            return -1;
        
        file.Seek(iDataLen, CFile::current);
    }

    return 0;
}

int  CTorqueApp::SeekPBDataPos(CFile &file, int iCurPos)
{
    int     i = 0;
    int     iFileLen = 0;
    char    cTmpRead[100];

    COMP_BFALSE_R(g_tReadData.bHaveHead, 0);
    
    iFileLen = (int)file.GetLength();
    if((iCurPos+MIN_TORQDATALEN) >= iFileLen)
        return -1;

    file.Read(cTmpRead,100);

    /* 查找数据文件头 */
    for(i=0; i<100-PBHEADLEN; i++)
    {
        if(memcmp(&cTmpRead[i], &m_nPBHead, PBHEADLEN) == 0)
        {
            file.Seek(iCurPos + i + PBHEADLEN, CFile::begin);
            return 0;
        }
    }

    return -1;
}

BOOL CTorqueApp::ReadHisTorqFromFile(CString strDataName)
{
    COMP_BFALSE_R(GetTorqDataFromFile(strDataName), FALSE);

    /* 没有更新入井序号，直接返回正确，否则重新读一次 */
    COMP_BFALSE_R(ReCalWellNO(strDataName), TRUE);

    return GetTorqDataFromFile(strDataName);
}

/* 读取历史的扭矩数据文件
   一次最多读取MAXWELLNUM条 */
BOOL CTorqueApp::GetTorqDataFromFile(CString strDataName)
{
    CFile   file;
    int     i       = 0;
    int     j       = 0;
    int     iCtrlCount = 0;
    UINT    nNum    = 0;
    UINT    nValid  = 0;
    int     iFilePos = 0;
    int     iDataLen = 0;
    int     iTotalPnt= 0;
    CString strInfo;
    CString strTitle;
    bool    bRes;
    TorqData::Torque *ptTorq = NULL;
    SPLITPOINT       *pSplit = NULL;
    int     iSplitPos = 0;
    char    cPBHead[PBHEADLEN+1] = {0};
    DWORD   dwQuality = 0;
    double  fRatio    = NM2LBFT;
    //string strData;

    COMP_BTRUE_R(strDataName.IsEmpty(), FALSE);

    ASSERT_ZERO_R(file.Open(strDataName, CFile::modeRead|CFile::shareDenyNone),FALSE);

    m_strReadFile = strDataName;
    strTitle = file.GetFileTitle();
    strTitle.Delete(strTitle.GetLength() - 4, 4);
    m_strFileTitle = strTitle;

    ClearReadTorq();

    file.SeekToBegin();
    file.Read(&nNum,sizeof(UINT));
    if(nNum > MAXWELLNUM)
    {
        strInfo.Format(IDS_STRINFREADOVERFLOW, nNum, MAXWELLNUM);
        SaveShowMessage(strInfo);
        nNum = MAXWELLNUM;
    }

    /* 检查文件是否有头 */
    g_tReadData.bHaveHead = FALSE;
    file.Read(cPBHead,PBHEADLEN);
    file.Seek(sizeof(UINT), CFile::begin);
    if(memcmp(cPBHead, &m_nPBHead, PBHEADLEN) == 0)
    {
        g_tReadData.bHaveHead = TRUE;
    }

    BeginWaitCursor();

    for(i=0; i<(int)nNum; i++)
    {
        iFilePos = (int)file.GetPosition();
        strInfo.Format(IDS_STRINFTORQDATAERR, strDataName, nNum, i, file.GetPosition());

        iDataLen = SeekFileLen(file);
        if(iDataLen < 0)
        {
            SaveShowMessage(strInfo);
            break;
        }
        
        memset(m_cProtoBuf, 0, MAXPROBUFF);
        file.Read(m_cProtoBuf,iDataLen);

        if (iDataLen < MIN_TORQDATALEN)
        {
            strInfo.Format(IDS_STRINFTORQDATASHORT, strDataName, iDataLen, file.GetPosition(), i);
            SaveMessage(strInfo);
            continue;
        }


        bRes = g_tReadData.tData[nValid].ParseFromArray(m_cProtoBuf, iDataLen);
        if (!bRes)
            continue;

        /* 数据大于1屏时设置分屏信息 */
        /* 20190609最后一屏按控制周数，其他按满屏计算 */
        /* 20190916 如果数据大于控制周数，则需要分屏，最后一周在控制周数上 */
        ptTorq = &g_tReadData.tData[nValid];
        pSplit = &g_tReadData.tSplit[nValid];

        iTotalPnt = ptTorq->ftorque_size();
        if(VERSION_RECPLUS(ptTorq))
        {
            g_tReadData.nTotalPlus[nValid] = ptTorq->dwtotalplus();
            
            if(ptTorq->fplus() > 0 && ptTorq->fmaxcir() > 0)
                iTotalPnt = (int)ceil(ptTorq->dwtotalplus() / ptTorq->fplus() /
                                  ptTorq->fmaxcir() * MAXLINEITEM);
        }

        if(ptTorq->bshackle())   /* 从前往后分屏 */
        {
            if(iTotalPnt > MAXLINEITEM)
            {
                // 按 MAXLINEITEM 直接分屏
                pSplit->iCtrlPnt = MAXLINEITEM;
                pSplit->iSplitNum = (int)ceil(iTotalPnt * 1.0 / MAXLINEITEM);
                iSplitPos = 0;
                pSplit->iCur = 1;
                for(j=0; j<pSplit->iSplitNum && j<MAXSPLIITNUM; j++)
                {
                    pSplit->iBegin[j] = iSplitPos;
                    pSplit->iEnd[j]   = MIN(iSplitPos + MAXLINEITEM, iTotalPnt);
                    iSplitPos += MAXLINEITEM;
                    if(iSplitPos >= iTotalPnt)
                        break;
                }
            }
        }
        else    /* 从后往前分屏 */
        {
            iCtrlCount = int(GetCtrlCir(ptTorq)*MAXLINEITEM / GetMaxCir(ptTorq) + 0.5);
            if (iCtrlCount < 0)
            {
                break;
            }

            pSplit->iCtrlPnt = iCtrlCount;

            if(iTotalPnt > iCtrlCount)
            {
                pSplit->iSplitNum = 1 + (int)ceil((iTotalPnt-iCtrlCount) * 1.0 / MAXLINEITEM);
                iSplitPos = iTotalPnt;
                j = MIN(pSplit->iSplitNum, MAXSPLIITNUM);
                pSplit->iCur = j;
                
                // 第一屏到iCtrlCount, 其他满屏
                pSplit->iEnd[j-1]   = iSplitPos;
                pSplit->iBegin[j-1] = MAX(iSplitPos - iCtrlCount, 0);
                iSplitPos -= iCtrlCount;
                j--;
                for (; j>=0; j--)
                {
                    pSplit->iEnd[j-1]   = iSplitPos;
                    pSplit->iBegin[j-1] = MAX(iSplitPos - MAXLINEITEM, 0);
                    iSplitPos -= MAXLINEITEM;
                    if(iSplitPos <= 0)
                        break;
                }
            }
        }

        /* NM  < ---- > lbft (* ratio) */
        if(g_tGlbCfg.nTorqUnit != ptTorq->dwtorqunit())
        {
            /* 0 (N.M) lb.ft --> N.m  */
            fRatio    = LBFT2NM;
            if(g_tGlbCfg.nTorqUnit == 1) // N.m --> lb.ft
                fRatio = NM2LBFT;

            ptTorq->set_fmaxtorq(fRatio*ptTorq->fmaxtorq());
            ptTorq->set_fmaxlimit(fRatio*ptTorq->fmaxlimit());
            ptTorq->set_fupperlimit(fRatio*ptTorq->fupperlimit());
            ptTorq->set_fcontrol(fRatio*ptTorq->fcontrol());
            ptTorq->set_fopttorq(fRatio*ptTorq->fopttorq());
            ptTorq->set_flowerlimit(fRatio*ptTorq->flowerlimit());
            ptTorq->set_fspeeddown(fRatio*ptTorq->fspeeddown());
            ptTorq->set_fshow(fRatio*ptTorq->fshow());
            ptTorq->set_fbear(fRatio*ptTorq->fbear());
            ptTorq->set_fuppertai(fRatio*ptTorq->fuppertai());
            ptTorq->set_flowertai(fRatio*ptTorq->flowertai());

            for(j=0; j<ptTorq->ftorque_size(); j++)
            {
                ptTorq->set_ftorque(j, fRatio*ptTorq->ftorque(j));
            }
        }

        g_tReadData.nTotal++;
        dwQuality = GetQuality(ptTorq);
        if(dwQuality & QUA_RESU_QUALITYBIT)
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

TorqData::Torque * CTorqueApp::GetOrgTorqFromTorq(UINT nNO)
{
    TorqData::Torque *ptOrg = NULL;

    COMP_BGE_R(nNO, g_tReadData.nTotal, NULL);

    ptOrg = &g_tReadData.tData[nNO];
    ASSERT_ZERO_R(ptOrg->ftorque_size(), NULL);

    return ptOrg;
}

/*  nNO 从0开始计数
    iMulti在放大时使用 */
DRAWTORQDATA * CTorqueApp::GetDrawDataFromTorq(UINT nNO, int iMulti)
{
    int i           = 0;
    int iDrawPnt    = 0;
    int iDataPlus   = 0;
    int iDrawPlus   = 0;
    int iDrawIndex  = 0;
    int iDataIndex  = 0;
    int iInsCnt     = 0;
    int iPriorDataIndex = 0;
    int iPriorDrawIndex = 0;
    double fPreTorq = 0;
    double fCurTorq = 0;
    double fPreRpm  = 0;
    double fCurRpm  = 0;
    double fPlusPerPnt = 1.0;
    TorqData::Torque *ptOrg = NULL;
    DRAWTORQDATA     *ptDraw = NULL;

    COMP_BGE_R(nNO, g_tReadData.nTotal, NULL);
    COMP_BGE_R(nNO, MAXWELLNUM, NULL);

    ptOrg = &g_tReadData.tData[nNO];
    ASSERT_ZERO_R(ptOrg->ftorque_size(), NULL);

    ptDraw = &m_tCurDrawTorq;
    if(iMulti > 1)
        ptDraw = &m_tCurZoomTorq;

    memset(ptDraw, 0, sizeof(DRAWTORQDATA));
    ptDraw->ptOrgTorq = ptOrg;
    if(!VERSION_RECPLUS(ptOrg))   // 3.21及之前版本
    {
        for(i=0; i<ptOrg->ftorque_size(); i++)
        {
            ptDraw->fTorque[i] = ptOrg->ftorque(i);
            ptDraw->fRpm[i]    = ptOrg->frpm(i);
        }

        ptDraw->wCount = ptOrg->ftorque_size();
        
        return ptDraw;
    }

    fPlusPerPnt = ptOrg->fplus() * ptOrg->fmaxcir() / iMulti / MAXLINEITEM;

    iDrawPnt = (int)ceil(g_tReadData.nTotalPlus[nNO] / fPlusPerPnt);
    if(iDrawPnt < 2)
        iDrawPnt = 2;

    iDrawIndex = 0;
    iDataIndex = 0;
    iDataPlus  = ptOrg->dwdelplus(0);
    iDrawPlus  = int(ceil(iDrawIndex * fPlusPerPnt));
    ptDraw->fTorque[iDrawIndex]  = ptOrg->ftorque(0);
    ptDraw->fRpm[iDrawIndex]     = ptOrg->frpm(0);

    iPriorDataIndex = iDataIndex;
    iPriorDrawIndex = iDrawIndex;
    iDrawIndex = 1;
    for(iDataIndex=1; iDataIndex<ptOrg->ftorque_size()-1; iDataIndex++)
    {
        /* 跳过delta脉冲为0的情况 */
        while(ptOrg->dwdelplus(iDataIndex) == 0 && iDataIndex < ptOrg->ftorque_size() - 1)
            iDataIndex ++;
        iDataPlus += ptOrg->dwdelplus(iDataIndex);

        /* data分辨率大于draw分辨率时 */
        if(iDataPlus < iDrawPlus)
            continue;

        for(; iDrawIndex<iDrawPnt; iDrawIndex++)
        {
            iDrawPlus = int(ceil(iDrawIndex * fPlusPerPnt));
            // 当前的iDrawIndex不设置，取iDrawIndex-1
            if(iDrawPlus > iDataPlus)
                break;
        }

        /* (priorData+1) -- Data
           (priorDraw+1) -- Draw-1 */
        iInsCnt  = iDrawIndex - iPriorDrawIndex;
        fCurTorq = ptOrg->ftorque(iDataIndex);
        fCurRpm  = ptOrg->frpm(iDataIndex);
        if(iInsCnt <= 1)
        {
            ptDraw->fTorque[iDrawIndex-1] = fCurTorq;
            ptDraw->fRpm[iDrawIndex-1]    = fCurRpm;
        }
        else
        {
            fPreTorq = ptOrg->ftorque(iPriorDataIndex);
            fPreRpm  = ptOrg->frpm(iPriorDataIndex);
            for(i=0;i<iInsCnt; i++)
            {
                ptDraw->fTorque[i+iPriorDrawIndex+1] = (fCurTorq - fPreTorq)*(i+1)/iInsCnt + fPreTorq;
                ptDraw->fRpm[i+iPriorDrawIndex+1]    = (fCurRpm  - fPreRpm) *(i+1)/iInsCnt + fPreRpm;
            }
        }
        iPriorDrawIndex = iDrawIndex-1;
        iPriorDataIndex = iDataIndex;
    }

    iInsCnt = iDrawPnt -1 - iDrawIndex;
    if(iInsCnt <= 1)
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
            ptDraw->fTorque[i + iDrawIndex + 1] = (fCurTorq - fPreTorq)*(i + 1) / iInsCnt + fPreTorq;
            ptDraw->fRpm[i + iDrawIndex + 1] = (fCurRpm - fPreRpm) *(i + 1) / iInsCnt + fPreRpm;
        }
    }

    ptDraw->wCount = iDrawPnt;
    return ptDraw;
}

CString CTorqueApp::GetStatType(TorqData::Torque *ptTorq, WORD wPlace)
{
    ASSERT_NULL_R(ptTorq, NULLSTR);

    COMP_BGE_R(wPlace, ptTorq->tshow_size(), NULLSTR);

    return GetTorqShowValue(ptTorq, wPlace);
}

BOOL CTorqueApp::CheckPassWord()
{
    string          strSupPW;
    CDlgPassword    dlgPW;

    if(IDOK != dlgPW.DoModal())
    {
        return FALSE;
    }

    strSupPW = LoadstringFromRes(IDS_STRSUPPORPW);
    if( 0 != dlgPW.m_strPassword.Compare(g_tGlbCfg.strPassWord.c_str()) &&
        0 != dlgPW.m_strPassword.Compare(strSupPW.c_str()))
    {
        AfxMessageBox(LoadstringFromRes(IDS_STRERRORPW).c_str());
        return FALSE;
    }

    return TRUE;
}

string CTorqueApp::LoadstringFromRes( unsigned string_ID )
{
    char buffer[ MAX_LOADSTRING ];

    unsigned bytes_copied = LoadString(m_hLangDLL[g_tGlbCfg.nLangType], string_ID, buffer, MAX_LOADSTRING );
    if( !bytes_copied )
        throw std::runtime_error( "Resource not found!" );

    return string( buffer, bytes_copied );
}

string CTorqueApp::LoadstringFromRes(unsigned string_ID, int val)
{
    char buffer[MAX_LOADSTRING];
    LoadString(m_hLangDLL[g_tGlbCfg.nLangType], string_ID, buffer, MAX_LOADSTRING);
    snprintf(buffer, MAX_LOADSTRING, LoadstringFromRes(string_ID).c_str(), val);

    return string(buffer);
}

string CTorqueApp::LoadstringFromRes(unsigned string_ID, double val)
{
    char buffer[MAX_LOADSTRING];
    LoadString(m_hLangDLL[g_tGlbCfg.nLangType], string_ID, buffer, MAX_LOADSTRING);
    snprintf(buffer, MAX_LOADSTRING, LoadstringFromRes(string_ID).c_str(), val);

    return string(buffer);
}

string CTorqueApp::LoadstringFromRes(unsigned string_ID, string val)
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
void CTorqueApp::UpdateHisData(CString strName, int iDataPlace, TorqData::Torque *ptTorq)
{
    int     i       = 0;
    UINT    nCurPos = 0;    /* 当前数据位置 */
    UINT    nNextPos= 0;    /* 下一个数据的位置 */
    UINT    nLastPos = 0;
    int     iLeft   = 0;    /* 当前数据后的数据大小 */
    size_t  iCurLen = 0;     /* 当前数据的总长度 */
    UINT    iDataLen= 0;     /* 数据的总长度 */
    UINT    nTotal  = 0;
    CFile   file;
    BYTE    *pBuffer= NULL;
    char    *pcBuff = NULL;

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
    ASSERT_ZERO(file.Open(strName, CFile::modeReadWrite|CFile::shareDenyNone,NULL));
    
    /* 跳过文件的数据总条数 */
    file.Read(&nTotal, sizeof(UINT));
    if(iDataPlace == -1)
        iDataPlace = nTotal;

    /* 跳过之前记录 */
    if(0 != SeekTorque(file, iDataPlace-1))
    {
        file.Close();
        return;
    }
    //SEEK_TORQUE(((int)g_tReadData.nCur-1), nLeng);

    /* 获得当前数据的位置 */
    iDataLen = SeekFileLen(file);
    if(iDataLen < 0)
    {
        file.Close();
        return;
    }

    /* 获得当前数据长度的位置，跳过PBHEADLEN,往回跳4个字节 */
    nCurPos = (UINT)file.GetPosition() - sizeof(UINT);
    file.Seek(iDataLen, CFile::current); 

    /* 保留当前数据后面的数据 */
    nNextPos = (UINT)file.GetPosition();
    iLeft    = (int)(file.GetLength() - nNextPos);
    if(iLeft > 0)
    {
        pBuffer = new BYTE[iLeft];
        ASSERT_NULL(pBuffer);
        memset(pBuffer, 0, iLeft);
        file.Read(pBuffer,iLeft);
    }

    /* 跳到当前数据开始位置，写入更新的数据信息 */
    file.Seek(nCurPos, CFile::begin);
    
    file.Write(&iCurLen,sizeof(UINT));
    file.Write(pcBuff, iCurLen);

    /* 还原当前数据后面的数据 */
    if(pBuffer != NULL)
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

void CTorqueApp::GetShowDataRange(DRAWTORQDATA *ptDraw, int &iBegin, int &iEnd, SPLITPOINT *ptSplit, UINT nMulti)
{
    ASSERT_NULL(ptDraw);
    
    iBegin = 0;
    iEnd   = ptDraw->wCount;
    if(nMulti != 1 && VERSION_RECPLUS(ptDraw->ptOrgTorq))  // for 3.22 放大数据
        iEnd   = (int)ceil(ptDraw->wCount * 1.0 / nMulti );
    
    ASSERT_NULL(ptSplit);
    ASSERT_ZERO(ptSplit->iSplitNum);

    /* 默认第一屏 */
    iBegin = ptSplit->iBegin[0];
    iEnd   = ptSplit->iEnd[0];
    
    COMP_BG(ptSplit->iCur, ptSplit->iSplitNum);
    COMP_BL(ptSplit->iCur, 1);
    
    iBegin = ptSplit->iBegin[ptSplit->iCur-1];
    iEnd   = ptSplit->iEnd[ptSplit->iCur-1];

    return;
}

double CTorqueApp::GetCir(TorqData::Torque *ptTorq)
{
    double fCir = 0;

    ASSERT_NULL_R(ptTorq, 0);
    
    fCir = THOUSANDTH(ptTorq->ftorque_size()*ptTorq->fmaxcir()/MAXLINEITEM);
    if(VERSION_RECPLUS(ptTorq))   // for 3.22
    {
        fCir  = THOUSANDTH(ptTorq->dwtotalplus() / ptTorq->fplus());
    }

    return fCir;
}

double CTorqueApp::GetMaxCir(TorqData::Torque  *ptTorq)
{
    double fMaxCir = 1;

    ASSERT_NULL_R(ptTorq, fMaxCir);

    fMaxCir = ptTorq->fmaxcir();
    if(ptTorq->btimeline())
        fMaxCir = ptTorq->fmaxtime();

    return fMaxCir;
}

double CTorqueApp::GetCtrlCir(TorqData::Torque  *ptTorq)
{
    double fCtrlCir = 0.8;

    ASSERT_NULL_R(ptTorq, fCtrlCir);

    fCtrlCir = ptTorq->fcontrolcir();
    if(ptTorq->btimeline())
        fCtrlCir = ptTorq->fcontroltime();

    return fCtrlCir;
}

double CTorqueApp::GetUpperCir(TorqData::Torque  *ptTorq)
{
    double fUpperCir = 0.9;

    ASSERT_NULL_R(ptTorq, fUpperCir);

    fUpperCir = ptTorq->fuppercir();
    if(ptTorq->btimeline())
        fUpperCir = ptTorq->fuppertime();

    return fUpperCir;
}

double CTorqueApp::GetLowerCir(TorqData::Torque  *ptTorq)
{
    double fLowerCir = 0.1;

    ASSERT_NULL_R(ptTorq, fLowerCir);

    fLowerCir = ptTorq->flowercir();
    if(ptTorq->btimeline())
        fLowerCir = ptTorq->flowertime();

    return fLowerCir;
}

double CTorqueApp::GetOptTorq(TorqData::Torque  *ptTorq)
{
    double fOptTorq = 2000;

    ASSERT_NULL_R(ptTorq, fOptTorq);

    fOptTorq = ptTorq->fopttorq();
    if(0 == fOptTorq)
        fOptTorq = ptTorq->fcontrol();

    return fOptTorq;
}

void CTorqueApp::IncTorqNo()
{
    m_nCurNO++;
    m_dwTotalTorqNum++;
}

CString CTorqueApp::GetSaveDataPath()
{
    CString         strSavePath;
    
    strSavePath = m_strDataPath.c_str();
    strSavePath += m_strFileTitle.c_str();
    strSavePath += "\\";
    CreateDirectory(strSavePath, NULL);

    return strSavePath;
}

int CTorqueApp::SplitString(CString strSource, CStringList &slList)
{
    int     index = 0;
    CString strTemp;

    if(strSource.IsEmpty())
        return -1;

    while(TRUE)
    {
        index = strSource.Find(_T(" "));
        if(index == -1)
        {
            slList.AddTail(strSource);
            return 0;
        }
        if (index == 0) /* 第一个是空格，直接去掉 */
        {
            strSource = strSource.Right(strSource.GetLength() - 1);
            continue;
        }
        strTemp = strSource.Left(index);
        slList.AddTail(strTemp);
        strSource = strSource.Right(strSource.GetLength()-index-1);
    }

    return -2;
}

/* 返回是否有修改 
   20200302 每次都从最开始重新计算 */
BOOL CTorqueApp::ReCalWellNO(CString strDataName)
{
    int     i          = 0;
    int     iWellIndex = -1;
    UINT    nRunningNO = 1;
    BOOL    bModified  = FALSE;
    CString strRunningNO;
    CString strOldNO;
    TorqData::Torque    *ptTorq         = NULL;
    TorqData::ShowInfo  *ptRunningShow  = NULL;

    iWellIndex = GetMainWellIndexfromData(MAINSHOWWELL, &g_tReadData.tData[0]);

    COMP_BL_R(iWellIndex, 0, FALSE);

    BeginWaitCursor();
    /* 会从当前序号开始(开始数值为1), 顺序更新后续数据的入井序号 */
    for(i = 1; i<=(int)g_tReadData.nTotal; i++)
    {
        ptTorq = &g_tReadData.tData[i-1];

        strOldNO = GetTorqShowValue(ptTorq, iWellIndex);
        ptRunningShow = ptTorq->mutable_tshow(iWellIndex);
        if(QUA_RESU_GOOD == ptTorq->dwquality())
        {
            strRunningNO.Format("%d", nRunningNO++);
        }
        else
        {
            strRunningNO = NULLSTR;
        }

        if(0 == strOldNO.CompareNoCase(strRunningNO))
            continue;

        bModified = TRUE;
        ptRunningShow->set_strvalue(strRunningNO.GetBuffer());
    }

    EndWaitCursor();

    COMP_BFALSE_R(bModified, FALSE);

    SaveAllData(strDataName);

    return TRUE;
}

void CTorqueApp::SaveAllData(CString strDataName)
{
    UINT    i           = 0;
    size_t  nDataLen    = 0;
    CFile   file;
    
    /* write to file */
    if(file.Open(strDataName, CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone,NULL))
    {
        /*更新记录数*/
        file.Write(&g_tReadData.nTotal,sizeof(UINT));

        for(i = 0; i < g_tReadData.nTotal; i++)
        {
            nDataLen = g_tReadData.tData[i].ByteSizeLong();
            if(nDataLen == 0 || nDataLen >= MAXPROBUFF)
                continue;
            memset(m_cProtoBuf, 0, MAXPROBUFF);
            if (!g_tReadData.tData[i].SerializeToArray(m_cProtoBuf, nDataLen))
            {
                continue;
            }

            if(g_tReadData.bHaveHead)
                file.Write(&m_nPBHead, PBHEADLEN);
            file.Write(&nDataLen, sizeof(UINT));
            file.Write(m_cProtoBuf, nDataLen);
        }
    }

    file.Close();
}

string CTorqueApp::GetFixTubeValue(UINT nShowIndex, UINT nCurNO, FIXTUBEINFO *ptFix)
{
    UINT    i = 0;

    COMP_BGE_R(nShowIndex, MAXPARANUM, NULLSTR);
    ASSERT_NULL_R(ptFix, NULLSTR);

    if (!IsFixTube())
    {
        return m_tParaCfg.strValue[nShowIndex];
    }

    for (i = 0; i < ptFix->nNum; i++)
    {
        if (nCurNO == ptFix->ptPara[i].nNO)
        {
            return ptFix->ptPara[i].strName[g_tGlbCfg.nLangType];
        }
    }
    return "";
}

string CTorqueApp::GetTubeFactoryValue()
{
    return GetFixTubeValue(INDEX_SHOW_FACTORY, m_tParaCfg.tTubeCfg.nFixTube[INDEX_TUBE_FACTORY], &g_cTubing.m_tTubInfo[INDEX_TUBE_FACTORY]);
}

string CTorqueApp::GetTubeOEMValue()
{
    return GetFixTubeValue(INDEX_SHOW_OEM, m_tParaCfg.tTubeCfg.nFixTube[INDEX_TUBE_OEM], &g_cTubing.m_tTubInfo[INDEX_TUBE_OEM]);
}

string CTorqueApp::GetTubeSizeValue()
{
    return GetFixTubeValue(INDEX_SHOW_SIZE, m_tParaCfg.tTubeCfg.nFixTube[INDEX_TUBE_SIZE], &g_cTubing.m_tTubInfo[INDEX_TUBE_SIZE]);
}

string CTorqueApp::GetTubeMaterValue()
{
    return GetFixTubeValue(INDEX_SHOW_MATER, m_tParaCfg.tTubeCfg.nFixTube[INDEX_TUBE_MATER], &g_cTubing.m_tTubInfo[INDEX_TUBE_MATER]);
}

string CTorqueApp::GetTubeCouplValue()
{
    return GetFixTubeValue(INDEX_SHOW_COUPL, m_tParaCfg.tTubeCfg.nFixTube[INDEX_TUBE_COUPL], &g_cTubing.m_tTubInfo[INDEX_TUBE_COUPL]);
}

CString CTorqueApp::GetTorqShowName(TorqData::Torque *ptTorq, int iIndex)
{
    ASSERT_NULL_R(ptTorq, NULLSTR);
    COMP_BL_R(iIndex, 0, NULLSTR);
    COMP_BGE_R(iIndex, MAXPARANUM, NULLSTR);

    if(iIndex >= ptTorq->tshow_size())
        return NULLSTR;

    // cur version iIndex 从1开始, 0为Factory
    // 20220928 按listNO存储，NO从1~15，和show序号对应，不需要--
    /*if (ptTorq->dwver() < 2 && iIndex > 0)
        iIndex--;*/
    return ptTorq->tshow(iIndex).strname().c_str();
}

CString CTorqueApp::GetTorqShowValue(TorqData::Torque *ptTorq, int iIndex)
{
    ASSERT_NULL_R(ptTorq, NULLSTR);
    COMP_BL_R(iIndex, 0, NULLSTR);
    COMP_BGE_R(iIndex, MAXPARANUM, NULLSTR);

    if(iIndex > ptTorq->tshow_size())
        return NULLSTR;

    // cur version iIndex 从1开始, 0为Factory
    // 20220928 按listNO存储，NO从1~15，和show序号对应，不需要--
    /*if (ptTorq->dwver() < 2 && iIndex > 0)
        iIndex--;*/
    return ptTorq->tshow(iIndex).strvalue().c_str();
}

string CTorqueApp::GetListShowName(SHOWCFG* ptShow, UINT NO)
{
    ASSERT_NULL_R(ptShow, NULLSTR);
    COMP_BGE_R(NO, ptShow->nListNum, NULLSTR);

    return ptShow->strShow[ptShow->nList[NO]];

}
string CTorqueApp::GetMainShowName(SHOWCFG* ptShow, UINT NO)
{
    ASSERT_NULL_R(ptShow, NULLSTR);
    COMP_BGE_R(NO, ptShow->nMainNum, NULLSTR);

    return ptShow->strShow[ptShow->nMain[NO]];
}

BOOL CTorqueApp::IsFixTube()
{
    return CheckFixTube(&m_tParaCfg);
}

BOOL CTorqueApp::CheckFixTube(PARACFG* ptCfg)
{
    ASSERT_NULL_R(ptCfg, FALSE);
    COMP_BG_R(ptCfg->tTubeCfg.nIndex, 0, TRUE);
    return FALSE;
}

/* \/:*?"<>| 加 . */
BOOL CTorqueApp::FindNotFileChar(CString strFileName)
{
    char    cNCFileName[10] = { '\\', '/', ':', '*', '?', '"', '<', '>', '|', '.'};
    int     i  = 0;

    for(i=0; i<10; i++)
    {
        if(strFileName.Find(cNCFileName[i]) >=0)
            return TRUE;
    }

    return FALSE;
}
