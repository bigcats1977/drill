// TorqueDlg.cpp : implementation file
//
#include "stdafx.h"
#include "Torque.h"
#include "TorqueDlg.h"
#include "DlgParaSet.h"
#include "DlgFuncTest.h"
#include "DlgZero.h"
#include "DlgScm.h"
#include "DlgCollect.h"
#include "DlgHisList.h"
#include "DlgHisGrp.h"
#include "DlgHisSumup.h"
#include "DlgDataStat.h"
//#include "DlgBear.h"
#include "DlgShowSet.h"
#include "DlgModPW.h"
#include "DlgValveSet.h"
#include "DlgMainShowSet.h"
#include "DlgRemark.h"
#include "DlgSegCabl.h"
//#include "DlgApi.h"
#include <Nb30.h>
#include <ctime>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern string       g_strPropName;
extern HANDLE       g_hValue;

//CNewComThread       *g_lpNewComThread = NULL;     /* ���ڲ����߳� */
BYTE                g_ucCurValve = 0;


CDlgCollect         *m_pdlgCollect  = NULL;
CDlgSegCabl         *m_pdlgCalib    = NULL;

//#define             TESTPORT
#define             MAXREVERSEPLUS          300     /* 20201219 ���ת�����ݶ�300 */

/*********************ȫ�ֱ���************************************/
int g_iModBusHi[] = {
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};
/* CRC��λ�ֽ�ֵ��*/
int g_iModBusLow[] = {
        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
        0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
        0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
        0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
        0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
        0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
        0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
        0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
        0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
        0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
        0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
        0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
        0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
        0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
        0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
        0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
        0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
        0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
        0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
        0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
        0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
        0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};


/* �������� */
#define     MAXCMDNUM       24
CString g_strCmdName[MAXCMDNUM] = {
            "SCMREAD   ",
            "SCMADJUST ",
            "SCMQIPI   ",
            "SCMCOLLECT",
            "SCMSHOW   ",
            "SCMSPEED  ",
            "SCMCONTROL",
            "SCMBSPEED ",
            "SCMESPEED ",
            "SCMBUNLOAD",
            "SCMEUNLOAD",
            "SCMUPPER  ",
            "SCMLOWER  ",
            "SCMCUT    ",
            "SCMCOLLOK ",
            "SCMCOLLNOK",
            "SCMTIME   ",
            "SCMVPRESS ",
            "SCMVFLW   ",
            "SCMREADVALVE",
            "SCMREADMULTI",
            "SCMWRITECALIB",
            "SCMCTRLCALIB",
            "SCMREADCALIB"
//            "SCMVZOOM  "
};

/* �����������飬��ż�15�ٴ���
   ���8��BYTE */
void (CTorqueDlg::*g_FillByteProc[MAXCMDNUM])(void) =
{
    /*  0 */    &CTorqueDlg::FillRead,           /* ��ȡ���� */
    /*  1 */    &CTorqueDlg::FillMulti,          /* У׼���� */
    /*  2 */    &CTorqueDlg::FillQipi,           /* ȥƤ */
    /*  3 */    &CTorqueDlg::FillCollect,        /* �ɼ����� */
    /*  4 */    &CTorqueDlg::FillShow,           /* ��ʾŤ�� */
    /*  5 */    &CTorqueDlg::FillSpeed,          /* ����Ť�� */
    /*  6 */    &CTorqueDlg::FillControl,        /* ����Ť�� */
    /*  7 */    &CTorqueDlg::FillBSpeed,         /* ��ʼ���� */
    /*  8 */    &CTorqueDlg::FillESpeed,         /* ֹͣ���� */
    /*  9 */    &CTorqueDlg::FillBUnload,        /* ��ʼж�� */
    /* 10 */    &CTorqueDlg::FillEUnload,        /* ֹͣж�� */
    /* 11 */    &CTorqueDlg::FillUpper,          /* ����Ť�� */
    /* 12 */    &CTorqueDlg::FillLower,          /* ����Ť�� */
    /* 13 */    &CTorqueDlg::FillCut,            /* ����ϵ�� */
    /* 14 */    &CTorqueDlg::FillCollectOK,      /* �յ���Ч�����ٴβɼ����ݣ����һ��ȷ�Ϻ���Ҫ�ٴ��յ����� */
    /* 15 */    &CTorqueDlg::FillCollectNOK,     /* �յ���Ч�����ٴβɼ����� */
    /* 16 */    &CTorqueDlg::FillTime,           /* ���õ�Ƭ��ʱ�� */
    /* 17 */    &CTorqueDlg::FillValvePress,     /* ���ü�ѹ������ */
    /* 18 */    &CTorqueDlg::FillValveFlow,      /* �������������� */
    /* 19 */    &CTorqueDlg::FillReadValve,      /* ��ȡ����״̬���� */
    /* 20 */    &CTorqueDlg::FillReadMulti,      /* ��ȡ�������� */
    /* 21 */    &CTorqueDlg::FillWriteCalib,     /* д �ֶ�У׼ */
    /* 22 */    &CTorqueDlg::FillCtrlCalib,      /* ���Ʒֶ�У׼ */
    /* 23 */    &CTorqueDlg::FillReadCalib,      /* �� �ֶ�У׼ */
    /* 19       &CTorqueDlg::FillValveZoom         ���÷��Ŵ������� */
};

/* �ռ����ݺ������� */
BOOL (CTorqueDlg::*g_CollectProc[4])(COLLECTDATA* ptCollData) =
{
        /*  0 */    &CTorqueDlg::CollectPort,             /* �ռ��������� */
        /*  1 */    &CTorqueDlg::CollectRandData,         /* ����������� */
        /*  2 */    &CTorqueDlg::CollectTorqData,         /* �ֳ��Զ���¼Ť������ */
        /*  3 */    &CTorqueDlg::CollectMultiTorq         /* �ֳ��Զ���¼����Ť������ */
};

extern UINT   g_nValidLen[];


/*********************�����************************************/
#define INSERT_DATA(Torque,Rpm,bLast)   {               \
        InsertData(&m_tCollData, Torque, Rpm);          \
        m_wndTorque.SetPos(Torque,bLast);               \
        m_wndTorque.Go();                               \
        m_wndRpm.SetPos(Rpm,bLast);                     \
        m_wndRpm.Go();                                  \
    }

#define JUDGE_RUN_STATUS(wInfo)         {               \
        if(m_bRunStatus)                                \
        {                                               \
            AfxMessageBox(wInfo);                       \
            return;                                     \
        }                                               \
    }

/* ������ʾ�������ذ�ť */
#define SHOW_MAINDLG_BUTTON(bShow)      {               \
        GetDlgItem(IDC_EDITCRC)->ShowWindow(bShow);     \
        GetDlgItem(IDC_STATICCRC)->ShowWindow(bShow);   \
        GetDlgItem(IDC_DATASHOW)->ShowWindow(bShow);    \
        GetDlgItem(IDC_EDITCLASH)->ShowWindow(bShow);   \
        GetDlgItem(IDC_STATICCLASH)->ShowWindow(bShow); \
        GetDlgItem(IDC_EDITTOTAL)->ShowWindow(bShow);   \
        GetDlgItem(IDC_STATICTOTAL)->ShowWindow(bShow); \
    }
#if 0
#define CLOSE_PORTCOMM()                {               \
        if(g_lpNewComThread != NULL)                    \
        {                                               \
            g_lpNewComThread->CloseComm();              \
            delete g_lpNewComThread;                    \
            g_lpNewComThread = NULL;                    \
            theApp.SavePortOper(PORTOPR_CLOSE);         \
        }                                               \
    }
#else
#define CLOSE_PORTCOMM()                {               \
            m_tPort.ClosePort();                        \
            theApp.SavePortOper(PORTOPR_CLOSE);         \
    }
#endif

/* ɾ���Ի���ָ�� */
#define DELETE_DIALOG(pdlg)         {           \
        if((pdlg) != NULL)                      \
        {                                       \
            (pdlg)->DestroyWindow();            \
            delete (pdlg);                      \
            (pdlg) = NULL;                      \
        }                                       \
    }

/* MODBUS����������ʼ��ַ */
#define MODBUS_CONTENT          m_ucSndByte[4]

/* ���ͨ���ж�ʱ��5s */
#define MAXCOMMBREAKTIME        5000

static UINT BASED_CODE indicators[] =
{
    /*ID_SEPARATOR,         // status line indicator*/
    ID_PORTINFO,            //������Ϣ��ʾ
};

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
    typedef struct
    {
        ADAPTER_STATUS  adapt;
        NAME_BUFFER     NameBuff [30];
    } ASTAT;

public:
    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    CButtonST   m_btnReg;
    CButtonST   m_btnOK;
    CString     m_strReg[REGCODESEGNUM];
    CString     m_strName;
    CString     m_strMachine;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnRegedit();
    afx_msg void OnChangeReg1();
    afx_msg void OnChangeReg2();
    afx_msg void OnChangeReg3();
    afx_msg void OnChangeReg4();
    afx_msg void OnChangeReg5();
    afx_msg void OnChangeReg6();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    int     SplitRegCode(CString strSource, CString strDest[], int iBegin);
    void    AutoFillRegCode(UINT nEditNo);
    void    SaveRegFile();
    
    DBREG   *m_ptdbReg;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    int     i = 0;

    //{{AFX_DATA_INIT(CAboutDlg)
    m_strName    = _T("");
    m_strMachine = _T("");
    //}}AFX_DATA_INIT

    for(i=0; i<REGCODESEGNUM; i++)
        m_strReg[i] = _T("");
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    DDX_Control(pDX, IDC_REGEDIT, m_btnReg);
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Text(pDX, IDC_REG1, m_strReg[0]);
    DDX_Text(pDX, IDC_REG2, m_strReg[1]);
    DDX_Text(pDX, IDC_REG3, m_strReg[2]);
    DDX_Text(pDX, IDC_REG4, m_strReg[3]);
    DDX_Text(pDX, IDC_REG5, m_strReg[4]);
    DDX_Text(pDX, IDC_REG6, m_strReg[5]);
    DDX_Text(pDX, IDC_REGNAME, m_strName);
    DDX_Text(pDX, IDC_MACHINE, m_strMachine);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    ON_BN_CLICKED(IDC_REGEDIT, OnRegedit)
    ON_EN_CHANGE(IDC_REG1, OnChangeReg1)
    ON_EN_CHANGE(IDC_REG2, OnChangeReg2)
    ON_EN_CHANGE(IDC_REG3, OnChangeReg3)
    ON_EN_CHANGE(IDC_REG4, OnChangeReg4)
    ON_EN_CHANGE(IDC_REG5, OnChangeReg5)
    ON_EN_CHANGE(IDC_REG6, OnChangeReg6)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
    DWORD   adwPCVol[2];
    DWORD   adwPCMac[2];
    DWORD   adwRegVol[2];
    DWORD   adwRegMac[2];
    CString strRegCode;
    CTime   tNowDate  = CTime::GetCurrentTime();
    int     iYear,iMonth, iDay;

    CDialog::OnInitDialog();

    m_btnOK.SetIconAndText(IDI_APPLY, IDS_STROK);
    m_btnReg.SetIconAndText(IDI_LOCKUP, IDS_STRREGISTER);

    m_ptdbReg  = &theApp.m_tdbReg;
    m_strName  = m_ptdbReg->strName;
    strRegCode = m_ptdbReg->strRegCode;

    if((!m_ptdbReg->bReged) || (strRegCode.GetLength() < MAXREGCODE))
    {
        iYear  = tNowDate.GetYear();
        iMonth = tNowDate.GetMonth();
        iDay   = tNowDate.GetDay();
    }
    else
    {
        /* Reged */
        theApp.StringSubtract(m_strName,  REGCODEVALUE);
        theApp.StringSubtract(strRegCode, REGCODEVALUE);

        theApp.SplitRegString(m_strReg, strRegCode);

        m_btnReg.ShowWindow(FALSE);
        
        theApp.GetVolMacFromRegStr(m_strReg, adwRegVol, adwRegMac, iYear, iMonth, iDay);
    }

    /* ��ȡ�����ϵľ��MAC��Ϣ */
    theApp.GetVolMacInfo(adwPCVol, adwPCMac, iYear, iMonth, iDay);

    adwPCMac[0] ^= MAC0XOR;
    adwPCMac[1] ^= MAC1XOR;
    adwPCMac[0] &= MAC0AND;
    adwPCMac[1] &= MAC1AND;
    adwPCVol[0] ^= VOL0XOR;
    adwPCVol[1] ^= VOL1XOR;

    m_strMachine.Format("%08X%04X%08X%06X", adwPCVol[0], adwPCMac[1], adwPCVol[1], adwPCMac[0]);

    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::SaveRegFile()
{
    int             i   = 0;
    HANDLE          hDir;
    CFile           newfile;
    CString         strRegCode;

    hDir=CreateFile (theApp.m_strRegFile.c_str(),//folder
                     GENERIC_READ|GENERIC_WRITE,//readwrite
                     0,//share
                     NULL,//security attribute
                     CREATE_ALWAYS,//����
                     FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_HIDDEN,//dwFlagsAndAttributes,
                     NULL);//temphandle
    if(hDir==INVALID_HANDLE_VALUE)
    {
        AfxMessageBox(IDS_STRINFSAVEREGFAIL);
        CloseHandle(hDir);
        return;
    }
    CloseHandle(hDir);

    m_ptdbReg->bReged = 1;
    memset(m_ptdbReg->strName, 0 ,MAXNAME);
    memset(m_ptdbReg->strRegCode, 0 ,MAXREGCODE);
    strRegCode.Empty();
    memcpy(m_ptdbReg->strName, m_strName, m_strName.GetLength());
    for(i=0; i<m_strName.GetLength(); i++)
    {
        m_ptdbReg->strName[i] += REGCODEVALUE;
    }

    theApp.MergeRegString(m_strReg, strRegCode);
    memcpy(m_ptdbReg->strRegCode, strRegCode, strRegCode.GetLength());
    for(i=0; i<strRegCode.GetLength(); i++)
    {
        m_ptdbReg->strRegCode[i] += REGCODEVALUE;
    }

    newfile.Open(theApp.m_strRegFile.c_str(),CFile::modeWrite,NULL);
    newfile.Write(m_ptdbReg,sizeof(DBREG));
    newfile.Write(&theApp.m_dwTotalTorqNum,sizeof(DWORD));
    newfile.Close();
}

void CAboutDlg::OnRegedit()
{
    int     i = 0;
    DWORD   adwRegVol[2];
    DWORD   adwRegMac[2];
    DWORD   adwPCVol[2];
    DWORD   adwPCMac[2];
    UINT    nRegLen[REGCODESEGNUM];
    int     iYear,iMonth, iDay;

    UpdateData();

    for(i=0; i<REGCODESEGNUM; i++)
    {
        nRegLen[i] = m_strReg[i].GetLength();
    }

    if(memcmp(nRegLen, g_nValidLen, REGCODESEGNUM*sizeof(UINT)) !=0)
    {
        AfxMessageBox(IDS_STRINFREGCODEERR);
        return;
    }

    /* ��ȡע�����еľ��MAC��Ϣ */
    theApp.GetVolMacFromRegStr(m_strReg, adwRegVol, adwRegMac, iYear, iMonth, iDay);
    
    /* ��ȡ�����ϵľ��MAC��Ϣ */
    theApp.GetVolMacInfo(adwPCVol, adwPCMac, iYear, iMonth, iDay);

    if( memcmp(adwRegVol, adwPCVol, 2*sizeof(DWORD)) == 0 &&
        memcmp(adwRegMac, adwPCMac, 2*sizeof(DWORD)) == 0)
    {
        SaveRegFile();

        theApp.ShowMainTitle();
        AfxMessageBox(IDS_STRINFREGSUCC);
        OnOK();
        return;
    }

    /* ע��ʧ�� */
    theApp.ShowMainTitle();
    AfxMessageBox(IDS_STRINFREGFAIL);
}

/* ����һ����'-' �ָ��ע����룬�Զ��ӵ�ǰ����򽫺����������ַ�������������õ��������
   nEditNo��0��ʼ��ţ���ӦIDC_REG1... */
void CAboutDlg::AutoFillRegCode(UINT nEditNo)
{
    int     iPlace  = 0;
    int     i       = 0;
    CString strCode[REGCODESEGNUM];

    COMP_BGE(nEditNo, REGCODESEGNUM);

    UpdateData(TRUE);

    iPlace = SplitRegCode(m_strReg[nEditNo], strCode, nEditNo);
    COMP_BL(iPlace, 0);

    for(i=iPlace; i>= (int)nEditNo; i--)
    {
        m_strReg[i] = strCode[i];
    }

    UpdateData(FALSE);
}

int CAboutDlg::SplitRegCode(CString strSource, CString strDest[], int iBegin)
{
    CString strTemp;
    int     iPlace  = 0;
    int     iIndex  = 0;

    COMP_BGE_R(iBegin, REGCODESEGNUM, -1);

    iIndex = iBegin;

    strTemp = strSource;
    iPlace = strTemp.Find('-');
    while(iPlace != -1)
    {
        strDest[iIndex] = strTemp.Left(iPlace);
        strTemp.Delete(0,iPlace+1);
        iPlace = strTemp.Find('-');
        iIndex++;
        if(iIndex > REGCODESEGNUM)
            break;
    }
    if(!strTemp.IsEmpty())
        strDest[iIndex] = strTemp;

    return iIndex;
}

void CAboutDlg::OnChangeReg1()
{
    AutoFillRegCode(0);
}

void CAboutDlg::OnChangeReg2()
{
    AutoFillRegCode(1);
}

void CAboutDlg::OnChangeReg3()
{
    AutoFillRegCode(2);
}

void CAboutDlg::OnChangeReg4()
{
    AutoFillRegCode(3);
}

void CAboutDlg::OnChangeReg5()
{
    AutoFillRegCode(4);
}

void CAboutDlg::OnChangeReg6()
{
    UpdateData(TRUE);

    m_strReg[5] = m_strReg[5].Left(REGCODELEN6);

    UpdateData(FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// CTorqueDlg dialog

CTorqueDlg::CTorqueDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CTorqueDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CTorqueDlg)
    m_strTorque     = _T("0");
    m_fRpm          = 0.0f;
    m_nCur          = 1;
    m_nTotal        = 1;
    m_nCRCERR       = 0;
    m_nClashERR     = 0;
    m_nInterval     = 20;//ʱ����Ҫ�޸�20191208
    //m_strCircle     = _T("0");
    m_strTubeNO2    = _T("");
    m_iShackle      = 0;
    m_strRecvData   = _T("");
    m_strQuality    = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_bComm         = FALSE;
    m_bAutoFileOpen = FALSE;

    m_pdlgCollect   = NULL;
    m_pdlgCalib     = NULL;
    m_ptPortData    = NULL;

    /* δ�洢��Data��ֵ����ʼΪ0 */
    UpdateOutData(0, 0); 
}

void CTorqueDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTorqueDlg)
    DDX_Control(pDX, IDC_CHECKCOMM, m_cbComm);
    DDX_Control(pDX, IDC_CHECKALARM, m_cbAlarm);
    DDX_Control(pDX, IDC_CHECKSPEED, m_cbValveStatus[0]);
    DDX_Control(pDX, IDC_CHECKUNLOAD, m_cbValveStatus[1]);
    DDX_Control(pDX, IDC_CHECKPRESS, m_cbValveStatus[2]);
    DDX_Control(pDX, IDC_CHECKFLOW, m_cbValveStatus[3]);
    DDX_Control(pDX, IDC_CHECKMAKEUP, m_cbValveStatus[4]);
    DDX_Control(pDX, IDC_CHECKBREAKOUT, m_cbValveStatus[5]);
    DDX_Control(pDX, IDC_COMMSTATUS, m_CommStatus);
    DDX_Control(pDX, IDC_BTRUN, m_btnRun);
    DDX_Control(pDX, IDC_XAXIS1, m_xAxis1);
    DDX_Control(pDX, IDC_YAXIS1, m_yAxis1);
    DDX_Control(pDX, IDC_XAXIS2, m_xAxis2);
    DDX_Control(pDX, IDC_YAXIS2, m_yAxis2);
    //DDX_Text(pDX, IDC_EDITCIRCLE, m_strCircle);
    DDX_Text(pDX, IDC_EDITTORQUE, m_strTorque);
    DDX_Check(pDX, IDC_CHECKCOMM, m_bComm);
    DDX_Text(pDX, IDC_EDITCURNUM, m_nCur);
    DDX_Text(pDX, IDC_EDITCRC, m_nCRCERR);
    DDX_Text(pDX, IDC_DATASHOW, m_strRecvData);
    DDX_Text(pDX, IDC_EDITCLASH, m_nClashERR);
    DDX_Text(pDX, IDC_EDITTOTAL, m_nTotal);
    DDX_Check(pDX, IDC_SETIPPOINT, theApp.m_bCheckIP);
    DDX_Check(pDX, IDC_SETTOOLBUCK, m_bToolBuck);
    DDX_Text(pDX, IDC_QUALITY, m_strQuality);
    DDX_Radio(pDX, IDC_RADIOBUCKLE, m_iShackle);
    DDX_Text(pDX, IDC_MAINSHOW0, m_strMainName[0]);
    DDX_Text(pDX, IDC_MAINSHOW1, m_strMainName[1]);
    DDX_Text(pDX, IDC_MAINSHOW2, m_strMainName[2]);
    DDX_Text(pDX, IDC_MAINSHOW3, m_strMainName[3]);
    DDX_Text(pDX, IDC_MAINSHOW4, m_strMainName[4]);
    DDX_Text(pDX, IDC_MAINSHOW5, m_strMainName[5]);
    DDX_Text(pDX, IDC_MAINSHOW6, m_strMainName[6]);
    DDX_Text(pDX, IDC_EDMAINSHOW0, m_strMainValue[0]);
    DDX_Text(pDX, IDC_EDMAINSHOW1, m_strMainValue[1]);
    DDX_Text(pDX, IDC_EDMAINSHOW2, m_strMainValue[2]);
    DDX_Text(pDX, IDC_EDMAINSHOW3, m_strMainValue[3]);
    DDX_Text(pDX, IDC_EDMAINSHOW4, m_strMainValue[4]);
    DDX_Text(pDX, IDC_EDMAINSHOW5, m_strMainValue[5]);
    DDX_Text(pDX, IDC_EDMAINSHOW6, m_strMainValue[6]);
    DDX_Text(pDX, IDC_EDMAINSHOW42, m_strTubeNO2);
    DDX_Text(pDX, IDC_STATIC_M2, m_strLBM2);
    DDX_Text(pDX, IDC_STATIC_M9, m_strLBM9);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTorqueDlg, CDialog)
    //{{AFX_MSG_MAP(CTorqueDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_COMMAND(ID_SETPARA, OnSetpara)
    ON_COMMAND(ID_SETSHOW, OnSetShow)
    ON_COMMAND(ID_ABOUT, OnAbout)
    ON_COMMAND(ID_DEBUG, OnDebug)
    ON_BN_CLICKED(ID_APP_EXIT, OnAppExit)
    ON_COMMAND(ID_ZERO, OnZero)
    ON_COMMAND(ID_SCM, OnScm)
    ON_COMMAND(ID_HISTORYLIST, OnHistorylist)
    ON_COMMAND(ID_HISTORYSTAT, OnHistorystat)
    ON_COMMAND(ID_SHOWTEST, OnShowtest)
    ON_COMMAND(ID_COLLECTDATA, OnCollectdata)
    ON_BN_CLICKED(IDC_BTRUN, OnBtrun)
    ON_MESSAGE(WM_COLLECTTIMEROUT,CollectTimerOut)
    ON_MESSAGE(WM_GUARDTIMEROUT,GuardTimerOut)
    //ON_MESSAGE(WM_GUARDREADTIMEROUT,ResetTimerOut)
    ON_MESSAGE(WM_PORTBUFFTIMEROUT,PortBuffTimerOut)
    ON_MESSAGE(WM_SAVEDATATIMEROUT,SaveDataTimerOut)
    ON_MESSAGE(WM_SAVEDEBUGTIMEROUT,SaveDebugTimerOut)
    ON_MESSAGE(WM_ALARMPLAYTIMEROUT,ALarmPlayTimerOut)
    ON_MESSAGE(WM_READVALVETIMEROUT,ReadValveTimerOut)
    ON_MESSAGE(WM_COMM_RXCHAR, OnCommunication)
    ON_COMMAND(ID_LANG_CHN, OnLangChn)
    ON_UPDATE_COMMAND_UI(ID_LANG_CHN, OnUpdateLangChn)
    ON_COMMAND(ID_LANG_ENG, OnLangEng)
    ON_UPDATE_COMMAND_UI(ID_LANG_ENG, OnUpdateLangEng)
    /*ON_COMMAND(ID_LANG_RUS, OnLangRus)
    ON_UPDATE_COMMAND_UI(ID_LANG_RUS, OnUpdateLangRus)*/
    ON_COMMAND(ID_VER_BIGTORQ, OnVerBigTorq)
    ON_UPDATE_COMMAND_UI(ID_VER_BIGTORQ, OnUpdateVerBigTorq)
    ON_COMMAND(ID_VER_UNIT_NM, OnVerUnitNm)
    ON_UPDATE_COMMAND_UI(ID_VER_UNIT_NM, OnUpdateVerUnitNm)
    ON_COMMAND(ID_VER_UNIT_LBFT, OnVerUnitLbft)
    ON_UPDATE_COMMAND_UI(ID_VER_UNIT_LBFT, OnUpdateVerUnitLbft)
    //}}AFX_MSG_MAP
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_INITMENUPOPUP()
    ON_COMMAND(ID_MODPW, &CTorqueDlg::OnModpw)
    ON_COMMAND(ID_VALVESET, &CTorqueDlg::OnValveset)
    ON_BN_CLICKED(IDC_SETIPPOINT, &CTorqueDlg::OnBnClickedSetippoint)
    ON_BN_CLICKED(IDC_SETTOOLBUCK, &CTorqueDlg::OnBnClickedSettoolbuck)
    ON_BN_CLICKED(IDC_RADIOBUCKLE, &CTorqueDlg::OnBnClickedRadiobuckle)
    ON_BN_CLICKED(IDC_RADIOSHACKLE, &CTorqueDlg::OnBnClickedRadioshackle)
    ON_BN_CLICKED(IDC_BTNQUALITY, &CTorqueDlg::OnBnClickedBtnquality)
    ON_BN_CLICKED(IDC_BTNSHOWSET, &CTorqueDlg::OnBnClickedBtnshowset)
    ON_COMMAND(ID_SEGCALIB, &CTorqueDlg::OnSegcalib)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTorqueDlg message handlers

void CTorqueDlg::InitVariant()
{
    /* ��ʼ��theAppȫ�ֱ�����ָ�룬�������� */
    m_ptShowCfg = theApp.m_ptCurShow;
    m_ptCtrl    = &theApp.m_tParaCfg.tCtrl;
    m_ptComm    = &theApp.m_tParaCfg.tComm;
    
    /* ��ʼ������ */
    m_nCur      = theApp.m_nCurNO + 1;
    m_nTotal    = theApp.m_dwTotalTorqNum;
    m_iTest     = 0;
    m_iRecPNum  = 0;
    m_bRunStatus= FALSE;
    m_bAlarm    = FALSE;
    m_bCanModLastData   = FALSE;
    m_strMainValue[MAINSHOWWELL].Format("%d", theApp.m_nCurRunningNO);

    m_ptPortData = (PORTDATA*)(new BYTE[sizeof(PORTDATA)]);
    memset(m_ptPortData, 0, sizeof(PORTDATA));

    ResetData();
}

void CTorqueDlg::InitMainShowPara()
{
    WORD    i = 0;
    WORD    j = 0;
    CString  strTemp;

    for(i=0; i<m_ptShowCfg->wMainNum && i<MAXMAINPARA; i++)
    {
        m_strMainName[i] = m_ptShowCfg->strMain[i].c_str();

        if(i<4) /* �ܲ���ʾ���� */
        {
            continue;
        }

        if(i == MAINSHOWWELL)  // ��5��Ϊ�뾮��ţ�����Ҫʹ��combobox
            continue;

        for(j=0; j<m_ptShowCfg->wParaNum && j<MAXPARANUM; j++)
        {
            if(0 == m_strMainName[i].CompareNoCase(m_ptShowCfg->tShow[j].strName.c_str()))
            {
                strTemp = m_ptShowCfg->tShow[j].strValue.c_str();
                if(strTemp.IsEmpty() && m_ptShowCfg->tOption[i].wOptNum > 0)
                    m_strMainValue[i] = m_ptShowCfg->tOption[j].strOpt[0].c_str();
                else
                    m_strMainValue[i] = m_ptShowCfg->tShow[j].strValue.c_str();

                break;
            }
        }
    }

    m_strMainValue[0] = theApp.GetOEMValue();
    m_strMainValue[1] = theApp.GetTubSizeValue();
    m_strMainValue[2] = theApp.GetThreadMatValue();
    m_strMainValue[3] = theApp.GetCouplingValue();

    UpdateData(FALSE);
}

void CTorqueDlg::UpdateDlgLabel()
{
    switch(theApp.m_nLangType)
    {
        case LANGUAGE_CHINESE:
            m_strLBM2.Format("Ť��-��������ͼ        ����λ:����=%s, ����=�ܣ�", theApp.m_strUnit);
            m_strLBM9.Format("Ť��(%s)", theApp.m_strUnit);
            break;
        case LANGUAGE_ENGLISH:
            m_strLBM2.Format("Torque-Turn Graph:    (Vert=%s, Hori=Turn)", theApp.m_strUnit);
            m_strLBM9.Format("Torque(%s)", theApp.m_strUnit);
            break;
        /*case LANGUAGE_RUSSIAN:
            m_strLBM2.Format("�ԧ�ѧ�. ��������(����.:  ��. ����=%s, ��. ����=���.)", theApp.m_strUnit);
            m_strLBM9.Format("������ާ��(%s)", theApp.m_strUnit);
            break;*/
    }
}

void CTorqueDlg::InitDlgControl()
{
    CFont       myfont;   /*������*/
    CString     strFontName;
    int         i = 0;

    // Checkbox
    m_cbAlarm.SetSameIcon(IDI_LEDREDOFF);
    m_cbComm.SetSameIcon(IDI_LEDREDON);
    for(i=0; i<VALVEMAXNUM; i++)
    {
        m_cbValveStatus[i].SetSameIcon(IDI_LEDREDON);
        m_bValveStatus[i] = FALSE;
    }
    m_nValveMark[0] = VALVESTATUS_SPEED;
    m_nValveMark[1] = VALVESTATUS_UNLOAD;
    m_nValveMark[2] = VALVESTATUS_PRESS;
    m_nValveMark[3] = VALVESTATUS_FLOW;
    m_nValveMark[4] = VALVESTATUS_MAKEUP;
    m_nValveMark[5] = VALVESTATUS_BREAKOUT;

    // button
    m_btnRun.SetIconAndText(IDI_RUN, IDS_STRMAINRUN);
   
    /* ����Ruler���ͺͱ�� */
    m_xAxis1.SetRulerType(RT_HORIZONTAL);
    m_yAxis1.SetRulerType(RT_VERTICAL);
    m_xAxis2.SetRulerType(RT_HORIZONTAL);
    m_yAxis2.SetRulerType(RT_VERTICAL);

    m_wndTorque.SubclassDlgItem(IDC_LINE_TORQUE, this);
    m_wndRpm.SubclassDlgItem(IDC_LINE_RPM, this);
    m_wndTorque.m_bBear = m_ptComm->bBear;

    ResetLineChart();

    InitMainShowPara();

    /* �򿪴��� */
    m_bComm     = ChangeCommParam();

    return;
}

BOOL CTorqueDlg::OnInitDialog()
{
    CRect       rcTmp;
    CString     strAboutMenu;
    CMenu       *pSysMenu = NULL;
    //CDlgBear    dlgBear;
    

    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    // Load accelerators
    m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(), m_lpszTemplateName);
    ASSERT(m_hAccel);

    pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    m_tMenu[LANGUAGE_CHINESE].LoadMenu(IDR_MAINMENU);
    m_tMenu[LANGUAGE_ENGLISH].LoadMenu(IDR_MAINMENU_ENG);
    //m_tMenu[LANGUAGE_RUSSIAN].LoadMenu(IDR_MAINMENU_RUS);
    SetMenu(&m_tMenu[theApp.m_nLangType]);

    m_iShackle = theApp.m_bShackle;
#if 0
    if(theApp.m_bFirstRun)
    {
        /* ��ʾѡ���Ť�ضԻ��򣬶�ʱ�Զ��ر� */
        dlgBear.m_bBigTorq = theApp.m_bBigTorq;
        dlgBear.m_bShackle = m_iShackle;
        dlgBear.DoModal();
        m_iShackle = dlgBear.m_bShackle;
        theApp.m_bFirstRun = FALSE;
    }
