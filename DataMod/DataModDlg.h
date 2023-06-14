// DataModDlg.h : header file
//

#if !defined(AFX_DATAMODDLG_H__223ECD0E_42FA_4550_A305_C8AA4751D299__INCLUDED_)
#define AFX_DATAMODDLG_H__223ECD0E_42FA_4550_A305_C8AA4751D299__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mylistctrl.h"


#define     MAXDELRECORD    500
/////////////////////////////////////////////////////////////////////////////
// CDataModDlg dialog

class CDataModDlg : public CDialog
{
    // Construction
public:
    CDataModDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
    //{{AFX_DATA(CDataModDlg)
    enum { IDD = IDD_DATAMOD_DIALOG };
    CButton m_btnReplace;
    CButton m_btnOpt;
    CButton m_btnOpen;
    CButton m_btnInsert;
    CButton m_btnDel;
    CButton m_btnPW;
    CButton m_btnMerge;
    CMylistctrl m_listData;
    CMylistctrl m_listData2;
    CString m_strDataName;
    CString m_strDataName2;
    UINT    m_nSrc1;     /* 源位置 */
    UINT    m_nDest;    /* 目的位置或者目的起始位置*/
    UINT    m_nInsert;  /* 插入起始位置 */
    UINT    m_nDestEnd; /* 多个替换的目的结束位置  */
    int     m_iReplace; /* 0:单个替换；1:多个替换 */
    UINT    m_nHisTotalRec;
    UINT    m_nHisQualyRec;
    UINT    m_nHisUnQualyRec;
    BOOL    m_bUpdWellNO;
    BOOL    m_bHaveOtherSrc;
    UINT    m_nTotal2;
    UINT    m_nSrc2;     /* 第2文件的源位置 */
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDataModDlg)
public:
    virtual BOOL DestroyWindow();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

public:
    void ReReadTorqFile();

    // Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CDataModDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBtnopendata();
    afx_msg void OnBtnopt();
    afx_msg void OnBtndel();
    afx_msg void OnBtnreplace();
    afx_msg void OnBtninsert();
    afx_msg void OnRadiosingle();
    afx_msg void OnRadiomulti();
    afx_msg LRESULT ReReadDataFun(WPARAM wParam, LPARAM lParam);
    afx_msg void OnLvnItemchangedListdata(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBnClickedCheckupdwellno();
    afx_msg void OnEnKillfocusEditwellname();
    afx_msg void OnBnClickedBtnpw();
    afx_msg void OnBnClickedBtnstatexp();
    afx_msg void OnBnClickedBtndocexp();
    afx_msg void OnBnClickedBtnimgexp();
    afx_msg void OnBnClickedCheckother();
    afx_msg void OnBtnopendata2();
    afx_msg void OnBnClickedBtnmerge();
    afx_msg void OnNMClickListdata(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMClickListdata2(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    BOOL DelSelectData();
    void ReplaceTorque(UINT nDest, TorqData::Torque* ptSrc);
    UINT GetSelectItem(CString* pstrItem);
    void EnableCtrl();
    void EnableCtrl2();
    void UpdateTorqNum();
    CString GetFileName(CString path);

    /* for dlghislist */
    void    ShowHisTorqList(bool bFirst = true);
    UINT    GetSelectItem();
    int     GetOperInfo(CStringList& slOper);
    bool    GetSrcTorq(TorqData::Torque** ptSrc);

    int     m_iWidth;
    string  m_strFixHead;
    BOOL    m_bCanMod;
    UINT    m_nSelItem[MAXWELLNUM];     /* 记录选定的记录的序号 */
    TorqData::Torque* m_ptStatTorq;     /* 统计数据的地址，从最后开始排除工具扣,且没有备注 */
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAMODDLG_H__223ECD0E_42FA_4550_A305_C8AA4751D299__INCLUDED_)
