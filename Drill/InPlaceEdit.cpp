// InPlaceEdit.cpp : implementation file
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Copyright (c) 1998.
//
// The code contained in this file is based on the original
// CInPlaceEdit from http://www.codeguru.com/listview/edit_subitems.shtml
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then acknowledgement must be made to the author of this file 
// (in whatever form you wish).
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// Expect bugs!
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
// Modifed 10 May 1998  Uses GVN_ notifications instead of LVN_,
//                      Sends notification messages to the parent, 
//                      instead of the parent's parent.
//
//         15 May 1998  There was a problem when editing with the in-place editor, 
//                      there arises a general protection fault in user.exe, with a 
//                      few qualifications:
//                         (1) This only happens with owner-drawn buttons;
//                         (2) This only happens in Win95
//                         (3) This only happens if the handler for the button does not 
//                             create a new window (even an AfxMessageBox will avoid the 
//                             crash)
//                         (4) This will not happen if Spy++ is running.
//                      PreTranslateMessage was added to route messages correctly.
//                      (Matt Weagle found and fixed this problem)
//         26 Jul 1998  Removed the ES_MULTILINE style - that fixed a few probs!
//          6 Aug 1998  Added nID to the constructor param list
//          6 Sep 1998  Space no longer clears selection when starting edit (Franco Bez)
//         10 Apr 1999  Enter, Tab and Esc key prob fixed (Koay Kah Hoe)
//                      Workaround for bizzare "shrinking window" problem in CE
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TCHAR.h"
#include "InPlaceEdit.h"

#include "GridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit

UINT    g_nReturnKey[] = {
                VK_LBUTTON,
                VK_RETURN
        };

UINT    g_nDirectionKey[] = {
                VK_TAB,
                VK_DOWN,
                VK_UP,
                VK_RIGHT,
                VK_LEFT,
                VK_NEXT,
                VK_PRIOR,
                VK_HOME,
                VK_SPACE,
                VK_END
        };

UINT    g_nBackKey[] = {
                VK_BACK
        };

UINT    g_nArrowKey[] = {
                VK_PRIOR,
                VK_NEXT,
                VK_DOWN,
                VK_UP,
                VK_RIGHT,
                VK_LEFT
        };

BOOL CInPlaceEdit::FindKey(UINT pnKeyArray[], UINT nTarKey, int iKeyNum)
{
    int     i       = 0;

    ASSERT_NULL_R(pnKeyArray, FALSE);

    for(i=0; i<iKeyNum; i++)
    {
        COMP_BE_R(nTarKey, pnKeyArray[i], TRUE);
    }

    return FALSE;
}

BOOL CInPlaceEdit::ReturnKeyInit(UINT  nChar)
{
    if(FindKey(g_nReturnKey, nChar, sizeof(g_nReturnKey)/sizeof(UINT)))
    {
        SetSel((int)_tcslen(m_strInitText), -1);
        return TRUE;
    }

    return FALSE;
}

BOOL CInPlaceEdit::DirectionKeyInit(UINT  nChar)
{
    if(FindKey(g_nDirectionKey, nChar, sizeof(g_nDirectionKey)/sizeof(UINT)))
    {
        SetSel(0,-1);
        return TRUE;
    }

    return FALSE;
}

BOOL CInPlaceEdit::BackKeyInit(UINT  nChar)
{
    if(FindKey(g_nBackKey, nChar, sizeof(g_nBackKey)/sizeof(UINT)))
    {
        SetSel((int)_tcslen(m_strInitText), -1);
        return TRUE;
    }

    return FALSE;
}

CInPlaceEdit::CInPlaceEdit(CWnd* pParent, CRect& rc, DWORD dwStyle, UINT nID,
                           int iRow, int iCol, CString strInitText, UINT nFirstChar)
{
    DWORD  dwEditStyle = 0;

    m_strInitText   = strInitText;
    m_iRow          = iRow;
    m_iCol          = iCol;
    m_nLastChar     = 0; 
    m_rcSelf        = rc;  // For bizarre CE bug.
    m_bExitOnArrows = (nFirstChar != VK_LBUTTON);    // If mouse click brought us here,
                                                     // then no exit on arrows
    dwEditStyle     = WS_BORDER|WS_CHILD|WS_VISIBLE| ES_AUTOHSCROLL | dwStyle; //|ES_MULTILINE

    COMP_BFALSE(Create(dwEditStyle, rc, pParent, nID));
    
    SetFont(pParent->GetFont());
    
    SetWindowText(strInitText);
    SetFocus();

    COMP_BTRUE(ReturnKeyInit(nFirstChar));

    COMP_BTRUE(DirectionKeyInit(nFirstChar));

    /* BackKey在函数中初始化了，其他的Key在if语句里面初始化 */
    if(!BackKeyInit(nFirstChar))
    {
        SetSel(0,-1);
    }
    
    SendMessage(WM_CHAR, nFirstChar);
}

