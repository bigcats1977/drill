// DlgRemark.cpp : implementation file
//

#include "stdafx.h"
#include "torque.h"
#include "DlgRemark.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRemark dialog


CDlgRemark::CDlgRemark(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgRemark::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgRemark)
    m_iQuality = 0;
    m_nShoulder= 0;
    //}}AFX_DATA_INIT
}


void CDlgRemark::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgRemark)
    DDX_Radio(pDX, IDC_RADIONOQUALITY, m_iQuality);
    DDX_Control(pDX, IDC_COMBOREMARK, m_cbRemark);
    DDX_Text(pDX, IDC_EDITSHOULDER, m_nShoulder);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRemark, CDialog)
    //{{AFX_MSG_MAP(CDlgRemark)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_RADIONOQUALITY, &CDlgRemark::OnBnClickedRadionoquality)
    ON_BN_CLICKED(IDC_RADIOQUALITY, &CDlgRemark::OnBnClickedRadioquality)
    ON_CBN_SELCHANGE(IDC_COMBOREMARK, &CDlgRemark::OnCbnSelchangeComboremark)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRemark message handlers

void CDlgRemark::OnOK()
{
    UpdateData(TRUE);

    if (QUA_RESU_BAD == m_iQuality)
        m_iCause = m_lsCause[m_cbRemark.GetCurSel()];

    CDialog::OnOK();
}

BOOL CDlgRemark::OnInitDialog()
{
    int     i = 0;
    string  strQuality;

    CDialog::OnInitDialog();
    // TODO:  Add extra initialization here
/*
    bit1    - TORQ_LESS_LIMIT   ʵ��Ť�ع�С
    bit2    - TORQ_MORE_LIMIT   �����Ť��
    bit3    - TORQ_MORE_START   ��ʼŤ�ش��ڿ���Ť��15%
    bit4    - TORQ_MORE_CTRL    ���ڿ���Ť��10%
    bit5    - CIRC_LESS_LIMIT   ����0.20Ȧ��
    bit6    - CIRC_MORE_LIMIT   ����Ȧ������
    bit7    - TRANSLATE         ͼ�γ���̨��Ť��(����Ť��15%)��ƽ��
    
    bit8    - NOIPPOINT         �޹յ�Ť�� // ����汾������
    bit9    - LOW_SHOULD        �յ�Ť��<Min shoulder(15%)
    bit10   - HIGHT_SHOULD      �յ�Ť��>Max shoulder(70%)
    bit11   - LOW_DELTATURN     < Min D. Tns
    bit12   - HIGHT_DELTATURN   > Max D. Tns
    bit13   - LOW_SLOPE         б�� < Min Slope
    bit14   - OTHER_CAUSE       ����,���ݲ�ͬ��׼���������ڷ�Χ�ڵĹ��õ�������
    bit15   - SHACK_INSPECT     ж�ۼ��    �˹��趨
    bit16   - GALLING           ճ��        �˹��趨
    //bit17   - HYDTONGSLIP       Һѹǯ��      �˹��趨
    bit17   - THREADNOTCLEAN    ˿����ϴ���ɾ�  �˹��趨
    bit18   - GASSEALINSPECT    ���첻�ϸ�  �˹��趨
*/
    /* 
        1.ʵ��Ť�ع�С
        2.�����Ť��
        3.�гִ�
        4.��������С
        5.��ʼŤ�ع���
        6.�յ�б��С
        7.���յ�Ť��
        8.�յ�Ť��С
        9.��������ֵ
        10.ж�ۼ��
        11.ճ��
        12.˿����ϴ���ɾ�
        13.���񲻺ϸ�
        14.����
    */
    i = 0;
    m_lsCause[i++] = 1;     // bit1 
    m_lsCause[i++] = 2;     // bit2 
    m_lsCause[i++] = 7;
    m_lsCause[i++] = 5;
    m_lsCause[i++] = 3;
    m_lsCause[i++] = 13;
    m_lsCause[i++] = 10;
    m_lsCause[i++] = 9;
    m_lsCause[i++] = 12;
    m_lsCause[i++] = 15;
    m_lsCause[i++] = 16;
    m_lsCause[i++] = 17;
    m_lsCause[i++] = 18;
    m_lsCause[i++] = 14;

    for(i=0; i<MAX_USE_CAUSE; i++)
    {
        //strQuality.Format(IDS_STRMARKDISQUAL + m_lsCause[i]);
        strQuality = theApp.LoadstringFromRes(IDS_STRMARKDISQUAL + m_lsCause[i]);
        m_cbRemark.AddString(strQuality.c_str());
    }
    
    SetRejCause();
    
    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRemark::SetRejCause()
{
    int i = 0;
    
    if(m_iQuality == QUA_RESU_GOOD)
    {
        m_cbRemark.EnableWindow(FALSE);
        return;
    }

    m_cbRemark.EnableWindow(TRUE);
    
    for(i=0; i<MAX_USE_CAUSE; i++)
    {
        if(m_iCause == m_lsCause[i])
        {
            m_cbRemark.SetCurSel(i);
            return;
        }
    }
    //����ԭ��
    m_cbRemark.SetCurSel(MAX_USE_CAUSE-1);
}

void CDlgRemark::OnBnClickedRadionoquality()
{
    m_iQuality = QUA_RESU_BAD;
    SetRejCause();
}


void CDlgRemark::OnBnClickedRadioquality()
{
    m_iQuality = QUA_RESU_GOOD;
    SetRejCause();
}


void CDlgRemark::OnCbnSelchangeComboremark()
{
    int  i = 0;
    UpdateData(TRUE);
    i = m_cbRemark.GetCurSel();
    m_iCause = m_lsCause[i];
}
