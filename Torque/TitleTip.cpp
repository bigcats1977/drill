////////////////////////////////////////////////////////////////////////////
// TitleTip.cpp : implementation file
//
// Code taken from www.codeguru.com. - thanks Zafir!
//
// Modifed 10 Apr 1999  Now accepts a LOGFONT pointer and
//                      a tracking rect in Show(...)  (Chris Maunder)
//         18 Apr 1999  Resource leak in Show fixed by Daniel Gehriger


#include "stdafx.h"
#include "gridctrl.h"

#ifndef GRIDCONTROL_NO_TITLETIPS

#include "TitleTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTitleTip

CTitleTip::CTitleTip()
{
    // Register the window class if it has not already been registered.
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if(!(::GetClassInfo(hInst, TITLETIP_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_SAVEBITS;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = LoadCursor( hInst, IDC_ARROW );
        wndcls.hbrBackground    = (HBRUSH)(COLOR_INFOBK + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = TITLETIP_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
            AfxThrowResourceException();
    }
}

CTitleTip::~CTitleTip()
{
}


BEGIN_MESSAGE_MAP(CTitleTip, CWnd)
    //{{AFX_MSG_MAP(CTitleTip)
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTitleTip message handlers

BOOL CTitleTip::Create(CWnd * pParentWnd)
{
    DWORD dwStyle   = 0;
    DWORD dwExStyle = 0;

    ASSERT_VALID(pParentWnd);

    dwStyle   = WS_BORDER | WS_POPUP;
    dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
    m_pParentWnd = pParentWnd;

    return CreateEx(dwExStyle, TITLETIP_CLASSNAME, NULL, dwStyle,
                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                    NULL, NULL, NULL );
}


// Show          - Show the titletip if needed
// rectTitle     - The rectangle within which the original
//                  title is constrained - in client coordinates
// lpszTitleText - The text to be displayed
// iXOffset      - Number of pixel that the text is offset from
//                 left border of the cell
void CTitleTip::Show(CRect rcTitle, LPCTSTR lpszTitleText, int iXOffset /*=0*/,
                     LPRECT lprcHover /*=NULL*/,
                     LPLOGFONT lpLogFont /*=NULL*/)
{
    CSize       szTmp;
    CClientDC   dc(this);
    CString     strTitle = _T("");
    TEXTMETRIC  tm;
    CRect       rcDisplay;
    CFont       ftTip;
    CFont       *pOldFont = NULL;

    ASSERT( ::IsWindow( GetSafeHwnd() ) );

    COMP_BTRUE(rcTitle.IsRectEmpty());

    m_rcHover = (lprcHover != NULL)? lprcHover : rcTitle;

    m_pParentWnd->ClientToScreen( m_rcHover );
    ScreenToClient( m_rcHover );

    // If titletip is already displayed, don't do anything.
    COMP_BTRUE(IsWindowVisible());

    // Do not display the titletip is app does not have focus
    COMP_BE(GetFocus(), NULL);

    // Define the rectangle outside which the titletip will be hidden.
    // We add a buffer of one pixel around the rectangle
    m_rcTitle.top    = -1;
    m_rcTitle.left   = -iXOffset-1;
    m_rcTitle.right  = rcTitle.Width()-iXOffset;
    m_rcTitle.bottom = rcTitle.Height()+1;

    // Determine the width of the text
    m_pParentWnd->ClientToScreen( rcTitle );

    strTitle += _T(" ");
    strTitle += lpszTitleText;
    strTitle += _T(" ");

    if (lpLogFont)
    {
        ftTip.CreateFontIndirect(lpLogFont);
        pOldFont = dc.SelectObject( &ftTip );
    }
    else
    {
        // use same font as ctrl
        pOldFont = dc.SelectObject( m_pParentWnd->GetFont() );
    }

    szTmp = dc.GetTextExtent( strTitle );

    dc.GetTextMetrics(&tm);
    szTmp.cx += tm.tmOverhang;

    rcDisplay = rcTitle;
    rcDisplay.left += iXOffset;
    rcDisplay.right = rcDisplay.left + szTmp.cx + iXOffset;

    // Do not display if the text fits within available space
    if( rcDisplay.right > rcTitle.right-iXOffset )
    {
        // Show the titletip
        SetWindowPos( &wndTop, rcDisplay.left, rcDisplay.top,
                      rcDisplay.Width(), rcDisplay.Height(),
                      SWP_SHOWWINDOW|SWP_NOACTIVATE );

        dc.SetBkMode( TRANSPARENT );
        dc.TextOut( 0, 0, strTitle );
        SetCapture();
    }

    dc.SelectObject( pOldFont );
}

void CTitleTip::Hide()
{
    COMP_BFALSE(::IsWindow(GetSafeHwnd()));

    if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
        ReleaseCapture();

    ShowWindow( SW_HIDE );
}

void CTitleTip::OnMouseMove(UINT nFlags, CPoint point)
{
    int     iHitTest = 0;
    CWnd    *pWnd    = NULL;

    COMP_BTRUE(m_rcHover.PtInRect(point));
    
    Hide();

    // Forward the message
    ClientToScreen( &point );
    pWnd = WindowFromPoint( point );
    if ( pWnd == this )
        pWnd = m_pParentWnd;

    iHitTest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));

    if (iHitTest == HTCLIENT) 
    {
        pWnd->ScreenToClient( &point );
        pWnd->PostMessage( WM_MOUSEMOVE, nFlags, MAKELONG(point.x,point.y) );
        return;
    }
    /* else */
    pWnd->PostMessage( WM_NCMOUSEMOVE, iHitTest, MAKELONG(point.x,point.y) );
}

void CTitleTip::MouseBtnDownMessage(MSG* pMsg)
{
    int     iHitTest = 0;
    POINTS  ptsTmp;
    POINT   ptTmp;
    CWnd    *pWnd    = NULL;

    ASSERT_NULL(pMsg);

    ptsTmp = MAKEPOINTS( pMsg->lParam );
    ptTmp.x = ptsTmp.x;
    ptTmp.y = ptsTmp.y;
    ClientToScreen( &ptTmp );
    pWnd = WindowFromPoint( ptTmp );
    if( pWnd == this )
        pWnd = m_pParentWnd;

    iHitTest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(ptTmp.x,ptTmp.y));

    if (iHitTest == HTCLIENT) 
    {
        pWnd->ScreenToClient( &ptTmp );
        pMsg->lParam = MAKELONG(ptTmp.x,ptTmp.y);

        KeyDownMessage(pMsg);
        return;
    }

    /* iHitTest != HTCLIENTlse  */
    switch (pMsg->message) 
    {
    case WM_LBUTTONDOWN:
        pMsg->message = WM_NCLBUTTONDOWN;
        break;
    case WM_RBUTTONDOWN:
        pMsg->message = WM_NCRBUTTONDOWN;
        break;
    case WM_MBUTTONDOWN:
        pMsg->message = WM_NCMBUTTONDOWN;
        break;
    }
    pMsg->wParam = iHitTest;
    pMsg->lParam = MAKELONG(ptTmp.x,ptTmp.y);
    KeyDownMessage(pMsg);

    return;
}

void CTitleTip::KeyDownMessage(MSG* pMsg)
{
    ASSERT_NULL(pMsg);
    
    Hide();
    m_pParentWnd->PostMessage( pMsg->message, pMsg->wParam, pMsg->lParam );
}

BOOL CTitleTip::PreTranslateMessage(MSG* pMsg)
{
    ASSERT_NULL_R(pMsg, FALSE);

    switch (pMsg->message)
    {
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
        MouseBtnDownMessage(pMsg);
        return TRUE;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        KeyDownMessage(pMsg);
        return TRUE;
    }

    if( GetFocus() == NULL )
    {
        Hide();
        return TRUE;
    }

    return CWnd::PreTranslateMessage(pMsg);
}

#endif // GRIDCONTROL_NO_TITLETIPS