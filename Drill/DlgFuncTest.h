#if !defined(AFX_DLGFUNCTEST_H__78B754D2_B2F0_48C4_83BB_E23831236258__INCLUDED_)
#define AFX_DLGFUNCTEST_H__78B754D2_B2F0_48C4_83BB_E23831236258__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFuncTest.h : header file
//

#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFuncTest dialog

class CDlgFuncTest : public CDialog
{
    // Construction
public:
    CDlgFuncTest(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CDlgFuncTest)
    enum { IDD = IDD_DLGTEST };
    CButtonST  m_btnOK;
    CButtonST  m_btnUnloadE;
    CButtonST  m_btnUnloadB;
    CButtonST  m_btnSpeedE;
    CButtonST  m_btnSpeedB;
    CButtonST  m_btnAlarmE;
    CButtonST  m_btnAlarmB;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgFuncTest)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgFuncTest)
    virtual BOOL OnInitDialog();
    afx_msg void OnBtnspeedbegin();
    afx_msg void OnBtnspeedend();
    afx_msg void OnBtnunloadbegin();
    afx_msg void OnBtnunloadend();
    afx_msg void OnBtnalarmbegin();
    afx_msg void OnBtnalarmend();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFUNCTEST_H__78B754D2_B2F0_48C4_83BB_E23831236258__INCLUDED_)