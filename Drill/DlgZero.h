#if !defined(AFX_DLGZERO_H__69BC7DA3_3D76_4C8F_A571_FE2420403868__INCLUDED_)
#define AFX_DLGZERO_H__69BC7DA3_3D76_4C8F_A571_FE2420403868__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgZero.h : header file
//
#include "BtnSt.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgZero dialog

class CDlgZero : public CDialog
{
    // Construction
public:
    CDlgZero(CWnd* pParent = NULL);   // standard constructor

    double  m_fZero;
    // Dialog Data
        //{{AFX_DATA(CDlgZero)
    enum { IDD = IDD_DLGZERO };
    CButtonST   m_ZeroOK;
    CButtonST   m_ZeroCancel;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgZero)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgZero)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGZERO_H__69BC7DA3_3D76_4C8F_A571_FE2420403868__INCLUDED_)
