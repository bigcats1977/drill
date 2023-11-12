#pragma once

#include "ColorRadio.h"

// CDlgWITSCfg dialog

class CDlgWITSCfg : public CDialog
{
	DECLARE_DYNAMIC(CDlgWITSCfg)

public:
	CDlgWITSCfg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgWITSCfg();
	SHOWCFG		m_tempShow;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGTCPCFG };
#endif
	UINT			m_nFixItems[WITSRPT_FIXHEADNUM];
	UINT			m_nRepeatItems[WITSRPT_REPEATNUM];
	UINT			m_nCalItems[WITSRPT_CALPARANUM];
	UINT			m_nConfigItems[WITSRPT_CONFIGNUM];
	UINT			m_nShowItems[WITSRPT_SHOWPARANUM];
	BOOL			m_bChecked[WITSRPT_SHOWPARANUM];
	CColorRadio  	m_ckShowPara[WITSRPT_SHOWPARANUM];
	CEdit			m_edShowPara[WITSRPT_SHOWPARANUM];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCheckwits01();
	afx_msg void OnBnClickedCheckwits02();
	afx_msg void OnBnClickedCheckwits03();
	afx_msg void OnBnClickedCheckwits04();
	afx_msg void OnBnClickedCheckwits05();
	afx_msg void OnBnClickedCheckwits06();
	afx_msg void OnBnClickedCheckwits07();
	afx_msg void OnBnClickedCheckwits08();
	afx_msg void OnBnClickedCheckwits09();
	afx_msg void OnBnClickedCheckwits10();
	afx_msg void OnBnClickedCheckwits11();
	afx_msg void OnBnClickedCheckwits12();
	afx_msg void OnBnClickedCheckwits13();
	afx_msg void OnBnClickedCheckwits14();
	afx_msg void OnBnClickedCheckwits15();

	DECLARE_MESSAGE_MAP()

private:
	bool GetParaValue(WITSCFG* ptCfg);
	void SetParaValue(WITSCFG* ptCfg);
	void JudgeShowParaCheck(int iCtrlIdx);
	int  GetItemIndexByPara(WITSCFG* ptCfg, int para);
	COLORREF    m_clrNormal;
	COLORREF    m_clrChanged;
	WITSCFG*	m_pCurCfg;
};
