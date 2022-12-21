// TorqueDlg.cpp : implementation file
//
#include "stdafx.h"
#include "Torque.h"
#include "TorqueDlg.h"
#include "DlgParaSet.h"
#include "DlgFuncTest.h"
#include "DlgZero.h"
#include "DlgScm.h"
#include "DlgCollect.h"
#include "DlgHisList.h"
#include "DlgHisGrp.h"
#include "DlgHisSumup.h"
#include "DlgDataStat.h"
#include "DlgShowSet.h"
#include "DlgModPW.h"
#include "DlgValveSet.h"
#include "DlgMainShowSet.h"
#include "DlgRemark.h"
#include "DlgSegCabl.h"
#include "DlgGlbCfg.h"
#include <Nb30.h>
#include <ctime>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BYTE                g_ucCurValve = 0;

CDlgCollect         *m_pdlgCollect  = NULL;
CDlgSegCabl         *m_pdlgCalib    = NULL;

//#define             TESTPORT

/*********************全局变量************************************/

/* 填充命令函数数组，序号减15再处理
   最长填8个BYTE */
void (CTorqueDlg::*g_FillByteProc[MAXCMDNUM])(void) =
{
    /*  0 */    &CTorqueDlg::FillRead,           /* 读取数据 */
    /*  1 */    &CTorqueDlg::FillMulti,          /* 校准倍数 */
    /*  2 */    &CTorqueDlg::FillQipi,           /* 去皮 */
    /*  3 */    &CTorqueDlg::FillCollect,        /* 采集数据 */
    /*  4 */    &CTorqueDlg::FillShow,           /* 显示扭矩 */
    /*  5 */    &CTorqueDlg::FillSpeed,          /* 减速扭矩 */
    /*  6 */    &CTorqueDlg::FillControl,        /* 控制扭矩 */
    /*  7 */    &CTorqueDlg::FillBSpeed,         /* 开始减速 */
    /*  8 */    &CTorqueDlg::FillESpeed,         /* 停止减速 */
    /*  9 */    &CTorqueDlg::FillBUnload,        /* 开始卸荷 */
    /* 10 */    &CTorqueDlg::FillEUnload,        /* 停止卸荷 */
    /* 11 */    &CTorqueDlg::FillUpper,          /* 上限扭矩 */
    /* 12 */    &CTorqueDlg::FillLower,          /* 下限扭矩 */
    /* 13 */    &CTorqueDlg::FillCut,            /* 打折系数 */
    /* 14 */    &CTorqueDlg::FillCollectOK,      /* 收到有效数据再次采集数据：最后一次确认后不需要再次收到数据 */
    /* 15 */    &CTorqueDlg::FillCollectNOK,     /* 收到无效数据再次采集数据 */
    /* 16 */    &CTorqueDlg::FillTime,           /* 设置单片机时间 */
    /* 17 */    &CTorqueDlg::FillValvePress,     /* 设置减压阀参数 */
    /* 18 */    &CTorqueDlg::FillValveFlow,      /* 设置流量阀参数 */
    /* 19 */    &CTorqueDlg::FillReadValve,      /* 读取阀门状态参数 */
    /* 20 */    &CTorqueDlg::FillReadMulti,      /* 读取多组数据 */
    /* 21 */    &CTorqueDlg::FillWriteCalib,     /* 写 分段校准 */
    /* 22 */    &CTorqueDlg::FillCtrlCalib,      /* 控制分段校准 */
    /* 23 */    &CTorqueDlg::FillReadCalib,      /* 读 分段校准 */
    /* 19       &CTorqueDlg::FillValveZoom         设置阀放大倍数参数 */
};

/* 收集数据函数数组 */
BOOL (CTorqueDlg::*g_CollectProc[4])(COLLECTDATA* ptCollData) =
{
        /*  0 */    &CTorqueDlg::CollectPort,             /* 收集串口数据 */
        /*  1 */    &CTorqueDlg::CollectRandData,         /* 随机测试数据 */
        /*  2 */    &CTorqueDlg::CollectTorqData,         /* 现场自动记录扭矩数据 */
        /*  3 */    &CTorqueDlg::CollectMultiTorq         /* 现场自动记录多组扭矩数据 */
};

/*********************代码宏************************************/
#define INSERT_DATA(Torque,Rpm,bLast)   {               \
        InsertData(&m_tCollData, Torque, Rpm);          \
        m_wndTorque.SetPos(Torque,bLast);               \
        m_wndTorque.Go();                               \
        m_wndRpm.SetPos(Rpm,bLast);                     \
        m_wndRpm.Go();                                  \
    }

//#define JUDGE_RUN_STATUS(wInfo)         {               \
//        if(m_bRunStatus)                                \
//        {                                               \            
//            AfxMessageBox(wInfo);                       \
//            return;                                     \
//        }                                               \
//    }

/* 批量显示或者隐藏按钮 */
#define SHOW_MAINDLG_BUTTON(bShow)      {               \
        GetDlgItem(IDC_EDITCRC)->ShowWindow(bShow);     \
        GetDlgItem(IDC_STATICCRC)->ShowWindow(bShow);   \
        GetDlgItem(IDC_DATASHOW)->ShowWindow(bShow);    \
        GetDlgItem(IDC_EDITCLASH)->ShowWindow(bShow);   \
        GetDlgItem(IDC_STATICCLASH)->ShowWindow(bShow); \
        GetDlgItem(IDC_EDITTOTAL)->ShowWindow(bShow);   \
        GetDlgItem(IDC_STATICTOTAL)->ShowWindow(bShow); \
    }
#if 0
#define CLOSE_PORTCOMM()                {               \
        if(g_lpNewComThread != NULL)                    \
        {                                               \
            g_lpNewComThread->CloseComm();              \
            delete g_lpNewComThread;                    \
            g_lpNewComThread = NULL;                    \
            theApp.SavePortOper(PORTOPR_CLOSE);         \
        }                                               \
    }
#else
#define CLOSE_PORTCOMM()                {               \
            m_tPort.ClosePort();                        \
            theApp.SavePortOper(PORTOPR_CLOSE);         \
    }
#endif

/* 删除对话框指针 */
#define DELETE_DIALOG(pdlg)         {           \
        if((pdlg) != NULL)                      \
        {                                       \
            (pdlg)->DestroyWindow();            \
            delete (pdlg);                      \
            (pdlg) = NULL;                      \
        }                                       \
    }


static UINT BASED_CODE indicators[] =
{
    /*ID_SEPARATOR,         // status line indicator*/
    ID_PORTINFO,            //串口信息显示
};

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
    typedef struct
    {
        ADAPTER_STATUS  adapt;
        NAME_BUFFER     NameBuff [30];
    } ASTAT;

public:
    CAboutDlg();

    CTorqueDlg* m_ptParent;
// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    CButtonST   m_btnReg;
    CButtonST   m_btnOK;
    CString     m_strReg[REGCODESEGNUM];
    CString     m_strName;
    CString     m_strMachine;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnRegedit();
    afx_msg void OnChangeReg1();
    afx_msg void OnChangeReg2();
    afx_msg void OnChangeReg3();
    afx_msg void OnChangeReg4();
    afx_msg void OnChangeReg5();
    afx_msg void OnChangeReg6();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    int     SplitRegCode(CString strSource, CString strDest[], int iBegin);
    void    AutoFillRegCode(UINT nEditNo);
    void    SaveRegFile();
    
    DBREG   *m_ptdbReg;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    int     i = 0;

    //{{AFX_DATA_INIT(CAboutDlg)
    m_strName    = _T("");
    m_strMachine = _T("");
    //}}AFX_DATA_INIT

    for(i=0; i<REGCODESEGNUM; i++)
        m_strReg[i] = _T("");
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    DDX_Control(pDX, IDC_REGEDIT, m_btnReg);
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Text(pDX, IDC_REG1, m_strReg[0]);
    DDX_Text(pDX, IDC_REG2, m_strReg[1]);
    DDX_Text(pDX, IDC_REG3, m_strReg[2]);
    DDX_Text(pDX, IDC_REG4, m_strReg[3]);
    DDX_Text(pDX, IDC_REG5, m_strReg[4]);
    DDX_Text(pDX, IDC_REG6, m_strReg[5]);
    DDX_Text(pDX, IDC_REGNAME, m_strName);
    DDX_Text(pDX, IDC_MACHINE, m_strMachine);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    ON_BN_CLICKED(IDC_REGEDIT, OnRegedit)
    ON_EN_CHANGE(IDC_REG1, OnChangeReg1)
    ON_EN_CHANGE(IDC_REG2, OnChangeReg2)
    ON_EN_CHANGE(IDC_REG3, OnChangeReg3)
    ON_EN_CHANGE(IDC_REG4, OnChangeReg4)
    ON_EN_CHANGE(IDC_REG5, OnChangeReg5)
    ON_EN_CHANGE(IDC_REG6, OnChangeReg6)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
    DWORD   adwPCVol[2];
    DWORD   adwPCMac[2];
    DWORD   adwRegVol[2];
    DWORD   adwRegMac[2];
    CString strRegCode;
    CTime   tNowDate  = CTime::GetCurrentTime();
    int     iYear,iMonth, iDay;

    CDialog::OnInitDialog();

    m_btnOK.SetIconAndText(IDI_APPLY, IDS_STROK);
    m_btnReg.SetIconAndText(IDI_LOCKUP, IDS_STRREGISTER);

    m_ptdbReg  = &theApp.m_tdbReg;
    m_strName  = m_ptdbReg->strName;
    strRegCode = m_ptdbReg->strRegCode;

    if((!m_ptdbReg->bReged) || (strRegCode.GetLength() < MAXREGCODE))
    {
        iYear  = tNowDate.GetYear();
        iMonth = tNowDate.GetMonth();
        iDay   = tNowDate.GetDay();
    }
    else
    {
        /* Reged */
        theApp.StringSubtract(m_strName,  REGCODEVALUE);
        theApp.StringSubtract(strRegCode, REGCODEVALUE);

        theApp.SplitRegString(m_strReg, strRegCode);

        m_btnReg.ShowWindow(FALSE);
        
        m_ptParent->GetVolMacFromRegStr(m_strReg, adwRegVol, adwRegMac, iYear, iMonth, iDay);
    }

    /* 获取机器上的卷和MAC信息 */
    m_ptParent->GetVolMacInfo(adwPCVol, adwPCMac, iYear, iMonth, iDay);

    adwPCMac[0] ^= MAC0XOR;
    adwPCMac[1] ^= MAC1XOR;
    adwPCMac[0] &= MAC0AND;
    adwPCMac[1] &= MAC1AND;
    adwPCVol[0] ^= VOL0XOR;
    adwPCVol[1] ^= VOL1XOR;

    m_strMachine.Format("%08X%04X%08X%06X", adwPCVol[0], adwPCMac[1], adwPCVol[1], adwPCMac[0]);

    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::SaveRegFile()
{
    int             i   = 0;
    HANDLE          hDir;
    CFile           newfile;
    CString         strRegCode;

    hDir=CreateFile (theApp.m_strRegFile.c_str(),//folder
                     GENERIC_READ|GENERIC_WRITE,//readwrite
                     0,//share
                     NULL,//security attribute
                     CREATE_ALWAYS,//创建
                     FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_HIDDEN,//dwFlagsAndAttributes,
                     NULL);//temphandle
    if(hDir==INVALID_HANDLE_VALUE)
    {
        AfxMessageBox(theApp.LoadstringFromRes(IDS_STRINFSAVEREGFAIL).c_str());
        CloseHandle(hDir);
        return;
    }
    CloseHandle(hDir);

    m_ptdbReg->bReged = 1;
    memset(m_ptdbReg->strName, 0 ,MAXNAME);
    memset(m_ptdbReg->strRegCode, 0 ,MAXREGCODE);
    strRegCode.Empty();
    memcpy(m_ptdbReg->strName, m_strName, m_strName.GetLength());
    for(i=0; i<m_strName.GetLength(); i++)
    {
        m_ptdbReg->strName[i] += REGCODEVALUE;
    }

    theApp.MergeRegString(m_strReg, strRegCode);
    memcpy(m_ptdbReg->strRegCode, strRegCode, strRegCode.GetLength());
    for(i=0; i<strRegCode.GetLength(); i++)
    {
        m_ptdbReg->strRegCode[i] += REGCODEVALUE;
    }

    newfile.Open(theApp.m_strRegFile.c_str(),CFile::modeWrite,NULL);
    newfile.Write(m_ptdbReg,sizeof(DBREG));
    newfile.Write(&m_ptParent->m_dwTotalTorqNum,sizeof(DWORD));
    newfile.Close();
}

void CAboutDlg::OnRegedit()
{
    int     i = 0;
    DWORD   adwRegVol[2];
    DWORD   adwRegMac[2];
    DWORD   adwPCVol[2];
    DWORD   adwPCMac[2];
    UINT    nRegLen[REGCODESEGNUM];
    int     iYear,iMonth, iDay;

    UpdateData();

    for(i=0; i<REGCODESEGNUM; i++)
    {
        nRegLen[i] = m_strReg[i].GetLength();
    }

    if(memcmp(nRegLen, g_nValidLen, REGCODESEGNUM*sizeof(UINT)) !=0)
    {
        AfxMessageBox(IDS_STRINFREGCODEERR);
        return;
    }

    /* 获取注册码中的卷和MAC信息 */
    m_ptParent->GetVolMacFromRegStr(m_strReg, adwRegVol, adwRegMac, iYear, iMonth, iDay);
    
    /* 获取机器上的卷和MAC信息 */
    m_ptParent->GetVolMacInfo(adwPCVol, adwPCMac, iYear, iMonth, iDay);

    if( memcmp(adwRegVol, adwPCVol, 2*sizeof(DWORD)) == 0 &&
        memcmp(adwRegMac, adwPCMac, 2*sizeof(DWORD)) == 0)
    {
        SaveRegFile();

        theApp.ShowMainTitle();
        AfxMessageBox(theApp.LoadstringFromRes(IDS_STRINFREGSUCC).c_str());
        OnOK();
        return;
    }

    /* 注册失败 */
    theApp.ShowMainTitle();
    AfxMessageBox(theApp.LoadstringFromRes(IDS_STRINFREGFAIL).c_str());
}

/* 输入一串有'-' 分割的注册号码，自动从当前输入框将后续输入框的字符串检出，并设置到输入框中
   nEditNo从0开始编号，对应IDC_REG1... */
void CAboutDlg::AutoFillRegCode(UINT nEditNo)
{
    int     iPlace  = 0;
    int     i       = 0;
    CString strCode[REGCODESEGNUM];

    COMP_BGE(nEditNo, REGCODESEGNUM);

    UpdateData(TRUE);

    iPlace = SplitRegCode(m_strReg[nEditNo], strCode, nEditNo);
    COMP_BL(iPlace, 0);

    for(i=iPlace; i>= (int)nEditNo; i--)
    {
        m_strReg[i] = strCode[i];
    }

    UpdateData(FALSE);
}

int CAboutDlg::SplitRegCode(CString strSource, CString strDest[], int iBegin)
{
    CString strTemp;
    int     iPlace  = 0;
    int     iIndex  = 0;

    COMP_BGE_R(iBegin, REGCODESEGNUM, -1);

    iIndex = iBegin;

    strTemp = strSource;
    iPlace = strTemp.Find('-');
    while(iPlace != -1)
    {
        strDest[iIndex] = strTemp.Left(iPlace);
        strTemp.Delete(0,iPlace+1);
        iPlace = strTemp.Find('-');
        iIndex++;
        if(iIndex > REGCODESEGNUM)
            break;
    }
    if(!strTemp.IsEmpty())
        strDest[iIndex] = strTemp;

    return iIndex;
}

void CAboutDlg::OnChangeReg1()
{
    AutoFillRegCode(0);
}

void CAboutDlg::OnChangeReg2()
{
    AutoFillRegCode(1);
}

void CAboutDlg::OnChangeReg3()
{
    AutoFillRegCode(2);
}

void CAboutDlg::OnChangeReg4()
{
    AutoFillRegCode(3);
}

void CAboutDlg::OnChangeReg5()
{
    AutoFillRegCode(4);
}

void CAboutDlg::OnChangeReg6()
{
    UpdateData(TRUE);

    m_strReg[5] = m_strReg[5].Left(REGCODELEN6);

    UpdateData(FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// CTorqueDlg dialog

CTorqueDlg::CTorqueDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CTorqueDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CTorqueDlg)
    m_strTorque     = _T("0");
    m_fRpm          = 0.0f;
    m_nCur          = 1;
    m_nTotal        = 1;
    m_nCRCERR       = 0;
    m_nClashERR     = 0;
    m_nInterval     = 20;//时间需要修改20191208
    m_iBreakout      = 0;
    m_strRecvData   = _T("");
    m_strQuality    = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_bComm         = FALSE;
    m_bAutoFileOpen = FALSE;

    m_pdlgCollect   = NULL;
    m_pdlgCalib     = NULL;
    m_ptPortData    = NULL;

    /* 未存储入Data的值，初始为0 */
    UpdateOutData(0, 0); 
}

void CTorqueDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTorqueDlg)
    DDX_Control(pDX, IDC_CHECKCOMM, m_cbComm);
    DDX_Control(pDX, IDC_CHECKALARM, m_cbAlarm);
    DDX_Control(pDX, IDC_CHECKSPEED, m_cbValveStatus[0]);
    DDX_Control(pDX, IDC_CHECKUNLOAD, m_cbValveStatus[1]);
    DDX_Control(pDX, IDC_CHECKPRESS, m_cbValveStatus[2]);
    DDX_Control(pDX, IDC_CHECKFLOW, m_cbValveStatus[3]);
    DDX_Control(pDX, IDC_CHECKMAKEUP, m_cbValveStatus[4]);
    DDX_Control(pDX, IDC_CHECKBREAKOUT, m_cbValveStatus[5]);
    DDX_Control(pDX, IDC_COMMSTATUS, m_CommStatus);
    DDX_Control(pDX, IDC_BTRUN, m_btnRun);
    DDX_Control(pDX, IDC_XAXIS1, m_xAxis1);
    DDX_Control(pDX, IDC_YAXIS1, m_yAxis1);
    DDX_Control(pDX, IDC_XAXIS2, m_xAxis2);
    DDX_Control(pDX, IDC_YAXIS2, m_yAxis2);
    DDX_Text(pDX, IDC_EDITTORQUE, m_strTorque);
    DDX_Check(pDX, IDC_CHECKCOMM, m_bComm);
    DDX_Text(pDX, IDC_EDITCURNUM, m_nCur);
    DDX_Text(pDX, IDC_EDITCRC, m_nCRCERR);
    DDX_Text(pDX, IDC_DATASHOW, m_strRecvData);
    DDX_Text(pDX, IDC_EDITCLASH, m_nClashERR);
    DDX_Text(pDX, IDC_EDITTOTAL, m_nTotal);
    DDX_Check(pDX, IDC_SETTOOLBUCK, m_bToolBuck);
    DDX_Text(pDX, IDC_QUALITY, m_strQuality);
    DDX_Radio(pDX, IDC_RADIOMAKEUP, m_iBreakout);
    DDX_Text(pDX, IDC_MAINSHOW1, m_strMainName[1]);
    DDX_Text(pDX, IDC_MAINSHOW2, m_strMainName[2]);
    DDX_Text(pDX, IDC_MAINSHOW3, m_strMainName[3]);
    DDX_Text(pDX, IDC_MAINSHOW4, m_strMainName[4]);
    DDX_Text(pDX, IDC_MAINSHOW5, m_strMainName[5]);
    DDX_Text(pDX, IDC_MAINSHOW6, m_strMainName[6]);
    DDX_Text(pDX, IDC_MAINSHOW7, m_strMainName[7]);
    DDX_Text(pDX, IDC_EDMAINSHOW1, m_strMainValue[1]);
    DDX_Text(pDX, IDC_EDMAINSHOW2, m_strMainValue[2]);
    DDX_Text(pDX, IDC_EDMAINSHOW3, m_strMainValue[3]);
    DDX_Text(pDX, IDC_EDMAINSHOW4, m_strMainValue[4]);
    DDX_Text(pDX, IDC_EDMAINSHOW5, m_strMainValue[5]);
    DDX_Text(pDX, IDC_EDMAINSHOW6, m_strMainValue[6]);
    DDX_Text(pDX, IDC_EDMAINSHOW7, m_strMainValue[7]);
    DDX_Text(pDX, IDC_STATIC_M2, m_strLBM2);
    DDX_Text(pDX, IDC_STATIC_M9, m_strTorqType);
    DDX_Text(pDX, IDC_EDBREAKOUTFILE, m_strBreakoutFile);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTorqueDlg, CDialog)
    //{{AFX_MSG_MAP(CTorqueDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_COMMAND(ID_SETPARA, OnSetpara)
    ON_COMMAND(ID_SETSHOW, OnSetShow)
    ON_COMMAND(ID_ABOUT, OnAbout)
    ON_COMMAND(ID_DEBUG, OnDebug)
    ON_BN_CLICKED(ID_APP_EXIT, OnAppExit)
    ON_COMMAND(ID_ZERO, OnZero)
    ON_COMMAND(ID_SCM, OnScm)
    ON_COMMAND(ID_HISTORYLIST, OnHistorylist)
    ON_COMMAND(ID_HISTORYSTAT, OnHistorystat)
    ON_COMMAND(ID_SHOWTEST, OnShowtest)
    ON_COMMAND(ID_COLLECTDATA, OnCollectdata)
    ON_BN_CLICKED(IDC_BTRUN, OnBtrun)
    ON_MESSAGE(WM_COLLECTTIMEROUT,CollectTimerOut)
    ON_MESSAGE(WM_GUARDTIMEROUT,GuardTimerOut)
    ON_MESSAGE(WM_PORTBUFFTIMEROUT,PortBuffTimerOut)
    ON_MESSAGE(WM_SAVEDATATIMEROUT,SaveDataTimerOut)
    ON_MESSAGE(WM_SAVEDEBUGTIMEROUT,SaveDebugTimerOut)
    ON_MESSAGE(WM_ALARMPLAYTIMEROUT,ALarmPlayTimerOut)
    ON_MESSAGE(WM_READVALVETIMEROUT,ReadValveTimerOut)
    ON_MESSAGE(WM_COMM_RXCHAR, OnCommunication)
    ON_COMMAND(ID_LANG_CHN, OnLangChn)
    ON_UPDATE_COMMAND_UI(ID_LANG_CHN, OnUpdateLangChn)
    ON_COMMAND(ID_LANG_ENG, OnLangEng)
    ON_UPDATE_COMMAND_UI(ID_LANG_ENG, OnUpdateLangEng)
    ON_COMMAND(ID_VER_BIGTORQ, OnVerBigTorq)
    ON_UPDATE_COMMAND_UI(ID_VER_BIGTORQ, OnUpdateVerBigTorq)
    ON_COMMAND(ID_VER_UNIT_NM, OnVerUnitNm)
    ON_UPDATE_COMMAND_UI(ID_VER_UNIT_NM, OnUpdateVerUnitNm)
    ON_COMMAND(ID_VER_UNIT_LBFT, OnVerUnitLbft)
    ON_UPDATE_COMMAND_UI(ID_VER_UNIT_LBFT, OnUpdateVerUnitLbft)
    //}}AFX_MSG_MAP
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_INITMENUPOPUP()
    ON_COMMAND(ID_MODPW, &CTorqueDlg::OnModpw)
    ON_COMMAND(ID_VALVESET, &CTorqueDlg::OnValveset)
    ON_BN_CLICKED(IDC_SETTOOLBUCK, &CTorqueDlg::OnBnClickedSettoolbuck)
    ON_BN_CLICKED(IDC_RADIOMAKEUP, &CTorqueDlg::OnBnClickedRadiomakeup)
    ON_BN_CLICKED(IDC_RADIOBREAKOUT, &CTorqueDlg::OnBnClickedRadiobreakout)
    ON_BN_CLICKED(IDC_BTNQUALITY, &CTorqueDlg::OnBnClickedBtnquality)
    ON_BN_CLICKED(IDC_BTNSHOWSET, &CTorqueDlg::OnBnClickedBtnshowset)
    ON_BN_CLICKED(IDC_BTNBREAKOUTFILE, &CTorqueDlg::OnBnClickedBtnBreakoutFile)
    ON_COMMAND(ID_SEGCALIB, &CTorqueDlg::OnSegcalib)
    ON_COMMAND(ID_GLBCFG, &CTorqueDlg::OnGlbCfg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTorqueDlg message handlers

void CTorqueDlg::InitVariant()
{
    /* 初始化theApp全局变量的指针，方便引用 */
    m_ptShow = theApp.m_ptCurShow;
    m_ptCfg     = &theApp.m_tParaCfg;
    m_ptCtrl    = &theApp.m_tParaCfg.tCtrl;
    m_ptComm    = &theApp.m_tParaCfg.tComm;
    
    /* 初始化变量 */
    m_nCur      = m_nCurNO + 1;
    m_nTotal    = m_dwTotalTorqNum;
    m_iTest     = 0;
    m_iRecPNum  = 0;
    m_bRunStatus= FALSE;
    m_bAlarm    = FALSE;
    m_bCanModLastData   = FALSE;
    m_strMainValue[MAINSHOWTALLY].Format("%d", m_nCurRunningNO);
    m_iMaxReadTimes = (int)ceil(MAXCOMMBREAKTIME / g_tGlbCfg.nCollectDur) + 1;

    m_ptPortData = (PORTDATA*)(new BYTE[sizeof(PORTDATA)]);
    memset(m_ptPortData, 0, sizeof(PORTDATA));

    ResetData();
}

void CTorqueDlg::InitMainShowPara()
{
    WORD    i = 0;

    for(i=0; i< m_ptShow->nMainNum && i<MAXMAINPARA; i++)
    {
        m_strMainName[i] = theApp.GetMainShowName(m_ptShow, i).c_str();

        //if(i<4) /* 管材显示参数，在后面没获取 */
        //{
        //    continue;
        //}
        if(i == MAINSHOWTALLY)  // 第5个为入井序号，不需要使用combobox
            continue;

        m_strMainValue[i] = m_ptCfg->strValue[m_ptShow->nMain[i]].c_str();
    }

    UpdateData(FALSE);
}

void CTorqueDlg::UpdateDlgLabel()
{
    m_strLBM2 = theApp.LoadstringFromRes(IDS_STRLINELABEL, g_tGlbCfg.strUnit).c_str();
    if(m_iBreakout)
        m_strTorqType = theApp.LoadstringFromRes(IDS_STRBREAKOUT).c_str();
    else
        m_strTorqType = theApp.LoadstringFromRes(IDS_STRMAKEUP).c_str();
}

void CTorqueDlg::InitDlgControl()
{
    CFont       myfont;   /*字体风格*/
    CString     strFontName;
    int         i = 0;

    // Checkbox
    m_cbAlarm.SetSameIcon(IDI_LEDREDOFF);
    m_cbComm.SetSameIcon(IDI_LEDREDON);
    for(i=0; i<VALVEMAXNUM; i++)
    {
        m_cbValveStatus[i].SetSameIcon(IDI_LEDREDON);
        m_bValveStatus[i] = FALSE;
    }
    m_nValveMark[0] = VALVESTATUS_SPEED;
    m_nValveMark[1] = VALVESTATUS_UNLOAD;
    m_nValveMark[2] = VALVESTATUS_PRESS;
    m_nValveMark[3] = VALVESTATUS_FLOW;
    m_nValveMark[4] = VALVESTATUS_MAKEUP;
    m_nValveMark[5] = VALVESTATUS_BREAKOUT;

    // button
    m_btnRun.SetIconAndText(IDI_RUN, IDS_STRMAINRUN);
   
    /* 设置Ruler类型和标尺 */
    m_xAxis1.SetRulerType(RT_HORIZONTAL);
    m_yAxis1.SetRulerType(RT_VERTICAL);
    m_xAxis2.SetRulerType(RT_HORIZONTAL);
    m_yAxis2.SetRulerType(RT_VERTICAL);

    m_wndTorque.SubclassDlgItem(IDC_LINE_TORQUE, this);
    m_wndRpm.SubclassDlgItem(IDC_LINE_RPM, this);

    ResetLineChart();

    InitMainShowPara();

    /* 打开串口 */
    m_bComm     = ChangeCommParam();

    return;
}

/* 获取注册信息和校验 */
void  CTorqueDlg::CheckAppReg()
{
    CFile           file;
    CFileFind       find;
    ULONGLONG       iLen = 0;
    CString         strRegCode;
    CString         strReg[REGCODESEGNUM];

    memset(&theApp.m_tdbReg, 0, sizeof(DBREG));
    theApp.m_tdbReg.bRsv1 = 6;
    theApp.m_tdbReg.bRsv2 = 12;

    COMP_BFALSE(find.FindFile(theApp.m_strRegFile.c_str(), 0));

    file.Open(theApp.m_strRegFile.c_str(), CFile::modeRead | CFile::shareDenyNone, NULL);
    iLen = file.GetLength();
    if (iLen == 0)
    {
        file.Close();
        return;
    }
    file.Read(&theApp.m_tdbReg, sizeof(DBREG));
    if (iLen > sizeof(DBREG))
    {
        file.Read(&m_dwTotalTorqNum, sizeof(DWORD));
    }
    file.Close();

    /*校验注册码是否正常*/
    strRegCode = theApp.m_tdbReg.strRegCode;
    theApp.StringSubtract(strRegCode, REGCODEVALUE);

    theApp.SplitRegString(strReg, strRegCode);

    /* 校验注册码 */
    /* 免注册版，直接注释掉Check */
    theApp.m_tdbReg.bReged = 0;
    if (CheckReg(strReg))
    {
        theApp.m_tdbReg.bReged = 1;
    }

    return;
}


/* 验证注册码是否有效 */
BOOL CTorqueDlg::CheckReg(CString strReg[])
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

    if (memcmp(adwRegVol, adwPCVol, 2 * sizeof(DWORD)) != 0 ||
        memcmp(adwRegMac, adwPCMac, 2 * sizeof(DWORD)) != 0)
    {
        return FALSE;
    }

    // 检查日期是否超过5年
    tNowDate = CTime::GetCurrentTime();
    tNowDate = CTime(tNowDate.GetYear() - VALID_YEAR,
        tNowDate.GetMonth(),
        tNowDate.GetDay(),
        0, 0, 0);
    strNowDate = tNowDate.Format("%Y%m%d");
    strRegDate.Format("%4d%02d%02d", iYear, iMonth, iDay);

    if (strNowDate > strRegDate)
    {
        theApp.SaveMessage(strRegDate);
        return FALSE;
    }

    return TRUE;
}

/* 从原始注册码解密运算 */
BOOL CTorqueDlg::GetVolMacFromRegStr(CString strReg[], DWORD pdwVol[], DWORD pdwMac[], int& iYear, int& iMonth, int& iDay)
{
    ASSERT_NULL_R(strReg, FALSE);
    ASSERT_NULL_R(pdwVol, FALSE);
    ASSERT_NULL_R(pdwMac, FALSE);

    pdwVol[0] = strtoul(strReg[4] + strReg[5], NULL, 16);
    pdwVol[1] = strtoul(strReg[0] + strReg[1], NULL, 16);
    pdwMac[0] = strtoul(strReg[3], NULL, 16);
    pdwMac[1] = strtoul(strReg[2], NULL, 16);

    pdwVol[0] = ~pdwVol[0];    ///////// 解密运算
    pdwVol[0] ^= VOL0DEC;       ///////// 解密运算
    pdwVol[0] ^= VOL0XOR;

    pdwVol[1] = ~pdwVol[1];    ///////// 解密运算
    pdwVol[1] ^= VOL1DEC;       ///////// 解密运算
    pdwVol[1] ^= VOL1XOR;

    pdwMac[0] = ~pdwMac[0];
    pdwMac[0] ^= MAC0DEC;
    pdwMac[0] ^= MAC0XOR;
    pdwMac[0] &= 0xFFFFFF;

    pdwMac[1] = ~pdwMac[1];
    pdwMac[1] ^= MAC1DEC;
    pdwMac[1] ^= MAC1XOR;
    pdwMac[1] &= 0xFFFF;

    iYear = (pdwVol[1] & 0x00FFFF00) >> 8;
    iMonth = (pdwVol[1] & 0x000000FF);
    iDay = (pdwVol[1] & 0xFF000000) >> 24;

    return TRUE;
}

BOOL CTorqueDlg::GetVolMacInfo(DWORD pdwVol[], DWORD pdwMac[], int iYear, int iMonth, int iDay)
{
    UCHAR ucMac[5];

    ASSERT_NULL_R(pdwVol, FALSE);
    ASSERT_NULL_R(pdwMac, FALSE);

    GetVolumeInformation("C:\\", NULL, NULL, &pdwVol[0], NULL, NULL, NULL, 0);
    pdwVol[1] = (iDay << 24) + (iYear << 8) + iMonth;

    theApp.GetMACAddr(ucMac);
    pdwMac[0] = ((ucMac[0] * 256) + ucMac[1]) * 256 + ucMac[2];
    pdwMac[1] = (ucMac[3] * 256) + ucMac[4];

    return TRUE;
}

void CTorqueDlg::SaveTorqNum()
{
    CFile           file;
    CFileFind       find;

    COMP_BFALSE(find.FindFile(theApp.m_strRegFile.c_str(), 0));

    file.Open(theApp.m_strRegFile.c_str(), CFile::modeRead | CFile::modeWrite | CFile::shareDenyNone, NULL);
    if (file.GetLength() == 0)
    {
        file.Close();
        return;
    }
    file.Seek(sizeof(DBREG), CFile::current);
    file.Write(&m_dwTotalTorqNum, sizeof(DWORD));
    file.Close();
}


BOOL CTorqueDlg::OnInitDialog()
{
    CRect       rcTmp;
    CString     strAboutMenu;
    CMenu       *pSysMenu = NULL;    

    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    // Load accelerators
    m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(), m_lpszTemplateName);
    ASSERT(m_hAccel);

    pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
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

    m_tMenu[LANGUAGE_CHINESE].LoadMenu(IDR_MAINMENU);
    m_tMenu[LANGUAGE_ENGLISH].LoadMenu(IDR_MAINMENU_ENG);
    //m_tMenu[LANGUAGE_RUSSIAN].LoadMenu(IDR_MAINMENU_RUS);
    SetMenu(&m_tMenu[g_tGlbCfg.nLangType]);

    m_iBreakout = g_tGlbCfg.bBreakOut;

    // TODO: Add extra initialization here
    /* 状态栏处理 */
    m_StatusBar.CreateEx(this,SBT_TOOLTIPS,WS_CHILD|WS_VISIBLE|CBRS_BOTTOM,AFX_IDW_STATUS_BAR);
    // Set the indicators namely caps and nums lock status
    m_StatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT));
    GetClientRect(&rcTmp);
    m_StatusBar.SetPaneInfo(0,ID_INDICATOR_NUM,SBPS_STRETCH ,rcTmp.Width()-100);
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,ID_INDICATOR_NUM);

    theApp.AdaptDlgCtrlSize(this, 0);

    /* 初始数据区和变量 */
    InitVariant();

    /* 初始化对话框控件 */
    InitDlgControl();

    /* 获取当前数据序号 */
    ReGetTorqNo();

    //GetCurNum();

    m_dwTotalTorqNum = 0;
    /*检查注册情况*/
    CheckAppReg();
    /* 检查产品有效日期 */
    if (!theApp.CheckProductDate())
    {
        theApp.m_tdbReg.bReged = 0;
    }
    //theApp.m_tdbReg.bReged = 1;

    if (0 == m_dwTotalTorqNum)
        m_dwTotalTorqNum = m_nCurNO;

    SHOW_MAINDLG_BUTTON(theApp.m_bShowCRC);

    CalcDelayCount();


    //设置窗口属性
    SetProp(m_hWnd,g_strPropName.c_str(),g_hValue);
    theApp.ShowMainTitle();
	
    UpdateData(FALSE);

    theApp.SaveAppStatus(STATUS_START, __FUNCTION__);

    StartGetValveStatus();

    //Invalidate(TRUE);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTorqueDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.m_ptParent = this;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}


