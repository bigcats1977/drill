// DlgMainShowSet.cpp : implementation file
//

#include "stdafx.h"
#include "Torque.h"
#include "DlgMainShowSet.h"
#include "afxdialogex.h"


/* 7个参数, 1~6固定 */
#define         MAINSHOWBEGIN           0   /* 第1个显示参数固定(1-1) */
#define         MAINSHOWEND             3   /* 第4个显示参数固定(4-1) */

#define         CBMAINSHOW_OEM          0
#define         CBMAINSHOW_SIZE         1
#define         CBMAINSHOW_MAT          2
#define         CBMAINSHOW_COUPL        3

// CDlgMainShowSet dialog

IMPLEMENT_DYNAMIC(CDlgMainShowSet, CDialogEx)

CDlgMainShowSet::CDlgMainShowSet(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DLGMAINSHOW, pParent)
{
    m_bFixedTubing  = TRUE;
}

CDlgMainShowSet::~CDlgMainShowSet()
{
}

void CDlgMainShowSet::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_MAINPARANAME0, m_strMainShow[0]);
    DDX_Text(pDX, IDC_MAINPARANAME1, m_strMainShow[1]);
    DDX_Text(pDX, IDC_MAINPARANAME2, m_strMainShow[2]);
    DDX_Text(pDX, IDC_MAINPARANAME3, m_strMainShow[3]);
    DDX_Text(pDX, IDC_MAINPARANAME4, m_strMainShow[4]);
    DDX_Text(pDX, IDC_MAINPARANAME5, m_strMainShow[5]);
    DDX_Text(pDX, IDC_MAINPARANAME6, m_strMainShow[6]);
    DDX_Control(pDX, IDC_CBMAINPARA0, m_cbMainOption[0]);
    DDX_Control(pDX, IDC_CBMAINPARA1, m_cbMainOption[1]);
    DDX_Control(pDX, IDC_CBMAINPARA2, m_cbMainOption[2]);
    DDX_Control(pDX, IDC_CBMAINPARA3, m_cbMainOption[3]);
    DDX_Control(pDX, IDC_CBMAINPARA4, m_cbMainOption[4]);
    //DDX_Control(pDX, IDC_CBMAINPARA5, m_cbMainOption[5]);
    DDX_Control(pDX, IDC_CBMAINPARA6, m_cbMainOption[6]);
    DDX_CBString(pDX, IDC_CBMAINPARA0, m_strMainOption[0]);
    DDX_CBString(pDX, IDC_CBMAINPARA1, m_strMainOption[1]);
    DDX_CBString(pDX, IDC_CBMAINPARA2, m_strMainOption[2]);
    DDX_CBString(pDX, IDC_CBMAINPARA3, m_strMainOption[3]);
    DDX_CBString(pDX, IDC_CBMAINPARA4, m_strMainOption[4]);
    //DDX_CBString(pDX, IDC_EDMAINPARA5, m_strMainOption[5]);
    DDX_CBString(pDX, IDC_CBMAINPARA6, m_strMainOption[6]);
    DDX_Text(pDX, IDC_EDMAINPARA42, m_strTubeNO2);
    DDX_Control(pDX, IDC_CBMAINPARAFACTORY, m_cbMainFactory);
    DDX_CBString(pDX, IDC_CBMAINPARAFACTORY, m_strMainFactory);
    DDX_Control(pDX, IDC_CBMFIXPARAFACTORY, m_cbMFixFactory);
    DDX_CBString(pDX, IDC_CBMFIXPARAFACTORY, m_strMFixFactory);
    DDX_Check(pDX, IDC_CKMAINFIXED, m_bFixedTubing);
    DDX_Control(pDX, IDC_CBMFIXPARA0, m_cbMFixOption[0]);
    DDX_Control(pDX, IDC_CBMFIXPARA1, m_cbMFixOption[1]);
    DDX_Control(pDX, IDC_CBMFIXPARA2, m_cbMFixOption[2]);
    DDX_Control(pDX, IDC_CBMFIXPARA3, m_cbMFixOption[3]);
    DDX_CBString(pDX, IDC_CBMFIXPARA0, m_strMFixOption[0]);
    DDX_CBString(pDX, IDC_CBMFIXPARA1, m_strMFixOption[1]);
    DDX_CBString(pDX, IDC_CBMFIXPARA2, m_strMFixOption[2]);
    DDX_CBString(pDX, IDC_CBMFIXPARA3, m_strMFixOption[3]);
}


