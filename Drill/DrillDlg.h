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

//#define  TESTRUN   /* 测试定义宏值 */

/////////////////////////////////////////////////////////////////////////////
// CDrillDlg dialog
class CDrillDlg : public CDialog
{
    // Construction
public:
    CDrillDlg(CWnd* pParent = NULL);   // standard constructor

    /* 串口读取CRC错误时，停止尝试保存数据，重启串口 */
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

    /* 填写ModBus命令，函数数组统一调用 */
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

    /* 收集数据(包括测试函数) */
    BOOL        CollectPort(COLLECTDATA* ptCollData);
    BOOL        CollectRandData(COLLECTDATA* ptCollData);
    BOOL        CollectTorqData(COLLECTDATA* ptCollData);
    BOOL        CollectMultiTorq(COLLECTDATA* ptCollData);
    BOOL        CollectHisData();
    BOOL        RunIniAutoFile();
    BOOL        PortQueIsEmpty();

    BOOL    GetVolMacFromRegStr(CString strReg[], DWORD pdwVol[], DWORD pdwMac[], int& iYear, int& iMonth, int& iDay);
    BOOL    GetVolMacInfo(DWORD pdwVol[], DWORD pdwMac[], int iYear, int iMonth, int iDay);

    BYTE        m_ucRcvByte[PORTBUFF];      /*modbus接收消息数值*/
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
    //CString     m_strCircle;  /* m_fCircle 大于1周时，显示当前周数/最大周数 */
    CString     m_strTorque;  /* m_fTorque 界面显示的扭矩；卸扣时显示　当前扭矩/最大扭矩 */
    double      m_fRpm;     /* 单片机上读取到的速度，为0时根据圈数计算 */
    BOOL        m_bComm;
    CString     m_strSeqNO;
    UINT        m_nCRCERR;
    UINT        m_nClashERR;
    CString     m_strRecvData;
    DWORD       m_dwTotalTorqNum;           /* 扭矩上扣最大数目 */
    BOOL        m_bToolBuck;        /* 是否为工具扣 */
    BOOL        m_iBreakOut;        /* 是否进行卸扣操作 0:上扣；1：卸扣*/
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
    /* 填充发送串口命令的参数 */
    WORD FillSendByte(UINT nParaType);
    BOOL RunInitRand();
    BOOL RunInitPort();
    void RunTorque();
    void StopTorque();
    void DrawLastPoint();
    BOOL TimeReadPort(UINT nCurStatus);
    BOOL CheckReadInterval();
    BOOL CheckDataValid();
    static void HRTReadPort(CWnd* pUser);  /* 读取串口定时器Timer1到时 */
    static void HRTGuard(CWnd* pUser);     /* 保护复位定时器Timer2到时 */
    static void HRTPortBuff(CWnd* pUser);  /* 读串口前缓冲区有数据等待单片机写定时器Timer7到时 */
    static void HRTSaveData(CWnd* pUser);  /* 保存数据定时器Timer8?到时 */
    static void HRTSaveDebug(CWnd* pUser); /* 定时保存CRC错误和调试信息定时器 */
    static void HRTPlayAlarm(CWnd* pUser); /* 告警音播放定时器 */
    static void HRTReadValve(CWnd* pUser); /* 读取阀门状态定时器 */
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
    void SetCommShowInfo(UINT nStatus);   /* 设置串口相关图标和信息显示状态 */
    void FillWordValue(BYTE* pAddr, int iValue);
    void FillReadCommand(int iCommand);
    void FillWriteCommand(int iCommand);
    void SavePortNormalInfo(COLLECTDATA* ptCollData); //保存一条串口读取的记录
    void SavePortMultiDataInfo(COLLECTDATA* ptCollData); //保存一次串口读取的多条记录

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

    /* 扭矩数据相关函数 */
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

    CLineChartCtrlEx m_wndTorque;       /*扭矩显示界面*/
    CLineChartCtrl   m_wndRpm;          /*转速显示界面*/
    BOOL            m_bRunStatus;       /* 运行状态：TRUE运行, FALSE停止 */
    int             m_iRecPNum;         /* 上一次记录的总的点的数目 */
    int             m_iOutPoints;       /* 上一次记录的单次点的数目 */
    double          m_fOutTorque;       /* 上一次记录的扭矩 */
    UINT            m_nInterval;        /* modbus发送消息后休眠的时间, 现在为200ms */
    BYTE            m_ucSndByte[PORTBUFF]; /* modbus发送消息数值 */
    HACCEL          m_hAccel;           /* 快捷键 */
    CStdioFile      m_AutoSavefile;
    bool            m_bAutoFileOpen;    /* 调试函数文件是否打开 */

    int             m_iShowPlus;        /* 显示前的脉冲数，实际脉冲需要减掉显示脉冲，才能计算圈数 */
    int             m_iPriorCnt;        /* 之前屏的点数 */
    int             m_iPriorPlus;       /* 上一次脉冲数 */
    __time64_t      m_tStartTime;       /* 开始上扣的时间 */

    /* 设置定时读取串口时的状态 */
    UINT            m_nClashSta;

    /* 随机生成数据变量 */
    double          m_fTestTorq[TESTNUM];
    int             m_iTestPlus[TESTNUM];
    int             m_iTest;
    BOOL            m_bAlarm;           /* 是否需要告警 */

    /* theApp全局变量指针，方便引用 */
    SHOWCFG* m_ptShow;
    PARACFG* m_ptCfg;
    CONTROLPARA* m_ptCtrl;
    COMMONCFG* m_ptComm;
    COLLECTTORQUE   m_tCollData;        /* 当前的扭矩结构数据，可以超过正常图形4倍 */
    PORTDATA* m_ptPortData;
    TorqData::Torque   m_tSaveData;        /* 从collectData获取最后MAXPOINT保存到saveData */

    /* 高精度定时器 */
    CHighResTimer   m_hrtReadPort;      /* 串口定时器 */
    CHighResTimer   m_hrtGuard;         /* 保护复位定时器 */
    CHighResTimer   m_hrtPortBuff;      /* 串口缓冲满 */
    CHighResTimer   m_hrtSaveData;      /* 保存数据定时器 */
    CHighResTimer   m_hrtSaveDebug;     /* 定时保存CRC错误和调试信息定时器 */
    CHighResTimer   m_hrtPlayAlarm;     /* 放告警音定时器 */
    CHighResTimer   m_hrtReadValve;     /* 读取阀门状态 */
    double          m_fPreReadTime;     /* 上一次读取数据时间ms */
    /* 延迟1s结束，避免状态3后扭矩继续冲高 */
    UINT            m_nDelayCount;
    UINT            m_nCurDelay;
    int             m_iValveBreakCnt;       /* 读取阀门状态失败次数，设置为初始5次，小于等于0说明通信中断，如果一直读取不到，说明串口状态不对 */
    int             m_iTorqBreakCnt;
    int             m_iMaxReadTimes;    /* 串口最大读取次数 */

    BOOL            m_bCanModLastData;  /* 复位时间前，可以修改最后一条数据 */
    //CString         m_strBreakoutPath;

    BOOL            m_bValveStatus[VALVEMAXNUM];
    UINT            m_nValveMark[VALVEMAXNUM];

    UINT            m_nCurRunningNO;       /* 入井序号 */
};

extern CDrillDlg    *thepDlg;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRILLDLG_H__63474767_3526_4CC3_8512_1F93AF2AE779__INCLUDED_)
