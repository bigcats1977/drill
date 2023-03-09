// DlgListMod.cpp : implementation file
//

#include "stdafx.h"
#include "DataMod.h"
#include "DlgListMod.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgListMod dialog

BOOL(CDlgListMod::* g_OptimitProc[MAX_OPTIM_METH_NUM])(TorqData::Torque* pSrcData, TorqData::Torque* pDestData) =
{
    /*  0 */    &CDlgListMod::CtrlTorqOptim,    /* 控制扭矩优化 */
    /*  1 */    &CDlgListMod::CircleOptim,      /* 周数优化 */
    /*  2 */    &CDlgListMod::ModDataOptim,     /* 直接修改数据优化 */
    /*  3 */    &CDlgListMod::OverlapOptim,     /* 重叠图形优化 */
};


CDlgListMod::CDlgListMod(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgListMod::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgListMod)
    m_fCir = 0.0f;
    m_nTorque = 0;
    m_iMethod = OPTIM_METH_CTRLTORQ;
    m_bQuality = TRUE;
    //}}AFX_DATA_INIT
}


void CDlgListMod::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgListMod)
    DDX_Control(pDX, IDC_LISTDATA, m_lsData);
    DDX_Text(pDX, IDC_EDITCIRCLE, m_fCir);
    DDX_Radio(pDX, IDC_RADIOCONTROL, m_iMethod);
    DDX_Text(pDX, IDC_EDITCTRL, m_nTorque);
    DDX_Check(pDX, IDC_CHECKQUALITY, m_bQuality);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgListMod, CDialog)
    //{{AFX_MSG_MAP(CDlgListMod)
    ON_NOTIFY(NM_CLICK, IDC_LISTDATA, OnClickListdata)
    ON_BN_CLICKED(ID_OPTIMITED, OnOptimited)
    ON_BN_CLICKED(IDC_RADIOCONTROL, OnRadiocontrol)
    ON_BN_CLICKED(IDC_RADIOCIRCLE, OnRadiocircle)
    ON_BN_CLICKED(IDC_RADIODETAIL, OnRadiodetail)
    ON_BN_CLICKED(IDC_RADIOOVERLAP, OnRadiooverlap)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgListMod message handlers

