// Drill.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Drill.h"
#include "DrillDlg.h"
#include "Winver.h"
#include "Windows.h"
#include "excel.h"
#include "lodepng.h"
#include "CrashHandler.h"
#include "DlgPassword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define     TEST_QUALITY

/////////////////////////////////////////////////////////////////////////////
// CDrillApp

BEGIN_MESSAGE_MAP(CDrillApp, CWinApp)
    //{{AFX_MSG_MAP(CDrillApp)
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

#define INC_TCP_INFO()                  {                       \
        m_tSaveTCP.iCur += iLen;                                \
        pData += iLen;                                          \
        if(m_tSaveTCP.iCur >= MAXSAVELEN)   {                   \
            SaveTCPInfo();                                      \
            pData = &m_tSaveTCP.aucLog[m_tSaveTCP.iCur];        \
        }                                                       \
    }

   /////////////////////////////////////////////////////////////////////////////
   // CDrillApp construction

CDrillApp::CDrillApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDrillApp object

CDrillApp theApp;
CDBAccess theDB;

/////////////////////////////////////////////////////////////////////////////
// CDrillApp initialization

BOOL CALLBACK EnumWndProc(HWND hwnd, LPARAM lParam)
{
    HANDLE h = GetProp(hwnd, g_strPropName.c_str());

    COMP_BNE_R(h, g_hValue, TRUE);

    *(HWND*)lParam = hwnd;
    return FALSE;
}

void CDrillApp::InitSysPath()
{
    CTime   time = CTime::GetCurrentTime();//得到当前时间
    char buffer[MAX_PATH + 1] = { 0 };

    GetModuleFileName(NULL, buffer, MAX_PATH);
    (_tcsrchr(buffer, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
    m_strAppPath = buffer;

    /* 创建数据路径 */
    m_strDataPath = m_strAppPath + "Data\\";
    CreateDirectory(m_strDataPath.c_str(), NULL);

    /* 创建操作记录路径 */
    m_strLogPath = m_strAppPath + "Log\\";
    CreateDirectory(m_strLogPath.c_str(), NULL);

    /*注册文件路径*/
    m_strRegFile = m_strAppPath + REGNAME;

    /* 动态链接库路径 */
    m_strDllFile = m_strAppPath + CHNDLLNAME;

    /*构造保存正常Log数据的文件路径*/
    m_strLogFile = m_strLogPath;
    m_strLogFile += time.Format(IDS_STRDATEFORM);
    m_strLogFile += _T(".dbg");
    
    m_strTCPFile = m_strLogPath;
    m_strTCPFile += time.Format(IDS_STRDATEFORM);
    m_strTCPFile += _T("_TCP.dbg");
}

void CDrillApp::InitVariant()
{
    m_bShowCRC = FALSE;
    m_nPBHead = htonl(PBHEAD);
    m_tReg = CRegProc(m_strRegFile);
}

void CDrillApp::InitLanguage()
{
    for (int i = 0; i < LANGUAGE_NUM; i++)
    {
        m_hLangDLL[i] = NULL;
    }

    m_hLangDLL[LANGUAGE_CHINESE] = ::LoadLibrary(CHNDLLNAME);
    m_hLangDLL[LANGUAGE_ENGLISH] = ::LoadLibrary(ENGDLLNAME);

    LoadLanguageDll(g_tGlbCfg.nLangType, FALSE);
    m_ptCurShow = &m_tShowCfg[g_tGlbCfg.nLangType];
}

void CDrillApp::ClearTorqCfgPara(PARACFG* ptCfg)
{
    int i = 0;

    memset(&m_tParaCfg.tCtrl, 0, sizeof(CONTROLPARA));
    //memset(&m_tParaCfg.tComm, 0, sizeof(COMMONCFG));
    m_tParaCfg.strAlias.clear();
    m_tParaCfg.strMemo.clear();
    for (i = 0; i < MAXPARANUM; i++)
        ptCfg->strValue[i].clear();
    //m_tParaCfg.tCtrl.ucVer = 2;
}

void CDrillApp::ReloadTorqCfg()
{
    if (!theDB.ReadTorqCfgPara(m_ptCurShow->nAlias, &m_tParaCfg))
    {
        InitTorqCfgPara(&theApp.m_tParaCfg);
    }
}

void CDrillApp::InitTorqCfgPara(PARACFG* ptCfg)
{
    CONTROLPARA* ptCtrl = NULL;

    ASSERT_NULL(ptCfg);

    ClearTorqCfgPara(ptCfg);

    ptCtrl = &ptCfg->tCtrl;

    ptCtrl->fTorqConf[INDEX_TORQ_MAXLIMIT] = stod(LoadstringFromRes(IDS_STRPVMAXBOUND));
    //ptCtrl->fTorqConf[INDEX_TORQ_UPPERLIMIT] = stod(LoadstringFromRes(IDS_STRPVUPLIMIT));
    ptCtrl->fTorqConf[INDEX_TORQ_CONTROL] = stod(LoadstringFromRes(IDS_STRPVCONTROL));
    ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL] = stod(LoadstringFromRes(IDS_STRPVOPTTORQ));
    //ptCtrl->fTorqConf[INDEX_TORQ_LOWERLIMIT] = stod(LoadstringFromRes(IDS_STRPVLOWLIMIT));
    //ptCtrl->fTorqConf[INDEX_TORQ_SPEEDDOWN] = stod(LoadstringFromRes(IDS_STRPVSPEEDDOWN));
    ptCtrl->fTorqConf[INDEX_TORQ_SHOW] = stod(LoadstringFromRes(IDS_STRPVSHOW));
    /*ptCtrl->fTorqConf[INDEX_TORQ_BEAR] = stod(LoadstringFromRes(IDS_STRPVBEAR));
    ptCtrl->fTorqConf[INDEX_TORQ_UPPERTAI] = stod(LoadstringFromRes(IDS_STRPVUPTAI));
    ptCtrl->fTorqConf[INDEX_TORQ_LOWERTAI] = stod(LoadstringFromRes(IDS_STRPVLOWTAI));*/

    ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT] = stod(LoadstringFromRes(IDS_STRPVMAXCIR));
    ptCtrl->fTurnConf[INDEX_TURN_UPPERLIMIT] = stod(LoadstringFromRes(IDS_STRPVUPCIR));
    ptCtrl->fTurnConf[INDEX_TURN_CONTROL] = stod(LoadstringFromRes(IDS_STRPVCTRLCIR));
    ptCtrl->fTurnConf[INDEX_TURN_LOWERLIMIT] = stod(LoadstringFromRes(IDS_STRPVLOWCIR));
    /*ptCtrl->fTurnConf[INDEX_TURN_MAXDELTA] = stod(LoadstringFromRes(IDS_STRPVMAXDELTACIR));
    ptCtrl->fTurnConf[INDEX_TURN_MINDELTA] = stod(LoadstringFromRes(IDS_STRPVMINDELTACIR));*/

    ptCtrl->fFullRPM = stod(LoadstringFromRes(IDS_STRPVMAXRPM));
    //ptCtrl->fMinShlSlope = stod(LoadstringFromRes(IDS_STRPVMINSHLSLOPE));
}

void CDrillApp::InitValvePara(VALVECFG* ptCfg)
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

void CDrillApp::InitServerPara(SERVERCFG* ptCfg)
{
    ASSERT_NULL(ptCfg);

    ptCfg->strFTPAddr = LoadstringFromRes(IDS_STRSERVFTPADDR);// "kt888.synology.me";
    ptCfg->nFTPPort = stoi(LoadstringFromRes(IDS_STRSERVFTPPORT)); // 21;
    ptCfg->strUserName = LoadstringFromRes(IDS_STRSERVUSERNAME); // "ftpupload";
    ptCfg->strPassword = LoadstringFromRes(IDS_STRSERVPASSWORD); // "FTP@upload2";
    ptCfg->strTargetPath = LoadstringFromRes(IDS_STRSERVTARGETPATH); // "/homes/ftpupload/";
    ptCfg->nTCPPort = stoi(LoadstringFromRes(IDS_STRSERVTCPPORT)); // 9600;
}

void CDrillApp::InitWITSPara(WITSCFG* ptCfg)
{
    ASSERT_NULL(ptCfg);
    vector<int> showParas = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };    //{ 1, 3, 4, 6, 14 };
    vector<int> showItems = { 8016,8031,8012,8018,8032,8021,8033,8034,8035,8036,8037,8038,8039,8017,8040 }; // { 8016, 8012, 8018, 8021, 8017 };
    vector<int> fixItems;
    vector<int> repeatItems;
    vector<int> calItems;
    vector<int> configItems;

    ptCfg->ShowParas = showParas;
    ptCfg->ShowItems = showItems;

    fixItems.resize(WITSRPT_FIXHEADNUM);
    std::iota(fixItems.begin(), fixItems.end(), 8001);
    ptCfg->FixItems = fixItems;

    repeatItems.resize(WITSRPT_REPEATNUM);
    std::iota(repeatItems.begin(), repeatItems.end(), 8051);
    ptCfg->RepeatItems = repeatItems;

    calItems.resize(WITSRPT_CALPARANUM);
    std::iota(calItems.begin(), calItems.end(), 8024);
    ptCfg->CalItems = calItems;

    configItems.resize(WITSRPT_CONFIGNUM);
    std::iota(configItems.begin(), configItems.end(), 8061);
    ptCfg->ConfigItems = configItems;
}

BOOL CDrillApp::LoadLanguageDll(UINT nLang, BOOL bUpdate)
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

void CDrillApp::InitArray()
{
    m_ucDefaultMac[0] = 0x12;
    m_ucDefaultMac[1] = 0x79;
    m_ucDefaultMac[2] = 0x56;
    m_ucDefaultMac[3] = 0x58;
    m_ucDefaultMac[4] = 0x11;

    m_strDbgHead[0] = _T("#### ");
    m_strDbgHead[1] = _T("**** ");
    m_strDbgHead[2] = _T("#COL ");
    m_strDbgHead[3] = _T("#INF ");
    m_strDbgHead[4] = _T("@SND ");
    m_strDbgHead[5] = _T("@RCV ");
    m_strDbgHead[6] = _T("@TCP ");
}

void CDrillApp::InitTCPServer()
{
    string strInfo;
    if(isTCPServer())
        CloseSockets();

    //u32_BindIP = 0          -- > listen on all network adapters
    DWORD u32_EventTimeout = (PROCESS_EVENTS_IN_GUI_THREAD) ? 50 : INFINITE;
    DWORD u32_Err = mi_Socket.Listen(0, m_tServCfg.nTCPPort, u32_EventTimeout, MAX_SERVER_IDLE_TIME);
    if (u32_Err)
    {
        strInfo = string_format("Listen Error %s", mi_Socket.GetErrMsg(u32_Err));
        SaveTCPData(strInfo);
        CloseSockets();
        return;
    }

    // runs until an error occurred or all sockets have closed
#if PROCESS_EVENTS_IN_GUI_THREAD
    ProcessEvents();
#else
    DWORD u32_ID;
    mh_Thread = ::CreateThread(0, 0, ProcessEventThread, this, 0, &u32_ID);
#endif

    strInfo = string_format("%s Success.", __func__);
    SaveTCPData(strInfo);
}