BEGIN_MESSAGE_MAP(CDlgMainShowSet, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDlgMainShowSet::OnBnClickedOk)
    ON_WM_CTLCOLOR()
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA0, &CDlgMainShowSet::OnCbnKillfocusCbmainpara0)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA1, &CDlgMainShowSet::OnCbnKillfocusCbmainpara1)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA2, &CDlgMainShowSet::OnCbnKillfocusCbmainpara2)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA3, &CDlgMainShowSet::OnCbnKillfocusCbmainpara3)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA4, &CDlgMainShowSet::OnCbnKillfocusCbmainpara4)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARA6, &CDlgMainShowSet::OnCbnKillfocusCbmainpara6)
    ON_CBN_KILLFOCUS(IDC_CBMAINPARAFACTORY, &CDlgMainShowSet::OnCbnKillfocusCbmainfactory)
    ON_BN_CLICKED(IDC_CKMAINFIXED, &CDlgMainShowSet::OnBnClickedCkmainfixed)
    ON_CBN_KILLFOCUS(IDC_CBMFIXPARA0, &CDlgMainShowSet::OnCbnKillfocusCbmfixpara0)
    ON_CBN_KILLFOCUS(IDC_CBMFIXPARA1, &CDlgMainShowSet::OnCbnKillfocusCbmfixpara1)
    ON_CBN_KILLFOCUS(IDC_CBMFIXPARA2, &CDlgMainShowSet::OnCbnKillfocusCbmfixpara2)
    ON_CBN_KILLFOCUS(IDC_CBMFIXPARA3, &CDlgMainShowSet::OnCbnKillfocusCbmfixpara3)
    ON_CBN_KILLFOCUS(IDC_CBMFIXPARAFACTORY, &CDlgMainShowSet::OnCbnKillfocusCbmfixfactory)
    ON_CBN_SELCHANGE(IDC_CBMFIXPARA0, &CDlgMainShowSet::OnCbnSelchangeCbmfixpara0)
    ON_CBN_SELCHANGE(IDC_CBMFIXPARA1, &CDlgMainShowSet::OnCbnSelchangeCbmfixpara1)
    ON_CBN_SELCHANGE(IDC_CBMFIXPARA2, &CDlgMainShowSet::OnCbnSelchangeCbmfixpara2)
    ON_CBN_SELCHANGE(IDC_CBMFIXPARA3, &CDlgMainShowSet::OnCbnSelchangeCbmfixpara3)
    ON_CBN_SELCHANGE(IDC_CBMFIXPARAFACTORY, &CDlgMainShowSet::OnCbnSelchangeCbmfixfactory)
END_MESSAGE_MAP()


// CDlgMainShowSet message handlers

BOOL CDlgMainShowSet::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_nCurLang = theApp.m_nLangType;

    InitMainShowPara();

    InitFixShowPara();

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CDlgMainShowSet::InitMainShowPara()
{
    WORD    i = 0;
    WORD    j = 0;
    WORD    k = 0;

    m_clrNormal  = RGB(0, 0, 0); 
    m_clrChanged = RGB(255, 0, 0);

    m_bFixedTubing = m_tempShow.bFixTub;

    for(i=0; i<m_tempShow.wMainNum && i<MAXMAINPARA; i++)
    {
        m_strMainShow[i] = m_tempShow.strMain[i].c_str();

        if(i == MAINSHOWWELL)  // 第6个为入井序号，不需要使用combobox
            continue;

        for(j=0; j<m_tempShow.wParaNum && j<MAXPARANUM; j++)
        {
            if (0 == m_strMainShow[i].CompareNoCase(m_tempShow.tShow[j].strName.c_str()))
            {
                for(k=0; k<m_tempShow.tOption[j].wOptNum && k<MAXOPTIONNUM; k++)
                {
                    m_cbMainOption[i].AddString(m_tempShow.tOption[j].strOpt[k].c_str());
                }

                m_strMainOption[i] = m_tempShow.tShow[j].strValue.c_str();
                if(m_strMainOption[i].IsEmpty() && m_tempShow.tOption[i].wOptNum > 0)
                    m_strMainOption[i] = m_tempShow.tOption[j].strOpt[0].c_str();

                break;
            }
        }
    }

    /* factory单独处理，直接取第15个 */
    for(k=0; k<m_tempShow.tOption[15].wOptNum && k<MAXOPTIONNUM; k++)
    {
        m_cbMainFactory.AddString(m_tempShow.tOption[15].strOpt[k].c_str());
    }
    m_strMainFactory  = m_tempShow.tShow[15].strValue.c_str();
}

