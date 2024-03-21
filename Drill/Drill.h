// Drill.h : main header file for the DRILL application
//

#if !defined(AFX_DRILL_H__B8387AE5_653F_4729_8A7A_95783AA1DEC1__INCLUDED_)
#define AFX_DRILL_H__B8387AE5_653F_4729_8A7A_95783AA1DEC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
error include 'stdafx.h' before including this file for PCH
#endif

//#define CRT_DBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Nb30.h>

#include "resource.h"       // main symbols

#include "DBAccess.h"
#include "RegProc.h"
#include "Socket.h"

/////////////////////////////////////////////////////////////////////////////
// CDrillApp:
// See Drill.cpp for the implementation of this class
//
typedef struct tagASTAT
{
    ADAPTER_STATUS  adapt;
    /*NAME_BUFFER     NameBuff [30];*/
} ASTAT;

class CDrillApp : public CWinApp
{
public:
    CDrillApp();
    /* 保存调试信息函数 */
    /* 保存程序启动退出开始关闭设置等状态 */
    void    SaveAppStatus(UINT nStatus, string strInfo);
    /* 保存采集数据错误信息 */
    void    SaveCollectErrorData(string strError, BYTE* pucRcvByte, WORD wLen);
    /* 保存采集数据普通信息 */
    void    SaveCollectOrgData(BYTE* pucRcvByte, WORD wLen);
    /* 保存MessageBox显示的信息到文件 */
    void    SaveMessage(string strMessage);
    void    SaveShowMessage(string strMessage, UINT nType = MB_OK);
    // 保存串口发送错误数据
    void    SaveSendFailure(UINT nCmdType = 15); /* 默认SCMREAD15 */
    // 保存CRC或者串口错误、长度不对数据
    void    SaveCrcErrorData(BYTE* pucRcvByte, WORD wLen, UINT& nCRCErr);
    /* 保存服务器客户端通信的信息到文件 */
    void    SaveCommunication(BYTE* msg, WORD wLen, UINT nType);
    // 保存正常数据信息
    void    SaveOrdData(ORGDATA* ptOrgData, BYTE* pucRcvByte, WORD wLen);
    void    SaveMultiData(ORGDATA* ptOrgData, BYTE* pucRcvByte, WORD wLen);
    void    SavePortBufData(BYTE* pucRcvByte, WORD wLen, UINT nClashSta);
    void    SaveLogInfo();
    void    SaveTCPInfo();
    // 保存复位时的串口数据
    void    SaveResetData(BYTE* pucRcvByte, WORD wLen);
    // 保存上位机发送到串口数据
    void    SaveSendData(string strCmd, BYTE* pucRcvByte, WORD wLen);
    // 保存串口操作记录
    void    SavePortOper(UINT nPortOpr);
    // 保存字符串数据到log文件
    void    SaveStreamData(string strStream);
    void    SaveTCPData(string strData, bool Status = true);

    BOOL    IsDebugInfo(string strContent);
    void    AdjustParaValue(PARACFG* ptCfg); /* 检查参数是否发生变化 */
    /* 将列表内容导出到excle表格中 */
    //BOOL    SaveList2XlsFile(CString strFileName, CString strSheetName, CListCtrl* ptlistData);
    bool    SaveList2XlsFile(string filename, CListCtrl* ptList);
    void    AdaptDlgCtrlSize(CDialog* pdlgAdapt, UINT nSheetType = 0);
    void    ShowMainTitle();
    string  GetQualityInfo(TorqData::Torque* ptTorq);
    int     GetQualityIndex(TorqData::Torque* ptTorq);
    void    GetMACAddr(UCHAR* pcMac);
    BOOL    LoadLanguageDll(UINT nLang = LANGUAGE_CURRENT, BOOL bUpdate = TRUE);
    void    ReOpenWindow();
    void    PlayAlarmSound();
    void    StopAlarmSound();

