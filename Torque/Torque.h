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

/* ȫ0�Ķ�ȡ����ԭʼ����
    0x21,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x22,0x00,0x00,0x08 
    PORT  ��   ������     Torque     PLUS   8B��    CRC    ����
*/
#define PORT485         0x21    /* 485���ڵ���Ч��ʼֵ */
#define BUS_READ        0x03    /* MODBUS������ */
#define BUS_WRITE       0x06    /* MODBUSд���� */

#define PORT_MAXDATANUM 10      /* ����һ������ȡ���ݸ��� */
#define PORT_VALIDLEN   12      /* ��Ч���ݳ���Ϊ12 */
#define PORT_DATAPLACE  4       /* ʵ��������ʼλ��4 */
#define PORT_DATALEN    7       /* ʵ�����ݳ��� 7;+1CHECK+2SPEED  */
#define PORT_SUMPLACE   8       /* ǰ������У���λ�� 8 */
#define PORT_VLDVLVLEN  14      /* ��2���ֽڷ���״̬����Ч���ݳ���Ϊ14 */
#define PORT_MAXVLDDATA 9       /* +�������ݳ��� 9; 4+1CHECK+2SPEED+2VALVE */
#define PORT_GRPDATA    4       /* ��ȡ��������ʱ��ÿ����Ч���ݵĳ���Ϊ4 */

#define ORG_DATAPLACE   15      /* ʵ��������ʼλ��15 */
#define ORG_PRNTIMELEN  12      /* �Զ������ӡ��ʱ�䳤�� 04:05:45.984 : */
#define ORG_PRNDATALEN  12      /* �Զ������ӡ�����ݳ��� 36 cc 43 56 */

#define CTIMELEN        4       /* ǿ������CTime�ĳ���Ϊ4, VC2008����Ϊ8��VC6Ϊ4�����߲�һ�� */
#define RPM_OPTNUM      5       /* RMP�Ż�����Ŀ */
#define RMP_OPTTHRES    20      /* RMP��Ҫ�Ż�����ֵ */

#define REGCODEVALUE    0x60    /* �����ע����Ĳ�ֵ */
#define REGCODESEGNUM   6       /* ע�����Ϊ6�Σ����ȷֱ������� */
#define REGCODELEN1     4
#define REGCODELEN2     4
#define REGCODELEN3     4
#define REGCODELEN4     6
#define REGCODELEN5     4
#define REGCODELEN6     4

#define PORTSENDTIME    200     /* ���ڷ��������ı���ʱ�� sleep 200ms */

/* 20220215 ʵ���Ͽ�ʱ��Ť�ش��� */
#define SWITCHLOWLIMIT  5000    /* ����Ť��С�ڸ�ֵʱ������СŤ���·�����Ť�� */

/* �������õ���Ĭ��ֵ */
#define DIFF_TORQUE     10
#define DIFF_CIRCUIT    0.1
#define DIFF_TIME       1
#define FULLCIR4SAVE    0.20        // �ɼ�Ƶ�� 1/2500  1��/(500/0.2)

/* REGCODE����ĳ������� */
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

/* ���������ϢԼ����Ϣͷ��ֵ���ַ��� */
#define DBG_HASH            0   /* # */
#define DBG_START           1   /* * reset */
#define DBG_COLLECT         2   /* collect */
#define DBG_MESSAGE         3   /* MessageBox��ʾ��Ϣ */
#define DBG_SNDCMD          4   /* ���ʹ������� */
#define DBG_MAXNUM          (DBG_SNDCMD+1)
/* ������Ϣͷ�ĳ��ȹ̶�Ϊ4 */
#define DBG_HEADLEN         4

/* ���ö�ʱ��ȡ����ʱ��״̬ */
/* ����Ϊ����״̬����Ҫ�ȶ����壻ż��Ϊ�����Ѿ���գ������������� */
#define RS_NORMAL       0   /* ����״̬��������Ϊ�գ�ֱ�ӷ��ͣ��������ǿ�תCLASH״̬ */
#define RS_READCLASH    1   /* READʱ��Ƭ������ǿ� */
#define RS_RESETCLASH   2   /* RESETʱ��Ƭ������ǿ� */
#define RS_MAX          RS_RESETCLASH

