#pragma once


// CDlgValveSet dialog

class CDlgValveSet : public CDialogEx
{
    DECLARE_DYNAMIC(CDlgValveSet)

public:
    CDlgValveSet(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDlgValveSet();

    VALVECFG    m_tempValve;

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DLGVALVESET };
#endif
    BYTE m_ucValve1TorqInit;
    BYTE m_ucValve1TorqDece1;   /* 比例1~100 1:初始值; 2:减速阶段1; 3:减速阶段2; */
    BYTE m_ucValve1TorqDece2;
    BYTE m_ucValve1PressInit;
    BYTE m_ucValve1PressDece1;
    BYTE m_ucValve1PressDece2;
    BYTE m_ucValve2TorqInit;
    BYTE m_ucValve2TorqDece1;
    BYTE m_ucValve2TorqDece2;
    BYTE m_ucValve2FlowInit;
    BYTE m_ucValve2FlowDece1;
    BYTE m_ucValve2FlowDece2;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnvalve();
    virtual BOOL OnInitDialog();
};
