// Torque.h : main header file for the TORQUE application
//

#if !defined(AFX_TORQUE_H__B8387AE5_653F_4729_8A7A_95783AA1DEC1__INCLUDED_)
#define AFX_TORQUE_H__B8387AE5_653F_4729_8A7A_95783AA1DEC1__INCLUDED_

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

#include "odbcinst.h"
//#include "Mylistctrl.h"

#include "DBAccess.h"

/////////////////////////////////////////////////////////////////////////////
// CTorqueApp:
// See Torque.cpp for the implementation of this class
//
typedef struct tagASTAT
{
    ADAPTER_STATUS  adapt;
    /*NAME_BUFFER     NameBuff [30];*/
} ASTAT;

class CTorqueApp : public CWinApp
{
public:
    CTorqueApp();
    /* 保存调试信息函数 */
    /* 保存程序启动退出开始关闭设置等状态 */
    void    SaveAppStatus(UINT nStatus, CString strInfo);
    /* 保存采集数据错误信息 */
    void    SaveCollectErrorData(CString strError, BYTE *pucRcvByte, WORD wLen);
    /* 保存采集数据普通信息 */
    void    SaveCollectOrgData(BYTE *pucRcvByte, WORD wLen);
    /* 保存MessageBox显示的信息到文件 */
    void    SaveMessage(CString strMessage);
    void    SaveShowMessage(CString strMessage, UINT nType = MB_OK);
    // 保存串口发送错误数据
    void    SaveSendFailure(UINT nCmdType = 15); /* 默认SCMREAD15 */
    // 保存CRC或者串口错误、长度不对数据
    void    SaveCrcErrorData(BYTE *pucRcvByte, WORD wLen, UINT &nCRCErr);
    // 保存正常数据信息
    void    SaveOrdData(ORGDATA *ptOrgData,BYTE *pucRcvByte, WORD wLen);
    void    SaveMultiData(ORGDATA *ptOrgData,BYTE *pucRcvByte, WORD wLen);
    void    SavePortBufData(BYTE *pucRcvByte, WORD wLen, UINT nClashSta);
    void    SaveLogInfo();
    // 保存复位时的串口数据
    void    SaveResetData(BYTE *pucRcvByte, WORD wLen);
    // 保存上位机发送到串口数据
    void    SaveSendData(CString strCmd, BYTE *pucRcvByte, WORD wLen);
    // 保存串口操作记录
    void    SavePortOper(UINT nPortOpr);
    // 保存字符串数据到log文件
    void    SaveStreamData(CString strStream);

    BOOL    IsDebugInfo(CString strContent);
    void    AdjustParaValue(PARACFG *ptCfg); /* 检查参数是否发生变化 */
    /* 将列表内容导出到excle表格中 */
    BOOL    SaveList2XlsFile(CString strFileName, CString strSheetName, CListCtrl*ptlistData);

    void    AdaptDlgCtrlSize(CDialog *pdlgAdapt, UINT nSheetType = 0);
    void    ShowMainTitle();
    string  GetQualityInfo(TorqData::Torque *ptTorq);
    int     GetQualityIndex(TorqData::Torque *ptTorq);
    void    GetCurWellFile();
    void    GetCurNum();
    /*void    ReadPara(string strParaName, PARACFG *ptCfg, BOOL bNeedWrite = TRUE);
    void    WritePara(string strParaName, PARACFG *ptCfg);*/



