#pragma once

#include "Torque.h"
//#include "ColorRadio.h"
#include "ColorComboBox.h"

// CDlgXlsStatSet 对话框

class CDlgXlsStatSet : public CDialogEx
{
    DECLARE_DYNAMIC(CDlgXlsStatSet)
    //XLSSTATCFG  m_tempStat;
    XLSSTATCFG  *m_ptStat;

public:
    CDlgXlsStatSet(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~CDlgXlsStatSet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DLGXLSSTATSET };
#endif
    CColorComboBox  m_cbGenParaName[STATPARA_GENNUM];
    int m_iGenParaIndex[STATPARA_GENNUM];
    CColorComboBox  m_cbJobParaName[STATPARA_JOBNUM];
    int m_iJobParaIndex[STATPARA_JOBNUM];
    CColorComboBox  m_cbInfoParaName[STATPARA_INFONUM];
    int m_iInfoParaIndex[STATPARA_INFONUM];

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    afx_msg void OnCbnKillfocusScbgenname1();
    afx_msg void OnCbnKillfocusScbgenname2();
    afx_msg void OnCbnKillfocusScbgenname3();
    afx_msg void OnCbnKillfocusScbgenname4();
    afx_msg void OnCbnKillfocusScbgenname5();
    afx_msg void OnCbnKillfocusScbjobname1();
    afx_msg void OnCbnKillfocusScbjobname2();
    afx_msg void OnCbnKillfocusScbjobname3();
    afx_msg void OnCbnKillfocusScbjobname4();
    afx_msg void OnCbnKillfocusScbjobname5();
    afx_msg void OnCbnKillfocusScbjobname6();
    afx_msg void OnCbnKillfocusScbinfoname1();
    afx_msg void OnCbnKillfocusScbinfoname2();
    DECLARE_MESSAGE_MAP()

private:
    COLORREF    m_clrNormal;
    COLORREF    m_clrChanged;

    void        InitCBContent(CComboBox* ptCB, UINT index);
    void        JudgeGenParaChanged(UINT no);
    void        JudgeJobParaChanged(UINT no);
    void        JudgeInfoParaChanged(UINT no);
};