BOOL CDlgListMod::OnInitDialog()
{
    DWORD dwStyle = 0;
    int   nCount = 0;

    CDialog::OnInitDialog();

    /* 扭矩数据无效，直接返回 */
    ASSERT_NULL_R(m_ptModTorq, FALSE);
    nCount = m_ptModTorq->ftorque_size();
    ASSERT_ZERO_R(nCount, FALSE);

    m_pdlgParent = (CDlgHisGrp*)GetParent();

    dwStyle = m_lsData.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
    dwStyle |= LVS_EX_GRIDLINES;    //网格线（只适用与report风格的listctrl）
    //dwStyle |= LVS_EX_CHECKBOXES; //item前生成checkbox控件
    m_lsData.SetExtendedStyle(dwStyle); //设置扩展风格

    m_lceEdit.CreateEx(this, &m_lsData, ES_CENTER);

    m_lsData.InsertColumn(0, "序号", LVCFMT_CENTER, 50);//插入列
    m_lsData.InsertColumn(1, "扭矩", LVCFMT_CENTER, 135);
    m_lsData.InsertColumn(2, "转速", LVCFMT_CENTER, 135);
    //m_lsData.InsertColumn(3, "脉冲", LVCFMT_CENTER, 100);

    /* 在扭矩列插入Edit控件 */
    m_lceEdit.Insert(1);

    double fTorque;
    GET_CTRL_TORQ(fTorque, m_ptModTorq);
    m_fCir = theApp.GetCir(m_ptModTorq, TYPE_TOTAL);
    m_nTorque = (UINT)fTorque;

    m_lsData.EnableWindow(FALSE);

    ShowTorqValue();

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgListMod::ShowTorqValue()
{
    int     i = 0;
    int     iRow = 0;
    int     nCount = 0;
    CString strNo, strTorq, strRpm, strPulse;

    m_lsData.DeleteAllItems();

    ASSERT_NULL(m_ptModTorq);
    nCount = m_ptModTorq->ftorque_size();
    ASSERT_ZERO(nCount);

    m_fCir = theApp.GetCir(m_ptModTorq, TYPE_TOTAL);

    for (i = (nCount - 1); i >= 0; i--)
    {
        strNo.Format("%d", (i + 1));
        strTorq.Format("%8.2f ", m_ptModTorq->ftorque(i));
        strRpm.Format("%8.2f ", m_ptModTorq->frpm(i));
        strPulse.Format("%d ", m_ptModTorq->dwdelplus(i));

        iRow = m_lsData.InsertItem(0, strNo);  //插入行
        m_lsData.SetItemText(iRow, 1, strTorq); //设置扭矩
        m_lsData.SetItemText(iRow, 2, strRpm); //设置转速
        //m_lsData.SetItemText(iRow, 3, strPulse); //设置脉冲
    }
}

void CDlgListMod::OnClickListdata(NMHDR* pNMHDR, LRESULT* pResult)
{
    int iItem = 0;
    int iSubItem = 0;

    CListCtrlEditBase::HitTestEx(&m_lsData, pNMHDR, iItem, iSubItem);

    if (iSubItem != 0)
        m_lceEdit.ShowEdit(iItem, iSubItem);

    *pResult = 0;
}

void CDlgListMod::OnRadiocontrol()
{
    m_iMethod = OPTIM_METH_CTRLTORQ;
    m_lsData.EnableWindow(FALSE);
    GetDlgItem(IDC_EDITCTRL)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDITCIRCLE)->EnableWindow(FALSE);
}

void CDlgListMod::OnRadiocircle()
{
    m_iMethod = OPTIM_METH_CIRCLE;
    m_lsData.EnableWindow(FALSE);
    GetDlgItem(IDC_EDITCTRL)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDITCIRCLE)->EnableWindow(TRUE);
}

void CDlgListMod::OnRadiodetail()
{
    m_iMethod = OPTIM_METH_MODDATA;
    m_lsData.EnableWindow(TRUE);
    GetDlgItem(IDC_EDITCTRL)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDITCIRCLE)->EnableWindow(FALSE);
}

void CDlgListMod::OnRadiooverlap()
{
    m_iMethod = OPTIM_METH_OVERLAP;
    m_lsData.EnableWindow(FALSE);
    GetDlgItem(IDC_EDITCTRL)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDITCIRCLE)->EnableWindow(FALSE);
}

/* 修改控制扭矩，图像上下拉伸 */
BOOL CDlgListMod::CtrlTorqOptim(TorqData::Torque* pSrcData, TorqData::Torque* pDestData)
{
    int     i = 0;
    UINT    nCount = 0;
    double  fRate = 1.0f;  /* 扭矩优化比例 */
    double  fSrcCtrl = 0;

    /* 源数据个数为0，或者源数据最大扭矩为0，直接返回 */
    ASSERT_NULL_R(pSrcData, FALSE);
    ASSERT_NULL_R(pDestData, FALSE);
    ASSERT_ZERO_R(m_nTorque, FALSE);

    nCount = pSrcData->ftorque_size();
    GET_CTRL_TORQ(fSrcCtrl, pSrcData);
    ASSERT_ZERO_R(fSrcCtrl, FALSE);
    ASSERT_ZERO_R(nCount, FALSE);

    /* 目的扭矩和现有控制扭矩相等, 不需要优化，直接返回 */
    COMP_BE_R(m_nTorque, fSrcCtrl, FALSE);

    fRate = m_nTorque / fSrcCtrl;
    for (i = 0; i < (int)nCount; i++)
    {
        pDestData->set_ftorque(i, pSrcData->ftorque(i) * fRate);
    }
    if (theApp.HaveMakeUP(pSrcData))
    {
        pDestData->set_fmumaxtorq(pSrcData->fmumaxtorq() * fRate);
    }
    if (theApp.HaveBreakout(pSrcData))
    {
        pDestData->set_fbomaxtorq(pSrcData->fbomaxtorq() * fRate);
    }
    return TRUE;
}