void CDlgMainShowSet::InitFixShowPara()
{
    m_cTubing.SetFactoryBox(&m_cbMFixFactory, &m_tCurTub, m_nCurLang);
    m_cTubing.SetOEMBox(&m_cbMFixOption[CBMAINSHOW_OEM], &m_tCurTub, m_nCurLang);
    m_cTubing.SetSizeBox(&m_cbMFixOption[CBMAINSHOW_SIZE], &m_tCurTub, m_nCurLang);
    m_cTubing.SetMatBox(&m_cbMFixOption[CBMAINSHOW_MAT], &m_tCurTub, m_nCurLang);
    m_cTubing.SetCouplBox(&m_cbMFixOption[CBMAINSHOW_COUPL], &m_tCurTub, m_nCurLang);

    ShowTubingCtrl();
}

void CDlgMainShowSet::ShowTubingCtrl()
{
    m_cbMainFactory.ShowWindow(!m_bFixedTubing);
    m_cbMainOption[CBMAINSHOW_OEM].ShowWindow(!m_bFixedTubing);
    m_cbMainOption[CBMAINSHOW_SIZE].ShowWindow(!m_bFixedTubing);
    m_cbMainOption[CBMAINSHOW_MAT].ShowWindow(!m_bFixedTubing);
    m_cbMainOption[CBMAINSHOW_COUPL].ShowWindow(!m_bFixedTubing);

    m_cbMFixFactory.ShowWindow(m_bFixedTubing);
    m_cbMFixOption[CBMAINSHOW_OEM].ShowWindow(m_bFixedTubing);
    m_cbMFixOption[CBMAINSHOW_SIZE].ShowWindow(m_bFixedTubing);
    m_cbMFixOption[CBMAINSHOW_MAT].ShowWindow(m_bFixedTubing);
    m_cbMFixOption[CBMAINSHOW_COUPL].ShowWindow(m_bFixedTubing);
}

BOOL CDlgMainShowSet::GetParaValue(SHOWCFG *ptShow)
{
    WORD        i = 0;
    WORD        j = 0;

    ASSERT_NULL_R(ptShow, FALSE);
    
    for (i = 0; i<ptShow->wMainNum && i<MAXMAINPARA; i++)
    {
        if(i == MAINSHOWWELL)  // 第6个为入井序号，不需要使用combobox
            continue;
        for (j = 0; j<ptShow->wParaNum && j<MAXPARANUM; j++)
        {
            if (0 == m_strMainShow[i].CompareNoCase(m_tempShow.tShow[j].strName.c_str()))
            {
                ptShow->tShow[j].strValue = m_strMainOption[i];
                break;
            }
        }
    }
    
    /* factory单独处理，直接取第15个 */
    ptShow->tShow[15].strValue = m_strMainFactory;

    ptShow->bFixTub      = m_bFixedTubing;

    return TRUE;
}

void CDlgMainShowSet::CheckShowOption(SHOWCFG *ptShow)
{
    WORD    i           = 0;
    WORD    j           = 0;
    WORD    k           = 0;
    WORD    wMainID     = 0;
    BOOL    bFindStr    = FALSE;
    CString strTemp;

     ASSERT_NULL(ptShow);

    for (i = 0; i<ptShow->wMainNum && i<MAXMAINPARA; i++)
    {
        if(i == MAINSHOWWELL)  // 第6个为入井序号，不需要使用combobox
            continue;
        for(j=0; j<ptShow->wParaNum && j<MAXPARANUM; j++)
        {
            if (0 == m_strMainShow[i].CompareNoCase(m_tempShow.tShow[j].strName.c_str()))
            {
                bFindStr = FALSE;
                if(m_strMainOption[i].IsEmpty())
                    continue;
                for(k=0; k<ptShow->tOption[j].wOptNum && k<MAXOPTIONNUM; k++)
                {
                    strTemp = ptShow->tOption[j].strOpt[k].c_str();
                    if(m_strMainOption[i].CompareNoCase(strTemp) == 0)
                    {
                        bFindStr = TRUE;
                        break;
                    }
                }

                if(!bFindStr)
                {
                    theApp.InsertShowOption(ptShow, j, m_strMainOption[i], &m_cbMainOption[i]);
                }
                break;
            }
        }
    }
    return;
}

