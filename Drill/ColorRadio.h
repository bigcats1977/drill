#pragma once
#include <afxwin.h>
class CColorRadio : public CButton
{
public:
    CColorRadio();
    ~CColorRadio();

    COLORREF m_ForeColor;
    COLORREF m_BackColor;
    CBrush   m_BkBrush;

protected:
    //{{AFX_MSG(CMyComboBox)
    //afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

public:
    void SetForeColor(COLORREF color);
    void SetBkColor(COLORREF color);
};

