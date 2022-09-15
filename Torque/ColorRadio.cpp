#include "ColorRadio.h"



CColorRadio::CColorRadio()
{
    m_ForeColor = GetSysColor(COLOR_BTNTEXT);        //������ɫ
    m_BackColor = GetSysColor(COLOR_BTNFACE);        //����ɫ
    m_BkBrush.CreateSolidBrush(m_BackColor);        //����ˢ
}


CColorRadio::~CColorRadio()
{
}

BEGIN_MESSAGE_MAP(CColorRadio, CButton)
    //{{AFX_MSG_MAP(CColorRadio)
    ON_WM_CTLCOLOR_REFLECT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyRadio message handlers

//���ص���Ϣ������=WM_CTLCOLOR��ע������WM_CTLCOLOR��
//�����޸Ŀؼ�ǰ��ɫ�ͱ���ɫ
HBRUSH CColorRadio::CtlColor(CDC* pDC, UINT nCtlColor)
{
    pDC->SetTextColor(m_ForeColor);
    pDC->SetBkColor(m_BackColor);
    return (HBRUSH)m_BkBrush.GetSafeHandle();
}

void CColorRadio::SetForeColor(COLORREF color)    //�����ı���ɫ
{
    m_ForeColor = color;
}

void CColorRadio::SetBkColor(COLORREF color)        //���ñ�����ɫ
{
    m_BackColor = color;
    m_BkBrush.Detach();
    m_BkBrush.CreateSolidBrush(m_BackColor);
}