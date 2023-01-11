// DrillDlg.h : header file
//

#if !defined(AFX_DRILLDLG_H__63474767_3526_4CC3_8512_1F93AF2AE779__INCLUDED_)
#define AFX_DRILLDLG_H__63474767_3526_4CC3_8512_1F93AF2AE779__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BtnST.h"
#include "LineChartCtrlEx.h"
#include "Ruler.h"

#include "MySheet.h"
#include "HighResTimer.h"
#include "afxwin.h"

#include "serialport.h"

//#define  TESTRUN   /* ���Զ����ֵ */

/////////////////////////////////////////////////////////////////////////////
// CDrillDlg dialog
class CDrillDlg : public CDialog
{
    // Construction
public:
    CDrillDlg(CWnd* pParent = NULL);   // standard constructor

    /* ���ڶ�ȡCRC����ʱ��ֹͣ���Ա������ݣ��������� */
    void        RestartComm();

    BOOL        SendData(UINT nParaType);
    void        ReStart();
    void        TestSpeedLed(int iLedIcon);
    void        TestUnloadLed(int iLedIcon);
    void        TestAlarmLed(int iLedIcon);
    void        TestCommLed(int iLedIcon);
    void        ResetLineChart();//BOOL bRedraw = FALSE);
    void        SendAllCommand();
    void        SendValveCommand();
    void        StartGetValveStatus();
    void        StopGetValveStatus();

    /* ��дModBus�����������ͳһ���� */
    void        FillRead(void);
    void        FillMulti();
    void        FillQipi();
    void        FillCollect();
    void        FillShow();
    void        FillSpeed();
    void        FillControl();
    void        FillBSpeed();
    void        FillESpeed();
    void        FillBUnload();
    void        FillEUnload();
    void        FillUpper();
    void        FillLower();
    void        FillCut();
    void        FillCollectOK();
    void        FillCollectNOK();
    void        FillTime();
    void        FillValvePress();
    void        FillValveFlow();
    void        FillReadValve();
    void        FillReadMulti(void);
    void        FillWriteCalib();
    void        FillCtrlCalib();
    void        FillReadCalib();

    /* �ռ�����(�������Ժ���) */
    BOOL        CollectPort(COLLECTDATA* ptCollData);
    BOOL        CollectRandData(COLLECTDATA* ptCollData);
    BOOL        CollectTorqData(COLLECTDATA* ptCollData);
    BOOL        CollectMultiTorq(COLLECTDATA* ptCollData);
    BOOL        CollectHisData();
    BOOL        RunIniAutoFile();
    BOOL        PortQueIsEmpty();

    BOOL    GetVolMacFromRegStr(CString strReg[], DWORD pdwVol[], DWORD pdwMac[], int& iYear, int& iMonth, int& iDay);
    BOOL    GetVolMacInfo(DWORD pdwVol[], DWORD pdwMac[], int iYear, int iMonth, int iDay);

    BYTE        m_ucRcvByte[PORTBUFF];      /*modbus������Ϣ��ֵ*/
    WORD        m_wRcvLen;
    CTime       m_tSetTime;

    CSerialPort m_tPort;

