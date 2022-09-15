#if !defined(AFX_DLGAPI_H__8D4795FE_CECF_4E06_B776_B3FF48B3A733__INCLUDED_)
#define AFX_DLGAPI_H__8D4795FE_CECF_4E06_B776_B3FF48B3A733__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAPI.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAPI dialog
#include "GridCtrl.h"
//#include "OwnWnd.h"

class CDlgAPI : public CDialog
{
// Construction
public:
    CDlgAPI(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CDlgAPI)
    enum { IDD = IDD_DLGAPI };
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgAPI)
    public:
    virtual BOOL DestroyWindow();
    void  FreeAPIString();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:


    // Generated message map functions
    //{{AFX_MSG(CDlgAPI)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    CGridCtrl   *m_pGridCtrl;
    //COwnWnd     *m_ownWnd;

private:
    void SetGridRowCol();
    void SetGridSecondRow();
    void FillItemData();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAPI_H__8D4795FE_CECF_4E06_B776_B3FF48B3A733__INCLUDED_)
