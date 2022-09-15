// TorqueDlg.h : header file
//

#if !defined(AFX_TORQUEDLG_H__63474767_3526_4CC3_8512_1F93AF2AE779__INCLUDED_)
#define AFX_TORQUEDLG_H__63474767_3526_4CC3_8512_1F93AF2AE779__INCLUDED_

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

#define STEPTORQUE      20  /* 削平突起时，后一个数据比前一个数据累增的步长 */
#define PRIKETORQ       500 /* 小于这个扭矩且小于台阶扭矩，不削平 */

/* MODBUS自定义命令 */
#define COM_READ        0x0001  /* 读取数据 */
#define COM_ADJUST      0x0002  /* 校准倍数 */
#define COM_QIPI        0x0003  /* 去皮 */
#define COM_COLLECT     0x0004  /* 采集单片机数据 */
#define COM_SHOW        0x0005  /* 显示扭矩 */
#define COM_SPEED       0x0006  /* 减速扭矩 */
#define COM_CONTROL     0x0007  /* 控制扭矩 */
#define COM_BSPEED      0x0008  /* 开始减速控制 */
#define COM_ESPEED      0x0009  /* 停止减速控制 */
#define COM_BUNLOAD     0x000A  /* 开始卸荷控制 */
#define COM_EUNLOAD     0x000B  /* 停止卸荷控制 */
#define COM_UPPER       0x000C  /* 上限扭矩 */
#define COM_LOWER       0x000D  /* 下限扭矩 */
#define COM_CUT         0x000E  /* 打折系数 */
#define COM_TIME        0x000F  /* 设置时间 */
#define COM_VPRESS      0x0010  /* 减压阀 */
#define COM_VFLOW       0x0011  /* 流量阀 */
#define COM_VZOOM       0x0012  /* 放大倍数 */
#define COM_READVALVE   0x0014  /* 读取阀门状态 */
#define COM_READMULTI   0x0015  /* 读取多组数据 */
#define COM_WRITECALIB  0x0017  /* 分段校准 写 */
#define COM_CTRLCALIB   0x0018  /* 分段校准控制 */
#define COM_READCALIB   0x0019  /* 分段校准 读 */
/* 单片机命令定义 */
#define SCMREAD         15      /* 读取数据 */
#define SCMADJUST       16      /* 校准 */
#define SCMQIPI         17      /* 去皮 */
#define SCMCOLLECT      18      /* 采集数据 */
#define SCMSHOW         19      /* 显示扭矩 */
#define SCMSPEED        20      /* 减速扭矩 */
#define SCMCONTROL      21      /* 控制扭矩 */
#define SCMBSPEED       22      /* 开始减速 */
#define SCMESPEED       23      /* 停止减速 */
#define SCMBUNLOAD      24      /* 开始卸荷 */
#define SCMEUNLOAD      25      /* 停止卸荷 */
#define SCMUPPER        26      /* 上限扭矩 */
#define SCMLOWER        27      /* 下限扭矩 */
#define SCMCUT          28      /* 打折系数 */
#define SCMCOLLECTOK    29      /* 收到有效数据再次采集数据：最后一次确认后不需要再次收到数据 */
#define SCMCOLLECTNOK   30      /* 收到无效数据再次采集数据 */
#define SCMTIME         31      /* 设置单片机时间 */
#define SCMVPRESS       32      /* 减压阀 */
#define SCMVFLOW        33      /* 流量阀 */
#define SCMREADVALVE    34      /* 读取阀门状态 */
#define SCMREADMULTI    35      /* 读取多组数据 */
#define SCMWRITECALIB   36      /* 写分段校准 */
#define SCMCTRLCALIB    37      /* 分段校准控制 */
#define SCMREADCALIB    38      /* 读分段校准 */
// #define SCMVZOOM        34      /* 放大倍数 */
/*#define CIRNUMS         4615.4*/    /* 1200脉冲为一小周,100小周为26大周,1200/0.26= */