#define RS_READ_NORMAL  0   /* ����״̬ʱ������ */
#define RS_READ_RESET   1   /* ��λ״̬ʱ������ */

#define IP_SLOPE_PER    1   /* Ĭ�Ϲյ�ٷֱ� */

#define     SHOWPARANAMELEN             25
#define     MAXPARALEN                  200
#define     HALFPARALEN                 50
    
#define     MAXPWLEN                    32
#define     MAXSKIPLEN                  64  /* �ܹ���64���ֽ� */

#define     MAXPROBUFF                  819200  // 3.22 buf����2��
#define     OVERFLOWTORQ                300000  /* �����Ť��ֵ��������ֵ�϶���Ч */


/* ��������״̬ */
#define STATUS_INVALID  0   /* ��Ч״̬ */
#define STATUS_START    1   /* ��������״̬ */
#define STATUS_EXIT     2   /* �����˳�״̬ */
#define STATUS_RUN      3   /* ��������״̬ */
#define STATUS_STOP     4   /* ����ֹͣ״̬ */
#define STATUS_SETPARA  5   /* ��������״̬ */
#define STATUS_FUNTEST  6   /* ���ܵ���״̬ */
#define STATUS_COLLECT  7   /* �ռ���Ƭ������״̬ */
#define STATUS_HISTORY  8   /* ��ȡ��ʷ����״̬ */
#define STATUS_RESTART  9   /* ���´򿪴��� */
#define STATUS_CHGLAN   10  /* �ı��������´򿪴��� */
#define STATUS_HISSTAT  11  /* �Ͼ�����ͳ�� */
#define STATUS_SETVALVE 12  /* ���ñ��������� */
#define STATUS_CIRCLE   13  /* ����Ϊ�ܰ汾 */
#define STATUS_TIME     14  /* ����Ϊʱ��汾 */
#define STATUS_SETSHOW  15  /* �޸Ľ�����ʾ���� */
#define STATUS_CHGUNIT  16  /* �޸�Ť�ص�λ */
#define STATUS_CALIB    17  /* У׼ */
#define STATUS_MAXNUM   (STATUS_CALIB +1)

/////////////////////////////////////////////////////////////////////////////
// CTorqueApp:
// See Torque.cpp for the implementation of this class
//
typedef struct tagCONTROLPARA
{
    /* Ť����ʾ��ز��� */
    double      fMaxLimit;    /* ������� */
    double      fUpperLimit;  /* ���Ť�� */
    double      fControl;     /* ����Ť�� */
    double      fOptTorq;     /* ���Ť�� */
    double      fLowerLimit;  /* ��СŤ�� */
    double      fSpeedDown;   /* ����Ť�� */
    double      fShow;        /* ��ʾŤ�� */
    double      fBear;        /* �縺Ť�� */
    double      fCut;         /* ���۱��� 0.8 */
    /* Ťš���� */
    double      fMaxCir;      /* ������� */
    double      fUpperCir;    /* �������� */
    double      fControlCir;  /* �������� */
    double      fLowerCir;    /* �������� */
    double      fPlus;        /* �������� */
    /* ����ת�� */
    double      fMaxRPM;      /* ���ת�� */
    
    WORD        wIPPos;       /* �յ�������λ�ã�0��ʾû�йյ�,�����������ݣ��ֹ�����յ� */
    BYTE        ucVer;        /* 0: 2017�����ݽṹ
                                 1: ��ʾ�������Ƴ��ȹ̶�25�����30����ʾ������NLV��ʽ: ��ʾ������ֵ���ȸ���L */
    BYTE        ucRsv;
}CONTROLPARA;

