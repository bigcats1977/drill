#include "afxwin.h"
#if !defined(AFX_DLGREMARK_H__374A35AF_9988_4B18_A5CE_297BCFC26348__INCLUDED_)
#define AFX_DLGREMARK_H__374A35AF_9988_4B18_A5CE_297BCFC26348__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRemark.h : header file
//

#define MAX_USE_CAUSE           14
/////////////////////////////////////////////////////////////////////////////
// CDlgRemark dialog

class CDlgRemark : public CDialog
{
// Construction
public:
    CDlgRemark(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CDlgRemark)
    enum { IDD = IDD_DLGREMARK };
    int     m_iQuality;
    CComboBox m_cbRemark;
    UINT    m_nShoulder;
    //}}AFX_DATA
    int     m_iCause;
    int     m_lsCause[MAX_USE_CAUSE];


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgRemark)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgRemark)
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();

private:
    void SetRejCause();
public:
    afx_msg void OnBnClickedRadionoquality();
    afx_msg void OnBnClickedRadioquality();
    afx_msg void OnCbnSelchangeComboremark();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGREMARK_H__374A35AF_9988_4B18_A5CE_297BCFC26348__INCLUDED_)