    // Dialog Data
        //{{AFX_DATA(CDrillDlg)
    enum { IDD = IDD_DRILL_DIALOG };
    CButtonST   m_cbComm;
    CButtonST   m_cbAlarm;
    CButtonST   m_cbValveStatus[VALVEMAXNUM];
    CButtonST   m_btnRun;
    CStatic     m_CommStatus;
    CRuler      m_xAxis1;
    CRuler      m_yAxis1;
    CRuler      m_xAxis2;
    CRuler      m_yAxis2;
    //CString     m_strCircle;  /* m_fCircle ����1��ʱ����ʾ��ǰ����/������� */
    CString     m_strTorque;  /* m_fTorque ������ʾ��Ť�أ�ж��ʱ��ʾ����ǰŤ��/���Ť�� */
    double      m_fRpm;     /* ��Ƭ���϶�ȡ�����ٶȣ�Ϊ0ʱ����Ȧ������ */
    BOOL        m_bComm;
    CString     m_strSeqNO;
    UINT        m_nCRCERR;
    UINT        m_nClashERR;
    CString     m_strRecvData;
    DWORD       m_dwTotalTorqNum;           /* Ť���Ͽ������Ŀ */
    BOOL        m_bToolBuck;        /* �Ƿ�Ϊ���߿� */
    BOOL        m_iBreakOut;        /* �Ƿ����ж�۲��� 0:�Ͽۣ�1��ж��*/
    CString     m_strQuality;
    CString     m_strLBM2;
    CString     m_strTorqType;
    CString     m_strBreakoutNO;
    CString     m_strMainName[MAXMAINPARA];
    CString     m_strMainValue[MAXMAINPARA];
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDrillDlg)
public:
    virtual BOOL DestroyWindow();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON       m_hIcon;
    CToolBar    m_wndtoolbar;
    CStatusBar  m_StatusBar;
    CMenu     m_tMenu[LANGUAGE_NUM];

    // Generated message map functions
    //{{AFX_MSG(CDrillDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
    //afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnSetpara();
    afx_msg void OnSetShow();
    afx_msg void OnAbout();
    afx_msg void OnDebug();
    afx_msg void OnAppExit();
    afx_msg void OnZero();
    afx_msg void OnScm();
    afx_msg void OnHistorylist();
    afx_msg void OnHistorystat();
    afx_msg void OnShowtest();
    afx_msg void OnCollectdata();
    afx_msg void OnBtrun();
    afx_msg void OnLangChn();
    afx_msg void OnUpdateLangChn(CCmdUI* pCmdUI);
    afx_msg void OnLangEng();
    afx_msg void OnUpdateLangEng(CCmdUI* pCmdUI);
    afx_msg void OnVerBigTorq();
    afx_msg void OnUpdateVerBigTorq(CCmdUI* pCmdUI);
    afx_msg void OnVerUnitNm();
    afx_msg void OnUpdateVerUnitNm(CCmdUI* pCmdUI);
    afx_msg void OnVerUnitLbft();
    afx_msg void OnUpdateVerUnitLbft(CCmdUI* pCmdUI);
    afx_msg void OnModpw();
    afx_msg void OnValveset();
    afx_msg void OnBnClickedSettoolbuck();
    afx_msg void OnBnClickedBtnquality();
    //afx_msg void OnBnClickedBtnBreakoutFile();
    afx_msg void OnBnClickedRadiomakeup();
    afx_msg void OnBnClickedRadiobreakout();
    afx_msg void OnBnClickedBtnshowset();
    //}}AFX_MSG
    afx_msg LRESULT CollectTimerOut(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT GuardTimerOut(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT PortBuffTimerOut(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT SaveDataTimerOut(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT SaveDebugTimerOut(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT ALarmPlayTimerOut(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT ReadValveTimerOut(WPARAM wParam, LPARAM lParam);
    afx_msg LONG OnCommunication(WPARAM ch, LPARAM port);
    afx_msg void OnSegcalib();
    afx_msg void OnGlbCfg();
    DECLARE_MESSAGE_MAP()

private:
    void InitVariant();
    void InitMainShowPara();
    void InitDlgControl();
    void UpdateDlgLabel();
    void ClearInfo(BOOL bClrTorq = TRUE);
    BOOL ChangeCommParam(BOOL bUpdateText = TRUE);
    WORD GetCRCValue(BYTE* pucCrc, WORD iLen);
    BOOL CompSysPara(const BYTE* pPara1, const BYTE* pPara2, WORD wSize);
    int  GetInsertNum(double fT1, double fT2);
    /* ��䷢�ʹ�������Ĳ��� */
    WORD FillSendByte(UINT nParaType);
    BOOL RunInitRand();
    BOOL RunInitPort();
    void RunTorque();
    void StopTorque();
    void DrawLastPoint();
    BOOL TimeReadPort(UINT nCurStatus);
    BOOL CheckReadInterval();
    BOOL CheckDataValid();
    static void HRTReadPort(CWnd* pUser);  /* ��ȡ���ڶ�ʱ��Timer1��ʱ */
    static void HRTGuard(CWnd* pUser);     /* ������λ��ʱ��Timer2��ʱ */
    static void HRTPortBuff(CWnd* pUser);  /* ������ǰ�����������ݵȴ���Ƭ��д��ʱ��Timer7��ʱ */
    static void HRTSaveData(CWnd* pUser);  /* �������ݶ�ʱ��Timer8?��ʱ */
    static void HRTSaveDebug(CWnd* pUser); /* ��ʱ����CRC����͵�����Ϣ��ʱ�� */
    static void HRTPlayAlarm(CWnd* pUser); /* �澯�����Ŷ�ʱ�� */
    static void HRTReadValve(CWnd* pUser); /* ��ȡ����״̬��ʱ�� */
    BOOL Status3Proc(COLLECTDATA* ptCollData, BOOL* pFinish);
    BOOL Status4Proc(COLLECTDATA* ptCollData, BOOL* pFinish);
    BOOL Status254Proc(COLLECTDATA* ptCollData, BOOL* pFinish);
    void CalcPointNum(COLLECTDATA* ptCollData, ORGDATA* ptOrgData);
    void ZeroPointUpdateData(COLLECTDATA* ptCollData, BOOL bFinish);
    void MorePointInsertData(COLLECTDATA* ptCollData, BOOL bFinish);
    void FinishSetStatus();
    void FinishControl();
    void UpdateOutData(double fTorque, int iPoints);
    void UpdateOutData(COLLECTDATA* ptColl);
    BOOL ValidStatus3(COLLECTDATA* ptCollData);
    void SetCommShowInfo(UINT nStatus);   /* ���ô������ͼ�����Ϣ��ʾ״̬ */
    void FillWordValue(BYTE* pAddr, int iValue);
    void FillReadCommand(int iCommand);
    void FillWriteCommand(int iCommand);
    void SavePortNormalInfo(COLLECTDATA* ptCollData); //����һ�����ڶ�ȡ�ļ�¼
    void SavePortMultiDataInfo(COLLECTDATA* ptCollData); //����һ�δ��ڶ�ȡ�Ķ�����¼

    // APP function move into MainDlg
    void IncTorqNo();
    void GetMakeupCurNum();
    //void GetBreakoutCurNum();
    void GetCurNum();
    void ReGetTorqNo();
    void GetCurWellFile();
    void CreateNewWellFile();
    BOOL TimeValidWell(CString strFileName);
    void CheckAppReg();
    BOOL CheckReg(CString strReg[]);
    void SaveTorqNum();

    /* Ť��������غ��� */
    void ResetData();
    BOOL InsertData(COLLECTTORQUE* ptColl, double torque, double rpm);
    void UpdateTorqueData(double torque, double rpm);
    void SaveIntoData(TorqData::Torque* ptPBData);
    void SaveMakeupData(TorqData::Torque* ptPBData);
    void SaveBreakoutData(TorqData::Torque* ptPBData, int iSeqNO, int iOutwellNO);
    void SetQuality(DWORD dwQuality);
    void KillAllTimer();
    void GetValidTorqData();
    void SetTorqDataCfg(TorqData::Torque* ptPBData);
    void SetShowPara(TorqData::Torque* ptPBData);
    BOOL ClearExcepTorq(int iOrgTorq, int& iDestTorq);
    void LightValveStatus(BYTE* pucStatus);
    void CalcDelayCount();
    CDialog* GetDlgPoint(CDialog* pDialog, BYTE uDlgKind, UINT iDlgID);
    BOOL CheckPortData(BYTE* pData, int iLen, BOOL& bFini);
    int  RcvTorqDataProc(COLLECTDATA* ptCollData);
    void UnitChangeTorq();
    void UpdCfgLangChg(UINT nLang);
    void StopPlayAlam();
    int  GetIPPlus(TorqData::Torque* ptTorq, WORD wIPPos);
    double OverTorqOpt(double fTorq, BYTE ucStatus);
    BOOL   JudgeRunStatus(unsigned wInfo);
    void   CanModLastData(BOOL bCan);
    void   CheckBreakOut();
    void   UpdateSeqNO();

    CLineChartCtrlEx m_wndTorque;       /*Ť����ʾ����*/
    CLineChartCtrl   m_wndRpm;          /*ת����ʾ����*/
    BOOL            m_bRunStatus;       /* ����״̬��TRUE����, FALSEֹͣ */
    int             m_iRecPNum;         /* ��һ�μ�¼���ܵĵ����Ŀ */
    int             m_iOutPoints;       /* ��һ�μ�¼�ĵ��ε����Ŀ */
    double          m_fOutTorque;       /* ��һ�μ�¼��Ť�� */
    UINT            m_nInterval;        /* modbus������Ϣ�����ߵ�ʱ��, ����Ϊ200ms */
    BYTE            m_ucSndByte[PORTBUFF]; /* modbus������Ϣ��ֵ */
    HACCEL          m_hAccel;           /* ��ݼ� */
    CStdioFile      m_AutoSavefile;
    bool            m_bAutoFileOpen;    /* ���Ժ����ļ��Ƿ�� */

    int             m_iShowPlus;        /* ��ʾǰ����������ʵ��������Ҫ������ʾ���壬���ܼ���Ȧ�� */
    int             m_iPriorCnt;        /* ֮ǰ���ĵ��� */
    int             m_iPriorPlus;       /* ��һ�������� */
    __time64_t      m_tStartTime;       /* ��ʼ�Ͽ۵�ʱ�� */

    /* ���ö�ʱ��ȡ����ʱ��״̬ */
    UINT            m_nClashSta;

    /* ����������ݱ��� */
    double          m_fTestTorq[TESTNUM];
    int             m_iTestPlus[TESTNUM];
    int             m_iTest;
    BOOL            m_bAlarm;           /* �Ƿ���Ҫ�澯 */

    /* theAppȫ�ֱ���ָ�룬�������� */
    SHOWCFG* m_ptShow;
    PARACFG* m_ptCfg;
    CONTROLPARA* m_ptCtrl;
    COMMONCFG* m_ptComm;
    COLLECTTORQUE   m_tCollData;        /* ��ǰ��Ť�ؽṹ���ݣ����Գ�������ͼ��4�� */
    PORTDATA* m_ptPortData;
    TorqData::Torque   m_tSaveData;        /* ��collectData��ȡ���MAXPOINT���浽saveData */

    /* �߾��ȶ�ʱ�� */
    CHighResTimer   m_hrtReadPort;      /* ���ڶ�ʱ�� */
    CHighResTimer   m_hrtGuard;         /* ������λ��ʱ�� */
    CHighResTimer   m_hrtPortBuff;      /* ���ڻ����� */
    CHighResTimer   m_hrtSaveData;      /* �������ݶ�ʱ�� */
    CHighResTimer   m_hrtSaveDebug;     /* ��ʱ����CRC����͵�����Ϣ��ʱ�� */
    CHighResTimer   m_hrtPlayAlarm;     /* �Ÿ澯����ʱ�� */
    CHighResTimer   m_hrtReadValve;     /* ��ȡ����״̬ */
    double          m_fPreReadTime;     /* ��һ�ζ�ȡ����ʱ��ms */
    /* �ӳ�1s����������״̬3��Ť�ؼ������ */
    UINT            m_nDelayCount;
    UINT            m_nCurDelay;
    int             m_iValveBreakCnt;       /* ��ȡ����״̬ʧ�ܴ���������Ϊ��ʼ5�Σ�С�ڵ���0˵��ͨ���жϣ����һֱ��ȡ������˵������״̬���� */
    int             m_iTorqBreakCnt;
    int             m_iMaxReadTimes;    /* ��������ȡ���� */

    BOOL            m_bCanModLastData;  /* ��λʱ��ǰ�������޸����һ������ */
    //CString         m_strBreakoutPath;

    BOOL            m_bValveStatus[VALVEMAXNUM];
    UINT            m_nValveMark[VALVEMAXNUM];

    UINT            m_nCurRunningNO;       /* �뾮��� */
};

extern CDrillDlg    *thepDlg;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRILLDLG_H__63474767_3526_4CC3_8512_1F93AF2AE779__INCLUDED_)
