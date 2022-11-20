// CDlgXlsStatSet.cpp: 实现文件
//

#include "stdafx.h"
#include "Torque.h"
#include "DlgXlsStatSet.h"
#include "afxdialogex.h"


// CDlgXlsStatSet 对话框


TorqData::Torque   *g_ptLast = NULL;

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
    DDX_Control(pDX, IDC_SCBGENNO1, m_cbGenParaIndex[0]);
    DDX_Control(pDX, IDC_SCBGENNO2, m_cbGenParaIndex[1]);
    DDX_Control(pDX, IDC_SCBGENNO3, m_cbGenParaIndex[2]);
    DDX_Control(pDX, IDC_SCBGENNO4, m_cbGenParaIndex[3]);
    DDX_Control(pDX, IDC_SCBGENNO5, m_cbGenParaIndex[4]);
    DDX_Control(pDX, IDC_SCBGENNAME1, m_cbGenParaName[0]);
    DDX_Control(pDX, IDC_SCBGENNAME2, m_cbGenParaName[1]);
    DDX_Control(pDX, IDC_SCBGENNAME3, m_cbGenParaName[2]);
    DDX_Control(pDX, IDC_SCBGENNAME4, m_cbGenParaName[3]);
    DDX_Control(pDX, IDC_SCBGENNAME5, m_cbGenParaName[4]);
    DDX_Control(pDX, IDC_SCBJOBNO1, m_cbJobParaIndex[0]);
    DDX_Control(pDX, IDC_SCBJOBNO2, m_cbJobParaIndex[1]);
    DDX_Control(pDX, IDC_SCBJOBNO3, m_cbJobParaIndex[2]);
    DDX_Control(pDX, IDC_SCBJOBNO4, m_cbJobParaIndex[3]);
    DDX_Control(pDX, IDC_SCBJOBNO5, m_cbJobParaIndex[4]);
    DDX_Control(pDX, IDC_SCBJOBNO6, m_cbJobParaIndex[5]);
    DDX_Control(pDX, IDC_SCBJOBNAME1, m_cbJobParaName[0]);
    DDX_Control(pDX, IDC_SCBJOBNAME2, m_cbJobParaName[1]);
    DDX_Control(pDX, IDC_SCBJOBNAME3, m_cbJobParaName[2]);
    DDX_Control(pDX, IDC_SCBJOBNAME4, m_cbJobParaName[3]);
    DDX_Control(pDX, IDC_SCBJOBNAME5, m_cbJobParaName[4]);
    DDX_Control(pDX, IDC_SCBJOBNAME6, m_cbJobParaName[5]);
    DDX_Control(pDX, IDC_SCBINFONO1, m_cbInfoParaIndex[0]);
    DDX_Control(pDX, IDC_SCBINFONO2, m_cbInfoParaIndex[1]);
    DDX_Control(pDX, IDC_SCBINFONAME1, m_cbInfoParaName[0]);
    DDX_Control(pDX, IDC_SCBINFONAME2, m_cbInfoParaName[1]);
}


BEGIN_MESSAGE_MAP(CDlgXlsStatSet, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDlgXlsStatSet::OnBnClickedOk)
    ON_CBN_KILLFOCUS(IDC_SCBGENNO1, &CDlgXlsStatSet::OnCbnKillfocusScbgenno1)
    ON_CBN_KILLFOCUS(IDC_SCBGENNO2, &CDlgXlsStatSet::OnCbnKillfocusScbgenno2)
    ON_CBN_KILLFOCUS(IDC_SCBGENNO3, &CDlgXlsStatSet::OnCbnKillfocusScbgenno3)
    ON_CBN_KILLFOCUS(IDC_SCBGENNO4, &CDlgXlsStatSet::OnCbnKillfocusScbgenno4)
    ON_CBN_KILLFOCUS(IDC_SCBGENNO5, &CDlgXlsStatSet::OnCbnKillfocusScbgenno5)
    ON_CBN_KILLFOCUS(IDC_SCBGENNAME1, &CDlgXlsStatSet::OnCbnKillfocusScbgenname1)
    ON_CBN_KILLFOCUS(IDC_SCBGENNAME2, &CDlgXlsStatSet::OnCbnKillfocusScbgenname2)
    ON_CBN_KILLFOCUS(IDC_SCBGENNAME3, &CDlgXlsStatSet::OnCbnKillfocusScbgenname3)
    ON_CBN_KILLFOCUS(IDC_SCBGENNAME4, &CDlgXlsStatSet::OnCbnKillfocusScbgenname4)
    ON_CBN_KILLFOCUS(IDC_SCBGENNAME5, &CDlgXlsStatSet::OnCbnKillfocusScbgenname5)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNO1, &CDlgXlsStatSet::OnCbnKillfocusScbjobno1)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNO2, &CDlgXlsStatSet::OnCbnKillfocusScbjobno2)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNO3, &CDlgXlsStatSet::OnCbnKillfocusScbjobno3)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNO4, &CDlgXlsStatSet::OnCbnKillfocusScbjobno4)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNO5, &CDlgXlsStatSet::OnCbnKillfocusScbjobno5)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNO6, &CDlgXlsStatSet::OnCbnKillfocusScbjobno6)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNAME1, &CDlgXlsStatSet::OnCbnKillfocusScbjobname1)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNAME2, &CDlgXlsStatSet::OnCbnKillfocusScbjobname2)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNAME3, &CDlgXlsStatSet::OnCbnKillfocusScbjobname3)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNAME4, &CDlgXlsStatSet::OnCbnKillfocusScbjobname4)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNAME5, &CDlgXlsStatSet::OnCbnKillfocusScbjobname5)
    ON_CBN_KILLFOCUS(IDC_SCBJOBNAME6, &CDlgXlsStatSet::OnCbnKillfocusScbjobname6)
    ON_CBN_KILLFOCUS(IDC_SCBINFONO1, &CDlgXlsStatSet::OnCbnKillfocusScbinfono1)
    ON_CBN_KILLFOCUS(IDC_SCBINFONO2, &CDlgXlsStatSet::OnCbnKillfocusScbinfono2)
    ON_CBN_KILLFOCUS(IDC_SCBINFONAME1, &CDlgXlsStatSet::OnCbnKillfocusScbinfoname1)
    ON_CBN_KILLFOCUS(IDC_SCBINFONAME2, &CDlgXlsStatSet::OnCbnKillfocusScbinfoname2)