ULONG WINAPI CDrillApp::ProcessEventThread(void* p_Param)
{
    CDrillApp* p_This = (CDrillApp*)p_Param;
    p_This->ProcessEvents();
    CloseHandle(p_This->mh_Thread);
    return 0;
}

// Process all events which occur on one of the open sockets
void CDrillApp::ProcessEvents()
{
    while (TRUE) // Loop runs until the main window was closed or a severe error occurred
    {
#if PROCESS_EVENTS_IN_GUI_THREAD
        PumpMessages();
#endif

        TCP::cSocket::cMemory* pi_RecvMem;
        SOCKET  h_Socket;
        DWORD u32_Event, u32_IP, u32_Read, u32_Sent;
        DWORD u32_Err = mi_Socket.ProcessEvents(&u32_Event, &u32_IP, &h_Socket, &pi_RecvMem, &u32_Read, &u32_Sent);
        string strInfo;

        if (u32_Err == ERROR_TIMEOUT) // 50 ms interval has elapsed
            continue;

        CString s_Msg, s_Events;
        if (u32_Event) // ATTENTION: u32_Event may be == 0 -> do nothing.
        {
            char s8_Events[200];
            mi_Socket.FormatEvents(u32_Event, s8_Events);
            s_Events += s8_Events;

            if (u32_Event & FD_READ)  s_Msg.Format(_T(" %d Bytes received."), u32_Read);
            if (u32_Event & FD_WRITE) s_Msg.Format(_T(" %d Bytes sent"), u32_Sent);
            
            strInfo = s_Events.GetBuffer(0);
            strInfo += s_Msg.GetBuffer(0);
            if (u32_Event & FD_ACCEPT || u32_Event & FD_CLOSE)
            {
                mi_Socket.GetAllConnectedSockets(&mi_SocketList);
                SOCKET h_Socket = mi_SocketList.GetKeyByIndex(0);
                struct sockaddr_in s;
                int len = sizeof(s);
                getpeername(h_Socket, (struct sockaddr FAR*) & s, &len);
                s_Msg.Format(_T(" Client IP: %d.%d.%d.%d, Port %d"), s.sin_addr.S_un.S_un_b.s_b1, s.sin_addr.S_un.S_un_b.s_b2,
                    s.sin_addr.S_un.S_un_b.s_b3, s.sin_addr.S_un.S_un_b.s_b4,
                    htons(s.sin_port));
                strInfo += s_Msg.GetBuffer(0);
            }
            SaveTCPData(strInfo);

            if (u32_Event & FD_READ && pi_RecvMem) // pi_RecvMem may be NULL if an error occurred!!
            {
                ProcessReceivedDataNormal(pi_RecvMem);
                /*switch (me_DemoMode)
                {
                case E_NORMAL:   ProcessReceivedDataNormal(pi_RecvMem); break;
                case E_PREFIXED: ProcessReceivedDataPrefix(pi_RecvMem); break;
                case E_TELNET:   ProcessReceivedDataTelnet(pi_RecvMem); break;
                }*/
            }
        }

        // It is NOT necessary to update the Combobox after FD_READ or FD_WRITE
        //mb_RefreshCombo |= (u32_Event & (FD_ACCEPT | FD_CONNECT | FD_CLOSE) || u32_Err);
        if (u32_Err)
        {
            // mi_Socket.Close() has been called -> don't print this error message
            if (u32_Err == WSAENOTCONN)
                break;

            // Print all the other error messages
            strInfo = string_format("ProcessEvent Error %s\r\n", mi_Socket.GetErrMsg(u32_Err));
            SaveTCPData(strInfo);

            // An error normally means that the socket has a problem -> abort the loop.
            // A few errors should not abort the processing:
            if (u32_Err != WSAECONNABORTED && // e.g. after the other side was killed in TaskManager 
                u32_Err != WSAECONNRESET && // Connection reset by peer.
                u32_Err != WSAECONNREFUSED && // FD_ACCEPT with already 62 clients connected
                u32_Err != WSAESHUTDOWN)      // Sending data to a socket just in the short timespan 
                break;                        //   between shutdown() and closesocket()
        }
    }; // end loop

    CloseSockets();

    SaveTCPData(_T("Stop Listening.\r\n"));
}

void CDrillApp::ProcessReceivedDataNormal(TCP::cSocket::cMemory* pi_RecvMem)
{
    char* s8_Buf = pi_RecvMem->GetBuffer();
    DWORD u32_Len = pi_RecvMem->GetLength();
    string s_String;

#if 0 // for hex TCP data
    std::stringstream ss;
    for (int i = 0; i < u32_Len; ++i)
        ss << hex << (int)s8_Buf[i];
    s_String = ss.str();
#else
    s_String.assign(s8_Buf, s8_Buf + u32_Len);
#endif

    s_String.insert(0, "Received: ");
    SaveTCPData(s_String, false);
    // Delete all received data from the receive memory
    pi_RecvMem->DeleteLeft(u32_Len);
}

BOOL CDrillApp::InitInstance()
{
    string  strFont;
    HDC     hdcScreen;

    CCrashHandler ch;
    ch.SetProcessExceptionHandlers();
    ch.SetThreadExceptionHandlers();

    //_CrtSetBreakAlloc(9450);

    AfxEnableControlContainer();

    AfxInitRichEdit2();

    // 添加了OLE/COM的初始化代码
    AfxOleInit();
    /* 避免弹出“由于另一个程序正在运行... */
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
    float dpiY = (float)GetDeviceCaps(hdcScreen, LOGPIXELSY) / 96;

    ScaleY = (ScaleY > dpiY) ? ScaleY : dpiY;
    m_ucDPILevel = (BYTE)ceil(ScaleY * 4);// + 0.5);

    m_nScreenX = GetSystemMetrics(SM_CXFULLSCREEN);
    m_nScreenY = GetSystemMetrics(SM_CYFULLSCREEN);

    strFont = LoadstringFromRes(IDS_STRFONTTIMESNR);
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
    InitSysPath();

    /* 打开LogFile*/
    m_tSaveLog.iCur = 0;
    m_SaveLogFile.Open(m_strLogFile.c_str(), CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone, NULL);
    
    /* 打开TCPLogFile*/
    m_tSaveTCP.iCur = 0;
    m_SaveTCPFile.Open(m_strTCPFile.c_str(), CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone, NULL);

    InitArray();
    /* 获取数据库文件 */
    UINT initStep = 0xFFFFFFFF;
    if (!theDB.InitConfigFromDB(initStep))
    {
        theApp.SaveMessage("InitConfigFromDB fail!! Using Default Config.");
        InitDefaultConfig(initStep);
    }

    // for debug
    // g_tGlbCfg.nTest = 4;
    /* 初始化数组、变量 */
    InitVariant();
    InitLanguage();
    SetRegistryKey(_T("zsg Applications"));

    //InitTCPServer();

    CDrillDlg dlg;
    m_pMainWnd = &dlg;
    thepDlg = &dlg;
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

int CDrillApp::ExitInstance()
{
    int i = 0;

    CloseSockets();

    m_SaveLogFile.Close();
    m_SaveTCPFile.Close();

    DeleteObject(m_tLineTextFont);
    DeleteObject(m_tRuleHFont);
    DeleteObject(m_tRuleVFont);
    DeleteObject(m_tPntTextFont);
    for (i = 0; i < DBG_MAXNUM; i++)
        m_strDbgHead[i].Empty();
    for (i = 0; i < LANGUAGE_NUM; i++)
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


void CDrillApp::ClearShowPara(SHOWCFG* ptShow)
{
    int     i = 0;
    int     j = 0;

    ASSERT_NULL(ptShow);

    ptShow->nParaNum = 0;
    //ptShow->nListNum = 0;
    ptShow->nMainNum = 0;
    ptShow->nFileName = 0;
    ptShow->nStatType = 0;
    ptShow->nJointOD = 0;
    //memset(ptShow->nList, 0, sizeof(UINT) * MAXPARANUM);
    memset(ptShow->nMain, 0, sizeof(UINT) * MAXMAINPARA);
    for (i = 0; i < MAXPARANUM; i++)
    {
        ptShow->strShow[i].clear();
    }
}

void CDrillApp::InitShowPara(SHOWCFG* ptShow, UINT nLang)
{
    int      i = 0;
    string   strPW;

    ASSERT_NULL(ptShow);
    CheckLanguage(nLang);

    for (i = 0; i < MAXPARANUM; i++)
    {
        ptShow->strShow[i] = g_tNameInfo[i].strName[nLang];
        //ptShow->nList[i] = i;
    }

    for (i = 0; i < MAXMAINPARA; i++)
    {
        ptShow->nMain[i] = g_nMainNameNO[i];
    }

    ptShow->nParaNum = MAXPARANUM;
    //ptShow->nListNum = MAXPARANUM - 1;
    ptShow->nMainNum = MAXMAINPARA;
    ptShow->nFileName = 7;
    ptShow->nStatType = 2;
    ptShow->nJointOD = 4;
}

void CDrillApp::InitDefaultConfig(int initStep)
{
    int i = 0;
    string  strInfo;

    strInfo = string_format("InitConfigFromDB fail(0x%x)!", initStep);
    SaveMessage(strInfo);

    // global parameter
    if (initStep & DB_INIT_GLOBAL)
        InitGlobalPara();

    // Show parameter
    for (i = 0; i < LANGUAGE_NUM; i++)
    {
        if (initStep & DB_INIT_SHOW)
            InitShowPara(&m_tShowCfg[i], i);
        if (initStep & DB_INIT_XLS_STAT)
            InitXlsStatPara(&m_tXlsStatCfg[i]);
    }

    if (initStep & DB_INIT_TORQUE_CFG)
    {
        InitTorqCfgPara(&m_tParaCfg);
    }
    if (initStep & DB_INIT_VALVE_CFG)
    {
        InitValvePara(&m_tValveCfg);
    }
    if (initStep & DB_INIT_SERVER_CFG)
    {
        InitServerPara(&m_tServCfg);
    }
    if (initStep & DB_INIT_WITS_CFG)
    {
        InitWITSPara(&m_tWITSCfg);
    }

    return;
}

void CDrillApp::InitGlobalPara()
{
    g_tGlbCfg.nLangType = LANGUAGE_CHINESE;
    g_tGlbCfg.nPortNO = 1;
    g_tGlbCfg.nPlusPerTurn = stoi(LoadstringFromRes(IDS_STRPVPLUS));
    g_tGlbCfg.nTorqUnit = 0;
    g_tGlbCfg.nCollectDur = 100;
    g_tGlbCfg.nResetDur = 10000;
    g_tGlbCfg.nSaveDur = 30000;
    //g_tGlbCfg.nIPShowMode = 1;
    g_tGlbCfg.nZoomIn = 5;
    g_tGlbCfg.nImgNum = 8;
    g_tGlbCfg.nTest = 0;
    //g_tGlbCfg.iBreakOut = 0;

    g_tGlbCfg.fDiscount = 1;
    g_tGlbCfg.fMulti = 1;
    g_tGlbCfg.fRpmAdj = 3.5;
    //g_tGlbCfg.fIPDeltaVal = 0.1;

    //g_tGlbCfg.bCheckIP = true;
    g_tGlbCfg.bBigTorq = false;
    g_tGlbCfg.bDateBehind = false;

    g_tGlbCfg.strPassWord = LoadstringFromRes(IDS_STRPVPASSWORD);
    g_tGlbCfg.strDataPath = NULLSTR;

    g_tGlbCfg.strUnit = string_format(LoadstringFromRes(IDS_STRTORQNMUNIT).c_str(), BIGPOINT);
}

BOOL CDrillApp::SetShowNameFromID(string lsID, SHOWCFG* ptShow, UINT nLang)
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

BOOL CDrillApp::SetShowNOFromID(int iType, string lsID, SHOWCFG* ptShow)
{
    UINT i;
    UINT iParaNum = 0;
    vector<int> lsNO;
    UINT* plsNO = NULL;

    ASSERT_NULL_R(ptShow, FALSE);
    if (lsID.empty())
        return FALSE;

    //iParaNum = ptShow->nListNum;
    //if (0 == iType)     //当前list
    //{
    //    iParaNum = ptShow->nListNum;
    //    plsNO = &ptShow->nList[0];
    //    lsNO = GetIDFromList(lsID);
    //}
    //else                //当前main
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

int CDrillApp::GetOptionIDbyValue(string name, string value, UINT nLang)
{
    if (name.empty() || value.empty())
        return -1;
    CheckLanguage(nLang);

    return 0;
}
BOOL CDrillApp::UpdateCurOptions(WORD wNum, string value[], UINT nLang)
{
    CheckLanguage(nLang);
    return FALSE;
}

void CDrillApp::InitXlsStatPara(XLSSTATCFG* ptStat)
{
    ASSERT_NULL(ptStat);

    memset(ptStat, 0, sizeof(XLSSTATCFG));

    // 7,16,6,11,15,
    ptStat->GenPara[0] = 7;        /* 7  井号 */
    ptStat->GenPara[1] = 16;       /* 16 井深 */
    ptStat->GenPara[2] = 6;        /* 6  甲方名称 */
    ptStat->GenPara[3] = 11;       /* 11 操作工 */
    ptStat->GenPara[4] = 15;       /* 15 入井序号 */

    // 1,2,3,5,8,12,
    ptStat->JobPara[0] = 1;         /* 1  钻杆材质 */
    ptStat->JobPara[1] = 2;         /* 2 钻杆规格 */
    ptStat->JobPara[2] = 3;         /* 3  钻杆级别 */
    ptStat->JobPara[3] = 5;         /* 5 油田名称 */
    ptStat->JobPara[4] = 8;         /* 8 勘探公司 */
    ptStat->JobPara[5] = 12;        /* 12 钻杆钳 */

    // 1,2,
    ptStat->InfoPara[0] = 1;        /* 1  钻杆材质 */
    ptStat->InfoPara[1] = 2;        /* 2 钻杆规格 */
}

/* 扭矩参数 相差 DIFF_TORQUE 10 */
void CDrillApp::AdjustTorquePara(CONTROLPARA* ptCtrl)
{
    ASSERT_NULL(ptCtrl);

    CHECK_PARA_UP(ptCtrl->fTorqConf[INDEX_TORQ_CONTROL], ptCtrl->fTorqConf[INDEX_TORQ_MAXLIMIT], DIFF_TORQUE);
    CHECK_PARA_UP(ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL], ptCtrl->fTorqConf[INDEX_TORQ_MAXLIMIT], DIFF_TORQUE);
    CHECK_PARA_UP(ptCtrl->fTorqConf[INDEX_TORQ_SHOW], ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL], DIFF_TORQUE);
    //CHECK_PARA_UP(ptCtrl->fTorqConf[INDEX_TORQ_SHOW],       ptCtrl->fTorqConf[INDEX_TORQ_SPEEDDOWN],  DIFF_TORQUE);
    //CHECK_PARA_LOW(ptCtrl->fTorqConf[INDEX_TORQ_BEAR],      ptCtrl->fTorqConf[INDEX_TORQ_SPEEDDOWN],  DIFF_TORQUE);
}

/* 周参数 相差 DIFF_CIRCUIT 0.1 */
void CDrillApp::AdjustCircuitPara(CONTROLPARA* ptCtrl)
{
    ASSERT_NULL(ptCtrl);

    CHECK_PARA_UP(ptCtrl->fTurnConf[INDEX_TURN_UPPERLIMIT], ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT], DIFF_CIRCUIT);
    CHECK_PARA_UP(ptCtrl->fTurnConf[INDEX_TURN_CONTROL], ptCtrl->fTurnConf[INDEX_TURN_UPPERLIMIT], DIFF_CIRCUIT);
    CHECK_PARA_UP(ptCtrl->fTurnConf[INDEX_TURN_LOWERLIMIT], ptCtrl->fTurnConf[INDEX_TURN_CONTROL], DIFF_CIRCUIT);
}

