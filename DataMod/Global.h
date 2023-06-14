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

/* ����ĳ���ȫ�ֺ궨�� */
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
/*��ʱ��ID����*/
#define COOLCONTROL_TIMER       0       // COOL�ؼ�ʹ�ö�ʱ��
#define PORTREAD_TIMER          1       // ��ȡ�������ݶ�ʱ��
#define GUARD_TIMER             2       // �Ͽۺ󱣻���ʱ��
#define GUARDREAD_TIMER         4       // �Ͽ۱���ʱ��ȡ�������ݵĶ�ʱ��
#define BEARSHOW_TIMER          5       // ��ʾ�縺ѡ��Ի���ʱ��
#define BEARSHOW_LEN            5000    // ��������ǰѡ��縺�ʹ�Ť�ضԻ���ȴ�ʱ��
#define AUTOSAVE_TIMER          6       // ��ʱ����CRC����͵�����Ϣ��ʱ��
#define AUTOSAVE_TLEN           5000    // ��ʱ����CRC����͵�����Ϣ��ʱʱ��
#define PORTBUFF_TIMER          7       // 485���ڱ�����ʱ������λ����������ǰ��������������ݣ���ʱ����
                                    // ��Ƭ��������ɺ��ٷ�������
#define PORTBUFF_TLEN           15      // ���ڷ���12��BYTE��Ҫ13ms,��ʱ������Ϊ15ms
#define WITSRPT_TIMER           8       // ��ʱͨ��TCP�ϱ�WITS���ݸ��ɼ��ն�
#define WITSRPT_TLEN            1000    // 1s�ϱ�һ������
#define COLLECT_TIMER           10      // �ռ���Ƭ�����ݶ�ʱ��
#define COLLECT_TLEN            1000    // �ռ���Ƭ�����ݶ�ʱʱ�� //2000
#define WNDSHOW_TIMER           11      // ������ת��ʾ��ʱ��
#define WNDSHOW_TLEN            100     // ������ת��ʾ��ʱʱ��
#define RESET_TIMER             12
#define ALARM_TIMER             13
#define ALRAM_DELAY             5000    /* �澯������ʱ��5s */
#define READVALVE_TIMER         14      /* ��ʱ��ȡ����״̬��ʱ����ʱ��1s */
#define READVALVE_LEN           1000
#define SETCALIB_TIMER          15      /* ����У׼��Ϣ��ʱ�� */
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
#define CLR_ABNORMDATA          RGB(255,0,0)        // �쳣������ɫ
#define CLR_TOOLBUCKLE          RGB(0,0,255)        // ���߿�������ɫ
#define CLR_ALTERBACKG          RGB(215,247,241)    // ���б�����ɫ

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

#define PORTOPR_OPEN    0   /* �򿪴��� */
#define PORTOPR_CLOSE   1   /* �رմ��� */
#define PORTOPR_MAXNUM  (PORTOPR_CLOSE +1)

#define PORTBUFF                108

/* 0: ��ʵ���ڣ�1��ģ����ԣ�2����ȡdatŤ�����ݣ�3: ��ȡ����Ť�����ݣ�4������־�лָ����ݣ����ڵ�ǰ���ݿ�Ͷ��д���5: ��ȡdat ��ʷ���ݣ� */
#define COLL_PORT               0
#define COLL_RAND               1
#define COLL_TORQUE             2
#define COLL_MULTITORQ          3
#define COLL_RECOVERY           4
#define COLL_HISTORY            5   // no used in glbcfg

/* ȫ0�Ķ�ȡ����ԭʼ����
    0x21,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x22,0x00,0x00,0x08
    PORT  ��   ������     Torque     PLUS   8B��    CRC    ����
*/
#define PORT485                 0x21    /* 485���ڵ���Ч��ʼֵ */
#define BUS_READ                0x03    /* MODBUS������ */
#define BUS_WRITE               0x06    /* MODBUSд���� */
/* MODBUS����������ʼ��ַ */
#define MODBUS_CONTENT          m_ucSndByte[4]

#define PORTSENDTIME            200     /* ���ڷ��������ı���ʱ�� sleep 200ms */

/* ���ö�ʱ��ȡ����ʱ��״̬ */
/* ����Ϊ����״̬����Ҫ�ȶ����壻ż��Ϊ�����Ѿ���գ������������� */
#define RS_NORMAL               0       /* ����״̬��������Ϊ�գ�ֱ�ӷ��ͣ��������ǿ�תCLASH״̬ */
#define RS_READCLASH            1       /* READʱ��Ƭ������ǿ� */
#define RS_RESETCLASH           2       /* RESETʱ��Ƭ������ǿ� */
#define RS_MAX                  RS_RESETCLASH

#define RS_READ_NORMAL          0       /* ����״̬ʱ������ */
#define RS_READ_RESET           1       /* ��λ״̬ʱ������ */

