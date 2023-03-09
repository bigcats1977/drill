// DataMod.h : main header file for the DATAMOD application
//

#if !defined(AFX_DATAMOD_H__AF538F10_38AA_4A1B_837F_A96FE0F25194__INCLUDED_)
#define AFX_DATAMOD_H__AF538F10_38AA_4A1B_837F_A96FE0F25194__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
//#include "CoolControlsManager.h"

/////////////////////////////////////////////////////////////////////////////
// CDataModApp:
// See DataMod.cpp for the implementation of this class
//

#include "DlgHisGrp.h"

class CDataModApp : public CWinApp
{
public:
    CDataModApp();
    string  GetQualityInfo(TorqData::Torque* ptTorq);
    int     GetQualityIndex(TorqData::Torque* ptTorq);
    double  GetCir(TorqData::Torque* ptTorq, UINT nType = TYPE_TOTAL);
    DWORD   GetQuality(TorqData::Torque* ptTorq);
    DWORD   JudgeQuality(TorqData::Torque* ptTorq, int iShackle = 0);
    BOOL    ReadHisTorqFromFile(string strDataName, TORQUEDATA* pAllData = NULL);
    CString GetTorqCollTime(TorqData::Torque* ptTorq, bool bBreakout = false);
    void    UpdateTorqRpm(TorqData::Torque* ptTorq, int index, double fTorq, double fRpm);
    void    UpdateDelplus(TorqData::Torque* ptTorq, int index, UINT nDelplus);
    void    ClearReadTorq(TORQUEDATA* pAllData = NULL);
    BOOL    CheckPassWord();
    string  LoadstringFromRes(unsigned string_ID);
    string  LoadstringFromRes(unsigned string_ID, int val);
    string  LoadstringFromRes(unsigned string_ID, double val);
    string  LoadstringFromRes(unsigned string_ID, string val);
    /* 为时间版本减少差异，获取横轴坐标的参数，包括下限/上限/控制/最大4个函数 */
    double  GetMaxCir(TorqData::Torque* ptTorq);
    double  GetCtrlCir(TorqData::Torque* ptTorq);
    double  GetUpperCir(TorqData::Torque* ptTorq);
    double  GetLowerCir(TorqData::Torque* ptTorq);
    double  GetOptTorq(TorqData::Torque* ptTorq);
    UINT    SerialTorq(TorqData::Torque* ptTorq);
    int     SeekTorque(CFile& file, int iDataNum);
    int     SeekPBDataPos(CFile& file, int iCurPos);
    int     SeekFileLen(CFile& file);
    void    UpdateHisData(CString filename, int iDataPlace, TorqData::Torque* ptTorq);

    int     GetMainIndexfromData(TorqData::Torque* ptTorq);

    CString GetTorqShowName(TorqData::Torque* ptTorq, int iIndex);
    CString GetTorqShowValue(TorqData::Torque* ptTorq, int iIndex);
    DRAWTORQDATA* GetDrawDataFromTorq(TorqData::Torque* ptTorq, UINT nMulti = 1, UINT nType = 3);
    bool HaveMakeUP(TorqData::Torque* ptTorq);
    bool HaveBreakout(TorqData::Torque* ptTorq);
    bool HaveTallyNO(TorqData::Torque* ptTorq);

    PARACFG         m_tParaCfg;
    CDlgHisGrp* m_pdlgOpt;
    UINT            m_nPBHead;

    BYTE            m_ucDPILevel;
    UINT            m_nScreenX;
    UINT            m_nScreenY;
    CFont           m_tLineTextFont;
    CFont           m_tRuleVFont;
    CFont           m_tRuleHFont;
    CFont           m_tPntTextFont;
    //string          m_strReadFile;
    char            m_cProtoBuf[MAXPROBUFF];
    DRAWTORQDATA    m_tCurDrawTorq;
    DRAWTORQDATA    m_tCurZoomTorq;
    CString         m_strTallyName;


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTorqueApp)
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CDataModApp)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    BOOL GetTorqDataFromFile(string strDataName, TORQUEDATA* pAllData);
    BOOL ReCalTallyNO(string strDataName, TORQUEDATA* pAllData);
    void SaveAllData(string strDataName, TORQUEDATA* pAllData);
    BOOL JudgeTranslate(TorqData::Torque* ptTorq);
    bool GetMakeupDrawData(TorqData::Torque* ptOrg, DRAWTORQDATA* ptDraw, UINT nMulti);
    bool GetBreakoutDrawData(TorqData::Torque* ptOrg, DRAWTORQDATA* ptDraw, UINT nMulti);
};

extern CDataModApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAMOD_H__AF538F10_38AA_4A1B_837F_A96FE0F25194__INCLUDED_)

