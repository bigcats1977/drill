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

#define CRT_DBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "resource.h"       // main symbols
#include <Nb30.h>
#include <afxdb.h>
#include <io.h>
#include <string>

#include "odbcinst.h"
#include "Mylistctrl.h"
#include "TorqData.pb.h"

#define DEFAULTNAME     "default.ini"
#define SHOWCONFNAME    "showcfg.ini"
#define VITTANAME       "vitta.ini"
#define REGNAME         "reg.dat"
#define CHNDLLNAME      "Chinese.dll"
#define ENGDLLNAME      "English.dll"
#define RUSDLLNAME      "Russian.dll"

/* 全0的读取串口原始数据
    0x21,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x22,0x00,0x00,0x08 
    PORT  读   命令码     Torque     PLUS   8B和    CRC    结束
*/
#define PORT485         0x21    /* 485串口的有效起始值 */
#define BUS_READ        0x03    /* MODBUS读操作 */
#define BUS_WRITE       0x06    /* MODBUS写操作 */

#define PORT_MAXDATANUM 10      /* 串口一次最大读取数据个数 */
#define PORT_VALIDLEN   12      /* 有效数据长度为12 */
#define PORT_DATAPLACE  4       /* 实际数据起始位置4 */
#define PORT_DATALEN    7       /* 实际数据长度 7;+1CHECK+2SPEED  */
#define PORT_SUMPLACE   8       /* 前面数据校验和位置 8 */
#define PORT_VLDVLVLEN  14      /* 加2个字节阀门状态的有效数据长度为14 */
#define PORT_MAXVLDDATA 9       /* +阀门数据长度 9; 4+1CHECK+2SPEED+2VALVE */
#define PORT_GRPDATA    4       /* 读取多组数据时，每组有效数据的长度为4 */

#define ORG_DATAPLACE   15      /* 实际数据起始位置15 */
#define ORG_PRNTIMELEN  12      /* 自动保存打印的时间长度 04:05:45.984 : */
#define ORG_PRNDATALEN  12      /* 自动保存打印的数据长度 36 cc 43 56 */

#define CTIMELEN        4       /* 强制设置CTime的长度为4, VC2008长度为8，VC6为4，两者不一致 */
#define RPM_OPTNUM      5       /* RMP优化的数目 */
#define RMP_OPTTHRES    20      /* RMP需要优化的阈值 */

#define REGCODEVALUE    0x60    /* 保存的注册码的差值 */
#define REGCODESEGNUM   6       /* 注册码分为6段，长度分别定义如下 */
#define REGCODELEN1     4
#define REGCODELEN2     4
#define REGCODELEN3     4
#define REGCODELEN4     6
#define REGCODELEN5     4
#define REGCODELEN6     4

#define PORTSENDTIME    200     /* 串口发送命令后的保护时间 sleep 200ms */

/* 20220215 实际上扣时超扭矩处理 */
#define SWITCHLOWLIMIT  5000    /* 控制扭矩小于该值时，按最小扭矩下发控制扭矩 */

/* 参数设置调整默认值 */
#define DIFF_TORQUE     10
#define DIFF_CIRCUIT    0.1
#define DIFF_TIME       1
#define FULLCIR4SAVE    0.20        // 采集频率 1/2500  1周/(500/0.2)

/* REGCODE计算的常数定义 */
#define MAC0XOR         0x789789
#define MAC0AND         0x00FFFFFF
#define MAC0DEC         0x197712
#define MAC1XOR         0x7890
#define MAC1AND         0x0000FFFF
#define MAC1DEC         0x1207
#define VOL0XOR         0x51085759
#define VOL0DEC         0x66666666
#define VOL1XOR         0x13963349
#define VOL1DEC         0x12345678
#define DPATHV2         0x1f5d876e

#define PBHEAD          0xFFFDFBF9
#define PBHEADLEN       4

/* 保存调试信息约定消息头宏值和字符串 */
#define DBG_HASH            0   /* # */
#define DBG_START           1   /* * reset */
#define DBG_COLLECT         2   /* collect */
#define DBG_MESSAGE         3   /* MessageBox显示信息 */
#define DBG_SNDCMD          4   /* 发送串口请求 */
#define DBG_MAXNUM          (DBG_SNDCMD+1)
/* 调试信息头的长度固定为4 */
#define DBG_HEADLEN         4

