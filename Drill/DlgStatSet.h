#pragma once
//#include "afxwin.h"


// CDlgStatSet dialog

class CDlgStatSet : public CDialogEx
{
    DECLARE_DYNAMIC(CDlgStatSet)

public:
    CDlgStatSet(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDlgStatSet();

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DLGSTATPARA };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    afx_msg void OnEnKillfocusEdctrlmin();
    afx_msg void OnEnKillfocusEdctrlrang1();
    afx_msg void OnEnKillfocusEdctrlmax();
    //afx_msg void OnEnKillfocusEddeltamin();
    //afx_msg void OnEnKillfocusEddeltarang1();
    //afx_msg void OnEnKillfocusEddeltamax();
    //afx_msg void OnEnKillfocusEdshouldmin();
    //afx_msg void OnEnKillfocusEdshouldrang1();
    //afx_msg void OnEnKillfocusEdshouldmax();

    DECLARE_MESSAGE_MAP()
public:
    STATCFG m_tempStat;
    double  m_fCtrlZero;
    double  m_fCtrlMax;
    double  m_fCtrlRange[STATRANGENUM];
    //double  m_fShouldZero;
    //double  m_fShouldMax;
    //double  m_fShouldRange[STATRANGENUM];
    //double  m_fDeltaZero;
    //double  m_fDeltaMax;
    //double  m_fDeltaRange[STATRANGENUM];
    UINT    m_nMoveLeft;
    UINT    m_nMoveRange;

private:
    void InitCtrl();
    //void InitShould();
    //void InitDelta();
};