void CDrillApp::AdjustParaValue(PARACFG* ptCfg)
{
    int         i = 0;
    CONTROLPARA* ptCtrl = NULL;

    ASSERT_NULL(ptCfg);
    ptCtrl = &ptCfg->tCtrl;

    /* 扭矩参数 相差10 */
    AdjustTorquePara(ptCtrl);

    /* 扭拧周数 相差0.1 */
    AdjustCircuitPara(ptCtrl);
}

bool CDrillApp::GetProductVersion(CString& strVersion)
{
    DWORD   dwSize = 0;
    UINT    nSize = 0;
    LPVOID  pBlock = NULL;
    char* pVerValue = NULL;
    int     iPlace = -1;
    CString strTemp;

    dwSize = GetFileVersionInfoSize(m_strDllFile.c_str(), NULL);
    COMP_BLE_R(dwSize, 0, false);
    pBlock = malloc(dwSize + 1);
    ASSERT_NULL_R(pBlock, false);
    GetFileVersionInfo(m_strDllFile.c_str(), 0, dwSize, pBlock);
    ASSERT_ZERO_R(dwSize, false);

    VerQueryValue(pBlock, "\\StringFileInfo\\080404b0\\ProductVersion", (LPVOID*)&pVerValue, &nSize);
    ASSERT_ZERO_R(nSize, false);

    strTemp.Format("%s", pVerValue);
    iPlace = strTemp.ReverseFind(',');
    COMP_BE_R(iPlace, -1, false);

    strVersion = strTemp.Right(strTemp.GetLength() - iPlace - 1);

    free(pBlock);

    return true;
}

/* 获取MAC地址 */
void CDrillApp::GetMACAddr(UCHAR* pcMac)
{
    NCB     ncb;
    UCHAR   uRetCode;
    ASTAT   Adapter;
    int     i = 0;

    ASSERT_NULL(pcMac);

    memset(&ncb, 0, sizeof(ncb));
    ncb.ncb_command = NCBRESET;
    ncb.ncb_lana_num = 0; // 指定网卡号
    // 首先对选定的网卡发送一个NCBRESET命令,以便进行初始化
    Netbios(&ncb);

    memset(&ncb, 0, sizeof(ncb));
    ncb.ncb_command = NCBASTAT;
    ncb.ncb_lana_num = 0; // 指定网卡号
    ncb.ncb_callname[0] = '*';
    ncb.ncb_callname[1] = '\0';
    ncb.ncb_buffer = (unsigned char*)&Adapter; // 指定返回的信息存放的变量
    ncb.ncb_length = sizeof(Adapter);
    // 接着,可以发送NCBASTAT命令以获取网卡的信息
    uRetCode = Netbios(&ncb);
    // 强制设定为获取网卡失败，使用默认Mac
    uRetCode = 1;
    if (uRetCode == 0)
    {
        for (i = 0; i < 5; i++)
            *(pcMac + i) = Adapter.adapt.adapter_address[i + 1];
    }
    else
    {
        memcpy(pcMac, m_ucDefaultMac, 5);
    }

    return;
}

int CDrillApp::GetMainIndex(UINT nNO)
{
    UINT        i = 0;
    string      strFindName;

    strFindName = GetMainShowName(m_ptCurShow, nNO);

    for (i = 0; i < m_ptCurShow->nParaNum && i < MAXPARANUM; i++)
    {
        if (strFindName == m_tShowCfg[g_tGlbCfg.nLangType].strShow[i])
        {
            return i;
        }
    }
    return -1;
}

int CDrillApp::GetMainIndexfromData(UINT nNO, TorqData::Torque* ptTorq)
{
    int         i = 0;
    string      strFindName;
    string      strName;

    ASSERT_NULL_R(ptTorq, -1);

    strFindName = GetMainShowName(m_ptCurShow, nNO);
    if (strFindName.empty())
        return -1;

    for (i = 0; i < ptTorq->tshow_size() && i < MAXPARANUM; i++)
    {
        strName = GetTorqShowName(ptTorq, i);
        if (strName == strFindName)
            return i;
    }
    return -1;
}

