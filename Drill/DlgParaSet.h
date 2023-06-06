#if !defined(AFX_DLGPARASET_H__4ED7C23C_5F6E_4A6E_98BA_B7E3D9ACFE62__INCLUDED_)
#define AFX_DLGPARASET_H__4ED7C23C_5F6E_4A6E_98BA_B7E3D9ACFE62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgParaSet.h : header file
//

#include "afxwin.h"
#include "ColorComboBox.h"
#include "NumEdit.h"
#include "ColorRadio.h"
//#include "TubeCfg.h"

#define  MAXCOLUMNS     3
/////////////////////////////////////////////////////////////////////////////
// CDlgParaSet dialog

class CDlgParaSet : public CDialog
{

    // Construction
public:
    CDlgParaSet(CWnd* pParent = NULL);   // standard constructor

    bool        m_bParaChg;
    PARACFG     m_tempCfg;
    SHOWCFG     m_tempShow;

    // Dialog Data
        //{{AFX_DATA(CDlgParaSet)
    enum { IDD = IDD_DLGPARASET };
    //double      m_fControlCir;
    double      m_fControl;
    double      m_fOptTorq;
    double      m_fLowerCir;
    //double      m_fLowerLimit;
    double      m_fMaxCir;
    double      m_fMaxLimit;
    double      m_fMaxRPM;
    double      m_fShow;
    //double      m_fSpeedDown;
    double      m_fUpperCir;
    //double      m_fUpperLimit;
    CString     m_strParaAlias;
    CColorComboBox m_cbAlias;
    //double      m_fLowerTai;
    //double      m_fUpperTai;
    //double      m_fMinShlSlope;
    //double      m_fMaxDeltaCir;
    //double      m_fMinDeltaCir;
    CString     m_strMemo;
    CString     m_strLBM10;
    CString     m_strSetShowName[MAXPARANUM];
    CString     m_strSetShowOption[MAXPARANUM];
    CColorComboBox  m_cbSetShowOption[MAXPARANUM];
    //CNumEdit    m_neControlCir;
    CNumEdit    m_neOptTorq;
    CNumEdit    m_neControl;
    CNumEdit    m_neLowerCir;
    //CNumEdit    m_neLowerLimit;
    CNumEdit    m_neMaxCir;
    CNumEdit    m_neMaxLimit;
    CNumEdit    m_neMaxRPM;
    CNumEdit    m_neShow;
    //CNumEdit    m_neSpeedDown;
    CNumEdit    m_neUpperCir;
    //CNumEdit    m_neUpperLimit;
    //CNumEdit    m_neLowerTai;
    //CNumEdit    m_neUpperTai;
    //CNumEdit    m_neMinShlSlope;
    //CNumEdit    m_neMaxDeltaCir;
    //CNumEdit    m_neMinDeltaCir;
    int           m_iSingleSTD;       //单根/两根/三根 0/1/2
    CColorRadio   m_rdSingle[MAXCOLUMNS];
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgParaSet)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgParaSet)
    virtual BOOL OnInitDialog();
    afx_msg void OnParasave();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnCbnKillfocusCbpara01();
    afx_msg void OnCbnKillfocusCbpara02();
    afx_msg void OnCbnKillfocusCbpara03();
    afx_msg void OnCbnKillfocusCbpara04();
    afx_msg void OnCbnKillfocusCbpara05();
    afx_msg void OnCbnKillfocusCbpara06();
    afx_msg void OnCbnKillfocusCbpara07();
    afx_msg void OnCbnKillfocusCbpara08();
    afx_msg void OnCbnKillfocusCbpara09();
    afx_msg void OnCbnKillfocusCbpara10();
    afx_msg void OnCbnKillfocusCbpara11();
    afx_msg void OnCbnKillfocusCbpara12();
    afx_msg void OnCbnKillfocusCbpara13();
    afx_msg void OnCbnKillfocusCbpara14();
    afx_msg void OnCbnKillfocusCbpara15();
    afx_msg void OnCbnKillfocusCbpara16();
    //afx_msg void OnCbnKillfocusCbpara17();
    //afx_msg void OnCbnKillfocusCbpara18();
    afx_msg void OnEnKillfocusEditmaxcir();
    afx_msg void OnKillfocusEditopttorq();
    afx_msg void OnCbnSelchangeCbalias();
    afx_msg void OnCbnKillfocusCbalias();
    afx_msg void OnBnClickedDelalias();
    afx_msg void OnBnClickedRadiosetone();
    afx_msg void OnBnClickedRadiosettwo();
    afx_msg void OnBnClickedRadiosetthree();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    BOOL GetParaValue(PARACFG* ptCfg);
    void SetParaValue(PARACFG* ptCfg, SHOWCFG* ptShow);
    void JudgeShowParaChanged(UINT nIndex);
    void UpdateDlgLabel();
    void InitAliasShow();
    void CheckColumns(int cur);
    CString DelCurAliasContent();

    COLORREF    m_clrNormal;
    COLORREF    m_clrChanged;
    UINT        m_nCurLang;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPARASET_H__4ED7C23C_5F6E_4A6E_98BA_B7E3D9ACFE62__INCLUDED_)

