#if !defined(AFX_COLORCOMBOBOX_H__1555680E_4B5D_4166_97F6_F497FFB551E5__INCLUDED_)
#define AFX_COLORCOMBOBOX_H__1555680E_4B5D_4166_97F6_F497FFB551E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorComboBox window

class CColorComboBox : public CComboBox
{
// Construction
public:
    CColorComboBox();

    COLORREF m_ColorText,m_ColorBk;
    CBrush m_Brush;

    //设置字体大小
    void SetFontSize(int iSize,CFont* font);
// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyComboBox)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CColorComboBox();

    // Generated message map functions
protected:
    //{{AFX_MSG(CMyComboBox)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORCOMBOBOX_H__1555680E_4B5D_4166_97F6_F497FFB551E5__INCLUDED_)