/* 设置定时读取串口时的状态 */
/* 奇数为正常状态，需要先读缓冲；偶数为缓冲已经清空，正常发送命令 */
#define RS_NORMAL       0   /* 正常状态，缓冲区为空，直接发送，换成区非空转CLASH状态 */
#define RS_READCLASH    1   /* READ时单片机缓冲非空 */
#define RS_RESETCLASH   2   /* RESET时单片机缓冲非空 */
#define RS_MAX          RS_RESETCLASH

#define RS_READ_NORMAL  0   /* 正常状态时读串口 */
#define RS_READ_RESET   1   /* 复位状态时读串口 */

#define IP_SLOPE_PER    1   /* 默认拐点百分比 */

#define     SHOWPARANAMELEN             25
#define     MAXPARALEN                  200
#define     HALFPARALEN                 50
    
#define     MAXPWLEN                    32
#define     MAXSKIPLEN                  64  /* 总共跳64个字节 */

#define     MAXPROBUFF                  819200  // 3.22 buf扩大2倍
#define     OVERFLOWTORQ                300000  /* 溢出的扭矩值，超过该值肯定无效 */


/* 定义程序的状态 */
#define STATUS_INVALID  0   /* 无效状态 */
#define STATUS_START    1   /* 程序启动状态 */
#define STATUS_EXIT     2   /* 程序退出状态 */
#define STATUS_RUN      3   /* 程序运行状态 */
#define STATUS_STOP     4   /* 程序停止状态 */
#define STATUS_SETPARA  5   /* 参数设置状态 */
#define STATUS_FUNTEST  6   /* 功能调试状态 */
#define STATUS_COLLECT  7   /* 收集单片机数据状态 */
#define STATUS_HISTORY  8   /* 读取历史数据状态 */
#define STATUS_RESTART  9   /* 重新打开串口 */
#define STATUS_CHGLAN   10  /* 改变语言重新打开窗口 */
#define STATUS_HISSTAT  11  /* 上井数据统计 */
#define STATUS_SETVALVE 12  /* 设置比例阀参数 */
#define STATUS_CIRCLE   13  /* 横轴为周版本 */
#define STATUS_TIME     14  /* 横轴为时间版本 */
#define STATUS_SETSHOW  15  /* 修改界面显示参数 */
#define STATUS_CHGUNIT  16  /* 修改扭矩单位 */
#define STATUS_CALIB    17  /* 校准 */
#define STATUS_MAXNUM   (STATUS_CALIB +1)

