#include "ColorRadio.h"



CColorRadio::CColorRadio()
{
    m_ForeColor = GetSysColor(COLOR_BTNTEXT);        //文字颜色
    m_BackColor = GetSysColor(COLOR_BTNFACE);        //背景色
    m_BkBrush.CreateSolidBrush(m_BackColor);        //背景刷
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

//重载的消息函数（=WM_CTLCOLOR，注：不是WM_CTLCOLOR）
//用于修改控件前景色和背景色
HBRUSH CColorRadio::CtlColor(CDC* pDC, UINT nCtlColor)
{
    pDC->SetTextColor(m_ForeColor);
    pDC->SetBkColor(m_BackColor);
    return (HBRUSH)m_BkBrush.GetSafeHandle();
}

void CColorRadio::SetForeColor(COLORREF color)    //设置文本颜色
{
    m_ForeColor = color;
}

void CColorRadio::SetBkColor(COLORREF color)        //设置背景颜色
{
    m_BackColor = color;
    m_BkBrush.Detach();
    m_BkBrush.CreateSolidBrush(m_BackColor);
}