BOOL CTorqueDlg::JudgeRunStatus(unsigned wInfo)
{
    string strInfo;

    if (m_bRunStatus)
    {
        strInfo = theApp.LoadstringFromRes(wInfo);
        AfxMessageBox(strInfo.c_str());
        return FALSE;
    }
    return TRUE;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CTorqueDlg::OnPaint()
{
    int     iCxIcon = 0;
    int     iCyIcon = 0;
    int     x       = 0;
    int     y       = 0;
    CRect   rcTmp;

    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        iCxIcon = GetSystemMetrics(SM_CXICON);
        iCyIcon = GetSystemMetrics(SM_CYICON);
        GetClientRect(&rcTmp);
        x = (rcTmp.Width()  - iCxIcon + 1) / 2;
        y = (rcTmp.Height() - iCyIcon + 1) / 2;

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
HCURSOR CTorqueDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

/* IDS_STRDATEFORM         "2014-11-18_" */
BOOL CTorqueDlg::TimeValidWell(CString strFileName)
{
    CString     strDate;
    int         iYear = 0;
    int         iMonth = 0;
    int         iDay = 0;
    CTime       time = CTime::GetCurrentTime();//得到当前时间
    CTime       oldtime;
    CTimeSpan   tSpan;

    if (!g_tGlbCfg.bDateBehind) //  日期在前
    {
        strDate = strFileName;
    }
    else    /* "2014-11-18.pbd" */
    {
        strDate = strFileName.Right(14);
    }
    iYear = atoi(strDate.Left(4));
    strDate = strDate.Right(strDate.GetLength() - 5);
    iMonth = atoi(strDate.Left(2));
    strDate = strDate.Right(strDate.GetLength() - 3);
    iDay = atoi(strDate.Left(2));

    oldtime = CTime(iYear, iMonth, iDay, 0, 0, 0, 0);
    tSpan = time - oldtime;
    COMP_BLE_R(tSpan.GetDays(), 60, TRUE);

    /* else */
    return FALSE;
}

void  CTorqueDlg::GetCurWellFile()
{
    WIN32_FIND_DATA findData;
    TCHAR       tcsExename[MAX_PATH];
    CString     strSearch;
    CString     strWell;
    HANDLE      hFindHandle;

    SetCurrentDirectory(theApp.m_strDataPath.c_str());

    /* 以井号保存文件 */
    strWell = m_ptCfg->strValue[m_ptShow->nFileName].c_str();
    if (strWell.IsEmpty())
    {
        CreateNewWellFile();
        return;
    }

    if (!g_tGlbCfg.bDateBehind)
        strSearch = _T("*_") + strWell + _T(".pbd");
    else
        strSearch = strWell + _T("_*") + _T(".pbd");

    hFindHandle = FindFirstFile(strSearch, &findData);

    if ((hFindHandle != INVALID_HANDLE_VALUE))
    {
        GetLongPathName(findData.cFileName, tcsExename, MAX_PATH);
        /* 没有超过两个月 */
        if (TimeValidWell(tcsExename))
        {
            theApp.m_strDataFile = theApp.m_strDataPath + tcsExename;
            return;
        }

        /* else */
        while (FindNextFile(hFindHandle, &findData) != 0)
        {
            /* 没有超过两个月 */
            GetLongPathName(findData.cFileName, tcsExename, MAX_PATH);
            if (TimeValidWell(tcsExename))
            {
                theApp.m_strDataFile = theApp.m_strDataPath + tcsExename;
                return;
            }
        }
    }

    /*超过两个月，或者没有找到文件，生成新文件*/
    CreateNewWellFile();

    return;
}

void CTorqueDlg::CreateNewWellFile()
{
    CString strTemp;
    CTime   time = CTime::GetCurrentTime();//得到当前时间
    string  strDataFile;

    /* 以井号保存文件 */
    strTemp = m_ptCfg->strValue[m_ptShow->nFileName].c_str();
    strDataFile = theApp.m_strDataPath;
    if (!g_tGlbCfg.bDateBehind)
    {
        strDataFile += time.Format(IDS_STRDATEFORM);
        strDataFile += _T("_");
        strDataFile += strTemp;
    }
    else
    {
        strDataFile += strTemp;
        strDataFile += _T("_");
        strDataFile += time.Format(IDS_STRDATEFORM);
    }
    strDataFile += _T(".pbd");
    theApp.m_strDataFile = strDataFile;
    m_nCurNO = 0;
    return;
}
void CTorqueDlg::GetMakeupCurNum()
{
    CFile   file;
    int     iTallyIndex = -1;
    int     iTallyNO = 0;
    int     i = 0;
    CString strNumInfo;
    TorqData::Torque* ptTorq = NULL;
    m_nCurNO = 0;

    /* 获取当前的记录文件，超过60天生成新的文件，序号归零 */
    GetCurWellFile();
    file.Open(theApp.m_strDataFile.c_str(), CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone, NULL);
    if (file.GetLength() != 0)
    {
        file.Read(&m_nCurNO, sizeof(UINT));
    }
    file.Close();

    /* 文件记录超量，生成新的文件，序号归零 */
    if (m_nCurNO >= MAXWELLNUM)
        // || m_nCurNO == 0)  为什么每天生成一个数据文件？？的源头 20190927
    {
        CreateNewWellFile();
    }

    if (m_nCurNO > 0 && theApp.ReadHisTorqFromFile(theApp.m_strDataFile.c_str()))
    {
        iTallyIndex = theApp.GetMainIndex(MAINSHOWTALLY);
        if (iTallyIndex >= 0)
        {
            /* 从后往前找最新的入井序号 */
            for (i = g_tReadData.nTotal - 1; i >= 0; i--)
            {
                ptTorq = &g_tReadData.tData[i];
                if (iTallyIndex >= ptTorq->tshow_size())
                    continue;

                iTallyNO = atoi(theApp.GetTorqShowValue(ptTorq, iTallyIndex));
                if (iTallyNO > 0)
                    break;
            }
            /* else iWellNO is 0*/
        }
    }

    /* 记录当前文件名及序号 */
    strNumInfo.Format("%s--%d", theApp.m_strDataFile.c_str(), m_nCurNO);
    theApp.SaveMessage(strNumInfo);

    m_nCurRunningNO = m_nCurNO + 1;
    if (iTallyNO >= 0)
        m_nCurRunningNO = iTallyNO + 1;
}

void CTorqueDlg::IncTorqNo()
{
    m_nCurNO++;
    m_dwTotalTorqNum++;
}

void CTorqueDlg::GetBreakoutCurNum()
{
    CFile   file;
    int     iTallyIndex = -1;
    int     iTallyNO = 0;
    int     i = 0;
    CString strNumInfo;
    TorqData::Torque* ptTorq = NULL;
    m_nCurNO = 0;

    if (g_tGlbCfg.strBreakOutPath.empty())
        return;
    if (theApp.ReadHisTorqFromFile(g_tGlbCfg.strBreakOutPath.c_str()))
    {
        iTallyIndex = theApp.GetMainIndex(MAINSHOWTALLY);
        if (iTallyIndex >= 0)
        {
            /* 从后往前找最新的入井序号 */
            for (i = g_tReadData.nTotal - 1; i >= 0; i--)
            {
                ptTorq = &g_tReadData.tData[i];
                if (iTallyIndex >= ptTorq->tshow_size())
                    continue;

                iTallyNO = atoi(theApp.GetTorqShowValue(ptTorq, iTallyIndex));
                if (iTallyNO > 0)
                    break;
            }
            /* else iWellNO is 0*/
        }
    }

    /* 记录当前文件名及序号 */
    strNumInfo.Format("%s--%d", g_tGlbCfg.strBreakOutPath.c_str(), m_nCurNO);
    theApp.SaveMessage(strNumInfo);

    m_nCurRunningNO = m_nCurNO + 1;
    if (iTallyNO >= 0)
        m_nCurRunningNO = iTallyNO + 1;
}

/* 获取当前扭矩的序号 */
void CTorqueDlg::GetCurNum()
{
    if (!g_tGlbCfg.bBreakOut)
    {
        GetMakeupCurNum();
    }
    else
    {
        GetBreakoutCurNum();
    }
}

/* 根据原始plus和显示plus，计算数据画图的点数 */
/* 20190126 脉冲差值为负，说明卸扣结束 */
/* 20200313 20200223 两组数据可能重复，脉冲数会比上一组最后一个小，跳过，所以m_iOutPoints不能清零
   如果异常，在ResetData中清零
   20200706 固定SAVE_FULLCIR保存，计算两个点数数据，一个是保存的，一个是显示的
   */
void CTorqueDlg::CalcPointNum(COLLECTDATA *ptCollData, ORGDATA *ptOrgData)
{
    int     iPlus       = 0;
    int     iCurPoints  = 0;
    //int     iSavePnts   = 0;
    
    ASSERT_NULL(ptCollData);

    /* 20191207 第一个数据脉冲数比较大，修改显示脉冲数为当前脉冲 */
    if(m_iShowPlus == 0 && ptCollData->nOrgPlus > 0)
    {
        m_iShowPlus  = ptCollData->nOrgPlus;
    }

    iPlus = ptCollData->nOrgPlus - m_iShowPlus;
    if(iPlus < 0)
    {
        //if(1 == m_iBreakout)
        //    ptCollData->ucStatus = PLCSTATUS_BREAKOUT;  /* FE:卸扣结束254 */
        iPlus = 0;
        //m_iOutPoints = 0;
    }

    iCurPoints =int((iPlus*MAXLINEITEM) /
                    (g_tGlbCfg.nPlusPerTurn *m_ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT]) + 0.5);

    if(ptOrgData != NULL)
        ptOrgData->ucPointNum = iCurPoints;

    ptCollData->iPointNum = iCurPoints - m_iRecPNum;
    if(ptCollData->iPointNum < 0)
    {
        ptCollData->iPointNum = 0;
    }

    if(iCurPoints < COLLECTPOINTS)
        m_iRecPNum = iCurPoints;

    return;
}

/* 定时读取串口数据，在复位和运行状态时调用
   返回值FALSE，状态异常，返回失败
         TRUE,且bDataValid为TRUE， 数据OK，继续处理数据
         TRUE,但bDataValid为FALSE，状态正常，数据不OK，不处理数据 */
BOOL CTorqueDlg::TimeReadPort(UINT nCurStatus)
{
    BOOL    bReadRes = FALSE;
    CString strInfo;
    /* 状态异常 */
    COMP_BG_R(m_nClashSta, RS_MAX, FALSE);

    /* CLASH 状态，已经清空了缓冲，直接发送READ请求 
       或者正常状态，缓冲为空的情况，直接发送READ请求 */
    m_nClashSta = RS_NORMAL;

    if(m_iTorqBreakCnt > 0)
    {
        SetCommShowInfo(RS_COMM_OPEN);
    }

    bReadRes = SendData(SCMREADMULTI);
    m_iTorqBreakCnt--;

    /* 连续5次没有读取到，设置串口为关闭状态 */
    if(m_iTorqBreakCnt <= 0)
    {
        strInfo.Format("BreakCnt STOP by Func(%s) on Line(%d) ", __FUNCTION__, __LINE__);
        theApp.SaveMessage(strInfo);
        StopTorque();
        
        m_bComm      = FALSE;
        m_bRunStatus = FALSE;
        m_bAlarm     = TRUE;
        m_cbAlarm.SetSameIcon(IDI_LEDREDON);
        theApp.PlayAlarmSound();
        m_hrtPlayAlarm.CreateTimer(this, ALRAM_DELAY, HRTPlayAlarm);
        return FALSE;
    }
    
    if(bReadRes)
        return TRUE;
    
    /* 发送数据失败，需要保存异常记录 */
    theApp.SaveSendFailure(SCMREADMULTI);

    return FALSE;
}

/* 写串口之前判断串口入队列是否为空，如非空需要Clash处理，等待队列空后再写 */
BOOL CTorqueDlg::PortQueIsEmpty()
{
    return TRUE;
#if 0
    /* 正常状态，判断缓冲是否为空 */
    ASSERT_NULL_R(g_lpNewComThread, FALSE);

    return g_lpNewComThread->InQueIsEmpty();
#endif
}

/* 如果时间小于读取时间间隔，返回失败 */
BOOL CTorqueDlg::CheckReadInterval()
{
    double  fCurTime = 0;
    double  fDiff     = 0;
    CString strInfo;
    fCurTime = clock() * 1000.0 / CLOCKS_PER_SEC;

    fDiff = fCurTime - m_fPreReadTime;
    m_fPreReadTime = fDiff;

    COMP_BGE_R(fDiff, g_tGlbCfg.nCollectDur, TRUE);

    strInfo.Format("Read Interval is %.0f(<%ld)", fDiff, g_tGlbCfg.nCollectDur);
    theApp.SaveMessage(strInfo);
    return FALSE;
}

/* 有效数据的长度应该大于12，有长度为24的数据，也可以尽可能取出一条来，
   但是前12个数据通过CRC校验，则当做普通数据处理，调试开关打开时也将原始数据保存下来 */
BOOL CTorqueDlg::CheckDataValid()
{
    int     i           = 0;
    BYTE    ucSCMRejust = 0;
    BOOL    bValid      = TRUE;

    if (m_wRcvLen == 0)
        return FALSE;

    /* 检验长度 */
    bValid &= (m_wRcvLen >= PORT_VALIDLEN);
    /* 检验串口号 */
    bValid &= (m_ucRcvByte[0] == PORT485);
    /* 检验数据有效性 */
    if(m_ucRcvByte[3]== COM_READ)
    {
        for(i=0; i<PORT_SUMPLACE; i++)
            ucSCMRejust += m_ucRcvByte[i];
        bValid &= (ucSCMRejust == m_ucRcvByte[PORT_SUMPLACE]);
    }
    /* 暂不校验CRC */
    else if(m_ucRcvByte[3]== COM_READMULTI) 
    {
        /*for(i=0; i<m_wRcvLen-1; i++)
            ucSCMRejust += m_ucRcvByte[i];
        bValid &= (ucSCMRejust == m_ucRcvByte[m_wRcvLen-1]);*/
    }
    else    /* error command */
        bValid = FALSE;

    COMP_BTRUE_R(bValid, TRUE);
    
    theApp.SaveCrcErrorData(m_ucRcvByte, m_wRcvLen, m_nCRCERR);
    return FALSE;
}

/*  m_nTestFunc = 0(COLL_PORT)
    实际读取串口的数据，需要判断第一个数据为PORT485，否则为无效数据
*/
BOOL CTorqueDlg::CollectPort(COLLECTDATA *ptCollData)
{
#ifdef TESTPORT
    /*BYTE   ucTest[] = {0x21,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x22,0x00,0x00,0x08,
                       0x21,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x22,0x00,0x00,0x08 };*/
    BYTE   ucTest[] = {0x21,0x03,0x00,0x15,0x00,0x62,0x00,0xFF,0x05,0x00,0x52,0x21,0x17,
                       0x00,0x7e,0x21,0x26,0x00,0x8c,0x21,0x35,0x00,0x9c ,0x21,0x47,
                       0x00,0xa2,0x21,0x54,0x08 };
#endif

    ASSERT_NULL_R(ptCollData, FALSE);

    //m_iReadTimes++;

    /* 如果时间小写读取时间间隔，返回失败 */
    COMP_BFALSE_R(CheckReadInterval(), FALSE);
#ifndef TESTPORT
    /* 读取数据失败，返回 */
    COMP_BFALSE_R(TimeReadPort(RS_READ_NORMAL), FALSE);
    /* bRead && bData, 往下处理数据 */
    /* 有效数据的长度应该大于12，有长度为24的数据，也可以尽可能取出一条来，
       但是前12个数据通过CRC校验，则当做普通数据处理，调试开关打开时也将原始数据保存下来 */
    //COMP_BFALSE_R(CheckDataValid(), FALSE);
#else
    memcpy(m_ucRcvByte, ucTest, 30);
    m_wRcvLen = 30;
#endif

    return TRUE;
}

/*  m_nTestFunc = (COLL_RAND)1
    Run时随机生成的数据，家里程序上位机测试使用
    Run一次所有数据都一样，Stop后再随机生成一次
*/
BOOL CTorqueDlg::CollectRandData(COLLECTDATA* ptCollData)
{
    int     iCurPoints  = 0;
    int     iPlus       = 0;

    ASSERT_NULL_R(ptCollData, FALSE);

    ptCollData->fTorque   = m_fTestTorq[m_iTest];
    ptCollData->nOrgPlus  = m_iTestPlus[m_iTest++];

    /* 根据Plus计算数据的点数 */
    CalcPointNum(ptCollData, NULL);
    if(0 == m_iBreakout)
    {
        ptCollData->ucStatus = PLCSTATUS_NORMAL;
        if(ptCollData->fTorque > m_ptCtrl->fTorqConf[INDEX_TORQ_CONTROL])
        {
            ptCollData->ucStatus = PLCSTATUS_LOCKON;

            /* 串口数据重新初始化 */
            RunInitRand();
        }
    }

    if(m_iTest >= TESTNUM)
    {
        ptCollData->fTorque = m_ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL];
        m_iTest = 0;
    }

    /* 调试显示随机数据 */
    m_strRecvData.Format("%ld,%ld,%d",(int)ptCollData->fTorque,ptCollData->nOrgPlus,ptCollData->ucStatus);

    /*300编码器，转速太快，除2降1倍*/
    ptCollData->fRpm = m_iOutPoints * m_ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT] * 0.8/g_tGlbCfg.fRpmAdj;

    return TRUE;
}

/*  m_nTestFunc = (COLL_TORQUE)2
    读AutoSave扭矩数据，并显示
    功能为获取现场测试保存的数据，复现现场的图形
*/
BOOL CTorqueDlg::CollectTorqData(COLLECTDATA* ptCollData)
{
    BOOL    bReadData   = FALSE;
    BOOL    bValidData  = FALSE;
    int     iCurPoints  = 0;
    int     iPlus       = 0;
    int     i           = 0;
    char    *endptr     = NULL;
    char    acData[3*PORT_MAXVLDDATA+1] = {0};
    BYTE    ucByte[PORT_MAXVLDDATA]     = {0};  //{0x12,0x34,0x56,0x78};
    /* 串口获取的原始有效数据和计算的扭矩等信息 */
    ORGDATA tOrgData    = {0};
    double  fShowTorq   = 0;
    int     iOrgTorq    = 0;
    int     iTorque     = 0;
    double  fRpm        = 0;
    int     iDataLen    = 0;
    CString strTest;
    CString strTime;

    ASSERT_NULL_R(ptCollData, FALSE);
    COMP_BFALSE_R(m_bAutoFileOpen, FALSE);

    /* 读取很多无效值时，可能会长时间循环，设置漏斗鼠标 */
    BeginWaitCursor();

    while(!bValidData)
    {
        bReadData = m_AutoSavefile.ReadString(strTest);
        /*数据读完，调用OnBtrun停止*/
        if(!bReadData)
        {
            OnBtrun();
            return FALSE;
        }

        /*调试信息，直接跳过*/
        COMP_BTRUE_CONTINUE(theApp.IsDebugInfo(strTest));

        /* 小于有效长度的数据，跳过 */
        COMP_BTRUE_CONTINUE(strTest.GetLength() < (ORG_DATAPLACE+ORG_PRNDATALEN));

        /*获取数据，去掉前面的时间信息*/
        strTime = strTest.Left(ORG_PRNTIMELEN);

        /* 根据 " T " 找到前面的有效数据 */
        iDataLen = strTest.Find("T ") - ORG_DATAPLACE;
        if (iDataLen < 3 * PORT_DATALEN || iDataLen % 3 != 0)
            continue;
        if (iDataLen > 3 * PORT_MAXVLDDATA)
            iDataLen = 3 * PORT_MAXVLDDATA;

        memcpy(acData, strTest.Mid(ORG_DATAPLACE, iDataLen), iDataLen);
        iDataLen /= 3;
        for (i = 0; i < iDataLen; i++)
            ucByte[i] = (BYTE)strtol(&acData[i * 3], &endptr, 16);

        /* 处理有效数据 */
        bValidData = TRUE;
        memcpy(tOrgData.ucRcvByte, &ucByte, PORT_DATALEN);
        iOrgTorq = ucByte[0]*256+ ucByte[1];
        ptCollData->ucStatus =  (ucByte[2] & 0xE0) / 32;
        ptCollData->nOrgPlus = ((ucByte[2] & 0x1F) * 256 +ucByte[3]) * 4;
        fRpm                 = ucByte[5] * 256 + ucByte[6];  // 200ms 脉冲数
        /* 奇偶处理需要使用源扭矩，因此10倍放到下面相乘 */
        /* 3.17和4.17合一，扭矩需要乘倍率 */
        if(!ClearExcepTorq(iOrgTorq, iTorque))
            ptCollData->ucStatus = PLCSTATUS_REVERSE;
        // ptCollData->fTorque  = iTorque * theApp.m_nTorqMulti;
        fShowTorq = iTorque * theApp.m_nTorqMulti;
        /* 20220215 处理未打折的数据 ==> 对齐PLC打折处理 */
        ptCollData->fTorque = (int)OverTorqOpt(fShowTorq, ptCollData->ucStatus);

        if (iDataLen >= PORT_DATALEN + 2)
        {
            LightValveStatus(&ucByte[7]);
        }
    }

    EndWaitCursor();

    /* 正常数据保存 */
    tOrgData.ucStatus = ptCollData->ucStatus;
    tOrgData.nTorque = (UINT)ptCollData->fTorque;
    tOrgData.nPlus   = ptCollData->nOrgPlus;

    /* 根据Plus计算数据的点数 */
    CalcPointNum(ptCollData, &tOrgData);

    /* 300编码器，转速太快，除m_fRpmAdj降速度 */
    if(fRpm == 0)
        tOrgData.fRpm = ptCollData->fRpm = m_iOutPoints * m_ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT] * 0.8/g_tGlbCfg.fRpmAdj;
    else
        tOrgData.fRpm = ptCollData->fRpm = fRpm * 5 * 60 / g_tGlbCfg.nPlusPerTurn/2;

    /* 调试显示接收到的有效数据 */
    m_strRecvData.Format("%ld,%ld,%d,%.1f(%s)",(int)ptCollData->fTorque,ptCollData->nOrgPlus,ptCollData->ucStatus,ptCollData->fRpm,strTime);

    return TRUE;
}

/*  m_nTestFunc = (COLL_MULTITORQ)3
    读AutoSave扭矩数据，并显示
    功能为获取现场测试保存的数据，复现现场的图形
    读取多组数据
    20:40:26.627 : ff 15 00 62 00 ff 05 00 52 21 17 00 7e 21 26 00 8c 21 35 00 9c 21 47 00 a2 21 54 08
*/
BOOL CTorqueDlg::CollectMultiTorq(COLLECTDATA* ptCollData)
{
    BOOL    bReadData   = FALSE;
    BOOL    bValidData  = FALSE;
    int     i           = 0;
    char    *endptr     = NULL;
    BYTE    *pucData    = NULL;
    char    acData[150] = {0};
    BYTE    ucByte[50]  = {0};  //{0x12,0x34,0x56,0x78};
    /* 串口获取的原始有效数据和计算的扭矩等信息 */
    int     iOrgTorq    = 0;
    int     iTorque     = 0;
    BYTE    nDataNum    = 0;
    double  fRpm        = 0;
    int     iDataLen    = 0;
    WORD    wCalLen     = 0;
    COLLECTDATA* ptOrgData = NULL;
    double  fShowTorq   = 0;
    CString strTest;
    CString strTime;

    ASSERT_NULL_R(ptCollData, FALSE);
    COMP_BFALSE_R(m_bAutoFileOpen, FALSE);

    ptOrgData = ptCollData;

    /* 读取很多无效值时，可能会长时间循环，设置漏斗鼠标 */
    BeginWaitCursor();

    while(!bValidData)
    {
        bReadData = m_AutoSavefile.ReadString(strTest);
        /*数据读完，调用OnBtrun停止*/
        if(!bReadData)
        {
            OnBtrun();
            return FALSE;
        }

        /*调试信息，直接跳过*/
        COMP_BTRUE_CONTINUE(theApp.IsDebugInfo(strTest));

        /* 小于有效长度的数据，跳过 */
        COMP_BTRUE_CONTINUE(strTest.GetLength() < (ORG_DATAPLACE+33));

        /*获取数据，去掉前面的时间信息*/
        strTime = strTest.Left(ORG_PRNTIMELEN);

        iDataLen = strTest.GetLength() - ORG_DATAPLACE + 1;

        memcpy(acData, strTest.Mid(ORG_DATAPLACE, iDataLen), iDataLen);
        iDataLen /= 3;
        for (i = 0; i < iDataLen; i++)
            ucByte[i] = (BYTE)strtol(&acData[i * 3], &endptr, 16);

        if (ucByte[0] != 0xFF || ucByte[1] != COM_READMULTI)
            continue;
        
        /* 处理有效数据 */
        bValidData = TRUE;
    }

    EndWaitCursor();

    LightValveStatus(&ucByte[4]);
    
    fRpm = ucByte[2] * 256 + ucByte[3];  // 200ms 脉冲数

    nDataNum = ucByte[6];
    pucData  = &ucByte[7];
    wCalLen  = 7 + nDataNum*PORT_GRPDATA;
    if(iDataLen < wCalLen)
        nDataNum = (iDataLen - 7) / PORT_GRPDATA;
    for(i=0; i<nDataNum && i<PORT_MAXDATANUM; i++)
    {
        iOrgTorq = (*(pucData) * 256 + *(pucData+1));
        ptOrgData->ucStatus =  (*(pucData+2) & 0xE0) / 32;
        ptOrgData->nOrgPlus = ((*(pucData+2) & 0x1F) * 256 + *(pucData+3)) * 4;
        ptOrgData->fRpm     = fRpm * 5 * 60 / g_tGlbCfg.nPlusPerTurn/2;
        /* 奇偶处理需要使用源扭矩，因此10倍放到下面相乘 */
        if(!ClearExcepTorq(iOrgTorq, iTorque))
            ptCollData->ucStatus = PLCSTATUS_REVERSE;
        //ptOrgData->fTorque  = iTorque * theApp.m_nTorqMulti;
        fShowTorq = iTorque * theApp.m_nTorqMulti;
        /* 20220215 处理未打折的数据 ==> 对齐PLC打折处理 */
        ptOrgData->fTorque = (int)OverTorqOpt(fShowTorq, ptCollData->ucStatus);
        
        /* 根据Plus计算数据的点数 */
        CalcPointNum(ptOrgData, NULL);

        ptOrgData++;
        pucData += 4;
    }

    /* 调试显示接收到的有效数据 */
    ptOrgData = &ptCollData[nDataNum-1];
    m_strRecvData.Format("%ld,%ld,%d,%.1f(%s)",(int)ptOrgData->fTorque,ptOrgData->nOrgPlus,ptOrgData->ucStatus,ptOrgData->fRpm,strTime);
    return TRUE;
}

/*  m_nTestFunc = 4
    读AutoSave Collect数据，并显示
    由DlgCollect调用
*/
BOOL CTorqueDlg::CollectHisData()
{
    BOOL    bReadData   = FALSE;
    BOOL    bValidData  = FALSE;
    int     iPlace      = -1;
    int     i           = 0;
    int     iBufIndex   = 0;
    unsigned long ulTmp = 0;
    char    ucTemp[5];
    char    *pcStopStr  = NULL;
    CString strTest;

    COMP_BFALSE_R(m_bAutoFileOpen, FALSE);

    ucTemp[0] = '0';
    ucTemp[1] = 'X';

    /* 读取很多无效值时，可能会长时间循环，设置漏斗鼠标 */
    BeginWaitCursor();
    while(!bValidData)
    {
        bReadData = m_AutoSavefile.ReadString(strTest);
        /* 数据读完返回 */
        if(!bReadData)
        {
            m_AutoSavefile.Close();
            m_bAutoFileOpen = FALSE;
            return FALSE;
        }

        /* 非COL信息，直接跳过 */
        iPlace = strTest.Find(theApp.m_strDbgHead[DBG_COLLECT]);
        COMP_BTRUE_CONTINUE(iPlace == -1);

        /* 找到有效数据位置 */
        iPlace = strTest.Find(") ");
        COMP_BTRUE_CONTINUE(iPlace == -1);

        for(i=iPlace+2; i<strTest.GetLength(); i++)
        {
            /* 一个数字形如"21 "，后面有一个空格，空格由for循环的i自己累加 */
            ucTemp[2] = strTest.GetAt(i++);
            ucTemp[3] = strTest.GetAt(i++);
            ucTemp[4] = 0;
            ulTmp = strtoul(&ucTemp[0], &pcStopStr, 0);
            m_ucRcvByte[iBufIndex++] = (BYTE)ulTmp;
        }

        m_wRcvLen  = iBufIndex;
        bValidData = TRUE;
    }
    EndWaitCursor();

    return TRUE;
}

BOOL CTorqueDlg::ValidStatus3(COLLECTDATA* ptCollData)
{
    ASSERT_NULL_R(ptCollData, FALSE);

    /* 前面有有效的数据， 状态3有效 */
    COMP_BG_R(m_tCollData.nAllCount, 1, TRUE);

    /* 前一次的数据如果无效（无效会清除，一般为0），马上出现状态3，为无效的状态3
       需要更新ShowPlus，否则转速会很大 */
    COMP_BG_R(m_fOutTorque, m_ptCtrl->fTorqConf[INDEX_TORQ_SHOW], TRUE);

    ReStart();
    m_iShowPlus = ptCollData->nOrgPlus;
    return FALSE;
}

