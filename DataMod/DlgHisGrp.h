#if !defined(AFX_DLGHISGRP_H__33A0139A_E4B3_4505_926C_4B720232532E__INCLUDED_)
#define AFX_DLGHISGRP_H__33A0139A_E4B3_4505_926C_4B720232532E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHisGrp.h : header file
//
#include "LineChartCtrlEx.h"
#include "LineChartCtrl.h"
#include "Ruler.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgHisGrp dialog

class CDlgHisGrp : public CDialog
{
    // Construction
public:
    CDlgHisGrp(CWnd* pParent = NULL);   // standard constructor

    void    ShowCurData(bool bNew = true);
    void    ResetHisLineByCurData();
    void    ResetHisLineByCfg(PARACFG* ptCfg);
    void    OnPrint(CDC* pDC, CPrintInfo* pInfo);

    // Dialog Data
        //{{AFX_DATA(CDlgHisGrp)
    enum { IDD = IDD_DLGHISGRP };
    CRuler      m_xHisAxis1;
    CRuler      m_yHisAxis1;
    CRuler      m_xHisAxis2;
    CRuler      m_yHisAxis2;
    CString     m_strMemo;
    CString     m_strControl;
    CString     m_strBOTorq;
    CString     m_strLowCir;
    CString     m_strQuality;
    CString     m_strTime;
    CString     m_strBOTime;
    CString     m_strNo;
    CString     m_strCir;
    CString     m_strBOCir;
    BOOL        m_bToolBuck;
    //BOOL        m_bBreakOut;
    //CString     m_strTorqType;
    CString     m_strLBG1;
    CString     m_strLBG10;
    CString     m_strLBG16;
    CString     m_strHisShowName[MAXPARANUM];
    CString     m_strHisShowValue[MAXPARANUM];
    CString     m_strOutJoint;      //  接头外径
    CString     m_strOutWellNO;     //  起出序号
    int         m_iSingleSTD;       // 单根/立柱 0/1
    int         m_iGrpType;         // 显示图形类型 0：全部;1:上扣;2:卸扣
    //}}AFX_DATA
public:
    CLineChartCtrlEx    m_wndLineHis;
    CLineChartCtrl      m_wndRpmHis;
    BOOL        m_bCanMod;
    bool        m_bSencond;
    TORQUEDATA* m_ptTorData;

    void SetModified();

    // Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CDlgHisGrp)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CDlgHisGrp)
    virtual BOOL OnInitDialog();
    afx_msg void OnBtnpri();
    afx_msg void OnBtnnext();
    afx_msg void OnBtnmod();
    afx_msg void OnBtnmodpara();
    afx_msg void OnBtnremark();
    afx_msg void OnBnClickedChecktoolbuck();
    afx_msg LRESULT SelPosChange(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void    DrawCurTorque();
    BOOL    CheckCurData(UINT* pnCur, UINT nMax);
    //void    CheckCurSplit();
    void    UpdateDlgLabel(UINT nUnit);
    BOOL    JudgeValidPosition(int iPos);
    //BOOL    GetCirRange(double* fMin, double* fMax);
    void    EnableCtrlforMod();
    void    UpdateTallyNO();
    void    CheckGrpType();
    __time64_t GetTimeFromStr(CString strTime);

    /* 清空EDIT控件信息 */
    void    EmptyEdit();
    /* 设置EDIT控件内容 */
    void    SetCurEdit();
    void    SaveCurData();
    BOOL    CheckCursor(UINT* pnCur, UINT nMax);

    BOOL    m_bModified;
    UINT    m_nIPTorq;
    UINT    m_nBeginPos;
    //SPLITPOINT  m_tCurSplit;
    DRAWTORQDATA* m_ptCurDraw;
    TorqData::Torque* m_ptCurTorq;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHISGRP_H__33A0139A_E4B3_4505_926C_4B720232532E__INCLUDED_)