    //BOOL    UpdateShowPara(UINT nLang, SHOWCFG* ptShow);
    //void    WriteXlsStatPara(string strParaName, XLSSTATCFG *ptStat);
    //void    WritePortPara(string strParaName, PORTCFG *ptPort);
    //void    WriteOneTubing(string strParaName, string strLang, unsigned string_ID, CString strValue);
    //void    WriteFixTubingPara(string strParaName, string strLang, TUBINGCFG *ptTubing);
    //void    WriteValvePara(string strParaName, VALVECFG *ptValve);
    //void    WriteParaFileName(string strFileName);
    //void    WriteCalibPara(string strParaName, CALIBCFG *ptCalib);
    BOOL    CheckReg(CString strReg[]);
    BOOL    GetVolMacFromRegStr(CString strReg[], DWORD pdwVol[], DWORD pdwMac[], int& iYear, int& iMonth, int& iDay);
    BOOL    GetVolMacInfo(DWORD pdwVol[], DWORD pdwMac[], int iYear, int iMonth, int iDay);
    void    GetMACAddr(UCHAR *pcMac);
    void    StringSubtract(CString& strValue, BYTE ucChar);
    void    SplitRegString(CString strReg[], CString strRegCode);
    void    MergeRegString(CString strReg[], CString& strRegCode);
    BOOL    LoadLanguageDll(UINT nLang= LANGUAGE_CURRENT, BOOL bUpdate=TRUE);
    void    ReOpenWindow();
    void    PlayAlarmSound();
    void    StopAlarmSound();

    HBITMAP CopyDCToBitmap(HDC hScrDC, LPRECT lprcScr);
    int     CopyDCToPNGFile(HDC hScrDC, UINT nNO, CString strFile, LPRECT lprcScr, HDC hMemDC = NULL, HBITMAP hBitmap = NULL);
    HANDLE  GetImgData(HBITMAP hBitmap, LPBITMAPINFOHEADER& lpbi, DWORD& dwBmBitsSize);
    BOOL    SaveBmp(HBITMAP hBitmap, CString FileName);
    BOOL    SavePNG(HBITMAP hBitmap, CString FileName);
    WORD    SearchIPPoint(TorqData::Torque *ptTorq, BOOL bCheckIP);
    WORD    SearchDeltaIP(TorqData::Torque *ptTorq, BOOL bCheckIP);
    WORD    SearchIP4RECPLUS(TorqData::Torque *ptTorq);
    int     GetIPPlus(TorqData::Torque *ptTorq, WORD wIPPos);
    BOOL    SetIPInfo(TorqData::Torque *ptTorq, double fIPTorq);
    double  GetFlopeFactor(TorqData::Torque *ptTorq, WORD wIPPos, UINT nIPTorq);
    UINT    GetIPTorq(TorqData::Torque *ptTorq, WORD &wIPPos, WORD &wSchPos);
    double  GetIPDelCir(TorqData::Torque *ptTorq, WORD wIPPos);
    double  GetCir(TorqData::Torque *ptTorq);
    void    SaveTorqNum();
    DWORD   GetQuality(TorqData::Torque *ptTorq);
    DWORD   JudgeQuality(TorqData::Torque *ptTorq, int iShackle = 0);
    BOOL    ReadHisTorqFromFile(CString strDataName);
    //BOOL    InsertShowOption(SHOWCFG *ptShow, WORD wShowName, CString strOption, CComboBox *ptcbOption);
    CString GetStatType(TorqData::Torque *ptTorq, WORD wPlace);
    CString GetTorqSimpDate(TorqData::Torque *ptTorq);
    CString GetTorqFullDate(TorqData::Torque *ptTorq);
    CString GetTorqCollTime(TorqData::Torque *ptTorq);
    void    ClearReadTorq();
    BOOL    CheckPassWord();
    string  LoadstringFromRes( unsigned string_ID );
    string  LoadstringFromRes(unsigned string_ID, int val);
    string  LoadstringFromRes(unsigned string_ID, double val);
    string  LoadstringFromRes(unsigned string_ID, string val);
    //BOOL    LoadstringFromRes( unsigned string_ID , string &strValue);
    void    GetShowDataRange(DRAWTORQDATA *ptDraw, int &iBegin, int &iEnd, SPLITPOINT *ptSplit, UINT nMulti=1);
    void    IncTorqNo();
    CString GetSaveDataPath();
    int     SplitString(CString strSource, CStringList &slList);
    /* 为时间版本减少差异，获取横轴坐标的参数，包括下限/上限/控制/最大4个函数 */
    double  GetMaxCir(TorqData::Torque *ptTorq);
    double  GetCtrlCir(TorqData::Torque *ptTorq);
    double  GetUpperCir(TorqData::Torque *ptTorq);
    double  GetLowerCir(TorqData::Torque *ptTorq);
    double  GetOptTorq(TorqData::Torque  *ptTorq);
    
