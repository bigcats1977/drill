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
    /* ���������Ϣ���� */
    /* ������������˳���ʼ�ر����õ�״̬ */
    void    SaveAppStatus(UINT nStatus, CString strInfo);
    /* ����ɼ����ݴ�����Ϣ */
    void    SaveCollectErrorData(CString strError, BYTE *pucRcvByte, WORD wLen);
    /* ����ɼ�������ͨ��Ϣ */
    void    SaveCollectOrgData(BYTE *pucRcvByte, WORD wLen);
    /* ����MessageBox��ʾ����Ϣ���ļ� */
    void    SaveMessage(CString strMessage);
    void    SaveShowMessage(CString strMessage, UINT nType = MB_OK);
    // ���洮�ڷ��ʹ�������
    void    SaveSendFailure(UINT nCmdType = 15); /* Ĭ��SCMREAD15 */
    // ����CRC���ߴ��ڴ��󡢳��Ȳ�������
    void    SaveCrcErrorData(BYTE *pucRcvByte, WORD wLen, UINT &nCRCErr);
    // ��������������Ϣ
    void    SaveOrdData(ORGDATA *ptOrgData,BYTE *pucRcvByte, WORD wLen);
    void    SaveMultiData(ORGDATA *ptOrgData,BYTE *pucRcvByte, WORD wLen);
    void    SavePortBufData(BYTE *pucRcvByte, WORD wLen, UINT nClashSta);
    void    SaveLogInfo();
    // ���渴λʱ�Ĵ�������
    void    SaveResetData(BYTE *pucRcvByte, WORD wLen);
    // ������λ�����͵���������
    void    SaveSendData(CString strCmd, BYTE *pucRcvByte, WORD wLen);
    // ���洮�ڲ�����¼
    void    SavePortOper(UINT nPortOpr);
    // �����ַ������ݵ�log�ļ�
    void    SaveStreamData(CString strStream);

    BOOL    IsDebugInfo(CString strContent);
    void    AdjustParaValue(PARACFG *ptCfg); /* �������Ƿ����仯 */
    /* ���б����ݵ�����excle����� */
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
    /* Ϊʱ��汾���ٲ��죬��ȡ��������Ĳ�������������/����/����/���4������ */
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
    SHOWCFG         m_tShowCfg[LANGUAGE_NUM];         /* ��ʾ���������в������� */
    SHOWCFG         *m_ptCurShow;
    XLSSTATCFG      m_tXlsStatCfg[LANGUAGE_NUM];
    DBREG           m_tdbReg;
    VALVECFG        m_tValveCfg;        /* ��ֵ���� */
    //CALIBCFG        m_tCalibCfg;        /* У׼�������� */
    string          m_strDllFile;       /* ��̬���ӿ��ļ����� */
    string          m_strAppPath;
    /*string          m_strParaFile;       ����ϵͳ���������ļ�
                                           ��.exeͬ·������׺��Ϊ.ini */
    /*string          m_strShowFile;      ����ϵͳ��������ʾ���������ļ������� 
                                             ��.exeͬ·������׺��Ϊ.ini */
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
    //string          m_strParity;        /* ������żУ����ַ��� */
    //BOOL            m_bBigTorq;         /* �Ƿ�Ϊ��Ť�ذ汾2.4.9, TRUE:�͵�Ƭ��Ť�����10��,FALSE:1�� */
    //BOOL            m_bShackle;         /* �Ƿ���ж�۰汾���ǵ�����Ť�ز������ߣ�����Ƭ��������ʾ */
    //BOOL            m_bCheckIP;         /* �����ж��Ƿ����ø߼��жϣ����ж��Կ���Ť��Ϊ׼�� �������Ť�غ͵�����СŤ��Ϊ���ϸ�Ĭ��Ϊ1 */
    //BOOL            m_bFirstRun;      /* �Ƿ��һ�����У��ǵĻ�����DlgBear */
    //BOOL            m_bFileBehindDate;  /* �������ļ������ĺ��� */
    UINT            m_nTorqMulti;       /* BigTorq: True:10; False:1*/
    //UINT            m_nTestFunc;        /* 0: ��ʵ���ڣ�1��ģ����ԣ�2����ȡdatŤ�����ݣ�3: ��ȡdat ��ʷ����; 4: ������ʷ����*/
    //UINT            m_nColletTime;      /* ��ʱ�ռ����ݵ�ʱ�䣬ms��Ĭ��250 */
    //UINT            m_nSaveTime;        /* ������ʾŤ�غ󱣴����ݵ�ʱ�䣬Ĭ��30s */
    //UINT            m_nReset;           /* ��λʱ�䣬Ĭ��10s */
    //UINT            m_nImgNum;          /* ��������ͼ��ʱ��һ��jpg�ļ��а������ٸ�ͼ�� */
    //UINT            m_nZoomIn;          /* ͼ�ηŴ��� */
    /*UINT            m_nIPShowMode;      /*  1: ֻ�������еĹյ�
                                            2: ֻ������յ�
                                            3: ���ݹյ�ͼ���յ㶼��*/
    //double          m_fIPDeltaVal;      /* Ĭ��0.7 */
    UINT            m_nCurNO;           /* ��ǰŤ�ص���ţ������ļ����� */
    DWORD           m_dwTotalTorqNum;   /* Ť���Ͽ������Ŀ */
    //BOOL            m_bParaChg;         /* ���������Լ��޸ģ���Ҫ������д���ļ� */
    UINT            m_nPBHead;
    BOOL            m_bShowCRC;
    //UINT            m_nTorqUnit;
    //CString         m_strUnit;
    SAVELOGDATA     m_tSaveLog;
    CFile           m_SaveLogFile;
    CString         m_strDbgHead[DBG_MAXNUM];
    //string          m_strPassWord;

    /* ������֧�� */
    HINSTANCE       m_hLangDLL[LANGUAGE_NUM];
    //UINT            m_nLangType;        /* ��ǰ�������� */

    BYTE            m_ucDPILevel;
    UINT            m_nScreenX;
    UINT            m_nScreenY;
    CFont           m_tLineTextFont;
    CFont           m_tRuleVFont;
    CFont           m_tRuleHFont;
    CFont           m_tPntTextFont;

    UINT            m_nCurRunningNO;       /* �뾮��� */
    double          m_fSheetRatio;
    char            m_cProtoBuf[MAXPROBUFF];
    DRAWTORQDATA    m_tCurDrawTorq;
    DRAWTORQDATA    m_tCurZoomTorq;
    
    CALIBCTRL       m_tCalibCtrl;           /* �ֶ�У׼��Ϣ */


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

    /* ���б����ݵ�����excle����� */
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
    
    /* �Ҳ���MACʱ��Ĭ��MAC */
    BYTE     m_ucDefaultMac[5];
    /* ������Ч������ */
    //UINT     m_nBand[10];


    /* ����յ��ȫ�ֱ��� */
    double  m_fAdjSlope[COLLECTPOINTS];             //���ڵ�б��
    double  m_fAdjInfPnt[COLLECTPOINTS];            //���ڵ�յ�
    double  m_fIntSlope[COLLECTPOINTS];             //�����б��
    double  m_fIntInfPnt[COLLECTPOINTS];            //�����յ�
};


extern CTorqueApp theApp;
extern CDBAccess  theDB;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TORQUE_H__B8387AE5_653F_4729_8A7A_95783AA1DEC1__INCLUDED_)

