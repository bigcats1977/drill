// ColorComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "ColorComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorComboBox

CColorComboBox::CColorComboBox()
{   
    m_ColorText=::GetSysColor(COLOR_WINDOWTEXT);//RGB(255,0,0);
    m_ColorBk = RGB(255,255,255);
    m_Brush.CreateSolidBrush(m_ColorBk);
}

CColorComboBox::~CColorComboBox()
{
}


BEGIN_MESSAGE_MAP(CColorComboBox, CComboBox)
    //{{AFX_MSG_MAP(CColorComboBox)
    ON_WM_CTLCOLOR()
    ON_WM_CTLCOLOR_REFLECT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorComboBox message handlers
HBRUSH CColorComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
    pDC->SetTextColor(m_ColorText);
    //pDC->SetTextColor(RGB(255,0,0));
    pDC->SetBkColor(m_ColorBk);
    return m_Brush;
}

HBRUSH CColorComboBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    pDC->SetTextColor(m_ColorText);
    //pDC->SetTextColor(RGB(0, 255, 0));
    pDC->SetBkColor(m_ColorBk);
    return m_Brush;
}
//ÉèÖÃ×Ö´óÐ¡
void CColorComboBox::SetFontSize(int iSize,CFont* font)
{
    font->CreatePointFont(iSize,_T("Times New Roman"));
    LOGFONT lf;
    font->GetLogFont(&lf);
    font->DeleteObject();
    lf.lfWeight = FW_BOLD;
    font->CreateFontIndirect(&lf);
    SetFont(font);
}
