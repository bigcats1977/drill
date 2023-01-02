#if !defined(AFX_DLGSCM_H__A62E0D2C_49A3_491F_A926_7B9218373399__INCLUDED_)
#define AFX_DLGSCM_H__A62E0D2C_49A3_491F_A926_7B9218373399__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgScm.h : header file
//
#include "BtnST.h"
#include "TorqueDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgScm dialog

class CDlgScm : public CDialog
{
    // Construction
public:
    CDlgScm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CDlgScm)
    enum { IDD = IDD_DLGSCM };
    CButtonST   m_btnTime;
    CButtonST   m_btnScmCancel;
    CButtonST   m_btnQipi;
    CButtonST   m_btnInitial;
    CButtonST   m_btnMulti;
    double      m_fMulti;
    double      m_fControl;
    double      m_fShow;
    double      m_fSpeed;
    /*double      m_fLower;
    double      m_fUpper;*/
    CTime       m_tDate;
    CTime       m_tTime;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgScm)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CDlgScm)
    afx_msg void OnScminitial();
    afx_msg void OnScmmulti();
    afx_msg void OnScmqipi();
    virtual BOOL OnInitDialog();
    afx_msg void OnScmtime();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    CTorqueDlg* m_pParentDlg;
    PARACFG* m_ptParaCfg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSCM_H__A62E0D2C_49A3_491F_A926_7B9218373399__INCLUDED_)
