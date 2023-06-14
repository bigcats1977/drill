#pragma once
#ifndef WINVER // Specifies that the minimum required platform is Windows Vista.   
#define WINVER 0x0601 // WIN7   
#endif 

#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <time.h> 
#include <afxdb.h>
#include <io.h>
#include <math.h>

using namespace std;

#include "DrillData.pb.h"

/* 程序的常用全局宏定义 */
#pragma region MACRO DEFINE

#pragma region MESSAGE ID
#define WM_USER_SHOW_EDIT       WM_USER + 1002

#define WM_COLLECTTIMEROUT      WM_USER + 5001
#define WM_GUARDTIMEROUT        WM_USER + 5002
#define WM_GUARDREADTIMEROUT    WM_USER + 5003
#define WM_PORTBUFFTIMEROUT     WM_USER + 5004
#define WM_COLLECTSCMTIMEROUT   WM_USER + 5005
#define WM_SAVEDATATIMEROUT     WM_USER + 5006
#define WM_SAVEDEBUGTIMEROUT    WM_USER + 5007
#define WM_ALARMPLAYTIMEROUT    WM_USER + 5008
#define WM_READVALVETIMEROUT    WM_USER + 5009
#define WM_WITSRPTTIMEROUT      WM_USER + 5010

#define WM_RE_READDATA          WM_USER + 5021

#define WM_INTERPT_CHANGE       WM_USER + 6001
#define WM_UPDATE_SELPOS        WM_USER + 6002
#define WM_INTERPT_ZOOMIN       WM_USER + 6003
#define WM_COLLECT_DATA         WM_USER + 6004
#define WM_CALIB_DATA           WM_USER + 6005
#pragma endregion

#pragma region TIMERID DUR
/*定时器ID定义*/
#define COOLCONTROL_TIMER       0       // COOL控件使用定时器
#define PORTREAD_TIMER          1       // 读取串口数据定时器
#define GUARD_TIMER             2       // 上扣后保护定时器
#define GUARDREAD_TIMER         4       // 上扣保护时读取串口数据的定时器
#define BEARSHOW_TIMER          5       // 显示肩负选择对话框定时器
#define BEARSHOW_LEN            5000    // 程序启动前选择肩负和大扭矩对话框等待时间
#define AUTOSAVE_TIMER          6       // 定时保存CRC错误和调试信息定时器
#define AUTOSAVE_TLEN           5000    // 定时保存CRC错误和调试信息定时时长
#define PORTBUFF_TIMER          7       // 485串口保护定时器，上位机发送命令前，如果串口有数据，定时保护
                                    // 单片机发送完成后，再发送命令
#define PORTBUFF_TLEN           15      // 串口发送12个BYTE需要13ms,定时器设置为15ms
#define WITSRPT_TIMER           8       // 定时通过TCP上报WITS数据给采集终端
#define WITSRPT_TLEN            1000    // 1s上报一组数据
#define COLLECT_TIMER           10      // 收集单片机数据定时器
#define COLLECT_TLEN            1000    // 收集单片机数据定时时长 //2000
#define WNDSHOW_TIMER           11      // 帮助旋转显示定时器
#define WNDSHOW_TLEN            100     // 帮助旋转显示定时时长
#define RESET_TIMER             12
#define ALARM_TIMER             13
#define ALRAM_DELAY             5000    /* 告警音播放时长5s */
#define READVALVE_TIMER         14      /* 定时读取阀门状态定时器，时长1s */
#define READVALVE_LEN           1000
#define SETCALIB_TIMER          15      /* 设置校准信息定时器 */
#define SETCALIB_LEN            400
#pragma endregion

#pragma region DEFAULT FILE
//#define DEFAULTNAME             _T("default.ini")
//#define SHOWCONFNAME            _T("showcfg.ini")
//#define VITTANAME               _T("vitta.ini")
#define CHNDLLNAME              _T("Chinese.dll")
#define ENGDLLNAME              _T("English.dll")
#define RUSDLLNAME              _T("Russian.dll")
#define SQLITEFILE              _T("TorqueCfg.db")

#define TEMPLATE_RPTCHN         _T("RptTmpChn.xlsx")
#define TEMPLATE_RPTENG         _T("RptTmpEng.xlsx")
#define TEMPLATE_GRAPHY         _T("GrpTmp.xlsx")
#pragma endregion

#pragma region LANGUAGE
#define LANGUAGE_CHINESE        0
#define LANGUAGE_ENGLISH        1
#define LANGUAGE_RUSSIAN        2
#define LANGUAGE_NUM            (LANGUAGE_RUSSIAN+1)
#define LANGUAGE_CURRENT        0xFF
#pragma endregion

#pragma region COLOR
#define CLR_ABNORMDATA          RGB(255,0,0)        // 异常数据颜色
#define CLR_TOOLBUCKLE          RGB(0,0,255)        // 工具扣数据颜色
#define CLR_ALTERBACKG          RGB(215,247,241)    // 隔行背景颜色

#pragma region LINECTRL COLOR
#define CTRLBKCOLOR             RGB(0,0,0)
#define CTRLWHITECOLOR          RGB(255,255,255)
#define CTRLBLACKCOLOR          RGB(0,0,0)
#define LC_BKCOLOR              RGB(50,50,50)
#define LC_ZOOMCOLOR            RGB(218,218,218)
#define LC_SAFECOLOR            RGB(0,255,0)
#define LC_ALARMCOLOR           RGB(255,0,0)
#define LC_SPEEDCOLOR           RGB(0,0,255)
#define LC_SHOWCOLOR            RGB(0,255,255)
#define LC_BEARCOLOR            RGB(255,0,0)
#define LC_TAICOLOR             RGB(0,0,255)
#define IP_INFCOLOR             RGB(0,255,0)
#define IP_SELCOLOR             RGB(255,0,255)
#define ZOOM_SELCOLOR           RGB(255,0,255)
//#define IP_INFCOLOR           RGB(160,32,240)
#define LC_HISSAFECLR           RGB(136,0, 21)
#define LC_HISSHOWCLR           RGB(164,82,164)

#define SCATTER_BG              RGB(127,127,127)
#define SCATTER_ACC_CTRL        RGB(0,  255,255)
#define SCATTER_ACC_IP          RGB(0,  255,0  )
#define SCATTER_REJ_CTRL        RGB(127,0,  0  )
#define SCATTER_REJ_IP          RGB(255,0,  0  )
#pragma endregion
#pragma endregion

#pragma region SERIAL PORT

#define PORTOPR_OPEN    0   /* 打开串口 */
#define PORTOPR_CLOSE   1   /* 关闭串口 */
#define PORTOPR_MAXNUM  (PORTOPR_CLOSE +1)

#define PORTBUFF                108

/* 0: 真实串口；1：模拟测试；2：读取dat扭矩数据；3: 读取多行扭矩数据；4：从日志中恢复数据，基于当前数据库和多行处理；5: 读取dat 历史数据； */
#define COLL_PORT               0
#define COLL_RAND               1
#define COLL_TORQUE             2
#define COLL_MULTITORQ          3
#define COLL_RECOVERY           4
#define COLL_HISTORY            5   // no used in glbcfg

/* 全0的读取串口原始数据
    0x21,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x22,0x00,0x00,0x08
    PORT  读   命令码     Torque     PLUS   8B和    CRC    结束
*/
#define PORT485                 0x21    /* 485串口的有效起始值 */
#define BUS_READ                0x03    /* MODBUS读操作 */
#define BUS_WRITE               0x06    /* MODBUS写操作 */
/* MODBUS命令内容起始地址 */
#define MODBUS_CONTENT          m_ucSndByte[4]

#define PORTSENDTIME            200     /* 串口发送命令后的保护时间 sleep 200ms */

/* 设置定时读取串口时的状态 */
/* 奇数为正常状态，需要先读缓冲；偶数为缓冲已经清空，正常发送命令 */
#define RS_NORMAL               0       /* 正常状态，缓冲区为空，直接发送，换成区非空转CLASH状态 */
#define RS_READCLASH            1       /* READ时单片机缓冲非空 */
#define RS_RESETCLASH           2       /* RESET时单片机缓冲非空 */
#define RS_MAX                  RS_RESETCLASH

