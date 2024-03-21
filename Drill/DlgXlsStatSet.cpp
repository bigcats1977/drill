// CDlgXlsStatSet.cpp: 实现文件
//

#include "stdafx.h"
#include "Drill.h"
#include "DlgXlsStatSet.h"
#include "afxdialogex.h"


// CDlgXlsStatSet 对话框


TorqData::Torque* g_ptLast = NULL;

IMPLEMENT_DYNAMIC(CDlgXlsStatSet, CDialogEx)

CDlgXlsStatSet::CDlgXlsStatSet(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DLGXLSSTATSET, pParent)
{

}

CDlgXlsStatSet::~CDlgXlsStatSet()
{
}

void CDlgXlsStatSet::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SCBGENNAME1, m_cbGenParaName[0]);
    DDX_Control(pDX, IDC_SCBGENNAME2, m_cbGenParaName[1]);
    DDX_Control(pDX, IDC_SCBGENNAME3, m_cbGenParaName[2]);
    DDX_Control(pDX, IDC_SCBGENNAME4, m_cbGenParaName[3]);
    DDX_Control(pDX, IDC_SCBGENNAME5, m_cbGenParaName[4]);
    DDX_CBIndex(pDX, IDC_SCBGENNAME1, m_iGenParaIndex[0]);
    DDX_CBIndex(pDX, IDC_SCBGENNAME2, m_iGenParaIndex[1]);
    DDX_CBIndex(pDX, IDC_SCBGENNAME3, m_iGenParaIndex[2]);
    DDX_CBIndex(pDX, IDC_SCBGENNAME4, m_iGenParaIndex[3]);
    DDX_CBIndex(pDX, IDC_SCBGENNAME5, m_iGenParaIndex[4]);
    DDX_Control(pDX, IDC_SCBJOBNAME1, m_cbJobParaName[0]);
    DDX_Control(pDX, IDC_SCBJOBNAME2, m_cbJobParaName[1]);
    DDX_Control(pDX, IDC_SCBJOBNAME3, m_cbJobParaName[2]);
    DDX_Control(pDX, IDC_SCBJOBNAME4, m_cbJobParaName[3]);
    DDX_Control(pDX, IDC_SCBJOBNAME5, m_cbJobParaName[4]);
    DDX_Control(pDX, IDC_SCBJOBNAME6, m_cbJobParaName[5]);
    DDX_CBIndex(pDX, IDC_SCBJOBNAME1, m_iJobParaIndex[0]);
    DDX_CBIndex(pDX, IDC_SCBJOBNAME2, m_iJobParaIndex[1]);
    DDX_CBIndex(pDX, IDC_SCBJOBNAME3, m_iJobParaIndex[2]);
    DDX_CBIndex(pDX, IDC_SCBJOBNAME4, m_iJobParaIndex[3]);
    DDX_CBIndex(pDX, IDC_SCBJOBNAME5, m_iJobParaIndex[4]);
    DDX_CBIndex(pDX, IDC_SCBJOBNAME6, m_iJobParaIndex[5]);
    DDX_Control(pDX, IDC_SCBINFONAME1, m_cbInfoParaName[0]);
    DDX_Control(pDX, IDC_SCBINFONAME2, m_cbInfoParaName[1]);
    DDX_CBIndex(pDX, IDC_SCBINFONAME1, m_iInfoParaIndex[0]);
    DDX_CBIndex(pDX, IDC_SCBINFONAME2, m_iInfoParaIndex[1]);
}


BEGIN_MESSAGE_MAP(CDlgXlsStatSet, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDlgXlsStatSet::OnBnClickedOk)
    ON_CBN_KILLFOCUS(IDC_SCBGENNAME1, &CDlgXlsStatSet::OnCbnKillfocusScbgenname1)
    ON_CBN_KILLFOCUS(IDC_SCBGENNAME2, &CDlgXlsStatSet::OnCbnKillfocusScbgenname2)
    ON_CBN_KILLFOCUS(IDC_SCBGENNAME3, &CDlgXlsStatSet::OnCbnKillfocusScbgenname3)
    ON_CBN_KILLFOCUS(IDC_SCBGENNAME4, &CDlgXlsStatSet::OnCbnKillfocusScbgenname4)
    ON_CBN_KILLFOCUS(IDC_SCBGENNAME5, &CDlgXlsStatSet::OnCbnKillfocusScbgenname5)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNAME1, &CDlgXlsStatSet::OnCbnKillfocusScbjobname1)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNAME2, &CDlgXlsStatSet::OnCbnKillfocusScbjobname2)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNAME3, &CDlgXlsStatSet::OnCbnKillfocusScbjobname3)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNAME4, &CDlgXlsStatSet::OnCbnKillfocusScbjobname4)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNAME5, &CDlgXlsStatSet::OnCbnKillfocusScbjobname5)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNAME6, &CDlgXlsStatSet::OnCbnKillfocusScbjobname6)
    ON_CBN_KILLFOCUS(IDC_SCBINFONAME1, &CDlgXlsStatSet::OnCbnKillfocusScbinfoname1)
    ON_CBN_KILLFOCUS(IDC_SCBINFONAME2, &CDlgXlsStatSet::OnCbnKillfocusScbinfoname2)
END_MESSAGE_MAP()