typedef struct tagPORTCFG
{
    BYTE        ucPortNo;       /* ���ں� */
    BYTE        ucParity;       /* ��żУ�� */
    BYTE        ucDataBit;      /* ����λ */
    BYTE        ucStopBit;      /* ֹͣλ */
    UINT        nBand;          /* ������ */
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
    UINT    nFactory;       // ����  JFE
    UINT    nOEM;           // �ܼ�����  2-7/8�� JFE
    UINT    nSize;          // �ܼ���� TubingSize
    UINT    nMat;           // ���Ͳ��� EUE          N80
    UINT    nCoupling;      // �ӹ���� 93.2X    �׻�
    /* N.m */
    UINT    nMaxTorq;       // ���Ť��
    UINT    nOptTorq;       // ���Ť��
    UINT    nMinTorq;       // ��СŤ��
    /* lb.ft */
    UINT    lMaxTorq;       // ���Ť��
    UINT    lOptTorq;       // ���Ť��
    UINT    lMinTorq;       // ��СŤ��
}TUBINGCFG;

typedef struct tagAllTUBCFG
{
    UINT        nNum;
    TUBINGCFG  *ptCfg;
}AllTUBCFG;

#define         MAXMEMOLEN              128
typedef struct tagCOMMONCFG
{
    /* double�������� ���16�� */
    double      fMaxTorq;       /* ���ã���¼���ݵ����Ť�� */
    double      fUpperTai;      /* ���̨��Ť�� */
    double      fLowerTai;      /* ��С̨��Ť�� */
    double      fRpmAdj;        /* ת�ٵ��� */
    double      fMulti;         /* У׼������Χ0~2 */
    double      fMinShlSlope;   /* ��С�縺б��5.0 */
    double      fMaxDeltaCir;   /* ���Delta����0.1 */
    double      fMinDeltaCir;   /* ��СDelta����0.1 */
    double      fRsv[8];        /* ����double�ֶΣ���16�� */

    /* DWORD�������� ���16�� */
    DWORD       dwSeqNo;
    DWORD       dwRsv[15];

    /* ����ֵ���ã�ÿ������ֵʹ��һ��BITλ */
    /* BITλ���� ���8*32�� */
    DWORD       bBear:1;        /* �Ƿ��м縺,Ĭ��Ϊû�� */
    DWORD       bToolBuck:1;    /* �Ƿ�Ϊ���߿�,Ĭ��Ϊ�� */
    DWORD       bRsv:30;
    DWORD       dwRsv2[7];

    char        aucMemo[MAXMEMOLEN];        /* ��ע */
    char        aucRemark[MAXPARALEN];      /*��ע˵��remark*/
}COMMONCFG;

typedef struct tagPARACFG
{
    CONTROLPARA tCtrl;
    COMMONCFG   tComm;
}PARACFG;

#define         MAXNAMENUM              15
/* ͨ�� 6 */
#define         COMMPARA_WELL           1   /* ʩ������ */
#define         COMMPARA_TEAM           2   /* �� ҵ �� */
#define         COMMPARA_CONTRACT       3   /* ��ͬ�� */
#define         COMMPARA_OPERATOR       4   /* ����Ա */
#define         COMMPARA_BELTLINE       5   /* �����ߺ� */
#define         COMMPARA_STAND          6   /* ִ�б�׼ */
#define         COMMPARA_OPERATION      7   /* ��ҵ��ʽ */
#define         COMMPARA_PARTYA         8   /* �׷� */
#define         COMMPARA_CASINGTEAM     9   /* ���׹ܶ� */
#define         COMMPARA_SHIFTLEADER    10  /* ����೤ */
/* �������  */
/* ���� 7 */
#define         TUBEPARA_DIAMETER       100 /* �����⾶ */
#define         TUBEPARA_THICKEN        101 /* ����ں� */
#define         TUBEPARA_GRADE          102 /* ����ּ� */
#define         TUBEPARA_ENDTYPE        103 /* �ܶ����� */
#define         TUBEPARA_TUBENO         104 /* ����� */
#define         TUBEPARA_STOVE          105 /* ����¯�� */
#define         TUBEPARA_BATCH          106 /* �������� */
/* ��̩���� 3 */
#define         TUBEPARA_NAME           120 /* �ܼ����� */
#define         TUBEPARA_MAKEUPTYPE     121 /* �Ͽ����� */
#define         TUBEPARA_BUCKLETYPE     122 /* ���Ͳ��� */
#define         TUBEPARA_FACTORY        123 /* �ܼ����� */
#define         TUBEPARA_RUNNINGNO      124 /* �뾮��� */
/* �ӹ�����  */
/* ���� 6 */
#define         COUPPARA_DIAMETER       201 /* �ӹ��⾶ */
#define         COUPPARA_LENGTH         202 /* �ӹ����� */
#define         COUPPARA_MATER          203 /* �ӹ����� */
#define         COUPPARA_STOVE          204 /* �ӹ�¯�� */
#define         COUPPARA_BATCH          205 /* �ӹ����� */
#define         COUPPARA_COUPNO         206 /* �ӹ��� */
/* ��̩���� 3 */
#define         COUPPARA_WEIGHT         220 /* ���� */
#define         COUPPARA_LUBRICANT      221 /* ��֬ */
#define         COUPPARA_COUPLING       222 /* �ӹ����� */
#define         COUPPARA_THREADDOPE     223 /* ˿���� */
#define         COUPPARA_HANDDEVICE     224 /* �������� */
#define         COUPPARA_HYDTONG        225 /* Һѹǯ */
#define         COUPPARA_OEM            226 /* ���� */