/////////////////////////////////////////////////////////////////////////////
// CTorqueApp:
// See Torque.cpp for the implementation of this class
//
typedef struct tagCONTROLPARA
{
    /* 扭矩显示相关参数 */
    double      fMaxLimit;    /* 最大上限 */
    double      fUpperLimit;  /* 最大扭矩 */
    double      fControl;     /* 控制扭矩 */
    double      fOptTorq;     /* 最佳扭矩 */
    double      fLowerLimit;  /* 最小扭矩 */
    double      fSpeedDown;   /* 减速扭矩 */
    double      fShow;        /* 显示扭矩 */
    double      fBear;        /* 肩负扭矩 */
    double      fCut;         /* 打折比例 0.8 */
    /* 扭拧周数 */
    double      fMaxCir;      /* 最大周数 */
    double      fUpperCir;    /* 上限周数 */
    double      fControlCir;  /* 控制周数 */
    double      fLowerCir;    /* 下限周数 */
    double      fPlus;        /* 周脉冲数 */
    /* 满屏转速 */
    double      fMaxRPM;      /* 最大转速 */
    
    WORD        wIPPos;       /* 拐点纵坐标位置，0表示没有拐点,或者是老数据，手工补充拐点 */
    BYTE        ucVer;        /* 0: 2017年数据结构
                                 1: 显示参数名称长度固定25；最多30个显示参数；NLV格式: 显示参数的值长度根据L */
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

typedef struct tagFixTubPara
{
    UINT        nNO;
    string      strName[LANGUAGE_NUM];
}FIXTUBPARA;

typedef struct tagFixTubInfo
{
    UINT        nNum;
    FIXTUBPARA  *ptPara;
}FIXTUBINFO;

typedef struct tagTUBINGCFG
{
    UINT    nFactory;       // 厂家  JFE
    UINT    nOEM;           // 管件厂家  2-7/8″ JFE
    UINT    nSize;          // 管件规格 TubingSize
    UINT    nMat;           // 扣型材质 EUE          N80
    UINT    nCoupling;      // 接箍规格 93.2X    磷化
    /* N.m */
    UINT    nMaxTorq;       // 最大扭矩
    UINT    nOptTorq;       // 最佳扭矩
    UINT    nMinTorq;       // 最小扭矩
    /* lb.ft */
    UINT    lMaxTorq;       // 最大扭矩
    UINT    lOptTorq;       // 最佳扭矩
    UINT    lMinTorq;       // 最小扭矩
}TUBINGCFG;

typedef struct tagAllTUBCFG
{
    UINT        nNum;
    TUBINGCFG  *ptCfg;
}AllTUBCFG;

#define         MAXMEMOLEN              128
typedef struct tagCOMMONCFG
{
    /* double类型配置 最多16个 */
    double      fMaxTorq;       /* 启用，记录数据的最大扭矩 */
    double      fUpperTai;      /* 最大台阶扭矩 */
    double      fLowerTai;      /* 最小台阶扭矩 */
    double      fRpmAdj;        /* 转速调整 */
    double      fMulti;         /* 校准参数范围0~2 */
    double      fMinShlSlope;   /* 最小肩负斜率5.0 */
    double      fMaxDeltaCir;   /* 最大Delta周数0.1 */
    double      fMinDeltaCir;   /* 最小Delta周数0.1 */
    double      fRsv[8];        /* 保留double字段，共16个 */

    /* DWORD类型配置 最多16个 */
    DWORD       dwSeqNo;
    DWORD       dwRsv[15];

    /* 开关值设置，每个开关值使用一个BIT位 */
    /* BIT位定义 最多8*32个 */
    DWORD       bBear:1;        /* 是否有肩负,默认为没有 */
    DWORD       bToolBuck:1;    /* 是否为工具扣,默认为否 */
    DWORD       bRsv:30;
    DWORD       dwRsv2[7];

    char        aucMemo[MAXMEMOLEN];        /* 备注 */
    char        aucRemark[MAXPARALEN];      /*标注说明remark*/
}COMMONCFG;

typedef struct tagPARACFG
{
    CONTROLPARA tCtrl;
    COMMONCFG   tComm;
}PARACFG;

#define         MAXNAMENUM              15
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


//#define       HALFPARALEN             25
//#define       MAXPARALEN              50

#define         MAXPARANUM              15
#define         MAXMAINPARA             7
#define         MAXOPTIONNUM            50
#define         MAINSHOWTUBE            4   /* 第5个主界面显示参数为管体序号(5-1) */
#define         MAINSHOWWELL            5   /* 第6个主界面显示参数为入井序号(6-1) */

/* 15个参数, 4~7固定 */
#define         FIXSHOWBEGIN            3   /* 第4个显示参数固定(4-1) */
#define         FIXSHOWEND              6   /* 第7个显示参数固定(7-1) */

/* 输出excel统计报告常量 */
#define         MAXSUMMARYPARA          6   /* 总结(Page1)最多 6个显示参数, 3占4格 / 4占8格 */
#define         MAX3VALUES              4
#define         MAX4VALUES              8
#define         MAXREPORTPARA           3   /* 报表(Page4)最多 3个显示参数 */

/* 显示参数的名称定义，在代码写死，确定显示的名称/顺序 */
#define         MAXNAMELEN              32
typedef struct tagSHOWPARANAME
{
    WORD        wNameID;
    string      strName[LANGUAGE_NUM];
}SHOWPARANAME;

/* 当前扭矩的显示参数的设置，保存到文件中 */
typedef struct tagSHOWPARA
{
    string      strName;
    string      strValue;
}SHOWPARA;

typedef struct tagSHOWOPTION
{
    WORD        wOptNum;
    string      strOpt[MAXOPTIONNUM];
}SHOWOPTION;

typedef struct tagSHOWCFG
{
    WORD        wParaNum;               /* 当前显示参数个数;图形参数个数等于显示参数个数 */
    WORD        wListNum;               /* 当前列表显示参数个数 */
    WORD        wMainNum;               /* 当前主界面显示参数个数 */
    WORD        wFileName;              /* 命名数据文件的参数，是各个参数中的一个 */
    WORD        wStatType;              /* 统计参数，是各个参数中的一个 */
    SHOWPARA    tShow[MAXPARANUM+1];    /* 当前选择的显示参数及值 +1 厂家(16)，参数设置时使用 */
    SHOWOPTION  tOption[MAXPARANUM+1];  /* 当前显示参数的可选项 +1 厂家(16)，参数设置时使用  */
    string      strList[MAXPARANUM+1];
    string      strMain[MAXPARANUM+1];

    BOOL        bFixTub;        
    TUBINGCFG   tTubingCfg;               /* 5个油管参数的固化定义，非固化参数在SHOWPARA中 */
}SHOWCFG;

/* XLS统计结果参数配置 */
typedef struct tagXLSSTATCFG
{
    string      strName[MAXPARANUM];    /* 显示参数名称 */
    BOOL        bSummary[MAXPARANUM];   /* 摘要(Page1)显示参数序号 */
    BOOL        bReport[MAXPARANUM];    /* 报告(Page4)显示参数序号 */
    BYTE        ucOperator;             /* 当班班长(操作者)参数序号 */
    BYTE        ucTally;                /* 入井序号参数序号 */
}XLSSTATCFG;

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

#define     MAXNAME     50
#define     MAXREGCODE  26
typedef struct tagDBREG
{
    BYTE        strRegCode[MAXREGCODE];
    BYTE        bRsv1   : 3;
    BYTE        bReged  : 1;
    BYTE        bRsv2   : 4;
    BYTE        strName[MAXNAME];
}DBREG;

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

typedef struct tagASTAT
{
    ADAPTER_STATUS  adapt;
    /*NAME_BUFFER     NameBuff [30];*/
} ASTAT;

#define     MAXSPLIITNUM        10
typedef struct tagSplit
{
    int     iCur;           /* 范围1~iSplitNum */
    int     iCtrlPnt;       /* 控制周数对应点数，上扣有效，卸扣写死为500 */
    int     iSplitNum;
    int     iBegin[MAXSPLIITNUM];
    int     iEnd[MAXSPLIITNUM];
}SPLITPOINT;

#define     MAXWELLNUM   5000
typedef struct tagTORQUEDATA
{
    UINT    nCur;               /* nCur从1开始计数，相当于数组序号+1 */
    UINT    nTotal;
    UINT    nQualy;
    UINT    nUnQualy;
    BOOL    bHaveHead;  /* 文件中每个数据前面是否有头隔断 */
    UINT    nTotalPlus[MAXWELLNUM];
    TorqData::Torque  tData[MAXWELLNUM];
    SPLITPOINT        tSplit[MAXWELLNUM];   // 多屏数据时，读取后设置分屏起点终点
}TORQUEDATA;

/* 最大10屏，考虑放大5倍，10*500*5 */
typedef struct tagDRAWTORQDATA
{
    WORD        wCount;
    double      fTorque[25000];
    double      fRpm[25000];

    TorqData::Torque *ptOrgTorq;
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
    void    WriteConfigStr(CString strclassName,CString strclassItem, CString strValue, string strParaName);
    void    WriteConfigStr(WORD wclassName, WORD wclassItem, CString strValue, string strParaName);
    void    AdjustParaValue(PARACFG *ptCfg); /* 检查参数是否发生变化 */
    /* 将列表内容导出到excle表格中 */
    BOOL    SaveList2XlsFile(CString strFileName, CString strSheetName, CMylistctrl *ptlistData);

    void    AdaptDlgCtrlSize(CDialog *pdlgAdapt, UINT nSheetType = 0);
    void    ShowMainTitle();
    CString GetQualityInfo(TorqData::Torque *ptTorq);
    int     GetQualityIndex(TorqData::Torque *ptTorq);
    void    GetCurWellFile();
    void    GetCurNum();
    void    ReadPara(string strParaName, PARACFG *ptCfg, BOOL bNeedWrite = TRUE);
    void    WritePara(string strParaName, PARACFG *ptCfg);
    void    ReadGlbShowPara();
    BOOL    ReadShowPara (BYTE ucLang, SHOWCFG *ptShow);
    void    WriteGlbShowPara();
    void    WriteShowPara();
    void    WriteXlsStatPara(string strParaName, XLSSTATCFG *ptStat);
    void    WritePortPara(string strParaName, PORTCFG *ptPort);
    void    WriteOneTubing(string strParaName, string strLang, unsigned string_ID, CString strValue);
    void    WriteFixTubingPara(string strParaName, string strLang, TUBINGCFG *ptTubing);
    void    WriteValvePara(string strParaName, VALVECFG *ptValve);
    void    WriteParaFileName(string strFileName);
    //void    WriteCalibPara(string strParaName, CALIBCFG *ptCalib);
    BOOL    CheckReg(CString strReg[]);
    BOOL    GetVolMacFromRegStr(CString strReg[], DWORD pdwVol[], DWORD pdwMac[], int& iYear, int& iMonth, int& iDay);
    BOOL    GetVolMacInfo(DWORD pdwVol[], DWORD pdwMac[], int iYear, int iMonth, int iDay);
    void    GetMACAddr(UCHAR *pcMac);
    void    StringSubtract(CString& strValue, BYTE ucChar);
    void    SplitRegString(CString strReg[], CString strRegCode);
    void    MergeRegString(CString strReg[], CString& strRegCode);
    BOOL    LoadLanguageDll(UINT nLangType, BOOL bUpdate=TRUE);
    void    ReOpenWindow();
    void    PlayAlarmSound();
    void    StopAlarmSound();
    void    InitShowCfg(SHOWCFG *ptShow);

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
    BOOL    InsertShowOption(SHOWCFG *ptShow, WORD wShowName, CString strOption, CComboBox *ptcbOption);
    CString GetStatType(TorqData::Torque *ptTorq, WORD wPlace);
    CString GetTorqSimpDate(TorqData::Torque *ptTorq);
    CString GetTorqFullDate(TorqData::Torque *ptTorq);
    CString GetTorqCollTime(TorqData::Torque *ptTorq);
    void    ClearReadTorq();
    BOOL    CheckPassWord();
    string  LoadString4string( unsigned string_ID );
    BOOL    LoadString4string( unsigned string_ID , string &strValue);
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
    int     GetMainWellIndexfromData(CString strWellName, TorqData::Torque *ptTorq);
    void    UpdateAutoSaveFileName();
    CString GetFactoryValue();
    CString GetOEMValue();
    CString GetTubSizeValue();
    CString GetThreadMatValue();
    CString GetCouplingValue();
    BOOL    FindNotFileChar(CString strName);
    CString GetTorqShowName(TorqData::Torque *ptTorq, int iIndex);
    CString GetTorqShowValue(TorqData::Torque *ptTorq, int iIndex);
    DRAWTORQDATA * GetDrawDataFromTorq(UINT nNO, int iMulti = 1);
    TorqData::Torque * GetOrgTorqFromTorq(UINT nNO);

    PARACFG         m_tParaCfg;
    PORTCFG         m_tPortCfg;
    SHOWCFG         m_tShowCfg[LANGUAGE_NUM];         /* 显示参数的所有参数设置 */
    SHOWCFG         *m_ptCurShow;
    XLSSTATCFG      m_tXlsStatCfg;
    DBREG           m_tdbReg;
    VALVECFG        m_tValveCfg;        /* 阀值配置 */
    //CALIBCFG        m_tCalibCfg;        /* 校准参数配置 */
    string          m_strDllFile;       /* 动态链接库文件名称 */
    string          m_strAppPath;
    string          m_strParaFile;      /* 保存系统产生配置文件
                                           和.exe同路径，后缀名为.ini */
    string          m_strShowFile;      /* 保存系统产生的显示参数配置文件，隐藏 
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
    string          m_strParity;        /* 串口奇偶校验的字符串 */
    BOOL            m_bBigTorq;         /* 是否为大扭矩版本2.4.9, TRUE:和单片机扭矩相差10倍,FALSE:1倍 */
    BOOL            m_bShackle;         /* 是否是卸扣版本，是到控制扭矩不画竖线，按单片机数据显示 */
    BOOL            m_bCheckIP;         /* 质量判断是否启用高级判断，简单判断以控制扭矩为准， 超过最大扭矩和低于最小扭矩为不合格，默认为1 */
    //BOOL            m_bFirstRun;      /* 是否第一次运行，是的话运行DlgBear */
    BOOL            m_bFileBehindDate;  /* 日期在文件命名的后面 */
    UINT            m_nTorqMulti;       /* BigTorq: True:10; False:1*/
    UINT            m_nTestFunc;        /* 0: 真实串口；1：模拟测试；2：读取dat扭矩数据；3: 读取dat 历史数据; 4: 多组历史数据*/
    UINT            m_nColletTime;      /* 定时收集数据的时间，ms，默认250 */
    UINT            m_nSaveTime;        /* 大于显示扭矩后保存数据的时间，默认30s */
    UINT            m_nReset;           /* 复位时间，默认10s */
    UINT            m_nImgNum;          /* 批量导出图形时，一个jpg文件中包含多少个图形 */
    UINT            m_nZoomIn;          /* 图形放大倍数 */
    UINT            m_nIPShowMode;      /*  1: 只画数据中的拐点
                                            2: 只画计算拐点
                                            3: 数据拐点和计算拐点都画*/
    double          m_fIPDeltaVal;      /* 默认0.7 */
    UINT            m_nCurNO;           /* 当前扭矩的序号，更换文件清零 */
    DWORD           m_dwTotalTorqNum;   /* 扭矩上扣最大数目 */
    BOOL            m_bParaChg;         /* 参数设置以及修改，需要将参数写入文件 */
    UINT            m_nPBHead;
    BOOL            m_bShowCRC;
    UINT            m_nTorqUnit;
    CString         m_strUnit;
    SAVELOGDATA     m_tSaveLog;
    CFile           m_SaveLogFile;
    CString         m_strDbgHead[DBG_MAXNUM];
    char            m_aucPassWord[MAXPWLEN];

    /* 多语言支持 */
    HINSTANCE       m_hLangDLL[LANGUAGE_NUM];
    UINT            m_nLangType;        /* 当前语言类型 */

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

    BOOL GetConfigStr( string strParaName, WORD wclassName, WORD wclassItem, WORD wclassDefault, 
                       char* pDest, int iParaLen = MAXPARALEN);
    BOOL GetConfigStr( string strParaName, CString strclassName, CString strclassItem, CString strclassDefault, 
                       char* pDest, int iParaLen = MAXPARALEN);
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
    void ExportListToExcel(CString strSheetName, CDatabase* ptDb, CMylistctrl *ptlistData);
    BOOL GetDefaultXlsFileName(CString sDefTitle, CString& sExcelFile);
    BOOL MakeSurePathExists( CString &Path, bool FilenameIncluded);

    void InitArray();
    void InitVariant();
    void InitLanguage();

    void SetDefaultShow(BYTE ucLang, SHOWCFG *ptShow);
    void SetDefaultXlsStat(XLSSTATCFG *ptStat);
    void ReadTorquePara (string strParaName, CONTROLPARA *ptCtrl, COMMONCFG *ptComm);
    void ReadCircuitPara(string strParaName, CONTROLPARA *ptCtrl, COMMONCFG *ptComm);
    void ReadRpmPara    (string strParaName, CONTROLPARA *ptCtrl, COMMONCFG *ptComm);
    void ReadPortPara   (string strParaName, PORTCFG     *ptPort);
    void ReadFixTubingPara(string strParaName, string strLang, TUBINGCFG *ptTubing);
    void ReadXlsStatPara(string strParaName, XLSSTATCFG  *ptStat);
    void ReadValvePara  (string strParaName, VALVECFG    *ptValve);
    //void ReadCalibPara  (string strParaName, CALIBCFG    *ptCalib);
    void ReadOtherPara  (string strParaName, PARACFG     *ptCfg);
    void ReadParaFileName(void);

    void WriteTorquePara (string strParaName, CONTROLPARA *ptCtrl, COMMONCFG *ptComm);
    void WriteCircuitPara(string strParaName, CONTROLPARA *ptCtrl, COMMONCFG *ptComm);
    void WriteRpmPara    (string strParaName, CONTROLPARA *ptCtrl, COMMONCFG *ptComm);
    void WriteOtherPara  (string strParaName, PARACFG     *ptCfg);

    void AdjustTorquePara (CONTROLPARA *ptCtrl);
    void AdjustCircuitPara(CONTROLPARA *ptCtrl);
    void AdjustPortPara   (PORTCFG *ptPort);
    void AdjustOtherPara  (PARACFG *ptCfg);

    BOOL JudgeTranslate(TorqData::Torque *ptTorq);
    CString GetFixTubingValue(UINT nShowIndex, UINT nCurNO, FIXTUBINFO *ptFix);
    WORD GetIPPlace(int iCurPnt, int iInterval);

    unsigned decodeBMP(std::vector<unsigned char>& image, unsigned& w, unsigned& h, unsigned char * bmp);

    /* array */
    string   m_slParity[3];
    BYTE     m_ucParity[3];
    
    /* 找不到MAC时的默认MAC */
    BYTE     m_ucDefaultMac[5];
    /* 串口有效波特率 */
    UINT     m_nBand[10];
};


extern CTorqueApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TORQUE_H__B8387AE5_653F_4729_8A7A_95783AA1DEC1__INCLUDED_)