/* 下位机上报状态3的处理 */
BOOL CTorqueDlg::Status3Proc(COLLECTDATA* ptCollData, BOOL *pFinish)
{
    static int  iPreTorq    = 0;

    ASSERT_NULL_R(pFinish, FALSE);
    ASSERT_NULL_R(ptCollData, FALSE);

    /* 20140211 保存一个数据后，如果马上出现状态3，则是缓存中的垃圾数据，直接删除 */
    COMP_BFALSE_R(ValidStatus3(ptCollData), FALSE);

    m_nCurDelay++;
    if(m_nCurDelay == 1)
    {
        iPreTorq    = (int)ptCollData->fTorque;
        if(ptCollData->iPointNum == 0)
            ptCollData->iPointNum = 1;
        return TRUE;
    }
    else if(m_nCurDelay >= m_nDelayCount)
    {
        iPreTorq    = 0;
        m_nCurDelay = 0;
        *pFinish    = TRUE;
        return TRUE;
    }
    else
    {
        // 扭矩没有变化，直接返回
        // 20200315 前一个扭矩可能比当前扭矩大，上一次数据状态3后冲高，这次重复上次数据
        if(iPreTorq >= ptCollData->fTorque)
        {
            ptCollData->ucStatus = PLCSTATUS_REPEAT;
            return TRUE;
        }
        // 至少插入一个点
        iPreTorq = (int)ptCollData->fTorque;
        if(ptCollData->iPointNum == 0)
            ptCollData->iPointNum = 1;
    } 

    return TRUE;
}

/* 下位机上报状态4的处理 */
BOOL CTorqueDlg::Status4Proc(COLLECTDATA* ptCollData, BOOL *pFinish)
{
    ASSERT_NULL_R(ptCollData, FALSE);
    ASSERT_NULL_R(pFinish, FALSE);

    if(m_iOutPoints == 0)
    {
        ZeroPointUpdateData(ptCollData, FALSE);
    }
    /* 插入数据(包括构造多个数据) */
    else
    {
        MorePointInsertData(ptCollData, FALSE);
    }

    *pFinish  = TRUE;
    return TRUE;
}

/* 卸扣版本，脉冲数反转，设置状态为254 */
BOOL CTorqueDlg::Status254Proc(COLLECTDATA* ptCollData, BOOL *pFinish)
{
    double  fPreTorque  = 0;    /* 前一次扭矩，PointNum > 1时计算数据 */
    double  fPreRpm     = 0;    /* 前一次转速 */
    
    ASSERT_NULL_R(ptCollData, FALSE);
    ASSERT_NULL_R(pFinish, FALSE);

    ASSERT_ZERO_R(m_tCollData.nAllCount, FALSE);

    fPreTorque = m_tCollData.fTorque[m_tCollData.nAllCount - 1];
    fPreRpm    = m_tCollData.fRpm[m_tCollData.nAllCount - 1];

    /* 插入状态254的数据暂时只考虑插入一个数据,在Finish控制时插入且将bLastPoint置为TRUE */
    INSERT_DATA(fPreTorque, fPreRpm, FALSE);

    m_wndTorque.UpdatePos(fPreTorque);
    m_wndRpm.UpdatePos(fPreRpm);
    UpdateTorqueData(fPreTorque, fPreRpm);

    UpdateOutData(ptCollData);

    *pFinish  = TRUE;
    return TRUE;
}

/* 收集到的数据脉冲没有变化，只更新扭矩 */
void CTorqueDlg::ZeroPointUpdateData(COLLECTDATA *ptCollData, BOOL bFinish)
{
    ASSERT_NULL(ptCollData);

    /* 第一个数据会在Zero中处理，记录fOutTorque，OutPointNum为1
       第二个数据时，会在More中更新到结构中 */
    if(m_tCollData.nAllCount == 0)
    {
        //m_tCollData.nAllCount = 1;
        INSERT_DATA(ptCollData->fTorque, ptCollData->fRpm, FALSE);
        UpdateOutData(ptCollData->fTorque, 1);
        return;
    }

    /* 20200418 zero更新只更新扭矩更大的值 */
    if(m_fOutTorque > m_tCollData.fTorque[m_tCollData.nAllCount-1])
    {
        m_wndTorque.UpdatePos(m_fOutTorque);
        m_wndRpm.UpdatePos(ptCollData->fRpm);
        UpdateTorqueData(m_fOutTorque, ptCollData->fRpm);
    }

    UpdateOutData(ptCollData);

    return;
}

/* 收集到的数据脉冲有变化，更新扭矩和转速 */
void CTorqueDlg::MorePointInsertData(COLLECTDATA *ptCollData, BOOL bFinish)
{
    int     i           = 0;
    double  fPreTorque  = 0;    /* 前一次扭矩，PointNum > 1时计算数据 */
    double  fPreRpm     = 0;    /* 前一次转速 */
    double  fInsTorque  = 0;
    double  fInsRpm     = 0;
    BOOL    bInsertOK   = FALSE;

    ASSERT_NULL(ptCollData);

    /* 第一个数据，插入保存的Out数据，记录当前的扭矩和点数 */
    if(m_tCollData.nAllCount == 0)
    {
        if(m_fOutTorque != 0)
        {
            INSERT_DATA(ptCollData->fTorque, ptCollData->fRpm, FALSE);
        }

        UpdateOutData(ptCollData);

        return;
    }

    /*插入数据*/
    fPreTorque = m_tCollData.fTorque[m_tCollData.nAllCount - 1];
    fPreRpm    = m_tCollData.fRpm[m_tCollData.nAllCount - 1];

    for(i=0; i<m_iOutPoints; i++)
    {
        fInsTorque = (m_fOutTorque -fPreTorque)*(i+1)/m_iOutPoints + fPreTorque;
        fInsRpm    = (ptCollData->fRpm-fPreRpm)*(i+1)/m_iOutPoints + fPreRpm;

        INSERT_DATA(fInsTorque, fInsRpm, FALSE);
    }

    UpdateOutData(ptCollData);

    return;
}

void CTorqueDlg::SetTorqDataCfg(TorqData::Torque *ptPBData)
{
    ASSERT_NULL(ptPBData);
    ptPBData->set_btoolbuck(m_bToolBuck);
    ptPBData->set_strmemo(m_ptCfg->strMemo);
    ptPBData->set_strremark(m_ptCfg->strRemark);
    ptPBData->set_dwver(m_ptCtrl->ucVer);

    ptPBData->set_fmaxlimit(m_ptCtrl->fTorqConf[INDEX_TORQ_MAXLIMIT]);
    ptPBData->set_fcontrol(m_ptCtrl->fTorqConf[INDEX_TORQ_CONTROL]);
    ptPBData->set_fopttorq(m_ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL]);
    ptPBData->set_fshow(m_ptCtrl->fTorqConf[INDEX_TORQ_SHOW]);

    ptPBData->set_fmaxcir(m_ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT]);
    ptPBData->set_fuppercir(m_ptCtrl->fTurnConf[INDEX_TURN_UPPERLIMIT]);
    ptPBData->set_fcontrolcir(m_ptCtrl->fTurnConf[INDEX_TURN_CONTROL]);
    ptPBData->set_flowercir(m_ptCtrl->fTurnConf[INDEX_TURN_LOWERLIMIT]);

    ptPBData->set_fmaxrpm(m_ptCtrl->fFullRPM);

    ptPBData->set_fplus(g_tGlbCfg.nPlusPerTurn);
    ptPBData->set_fcut(g_tGlbCfg.fDiscount);
    ptPBData->set_frpmadj(g_tGlbCfg.fRpmAdj);
    ptPBData->set_fmulti(g_tGlbCfg.fMulti);
}

/*扭矩控制完成，LastPoint设置为TRUE，判断控制质量并存盘*/
void CTorqueDlg::FinishSetStatus()
{
    int     i = 0;
    DWORD   dwQuality   = 0;
    WORD    wIPPos      = 0;
    WORD    wIPPlus     = 0;
    int     iCurIPPos   = 0;
    double  fDelCir     = 0;

    COMP_BL(m_tSaveData.ftorque_size(), 1);

    SetTorqDataCfg(&m_tSaveData);

    if(m_iBreakout == 0)
    {
        for(i = m_tCollData.nAllCount - 1; i>0; i--)
        {
            if(m_tCollData.fTorque[i] <= m_tSaveData.ftorque(wIPPos))
            {
                iCurIPPos = i - m_iPriorCnt;
                if (m_tCollData.nCurCount > 0 && iCurIPPos < 0)
                {
                    if(iCurIPPos > -5)
                        iCurIPPos = 1;
                }
                break;
            }
        }
    }
    
    /* 质量判定 */
    dwQuality = theApp.JudgeQuality(&m_tSaveData, m_iBreakout);
    SetQuality(dwQuality);
    m_strQuality    = theApp.GetQualityInfo(&m_tSaveData).c_str();
    if(dwQuality != QUA_RESU_GOOD)
    {
        m_bAlarm = TRUE;
        m_cbAlarm.SetSameIcon(IDI_LEDREDON);
    }

    SetShowPara(&m_tSaveData);
    UpdateData(FALSE);

    return;
}

/* 从collectData获取最后MAXPOINT保存到saveData */
/* 3.20 所有数据都保存，显示时再分屏处理 */
/* 3.22 SaveData所有串口收集的数据都保存，去掉重复项 */
void CTorqueDlg::GetValidTorqData()
{
    int  i          = 0;
    int  iTotalPlus = 0;

    ASSERT_NULL(m_ptPortData);

    m_tSaveData.Clear();
    /* ××××20190916: 只保存最后控制周数的数据，不再保存多屏的数据×××× */
    /* 20200706 从PortData获取保存的数据 */
    for (i = 0; i < (int)m_ptPortData->nSaveCount; i++)
    {
        m_tSaveData.add_ftorque(m_ptPortData->fTorque[i]);
        m_tSaveData.add_frpm(m_ptPortData->fRpm[i]);
        m_tSaveData.add_dwdelplus(m_ptPortData->iDelPlus[i]);
        iTotalPlus += m_ptPortData->iDelPlus[i];
    }
    m_tSaveData.set_dwtotalplus(iTotalPlus);
    return;
}

/*扭矩控制完成，完成画图、优化、数据保存、复位等操作*/
void CTorqueDlg::FinishControl()
{
    /* 20191211 finish时只画结束线，其他线在外面已经画好 */
    m_wndTorque.DrawFinLine();
    m_wndRpm.DrawFinLine();

    //SHOWCIRINFO(m_tCollData.nCurCount, m_tCollData.nAllCount, m_ptCtrl->fMaxCir, m_strCircle);

    /* 从CollectData中获取最后的数据到SaveData */
    GetValidTorqData();

    /* 完成控制处理 */
    FinishSetStatus();

    /* 画拐点  放入 FinishSetStatus 中*/
    // DrawInflectionPoint();

    /* 大于减速扭矩才存盘 */
    if(m_ptComm->fMaxTorq >= m_ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL]*RATIO_OPTSHOULD)
    {
        SaveIntoData(&m_tSaveData);
    }

    /* 设置保护复位时间 */
    /* 20171031 不设置保护复位时间，大于显示扭矩时，开始下一个数据 */
    /* 20171206 保护复位定时器继续保留，单片机上上报的数据会在控制扭矩上保留一段时间 */
    m_hrtReadPort.KillTimer();
    m_hrtGuard.CreateTimer(this, g_tGlbCfg.nResetDur, HRTGuard);

    UpdateOutData(0,0);
    
    Invalidate(TRUE);

    ResetData();
    if(m_bAlarm)
    {
        theApp.PlayAlarmSound();
        m_hrtPlayAlarm.CreateTimer(this, ALRAM_DELAY, HRTPlayAlarm);
    }
    return;
}

void CTorqueDlg::HRTReadPort(CWnd *pUser)
{
    CTorqueDlg *pThis = (CTorqueDlg *)pUser;
    pThis->SendMessage(WM_COLLECTTIMEROUT, 0, 0);
}

void CTorqueDlg::HRTSaveData(CWnd *pUser)
{
    CTorqueDlg *pThis = (CTorqueDlg *)pUser;
    pThis->SendMessage(WM_SAVEDATATIMEROUT, 0, 0);
}

void CTorqueDlg::HRTGuard(CWnd *pUser)
{
    CTorqueDlg *pThis = (CTorqueDlg *)pUser;
    pThis->SendMessage(WM_GUARDTIMEROUT, 0, 0);
}
/*
void CTorqueDlg::HRTGuardRead(CWnd *pUser)
{
    CTorqueDlg *pThis = (CTorqueDlg *)pUser;
    pThis->SendMessage(WM_GUARDREADTIMEROUT, 0, 0);
}
*/
void CTorqueDlg::HRTPortBuff(CWnd *pUser)
{
    CTorqueDlg *pThis = (CTorqueDlg *)pUser;
    pThis->SendMessage(WM_PORTBUFFTIMEROUT, 0, 0);
}

void CTorqueDlg::HRTSaveDebug(CWnd *pUser)
{
    CTorqueDlg *pThis = (CTorqueDlg *)pUser;
    pThis->SendMessage(WM_SAVEDEBUGTIMEROUT, 0, 0);
}

void CTorqueDlg::HRTPlayAlarm(CWnd *pUser)
{
    CTorqueDlg *pThis = (CTorqueDlg *)pUser;
    pThis->SendMessage(WM_ALARMPLAYTIMEROUT, 0, 0);
}

void CTorqueDlg::HRTReadValve(CWnd *pUser)
{
    CTorqueDlg *pThis = (CTorqueDlg *)pUser;
    pThis->SendMessage(WM_READVALVETIMEROUT, 0, 0);
}

/* 正常版本偶数扭矩正常，奇数扭矩为0
   卸扣版本奇数扭矩正常，偶数扭矩为0 
   20200303 : 奇偶不对时，返回-1，转换为UINT为一个非常大的值，先考虑返回为0 
   20200320 : 奇偶不对时，说明反转，返回FALSE，在函数外面设置状态为0xCC，说明反转，数据清零
   */
BOOL CTorqueDlg::ClearExcepTorq(int iOrgTorq, int& iDestTorq)
{
    BOOL  bOdd = FALSE;     // 奇数

    // 无论奇偶，等于0都不画
    /* 20191106 0正常画，通过显示扭矩控制，便于查看一圈脉冲数 */
    //ASSERT_ZERO_R(nOrgTorq, -1);

    bOdd = (iOrgTorq%2 == 1 ? TRUE : FALSE);
    iDestTorq = iOrgTorq; 
    if(bOdd)
        iDestTorq --;

    ASSERT_ZERO_R(iDestTorq, TRUE);

    if(m_iBreakout > 0)
    {
        COMP_BFALSE_R(bOdd, FALSE);
    }
    else
    {
        COMP_BTRUE_R(bOdd, FALSE);
    }

    return TRUE;
}

double CTorqueDlg::OverTorqOpt(double fTorq, BYTE ucStatus)
{
    double fPostTorq = 0;
    double fRandTorq = 0;

    /* 20220310 单片机显示不一致，直接返回原值 */
    return fTorq;

    /* 小于最小扭矩，直接返回 */
    COMP_BLE_R(fTorq, theApp.m_tParaCfg.tCtrl.fTorqConf[INDEX_TORQ_OPTIMAL]*RATIO_LOWERLIMIT, fTorq);

    /* 扭矩超过25%，不作优化 */
    COMP_BG_R(fTorq, theApp.m_tParaCfg.tCtrl.fTorqConf[INDEX_TORQ_OPTIMAL]*RATIO_UPPERLIMIT * 1.25, fTorq);

    /* 控制扭矩大于5000 直接返回 */
    COMP_BG_R(theApp.m_tParaCfg.tCtrl.fTorqConf[INDEX_TORQ_CONTROL], SWITCHLOWLIMIT, fTorq);

    srand((unsigned)time(NULL));
    fPostTorq = fTorq;
    fRandTorq = rand() * (theApp.m_tParaCfg.tCtrl.fTorqConf[INDEX_TORQ_OPTIMAL]*RATIO_UPPERLIMIT/100) / RAND_MAX;

    /* 上扣完成状态 3，4，直接修改为最大扭矩下的值 */
    if (ucStatus >= PLCSTATUS_UNLOAD)
    {
        /* 小于最大扭矩，不处理直接返回，后续的上扣扭矩肯定大于最大扭矩
           即使当前上扣扭矩小于最小扭矩，不是过充，不考虑 */
        COMP_BLE_R(fTorq, theApp.m_tParaCfg.tCtrl.fTorqConf[INDEX_TORQ_OPTIMAL]*RATIO_UPPERLIMIT, fTorq);

        /* 取控制扭矩和上扣扭矩均值，如果小于最大扭矩，则OK */
        fPostTorq = (theApp.m_tParaCfg.tCtrl.fTorqConf[INDEX_TORQ_CONTROL]+ fTorq) / 2;
        COMP_BLE_R(fPostTorq, theApp.m_tParaCfg.tCtrl.fTorqConf[INDEX_TORQ_OPTIMAL]*RATIO_UPPERLIMIT, fPostTorq);

        /* 超过上面的均值，最大扭矩-1%的控制扭矩随机值作为上扣扭矩 */
        fPostTorq = theApp.m_tParaCfg.tCtrl.fTorqConf[INDEX_TORQ_OPTIMAL]*RATIO_UPPERLIMIT - fRandTorq;
        return fPostTorq;
    }

    /* 状态为运行态 0，1，2，保证在最佳扭矩下 */
    /* 20220221 压在最小扭矩==》最佳扭矩下  */
    /* 大于最小扭矩和最佳扭矩均值才处理 */
    COMP_BL_R(fTorq, ((theApp.m_tParaCfg.tCtrl.fTorqConf[INDEX_TORQ_OPTIMAL]*RATIO_LOWERLIMIT+ theApp.m_tParaCfg.tCtrl.fTorqConf[INDEX_TORQ_OPTIMAL])/2), fTorq);

    /* m_fOutTorque 小于最佳扭矩，和当前扭矩取均值，如果小于控制扭矩则OK */
    fPostTorq = (m_fOutTorque + fTorq) / 2;
    COMP_BL_R(fPostTorq, theApp.m_tParaCfg.tCtrl.fTorqConf[INDEX_TORQ_OPTIMAL], fPostTorq);

    /* 超过上面的均值，最佳扭矩-1%的最大扭矩随机值作为当前扭矩, 如果大于前一个扭矩则OK */
    // 1%随机值容易形成横线，暂时不用
    /*fPostTorq = theApp.m_tParaCfg.tCtrl.fControl - fRandTorq;
    COMP_BG_R(fPostTorq, m_fOutTorque, fPostTorq);*/

    /* 取前一个扭矩和最佳扭矩的均值作为当前扭矩 */
    fPostTorq = (m_fOutTorque + theApp.m_tParaCfg.tCtrl.fTorqConf[INDEX_TORQ_OPTIMAL]) / 2;

    return fPostTorq;
}

/* 保存一条串口读取的记录() */
void CTorqueDlg::SavePortNormalInfo(COLLECTDATA *ptCollData) 
{
    int     i           = 0;
    BOOL    bHaveData   = FALSE;
    BYTE    ucSCMRejust = 0;
    BYTE    *pucData    = NULL;
    int     iOrgTorq    = 0;
    int     iTorque     = 0;
    double  fRpm        = 0;
    /* 串口获取的原始有效数据和计算的扭矩等信息 */
    ORGDATA tOrgData    = {0};
    double  fShowTorq   = 0;
    CString strTime;
    SYSTEMTIME ts;

    ASSERT_NULL(ptCollData);

    /* 3.20及之前版本，有效长度12 */
    memcpy(tOrgData.ucRcvByte, &m_ucRcvByte[PORT_DATAPLACE], PORT_DATALEN);
    /* 3.21之后版本，速度后增加2字节表示阀门状态 */
    if(m_wRcvLen >= PORT_VALIDLEN + 2)
    {
        LightValveStatus(&m_ucRcvByte[PORT_DATAPLACE+PORT_DATALEN]);
    }

    /* 计算扭矩和RPM */
    /* 3.17和4.17合一，扭矩需要乘倍率 */
    pucData = &m_ucRcvByte[PORT_DATAPLACE];
    iOrgTorq = (*(pucData) * 256 + *(pucData+1));
    ptCollData->ucStatus    =  (*(pucData+2) & 0xE0) / 32;
    ptCollData->nOrgPlus    = ((*(pucData+2) & 0x1F) * 256 + *(pucData+3)) * 4;
    fRpm                    = *(pucData+5) * 256 + *(pucData+6);  // 200ms 脉冲数
    /* 奇偶处理需要使用源扭矩，因此10倍放到下面相乘 */
    if(!ClearExcepTorq(iOrgTorq, iTorque))
        ptCollData->ucStatus = PLCSTATUS_REVERSE;
    // ptCollData->fTorque = iTorque * (int)theApp.m_nTorqMulti;
    fShowTorq = iTorque * (int)theApp.m_nTorqMulti;
    /* 20220215 处理未打折的数据 ==> 对齐PLC打折处理 */
    ptCollData->fTorque = (int)OverTorqOpt(fShowTorq, ptCollData->ucStatus);

    /* 正常数据保存 */
    tOrgData.ucStatus = ptCollData->ucStatus;
    tOrgData.nTorque = (UINT)ptCollData->fTorque;
    tOrgData.nPlus   = ptCollData->nOrgPlus;

    /* 调试显示接收到的有效数据 */
    GetLocalTime(&ts);
    strTime.Format("%02d:%02d:%02d.%03d",ts.wHour,ts.wMinute, ts.wSecond, ts.wMilliseconds);
    m_strRecvData.Format("%ld,%ld,%d,%.1f(%s)",(int)ptCollData->fTorque,ptCollData->nOrgPlus,ptCollData->ucStatus,fRpm,strTime);

    /* 根据Plus计算数据的点数 */
    CalcPointNum(ptCollData, &tOrgData);

    /* 300编码器，转速太快，除m_fRpmAdj降速度 */
    if(fRpm == 0)
        tOrgData.fRpm = ptCollData->fRpm = m_iOutPoints * m_ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT] * 0.8/g_tGlbCfg.fRpmAdj;
    else
        tOrgData.fRpm = ptCollData->fRpm = fRpm * 5 * 60 / g_tGlbCfg.nPlusPerTurn /2;

    /* 根据脉冲差值计算转速 */
    theApp.SaveOrdData(&tOrgData, m_ucRcvByte, m_wRcvLen);
}

