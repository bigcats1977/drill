#pragma once

#include "Mylistctrl.h"
#include "excel.h"

// CDlgSegCabl dialog


class CDlgSegCabl : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSegCabl)

public:
	CDlgSegCabl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSegCabl();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGSEGCALIB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
    UINT        m_nCalibTorq;
    UINT        m_nLowTorq;
    UINT        m_nHighTorq;
    UINT        m_nSCMTorq;
    int         m_iType;
    CComboBox   m_cbSegment;
    CMylistctrl m_lsCalibInfo;

    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedBtncalibbegin();
    afx_msg void OnBnClickedBtncalibfin();
    afx_msg void OnBnClickedBtncalibgo();
    afx_msg void OnBnClickedBtncalibexp();
    afx_msg void OnBnClickedBtncalibread();
    afx_msg void OnBnClickedBtncalibimp();
    afx_msg LRESULT ProcReadCalib(WPARAM wParam, LPARAM lParam);
    afx_msg void OnDestroy();

private:
    BYTE        m_ucCurSeg;
    CALIBINFO   m_tCalibInfo[MAXSEGNUM];
    Excel       m_impExc;

    BOOL        CheckTorqValue();
    BOOL        CheckReadCalib(CALIBCTRL *ptRead, CString &strError);
    void        GetCalibInfo();
    void        UpdateCalibList();
    void        EnableCtrl(BOOL bBegin);
    void        InitTestData();
    BOOL        GetCalibInfoFromExcel(CString strName);
    BOOL        CheckCalib(CALIBINFO *ptInfo);
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