//#define       HALFPARALEN             25
//#define       MAXPARALEN              50

#define         MAXPARANUM              15
#define         MAXMAINPARA             7
#define         MAXOPTIONNUM            50
#define         MAINSHOWTUBE            4   /* ��5����������ʾ����Ϊ�������(5-1) */
#define         MAINSHOWWELL            5   /* ��6����������ʾ����Ϊ�뾮���(6-1) */

/* 15������, 4~7�̶� */
#define         FIXSHOWBEGIN            3   /* ��4����ʾ�����̶�(4-1) */
#define         FIXSHOWEND              6   /* ��7����ʾ�����̶�(7-1) */

/* ���excelͳ�Ʊ��泣�� */
#define         MAXSUMMARYPARA          6   /* �ܽ�(Page1)��� 6����ʾ����, 3ռ4�� / 4ռ8�� */
#define         MAX3VALUES              4
#define         MAX4VALUES              8
#define         MAXREPORTPARA           3   /* ����(Page4)��� 3����ʾ���� */

/* ��ʾ���������ƶ��壬�ڴ���д����ȷ����ʾ������/˳�� */
#define         MAXNAMELEN              32
typedef struct tagSHOWPARANAME
{
    WORD        wNameID;
    string      strName[LANGUAGE_NUM];
}SHOWPARANAME;

/* ��ǰŤ�ص���ʾ���������ã����浽�ļ��� */
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
    WORD        wParaNum;               /* ��ǰ��ʾ��������;ͼ�β�������������ʾ�������� */
    WORD        wListNum;               /* ��ǰ�б���ʾ�������� */
    WORD        wMainNum;               /* ��ǰ��������ʾ�������� */
    WORD        wFileName;              /* ���������ļ��Ĳ������Ǹ��������е�һ�� */
    WORD        wStatType;              /* ͳ�Ʋ������Ǹ��������е�һ�� */
    SHOWPARA    tShow[MAXPARANUM+1];    /* ��ǰѡ�����ʾ������ֵ +1 ����(16)����������ʱʹ�� */
    SHOWOPTION  tOption[MAXPARANUM+1];  /* ��ǰ��ʾ�����Ŀ�ѡ�� +1 ����(16)����������ʱʹ��  */
    string      strList[MAXPARANUM+1];
    string      strMain[MAXPARANUM+1];

    BOOL        bFixTub;        
    TUBINGCFG   tTubingCfg;               /* 5���͹ܲ����Ĺ̻����壬�ǹ̻�������SHOWPARA�� */
}SHOWCFG;

/* XLSͳ�ƽ���������� */
typedef struct tagXLSSTATCFG
{
    string      strName[MAXPARANUM];    /* ��ʾ�������� */
    BOOL        bSummary[MAXPARANUM];   /* ժҪ(Page1)��ʾ������� */
    BOOL        bReport[MAXPARANUM];    /* ����(Page4)��ʾ������� */
    BYTE        ucOperator;             /* ����೤(������)������� */
    BYTE        ucTally;                /* �뾮��Ų������ */
}XLSSTATCFG;