string  CDrillApp::GetQualityInfo(TorqData::Torque* ptTorq)
{
    int     i = 0;
    DWORD   dwFlag = 1;
    string  strQuality;
    //CString strInfo;
    DWORD   dwQuality = 0;

    ASSERT_NULL_R(ptTorq, strQuality);

    /* 卸扣不考虑质量问题，直接返回卸扣图形 */
    if (HaveBreakout(ptTorq) && !HaveMakeUP(ptTorq))
        return LoadstringFromRes(IDS_STRMARKSHACKLE);

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

int CDrillApp::GetQualityIndex(TorqData::Torque* ptTorq)
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

bool CDrillApp::SaveList2XlsFile(string filename, CListCtrl* ptList)
{
    int         i = 0, j = 0;
    int         iRowNum = 0, iColNum = 0;
    //LVCOLUMN    tColData;
    CString     strInfo;
    //CString     strColName;
    HDITEM      hdi;
    TCHAR       lpBuffer[256] = { 0 };
    CString     strTime;
    Excel       tSaveExc;
    CTime       tDay = CTime::GetCurrentTime();
    CStringList slContent;

    ASSERT_NULL_R(ptList, false);

    if (filename.empty())
    {
        strTime.Format("%04d%02d%02d%02d%02d%02d",
            tDay.GetYear(),      //yyyy年
            tDay.GetMonth(),     //mm月份
            tDay.GetDay(),       //dd日
            tDay.GetHour(),      //hh小时
            tDay.GetMinute(),    //mm分钟
            tDay.GetSecond());   //ss秒
        filename = strTime + ".xlsx";
    }

    if (!tSaveExc.initExcel())
    {
        strInfo.Format(IDS_STRINFNODRIVE);
        SaveShowMessage(strInfo.GetBuffer(0));
        return false;
    }

    if (!tSaveExc.open(filename.c_str()) || !tSaveExc.loadSheet(1))
    {
        tSaveExc.close();
        tSaveExc.release();
        return false;
    }

    CHeaderCtrl* ptHead = ptList->GetHeaderCtrl();
    if (!ptHead)
    {
        tSaveExc.close();
        tSaveExc.release();
        return false;
    }

    iColNum = ptHead->GetItemCount();
    hdi.mask = HDI_TEXT;
    hdi.pszText = lpBuffer;
    hdi.cchTextMax = 256;
    for (i = 0; i < iColNum; i++)
    {
        ptHead->GetItem(i, &hdi);
        slContent.AddTail(hdi.pszText);
    }

    // 导入list里面的数据
    iRowNum = ptList->GetItemCount();
    for (i = 0; i < iRowNum; i++)
    {
        for (j = 0; j < iColNum; j++)
        {
            slContent.AddTail(ptList->GetItemText(i, j));
        }
    }

    tSaveExc.SetMultiCellContent(1, iRowNum + 1, 1, iColNum, slContent);

    tSaveExc.saveAsXLSFile(filename);
    tSaveExc.close();
    tSaveExc.release();

    return true;
}

void CDrillApp::ShowMainTitle()
{
    string strAppName;
    CWnd* m_pCWnd = AfxGetMainWnd();

    m_nTorqMulti = 1;

    if (m_tReg.Reged())
    {
        strAppName = LoadstringFromRes(IDS_STRTITLE);
        //strAppName.Format(IDS_STRTITLE);

        if (g_tGlbCfg.bBigTorq)
        {
            m_nTorqMulti = 10;
            strAppName = LoadstringFromRes(IDS_STRBIGTITLE);
            //strAppName.Format(IDS_STRBIGTITLE);
        }
        ::SetWindowText(*m_pCWnd, strAppName.c_str());
        return;
    }

    /* else */
    strAppName = LoadstringFromRes(IDS_STRTRYOUT);
    //strAppName.Format(IDS_STRTRYOUT);
    if (g_tGlbCfg.bBigTorq)
    {
        m_nTorqMulti = 10;
        strAppName = LoadstringFromRes(IDS_STRBIGTRYOUT);
        //strAppName.Format(IDS_STRBIGTRYOUT);
    }

    ::SetWindowText(*m_pCWnd, strAppName.c_str());
    return;
}

/* 无效数据直接跳过，节省读取时间
   无效数据现在都以"####"开头
   CRC数据也不计数了 */
BOOL CDrillApp::IsDebugInfo(string strContent)
{
    int     i = 0;
    int     iPlace = -1;

    COMP_BTRUE_R(strContent.empty(), FALSE);

    for (i = 0; i < DBG_MAXNUM; i++)
    {
        iPlace = strContent.find(m_strDbgHead[i].GetBuffer(0));

        COMP_BNE_R(iPlace, -1, TRUE);
    }

    return FALSE;
}

void CDrillApp::SaveCurTime()
{
    char* pData = NULL;
    int         iLen = 0;
    SYSTEMTIME  ts;

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    GetLocalTime(&ts);

    iLen = sprintf_s(pData, SPRINTFLEN, "%02d:%02d:%02d.%03d : ",
        ts.wHour, ts.wMinute, ts.wSecond, ts.wMilliseconds);
    INC_DBG_INFO();

    return;
}

void CDrillApp::SaveDbgHead(UINT nType)
{
    char* pData = NULL;
    int     iLen = DBG_HEADLEN;

    COMP_BGE(nType, DBG_MAXNUM);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    memcpy(pData, m_strDbgHead[nType], DBG_HEADLEN);
    INC_DBG_INFO();

    return;
}

void CDrillApp::SaveCurTimeAndHead(UINT nType)
{
    /* Time */
    SaveCurTime();

    /* debug head */
    SaveDbgHead(nType);
}

void CDrillApp::SaveStreamData(string strStream)
{
    char* pData = NULL;
    int     iLen = 0;

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    iLen = sprintf_s(pData, SPRINTFLEN, "%s", strStream.c_str());
    INC_DBG_INFO();
}

void CDrillApp::SaveTCPData(string strData, bool Status)
{
    int     iLen = 0;
    char* pData = NULL;
    SYSTEMTIME  ts;

    ASSERT_ZERO(strData.size());
    if (!Status && !m_bShowCRC)
        return;

    pData = &m_tSaveTCP.aucLog[m_tSaveTCP.iCur];
    /* Send Communication Time && Head */
    GetLocalTime(&ts);

    iLen = sprintf_s(pData, SPRINTFLEN, "%02d:%02d:%02d.%03d : %s",
        ts.wHour, ts.wMinute, ts.wSecond, ts.wMilliseconds, m_strDbgHead[DBG_TCPMSG].GetBuffer(0));
    INC_TCP_INFO();

    strData += "\r\n";

    iLen = (int)strData.size();
    if (m_tSaveLog.iCur + iLen >= MAXSAVELEN)
    {
        SaveTCPInfo();
    }

    memcpy(&m_tSaveTCP.aucLog[m_tSaveTCP.iCur], (LPCTSTR)strData.c_str(), iLen);
    m_tSaveTCP.iCur += iLen;
    return;
}

void CDrillApp::SaveHexData(BYTE* pucRcvByte, WORD wLen)
{
    int     i = 0;
    int     iLen = 0;
    char* pData = NULL;

    ASSERT_NULL(pucRcvByte);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    /* source serial data */
    for (i = 0; i < wLen; i++)
    {
        iLen = sprintf_s(pData, SPRINTFLEN, "%02x ", pucRcvByte[i]);
        INC_DBG_INFO();
    }

    iLen = 2;
    memcpy(pData, "\r\n", iLen);
    INC_DBG_INFO();
}

void CDrillApp::SaveCommunication(BYTE* msg, WORD wLen, UINT nType)
{
    int     iLen = 0;
    char* pData = NULL;

    ASSERT_NULL(msg);
    ASSERT_ZERO(wLen);
    COMP_BL(nType, DBG_SNDCMD);
    COMP_BG(nType, DBG_RCVCOM);
    COMP_BFALSE(m_bShowCRC);

    /* Send Communication Time */
    SaveCurTimeAndHead(nType);

    SaveHexData(msg, wLen);
}

/* 保存正常的原始数据，测试和定位使用
   正常有效数据，不需要DEBUG HEAD
   在CTRL+SHIFT+T 显示CRC 框后生效 */
void CDrillApp::SaveOrdData(ORGDATA* ptOrgData, BYTE* pucRcvByte, WORD wLen)
{
    char* pData = NULL;
    int     iLen = 0;

    ASSERT_NULL(ptOrgData);

    COMP_BFALSE(m_bShowCRC);

    /* Time */
    SaveCurTime();

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    /* 原始串口的扭矩和脉冲数据 */
    if (wLen == PORT_VLDVLVLEN)  // 打印阀门的2个状态字节
    {
        iLen = sprintf_s(pData, SPRINTFLEN, "%02x %02x %02x %02x %02x %02x %02x %02x %02x ",
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
        iLen = sprintf_s(pData, SPRINTFLEN, "%02x %02x %02x %02x %02x %02x %02x ",
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
    iLen = sprintf_s(pData, 60, "T%10ld, P%10ld, S%4d, N%4d, R%6.2f\r\n",
        ptOrgData->nTorque,
        ptOrgData->nPlus,
        ptOrgData->ucStatus,
        ptOrgData->ucPointNum,
        ptOrgData->fRpm);
    INC_DBG_INFO();

    /* 有效数据的长度应该大于12，有长度为24的数据，也可以尽可能取出一条来，
       但是前12个数据通过CRC校验，则当做普通数据处理，调试开关打开时也将原始数据保存下来 */
    if (wLen != PORT_VALIDLEN && wLen != PORT_VLDVLVLEN)
    {
        /* Time */
        SaveCurTimeAndHead(DBG_HASH);

        pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

        /* data len */
        iLen = sprintf_s(pData, SPRINTFLEN, "ORG DATA(len%2d) ", wLen);
        INC_DBG_INFO();

        /* source serial data */
        SaveHexData(pucRcvByte, wLen);
    }
    return;
}

/* 保存正常的原始数据，测试和定位使用
   正常有效数据，不需要DEBUG HEAD
   在CTRL+SHIFT+T 显示CRC 框后生效 */
void CDrillApp::SaveMultiData(ORGDATA* ptOrgData, BYTE* pucRcvByte, WORD wLen)
{
    char* pData = NULL;
    int     iLen = 0;
    int     i = 0;
    WORD    tmpTorq = 0;

    ASSERT_NULL(ptOrgData);

    COMP_BFALSE(m_bShowCRC);

    /* Time */
    SaveCurTime();

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    iLen = sprintf_s(pData, SPRINTFLEN, "%02x %02x ", 0xFF, 0x15);
    INC_DBG_INFO();

    for (i = 4; i < wLen; i++)
    {
        iLen = sprintf_s(pData, SPRINTFLEN, "%02x ", pucRcvByte[i]);
        INC_DBG_INFO();
    }

    iLen = 2;
    memcpy(pData, "\r\n", iLen);
    INC_DBG_INFO();

    return;
}

BOOL CDrillApp::MsgLenIsZero(WORD wLen, UINT nType)
{
    char* pData = NULL;
    int     iLen = 0;

    COMP_BNE_R(wLen, 0, FALSE);

    /* 只有m_bShowCRC打开，才记录长度为0的数据 */
    COMP_BFALSE_R(m_bShowCRC, TRUE);

    SaveCurTimeAndHead(nType);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    /* Save Info */
    iLen = sprintf_s(pData, SPRINTFLEN, "Recv Len 0 Data!\r\n");
    INC_DBG_INFO();

    return TRUE;
}

/* 保存CRC错误的原始数据，测试和定位使用
   有CRC错误都会自动保存
   CRC或者串口错误、长度不对数据 */
void CDrillApp::SaveCrcErrorData(BYTE* pucRcvByte, WORD wLen, UINT& nCRCErr)
{
    char* pData = NULL;
    int     iLen = 0;
    //CDrillDlg* pdlgMain = (CDrillDlg*)m_pMainWnd;

    ASSERT_NULL(pucRcvByte);

    COMP_BNE(g_tGlbCfg.nTest, COLL_PORT);

    /* 消息长度为0，处理后直接返回 */
    COMP_BTRUE(MsgLenIsZero(wLen, DBG_HASH));

    /* 对话框中的CRC错误数目+1 */
    nCRCErr++;

    SaveCurTimeAndHead(DBG_HASH);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];
    /* data len */
    iLen = sprintf_s(pData, SPRINTFLEN, "CRC  Error(len%2d) ", wLen);
    INC_DBG_INFO();

    /* source serial data */
    SaveHexData(pucRcvByte, wLen);

    return;
}

/* 采集数据时出错信息保存 */
void CDrillApp::SaveCollectErrorData(string strError, BYTE* pucRcvByte, WORD wLen)
{
    char* pData = NULL;
    int     iLen = 0;
    int     i = 0;

    ASSERT_NULL(pucRcvByte);//m_wRcvLen

    COMP_BNE(g_tGlbCfg.nTest, COLL_PORT);

    /* 消息长度为0，处理后直接返回 */
    COMP_BTRUE(MsgLenIsZero(wLen, DBG_COLLECT));

    SaveCurTimeAndHead(DBG_COLLECT);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];
    /* Info and data len */
    iLen = sprintf_s(pData, SPRINTFLEN, "%s (len%2d) ", strError.c_str(), wLen);
    INC_DBG_INFO();

    /* source serial data */
    SaveHexData(pucRcvByte, wLen);

    return;
}

/* 保存采集数据普通信息 */
void CDrillApp::SaveCollectOrgData(BYTE* pucRcvByte, WORD wLen)
{
    char* pData = NULL;
    int     iLen = 0;
    int     i = 0;

    ASSERT_NULL(pucRcvByte);
    /* COMP_BFALSE(m_bShowCRC); */

    SaveCurTimeAndHead(DBG_COLLECT);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];
    /* data len */
    iLen = sprintf_s(pData, SPRINTFLEN, "ORG  Data(len%2d) ", wLen);
    INC_DBG_INFO();

    /* source serial data */
    SaveHexData(pucRcvByte, wLen);

    return;
}

//保存复位时的串口数据
void CDrillApp::SaveResetData(BYTE* pucRcvByte, WORD wLen)
{
    char* pData = NULL;
    int     iLen = 0;
    int     i = 0;

    ASSERT_NULL(pucRcvByte);

    COMP_BFALSE(m_bShowCRC);
    COMP_BNE(g_tGlbCfg.nTest, COLL_PORT);

    SaveCurTimeAndHead(DBG_START);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];
    /* data len */
    iLen = sprintf_s(pData, SPRINTFLEN, "Reset Data(len%2d) ", wLen);
    INC_DBG_INFO();

    /* source serial data */
    SaveHexData(pucRcvByte, wLen);

    return;
}

