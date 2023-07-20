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
    UINT			m_nFTPPort;
    CString			m_strFTPAddr;
    CString			m_strUsername;
    CString			m_strPassword;
    CString			m_strTargetPath;
    CIPAddressCtrl	m_hostIP;
    UINT			m_nTCPPort;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedTcpstart();
    afx_msg void OnBnClickedTcpstop();
    //afx_msg void OnBnClickedTcptest();

    DECLARE_MESSAGE_MAP()

private:
    bool GetParaValue(SERVERCFG* ptCfg);
    void SetParaValue(SERVERCFG* ptCfg);
    void GetIPAddr();
    void EnableTCPButton(bool Started);

    COLORREF    m_clrNormal;
    COLORREF    m_clrChanged;
};