#define PORT_MAXDATANUM         10      /* ����һ������ȡ���ݸ��� */
#define PORT_VALIDLEN           12      /* ��Ч���ݳ���Ϊ12 */
#define PORT_DATAPLACE          4       /* ʵ��������ʼλ��4 */
#define PORT_DATALEN            7       /* ʵ�����ݳ��� 7;+1CHECK+2SPEED  */
#define PORT_SUMPLACE           8       /* ǰ������У���λ�� 8 */
#define PORT_VLDVLVLEN          14      /* ��2���ֽڷ���״̬����Ч���ݳ���Ϊ14 */
#define PORT_MAXVLDDATA         9       /* +�������ݳ��� 9; 4+1CHECK+2SPEED+2VALVE */
#define PORT_GRPDATA            4       /* ��ȡ��������ʱ��ÿ����Ч���ݵĳ���Ϊ4 */

#define ORG_DATAPLACE           15      /* ʵ��������ʼλ��15 */
#define ORG_PRNTIMELEN          12      /* �Զ������ӡ��ʱ�䳤�� 04:05:45.984 : */
#define ORG_PRNDATALEN          12      /* �Զ������ӡ�����ݳ��� 36 cc 43 56 */

/* MODBUS�Զ������� */
#define COM_READ                0x0001  /* ��ȡ���� */
#define COM_ADJUST              0x0002  /* У׼���� */
#define COM_QIPI                0x0003  /* ȥƤ */
#define COM_COLLECT             0x0004  /* �ɼ���Ƭ������ */
#define COM_SHOW                0x0005  /* ��ʾŤ�� */
#define COM_SPEED               0x0006  /* ����Ť�� */
#define COM_CONTROL             0x0007  /* ����Ť�� */
#define COM_BSPEED              0x0008  /* ��ʼ���ٿ��� */
#define COM_ESPEED              0x0009  /* ֹͣ���ٿ��� */
#define COM_BUNLOAD             0x000A  /* ��ʼж�ɿ��� */
#define COM_EUNLOAD             0x000B  /* ֹͣж�ɿ��� */
#define COM_UPPER               0x000C  /* ����Ť�� */
#define COM_LOWER               0x000D  /* ����Ť�� */
#define COM_CUT                 0x000E  /* ����ϵ�� */
#define COM_TIME                0x000F  /* ����ʱ�� */
#define COM_VPRESS              0x0010  /* ��ѹ�� */
#define COM_VFLOW               0x0011  /* ������ */
#define COM_VZOOM               0x0012  /* �Ŵ��� */
#define COM_READVALVE           0x0014  /* ��ȡ����״̬ */
#define COM_READMULTI           0x0015  /* ��ȡ�������� */
#define COM_WRITECALIB          0x0017  /* �ֶ�У׼ д */
#define COM_CTRLCALIB           0x0018  /* �ֶ�У׼���� */
#define COM_READCALIB           0x0019  /* �ֶ�У׼ �� */
/* ��Ƭ������� */
#define SCMREAD                 15      /* ��ȡ���� */
#define SCMADJUST               16      /* У׼ */
#define SCMQIPI                 17      /* ȥƤ */
#define SCMCOLLECT              18      /* �ɼ����� */
#define SCMSHOW                 19      /* ��ʾŤ�� */
#define SCMSPEED                20      /* ����Ť�� */
#define SCMCONTROL              21      /* ����Ť�� */
#define SCMBSPEED               22      /* ��ʼ���� */
#define SCMESPEED               23      /* ֹͣ���� */
#define SCMBUNLOAD              24      /* ��ʼж�� */
#define SCMEUNLOAD              25      /* ֹͣж�� */
#define SCMUPPER                26      /* ����Ť�� */
#define SCMLOWER                27      /* ����Ť�� */
#define SCMCUT                  28      /* ����ϵ�� */
#define SCMCOLLECTOK            29      /* �յ���Ч�����ٴβɼ����ݣ����һ��ȷ�Ϻ���Ҫ�ٴ��յ����� */
#define SCMCOLLECTNOK           30      /* �յ���Ч�����ٴβɼ����� */
#define SCMTIME                 31      /* ���õ�Ƭ��ʱ�� */
#define SCMVPRESS               32      /* ��ѹ�� */
#define SCMVFLOW                33      /* ������ */
#define SCMREADVALVE            34      /* ��ȡ����״̬ */
#define SCMREADMULTI            35      /* ��ȡ�������� */
#define SCMWRITECALIB           36      /* д�ֶ�У׼ */
#define SCMCTRLCALIB            37      /* �ֶ�У׼���� */
#define SCMREADCALIB            38      /* ���ֶ�У׼ */
// #define SCMVZOOM             34      /* �Ŵ��� */
/*#define CIRNUMS               4615.4*/    /* 1200����ΪһС��,100С��Ϊ26����,1200/0.26= */

#define RS_COMM_CLOSE           0       /* ���ڹر� */
#define RS_COMM_OPEN            1       /* ���ڴ� */

/* ���ڲ��Դ򿪣�ֵΪ m_nTestFunc+1 */
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

#define RPM_OPTNUM              5       /* RMP�Ż�����Ŀ */
#define RMP_OPTTHRES            20      /* RMP��Ҫ�Ż�����ֵ */
/* 20220215 ʵ���Ͽ�ʱ��Ť�ش��� */
#define SWITCHLOWLIMIT          5000    /* ����Ť��С�ڸ�ֵʱ������СŤ���·�����Ť�� */

