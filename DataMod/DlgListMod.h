#if !defined(AFX_DLGLISTMOD_H__17B36752_E408_4D1B_A0EB_BB7070626585__INCLUDED_)
#define AFX_DLGLISTMOD_H__17B36752_E408_4D1B_A0EB_BB7070626585__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgListMod.h : header file
//

#include "DlgHisGrp.h"
#include "ListCtrlEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgListMod dialog

#define     OPTIM_METH_CTRLTORQ     0
#define     OPTIM_METH_CIRCLE       1
#define     OPTIM_METH_MODDATA      2
#define     OPTIM_METH_OVERLAP      3
#define     MAX_OPTIM_METH_NUM      (OPTIM_METH_OVERLAP+1)

class CDlgListMod : public CDialog
{
    // Construction
public:
    CDlgListMod(CWnd* pParent = NULL);   // standard constructor

    /* 扭矩优化函数 */
    /* 修改控制扭矩，图像上下拉伸 */
    BOOL CtrlTorqOptim(TorqData::Torque* pSrcData, TorqData::Torque* pDestData);
    /* 修改周数，图像左右拉伸 */
    BOOL CircleOptim(TorqData::Torque* pSrcData, TorqData::Torque* pDestData);
    /* 修改数据，周数不变，扭矩值修改 */
    BOOL ModDataOptim(TorqData::Torque* pSrcData, TorqData::Torque* pDestData);
    /* 重叠图形优化 */
    BOOL OverlapOptim(TorqData::Torque* pSrcData, TorqData::Torque* pDestData);

    // Dialog Data
        //{{AFX_DATA(CDlgListMod)
    enum { IDD = IDD_DLGLISTMOD };
    CListCtrl   m_lsData;
    double      m_fTorque;
    double      m_fCir;
    int         m_iMethod;
    BOOL        m_bQuality;
    //}}AFX_DATA

public:
    TorqData::Torque* m_ptModTorq;

    // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDlgListMod)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    CListCtrlEdit       m_lceEdit;
    // Generated message map functions
    //{{AFX_MSG(CDlgListMod)
    virtual BOOL OnInitDialog();
    afx_msg void OnClickListdata(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnOptimited();
    afx_msg void OnRadiocontrol();
    afx_msg void OnRadiocircle();
    afx_msg void OnRadiodetail();
    afx_msg void OnRadiooverlap();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    CDlgHisGrp* m_pdlgParent;

    void ShowTorqValue();
    int  GetInsertNum(double fT1, double fT2);
    BOOL CircleOptimByTorq(TorqData::Torque* pSrcData, TorqData::Torque* pDestData);
    BOOL CircleOptimByPulse(TorqData::Torque* pSrcData, TorqData::Torque* pDestData);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLISTMOD_H__17B36752_E408_4D1B_A0EB_BB7070626585__INCLUDED_)