/* 保存一条串口读取的记录() */
/* 0x21 03 00 0x15 速度脉冲H 速度脉冲L 状态位高 状态位低 个数(<=5) 扭矩1 脉冲数1  */
void CTorqueDlg::SavePortMultiDataInfo(COLLECTDATA *ptCollData) 
{
    int     i           = 0;
    BYTE    *pucData    = NULL;
    int     iOrgTorq    = 0;
    int     iTorque     = 0;
    double  fRpm        = 0;
    BYTE    nDataNum    = 0;
    ORGDATA tOrgData    = {0};
    WORD    wCalLen     = 0;
    COLLECTDATA *ptOrgColl = NULL;
    CString strTime;
    SYSTEMTIME ts;

    ASSERT_NULL(ptCollData);
    ptOrgColl = ptCollData;

    LightValveStatus(&m_ucRcvByte[6]);
    
    fRpm = m_ucRcvByte[4] * 256 + m_ucRcvByte[5];  // 200ms 脉冲数

    nDataNum = m_ucRcvByte[8];
    pucData  = &m_ucRcvByte[9];
    wCalLen  = 9 + nDataNum*PORT_GRPDATA;
    if(m_wRcvLen < wCalLen)
        nDataNum = (m_wRcvLen - 9) / PORT_GRPDATA;

    for(i=0; i<nDataNum && i<PORT_MAXDATANUM; i++)
    {
        iOrgTorq = (*(pucData) * 256 + *(pucData+1));
        ptOrgColl->ucStatus =  (*(pucData+2) & 0xE0) / 32;
        ptOrgColl->nOrgPlus = ((*(pucData+2) & 0x1F) * 256 + *(pucData+3)) * 4;
        ptOrgColl->fRpm     = fRpm * 5 * 60 / g_tGlbCfg.nPlusPerTurn/2;
        /* 奇偶处理需要使用源扭矩，因此10倍放到下面相乘 */
        if(!ClearExcepTorq(iOrgTorq, iTorque))
            ptOrgColl->ucStatus = ptCollData->ucStatus = PLCSTATUS_REVERSE;
        ptOrgColl->fTorque  = iTorque * theApp.m_nTorqMulti;
        
        /* 正常数据保存 */
        tOrgData.ucStatus = ptOrgColl->ucStatus;
        tOrgData.nTorque = (UINT)ptOrgColl->fTorque;
        tOrgData.nPlus   = ptOrgColl->nOrgPlus;
        tOrgData.fRpm    = ptOrgColl->fRpm;
        
        /* 根据Plus计算数据的点数 */
        CalcPointNum(ptOrgColl, &tOrgData);

        ptOrgColl++;
        pucData += PORT_GRPDATA;
    }

    /* 调试显示接收到的有效数据 */
    ptOrgColl = &ptCollData[nDataNum-1];
    GetLocalTime(&ts);
    strTime.Format("%02d:%02d:%02d.%03d",ts.wHour,ts.wMinute, ts.wSecond, ts.wMilliseconds);

    m_strRecvData.Format("%ld,%ld,%d,%.1f(%s)",(int)ptOrgColl->fTorque,ptOrgColl->nOrgPlus,ptOrgColl->ucStatus,fRpm,strTime);
    theApp.SaveMultiData(&tOrgData, m_ucRcvByte, m_wRcvLen);
}

LRESULT CTorqueDlg::CollectTimerOut(WPARAM wParam, LPARAM lParam)
{
    BOOL    bCollectOK  = FALSE;
    
    /* 定时获取的数据信息，封装到各个collect函数中设置值 */
    COLLECTDATA tCollData[PORT_MAXDATANUM] = {0};

    /* 获取一个原始数据 */
    bCollectOK = (this->* g_CollectProc[g_tGlbCfg.nTest])(&tCollData[0]);
    COMP_BFALSE_R(bCollectOK, 0);

    /* 测试代码数据在tCollData中，否则在m_ucRcvByte中 */
    if(g_tGlbCfg.nTest >= COLL_RAND)
    {
        RcvTorqDataProc(&tCollData[0]);
    }

    return 0;
}

BOOL CTorqueDlg::CheckPortData(BYTE *pData, int iLen, BOOL &bFini)
{
    int     iNum = 0;
    ASSERT_NULL_R(pData, FALSE);
    ASSERT_ZERO_R(iLen, FALSE);
    
    /* 第一个字节不是PORT485，跳过，等下一条命令 */
    if(PORT485 != pData[0])
    {
        return FALSE;
    }

    if(iLen < 11)
        return TRUE;

    switch(pData[1])
    {
        case 0x10:
        case 0x88:
            if(iLen >= 11)
            {
                iNum = pData[2];
                if(iLen >= (3 + iNum*8))
                    bFini = TRUE;
            }
            return TRUE;
            break;

        case 0x06:
            if( pData[3] != COM_READVALVE &&
                pData[3] != COM_READCALIB )
                return FALSE;

            if(iLen >= PORT_VALIDLEN)
            {
                bFini = TRUE;
            }
            return TRUE;
            break;

        case 0x03:
            if(iLen >= PORT_VALIDLEN)
            {
                switch(pData[3])
                {
                    case COM_READ:
                        if(iLen == PORT_VALIDLEN || iLen >= PORT_VLDVLVLEN)
                            bFini = TRUE;
                        break;

                    case COM_READMULTI:
                        iNum = pData[8];
                        if(iLen == (10 + iNum*4))
                            bFini = TRUE;
                        break;
                    default:
                        return FALSE;
                        break;
                }
            }
            return TRUE;
            break;

        default:
            return FALSE;
            break;
    }

    return FALSE;
}

LONG CTorqueDlg::OnCommunication(WPARAM Char, LPARAM num)
{
    static BYTE PortBuf[200]  = {0};
    static int  iDataLen      = 0;  //数据长度，一次处理一个数据
    BOOL    bDataOK  = FALSE;
    BOOL    bInvalid = FALSE;
    int     iLen = (int)num;

    ASSERT_ZERO_R(iLen, 0);

    if (iDataLen + iLen >= 200)
    {
        iDataLen = 0;
        return 0;
    }
    
    /* 定时获取的数据信息，封装到各个collect函数中设置值 */
    memcpy(&PortBuf[iDataLen], (BYTE*)Char, iLen);
    theApp.SaveCommunication((BYTE*)Char, iLen, DBG_RCVCOM);
    iDataLen += iLen;

    /* 非法数据，清零返回 */
    if(!CheckPortData(&PortBuf[0], iDataLen, bDataOK))
    {
        iDataLen = 0;
        return 0;
    }

    /* 数据没有收全，直接返回 */
    COMP_BFALSE_R(bDataOK, 0);

    m_wRcvLen = iDataLen;
    memcpy(&m_ucRcvByte[0], &PortBuf[0], iDataLen);
    iDataLen = 0;

    switch(PortBuf[1])
    {
        /* collect */
        case 0x10:
        case 0x88:
            ::SendMessage(m_pdlgCollect->m_hWnd, WM_COLLECT_DATA, m_wRcvLen, 0);
            break;

        case 0x03:
            if(m_ucRcvByte[3] == COM_READ || m_ucRcvByte[3] == COM_READMULTI)   // 读取扭矩数据
            {
                RcvTorqDataProc(NULL);
                m_iTorqBreakCnt = m_iMaxReadTimes;
            }
            break;

        case 0x06:
            if (PortBuf[3] == COM_READVALVE)     // 设置阀值
            {
                LightValveStatus(&m_ucRcvByte[4]);
                m_iValveBreakCnt = (int)ceil(MAXCOMMBREAKTIME / READVALVE_LEN);
            }
            if (PortBuf[3] == COM_READCALIB)     // 读取单片机校准信息
            {
                ::SendMessage(m_pdlgCalib->m_hWnd, WM_CALIB_DATA, m_wRcvLen, 0);
            }

            break;

        default:
            break;
    }

    return 0;
}

int CTorqueDlg::RcvTorqDataProc(COLLECTDATA* ptCollData)
{
    int     i = 0;
    BOOL    bHaveS3 = FALSE;
    BOOL    bFinish = FALSE;
    BOOL    bCollectOK = FALSE;
    double  fCurCir = 0;
    UINT    nPriorPlus = 0;
    int     iDelPlus = 0;
    UINT    nDataNum = 0;
    UINT    nSaveNum = 0;
    CString strInfo;

    COLLECTDATA tCollData[PORT_MAXDATANUM] = { 0 };

    ASSERT_NULL_R(m_ptPortData, 0);

    if (ptCollData == NULL)
    {
        /* bRead && bData, 往下处理数据 */
        /* 有效数据的长度应该大于12，有长度为24的数据，也可以尽可能取出一条来，
           但是前12个数据通过CRC校验，则当做普通数据处理，调试开关打开时也将原始数据保存下来 */
        COMP_BFALSE_R(CheckDataValid(), 0);

        if (m_ucRcvByte[3] == COM_READMULTI)
            SavePortMultiDataInfo(&tCollData[0]);  //保存一次串口读取的多条记录
        else    //COM_READ
            SavePortNormalInfo(&tCollData[0]); //保存一条串口读取的记录
    }
    else
    {
        memcpy(&tCollData[0], ptCollData, PORT_MAXDATANUM * sizeof(COLLECTDATA));
    }
    /* 获取数据完成，开始处理数据，画图或者保存 */
    fCurCir = m_tCollData.nAllCount * m_ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT] / MAXLINEITEM;

    for (i = 0; i < PORT_MAXDATANUM; i++)
    {
        if (PLCSTATUS_REVERSE == tCollData[i].ucStatus)
        {
            ReStart();
            m_iShowPlus = tCollData[i].nOrgPlus;
            UpdateOutData(0, 0);
            UpdateData(FALSE);
            return 0;  // no continue;
        }

        if ((tCollData[i].fTorque == 0 && tCollData[i].nOrgPlus == 0) ||
            (tCollData[i].fTorque > OVERFLOWTORQ))
        {
            break;
        }

        /* 将有效数据保存到串口保存结构中 */
        //if(NULL != m_ptPortData)
        {
            /* 脉冲必须递增，或者反转，否则认为是重复 */
            if (m_ptPortData->nLastPlus <= tCollData[i].nOrgPlus ||
                m_ptPortData->nLastPlus - tCollData[i].nOrgPlus >= MAXREVERSEPLUS)  // 重复
            {
                if (m_ptPortData->nLastPlus == 0)
                    iDelPlus = 0;
                else
                    iDelPlus = tCollData[i].nOrgPlus - m_ptPortData->nLastPlus;
                if (iDelPlus >= 0)       // 小于0为重复，不存数据
                {
                    nSaveNum = m_ptPortData->nSaveCount;
                    m_ptPortData->fTorque[nSaveNum] = tCollData[i].fTorque;
                    m_ptPortData->fRpm[nSaveNum] = tCollData[i].fRpm;
                    m_ptPortData->iDelPlus[nSaveNum] = iDelPlus;
                    m_ptPortData->nLastPlus = tCollData[i].nOrgPlus;
                    m_ptPortData->nSaveCount++;
                }
            }
        }
        if (bFinish)
        {
            tCollData[i].ucStatus = PLCSTATUS_REPEAT;
            continue;
        }

        /* 小于显示扭矩，不记录，更新脉冲和图像的点数，不影响下次的画图 */
        /* 如果从大于减速扭矩到小于显示扭矩，也保存数据 */
        if (tCollData[i].fTorque < m_ptCtrl->fTorqConf[INDEX_TORQ_SHOW])
        {
            if (m_iBreakout > 0)
            {
                if (m_ptComm->fMaxTorq > m_ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL]* RATIO_OPTSHOULD && fCurCir >= 0.01)
                {
                    strInfo.Format("< ShowTorq FinishControl by Func(%s) on Line(%d) ", __FUNCTION__, __LINE__);
                    theApp.SaveMessage(strInfo);
                    m_hrtSaveData.KillTimer();
                    FinishControl();
                    return 0;
                }
            }

            ReStart();

            m_iShowPlus = tCollData[i].nOrgPlus;
            UpdateOutData(0, 0);
            UpdateData(FALSE);
            return 0;
        }

        /* 脉冲数翻转，直接清零 */
        nPriorPlus = m_iPriorPlus;
        if (tCollData[i].nOrgPlus <= nPriorPlus && tCollData[i].ucStatus != PLCSTATUS_UNLOAD)
        {
            /* 20200223 两组数据可能重复，脉冲数会比上一组最后一个小，跳过 */
            /* 20200315 避免跳过数据被存盘，设置状态F0，在INSERTDATA时也跳过 */
            /* 20200320 脉冲数小于300，不是停顿，认为数据重复 ;20200809 300->3000, 脉冲锁定10s */
            /* 20201219 3000->3000；现场数据没有画出来，改回300 MAXREVERSEPLUS */
            if (nPriorPlus - tCollData[i].nOrgPlus < MAXREVERSEPLUS)
            {
                tCollData[i].ucStatus = PLCSTATUS_REPEAT;
                continue;
            }

            /* 20200320 脉冲数突然降低，则认为是扭矩仪停顿后单片机脉冲清零 */
            m_iPriorPlus = 0;
            m_iShowPlus = tCollData[i].nOrgPlus;
            /* 20201219 反转时LastPlus得清零，否则脉冲0后面的数据显示不出来 */
            m_ptPortData->nLastPlus = tCollData[i].nOrgPlus;
        }
        if (tCollData[i].nOrgPlus > (UINT)m_iPriorPlus)
            m_iPriorPlus = tCollData[i].nOrgPlus;

        if (m_iBreakout > 0)
        {
            if (!m_hrtSaveData.ValidTimer())
            {
                m_hrtSaveData.CreateTimer(this, g_tGlbCfg.nSaveDur, HRTSaveData);
            }
            if (tCollData[i].ucStatus == PLCSTATUS_BREAKOUT)
            {
                Status254Proc(&tCollData[i], &bFinish);
            }
        }
        else
        {
            /* 状态3卸荷数据的特殊处理 */
            if (tCollData[i].ucStatus == PLCSTATUS_UNLOAD)
            {
                COMP_BFALSE_R(Status3Proc(&tCollData[i], &bFinish), 0);
                bHaveS3 = TRUE;
            }
            /* 状态4非100型抱死，直接完成并显示数据 */
            else if (tCollData[i].ucStatus == PLCSTATUS_LOCKON)
            {
                Status4Proc(&tCollData[i], &bFinish);
                bHaveS3 = TRUE;
            }
            else
                m_nCurDelay = 0;
        }

        /*直接显示扭矩数据20111030*/
        if (tCollData[i].fTorque > m_ptComm->fMaxTorq)
            m_ptComm->fMaxTorq = tCollData[i].fTorque;
    }
    nDataNum = i;
    //ASSERT_ZERO_R(nDataNum, 0);
    if (i >= PORT_MAXDATANUM)
        nDataNum = PORT_MAXDATANUM;

    if (nDataNum > 0)
    {
        /* for 卸扣 */
        if (m_iBreakout > 0)
            m_strTorque.Format("%.0f / %.0f", tCollData[nDataNum - 1].fTorque, m_ptComm->fMaxTorq);
        else if (bHaveS3)
            m_strTorque.Format("%.0f", m_ptComm->fMaxTorq);
        else    /* 显示最后一个有效数据 */
            m_strTorque.Format("%.0f", tCollData[nDataNum - 1].fTorque);
        m_fRpm = tCollData[nDataNum - 1].fRpm;

        for (i = 0; i < (int)nDataNum && i < PORT_MAXDATANUM; i++)
        {
            /* 20200315 避免跳过数据被存盘，设置状态F0，在INSERTDATA时也跳过 */
            if (PLCSTATUS_REPEAT == tCollData[i].ucStatus)
                continue;
            /* 只是扭矩更新，脉冲数没有增加，在现有数据基础上更新扭矩，不插入新数据 */
            if (m_iOutPoints == 0)
            {
                ZeroPointUpdateData(&tCollData[i], bFinish);
            }
            /* 插入数据(包括构造多个数据) */
            else
            {
                MorePointInsertData(&tCollData[i], bFinish);
            }
        }

        /* 计算圈数 */
        //SHOWCIRINFO(m_tCollData.nCurCount, m_tCollData.nAllCount, m_ptCtrl->fMaxCir, m_strCircle);

        /* 图形超过控制周数，自动刷新
           现场有时因为周数设置小，图形超过，满屏周数 没图形 */
        fCurCir = (m_tCollData.nCurCount * m_ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT] / MAXLINEITEM);
        if (fCurCir > m_ptCtrl->fTurnConf[INDEX_TURN_CONTROL] || fCurCir > m_ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT])
        {
            ClearInfo(FALSE);
            ResetLineChart();
            m_iPriorCnt += m_tCollData.nCurCount;
            m_tCollData.nCurCount = 0;
            m_iShowPlus = tCollData[nDataNum - 1].nOrgPlus;
        }

        UpdateData(FALSE);
    }

    /* 控制下降 */
    if (bFinish)
    {
        strInfo.Format("bFinish TRUE FinishControl by Func(%s) on Line(%d) ", __FUNCTION__, __LINE__);
        theApp.SaveMessage(strInfo);
        FinishControl();
        UpdateData(FALSE);
        return 0;
    }

    return 0;
}

/* 控制扭矩或者停止后10s，需要清零数据、清除减速、卸荷灯 */
LRESULT CTorqueDlg::GuardTimerOut(WPARAM wParam, LPARAM lParam)
{
    m_iTest = 0;
    ResetLineChart();
    ClearInfo();

    m_nCur = m_nCurNO + 1;

    if (theApp.HaveTallyNO(&m_tSaveData))
    {
        m_nCurRunningNO++;
    }

    m_strQuality.Empty();
    m_tSaveData.Clear();

    m_strMainValue[MAINSHOWTALLY].Format("%d", m_nCurRunningNO);
    CanModLastData(FALSE);
    Invalidate(TRUE);

    //杀复位定时器
    m_hrtGuard.KillTimer();
    //复位时间定时获取数据
    //m_hrtGuardRead.KillTimer();

    if (m_bRunStatus) /* 如果还在运行 */
    {
        /* 定时器到后，重新打开串口，清除串口数据，以免串口历史数据形成无效扭矩记录 */
        /* 20171028 : 串口多次打开导致指针非法，去掉重新打开串口设置 */
        //m_bComm = ChangeCommParam(FALSE);

        /* 复位定时器到时后，重新打开Timer1收集串口数据 */
        m_hrtReadPort.CreateTimer(this, g_tGlbCfg.nCollectDur, HRTReadPort);
        UpdateOutData(0, 0);
    }

    return 0;
}

/* 读串口前缓冲区有数据等待单片机写定时器Timer7到时，读掉Inque缓冲，然后重新发送消息 */
LRESULT CTorqueDlg::PortBuffTimerOut(WPARAM wParam, LPARAM lParam)
{
#if 0
    BYTE    ucTmpRecv[PORTBUFF];      /* modbus接收消息数值 */
    WORD    wRecvLen = 0;

    /* 杀掉缓冲保护定时器，清空缓冲 */
    m_hrtPortBuff.KillTimer();

    ASSERT_NULL_R(g_lpNewComThread, 0);
    memset(ucTmpRecv, 0, PORTBUFF);

    g_lpNewComThread->ReceiveData_Command(ucTmpRecv, &wRecvLen);

    theApp.SavePortBufData(ucTmpRecv, wRecvLen, m_nClashSta);
    /* 缓冲非空，设置SERIALBUFF_TIMER定时器，定时后再清缓冲 */
    if (m_nClashSta == RS_READCLASH)  /* Read */
    {
        m_nClashERR++;
        CollectTimerOut(0, 0);
        m_hrtReadPort.CreateTimer(this, theApp.m_nColletTime, HRTReadPort);
    }
    else if (m_nClashSta == RS_RESETCLASH)
    {
        /* reset时的冲突不计算Clash，但是打印保存 */
        //ResetTimerOut(0,0);
        //m_hrtGuardRead.CreateTimer(this,theApp.m_nColletTime, HRTGuardRead);
    }
#endif
    return 0;
}

LRESULT CTorqueDlg::SaveDataTimerOut(WPARAM wParam, LPARAM lParam)
{
    CString strInfo;

    strInfo.Format("FinishControl by Func(%s) on Line(%d) ", __FUNCTION__, __LINE__);
    theApp.SaveMessage(strInfo);

    /* 杀掉数据保存定时器 */
    m_hrtSaveData.KillTimer();

    FinishControl();
    return 0;
}

LRESULT CTorqueDlg::SaveDebugTimerOut(WPARAM wParam, LPARAM lParam)
{
    SaveTorqNum();
    theApp.SaveLogInfo();
    return 0;
}

void CTorqueDlg::StopPlayAlam()
{
    m_cbAlarm.SetSameIcon(IDI_LEDREDOFF);
    theApp.StopAlarmSound();
    m_hrtPlayAlarm.KillTimer();
    m_bAlarm = FALSE;
}

LRESULT CTorqueDlg::ALarmPlayTimerOut(WPARAM wParam, LPARAM lParam)
{
    StopPlayAlam();
    return 0;
}

