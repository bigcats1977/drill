#pragma once


// CDlgModPW dialog

class CDlgModPW : public CDialogEx
{
    DECLARE_DYNAMIC(CDlgModPW)

public:
    CDlgModPW(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDlgModPW();

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DLGMODPW };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    CString m_strOldPW;
    CString m_strNewPW;
    afx_msg void OnBnClickedBtnmodpw();
};