/* dwQuality��������Լ�� */
/*
    bit0    - 1:GOOD; 0:BAD
    ����λ��ʾ����NOKԭ��
    bit1    - TORQ_LESS_LIMIT   ����Ť������
    bit2    - TORQ_MORE_LIMIT   ����Ť������
    bit3    - TORQ_MORE_START   ��ʼŤ�ش��ڿ���Ť��15%
    bit4    - TORQ_MORE_CTRL    ���ڿ���Ť��10%
    bit5    - CIRC_LESS_LIMIT   ����0.20Ȧ��
    bit6    - CIRC_MORE_LIMIT   ����Ȧ������
    bit7    - TRANSLATE         ͼ�γ���̨��Ť��(����Ť��15%)��ƽ��
    
    bit8    - NOIPPOINT         �޹յ�Ť�� // ����汾������
    bit9    - LOW_SHOULD        �յ�Ť��<Min shoulder(15%)
    bit10   - HIGHT_SHOULD      �յ�Ť��>Max shoulder(70%)
    bit11   - LOW_DELTATURN     < Min D. Tns
    bit12   - HIGHT_DELTATURN   > Max D. Tns
    bit13   - LOW_SLOPE         б�� < Min Slope
    bit14   - OTHER_CAUSE       ����,���ݲ�ͬ��׼���������ڷ�Χ�ڵĹ��õ�������
    bit15   - SHACK_INSPECT     ж�ۼ��        �˹��趨

    bit16   - GALLING           ճ��            �˹��趨
    bit17   - HYDTONGSLIP       Һѹǯ��      �˹��趨
    bit18   - THREADNOTCLEAN    ˿����ϴ���ɾ�  �˹��趨
    bit19   - GASSEALINSPECT    ���첻�ϸ�  �˹��趨
*/
#define QUA_RESU_QUALITYBIT     0x00000001 /* ����λΪ���λ */
#define QUA_RESU_BAD            0x00000000
#define QUA_RESU_GOOD           0x00000001

#define QUA_TORQ_LESS_LIMIT     0x00000002
#define QUA_TORQ_MORE_LIMIT     0x00000004
#define QUA_TORQ_MORE_START     0x00000008
#define QUA_TORQ_MORE_CTRL      0x00000010
#define QUA_CIRC_LESS_LIMIT     0x00000020
#define QUA_CIRC_MORE_LIMIT     0x00000040
#define QUA_TRANSLATE           0x00000080 /* ͼ�γ���̨��Ť�غ�ƽ�� */
#define QUA_NOIPPOINT           0x00000100 /* �޹յ�Ť�� */
#define QUA_LOW_SHOULD          0x00000200
#define QUA_HIGHT_SHOULD        0x00000400
#define QUA_LOW_DELTATURN       0x00000800
#define QUA_HIGHT_DELTATURN     0x00001000
#define QUA_LOW_SLOPE           0x00002000
#define QUA_OTHER_CAUSE         0x00004000
#define QUA_SHACK_INSPECT       0x00008000  /* ж�ۼ�� �ֹ����� shackle inspection */
#define QUA_GALLING             0x00010000  /* ճ�� �ֹ����� */
#define QUA_THREADNOTCLEAN      0x00020000  /* ˿����ϴ���ɾ� �ֹ����� */
#define QUA_GASSEALINSPECT      0x00040000  /* ���첻�ϸ�   �ֹ����� */
//#define QUA_HYDTONGSLIP         0x00020000  /* Һѹǯ�� �ֹ����� ɾ�� */

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