void CDlgMainShowSet::OnBnClickedOk()
{
    UpdateData(TRUE);

    GetParaValue(&m_tempShow);

    CheckShowOption(&m_tempShow);

    UpdateData(FALSE);

    CDialogEx::OnOK();
}

HBRUSH CDlgMainShowSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    COLORREF    clrCtrl;
    CString     strContent;

    GetDlgItemText(pWnd->GetDlgCtrlID(), strContent);
    if (strContent.IsEmpty())
        return hbr;

    // TODO:  在此更改 DC 的任何特性
    switch (pWnd->GetDlgCtrlID())//对某一个特定控件进行判断  
    {
    case IDC_EDMAINPARA42:
        JUDGE_STRPARA_CHANGE(strContent, m_strTubeNO2);
        break;
    default:
        hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
        break;
    }
    
    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}


void CDlgMainShowSet::JudgeMainShowParaChanged(UINT nIndex, CString strContent)
{
    COLORREF    clrCtrl;

    COMP_BGE(nIndex, MAXMAINPARA);
    COMP_BGE(nIndex, m_tempShow.wMainNum);

    clrCtrl = m_clrNormal;
    if (strContent.Compare(m_strMainOption[nIndex]) != 0)
        clrCtrl = m_clrChanged;
    m_cbMainOption[nIndex].m_ColorText = clrCtrl;
    m_cbMainOption[nIndex].Invalidate();
}

void CDlgMainShowSet::OnCbnKillfocusCbmainpara0()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA0, strContent);
    JudgeMainShowParaChanged(0, strContent);
}

void CDlgMainShowSet::OnCbnKillfocusCbmainpara1()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA1, strContent);
    JudgeMainShowParaChanged(1, strContent);
}

void CDlgMainShowSet::OnCbnKillfocusCbmainpara2()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA2, strContent);
    JudgeMainShowParaChanged(2, strContent);
}

void CDlgMainShowSet::OnCbnKillfocusCbmainpara3()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA3, strContent);
    JudgeMainShowParaChanged(3, strContent);
}

void CDlgMainShowSet::OnCbnKillfocusCbmainpara4()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA4, strContent);
    JudgeMainShowParaChanged(4, strContent);
}

void CDlgMainShowSet::OnCbnKillfocusCbmainpara6()
{
    CString     strContent;
    GetDlgItemText(IDC_CBMAINPARA6, strContent);
    JudgeMainShowParaChanged(6, strContent);
}

void CDlgMainShowSet::OnCbnKillfocusCbmainfactory()
{
    CString     strContent;    
    COLORREF    clrCtrl;

    GetDlgItemText(IDC_CBMAINPARAFACTORY, strContent);

    clrCtrl = m_clrNormal;
    if (strContent.Compare(m_strMainFactory) != 0)
        clrCtrl = m_clrChanged;
    m_cbMainFactory.m_ColorText = clrCtrl;
    m_cbMainFactory.Invalidate();
}

void CDlgMainShowSet::OnCbnKillfocusCbmfixpara0()
{
    JudgeFixShowParaChanged(0);
}

void CDlgMainShowSet::OnCbnKillfocusCbmfixpara1()
{
    JudgeFixShowParaChanged(1);
}

void CDlgMainShowSet::OnCbnKillfocusCbmfixpara2()
{
    JudgeFixShowParaChanged(2);
}

void CDlgMainShowSet::OnCbnKillfocusCbmfixpara3()
{
    JudgeFixShowParaChanged(3);
}

void CDlgMainShowSet::OnCbnKillfocusCbmfixfactory()
{
    JudgeFixShowParaChanged(15);
}