#define RS_COMM_CLOSE   0   /* 串口关闭 */
#define RS_COMM_OPEN    1   /* 串口打开 */
/* 串口测试打开，值为 m_nTestFunc+1 */
#define RS_COMM_RAND    2   /* COLL_RAND+1 */
#define RS_COMM_TORQUE  3   /* COLL_TORQUE+1 */
#define RS_COMM_HISTORY 4   /* COLL_HISTORY+1 */
#define RS_COMM_MULTITORQ  5   /* COLL_MULTITORQ+1 */
/* #define COLL_RAND           1
   #define COLL_TORQUE         2
   #define COLL_HISTORY        3
   #define COLL_MULTITORQ      4
*/

/* 目前低4位有效，由高到低分别代表：卸扣、上扣、流量阀、压力阀、卸荷阀、减速阀。=1 表示错误 */
#define VALVEMAXNUM             6
#define VALVESTATUS_SPEED       0x0001
#define VALVESTATUS_UNLOAD      0x0002
#define VALVESTATUS_PRESS       0x0004
#define VALVESTATUS_FLOW        0x0008
#define VALVESTATUS_MAKEUP      0x0010
#define VALVESTATUS_BREAKOUT    0x0020

#define PORTOPR_OPEN    0   /* 打开串口 */
#define PORTOPR_CLOSE   1   /* 关闭串口 */
#define PORTOPR_MAXNUM  (PORTOPR_CLOSE +1)

#define COLLECTPOINTS   5*MAXLINEITEM
#define TESTNUM         COLLECTPOINTS

/*
    0：待机/反转、数据丢弃
    1：正常显示；
    2：减速
    3：卸荷
    4: 抱死
    CC: 上扣/卸扣反转，扭矩奇数/偶数互换
    F0: 多个数据时，数据重复，该条数据需要跳过continue
    FE:卸扣结束254
    FF:CRC ERROR
*/
#define     PLCSTATUS_WAIT      0
#define     PLCSTATUS_NORMAL    1
#define     PLCSTATUS_DECEL     2
#define     PLCSTATUS_UNLOAD    3
#define     PLCSTATUS_LOCKON    4
#define     PLCSTATUS_REVERSE   0xCC
#define     PLCSTATUS_REPEAT    0xF0
#define     PLCSTATUS_BREAKOUT  0xFE
#define     PLCSTATUS_CRCERR    0xFF

typedef struct tagCOLLECTORQUE
{
    UINT        nCurCount;
    UINT        nAllCount;
    DWORD       dwQuality;
    CTime       tTime;
    double      fTorque[COLLECTPOINTS];
    double      fRpm[COLLECTPOINTS];
    PARACFG     tParaCfg;
}COLLECTTORQUE;

#define         COLLPORTNUM     50000
typedef struct tagPORTDATA
{
    UINT        nSaveCount;
    UINT        nLastPlus;
    double      fTorque[COLLPORTNUM];
    double      fRpm[COLLPORTNUM];
    int         iDelPlus[COLLPORTNUM];
}PORTDATA;


/////////////////////////////////////////////////////////////////////////////
// CTorqueDlg dialog
class CTorqueDlg : public CDialog
{
// Construction
public:
    CTorqueDlg(CWnd* pParent = NULL);   // standard constructor

    /* 串口读取CRC错误时，停止尝试保存数据，重启串口 */
    void        RestartComm();

    BOOL        SendData( UINT nParaType );
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
    //void        FillValveZoom();

    /* 收集数据(包括测试函数) */
    BOOL        CollectPort(COLLECTDATA* ptCollData);
    BOOL        CollectRandData(COLLECTDATA* ptCollData);
    BOOL        CollectTorqData(COLLECTDATA* ptCollData);
    BOOL        CollectMultiTorq(COLLECTDATA* ptCollData);
    BOOL        CollectHisData();
    BOOL        RunIniAutoFile();
    BOOL        PortQueIsEmpty();
    
    BYTE        m_ucRcvByte[PORTBUFF];      /*modbus接收消息数值*/
    WORD        m_wRcvLen;
    CTime       m_tSetTime;
    