    int     SeekTorque(CFile &file, int iDataNum);
    int     SeekPBDataPos(CFile &file, int iCurPos);
    int     SeekFileLen(CFile &file);
    void    UpdateHisData(CString strName, int iDataPlace, TorqData::Torque *ptTorq);
    int     GetMainWellIndex();
    int     GetMainTubeIndex();
    int     GetMainWellIndexfromData(UINT nWellNO, TorqData::Torque *ptTorq);
    //void    UpdateAutoSaveFileName();
    string  GetTubeFactoryValue();
    string  GetTubeOEMValue();
    string  GetTubeSizeValue();
    string  GetTubeMaterValue();
    string  GetTubeCouplValue();
    BOOL    IsFixTube();
    BOOL    CheckFixTube(PARACFG* ptCfg);
    BOOL    FindNotFileChar(CString strName);
    CString GetTorqShowName(TorqData::Torque *ptTorq, int iIndex);
    CString GetTorqShowValue(TorqData::Torque *ptTorq, int iIndex);
    DRAWTORQDATA * GetDrawDataFromTorq(UINT nNO, int iMulti = 1);
    TorqData::Torque * GetOrgTorqFromTorq(UINT nNO);
    string  GetListShowName(SHOWCFG* ptShow, UINT NO);
    string  GetMainShowName(SHOWCFG* ptShow, UINT NO);
    void ClearShowPara(SHOWCFG* ptShow);
    void ClearTorqCfgPara(PARACFG* ptCfg);
    BOOL SetShowNameFromID(string lsID, SHOWCFG* ptShow, UINT nLang = LANGUAGE_CURRENT);
    BOOL SetShowNOFromID(int iType, string lsID, SHOWCFG* ptShow);

    PARACFG         m_tParaCfg;
    //PORTCFG         m_tPortCfg;
    SHOWCFG         m_tShowCfg[LANGUAGE_NUM];         /* 显示参数的所有参数设置 */
    SHOWCFG         *m_ptCurShow;
    XLSSTATCFG      m_tXlsStatCfg[LANGUAGE_NUM];
    DBREG           m_tdbReg;
    VALVECFG        m_tValveCfg;        /* 阀值配置 */
    //CALIBCFG        m_tCalibCfg;        /* 校准参数配置 */
    string          m_strDllFile;       /* 动态链接库文件名称 */
    string          m_strAppPath;
    /*string          m_strParaFile;       保存系统产生配置文件
                                           和.exe同路径，后缀名为.ini */
    /*string          m_strShowFile;      保存系统产生的显示参数配置文件，隐藏 
                                             和.exe同路径，后缀名为.ini */
    string          m_strRegFile;       /* 保存注册信息的文件，隐藏
                                           和.exe同路径，后缀名为.dat */
    string          m_strDataPath;
    string          m_strDataFile;      /* 自动保存的扭矩结构文件
                                           在data路径下，后缀名为.pbd */
    string          m_strLogFile;
    string          m_strLogPath;       /* log路径 */
    
