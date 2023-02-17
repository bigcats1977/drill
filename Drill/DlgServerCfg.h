#pragma once


// CDlgServerCfg dialog

class CDlgServerCfg : public CDialog
{
    DECLARE_DYNAMIC(CDlgServerCfg)

public:
    CDlgServerCfg(CWnd* pParent = nullptr);   // standard constructor
    virtual ~CDlgServerCfg();

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DLGSERVCFG };
#endif
    CIPAddressCtrl	m_IPAddr;
    UINT			m_nFTPPort;
    CString			m_strUsername;
    CString			m_strPassword;
    CString			m_strTargetPath;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    DECLARE_MESSAGE_MAP()

private:
    bool GetParaValue(SERVERCFG* ptCfg);
    void SetParaValue(SERVERCFG* ptCfg);

    COLORREF    m_clrNormal;
    COLORREF    m_clrChanged;
};