/*  ״̬����μ�TorqueDlg.h, ��PLCSTATUS_WAIT
    0������/��ת�����ݶ���
    1��������ʾ��
    2������
    3��ж��
    4: ����
    CC: �Ͽ�/ж�۷�ת��Ť������/ż������
    F0: �������ʱ�������ظ�������������Ҫ����continue
    FE:ж�۽���254
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
    double      fTorque;        /*��ǰŤ��*/
    double      fRpm;           /*��ǰת��*/
    int         iPointNum;      /*һ�����ݻ�ͼ�ĵ���
                                  0;û��ת��ֻ�Ǹ���Ť��ֵ��������ͼ
                                  1:��������һ�����ݣ���һ����
                                  2~N:�м����ݶ�ʧ����һ��plus�Ƚ϶࣬��Ҫ���������
                                      �����Զ����������ݣ�ʹͼ�����ƽ������*/
    UINT        nOrgPlus;       /* ԭʼ��¼��������������ת��ʱ�����ȥshowplus */
    BYTE        ucStatus;       /* ��λ����״̬, ͬORGDATA��ucStatus���� */
}COLLECTDATA;

/* ͳ�Ʋ������� */
#define     STATRANGENUM        3
typedef struct tagSTATCFG
{
    double  fCtrlRange[STATRANGENUM];       /* �ؼ�Ť�ط�Χ */
    double  fShouldRange[STATRANGENUM];     /* �յ�Ť��ͳ�Ʒ�Χ */
    double  fDeltaRange[STATRANGENUM];      /* Delta����ͳ�Ʒ�Χ */
}STATCFG;

typedef struct tagASTAT
{
    ADAPTER_STATUS  adapt;
    /*NAME_BUFFER     NameBuff [30];*/
} ASTAT;

#define     MAXSPLIITNUM        10
typedef struct tagSplit
{
    int     iCur;           /* ��Χ1~iSplitNum */
    int     iCtrlPnt;       /* ����������Ӧ�������Ͽ���Ч��ж��д��Ϊ500 */
    int     iSplitNum;
    int     iBegin[MAXSPLIITNUM];
    int     iEnd[MAXSPLIITNUM];
}SPLITPOINT;

#define     MAXWELLNUM   5000
typedef struct tagTORQUEDATA
{
    UINT    nCur;               /* nCur��1��ʼ�������൱���������+1 */
    UINT    nTotal;
    UINT    nQualy;
    UINT    nUnQualy;
    BOOL    bHaveHead;  /* �ļ���ÿ������ǰ���Ƿ���ͷ���� */
    UINT    nTotalPlus[MAXWELLNUM];
    TorqData::Torque  tData[MAXWELLNUM];
    SPLITPOINT        tSplit[MAXWELLNUM];   // ��������ʱ����ȡ�����÷�������յ�
}TORQUEDATA;

/* ���10�������ǷŴ�5����10*500*5 */
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

//6k�洢�ռ䣨1M�ȴ������
//#define     MAXSAVEDATALEN  1048576
//#define     MAXSAVELEN      1000000  
#define     MAXSAVEDATALEN  65530
#define     MAXSAVELEN      60000  
typedef struct tagSAVELOGDATA
{
    int         iCur;
    char        aucLog[MAXSAVEDATALEN];
}SAVELOGDATA;


/* �������������� */
#define     VALVETYPENUM        2
#define     VALVERATIONUM       3
typedef struct tagVALVECFG
{
    BYTE    ucTorq[VALVETYPENUM][VALVERATIONUM];    /* Ť�ر��� */
    BYTE    ucRatio[VALVETYPENUM][VALVERATIONUM];   /* ��ֵ���� */

    //BYTE    ucZoomRatio;                            /* �Ŵ��� */
}VALVECFG;