    string          m_strReadFile;      /* 读取历史文件路径 */
    string          m_strFileTitle;     /* 读取历史文件纯名称 */
    string          m_strAutoSaveFile;  /* CTRL+SHIFT+T 保存的数据文件名称?
                                           在data个路径下，后缀名修改为.dbg */
    //string          m_strParity;        /* 串口奇偶校验的字符串 */
    //BOOL            m_bBigTorq;         /* 是否为大扭矩版本2.4.9, TRUE:和单片机扭矩相差10倍,FALSE:1倍 */
    //BOOL            m_bShackle;         /* 是否是卸扣版本，是到控制扭矩不画竖线，按单片机数据显示 */
    //BOOL            m_bCheckIP;         /* 质量判断是否启用高级判断，简单判断以控制扭矩为准， 超过最大扭矩和低于最小扭矩为不合格，默认为1 */
    //BOOL            m_bFirstRun;      /* 是否第一次运行，是的话运行DlgBear */
    //BOOL            m_bFileBehindDate;  /* 日期在文件命名的后面 */
    UINT            m_nTorqMulti;       /* BigTorq: True:10; False:1*/
    //UINT            m_nTestFunc;        /* 0: 真实串口；1：模拟测试；2：读取dat扭矩数据；3: 读取dat 历史数据; 4: 多组历史数据*/
    //UINT            m_nColletTime;      /* 定时收集数据的时间，ms，默认250 */
    //UINT            m_nSaveTime;        /* 大于显示扭矩后保存数据的时间，默认30s */
    //UINT            m_nReset;           /* 复位时间，默认10s */
    //UINT            m_nImgNum;          /* 批量导出图形时，一个jpg文件中包含多少个图形 */
    //UINT            m_nZoomIn;          /* 图形放大倍数 */
    /*UINT            m_nIPShowMode;      /*  1: 只画数据中的拐点
                                            2: 只画计算拐点
                                            3: 数据拐点和计算拐点都画*/
    //double          m_fIPDeltaVal;      /* 默认0.7 */
    UINT            m_nCurNO;           /* 当前扭矩的序号，更换文件清零 */
    DWORD           m_dwTotalTorqNum;   /* 扭矩上扣最大数目 */
    //BOOL            m_bParaChg;         /* 参数设置以及修改，需要将参数写入文件 */
    UINT            m_nPBHead;
    BOOL            m_bShowCRC;
    //UINT            m_nTorqUnit;
    //CString         m_strUnit;
    SAVELOGDATA     m_tSaveLog;
    CFile           m_SaveLogFile;
    CString         m_strDbgHead[DBG_MAXNUM];
    //string          m_strPassWord;

    /* 多语言支持 */
    HINSTANCE       m_hLangDLL[LANGUAGE_NUM];
    //UINT            m_nLangType;        /* 当前语言类型 */

    BYTE            m_ucDPILevel;
    UINT            m_nScreenX;
    UINT            m_nScreenY;
    CFont           m_tLineTextFont;
    CFont           m_tRuleVFont;
    CFont           m_tRuleHFont;
    CFont           m_tPntTextFont;

    UINT            m_nCurRunningNO;       /* 入井序号 */
    double          m_fSheetRatio;
    char            m_cProtoBuf[MAXPROBUFF];
    DRAWTORQDATA    m_tCurDrawTorq;
    DRAWTORQDATA    m_tCurZoomTorq;
    