//保存上位机发送到串口数据
void CDrillApp::SaveSendData(string strCmd, BYTE* pucRcvByte, WORD wLen)
{
    char* pData = NULL;
    int     iLen = 0;
    int     i = 0;

    ASSERT_NULL(pucRcvByte);

    /* COMP_BFALSE(m_bShowCRC); */

    SaveCurTimeAndHead(DBG_SNDCMD);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    /* data len */
    iLen = sprintf_s(pData, SPRINTFLEN, "%s(len%2d)\r\n", strCmd.c_str(), wLen);
    INC_DBG_INFO();

    /* 对外不展现modbus编码 */
    return;

    /* source serial data */
    SaveHexData(pucRcvByte, wLen);
}

//保存串口发送错误数据
void CDrillApp::SaveSendFailure(UINT nCmdType)
{
    char* pData = NULL;
    int     iLen = 0;
    string  strCmd;

    COMP_BNE(g_tGlbCfg.nTest, COLL_PORT);

    SaveCurTimeAndHead(DBG_HASH);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];
    strCmd = g_strCmdName[nCmdType - SCMREAD];

    /* Save Info */
    iLen = sprintf_s(pData, SPRINTFLEN, "Send %s Command Failure!\r\n", strCmd.c_str());
    INC_DBG_INFO();
    return;
}

void CDrillApp::AutoupdateLogFile()
{
    CTime   time = CTime::GetCurrentTime();//得到当前时间
    string  newLog;

    newLog = m_strLogPath;
    newLog += time.Format(IDS_STRDATEFORM);
    newLog += _T(".dbg");
    if (m_strLogFile == newLog)
        return;

    m_SaveLogFile.Close();
    m_strLogFile = newLog;

    m_SaveLogFile.Open(m_strLogFile.c_str(), CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone, NULL);
    
    m_SaveTCPFile.Close();
    newLog.insert(newLog.size()-4, "_TCP");
    m_strTCPFile = newLog;

    m_SaveTCPFile.Open(m_strTCPFile.c_str(), CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone, NULL);
}

void CDrillApp::SaveLogInfo()
{
    COMP_BL(m_tSaveLog.iCur, 1);

    // 20230524 到新的一天，自动更新log文件
    AutoupdateLogFile();

    m_SaveLogFile.SeekToEnd();
    m_SaveLogFile.Write(m_tSaveLog.aucLog, m_tSaveLog.iCur);
    m_tSaveLog.iCur = 0;

    return;
}

void CDrillApp::SaveTCPInfo()
{
    COMP_BL(m_tSaveTCP.iCur, 1);

    // 20230524 到新的一天，自动更新log文件
    AutoupdateLogFile();

    m_SaveTCPFile.SeekToEnd();
    m_SaveTCPFile.Write(m_tSaveTCP.aucLog, m_tSaveTCP.iCur);
    m_tSaveTCP.iCur = 0;

    return;
}

//保存程序运行状态
void CDrillApp::SaveAppStatus(UINT nStatus, string strInfo)
{
    char* pData = NULL;
    int     iLen = 0;

    COMP_BGE(nStatus, STATUS_MAXNUM);

    /* Send app status Time */
    SaveCurTimeAndHead(DBG_HASH);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    /* Save Info */
    //iLen = sprintf_s(pData, 100, "Application is %s Status(%s)!\r\n", (LPCTSTR)g_strStatus[nStatus].c_str(), (LPSTR)(LPCTSTR)strInfo);
    iLen = sprintf_s(pData, 100, "Application is %s Status(%s)!\r\n", (LPCTSTR)g_strStatus[nStatus].c_str(), (LPCTSTR)strInfo.c_str());
    INC_DBG_INFO();
    return;
}

//保存端口操作记录 打开/关闭
void CDrillApp::SavePortOper(UINT nPortOpr)
{
    return;

    char* pData = NULL;
    int     iLen = 0;

    COMP_BGE(nPortOpr, PORTOPR_MAXNUM);

    /* Send port operation Time */
    SaveCurTimeAndHead(DBG_HASH);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    /* Save Info */
    iLen = sprintf_s(pData, SPRINTFLEN, "Port %d(%d) is %s!\r\n", g_tGlbCfg.nPortNO, g_tGlbCfg.nBaudRate, (LPCTSTR)g_strPortOpr[nPortOpr].c_str());
    INC_DBG_INFO();
    return;
}

//保存发送消息时的异常的串口缓冲数据
void CDrillApp::SavePortBufData(BYTE* pucRcvByte, WORD wLen, UINT nClashSta)
{
    char* pData = NULL;
    int     iLen = 0;
    int     i = 0;

    ASSERT_NULL(pucRcvByte);
    ASSERT_ZERO(wLen);

    /* data Time */
    SaveCurTimeAndHead(DBG_HASH);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    /* data len */
    if (nClashSta == RS_READCLASH)
        iLen = sprintf_s(pData, SPRINTFLEN, "ReadCls  Data(len%2d) ", wLen);
    else if (nClashSta == RS_RESETCLASH)
        iLen = sprintf_s(pData, SPRINTFLEN, "ResetCls Data(len%2d) ", wLen);
    INC_DBG_INFO();

    /* source serial data */
    SaveHexData(pucRcvByte, wLen);

    return;
}

/* 保存MessageBox显示的信息到文件 */
void CDrillApp::SaveMessage(string strMessage)
{
    char* pData = NULL;
    int     iLen = 0;

    /* Send MessageBox Time */
    SaveCurTimeAndHead(DBG_MESSAGE);

    pData = &m_tSaveLog.aucLog[m_tSaveLog.iCur];

    /* Save Info */
    iLen = sprintf_s(pData, 200, "%s\r\n", (LPCTSTR)strMessage.c_str());
    INC_DBG_INFO();
    return;
}

void CDrillApp::SaveShowMessage(string strMessage, UINT nType)
{
    SaveMessage(strMessage);
    AfxMessageBox(strMessage.c_str(), nType);
}

void CDrillApp::AdaptDlgCtrlSize(CDialog* pdlgAdapt, UINT nSheetType)
{
    double      fRatiox = 1, fRatioy = 1, fRatio = 1;
    int         woc;
    POINT       Newp;
    CRect       oldRect;
    CRect       newRect;
    CRect       Rect;
    //左右上角的数据  
    CPoint      OldTLPoint, NewTLPint;
    CPoint      OldBRPoint, NewBRPint;
    CFont* pFont = NULL;
    UINT        nPageHeight = 0;

    ASSERT_NULL(pdlgAdapt);

    if (nSheetType == 2)
    {
        //nPageWidth = 20 * m_ucDPILevel / 4;
        nPageHeight = 120 * m_ucDPILevel / 4;
    }

    pdlgAdapt->GetClientRect(&oldRect);                        /* 对话框大小 */

    COMP_BGE(m_nScreenY, (oldRect.Height() + nPageHeight));

    fRatiox = 1.0 * m_nScreenX / oldRect.Width();
    fRatioy = 1.0 * m_nScreenY / (oldRect.Height() + nPageHeight);
    fRatio = (fRatiox < fRatioy) ? fRatiox : fRatioy;
    if (nSheetType == 1)
    {
        fRatio = m_fSheetRatio + 0.05;
    }
    else if (nSheetType == 2)
    {
        if (m_fSheetRatio != 1.0)
        {
            fRatio = m_fSheetRatio;
        }
        else
        {
            m_fSheetRatio = fRatio;
        }
    }

    Newp.x = int(oldRect.Width() * fRatio);
    Newp.y = int(oldRect.Height() * fRatio);

    /* 对话框先缩小 */
    pdlgAdapt->MoveWindow(0, 0, Newp.x, Newp.y);
    pdlgAdapt->CenterWindow();

    //获取现在对话框的大小 
    pdlgAdapt->GetClientRect(&newRect);     //取客户区大小  
    HWND  hwndChild = ::GetWindow(pdlgAdapt->m_hWnd, GW_CHILD);

    while (hwndChild)
    {
        //取得ID  
        woc = ::GetDlgCtrlID(hwndChild);
        pdlgAdapt->GetDlgItem(woc)->GetWindowRect(Rect);
        pdlgAdapt->ScreenToClient(Rect);

        Rect.left = Rect.left * newRect.Width() / oldRect.Width();
        Rect.right = Rect.right * newRect.Width() / oldRect.Width();
        Rect.top = Rect.top * newRect.Height() / oldRect.Height();
        Rect.bottom = Rect.bottom * newRect.Height() / oldRect.Height();
        pdlgAdapt->GetDlgItem(woc)->MoveWindow(Rect, TRUE);

        hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
    }
}

void CDrillApp::ReOpenWindow()
{
    CDrillDlg dlgMain;

    m_pMainWnd = &dlgMain;
    thepDlg = &dlgMain;
    dlgMain.DoModal();
}