// CDlgXlsStatSet 消息处理程序
BOOL CDlgXlsStatSet::OnInitDialog()
{
    int     i = 0;

    CDialog::OnInitDialog();

    g_ptLast = &g_tReadData.tData[g_tReadData.nTotal - 1];
    m_ptStat = &theApp.m_tXlsStatCfg[g_tGlbCfg.nLangType];

    m_clrNormal = RGB(0, 0, 0);
    m_clrChanged = RGB(255, 0, 0);

    for (i = 0; i < STATPARA_GENNUM; i++) {
        m_iGenParaIndex[i] = m_ptStat->GenPara[i];
        InitCBContent(&m_cbGenParaName[i], m_iGenParaIndex[i]);
    }
    for (i = 0; i < STATPARA_JOBNUM; i++) {
        m_iJobParaIndex[i] = m_ptStat->JobPara[i];
        InitCBContent(&m_cbJobParaName[i], m_iJobParaIndex[i]);
    }
    for (i = 0; i < STATPARA_INFONUM; i++) {
        m_iInfoParaIndex[i] = m_ptStat->InfoPara[i];
        InitCBContent(&m_cbInfoParaName[i], m_iInfoParaIndex[i]);
    }

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CDlgXlsStatSet::InitCBContent(CComboBox* ptCB, UINT index)
{
    ASSERT_NULL(ptCB);
    COMP_BGE(index, MAXPARANUM);

    ptCB->Clear();
    for (int i = 0; i < MAXPARANUM; i++) {
        ptCB->AddString(theApp.m_ptCurShow->strShow[i].c_str());
    }
    ptCB->SetCurSel(index);
    return;
}

void CDlgXlsStatSet::OnBnClickedOk()
{
    int     i = 0;

    UpdateData(TRUE);

    for (i = 0; i < STATPARA_GENNUM; i++) {
        m_ptStat->GenPara[i] = m_iGenParaIndex[i];
    }
    for (i = 0; i < STATPARA_JOBNUM; i++) {
        m_ptStat->JobPara[i] = m_iJobParaIndex[i];
    }
    for (i = 0; i < STATPARA_INFONUM; i++) {
        m_ptStat->InfoPara[i] = m_iInfoParaIndex[i];
    }

    theDB.UpdateXlsStatPara(m_ptStat);

    CDialog::OnOK();
}

void CDlgXlsStatSet::JudgeGenParaChanged(UINT no)
{
    COLORREF    clrCtrl;

    COMP_BGE(no, STATPARA_GENNUM);

    UpdateData(TRUE);
    clrCtrl = m_clrNormal;
    if (m_iGenParaIndex[no] != m_ptStat->GenPara[no])
        clrCtrl = m_clrChanged;
    m_cbGenParaName[no].m_ColorText = clrCtrl;
    m_cbGenParaName[no].Invalidate();
}

void CDlgXlsStatSet::JudgeJobParaChanged(UINT no)
{
    COLORREF    clrCtrl;

    COMP_BGE(no, STATPARA_JOBNUM);

    UpdateData(TRUE);
    clrCtrl = m_clrNormal;
    if (m_iJobParaIndex[no] != m_ptStat->JobPara[no])
        clrCtrl = m_clrChanged;
    m_cbJobParaName[no].m_ColorText = clrCtrl;
    m_cbJobParaName[no].Invalidate();
}

void CDlgXlsStatSet::JudgeInfoParaChanged(UINT no)
{
    COLORREF    clrCtrl;

    COMP_BGE(no, STATPARA_INFONUM);

    UpdateData(TRUE);
    clrCtrl = m_clrNormal;
    if (m_iInfoParaIndex[no] != m_ptStat->InfoPara[no])
        clrCtrl = m_clrChanged;
    m_cbInfoParaName[no].m_ColorText = clrCtrl;
    m_cbInfoParaName[no].Invalidate();
}

void CDlgXlsStatSet::OnCbnKillfocusScbgenname1()
{
    JudgeGenParaChanged(0);
}
void CDlgXlsStatSet::OnCbnKillfocusScbgenname2()
{
    JudgeGenParaChanged(1);
}
void CDlgXlsStatSet::OnCbnKillfocusScbgenname3()
{
    JudgeGenParaChanged(2);
}
void CDlgXlsStatSet::OnCbnKillfocusScbgenname4()
{
    JudgeGenParaChanged(3);
}
void CDlgXlsStatSet::OnCbnKillfocusScbgenname5()
{
    JudgeGenParaChanged(4);
}

void CDlgXlsStatSet::OnCbnKillfocusScbjobname1()
{
    JudgeJobParaChanged(0);
}
void CDlgXlsStatSet::OnCbnKillfocusScbjobname2()
{
    JudgeJobParaChanged(1);
}
void CDlgXlsStatSet::OnCbnKillfocusScbjobname3()
{
    JudgeJobParaChanged(2);
}
void CDlgXlsStatSet::OnCbnKillfocusScbjobname4()
{
    JudgeJobParaChanged(3);
}
void CDlgXlsStatSet::OnCbnKillfocusScbjobname5()
{
    JudgeJobParaChanged(4);
}
void CDlgXlsStatSet::OnCbnKillfocusScbjobname6()
{
    JudgeJobParaChanged(5);
}

void CDlgXlsStatSet::OnCbnKillfocusScbinfoname1()
{
    JudgeInfoParaChanged(0);
}
void CDlgXlsStatSet::OnCbnKillfocusScbinfoname2()
{
    JudgeInfoParaChanged(1);
}
