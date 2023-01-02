// DlgMainShowSet.cpp : implementation file
//

#include "stdafx.h"
#include "Torque.h"
#include "DlgMainShowSet.h"
#include "afxdialogex.h"


//extern CTubeCfg    g_cTubing;
// CDlgMainShowSet dialog

IMPLEMENT_DYNAMIC(CDlgMainShowSet, CDialogEx)

CDlgMainShowSet::CDlgMainShowSet(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DLGMAINSHOW, pParent)
{
}

CDlgMainShowSet::~CDlgMainShowSet()
{
}

void CDlgMainShowSet::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_MAINPARANAME1, m_strMainShow[0]);
    DDX_Text(pDX, IDC_MAINPARANAME2, m_strMainShow[1]);
    DDX_Text(pDX, IDC_MAINPARANAME3, m_strMainShow[2]);
    DDX_Text(pDX, IDC_MAINPARANAME4, m_strMainShow[3]);
    DDX_Text(pDX, IDC_MAINPARANAME5, m_strMainShow[4]);
    DDX_Text(pDX, IDC_MAINPARANAME6, m_strMainShow[5]);
    DDX_Text(pDX, IDC_MAINPARANAME7, m_strMainShow[6]);
    DDX_Text(pDX, IDC_MAINPARANAME8, m_strMainShow[7]);
    DDX_Control(pDX, IDC_CBMAINPARA1, m_cbMainOption[0]);
    DDX_Control(pDX, IDC_CBMAINPARA2, m_cbMainOption[1]);
    DDX_Control(pDX, IDC_CBMAINPARA3, m_cbMainOption[2]);
    DDX_Control(pDX, IDC_CBMAINPARA4, m_cbMainOption[3]);
    DDX_Control(pDX, IDC_CBMAINPARA5, m_cbMainOption[4]);
    DDX_Control(pDX, IDC_CBMAINPARA6, m_cbMainOption[5]);
    DDX_Control(pDX, IDC_CBMAINPARA7, m_cbMainOption[6]);
    DDX_Control(pDX, IDC_CBMAINPARA8, m_cbMainOption[7]);
    DDX_CBString(pDX, IDC_CBMAINPARA1, m_strMainOption[0]);
    DDX_CBString(pDX, IDC_CBMAINPARA2, m_strMainOption[1]);
    DDX_CBString(pDX, IDC_CBMAINPARA3, m_strMainOption[2]);
    DDX_CBString(pDX, IDC_CBMAINPARA4, m_strMainOption[3]);
    DDX_CBString(pDX, IDC_CBMAINPARA5, m_strMainOption[4]);
    DDX_CBString(pDX, IDC_CBMAINPARA6, m_strMainOption[5]);
    DDX_CBString(pDX, IDC_CBMAINPARA7, m_strMainOption[6]);
    DDX_CBString(pDX, IDC_CBMAINPARA8, m_strMainOption[7]);
}


BEGIN_MESSAGE_MAP(CDlgMainShowSet, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDlgMainShowSet::OnBnClickedOk)
    ON_WM_CTLCOLOR()
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA1, &CDlgMainShowSet::OnCbnKillfocusCbmainpara1)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA2, &CDlgMainShowSet::OnCbnKillfocusCbmainpara2)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA3, &CDlgMainShowSet::OnCbnKillfocusCbmainpara3)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA4, &CDlgMainShowSet::OnCbnKillfocusCbmainpara4)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA5, &CDlgMainShowSet::OnCbnKillfocusCbmainpara5)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA6, &CDlgMainShowSet::OnCbnKillfocusCbmainpara6)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA7, &CDlgMainShowSet::OnCbnKillfocusCbmainpara7)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA8, &CDlgMainShowSet::OnCbnKillfocusCbmainpara8)
END_MESSAGE_MAP()


// CDlgMainShowSet message handlers