    HBITMAP CopyDCToBitmap(HDC hScrDC, UINT width, UINT height);
    int     CopyDCToPNGFile(HDC hScrDC, UINT nNO, string strFile, UINT width, UINT height, bool bRotate = false);
    HANDLE  GetImgData(HBITMAP hBitmap, LPBITMAPINFOHEADER& lpbi, DWORD& dwBmBitsSize);
    HANDLE  RotateImgData(HBITMAP hBitmap, LPBITMAPINFOHEADER& lpOldbi, LPBITMAPINFOHEADER& lpNewbi, DWORD& dwBmBitsSize);
    bool    SaveBmp(HBITMAP hBitmap, string FileName);
    bool    SavePNG(HBITMAP hBitmap, string FileName, bool bRotate = false);
    double  GetCir(TorqData::Torque* ptTorq, UINT nType = TYPE_TOTAL);
    DWORD   GetQuality(TorqData::Torque* ptTorq);
    DWORD   JudgeQuality(TorqData::Torque* ptTorq);
    BOOL    ReadHisTorqFromFile(string strDataName);
    CString GetStatType(TorqData::Torque* ptTorq, WORD wPlace);
    CString GetTorqSimpDate(TorqData::Torque* ptTorq, bool bBreakout = false);
    CString GetTorqFullDate(TorqData::Torque* ptTorq, bool bBreakout = false);
    CString GetTorqCollTime(TorqData::Torque* ptTorq, bool bBreakout = false);
    BOOL    CheckPassWord();
    string  LoadstringFromRes(unsigned string_ID);
    string  GetSaveDataPath();
    int     SplitString(CString strSource, CStringList& slList);
    /* 为时间版本减少差异，获取横轴坐标的参数，包括下限/上限/控制/最大4个函数 */
    double  GetMaxCir(TorqData::Torque* ptTorq);
    double  GetCtrlCir(TorqData::Torque* ptTorq);
    double  GetUpperCir(TorqData::Torque* ptTorq);
    double  GetLowerCir(TorqData::Torque* ptTorq);
    double  GetOptTorq(TorqData::Torque* ptTorq);

    int     SeekTorque(CFile& file, int iDataNum);
    int     SeekPBDataPos(CFile& file, int iCurPos);
    int     SeekFileLen(CFile& file);
    void    UpdateHisData(string strName, int iDataPlace, TorqData::Torque* ptTorq);
    int     GetMainIndex(UINT nNO);
    int     GetMainIndexfromData(UINT nNO, TorqData::Torque* ptTorq);
    BOOL    FindNotFileChar(CString strName);
    CString GetTorqShowName(TorqData::Torque* ptTorq, int iIndex);
    CString GetTorqShowValue(TorqData::Torque* ptTorq, int iIndex);
    // nType 1:MakeUP, 2: BreakOut, 3: All
    DRAWTORQDATA* GetDrawDataFromTorq(UINT nNO, UINT nMulti = 1, UINT nType = 3);
    DRAWTORQDATA* GetDrawDataFromTorq(TorqData::Torque* ptOrg, UINT nMulti = 1, UINT nType = 3);
    TorqData::Torque* GetOrgTorqFromTorq(UINT nNO);
    //string  GetListShowName(SHOWCFG* ptShow, UINT NO);
    string  GetMainShowName(SHOWCFG* ptShow, UINT NO);
    void ClearShowPara(SHOWCFG* ptShow);
    void ClearTorqCfgPara(PARACFG* ptCfg);
    BOOL SetShowNameFromID(string lsID, SHOWCFG* ptShow, UINT nLang = LANGUAGE_CURRENT);
    BOOL SetShowNOFromID(int iType, string lsID, SHOWCFG* ptShow);
    bool HaveMakeUP(TorqData::Torque* ptTorq);
    bool HaveBreakout(TorqData::Torque* ptTorq);
    bool HaveTallyNO(TorqData::Torque* ptTorq);
    void SaveAllData(string strDataName);
    string GetFileNameFromPath(string path);
    void ReloadTorqCfg();
    bool GetProductVersion(CString& strVersion);
    void InitTCPServer();
    int  ReportWITSByTCP(string strData);
    void CloseSockets();
    bool isTCPConnected();
    bool isTCPServer();
    bool GetTimeFromString(CString strTime, __time64_t& time);

    PARACFG         m_tParaCfg;
    SHOWCFG         m_tShowCfg[LANGUAGE_NUM];         /* 显示参数的所有参数设置 */
    SHOWCFG*        m_ptCurShow;
    //DBREG           m_tdbReg;
    CRegProc        m_tReg;
    XLSSTATCFG      m_tXlsStatCfg[LANGUAGE_NUM];
    VALVECFG        m_tValveCfg;        /* 阀值配置 */
    SERVERCFG       m_tServCfg;
    WITSCFG         m_tWITSCfg;
    string          m_strDllFile;       /* 动态链接库文件名称 */
    string          m_strAppPath;
    string          m_strRegFile;       /* 保存注册信息的文件，隐藏
                                           和.exe同路径，后缀名为.dat */
    string          m_strDataPath;
    string          m_strDataFile;      /* 自动保存的扭矩结构文件
                                           在data路径下，后缀名为.pbd */
    string          m_strLogFile;
    string          m_strLogPath;       /* log路径 */
    string          m_strTCPFile;