/* �������õ���Ĭ��ֵ */
#define DIFF_TORQUE             10
#define DIFF_CIRCUIT            0.1
#define DIFF_TIME               1
#define FULLCIR4SAVE            0.20    // �ɼ�Ƶ�� 1/2500  1��/(500/0.2)
#define AUTOUPDTURNRATIO        0.8     // ����80%(400)���Զ���������

#define IP_SLOPE_PER            1       /* Ĭ�Ϲյ�ٷֱ� */


#define MAXREVERSEPLUS          300     /* 20201219 ���ת�����ݶ�300 */

#define STEPTORQUE              20      /* ��ƽͻ��ʱ����һ�����ݱ�ǰһ�����������Ĳ��� */
#define PRIKETORQ               500     /* С�����Ť����С��̨��Ť�أ�����ƽ */

// �Ͽ�/ж���������Ͷ���
#define TYPE_MAKEUP             0x01    // 0001
#define TYPE_BREAKOUT           0x02    // 0010
#define TYPE_TOTAL              0x03    // 0011

/*
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
#define PLCSTATUS_WAIT          0
#define PLCSTATUS_NORMAL        1
#define PLCSTATUS_DECEL         2
#define PLCSTATUS_UNLOAD        3
#define PLCSTATUS_LOCKON        4
#define PLCSTATUS_REVERSE       0xCC
#define PLCSTATUS_REPEAT        0xF0
#define PLCSTATUS_BREAKOUT      0xFE
#define PLCSTATUS_CRCERR        0xFF

/* Ŀǰ��4λ��Ч���ɸߵ��ͷֱ����ж�ۡ��Ͽۡ���������ѹ������ж�ɷ������ٷ���=1 ��ʾ���� */
#define VALVEMAXNUM             6
#define VALVESTATUS_SPEED       0x0001
#define VALVESTATUS_UNLOAD      0x0002
#define VALVESTATUS_PRESS       0x0004
#define VALVESTATUS_FLOW        0x0008
#define VALVESTATUS_MAKEUP      0x0010
#define VALVESTATUS_BREAKOUT    0x0020

#define INDEX_TORQ_MAXLIMIT         0       /* ������� */
#define INDEX_TORQ_UPPERLIMIT       1       /* ���Ť�� */
#define INDEX_TORQ_CONTROL          2       /* ����Ť�� */
#define INDEX_TORQ_OPTIMAL          3       /* ���Ť�� */
#define INDEX_TORQ_LOWERLIMIT       4       /* ��СŤ�� */
#define INDEX_TORQ_SPEEDDOWN        5       /* ����Ť�� */
#define INDEX_TORQ_SHOW             6       /* ��ʾŤ�� */
#define INDEX_TORQ_BEAR             7       /* �縺Ť�� */
#define INDEX_TORQ_UPPERTAI         8       /* ���̨��Ť�� */
#define INDEX_TORQ_LOWERTAI         9       /* ��С̨��Ť�� */
#define INDEX_TORQ_INITBO           10      /* ж�۳�ʼŤ�� */
#define MAXTORQCONFNUM              (INDEX_TORQ_INITBO+1)


#define INDEX_TURN_MAXLIMIT         0       /* ������� */
#define INDEX_TURN_UPPERLIMIT       1       /* �������� */
#define INDEX_TURN_CONTROL          2       /* �������� */
#define INDEX_TURN_LOWERLIMIT       3       /* �������� */
#define INDEX_TURN_MAXDELTA         4       /* ���Delta����0.1 */
#define INDEX_TURN_MINDELTA         5       /* ���Delta����0.1 */
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

/* ���excelͳ�Ʊ��泣�� */
#define         STATPARA_GENNUM         4
#define         STATPARA_JOBNUM         6       /* �ܽ�(Page1)��� 6����ʾ����, 3ռ4�� / 4ռ8�� */
#define         STATPARA_INFONUM        2       /* ����(Page4)��� 3����ʾ���� */
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
/* ���������ϢԼ����Ϣͷ��ֵ���ַ��� */
#define DBG_HASH                    0   /* # */
#define DBG_START                   1   /* * reset */
#define DBG_COLLECT                 2   /* collect */
#define DBG_MESSAGE                 3   /* MessageBox��ʾ��Ϣ */
#define DBG_SNDCMD                  4   /* ���ʹ������� */
#define DBG_RCVCOM                  5   /* ���մ�����Ϣ */
#define DBG_SNDTCP                  6   /* ����TCP��Ϣ */
#define DBG_MAXNUM                  (DBG_SNDTCP+1)
/* ������Ϣͷ�ĳ��ȹ̶�Ϊ4 */
#define DBG_HEADLEN                 5
#pragma endregion

#pragma region MAX RANGE

#define     MAXPWLEN            32
#define     MAXSKIPLEN          64  /* �ܹ���64���ֽ� */

#define     MAXPROBUFF          819200  // 3.22 buf����2��
#define     OVERFLOWTORQ        300000  /* �����Ť��ֵ��������ֵ�϶���Ч */

//#define     VITTANAMENUM        50

#define     SPRINTFLEN          60

#define     MAXCMDNUM           24
/* ���ͨ���ж�ʱ��5s */
#define     MAXCOMMBREAKTIME    5000