void CDlgMainShowSet::OnCbnSelchangeCbmfixfactory()
{
    UINT    i = 0;
    UINT    nOldFactory = m_tCurTub.nFactory;
    CString strFactory;
    FIXTUBPARA  *ptTubPara = m_cTubing.m_tFactory.ptPara;

    ASSERT_NULL(ptTubPara);

    m_cbMFixFactory.GetWindowText(strFactory);

    for (i = 0; i < m_cTubing.m_tFactory.nNum; i++)
    {
        if (0 == strFactory.CompareNoCase(ptTubPara[i].strName[m_nCurLang].c_str()))
        {
            m_tCurTub.nFactory = ptTubPara[i].nNO;
            break;
        }
    }

    if(nOldFactory == m_tCurTub.nFactory)
        return;

    m_cTubing.InitOEMValue(&m_tCurTub);
    m_cTubing.InitSizeValue(&m_tCurTub);
    m_cTubing.InitMatValue(&m_tCurTub);
    m_cTubing.InitCouplValue(&m_tCurTub);

    InitFixShowPara();
    ChangeTubingShowBox(5);
}

void CDlgMainShowSet::OnCbnSelchangeCbmfixpara0()
{
    UINT    i = 0;
    UINT    nOldOEM = m_tCurTub.nOEM;
    CString strOEM;
    FIXTUBPARA  *ptTubPara = m_cTubing.m_tOEM.ptPara;

    ASSERT_NULL(ptTubPara);

    m_cbMFixOption[CBMAINSHOW_OEM].GetWindowText(strOEM);

    for (i = 0; i < m_cTubing.m_tOEM.nNum; i++)
    {
        /* OEM有厂家名称，不用比较厂家的值 */
        if (0 == strOEM.CompareNoCase(ptTubPara[i].strName[m_nCurLang].c_str()))
        {
            m_tCurTub.nOEM = ptTubPara[i].nNO;
            break;
        }
    }

    if(nOldOEM == m_tCurTub.nOEM)
        return;

    m_cTubing.InitSizeValue(&m_tCurTub);
    m_cTubing.InitMatValue(&m_tCurTub);
    m_cTubing.InitCouplValue(&m_tCurTub);

    InitFixShowPara();
    ChangeTubingShowBox(4);
}

void CDlgMainShowSet::OnCbnSelchangeCbmfixpara1()
{
    UINT    i = 0;
    UINT    nOldSize = m_tCurTub.nSize;
    CString strSize;
    FIXTUBPARA  *ptTubPara = m_cTubing.m_tSize.ptPara;

    ASSERT_NULL(ptTubPara);

    m_cbMFixOption[CBMAINSHOW_SIZE].GetWindowText(strSize);

    for (i = 0; i < m_cTubing.m_tSize.nNum; i++)
    {
        /* 厂家ID不同，跳过 */
        if((ptTubPara[i].nNO & 0xFF00) != (nOldSize & 0xFF00))
            continue;
        if (0 == strSize.CompareNoCase(ptTubPara[i].strName[m_nCurLang].c_str()))
        {
            m_tCurTub.nSize = ptTubPara[i].nNO;
            break;
        }
    }

    if(nOldSize == m_tCurTub.nSize)
        return;

    m_cTubing.InitMatValue(&m_tCurTub);
    m_cTubing.InitCouplValue(&m_tCurTub);

    InitFixShowPara();
    ChangeTubingShowBox(3);
}

void CDlgMainShowSet::OnCbnSelchangeCbmfixpara2()
{
    UINT    i = 0;
    UINT    nOldMat = m_tCurTub.nMat;
    CString strMat;
    FIXTUBPARA  *ptTubPara = m_cTubing.m_tMater.ptPara;

    ASSERT_NULL(ptTubPara);

    m_cbMFixOption[CBMAINSHOW_MAT].GetWindowText(strMat);

    for (i = 0; i < m_cTubing.m_tMater.nNum; i++)
    {
        /* 厂家ID不同，跳过 */
        if((ptTubPara[i].nNO & 0xFF00) != (nOldMat & 0xFF00))
            continue;
        if (0 == strMat.CompareNoCase(ptTubPara[i].strName[m_nCurLang].c_str()))
        {
            m_tCurTub.nMat = ptTubPara[i].nNO;
            break;
        }
    }

    if(nOldMat == m_tCurTub.nMat)
        return;

    m_cTubing.InitCouplValue(&m_tCurTub);

    InitFixShowPara();
    ChangeTubingShowBox(2);
}

