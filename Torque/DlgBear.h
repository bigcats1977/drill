#if !defined(AFX_DLGBEAR_H__7C0D6A94_0899_490D_B098_79EB807A54FD__INCLUDED_)
#define AFX_DLGBEAR_H__7C0D6A94_0899_490D_B098_79EB807A54FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBear.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBear dialog

class CDlgBear : public CDialog
{
// Construction
public:
    CDlgBear(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CDlgBear)
    enum { IDD = IDD_DLGBEAR };
    BOOL        m_bBigTorq;
    BOOL        m_bShackle;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgBear)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgBear)
    virtual BOOL OnInitDialog();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnSelchangeCombobear();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void SetBear(BOOL bBear);
public:
    afx_msg void OnBnClickedCheckbigtorq();
    afx_msg void OnBnClickedCheckshackle();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBEAR_H__7C0D6A94_0899_490D_B098_79EB807A54FD__INCLUDED_)