    CALIBCTRL       m_tCalibCtrl;           /* 分段校准信息 */


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTorqueApp)
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CTorqueApp)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void SaveCurTime();
    void SaveDbgHead(UINT nType);
    void SaveCurTimeAndHead(UINT nType);
    BOOL MsgLenIsZero(WORD wLen, UINT nType);
    void SaveSerialData(BYTE *pucRcvByte, WORD wLen);
    void CheckAppReg();
    BOOL CheckProductDate();
    BOOL GetProductVersion(CString &strVersion);
    void CreateNewWellFile();
    BOOL TimeValidWell(CString strFileName);
    BOOL GetTorqDataFromFile(CString strDataName);
    BOOL ReCalWellNO(CString strDataName);
    void SaveAllData(CString strDataName);

    /* 将列表内容导出到excle表格中 */
    BOOL CheckExcelDriver(CString &strDriver);
    void ExportListToExcel(CString strSheetName, CDatabase* ptDb, CListCtrl*ptlistData);
    BOOL GetDefaultXlsFileName(CString sDefTitle, CString& sExcelFile);
    BOOL MakeSurePathExists( CString &Path, bool FilenameIncluded);

    void InitArray();
    void InitVariant();
    void InitLanguage();
    void InitDefaultConfig(int initstep);
    
    //void InitDBHandle();
    void InitConfigFromDB();
    void InitGlobalPara();
    void InitShowPara(SHOWCFG *ptShow, UINT nLang= LANGUAGE_CURRENT);
    void InitTorqCfgPara(PARACFG *ptCfg);
    void InitXlsStatPara(XLSSTATCFG* ptStat);
    void InitValvePara(VALVECFG* ptCfg);
    /*void ReadTorquePara (string strParaName, CONTROLPARA *ptCtrl, COMMONCFG *ptComm);
    void ReadCircuitPara(string strParaName, CONTROLPARA *ptCtrl, COMMONCFG *ptComm);
    void ReadRpmPara    (string strParaName, CONTROLPARA *ptCtrl, COMMONCFG *ptComm);*/
    //void ReadPortPara   (string strParaName, PORTCFG     *ptPort);
    //void ReadFixTubingPara(string strParaName, string strLang, TUBINGCFG *ptTubing);
    //void ReadXlsStatPara();
    //void ReadValvePara  (string strParaName, VALVECFG    *ptValve);
    //void ReadCalibPara  (string strParaName, CALIBCFG    *ptCalib);
    //void ReadOtherPara  (string strParaName, PARACFG     *ptCfg);
    //void CheckParaFileName(void);

    /*void WriteTorquePara (string strParaName, CONTROLPARA *ptCtrl, COMMONCFG *ptComm);
    void WriteCircuitPara(string strParaName, CONTROLPARA *ptCtrl, COMMONCFG *ptComm);
    void WriteRpmPara    (string strParaName, CONTROLPARA *ptCtrl, COMMONCFG *ptComm);
    void WriteOtherPara  (string strParaName, PARACFG     *ptCfg);*/

    void AdjustTorquePara (CONTROLPARA *ptCtrl);
    void AdjustCircuitPara(CONTROLPARA *ptCtrl);
    //void AdjustPortPara   (PORTCFG *ptPort);
    //void AdjustOtherPara  (PARACFG *ptCfg);

    BOOL JudgeTranslate(TorqData::Torque *ptTorq);
    string GetFixTubeValue(UINT nShowIndex, UINT nCurNO, FIXTUBEINFO *ptFix);
    WORD GetIPPlace(int iCurPnt, int iInterval);

    //BOOL UpdateShowName(UINT nLang, WORD wNum, string name[]);
    BOOL UpdateCurOptions(WORD wNum, string value[], UINT nLang = LANGUAGE_CURRENT);
    BOOL GetOptionIDbyValue(string name, string value, UINT nLang = LANGUAGE_CURRENT);

    unsigned decodeBMP(std::vector<unsigned char>& image, unsigned& w, unsigned& h, unsigned char * bmp);


    /* array */
    /*string   m_slParity[3];
    BYTE     m_ucParity[3];*/
    
    /* 找不到MAC时的默认MAC */
    BYTE     m_ucDefaultMac[5];
    /* 串口有效波特率 */
    //UINT     m_nBand[10];


    /* 计算拐点的全局变量 */
    double  m_fAdjSlope[COLLECTPOINTS];             //相邻点斜率
    double  m_fAdjInfPnt[COLLECTPOINTS];            //相邻点拐点
    double  m_fIntSlope[COLLECTPOINTS];             //间隔点斜率
    double  m_fIntInfPnt[COLLECTPOINTS];            //间隔点拐点
};


extern CTorqueApp theApp;
extern CDBAccess  theDB;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TORQUE_H__B8387AE5_653F_4729_8A7A_95783AA1DEC1__INCLUDED_)