void CTorqueDlg::LightValveStatus(BYTE* pucStatus)
{
    int     i = 0;

    ASSERT_NULL(pucStatus);

    for (i = 0; i < VALVEMAXNUM; i++)
    {
        if (m_bValveStatus[i] == (pucStatus[1] & m_nValveMark[i]))
            continue;

        /* 前面状态OK，变为NOK，点红等 */
        if (m_bValveStatus[i])
            m_cbValveStatus[i].SetSameIcon(IDI_LEDREDON);
        else
            m_cbValveStatus[i].SetSameIcon(IDI_LEDON);

        m_bValveStatus[i] = (pucStatus[1] & m_nValveMark[i]);
    }
}

LRESULT CTorqueDlg::ReadValveTimerOut(WPARAM wParam, LPARAM lParam)
{
    BOOL    bReadRes = FALSE;

    COMP_BGE_R(g_tGlbCfg.nTest, COLL_RAND, 0);
    if (m_iValveBreakCnt > 0)
    {
        SetCommShowInfo(RS_COMM_OPEN);
    }

    bReadRes = SendData(SCMREADVALVE);
    m_iValveBreakCnt--;

    /* 连续5次没有读取到，设置串口为关闭状态 */
    if (m_iValveBreakCnt <= 0)
    {
        SetCommShowInfo(RS_COMM_CLOSE);
        m_bRunStatus = FALSE;
        m_bComm = FALSE;
    }

    if (!bReadRes)
    {
        theApp.SaveSendFailure(SCMREADVALVE);
        return -1;
    }

    return 0;
}

BOOL CTorqueDlg::RunInitPort()
{
    BOOL bRes = FALSE;

    bRes = SendData(SCMCUT);
    Sleep(PORTSENDTIME);

    return TRUE;
}

BOOL CTorqueDlg::RunInitRand()
{
    int     i = 0;
    double  fTorq = 0;
    double  fRpm = 0;
    int     iCtrl = 1;

    m_iTest = 0;
    m_iShowPlus = 0;
    m_iPriorPlus = 0;
    memset(m_fTestTorq, 0, TESTNUM * sizeof(double));
    memset(m_iTestPlus, 0, TESTNUM * sizeof(int));
    srand((unsigned)time(NULL));

    for (i = 1; i < TESTNUM; i++)
    {
        fTorq = rand() * 100.0 / RAND_MAX;
        m_fTestTorq[i] = m_fTestTorq[i - 1] + fTorq * iCtrl;
        if (m_iBreakout > 0)
        {
            if (iCtrl == -1)
            {
                if (m_fTestTorq[i] < m_ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL] * RATIO_OPTSHOULD)
                {
                    m_fTestTorq[i] = m_ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL] * RATIO_OPTSHOULD;
                    m_iTestPlus[i] = m_iTestPlus[i - 1];
                }
            }
            else if (m_fTestTorq[i] > m_ptCtrl->fTorqConf[INDEX_TORQ_CONTROL])
                iCtrl = -1;
        }

        fRpm = rand() * 100.0 / RAND_MAX;
        m_iTestPlus[i] = m_iTestPlus[i - 1] + (int)fRpm;
    }

    return TRUE;
}

BOOL CTorqueDlg::RunIniAutoFile()
{
    CString strFullFile;

    if (m_bAutoFileOpen)
    {
        m_AutoSavefile.Close();
    }
    m_bAutoFileOpen = FALSE;

    //theApp.UpdateAutoSaveFileName();

    strFullFile = theApp.m_strAppPath.c_str();
    strFullFile += theApp.m_strAutoSaveFile.c_str();
    /* 打开的autosavefile名称系统自动保存的文件名称一样，为了避免冲突，打开失败 */
    COMP_BE_R(strFullFile.CompareNoCase(theApp.m_strLogFile.c_str()), 0, FALSE);

    ASSERT_ZERO_R(m_AutoSavefile.Open(strFullFile, CFile::modeRead | CFile::shareDenyNone), FALSE);

    m_bAutoFileOpen = TRUE;
    return TRUE;
}

void CTorqueDlg::RunTorque()
{
    BOOL    bInit = FALSE;
    CString strInfo;

    if (!m_bComm)
    {
        m_bComm = ChangeCommParam();
        if (!m_bComm)
        {
            m_btnRun.SetIconAndText(IDI_RUN, IDS_STRMAINRUN);
            m_btnRun.SetWindowText(theApp.LoadstringFromRes(IDS_STRMAINRUN).c_str());
            m_bRunStatus = FALSE;
            return;
        }
    }

    m_iTorqBreakCnt = m_iMaxReadTimes;

    switch (g_tGlbCfg.nTest)
    {
    case COLL_RAND:
        bInit = RunInitRand();
        break;
    case COLL_TORQUE:
    case COLL_MULTITORQ:
        bInit = RunIniAutoFile();
        break;
    default:
        bInit = RunInitPort();
        break;
    }

    /* 初始化失败，不运行，状态不迁移 */
    if (!bInit)
    {
        m_bRunStatus = FALSE;
        strInfo.Format(IDS_STRINFMAININITFAIL);
        theApp.SaveShowMessage(strInfo);
        return;
    }

    /* 界面设置 */
    StopPlayAlam();

    m_btnRun.SetIconAndText(IDI_STOP, IDS_STRMAINSTOP);
    m_btnRun.SetWindowText(theApp.LoadstringFromRes(IDS_STRMAINSTOP).c_str());

    /* 初始化数据 */
    m_strQuality.Empty();
    ResetData();
    ResetLineChart();

    /* 关键变量初始化 */
    m_strTorque = _T("0");
    m_fRpm = 0;
    m_iRecPNum = 0;
    m_iShowPlus = 0;
    m_iPriorPlus = 0;
    m_strRecvData.Empty();

    /* 设置定时器定时读取串口 */
    m_fPreReadTime = clock() * 1000.0 / CLOCKS_PER_SEC;
    m_hrtReadPort.CreateTimer(this, g_tGlbCfg.nCollectDur, HRTReadPort);
    /* 定时保存CRC和调试信息 */
    m_hrtSaveDebug.CreateTimer(this, AUTOSAVE_TLEN, HRTSaveDebug);
    CanModLastData(FALSE);

    theApp.SaveAppStatus(STATUS_RUN, __FUNCTION__);
    Invalidate(TRUE);
    return;
}

/* 某些异常主动触发画最后一个点，需要更新序号FinishSetStatus(比如停止，重启)
   否则在Guard定时器超时后更新序号 */
void CTorqueDlg::DrawLastPoint()
{
    COLLECTDATA tNullData = { 0 };

    if (m_tCollData.nAllCount != 0)
    {
        /* 20200806 fOutTorque没有画时尝试再画一次 */
        if (m_fOutTorque > m_ptCtrl->fTorqConf[INDEX_TORQ_SHOW])
        {
            tNullData.fRpm = m_tCollData.fRpm[m_tCollData.nAllCount - 1];
            if (m_iOutPoints > 0)
                MorePointInsertData(&tNullData, TRUE);
            else
                ZeroPointUpdateData(&tNullData, TRUE);
            // out清零
            m_fOutTorque = 0;
            m_iOutPoints = 0;
        }
        m_wndTorque.FinishDraw();
        m_wndRpm.FinishDraw();

        /* 从CollectData中获取最后的数据到SaveData */
        GetValidTorqData();
        /* 完成控制处理 */
        FinishSetStatus();

        /* 大于减速扭矩才存盘 */
        if (m_ptComm->fMaxTorq >= m_ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL] * RATIO_OPTSHOULD)
        {
            SaveIntoData(&m_tSaveData);
        }
    }

    /* 数据复位 */
    ResetData();

    if (m_bAlarm)
    {
        theApp.PlayAlarmSound();
        m_hrtPlayAlarm.CreateTimer(this, ALRAM_DELAY, HRTPlayAlarm);
    }
}

void CTorqueDlg::StopTorque()
{
    double  fLastTorque = 0.0;
    //CString strBtnText;

    m_iTest = 0;
    m_iTorqBreakCnt = m_iMaxReadTimes;

    if ((g_tGlbCfg.nTest == COLL_TORQUE ||
        g_tGlbCfg.nTest == COLL_MULTITORQ)
        && m_bAutoFileOpen)
    {
        m_AutoSavefile.Close();
        m_bAutoFileOpen = FALSE;
    }

    KillAllTimer();

    m_btnRun.SetIconAndText(IDI_RUN, IDS_STRMAINRUN);
    m_btnRun.SetWindowText(theApp.LoadstringFromRes(IDS_STRMAINRUN).c_str());
    theApp.SaveAppStatus(STATUS_STOP, __FUNCTION__);

    theApp.SaveLogInfo();

    DrawLastPoint();

    CLOSE_PORTCOMM();

    if (theApp.HaveTallyNO(&m_tSaveData))
    {
        m_nCurRunningNO++;
    }
    m_nCur = m_nCurNO + 1;
    m_strMainValue[MAINSHOWTALLY].Format("%d", m_nCurRunningNO);

    m_tSaveData.Clear();
    m_strQuality.Empty();
    CanModLastData(FALSE);

    m_bComm = FALSE;
    SetCommShowInfo(RS_COMM_CLOSE);

    return;
}

void CTorqueDlg::OnBtrun()
{
    string strInfo;

    JUDGE_REG_STATUS();

    if (g_tGlbCfg.nTest == COLL_HISTORY)
    {
        strInfo = theApp.LoadstringFromRes(IDS_STRINFCOLLHISTEST);
        theApp.SaveShowMessage(strInfo.c_str());
        return;
    }

    m_bRunStatus = !m_bRunStatus;
    m_nClashSta = RS_NORMAL;

    if (m_iRecPNum > COLLECTPOINTS)
    {
        m_bRunStatus = FALSE;
        m_iRecPNum = 0;
    }

    /* Run */
    if (m_bRunStatus)
    {
        StopGetValveStatus();
        RunTorque();
        UpdateData(FALSE);
        return;
    }

    /* else stop */
    StopTorque();
    StartGetValveStatus();
    UpdateData(FALSE);

}

void CTorqueDlg::ResetLineChart()//BOOL bRedraw)
{
    m_wndTorque.RemoveAt();
    m_wndRpm.RemoveAt();

    //m_wndTorque.m_fUpperLimit = m_ptCtrl->fTorqConf[INDEX_TORQ_UPPERLIMIT];     /* 最大扭矩 */
    //m_wndTorque.m_fLowerLimit = m_ptCtrl->fTorqConf[INDEX_TORQ_LOWERLIMIT];     /* 最小扭矩 */
    m_wndTorque.m_fOptTorq = m_ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL];        /* 最佳扭矩 */
    //m_wndTorque.m_fSpeedDown = m_ptCtrl->fTorqConf[INDEX_TORQ_SPEEDDOWN];      /* 减速扭矩 */
    m_wndTorque.m_fShow = m_ptCtrl->fTorqConf[INDEX_TORQ_SHOW];           /* 显示扭矩 */
    //m_wndTorque.m_fBear = m_ptCtrl->fTorqConf[INDEX_TORQ_BEAR];           /* 肩负扭矩 */
    m_wndTorque.m_fControlCir = m_ptCtrl->fTurnConf[INDEX_TURN_CONTROL];     /* 控制周数 */
    m_wndTorque.m_fUpperCir = m_ptCtrl->fTurnConf[INDEX_TURN_UPPERLIMIT];       /* 上限周数 */
    m_wndTorque.m_fLowerCir = m_ptCtrl->fTurnConf[INDEX_TURN_LOWERLIMIT];       /* 下限周数 */
    m_wndTorque.m_fMaxCir = m_ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT];         /* 最大周数 */
    m_wndTorque.m_fWidthCir = m_ptCtrl->fTurnConf[INDEX_TURN_MAXLIMIT];
    m_wndTorque.m_fMaxLimit = m_ptCtrl->fTorqConf[INDEX_TORQ_MAXLIMIT];       /* 最大上限 */
    //m_wndTorque.m_bBear = m_ptComm->bBear;

    m_wndTorque.Add(RGB(255, 255, 255), m_ptCtrl->fTorqConf[INDEX_TORQ_MAXLIMIT], 0.0, LINETYPE_MAIN);
    m_xAxis1.SetTickPara(10, m_wndTorque.m_fMaxCir);
    m_yAxis1.SetTickPara(20, m_ptCtrl->fTorqConf[INDEX_TORQ_MAXLIMIT]);
    m_wndTorque.DrawBkLine();
    m_wndRpm.Add(RGB(255, 255, 255), m_ptCtrl->fFullRPM, 0.0);
    m_xAxis2.SetTickPara(10, m_wndTorque.m_fMaxCir);
    m_yAxis2.SetTickPara(3, m_ptCtrl->fFullRPM);
    m_wndRpm.DrawBkLine();

    UpdateDlgLabel();

    /* 20200320 : 擦除之前的曲线 */
    m_wndTorque.Erase();
    m_wndRpm.Erase();
}

/* 一次给下位机发送所有的命令 */
void CTorqueDlg::SendAllCommand()
{
    COMP_BG(g_tGlbCfg.nTest, 0);

    BeginWaitCursor();

    SendData(SCMSHOW);
    Sleep(PORTSENDTIME);
    SendData(SCMSPEED);
    Sleep(PORTSENDTIME);
    SendData(SCMCONTROL);
    Sleep(PORTSENDTIME);
    SendData(SCMLOWER);
    Sleep(PORTSENDTIME);
    SendData(SCMUPPER);
    Sleep(PORTSENDTIME);
    SendData(SCMCUT);
    Sleep(PORTSENDTIME);

    EndWaitCursor();

    return;
}

void CTorqueDlg::SendValveCommand()
{
    int  i = 0;

    BeginWaitCursor();

    for (i = 0; i < VALVERATIONUM; i++)
    {
        g_ucCurValve = i;
        SendData(SCMVPRESS);
        Sleep(PORTSENDTIME);
    }

    for (i = 0; i < VALVERATIONUM; i++)
    {
        g_ucCurValve = i;
        SendData(SCMVFLOW);
        Sleep(PORTSENDTIME);
    }

    EndWaitCursor();

    return;
}

/* 井名发生变化，数据记录文件名称和数据序号需要变化 */
void CTorqueDlg::ReGetTorqNo()
{
    GetCurNum();
    m_nCur = m_nCurNO + 1;
    m_strMainValue[MAINSHOWTALLY].Format("%d", m_nCurRunningNO);
	
    EnableBreakoutfile(g_tGlbCfg.bBreakOut);
}

void CTorqueDlg::OnSetpara()
{
    CDlgParaSet dlgParaSet;
    PARACFG* ptCurCfg = NULL;
    SHOWCFG* ptCurShow = NULL;

    COMP_BFALSE(JudgeRunStatus(IDS_STRINFRUNNSETPARA));

    dlgParaSet.m_tempCfg = theApp.m_tParaCfg;
    dlgParaSet.m_tempShow = *m_ptShow;
    dlgParaSet.DoModal();
    if (!dlgParaSet.m_bParaChg)
        return;

    ptCurCfg = &dlgParaSet.m_tempCfg;
    ptCurShow = &dlgParaSet.m_tempShow;

    theApp.m_tParaCfg = dlgParaSet.m_tempCfg;
    *m_ptShow = dlgParaSet.m_tempShow;
    /* 井名发生变化，数据记录文件名称和数据序号需要变化 */
    ReGetTorqNo();

    /*if (!bCtrl || !bComm)*/
    {
        ResetLineChart();
        Invalidate(TRUE);
        SendAllCommand();
    }

    /*显示参数*/
    InitMainShowPara();
    theApp.SaveAppStatus(STATUS_SETPARA, __FUNCTION__);

    UpdateData(FALSE);
}

void CTorqueDlg::OnGlbCfg()
{
    CDlgGlbCfg  dlgGlbCfg;

    COMP_BFALSE(JudgeRunStatus(IDS_STRINFRUNNGLBCFG));

    if (IDOK != dlgGlbCfg.DoModal())
        return;
}

void CTorqueDlg::OnBnClickedBtnshowset()
{
    CDlgMainShowSet dlgMain;

    COMP_BFALSE(JudgeRunStatus(IDS_STRINFRUNNSHOWPARA));

    dlgMain.m_tempCfg = *m_ptCfg;
    dlgMain.m_ptShow = m_ptShow;
    if (IDOK != dlgMain.DoModal())
        return;
    theDB.UpdateShowPara(m_ptShow);

    /*显示参数*/
    InitMainShowPara();

    theApp.SaveAppStatus(STATUS_SETPARA, __FUNCTION__);

    UpdateData(FALSE);
}

void CTorqueDlg::OnSetShow()
{
    WORD        i = 0;
    WORD        j = 0;
    WORD        k = 0;
    SHOWCFG     tCurShow;
    CString     strName;
    //BOOL        bFind;
    CDlgShowSet dlgShow;

    COMP_BFALSE(theApp.CheckPassWord());

    dlgShow.m_tempShow = *m_ptShow;
    dlgShow.DoModal();

    if (!dlgShow.m_bModified)
        return;

    /* 井名发生变化，数据记录文件名称和数据序号需要变化 */
    ReGetTorqNo();

    /*显示参数*/
    // dlgShow已经更新数据库
    theApp.SaveAppStatus(STATUS_SETSHOW, __FUNCTION__);

    //销毁当前窗口
    theApp.m_pMainWnd = NULL;
    this->DestroyWindow();

    //重新打开窗口
    theApp.ReOpenWindow();
}

void CTorqueDlg::OnAbout()
{
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
}

void CTorqueDlg::OnDebug()
{
    CDlgFuncTest dlgTest;
    int          iOldIcon[4];

    COMP_BFALSE(JudgeRunStatus(IDS_STRINFRUNNFUNTEST));
    //JUDGE_RUN_STATUS(IDS_STRINFRUNNFUNTEST);

    theApp.SaveAppStatus(STATUS_FUNTEST, __FUNCTION__);

    /* 记录当前状态灯 */
    iOldIcon[0] = m_cbValveStatus[0].m_iIconSame;
    iOldIcon[1] = m_cbValveStatus[1].m_iIconSame;
    iOldIcon[2] = m_cbAlarm.m_iIconSame;
    iOldIcon[3] = m_cbComm.m_iIconSame;

    StopGetValveStatus();
    dlgTest.DoModal();

    /* 恢复测试前状态 */
    TestSpeedLed(iOldIcon[0]);
    TestUnloadLed(iOldIcon[1]);
    TestAlarmLed(iOldIcon[2]);
    TestCommLed(iOldIcon[3]);

    StartGetValveStatus();
}

BOOL CTorqueDlg::DestroyWindow()
{
    int i = 0;

    delete m_ptPortData;
    m_ptPortData = NULL;
    m_strRecvData.Empty();

    KillAllTimer();

    DELETE_DIALOG(m_pdlgCollect);
    DELETE_DIALOG(m_pdlgCalib);

    CLOSE_PORTCOMM();

    theApp.SaveAppStatus(STATUS_EXIT, __FUNCTION__);

    SaveTorqNum();
    theApp.SaveLogInfo();

    if (m_bAutoFileOpen)
    {
        m_AutoSavefile.Close();
    }

    return CDialog::DestroyWindow();
}

void CTorqueDlg::OnAppExit()
{
    CDialog::OnOK();
}

BOOL CTorqueDlg::PreTranslateMessage(MSG* pMsg)
{
    HACCEL hAccel;

    if (WM_KEYFIRST <= pMsg->message &&
        pMsg->message <= WM_KEYLAST)
    {
        hAccel = m_hAccel;
        if (hAccel && ::TranslateAccelerator(m_hWnd, hAccel, pMsg))
            return TRUE;
    }

    return CDialog::PreTranslateMessage(pMsg);
}

BOOL CTorqueDlg::ChangeCommParam(BOOL bUpdateText)
{
    if (g_tGlbCfg.nTest > COLL_PORT)
    {
        if (bUpdateText)
            SetCommShowInfo(g_tGlbCfg.nTest + 1);
        return TRUE;
    }
#if 0
    CLOSE_PORTCOMM();
    g_lpNewComThread = new CNewComThread();
    theApp.SavePortOper(PORTOPR_OPEN);
    if (g_lpNewComThread == NULL)
    {
        SetCommShowInfo(RS_COMM_CLOSE);
        return FALSE;
    }

    if ( g_lpNewComThread->InitComm(tPort.ucPortNo,
                                    tPort.nBand,
                                    tPort.ucParity,
                                    tPort.ucDataBit,
                                    tPort.ucStopBit))
    {
        g_lpNewComThread->Set_TimeInterval(m_nInterval);

        SetCommShowInfo(RS_COMM_OPEN);

        return TRUE;
    }
#else
    CLOSE_PORTCOMM();
    if (m_tPort.InitPort(this, g_tGlbCfg.nPortNO, g_tGlbCfg.nBaudRate, 'N', 8, 1, EV_RXCHAR | EV_CTS, 512))
    {
        m_tPort.StartMonitoring();
        theApp.SavePortOper(PORTOPR_OPEN);

        SetCommShowInfo(RS_COMM_OPEN);

        return TRUE;
    }
#endif

    /* else */
    theApp.SaveMessage("OpenPort Failure!!");
    SetCommShowInfo(RS_COMM_CLOSE);
    return FALSE;
}

BOOL CTorqueDlg::CompSysPara(const BYTE* pPara1, const BYTE* pPara2, WORD wSize)
{
    ASSERT_NULL_R(pPara1, FALSE);
    ASSERT_NULL_R(pPara2, FALSE);

    COMP_BNE_R(memcmp((void*)pPara1, (void*)pPara2, wSize), 0, FALSE);

    return TRUE;
}

/* 读取数据 */
void CTorqueDlg::FillRead(void)
{
    FillReadCommand(COM_READ);
}

/* 读取多组数据 */
void CTorqueDlg::FillReadMulti(void)
{
    FillReadCommand(COM_READMULTI);
}

