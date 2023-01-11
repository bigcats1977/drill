#if !defined(AFX_MYSHEET_H__EB7DFC56_D06D_49F3_B809_965B0BEA82FC__INCLUDED_)
#define AFX_MYSHEET_H__EB7DFC56_D06D_49F3_B809_965B0BEA82FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySheet

class CMySheet : public CPropertySheet
{
    DECLARE_DYNAMIC(CMySheet)

    // Construction
public:
    CMySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
    CMySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

    // Attributes
public:

    // Operations
public:

    // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CMySheet)
public:
    virtual BOOL OnInitDialog();
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CMySheet();

    // Generated message map functions
protected:
    //{{AFX_MSG(CMySheet)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    CImageList  m_ImageList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSHEET_H__EB7DFC56_D06D_49F3_B809_965B0BEA82FC__INCLUDED_)
