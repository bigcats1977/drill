#pragma once

#include "ColorComboBox.h"
#include "NumEdit.h"
#include "TubeCfg.h"

// DlgTubeCfg dialog

class CDlgTubeCfg : public CDialog
{
public:
	CDlgTubeCfg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTubeCfg();

    TUBECFG tTubeCfg;

    // Dialog Data
        //{{AFX_DATA(CDlgTubeCfg)
    enum { IDD = IDD_DLGTUBECFG};
    CComboBox m_cbTypeKind;
    CComboBox m_cbFactoryName;
    CColorComboBox m_cbKindNO;

    CColorComboBox  m_cbFixTubingOpt[MAXTUBECFGNUM];
    CNumEdit m_neTorqVal[MAXTUBETORQNUM];
    CEdit  m_eKindName[LANGUAGE_NUM];
    CString m_strKindNORange;
    CString m_strKindName[LANGUAGE_NUM];
    CString m_strFixTubingOpt[MAXTUBECFGNUM];
    CString m_strTorqVal[MAXTUBETORQNUM];
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgCollect)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgTubeCfg)
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedAddtubekind();
    afx_msg void OnBnClickedModtubekind();
    //afx_msg void OnBnClickedDeltubekind();
    afx_msg void OnBnClickedAddtubetorq();
    afx_msg void OnBnClickedDeltubetorq();
    afx_msg void OnBnClickedModtubetorq();
    afx_msg void OnCbnSelchangeCbtypekind();
    afx_msg void OnCbnSelchangeCbfactoryname();
    afx_msg void OnCbnSelchangeCbkindno();
    afx_msg void OnCbnKillfocusCbkindno();
    afx_msg void OnCbnSelchangeCbfactory();
    afx_msg void OnCbnSelchangeCboem();
    afx_msg void OnCbnSelchangeCbtubingsize();
    afx_msg void OnCbnSelchangeCbmaterial();
    afx_msg void OnCbnSelchangeCbcoupling();
    afx_msg void OnEnKillfocusEdittubemaxtorq();
    afx_msg void OnEnKillfocusEdittubeopttorq();
    afx_msg void OnEnKillfocusEdittubemintorq();
    afx_msg void OnCbnKillfocusCbtypekind();
    afx_msg void OnCbnKillfocusCbfactoryname();
    afx_msg void OnCbnKillfocusCbfactory();
    afx_msg void OnCbnKillfocusCboem();
    afx_msg void OnCbnKillfocusCbtubingsize();
    afx_msg void OnCbnKillfocusCbmaterial();
    afx_msg void OnCbnKillfocusCbcoupling();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    COLORREF    m_clrNormal;
    COLORREF    m_clrChanged;
    UINT        m_nCurLang;

    // for —°œÓ√˚≥∆
    int     m_iTubeKind;
    int     m_iTubeFactory;
    int     m_iKindLow;
    int     m_iKindHigh;
    int     m_iKindNO;
    int     m_iNOIndex;
    FIXTUBEINFO  *m_ptTubeInfo;
    //CDBText     *m_ptDBTubeText;

    void    InitFixTypeKind();
    void    InitFixFactoryName();
    void    InitFixKindNO();
    void    InitFixKindName();
    void    SetKindName(int iIndex);
    void    InitTubeVariable();
    void    GetCurTubeInfo();
    void    GetTubeKindNO();
    BOOL    CheckTubeInfo(BOOL bNeedExist, BOOL bDel);
    void    EnableFactoryCtrl(BOOL bEnable);
    void    EnableKindCtrl(BOOL bExist);
    void    ReportOperResult(UINT nBtnID, BOOL bSucc);

    // for ≈§æÿ≈‰÷√
    TUBECFG       m_tSrcTub;
    TUBECFG       m_tCurTub;
    //CDBTubeCfg      *m_ptDBTubeCfg;

    void    InitTorqShow(TUBECFG* ptTube);
    void    SetTorqShow(TUBECFG* ptTube);
    void    SetTorqValue(TUBECFG* ptTube);
    BOOL    GetTorqValue(TUBECFG* ptTube);
    void    ClearTorqValue(TUBECFG* ptTube);
    void    GetCurTorqInfo();
    void    SelchangeComboBox(UINT nIndex);
    void    ChangeTubingShowBox(UINT nIndex);
    void    JudgeTorqCBParaChanged(UINT nIndex, TUBECFG* ptTube);
    void    EnableTorqCtrl(BOOL bExist);
    void    UpdateLBFTTorq(UINT nIndex);
};