#define RS_READ_NORMAL          0       /* 正常状态时读串口 */
#define RS_READ_RESET           1       /* 复位状态时读串口 */

#define PORT_MAXDATANUM         10      /* 串口一次最大读取数据个数 */
#define PORT_VALIDLEN           12      /* 有效数据长度为12 */
#define PORT_DATAPLACE          4       /* 实际数据起始位置4 */
#define PORT_DATALEN            7       /* 实际数据长度 7;+1CHECK+2SPEED  */
#define PORT_SUMPLACE           8       /* 前面数据校验和位置 8 */
#define PORT_VLDVLVLEN          14      /* 加2个字节阀门状态的有效数据长度为14 */
#define PORT_MAXVLDDATA         9       /* +阀门数据长度 9; 4+1CHECK+2SPEED+2VALVE */
#define PORT_GRPDATA            4       /* 读取多组数据时，每组有效数据的长度为4 */

#define ORG_DATAPLACE           15      /* 实际数据起始位置15 */
#define ORG_PRNTIMELEN          12      /* 自动保存打印的时间长度 04:05:45.984 : */
#define ORG_PRNDATALEN          12      /* 自动保存打印的数据长度 36 cc 43 56 */

/* MODBUS自定义命令 */
#define COM_READ                0x0001  /* 读取数据 */
#define COM_ADJUST              0x0002  /* 校准倍数 */
#define COM_QIPI                0x0003  /* 去皮 */
#define COM_COLLECT             0x0004  /* 采集单片机数据 */
#define COM_SHOW                0x0005  /* 显示扭矩 */
#define COM_SPEED               0x0006  /* 减速扭矩 */
#define COM_CONTROL             0x0007  /* 控制扭矩 */
#define COM_BSPEED              0x0008  /* 开始减速控制 */
#define COM_ESPEED              0x0009  /* 停止减速控制 */
#define COM_BUNLOAD             0x000A  /* 开始卸荷控制 */
#define COM_EUNLOAD             0x000B  /* 停止卸荷控制 */
#define COM_UPPER               0x000C  /* 上限扭矩 */
#define COM_LOWER               0x000D  /* 下限扭矩 */
#define COM_CUT                 0x000E  /* 打折系数 */
#define COM_TIME                0x000F  /* 设置时间 */
#define COM_VPRESS              0x0010  /* 减压阀 */
#define COM_VFLOW               0x0011  /* 流量阀 */
#define COM_VZOOM               0x0012  /* 放大倍数 */
#define COM_READVALVE           0x0014  /* 读取阀门状态 */
#define COM_READMULTI           0x0015  /* 读取多组数据 */
#define COM_WRITECALIB          0x0017  /* 分段校准 写 */
#define COM_CTRLCALIB           0x0018  /* 分段校准控制 */
#define COM_READCALIB           0x0019  /* 分段校准 读 */
/* 单片机命令定义 */
#define SCMREAD                 15      /* 读取数据 */
#define SCMADJUST               16      /* 校准 */
#define SCMQIPI                 17      /* 去皮 */
#define SCMCOLLECT              18      /* 采集数据 */
#define SCMSHOW                 19      /* 显示扭矩 */
#define SCMSPEED                20      /* 减速扭矩 */
#define SCMCONTROL              21      /* 控制扭矩 */
#define SCMBSPEED               22      /* 开始减速 */
#define SCMESPEED               23      /* 停止减速 */
#define SCMBUNLOAD              24      /* 开始卸荷 */
#define SCMEUNLOAD              25      /* 停止卸荷 */
#define SCMUPPER                26      /* 上限扭矩 */
#define SCMLOWER                27      /* 下限扭矩 */
#define SCMCUT                  28      /* 打折系数 */
#define SCMCOLLECTOK            29      /* 收到有效数据再次采集数据：最后一次确认后不需要再次收到数据 */
#define SCMCOLLECTNOK           30      /* 收到无效数据再次采集数据 */
#define SCMTIME                 31      /* 设置单片机时间 */
#define SCMVPRESS               32      /* 减压阀 */
#define SCMVFLOW                33      /* 流量阀 */
#define SCMREADVALVE            34      /* 读取阀门状态 */
#define SCMREADMULTI            35      /* 读取多组数据 */
#define SCMWRITECALIB           36      /* 写分段校准 */
#define SCMCTRLCALIB            37      /* 分段校准控制 */
#define SCMREADCALIB            38      /* 读分段校准 */
// #define SCMVZOOM             34      /* 放大倍数 */
/*#define CIRNUMS               4615.4*/    /* 1200脉冲为一小周,100小周为26大周,1200/0.26= */

#define RS_COMM_CLOSE           0       /* 串口关闭 */
#define RS_COMM_OPEN            1       /* 串口打开 */

/* 串口测试打开，值为 m_nTestFunc+1 */
#define RS_COMM_RAND            2       /* COLL_RAND+1 */
#define RS_COMM_TORQUE          3       /* COLL_TORQUE+1 */
#define RS_COMM_MULTITORQ       4       /* COLL_MULTITORQ+1 */
#define RS_COMM_RECOVERY        5       /* COLL_RECOVERY+1 */
#define RS_COMM_HISTORY         6       /* COLL_HISTORY+1 */
/* #define COLL_RAND           1
   #define COLL_TORQUE         2
   #define COLL_HISTORY        3
   #define COLL_MULTITORQ      4
*/

#pragma endregion

#pragma region TORQUE CONTROL
#define RforCur                 2
#define MIN_TORQDATALEN         0xFF

#define RPM_OPTNUM              5       /* RMP优化的数目 */
#define RMP_OPTTHRES            20      /* RMP需要优化的阈值 */
/* 20220215 实际上扣时超扭矩处理 */
#define SWITCHLOWLIMIT          5000    /* 控制扭矩小于该值时，按最小扭矩下发控制扭矩 */

/* 参数设置调整默认值 */
#define DIFF_TORQUE             10
#define DIFF_CIRCUIT            0.1
#define DIFF_TIME               1
#define FULLCIR4SAVE            0.20    // 采集频率 1/2500  1周/(500/0.2)
#define AUTOUPDTURNRATIO        0.8     // 超过80%(400)点自动增加周数

#define IP_SLOPE_PER            1       /* 默认拐点百分比 */


#define MAXREVERSEPLUS          300     /* 20201219 最大反转脉冲暂定300 */

#define STEPTORQUE              20      /* 削平突起时，后一个数据比前一个数据累增的步长 */
#define PRIKETORQ               500     /* 小于这个扭矩且小于台阶扭矩，不削平 */

// 上扣/卸扣数据类型定义
#define TYPE_MAKEUP             0x01    // 0001
#define TYPE_BREAKOUT           0x02    // 0010
#define TYPE_TOTAL              0x03    // 0011

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
#define PLCSTATUS_WAIT          0
#define PLCSTATUS_NORMAL        1
#define PLCSTATUS_DECEL         2
#define PLCSTATUS_UNLOAD        3
#define PLCSTATUS_LOCKON        4
#define PLCSTATUS_REVERSE       0xCC
#define PLCSTATUS_REPEAT        0xF0
#define PLCSTATUS_BREAKOUT      0xFE
#define PLCSTATUS_CRCERR        0xFF

/* 目前低4位有效，由高到低分别代表：卸扣、上扣、流量阀、压力阀、卸荷阀、减速阀。=1 表示错误 */
#define VALVEMAXNUM             6
#define VALVESTATUS_SPEED       0x0001
#define VALVESTATUS_UNLOAD      0x0002
#define VALVESTATUS_PRESS       0x0004
#define VALVESTATUS_FLOW        0x0008
#define VALVESTATUS_MAKEUP      0x0010
#define VALVESTATUS_BREAKOUT    0x0020