#define     DEFIPDELTAVAL               0.1

#define     DEFTALLYNAME                _T("�뾮���")
#pragma endregion

#pragma region STATUS
/* ��������״̬ */
#define STATUS_INVALID          0   /* ��Ч״̬ */
#define STATUS_START            1   /* ��������״̬ */
#define STATUS_EXIT             2   /* �����˳�״̬ */
#define STATUS_RUN              3   /* ��������״̬ */
#define STATUS_STOP             4   /* ����ֹͣ״̬ */
#define STATUS_SETPARA          5   /* ��������״̬ */
#define STATUS_FUNTEST          6   /* ���ܵ���״̬ */
#define STATUS_COLLECT          7   /* �ռ���Ƭ������״̬ */
#define STATUS_HISTORY          8   /* ��ȡ��ʷ����״̬ */
#define STATUS_RESTART          9   /* ���´򿪴��� */
#define STATUS_CHGLAN           10  /* �ı��������´򿪴��� */
#define STATUS_HISSTAT          11  /* �Ͼ�����ͳ�� */
#define STATUS_SETVALVE         12  /* ���ñ��������� */
#define STATUS_CIRCLE           13  /* ����Ϊ�ܰ汾 */
#define STATUS_TIME             14  /* ����Ϊʱ��汾 */
#define STATUS_SETSHOW          15  /* �޸Ľ�����ʾ���� */
#define STATUS_CHGUNIT          16  /* �޸�Ť�ص�λ */
#define STATUS_CALIB            17  /* У׼ */
#define STATUS_TUBECFG          18  /* �͹ܲ������� */
#define STATUS_MAXNUM           (STATUS_TUBECFG +1)
#pragma endregion

#pragma region DATABASE
/* ��������״̬ */
#define D_LANGUAGE              0   /* �����ֵ�� */
#define T_SHOWNAME              1   /* ��ʾ�������Ʊ� */
#define T_GLBCFG                2   /* ȫ�����ñ� */
#define T_SHOWCFG               3   /* ��ʾ�������ñ� */
#define T_SHOWOPTION            4   /* ��ʾ����ѡ��� */
#define T_VALTORQUE             5   /* Ť�ؿ���ֵ���ñ� */
#define T_VALTURN               6   /* ��������ֵ���ñ� */
#define T_TORQUECFG             7   /* �������ֵ���ñ� */
#define T_XLSSTATCFG            8   /* ����excelͳ�����ñ� */
#define T_VALVECFG              9   /* ���������ñ� */
#define T_SERVERCFG             10  /* FTP���������ñ� */
#define T_WITSCFG               11  /* WITS(TCP)���ñ� */
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
#endif
#define         HALFPARALEN             25
#define         MAXPARALEN              50

#define         MAXPARANUM              16  /* 0 Factory + 15  */
#define         MAXMAINPARA             8   /* 0 Factory + 7�� */
#define         MAXOPTIONNUM            50
#define         MAINSHOWTUBE            5   /* ��5����������ʾ����Ϊ�������(5) */
#define         MAINSHOWTALLY           6   /* ��6����������ʾ����Ϊ�뾮���(6) */

/* 15������, 4~7�̶� */
#define         FIXSHOWBEGIN            4   /* ��4����ʾ�����̶�(4) */
#define         FIXSHOWEND              7   /* ��7����ʾ�����̶�(7) */
//#define         TUBESN                  9   /* ������� */
//#define         TALLYNO                 10  /* �뾮��� */

/* ������7������, 1~6�̶� */
#define         MAINSHOWBEGIN           0   /* ��1����ʾ�����̶�(1-1) 0 for Factory*/
#define         MAINSHOWEND             4   /* ��4����ʾ�����̶�(4-1) */

//#define         CBMAINSHOW_FACTORY      0
//#define         CBMAINSHOW_OEM          1
//#define         CBMAINSHOW_SIZE         2
//#define         CBMAINSHOW_MAT          3
//#define         CBMAINSHOW_COUPL        4

/* DlgParaSet ϵͳ�ܲĵ�combobox��Ӧ��ʾ��������� */
#define         FIXINDEX_FACTORY        0
#define         FIXINDEX_OEM            4
#define         FIXINDEX_SIZE           5
#define         FIXINDEX_MATER          6
#define         FIXINDEX_COUPL          7
#pragma endregion

#pragma region QUALITY

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
#pragma endregion

#pragma region OTHER
#define VALID_YEAR              5       // �����Ч�����
/* ǿ������CTime�ĳ���Ϊ4, VC2008��Ϊ����Ϊ8��VC6��ΪΪ0�����߲�һ�� */
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
/* TCP����Э�飺
1��֡ͷ && ��֡β!!��ÿ������֮��س����зָ���ʮ������Ϊ0D 0A��
2��ÿ�п�ͷ�̶���Ŀ���ֶ�Ϊ80
3�����崫�����ݣ�ÿ�н�һ������������ݣ�һ�����ݿɰ�����������
4��ÿ�����贫�����ݰ�����ΪŤ�����鳤�� + 1������涨Ϊ��
(1)	ÿ�����ݹ̶�����01��02��03����
(2)	��һ�����ݴ���01~03��11~28
(3)	���������ݴ���01~03��51~53
*/
#define WITSRPT_FIXHEADNUM      3   // ÿ�����Ĺ̶�����: ���ڣ�ʱ�䣬�׹ܺ�
#define WITSRPT_REPEATNUM       3   // �ظ��ϱ�����: Ť�أ�������ʱ��
#define WITSRPT_CALPARANUM      5   // �Ͽ���ɺ�Ť�ؼ�������������Ͽ�Ť�أ��յ�Ť�أ�����Ť�أ�̨�ױȣ�̨��ʱ���
#define WITSRPT_SHOWPARANUM     15  // ���15����ʾ����

#pragma endregion

#pragma region Struct DEFINE

typedef struct tagCONTROLPARA
{
    /* Ť����ʾ��ز��� */
    double      fTorqConf[MAXTORQCONFNUM];
#if 0
    double      fMaxLimit;    /* ������� */
    double      fUpperLimit;  /* ���Ť�� */
    double      fControl;     /* ����Ť�� */
    double      fOptTorq;     /* ���Ť�� */
    double      fLowerLimit;  /* ��СŤ�� */
    double      fSpeedDown;   /* ����Ť�� */
    double      fShow;        /* ��ʾŤ�� */
    double      fBear;        /* �縺Ť�� */
    double      fUpperTai;      /* ���̨��Ť�� */
    double      fLowerTai;      /* ��С̨��Ť�� */
#endif
    //double      fCut;         /* ���۱��� 0.8 */
    /* Ťš���� */
    double      fTurnConf[MAXTURNCONFNUM];
#if 0
    double      fMaxCir;      /* ������� */
    double      fUpperCir;    /* �������� */
    double      fControlCir;  /* �������� */
    double      fLowerCir;    /* �������� */
#endif
    //double      fPlus;        /* �������� */
    /* ����ת�� */
    double      fFullRPM;       /* ��ͼ�ϵ����ת�� */
    double      fMinShlSlope;   /* ��С�縺б��5.0 */

    WORD        wIPPos;       /* �յ�������λ�ã�0��ʾû�йյ�,�����������ݣ��ֹ�����յ� */
    BYTE        ucVer;        /* 0: 2017�����ݽṹ
                                 1: ��ʾ�������Ƴ��ȹ̶�25�����30����ʾ������NLV��ʽ: ��ʾ������ֵ���ȸ���L
                                    0��14��������ʾ���ƣ�15 Factory
                                 2: 2022 0Factory, 1~15�������� */
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
    UINT    nFactory;       // ����  JFE
    UINT    nOEM;           // �ܼ�����  2-7/8�� JFE
    UINT    nSize;          // �ܼ���� TubingSize
    UINT    nMat;           // ���Ͳ��� EUE          N80
    UINT    nCoupling;      // �ӹ���� 93.2X    �׻�
#else
    UINT    nFixTube[MAXTUBECFGNUM];
#endif
#if 0
    /* N.m */
    UINT    nMaxTorq;       // ���Ť��
    UINT    nOptTorq;       // ���Ť��
    UINT    nMinTorq;       // ��СŤ��
    /* lb.ft */
    UINT    lMaxTorq;       // ���Ť��
    UINT    lOptTorq;       // ���Ť��
    UINT    lMinTorq;       // ��СŤ��
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
//    double      fMaxTorq;       /* ���ã���¼���ݵ����Ť�� */
//    /* double�������� ���16�� */
//    //double      fRpmAdj;        /* ת�ٵ��� */
//    //double      fMulti;         /* У׼������Χ0~2 */
//    //double      fMinShlSlope;   /* ��С�縺б��5.0 */
//    //double      fMaxDeltaCir;   /* ���Delta����0.1 */
//    //double      fMinDeltaCir;   /* ��СDelta����0.1 */
//    double      fRsv[12];        /* ����double�ֶΣ���16�� */
//
//    /* DWORD�������� ���16�� */
//    DWORD       dwSeqNo;
//    DWORD       dwRsv[15];
//
//    /* ����ֵ���ã�ÿ������ֵʹ��һ��BITλ */
//    /* BITλ���� ���8*32�� */
//    DWORD       bBear : 1;        /* �Ƿ��м縺,Ĭ��Ϊû�� */
//    DWORD       bToolBuck : 1;    /* �Ƿ�Ϊ���߿�,Ĭ��Ϊ�� */
//    DWORD       bRsv : 30;
//    DWORD       dwRsv2[7];
//
//    //char        aucMemo[MAXMEMOLEN];        /* ��ע */
//    //char        aucRemark[MAXPARALEN];      /*��ע˵��remark*/
//}COMMONCFG;

typedef struct tagPARACFG
{
    CONTROLPARA tCtrl;
    //COMMONCFG   tComm;
    TUBECFG     tTubeCfg;               /* 5���͹ܲ����Ĺ̻����壬�ǹ̻�������SHOWPARA�� */

    string      strAlias;
    string      strValue[MAXPARANUM];   /* ��ǰѡ�����ʾ������ֵ +1 ����(0)����������ʱʹ�� */

    //string      strAlias;
    string      strMemo;
    string      strRemark;
}PARACFG;

typedef struct tagGLBCFG
{
    UINT        nLangType;      /* �������� */
    //UINT        nParaIndex;     /* ���Ʋ����������� */
    UINT        nPortNO;        /* ���ں� */
    UINT        nBaudRate;      /* ������ */
    UINT        nPlusPerTurn;   /* �������� */
    UINT        nTorqUnit;      /* Ť�ص�λ: 0:N.m; 1:lb.ft */
    UINT        nCollectDur;    /* ��ʱ�ռ����ݵ�ʱ�䣬ms��Ĭ��250 */
    UINT        nResetDur;      /* ��λʱ�䣬Ĭ��10s */
    UINT        nSaveDur;       /* ������ʾŤ�غ󱣴����ݵ�ʱ�䣬Ĭ��30s */
    /*UINT        nIPShowMode;    /* �յ���ʾ��ʽ��1: ֻ�������еĹյ�
                                                2: ֻ������յ�
                                                3: ���ݹյ�ͼ���յ㶼�� */
    UINT        nZoomIn;        /* ͼ�ηŴ��� */
    UINT        nImgNum;        /* ��������ͼ��ʱ��һ��ͼ���ļ��а������ٸ�ͼ�� */
    UINT        nTest;          /* 0: ��ʵ���ڣ�1��ģ����ԣ�2����ȡdatŤ�����ݣ�3: ��ȡ����Ť�����ݣ�4������־�лָ����ݣ����ڵ�ǰ���ݿ�Ͷ��д���5: ��ȡdat ��ʷ���ݣ� */

    double      fDiscount;      /* fCut ���۱��� 0.8 */
    double      fMulti;         /* У׼������Χ0~2 */
    double      fRpmAdj;        /* ת�ٵ��� */
    //double      fIPDeltaVal;    /* Ĭ��0.7 */

    //BOOL        bCheckIP;       /* �����ж��Ƿ����ø߼��жϣ����ж��Կ���Ť��Ϊ׼�� �������Ť�غ͵�����СŤ��Ϊ���ϸ�Ĭ��Ϊ1 �Ϳؼ�����*/
    bool        bBigTorq;       /* �Ƿ�Ϊ��Ť�ذ汾2.4.9, TRUE:�͵�Ƭ��Ť�����10��,FALSE:1�� */
    bool        bDateBehind;    /* �������ļ������ĺ��� */

    string      strPassWord;
    string      strDataPath;

    string      strUnit;        /* ��ӦŤ�ص�λ���ַ��� */
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
    UINT        nParaNum;               /* ��ǰ��ʾ��������;ͼ�β�������������ʾ�������� */
    //UINT        nListNum;               /* ��ǰ�б���ʾ�������� */
    UINT        nMainNum;               /* ��ǰ��������ʾ�������� */
    UINT        nFileName;              /* ���������ļ��Ĳ������Ǹ��������е�һ�� */
    UINT        nStatType;              /* ͳ�Ʋ������Ǹ��������е�һ�� */
    UINT        nShow[MAXPARANUM];      /* strShow�ַ�����Ӧ�洢��index�� */
    //UINT        nList[MAXPARANUM];      /* strShow����ţ���0��ʼ��������Ŵ�strShow���ʾֵ */
    UINT        nMain[MAXMAINPARA];     /* strShow����ţ���0��ʼ��������Ŵ�strShow���ʾֵ */
    string      strShow[MAXPARANUM];    /* ��ǰѡ�����ʾ������ֵ +1 ����(0)����������ʱʹ�� */
    //string      strValue[MAXPARANUM];   /* ��ǰѡ�����ʾ������ֵ +1 ����(0)����������ʱʹ�� */
    //SHOWOPTION  tOption[MAXPARANUM];  /* ��ǰ��ʾ�����Ŀ�ѡ�� +1 ����(0)����������ʱʹ��  */

    UINT        nAlias;
    //bool        bFixTube;
    //TUBECFG     tTubeCfg;               /* 5���͹ܲ����Ĺ̻����壬�ǹ̻�������SHOWPARA�� */
}SHOWCFG;

typedef struct tagTorqCfgID
{
    UINT        nTorqueID;
    UINT        nTurnID;
    UINT        nTubeID;
    string      strOptionID;
}TORQCFGID;

/* XLSͳ�ƽ���������� */
typedef struct tagXLSSTATCFG
{
    //string      strName[MAXPARANUM];    /* ��ʾ�������� */
    ///BOOL        bSummary[MAXPARANUM];   /* ժҪ(Page1)��ʾ������� */
    //BOOL        bReport[MAXPARANUM];    /* ����(Page4)��ʾ������� */
    //BYTE        ucOperator;             /* ����೤(������)������� */
    //BYTE        ucTally;                /* �뾮��Ų������ */
    int         GenPara[STATPARA_GENNUM];
    int         JobPara[STATPARA_JOBNUM];       /* ժҪ(Page1)��ʾ������� */
    int         InfoPara[STATPARA_INFONUM];     /* ����(Page4)��ʾ������� */
}XLSSTATCFG;

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

#define     MAXWELLNUM   5000
typedef struct tagTORQUEDATA
{
    UINT    nCur;               /* nCur��1��ʼ�������൱���������+1 */
    UINT    nTotal;
    UINT    nQualy;
    UINT    nUnQualy;
    UINT    nTotalPlus[MAXWELLNUM];
    TorqData::Torque  tData[MAXWELLNUM];
    string  strFileName;
}TORQUEDATA;

/* ���10�������ǷŴ�5����10*500*5 */
#define SPLITPOSNUM     20          // �Ͽ�Ť�غ�ж��Ť������֮��ļ������
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

/* ��ʾ���������ƶ��壬�ڴ���д����ȷ����ʾ������/˳�� */
#define         MAXNAMELEN              32
typedef struct tagSHOWPARANAME
{
    // WORD        nNameID;
    string      strName[LANGUAGE_NUM];
}SHOWPARANAME;


#define MAXLINEITEM     500
#define COLLECTPOINTS   (MAXLINEITEM + 50) // �Զ���������
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

/* ���ݱ�׼����С�������λ */
#define PERCENTDATA(fData)    (int(fData*100)/100.0)

#define MIN(a, b)            ((a) > (b) ? (b) : (a))
#define MAX(a, b)            ((a) > (b) ? (a) : (b))

/* 3.22�����д������ݶ���¼,���ݰ���������Ϣ���������������� */
// 20221231 ������plussize51,torquesize540
//#define VERSION_RECPLUS(ptTorq)  (ptTorq->dwdelplus_size() == ptTorq->ftorque_size())

/* �Զ������꣬���ʹ���Ȧ���Ӷ� */
/* ���򹫹���ֵ */
/* ָ��Ϊ�գ�ֱ�ӷ��� */
#define ASSERT_NULL(point)              {                   \
        if((point) == NULL)                                 \
            return;                                         \
    }

