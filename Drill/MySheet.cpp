// MySheet.cpp : implementation file
//

#include "stdafx.h"
#include "MySheet.h"
#include "Drill.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


LPCTSTR lpszFace = _T("Tahoma");
const WORD wSize = 8;

/////////////////////////////////////////////////////////////////////////////
// CMySheet
IMPLEMENT_DYNAMIC(CMySheet, CPropertySheet)

CMySheet::CMySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
    :CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CMySheet::CMySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
    : CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CMySheet::~CMySheet()
{

}


BEGIN_MESSAGE_MAP(CMySheet, CPropertySheet)
    //{{AFX_MSG_MAP(CMySheet)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySheet message handlers

BOOL CMySheet::OnInitDialog()
{
    BOOL        bResult = FALSE;
    CRect       btnRect;
    CRect       wdnRect;

    bResult = CPropertySheet::OnInitDialog();

    GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
    GetDlgItem(IDHELP)->ShowWindow(SW_HIDE);
    GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
    GetDlgItem(ID_APPLY_NOW)->ShowWindow(SW_HIDE);

    //获取窗体尺寸
    GetDlgItem(IDCANCEL)->GetWindowRect(&btnRect);
    GetWindowRect(&wdnRect);

    //调整窗体大小
    ::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, wdnRect.Width(), wdnRect.Height() - btnRect.Height(), SWP_NOMOVE | SWP_NOZORDER);

    theApp.AdaptDlgCtrlSize((CDialog*)this, 1);

    return bResult;
}