    //string          m_strReadFile;      /* 读取历史文件路径 now in g_tReadData*/
    string          m_strFileTitle;     /* 读取历史文件纯名称 */
    string          m_strAutoSaveFile;  /* CTRL+SHIFT+T 保存的数据文件名称?
                                           在data个路径下，后缀名修改为.dbg */
    UINT            m_nTorqMulti;       /* BigTorq: True:10; False:1*/
    UINT            m_nPBHead;
    BOOL            m_bShowCRC;
    SAVELOGDATA     m_tSaveLog;
    CFile           m_SaveLogFile;
    SAVELOGDATA     m_tSaveTCP;         /* 记录TCP相关信息 */
    CFile           m_SaveTCPFile;
    CString         m_strDbgHead[DBG_MAXNUM];

    /* 多语言支持 */
    HINSTANCE       m_hLangDLL[LANGUAGE_NUM];

    BYTE            m_ucDPILevel;
    UINT            m_nScreenX;
    UINT            m_nScreenY;
    CFont           m_tLineTextFont;
    CFont           m_tRuleVFont;
    CFont           m_tRuleHFont;
    CFont           m_tPntTextFont;

    double          m_fSheetRatio;
    char            m_cProtoBuf[MAXPROBUFF];
    DRAWTORQDATA    m_tCurDrawTorq;
    DRAWTORQDATA    m_tCurZoomTorq;

    CALIBCTRL       m_tCalibCtrl;           /* 分段校准信息 */

    TCP::cSocket      mi_Socket;
    HANDLE            mh_Thread;
    TCP::cSocket::cHash<SOCKET, DWORD> mi_SocketList;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDrillApp)
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL
    static ULONG WINAPI ProcessEventThread(void* p_Param);
    void    ProcessEvents();

// Implementation

    //{{AFX_MSG(CDrillApp)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void SaveCurTime();
    void SaveDbgHead(UINT nType);
    void SaveCurTimeAndHead(UINT nType);
    BOOL MsgLenIsZero(WORD wLen, UINT nType);
    void SaveHexData(BYTE* pucRcvByte, WORD wLen);
    void CreateNewWellFile();
    BOOL GetTorqDataFromFile(string strDataName);
    BOOL ReCalTallyNO(string strDataName);
    void ClearReadTorq(TORQUEDATA* pAllData);

    void InitArray();
    void InitVariant();
    void InitLanguage();
    void InitSysPath();
    void InitDefaultConfig(int initstep);

    void InitConfigFromDB();
    void InitGlobalPara();
    void InitShowPara(SHOWCFG* ptShow, UINT nLang = LANGUAGE_CURRENT);
    void InitTorqCfgPara(PARACFG* ptCfg);
    void InitXlsStatPara(XLSSTATCFG* ptStat);
    void InitValvePara(VALVECFG* ptCfg);
    void InitServerPara(SERVERCFG* ptCfg);
    void InitWITSPara(WITSCFG* ptCfg);

    void AdjustTorquePara(CONTROLPARA* ptCtrl);
    void AdjustCircuitPara(CONTROLPARA* ptCtrl);

    BOOL JudgeTranslate(TorqData::Torque* ptTorq);

    BOOL UpdateCurOptions(WORD wNum, string value[], UINT nLang = LANGUAGE_CURRENT);
    BOOL GetOptionIDbyValue(string name, string value, UINT nLang = LANGUAGE_CURRENT);
    bool GetMakeupDrawData(TorqData::Torque* ptOrg, DRAWTORQDATA* ptDraw, UINT nMulti);
    bool GetBreakoutDrawData(TorqData::Torque* ptOrg, DRAWTORQDATA* ptDraw, UINT nMulti);
    void AutoupdateLogFile();

    unsigned decodeBMP(std::vector<unsigned char>& image, unsigned& w, unsigned& h, unsigned char* bmp);
    void ProcessReceivedDataNormal(TCP::cSocket::cMemory* pi_RecvMem);

    /* 找不到MAC时的默认MAC */
    BYTE     m_ucDefaultMac[5];
};


extern CDrillApp theApp;
extern CDBAccess theDB;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRILL_H__B8387AE5_653F_4729_8A7A_95783AA1DEC1__INCLUDED_)