/* 播放3s告警，放一次，停300ms，播放10次 */
void CDrillApp::PlayAlarmSound()
{
    PlaySound((LPCTSTR)SND_ALIAS_SYSTEMEXCLAMATION, NULL, SND_ALIAS_ID | SND_ASYNC | SND_LOOP);
}

void CDrillApp::StopAlarmSound()
{
    PlaySound(NULL, 0, 0);
}

DWORD CDrillApp::GetQuality(TorqData::Torque* ptTorq)
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
// 卸扣数据不在此处判断
DWORD CDrillApp::JudgeQuality(TorqData::Torque* ptTorq)
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
    fCircle = GetCir(ptTorq, TYPE_MAKEUP);

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
BOOL CDrillApp::JudgeTranslate(TorqData::Torque* ptTorq)
{
    int     i = 0;
    int     iTranCount = 1;
    double  fCtrlTorq = 1;
    double  fTemp;
    int     iCount = 0;

    ASSERT_NULL_R(ptTorq, TRUE);
    ASSERT_ZERO_R(ptTorq->dwmucount(), TRUE);

    iTranCount = (int)ceil((0.2 / ptTorq->fmaxcir()) * MAXLINEITEM);
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

CString CDrillApp::GetTorqCollTime(TorqData::Torque* ptTorq, bool bBreakout)
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

CString CDrillApp::GetTorqFullDate(TorqData::Torque* ptTorq, bool bBreakout)
{
    __time64_t  colTime;

    ASSERT_NULL_R(ptTorq, _T(""));

    colTime = ptTorq->mucoltime();
    if (bBreakout)
        colTime = ptTorq->bocoltime();

    if (colTime == 0)
        return "";
    COleDateTime olett(colTime);

    if (LANGUAGE_CHINESE == g_tGlbCfg.nLangType)
    {
        return olett.Format(_T("%Y年%m月%d日"));
    }

    /* February 17, 2020 */
    return olett.Format(_T("%b %d, %Y"));
}

CString CDrillApp::GetTorqSimpDate(TorqData::Torque* ptTorq, bool bBreakout)
{
    __time64_t  colTime;

    ASSERT_NULL_R(ptTorq, _T(""));

    colTime = ptTorq->mucoltime();
    if (bBreakout)
        colTime = ptTorq->bocoltime();

    if (colTime == 0)
        return "";
    COleDateTime olett(colTime);
    return olett.Format(_T("%Y-%m-%d"));
}

/* 2018-03-31 23:25 */
bool CDrillApp::GetTimeFromString(CString strTime, __time64_t& time)
{
    struct tm tmTime = { 0 };

    if (strTime.IsEmpty())
        return false;

    sscanf_s(strTime, "%4d-%2d-%2d %2d:%2d:%2d",
        &tmTime.tm_year,
        &tmTime.tm_mon,
        &tmTime.tm_mday,
        &tmTime.tm_hour,
        &tmTime.tm_min,
        &tmTime.tm_sec);

    tmTime.tm_year -= 1900;
    tmTime.tm_mon--;
    tmTime.tm_isdst = -1;
    time = mktime(&tmTime);

    /*COleDateTime olett(time);
    CString temp = olett.Format(_T("%Y-%m-%d %H:%M:%S"));*/
    return true;
}

/* 拷贝指定区域到DC的位图中 */
HBITMAP CDrillApp::CopyDCToBitmap(HDC hScrDC, UINT width, UINT height)
{
    HDC         hMemDC = NULL; // 屏幕和内存设备描述表
    HBITMAP     hBitmap = NULL; // 位图句柄
    HBITMAP     hOldBitmap = NULL;

    // 确保选定区域不为空矩形
    ASSERT_ZERO_R(width, NULL);
    ASSERT_ZERO_R(height, NULL);

    // 为屏幕设备描述表创建兼容的内存设备描述表
    hMemDC = CreateCompatibleDC(hScrDC);

    // 创建一个与屏幕设备描述表兼容的位图
    hBitmap = CreateCompatibleBitmap(hScrDC, width, height);
    /*while(!hBitmap)
    {
        fFactor  -= 0.05f;
        hBitmap  = CreateCompatibleBitmap(hScrDC, (int)(iWidth*fFactor), (int)(iHeight*fFactor));
    }*/

    // 把新位图选到内存设备描述表中
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    // 把屏幕设备描述表拷贝到内存设备描述表中
    StretchBlt(hMemDC, 0, 0, width, height, hScrDC, 0, 0, width, height, SRCCOPY);

    // 得到位图的句柄
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

    // 清除
    DeleteDC(hMemDC);
    DeleteObject(hOldBitmap);

    // 返回位图句柄
    return   hBitmap;
}

int CDrillApp::CopyDCToPNGFile(HDC hScrDC, UINT nNO, string strFile, UINT width, UINT height, bool bRotate)
{
    HDC         hMemDC = NULL; // 屏幕和内存设备描述表
    HBITMAP     hBitmap = NULL; // 位图句柄
    HBITMAP     hOldBitmap = NULL;

    // 确保选定区域不为空矩形
    ASSERT_ZERO_R(width, -1);
    ASSERT_ZERO_R(height, -1);

    // 为屏幕设备描述表创建兼容的内存设备描述表
    hMemDC = CreateCompatibleDC(hScrDC);

    // 创建一个与屏幕设备描述表兼容的位图
    hBitmap = CreateCompatibleBitmap(hScrDC, width, height);

    // 把新位图选到内存设备描述表中
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    // 把屏幕设备描述表拷贝到内存设备描述表中
    StretchBlt(hMemDC, 0, 0, width, height, hScrDC, 0, 0, width, height, SRCCOPY);

    // 得到位图的句柄
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
    SavePNG(hBitmap, strFile, bRotate);
    // 清除
    DeleteDC(hMemDC);
    DeleteObject(hOldBitmap);
    DeleteObject(hBitmap);

    // 返回位图句柄
    return   0;
}

/* 根据HBITMAP获取图像数据，供后面保存文件使用，保存后需要释放HANDLE */
HANDLE CDrillApp::GetImgData(HBITMAP hBitmap, LPBITMAPINFOHEADER& lpbi, DWORD& dwBmBitsSize)
{
    WORD                wBitCount = 24;   //位图中每象素所占字节数
    DWORD               dwLineSize = 0;
    BITMAP              Bitmap = { 0 };  //位图属性结构
    BITMAPINFOHEADER    bi = { 0 };  //位图信息头结构
    HDC                 hDC = NULL;
    HANDLE              hDib = NULL;
    HANDLE              hPal = NULL;
    HANDLE              hOldPal = NULL;

    ASSERT_NULL_R(hBitmap, NULL);

    GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = Bitmap.bmWidth;
    bi.biHeight = Bitmap.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = wBitCount;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrImportant = 0;
    bi.biClrUsed = 0;

    dwLineSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4;
    dwBmBitsSize = dwLineSize * Bitmap.bmHeight;

    // 为位图内容分配内存,24位位图没有调色板
    hDib = GlobalAlloc(GHND, dwBmBitsSize + sizeof(BITMAPINFOHEADER));
    ASSERT_NULL_R(hDib, NULL);

    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    *lpbi = bi;

    // 处理调色板
    hPal = GetStockObject(DEFAULT_PALETTE);
    if (hPal)
    {
        hDC = ::GetDC(NULL);
        hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
        RealizePalette(hDC);
    }

    // 获取该调色板下新的像素值
    GetDIBits(hDC, hBitmap, 0, Bitmap.bmHeight,
        (LPSTR)lpbi + sizeof(BITMAPINFOHEADER),
        (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

    // 恢复调色板
    if (hOldPal)
    {
        ::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
        RealizePalette(hDC);
        ::ReleaseDC(NULL, hDC);
    }

    return hDib;
}

HANDLE CDrillApp::RotateImgData(HBITMAP hBitmap, LPBITMAPINFOHEADER& lpOldbi, LPBITMAPINFOHEADER& lpNewbi, DWORD& dwBmBitsSize)
{
    WORD                wBitCount = 24;   //位图中每象素所占字节数
    DWORD               dwLineSize = 0;
    DWORD               dwOldLine = 0;
    BITMAP              Bitmap = { 0 };  //位图属性结构
    BITMAPINFOHEADER    bi = { 0 };  //位图信息头结构
    HANDLE              hDib = NULL;
    LONG                wNewW, wNewH;

    GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
    wNewW = Bitmap.bmHeight;
    wNewH = Bitmap.bmWidth;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = wNewW;
    bi.biHeight = wNewH;
    bi.biPlanes = 1;
    bi.biBitCount = wBitCount;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrImportant = 0;
    bi.biClrUsed = 0;

    dwLineSize = ((wNewW * wBitCount + 31) / 32) * 4;
    dwOldLine = ((wNewH * wBitCount + 31) / 32) * 4;
    dwBmBitsSize = dwLineSize * wNewH;

    // 为位图内容分配内存,24位位图没有调色板
    hDib = GlobalAlloc(GHND, dwBmBitsSize + sizeof(BITMAPINFOHEADER));
    ASSERT_NULL_R(hDib, NULL);

    lpNewbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    *lpNewbi = bi;
    BYTE* pNewBits = ((BYTE*)lpNewbi) + sizeof(BITMAPINFOHEADER);
    BYTE* pOldBits = ((BYTE*)lpOldbi) + sizeof(BITMAPINFOHEADER);

    //旋转90度
    double             cosa = 0, sina = 1; //cos(90),sin(90)
    double			   num1, num2;
    //num1 = -0.5 * Wnew * cosa - 0.5 * Hnew * sina + 0.5 * Wold;
    //num2 = 0.5 * Wnew * sina - 0.5 * Hnew * cosa + 0.5 * Hold;
    num1 = 0;
    num2 = wNewW;

    int x0, y0, x1, y1;
    for (y1 = 0; y1 < wNewH; y1++)
        for (x1 = 0; x1 < wNewW; x1++)
        {
            x0 = (DWORD)(x1 * cosa + y1 * sina + num1);
            y0 = (DWORD)(-1.0f * x1 * sina + y1 * cosa + num2);
            if ((x0 >= 0) && (x0 < wNewH) && (y0 >= 0) && (y0 < wNewW))
            {
                for (int i = 0; i < 3; i++)
                    pNewBits[(wNewH - y1 - 1) * dwLineSize + x1 * 3 + i] =
                    pOldBits[(wNewW - 1 - y0) * dwOldLine + x0 * 3 + i];
            }
        }

    return hDib;
}

/* 保存Bmp图像 */
bool CDrillApp::SaveBmp(HBITMAP hBitmap, string FileName)
{
    // 定义 位图中像素字节大小，位图文件大小，写入文件字节数
    DWORD               dwBmBitsSize = 0;
    DWORD               dwDIBSize = 0;
    DWORD               dwWritten = 0;
    // 定义文件，分配内存句柄，调色板句柄
    HANDLE              fh = 0;
    BITMAPFILEHEADER    bmfHdr;
    LPBITMAPINFOHEADER  lpbi = NULL; //指向位图信息头结构
    HANDLE              hDib = NULL;

    hDib = GetImgData(hBitmap, lpbi, dwBmBitsSize);
    ASSERT_NULL_R(hDib, FALSE);
    ASSERT_NULL_R(lpbi, FALSE);

    //创建位图文件
    fh = CreateFile(FileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    COMP_BE_R(fh, INVALID_HANDLE_VALUE, FALSE);

    //设置位图文件头
    bmfHdr.bfType = BFT_BITMAP;   //   "BM "
    dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwBmBitsSize;
    bmfHdr.bfSize = dwDIBSize;
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

    // 写入位图文件头
    WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
    // 写入位图文件其余内容
    WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
    // 清除
    CloseHandle(fh);
    GlobalUnlock(hDib);
    GlobalFree(hDib);

    return true;
}

//returns 0 if all went ok, non-0 if error
//output image is always given in RGBA (with alpha channel), even if it's a BMP without alpha channel
unsigned CDrillApp::decodeBMP(std::vector<unsigned char>& image, unsigned& w, unsigned& h, unsigned char* bmp)
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

bool CDrillApp::SavePNG(HBITMAP hBitmap, string FileName, bool bRotate)
{
    // 定义 位图中像素字节大小，位图文件大小，写入文件字节数
    DWORD               dwBmBitsSize = 0;
    DWORD               dwDIBSize = 0;
    BITMAPFILEHEADER    bmfHdr;
    LPBITMAPINFOHEADER  lpbi = NULL; //指向位图信息头结构
    LPBITMAPINFOHEADER  lpNewbi = NULL; //指向Rotate后的位图信息头结构
    HANDLE              hDib = NULL;
    BYTE* pBuf = NULL;
    BYTE* pTmp = NULL;
    std::vector<unsigned char> image;
    std::vector<unsigned char> png;
    unsigned            w, h;

    hDib = GetImgData(hBitmap, lpbi, dwBmBitsSize);
    ASSERT_NULL_R(hDib, false);
    ASSERT_NULL_R(lpbi, false);
    if (bRotate)
    {
        HANDLE hNewDib = RotateImgData(hBitmap, lpbi, lpNewbi, dwBmBitsSize);
        GlobalUnlock(hDib);
        GlobalFree(hDib);

        ASSERT_NULL_R(hNewDib, false);
        ASSERT_NULL_R(lpNewbi, false);
        hDib = hNewDib;
        lpbi = lpNewbi;
    }

    pBuf = new BYTE[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwBmBitsSize];

    //设置位图文件头
    bmfHdr.bfType = BFT_BITMAP;   //   "BM "
    dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwBmBitsSize;
    bmfHdr.bfSize = dwDIBSize;
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

    memcpy(pBuf, &bmfHdr, sizeof(BITMAPFILEHEADER));
    pTmp = pBuf + sizeof(BITMAPFILEHEADER);
    memcpy(pTmp, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER) + dwBmBitsSize);

    decodeBMP(image, w, h, pBuf);

    lodepng::encode(png, image, lpbi->biWidth, lpbi->biHeight);
    lodepng::save_file(png, FileName);

    delete[] pBuf;
    GlobalUnlock(hDib);
    GlobalFree(hDib);
    return true;
}

void CDrillApp::ClearReadTorq(TORQUEDATA* pAllData)
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

/* 从文件当前读取位置获取数据的长度，文件位置为读取有效的文件长度之后 */
int CDrillApp::SeekFileLen(CFile& file)
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
int CDrillApp::SeekTorque(CFile& file, int iDataNum)
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

int  CDrillApp::SeekPBDataPos(CFile& file, int iCurPos)
{
    int     i = 0, j = 0, skip = 0;
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
            skip = 0;
            for (j = i + 4; j < MAXSKIPLEN; j += 4)
            {
                if (memcmp(&cTmpRead[j], &m_nPBHead, PBHEADLEN) == 0)
                {
                    skip = j - i;
                }
                else
                    break;
            }
            file.Seek(iCurPos + i + skip + PBHEADLEN, CFile::begin);
            return 0;
        }
    }

    // 没有找到数据文件头，返回失败
    return -1;
}

BOOL CDrillApp::ReadHisTorqFromFile(string strDataName)
{
    COMP_BFALSE_R(GetTorqDataFromFile(strDataName), FALSE);

    /* 没有更新入井序号，直接返回正确，否则重新读一次 */
    COMP_BFALSE_R(ReCalTallyNO(strDataName), TRUE);

    return GetTorqDataFromFile(strDataName);
}

/* 读取历史的扭矩数据文件
   一次最多读取MAXWELLNUM 5000条 */
BOOL CDrillApp::GetTorqDataFromFile(string strDataName)
{
    CFile   file;
    int     i = 0;
    int     j = 0;
    int     iCtrlCount = 0;
    UINT    nNum = 0;
    UINT    nValid = 0;
    int     iFilePos = 0;
    int     iDataLen = 0;
    //int     iTotalPnt = 0;
    CString strInfo;
    CString strTitle;
    bool    bRes;
    TorqData::Torque* ptTorq = NULL;
    //SPLITPOINT* pSplit = NULL;
    //int     iSplitPos = 0;
    char    cPBHead[PBHEADLEN + 1] = { 0 };
    DWORD   dwQuality = 0;
    double  fRatio = NM2LBFT;
    TORQUEDATA* pAllData = NULL;
    //string strData;

    COMP_BTRUE_R(strDataName.empty(), FALSE);

    ASSERT_ZERO_R(file.Open(strDataName.c_str(), CFile::modeReadWrite | CFile::shareDenyNone), FALSE);

    //m_strReadFile = strDataName;
    strTitle = file.GetFileTitle();
    strTitle.Delete(strTitle.GetLength() - 4, 4);
    m_strFileTitle = strTitle;

    if (NULL == pAllData)
        pAllData = &g_tReadData;
    ClearReadTorq(pAllData);
    pAllData->strFileName = strDataName;

    file.SeekToBegin();
    file.Read(&nNum, sizeof(UINT));
    if (nNum > MAXWELLNUM)
    {
        strInfo.Format(IDS_STRINFREADOVERFLOW, nNum, MAXWELLNUM);
        SaveShowMessage(strInfo.GetBuffer(0));
        nNum = MAXWELLNUM;
    }

    BeginWaitCursor();

    for (i = 0; i < (int)nNum; i++)
    {
        iFilePos = (int)file.GetPosition();
        strInfo.Format(IDS_STRINFTORQDATAERR, strDataName.c_str(), nNum, i, file.GetPosition());

        iDataLen = SeekFileLen(file);
        if (iDataLen < 0)
        {
            SaveShowMessage(strInfo.GetBuffer(0));
            break;
        }

        memset(m_cProtoBuf, 0, MAXPROBUFF);
        file.Read(m_cProtoBuf, iDataLen);

        if (iDataLen < MIN_TORQDATALEN)
        {
            strInfo.Format(IDS_STRINFTORQDATASHORT, strDataName.c_str(), iDataLen, file.GetPosition(), i);
            SaveMessage(strInfo.GetBuffer(0));
            continue;
        }

        bRes = pAllData->tData[nValid].ParseFromArray(m_cProtoBuf, iDataLen);
        if (!bRes)
            continue;

        /* 数据大于1屏时设置分屏信息 */
        /* 20190609最后一屏按控制周数，其他按满屏计算 */
        /* 20190916 如果数据大于控制周数，则需要分屏，最后一周在控制周数上 */
        ptTorq = &pAllData->tData[nValid];

        // 20230606 老版本单根立柱值在bsinglestd中，需要设置到columns中，以便后续程序通过columns显示和设置
        if (ptTorq->bsinglestd() && ptTorq->dwcolumns() == 0)
            ptTorq->set_dwcolumns(1);
        pAllData->nTotalPlus[nValid] = 0;
        if (HaveMakeUP(ptTorq))
            pAllData->nTotalPlus[nValid] += ptTorq->dwmuplus();
        if (HaveBreakout(ptTorq))
            pAllData->nTotalPlus[nValid] += ptTorq->dwboplus();
        if (ptTorq->fplus() > 0 && ptTorq->fmaxcir() > 0)
        {
            double maxcir = pAllData->nTotalPlus[nValid] / ptTorq->fplus();
            // 20230503 最大周数比实际周数小(没有动态更新周数或者更新失败)时，重新设置最大周数，保证一屏能显示全部数据
            if (maxcir > ptTorq->fmaxcir() * AUTOUPDTURNRATIO)
            {
                maxcir = (int)ceil(maxcir / AUTOUPDTURNRATIO);
                ptTorq->set_fmaxcir(maxcir);
            }
            //iTotalPnt = (int)ceil(pAllData->nTotalPlus[nValid] / ptTorq->fplus() / ptTorq->fmaxcir() * MAXLINEITEM);
        }

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

TorqData::Torque* CDrillApp::GetOrgTorqFromTorq(UINT nNO)
{
    TorqData::Torque* ptOrg = NULL;

    COMP_BGE_R(nNO, g_tReadData.nTotal, NULL);

    ptOrg = &g_tReadData.tData[nNO];
    ASSERT_ZERO_R(ptOrg->ftorque_size(), NULL);

    return ptOrg;
}

bool  CDrillApp::GetMakeupDrawData(TorqData::Torque* ptOrg, DRAWTORQDATA* ptDraw, UINT nMulti)
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
        /* 跳过delta脉冲为0的情况 */
        if (ptOrg->dwdelplus(iDataIndex) == 0)
            continue;
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

bool CDrillApp::GetBreakoutDrawData(TorqData::Torque* ptOrg, DRAWTORQDATA* ptDraw, UINT nMulti)
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

        // 上扣卸扣之间增加20个空数据
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
        /* 跳过delta脉冲为0的情况 */
        if (ptOrg->dwdelplus(iDataIndex) == 0)
            continue;
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

DRAWTORQDATA* CDrillApp::GetDrawDataFromTorq(UINT nNO, UINT nMulti, UINT nType)
{
    COMP_BGE_R(nNO, g_tReadData.nTotal, NULL);
    COMP_BGE_R(nNO, MAXWELLNUM, NULL);

    TorqData::Torque* ptOrg = &g_tReadData.tData[nNO];
    ASSERT_ZERO_R(ptOrg->ftorque_size(), NULL);

    return GetDrawDataFromTorq(ptOrg, nMulti, nType);
}

/*  nNO 从0开始计数
    iMulti在放大时使用 */
DRAWTORQDATA* CDrillApp::GetDrawDataFromTorq(TorqData::Torque* ptOrg, UINT nMulti, UINT nType)
{
    DRAWTORQDATA* ptDraw = NULL;

    ASSERT_NULL_R(ptOrg, NULL);
    ASSERT_ZERO_R(ptOrg->ftorque_size(), NULL);

    ptDraw = &m_tCurDrawTorq;
    if (nMulti > 1)
        ptDraw = &m_tCurZoomTorq;

    memset(ptDraw, 0, sizeof(DRAWTORQDATA));
    ptDraw->ptOrgTorq = ptOrg;
    ptDraw->fMaxCir = ptOrg->fmaxcir();

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

CString CDrillApp::GetStatType(TorqData::Torque* ptTorq, WORD wPlace)
{
    ASSERT_NULL_R(ptTorq, NULLSTR);

    COMP_BGE_R(wPlace, ptTorq->tshow_size(), NULLSTR);

    return GetTorqShowValue(ptTorq, wPlace);
}

BOOL CDrillApp::CheckPassWord()
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

string CDrillApp::LoadstringFromRes(unsigned string_ID)
{
    char buffer[MAX_LOADSTRING];

    unsigned bytes_copied = LoadString(m_hLangDLL[g_tGlbCfg.nLangType], string_ID, buffer, MAX_LOADSTRING);
    if (!bytes_copied)
        throw std::runtime_error("Resource not found!");

    return string(buffer, bytes_copied);
}

//string CDrillApp::LoadstringFromRes(unsigned string_ID, int val)
//{
//    char buffer[MAX_LOADSTRING];
//    LoadString(m_hLangDLL[g_tGlbCfg.nLangType], string_ID, buffer, MAX_LOADSTRING);
//    snprintf(buffer, MAX_LOADSTRING, LoadstringFromRes(string_ID).c_str(), val);
//
//    return string(buffer);
//}
//
//string CDrillApp::LoadstringFromRes(unsigned string_ID, double val)
//{
//    char buffer[MAX_LOADSTRING];
//    LoadString(m_hLangDLL[g_tGlbCfg.nLangType], string_ID, buffer, MAX_LOADSTRING);
//    snprintf(buffer, MAX_LOADSTRING, LoadstringFromRes(string_ID).c_str(), val);
//
//    return string(buffer);
//}
//
//string CDrillApp::LoadstringFromRes(unsigned string_ID, string val)
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
    nDataPlace: form 1 开始
    -1表示最后一个数据
*/
void CDrillApp::UpdateHisData(string strName, int iDataPlace, TorqData::Torque* ptTorq)
{
    int     i = 0;
    UINT    nCurPos = 0;        /* 当前数据位置 */
    UINT    nNextPos = 0;       /* 下一个数据的位置 */
    UINT    nLastPos = 0;
    int     iLeft = 0;          /* 当前数据后的数据大小 */
    size_t  iCurLen = 0;        /* 当前数据的总长度 */
    UINT    iDataLen = 0;       /* 数据的总长度 */
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

    file.Write(&theApp.m_nPBHead, PBHEADLEN);
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

double CDrillApp::GetCir(TorqData::Torque* ptTorq, UINT nType)
{
    double fCir = 0;

    ASSERT_NULL_R(ptTorq, 0);

    if (nType & TYPE_MAKEUP)
        fCir += THOUSANDTH(ptTorq->dwmuplus() / ptTorq->fplus());
    if (nType & TYPE_BREAKOUT)
        fCir += THOUSANDTH(ptTorq->dwboplus() / ptTorq->fplus());

    return fCir;
}

double CDrillApp::GetMaxCir(TorqData::Torque* ptTorq)
{
    double fMaxCir = 1;

    ASSERT_NULL_R(ptTorq, fMaxCir);

    fMaxCir = ptTorq->fmaxcir();

    return fMaxCir;
}

double CDrillApp::GetCtrlCir(TorqData::Torque* ptTorq)
{
    double fCtrlCir = 0.8;

    ASSERT_NULL_R(ptTorq, fCtrlCir);

    fCtrlCir = ptTorq->fcontrolcir();

    return fCtrlCir;
}

double CDrillApp::GetUpperCir(TorqData::Torque* ptTorq)
{
    double fUpperCir = 0.9;

    ASSERT_NULL_R(ptTorq, fUpperCir);

    fUpperCir = ptTorq->fuppercir();

    return fUpperCir;
}

double CDrillApp::GetLowerCir(TorqData::Torque* ptTorq)
{
    double fLowerCir = 0.1;

    ASSERT_NULL_R(ptTorq, fLowerCir);

    fLowerCir = ptTorq->flowercir();

    return fLowerCir;
}

double CDrillApp::GetOptTorq(TorqData::Torque* ptTorq)
{
    double fOptTorq = 2000;

    ASSERT_NULL_R(ptTorq, fOptTorq);

    fOptTorq = ptTorq->fopttorq();
    if (0 == fOptTorq)
        fOptTorq = ptTorq->fcontrol();

    return fOptTorq;
}

string CDrillApp::GetSaveDataPath()
{
    string         strSavePath;

    strSavePath = m_strDataPath;
    strSavePath += m_strFileTitle;
    strSavePath += "\\";
    CreateDirectory(strSavePath.c_str(), NULL);

    return strSavePath;
}

int CDrillApp::SplitString(CString strSource, CStringList& slList)
{
    int     index = 0;
    CString strTemp;

    if (strSource.IsEmpty())
        return -1;

    while (TRUE)
    {
        index = strSource.Find(_T(" "));
        if (index == -1)
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
        strSource = strSource.Right(strSource.GetLength() - index - 1);
    }

    return -2;
}

bool CDrillApp::HaveMakeUP(TorqData::Torque* ptTorq)
{
    ASSERT_NULL_R(ptTorq, false);
    if (ptTorq->dwmucount() > 0)
        return true;
    return false;
}

bool CDrillApp::HaveBreakout(TorqData::Torque* ptTorq)
{
    ASSERT_NULL_R(ptTorq, false);
    if (ptTorq->dwbocount() > 0)
        return true;
    return false;
}

bool CDrillApp::HaveTallyNO(TorqData::Torque* ptTorq)
{
    ASSERT_NULL_R(ptTorq, false);

    if (ptTorq->dwmucount() > 0 && QUA_RESU_GOOD == ptTorq->dwquality())
        return true;
    return false;
}

/* 返回是否有修改
   20200302 每次都从最开始重新计算 */
BOOL CDrillApp::ReCalTallyNO(string strDataName)
{
    int     i = 0;
    int     iTallyIndex = -1;
    UINT    nRunningNO = 1;
    BOOL    bModified = FALSE;
    CString strRunningNO;
    CString strOldNO;
    TorqData::Torque* ptTorq = NULL;
    TorqData::ShowInfo* ptRunningShow = NULL;
    TORQUEDATA* pAllData = NULL;

    if (NULL == pAllData)
        pAllData = &g_tReadData;
    iTallyIndex = GetMainIndexfromData(MAINSHOWTALLY, &pAllData->tData[0]);
    COMP_BL_R(iTallyIndex, 0, FALSE);

    BeginWaitCursor();
    /* 会从当前序号开始(开始数值为1), 顺序更新后续数据的入井序号 */
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

    SaveAllData(strDataName);

    return TRUE;
}

void CDrillApp::SaveAllData(string strDataName)
{
    UINT    i = 0;
    size_t  nDataLen = 0;
    CFile   file;
    TORQUEDATA* pAllData = NULL;

    if (NULL == pAllData)
        pAllData = &g_tReadData;

    /* write to file */
    if (file.Open(strDataName.c_str(), CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone, NULL))
    {
        /*更新记录数*/
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

CString CDrillApp::GetTorqShowName(TorqData::Torque* ptTorq, int iIndex)
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
    return ptTorq->tshow(iIndex).strname().c_str();
}

CString CDrillApp::GetTorqShowValue(TorqData::Torque* ptTorq, int iIndex)
{
    ASSERT_NULL_R(ptTorq, NULLSTR);
    COMP_BL_R(iIndex, 0, NULLSTR);
    COMP_BGE_R(iIndex, MAXPARANUM, NULLSTR);

    if (iIndex >= ptTorq->tshow_size())
        return NULLSTR;

    return ptTorq->tshow(iIndex).strvalue().c_str();
}

//string CDrillApp::GetListShowName(SHOWCFG* ptShow, UINT NO)
//{
//    ASSERT_NULL_R(ptShow, NULLSTR);
//    COMP_BGE_R(NO, ptShow->nListNum, NULLSTR);
//
//    return ptShow->strShow[ptShow->nList[NO]];
//
//}
string CDrillApp::GetMainShowName(SHOWCFG* ptShow, UINT NO)
{
    ASSERT_NULL_R(ptShow, NULLSTR);
    COMP_BGE_R(NO, ptShow->nMainNum, NULLSTR);

    return ptShow->strShow[ptShow->nMain[NO]];
}

/* \/:*?"<>| 加 . */
BOOL CDrillApp::FindNotFileChar(CString strFileName)
{
    char    cNCFileName[10] = { '\\', '/', ':', '*', '?', '"', '<', '>', '|', '.' };
    int     i = 0;

    for (i = 0; i < 10; i++)
    {
        if (strFileName.Find(cNCFileName[i]) >= 0)
            return TRUE;
    }

    return FALSE;
}

string CDrillApp::GetFileNameFromPath(string path)
{
    string filename = NULLSTR;
    int iPos;
    if (path.size() < 5)
        return filename;
    iPos = path.find_last_of('\\') + 1;
    filename = path.substr(iPos, path.length() - iPos);
    return filename;
}

int CDrillApp::ReportWITSByTCP(string strData)
{
    string strInfo;

    COMP_BFALSE_R(isTCPConnected(), -1);
    //ASSERT_ZERO_R(mi_Socket.GetSocketCount(), -1);
    ASSERT_ZERO_R(strData.size(), -2);

    mi_Socket.GetAllConnectedSockets(&mi_SocketList);
    SOCKET h_Socket = mi_SocketList.GetKeyByIndex(0);
    DWORD u32_Err = mi_Socket.SendTo(h_Socket, (char*)strData.c_str(), strData.size());

    SaveTCPData(strData, false);

    ASSERT_ZERO_R(u32_Err, 0);
    switch (u32_Err)
    {
    case WSAEWOULDBLOCK:
        SaveTCPData(_T("WSAEWOULDBLOCK -> The data will be send after the next FD_WRITE event."));
        return 0;

    case WSA_IO_PENDING:
        SaveTCPData(_T("WSA_IO_PENDING -> Error: A previous Send operation is still pending. This data will not be sent."));
        return 0;

    default:
        strInfo = string_format("-> Error %s", mi_Socket.GetErrMsg(u32_Err));
        SaveTCPData(strInfo);
        // Severe error -> abort event loop
        CloseSockets();
        return u32_Err;
    };

    return 0;
}

bool CDrillApp::isTCPConnected()
{
    return mi_Socket.GetSocketCount() > 1;
}

bool CDrillApp::isTCPServer()
{
    return mi_Socket.GetState() & TCP::cSocket::E_Server;
}

void CDrillApp::CloseSockets()
{
    int count = mi_Socket.GetSocketCount() - 1;
    if(count > 0)
    {
        mi_Socket.GetAllConnectedSockets(&mi_SocketList);
        for (int i = 0; i < count; i++)
        {
            mi_Socket.DisconnectClient(mi_SocketList.GetKeyByIndex(i));
        }
    }
    
    mi_Socket.Close();
}