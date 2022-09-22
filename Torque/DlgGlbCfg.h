#pragma once


#include "afxwin.h"
#include "ColorComboBox.h"
#include "NumEdit.h"

// CDlgGlbCfg dialog

class CDlgGlbCfg : public CDialog
{
	DECLARE_DYNAMIC(CDlgGlbCfg)

public:
	CDlgGlbCfg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgGlbCfg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGGLBCFG };
#endif
	CComboBox   m_cbPort;
	CComboBox   m_cbImgNum;
	CNumEdit    m_nePlus;

	UINT        m_nPort;
	UINT        m_nPlus;
	UINT        m_nReset;
	UINT        m_nCollect;
	UINT		m_nSave;
	BOOL		m_bDateBehind;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()


private:
	BOOL GetParaValue(GLBCFG* ptCfg);
	void SetParaValue(GLBCFG* ptCfg);
	void SetImgNum();

	COLORREF    m_clrNormal;
	COLORREF    m_clrChanged;
	UINT        m_nCurLang;
};
