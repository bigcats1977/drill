#pragma once
#include "afxwin.h"


// CDlgHisSumup dialog

class CDlgHisSumup : public CPropertyPage
{
    DECLARE_DYNAMIC(CDlgHisSumup)

public:
    CDlgHisSumup();
    virtual ~CDlgHisSumup();
    virtual BOOL OnSetActive();
    virtual BOOL OnInitDialog();

    // Dialog Data
    enum { IDD = IDD_DLGHISSUMUP };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnmodifysum();
    afx_msg void OnBnClickedBtnsavesum();
    afx_msg void OnBnClickedBtnmodfont();

    CRichEditCtrl m_richSum;

private:
    string  m_strSumFile;
    bool    m_bReadOnly;
};