    CSerialPort m_tPort;

// Dialog Data
    //{{AFX_DATA(CTorqueDlg)
    enum { IDD = IDD_TORQUE_DIALOG };
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
    UINT        m_nCur;
    UINT        m_nCRCERR;
    UINT        m_nClashERR;
    CString     m_strRecvData;
    UINT        m_nTotal;    
    BOOL        m_bToolBuck;        /* 是否为工具扣 */
    CString     m_strQuality;
    int         m_iShackle;         /* 0: 上扣,1:卸扣 */
    CString     m_strLBM2;
    CString     m_strLBM9;
    CString     m_strMainName[MAXMAINPARA];
    CString     m_strMainValue[MAXMAINPARA];
    //}}AFX_DATA
    
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTorqueDlg)
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
    //{{AFX_MSG(CTorqueDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu );
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
    afx_msg void OnUpdateLangChn(CCmdUI *pCmdUI);
    afx_msg void OnLangEng();
    afx_msg void OnUpdateLangEng(CCmdUI *pCmdUI);
    afx_msg void OnLangRus();
    afx_msg void OnUpdateLangRus(CCmdUI *pCmdUI);
    afx_msg void OnVerBigTorq();
    afx_msg void OnUpdateVerBigTorq(CCmdUI *pCmdUI);
    afx_msg void OnVerUnitNm();
    afx_msg void OnUpdateVerUnitNm(CCmdUI *pCmdUI);
    afx_msg void OnVerUnitLbft();
    afx_msg void OnUpdateVerUnitLbft(CCmdUI *pCmdUI);
    afx_msg void OnModpw();
    afx_msg void OnValveset();
    afx_msg void OnBnClickedSetippoint();
    afx_msg void OnBnClickedSettoolbuck();
    afx_msg void OnBnClickedBtnquality();
    afx_msg void OnBnClickedRadiobuckle();
    afx_msg void OnBnClickedRadioshackle();
    afx_msg void OnBnClickedBtnshowset();
    //}}AFX_MSG
    afx_msg LRESULT CollectTimerOut(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT GuardTimerOut(WPARAM wParam, LPARAM lParam);
    //afx_msg LRESULT ResetTimerOut(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT PortBuffTimerOut(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT SaveDataTimerOut(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT SaveDebugTimerOut(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT ALarmPlayTimerOut(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT ReadValveTimerOut(WPARAM wParam, LPARAM lParam);
    afx_msg LONG OnCommunication(WPARAM ch, LPARAM port);
    afx_msg void OnSegcalib();
    DECLARE_MESSAGE_MAP()

private:
    void InitVariant();
    void InitMainShowPara();
    void InitDlgControl();
    void UpdateDlgLabel();
    void ClearInfo(BOOL bClrTorq = TRUE);
    BOOL ChangeCommParam(BOOL bUpdateText = TRUE);
    WORD GetCRCValue(BYTE *pucCrc,WORD iLen);
    BOOL CompSysPara(const BYTE *pPara1, const BYTE *pPara2, WORD wSize);
    int  GetInsertNum(double fT1, double fT2);
    /* 填充发送串口命令的参数 */
    WORD FillSendByte( UINT nParaType );
    BOOL RunInitRand();
    BOOL RunInitPort();
    void RunTorque();
    void StopTorque();
    void DrawLastPoint();
    BOOL TimeReadPort(UINT nCurStatus);
    BOOL CheckReadInterval();
    BOOL CheckDataValid();
    static void HRTReadPort(CWnd *pUser);  /* 读取串口定时器Timer1到时 */
    static void HRTGuard(CWnd *pUser);     /* 保护复位定时器Timer2到时 */
    //static void HRTGuardRead(CWnd *pUser); /* 复位时读取串口定时器Timer4到时 */
    static void HRTPortBuff(CWnd *pUser);  /* 读串口前缓冲区有数据等待单片机写定时器Timer7到时 */
    static void HRTSaveData(CWnd *pUser);  /* 保存数据定时器Timer8?到时 */
    static void HRTSaveDebug(CWnd *pUser); /* 定时保存CRC错误和调试信息定时器 */
    static void HRTPlayAlarm(CWnd *pUser); /* 告警音播放定时器 */
    static void HRTReadValve(CWnd *pUser); /* 读取阀门状态定时器 */
    BOOL Status3Proc(COLLECTDATA* ptCollData, BOOL *pFinish);
    BOOL Status4Proc(COLLECTDATA* ptCollData, BOOL *pFinish);
    BOOL Status254Proc(COLLECTDATA* ptCollData, BOOL *pFinish);
    void CalcPointNum(COLLECTDATA *ptCollData, ORGDATA *ptOrgData);
    void ZeroPointUpdateData(COLLECTDATA *ptCollData, BOOL bFinish);
    void MorePointInsertData(COLLECTDATA *ptCollData, BOOL bFinish);
    void FinishSetStatus();
    void FinishControl();
    void UpdateOutData(double fTorque, int iPoints);
    void UpdateOutData(COLLECTDATA *ptColl);
    BOOL ValidStatus3(COLLECTDATA* ptCollData);
    void SetCommShowInfo(UINT nStatus);   /* 设置串口相关图标和信息显示状态 */
    void FillWordValue(BYTE *pAddr, int iValue);
    void FillReadCommand(int iCommand);
    void FillWriteCommand(int iCommand);
    void SavePortNormalInfo(COLLECTDATA *ptCollData); //保存一条串口读取的记录
    void SavePortMultiDataInfo(COLLECTDATA *ptCollData); //保存一次串口读取的多条记录
    
    void ReGetTorqNo();

    /* 扭矩数据相关函数 */
    void ResetData();
    BOOL InsertData(COLLECTTORQUE *ptColl, double torque, double rpm);
    void UpdateTorqueData(double torque, double rpm);
    void SaveIntoData(TorqData::Torque *ptPBData);
    void SetQuality(DWORD dwQuality);
    void KillAllTimer();
    void GetValidTorqData();
    void SetTorqDataCfg(TorqData::Torque *ptPBData);
    void SetShowPara(TorqData::Torque *ptPBData);
    BOOL ClearExcepTorq(int iOrgTorq, int& iDestTorq);
    void LightValveStatus(BYTE *pucStatus);
    void CalcDelayCount();
    CDialog *GetDlgPoint(CDialog * pDialog, BYTE uDlgKind, UINT iDlgID);
    BOOL CheckPortData(char *pData, int iLen, BOOL &bFini);
    int  RcvTorqDataProc(COLLECTDATA *ptCollData);
    void UnitChangeTorq();
    void UpdCfgLangChg(UINT nLang);
    void StopPlayAlam();
    int  GetIPPlus(TorqData::Torque *ptTorq, WORD wIPPos);
    double OverTorqOpt(double fTorq, BYTE ucStatus);

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
    BOOL            m_bAutoFileOpen;    /* 调试函数文件是否打开 */
    
    int             m_iShowPlus;        /* 显示前的脉冲数，实际脉冲需要减掉显示脉冲，才能计算圈数 */
    int             m_iPriorCnt;        /* 之前屏的点数 */
    int             m_iPriorPlus;       /* 上一次脉冲数 */
    //int             m_iReadTimes;       /* 读端口次数 */
    __time64_t      m_tStartTime;       /* 开始上扣的时间 */

    /* 设置定时读取串口时的状态 */
    UINT            m_nClashSta;

    /* 随机生成数据变量 */
    double          m_fTestTorq[TESTNUM];
    int             m_iTestPlus[TESTNUM];
    int             m_iTest;
    BOOL            m_bAlarm;           /* 是否需要告警 */

    /* theApp全局变量指针，方便引用 */
    SHOWCFG         *m_ptShowCfg;
    CONTROLPARA     *m_ptCtrl;
    COMMONCFG       *m_ptComm;
    COLLECTTORQUE   m_tCollData;        /* 当前的扭矩结构数据，可以超过正常图形4倍 */
    PORTDATA        *m_ptPortData;
    TorqData::Torque   m_tSaveData;        /* 从collectData获取最后MAXPOINT保存到saveData */

    /* 高精度定时器 */
    CHighResTimer   m_hrtReadPort;      /* 串口定时器 */
    CHighResTimer   m_hrtGuard;         /* 保护复位定时器 */
    //CHighResTimer   m_hrtGuardRead;     /* 复位时读取串口定时器 */
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
    
    CString         m_strTubeNO2;       /* 管体编号 第二段 */
    BOOL            m_bCanModLastData;  /* 复位时间前，可以修改最后一条数据 */

    BOOL            m_bValveStatus[VALVEMAXNUM];
    UINT            m_nValveMark[VALVEMAXNUM];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TORQUEDLG_H__63474767_3526_4CC3_8512_1F93AF2AE779__INCLUDED_)
