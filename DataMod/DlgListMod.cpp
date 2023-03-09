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
    /*  0 */    &CDlgListMod::CtrlTorqOptim,    /* ����Ť���Ż� */
    /*  1 */    &CDlgListMod::CircleOptim,      /* �����Ż� */
    /*  2 */    &CDlgListMod::ModDataOptim,     /* ֱ���޸������Ż� */
    /*  3 */    &CDlgListMod::OverlapOptim,     /* �ص�ͼ���Ż� */
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

    /* Ť��������Ч��ֱ�ӷ��� */
    ASSERT_NULL_R(m_ptModTorq, FALSE);
    nCount = m_ptModTorq->ftorque_size();
    ASSERT_ZERO_R(nCount, FALSE);

    m_pdlgParent = (CDlgHisGrp*)GetParent();

    dwStyle = m_lsData.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
    dwStyle |= LVS_EX_GRIDLINES;    //�����ߣ�ֻ������report����listctrl��
    //dwStyle |= LVS_EX_CHECKBOXES; //itemǰ����checkbox�ؼ�
    m_lsData.SetExtendedStyle(dwStyle); //������չ���

    m_lceEdit.CreateEx(this, &m_lsData, ES_CENTER);

    m_lsData.InsertColumn(0, "���", LVCFMT_CENTER, 50);//������
    m_lsData.InsertColumn(1, "Ť��", LVCFMT_CENTER, 135);
    m_lsData.InsertColumn(2, "ת��", LVCFMT_CENTER, 135);
    //m_lsData.InsertColumn(3, "����", LVCFMT_CENTER, 100);

    /* ��Ť���в���Edit�ؼ� */
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

        iRow = m_lsData.InsertItem(0, strNo);  //������
        m_lsData.SetItemText(iRow, 1, strTorq); //����Ť��
        m_lsData.SetItemText(iRow, 2, strRpm); //����ת��
        //m_lsData.SetItemText(iRow, 3, strPulse); //��������
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

/* �޸Ŀ���Ť�أ�ͼ���������� */
BOOL CDlgListMod::CtrlTorqOptim(TorqData::Torque* pSrcData, TorqData::Torque* pDestData)
{
    int     i = 0;
    UINT    nCount = 0;
    double  fRate = 1.0f;  /* Ť���Ż����� */
    double  fSrcCtrl = 0;

    /* Դ���ݸ���Ϊ0������Դ�������Ť��Ϊ0��ֱ�ӷ��� */
    ASSERT_NULL_R(pSrcData, FALSE);
    ASSERT_NULL_R(pDestData, FALSE);
    ASSERT_ZERO_R(m_nTorque, FALSE);

    nCount = pSrcData->ftorque_size();
    GET_CTRL_TORQ(fSrcCtrl, pSrcData);
    ASSERT_ZERO_R(fSrcCtrl, FALSE);
    ASSERT_ZERO_R(nCount, FALSE);

    /* Ŀ��Ť�غ����п���Ť�����, ����Ҫ�Ż���ֱ�ӷ��� */
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

/* �޸�������ͼ���������� */
BOOL CDlgListMod::CircleOptim(TorqData::Torque* pSrcData, TorqData::Torque* pDestData)
{
    int     i = 0;
    double  fRate = 0;
    double  fScrCir = 0;

    ASSERT_NULL_R(pSrcData, FALSE);
    ASSERT_NULL_R(pDestData, FALSE);

    /* Դ���ݸ���Ϊ0��ֱ�ӷ���ʧ�� */
    fScrCir = theApp.GetCir(pSrcData, TYPE_TOTAL);
    ASSERT_ZERO_R(fScrCir, FALSE);

    /* Ŀ������������������ȣ�ֱ�ӷ��سɹ� */
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

/* �޸����ݣ��������䣬Ť��ֵ/RPMֱ���޸� */
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

/* �ص�ͼ���Ż� */
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

    //AfxMessageBox("ͼ������̫С���޷��Զ��Ż���",MB_ICONINFORMATION);
    COMP_BLE_R(nCount, 5, FALSE);

    fT1 = pSrcData->ftorque(nCount - 1);
    fT2 = pSrcData->ftorque(nCount - 2);
    fT3 = pSrcData->ftorque(nCount - 3);

    fR1 = pSrcData->frpm(nCount - 1);
    fR2 = pSrcData->frpm(nCount - 2);

    /*�ڵ����ڶ��͵���������֮�����*/
    /*��ȡ��Ҫ����ĵ����Ŀ*/
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
        /* �Ƚ�T3���������(T1��T2)���浽�µ�λ�� */
        theApp.UpdateTorqRpm(pDestData, index++, fT2, fR2);
        theApp.UpdateTorqRpm(pDestData, index++, fT1, fR1);
    }

    /*�ڵ�����һ�͵ڶ�������֮�����*/
    /*��ȡ��Ҫ����ĵ����Ŀ*/
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
        /* �Ƚ�T2���������(T1)���浽�µ�λ�� */
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

    /*�Ƚ�Դ���ݸ��Ƶ���ʱ����*/
    tTempData = *m_ptModTorq;

    if ((tTempData.dwquality() & 0x00000001) == 0 && m_bQuality)
        bModQuality = TRUE;

    COMP_BGE(m_iMethod, MAX_OPTIM_METH_NUM);

    /* �����Ż����������ò�ͬ���Ż����������Ż� */
    bChanged = (this->*g_OptimitProc[m_iMethod])(m_ptModTorq, &tTempData);
    /* ����û���޸ģ��Ҳ���Ҫǿ���޸�Ϊ�ϸ����ݣ�ֱ�ӷ��� */
    if (!bChanged && !bModQuality)
        return;

    /*���ݹ���*/
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
    �������Ŀ���Լ��ٷֱȴ���
    ������4���㣬����20%С��40����1���㣬����λ��Ϊ1/3λ�á��Դ����ơ�
    ���һ��λ��Ҫ��һ�㣬��Ϊ����ʱ����ߣ�Ť�ش�һЩ���Ӻ���
    �����İٷֱ�Ϊ 1/(n+2),2/(n+2),...,n/(n+2),
    1��1/3              CTRL:2/3
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