/* 修改周数，图像左右拉伸 */
BOOL CDlgListMod::CircleOptim(TorqData::Torque* pSrcData, TorqData::Torque* pDestData)
{
    int     i = 0;
    double  fRate = 0;
    double  fScrCir = 0;

    ASSERT_NULL_R(pSrcData, FALSE);
    ASSERT_NULL_R(pDestData, FALSE);

    /* 源数据个数为0，直接返回失败 */
    fScrCir = theApp.GetCir(pSrcData, TYPE_TOTAL);
    ASSERT_ZERO_R(fScrCir, FALSE);

    /* 目的周数和现有周数相等，直接返回成功 */
    COMP_BE_R(m_fCir, fScrCir, FALSE);

    fRate = m_fCir / fScrCir;
    pDestData->set_dwmuplus((UINT)(pDestData->dwmuplus() * fRate));
    pDestData->set_dwboplus((UINT)(pDestData->dwboplus() * fRate));

    for (i = 0; i < pSrcData->dwdelplus_size(); i++)
    {
        theApp.UpdateDelplus(pDestData, i, (UINT)(pSrcData->dwdelplus(i) * fRate));
        theApp.UpdateTorqRpm(pDestData, i, pSrcData->ftorque(i), pSrcData->frpm(i) * fRate);
    }

    return TRUE;
}

/* 修改数据，周数不变，扭矩值/RPM直接修改 */
BOOL CDlgListMod::ModDataOptim(TorqData::Torque* pSrcData, TorqData::Torque* pDestData)
{
    int     i = 0;
    CString strTemp;

    ASSERT_NULL_R(pSrcData, FALSE);
    ASSERT_NULL_R(pDestData, FALSE);

    for (i = 0; i < pSrcData->ftorque_size(); i++)
    {
        strTemp = m_lsData.GetItemText(i, 1);
        pDestData->set_ftorque(i, atof(strTemp));
        strTemp = m_lsData.GetItemText(i, 2);
        pDestData->set_frpm(i, atof(strTemp));
    }

    return TRUE;
}

/* 重叠图形优化 */
BOOL CDlgListMod::OverlapOptim(TorqData::Torque* pSrcData, TorqData::Torque* pDestData)
{
    int     i = 0;
    int     iInsNum = 0;
    double  fT1 = 0, fT2 = 0, fT3 = 0, fDiff = 0;
    double  fR1 = 0, fR2 = 0;
    double  fInsTorq = 0;
    int     index = 0;
    int     nCount;

    ASSERT_NULL_R(pSrcData, FALSE);
    ASSERT_NULL_R(pDestData, FALSE);

    nCount = pSrcData->ftorque_size();

    //AfxMessageBox("图形数据太小，无法自动优化！",MB_ICONINFORMATION);
    COMP_BLE_R(nCount, 5, FALSE);

    fT1 = pSrcData->ftorque(nCount - 1);
    fT2 = pSrcData->ftorque(nCount - 2);
    fT3 = pSrcData->ftorque(nCount - 3);

    fR1 = pSrcData->frpm(nCount - 1);
    fR2 = pSrcData->frpm(nCount - 2);

    /*在倒数第二和第三两个点之间插入*/
    /*获取需要插入的点的数目*/
    iInsNum = GetInsertNum(fT2, fT3);
    if (iInsNum > 0)
    {
        nCount += iInsNum;
        fDiff = fT2 - fT3;

        for (i = 1; i <= iInsNum; i++)
        {
            index = nCount - 3 + i;
            fInsTorq = fT3 + fDiff * i / (iInsNum + 2);
            theApp.UpdateTorqRpm(pDestData, index, fInsTorq, fR2);
        }
        /* 先将T3后面的数据(T1和T2)保存到新的位置 */
        theApp.UpdateTorqRpm(pDestData, index++, fT2, fR2);
        theApp.UpdateTorqRpm(pDestData, index++, fT1, fR1);
    }

    /*在倒数第一和第二两个点之间插入*/
    /*获取需要插入的点的数目*/
    iInsNum = GetInsertNum(fT1, fT2);
    if (iInsNum > 0)
    {
        nCount += iInsNum;
        fDiff = fT1 - fT2;

        for (i = 1; i <= iInsNum; i++)
        {
            index = nCount - 2 - iInsNum + i;
            fInsTorq = fT2 + fDiff * i / (iInsNum + 2);
            theApp.UpdateTorqRpm(pDestData, index, fInsTorq, fR2);
        }
        /* 先将T2后面的数据(T1)保存到新的位置 */
        theApp.UpdateTorqRpm(pDestData, index, fT1, fR1);
    }
    return TRUE;
}

