#if !defined(AFX_DLGCOLLECT_H__BB238C9D_DE6F_4046_97A4_533458D5AB16__INCLUDED_)
#define AFX_DLGCOLLECT_H__BB238C9D_DE6F_4046_97A4_533458D5AB16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCollect.h : header file
//


#include "Mylistctrl.h"

#include "BtnST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgCollect dialog

class CDlgCollect : public CDialog
{
// Construction
public:
    void    ShowCollHisData();
    void    ResetCollHisData();
    CDlgCollect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CDlgCollect)
    enum { IDD = IDD_DLGCOLLECT };
    CButtonST   m_btnCollect;
    CButtonST   m_btnCollCancel;
    CButtonST   m_btnXls;
    CMylistctrl m_listData;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgCollect)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg LRESULT ProcessCollData(WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgCollect)
    virtual BOOL OnInitDialog();
    afx_msg void OnBtnxlssave();
    afx_msg void OnBtncollect();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedCancel();
    afx_msg void OnDestroy();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    BOOL    CheckDataValid(int &iCollNum);
    void    ReadCollData(int iCollNum);
    void    CheckErrorTimes();

    HISDATA m_tHisData;
    UINT    m_nErrorNum;
#define     NODATA      0
#define     VALIDDATA   1
#define     LASTDATA    2
#define     INVALIDDATA 3
    UINT    m_nCollectStatus;   /* 收集数据的状态 */
    UINT    m_nCollCmdType[4];
    CString m_strLastError;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOLLECT_H__BB238C9D_DE6F_4046_97A4_533458D5AB16__INCLUDED_)