#define INDEX_TORQ_MAXLIMIT         0       /* 最大上限 */
#define INDEX_TORQ_UPPERLIMIT       1       /* 最大扭矩 */
#define INDEX_TORQ_CONTROL          2       /* 控制扭矩 */
#define INDEX_TORQ_OPTIMAL          3       /* 最佳扭矩 */
#define INDEX_TORQ_LOWERLIMIT       4       /* 最小扭矩 */
#define INDEX_TORQ_SPEEDDOWN        5       /* 减速扭矩 */
#define INDEX_TORQ_SHOW             6       /* 显示扭矩 */
#define INDEX_TORQ_BEAR             7       /* 肩负扭矩 */
#define INDEX_TORQ_UPPERTAI         8       /* 最大台阶扭矩 */
#define INDEX_TORQ_LOWERTAI         9       /* 最小台阶扭矩 */
#define INDEX_TORQ_INITBO           10      /* 卸扣初始扭矩 */
#define MAXTORQCONFNUM              (INDEX_TORQ_INITBO+1)


#define INDEX_TURN_MAXLIMIT         0       /* 最大周数 */
#define INDEX_TURN_UPPERLIMIT       1       /* 上限周数 */
#define INDEX_TURN_CONTROL          2       /* 控制周数 */
#define INDEX_TURN_LOWERLIMIT       3       /* 下限周数 */
#define INDEX_TURN_MAXDELTA         4       /* 最大Delta周数0.1 */
#define INDEX_TURN_MINDELTA         5       /* 最大Delta周数0.1 */
#define MAXTURNCONFNUM              (INDEX_TURN_MINDELTA+1)

#pragma endregion

#pragma region STATISTICS

#define SHEET_COVER     _T("Cover")
#define SHEET_SUMMARY   _T("Page1")
#define SHEET_QUALITY   _T("Page2")
#define SHEET_SCATTER   _T("Page3")
#define SHEET_REPORT    _T("Page4")

//#define SHOWPARA_WELLNAME           1
//#define SHOWPARA_COMPANY            3
//#define SHOWPARA_TUBEOEM            4
#define SHOWPARA_TUBETYPE           5

/* 输出excel统计报告常量 */
#define         STATPARA_GENNUM         4
#define         STATPARA_JOBNUM         6       /* 总结(Page1)最多 6个显示参数, 3占4格 / 4占8格 */
#define         STATPARA_INFONUM        2       /* 报表(Page4)最多 3个显示参数 */
#define         MAX1VALUES              4
#define         MAX2VALUES              8
//#define         MAX3VALUES              4

#define         STATPARA_GENWELLNO      0
#define         STATPARA_GENCOMPANY     1
#define         STATPARA_GENOPERATOR    2
#define         STATPARA_GENTALLY       3
//#define         STATPARA_GENWELLDEPTH   1

#pragma endregion

#pragma region TUBE CONFIG

#define     INDEX_TUBE_FACTORY      0
#define     INDEX_TUBE_OEM          1
#define     INDEX_TUBE_SIZE         2
#define     INDEX_TUBE_MATER        3
#define     INDEX_TUBE_COUPL        4
#define     MAXTUBECFGNUM           (INDEX_TUBE_COUPL+1)

#define     INDEX_SHOW_FACTORY      0
#define     INDEX_SHOW_OEM          4
#define     INDEX_SHOW_SIZE         5
#define     INDEX_SHOW_MATER        6
#define     INDEX_SHOW_COUPL        7

#define     INDEX_TUBE_MAXTORQ      0
#define     INDEX_TUBE_OPTTORQ      1
#define     INDEX_TUBE_MINTORQ      2
#define     INDEX_TUBE_LBMAXTORQ    3
#define     INDEX_TUBE_LBOPTTORQ    4
#define     INDEX_TUBE_LBMINTORQ    5
#define     MAXTUBETORQNUM          (INDEX_TUBE_LBMINTORQ+1)


#define     MAXDEFFACTORYNUM        8
#define     MAXDEFOEMNUM            34
#define     MAXDEFSIZENUM           105
#define     MAXDEFMATERNUM          33
#define     MAXDEFCOUPLNUM          81

#define     MAXDEFTUBECFGNUM        131
#pragma endregion

#pragma region DEBUGINFO
/* 保存调试信息约定消息头宏值和字符串 */
#define DBG_HASH                    0   /* # */
#define DBG_START                   1   /* * reset */
#define DBG_COLLECT                 2   /* collect */
#define DBG_MESSAGE                 3   /* MessageBox显示信息 */
#define DBG_SNDCMD                  4   /* 发送串口请求 */
#define DBG_RCVCOM                  5   /* 接收串口消息 */
#define DBG_SNDTCP                  6   /* 发送TCP消息 */
#define DBG_MAXNUM                  (DBG_SNDTCP+1)
/* 调试信息头的长度固定为4 */
#define DBG_HEADLEN                 5
#pragma endregion

#pragma region MAX RANGE

#define     MAXPWLEN            32
#define     MAXSKIPLEN          64  /* 总共跳64个字节 */

#define     MAXPROBUFF          819200  // 3.22 buf扩大2倍
#define     OVERFLOWTORQ        300000  /* 溢出的扭矩值，超过该值肯定无效 */

//#define     VITTANAMENUM        50

#define     SPRINTFLEN          60

#define     MAXCMDNUM           24
/* 最大通信中断时间5s */
#define     MAXCOMMBREAKTIME    5000

#define     DEFIPDELTAVAL               0.1

#define     DEFTALLYNAME                _T("入井序号")
#pragma endregion

#pragma region STATUS
/* 定义程序的状态 */
#define STATUS_INVALID          0   /* 无效状态 */
#define STATUS_START            1   /* 程序启动状态 */
#define STATUS_EXIT             2   /* 程序退出状态 */
#define STATUS_RUN              3   /* 程序运行状态 */
#define STATUS_STOP             4   /* 程序停止状态 */
#define STATUS_SETPARA          5   /* 参数设置状态 */
#define STATUS_FUNTEST          6   /* 功能调试状态 */
#define STATUS_COLLECT          7   /* 收集单片机数据状态 */
#define STATUS_HISTORY          8   /* 读取历史数据状态 */
#define STATUS_RESTART          9   /* 重新打开串口 */
#define STATUS_CHGLAN           10  /* 改变语言重新打开窗口 */
#define STATUS_HISSTAT          11  /* 上井数据统计 */
#define STATUS_SETVALVE         12  /* 设置比例阀参数 */
#define STATUS_CIRCLE           13  /* 横轴为周版本 */
#define STATUS_TIME             14  /* 横轴为时间版本 */
#define STATUS_SETSHOW          15  /* 修改界面显示参数 */
#define STATUS_CHGUNIT          16  /* 修改扭矩单位 */
#define STATUS_CALIB            17  /* 校准 */
#define STATUS_TUBECFG          18  /* 油管参数设置 */
#define STATUS_MAXNUM           (STATUS_TUBECFG +1)
#pragma endregion

#pragma region DATABASE
/* 定义程序的状态 */
#define D_LANGUAGE              0   /* 语言字典表 */
#define T_SHOWNAME              1   /* 显示参数名称表 */
#define T_GLBCFG                2   /* 全局配置表 */
#define T_SHOWCFG               3   /* 显示参数配置表 */
#define T_SHOWOPTION            4   /* 显示参数选项表 */
#define T_VALTORQUE             5   /* 扭矩控制值配置表 */
#define T_VALTURN               6   /* 周数控制值配置表 */
#define T_TORQUECFG             7   /* 程序参数值配置表 */
#define T_XLSSTATCFG            8   /* 导出excel统计配置表 */
#define T_VALVECFG              9   /* 比例阀配置表 */
#define T_SERVERCFG             10  /* FTP服务器配置表 */
#define T_WITSCFG               11  /* WITS(TCP)配置表 */
#define MAXTABLENUM             (T_WITSCFG +1)

#define DB_INVALID_VAL          -1
#define DB_INVALID_UINT         2147483647

#define DB_INIT_SUCCESS         0x0000
#define DB_INIT_GLOBAL          0x0001
#define DB_INIT_SHOW            0x0002
#define DB_INIT_XLS_STAT        0x0004
#define DB_INIT_TORQUE_CFG      0x0008
#define DB_INIT_TUBING_INFO     0x0010
#define DB_INIT_TUBING_CFG      0x0020
#define DB_INIT_VALVE_CFG       0x0040
#define DB_INIT_SERVER_CFG      0x0080
#define DB_INIT_REJECT_CAUSE    0x0100
#define DB_INIT_WITS_CFG        0x0200
#pragma endregion

