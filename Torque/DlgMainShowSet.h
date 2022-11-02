#pragma once
#include "afxwin.h"

#include "ColorComboBox.h"
//#include "TubeCfg.h"

// CDlgMainShowSet dialog

class CDlgMainShowSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMainShowSet)

public:
	CDlgMainShowSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMainShowSet();
    SHOWCFG     *m_ptShow;
    PARACFG     m_tempCfg;
    //CString     m_strTubeNO2;       /* ¹ÜÌå±àºÅ */
    //UINT        m_nRunningNO;       /* Èë¾®ÐòºÅ */
    //TUBECFG   m_tCurTub;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGMAINSHOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    //afx_msg void OnCbnKillfocusCbmainpara0();
    afx_msg void OnCbnKillfocusCbmainpara1();
    afx_msg void OnCbnKillfocusCbmainpara2();
    afx_msg void OnCbnKillfocusCbmainpara3();
    afx_msg void OnCbnKillfocusCbmainpara4();
    afx_msg void OnCbnKillfocusCbmainpara5();
    afx_msg void OnCbnKillfocusCbmainpara6();
    afx_msg void OnCbnKillfocusCbmainpara7();
    afx_msg void OnCbnKillfocusCbmainpara8();
    /*afx_msg void OnBnClickedCkmainfixed();
    afx_msg void OnCbnKillfocusCbmfixpara0();
    afx_msg void OnCbnKillfocusCbmfixpara1();
    afx_msg void OnCbnKillfocusCbmfixpara2();
    afx_msg void OnCbnKillfocusCbmfixpara3();
    afx_msg void OnCbnKillfocusCbmfixpara4();
    afx_msg void OnCbnSelchangeCbmfixpara0();
    afx_msg void OnCbnSelchangeCbmfixpara1();
    afx_msg void OnCbnSelchangeCbmfixpara2();
    afx_msg void OnCbnSelchangeCbmfixpara3();
    afx_msg void OnCbnSelchangeCbmfixpara4();*/

    CString         m_strMainShow[MAXMAINPARA];
    CColorComboBox  m_cbMainOption[MAXMAINPARA];
    CString         m_strMainOption[MAXMAINPARA];

    /*CColorComboBox  m_cbFixTubingOpt[MAXTUBECFGNUM];
    CString         m_strFixTubingOpt[MAXTUBECFGNUM];
    BOOL            m_bFixedTube;*/

private:
    void InitMainShowPara();
    BOOL GetParaValue();
    //void CheckShowOption(PARACFG* ptCfg);
    void JudgeMainShowParaChanged(UINT nIndex, CString strContent);
    /*void InitFixShowPara();
    void ShowTubingCtrl();
    void ChangeTubingShowBox(UINT nIndex);
    void JudgeFixShowParaChanged(UINT nIndex, TUBECFG* ptTubing);*/

    COLORREF    m_clrNormal;
    COLORREF    m_clrChanged;
    UINT        m_nCurLang;
};