/* 校准倍数 */
void CTorqueDlg::FillMulti()
{
    FillWriteCommand(COM_ADJUST);

    /*校准倍数值*/
    FillWordValue(&MODBUS_CONTENT, (int)(g_tGlbCfg.fMulti * 100));
}

/* 去皮 */
void CTorqueDlg::FillQipi()
{
    FillWriteCommand(COM_QIPI);
}

/*收集数据*/
void CTorqueDlg::FillCollect()
{
    FillWriteCommand(COM_COLLECT);

    MODBUS_CONTENT = 0x00;
}

/*收到有效数据再次采集数据：最后一次确认后不需要再次收到数据*/
void CTorqueDlg::FillCollectOK()
{
    FillWriteCommand(COM_COLLECT);

    MODBUS_CONTENT = 0x01;
}

/*收到无效数据再次采集数据*/
void CTorqueDlg::FillCollectNOK()
{
    FillWriteCommand(COM_COLLECT);

    MODBUS_CONTENT = 0x02;
}

/* 显示扭矩 */
void CTorqueDlg::FillShow()
{
    FillWriteCommand(COM_SHOW);

    /* 3.17和4.17合一，扭矩需要乘倍率 */
    FillWordValue(&MODBUS_CONTENT, (int)(m_ptCtrl->fTorqConf[INDEX_TORQ_SHOW] / theApp.m_nTorqMulti));
}

/* 减速扭矩 */
void CTorqueDlg::FillSpeed()
{
    FillWriteCommand(COM_SPEED);

    /* 3.17和4.17合一，扭矩需要乘倍率 */
    FillWordValue(&MODBUS_CONTENT, (int)(m_ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL] * RATIO_OPTSHOULD / theApp.m_nTorqMulti));
}

/* 控制扭矩 */
void CTorqueDlg::FillControl()
{
    FillWriteCommand(COM_CONTROL);

    /* 3.17和4.17合一，扭矩需要乘倍率 */
    FillWordValue(&MODBUS_CONTENT, (int)(m_ptCtrl->fTorqConf[INDEX_TORQ_CONTROL] / theApp.m_nTorqMulti));
}

/* 开始减速 */
void CTorqueDlg::FillBSpeed()
{
    FillWriteCommand(COM_BSPEED);
}

/* 停止减速 */
void CTorqueDlg::FillESpeed()
{
    FillWriteCommand(COM_ESPEED);
}

/* 开始卸荷 */
void CTorqueDlg::FillBUnload()
{
    FillWriteCommand(COM_BUNLOAD);
}

/* 停止卸荷 */
void CTorqueDlg::FillEUnload()
{
    FillWriteCommand(COM_EUNLOAD);
}

/* 最大扭矩 */
void CTorqueDlg::FillUpper()
{
    FillWriteCommand(COM_UPPER);

    /* 3.17和4.17合一，扭矩需要乘倍率 */
    FillWordValue(&MODBUS_CONTENT, (int)(m_ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL] * RATIO_UPPERLIMIT / theApp.m_nTorqMulti));
}

/* 最小扭矩 */
void CTorqueDlg::FillLower()
{
    FillWriteCommand(COM_LOWER);

    /* 3.17和4.17合一，扭矩需要乘倍率 */
    FillWordValue(&MODBUS_CONTENT, (int)(m_ptCtrl->fTorqConf[INDEX_TORQ_OPTIMAL] * RATIO_LOWERLIMIT / theApp.m_nTorqMulti));
}

/* 打折系数 */
void CTorqueDlg::FillCut()
{
    FillWriteCommand(COM_CUT);

    FillWordValue(&MODBUS_CONTENT, (int)(g_tGlbCfg.fDiscount * 100));   /*打折值*/
}

/*设置时间*/
void CTorqueDlg::FillTime()
{
    BYTE* pPlace = NULL;

    FillWriteCommand(COM_TIME);

    /* 年*12+月 */
    pPlace = &MODBUS_CONTENT;
    *pPlace++ = (m_tSetTime.GetYear() - 2000) * 12 + m_tSetTime.GetMonth();
    *pPlace++ = m_tSetTime.GetDay();
    *pPlace++ = m_tSetTime.GetHour();
    *pPlace++ = m_tSetTime.GetMinute();
}

/* 设置减压阀参数 */
void CTorqueDlg::FillValvePress()
{
    BYTE* pPlace = NULL;

    FillWriteCommand(COM_VPRESS);

    COMP_BGE(g_ucCurValve, VALVERATIONUM);

    /*  组数1~3
        扭矩的百分比
        压力阀值
        压力阀值
    */
    pPlace = &MODBUS_CONTENT;
    *pPlace++ = (g_ucCurValve + 1);
    *pPlace++ = theApp.m_tValveCfg.ucTorq[0][g_ucCurValve];
    *pPlace++ = theApp.m_tValveCfg.ucRatio[0][g_ucCurValve];
    *pPlace++ = theApp.m_tValveCfg.ucRatio[0][g_ucCurValve];
}

/* 设置流量阀参数 */
void CTorqueDlg::FillValveFlow()
{
    BYTE* pPlace = NULL;

    FillWriteCommand(COM_VFLOW);

    COMP_BGE(g_ucCurValve, VALVERATIONUM);

    /*  组数1~3
        扭矩的百分比
        流量阀值
        流量阀值
    */
    pPlace = &MODBUS_CONTENT;
    *pPlace++ = (g_ucCurValve + 1);
    *pPlace++ = theApp.m_tValveCfg.ucTorq[1][g_ucCurValve];
    *pPlace++ = theApp.m_tValveCfg.ucRatio[1][g_ucCurValve];
    *pPlace++ = theApp.m_tValveCfg.ucRatio[1][g_ucCurValve];
}

/* 读取阀门状态参数 */
void CTorqueDlg::FillReadValve()
{
    BYTE* pPlace = NULL;

    FillWriteCommand(COM_READVALVE);
}


/* 发送写分段校准指令 */
void CTorqueDlg::FillWriteCalib()
{
    BYTE* pPlace = NULL;

    FillWriteCommand(COM_WRITECALIB);

    pPlace = &MODBUS_CONTENT;
    *pPlace++ = theApp.m_tCalibCtrl.ucType;
    *pPlace++ = theApp.m_tCalibCtrl.tInfo.ucSegNO;

    FillWordValue(pPlace, theApp.m_tCalibCtrl.tInfo.iCalibTorq / theApp.m_nTorqMulti);
    pPlace += 3;

    FillWordValue(pPlace, theApp.m_tCalibCtrl.tInfo.iSCMTorq / theApp.m_nTorqMulti);
}

/* 发送控制分段校准指令 */
void CTorqueDlg::FillCtrlCalib()
{
    BYTE* pPlace = NULL;

    FillWriteCommand(COM_CTRLCALIB);

    pPlace = &MODBUS_CONTENT;
    *pPlace++ = theApp.m_tCalibCtrl.ucProc;
    *pPlace++ = theApp.m_tCalibCtrl.ucType;
}

/* 发送读分段校准指令 */
void CTorqueDlg::FillReadCalib()
{
    BYTE* pPlace = NULL;

    FillWriteCommand(COM_READCALIB);

    pPlace = &MODBUS_CONTENT;
    *pPlace++ = theApp.m_tCalibCtrl.ucType;
    *pPlace++ = theApp.m_tCalibCtrl.tInfo.ucSegNO;
}

/*           单片机控制数据
    #define SCMREAD         15  *读取数据*
    #define SCMADJUST       16  *校准*
    #define SCMZERO         17  *去皮*
    #define SCMCOLLECT      18  *采集数据*
    #define SCMSHOW         19  *显示扭矩*
    #define SCMSPEED        20  *减速扭矩*
    #define SCMCONTROL      21  *控制扭矩*
    #define SCMBSPEED       22  *开始减速*
    #define SCMESPEED       23  *停止减速*
    #define SCMBUNLOAD      24  *开始卸荷*
    #define SCMEUNLOAD      25  *停止卸荷*
    #define SCMUPPER        26  *上限扭矩*
    #define SCMLOWER        27  *下限扭矩*
    #define SCMCUT          28  *打折系数*
    #define SCMCOLLECTOK    29  *收到有效数据再次采集数据：最后一次确认后不需要再次收到数据*
    #define SCMCOLLECTNOK   30  *收到无效数据再次采集数据*
    #define SCMTIME         31  *设置单片机时间*
    #define SCMVPRESS       32  * 减压阀 *
    #define SCMVFLOW        33  * 流量阀 *
    #define SCMREADVALVE    34  * 读取阀门状态 *
    #define SCMREADMULTI    35  * 读取多组数据 *
    #define SCMWRITECALIB   36  * 写分段校准 *
    #define SCMCALIBCTRL    37  * 分段校准控制
    #define SCMREADCALIB    38  * 读分段校准 **/
    // #define SCMVZOOM        34  * 放大倍数 *
WORD CTorqueDlg::FillSendByte(UINT nParaType)
{
    int     i = 0;
    int     iSCMRejust = 0;
    WORD    wCrcvalue = 0;
    WORD    wLen = 8;    /* 初始命令长度 */

    /* 设置各个命令的参数内容 */
    (this->*g_FillByteProc[nParaType - SCMREAD])();

    /*校验位*/
    for (i = 0; i < wLen; i++)
        iSCMRejust += m_ucSndByte[i];
    m_ucSndByte[wLen++] = (BYTE)iSCMRejust;

    /* SCMWRITECALIB 8/9字节为 A/D扭矩 */
    if (nParaType == SCMWRITECALIB)
        wLen += 2;
    else
    {
        wCrcvalue = GetCRCValue(m_ucSndByte, wLen);
        m_ucSndByte[wLen++] = (BYTE)(wCrcvalue / 256);
        m_ucSndByte[wLen++] = (BYTE)(wCrcvalue % 256);
    }

    /*           单片机控制数据                   */
    /*结束位*/
    m_ucSndByte[wLen++] = 0x08;

    return wLen;
}

/*
扭矩送入                40001
脉冲送入（低16位）送入  40002
脉冲送入（高16位）送入  40003
扭矩的量程              40004
扭矩的设定值            40005

上位机修改数据
写状态指示位            40007        写操作的时候，请置1，
写扭矩量程              40008
写设定扭矩              40009
写扭矩校准              40010        减速扭矩
开机信号                40011
100/26*1200
*/
BOOL CTorqueDlg::SendData(UINT nParaType)
{
    int     wLen = 0;
    //string  strCmd;

    COMP_BG_R(g_tGlbCfg.nTest, 0, FALSE);

    /* 串口没有打开 */
    if (!m_bComm)
    {
        m_bComm = ChangeCommParam();
    }
    COMP_BFALSE_R(m_bComm, FALSE);
    COMP_BL_R(nParaType, SCMREAD, FALSE);

    memset(m_ucSndByte, 0, PORTBUFF);
    memset(m_ucRcvByte, 0, PORTBUFF);

    wLen = FillSendByte(nParaType);
    //strCmd = g_strCmdName[nParaType-SCMREAD];
#if 0
    ASSERT_NULL_R(g_lpNewComThread, FALSE);

    if (g_lpNewComThread->SendData_Command(m_ucSndByte, wLen))
    {
        /* READ命令太多，不显示，只显示其他命令 */
        if (nParaType != SCMREAD &&
            nParaType != SCMREADVALVE &&
            nParaType != SCMREADMULTI)
            theApp.SaveSendData(strCmd, m_ucSndByte, wLen);
        /*Sleep(len*20);*/  /*发送函数里面已经有len*5+20ms的延时*/
        return g_lpNewComThread->ReceiveData_Command(m_ucRcvByte, &m_wRcvLen);
    }
#else
    COMP_BFALSE_R(m_bComm, FALSE);
    m_tPort.WriteToPort((char*)m_ucSndByte, wLen);
    theApp.SaveCommunication(m_ucSndByte, wLen, DBG_SNDCMD);
#endif
    return TRUE;
}

//---------------------------------------------------------------
//HH 05-04-22 本节[CRC16]校验方法摘录自 [ModBus 通讯协议]
//---------------------------------------------------------------
WORD CTorqueDlg::GetCRCValue(BYTE* pucCrc, WORD iLen)
{
    BYTE ucHigh = 0xFF;         /* 高CRC字节初始化  */
    BYTE ucLow = 0xFF;         /* 低CRC 字节初始化 */
    int  iIndex = 0;            /* CRC循环中的索引  */

    ASSERT_NULL_R(pucCrc, 0xFFFF);

    while (iLen--)                    /* 传输消息缓冲区   */
    {
        iIndex = ucHigh ^ *pucCrc++;  /* 计算CRC */
        ucHigh = ucLow ^ g_iModBusHi[iIndex];
        ucLow = g_iModBusLow[iIndex];
    }

    return (ucHigh << 8 | ucLow);
}

void CTorqueDlg::OnZero()
{
#if 0
    CDlgZero    dlgZero;

    dlgZero.m_fZero = m_ptParaCfg->iZero / 100.0;
    COMP_BNE(dlgZero.DoModal(), IDOK);

    if (m_ptParaCfg->iZero / 100.0 != dlgZero.m_fZero)
    {
        m_ptParaCfg->iZero = int(dlgZero.m_fZero * 100);
        theApp.WritePara(theApp.m_strParaFile, m_ptParaCfg);
    }
#endif
}

void CTorqueDlg::TestSpeedLed(int iLedIcon)
{
    COMP_BE(iLedIcon, -1);
    m_cbValveStatus[0].SetSameIcon(iLedIcon);
    UpdateData(FALSE);
}

void CTorqueDlg::TestUnloadLed(int iLedIcon)
{
    COMP_BE(iLedIcon, -1);
    m_cbValveStatus[1].SetSameIcon(iLedIcon);
    UpdateData(FALSE);
}

void CTorqueDlg::TestAlarmLed(int iLedIcon)
{
    COMP_BE(iLedIcon, -1);
    m_cbAlarm.SetSameIcon(iLedIcon);
    UpdateData(FALSE);
}

void CTorqueDlg::TestCommLed(int iLedIcon)
{
    COMP_BE(iLedIcon, -1);
    m_cbComm.SetSameIcon(iLedIcon);
    UpdateData(FALSE);
}

void CTorqueDlg::OnScm()
{
    CDlgScm dlgScm;

    COMP_BFALSE(JudgeRunStatus(IDS_STRINFRUNNMODSCM));
    //JUDGE_RUN_STATUS(IDS_STRINFRUNNMODSCM);

    dlgScm.DoModal();
}

void CTorqueDlg::ClearInfo(BOOL bClrTorq)
{
    m_iRecPNum = 0;
    if (bClrTorq)
    {
        m_strTorque = _T("0");
        //m_strCircle = _T("0");
    }
    m_fRpm = 0;
}

void CTorqueDlg::ReStart()
{
    ClearInfo();
    ResetLineChart();
    ResetData();

    m_strQuality.Empty();
    m_tSaveData.Clear();
}

void CTorqueDlg::OnHistorylist()
{
    CDlgHisList     dlgList;
    CDlgHisGrp      dlgGrp;
    CDlgHisSumup    dlgSum;
    CMySheet        sheet(theApp.LoadstringFromRes(IDS_STRMAINVIEWHIS).c_str(), this);

    COMP_BFALSE(JudgeRunStatus(IDS_STRINFRUNNVIEWHIS));

    theApp.SaveAppStatus(STATUS_HISTORY, __FUNCTION__);

    theApp.m_fSheetRatio = 1.0;

    sheet.AddPage(&dlgList);
    sheet.AddPage(&dlgGrp);
    sheet.AddPage(&dlgSum);
    sheet.DoModal();

    /* 20200302: 重新取下入井序号信息，避免修改质量属性后，入井序号不正确 */
    ReGetTorqNo();
    UpdateData(FALSE);
}

void CTorqueDlg::OnHistorystat()
{
    CDlgDataStat     dlgDataStat;

    COMP_BFALSE(JudgeRunStatus(IDS_STRINFRUNNVIEWHIS));

    COMP_BFALSE(theApp.CheckPassWord());

    theApp.SaveAppStatus(STATUS_HISSTAT, __FUNCTION__);

    dlgDataStat.DoModal();
}

void CTorqueDlg::OnShowtest()
{
    theApp.m_bShowCRC = !theApp.m_bShowCRC;

    SHOW_MAINDLG_BUTTON(theApp.m_bShowCRC);

    UpdateData(FALSE);
}

void CTorqueDlg::OnCollectdata()
{
    COMP_BFALSE(JudgeRunStatus(IDS_STRINFRUNNCOLLHIS));

    m_pdlgCollect = (CDlgCollect*)GetDlgPoint(m_pdlgCollect,
        DLG_COLLECTDATA,
        IDD_DLGCOLLECT);
    ASSERT_NULL(m_pdlgCollect);

    StopGetValveStatus();

    m_pdlgCollect->ShowWindow(SW_SHOW);
}

/*
插入点数目，以及百分比处理
最多插入4个点，大于20%小于40插入1个点，插入位置为1/3位置。以此类推。
最后一个位置要大一点，因为控制时最后冲高，扭矩大一些更加合理
插入点的百分比为 1/(n+2),2/(n+2),...,n/(n+2),
1：1/3              CTRL:2/3
2: 1/4 1/4          CTRL:1/2
3: 1/5 1/5 1/5      CTRL:2/5
4: 1/6 1/6 1/6 1/6  CTRL:1/3
*/
int  CTorqueDlg::GetInsertNum(double fT1, double fT2)
{
    double  fYu = 0;
    double  fLevel[5] = { 0,1.2,1.4,1.8,2.2 };
    int     i = 0;

    ASSERT_ZERO_R(fT2, 0);

    fYu = fT1 / fT2;
    for (i = 4; i > 0; i--)
    {
        COMP_BGE_R(fYu, fLevel[i], i);
    }

    return 0;
}

void CTorqueDlg::UpdateOutData(double fTorque, int iPoints)
{
    m_fOutTorque = fTorque;
    m_iOutPoints = iPoints;
    return;
}

void CTorqueDlg::UpdateOutData(COLLECTDATA* ptColl)
{
    ASSERT_NULL(ptColl);

    m_fOutTorque = ptColl->fTorque;
    m_iOutPoints = ptColl->iPointNum;
    return;
}

/* 串口读取CRC错误时，停止尝试保存数据，重启串口 */
void CTorqueDlg::RestartComm()
{
    double  fLastTorque = 0.0;
    double  fCircle = 0.0;

    m_iTest = 0;

    KillAllTimer();

    DrawLastPoint();

    theApp.SaveAppStatus(STATUS_RESTART, __FUNCTION__);

    COMP_BFALSE(m_bRunStatus);

    /* if(m_bRunStatus) *//* 如果还在运行 */
    /* 定时器到后，重新打开串口，清除串口数据，以免串口历史数据形成无效扭矩记录 */
    m_bComm = ChangeCommParam();

    /* 复位定时器到时后，重新打开Timer1收集串口数据 */
    m_hrtReadPort.CreateTimer(this, g_tGlbCfg.nCollectDur, HRTReadPort);

    return;
}

/* 设置串口相关图标和信息显示状态
    RS_COMM_CLOSE   0    串口关闭
    RS_COMM_OPEN    1    串口打开
    RS_COMM_RAND    2    串口模拟打开，随机数
    RS_COMM_TORQUE  3    串口模拟打开，读扭矩历史数据
    RS_COMM_HISTORY 4    串口模拟打开，读收集单片机历史数据
*/
void CTorqueDlg::SetCommShowInfo(UINT nStatus)
{
    string  strShow;
    string  strBtnText;

    if (nStatus == RS_COMM_CLOSE)
    {
        m_cbComm.SetSameIcon(IDI_LEDREDON);
        m_CommStatus.SetIcon(AfxGetApp()->LoadIcon(IDI_MODBUSCLOSE));
        strBtnText = theApp.LoadstringFromRes(IDS_STRMAINOPENPORT);
        strShow = theApp.LoadstringFromRes(IDS_STRMAINPORTCLOSE, (int)g_tGlbCfg.nPortNO);
        m_StatusBar.SetPaneText(0, strShow.c_str(), TRUE);
        return;
    }

    m_cbComm.SetSameIcon(IDI_LEDON);
    m_CommStatus.SetIcon(AfxGetApp()->LoadIcon(IDI_MODBUSOPEN));
    strBtnText = theApp.LoadstringFromRes(IDS_STRMAINCLOSEPORT);

    if (nStatus == RS_COMM_OPEN)
    {
        strShow = theApp.LoadstringFromRes(IDS_STRMAINPORTOPEN, (int)g_tGlbCfg.nPortNO);
        /*strShow.Format( IDS_STRMAINPORTOPEN,
                        g_tGlbCfg.nPortNO,
                        115200,
                        8,
                        "None",
                        1);*/
    }
    else
    {
        strShow = theApp.LoadstringFromRes((IDS_STRMAINPORTSIMRAND + nStatus - RS_COMM_RAND), (int)g_tGlbCfg.nPortNO);
    }

    m_StatusBar.SetPaneText(0, strShow.c_str(), TRUE);

    return;
}

void CTorqueDlg::ResetData()
{
    m_tCollData.nAllCount = 0;
    m_tCollData.nCurCount = 0;
    m_tCollData.dwQuality = 0;
    m_ptComm->fMaxTorq = 0;

    m_iShowPlus = 0;
    m_iPriorPlus = 0;
    m_iPriorCnt = 0;
    _time64(&m_tStartTime);

    if (NULL != m_ptPortData)
    {
        m_ptPortData->nSaveCount = 0;
        m_ptPortData->nLastPlus = 0;
    }
}

