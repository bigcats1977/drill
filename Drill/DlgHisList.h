#if !defined(AFX_DLGHISLIST_H__F42D7DFF_8237_490B_A8ED_871C7982D4D0__INCLUDED_)
#define AFX_DLGHISLIST_H__F42D7DFF_8237_490B_A8ED_871C7982D4D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHisList.h : header file
//
#include "Mylistctrl.h"
#include "excel.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgHisList dialog

class CDlgHisList : public CPropertyPage
{
    DECLARE_DYNCREATE(CDlgHisList)

    // Construction
public:
    CDlgHisList();
    ~CDlgHisList();

    // Dialog Data
        //{{AFX_DATA(CDlgHisList)
    enum { IDD = IDD_DLGHISLIST };
    CMylistctrl m_listHis;
    CString     m_strHisName;
    UINT        m_nHisTotalRec;
    UINT        m_nHisQualyRec;
    UINT        m_nHisUnQualyRec;
    BOOL        m_bExpAllImg;
    //}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CDlgHisList)
public:
    virtual BOOL OnSetActive();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CDlgHisList)
    virtual BOOL OnInitDialog();
    afx_msg void OnBtnhis();
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedBtnstatlist();
    afx_msg void OnBnClickedBtnexport();
    afx_msg void OnBnClickedBtnimgexp();
    afx_msg void OnBnClickedBtnOrgdata();
    afx_msg void OnBnClickedBtngraphexp();
    afx_msg void OnBnClickedBtnstatset();
    afx_msg void OnBnClickedBtnimportdepth();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

public:
    void        SetDataPlace(UINT nCur);

private:
    VOID    ShowHisTorqList();
    VOID    GetPrintDlg();
    UINT    GetSelectItem();
    void    WriteCoverSheet();
    void    WriteSummarySheet();
    void    WriteQualitySheet();
    void    WriteScatterSheet();
    void    WriteReportSheet();
    //void    AddExcelChart(double top, CString strRange);
    void    SetCell(long irow, long icolumn, CString new_string);
    void    SetCell(long irow, long icolumn, double fValue);
    void    SetCell(long irow, long icolumn, int iValue);
    CString GetOperInfo(CStringList& slOper);
    BOOL    SetMultiValue(int iSeq, int iIndex, int iBegin, int iMaxNum);
    int     GetParaValueInfo(int iIndex, vector<int>& listNo);
    CString GetWellNO(BOOL bSuffix = TRUE);
    CString GetWellDepth();
    void    Export1Img(UINT* pnSel, UINT nSelCount);
    void    Export2Img(UINT* pnSel, UINT nSelCount);
    void    Export3Img(UINT* pnSel, UINT nSelCount);
    void    Export8Img(UINT* pnSel, UINT nSelCount);
    CString GetConsSummaryInfo();
    void    SetCellFont(int iRow, int iCol, int iNum, UINT dwQuality);
    void    FillReportHead(int& iRow, TorqData::Torque* ptHeadTorq);
    void    FillReportData(int& iRow, TorqData::Torque* ptHeadTorq);
    bool    GetDepthInfo(vector<int>& SeqNO, vector<string>& Depth);

    UINT    m_nMaxShowNum;
    int     m_iWidth;
    string  m_strFixHead;
    UINT    m_nSelItem[MAXWELLNUM];     /* 记录选定的记录的序号 */
    UINT    m_nCurLang;
    TorqData::Torque* m_ptStatTorq;     /* 统计数据的地址，从最后开始排除工具扣,且没有备注 */
    Excel   m_tSaveExc;

    XLSSTATCFG* m_ptStat;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHISLIST_H__F42D7DFF_8237_490B_A8ED_871C7982D4D0__INCLUDED_)
