#pragma once

#include "Torque.h"
#include "ColorRadio.h"

// CDlgXlsStatSet 对话框

class CDlgXlsStatSet : public CDialogEx
{
    DECLARE_DYNAMIC(CDlgXlsStatSet)
    XLSSTATCFG  m_tempStat;
    XLSSTATCFG  *m_ptStat;

public:
    CDlgXlsStatSet(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~CDlgXlsStatSet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DLGXLSSTATSET };
#endif
    CString m_strShowName[MAXPARANUM];
    BOOL m_bSummary[MAXPARANUM];
    CColorRadio m_clSummary[MAXPARANUM];
    BOOL m_bReport[MAXPARANUM];
    CColorRadio m_clReport[MAXPARANUM];
    int m_iOperator;
    CColorRadio m_rdOperator[MAXPARANUM];
    int m_iTally;
    CColorRadio m_rdTally[MAXPARANUM];

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCheckrpt0();
    afx_msg void OnBnClickedCheckrpt1();
    afx_msg void OnBnClickedCheckrpt2();
    afx_msg void OnBnClickedCheckrpt3();
    afx_msg void OnBnClickedCheckrpt4();
    afx_msg void OnBnClickedCheckrpt5();
    afx_msg void OnBnClickedCheckrpt6();
    afx_msg void OnBnClickedCheckrpt7();
    afx_msg void OnBnClickedCheckrpt8();
    afx_msg void OnBnClickedCheckrpt9();
    afx_msg void OnBnClickedCheckrpt10();
    afx_msg void OnBnClickedCheckrpt11();
    afx_msg void OnBnClickedCheckrpt12();
    afx_msg void OnBnClickedCheckrpt13();
    afx_msg void OnBnClickedCheckrpt14();
    afx_msg void OnBnClickedChecksumm0();
    afx_msg void OnBnClickedChecksumm1();
    afx_msg void OnBnClickedChecksumm2();
    afx_msg void OnBnClickedChecksumm3();
    afx_msg void OnBnClickedChecksumm4();
    afx_msg void OnBnClickedChecksumm5();
    afx_msg void OnBnClickedChecksumm6();
    afx_msg void OnBnClickedChecksumm7();
    afx_msg void OnBnClickedChecksumm8();
    afx_msg void OnBnClickedChecksumm9();
    afx_msg void OnBnClickedChecksumm10();
    afx_msg void OnBnClickedChecksumm11();
    afx_msg void OnBnClickedChecksumm12();
    afx_msg void OnBnClickedChecksumm13();
    afx_msg void OnBnClickedChecksumm14();
    afx_msg void OnBnClickedRadiooper0();
    afx_msg void OnBnClickedRadiooper1();
    afx_msg void OnBnClickedRadiooper2();
    afx_msg void OnBnClickedRadiooper3();
    afx_msg void OnBnClickedRadiooper4();
    afx_msg void OnBnClickedRadiooper5();
    afx_msg void OnBnClickedRadiooper6();
    afx_msg void OnBnClickedRadiooper7();
    afx_msg void OnBnClickedRadiooper8();
    afx_msg void OnBnClickedRadiooper9();
    afx_msg void OnBnClickedRadiooper10();
    afx_msg void OnBnClickedRadiooper11();
    afx_msg void OnBnClickedRadiooper12();
    afx_msg void OnBnClickedRadiooper13();
    afx_msg void OnBnClickedRadiooper14();
    afx_msg void OnBnClickedRadiotally0();
    afx_msg void OnBnClickedRadiotally1();
    afx_msg void OnBnClickedRadiotally2();
    afx_msg void OnBnClickedRadiotally3();
    afx_msg void OnBnClickedRadiotally4();
    afx_msg void OnBnClickedRadiotally5();
    afx_msg void OnBnClickedRadiotally6();
    afx_msg void OnBnClickedRadiotally7();
    afx_msg void OnBnClickedRadiotally8();
    afx_msg void OnBnClickedRadiotally9();
    afx_msg void OnBnClickedRadiotally10();
    afx_msg void OnBnClickedRadiotally11();
    afx_msg void OnBnClickedRadiotally12();
    afx_msg void OnBnClickedRadiotally13();
    afx_msg void OnBnClickedRadiotally14();
    DECLARE_MESSAGE_MAP()

private:
    int         m_iShowNum;
    COLORREF    m_clrNormal;
    COLORREF    m_clrChanged;
    int         m_iPriorOperID;
    int         m_iPriorTallyID;

    void        JudgeSummaryCheck(int iCtrlIdx);
    void        JudgeReportCheck(int iCtrlIdx);
    void        JudgeOperRadio(int iCtrlIdx);
    void        JudgeTallyRadio(int iCtrlIdx);
};
