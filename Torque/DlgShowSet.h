#pragma once
#include "afxwin.h"
#include "Torque.h"
#include "ColorRadio.h"

// CDlgShowSet dialog

class CDlgShowSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgShowSet)

public:
	CDlgShowSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgShowSet();
    SHOWCFG m_tempShow;
    BOOL    m_bModified;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGSHOWPARA };
#endif
    CString m_strShowName[MAXPARANUM];
    BOOL m_bList[MAXPARANUM];
    CColorRadio m_clListName[MAXPARANUM];
    BOOL m_bMain[MAXPARANUM];
    CColorRadio m_clMainName[MAXPARANUM];
    int m_iFileName;
    CColorRadio m_rdShowName[MAXPARANUM];
    int m_iShowStat;
    CColorRadio m_rdStatName[MAXPARANUM];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedUpdateshow();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedRadioshowfile1();
    afx_msg void OnBnClickedRadioshowfile2();
    afx_msg void OnBnClickedRadioshowfile3();
    afx_msg void OnBnClickedRadioshowfile4();
    afx_msg void OnBnClickedRadioshowfile5();
    afx_msg void OnBnClickedRadioshowfile6();
    afx_msg void OnBnClickedRadioshowfile7();
    afx_msg void OnBnClickedRadioshowfile8();
    afx_msg void OnBnClickedRadioshowfile9();
    afx_msg void OnBnClickedRadioshowfile10();
    afx_msg void OnBnClickedRadioshowfile11();
    afx_msg void OnBnClickedRadioshowfile12();
    afx_msg void OnBnClickedRadioshowfile13();
    afx_msg void OnBnClickedRadioshowfile14();
    afx_msg void OnBnClickedRadioshowfile15();
    afx_msg void OnBnClickedRadioshowfile16();
    afx_msg void OnBnClickedRadioshowfile17();
    afx_msg void OnBnClickedRadioshowfile18();
    afx_msg void OnBnClickedRadioshowstat1();
    afx_msg void OnBnClickedRadioshowstat2();
    afx_msg void OnBnClickedRadioshowstat3();
    afx_msg void OnBnClickedRadioshowstat4();
    afx_msg void OnBnClickedRadioshowstat5();
    afx_msg void OnBnClickedRadioshowstat6();
    afx_msg void OnBnClickedRadioshowstat7();
    afx_msg void OnBnClickedRadioshowstat8();
    afx_msg void OnBnClickedRadioshowstat9();
    afx_msg void OnBnClickedRadioshowstat10();
    afx_msg void OnBnClickedRadioshowstat11();
    afx_msg void OnBnClickedRadioshowstat12();
    afx_msg void OnBnClickedRadioshowstat13();
    afx_msg void OnBnClickedRadioshowstat14();
    afx_msg void OnBnClickedRadioshowstat15();
    afx_msg void OnBnClickedRadioshowstat16();
    afx_msg void OnBnClickedRadioshowstat17();
    afx_msg void OnBnClickedRadioshowstat18();
    afx_msg void OnBnClickedChecklist1();
    afx_msg void OnBnClickedChecklist2();
    afx_msg void OnBnClickedChecklist3();
    afx_msg void OnBnClickedChecklist4();
    afx_msg void OnBnClickedChecklist5();
    afx_msg void OnBnClickedChecklist6();
    afx_msg void OnBnClickedChecklist7();
    afx_msg void OnBnClickedChecklist8();
    afx_msg void OnBnClickedChecklist9();
    afx_msg void OnBnClickedChecklist10();
    afx_msg void OnBnClickedChecklist11();
    afx_msg void OnBnClickedChecklist12();
    afx_msg void OnBnClickedChecklist13();
    afx_msg void OnBnClickedChecklist14();
    afx_msg void OnBnClickedChecklist15();
    afx_msg void OnBnClickedChecklist16();
    afx_msg void OnBnClickedChecklist17();
    afx_msg void OnBnClickedChecklist18();
    afx_msg void OnBnClickedCheckmain1();
    afx_msg void OnBnClickedCheckmain2();
    afx_msg void OnBnClickedCheckmain3();
    afx_msg void OnBnClickedCheckmain4();
    afx_msg void OnBnClickedCheckmain5();
    afx_msg void OnBnClickedCheckmain6();
    afx_msg void OnBnClickedCheckmain7();
    afx_msg void OnBnClickedCheckmain8();
    afx_msg void OnBnClickedCheckmain9();
    afx_msg void OnBnClickedCheckmain10();
    afx_msg void OnBnClickedCheckmain11();
    afx_msg void OnBnClickedCheckmain12();
    afx_msg void OnBnClickedCheckmain13();
    afx_msg void OnBnClickedCheckmain14();
    afx_msg void OnBnClickedCheckmain15();
    afx_msg void OnBnClickedCheckmain16();
    afx_msg void OnBnClickedCheckmain17();
    afx_msg void OnBnClickedCheckmain18();
	DECLARE_MESSAGE_MAP()

private:
    COLORREF    m_clrNormal;
    COLORREF    m_clrChanged;
    int         m_iPriorShowCtrlID;
    int         m_iPriorStatCtrlID;
    BOOL        m_bHisList[MAXPARANUM];
    BOOL        m_bHisMain[MAXPARANUM];

    void        JudgeShowRadio(int iCtrlIdx);
    void        JudgeStatRadio(int iCtrlIdx);
    void        JudgeListCheck(int iCtrlIdx);
    void        JudgeMainCheck(int iCtrlIdx);
    BOOL        SaveShowCfgInotDB();
    BOOL        CheckShowCfgChanged();
};
