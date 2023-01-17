#pragma once

#include "LineChartCtrlStat.h"
#include "Ruler.h"
#include "afxwin.h"
#include ".\TeeChart\tchart.h"

#define     MAXSTATLEVEL        4


// CDlgDataStat dialog
class CDlgDataStat : public CDialogEx
{
    DECLARE_DYNAMIC(CDlgDataStat)

public:
    CDlgDataStat(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDlgDataStat();

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DLGDATASTAT };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    CString m_strLBS3;
    void    ShowCurStat(CString strStatFile);
    string  SaveStatImg(void);

protected:
    virtual BOOL DestroyWindow();
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedBtnOpendata();
    afx_msg void OnBnClickedBtnsetstat();
    afx_msg void OnCbnSelchangeCombobucktype();
    afx_msg void OnCbnSelchangeCombostattype();
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedBtnsavestat();
    afx_msg void OnBnClickedCheckalign();

    CRuler              m_xStatAxis;
    CRuler              m_yStatAxis;
    CLineChartCtrlStat  m_wndLineStat;
    CString             m_strStatFile;
    UINT                m_nQuali;
    UINT                m_nUnQuali;
    CString             m_strBadNO;
    CString             m_strToolNO;
    CComboBox           m_cbBuckType;
    CComboBox           m_cbStatType;
    UINT                m_nBuckNum;
    double              m_fOptTorq;
    double              m_fMaxTorq;
    double              m_fMinTorq;
    BOOL                m_bAlignShow;

    CTChart             m_tccCtrl;
    //CTChart             m_tccShould;
    //CTChart             m_tccDelta;
    CString             m_strCtrlLabel[MAXSTATLEVEL];
    //CString             m_strShouldLabel[MAXSTATLEVEL];
    //CString             m_strDeltaLabel[MAXSTATLEVEL];
    COLORREF            m_clrLabel[MAXSTATLEVEL];


private:
    void ResetStatLine(PARACFG* ptCfg);
    void ResetStatLineByData(TorqData::Torque* ptTorq);
    void GetBasicStatInfo();
    void ClearFileInfo();
    BYTE GetValueRange(double* fRange, double fValue);
    void InitChart(CTChart* ptTChart, CString strCaption, CString strLabel[]);
    void BeginCalStat(BOOL bSetRange = FALSE);
    void InitLabelInfo();
    void InitStatRange(double fMin, double fCtrl, double fMax);
    void GetStatType();
    void DrawStatFlow(CTChart* ptChart, CString strRatio[], CString strLabel[], UINT pnNum[]);
    void DrawStatTorq(vector<int> listNo);
    void UpdateDlgLabel();
    BOOL IsSelType(TorqData::Torque* ptTorq);   /* 是否是选中的类型 */

    CString             m_strCtrlRatio[MAXSTATLEVEL];
    //CString             m_strInflRatio[MAXSTATLEVEL];
    //CString             m_strDeltRatio[MAXSTATLEVEL];

    CStringList         m_slStatType;
    WORD                m_nStatType;
    TorqData::Torque* m_ptDrawData;

    STATCFG             m_tStatCfg;         /* 统计配置 */
};