void CDlgListMod::OnOptimited()
{
    int     i = 0;
    BOOL    bModQuality = FALSE;
    BOOL    bChanged = FALSE;
    double  fMaxTorq = 0;

    TorqData::Torque  tTempData;

    ASSERT_NULL(m_ptModTorq);

    UpdateData(TRUE);

    /*先将源数据复制到临时数据*/
    tTempData = *m_ptModTorq;

    if ((tTempData.dwquality() & 0x00000001) == 0 && m_bQuality)
        bModQuality = TRUE;

    COMP_BGE(m_iMethod, MAX_OPTIM_METH_NUM);

    /* 根据优化方法，调用不同的优化函数进行优化 */
    bChanged = (this->*g_OptimitProc[m_iMethod])(m_ptModTorq, &tTempData);
    /* 数据没有修改，且不需要强制修改为合格数据，直接返回 */
    if (!bChanged && !bModQuality)
        return;

    /*数据规整*/
    for (i = 0; i < (int)tTempData.ftorque_size(); i++)
    {
        tTempData.set_ftorque(i, int(tTempData.ftorque(i) * 100) / 100.0);
        tTempData.set_frpm(i, int(tTempData.frpm(i) * 100) / 100.0);
        if (tTempData.ftorque(i) > fMaxTorq)
            fMaxTorq = int(tTempData.ftorque(i) * 100) / 100.0;
    }

    //tTempData.set_fmaxtorq(fMaxTorq);

    if (bModQuality)
        tTempData.set_dwquality(tTempData.dwquality() | 0x00000001);
    *m_ptModTorq = tTempData;

    ShowTorqValue();

    UpdateData(FALSE);
    m_pdlgParent->SetModified();
    m_pdlgParent->ShowCurData(TRUE);
}

/*
    插入点数目，以及百分比处理
    最多插入4个点，大于20%小于40插入1个点，插入位置为1/3位置。以此类推。
    最后一个位置要大一点，因为控制时最后冲高，扭矩大一些更加合理
    插入点的百分比为 1/(n+2),2/(n+2),...,n/(n+2),
    1：1/3              CTRL:2/3
    2: 1/4 1/4          CTRL:1/2
    3: 1/5 1/5 1/5      CTRL:2/5
    4: 1/6 1/6 1/6 1/6  CTRL:1/3
*/
int  CDlgListMod::GetInsertNum(double fT1, double fT2)
{
    int     i = 0;
    double  fYu = 0;
    double  fLevel[5] = { 0,1.2,1.4,1.8,2.2 };

    ASSERT_ZERO_R(fT2, 0);

    fYu = fT1 / fT2;
    for (i = 4; i > 0; i--)
    {
        COMP_BGE_R(fYu, fLevel[i], i);
    }

    return 0;
}