/* ָ��Ϊ�գ�����ret(BOOL, value, NULL) */
#define ASSERT_NULL_R(point, ret)       {                   \
        if((point) == NULL)                                 \
            return (ret);                                   \
    }

/* ����Ϊ0��ֱ�ӷ��� */
#define ASSERT_ZERO(value)              {                   \
        if((value) == 0)                                    \
            return;                                         \
    }

/* ����Ϊ0������ret(BOOL, value, NULL)  */
#define ASSERT_ZERO_R(value, ret)       {                   \
        if((value) == 0)                                    \
            return (ret);                                   \
    }

/* ����ΪTRUE��ֱ�ӷ��� */
#define COMP_BTRUE(value)               {                   \
        if((value))                                         \
            return;                                         \
    }

/* ����ΪFALSE��ֱ�ӷ��� */
#define COMP_BFALSE(value)              {                   \
        if(!(value))                                        \
            return;                                         \
    }

/* ����ΪTRUE������ ret */
#define COMP_BTRUE_R(value, ret)        {                   \
        if((value))                                         \
            return (ret);                                   \
    }

/* ����ΪFALSE������ ret */
#define COMP_BFALSE_R(value, ret)        {                  \
        if(!(value))                                        \
            return (ret);                                   \
    }

/* ��������cp��ֱ�ӷ��� */
#define COMP_BE(value, cp)              {                   \
        if((value) == (cp))                                 \
            return;                                         \
    }