void CDlgMainShowSet::OnCbnSelchangeCbmfixpara3()
{
    UINT    i = 0;
    UINT    nOldCoupl = m_tCurTub.nCoupling;
    CString strCoupl;
    FIXTUBPARA  *ptTubPara = m_cTubing.m_tCoupl.ptPara;

    ASSERT_NULL(ptTubPara);

    m_cbMFixOption[CBMAINSHOW_COUPL].GetWindowText(strCoupl);

    for (i = 0; i < m_cTubing.m_tCoupl.nNum; i++)
    {
        /* 厂家ID不同，跳过 */
        if((ptTubPara[i].nNO & 0xFF00) != (nOldCoupl & 0xFF00))
            continue;
        if (0 == strCoupl.CompareNoCase(ptTubPara[i].strName[m_nCurLang].c_str()))
        {
            m_tCurTub.nCoupling = ptTubPara[i].nNO;
            break;
        }
    }

    if(nOldCoupl == m_tCurTub.nCoupling)
        return;

    ChangeTubingShowBox(1);
}

void CDlgMainShowSet::JudgeFixShowParaChanged(UINT nIndex)
{
    COLORREF    clrCtrl;
    BOOL        bChanged = FALSE;
    TUBINGCFG   *ptOldTubing = &theApp.m_ptCurShow->tTubingCfg;

    clrCtrl = m_clrNormal;

    if (theApp.m_ptCurShow->bFixTub != m_bFixedTubing)
        bChanged = TRUE;
    else
    {
        switch(nIndex)
        {
            case 0:
                if(m_tCurTub.nOEM != ptOldTubing->nOEM)
                    bChanged = TRUE;
                break;
            case 1:
                if(m_tCurTub.nSize != ptOldTubing->nSize)
                    bChanged = TRUE;
                break;
            case 2:
                if(m_tCurTub.nMat != ptOldTubing->nMat)
                    bChanged = TRUE;
                break;
            case 3:
                if(m_tCurTub.nCoupling != ptOldTubing->nCoupling)
                    bChanged = TRUE;
                break;
            case 15:
                if(m_tCurTub.nFactory != ptOldTubing->nFactory)
                    bChanged = TRUE;
                break;
            default:
                return;
        }
    }

    if (bChanged)
        clrCtrl = m_clrChanged;
    if(nIndex < MAXMAINPARA)
    {
        m_cbMFixOption[nIndex].m_ColorText = clrCtrl;
        m_cbMFixOption[nIndex].Invalidate();
    }
    else
    {
        m_cbMFixFactory.m_ColorText = clrCtrl;
        m_cbMFixFactory.Invalidate();
    }
}

/*
    5: factory
    4: OEM
    3: Size
    2: Mat
    1: Coupling
*/
void CDlgMainShowSet::ChangeTubingShowBox(UINT nNum)
{
    switch(nNum)
    {
        case 5:
            JudgeFixShowParaChanged(15);
        case 4:
            JudgeFixShowParaChanged(0);
        case 3:
            JudgeFixShowParaChanged(1);
        case 2:
            JudgeFixShowParaChanged(2);
        case 1:
            JudgeFixShowParaChanged(3);
        default:
            break;
    }
}

void CDlgMainShowSet::OnBnClickedCkmainfixed()
{
    int i = 0;

    UpdateData(TRUE);
    ShowTubingCtrl();

    /* 自选管材转固定管材 */
    if (m_bFixedTubing && !theApp.m_ptCurShow->bFixTub)
    {
        ChangeTubingShowBox(5);
    }
    
    /* 固定管材转自选管材 */
    if(!m_bFixedTubing && theApp.m_ptCurShow->bFixTub)
    {
        for(i=MAINSHOWBEGIN; i<=MAINSHOWEND; i++)
        {
            m_cbMainOption[i].m_ColorText = m_clrChanged;
            m_cbMainOption[i].Invalidate();
        }
        m_cbMainFactory.m_ColorText = m_clrChanged;
        m_cbMainFactory.Invalidate();
    }
    UpdateData(FALSE);
}