#pragma region SHOW PARAMETER
//#define         MAXNAMENUM              15
#if 0
/* 通用 6 */
#define         COMMPARA_WELL           1   /* 施工井号 */
#define         COMMPARA_TEAM           2   /* 作 业 队 */
#define         COMMPARA_CONTRACT       3   /* 合同号 */
#define         COMMPARA_OPERATOR       4   /* 操作员 */
#define         COMMPARA_BELTLINE       5   /* 生产线号 */
#define         COMMPARA_STAND          6   /* 执行标准 */
#define         COMMPARA_OPERATION      7   /* 作业方式 */
#define         COMMPARA_PARTYA         8   /* 甲方 */
#define         COMMPARA_CASINGTEAM     9   /* 油套管队 */
#define         COMMPARA_SHIFTLEADER    10  /* 当班班长 */
/* 管体参数  */
/* 宝钢 7 */
#define         TUBEPARA_DIAMETER       100 /* 管体外径 */
#define         TUBEPARA_THICKEN        101 /* 管体壁厚 */
#define         TUBEPARA_GRADE          102 /* 管体钢级 */
#define         TUBEPARA_ENDTYPE        103 /* 管端类型 */
#define         TUBEPARA_TUBENO         104 /* 管体号 */
#define         TUBEPARA_STOVE          105 /* 管体炉号 */
#define         TUBEPARA_BATCH          106 /* 管体批号 */
/* 凯泰名称 3 */
#define         TUBEPARA_NAME           120 /* 管件名称 */
#define         TUBEPARA_MAKEUPTYPE     121 /* 上扣类型 */
#define         TUBEPARA_BUCKLETYPE     122 /* 扣型材质 */
#define         TUBEPARA_FACTORY        123 /* 管件厂家 */
#define         TUBEPARA_RUNNINGNO      124 /* 入井序号 */
/* 接箍参数  */
/* 宝钢 6 */
#define         COUPPARA_DIAMETER       201 /* 接箍外径 */
#define         COUPPARA_LENGTH         202 /* 接箍长度 */
#define         COUPPARA_MATER          203 /* 接箍材质 */
#define         COUPPARA_STOVE          204 /* 接箍炉号 */
#define         COUPPARA_BATCH          205 /* 接箍批号 */
#define         COUPPARA_COUPNO         206 /* 接箍号 */
/* 凯泰名称 3 */
#define         COUPPARA_WEIGHT         220 /* 重量 */
#define         COUPPARA_LUBRICANT      221 /* 润滑脂 */
#define         COUPPARA_COUPLING       222 /* 接箍参数 */
#define         COUPPARA_THREADDOPE     223 /* 丝扣油 */
#define         COUPPARA_HANDDEVICE     224 /* 悬吊工具 */
#define         COUPPARA_HYDTONG        225 /* 液压钳 */
#define         COUPPARA_OEM            226 /* 厂家 */
#endif
#define         HALFPARALEN             25
#define         MAXPARALEN              50

#define         MAXPARANUM              16  /* 0 Factory + 15  */
#define         MAXMAINPARA             8   /* 0 Factory + 7个 */
#define         MAXOPTIONNUM            50
#define         MAINSHOWTUBE            5   /* 第5个主界面显示参数为管体序号(5) */
#define         MAINSHOWTALLY           6   /* 第6个主界面显示参数为入井序号(6) */

/* 15个参数, 4~7固定 */
#define         FIXSHOWBEGIN            4   /* 第4个显示参数固定(4) */
#define         FIXSHOWEND              7   /* 第7个显示参数固定(7) */
//#define         TUBESN                  9   /* 管体序号 */
//#define         TALLYNO                 10  /* 入井序号 */

/* 主界面7个参数, 1~6固定 */
#define         MAINSHOWBEGIN           0   /* 第1个显示参数固定(1-1) 0 for Factory*/
#define         MAINSHOWEND             4   /* 第4个显示参数固定(4-1) */

//#define         CBMAINSHOW_FACTORY      0
//#define         CBMAINSHOW_OEM          1
//#define         CBMAINSHOW_SIZE         2
//#define         CBMAINSHOW_MAT          3
//#define         CBMAINSHOW_COUPL        4

/* DlgParaSet 系统管材的combobox对应显示参数的序号 */
#define         FIXINDEX_FACTORY        0
#define         FIXINDEX_OEM            4
#define         FIXINDEX_SIZE           5
#define         FIXINDEX_MATER          6
#define         FIXINDEX_COUPL          7
#pragma endregion

#pragma region QUALITY

/* dwQuality质量定义约定 */
/*
    bit0    - 1:GOOD; 0:BAD
    后续位表示质量NOK原因
    bit1    - TORQ_LESS_LIMIT   低于扭矩下限
    bit2    - TORQ_MORE_LIMIT   高于扭矩上限
    bit3    - TORQ_MORE_START   起始扭矩大于控制扭矩15%
    bit4    - TORQ_MORE_CTRL    高于控制扭矩10%
    bit5    - CIRC_LESS_LIMIT   低于0.20圈数
    bit6    - CIRC_MORE_LIMIT   高于圈数上限
    bit7    - TRANSLATE         图形超过台阶扭矩(控制扭矩15%)后平移

    bit8    - NOIPPOINT         无拐点扭矩 // 大庆版本不存在
    bit9    - LOW_SHOULD        拐点扭矩<Min shoulder(15%)
    bit10   - HIGHT_SHOULD      拐点扭矩>Max shoulder(70%)
    bit11   - LOW_DELTATURN     < Min D. Tns
    bit12   - HIGHT_DELTATURN   > Max D. Tns
    bit13   - LOW_SLOPE         斜率 < Min Slope
    bit14   - OTHER_CAUSE       其他,根据不同标准，将不属于范围内的归置到其他中
    bit15   - SHACK_INSPECT     卸扣检查        人工设定

    bit16   - GALLING           粘扣            人工设定
    bit17   - HYDTONGSLIP       液压钳打滑      人工设定
    bit18   - THREADNOTCLEAN    丝扣清洗不干净  人工设定
    bit19   - GASSEALINSPECT    气检不合格  人工设定
*/
#define QUA_RESU_QUALITYBIT     0x00000001 /* 质量位为最低位 */
#define QUA_RESU_BAD            0x00000000
#define QUA_RESU_GOOD           0x00000001

#define QUA_TORQ_LESS_LIMIT     0x00000002
#define QUA_TORQ_MORE_LIMIT     0x00000004
#define QUA_TORQ_MORE_START     0x00000008
#define QUA_TORQ_MORE_CTRL      0x00000010
#define QUA_CIRC_LESS_LIMIT     0x00000020
#define QUA_CIRC_MORE_LIMIT     0x00000040
#define QUA_TRANSLATE           0x00000080 /* 图形超过台阶扭矩后平移 */
#define QUA_NOIPPOINT           0x00000100 /* 无拐点扭矩 */
#define QUA_LOW_SHOULD          0x00000200
#define QUA_HIGHT_SHOULD        0x00000400
#define QUA_LOW_DELTATURN       0x00000800
#define QUA_HIGHT_DELTATURN     0x00001000
#define QUA_LOW_SLOPE           0x00002000
#define QUA_OTHER_CAUSE         0x00004000
#define QUA_SHACK_INSPECT       0x00008000  /* 卸扣检查 手工设置 shackle inspection */
#define QUA_GALLING             0x00010000  /* 粘扣 手工设置 */
#define QUA_THREADNOTCLEAN      0x00020000  /* 丝扣清洗不干净 手工设置 */
#define QUA_GASSEALINSPECT      0x00040000  /* 气检不合格   手工设置 */
//#define QUA_HYDTONGSLIP         0x00020000  /* 液压钳打滑 手工设置 删除 */

#define MAX_BAD_CAUSE           18

#define TORQ_NORMAL             0
#define TORQ_BAD_QUALITY        1
#define TORQ_TOOLBUCKLE         2
#pragma endregion