/* ��������cp������ret(BOOL, value, NULL)  */
#define COMP_BE_R(value, cp, ret)       {                   \
        if((value) == (cp))                                 \
            return (ret);                                   \
    }

/* ����������cp��ֱ�ӷ��� */
#define COMP_BNE(value, cp)             {                   \
        if((value) != (cp))                                 \
            return;                                         \
    }

/* ����������cp������ret(BOOL, value, NULL)  */
#define COMP_BNE_R(value, cp, ret)      {                   \
        if((value) != (cp))                                 \
            return (ret);                                   \
    }

/* ����С��cp��ֱ�ӷ��� */
#define COMP_BL(value, cp)              {                   \
        if((value) < (cp))                                  \
            return;                                         \
    }

/* ����С��cp������ret(BOOL, value, NULL)  */
#define COMP_BL_R(value, cp, ret)       {                   \
        if((value) < (cp))                                  \
            return (ret);                                   \
    }

/* ��������cp��ֱ�ӷ��� */
#define COMP_BG(value, cp)              {                   \
        if((value) > (cp))                                  \
            return;                                         \
    }

/* ��������cp������ret(BOOL, value, NULL)  */
#define COMP_BG_R(value, cp, ret)       {                   \
        if((value) > (cp))                                  \
            return (ret);                                   \
    }

/* ����С�ڵ���cp��ֱ�ӷ��� */
#define COMP_BLE(value, cp)             {                   \
        if((value) <= (cp))                                 \
            return;                                         \
    }

/* ����С�ڵ���cp������ret(BOOL, value, NULL)  */
#define COMP_BLE_R(value, cp, ret)      {                   \
        if((value) <= (cp))                                 \
            return (ret);                                   \
    }

/* �������ڵ���cp��ֱ�ӷ��� */
#define COMP_BGE(value, cp)             {                   \
        if((value) >= (cp))                                 \
            return;                                         \
    }

/* �������ڵ���cp������ret(BOOL, value, NULL)  */
#define COMP_BGE_R(value, cp, ret)      {                   \
        if((value) >= (cp))                                 \
            return (ret);                                   \
    }


