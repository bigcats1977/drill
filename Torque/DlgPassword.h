#pragma once


// CDlgPassword dialog

class CDlgPassword : public CDialog
{
    DECLARE_DYNAMIC(CDlgPassword)

public:
    CDlgPassword(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDlgPassword();

    // Dialog Data
    enum { IDD = IDD_DLGPASSWORD };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    CString m_strPassword;
};