CInPlaceEdit::~CInPlaceEdit()
{
}

BEGIN_MESSAGE_MAP(CInPlaceEdit, CEdit)
    //{{AFX_MSG_MAP(CInPlaceEdit)
    ON_WM_KILLFOCUS()
    ON_WM_CHAR()
    ON_WM_KEYDOWN()
    ON_WM_GETDLGCODE()
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit message handlers

// If an arrow key (or associated) is pressed, then exit if
//  a) The Ctrl key was down, or
//  b) m_bExitOnArrows == TRUE
void CInPlaceEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if (FindKey(g_nArrowKey, nChar, sizeof(g_nArrowKey)/sizeof(UINT)) &&
        (m_bExitOnArrows || GetKeyState(VK_CONTROL) < 0))
    {
        m_nLastChar = nChar;
        GetParent()->SetFocus();
        return;
    }
    
    CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

// As soon as this edit loses focus, kill it.
void CInPlaceEdit::OnKillFocus(CWnd* pNewWnd)
{
    CEdit::OnKillFocus(pNewWnd);
    EndEdit();
}

BOOL CInPlaceEdit::SpecialChar(UINT nChar)
{
    if (nChar == VK_TAB || nChar == VK_RETURN)
    {
        m_nLastChar = nChar;
        GetParent()->SetFocus();    // This will destroy this window
        return TRUE;
    }
    if (nChar == VK_ESCAPE) 
    {
        // restore previous text
        SetWindowText(m_strInitText);
        m_nLastChar = nChar;
        GetParent()->SetFocus();
        return TRUE;
    }

    return FALSE;
}

void CInPlaceEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CString     strWinText;
    CFont       *pFontDC = NULL;
    CSize       szTextEx;
    CRect       rcParent;
    CWindowDC   dc(this);

    COMP_BTRUE(SpecialChar(nChar));
    
    CEdit::OnChar(nChar, nRepCnt, nFlags);
    
    // Resize edit control if needed
    
    // Get text extent
    GetWindowText( strWinText );

    // add some extra buffer
    strWinText += _T("  ");
    
    pFontDC = dc.SelectObject(GetFont());
    szTextEx = dc.GetTextExtent( strWinText );
    dc.SelectObject( pFontDC );
       
    // Get client rect
    GetParent()->GetClientRect( &rcParent );
    
    // Check whether control needs to be resized
    // and whether there is space to grow
    if (szTextEx.cx > m_rcSelf.Width())
    {
        if( szTextEx.cx + m_rcSelf.left < rcParent.right )
            m_rcSelf.right = m_rcSelf.left + szTextEx.cx;
        else
            m_rcSelf.right = rcParent.right;
        MoveWindow( &m_rcSelf );
    }
}

UINT CInPlaceEdit::OnGetDlgCode() 
{
    return DLGC_WANTALLKEYS;
}

////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit overrides

// Stoopid win95 accelerator key problem workaround - Matt Weagle.
BOOL CInPlaceEdit::PreTranslateMessage(MSG* pMsg) 
{
    ASSERT_NULL_R(pMsg, FALSE);

    // Catch the Alt key so we don't choke if focus is going to an owner drawn button
    COMP_BE_R(pMsg->message, WM_SYSCHAR, TRUE);
    
    return CWnd::PreTranslateMessage(pMsg);
}

// Auto delete
void CInPlaceEdit::PostNcDestroy() 
{
    CEdit::PostNcDestroy();
    
    delete this;
}

////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit implementation

void CInPlaceEdit::EndEdit()
{
    CString     strWinText;
    CWnd        *pOwner = NULL;
    // Send Notification to parent
    GV_DISPINFO dispinfo;

    GetWindowText(strWinText);
    
    dispinfo.hdr.hwndFrom = GetSafeHwnd();
    dispinfo.hdr.idFrom   = GetDlgCtrlID();
    dispinfo.hdr.code     = GVN_ENDLABELEDIT;
    
    dispinfo.item.nMask   = LVIF_TEXT|LVIF_PARAM;
    dispinfo.item.iRow    = m_iRow;
    dispinfo.item.iCol    = m_iCol;
    dispinfo.item.szText  = strWinText;
    dispinfo.item.lParam  = (LPARAM) m_nLastChar; 
    
    pOwner = GetOwner();
    if (pOwner)
        pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo );
    
    // Close this window (PostNcDestroy will delete this)
    PostMessage(WM_CLOSE, 0, 0);
}