END_MESSAGE_MAP()

// CDlgXlsStatSet 消息处理程序
BOOL CDlgXlsStatSet::OnInitDialog()
{
    int     i = 0;

    CDialog::OnInitDialog();

    g_ptLast   = &g_tReadData.tData[g_tReadData.nTotal-1];
    m_ptStat   = &theApp.m_tXlsStatCfg[g_tGlbCfg.nLangType];
    m_iShowNum = g_ptLast->tshow_size();
    if (m_iShowNum > MAXPARANUM)
        m_iShowNum = MAXPARANUM;

    m_clrNormal  = RGB(0, 0, 0); 
    m_clrChanged = RGB(255, 0, 0);

    for (i = 0; i < STATPARA_GENNUM; i++) {
        InitCBContent(&m_cbGenParaIndex[i], m_ptStat->GenPara[i]);
    }
    for (i = 0; i < STATPARA_JOBNUM; i++) {
        InitCBContent(&m_cbJobParaIndex[i], m_ptStat->JobPara[i]);
    }
    for (i = 0; i < STATPARA_INFONUM; i++) {
        InitCBContent(&m_cbInfoParaIndex[i], m_ptStat->InfoPara[i]);
    }

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CDlgXlsStatSet::OnBnClickedOk()
{
    int     i       = 0;
    int     iSummary= 0;
    int     iReport = 0;

    UpdateData(TRUE);

    theDB.UpdateXlsStatPara(m_ptStat);

    CDialog::OnOK();
}


void CDlgXlsStatSet::InitCBContent(CComboBox* ptCB, int index, bool bName)
{
    int i = 0;
    ASSERT_NULL(ptCB);
    if (index < 0 || index > MAXPARANUM)
        return;
    ptCB->Clear();
    if (!bName) {
        for (i = 1; i <= MAXPARANUM; i++) {
            ptCB->AddString(to_string(i).c_str());

        ptCB->SetCurSel(index);
        }
        return;
    }
}

void CDlgXlsStatSet::OnCbnKillfocusScbgenno1()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbgenno2()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbgenno3()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbgenno4()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbgenno5()
{
    // TODO: Add your control notification handler code here
}

void CDlgXlsStatSet::OnCbnKillfocusScbgenname1()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbgenname2()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbgenname3()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbgenname4()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbgenname5()
{
    // TODO: Add your control notification handler code here
}

void CDlgXlsStatSet::OnCbnKillfocusScbjobno1()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbjobno2()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbjobno3()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbjobno4()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbjobno5()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbjobno6()
{
    // TODO: Add your control notification handler code here
}


void CDlgXlsStatSet::OnCbnKillfocusScbjobname1()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbjobname2()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbjobname3()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbjobname4()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbjobname5()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbjobname6()
{
    // TODO: Add your control notification handler code here
}


void CDlgXlsStatSet::OnCbnKillfocusScbinfono1()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbinfono2()
{
    // TODO: Add your control notification handler code here
}


void CDlgXlsStatSet::OnCbnKillfocusScbinfoname1()
{
    // TODO: Add your control notification handler code here
}
void CDlgXlsStatSet::OnCbnKillfocusScbinfoname2()
{
    // TODO: Add your control notification handler code here
}