#pragma region OTHER
#define VALID_YEAR              5       // 软件有效期年份
/* 强制设置CTime的长度为4, VC2008认为长度为8，VC6认为为0，两者不一致 */
#define CTIMELEN                4

#define MAX_LOADSTRING          512


//#define BIGPOINT                _T("..")
#define BIGPOINT                _T("\xA1\xF1")
//#define BIGPOINT              "\x00\xB7"

/* Dialog kind */
#define DLG_COLLECTDATA         0
#define DLG_SEGCALIB            1

#define PBHEAD                  0xFFFDFBF9
#define PBHEADLEN               4

#define NULLSTR                 _T("")
/* 1 N.m = 0.737463 lb.ft */
#define NM2LBFT                 0.737463
#define LBFT2NM                 1.35582

#ifndef BFT_BITMAP
#define BFT_BITMAP              0x4d42   // 'BM'
#endif

#pragma endregion

#pragma region Struct WITSREPORT
/* TCP传输协议：
1、帧头 && ，帧尾!!，每个数据之间回车换行分隔（十六进制为0D 0A）
2、每行开头固定项目数字定为80
3、具体传输数据，每行仅一个具体序号数据，一包数据可包含多行数据
4、每根管需传输数据包数，为扭矩数组长度 + 1，具体规定为：
(1)	每包数据固定包含01、02、03数据
(2)	第一包数据传输01~03，11~28
(3)	后续包数据传输01~03，51~53
*/
#define WITSRPT_FIXHEADNUM      3   // 每个报文固定参数: 日期，时间，套管号
#define WITSRPT_REPEATNUM       3   // 重复上报数据: 扭矩，周数，时间
#define WITSRPT_CALPARANUM      5   // 上扣完成后扭矩计算参数，比如上扣扭矩，拐点扭矩，增量扭矩，台阶比，台阶时间等
#define WITSRPT_SHOWPARANUM     15  // 最多15个显示参数

#pragma endregion

#pragma region Struct DEFINE

typedef struct tagCONTROLPARA
{
    /* 扭矩显示相关参数 */
    double      fTorqConf[MAXTORQCONFNUM];
#if 0
    double      fMaxLimit;    /* 最大上限 */
    double      fUpperLimit;  /* 最大扭矩 */
    double      fControl;     /* 控制扭矩 */
    double      fOptTorq;     /* 最佳扭矩 */
    double      fLowerLimit;  /* 最小扭矩 */
    double      fSpeedDown;   /* 减速扭矩 */
    double      fShow;        /* 显示扭矩 */
    double      fBear;        /* 肩负扭矩 */
    double      fUpperTai;      /* 最大台阶扭矩 */
    double      fLowerTai;      /* 最小台阶扭矩 */
#endif
    //double      fCut;         /* 打折比例 0.8 */
    /* 扭拧周数 */
    double      fTurnConf[MAXTURNCONFNUM];
#if 0
    double      fMaxCir;      /* 最大周数 */
    double      fUpperCir;    /* 上限周数 */
    double      fControlCir;  /* 控制周数 */
    double      fLowerCir;    /* 下限周数 */
#endif
    //double      fPlus;        /* 周脉冲数 */
    /* 满屏转速 */
    double      fFullRPM;       /* 画图上的最大转速 */
    double      fMinShlSlope;   /* 最小肩负斜率5.0 */

    WORD        wIPPos;       /* 拐点纵坐标位置，0表示没有拐点,或者是老数据，手工补充拐点 */
    BYTE        ucVer;        /* 0: 2017年数据结构
                                 1: 显示参数名称长度固定25；最多30个显示参数；NLV格式: 显示参数的值长度根据L
                                    0～14：正常显示名称，15 Factory
                                 2: 2022 0Factory, 1~15正常数据 */
    BYTE        ucRsv;
}CONTROLPARA;

typedef struct tagPORTCFG
{
    BYTE        ucPortNo;       /* 串口号 */
    BYTE        ucParity;       /* 奇偶校验 */
    BYTE        ucDataBit;      /* 数据位 */
    BYTE        ucStopBit;      /* 停止位 */
    UINT        nBand;          /* 波特率 */
}PORTCFG;

#define MAXTUBEPARALEN   51
typedef struct tagFixTubePara
{
    UINT        nNO;
    //char        aucName[LANGUAGE_NUM][MAXTUBEPARALEN];
    string      strName[LANGUAGE_NUM];
}FIXTUBEPARA;

typedef struct tagFixTubeInfo
{
    bool        bDbData;
    UINT        nNum;
    FIXTUBEPARA* ptPara;
}FIXTUBEINFO;

//#define  TUBECFGTORQNUM         6
#define  HALFTUBETORQNUM        3
typedef struct tagTubeCfg
{
    UINT    nIndex;
#if 0
    UINT    nFactory;       // 厂家  JFE
    UINT    nOEM;           // 管件厂家  2-7/8″ JFE
    UINT    nSize;          // 管件规格 TubingSize
    UINT    nMat;           // 扣型材质 EUE          N80
    UINT    nCoupling;      // 接箍规格 93.2X    磷化
#else
    UINT    nFixTube[MAXTUBECFGNUM];
#endif
#if 0
    /* N.m */
    UINT    nMaxTorq;       // 最大扭矩
    UINT    nOptTorq;       // 最佳扭矩
    UINT    nMinTorq;       // 最小扭矩
    /* lb.ft */
    UINT    lMaxTorq;       // 最大扭矩
    UINT    lOptTorq;       // 最佳扭矩
    UINT    lMinTorq;       // 最小扭矩
#else
    UINT    nTorqVal[MAXTUBETORQNUM];
#endif
}TUBECFG;

typedef struct tagFixTubeCfg
{
    bool        bDbData;
    UINT        nNum;
    TUBECFG* ptCfg;
}FIXTUBECFG;

//#define MAXMEMOLEN              128
//typedef struct tagCOMMONCFG
//{
//    double      fMaxTorq;       /* 启用，记录数据的最大扭矩 */
//    /* double类型配置 最多16个 */
//    //double      fRpmAdj;        /* 转速调整 */
//    //double      fMulti;         /* 校准参数范围0~2 */
//    //double      fMinShlSlope;   /* 最小肩负斜率5.0 */
//    //double      fMaxDeltaCir;   /* 最大Delta周数0.1 */
//    //double      fMinDeltaCir;   /* 最小Delta周数0.1 */
//    double      fRsv[12];        /* 保留double字段，共16个 */
//
//    /* DWORD类型配置 最多16个 */
//    DWORD       dwSeqNo;
//    DWORD       dwRsv[15];
//
//    /* 开关值设置，每个开关值使用一个BIT位 */
//    /* BIT位定义 最多8*32个 */
//    DWORD       bBear : 1;        /* 是否有肩负,默认为没有 */
//    DWORD       bToolBuck : 1;    /* 是否为工具扣,默认为否 */
//    DWORD       bRsv : 30;
//    DWORD       dwRsv2[7];
//
//    //char        aucMemo[MAXMEMOLEN];        /* 备注 */
//    //char        aucRemark[MAXPARALEN];      /*标注说明remark*/
//}COMMONCFG;

typedef struct tagPARACFG
{
    CONTROLPARA tCtrl;
    //COMMONCFG   tComm;
    TUBECFG     tTubeCfg;               /* 5个油管参数的固化定义，非固化参数在SHOWPARA中 */

    string      strAlias;
    string      strValue[MAXPARANUM];   /* 当前选择的显示参数及值 +1 厂家(0)，参数设置时使用 */

    //string      strAlias;
    string      strMemo;
    string      strRemark;
}PARACFG;

