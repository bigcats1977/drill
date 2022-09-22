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
    bit1    - TORQ_LESS_LIMIT   实际扭矩过小
    bit2    - TORQ_MORE_LIMIT   超最大扭矩
    bit3    - TORQ_MORE_START   起始扭矩大于控制扭矩15%
    bit4    - TORQ_MORE_CTRL    高于控制扭矩10%
    bit5    - CIRC_LESS_LIMIT   低于0.20圈数
    bit6    - CIRC_MORE_LIMIT   高于圈数上限
    bit7    - TRANSLATE         图形超过台阶扭矩(控制扭矩15%)后平移
    
    bit8    - NOIPPOINT         无拐点扭矩 // 大庆版本不存在
    bit9    - LOW_SHOULD        拐点扭矩<Min shoulder(15%)
    bit10   - HIGHT_SHOULD      拐点扭矩>Max shoulder(70%)
    bit11   - LOW_DELTATURN     < Min D. Tns
    bit12   - HIGHT_DELTATURN   > Max D. Tns
    bit13   - LOW_SLOPE         斜率 < Min Slope
    bit14   - OTHER_CAUSE       其他,根据不同标准，将不属于范围内的归置到其他中
    bit15   - SHACK_INSPECT     卸扣检查    人工设定
    bit16   - GALLING           粘扣        人工设定
    //bit17   - HYDTONGSLIP       液压钳打滑      人工设定
    bit17   - THREADNOTCLEAN    丝扣清洗不干净  人工设定
    bit18   - GASSEALINSPECT    气检不合格  人工设定
*/
    /* 
        1.实际扭矩过小
        2.超最大扭矩
        3.夹持打滑
        4.总周数过小
        5.起始扭矩过大
        6.拐点斜率小
        7.超拐点扭矩
        8.拐点扭矩小
        9.超周数差值
        10.卸扣检查
        11.粘扣
        12.丝扣清洗不干净
        13.气捡不合格
        14.其他
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
    //其他原因
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