#endif
    // TODO: Add extra initialization here
    /* ״̬������ */
    m_StatusBar.CreateEx(this,SBT_TOOLTIPS,WS_CHILD|WS_VISIBLE|CBRS_BOTTOM,AFX_IDW_STATUS_BAR);
    // Set the indicators namely caps and nums lock status
    m_StatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT));
    GetClientRect(&rcTmp);
    m_StatusBar.SetPaneInfo(0,ID_INDICATOR_NUM,SBPS_STRETCH ,rcTmp.Width()-100);
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,ID_INDICATOR_NUM);

    theApp.AdaptDlgCtrlSize(this, 0);

    //���ô�������
    SetProp(m_hWnd,g_strPropName.c_str(),g_hValue);
    theApp.ShowMainTitle();

    /* ��ʼ�������ͱ��� */
    InitVariant();

    /* ��ʼ���Ի���ؼ� */
    InitDlgControl();

    SHOW_MAINDLG_BUTTON(theApp.m_bShowCRC);

    CalcDelayCount();

    UpdateData(FALSE);

    theApp.SaveAppStatus(STATUS_START, __FUNCTION__);

    StartGetValveStatus();

    //Invalidate(TRUE);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTorqueDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CTorqueDlg::OnPaint()
{
    int     iCxIcon = 0;
    int     iCyIcon = 0;
    int     x       = 0;
    int     y       = 0;
    CRect   rcTmp;

    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        iCxIcon = GetSystemMetrics(SM_CXICON);
        iCyIcon = GetSystemMetrics(SM_CYICON);
        GetClientRect(&rcTmp);
        x = (rcTmp.Width()  - iCxIcon + 1) / 2;
        y = (rcTmp.Height() - iCyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTorqueDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

/* ����ԭʼplus����ʾplus���������ݻ�ͼ�ĵ��� */
/* 20190126 �����ֵΪ����˵��ж�۽��� */
/* 20200313 20200223 �������ݿ����ظ��������������һ�����һ��С������������m_iOutPoints��������
   ����쳣����ResetData������
   20200706 �̶�SAVE_FULLCIR���棬���������������ݣ�һ���Ǳ���ģ�һ������ʾ��
   */
void CTorqueDlg::CalcPointNum(COLLECTDATA *ptCollData, ORGDATA *ptOrgData)
{
    int     iPlus       = 0;
    int     iCurPoints  = 0;
    //int     iSavePnts   = 0;
    
    ASSERT_NULL(ptCollData);

    /* 20191207 ��һ�������������Ƚϴ��޸���ʾ������Ϊ��ǰ���� */
    if(m_iShowPlus == 0 && ptCollData->nOrgPlus > 0)
    {
        m_iShowPlus  = ptCollData->nOrgPlus;
    }

    iPlus = ptCollData->nOrgPlus - m_iShowPlus;
    if(iPlus < 0)
    {
        //if(1 == m_iShackle)
        //    ptCollData->ucStatus = PLCSTATUS_BREAKOUT;  /* FE:ж�۽���254 */
        iPlus = 0;
        //m_iOutPoints = 0;
    }

    iCurPoints =int((iPlus*MAXLINEITEM) /
                    (m_ptCtrl->fPlus*m_ptCtrl->fMaxCir) + 0.5);

    if(ptOrgData != NULL)
        ptOrgData->ucPointNum = iCurPoints;

    ptCollData->iPointNum = iCurPoints - m_iRecPNum;
    if(ptCollData->iPointNum < 0)
    {
        ptCollData->iPointNum = 0;
    }

    if(iCurPoints < COLLECTPOINTS)
        m_iRecPNum = iCurPoints;

    return;
}

/* ��ʱ��ȡ�������ݣ��ڸ�λ������״̬ʱ����
   ����ֵFALSE��״̬�쳣������ʧ��
         TRUE,��bDataValidΪTRUE�� ����OK��������������
         TRUE,��bDataValidΪFALSE��״̬���������ݲ�OK������������ */
BOOL CTorqueDlg::TimeReadPort(UINT nCurStatus)
{
    BOOL    bReadRes = FALSE;
    CString strInfo;
    /* ״̬�쳣 */
    COMP_BG_R(m_nClashSta, RS_MAX, FALSE);

    /* CLASH ״̬���Ѿ�����˻��壬ֱ�ӷ���READ���� 
       ��������״̬������Ϊ�յ������ֱ�ӷ���READ���� */
    m_nClashSta = RS_NORMAL;

    if(m_iTorqBreakCnt > 0)
    {
        SetCommShowInfo(RS_COMM_OPEN);
    }

    bReadRes = SendData(SCMREADMULTI);
    m_iTorqBreakCnt--;

    /* ����5��û�ж�ȡ�������ô���Ϊ�ر�״̬ */
    if(m_iTorqBreakCnt <= 0)
    {
        strInfo.Format("BreakCnt STOP by Func(%s) on Line(%d) ", __FUNCTION__, __LINE__);
        theApp.SaveMessage(strInfo);
        StopTorque();
        
        m_bComm      = FALSE;
        m_bRunStatus = FALSE;
        m_bAlarm     = TRUE;
        m_cbAlarm.SetSameIcon(IDI_LEDREDON);
        theApp.PlayAlarmSound();
        m_hrtPlayAlarm.CreateTimer(this, ALRAM_DELAY, HRTPlayAlarm);
        return FALSE;
    }
    
    if(bReadRes)
        return TRUE;
    
    /* ��������ʧ�ܣ���Ҫ�����쳣��¼ */
    theApp.SaveSendFailure(SCMREADMULTI);

    return FALSE;
}

/* д����֮ǰ�жϴ���������Ƿ�Ϊ�գ���ǿ���ҪClash�����ȴ����пպ���д */
BOOL CTorqueDlg::PortQueIsEmpty()
{
    return TRUE;
#if 0
    /* ����״̬���жϻ����Ƿ�Ϊ�� */
    ASSERT_NULL_R(g_lpNewComThread, FALSE);

    return g_lpNewComThread->InQueIsEmpty();
#endif
}

/* ���ʱ��С�ڶ�ȡʱ����������ʧ�� */
BOOL CTorqueDlg::CheckReadInterval()
{
    double  fCurTime = 0;
    double  fDiff     = 0;
    CString strInfo;
    fCurTime = clock() * 1000.0 / CLOCKS_PER_SEC;

    fDiff = fCurTime - m_fPreReadTime;
    m_fPreReadTime = fDiff;

    COMP_BGE_R(fDiff, theApp.m_nColletTime, TRUE);

    strInfo.Format("Read Interval is %.0f(<%ld)", fDiff, theApp.m_nColletTime);
    theApp.SaveMessage(strInfo);
    return FALSE;
}

/* ��Ч���ݵĳ���Ӧ�ô���12���г���Ϊ24�����ݣ�Ҳ���Ծ�����ȡ��һ������
   ����ǰ12������ͨ��CRCУ�飬������ͨ���ݴ������Կ��ش�ʱҲ��ԭʼ���ݱ������� */
BOOL CTorqueDlg::CheckDataValid()
{
    int     i           = 0;
    BYTE    ucSCMRejust = 0;
    BOOL    bValid      = TRUE;

    if (m_wRcvLen == 0)
        return FALSE;

    /* ���鳤�� */
    bValid &= (m_wRcvLen >= PORT_VALIDLEN);
    /* ���鴮�ں� */
    bValid &= (m_ucRcvByte[0] == PORT485);
    /* ����������Ч�� */
    if(m_ucRcvByte[3]== COM_READ)
    {
        for(i=0; i<PORT_SUMPLACE; i++)
            ucSCMRejust += m_ucRcvByte[i];
        bValid &= (ucSCMRejust == m_ucRcvByte[PORT_SUMPLACE]);
    }
    /* �ݲ�У��CRC */
    else if(m_ucRcvByte[3]== COM_READMULTI) 
    {
        /*for(i=0; i<m_wRcvLen-1; i++)
            ucSCMRejust += m_ucRcvByte[i];
        bValid &= (ucSCMRejust == m_ucRcvByte[m_wRcvLen-1]);*/
    }
    else    /* error command */
        bValid = FALSE;

    COMP_BTRUE_R(bValid, TRUE);
    
    theApp.SaveCrcErrorData(m_ucRcvByte, m_wRcvLen, m_nCRCERR);
    return FALSE;
}

/*  m_nTestFunc = 0(COLL_PORT)
    ʵ�ʶ�ȡ���ڵ����ݣ���Ҫ�жϵ�һ������ΪPORT485������Ϊ��Ч����
*/
BOOL CTorqueDlg::CollectPort(COLLECTDATA *ptCollData)
{
#ifdef TESTPORT
    /*BYTE   ucTest[] = {0x21,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x22,0x00,0x00,0x08,
                       0x21,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x22,0x00,0x00,0x08 };*/
    BYTE   ucTest[] = {0x21,0x03,0x00,0x15,0x00,0x62,0x00,0xFF,0x05,0x00,0x52,0x21,0x17,
                       0x00,0x7e,0x21,0x26,0x00,0x8c,0x21,0x35,0x00,0x9c ,0x21,0x47,
                       0x00,0xa2,0x21,0x54,0x08 };
#endif

    ASSERT_NULL_R(ptCollData, FALSE);

    //m_iReadTimes++;

    /* ���ʱ��Сд��ȡʱ����������ʧ�� */
    COMP_BFALSE_R(CheckReadInterval(), FALSE);
#ifndef TESTPORT
    /* ��ȡ����ʧ�ܣ����� */
    COMP_BFALSE_R(TimeReadPort(RS_READ_NORMAL), FALSE);
    /* bRead && bData, ���´������� */
    /* ��Ч���ݵĳ���Ӧ�ô���12���г���Ϊ24�����ݣ�Ҳ���Ծ�����ȡ��һ������
       ����ǰ12������ͨ��CRCУ�飬������ͨ���ݴ������Կ��ش�ʱҲ��ԭʼ���ݱ������� */
    //COMP_BFALSE_R(CheckDataValid(), FALSE);
#else
    memcpy(m_ucRcvByte, ucTest, 30);
    m_wRcvLen = 30;
#endif

    return TRUE;
}

/*  m_nTestFunc = (COLL_RAND)1
    Runʱ������ɵ����ݣ����������λ������ʹ��
    Runһ���������ݶ�һ����Stop�����������һ��
*/
BOOL CTorqueDlg::CollectRandData(COLLECTDATA* ptCollData)
{
    int     iCurPoints  = 0;
    int     iPlus       = 0;

    ASSERT_NULL_R(ptCollData, FALSE);

    ptCollData->fTorque   = m_fTestTorq[m_iTest];
    ptCollData->nOrgPlus  = m_iTestPlus[m_iTest++];

    /* ����Plus�������ݵĵ��� */
    CalcPointNum(ptCollData, NULL);
    if(0 == m_iShackle)
    {
        ptCollData->ucStatus = PLCSTATUS_NORMAL;
        if(ptCollData->fTorque > m_ptCtrl->fControl)
        {
            ptCollData->ucStatus = PLCSTATUS_LOCKON;

            /* �����������³�ʼ�� */
            RunInitRand();
        }
    }

    if(m_iTest >= TESTNUM)
    {
        ptCollData->fTorque = m_ptCtrl->fOptTorq;
        m_iTest = 0;
    }

    /* ������ʾ������� */
    m_strRecvData.Format("%d,%ld,%d",(int)ptCollData->fTorque,ptCollData->nOrgPlus,ptCollData->ucStatus);

    /*300��������ת��̫�죬��2��1��*/
    ptCollData->fRpm = m_iOutPoints * m_ptCtrl->fMaxCir * 0.8/m_ptComm->fRpmAdj;

    return TRUE;
}

/*  m_nTestFunc = (COLL_TORQUE)2
    ��AutoSaveŤ�����ݣ�����ʾ
    ����Ϊ��ȡ�ֳ����Ա�������ݣ������ֳ���ͼ��
*/
BOOL CTorqueDlg::CollectTorqData(COLLECTDATA* ptCollData)
{
    BOOL    bReadData   = FALSE;
    BOOL    bValidData  = FALSE;
    int     iCurPoints  = 0;
    int     iPlus       = 0;
    int     i           = 0;
    char    *endptr     = NULL;
    char    acData[3*PORT_MAXVLDDATA+1] = {0};
    BYTE    ucByte[PORT_MAXVLDDATA]     = {0};  //{0x12,0x34,0x56,0x78};
    /* ���ڻ�ȡ��ԭʼ��Ч���ݺͼ����Ť�ص���Ϣ */
    ORGDATA tOrgData    = {0};
    double  fShowTorq   = 0;
    int     iOrgTorq    = 0;
    int     iTorque     = 0;
    double  fRpm        = 0;
    int     iDataLen    = 0;
    CString strTest;
    CString strTime;

    ASSERT_NULL_R(ptCollData, FALSE);
    COMP_BFALSE_R(m_bAutoFileOpen, FALSE);

    /* ��ȡ�ܶ���Чֵʱ�����᳤ܻʱ��ѭ��������©����� */
    BeginWaitCursor();

    while(!bValidData)
    {
        bReadData = m_AutoSavefile.ReadString(strTest);
        /*���ݶ��꣬����OnBtrunֹͣ*/
        if(!bReadData)
        {
            OnBtrun();
            return FALSE;
        }

        /*������Ϣ��ֱ������*/
        COMP_BTRUE_CONTINUE(theApp.IsDebugInfo(strTest));

        /* С����Ч���ȵ����ݣ����� */
        COMP_BTRUE_CONTINUE(strTest.GetLength() < (ORG_DATAPLACE+ORG_PRNDATALEN));

        /*��ȡ���ݣ�ȥ��ǰ���ʱ����Ϣ*/
        strTime = strTest.Left(ORG_PRNTIMELEN);

        /* ���� " T " �ҵ�ǰ�����Ч���� */
        iDataLen = strTest.Find("T ") - ORG_DATAPLACE;
        if (iDataLen < 3 * PORT_DATALEN || iDataLen % 3 != 0)
            continue;
        if (iDataLen > 3 * PORT_MAXVLDDATA)
            iDataLen = 3 * PORT_MAXVLDDATA;

        memcpy(acData, strTest.Mid(ORG_DATAPLACE, iDataLen), iDataLen);
        iDataLen /= 3;
        for (i = 0; i < iDataLen; i++)
            ucByte[i] = (BYTE)strtol(&acData[i * 3], &endptr, 16);

        /* ������Ч���� */
        bValidData = TRUE;
        memcpy(tOrgData.ucRcvByte, &ucByte, PORT_DATALEN);
        iOrgTorq = ucByte[0]*256+ ucByte[1];
        ptCollData->ucStatus =  (ucByte[2] & 0xE0) / 32;
        ptCollData->nOrgPlus = ((ucByte[2] & 0x1F) * 256 +ucByte[3]) * 4;
        fRpm                 = ucByte[5] * 256 + ucByte[6];  // 200ms ������
        /* ��ż������Ҫʹ��ԴŤ�أ����10���ŵ�������� */
        /* 3.17��4.17��һ��Ť����Ҫ�˱��� */
        if(!ClearExcepTorq(iOrgTorq, iTorque))
            ptCollData->ucStatus = PLCSTATUS_REVERSE;
        // ptCollData->fTorque  = iTorque * theApp.m_nTorqMulti;
        fShowTorq = iTorque * theApp.m_nTorqMulti;
        /* 20220215 ����δ���۵����� ==> ����PLC���۴��� */
        ptCollData->fTorque = (int)OverTorqOpt(fShowTorq, ptCollData->ucStatus);

        if (iDataLen >= PORT_DATALEN + 2)
        {
            LightValveStatus(&ucByte[7]);
        }
    }

    EndWaitCursor();

    /* �������ݱ��� */
    tOrgData.ucStatus = ptCollData->ucStatus;
    tOrgData.nTorque = (UINT)ptCollData->fTorque;
    tOrgData.nPlus   = ptCollData->nOrgPlus;

    /* ����Plus�������ݵĵ��� */
    CalcPointNum(ptCollData, &tOrgData);

    /* 300��������ת��̫�죬��m_fRpmAdj���ٶ� */
    if(fRpm == 0)
        tOrgData.fRpm = ptCollData->fRpm = m_iOutPoints * m_ptCtrl->fMaxCir * 0.8/m_ptComm->fRpmAdj;
    else
        tOrgData.fRpm = ptCollData->fRpm = fRpm * 5 * 60 / m_ptCtrl->fPlus/2;

    /* ������ʾ���յ�����Ч���� */
    m_strRecvData.Format("%d,%ld,%d,%.1f(%s)",(int)ptCollData->fTorque,ptCollData->nOrgPlus,ptCollData->ucStatus,ptCollData->fRpm,strTime);

    return TRUE;
}

/*  m_nTestFunc = (COLL_MULTITORQ)3
    ��AutoSaveŤ�����ݣ�����ʾ
    ����Ϊ��ȡ�ֳ����Ա�������ݣ������ֳ���ͼ��
    ��ȡ��������
    20:40:26.627 : ff 15 00 62 00 ff 05 00 52 21 17 00 7e 21 26 00 8c 21 35 00 9c 21 47 00 a2 21 54 08
*/
BOOL CTorqueDlg::CollectMultiTorq(COLLECTDATA* ptCollData)
{
    BOOL    bReadData   = FALSE;
    BOOL    bValidData  = FALSE;
    int     i           = 0;
    char    *endptr     = NULL;
    BYTE    *pucData    = NULL;
    char    acData[150] = {0};
    BYTE    ucByte[50]  = {0};  //{0x12,0x34,0x56,0x78};
    /* ���ڻ�ȡ��ԭʼ��Ч���ݺͼ����Ť�ص���Ϣ */
    int     iOrgTorq    = 0;
    int     iTorque     = 0;
    BYTE    nDataNum    = 0;
    double  fRpm        = 0;
    int     iDataLen    = 0;
    WORD    wCalLen     = 0;
    COLLECTDATA* ptOrgData = NULL;
    double  fShowTorq   = 0;
    CString strTest;
    CString strTime;

    ASSERT_NULL_R(ptCollData, FALSE);
    COMP_BFALSE_R(m_bAutoFileOpen, FALSE);

    ptOrgData = ptCollData;

    /* ��ȡ�ܶ���Чֵʱ�����᳤ܻʱ��ѭ��������©����� */
    BeginWaitCursor();

    while(!bValidData)
    {
        bReadData = m_AutoSavefile.ReadString(strTest);
        /*���ݶ��꣬����OnBtrunֹͣ*/
        if(!bReadData)
        {
            OnBtrun();
            return FALSE;
        }

        /*������Ϣ��ֱ������*/
        COMP_BTRUE_CONTINUE(theApp.IsDebugInfo(strTest));

        /* С����Ч���ȵ����ݣ����� */
        COMP_BTRUE_CONTINUE(strTest.GetLength() < (ORG_DATAPLACE+33));

        /*��ȡ���ݣ�ȥ��ǰ���ʱ����Ϣ*/
        strTime = strTest.Left(ORG_PRNTIMELEN);

        iDataLen = strTest.GetLength() - ORG_DATAPLACE + 1;

        memcpy(acData, strTest.Mid(ORG_DATAPLACE, iDataLen), iDataLen);
        iDataLen /= 3;
        for (i = 0; i < iDataLen; i++)
            ucByte[i] = (BYTE)strtol(&acData[i * 3], &endptr, 16);

        if (ucByte[0] != 0xFF || ucByte[1] != COM_READMULTI)
            continue;
        
        /* ������Ч���� */
        bValidData = TRUE;
    }

    EndWaitCursor();

    LightValveStatus(&ucByte[4]);
    
    fRpm = ucByte[2] * 256 + ucByte[3];  // 200ms ������

    nDataNum = ucByte[6];
    pucData  = &ucByte[7];
    wCalLen  = 7 + nDataNum*PORT_GRPDATA;
    if(iDataLen < wCalLen)
        nDataNum = (iDataLen - 7) / PORT_GRPDATA;
    for(i=0; i<nDataNum && i<PORT_MAXDATANUM; i++)
    {
        iOrgTorq = (*(pucData) * 256 + *(pucData+1));
        ptOrgData->ucStatus =  (*(pucData+2) & 0xE0) / 32;
        ptOrgData->nOrgPlus = ((*(pucData+2) & 0x1F) * 256 + *(pucData+3)) * 4;
        ptOrgData->fRpm     = fRpm * 5 * 60 / m_ptCtrl->fPlus/2;
        /* ��ż������Ҫʹ��ԴŤ�أ����10���ŵ�������� */
        if(!ClearExcepTorq(iOrgTorq, iTorque))
            ptCollData->ucStatus = PLCSTATUS_REVERSE;
        //ptOrgData->fTorque  = iTorque * theApp.m_nTorqMulti;
        fShowTorq = iTorque * theApp.m_nTorqMulti;
        /* 20220215 ����δ���۵����� ==> ����PLC���۴��� */
        ptOrgData->fTorque = (int)OverTorqOpt(fShowTorq, ptCollData->ucStatus);
        
        /* ����Plus�������ݵĵ��� */
        CalcPointNum(ptOrgData, NULL);

        ptOrgData++;
        pucData += 4;
    }

    /* ������ʾ���յ�����Ч���� */
    ptOrgData = &ptCollData[nDataNum-1];
    m_strRecvData.Format("%d,%ld,%d,%.1f(%s)",(int)ptOrgData->fTorque,ptOrgData->nOrgPlus,ptOrgData->ucStatus,ptOrgData->fRpm,strTime);
    return TRUE;
}

/*  m_nTestFunc = 4
    ��AutoSave Collect���ݣ�����ʾ
    ��DlgCollect����
*/
BOOL CTorqueDlg::CollectHisData()
{
    BOOL    bReadData   = FALSE;
    BOOL    bValidData  = FALSE;
    int     iPlace      = -1;
    int     i           = 0;
    int     iBufIndex   = 0;
    unsigned long ulTmp = 0;
    char    ucTemp[5];
    char    *pcStopStr  = NULL;
    CString strTest;

    COMP_BFALSE_R(m_bAutoFileOpen, FALSE);

    ucTemp[0] = '0';
    ucTemp[1] = 'X';

    /* ��ȡ�ܶ���Чֵʱ�����᳤ܻʱ��ѭ��������©����� */
    BeginWaitCursor();
    while(!bValidData)
    {
        bReadData = m_AutoSavefile.ReadString(strTest);
        /* ���ݶ��귵�� */
        if(!bReadData)
        {
            m_AutoSavefile.Close();
            m_bAutoFileOpen = FALSE;
            return FALSE;
        }

        /* ��COL��Ϣ��ֱ������ */
        iPlace = strTest.Find(theApp.m_strDbgHead[DBG_COLLECT]);
        COMP_BTRUE_CONTINUE(iPlace == -1);

        /* �ҵ���Ч����λ�� */
        iPlace = strTest.Find(") ");
        COMP_BTRUE_CONTINUE(iPlace == -1);

        for(i=iPlace+2; i<strTest.GetLength(); i++)
        {
            /* һ����������"21 "��������һ���ո񣬿ո���forѭ����i�Լ��ۼ� */
            ucTemp[2] = strTest.GetAt(i++);
            ucTemp[3] = strTest.GetAt(i++);
            ucTemp[4] = 0;
            ulTmp = strtoul(&ucTemp[0], &pcStopStr, 0);
            m_ucRcvByte[iBufIndex++] = (BYTE)ulTmp;
        }

        m_wRcvLen  = iBufIndex;
        bValidData = TRUE;
    }
    EndWaitCursor();

    return TRUE;
}

BOOL CTorqueDlg::ValidStatus3(COLLECTDATA* ptCollData)
{
    ASSERT_NULL_R(ptCollData, FALSE);

    /* ǰ������Ч�����ݣ� ״̬3��Ч */
    COMP_BG_R(m_tCollData.nAllCount, 1, TRUE);

    /* ǰһ�ε����������Ч����Ч�������һ��Ϊ0�������ϳ���״̬3��Ϊ��Ч��״̬3
       ��Ҫ����ShowPlus������ת�ٻ�ܴ� */
    COMP_BG_R(m_fOutTorque, m_ptCtrl->fShow, TRUE);

    ReStart();
    m_iShowPlus = ptCollData->nOrgPlus;
    return FALSE;
}

/* ��λ���ϱ�״̬3�Ĵ��� */
BOOL CTorqueDlg::Status3Proc(COLLECTDATA* ptCollData, BOOL *pFinish)
{
    static int  iPreTorq    = 0;

    ASSERT_NULL_R(pFinish, FALSE);
    ASSERT_NULL_R(ptCollData, FALSE);

    /* 20140211 ����һ�����ݺ�������ϳ���״̬3�����ǻ����е��������ݣ�ֱ��ɾ�� */
    COMP_BFALSE_R(ValidStatus3(ptCollData), FALSE);

    m_nCurDelay++;
    if(m_nCurDelay == 1)
    {
        iPreTorq    = (int)ptCollData->fTorque;
        if(ptCollData->iPointNum == 0)
            ptCollData->iPointNum = 1;
        return TRUE;
    }
    else if(m_nCurDelay >= m_nDelayCount)
    {
        iPreTorq    = 0;
        m_nCurDelay = 0;
        *pFinish    = TRUE;
        return TRUE;
    }
    else
    {
        // Ť��û�б仯��ֱ�ӷ���
        // 20200315 ǰһ��Ť�ؿ��ܱȵ�ǰŤ�ش���һ������״̬3���ߣ�����ظ��ϴ�����
        if(iPreTorq >= ptCollData->fTorque)
        {
            ptCollData->ucStatus = PLCSTATUS_REPEAT;
            return TRUE;
        }
        // ���ٲ���һ����
        iPreTorq = (int)ptCollData->fTorque;
        if(ptCollData->iPointNum == 0)
            ptCollData->iPointNum = 1;
    } 

    return TRUE;
}

/* ��λ���ϱ�״̬4�Ĵ��� */
BOOL CTorqueDlg::Status4Proc(COLLECTDATA* ptCollData, BOOL *pFinish)
{
    ASSERT_NULL_R(ptCollData, FALSE);
    ASSERT_NULL_R(pFinish, FALSE);

    if(m_iOutPoints == 0)
    {
        ZeroPointUpdateData(ptCollData, FALSE);
    }
    /* ��������(��������������) */
    else
    {
        MorePointInsertData(ptCollData, FALSE);
    }

    *pFinish  = TRUE;
    return TRUE;
}

/* ж�۰汾����������ת������״̬Ϊ254 */
BOOL CTorqueDlg::Status254Proc(COLLECTDATA* ptCollData, BOOL *pFinish)
{
    double  fPreTorque  = 0;    /* ǰһ��Ť�أ�PointNum > 1ʱ�������� */
    double  fPreRpm     = 0;    /* ǰһ��ת�� */
    
    ASSERT_NULL_R(ptCollData, FALSE);
    ASSERT_NULL_R(pFinish, FALSE);

    ASSERT_ZERO_R(m_tCollData.nAllCount, FALSE);

    fPreTorque = m_tCollData.fTorque[m_tCollData.nAllCount - 1];
    fPreRpm    = m_tCollData.fRpm[m_tCollData.nAllCount - 1];

    /* ����״̬254��������ʱֻ���ǲ���һ������,��Finish����ʱ�����ҽ�bLastPoint��ΪTRUE */
    INSERT_DATA(fPreTorque, fPreRpm, FALSE);

    m_wndTorque.UpdatePos(fPreTorque);
    m_wndRpm.UpdatePos(fPreRpm);
    UpdateTorqueData(fPreTorque, fPreRpm);

    UpdateOutData(ptCollData);

    *pFinish  = TRUE;
    return TRUE;
}

/* �ռ�������������û�б仯��ֻ����Ť�� */
void CTorqueDlg::ZeroPointUpdateData(COLLECTDATA *ptCollData, BOOL bFinish)
{
    ASSERT_NULL(ptCollData);

    /* ��һ�����ݻ���Zero�д�����¼fOutTorque��OutPointNumΪ1
       �ڶ�������ʱ������More�и��µ��ṹ�� */
    if(m_tCollData.nAllCount == 0)
    {
        //m_tCollData.nAllCount = 1;
        INSERT_DATA(ptCollData->fTorque, ptCollData->fRpm, FALSE);
        UpdateOutData(ptCollData->fTorque, 1);
        return;
    }

    /* 20200418 zero����ֻ����Ť�ظ����ֵ */
    if(m_fOutTorque > m_tCollData.fTorque[m_tCollData.nAllCount-1])
    {
        m_wndTorque.UpdatePos(m_fOutTorque);
        m_wndRpm.UpdatePos(ptCollData->fRpm);
        UpdateTorqueData(m_fOutTorque, ptCollData->fRpm);
    }

    UpdateOutData(ptCollData);

    return;
}

/* �ռ��������������б仯������Ť�غ�ת�� */
void CTorqueDlg::MorePointInsertData(COLLECTDATA *ptCollData, BOOL bFinish)
{
    int     i           = 0;
    double  fPreTorque  = 0;    /* ǰһ��Ť�أ�PointNum > 1ʱ�������� */
    double  fPreRpm     = 0;    /* ǰһ��ת�� */
    double  fInsTorque  = 0;
    double  fInsRpm     = 0;
    BOOL    bInsertOK   = FALSE;

    ASSERT_NULL(ptCollData);

    /* ��һ�����ݣ����뱣���Out���ݣ���¼��ǰ��Ť�غ͵��� */
    if(m_tCollData.nAllCount == 0)
    {
        if(m_fOutTorque != 0)
        {
            INSERT_DATA(ptCollData->fTorque, ptCollData->fRpm, FALSE);
        }

        UpdateOutData(ptCollData);

        return;
    }

    /*��������*/
    fPreTorque = m_tCollData.fTorque[m_tCollData.nAllCount - 1];
    fPreRpm    = m_tCollData.fRpm[m_tCollData.nAllCount - 1];

    for(i=0; i<m_iOutPoints; i++)
    {
        fInsTorque = (m_fOutTorque -fPreTorque)*(i+1)/m_iOutPoints + fPreTorque;
        fInsRpm    = (ptCollData->fRpm-fPreRpm)*(i+1)/m_iOutPoints + fPreRpm;

        INSERT_DATA(fInsTorque, fInsRpm, FALSE);
    }

    UpdateOutData(ptCollData);

    return;
}

void CTorqueDlg::SetTorqDataCfg(TorqData::Torque *ptPBData)
{
    ASSERT_NULL(ptPBData);
    ptPBData->set_btoolbuck(m_bToolBuck);
    ptPBData->set_bncheckip(!theApp.m_bCheckIP);
    ptPBData->set_strmemo(m_ptComm->aucMemo);
    ptPBData->set_strremark(m_ptComm->aucRemark);

    ptPBData->set_fmaxlimit(m_ptCtrl->fMaxLimit);
    ptPBData->set_fupperlimit(m_ptCtrl->fUpperLimit);
    ptPBData->set_fcontrol(m_ptCtrl->fControl);
    ptPBData->set_fopttorq(m_ptCtrl->fOptTorq);
    ptPBData->set_flowerlimit(m_ptCtrl->fLowerLimit);
    ptPBData->set_fspeeddown(m_ptCtrl->fSpeedDown);
    ptPBData->set_fshow(m_ptCtrl->fShow);
    ptPBData->set_fbear(m_ptCtrl->fBear);
    ptPBData->set_fuppertai(m_ptComm->fUpperTai);
    ptPBData->set_flowertai(m_ptComm->fLowerTai);
    ptPBData->set_fmaxcir(m_ptCtrl->fMaxCir);
    ptPBData->set_fuppercir(m_ptCtrl->fUpperCir);
    ptPBData->set_fcontrolcir(m_ptCtrl->fControlCir);
    ptPBData->set_flowercir(m_ptCtrl->fLowerCir);

    ptPBData->set_fplus(m_ptCtrl->fPlus);
    ptPBData->set_fmaxrpm(m_ptCtrl->fMaxRPM);
    ptPBData->set_fcut(m_ptCtrl->fCut);
    ptPBData->set_frpmadj(m_ptComm->fRpmAdj);
    ptPBData->set_fmulti(m_ptComm->fMulti);

    ptPBData->set_fminshlslope(m_ptComm->fMinShlSlope);
    ptPBData->set_fmaxdeltacir(m_ptComm->fMaxDeltaCir);
    ptPBData->set_fmindeltacir(m_ptComm->fMinDeltaCir);
}

/*Ť�ؿ�����ɣ�LastPoint����ΪTRUE���жϿ�������������*/
void CTorqueDlg::FinishSetStatus()
{
    int     i = 0;
    DWORD   dwQuality   = 0;
    WORD    wIPPos      = 0;
    WORD    wIPPlus     = 0;
    int     iCurIPPos   = 0;
    double  fDelCir     = 0;

    COMP_BL(m_tSaveData.ftorque_size(), 1);

    SetTorqDataCfg(&m_tSaveData);

    if(m_iShackle == 0)
    {
        wIPPos= theApp.SearchDeltaIP(&m_tSaveData, theApp.m_bCheckIP);
        theApp.SetIPInfo(&m_tSaveData, m_tSaveData.ftorque(wIPPos));

        for(i = m_tCollData.nAllCount - 1; i>0; i--)
        {
            if(m_tCollData.fTorque[i] <= m_tSaveData.ftorque(wIPPos))
            {
                iCurIPPos = i - m_iPriorCnt;
                if (m_tCollData.nCurCount > 0 && iCurIPPos < 0)
                {
                    if(iCurIPPos > -5)
                        iCurIPPos = 1;
                }
                break;
            }
        }

        wIPPlus = theApp.GetIPPlus(&m_tSaveData, wIPPos);
        fDelCir = (m_tSaveData.dwtotalplus() - wIPPlus) / m_tSaveData.fplus();
        m_wndTorque.DrawInflection(iCurIPPos, fDelCir, m_tSaveData.ftorque(wIPPos));
    }
    
    /* �����ж� */
    dwQuality = theApp.JudgeQuality(&m_tSaveData, m_iShackle);
    SetQuality(dwQuality);
    m_strQuality    = theApp.GetQualityInfo(&m_tSaveData);
    if(dwQuality != QUA_RESU_GOOD)
    {
        m_bAlarm = TRUE;
        m_cbAlarm.SetSameIcon(IDI_LEDREDON);
    }

    SetShowPara(&m_tSaveData);
    UpdateData(FALSE);

    return;
}


/* ��collectData��ȡ���MAXPOINT���浽saveData */
/* 3.20 �������ݶ����棬��ʾʱ�ٷ������� */
/* 3.22 SaveData���д����ռ������ݶ����棬ȥ���ظ��� */
void CTorqueDlg::GetValidTorqData()
{
    int  i          = 0;
    int  iTotalPlus = 0;

    ASSERT_NULL(m_ptPortData);

    m_tSaveData.Clear();
    /* ��������20190916: ֻ�������������������ݣ����ٱ�����������ݡ������� */
    /* 20200706 ��PortData��ȡ��������� */
    for (i = 0; i < (int)m_ptPortData->nSaveCount; i++)
    {
        m_tSaveData.add_ftorque(m_ptPortData->fTorque[i]);
        m_tSaveData.add_frpm(m_ptPortData->fRpm[i]);
        m_tSaveData.add_dwdelplus(m_ptPortData->iDelPlus[i]);
        iTotalPlus += m_ptPortData->iDelPlus[i];
    }
    m_tSaveData.set_dwtotalplus(iTotalPlus);
    return;
}

/*Ť�ؿ�����ɣ���ɻ�ͼ���Ż������ݱ��桢��λ�Ȳ���*/
void CTorqueDlg::FinishControl()
{
    /* 20191211 finishʱֻ�������ߣ��������������Ѿ����� */
    m_wndTorque.DrawFinLine();
    m_wndRpm.DrawFinLine();

    //SHOWCIRINFO(m_tCollData.nCurCount, m_tCollData.nAllCount, m_ptCtrl->fMaxCir, m_strCircle);

    /* ��CollectData�л�ȡ�������ݵ�SaveData */
    GetValidTorqData();

    /* ��ɿ��ƴ��� */
    FinishSetStatus();

    /* ���յ�  ���� FinishSetStatus ��*/
    // DrawInflectionPoint();

    /* ���ڼ���Ť�زŴ��� */
    if(m_ptComm->fMaxTorq >= m_ptCtrl->fSpeedDown)
    {
        SaveIntoData(&m_tSaveData);
    }

    /* ���ñ�����λʱ�� */
    /* 20171031 �����ñ�����λʱ�䣬������ʾŤ��ʱ����ʼ��һ������ */
    /* 20171206 ������λ��ʱ��������������Ƭ�����ϱ������ݻ��ڿ���Ť���ϱ���һ��ʱ�� */
    m_hrtReadPort.KillTimer();
    m_hrtGuard.CreateTimer(this,theApp.m_nReset*1000, HRTGuard);

    UpdateOutData(0,0);
    
    Invalidate(TRUE);

    ResetData();
    if(m_bAlarm)
    {
        theApp.PlayAlarmSound();
        m_hrtPlayAlarm.CreateTimer(this, ALRAM_DELAY, HRTPlayAlarm);
    }
    return;
}

void CTorqueDlg::HRTReadPort(CWnd *pUser)
{
    CTorqueDlg *pThis = (CTorqueDlg *)pUser;
    pThis->SendMessage(WM_COLLECTTIMEROUT, 0, 0);
}

void CTorqueDlg::HRTSaveData(CWnd *pUser)
{
    CTorqueDlg *pThis = (CTorqueDlg *)pUser;
    pThis->SendMessage(WM_SAVEDATATIMEROUT, 0, 0);
}

void CTorqueDlg::HRTGuard(CWnd *pUser)
{
    CTorqueDlg *pThis = (CTorqueDlg *)pUser;
    pThis->SendMessage(WM_GUARDTIMEROUT, 0, 0);
}
/*
void CTorqueDlg::HRTGuardRead(CWnd *pUser)
{
    CTorqueDlg *pThis = (CTorqueDlg *)pUser;
    pThis->SendMessage(WM_GUARDREADTIMEROUT, 0, 0);
}
*/
void CTorqueDlg::HRTPortBuff(CWnd *pUser)
{
    CTorqueDlg *pThis = (CTorqueDlg *)pUser;
    pThis->SendMessage(WM_PORTBUFFTIMEROUT, 0, 0);
}

void CTorqueDlg::HRTSaveDebug(CWnd *pUser)
{
    CTorqueDlg *pThis = (CTorqueDlg *)pUser;
    pThis->SendMessage(WM_SAVEDEBUGTIMEROUT, 0, 0);
}

void CTorqueDlg::HRTPlayAlarm(CWnd *pUser)
{
    CTorqueDlg *pThis = (CTorqueDlg *)pUser;
    pThis->SendMessage(WM_ALARMPLAYTIMEROUT, 0, 0);
}

void CTorqueDlg::HRTReadValve(CWnd *pUser)
{
    CTorqueDlg *pThis = (CTorqueDlg *)pUser;
    pThis->SendMessage(WM_READVALVETIMEROUT, 0, 0);
}

/* �����汾ż��Ť������������Ť��Ϊ0
   ж�۰汾����Ť��������ż��Ť��Ϊ0 
   20200303 : ��ż����ʱ������-1��ת��ΪUINTΪһ���ǳ����ֵ���ȿ��Ƿ���Ϊ0 
   20200320 : ��ż����ʱ��˵����ת������FALSE���ں�����������״̬Ϊ0xCC��˵����ת����������
   */
BOOL CTorqueDlg::ClearExcepTorq(int iOrgTorq, int& iDestTorq)
{
    BOOL  bOdd = FALSE;     // ����

    // ������ż������0������
    /* 20191106 0��������ͨ����ʾŤ�ؿ��ƣ����ڲ鿴һȦ������ */
    //ASSERT_ZERO_R(nOrgTorq, -1);

    bOdd = (iOrgTorq%2 == 1 ? TRUE : FALSE);
    iDestTorq = iOrgTorq; 
    if(bOdd)
        iDestTorq --;

    ASSERT_ZERO_R(iDestTorq, TRUE);

    if(m_iShackle > 0)
    {
        COMP_BFALSE_R(bOdd, FALSE);
    }
    else
    {
        COMP_BTRUE_R(bOdd, FALSE);
    }

    return TRUE;
}


double CTorqueDlg::OverTorqOpt(double fTorq, BYTE ucStatus)
{
    double fPostTorq = 0;
    double fRandTorq = 0;

    /* 20220310 ��Ƭ����ʾ��һ�£�ֱ�ӷ���ԭֵ */
    return fTorq;

    /* С����СŤ�أ�ֱ�ӷ��� */
    COMP_BLE_R(fTorq, theApp.m_tParaCfg.tCtrl.fLowerLimit, fTorq);

    /* Ť�س���25%�������Ż� */
    COMP_BG_R(fTorq, theApp.m_tParaCfg.tCtrl.fUpperLimit * 1.25, fTorq);

    /* ����Ť�ش���5000 ֱ�ӷ��� */
    COMP_BG_R(theApp.m_tParaCfg.tCtrl.fControl, SWITCHLOWLIMIT, fTorq);

    srand((unsigned)time(NULL));
    fPostTorq = fTorq;
    fRandTorq = rand() * (theApp.m_tParaCfg.tCtrl.fUpperLimit/100) / RAND_MAX;

    /* �Ͽ����״̬ 3��4��ֱ���޸�Ϊ���Ť���µ�ֵ */
    if (ucStatus >= PLCSTATUS_UNLOAD)
    {
        /* С�����Ť�أ�������ֱ�ӷ��أ��������Ͽ�Ť�ؿ϶��������Ť��
           ��ʹ��ǰ�Ͽ�Ť��С����СŤ�أ����ǹ��䣬������ */
        COMP_BLE_R(fTorq, theApp.m_tParaCfg.tCtrl.fUpperLimit, fTorq);

        /* ȡ����Ť�غ��Ͽ�Ť�ؾ�ֵ�����С�����Ť�أ���OK */
        fPostTorq = (theApp.m_tParaCfg.tCtrl.fOptTorq+ fTorq) / 2;
        COMP_BLE_R(fPostTorq, theApp.m_tParaCfg.tCtrl.fUpperLimit, fPostTorq);

        /* ��������ľ�ֵ�����Ť��-1%�Ŀ���Ť�����ֵ��Ϊ�Ͽ�Ť�� */
        fPostTorq = theApp.m_tParaCfg.tCtrl.fUpperLimit - fRandTorq;
        return fPostTorq;
    }

    /* ״̬Ϊ����̬ 0��1��2����֤�����Ť���� */
    /* 20220221 ѹ����СŤ��==�����Ť����  */
    /* ������СŤ�غ����Ť�ؾ�ֵ�Ŵ��� */
    COMP_BL_R(fTorq, ((theApp.m_tParaCfg.tCtrl.fLowerLimit+ theApp.m_tParaCfg.tCtrl.fOptTorq)/2), fTorq);

    /* m_fOutTorque С�����Ť�أ��͵�ǰŤ��ȡ��ֵ�����С�ڿ���Ť����OK */
    fPostTorq = (m_fOutTorque + fTorq) / 2;
    COMP_BL_R(fPostTorq, theApp.m_tParaCfg.tCtrl.fOptTorq, fPostTorq);

    /* ��������ľ�ֵ�����Ť��-1%�����Ť�����ֵ��Ϊ��ǰŤ��, �������ǰһ��Ť����OK */
    // 1%���ֵ�����γɺ��ߣ���ʱ����
    /*fPostTorq = theApp.m_tParaCfg.tCtrl.fControl - fRandTorq;
    COMP_BG_R(fPostTorq, m_fOutTorque, fPostTorq);*/

    /* ȡǰһ��Ť�غ����Ť�صľ�ֵ��Ϊ��ǰŤ�� */
    fPostTorq = (m_fOutTorque + theApp.m_tParaCfg.tCtrl.fOptTorq) / 2;

    return fPostTorq;
}

/* ����һ�����ڶ�ȡ�ļ�¼() */
void CTorqueDlg::SavePortNormalInfo(COLLECTDATA *ptCollData) 
{
    int     i           = 0;
    BOOL    bHaveData   = FALSE;
    BYTE    ucSCMRejust = 0;
    BYTE    *pucData    = NULL;
    int     iOrgTorq    = 0;
    int     iTorque     = 0;
    double  fRpm        = 0;
    /* ���ڻ�ȡ��ԭʼ��Ч���ݺͼ����Ť�ص���Ϣ */
    ORGDATA tOrgData    = {0};
    double  fShowTorq   = 0;
    CString strTime;
    SYSTEMTIME ts;

    ASSERT_NULL(ptCollData);

    /* 3.20��֮ǰ�汾����Ч����12 */
    memcpy(tOrgData.ucRcvByte, &m_ucRcvByte[PORT_DATAPLACE], PORT_DATALEN);
    /* 3.21֮��汾���ٶȺ�����2�ֽڱ�ʾ����״̬ */
    if(m_wRcvLen >= PORT_VALIDLEN + 2)
    {
        LightValveStatus(&m_ucRcvByte[PORT_DATAPLACE+PORT_DATALEN]);
    }

    /* ����Ť�غ�RPM */
    /* 3.17��4.17��һ��Ť����Ҫ�˱��� */
    pucData = &m_ucRcvByte[PORT_DATAPLACE];
    iOrgTorq = (*(pucData) * 256 + *(pucData+1));
    ptCollData->ucStatus    =  (*(pucData+2) & 0xE0) / 32;
    ptCollData->nOrgPlus    = ((*(pucData+2) & 0x1F) * 256 + *(pucData+3)) * 4;
    fRpm                    = *(pucData+5) * 256 + *(pucData+6);  // 200ms ������
    /* ��ż������Ҫʹ��ԴŤ�أ����10���ŵ�������� */
    if(!ClearExcepTorq(iOrgTorq, iTorque))
        ptCollData->ucStatus = PLCSTATUS_REVERSE;
    // ptCollData->fTorque = iTorque * (int)theApp.m_nTorqMulti;
    fShowTorq = iTorque * (int)theApp.m_nTorqMulti;
    /* 20220215 ����δ���۵����� ==> ����PLC���۴��� */
    ptCollData->fTorque = (int)OverTorqOpt(fShowTorq, ptCollData->ucStatus);

    /* �������ݱ��� */
    tOrgData.ucStatus = ptCollData->ucStatus;
    tOrgData.nTorque = (UINT)ptCollData->fTorque;
    tOrgData.nPlus   = ptCollData->nOrgPlus;

    /* ������ʾ���յ�����Ч���� */
    GetLocalTime(&ts);
    strTime.Format("%02d:%02d:%02d.%03d",ts.wHour,ts.wMinute, ts.wSecond, ts.wMilliseconds);
    m_strRecvData.Format("%d,%ld,%d,%.1f(%s)",(int)ptCollData->fTorque,ptCollData->nOrgPlus,ptCollData->ucStatus,fRpm,strTime);

    /* ����Plus�������ݵĵ��� */
    CalcPointNum(ptCollData, &tOrgData);

    /* 300��������ת��̫�죬��m_fRpmAdj���ٶ� */
    if(fRpm == 0)
        tOrgData.fRpm = ptCollData->fRpm = m_iOutPoints * m_ptCtrl->fMaxCir * 0.8/m_ptComm->fRpmAdj;
    else
        tOrgData.fRpm = ptCollData->fRpm = fRpm * 5 * 60 / m_ptCtrl->fPlus/2;

    /* ���������ֵ����ת�� */
    theApp.SaveOrdData(&tOrgData, m_ucRcvByte, m_wRcvLen);
}

/* ����һ�����ڶ�ȡ�ļ�¼() */
/* 0x21 03 00 0x15 �ٶ�����H �ٶ�����L ״̬λ�� ״̬λ�� ����(<=5) Ť��1 ������1  */
void CTorqueDlg::SavePortMultiDataInfo(COLLECTDATA *ptCollData) 
{
    int     i           = 0;
    BYTE    *pucData    = NULL;
    int     iOrgTorq    = 0;
    int     iTorque     = 0;
    double  fRpm        = 0;
    BYTE    nDataNum    = 0;
    ORGDATA tOrgData    = {0};
    WORD    wCalLen     = 0;
    COLLECTDATA *ptOrgColl = NULL;
    CString strTime;
    SYSTEMTIME ts;

    ASSERT_NULL(ptCollData);
    ptOrgColl = ptCollData;

    LightValveStatus(&m_ucRcvByte[6]);
    
    fRpm = m_ucRcvByte[4] * 256 + m_ucRcvByte[5];  // 200ms ������

    nDataNum = m_ucRcvByte[8];
    pucData  = &m_ucRcvByte[9];
    wCalLen  = 9 + nDataNum*PORT_GRPDATA;
    if(m_wRcvLen < wCalLen)
        nDataNum = (m_wRcvLen - 9) / PORT_GRPDATA;

    for(i=0; i<nDataNum && i<PORT_MAXDATANUM; i++)
    {
        iOrgTorq = (*(pucData) * 256 + *(pucData+1));
        ptOrgColl->ucStatus =  (*(pucData+2) & 0xE0) / 32;
        ptOrgColl->nOrgPlus = ((*(pucData+2) & 0x1F) * 256 + *(pucData+3)) * 4;
        ptOrgColl->fRpm     = fRpm * 5 * 60 / m_ptCtrl->fPlus/2;
        /* ��ż������Ҫʹ��ԴŤ�أ����10���ŵ�������� */
        if(!ClearExcepTorq(iOrgTorq, iTorque))
            ptOrgColl->ucStatus = ptCollData->ucStatus = PLCSTATUS_REVERSE;
        ptOrgColl->fTorque  = iTorque * theApp.m_nTorqMulti;
        
        /* �������ݱ��� */
        tOrgData.ucStatus = ptOrgColl->ucStatus;
        tOrgData.nTorque = (UINT)ptOrgColl->fTorque;
        tOrgData.nPlus   = ptOrgColl->nOrgPlus;
        tOrgData.fRpm    = ptOrgColl->fRpm;
        
        /* ����Plus�������ݵĵ��� */
        CalcPointNum(ptOrgColl, &tOrgData);

        ptOrgColl++;
        pucData += PORT_GRPDATA;
    }

    /* ������ʾ���յ�����Ч���� */
    ptOrgColl = &ptCollData[nDataNum-1];
    GetLocalTime(&ts);
    strTime.Format("%02d:%02d:%02d.%03d",ts.wHour,ts.wMinute, ts.wSecond, ts.wMilliseconds);

    m_strRecvData.Format("%d,%ld,%d,%.1f(%s)",(int)ptOrgColl->fTorque,ptOrgColl->nOrgPlus,ptOrgColl->ucStatus,fRpm,strTime);
    theApp.SaveMultiData(&tOrgData, m_ucRcvByte, m_wRcvLen);
}

LRESULT CTorqueDlg::CollectTimerOut(WPARAM wParam, LPARAM lParam)
{
    BOOL    bCollectOK  = FALSE;
    
    /* ��ʱ��ȡ��������Ϣ����װ������collect����������ֵ */
    COLLECTDATA tCollData[PORT_MAXDATANUM] = {0};

    /* ��ȡһ��ԭʼ���� */
    bCollectOK = (this->* g_CollectProc[theApp.m_nTestFunc])(&tCollData[0]);
    COMP_BFALSE_R(bCollectOK, 0);

    /* ���Դ���������tCollData�У�������m_ucRcvByte�� */
    if(theApp.m_nTestFunc >= COLL_RAND)
    {
        RcvTorqDataProc(&tCollData[0]);
    }

    return 0;
}

BOOL CTorqueDlg::CheckPortData(char *pData, int iLen, BOOL &bFini)
{
    int     iNum = 0;
    ASSERT_NULL_R(pData, FALSE);
    ASSERT_ZERO_R(iLen, FALSE);
    
    /* ��һ���ֽڲ���PORT485������������һ������ */
    if(PORT485 != pData[0])
    {
        return FALSE;
    }

    if(iLen < 11)
        return TRUE;

    switch(pData[1])
    {
        case 0x10:
        case 0x88:
            if(iLen >= 11)
            {
                iNum = pData[2];
                if(iLen >= (3 + iNum*8))
                    bFini = TRUE;
            }
            return TRUE;
            break;

        case 0x06:
            if( pData[3] != COM_READVALVE &&
                pData[3] != COM_READCALIB )
                return FALSE;

            if(iLen >= PORT_VALIDLEN)
            {
                bFini = TRUE;
            }
            return TRUE;
            break;

        case 0x03:
            if(iLen >= PORT_VALIDLEN)
            {
                switch(pData[3])
                {
                    case COM_READ:
                        if(iLen == PORT_VALIDLEN || iLen >= PORT_VLDVLVLEN)
                            bFini = TRUE;
                        break;

                    case COM_READMULTI:
                        iNum = pData[8];
                        if(iLen == (10 + iNum*4))
                            bFini = TRUE;
                        break;
                    default:
                        return FALSE;
                        break;
                }
            }
            return TRUE;
            break;

        default:
            return FALSE;
            break;
    }

    return FALSE;
}

LONG CTorqueDlg::OnCommunication(WPARAM Char, LPARAM num)
{
    static char PortBuf[200]  = {0};
    static int  iDataLen      = 0;  //���ݳ��ȣ�һ�δ���һ������
    BOOL    bDataOK  = FALSE;
    BOOL    bInvalid = FALSE;

    ASSERT_ZERO_R(num, 0);

    if (iDataLen + num >= 200)
    {
        iDataLen = 0;
        return 0;
    }
    
    /* ��ʱ��ȡ��������Ϣ����װ������collect����������ֵ */
    memcpy(&PortBuf[iDataLen], (unsigned char*)Char, num);
    iDataLen += num;

    /* �Ƿ����ݣ����㷵�� */
    if(!CheckPortData(&PortBuf[0], iDataLen, bDataOK))
    {
        iDataLen = 0;
        return 0;
    }

    /* ����û����ȫ��ֱ�ӷ��� */
    COMP_BFALSE_R(bDataOK, 0);

    m_wRcvLen = iDataLen;
    memcpy(&m_ucRcvByte[0], &PortBuf[0], iDataLen);
    iDataLen = 0;

    switch(PortBuf[1])
    {
        /* collect */
        case 0x10:
        case 0x88:
            ::SendMessage(m_pdlgCollect->m_hWnd, WM_COLLECT_DATA, m_wRcvLen, 0);
            break;

        case 0x03:
            if(m_ucRcvByte[3] == COM_READ || m_ucRcvByte[3] == COM_READMULTI)   // ��ȡŤ������
            {
                RcvTorqDataProc(NULL);
                m_iTorqBreakCnt = (int)ceil(MAXCOMMBREAKTIME / theApp.m_nColletTime);
            }
            break;
            
        case 0x06:
            if(PortBuf[3] == COM_READVALVE)     // ���÷�ֵ
            {
                LightValveStatus(&m_ucRcvByte[4]);
                m_iValveBreakCnt = (int)ceil(MAXCOMMBREAKTIME / READVALVE_LEN);
            }
            if(PortBuf[3] == COM_READCALIB)     // ��ȡ��Ƭ��У׼��Ϣ
            {
                ::SendMessage(m_pdlgCalib->m_hWnd, WM_CALIB_DATA, m_wRcvLen, 0);
            }

            break;

        default:
            break;
    }

    return 0;
}

int CTorqueDlg::RcvTorqDataProc(COLLECTDATA *ptCollData)
{
    int     i           = 0;
    BOOL    bHaveS3     = FALSE;
    BOOL    bFinish     = FALSE;
    BOOL    bCollectOK  = FALSE;
    double  fCurCir     = 0;
    UINT    nPriorPlus  = 0;
    int     iDelPlus    = 0;
    UINT    nDataNum    = 0;
    UINT    nSaveNum    = 0;
    CString strInfo;
    
    COLLECTDATA tCollData[PORT_MAXDATANUM] = {0};

    ASSERT_NULL_R(m_ptPortData, 0);

    if(ptCollData == NULL)
    {
        /* bRead && bData, ���´������� */
        /* ��Ч���ݵĳ���Ӧ�ô���12���г���Ϊ24�����ݣ�Ҳ���Ծ�����ȡ��һ������
           ����ǰ12������ͨ��CRCУ�飬������ͨ���ݴ������Կ��ش�ʱҲ��ԭʼ���ݱ������� */
        COMP_BFALSE_R(CheckDataValid(), 0);
      
        if(m_ucRcvByte[3] == COM_READMULTI)
            SavePortMultiDataInfo(&tCollData[0]);  //����һ�δ��ڶ�ȡ�Ķ�����¼
        else    //COM_READ
            SavePortNormalInfo(&tCollData[0]); //����һ�����ڶ�ȡ�ļ�¼
    }
    else
    {
        memcpy(&tCollData[0], ptCollData, PORT_MAXDATANUM*sizeof(COLLECTDATA));
    }
    /* ��ȡ������ɣ���ʼ�������ݣ���ͼ���߱��� */
    fCurCir = m_tCollData.nAllCount*m_ptCtrl->fMaxCir / MAXLINEITEM;

    for(i=0; i<PORT_MAXDATANUM; i++)
    {
        if(PLCSTATUS_REVERSE == tCollData[i].ucStatus)
        {
            ReStart();
            m_iShowPlus = tCollData[i].nOrgPlus;
            UpdateOutData(0, 0);
            UpdateData(FALSE);
            return 0;  // no continue;
        }

        if( (tCollData[i].fTorque == 0 && tCollData[i].nOrgPlus == 0) ||
            (tCollData[i].fTorque > OVERFLOWTORQ))
        {
            break;
        }

        /* ����Ч���ݱ��浽���ڱ���ṹ�� */
        //if(NULL != m_ptPortData)
        {
            /* ���������������߷�ת��������Ϊ���ظ� */
            if( m_ptPortData->nLastPlus <= tCollData[i].nOrgPlus ||
                m_ptPortData->nLastPlus - tCollData[i].nOrgPlus >= MAXREVERSEPLUS)  // �ظ�
            {
                if (m_ptPortData->nLastPlus == 0)
                    iDelPlus = 0;
                else
                    iDelPlus = tCollData[i].nOrgPlus - m_ptPortData->nLastPlus;
                if (iDelPlus >= 0)       // С��0Ϊ�ظ�����������
                {
                    nSaveNum = m_ptPortData->nSaveCount;
                    m_ptPortData->fTorque[nSaveNum]  = tCollData[i].fTorque;
                    m_ptPortData->fRpm[nSaveNum]     = tCollData[i].fRpm;
                    m_ptPortData->iDelPlus[nSaveNum] = iDelPlus;
                    m_ptPortData->nLastPlus = tCollData[i].nOrgPlus;
                    m_ptPortData->nSaveCount++;
                }
            }
        }
        if(bFinish)
        {
            tCollData[i].ucStatus = PLCSTATUS_REPEAT;
            continue;
        }

        /* С����ʾŤ�أ�����¼�����������ͼ��ĵ�������Ӱ���´εĻ�ͼ */
        /* ����Ӵ��ڼ���Ť�ص�С����ʾŤ�أ�Ҳ�������� */
        if (tCollData[i].fTorque < m_ptCtrl->fShow)
        {
            if (m_iShackle > 0)
            {
                if (m_ptComm->fMaxTorq > m_ptCtrl->fSpeedDown && fCurCir >= 0.01)
                {
                    strInfo.Format("< ShowTorq FinishControl by Func(%s) on Line(%d) ", __FUNCTION__, __LINE__);
                    theApp.SaveMessage(strInfo);
                    m_hrtSaveData.KillTimer();
                    FinishControl();
                    return 0;
                }
            }

            ReStart();

            m_iShowPlus = tCollData[i].nOrgPlus;
            UpdateOutData(0, 0);
            UpdateData(FALSE);
            return 0;
        }

        /* ��������ת��ֱ������ */
        nPriorPlus = m_iPriorPlus;
        if( tCollData[i].nOrgPlus <= nPriorPlus && tCollData[i].ucStatus != PLCSTATUS_UNLOAD)
        {
            /* 20200223 �������ݿ����ظ��������������һ�����һ��С������ */
            /* 20200315 �����������ݱ����̣�����״̬F0����INSERTDATAʱҲ���� */
            /* 20200320 ������С��300������ͣ�٣���Ϊ�����ظ� ;20200809 300->3000, ��������10s */
            /* 20201219 3000->3000���ֳ�����û�л��������Ļ�300 MAXREVERSEPLUS */
            if (nPriorPlus - tCollData[i].nOrgPlus < MAXREVERSEPLUS)
            {
                tCollData[i].ucStatus = PLCSTATUS_REPEAT;
                continue;
            }

            /* 20200320 ������ͻȻ���ͣ�����Ϊ��Ť����ͣ�ٺ�Ƭ���������� */
            m_iPriorPlus = 0;
            m_iShowPlus  = tCollData[i].nOrgPlus;
            /* 20201219 ��תʱLastPlus�����㣬��������0�����������ʾ������ */
            m_ptPortData->nLastPlus = tCollData[i].nOrgPlus;
        }
        if(tCollData[i].nOrgPlus > (UINT)m_iPriorPlus)
            m_iPriorPlus = tCollData[i].nOrgPlus;

        if(m_iShackle > 0)
        {
            if(!m_hrtSaveData.ValidTimer())
            {   
                m_hrtSaveData.CreateTimer(this,theApp.m_nSaveTime*1000, HRTSaveData);
            }
            if(tCollData[i].ucStatus == PLCSTATUS_BREAKOUT)
            {
                Status254Proc(&tCollData[i], &bFinish);
            }
        }
        else
        {
            /* ״̬3ж�����ݵ����⴦�� */
            if(tCollData[i].ucStatus == PLCSTATUS_UNLOAD)
            {
                COMP_BFALSE_R(Status3Proc(&tCollData[i], &bFinish), 0);
                bHaveS3 = TRUE;
            }
            /* ״̬4��100�ͱ�����ֱ����ɲ���ʾ���� */
            else if(tCollData[i].ucStatus == PLCSTATUS_LOCKON)
            {
                Status4Proc(&tCollData[i], &bFinish);
                bHaveS3 = TRUE;
            }
            else
                m_nCurDelay = 0;
        }

        /*ֱ����ʾŤ������20111030*/
        if(tCollData[i].fTorque > m_ptComm->fMaxTorq)
            m_ptComm->fMaxTorq = tCollData[i].fTorque;
    }
    nDataNum = i;
    //ASSERT_ZERO_R(nDataNum, 0);
    if(i >= PORT_MAXDATANUM)
        nDataNum = PORT_MAXDATANUM;

    if(nDataNum > 0)
    {
        /* for ж�� */
        if(m_iShackle > 0)
            m_strTorque.Format("%.0f / %.0f", tCollData[nDataNum-1].fTorque, m_ptComm->fMaxTorq);
        else if(bHaveS3)
            m_strTorque.Format("%.0f", m_ptComm->fMaxTorq);
        else    /* ��ʾ���һ����Ч���� */
            m_strTorque.Format("%.0f", tCollData[nDataNum-1].fTorque);
        m_fRpm    = tCollData[nDataNum-1].fRpm;

        for(i=0; i<(int)nDataNum && i<PORT_MAXDATANUM; i++)
        {
            /* 20200315 �����������ݱ����̣�����״̬F0����INSERTDATAʱҲ���� */
            if(PLCSTATUS_REPEAT == tCollData[i].ucStatus)
                continue;
            /* ֻ��Ť�ظ��£�������û�����ӣ����������ݻ����ϸ���Ť�أ������������� */
            if(m_iOutPoints == 0)
            {
                ZeroPointUpdateData(&tCollData[i], bFinish);
            }
            /* ��������(��������������) */
            else
            {
                MorePointInsertData(&tCollData[i], bFinish);
            }
        }

        /* ����Ȧ�� */
        //SHOWCIRINFO(m_tCollData.nCurCount, m_tCollData.nAllCount, m_ptCtrl->fMaxCir, m_strCircle);

        /* ͼ�γ��������������Զ�ˢ��
           �ֳ���ʱ��Ϊ��������С��ͼ�γ������������� ûͼ�� */
        fCurCir   = (m_tCollData.nCurCount*m_ptCtrl->fMaxCir/MAXLINEITEM);
        if(fCurCir > m_ptCtrl->fControlCir || fCurCir > m_ptCtrl->fMaxCir)
        {
            ClearInfo(FALSE);
            ResetLineChart();
            m_iPriorCnt += m_tCollData.nCurCount;
            m_tCollData.nCurCount = 0;
            m_iShowPlus  = tCollData[nDataNum-1].nOrgPlus;
        }

        UpdateData(FALSE);
    }
    
    /* �����½� */
    if(bFinish)
    {
        strInfo.Format("bFinish TRUE FinishControl by Func(%s) on Line(%d) ", __FUNCTION__, __LINE__);
        theApp.SaveMessage(strInfo);
        FinishControl();
        UpdateData(FALSE);
        return 0;
    }

    return 0;
}

/* ����Ť�ػ���ֹͣ��10s����Ҫ�������ݡ�������١�ж�ɵ� */
LRESULT CTorqueDlg::GuardTimerOut(WPARAM wParam, LPARAM lParam)
{
    m_iTest = 0;
    ResetLineChart();
    ClearInfo();
    
    m_nCur = theApp.m_nCurNO + 1;
    
    if(QUA_RESU_GOOD == m_tSaveData.dwquality())
    {
        theApp.m_nCurRunningNO++;
    }
    
    m_strQuality.Empty();
    m_tSaveData.Clear();

    m_strMainValue[MAINSHOWWELL].Format("%d", theApp.m_nCurRunningNO);
    m_bCanModLastData = FALSE;
    GetDlgItem(IDC_BTNQUALITY)->EnableWindow(m_bCanModLastData);
    Invalidate(TRUE);

    //ɱ��λ��ʱ��
    m_hrtGuard.KillTimer();
    //��λʱ�䶨ʱ��ȡ����
    //m_hrtGuardRead.KillTimer();

    if(m_bRunStatus) /* ����������� */
    {
        /* ��ʱ���������´򿪴��ڣ�����������ݣ����⴮����ʷ�����γ���ЧŤ�ؼ�¼ */
        /* 20171028 : ���ڶ�δ򿪵���ָ��Ƿ���ȥ�����´򿪴������� */
        //m_bComm = ChangeCommParam(FALSE);

        /* ��λ��ʱ����ʱ�����´�Timer1�ռ��������� */
        m_hrtReadPort.CreateTimer(this,theApp.m_nColletTime, HRTReadPort);
        UpdateOutData(0,0);
    }

    return 0;
}

/* ��λʱ��ȡ���ڶ�ʱ��Timer4��ʱ */
#if 0
LRESULT CTorqueDlg::ResetTimerOut(WPARAM wParam, LPARAM lParam)
{
    m_nClashSta = RS_NORMAL;

    COMP_BFALSE_R(TimeReadPort(RS_READ_RESET), 0);
    /* 3.21֮��汾���ٶȺ�����2�ֽڱ�ʾ����״̬ */
    if(m_wRcvLen >= PORT_VALIDLEN + 2)
    {
        LightValveStatus(&m_ucRcvByte[PORT_DATAPLACE+PORT_DATALEN]);
    }

    /* bRead && bData, ���´������� */
    theApp.SaveResetData(m_ucRcvByte, m_wRcvLen);

    return 0;
}
#endif
/* ������ǰ�����������ݵȴ���Ƭ��д��ʱ��Timer7��ʱ������Inque���壬Ȼ�����·�����Ϣ */
LRESULT CTorqueDlg::PortBuffTimerOut(WPARAM wParam, LPARAM lParam)
{
#if 0
    BYTE    ucTmpRecv[PORTBUFF];      /* modbus������Ϣ��ֵ */
    WORD    wRecvLen = 0;

    /* ɱ�����屣����ʱ������ջ��� */
    m_hrtPortBuff.KillTimer();

    ASSERT_NULL_R(g_lpNewComThread, 0);
    memset(ucTmpRecv, 0, PORTBUFF);

    g_lpNewComThread->ReceiveData_Command(ucTmpRecv, &wRecvLen);
    
    theApp.SavePortBufData(ucTmpRecv, wRecvLen, m_nClashSta);
    /* ����ǿգ�����SERIALBUFF_TIMER��ʱ������ʱ�����建�� */
    if(m_nClashSta == RS_READCLASH)  /* Read */
    {
        m_nClashERR++;
        CollectTimerOut(0,0);
        m_hrtReadPort.CreateTimer(this,theApp.m_nColletTime, HRTReadPort);
    }
    else if(m_nClashSta == RS_RESETCLASH)
    {
        /* resetʱ�ĳ�ͻ������Clash�����Ǵ�ӡ���� */
        //ResetTimerOut(0,0);
        //m_hrtGuardRead.CreateTimer(this,theApp.m_nColletTime, HRTGuardRead);
    }
#endif
    return 0;
}

LRESULT CTorqueDlg::SaveDataTimerOut(WPARAM wParam, LPARAM lParam)
{
    CString strInfo;

    strInfo.Format("FinishControl by Func(%s) on Line(%d) ", __FUNCTION__, __LINE__);
    theApp.SaveMessage(strInfo);

    /* ɱ�����ݱ��涨ʱ�� */
    m_hrtSaveData.KillTimer();

    FinishControl();
    return 0;
}

LRESULT CTorqueDlg::SaveDebugTimerOut(WPARAM wParam, LPARAM lParam)
{
    theApp.SaveTorqNum();
    theApp.SaveLogInfo();
    return 0;
}

void CTorqueDlg::StopPlayAlam()
{
    m_cbAlarm.SetSameIcon(IDI_LEDREDOFF);
    theApp.StopAlarmSound();
    m_hrtPlayAlarm.KillTimer();
    m_bAlarm = FALSE;
}

LRESULT CTorqueDlg::ALarmPlayTimerOut(WPARAM wParam, LPARAM lParam)
{
    StopPlayAlam();
    return 0;
}

void CTorqueDlg::LightValveStatus(BYTE *pucStatus)
{
    int     i = 0;

    ASSERT_NULL(pucStatus);

    for(i=0; i<VALVEMAXNUM; i++)
    {
        if(m_bValveStatus[i] == (pucStatus[1] & m_nValveMark[i]))
            continue;
        
        /* ǰ��״̬OK����ΪNOK������ */
        if(m_bValveStatus[i]) 
            m_cbValveStatus[i].SetSameIcon(IDI_LEDREDON);
        else
            m_cbValveStatus[i].SetSameIcon(IDI_LEDON);
        
        m_bValveStatus[i] = (pucStatus[1] & m_nValveMark[i]);
    }
}

LRESULT CTorqueDlg::ReadValveTimerOut(WPARAM wParam, LPARAM lParam)
{
    BOOL    bReadRes = FALSE;

    COMP_BGE_R(theApp.m_nTestFunc, COLL_RAND, 0);
    if(m_iValveBreakCnt > 0)
    {
        SetCommShowInfo(RS_COMM_OPEN);
    }

    bReadRes = SendData(SCMREADVALVE);
    m_iValveBreakCnt--;

    /* ����5��û�ж�ȡ�������ô���Ϊ�ر�״̬ */
    if(m_iValveBreakCnt <= 0)
    {
        SetCommShowInfo(RS_COMM_CLOSE);
        m_bRunStatus = FALSE;
        m_bComm = FALSE;
    }
    
    if(!bReadRes)
    {
        theApp.SaveSendFailure(SCMREADVALVE);
        return -1;
    }

    return 0;
}

BOOL CTorqueDlg::RunInitPort()
{
    BOOL bRes = FALSE;

    bRes = SendData(SCMCUT);
    Sleep(PORTSENDTIME);

    return TRUE;
}

BOOL CTorqueDlg::RunInitRand()
{
    int     i     = 0;
    double  fTorq = 0;
    double  fRpm  = 0;
    int     iCtrl = 1;

    m_iTest = 0;
    m_iShowPlus = 0;
    m_iPriorPlus= 0;
    memset(m_fTestTorq, 0 ,TESTNUM*sizeof(double));
    memset(m_iTestPlus, 0 ,TESTNUM*sizeof(int));
    srand( (unsigned)time( NULL ) );

    for(i=1; i<TESTNUM; i++)
    {
        fTorq = rand() * 100.0 / RAND_MAX;
        m_fTestTorq[i] = m_fTestTorq[i-1] + fTorq*iCtrl;
        if(m_iShackle > 0)
        {
            if(iCtrl == -1)
            {
                if(m_fTestTorq[i] < m_ptCtrl->fSpeedDown)
                {
                    m_fTestTorq[i] = m_ptCtrl->fSpeedDown;
                    m_iTestPlus[i] = m_iTestPlus[i-1];
                }
            }
            else if(m_fTestTorq[i] > m_ptCtrl->fControl)
                iCtrl = -1;
        }

        fRpm  = rand() * 100.0 / RAND_MAX;
        m_iTestPlus[i] = m_iTestPlus[i-1] + (int)fRpm;
    }

    return TRUE;
}

BOOL CTorqueDlg::RunIniAutoFile()
{
    CString strFullFile;

    if(m_bAutoFileOpen)
    {
        m_AutoSavefile.Close();
    }
    m_bAutoFileOpen = FALSE;

    theApp.UpdateAutoSaveFileName();

    strFullFile = theApp.m_strAppPath.c_str();
    strFullFile += theApp.m_strAutoSaveFile.c_str();
    /* �򿪵�autosavefile����ϵͳ�Զ�������ļ�����һ����Ϊ�˱����ͻ����ʧ�� */
    COMP_BE_R(strFullFile.CompareNoCase(theApp.m_strLogFile.c_str()), 0, FALSE);

    ASSERT_ZERO_R(m_AutoSavefile.Open(strFullFile, CFile::modeRead|CFile::shareDenyNone), FALSE);

    m_bAutoFileOpen = TRUE;
    return TRUE;
}

void CTorqueDlg::RunTorque()
{
    BOOL    bInit = FALSE;
    CString strBtnText;
    CString strInfo;

    if(!m_bComm)
    {
        m_bComm = ChangeCommParam();
        if(!m_bComm)
        {
            strBtnText.Format(IDS_STRMAINRUN);
            m_btnRun.SetIconAndText(IDI_RUN, IDS_STRMAINRUN);
            m_btnRun.SetWindowText(strBtnText);
            m_bRunStatus = FALSE;
            return;
        }
    }
    
    m_iTorqBreakCnt = (int)ceil(MAXCOMMBREAKTIME / theApp.m_nColletTime);

    switch(theApp.m_nTestFunc)
    {
    case COLL_RAND:
        bInit = RunInitRand();
        break;
    case COLL_TORQUE:
    case COLL_MULTITORQ:
        bInit = RunIniAutoFile();
        break;
    default:
        bInit = RunInitPort();
        break;
    }

    /* ��ʼ��ʧ�ܣ������У�״̬��Ǩ�� */
    if(!bInit)
    {
        m_bRunStatus = FALSE;
        strInfo.Format(IDS_STRINFMAININITFAIL);
        theApp.SaveShowMessage(strInfo);
        return;
    }

    /* �������� */
    StopPlayAlam();

    strBtnText.Format(IDS_STRMAINSTOP);
    m_btnRun.SetIconAndText(IDI_STOP, IDS_STRMAINSTOP);
    m_btnRun.SetWindowText(strBtnText);

    /* ��ʼ������ */
    m_strQuality.Empty();
    ResetData();
    ResetLineChart();

    /* �ؼ�������ʼ�� */
    m_strTorque = _T("0");
    //m_strCircle = _T("0");
    m_fRpm      = 0;
    m_iRecPNum  = 0;
    m_iShowPlus = 0;
    m_iPriorPlus= 0;
    m_strRecvData.Empty();

    /* ���ö�ʱ����ʱ��ȡ���� */
    m_fPreReadTime = clock() * 1000.0 / CLOCKS_PER_SEC;
    m_hrtReadPort.CreateTimer(this,theApp.m_nColletTime, HRTReadPort);
    /* ��ʱ����CRC�͵�����Ϣ */
    m_hrtSaveDebug.CreateTimer(this, AUTOSAVE_TLEN, HRTSaveDebug);
    //SetTimer(AUTOSAVE_TIMER, AUTOSAVE_TLEN, NULL);
    m_bCanModLastData = FALSE;
    GetDlgItem(IDC_BTNQUALITY)->EnableWindow(m_bCanModLastData);

    theApp.SaveAppStatus(STATUS_RUN, __FUNCTION__);
    Invalidate(TRUE);
    return;
}

/* ĳЩ�쳣�������������һ���㣬��Ҫ�������FinishSetStatus(����ֹͣ������)
   ������Guard��ʱ����ʱ�������� */
void CTorqueDlg::DrawLastPoint()
{
    COLLECTDATA tNullData = {0};

    if(m_tCollData.nAllCount != 0)
    {
        /* 20200806 fOutTorqueû�л�ʱ�����ٻ�һ�� */
        if(m_fOutTorque > m_ptCtrl->fShow)
        {
            tNullData.fRpm    = m_tCollData.fRpm[m_tCollData.nAllCount - 1];
            if(m_iOutPoints > 0)
                MorePointInsertData(&tNullData, TRUE);
            else
                ZeroPointUpdateData(&tNullData, TRUE);
            // out����
            m_fOutTorque  = 0;
            m_iOutPoints  = 0;
        }
        m_wndTorque.FinishDraw();
        m_wndRpm.FinishDraw();

        /* ��CollectData�л�ȡ�������ݵ�SaveData */
        GetValidTorqData();
        /* ��ɿ��ƴ��� */
        FinishSetStatus();

        /* ���ڼ���Ť�زŴ��� */
        if(m_ptComm->fMaxTorq >= m_ptCtrl->fSpeedDown)
        {
            SaveIntoData(&m_tSaveData);
        }
    }

    /* ���ݸ�λ */
    ResetData();
    
    if(m_bAlarm)
    {
        theApp.PlayAlarmSound();
        m_hrtPlayAlarm.CreateTimer(this, ALRAM_DELAY, HRTPlayAlarm);
    }
}

void CTorqueDlg::StopTorque()
{
    double  fLastTorque = 0.0;
    CString strBtnText;

    m_iTest = 0;
    m_iTorqBreakCnt = (int)ceil(MAXCOMMBREAKTIME / theApp.m_nColletTime);

    if((theApp.m_nTestFunc == COLL_TORQUE ||
        theApp.m_nTestFunc == COLL_MULTITORQ)
        && m_bAutoFileOpen)
    {
        m_AutoSavefile.Close();
        m_bAutoFileOpen = FALSE;
    }

    KillAllTimer();

    /* ���ñ�����λʱ�� */
    //SetTimer(GUARD_TIMER,theApp.m_nReset*1000,NULL);

    strBtnText.Format(IDS_STRMAINRUN);
    m_btnRun.SetIconAndText(IDI_RUN, IDS_STRMAINRUN);
    m_btnRun.SetWindowText(strBtnText);
    theApp.SaveAppStatus(STATUS_STOP, __FUNCTION__);

    theApp.SaveLogInfo();

    DrawLastPoint();

    CLOSE_PORTCOMM();
    
    if(QUA_RESU_GOOD == m_tSaveData.dwquality())
    {
        theApp.m_nCurRunningNO++;
    }
    m_nCur = theApp.m_nCurNO + 1;
    m_strMainValue[MAINSHOWWELL].Format("%d", theApp.m_nCurRunningNO);
    
    m_tSaveData.Clear();
    m_strQuality.Empty();
    m_bCanModLastData = FALSE;
    GetDlgItem(IDC_BTNQUALITY)->EnableWindow(m_bCanModLastData);

    m_bComm = FALSE;
    SetCommShowInfo(RS_COMM_CLOSE);

    return;
}

void CTorqueDlg::OnBtrun()
{
    CString strInfo;

    JUDGE_REG_STATUS();

    if(theApp.m_nTestFunc == COLL_HISTORY)
    {
        strInfo.Format(IDS_STRINFCOLLHISTEST);
        theApp.SaveShowMessage(strInfo);
        return;
    }

    m_bRunStatus = !m_bRunStatus;
    m_nClashSta  = RS_NORMAL;

    if(m_iRecPNum > COLLECTPOINTS)
    {
        m_bRunStatus  = FALSE;
        m_iRecPNum = 0;
    }

    /* Run */
    if(m_bRunStatus)
    {
        StopGetValveStatus();
        RunTorque();
        UpdateData(FALSE);
        return;
    }

    /* else stop */
    StopTorque();
    StartGetValveStatus();
    UpdateData(FALSE);

}

void CTorqueDlg::ResetLineChart()//BOOL bRedraw)
{
    m_wndTorque.RemoveAt();
    m_wndRpm.RemoveAt();

    m_wndTorque.m_fUpperLimit = m_ptCtrl->fUpperLimit;     /* ���Ť�� */
    m_wndTorque.m_fLowerLimit = m_ptCtrl->fLowerLimit;     /* ��СŤ�� */
    m_wndTorque.m_fOptTorq    = m_ptCtrl->fOptTorq;        /* ���Ť�� */
    m_wndTorque.m_fSpeedDown  = m_ptCtrl->fSpeedDown;      /* ����Ť�� */
    m_wndTorque.m_fShow       = m_ptCtrl->fShow;           /* ��ʾŤ�� */
    m_wndTorque.m_fBear       = m_ptCtrl->fBear;           /* �縺Ť�� */
    m_wndTorque.m_fControlCir = m_ptCtrl->fControlCir;     /* �������� */
    m_wndTorque.m_fUpperCir   = m_ptCtrl->fUpperCir;       /* �������� */
    m_wndTorque.m_fLowerCir   = m_ptCtrl->fLowerCir;       /* �������� */
    m_wndTorque.m_fMaxCir     = m_ptCtrl->fMaxCir;         /* ������� */
    m_wndTorque.m_fWidthCir   = m_ptCtrl->fMaxCir;
    m_wndTorque.m_fMaxLimit   = m_ptCtrl->fMaxLimit;       /* ������� */
    m_wndTorque.m_bBear       = m_ptComm->bBear;

    m_wndTorque.m_fUpperTai   = m_ptComm->fUpperTai;
    m_wndTorque.m_fLowerTai   = m_ptComm->fLowerTai;

    m_wndTorque.Add(RGB(255,255,255),m_ptCtrl->fMaxLimit, 0.0, LINETYPE_MAIN);
    m_xAxis1.SetTickPara(10,m_wndTorque.m_fMaxCir);
    m_yAxis1.SetTickPara(20,m_ptCtrl->fMaxLimit);
    m_wndTorque.DrawBkLine();
    m_wndRpm.Add(RGB(255,255,255),m_ptCtrl->fMaxRPM, 0.0);
    m_xAxis2.SetTickPara(10,m_wndTorque.m_fMaxCir);
    m_yAxis2.SetTickPara(3,m_ptCtrl->fMaxRPM);
    m_wndRpm.DrawBkLine();

    UpdateDlgLabel();

    /* 20200320 : ����֮ǰ������ */
    m_wndTorque.Erase();
    m_wndRpm.Erase();
}

/* һ�θ���λ���������е����� */
void CTorqueDlg::SendAllCommand()
{
    COMP_BG(theApp.m_nTestFunc, 0);

    BeginWaitCursor();

    SendData(SCMSHOW);
    Sleep(PORTSENDTIME);
    SendData(SCMSPEED);
    Sleep(PORTSENDTIME);
    SendData(SCMCONTROL);
    Sleep(PORTSENDTIME);
    SendData(SCMLOWER);
    Sleep(PORTSENDTIME);
    SendData(SCMUPPER);
    Sleep(PORTSENDTIME);
    SendData(SCMCUT);
    Sleep(PORTSENDTIME);

    EndWaitCursor();

    return;
}

void CTorqueDlg::SendValveCommand()
{
    int  i = 0;

    BeginWaitCursor();

    for (i = 0; i < VALVERATIONUM; i++)
    {
        g_ucCurValve = i;
        SendData(SCMVPRESS);
        Sleep(PORTSENDTIME);
    }
    
    for(i=0; i<VALVERATIONUM; i++)
    {
        g_ucCurValve = i;
        SendData(SCMVFLOW);
        Sleep(PORTSENDTIME);
    }

    // SendData(SCMVZOOM);
    //Sleep(PORTSENDTIME);

    EndWaitCursor();

    return;
}

/* ���������仯�����ݼ�¼�ļ����ƺ����������Ҫ�仯 */
void CTorqueDlg::ReGetTorqNo()
{
    theApp.GetCurNum();
    m_nCur = theApp.m_nCurNO + 1;
    m_strMainValue[MAINSHOWWELL].Format("%d", theApp.m_nCurRunningNO);
}

void CTorqueDlg::OnSetpara()
{
    CDlgParaSet dlgParaSet;
    PARACFG     *ptCurCfg = NULL;
    SHOWCFG     *ptCurShow = NULL;
    BOOL        bCtrl     = FALSE;
    BOOL        bComm     = FALSE;

    JUDGE_RUN_STATUS(IDS_STRINFRUNNSETPARA);

    dlgParaSet.m_tempCfg  = theApp.m_tParaCfg;
    dlgParaSet.m_ucPort   = theApp.m_tPortCfg.ucPortNo;
    dlgParaSet.m_tempShow = *m_ptShowCfg;
    dlgParaSet.m_tCurTub  = theApp.m_ptCurShow->tTubingCfg;
    if(IDOK != dlgParaSet.DoModal())
        return;

    ptCurCfg  = &dlgParaSet.m_tempCfg;
    ptCurShow = &dlgParaSet.m_tempShow;

    /* �Ƚϲ����Ƿ����仯������仯����Ҫ�޸Ĳ���������Ч */
    bCtrl = CompSysPara((BYTE*)m_ptCtrl,
                        (BYTE*)&ptCurCfg->tCtrl,
                        sizeof(CONTROLPARA));
    bComm = CompSysPara((BYTE*)m_ptComm,
                        (BYTE*)&ptCurCfg->tComm,
                        sizeof(COMMONCFG));

    theApp.m_tParaCfg     = dlgParaSet.m_tempCfg;
    *m_ptShowCfg          = dlgParaSet.m_tempShow;
    theApp.m_ptCurShow->tTubingCfg   = dlgParaSet.m_tCurTub;
    /* ���������仯�����ݼ�¼�ļ����ƺ����������Ҫ�仯 */
    ReGetTorqNo();

    if(!bCtrl || !bComm)
    {
        ResetLineChart();
        Invalidate(TRUE);
        SendAllCommand();
    }
    if(0 != theApp.m_strParaFile.compare(dlgParaSet.m_strParaFile.GetBuffer(0)))
    {
        theApp.WriteParaFileName(dlgParaSet.m_strParaFile.GetBuffer(0));
        theApp.m_strParaFile = dlgParaSet.m_strParaFile;
    }

    if(dlgParaSet.m_ucPort != theApp.m_tPortCfg.ucPortNo)
    {
        m_bComm = ChangeCommParam();
        theApp.m_tPortCfg.ucPortNo = dlgParaSet.m_ucPort;
        theApp.WritePortPara(theApp.m_strParaFile, &theApp.m_tPortCfg);
    }
    theApp.WritePara(theApp.m_strParaFile, &theApp.m_tParaCfg);
    theApp.WriteShowPara();

    /*��ʾ����*/
    InitMainShowPara();
    theApp.SaveAppStatus(STATUS_SETPARA, __FUNCTION__);

    UpdateData(FALSE);
}

void CTorqueDlg::OnBnClickedBtnshowset()
{
    CDlgMainShowSet dlgMain;
    int         iWellIndex  = -1;
    CString     strOld, strNew;
    CString     strOther;
    CString     strRunningNO;
    BOOL        bFileChg = FALSE;
    SHOWCFG     *ptCurShow = NULL;

    JUDGE_RUN_STATUS(IDS_STRINFRUNNSETPARA);

    dlgMain.m_tempShow = *m_ptShowCfg;
    dlgMain.m_tCurTub  = theApp.m_ptCurShow->tTubingCfg;
    dlgMain.m_strTubeNO2  = m_strTubeNO2;
    if (IDOK != dlgMain.DoModal())
        return;

    ptCurShow = &dlgMain.m_tempShow;
    
    /* ����ļ����Ʒ����仯���ļ������޸��ˣ��������������� */
    if(0 != ptCurShow->  tShow[m_ptShowCfg->wFileName].strValue.compare(
            m_ptShowCfg->tShow[m_ptShowCfg->wFileName].strValue))
    {
        bFileChg = TRUE;
    }
    
    *m_ptShowCfg = dlgMain.m_tempShow;
    theApp.m_ptCurShow->tTubingCfg = dlgMain.m_tCurTub;
    m_strTubeNO2 = dlgMain.m_strTubeNO2;

    /* �ļ����Ʒ����仯�����ݼ�¼�ļ����ƺ����������Ҫ�仯 */
    if(bFileChg)
        ReGetTorqNo();
    theApp.WritePara(theApp.m_strParaFile, &theApp.m_tParaCfg);
    theApp.WriteShowPara();

    /*��ʾ����*/
    InitMainShowPara();

    theApp.SaveAppStatus(STATUS_SETPARA, __FUNCTION__);

    UpdateData(FALSE);
}

void CTorqueDlg::OnSetShow()
{
    WORD        i = 0;
    WORD        j = 0;
    WORD        k = 0;
    SHOWCFG     tCurShow;
    CString     strName;
    BOOL        bFind;
    CDlgShowSet dlgShow;

    COMP_BFALSE(theApp.CheckPassWord());

    dlgShow.m_tempShow = *m_ptShowCfg;
    if(IDOK != dlgShow.DoModal())
        return;

    theApp.InitShowCfg(&tCurShow);
    
    /* �ж��Ƿ�������/�޸�name��name˳�������name˳�������option��֮���� */
    tCurShow.wParaNum = dlgShow.m_tempShow.wParaNum;
    tCurShow.wListNum = dlgShow.m_tempShow.wListNum;
    tCurShow.wMainNum = dlgShow.m_tempShow.wMainNum;
    tCurShow.wFileName = dlgShow.m_tempShow.wFileName;
    tCurShow.wStatType = dlgShow.m_tempShow.wStatType;
    for(i=0; i<tCurShow.wListNum && i<MAXPARANUM; i++)
    {
        tCurShow.strList[i] = dlgShow.m_tempShow.strList[i];
    }
    for(i=0; i<tCurShow.wMainNum && i<MAXPARANUM; i++)
    {
        tCurShow.strMain[i] = dlgShow.m_tempShow.strMain[i];
    }

    for(i=0; i<tCurShow.wParaNum && i<MAXPARANUM; i++)
    {
        tCurShow.tShow[i].strName = dlgShow.m_tempShow.tShow[i].strName;

        strName = tCurShow.tShow[i].strName.c_str();
        
        bFind = FALSE;
        for (j = 0; j < m_ptShowCfg->wParaNum && j<MAXPARANUM; j++)
        {
            if (strName.CompareNoCase(m_ptShowCfg->tShow[j].strName.c_str()) == 0)
            {
                bFind = TRUE;
                break;
            }
        }
        
        tCurShow.tOption[i].wOptNum = 0;
        if(bFind)
        {
            tCurShow.tShow[i].strValue = m_ptShowCfg->tShow[j].strValue;
            tCurShow.tOption[i].wOptNum = m_ptShowCfg->tOption[j].wOptNum;
            for (k = 0; k < m_ptShowCfg->tOption[j].wOptNum && k<MAXOPTIONNUM; k++)
                tCurShow.tOption[i].strOpt[k] = m_ptShowCfg->tOption[j].strOpt[k];
        }
    }

    *m_ptShowCfg = tCurShow;

    /* ���������仯�����ݼ�¼�ļ����ƺ����������Ҫ�仯 */
    ReGetTorqNo();

    /*��ʾ����*/
    theApp.WriteShowPara();
    theApp.SaveAppStatus(STATUS_SETSHOW, __FUNCTION__);

    //���ٵ�ǰ����
    theApp.m_pMainWnd = NULL;
    this->DestroyWindow();

    //���´򿪴���
    theApp.ReOpenWindow();
}

void CTorqueDlg::OnAbout()
{
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
}

void CTorqueDlg::OnDebug()
{
    CDlgFuncTest dlgTest;
    int          iOldIcon[4];

    JUDGE_RUN_STATUS(IDS_STRINFRUNNFUNTEST);

    theApp.SaveAppStatus(STATUS_FUNTEST, __FUNCTION__);

    /* ��¼��ǰ״̬�� */
    iOldIcon[0] = m_cbValveStatus[0].m_iIconSame;
    iOldIcon[1] = m_cbValveStatus[1].m_iIconSame;
    iOldIcon[2] = m_cbAlarm.m_iIconSame;
    iOldIcon[3] = m_cbComm.m_iIconSame;

    StopGetValveStatus();
    dlgTest.DoModal();

    /* �ָ�����ǰ״̬ */
    TestSpeedLed(iOldIcon[0]);
    TestUnloadLed(iOldIcon[1]);
    TestAlarmLed(iOldIcon[2]);
    TestCommLed(iOldIcon[3]);

    StartGetValveStatus();
}

BOOL CTorqueDlg::DestroyWindow()
{
    int i = 0;

    delete m_ptPortData;
    m_ptPortData = NULL;
    m_strRecvData.Empty();
    for(i=0; i<17; i++)
        g_strCmdName[i].Empty();

    KillAllTimer();
    
    DELETE_DIALOG(m_pdlgCollect);
    DELETE_DIALOG(m_pdlgCalib);

    CLOSE_PORTCOMM();

    theApp.SaveAppStatus(STATUS_EXIT, __FUNCTION__);

    theApp.SaveTorqNum();
    theApp.SaveLogInfo();

    if(m_bAutoFileOpen)
    {
        m_AutoSavefile.Close();
    }

    return CDialog::DestroyWindow();
}

void CTorqueDlg::OnAppExit()
{
    CDialog::OnOK();
}

BOOL CTorqueDlg::PreTranslateMessage(MSG* pMsg)
{
    HACCEL hAccel;

    if (WM_KEYFIRST <= pMsg->message &&
        pMsg->message <= WM_KEYLAST)
    {
        hAccel = m_hAccel;
        if (hAccel && ::TranslateAccelerator(m_hWnd, hAccel, pMsg))
            return TRUE;
    }

    return CDialog::PreTranslateMessage(pMsg);
}

BOOL CTorqueDlg::ChangeCommParam(BOOL bUpdateText)
{
    PORTCFG tPort;
    CString strShow;

    if(theApp.m_nTestFunc > COLL_PORT)
    {
        if(bUpdateText)
            SetCommShowInfo(theApp.m_nTestFunc+1);
        return TRUE;
    }
    tPort = theApp.m_tPortCfg;
#if 0
    CLOSE_PORTCOMM();
    g_lpNewComThread = new CNewComThread();
    theApp.SavePortOper(PORTOPR_OPEN);
    if(g_lpNewComThread == NULL)
    {
        SetCommShowInfo(RS_COMM_CLOSE);
        return FALSE;
    }

    if ( g_lpNewComThread->InitComm(tPort.ucPortNo,
                                    tPort.nBand ,
                                    tPort.ucParity,
                                    tPort.ucDataBit,
                                    tPort.ucStopBit) )
    {
        g_lpNewComThread->Set_TimeInterval( m_nInterval );
        
        SetCommShowInfo(RS_COMM_OPEN);

        return TRUE;
    }
#else
    m_tPort.ClosePort();
    if (m_tPort.InitPort(this, tPort.ucPortNo, tPort.nBand, tPort.ucParity, tPort.ucDataBit, tPort.ucStopBit,
        EV_RXCHAR | EV_CTS, 512))
    {
        m_tPort.StartMonitoring();
        
        SetCommShowInfo(RS_COMM_OPEN);

        return TRUE;
    }
#endif

    /* else */
    SetCommShowInfo(RS_COMM_CLOSE);
    return FALSE;
}

BOOL CTorqueDlg::CompSysPara(const BYTE *pPara1, const BYTE *pPara2, WORD wSize)
{
    ASSERT_NULL_R(pPara1, FALSE);
    ASSERT_NULL_R(pPara2, FALSE);

    COMP_BNE_R(memcmp((void*)pPara1,(void*)pPara2,wSize), 0, FALSE);

    return TRUE;
}

/* ��ȡ���� */
void CTorqueDlg::FillRead(void)
{
    FillReadCommand(COM_READ);
}

/* ��ȡ�������� */
void CTorqueDlg::FillReadMulti(void)
{
    FillReadCommand(COM_READMULTI);
}

/* У׼���� */
void CTorqueDlg::FillMulti()
{
    FillWriteCommand(COM_ADJUST);

    /*У׼����ֵ*/
    FillWordValue(&MODBUS_CONTENT, (int)(m_ptComm->fMulti*100));
}

/* ȥƤ */
void CTorqueDlg::FillQipi()
{
    FillWriteCommand(COM_QIPI);
}

/*�ռ�����*/
void CTorqueDlg::FillCollect()
{
    FillWriteCommand(COM_COLLECT);

    MODBUS_CONTENT = 0x00;
}

/*�յ���Ч�����ٴβɼ����ݣ����һ��ȷ�Ϻ���Ҫ�ٴ��յ�����*/
void CTorqueDlg::FillCollectOK()
{
    FillWriteCommand(COM_COLLECT);

    MODBUS_CONTENT = 0x01;
}

/*�յ���Ч�����ٴβɼ�����*/
void CTorqueDlg::FillCollectNOK()
{
    FillWriteCommand(COM_COLLECT);

    MODBUS_CONTENT = 0x02;
}

/* ��ʾŤ�� */
void CTorqueDlg::FillShow()
{
    FillWriteCommand(COM_SHOW);

    /* 3.17��4.17��һ��Ť����Ҫ�˱��� */
    FillWordValue(&MODBUS_CONTENT, (int)(m_ptCtrl->fShow/theApp.m_nTorqMulti));
}

/* ����Ť�� */
void CTorqueDlg::FillSpeed()
{
    FillWriteCommand(COM_SPEED);

    /* 3.17��4.17��һ��Ť����Ҫ�˱��� */
    FillWordValue(&MODBUS_CONTENT, (int)(m_ptCtrl->fSpeedDown/theApp.m_nTorqMulti));
}

/* ����Ť�� */
void CTorqueDlg::FillControl()
{
    FillWriteCommand(COM_CONTROL);

    /* 3.17��4.17��һ��Ť����Ҫ�˱��� */
    FillWordValue(&MODBUS_CONTENT, (int)(m_ptCtrl->fControl/theApp.m_nTorqMulti));
}

/* ��ʼ���� */
void CTorqueDlg::FillBSpeed()
{
    FillWriteCommand(COM_BSPEED);
}

/* ֹͣ���� */
void CTorqueDlg::FillESpeed()
{
    FillWriteCommand(COM_ESPEED);
}

/* ��ʼж�� */
void CTorqueDlg::FillBUnload()
{
    FillWriteCommand(COM_BUNLOAD);
}

/* ֹͣж�� */
void CTorqueDlg::FillEUnload()
{
    FillWriteCommand(COM_EUNLOAD);
}

/* ���Ť�� */
void CTorqueDlg::FillUpper()
{
    FillWriteCommand(COM_UPPER);

    /* 3.17��4.17��һ��Ť����Ҫ�˱��� */
    FillWordValue(&MODBUS_CONTENT, (int)(m_ptCtrl->fUpperLimit/theApp.m_nTorqMulti));
}

/* ��СŤ�� */
void CTorqueDlg::FillLower()
{
    FillWriteCommand(COM_LOWER);

    /* 3.17��4.17��һ��Ť����Ҫ�˱��� */
    FillWordValue(&MODBUS_CONTENT, (int)(m_ptCtrl->fLowerLimit/theApp.m_nTorqMulti));
}

/* ����ϵ�� */
void CTorqueDlg::FillCut()
{
    FillWriteCommand(COM_CUT);

    FillWordValue(&MODBUS_CONTENT, (int)(m_ptCtrl->fCut*100));   /*����ֵ*/
}

/*����ʱ��*/
void CTorqueDlg::FillTime()
{
    BYTE    *pPlace  = NULL;

    FillWriteCommand(COM_TIME);

    /* ��*12+�� */
    pPlace = &MODBUS_CONTENT;
    *pPlace++ = (m_tSetTime.GetYear() - 2000)*12 + m_tSetTime.GetMonth();
    *pPlace++ =  m_tSetTime.GetDay();
    *pPlace++ =  m_tSetTime.GetHour();
    *pPlace++ =  m_tSetTime.GetMinute();
}

/* ���ü�ѹ������ */
void CTorqueDlg::FillValvePress()
{
    BYTE    *pPlace  = NULL;

    FillWriteCommand(COM_VPRESS);

    COMP_BGE(g_ucCurValve, VALVERATIONUM);
    
    /*  ����1~3
        Ť�صİٷֱ�
        ѹ����ֵ
        ѹ����ֵ
    */
    pPlace = &MODBUS_CONTENT;
    *pPlace++ = (g_ucCurValve+1);
    *pPlace++ = theApp.m_tValveCfg.ucTorq[0][g_ucCurValve];
    *pPlace++ = theApp.m_tValveCfg.ucRatio[0][g_ucCurValve];
    *pPlace++ = theApp.m_tValveCfg.ucRatio[0][g_ucCurValve];
}

/* �������������� */
void CTorqueDlg::FillValveFlow()
{
    BYTE    *pPlace  = NULL;

    FillWriteCommand(COM_VFLOW);

    COMP_BGE(g_ucCurValve, VALVERATIONUM);
    
    /*  ����1~3
        Ť�صİٷֱ�
        ������ֵ
        ������ֵ
    */
    pPlace = &MODBUS_CONTENT;
    *pPlace++ = (g_ucCurValve+1);
    *pPlace++ = theApp.m_tValveCfg.ucTorq[1][g_ucCurValve];
    *pPlace++ = theApp.m_tValveCfg.ucRatio[1][g_ucCurValve];
    *pPlace++ = theApp.m_tValveCfg.ucRatio[1][g_ucCurValve];
}

/* ��ȡ����״̬���� */
void CTorqueDlg::FillReadValve()
{
    BYTE    *pPlace  = NULL;

    FillWriteCommand(COM_READVALVE);
}


/* ����д�ֶ�У׼ָ�� */
void CTorqueDlg::FillWriteCalib()
{
    BYTE    *pPlace  = NULL;

    FillWriteCommand(COM_WRITECALIB);

    pPlace = &MODBUS_CONTENT;
    *pPlace++ = theApp.m_tCalibCtrl.ucType;
    *pPlace++ = theApp.m_tCalibCtrl.tInfo.ucSegNO;

    FillWordValue(pPlace, theApp.m_tCalibCtrl.tInfo.iCalibTorq/theApp.m_nTorqMulti);
    pPlace += 3;

    FillWordValue(pPlace, theApp.m_tCalibCtrl.tInfo.iSCMTorq/theApp.m_nTorqMulti);
}

/* ���Ϳ��Ʒֶ�У׼ָ�� */
void CTorqueDlg::FillCtrlCalib()
{
    BYTE    *pPlace  = NULL;

    FillWriteCommand(COM_CTRLCALIB);

    pPlace = &MODBUS_CONTENT;
    *pPlace++ = theApp.m_tCalibCtrl.ucProc;
    *pPlace++ = theApp.m_tCalibCtrl.ucType;
}

/* ���Ͷ��ֶ�У׼ָ�� */
void CTorqueDlg::FillReadCalib()
{
    BYTE    *pPlace  = NULL;

    FillWriteCommand(COM_READCALIB);

    pPlace = &MODBUS_CONTENT;
    *pPlace++ = theApp.m_tCalibCtrl.ucType;
    *pPlace++ = theApp.m_tCalibCtrl.tInfo.ucSegNO;
}

#if 0
/* ���÷��Ŵ������� */
void CTorqueDlg::FillValveZoom()
{
    BYTE    *pPlace  = NULL;

    FillWriteCommand(COM_VZOOM);

    /* ����1~30 */
    pPlace = &MODBUS_CONTENT;
    *pPlace++ = theApp.m_tValveCfg.ucZoomRatio;
    *pPlace++ = 0;
    *pPlace++ = 0;
}
#endif
/*           ��Ƭ����������
    #define SCMREAD         15  *��ȡ����*
    #define SCMADJUST       16  *У׼*
    #define SCMZERO         17  *ȥƤ*
    #define SCMCOLLECT      18  *�ɼ�����*
    #define SCMSHOW         19  *��ʾŤ��*
    #define SCMSPEED        20  *����Ť��*
    #define SCMCONTROL      21  *����Ť��*
    #define SCMBSPEED       22  *��ʼ����*
    #define SCMESPEED       23  *ֹͣ����*
    #define SCMBUNLOAD      24  *��ʼж��*
    #define SCMEUNLOAD      25  *ֹͣж��*
    #define SCMUPPER        26  *����Ť��*
    #define SCMLOWER        27  *����Ť��*
    #define SCMCUT          28  *����ϵ��*
    #define SCMCOLLECTOK    29  *�յ���Ч�����ٴβɼ����ݣ����һ��ȷ�Ϻ���Ҫ�ٴ��յ�����*
    #define SCMCOLLECTNOK   30  *�յ���Ч�����ٴβɼ�����*
    #define SCMTIME         31  *���õ�Ƭ��ʱ��*
    #define SCMVPRESS       32  * ��ѹ�� *
    #define SCMVFLOW        33  * ������ * 
    #define SCMREADVALVE    34  * ��ȡ����״̬ * 
    #define SCMREADMULTI    35  * ��ȡ�������� *  
    #define SCMWRITECALIB   36  * д�ֶ�У׼ *
    #define SCMCALIBCTRL    37  * �ֶ�У׼����
    #define SCMREADCALIB    38  * ���ֶ�У׼ **/
    // #define SCMVZOOM        34  * �Ŵ��� *

WORD CTorqueDlg::FillSendByte( UINT nParaType )
{
    int     i           = 0;
    int     iSCMRejust  = 0;
    WORD    wCrcvalue   = 0;
    WORD    wLen        = 8;    /* ��ʼ����� */

    /* ���ø�������Ĳ������� */
    (this->* g_FillByteProc[nParaType-SCMREAD])();

    /*У��λ*/
    for(i=0; i<wLen; i++)
        iSCMRejust += m_ucSndByte[i];
    m_ucSndByte[ wLen++ ] = (BYTE)iSCMRejust;

    /* SCMWRITECALIB 8/9�ֽ�Ϊ A/DŤ�� */
    if(nParaType == SCMWRITECALIB)
        wLen += 2;
    else
    {
        wCrcvalue = GetCRCValue( m_ucSndByte, wLen );
        m_ucSndByte[ wLen++ ] = (BYTE)(wCrcvalue / 256);
        m_ucSndByte[ wLen++ ] = (BYTE)(wCrcvalue % 256);
    }

    /*           ��Ƭ����������                   */
    /*����λ*/
    m_ucSndByte[ wLen++ ] = 0x08;

    return wLen;
}

/*
Ť������                40001
�������루��16λ������  40002
�������루��16λ������  40003
Ť�ص�����              40004
Ť�ص��趨ֵ            40005

��λ���޸�����
д״ָ̬ʾλ            40007        д������ʱ������1��
дŤ������              40008
д�趨Ť��              40009
дŤ��У׼              40010        ����Ť��
�����ź�                40011
100/26*1200
*/
BOOL CTorqueDlg::SendData( UINT nParaType )
{
    int     wLen     = 0;
    CString strCmd;
    
    COMP_BG_R(theApp.m_nTestFunc, 0, FALSE);

    /* ����û�д� */
    if(!m_bComm)
    {
        m_bComm = ChangeCommParam();
    }
    COMP_BFALSE_R(m_bComm, FALSE);
    COMP_BL_R(nParaType, SCMREAD, FALSE);

    memset(m_ucSndByte, 0, PORTBUFF);
    memset(m_ucRcvByte, 0, PORTBUFF);

    wLen = FillSendByte(nParaType);
    strCmd = g_strCmdName[nParaType-SCMREAD];
#if 0
    ASSERT_NULL_R(g_lpNewComThread, FALSE);

    if ( g_lpNewComThread->SendData_Command(m_ucSndByte, wLen ) )
    {
        /* READ����̫�࣬����ʾ��ֻ��ʾ�������� */
        if( nParaType != SCMREAD      &&
            nParaType != SCMREADVALVE &&
            nParaType != SCMREADMULTI)
            theApp.SaveSendData(strCmd, m_ucSndByte, wLen);
        /*Sleep(len*20);*/  /*���ͺ��������Ѿ���len*5+20ms����ʱ*/
        return g_lpNewComThread->ReceiveData_Command(m_ucRcvByte, &m_wRcvLen);
    }
#else
    COMP_BFALSE_R(m_bComm, FALSE);
    m_tPort.WriteToPort((char*)m_ucSndByte, wLen);
#endif
    return TRUE;
}

//---------------------------------------------------------------
//HH 05-04-22 ����[CRC16]У�鷽��ժ¼�� [ModBus ͨѶЭ��]
//---------------------------------------------------------------
WORD CTorqueDlg::GetCRCValue(BYTE *pucCrc, WORD iLen)
{
    BYTE ucHigh = 0xFF ;         /* ��CRC�ֽڳ�ʼ��  */
    BYTE ucLow  = 0xFF ;         /* ��CRC �ֽڳ�ʼ�� */
    int  iIndex = 0 ;            /* CRCѭ���е�����  */

    ASSERT_NULL_R(pucCrc, 0xFFFF);

    while (iLen--)                    /* ������Ϣ������   */
    {
        iIndex = ucHigh^ * pucCrc++ ;  /* ����CRC */
        ucHigh = ucLow ^ g_iModBusHi[iIndex];
        ucLow = g_iModBusLow[iIndex];
    }

    return (ucHigh << 8 | ucLow);
}

void CTorqueDlg::OnZero()
{
#if 0
    CDlgZero    dlgZero;

    dlgZero.m_fZero = m_ptParaCfg->iZero/100.0;
    COMP_BNE(dlgZero.DoModal(), IDOK);

    if(m_ptParaCfg->iZero/100.0 != dlgZero.m_fZero)
    {
        m_ptParaCfg->iZero = int(dlgZero.m_fZero*100);
        theApp.WritePara(theApp.m_strParaFile, m_ptParaCfg);
    }
#endif
}

void CTorqueDlg::TestSpeedLed(int iLedIcon)
{
    COMP_BE(iLedIcon, -1);
    m_cbValveStatus[0].SetSameIcon(iLedIcon);
    UpdateData(FALSE);
}

void CTorqueDlg::TestUnloadLed(int iLedIcon)
{
    COMP_BE(iLedIcon, -1);
    m_cbValveStatus[1].SetSameIcon(iLedIcon);
    UpdateData(FALSE);
}

void CTorqueDlg::TestAlarmLed(int iLedIcon)
{
    COMP_BE(iLedIcon, -1);
    m_cbAlarm.SetSameIcon(iLedIcon);
    UpdateData(FALSE);
}

void CTorqueDlg::TestCommLed(int iLedIcon)
{
    COMP_BE(iLedIcon, -1);
    m_cbComm.SetSameIcon(iLedIcon);
    UpdateData(FALSE);
}

void CTorqueDlg::OnScm()
{
    CDlgScm dlgScm;

    JUDGE_RUN_STATUS(IDS_STRINFRUNNMODSCM);

    dlgScm.DoModal();
}

void CTorqueDlg::ClearInfo(BOOL bClrTorq)
{
    m_iRecPNum  = 0;
    if(bClrTorq)
    {
        m_strTorque = _T("0");
        //m_strCircle = _T("0");
    }
    m_fRpm      = 0;
}

void CTorqueDlg::ReStart()
{
    ClearInfo();
    ResetLineChart();
    ResetData();
    
    m_strQuality.Empty();
    m_tSaveData.Clear();
}
/*
void CTorqueDlg::OnStand()
{
    CDlgAPI dlgApi;

    dlgApi.DoModal();
}
*/
void CTorqueDlg::OnHistorylist()
{
    CDlgHisList     dlgList;
    CDlgHisGrp      dlgGrp;
    CDlgHisSumup    dlgSum;
    CMySheet        sheet(IDS_STRMAINVIEWHIS,this);

    JUDGE_RUN_STATUS(IDS_STRINFRUNNVIEWHIS);

    //sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;

    theApp.SaveAppStatus(STATUS_HISTORY, __FUNCTION__);

    theApp.m_fSheetRatio = 1.0;

    sheet.AddPage(&dlgList);
    sheet.AddPage(&dlgGrp);
    sheet.AddPage(&dlgSum);
    sheet.DoModal();

    /* 20200302: ����ȡ���뾮�����Ϣ�������޸��������Ժ��뾮��Ų���ȷ */
    ReGetTorqNo();
    UpdateData(FALSE);
}

void CTorqueDlg::OnHistorystat()
{
    CDlgDataStat     dlgDataStat;

    JUDGE_RUN_STATUS(IDS_STRINFRUNNVIEWHIS);

    COMP_BFALSE(theApp.CheckPassWord());

    theApp.SaveAppStatus(STATUS_HISSTAT, __FUNCTION__);
    
    dlgDataStat.DoModal();
}

void CTorqueDlg::OnShowtest()
{
    theApp.m_bShowCRC = !theApp.m_bShowCRC;

    SHOW_MAINDLG_BUTTON(theApp.m_bShowCRC);

    UpdateData(FALSE);
}

void CTorqueDlg::OnCollectdata()
{
    JUDGE_RUN_STATUS(IDS_STRINFRUNNCOLLHIS);

    m_pdlgCollect = (CDlgCollect*)GetDlgPoint(m_pdlgCollect,
                                              DLG_COLLECTDATA,
                                              IDD_DLGCOLLECT);
    ASSERT_NULL(m_pdlgCollect);

    StopGetValveStatus();

    m_pdlgCollect->ShowWindow(SW_SHOW);
}

/*
�������Ŀ���Լ��ٷֱȴ���
������4���㣬����20%С��40����1���㣬����λ��Ϊ1/3λ�á��Դ����ơ�
���һ��λ��Ҫ��һ�㣬��Ϊ����ʱ����ߣ�Ť�ش�һЩ���Ӻ���
�����İٷֱ�Ϊ 1/(n+2),2/(n+2),...,n/(n+2),
1��1/3              CTRL:2/3
2: 1/4 1/4          CTRL:1/2
3: 1/5 1/5 1/5      CTRL:2/5
4: 1/6 1/6 1/6 1/6  CTRL:1/3
*/
int  CTorqueDlg::GetInsertNum(double fT1, double fT2)
{
    double  fYu      = 0;
    double  fLevel[5]= {0,1.2,1.4,1.8,2.2};
    int     i        = 0;

    ASSERT_ZERO_R(fT2, 0);

    fYu = fT1/fT2;
    for(i=4;i>0;i--)
    {
        COMP_BGE_R(fYu, fLevel[i], i);
    }

    return 0;
}

void CTorqueDlg::UpdateOutData(double fTorque, int iPoints)
{
    m_fOutTorque  = fTorque;
    m_iOutPoints  = iPoints;
    return;
}

void CTorqueDlg::UpdateOutData(COLLECTDATA *ptColl)
{
    ASSERT_NULL(ptColl);
    
    m_fOutTorque  = ptColl->fTorque;
    m_iOutPoints  = ptColl->iPointNum;
    return;
}

/* ���ڶ�ȡCRC����ʱ��ֹͣ���Ա������ݣ��������� */
void CTorqueDlg::RestartComm()
{
    double  fLastTorque = 0.0;
    double  fCircle     = 0.0;

    m_iTest = 0;

    KillAllTimer();

    DrawLastPoint();

    theApp.SaveAppStatus(STATUS_RESTART, __FUNCTION__);

    COMP_BFALSE(m_bRunStatus);

    /* if(m_bRunStatus) *//* ����������� */
    /* ��ʱ���������´򿪴��ڣ�����������ݣ����⴮����ʷ�����γ���ЧŤ�ؼ�¼ */
    m_bComm = ChangeCommParam();

    /* ��λ��ʱ����ʱ�����´�Timer1�ռ��������� */
    m_hrtReadPort.CreateTimer(this,theApp.m_nColletTime, HRTReadPort);

    return;
}

/* ���ô������ͼ�����Ϣ��ʾ״̬ 
    RS_COMM_CLOSE   0    ���ڹر�
    RS_COMM_OPEN    1    ���ڴ�
    RS_COMM_RAND    2    ����ģ��򿪣������
    RS_COMM_TORQUE  3    ����ģ��򿪣���Ť����ʷ����
    RS_COMM_HISTORY 4    ����ģ��򿪣����ռ���Ƭ����ʷ����
*/
void CTorqueDlg::SetCommShowInfo(UINT nStatus)
{
    CString strShow;
    CString strBtnText;
    PORTCFG port;
    
    port = theApp.m_tPortCfg;

    if(nStatus == RS_COMM_CLOSE)
    {
        m_cbComm.SetSameIcon(IDI_LEDREDON);
        m_CommStatus.SetIcon(AfxGetApp()->LoadIcon(IDI_MODBUSCLOSE));
        strBtnText.Format(IDS_STRMAINOPENPORT);
        strShow.Format( IDS_STRMAINPORTCLOSE, port.ucPortNo );
        m_StatusBar.SetPaneText(0, strShow, TRUE);
        return;
    }

    m_cbComm.SetSameIcon(IDI_LEDON);
    m_CommStatus.SetIcon(AfxGetApp()->LoadIcon(IDI_MODBUSOPEN));
    strBtnText.Format(IDS_STRMAINCLOSEPORT);

    if(nStatus == RS_COMM_OPEN)
    {
        strShow.Format( IDS_STRMAINPORTOPEN,
                        port.ucPortNo,
                        port.nBand,
                        port.ucDataBit,
                        theApp.m_strParity.c_str(),
                        port.ucStopBit);
    }
    else
    {
        strShow.Format( (IDS_STRMAINPORTSIMRAND + nStatus - RS_COMM_RAND) , port.ucPortNo );
    }

    m_StatusBar.SetPaneText(0, strShow, TRUE);

    return;
}

void CTorqueDlg::ResetData()
{
    m_tCollData.nAllCount = 0;
    m_tCollData.nCurCount = 0;
    m_tCollData.dwQuality = 0;
    m_ptComm->fMaxTorq    = 0;

    m_iShowPlus           = 0;
    m_iPriorPlus          = 0;
    m_iPriorCnt           = 0;
    _time64(&m_tStartTime);

    if(NULL != m_ptPortData)
    {
        m_ptPortData->nSaveCount = 0;
        m_ptPortData->nLastPlus = 0;
    }
}

BOOL CTorqueDlg::InsertData(COLLECTTORQUE *ptColl, double torque, double rpm)
{
    UINT     i = 0;

    ASSERT_NULL_R(ptColl, FALSE);

    /* ���ݴ���2500ʱ��ȥ��ǰ��500������ */
    if(ptColl->nAllCount >= COLLECTPOINTS)
    {
        for(i=0; i<ptColl->nAllCount-MAXLINEITEM; i++)
        {
            ptColl->fTorque[i] = ptColl->fTorque[i+MAXLINEITEM];
            ptColl->fRpm[i]    = ptColl->fRpm[i+MAXLINEITEM];
        }
        
        for(; i<ptColl->nAllCount; i++)
        {
            ptColl->fTorque[i] = 0;
            ptColl->fRpm[i]    = 0;
        }
        ptColl->nAllCount -= MAXLINEITEM;
        return FALSE;
    }
    if(ptColl->nCurCount >= MAXLINEITEM)
    {
        m_iPriorCnt += m_tCollData.nCurCount;
        ptColl->nCurCount = 0;
    }

    ptColl->fTorque[ptColl->nAllCount] = torque;
    ptColl->fRpm[ptColl->nAllCount] = rpm;
    ptColl->nAllCount++;
    ptColl->nCurCount++;

    return TRUE;
}

void CTorqueDlg::UpdateTorqueData(double torque, double rpm)
{
    COMP_BL(m_tCollData.nAllCount, 1);
    COMP_BG(m_tCollData.nAllCount, COLLECTPOINTS);

    m_tCollData.fTorque[m_tCollData.nAllCount-1] = torque;
    m_tCollData.fRpm[m_tCollData.nAllCount-1] = rpm;

    return;
}

/* ����ǰŤ��������ӵ�Ť�ؽṹ�ļ��� */
void CTorqueDlg::SaveIntoData(TorqData::Torque *ptPBData)
{
    /* ���ļ���Ť�����Ϊ1 */
    UINT    nTorqNum    = 1;
    UINT    nDataLen    = 0;
    int     i           = 0;
    UINT    nLeng       = 0;    /* ���ݵĳ��� */
    CString strFileName;
    CString strTemp;
    CFile   file;
    __time64_t curTime;
    char    *pcBuff = NULL;
    double  duration;

    ASSERT_NULL(ptPBData);

    _time64(&curTime);
    ptPBData->set_coltime(curTime);
    ptPBData->set_dwseqno(m_nCur);
    ptPBData->set_fmaxtorq(m_ptComm->fMaxTorq);
    ptPBData->set_bshackle(m_iShackle > 0);
    ptPBData->set_dwtorqunit(theApp.m_nTorqUnit);
    duration = _difftime64(curTime, m_tStartTime);
    ptPBData->set_fmakeupdur(_difftime64(curTime, m_tStartTime));
    
    nDataLen = ptPBData->ByteSize();
    ASSERT_ZERO(nDataLen);
    COMP_BGE(nDataLen, MAXPROBUFF);
    pcBuff = new char[nDataLen];
    ASSERT_NULL(pcBuff);
    memset(pcBuff, 0, nDataLen);
    if (!ptPBData->SerializeToArray(pcBuff, nDataLen))
    {
        delete pcBuff;
        return;
    }

    file.Open(theApp.m_strDataFile.c_str(), CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::shareDenyNone,NULL);

    if(file.GetLength() != 0)
    {
        file.Read(&nTorqNum,sizeof(UINT));
        nTorqNum ++;
    }

    /* Ť�ص�ǰ���+1 */
    m_ptComm->dwSeqNo = m_nCur;

    theApp.IncTorqNo();
    //m_nCurTubeNO++;
    m_nTotal = theApp.m_dwTotalTorqNum;

    /* �����ļ�������Ŀ */
    file.SeekToBegin();
    file.Write(&nTorqNum, sizeof(UINT));

    /* ����֮ǰ��¼ */
    //SEEK_TORQUE(((int)nTorqNum-1), nLeng);
    file.SeekToEnd();

    // ���°汾�����ͷ�ļ�
    file.Write(&theApp.m_nPBHead, PBHEADLEN);
    file.Write(&nDataLen, sizeof(UINT));
    file.Write(pcBuff, nDataLen);
    file.Close();

    delete pcBuff;

    m_bCanModLastData = TRUE;
    GetDlgItem(IDC_BTNQUALITY)->EnableWindow(m_bCanModLastData);
    return;
}

void CTorqueDlg::SetShowPara(TorqData::Torque *ptPBData)
{
    WORD    i        = 0;
    TorqData::ShowInfo *pbShow = NULL;
    CString strRunningNO;
    string  strWholeTube;

    ASSERT_NULL(ptPBData);

    strRunningNO.Format("%d", theApp.m_nCurRunningNO);

    for(i=0; i<m_ptShowCfg->wParaNum && i<MAXPARANUM; i++)
    {
        pbShow = ptPBData->add_tshow();
        pbShow->set_strname(m_ptShowCfg->tShow[i].strName);

        switch(i)
        {
            /* ������� */
            case 8:
                strWholeTube = m_ptShowCfg->tShow[i].strValue + "-" + m_strTubeNO2.GetBuffer(0);
                //_itoa_s(m_nCurTubeNO, cTemp, 10, 10);
                pbShow->set_strvalue(strWholeTube);
                break;
            /* �뾮��� */
            case 9:
                if(QUA_RESU_GOOD == ptPBData->dwquality())
                    pbShow->set_strvalue(strRunningNO.GetBuffer());
                break;
            /* �ܼ����� */
            case FIXSHOWBEGIN:
                pbShow->set_strvalue(theApp.GetOEMValue());
                break;
            /* �ܼ���� */
            case FIXSHOWBEGIN+1:
                pbShow->set_strvalue(theApp.GetTubSizeValue());
                break;
            /* ���Ͳ��� */
            case FIXSHOWBEGIN+2:
                pbShow->set_strvalue(theApp.GetThreadMatValue());
                break;
            /* �ӹ���� */
            case FIXSHOWBEGIN+3:
                pbShow->set_strvalue(theApp.GetCouplingValue());
                break;

            default:
                pbShow->set_strvalue(m_ptShowCfg->tShow[i].strValue);
                break;
        }
    }
}

void CTorqueDlg::SetQuality(DWORD dwQuality)
{
    m_tSaveData.set_dwquality(dwQuality);
}

void CTorqueDlg::KillAllTimer()
{
    m_hrtReadPort.KillTimer();
    m_hrtGuard.KillTimer();
    //m_hrtGuardRead.KillTimer();
    m_hrtPortBuff.KillTimer();
    m_hrtSaveData.KillTimer();
    m_hrtSaveDebug.KillTimer();
    m_hrtReadValve.KillTimer();
    //KillTimer(AUTOSAVE_TIMER);
}

/* ��дWORD16���ֽ��� */
void CTorqueDlg::FillWordValue(BYTE *pAddr, int iValue)
{
    ASSERT_NULL(pAddr);
    
    *(pAddr)   = (BYTE)(((iValue)&0xFF00) >> 8);
    *(pAddr+1) = (BYTE)((iValue)&0x00FF);
}

/* ��дMODBUS������ */
void CTorqueDlg::FillReadCommand(int iCommand)
{
    m_ucSndByte[0] = PORT485;   /* �豸�� */
    m_ucSndByte[1] = BUS_READ;  /* ������ ������ */
    FillWordValue(&m_ucSndByte[2], iCommand);
}

void CTorqueDlg::FillWriteCommand(int iCommand)
{
    m_ucSndByte[0] = PORT485;   /* �豸�� */
    m_ucSndByte[1] = BUS_WRITE; /* ������ д���� */
    FillWordValue(&m_ucSndByte[2], iCommand);
}

void CTorqueDlg::OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu)
{
    CCmdUI  state;
    HMENU   hParentMenu;

    ASSERT(pPopupMenu != NULL);
    ASSERT_NULL(pPopupMenu)
    // Check the enabled state of various menu items.

    state.m_pMenu = pPopupMenu;
    ASSERT(state.m_pOther == NULL);
    ASSERT(state.m_pParentMenu == NULL);

    // Determine if menu is popup in top-level menu and set m_pOther to
    // it if so (m_pParentMenu == NULL indicates that it is secondary popup).
    if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
        state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
    else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
    {
        CWnd* pParent = this;
        // Child Windows dont have menus--need to go to the top!
        if (pParent != NULL &&
           (hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
        {
           int nIndexMax = ::GetMenuItemCount(hParentMenu);
           for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
           {
            if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
            {
                // When popup is found, m_pParentMenu is containing menu.
                state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
                break;
            }
           }
        }
    }

    state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
    for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++)
    {
        state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
        if (state.m_nID == 0)
           continue; // Menu separator or invalid cmd - ignore it.

        ASSERT(state.m_pOther == NULL);
        ASSERT(state.m_pMenu != NULL);
        ASSERT_NULL(pPopupMenu)
        if (state.m_nID == (UINT)-1)
        {
            // Possibly a popup menu, route to first item of that popup.
            state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
            if ( state.m_pSubMenu == NULL ||
                (state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
                 state.m_nID == (UINT)-1)
            {
                continue;       // First item of popup cant be routed to.
            }
           state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
        }
        else
        {
            // Normal menu item.
            // Auto enable/disable if frame window has m_bAutoMenuEnable
            // set and command is _not_ a system command.
            state.m_pSubMenu = NULL;
            state.DoUpdate(this, FALSE);
        }

        // Adjust for menu deletions and additions.
        UINT nCount = pPopupMenu->GetMenuItemCount();
        if (nCount < state.m_nIndexMax)
        {
            state.m_nIndex -= (state.m_nIndexMax - nCount);
            while (state.m_nIndex < nCount &&
                   pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
            {
                state.m_nIndex++;
            }
        }
        state.m_nIndexMax = nCount;
    }
} 

void CTorqueDlg::UpdCfgLangChg(UINT nLang)
{
    COMP_BGE(nLang, LANGUAGE_NUM);
    COMP_BE(theApp.m_nLangType, nLang);
    
    theApp.LoadLanguageDll(nLang);
    SetMenu(&m_tMenu[nLang]);
    theApp.m_ptCurShow  = &theApp.m_tShowCfg[nLang];
    m_ptShowCfg         = theApp.m_ptCurShow;
    
    /* ���������仯�����ݼ�¼�ļ����ƺ����������Ҫ�仯 */
    ReGetTorqNo();

    theApp.SaveAppStatus(STATUS_CHGLAN, __FUNCTION__);

    //���ٵ�ǰ����
    theApp.m_pMainWnd = NULL;
    this->DestroyWindow();

    //���´򿪴���
    theApp.ReOpenWindow();
}

void CTorqueDlg::OnLangChn()
{
    UpdCfgLangChg(LANGUAGE_CHINESE);
}

void CTorqueDlg::OnUpdateLangChn(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck((theApp.m_nLangType == LANGUAGE_CHINESE));
}

void CTorqueDlg::OnLangEng()
{
    UpdCfgLangChg(LANGUAGE_ENGLISH);
}

void CTorqueDlg::OnUpdateLangEng(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck((theApp.m_nLangType == LANGUAGE_ENGLISH));
}
/*
void CTorqueDlg::OnLangRus()
{
    UpdCfgLangChg(LANGUAGE_RUSSIAN);
}

void CTorqueDlg::OnUpdateLangRus(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck((theApp.m_nLangType == LANGUAGE_RUSSIAN));
}
*/
void CTorqueDlg::OnVerBigTorq()
{
    CString  strValue;
    
    if(m_bRunStatus)
    {
        strValue.Format(IDS_STRINFRUNNSWITCH);
        m_StatusBar.SetPaneText(0, strValue, TRUE);
        UpdateData(FALSE);
        return;
    }

    theApp.m_bBigTorq = !theApp.m_bBigTorq;
    
    theApp.m_nTorqMulti = 1;
    if(theApp.m_bBigTorq)
        theApp.m_nTorqMulti = 10;

    theApp.ShowMainTitle();

    /*save into ini*/
    strValue.Format("%d",theApp.m_bBigTorq);
    theApp.WriteConfigStr(IDS_STRPNADJUST,IDS_STRPIBIGTORQ,strValue,theApp.m_strParaFile);
}

void CTorqueDlg::OnUpdateVerBigTorq(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck((theApp.m_bBigTorq == TRUE));
}

void CTorqueDlg::UnitChangeTorq()
{
    CString     strValue;
    PARACFG     tCurCfg   = {0};
    double      fRatio  = LBFT2NM;  // 1-->0 : lb.ft --> N.m

    tCurCfg  = theApp.m_tParaCfg;

    /* ���λ & ��λ�ı� */
    if(theApp.m_nTorqUnit == 1) // 0-->1 : N.m --> lb.ft
        fRatio = NM2LBFT;

    tCurCfg.tCtrl.fMaxLimit     = round(fRatio * tCurCfg.tCtrl.fMaxLimit);
    tCurCfg.tCtrl.fUpperLimit   = round(fRatio * tCurCfg.tCtrl.fUpperLimit);
    tCurCfg.tCtrl.fControl      = round(fRatio * tCurCfg.tCtrl.fControl);
    tCurCfg.tCtrl.fOptTorq      = round(fRatio * tCurCfg.tCtrl.fOptTorq);
    tCurCfg.tCtrl.fLowerLimit   = round(fRatio * tCurCfg.tCtrl.fLowerLimit);
    tCurCfg.tCtrl.fSpeedDown    = round(fRatio * tCurCfg.tCtrl.fSpeedDown);
    tCurCfg.tCtrl.fShow         = round(fRatio * tCurCfg.tCtrl.fShow);
    tCurCfg.tCtrl.fBear         = round(fRatio * tCurCfg.tCtrl.fBear);

    tCurCfg.tComm.fLowerTai     = round(fRatio * tCurCfg.tComm.fLowerTai);
    tCurCfg.tComm.fUpperTai     = round(fRatio * tCurCfg.tComm.fUpperTai);
    
    theApp.m_tParaCfg = tCurCfg;
    
    /*save into ini*/
    theApp.WritePara(theApp.m_strParaFile, &theApp.m_tParaCfg);
    strValue.Format("%d",theApp.m_nTorqUnit);
    theApp.WriteConfigStr(IDS_STRPNADJUST,IDS_STRPITORQUNIT,strValue,theApp.m_strParaFile);

    theApp.SaveAppStatus(STATUS_CHGUNIT, __FUNCTION__);

    ResetLineChart();
    Invalidate(TRUE);
    UpdateData(FALSE);
}

void CTorqueDlg::OnVerUnitNm()
{
    JUDGE_RUN_STATUS(IDS_STRINFRUNNSETPARA);

    if(0 == theApp.m_nTorqUnit)
        return;

    theApp.m_nTorqUnit = 0;
    theApp.m_strUnit.Format("N%sm", BIGPOINT);

    UpdateDlgLabel();

    UnitChangeTorq();
}

void CTorqueDlg::OnUpdateVerUnitNm(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck((theApp.m_nTorqUnit == 0));
}

void CTorqueDlg::OnVerUnitLbft()
{
    JUDGE_RUN_STATUS(IDS_STRINFRUNNSETPARA);

    if(1 == theApp.m_nTorqUnit)
        return;

    theApp.m_nTorqUnit = 1;
    theApp.m_strUnit.Format("lb%sft", BIGPOINT);

    UpdateDlgLabel();

    UnitChangeTorq();
}

void CTorqueDlg::OnUpdateVerUnitLbft(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck((theApp.m_nTorqUnit == 1));
}

void CTorqueDlg::OnModpw()
{
    CDlgModPW     dlgModPW;
    dlgModPW.DoModal();
}

void CTorqueDlg::OnValveset()
{
    CDlgValveSet dlgValveSet;

    JUDGE_RUN_STATUS(IDS_STRINFRUNNSETPARA);

    dlgValveSet.m_tempValve = theApp.m_tValveCfg;

    if(IDOK != dlgValveSet.DoModal())
        return;

    /* �Ƚϲ����Ƿ����仯������仯����Ҫ�޸Ĳ���������Ч */
    COMP_BTRUE(CompSysPara((BYTE*)&dlgValveSet.m_tempValve,
                            (BYTE*)&theApp.m_tValveCfg,
                            sizeof(VALVECFG)));

    theApp.m_tValveCfg = dlgValveSet.m_tempValve;
    theApp.WriteValvePara(theApp.m_strParaFile, &theApp.m_tValveCfg);

    theApp.SaveAppStatus(STATUS_SETVALVE, __FUNCTION__);

    SendValveCommand();

    UpdateData(FALSE);
    
}

void CTorqueDlg::OnBnClickedSetippoint()
{
    CString strValue;

    theApp.m_bCheckIP = !theApp.m_bCheckIP;

    /*save into ini*/
    strValue.Format("%d",theApp.m_bCheckIP);
    theApp.WriteConfigStr(IDS_STRPNADJUST,IDS_STRPIIPPOINT,strValue,theApp.m_strParaFile);
}


void CTorqueDlg::OnBnClickedSettoolbuck()
{
    m_bToolBuck = !m_bToolBuck;
}

void CTorqueDlg::OnBnClickedBtnquality()
{
    int         iQuality = 0;
    CString     strRunningNO;
    CDlgRemark  dlgRemark;
    BOOL        bModified = FALSE;
    int         i       = 0;
    WORD        wIPPos  = 0;
    WORD        wSchPos = 0;
    UINT        nShoulder = 0;
    int         iCause = 0;
    TorqData::ShowInfo *ptRunningShow = NULL;

    /* ��鵱ǰ������Ƿ���Ч */
    COMP_BL(m_tSaveData.ftorque_size(), 1);

    iQuality  = dlgRemark.m_iQuality  = m_tSaveData.dwquality() & QUA_RESU_QUALITYBIT;
    iCause    = dlgRemark.m_iCause    = theApp.GetQualityIndex(&m_tSaveData);

    nShoulder = dlgRemark.m_nShoulder = theApp.GetIPTorq(&m_tSaveData, wIPPos, wSchPos);

    //ɱ��λ��ʱ��
    m_hrtGuard.KillTimer();
    //��λʱ�䶨ʱ��ȡ����
    //m_hrtGuardRead.KillTimer();

    if(IDOK != dlgRemark.DoModal())
    {
        GuardTimerOut(0, 0);
        return;
    }
    
    /* �޸Ĺյ�Ť�� */
    if(nShoulder != dlgRemark.m_nShoulder)
    {
        bModified = TRUE;
        theApp.SetIPInfo(&m_tSaveData, dlgRemark.m_nShoulder);
    }

    /* �޸����������� */
    if(iQuality != dlgRemark.m_iQuality || iCause != dlgRemark.m_iCause)
    {
        bModified = TRUE;

        if(dlgRemark.m_iQuality == QUA_RESU_GOOD)
        {
            m_tSaveData.set_dwquality(QUA_RESU_GOOD);
        }
        else
        {
            m_tSaveData.set_dwquality(2 << (dlgRemark.m_iCause-1));
        }

        /* Ť���������Է����仯����Ҫ�޸��뾮��� */
        if(iQuality != dlgRemark.m_iQuality)
        {
            ptRunningShow = m_tSaveData.mutable_tshow(theApp.GetMainWellIndex());
            strRunningNO.Format("%d", theApp.m_nCurRunningNO);
            if(QUA_RESU_GOOD == dlgRemark.m_iQuality)
                ptRunningShow->set_strvalue(strRunningNO.GetBuffer());
            else
                ptRunningShow->set_strvalue(NULLSTR);
        }
    }

    m_strQuality = theApp.GetQualityInfo(&m_tSaveData);

    if(bModified)
    {
        theApp.UpdateHisData(theApp.m_strDataFile.c_str(), -1, &m_tSaveData);
        UpdateData(FALSE);
    }

    GuardTimerOut(0, 0);
}

void CTorqueDlg::OnBnClickedRadiobuckle()
{
    CString strShow;
    CString strValue;
    
    ASSERT_ZERO(m_iShackle);
    if(m_bRunStatus)
    {
        strShow.Format(IDS_STRINFRUNNSWITCH);
        m_StatusBar.SetPaneText(0, strShow, TRUE);
        UpdateData(FALSE);
        return;
    }

    if (IDNO == AfxMessageBox("�Ƿ�Ҫ�л��Ͽ�/ж�۰汾?", MB_YESNO | MB_ICONINFORMATION))
    {
        UpdateData(FALSE);
        return;
    }

    m_iShackle = 0;
    GetDlgItem(IDC_SETIPPOINT)->EnableWindow(TRUE);
    
    theApp.m_bShackle = FALSE;
    /*save into ini*/
    strValue.Format("%d",theApp.m_bShackle);
    theApp.WriteConfigStr(IDS_STRPNADJUST,IDS_STRPISHACKLE,strValue,theApp.m_strParaFile);

    ResetLineChart();
    UpdateData(FALSE);
}


void CTorqueDlg::OnBnClickedRadioshackle()
{
    CString strShow;
    CString strValue;
    
    COMP_BG(m_iShackle, 0);
    if(m_bRunStatus)
    {
        strShow.Format(IDS_STRINFRUNNSWITCH);
        m_StatusBar.SetPaneText(0, strShow, TRUE);
        UpdateData(FALSE);
        return;
    }

    if (IDNO == AfxMessageBox(IDS_STRINFSWITCHVER, MB_YESNO | MB_ICONINFORMATION))
    {
        UpdateData(FALSE);
        return;
    }

    m_iShackle = 1;
    GetDlgItem(IDC_SETIPPOINT)->EnableWindow(FALSE);
    
    theApp.m_bShackle = TRUE;
    /*save into ini*/
    strValue.Format("%d",theApp.m_bShackle);
    theApp.WriteConfigStr(IDS_STRPNADJUST,IDS_STRPISHACKLE,strValue,theApp.m_strParaFile);

    ResetLineChart();
    UpdateData(FALSE);
}

/* �ӳ�1s����������״̬3��Ť�ؼ������ */
void CTorqueDlg::CalcDelayCount()
{
    m_nDelayCount = 1;
    if(theApp.m_nColletTime >= 1000)
        return;

    if(theApp.m_nColletTime < 100)
    {
        m_nDelayCount = 10;
    }

    m_nDelayCount = 1000 / theApp.m_nColletTime;
}

CDialog * CTorqueDlg::GetDlgPoint(CDialog * pDialog, BYTE uDlgKind, UINT iDlgID)
{
    COMP_BNE_R(pDialog, NULL, pDialog);

    switch(uDlgKind)
    {
    case DLG_COLLECTDATA:
        pDialog = new CDlgCollect();
        break;
    case DLG_SEGCALIB:
        pDialog = new CDlgSegCabl();
        break;
    }

    ASSERT_NULL_R(pDialog, NULL);

    //Create success.
    if(pDialog->Create(iDlgID,this))
    {
        pDialog->SetOwner((CWnd*)this);
        return pDialog;
    }

    delete pDialog;
    return NULL;
}

void CTorqueDlg::StartGetValveStatus()
{
    m_hrtReadValve.CreateTimer(this, READVALVE_LEN, HRTReadValve);
    m_iValveBreakCnt = (int)ceil(MAXCOMMBREAKTIME / READVALVE_LEN);
}

void CTorqueDlg::StopGetValveStatus()
{
    m_hrtReadValve.KillTimer();
    m_iValveBreakCnt = (int)ceil(MAXCOMMBREAKTIME / READVALVE_LEN);
}

void CTorqueDlg::OnSegcalib()
{
    //CDlgSegCabl dlgCab;
    
    JUDGE_RUN_STATUS(IDS_STRINFRUNNCALIB);

    COMP_BFALSE(theApp.CheckPassWord());

    theApp.SaveAppStatus(STATUS_CALIB, __FUNCTION__);
/*
    StopGetValveStatus();
    dlgCab.DoModal();

    StartGetValveStatus();
*/
    m_pdlgCalib = (CDlgSegCabl*)GetDlgPoint(m_pdlgCalib,
                                              DLG_SEGCALIB,
                                              IDD_DLGSEGCALIB);
    ASSERT_NULL(m_pdlgCalib);

    StopGetValveStatus();

    m_pdlgCalib->ShowWindow(SW_SHOW);
}