typedef struct tagGLBCFG
{
    UINT        nLangType;      /* 语言类型 */
    //UINT        nParaIndex;     /* 控制参数配置索引 */
    UINT        nPortNO;        /* 串口号 */
    UINT        nBaudRate;      /* 波特率 */
    UINT        nPlusPerTurn;   /* 周脉冲数 */
    UINT        nTorqUnit;      /* 扭矩单位: 0:N.m; 1:lb.ft */
    UINT        nCollectDur;    /* 定时收集数据的时间，ms，默认250 */
    UINT        nResetDur;      /* 复位时间，默认10s */
    UINT        nSaveDur;       /* 大于显示扭矩后保存数据的时间，默认30s */
    /*UINT        nIPShowMode;    /* 拐点显示方式：1: 只画数据中的拐点
                                                2: 只画计算拐点
                                                3: 数据拐点和计算拐点都画 */
    UINT        nZoomIn;        /* 图形放大倍数 */
    UINT        nImgNum;        /* 批量导出图形时，一个图像文件中包含多少个图形 */
    UINT        nTest;          /* 0: 真实串口；1：模拟测试；2：读取dat扭矩数据；3: 读取多行扭矩数据；4：从日志中恢复数据，基于当前数据库和多行处理；5: 读取dat 历史数据； */

    double      fDiscount;      /* fCut 打折比例 0.8 */
    double      fMulti;         /* 校准参数范围0~2 */
    double      fRpmAdj;        /* 转速调整 */
    //double      fIPDeltaVal;    /* 默认0.7 */

    //BOOL        bCheckIP;       /* 质量判断是否启用高级判断，简单判断以控制扭矩为准， 超过最大扭矩和低于最小扭矩为不合格，默认为1 和控件关联*/
    bool        bBigTorq;       /* 是否为大扭矩版本2.4.9, TRUE:和单片机扭矩相差10倍,FALSE:1倍 */
    bool        bDateBehind;    /* 日期在文件命名的后面 */

    string      strPassWord;
    string      strDataPath;

    string      strUnit;        /* 对应扭矩单位的字符串 */
}GLBCFG;

typedef struct tagSERVERCFG
{
    UINT    nFTPPort;
    string  strFTPAddr;
    string  strUserName;
    string  strPassword;
    string  strTargetPath;
}SERVERCFG;

typedef struct tagWITSCFG
{
    UINT    nTCPPort;
    vector<int> ShowParas;
    vector<int> FixItems;
    vector<int> RepeatItems;
    vector<int> CalItems;
    vector<int> ShowItems;
}WITSCFG;

typedef struct tagSHOWOPTION
{
    UINT        nOptNum;
    string      strOpt[MAXOPTIONNUM];
}SHOWOPTION;

typedef struct tagSHOWCFG
{
    UINT        nParaNum;               /* 当前显示参数个数;图形参数个数等于显示参数个数 */
    //UINT        nListNum;               /* 当前列表显示参数个数 */
    UINT        nMainNum;               /* 当前主界面显示参数个数 */
    UINT        nFileName;              /* 命名数据文件的参数，是各个参数中的一个 */
    UINT        nStatType;              /* 统计参数，是各个参数中的一个 */
    UINT        nShow[MAXPARANUM];      /* strShow字符串对应存储的index号 */
    //UINT        nList[MAXPARANUM];      /* strShow的序号，从0开始，根据序号从strShow活动显示值 */
    UINT        nMain[MAXMAINPARA];     /* strShow的序号，从0开始，根据序号从strShow活动显示值 */
    string      strShow[MAXPARANUM];    /* 当前选择的显示参数及值 +1 厂家(0)，参数设置时使用 */
    //string      strValue[MAXPARANUM];   /* 当前选择的显示参数及值 +1 厂家(0)，参数设置时使用 */
    //SHOWOPTION  tOption[MAXPARANUM];  /* 当前显示参数的可选项 +1 厂家(0)，参数设置时使用  */

    UINT        nAlias;
    //bool        bFixTube;
    //TUBECFG     tTubeCfg;               /* 5个油管参数的固化定义，非固化参数在SHOWPARA中 */
}SHOWCFG;

typedef struct tagTorqCfgID
{
    UINT        nTorqueID;
    UINT        nTurnID;
    UINT        nTubeID;
    string      strOptionID;
}TORQCFGID;

/* XLS统计结果参数配置 */
typedef struct tagXLSSTATCFG
{
    //string      strName[MAXPARANUM];    /* 显示参数名称 */
    ///BOOL        bSummary[MAXPARANUM];   /* 摘要(Page1)显示参数序号 */
    //BOOL        bReport[MAXPARANUM];    /* 报告(Page4)显示参数序号 */
    //BYTE        ucOperator;             /* 当班班长(操作者)参数序号 */
    //BYTE        ucTally;                /* 入井序号参数序号 */
    int         GenPara[STATPARA_GENNUM];
    int         JobPara[STATPARA_JOBNUM];       /* 摘要(Page1)显示参数序号 */
    int         InfoPara[STATPARA_INFONUM];     /* 报告(Page4)显示参数序号 */
}XLSSTATCFG;

/*  状态定义参见TorqueDlg.h, 如PLCSTATUS_WAIT
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
typedef struct tagORGDATA
{
    BYTE        ucRcvByte[PORT_DATALEN];
    UINT        nTorque;
    UINT        nPlus;
    BYTE        ucStatus;   /*FF CRC ERROR*/
    BYTE        ucPointNum;
    double      fRpm;
}ORGDATA;

typedef struct tagCOLLECTDATA
{
    double      fTorque;        /*当前扭矩*/
    double      fRpm;           /*当前转速*/
    int         iPointNum;      /*一个数据画图的点数
                                  0;没有转，只是更新扭矩值，不往后画图
                                  1:正常插入一次数据，画一个点
                                  2~N:中间数据丢失或者一次plus比较多，需要画多次数据
                                      程序自动插入多个数据，使图像更加平滑合理*/
    UINT        nOrgPlus;       /* 原始记录的脉冲数；计算转速时，会减去showplus */
    BYTE        ucStatus;       /* 下位机的状态, 同ORGDATA的ucStatus定义 */
}COLLECTDATA;

/* 统计参数设置 */
#define     STATRANGENUM        3
typedef struct tagSTATCFG
{
    double  fCtrlRange[STATRANGENUM];       /* 控件扭矩范围 */
    double  fShouldRange[STATRANGENUM];     /* 拐点扭矩统计范围 */
    double  fDeltaRange[STATRANGENUM];      /* Delta周数统计范围 */
}STATCFG;

#define     MAXWELLNUM   5000
typedef struct tagTORQUEDATA
{
    UINT    nCur;               /* nCur从1开始计数，相当于数组序号+1 */
    UINT    nTotal;
    UINT    nQualy;
    UINT    nUnQualy;
    UINT    nTotalPlus[MAXWELLNUM];
    TorqData::Torque  tData[MAXWELLNUM];
    string  strFileName;
}TORQUEDATA;

/* 最大10屏，考虑放大5倍，10*500*5 */
#define SPLITPOSNUM     20          // 上扣扭矩和卸扣扭矩数据之间的间隔点数
typedef struct tagDRAWTORQDATA
{
    WORD        wCount;
    WORD        wMUEndPos;
    double      fTorque[25000];
    double      fRpm[25000];

    TorqData::Torque* ptOrgTorq;
}DRAWTORQDATA;

#define     MAXHISDATANUM   5000
typedef struct tagONEHISDATA
{
    CTime   tTime;
    double  fTorque;
    double  fCir;
}ONEHISDATA;

typedef struct tagHISDATA
{
    UINT        nReadCount;
    ONEHISDATA  tOneData[MAXHISDATANUM];
}HISDATA;

//6k存储空间（1M內存溢出）
//#define     MAXSAVEDATALEN  1048576
//#define     MAXSAVELEN      1000000  
#define     MAXSAVEDATALEN  65530
#define     MAXSAVELEN      60000  
typedef struct tagSAVELOGDATA
{
    int         iCur;
    char        aucLog[MAXSAVEDATALEN];
}SAVELOGDATA;

/* 比例阀参数设置 */
#define     VALVETYPENUM        2
#define     VALVERATIONUM       3
typedef struct tagVALVECFG
{
    BYTE    ucTorq[VALVETYPENUM][VALVERATIONUM];    /* 扭矩比例 */
    BYTE    ucRatio[VALVETYPENUM][VALVERATIONUM];   /* 阀值比例 */

    //BYTE    ucZoomRatio;                            /* 放大倍数 */
}VALVECFG;