/* ����ΪNULL��ѭ������continue */
#define COMP_BNULL_CONTINUE(value)      {                   \
        if((value) == NULL)                                 \
            continue;                                       \
    }

/* ����ΪTRUE��ѭ������continue */
#define COMP_BTRUE_CONTINUE(value)      {                   \
        if((value))                                         \
            continue;                                       \
    }

/* ����ΪFALSE��ѭ������continue */
#define COMP_BFALSE_CONTINUE(value)     {                   \
        if(!(value))                                        \
            continue;                                       \
    }

/* ɾ���ǿ�ָ�� */
#define DELETE_POINT(point)             {                   \
        if((point) != NULL)                                 \
        {                                                   \
            delete (point);                                 \
            (point) = NULL;                                 \
        }                                                   \
    }

/* ɾ����Ч��object */
#define DELETE_OBJECT(hObject)          {                   \
        if(hObject)                                         \
            ::DeleteObject(hObject);                        \
    }

/* �������޼�飬�������ֵ����ĳֵ������ֵ�޸�Ϊĳֵ */
#define CHECK_VALUE_UP(check, upper)    {                   \
        if(check > upper)                                   \
        {                                                   \
            (check) = (upper);                              \
        }                                                   \
    }

/* �������޼�飬�������ֵС��ĳֵ������ֵ�޸�Ϊĳֵ*/
#define CHECK_VALUE_LOW(check, lower)   {                   \
        if(check < lower)                                   \
        {                                                   \
            (check) = (lower);                              \
        }                                                   \
    }


/* �������к�ֵ */
/* �ж�Ť�ؼ�¼������Ƿ���Ч */
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

/* �ж�Ť�ؼ�¼������Ƿ���Ч����Ч��ӡ��Ϣ */
#define JUDGE_RANGE_MESS(nNo, iMaxNo, strInfo)  {           \
        if(((nNo) < 1                      )  ||            \
           ((nNo) > (iMaxNo)               )  ||            \
           ((nNo) > MAXWELLNUM             ))               \
        {                                                   \
            AfxMessageBox(strInfo);                         \
            return;                                         \
        }                                                   \
    }

/* ��ȡŤ�ؽṹ��ʷ�ļ�Խ�籣�� */
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
/* �ӵ�һ����¼��ʼ������ָ���ļ�¼λ��
   ˵��:�ļ��ĵ�һ��UINT��¼�����Ѿ�����
   forѭ���ĵ���Ϊint i
   nLeng ΪUNIT����,��ʾ�������������� */
#define SEEK_TORQUE(index, nLeng)      {                                    \
        for(i=0; i<(index); i++)                                            \
        {                                                                   \
            file.Read(&(nLeng), sizeof(UINT));                              \
            file.Seek((nLeng), CFile::current);                             \
        }                                                                   \
    }
#endif
   /* ��ȡͼ��Ŀ���Ť�� */
#define GET_CTRL_TORQ(fTorq, ptTorq)        {                       \
        fTorq = ptTorq->fmumaxtorq();                               \
        if(fTorq < ptTorq->fbomaxtorq())                            \
            fTorq = ptTorq->fbomaxtorq();                           \
    }

/* �жϳ���ע��״̬ */
#define JUDGE_REG_STATUS()              {                       \
        if(!theApp.m_tdbReg.Reged())                            \
        {                                                       \
            AfxMessageBox(IDS_STRINFNOREG, MB_ICONINFORMATION); \
            return;                                             \
        }                                                       \
    }

/* ���ÿؼ�������ΪSTRINGTABELָ�����ַ��� */
#define SET_CONTROL_NAME(CtrlID, strTabID, strTag)                          \
    {                                                                       \
        strTag.LoadString(strTabID);                                        \
        GetDlgItem(CtrlID)->SetWindowText(strTag);                          \
    }

/* ��������λ���ԣ�������������Ϊ1��������0 */
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

/* ��ʾ������Ϣ�����nCur=nAll����ʾ������������ʾ��ǰ����/������ */
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


/* �������޼�飬�������ֵ����ĳֵ������ֵ�޸�Ϊ��ĳֵС*** */
#define CHECK_PARA_UP(check, upper,diff) {                      \
        if(check > upper)                                       \
        {                                                       \
            (check) = (upper - diff);                           \
        }                                                       \
    }

/* �������޼�飬�������ֵС��ĳֵ������ֵ�޸�Ϊ��ĳֵ��*** */
#define CHECK_PARA_LOW(check, lower, diff) {                    \
        if(check < lower)                                       \
        {                                                       \
            (check) = (lower + diff);                           \
        }                                                       \
    }

/* ������Χ��飬�������ֵ���ڷ�Χ�ڣ�����ֵ����ΪĬ��ֵ */
#define CHECK_PARA_ROUND(check, lower, upper, defval) {         \
        if(check < lower || check > upper)                      \
        {                                                       \
            (check) = (defval);                                 \
        }                                                       \
    }

/* ������Χ��飬�������ֵ���������ڣ�����ֵ����ΪĬ��ֵ */
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

string UTF82ASCII(string& strUtf8Code);         //utf-8 ת ascii 
string ASCII2UTF8(string& strAsciiCode);        //ascii ת Utf8
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