BOOL CTorqueDlg::InsertData(COLLECTTORQUE* ptColl, double torque, double rpm)
{
    UINT     i = 0;

    ASSERT_NULL_R(ptColl, FALSE);

    /* 数据大于2500时，去掉前面500个数据 */
    if (ptColl->nAllCount >= COLLECTPOINTS)
    {
        for (i = 0; i < ptColl->nAllCount - MAXLINEITEM; i++)
        {
            ptColl->fTorque[i] = ptColl->fTorque[i + MAXLINEITEM];
            ptColl->fRpm[i] = ptColl->fRpm[i + MAXLINEITEM];
        }

        for (; i < ptColl->nAllCount; i++)
        {
            ptColl->fTorque[i] = 0;
            ptColl->fRpm[i] = 0;
        }
        ptColl->nAllCount -= MAXLINEITEM;
        return FALSE;
    }
    if (ptColl->nCurCount >= MAXLINEITEM)
    {
        m_iPriorCnt += m_tCollData.nCurCount;
        ptColl->nCurCount = 0;
    }

    ptColl->fTorque[ptColl->nAllCount] = torque;
    ptColl->fRpm[ptColl->nAllCount] = rpm;
    ptColl->nAllCount++;
    ptColl->nCurCount++;

    return TRUE;
}

void CTorqueDlg::UpdateTorqueData(double torque, double rpm)
{
    COMP_BL(m_tCollData.nAllCount, 1);
    COMP_BG(m_tCollData.nAllCount, COLLECTPOINTS);

    m_tCollData.fTorque[m_tCollData.nAllCount - 1] = torque;
    m_tCollData.fRpm[m_tCollData.nAllCount - 1] = rpm;

    return;
}

/* 将当前扭矩数据添加到扭矩结构文件中 */
void CTorqueDlg::SaveIntoData(TorqData::Torque* ptPBData)
{
    /* 空文件，扭矩序号为1 */
    UINT    nTorqNum = 1;
    size_t  nDataLen = 0;
    int     i = 0;
    UINT    nLeng = 0;    /* 数据的长度 */
    CString strFileName;
    CString strTemp;
    CFile   file;
    __time64_t curTime;
    char* pcBuff = NULL;
    double  duration;

    ASSERT_NULL(ptPBData);

    _time64(&curTime);
    ptPBData->set_coltime(curTime);
    ptPBData->set_dwseqno(m_nCur);
    ptPBData->set_fmaxtorq(m_ptComm->fMaxTorq);
    ptPBData->set_bbreakout(m_iBreakout > 0);
    ptPBData->set_dwtorqunit(g_tGlbCfg.nTorqUnit);
    duration = _difftime64(curTime, m_tStartTime);
    ptPBData->set_fmakeupdur(_difftime64(curTime, m_tStartTime));

    nDataLen = ptPBData->ByteSizeLong();
    ASSERT_ZERO(nDataLen);
    COMP_BGE(nDataLen, MAXPROBUFF);
    pcBuff = new char[nDataLen];
    ASSERT_NULL(pcBuff);
    memset(pcBuff, 0, nDataLen);
    if (!ptPBData->SerializeToArray(pcBuff, nDataLen))
    {
        delete pcBuff;
        return;
    }

    file.Open(theApp.m_strDataFile.c_str(), CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone, NULL);

    if (file.GetLength() != 0)
    {
        file.Read(&nTorqNum, sizeof(UINT));
        nTorqNum++;
    }

    /* 扭矩当前序号+1 */
    m_ptComm->dwSeqNo = m_nCur;

    IncTorqNo();
    m_nTotal = m_dwTotalTorqNum;

    /* 更新文件数据数目 */
    file.SeekToBegin();
    file.Write(&nTorqNum, sizeof(UINT));
    // 卸扣
    nTorqNum = 0;
    file.Write(&nTorqNum, sizeof(UINT));

    /* 跳过之前记录 */
    //SEEK_TORQUE(((int)nTorqNum-1), nLeng);
    file.SeekToEnd();

    // 最新版本都添加头文件
    file.Write(&theApp.m_nPBHead, PBHEADLEN);
    file.Write(&nDataLen, sizeof(UINT));
    file.Write(pcBuff, nDataLen);
    file.Close();

    delete pcBuff;

    CanModLastData(TRUE);
    return;
}

void CTorqueDlg::SetShowPara(TorqData::Torque* ptPBData)
{
    UINT    i = 0;
    TorqData::ShowInfo* pbShow = NULL;
    string  strName, strVal;
    string  strTallyName = theApp.GetMainShowName(m_ptShow, MAINSHOWTALLY);

    ASSERT_NULL(ptPBData);

    for (i = 0; i < m_ptShow->nParaNum && i < MAXPARANUM; i++)
    {
        pbShow = ptPBData->add_tshow();
        strName = m_ptShow->strShow[i];
        pbShow->set_strname(strName);
        strVal = m_ptCfg->strValue[i];

        if (strTallyName == strName)
        {
            if (theApp.HaveTallyNO(ptPBData))
                strVal = to_string(m_nCurRunningNO);
            else
                strVal = "";
        }
        pbShow->set_strvalue(strVal);
    }
}

void CTorqueDlg::SetQuality(DWORD dwQuality)
{
    m_tSaveData.set_dwquality(dwQuality);
}

void CTorqueDlg::KillAllTimer()
{
    m_hrtReadPort.KillTimer();
    m_hrtGuard.KillTimer();
    m_hrtPortBuff.KillTimer();
    m_hrtSaveData.KillTimer();
    m_hrtSaveDebug.KillTimer();
    m_hrtReadValve.KillTimer();
}

/* 填写WORD16到字节中 */
void CTorqueDlg::FillWordValue(BYTE* pAddr, int iValue)
{
    ASSERT_NULL(pAddr);

    *(pAddr) = (BYTE)(((iValue) & 0xFF00) >> 8);
    *(pAddr + 1) = (BYTE)((iValue) & 0x00FF);
}

/* 填写MODBUS读命令 */
void CTorqueDlg::FillReadCommand(int iCommand)
{
    m_ucSndByte[0] = PORT485;   /* 设备号 */
    m_ucSndByte[1] = BUS_READ;  /* 功能码 读命令 */
    FillWordValue(&m_ucSndByte[2], iCommand);
}

void CTorqueDlg::FillWriteCommand(int iCommand)
{
    m_ucSndByte[0] = PORT485;   /* 设备号 */
    m_ucSndByte[1] = BUS_WRITE; /* 功能码 写命令 */
    FillWordValue(&m_ucSndByte[2], iCommand);
}

void CTorqueDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
    CCmdUI  state;
    HMENU   hParentMenu;

    ASSERT(pPopupMenu != NULL);
    ASSERT_NULL(pPopupMenu)
    // Check the enabled state of various menu items.

    state.m_pMenu = pPopupMenu;
    ASSERT(state.m_pOther == NULL);
    ASSERT(state.m_pParentMenu == NULL);

    // Determine if menu is popup in top-level menu and set m_pOther to
    // it if so (m_pParentMenu == NULL indicates that it is secondary popup).
    if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
        state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
    else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
    {
        CWnd* pParent = this;
        // Child Windows dont have menus--need to go to the top!
        if (pParent != NULL &&
            (hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
        {
            int nIndexMax = ::GetMenuItemCount(hParentMenu);
            for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
            {
                if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
                {
                    // When popup is found, m_pParentMenu is containing menu.
                    state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
                    break;
                }
            }
        }
    }

    state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
    for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++)
    {
        state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
        if (state.m_nID == 0)
            continue; // Menu separator or invalid cmd - ignore it.

        ASSERT(state.m_pOther == NULL);
        ASSERT(state.m_pMenu != NULL);
        ASSERT_NULL(pPopupMenu)
            if (state.m_nID == (UINT)-1)
            {
                // Possibly a popup menu, route to first item of that popup.
                state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
                if (state.m_pSubMenu == NULL ||
                    (state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
                    state.m_nID == (UINT)-1)
                {
                    continue;       // First item of popup cant be routed to.
                }
                state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
            }
            else
            {
                // Normal menu item.
                // Auto enable/disable if frame window has m_bAutoMenuEnable
                // set and command is _not_ a system command.
                state.m_pSubMenu = NULL;
                state.DoUpdate(this, FALSE);
            }

        // Adjust for menu deletions and additions.
        UINT nCount = pPopupMenu->GetMenuItemCount();
        if (nCount < state.m_nIndexMax)
        {
            state.m_nIndex -= (state.m_nIndexMax - nCount);
            while (state.m_nIndex < nCount &&
                pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
            {
                state.m_nIndex++;
            }
        }
        state.m_nIndexMax = nCount;
    }
}

void CTorqueDlg::UpdCfgLangChg(UINT nLang)
{
    COMP_BGE(nLang, LANGUAGE_NUM);
    COMP_BE(g_tGlbCfg.nLangType, nLang);

    theApp.LoadLanguageDll(nLang);
    SetMenu(&m_tMenu[nLang]);
    theApp.m_ptCurShow = &theApp.m_tShowCfg[nLang];
    m_ptShow = theApp.m_ptCurShow;

    /* 井名发生变化，数据记录文件名称和数据序号需要变化 */
    ReGetTorqNo();

    theApp.SaveAppStatus(STATUS_CHGLAN, __FUNCTION__);

    //销毁当前窗口
    theApp.m_pMainWnd = NULL;
    this->DestroyWindow();

    //重新打开窗口
    theApp.ReOpenWindow();
}

void CTorqueDlg::OnLangChn()
{
    UpdCfgLangChg(LANGUAGE_CHINESE);
}

void CTorqueDlg::OnUpdateLangChn(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck((g_tGlbCfg.nLangType == LANGUAGE_CHINESE));
}

void CTorqueDlg::OnLangEng()
{
    UpdCfgLangChg(LANGUAGE_ENGLISH);
}

void CTorqueDlg::OnUpdateLangEng(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck((g_tGlbCfg.nLangType == LANGUAGE_ENGLISH));
}

void CTorqueDlg::OnVerBigTorq()
{
    CString  strValue;

    if (m_bRunStatus)
    {
        strValue.Format(IDS_STRINFRUNNSWITCH);
        m_StatusBar.SetPaneText(0, strValue, TRUE);
        UpdateData(FALSE);
        return;
    }

    g_tGlbCfg.bBigTorq = !g_tGlbCfg.bBigTorq;

    theApp.ShowMainTitle();

    theDB.UpdateGlobalPara();
}

void CTorqueDlg::OnUpdateVerBigTorq(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck((g_tGlbCfg.bBigTorq == TRUE));
}

void CTorqueDlg::UnitChangeTorq()
{
    CString     strValue;
    PARACFG     tCurCfg = { 0 };
    double      fRatio = LBFT2NM;  // 1-->0 : lb.ft --> N.m

    tCurCfg = theApp.m_tParaCfg;

    /* 最后单位 & 单位改变 */
    if (g_tGlbCfg.nTorqUnit == 1) // 0-->1 : N.m --> lb.ft
        fRatio = NM2LBFT;

    tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_MAXLIMIT] = round(fRatio * tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_MAXLIMIT]);
    //tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_UPPERLIMIT] = round(fRatio * tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_UPPERLIMIT]);
    tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_CONTROL] = round(fRatio * tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_CONTROL]);
    tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_OPTIMAL] = round(fRatio * tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_OPTIMAL]);
    //tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_LOWERLIMIT] = round(fRatio * tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_LOWERLIMIT]);
    //tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_SPEEDDOWN] = round(fRatio * tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_SPEEDDOWN]);
    tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_SHOW] = round(fRatio * tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_SHOW]);
    //tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_BEAR] = round(fRatio * tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_BEAR]);

    /*tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_LOWERTAI] = round(fRatio * tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_LOWERTAI]);
    tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_UPPERTAI] = round(fRatio * tCurCfg.tCtrl.fTorqConf[INDEX_TORQ_UPPERTAI]);*/

    theApp.m_tParaCfg = tCurCfg;

    theDB.UpdateGlobalPara();

    theApp.SaveAppStatus(STATUS_CHGUNIT, __FUNCTION__);

    ResetLineChart();
    Invalidate(TRUE);
    UpdateData(FALSE);
}

void CTorqueDlg::OnVerUnitNm()
{
    COMP_BFALSE(JudgeRunStatus(IDS_STRINFRUNNCHGUNIT));

    if (0 == g_tGlbCfg.nTorqUnit)
        return;

    g_tGlbCfg.nTorqUnit = 0;
    g_tGlbCfg.strUnit = theApp.LoadstringFromRes(IDS_STRTORQNMUNIT, BIGPOINT);

    UpdateDlgLabel();

    UnitChangeTorq();
}

void CTorqueDlg::OnUpdateVerUnitNm(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck((g_tGlbCfg.nTorqUnit == 0));
}

void CTorqueDlg::OnVerUnitLbft()
{
    COMP_BFALSE(JudgeRunStatus(IDS_STRINFRUNNCHGUNIT));

    if (1 == g_tGlbCfg.nTorqUnit)
        return;

    g_tGlbCfg.nTorqUnit = 1;
    g_tGlbCfg.strUnit = theApp.LoadstringFromRes(IDS_STRTORQLBFTUNIT, BIGPOINT);

    UpdateDlgLabel();

    UnitChangeTorq();
}

void CTorqueDlg::OnUpdateVerUnitLbft(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck((g_tGlbCfg.nTorqUnit == 1));
}

void CTorqueDlg::OnModpw()
{
    CDlgModPW     dlgModPW;
    dlgModPW.DoModal();
}

void CTorqueDlg::OnValveset()
{
    CDlgValveSet dlgValveSet;

    COMP_BFALSE(JudgeRunStatus(IDS_STRINFRUNNSETPARA));

    dlgValveSet.m_tempValve = theApp.m_tValveCfg;

    if (IDOK != dlgValveSet.DoModal())
        return;

    theApp.SaveAppStatus(STATUS_SETVALVE, __FUNCTION__);

    SendValveCommand();

    UpdateData(FALSE);
}

void CTorqueDlg::OnBnClickedSettoolbuck()
{
    m_bToolBuck = !m_bToolBuck;
}

void CTorqueDlg::OnBnClickedBtnquality()
{
    int         iQuality = 0;
    CDlgRemark  dlgRemark;
    BOOL        bModified = FALSE;
    int         i = 0;
    int         iCause = 0;
    TorqData::ShowInfo* ptRunningShow = NULL;

    /* 检查当前的序号是否有效 */
    COMP_BL(m_tSaveData.ftorque_size(), 1);

    iQuality = dlgRemark.m_iQuality = m_tSaveData.dwquality() & QUA_RESU_QUALITYBIT;
    iCause = dlgRemark.m_iCause = theApp.GetQualityIndex(&m_tSaveData);

    //杀复位定时器
    m_hrtGuard.KillTimer();

    if (IDOK != dlgRemark.DoModal())
    {
        GuardTimerOut(0, 0);
        return;
    }

    /* 修改了质量属性 */
    if (iQuality != dlgRemark.m_iQuality || iCause != dlgRemark.m_iCause)
    {
        bModified = TRUE;

        if (dlgRemark.m_iQuality == QUA_RESU_GOOD)
        {
            m_tSaveData.set_dwquality(QUA_RESU_GOOD);
        }
        else
        {
            m_tSaveData.set_dwquality(2 << (dlgRemark.m_iCause - 1));
        }

        /* 扭矩质量属性发生变化，需要修改入井序号 */
        if (iQuality != dlgRemark.m_iQuality)
        {
            ptRunningShow = m_tSaveData.mutable_tshow(theApp.GetMainIndex(MAINSHOWTALLY));
            if (theApp.HaveTallyNO(&m_tSaveData))
                ptRunningShow->set_strvalue(to_string(m_nCurRunningNO));
            else
                ptRunningShow->set_strvalue(NULLSTR);
        }
    }

    m_strQuality = theApp.GetQualityInfo(&m_tSaveData).c_str();

    if (bModified)
    {
        theApp.UpdateHisData(theApp.m_strDataFile.c_str(), -1, &m_tSaveData);
        UpdateData(FALSE);
    }

    GuardTimerOut(0, 0);
}

void CTorqueDlg::OnBnClickedBtnBreakoutFile()
{
    CString strFilter;
    CString strInfo, strPath;
    TorqData::Torque* ptTorq = NULL;

    strFilter.Format(IDS_STRDATFILTER);

    CFileDialog fileDlg(TRUE, "pbd", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, NULL);

    COMP_BNE(fileDlg.DoModal(), IDOK);

    strPath = fileDlg.GetPathName();
    m_strBreakoutFile = fileDlg.GetFileName();

    if (!theApp.ReadHisTorqFromFile(strPath))
    {
        strInfo.Format(IDS_STRINFFILEERR);
        theApp.SaveShowMessage(strInfo);
        return;
    }

    g_tGlbCfg.bBreakOut = true;
    g_tGlbCfg.strBreakOutPath = strPath.GetBuffer(0);
    /*save into ini*/
    theDB.UpdateGlobalPara();

    // find the breakout index

    UpdateData(FALSE);

    return;
}

void CTorqueDlg::EnableBreakoutfile(bool bEnable)
{
    GetDlgItem(IDC_BTNBREAKOUTFILE)->EnableWindow(bEnable);
    GetDlgItem(IDC_SETTOOLBUCK)->EnableWindow(!bEnable);
    GetDlgItem(IDC_BTNSHOWSET)->EnableWindow(!bEnable);
    
    if(!bEnable)
	{
        GetDlgItem(IDC_BTRUN)->EnableWindow(!bEnable);
        m_strBreakoutFile.Empty();
	}
    else 
    {
        GetDlgItem(IDC_BTRUN)->EnableWindow(g_tReadData.nCur > 0);
    }
}

void CTorqueDlg::OnBnClickedRadiomakeup()
{
    string  strShow;
    CString strValue;
    string  strInfo;

    ASSERT_ZERO(m_iBreakout);
    if (m_bRunStatus)
    {
        strShow = theApp.LoadstringFromRes(IDS_STRINFRUNNSWITCH);
        m_StatusBar.SetPaneText(0, strShow.c_str(), TRUE);
        UpdateData(FALSE);
        return;
    }

    strInfo = theApp.LoadstringFromRes(IDS_STRINFSWITCHVER);
    if (IDNO == AfxMessageBox(strInfo.c_str(), MB_YESNO | MB_ICONINFORMATION))
    {
        UpdateData(FALSE);
        return;
    }

    m_iBreakout = 0;
    g_tGlbCfg.bBreakOut = false;
    g_tGlbCfg.strBreakOutPath.clear();
    /*save into ini*/
    theDB.UpdateGlobalPara();
	
	ReGetTorqNo();

    ResetLineChart();
    UpdateData(FALSE);
}

void CTorqueDlg::OnBnClickedRadiobreakout()
{
    string  strShow;
    CString strValue;
    string  strInfo;

    COMP_BG(m_iBreakout, 0);
    if (m_bRunStatus)
    {
        strShow = theApp.LoadstringFromRes(IDS_STRINFRUNNSWITCH);
        m_StatusBar.SetPaneText(0, strShow.c_str(), TRUE);
        UpdateData(FALSE);
        return;
    }

    strInfo = theApp.LoadstringFromRes(IDS_STRINFSWITCHVER);
    if (IDNO == AfxMessageBox(strInfo.c_str(), MB_YESNO | MB_ICONINFORMATION))
    {
        UpdateData(FALSE);
        return;
    }

    m_iBreakout = 1;

    g_tGlbCfg.bBreakOut = true;
    /*save into ini*/
    theDB.UpdateGlobalPara();
	ReGetTorqNo();

    ResetLineChart();
    UpdateData(FALSE);
}

/* 延迟1s结束，避免状态3后扭矩继续冲高 */
void CTorqueDlg::CalcDelayCount()
{
    m_nDelayCount = 1;
    if (g_tGlbCfg.nCollectDur >= 1000)
        return;

    if (g_tGlbCfg.nCollectDur < 100)
    {
        m_nDelayCount = 10;
    }

    m_nDelayCount = 1000 / g_tGlbCfg.nCollectDur;
}

CDialog* CTorqueDlg::GetDlgPoint(CDialog* pDialog, BYTE uDlgKind, UINT iDlgID)
{
    COMP_BNE_R(pDialog, NULL, pDialog);

    switch (uDlgKind)
    {
    case DLG_COLLECTDATA:
        pDialog = new CDlgCollect();
        break;
    case DLG_SEGCALIB:
        pDialog = new CDlgSegCabl();
        break;
    }

    ASSERT_NULL_R(pDialog, NULL);

    //Create success.
    if (pDialog->Create(iDlgID, this))
    {
        pDialog->SetOwner((CWnd*)this);
        return pDialog;
    }

    delete pDialog;
    return NULL;
}

void CTorqueDlg::StartGetValveStatus()
{
    m_hrtReadValve.CreateTimer(this, READVALVE_LEN, HRTReadValve);
    m_iValveBreakCnt = (int)ceil(MAXCOMMBREAKTIME / READVALVE_LEN);
}

void CTorqueDlg::StopGetValveStatus()
{
    m_hrtReadValve.KillTimer();
    m_iValveBreakCnt = (int)ceil(MAXCOMMBREAKTIME / READVALVE_LEN);
}

void CTorqueDlg::CanModLastData(BOOL bCan)
{
    m_bCanModLastData = bCan;
    GetDlgItem(IDC_BTNQUALITY)->EnableWindow(m_bCanModLastData);
    GetDlgItem(IDC_SETTOOLBUCK)->EnableWindow(!m_bCanModLastData);
}

void CTorqueDlg::OnSegcalib()
{
    COMP_BFALSE(JudgeRunStatus(IDS_STRINFRUNNCALIB));

    COMP_BFALSE(theApp.CheckPassWord());

    theApp.SaveAppStatus(STATUS_CALIB, __FUNCTION__);

    m_pdlgCalib = (CDlgSegCabl*)GetDlgPoint(m_pdlgCalib,
        DLG_SEGCALIB,
        IDD_DLGSEGCALIB);
    ASSERT_NULL(m_pdlgCalib);

    StopGetValveStatus();

    m_pdlgCalib->ShowWindow(SW_SHOW);
}

         