/* 校准参数设置 */
#define     CALIBNUM            7
#define     CALIBPARALEN        10
typedef struct tagCALIBCFG
{
    BYTE    ucLoad[CALIBNUM][CALIBPARALEN];     /* 负荷 */
    BYTE    ucStroke[CALIBNUM][CALIBPARALEN];   /* 进程读数 */
    BYTE    ucReturn[CALIBNUM][CALIBPARALEN];   /* 回程读数 */
}CALIBCFG;

#define     MAXSEGNUM   10
typedef struct tagCalibInfo
{
    BYTE    ucSegNO;
    int     iCalibTorq;
    int     iSCMTorq;
    int     iLowTorq;
    int     iHighTorq;
}CALIBINFO;

typedef struct tagCalibCtrl
{
    BYTE        ucProc;
    BYTE        ucType;
    CALIBINFO   tInfo;
}CALIBCTRL;

/* 显示参数的名称定义，在代码写死，确定显示的名称/顺序 */
#define         MAXNAMELEN              32
typedef struct tagSHOWPARANAME
{
    // WORD        nNameID;
    string      strName[LANGUAGE_NUM];
}SHOWPARANAME;


#define MAXLINEITEM     500
#define COLLECTPOINTS   (MAXLINEITEM + 50) // 自动缩放周数
#define TESTNUM         10*MAXLINEITEM
typedef struct tagCOLLECTORQUE
{
    //UINT        nCurCount;
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

#define         WITSMAXRPTNUM       20
#define         MAXRPTNUM           10000
typedef struct tagWITSRPTDATA
{
    long        tStart;
    UINT        nCount;
    UINT        nRptIdx;
    double      fTorque[MAXRPTNUM];
    double      fTurn[MAXRPTNUM];
    double      fDuration[MAXRPTNUM];
}WITSRPTDATA;

#pragma endregion

#pragma region MACRO COMMAND

#ifndef WIDTHBYTES
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
#endif

/* 数据标准化，小数点后两位 */
#define PERCENTDATA(fData)    (int(fData*100)/100.0)

#define MIN(a, b)            ((a) > (b) ? (b) : (a))
#define MAX(a, b)            ((a) > (b) ? (a) : (b))

/* 3.22后所有串口数据都记录,数据包括脉冲信息，根据脉冲算周数 */
// 20221231 有数据plussize51,torquesize540
//#define VERSION_RECPLUS(ptTorq)  (ptTorq->dwdelplus_size() == ptTorq->ftorque_size())

/* 自定义代码宏，降低代码圈复杂度 */
/* 程序公共宏值 */
/* 指针为空，直接返回 */
#define ASSERT_NULL(point)              {                   \
        if((point) == NULL)                                 \
            return;                                         \
    }

/* 指针为空，返回ret(BOOL, value, NULL) */
#define ASSERT_NULL_R(point, ret)       {                   \
        if((point) == NULL)                                 \
            return (ret);                                   \
    }

/* 参数为0，直接返回 */
#define ASSERT_ZERO(value)              {                   \
        if((value) == 0)                                    \
            return;                                         \
    }

/* 参数为0，返回ret(BOOL, value, NULL)  */
#define ASSERT_ZERO_R(value, ret)       {                   \
        if((value) == 0)                                    \
            return (ret);                                   \
    }

/* 参数为TRUE，直接返回 */
#define COMP_BTRUE(value)               {                   \
        if((value))                                         \
            return;                                         \
    }

/* 参数为FALSE，直接返回 */
#define COMP_BFALSE(value)              {                   \
        if(!(value))                                        \
            return;                                         \
    }

/* 参数为TRUE，返回 ret */
#define COMP_BTRUE_R(value, ret)        {                   \
        if((value))                                         \
            return (ret);                                   \
    }

/* 参数为FALSE，返回 ret */
#define COMP_BFALSE_R(value, ret)        {                  \
        if(!(value))                                        \
            return (ret);                                   \
    }

/* 参数等于cp，直接返回 */
#define COMP_BE(value, cp)              {                   \
        if((value) == (cp))                                 \
            return;                                         \
    }

/* 参数等于cp，返回ret(BOOL, value, NULL)  */
#define COMP_BE_R(value, cp, ret)       {                   \
        if((value) == (cp))                                 \
            return (ret);                                   \
    }

/* 参数不等于cp，直接返回 */
#define COMP_BNE(value, cp)             {                   \
        if((value) != (cp))                                 \
            return;                                         \
    }

/* 参数不等于cp，返回ret(BOOL, value, NULL)  */
#define COMP_BNE_R(value, cp, ret)      {                   \
        if((value) != (cp))                                 \
            return (ret);                                   \
    }

/* 参数小于cp，直接返回 */
#define COMP_BL(value, cp)              {                   \
        if((value) < (cp))                                  \
            return;                                         \
    }

/* 参数小于cp，返回ret(BOOL, value, NULL)  */
#define COMP_BL_R(value, cp, ret)       {                   \
        if((value) < (cp))                                  \
            return (ret);                                   \
    }

/* 参数大于cp，直接返回 */
#define COMP_BG(value, cp)              {                   \
        if((value) > (cp))                                  \
            return;                                         \
    }

/* 参数大于cp，返回ret(BOOL, value, NULL)  */
#define COMP_BG_R(value, cp, ret)       {                   \
        if((value) > (cp))                                  \
            return (ret);                                   \
    }

/* 参数小于等于cp，直接返回 */
#define COMP_BLE(value, cp)             {                   \
        if((value) <= (cp))                                 \
            return;                                         \
    }

/* 参数小于等于cp，返回ret(BOOL, value, NULL)  */
#define COMP_BLE_R(value, cp, ret)      {                   \
        if((value) <= (cp))                                 \
            return (ret);                                   \
    }

/* 参数大于等于cp，直接返回 */
#define COMP_BGE(value, cp)             {                   \
        if((value) >= (cp))                                 \
            return;                                         \
    }

/* 参数大于等于cp，返回ret(BOOL, value, NULL)  */
#define COMP_BGE_R(value, cp, ret)      {                   \
        if((value) >= (cp))                                 \
            return (ret);                                   \
    }


/* 参数为NULL，循环里面continue */
#define COMP_BNULL_CONTINUE(value)      {                   \
        if((value) == NULL)                                 \
            continue;                                       \
    }

/* 参数为TRUE，循环里面continue */
#define COMP_BTRUE_CONTINUE(value)      {                   \
        if((value))                                         \
            continue;                                       \
    }

/* 参数为FALSE，循环里面continue */
#define COMP_BFALSE_CONTINUE(value)     {                   \
        if(!(value))                                        \
            continue;                                       \
    }

/* 删除非空指针 */
#define DELETE_POINT(point)             {                   \
        if((point) != NULL)                                 \
        {                                                   \
            delete (point);                                 \
            (point) = NULL;                                 \
        }                                                   \
    }

/* 删除有效的object */
#define DELETE_OBJECT(hObject)          {                   \
        if(hObject)                                         \
            ::DeleteObject(hObject);                        \
    }

/* 参数上限检查，如果检查的值大于某值，将该值修改为某值 */
#define CHECK_VALUE_UP(check, upper)    {                   \
        if(check > upper)                                   \
        {                                                   \
            (check) = (upper);                              \
        }                                                   \
    }

/* 参数下限检查，如果检查的值小于某值，将该值修改为某值*/
#define CHECK_VALUE_LOW(check, lower)   {                   \
        if(check < lower)                                   \
        {                                                   \
            (check) = (lower);                              \
        }                                                   \
    }


/* 程序特有宏值 */
/* 判断扭矩记录的序号是否有效 */
#define JUDGE_RANGE(nNo, iMaxNo)        {                   \
        if(((nNo) < 1                      )  ||            \
           ((nNo) > (iMaxNo)               )  ||            \
           ((nNo) > MAXWELLNUM             ))               \
        {                                                   \
            return;                                         \
        }                                                   \
    }

#define JUDGE_RANGE_BOOL(nNo, iMaxNo)   {                   \
        if(((nNo) < 1                      )  ||            \
           ((nNo) > (iMaxNo)               )  ||            \
           ((nNo) > MAXWELLNUM             ))               \
        {                                                   \
            return FALSE;                                   \
        }                                                   \
    }

