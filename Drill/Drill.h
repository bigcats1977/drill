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

//#include "odbcinst.h"
//#include "Mylistctrl.h"

#include "DBAccess.h"

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
    /* ���������Ϣ���� */
    /* ������������˳���ʼ�ر����õ�״̬ */
    void    SaveAppStatus(UINT nStatus, string strInfo);
    /* ����ɼ����ݴ�����Ϣ */
    void    SaveCollectErrorData(string strError, BYTE* pucRcvByte, WORD wLen);
    /* ����ɼ�������ͨ��Ϣ */
    void    SaveCollectOrgData(BYTE* pucRcvByte, WORD wLen);
    /* ����MessageBox��ʾ����Ϣ���ļ� */
    void    SaveMessage(string strMessage);
    void    SaveShowMessage(string strMessage, UINT nType = MB_OK);
    // ���洮�ڷ��ʹ�������
    void    SaveSendFailure(UINT nCmdType = 15); /* Ĭ��SCMREAD15 */
    // ����CRC���ߴ��ڴ��󡢳��Ȳ�������
    void    SaveCrcErrorData(BYTE* pucRcvByte, WORD wLen, UINT& nCRCErr);
    /* ����������ͻ���ͨ�ŵ���Ϣ���ļ� */
    void    SaveCommunication(BYTE* msg, WORD wLen, UINT nType);
    // ��������������Ϣ
    void    SaveOrdData(ORGDATA* ptOrgData, BYTE* pucRcvByte, WORD wLen);
    void    SaveMultiData(ORGDATA* ptOrgData, BYTE* pucRcvByte, WORD wLen);
    void    SavePortBufData(BYTE* pucRcvByte, WORD wLen, UINT nClashSta);
    void    SaveLogInfo();
    // ���渴λʱ�Ĵ�������
    void    SaveResetData(BYTE* pucRcvByte, WORD wLen);
    // ������λ�����͵���������
    void    SaveSendData(string strCmd, BYTE* pucRcvByte, WORD wLen);
    // ���洮�ڲ�����¼
    void    SavePortOper(UINT nPortOpr);
    // �����ַ������ݵ�log�ļ�
    void    SaveStreamData(string strStream);

    BOOL    IsDebugInfo(string strContent);
    void    AdjustParaValue(PARACFG* ptCfg); /* �������Ƿ����仯 */
    /* ���б����ݵ�����excle����� */
    //BOOL    SaveList2XlsFile(CString strFileName, CString strSheetName, CListCtrl* ptlistData);
    bool    SaveList2XlsFile(string filename, CListCtrl* ptList);
    void    AdaptDlgCtrlSize(CDialog* pdlgAdapt, UINT nSheetType = 0);
    void    ShowMainTitle();
    string  GetQualityInfo(TorqData::Torque* ptTorq);
    int     GetQualityIndex(TorqData::Torque* ptTorq);
    void    GetMACAddr(UCHAR* pcMac);
    void    StringSubtract(CString& strValue, BYTE ucChar);
    void    SplitRegString(CString strReg[], CString strRegCode);
    void    MergeRegString(CString strReg[], CString& strRegCode);
    BOOL    LoadLanguageDll(UINT nLang = LANGUAGE_CURRENT, BOOL bUpdate = TRUE);
    void    ReOpenWindow();
    void    PlayAlarmSound();
    void    StopAlarmSound();

    HBITMAP CopyDCToBitmap(HDC hScrDC, LPRECT lprcScr);
    int     CopyDCToPNGFile(HDC hScrDC, UINT nNO, string strFile, LPRECT lprcScr, HDC hMemDC = NULL, HBITMAP hBitmap = NULL);
    HANDLE  GetImgData(HBITMAP hBitmap, LPBITMAPINFOHEADER& lpbi, DWORD& dwBmBitsSize);
    bool    SaveBmp(HBITMAP hBitmap, string FileName);
    bool    SavePNG(HBITMAP hBitmap, string FileName);
    double  GetCir(TorqData::Torque* ptTorq, bool bBreakout = false);
    DWORD   GetQuality(TorqData::Torque* ptTorq);
    DWORD   JudgeQuality(TorqData::Torque* ptTorq, int iBreakOut = 0);
    BOOL    ReadHisTorqFromFile(string strDataName);
    CString GetStatType(TorqData::Torque* ptTorq, WORD wPlace);
    CString GetTorqSimpDate(TorqData::Torque* ptTorq);
    CString GetTorqFullDate(TorqData::Torque* ptTorq);
    CString GetTorqCollTime(TorqData::Torque* ptTorq, bool bBreakout = false);
    BOOL    CheckPassWord();
    string  LoadstringFromRes(unsigned string_ID);
    //string  LoadstringFromRes(unsigned string_ID, ...);
    //string  LoadstringFromRes(unsigned string_ID, int val);
    //string  LoadstringFromRes(unsigned string_ID, double val);
    //string  LoadstringFromRes(unsigned string_ID, string val);
    void    GetShowDataRange(DRAWTORQDATA* ptDraw, int& iBegin, int& iEnd, SPLITPOINT* ptSplit, UINT nMulti = 1);
    string  GetSaveDataPath();
    int     SplitString(CString strSource, CStringList& slList);
    /* Ϊʱ��汾���ٲ��죬��ȡ��������Ĳ�������������/����/����/���4������ */
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
    TorqData::Torque* GetOrgTorqFromTorq(UINT nNO);
    //string  GetListShowName(SHOWCFG* ptShow, UINT NO);
    string  GetMainShowName(SHOWCFG* ptShow, UINT NO);
    void ClearShowPara(SHOWCFG* ptShow);
    void ClearTorqCfgPara(PARACFG* ptCfg);
    BOOL SetShowNameFromID(string lsID, SHOWCFG* ptShow, UINT nLang = LANGUAGE_CURRENT);
    BOOL SetShowNOFromID(int iType, string lsID, SHOWCFG* ptShow);
    bool HaveTallyNO(TorqData::Torque* ptTorq);
    bool CheckProductDate();
    void SaveAllData(string strDataName);
    string GetFileNameFromPath(string path);
    void ReloadTorqCfg();

    PARACFG         m_tParaCfg;
    SHOWCFG         m_tShowCfg[LANGUAGE_NUM];         /* ��ʾ���������в������� */
    SHOWCFG* m_ptCurShow;
    DBREG           m_tdbReg;
    XLSSTATCFG      m_tXlsStatCfg[LANGUAGE_NUM];
    VALVECFG        m_tValveCfg;        /* ��ֵ���� */
    string          m_strDllFile;       /* ��̬���ӿ��ļ����� */
    string          m_strAppPath;
    string          m_strRegFile;       /* ����ע����Ϣ���ļ�������
                                           ��.exeͬ·������׺��Ϊ.dat */
    string          m_strDataPath;
    string          m_strDataFile;      /* �Զ������Ť�ؽṹ�ļ�
                                           ��data·���£���׺��Ϊ.pbd */
    string          m_strLogFile;
    string          m_strLogPath;       /* log·�� */

    string          m_strReadFile;      /* ��ȡ��ʷ�ļ�·�� */
    string          m_strFileTitle;     /* ��ȡ��ʷ�ļ������� */
    string          m_strAutoSaveFile;  /* CTRL+SHIFT+T ����������ļ�����?
                                           ��data��·���£���׺���޸�Ϊ.dbg */
    UINT            m_nTorqMulti;       /* BigTorq: True:10; False:1*/
    UINT            m_nPBHead;
    BOOL            m_bShowCRC;
    SAVELOGDATA     m_tSaveLog;
    CFile           m_SaveLogFile;
    CString         m_strDbgHead[DBG_MAXNUM];

    /* ������֧�� */
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

    CALIBCTRL       m_tCalibCtrl;           /* �ֶ�У׼��Ϣ */


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDrillApp)
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

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
    bool GetProductVersion(CString& strVersion);
    void CreateNewWellFile();
    BOOL GetTorqDataFromFile(string strDataName);
    BOOL ReCalTallyNO(string strDataName);
    void ClearReadTorq();

    /* ���б����ݵ�����excle����� */
    /*BOOL CheckExcelDriver(CString& strDriver);
    void ExportListToExcel(CString strSheetName, CDatabase* ptDb, CListCtrl* ptlistData);
    BOOL GetDefaultXlsFileName(CString sDefTitle, CString& sExcelFile);
    BOOL MakeSurePathExists(CString& Path, bool FilenameIncluded);*/

    void InitArray();
    void InitVariant();
    void InitLanguage();
    void InitDefaultConfig(int initstep);

    void InitConfigFromDB();
    void InitGlobalPara();
    void InitShowPara(SHOWCFG* ptShow, UINT nLang = LANGUAGE_CURRENT);
    void InitTorqCfgPara(PARACFG* ptCfg);
    void InitXlsStatPara(XLSSTATCFG* ptStat);
    void InitValvePara(VALVECFG* ptCfg);

    void AdjustTorquePara(CONTROLPARA* ptCtrl);
    void AdjustCircuitPara(CONTROLPARA* ptCtrl);

    BOOL JudgeTranslate(TorqData::Torque* ptTorq);
    WORD GetIPPlace(int iCurPnt, int iInterval);

    BOOL UpdateCurOptions(WORD wNum, string value[], UINT nLang = LANGUAGE_CURRENT);
    BOOL GetOptionIDbyValue(string name, string value, UINT nLang = LANGUAGE_CURRENT);
    bool GetMakeupDrawData(TorqData::Torque* ptOrg, DRAWTORQDATA* ptDraw, UINT nMulti);
    bool GetBreakoutDrawData(TorqData::Torque* ptOrg, DRAWTORQDATA* ptDraw, UINT nMulti);

    unsigned decodeBMP(std::vector<unsigned char>& image, unsigned& w, unsigned& h, unsigned char* bmp);

    /* �Ҳ���MACʱ��Ĭ��MAC */
    BYTE     m_ucDefaultMac[5];

    /* ����յ��ȫ�ֱ��� */
    double  m_fAdjSlope[COLLECTPOINTS];             //���ڵ�б��
    double  m_fAdjInfPnt[COLLECTPOINTS];            //���ڵ�յ�
    double  m_fIntSlope[COLLECTPOINTS];             //�����б��
    double  m_fIntInfPnt[COLLECTPOINTS];            //�����յ�
};


extern CDrillApp 	theApp;
extern CDBAccess  	theDB;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRILL_H__B8387AE5_653F_4729_8A7A_95783AA1DEC1__INCLUDED_)