BOOL CDlgMainShowSet::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_nCurLang = g_tGlbCfg.nLangType;

    InitMainShowPara();

    //InitFixShowPara();

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CDlgMainShowSet::InitMainShowPara()
{
    WORD    i = 0;
    WORD    j = 0;
    WORD    k = 0;
    vector<int> lsShowIndex;
    vector<string> lsOption;

    m_clrNormal = RGB(0, 0, 0);
    m_clrChanged = RGB(255, 0, 0);

    lsShowIndex = theDB.ReadCurOptionIndex(2);
    for (i = 0; i < m_ptShow->nMainNum && i < MAXMAINPARA; i++)
    {
        m_strMainShow[i] = theApp.GetMainShowName(theApp.m_ptCurShow, i).c_str();
        if (i == MAINSHOWTALLY)  //入井序号跳过
            continue;

        m_cbMainOption[i].ResetContent();

        lsOption.clear();

        lsOption = theDB.ReadOptionsByShowIndex(lsShowIndex[i]);
        for (j = 0; j < lsOption.size(); j++)
        {
            m_cbMainOption[i].AddString(lsOption[j].c_str());
            if (theApp.m_tParaCfg.strValue[m_ptShow->nMain[i]].c_str() == lsOption[j])
            {
                m_cbMainOption[i].SetCurSel(j);
                m_strMainOption[i] = lsOption[j].c_str();
            }
        }
    }
}

BOOL CDlgMainShowSet::GetParaValue()
{
    UINT        i = 0;
    UINT        j = 0;
    UINT        nFactory = 0;

    for (i = 0; i <= m_ptShow->nMainNum && i < MAXMAINPARA; i++)
    {
        if (i == MAINSHOWTALLY)  // 第6个为入井序号，不需要使用combobox
            continue;
        m_tempCfg.strValue[m_ptShow->nMain[i]] = m_strMainOption[i];
    }
    return TRUE;
}

void CDlgMainShowSet::OnBnClickedOk()
{
    UpdateData(TRUE);

    GetParaValue();

    theDB.UpdateTorqCfgPara(&m_tempCfg, theApp.m_ptCurShow);

    UpdateData(FALSE);

    CDialogEx::OnOK();
}

HBRUSH CDlgMainShowSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    //COLORREF    clrCtrl;
    CString     strContent;

    GetDlgItemText(pWnd->GetDlgCtrlID(), strContent);
    if (strContent.IsEmpty())
        return hbr;

    // TODO:  在此更改 DC 的任何特性
    //switch (pWnd->GetDlgCtrlID())//对某一个特定控件进行判断  
    //{
    ///*case IDC_EDMAINPARA52:
    //    JUDGE_STRPARA_CHANGE(strContent, m_strTubeNO2);
    //    break;*/
    //default:
    //    hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    //    break;
    //}
    hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void CDlgMainShowSet::JudgeMainShowParaChanged(UINT nIndex, CString strContent)
{
    COLORREF    clrCtrl;

    COMP_BGE(nIndex, MAXMAINPARA);
    COMP_BGE(nIndex, m_ptShow->nMainNum);

    clrCtrl = m_clrNormal;
    if (strContent.Compare(m_strMainOption[nIndex]) != 0)
        clrCtrl = m_clrChanged;
    m_cbMainOption[nIndex].m_ColorText = clrCtrl;
    m_cbMainOption[nIndex].Invalidate();
}

void CDlgMainShowSet::OnCbnKillfocusCbmainpara1()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA1, strContent);
    JudgeMainShowParaChanged(0, strContent);
}
void CDlgMainShowSet::OnCbnKillfocusCbmainpara2()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA2, strContent);
    JudgeMainShowParaChanged(1, strContent);
}
void CDlgMainShowSet::OnCbnKillfocusCbmainpara3()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA3, strContent);
    JudgeMainShowParaChanged(2, strContent);
}
void CDlgMainShowSet::OnCbnKillfocusCbmainpara4()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA4, strContent);
    JudgeMainShowParaChanged(3, strContent);
}
void CDlgMainShowSet::OnCbnKillfocusCbmainpara5()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA5, strContent);
    JudgeMainShowParaChanged(4, strContent);
}
void CDlgMainShowSet::OnCbnKillfocusCbmainpara6()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA6, strContent);
    JudgeMainShowParaChanged(5, strContent);
}
void CDlgMainShowSet::OnCbnKillfocusCbmainpara7()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA7, strContent);
    JudgeMainShowParaChanged(6, strContent);
}
void CDlgMainShowSet::OnCbnKillfocusCbmainpara8()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA8, strContent);
    JudgeMainShowParaChanged(7, strContent);
}