/* 判断扭矩记录的序号是否有效，无效打印信息 */
#define JUDGE_RANGE_MESS(nNo, iMaxNo, strInfo)  {           \
        if(((nNo) < 1                      )  ||            \
           ((nNo) > (iMaxNo)               )  ||            \
           ((nNo) > MAXWELLNUM             ))               \
        {                                                   \
            AfxMessageBox(strInfo);                         \
            return;                                         \
        }                                                   \
    }

/* 读取扭矩结构历史文件越界保护 */
#define CHECK_FILE_LEN(file, iFileLen)  {                   \
        if((file).GetPosition() > (iFileLen))               \
        {                                                   \
            return FALSE;                                   \
        }                                                   \
    }

#define CHECK_PARA_CHANGE(dstPara, modPara, bChange)  {     \
        if((dstPara) != (modPara))                          \
        {                                                   \
            (dstPara) = (modPara);                          \
            (bChange) |= TRUE;                              \
        }                                                   \
    }
#if 0
/* 从第一条记录开始，跳到指定的记录位置
   说明:文件的第一个UINT记录总数已经读出
   for循环的递增为int i
   nLeng 为UNIT类型,表示该数据完整长度 */
#define SEEK_TORQUE(index, nLeng)      {                                    \
        for(i=0; i<(index); i++)                                            \
        {                                                                   \
            file.Read(&(nLeng), sizeof(UINT));                              \
            file.Seek((nLeng), CFile::current);                             \
        }                                                                   \
    }
#endif
   /* 获取图像的控制扭矩 */
#define GET_CTRL_TORQ(fTorq, ptTorq)        {                       \
        fTorq = ptTorq->fmumaxtorq();                               \
        if(fTorq < ptTorq->fbomaxtorq())                            \
            fTorq = ptTorq->fbomaxtorq();                           \
    }

/* 判断程序注册状态 */
#define JUDGE_REG_STATUS()              {                       \
        if(!theApp.m_tdbReg.Reged())                            \
        {                                                       \
            AfxMessageBox(IDS_STRINFNOREG, MB_ICONINFORMATION); \
            return;                                             \
        }                                                       \
    }

/* 设置控件的名称为STRINGTABEL指定的字符串 */
#define SET_CONTROL_NAME(CtrlID, strTabID, strTag)                          \
    {                                                                       \
        strTag.LoadString(strTabID);                                        \
        GetDlgItem(CtrlID)->SetWindowText(strTag);                          \
    }

/* 设置质量位属性，满足条件设置为1，否则清0 */
#define SET_QUALITY_BIT(tCondition, bitFlag, dwQua)     {                   \
    if(tCondition)                                                          \
    {                                                                       \
        dwQua |= bitFlag;                                                   \
        return dwQua;                                                       \
    }                                                                       \
    else                                                                    \
        dwQua &= ~bitFlag;                                                  \
}

#define SET_STRING_VALUE(strVal, nVal)                  {                   \
    strVal.Empty();                                                         \
    if (nVal > 0)                                                           \
        strVal.Format("%d", nVal);                                          \
}

#define HUNDREDTH(fNum)     (((UINT)((fNum) * 100)) / 100.0)
#define THOUSANDTH(fNum)    (((UINT)((fNum) * 1000)) / 1000.0)
#define HAND_CEIL(fNum)     (ceil((fNum)/100) * 100)
#define HAND_FLOOR(fNum)    (floor((fNum)/100) * 100)
#define GetRandom( min, max ) ((rand() % (int)(((max)+1) - (min))) + (min))

/* 显示周数信息，如果nCur=nAll，显示周数；否则显示当前周数/总周数 */
#define SHOWCIRINFO(nCur, nAll, fMaxCir, strCir)                                \
    {                                                                           \
        if(nCur == nAll)                                                        \
            strCir.Format("%.3f", (nAll*fMaxCir/MAXLINEITEM));                  \
        else                                                                    \
            strCir.Format("%.3f / %.3f", (nCur*fMaxCir/MAXLINEITEM),            \
                                        (nAll*fMaxCir/MAXLINEITEM));            \
    }

#define JUDGE_STRPARA_CHANGE(str1,str2)     {                               \
        clrCtrl = m_clrNormal;                                              \
        if(str1.Compare(str2) != 0)                                         \
            clrCtrl = m_clrChanged;                                         \
        pDC->SetTextColor(clrCtrl);                                         \
    }
#define JUDGE_NUMBERPARA_CHANGE(val1,val2)  {                               \
        clrCtrl = m_clrNormal;                                              \
        if(val1 != val2)                                                    \
            clrCtrl = m_clrChanged;                                         \
        pDC->SetTextColor(clrCtrl);                                         \
    }
#define JUDGE_OTHERTUBE_CHANGE(strOrg)    {                                 \
        if (theApp.m_ptCurShow->tTubeCfg.bFixed == 0)      {                \
            JUDGE_NUMBERPARA_CHANGE(0, 1);          }                       \
        else                                        {                       \
            JUDGE_STRPARA_CHANGE(strContent,strOrg);}                       \
    }


/* 参数上限检查，如果检查的值大于某值，将该值修改为比某值小*** */
#define CHECK_PARA_UP(check, upper,diff) {                      \
        if(check > upper)                                       \
        {                                                       \
            (check) = (upper - diff);                           \
        }                                                       \
    }

/* 参数下限检查，如果检查的值小于某值，将该值修改为比某值大*** */
#define CHECK_PARA_LOW(check, lower, diff) {                    \
        if(check < lower)                                       \
        {                                                       \
            (check) = (lower + diff);                           \
        }                                                       \
    }

/* 参数范围检查，如果检查的值不在范围内，将该值设置为默认值 */
#define CHECK_PARA_ROUND(check, lower, upper, defval) {         \
        if(check < lower || check > upper)                      \
        {                                                       \
            (check) = (defval);                                 \
        }                                                       \
    }

/* 参数范围检查，如果检查的值不在数组内，将该值设置为默认值 */
#define CHECK_PARA_ARRAY(check, array, lower, upper, defval) {  \
        for(i=lower; i<upper; i++)                              \
        {                                                       \
            if(check == array[i])                               \
            {                                                   \
                break;                                          \
            }                                                   \
        }                                                       \
        if(i >= upper)                                          \
        {                                                       \
            (check) = defval;                                   \
        }                                                       \
    }
#pragma endregion

#pragma region Global VARIABLE
extern const int            g_iModBusHi[];
extern const int            g_iModBusLow[];
extern const string         g_tTableName[];
extern const string         g_strCmdName[];
extern const string         g_strStatus[];
extern const string         g_strPortOpr[];
extern const UINT           g_nMainNameNO[];

extern const SHOWPARANAME   g_tNameInfo[];
extern FIXTUBEPARA          g_tDefFactory[];
extern FIXTUBEPARA          g_tDefOEM[];
extern FIXTUBEPARA          g_tDefSize[];
extern FIXTUBEPARA          g_tDefMater[];
extern FIXTUBEPARA          g_tDefCoupl[];
extern TUBECFG              g_tDefTubeCfg[];

extern TORQUEDATA           g_tReadData;
extern TORQUEDATA           g_tReadData2;
extern GLBCFG               g_tGlbCfg;

#pragma endregion

#pragma region Global Function
string  GetCurTime();
string GetListFromVector(vector<int> array);
string GetListFromArray(UINT* parray, int num);
string GetListFromArray(BOOL* parray, int num);
string GetListFromArray(BYTE* parray, int num);
vector<int> GetIDFromList(string lsVals);
void CheckLanguage(UINT& nLang);
string GetCCBString(CComboBox* ptCCB);

string UTF82ASCII(string& strUtf8Code);         //utf-8 转 ascii 
string ASCII2UTF8(string& strAsciiCode);        //ascii 转 Utf8
string string_format(const char* format, ...);
#pragma endregion


class map_value_finder
{
public:
    map_value_finder(const int& cmp_int) :m_s_cmp_int(cmp_int) {}
    bool operator ()(const map<int, int>::value_type& pair)
    {
        return pair.second == m_s_cmp_int;
    }
private:
    const int& m_s_cmp_int;
};