/* У׼�������� */
#define     CALIBNUM            7
#define     CALIBPARALEN        10
typedef struct tagCALIBCFG
{
    BYTE    ucLoad[CALIBNUM][CALIBPARALEN];     /* ���� */
    BYTE    ucStroke[CALIBNUM][CALIBPARALEN];   /* ���̶��� */
    BYTE    ucReturn[CALIBNUM][CALIBPARALEN];   /* �س̶��� */
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
    void    WriteConfigStr(CString strclassName,CString strclassItem, CString strValue, string strParaName);
    void    WriteConfigStr(WORD wclassName, WORD wclassItem, CString strValue, string strParaName);
    void    AdjustParaValue(PARACFG *ptCfg); /* �������Ƿ����仯 */
    /* ���б����ݵ�����excle����� */
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
    SHOWCFG         m_tShowCfg[LANGUAGE_NUM];         /* ��ʾ���������в������� */
    SHOWCFG         *m_ptCurShow;
    XLSSTATCFG      m_tXlsStatCfg;
    DBREG           m_tdbReg;
    VALVECFG        m_tValveCfg;        /* ��ֵ���� */
    //CALIBCFG        m_tCalibCfg;        /* У׼�������� */
    string          m_strDllFile;       /* ��̬���ӿ��ļ����� */
    string          m_strAppPath;
    string          m_strParaFile;      /* ����ϵͳ���������ļ�
                                           ��.exeͬ·������׺��Ϊ.ini */
    string          m_strShowFile;      /* ����ϵͳ��������ʾ���������ļ������� 
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
    string          m_strParity;        /* ������żУ����ַ��� */
    BOOL            m_bBigTorq;         /* �Ƿ�Ϊ��Ť�ذ汾2.4.9, TRUE:�͵�Ƭ��Ť�����10��,FALSE:1�� */
    BOOL            m_bShackle;         /* �Ƿ���ж�۰汾���ǵ�����Ť�ز������ߣ�����Ƭ��������ʾ */
    BOOL            m_bCheckIP;         /* �����ж��Ƿ����ø߼��жϣ����ж��Կ���Ť��Ϊ׼�� �������Ť�غ͵�����СŤ��Ϊ���ϸ�Ĭ��Ϊ1 */
    //BOOL            m_bFirstRun;      /* �Ƿ��һ�����У��ǵĻ�����DlgBear */
    BOOL            m_bFileBehindDate;  /* �������ļ������ĺ��� */
    UINT            m_nTorqMulti;       /* BigTorq: True:10; False:1*/
    UINT            m_nTestFunc;        /* 0: ��ʵ���ڣ�1��ģ����ԣ�2����ȡdatŤ�����ݣ�3: ��ȡdat ��ʷ����; 4: ������ʷ����*/
    UINT            m_nColletTime;      /* ��ʱ�ռ����ݵ�ʱ�䣬ms��Ĭ��250 */
    UINT            m_nSaveTime;        /* ������ʾŤ�غ󱣴����ݵ�ʱ�䣬Ĭ��30s */
    UINT            m_nReset;           /* ��λʱ�䣬Ĭ��10s */
    UINT            m_nImgNum;          /* ��������ͼ��ʱ��һ��jpg�ļ��а������ٸ�ͼ�� */
    UINT            m_nZoomIn;          /* ͼ�ηŴ��� */
    UINT            m_nIPShowMode;      /*  1: ֻ�������еĹյ�
                                            2: ֻ������յ�
                                            3: ���ݹյ�ͼ���յ㶼��*/
    double          m_fIPDeltaVal;      /* Ĭ��0.7 */
    UINT            m_nCurNO;           /* ��ǰŤ�ص���ţ������ļ����� */
    DWORD           m_dwTotalTorqNum;   /* Ť���Ͽ������Ŀ */
    BOOL            m_bParaChg;         /* ���������Լ��޸ģ���Ҫ������д���ļ� */
    UINT            m_nPBHead;
    BOOL            m_bShowCRC;
    UINT            m_nTorqUnit;
    CString         m_strUnit;
    SAVELOGDATA     m_tSaveLog;
    CFile           m_SaveLogFile;
    CString         m_strDbgHead[DBG_MAXNUM];
    char            m_aucPassWord[MAXPWLEN];

    /* ������֧�� */
    HINSTANCE       m_hLangDLL[LANGUAGE_NUM];
    UINT            m_nLangType;        /* ��ǰ�������� */

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

    /* ���б����ݵ�����excle����� */
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
    
    /* �Ҳ���MACʱ��Ĭ��MAC */
    BYTE     m_ucDefaultMac[5];
    /* ������Ч������ */
    UINT     m_nBand[10];
};


extern CTorqueApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TORQUE_H__B8387AE5_653F_4729_8A7A_95783AA1DEC1__INCLUDED_)

