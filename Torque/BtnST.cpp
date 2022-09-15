#include "StdAfx.h"
#include "BtnST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonST

// Mask for control's type
//#define BS_TYPEMASK SS_TYPEMASK

/* 销毁有效的ICON */
#define DESTROY_ICON(hIcon)  {          \
        if(hIcon)                       \
            ::DestroyIcon(hIcon);       \
    }

#define REPAINT_STBTN(bRepaint)  {      \
    if (bRepaint)                       \
        Invalidate();                   \
    }

/* 删除有效的object */
#define DELETE_OBJECT(hObject)  {       \
        if(hObject)                     \
            ::DeleteObject(hObject);    \
    }

/* 避免Offset越界 */
#define MIN_OFFSET(value, off, newOff){ \
        if ((value) + (off) > 255)      \
            (newOff) = 255 - (value);   \
    }
#define MAX_OFFSET(value, off, newOff){ \
        if ((value) + (off) < 0)        \
            (newOff) = -1 * (value);    \
    }

CButtonST::CButtonST()
{
    m_bIsPressed    = FALSE;
    m_bIsFocused    = FALSE;
    m_bIsDisabled   = FALSE;
    m_bMouseOnButton= FALSE;
    m_bIconSame     = FALSE;
    m_iIconSame     = -1;

    FreeResources(FALSE);

    // Default type is "flat" button
    m_bIsFlat       = TRUE;
    // Button will be tracked also if when the window is inactive (like Internet Explorer)
    m_bAlwaysTrack  = TRUE;

    // By default draw border in "flat" button
    m_bDrawBorder   = TRUE;

    // By default icon is aligned horizontally
    m_ucAlign       = ST_ALIGN_HORIZ;

    // By default, for "flat" button, don't draw the focus rect
    m_bDrawFlatFocus = FALSE;

    // By default the button is not the default button
    m_bIsDefault    = FALSE;

    // Invalid value, since type still unknown
    m_nTypeStyle    = SS_TYPEMASK;

    // By default the button is not a checkbox
    m_bIsCheckBox   = FALSE;
    m_iCheck        = 0;

    // Set default colors
    SetDefaultColors(FALSE);

    // No tooltip created
    m_ToolTip.m_hWnd    = NULL;

    // Do not draw as a transparent button
    m_bDrawTransparent  = FALSE;
    m_pbmpOldBk         = NULL;

    // No URL defined
    SetURL(NULL);

    // No cursor defined
    m_hCursor           = NULL;

    // No associated menu
    m_hParentWndMenu        = NULL;
    m_bMenuDisplayed        = FALSE;

    m_bShowDisabledBitmap   = TRUE;
} // End of CButtonST

CButtonST::~CButtonST()
{
    // Restore old bitmap (if any)
    if (m_dcBk.m_hDC && m_pbmpOldBk)
    {
        m_dcBk.SelectObject(m_pbmpOldBk);
    } // if

    FreeResources();

    // Destroy the cursor (if any)
    if (m_hCursor) 
        ::DestroyCursor(m_hCursor);

    // Destroy the menu (if any)
    //if (m_menuPopup.m_hMenu)    
    //    m_menuPopup.DestroyMenu();
} // End of ~CButtonST

BEGIN_MESSAGE_MAP(CButtonST, CButton)
    //{{AFX_MSG_MAP(CButtonST)
    ON_WM_SETCURSOR()
    ON_WM_KILLFOCUS()
    ON_WM_MOUSEMOVE()
    ON_WM_SYSCOLORCHANGE()
    ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
    ON_WM_ACTIVATE()
    ON_WM_ENABLE()
    ON_WM_CANCELMODE()
    ON_WM_GETDLGCODE()
    ON_WM_CTLCOLOR_REFLECT()
    //}}AFX_MSG_MAP
    ON_WM_MENUCHAR()
    ON_WM_MEASUREITEM()

    ON_MESSAGE(BM_SETSTYLE, OnSetStyle)
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
    ON_MESSAGE(BM_SETCHECK, OnSetCheck)
    ON_MESSAGE(BM_GETCHECK, OnGetCheck)
END_MESSAGE_MAP()

void CButtonST::FreeResources(BOOL bCheckForNULL)
{
    if (bCheckForNULL)
    {
        // Destroy icons
        // Note: the following two lines MUST be here! even if
        // BoundChecker says they are unnecessary!
        DESTROY_ICON(m_csIcons[0].hIcon);
        DESTROY_ICON(m_csIcons[1].hIcon);

        // Destroy bitmaps
        DELETE_OBJECT(m_csBitmaps[0].hBitmap);
        DELETE_OBJECT(m_csBitmaps[1].hBitmap);

        // Destroy mask bitmaps
        DELETE_OBJECT(m_csBitmaps[0].hMask);
        DELETE_OBJECT(m_csBitmaps[1].hMask);
    } // if

    ::ZeroMemory(m_csIcons, sizeof(m_csIcons));
    ::ZeroMemory(m_csBitmaps, sizeof(m_csBitmaps));
} // End of FreeResources

void CButtonST::PreSubclassWindow()
{
    UINT nBS    = 0;

    nBS = GetButtonStyle();

    // Set initial control type
    m_nTypeStyle = nBS & SS_TYPEMASK;

    // Check if this is a checkbox
    if (nBS & BS_CHECKBOX) 
        m_bIsCheckBox = TRUE;

    // Set initial default state flag
    if (m_nTypeStyle == BS_DEFPUSHBUTTON)
    {
        // Set default state for a default button
        m_bIsDefault = TRUE;

        // Adjust style for default button
        m_nTypeStyle = BS_PUSHBUTTON;
    } // If

    // You should not set the Owner Draw before this call
    // (don't use the resource editor "Owner Draw" or
    // ModifyStyle(0, BS_OWNERDRAW) before calling PreSubclassWindow() )
    ASSERT(m_nTypeStyle != BS_OWNERDRAW);

    // Switch to owner-draw
    ModifyStyle(SS_TYPEMASK, BS_OWNERDRAW, SWP_FRAMECHANGED);

    CButton::PreSubclassWindow();
} // End of PreSubclassWindow

UINT CButtonST::OnGetDlgCode()
{
    UINT nCode = CButton::OnGetDlgCode();

    // Tell the system if we want default state handling
    // (losing default state always allowed)
    nCode |= (m_bIsDefault ? DLGC_DEFPUSHBUTTON : DLGC_UNDEFPUSHBUTTON);

    return nCode;
} // End of OnGetDlgCode

BOOL CButtonST::PreTranslateMessage(MSG *pMsg)
{
    InitToolTip();
    m_ToolTip.RelayEvent(pMsg);

    return CButton::PreTranslateMessage(pMsg);
} // End of PreTranslateMessage

LRESULT CButtonST::DefWindowProc(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
    if (nMessage == WM_LBUTTONDBLCLK)
    {
        nMessage = WM_LBUTTONDOWN;
    } // if

    return CButton::DefWindowProc(nMessage, wParam, lParam);
} // End of DefWindowProc

HBRUSH CButtonST::CtlColor(CDC *pDC, UINT nCtlColor)
{
    return (HBRUSH)::GetStockObject(NULL_BRUSH);
} // End of CtlColor

void CButtonST::OnSysColorChange()
{
    CButton::OnSysColorChange();

    m_dcBk.DeleteDC();
    m_bmpBk.DeleteObject();
} // End of OnSysColorChange

LRESULT CButtonST::OnSetStyle(WPARAM wParam, LPARAM lParam)
{
    UINT nNewType = (wParam & SS_TYPEMASK);

    // Update default state flag
    if (nNewType == BS_DEFPUSHBUTTON)
    {
        m_bIsDefault = TRUE;
    } // if
    else if (nNewType == BS_PUSHBUTTON)
    {
        // Losing default state always allowed
        m_bIsDefault = FALSE;
    } // if

    // Can't change control type after owner-draw is set.
    // Let the system process changes to other style bits
    // and redrawing, while keeping owner-draw style
    return DefWindowProc(BM_SETSTYLE, (wParam & ~SS_TYPEMASK) | BS_OWNERDRAW, lParam);
} // End of OnSetStyle

LRESULT CButtonST::OnSetCheck(WPARAM wParam, LPARAM lParam)
{
    ASSERT(m_bIsCheckBox);

    switch (wParam)
    {
        case BST_CHECKED:
        case BST_INDETERMINATE: // Indeterminate state is handled like checked state
            SetCheck(1);
            break;
        default:
            SetCheck(0);
            break;
    } // switch

    return 0;
} // End of OnSetCheck

LRESULT CButtonST::OnGetCheck(WPARAM wParam, LPARAM lParam)
{
    //ASSERT(m_bIsCheckBox);
    return GetCheck();
} // End of OnGetCheck

LRESULT CButtonST::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu)
{
    //if (BCMenu::IsMenu(pMenu))
    //    return BCMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);

    return CButton::OnMenuChar(nChar, nFlags, pMenu);
} // End of OnMenuChar

void CButtonST::OnMeasureItem(int iIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    BOOL bSetFlag = FALSE;
/*
    if (lpMeasureItemStruct->CtlType == ODT_MENU)
    {
        if (IsMenu((HMENU)lpMeasureItemStruct->itemID) &&
            BCMenu::IsMenu((HMENU)lpMeasureItemStruct->itemID))
        {
            m_menuPopup.MeasureItem(lpMeasureItemStruct);
            bSetFlag = TRUE;
            return;
        } // if
    } // if
*/
    //if (!bSetFlag) 
    CButton::OnMeasureItem(iIDCtl, lpMeasureItemStruct);
} // End of OnMeasureItem

void CButtonST::OnEnable(BOOL bEnable)
{
    CWnd*   pWnd = NULL;

    CButton::OnEnable(bEnable);

    COMP_BFALSE(bEnable);

    //if (bEnable == FALSE)
    pWnd = GetParent()->GetNextDlgTabItem(this);
    if (pWnd)
        pWnd->SetFocus();
    else
        GetParent()->SetFocus();

    CancelHover();
} // End of OnEnable

void CButtonST::OnKillFocus(CWnd *pNewWnd)
{
    CButton::OnKillFocus(pNewWnd);
    CancelHover();
} // End of OnKillFocus

void CButtonST::OnActivate(UINT nState, CWnd *pWndOther, BOOL bMinimized)
{
    CButton::OnActivate(nState, pWndOther, bMinimized);

    if (nState == WA_INACTIVE)  
        CancelHover();
} // End of OnActivate

void CButtonST::OnCancelMode()
{
    CButton::OnCancelMode();
    CancelHover();
} // End of OnCancelMode

BOOL CButtonST::OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT nMessage)
{
    // If a cursor was specified then use it!
    if (m_hCursor != NULL)
    {
        ::SetCursor(m_hCursor);
        return TRUE;
    } // if

    return CButton::OnSetCursor(pWnd, nHitTest, nMessage);
} // End of OnSetCursor

void CButtonST::CancelHover()
{
    COMP_BFALSE(m_bIsFlat);

    // Only for flat buttons
    // if (m_bIsFlat)
    {
        if (m_bMouseOnButton)
        {
            m_bMouseOnButton = FALSE;
            Invalidate();
        } // if
    } // if
} // End of CancelHover

void CButtonST::OnMouseMove(UINT nFlags, CPoint point)
{
    CWnd            *pwndUnderMouse = NULL;
    CWnd            *pwndActive     = this;
    TRACKMOUSEEVENT csTME;

    CButton::OnMouseMove(nFlags, point);

    ClientToScreen(&point);
    pwndUnderMouse = WindowFromPoint(point);

    // If the mouse enter the button with the left button pressed then do nothing
    if (nFlags & MK_LBUTTON && m_bMouseOnButton == FALSE) 
        return;

    // If our button is not flat then do nothing
    COMP_BFALSE(m_bIsFlat);

    if (m_bAlwaysTrack == FALSE)    
        pwndActive = GetActiveWindow();

    if (pwndUnderMouse && pwndUnderMouse->m_hWnd == m_hWnd && pwndActive)
    {
        if (!m_bMouseOnButton)
        {
            m_bMouseOnButton = TRUE;

            Invalidate();

            csTME.cbSize    = sizeof(csTME);
            csTME.dwFlags   = TME_LEAVE;
            csTME.hwndTrack = m_hWnd;
            ::_TrackMouseEvent(&csTME);
        } // if
        return;
    } 

    CancelHover();
} // End of OnMouseMove

// Handler for WM_MOUSELEAVE
LRESULT CButtonST::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
    CancelHover();
    return 0;
} // End of OnMouseLeave

BOOL CButtonST::OnClicked()
{
    CRect   rcWnd;
    DWORD   dwRetValue  = 0;
    //BCMenu  *psub       = NULL;
    SHELLEXECUTEINFO csSEI;

    SetFocus();

    if (m_bIsCheckBox)
    {
        m_iCheck = !m_iCheck;
        Invalidate();
        return FALSE;
    } // if
/*
    // Handle the menu (if any)
    if (m_menuPopup.m_hMenu)
    {
        GetWindowRect(rcWnd);

        m_bMenuDisplayed = TRUE;
        Invalidate();

        psub        = (BCMenu*)m_menuPopup.GetSubMenu(0);
        dwRetValue  = psub->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, rcWnd.left, rcWnd.bottom, this, NULL);

        m_bMenuDisplayed = FALSE;
        Invalidate();

        if (dwRetValue)
            ::PostMessage(m_hParentWndMenu, WM_COMMAND, MAKEWPARAM(dwRetValue, 0), (LPARAM)NULL);

        return FALSE;
    } // if
*/
    // else m_menuPopup.m_hMenu
    // Handle the URL (if any)
    if (_tcslen(m_szURL) > 0)
    {
        memset(&csSEI, 0, sizeof(csSEI));
        csSEI.cbSize = sizeof(SHELLEXECUTEINFO);
        csSEI.fMask = SEE_MASK_FLAG_NO_UI;
        csSEI.lpVerb = _T("open");
        csSEI.lpFile = m_szURL;
        csSEI.nShow = SW_SHOWMAXIMIZED;
        ::ShellExecuteEx(&csSEI);
    } // if

    return FALSE;
} // End of OnClicked

/* 获取当前按钮的属性，设置变量，供下面画按钮使用 */
void CButtonST::GetBtnProperty(LPDRAWITEMSTRUCT lpDIS)
{
    ASSERT_NULL(lpDIS);

    // Checkbox?
    if (m_bIsCheckBox)
    {
        m_bIsPressed  =  (lpDIS->itemState & ODS_SELECTED) || (m_iCheck != 0);
    } // if
    else    // Normal button OR other button style ...
    {
        m_bIsPressed  =  (lpDIS->itemState & ODS_SELECTED);

        // If there is a menu and it's displayed, draw the button as pressed
        //if (m_menuPopup.m_hMenu && m_bMenuDisplayed)
        //    m_bIsPressed = TRUE;
    } // else

    m_bIsFocused  = (lpDIS->itemState & ODS_FOCUS);
    m_bIsDisabled = (lpDIS->itemState & ODS_DISABLED);
}

void CButtonST::DeflateRect(CDC *pDC, CRect *prcItem)
{
    CBrush  br(RGB(0,0,0));
    
    ASSERT_NULL(pDC);
    ASSERT_NULL(prcItem);

    COMP_BTRUE(m_bIsFlat);

    //if (m_bIsFlat == FALSE)
    if (m_bIsFocused || m_bIsDefault)
    {
        pDC->FrameRect(prcItem, &br);
        prcItem->DeflateRect(1, 1);
    } // if

    return;
}

// Draw pressed button
void CButtonST::DrawPressedButton(CDC *pDC, CRect rcItem)
{
    CBrush brBtnShadow(GetSysColor(COLOR_BTNSHADOW));

    ASSERT_NULL(pDC);

    /* Flat Button */
    if (m_bIsFlat)
    {
        if (m_bDrawBorder)
        {
            OnDrawBorder(pDC, &rcItem);
        }
        return;
    }

    /* Not Flat Button */
    pDC->FrameRect(&rcItem, &brBtnShadow);

    return;
}

// draw non pressed button
void CButtonST::DrawNoPressButton(CDC *pDC, CRect rcItem)
{
    CPen    penBtnHiLight(PS_SOLID, 0, GetSysColor(COLOR_BTNHILIGHT)); // White
    CPen    pen3DLight   (PS_SOLID, 0, GetSysColor(COLOR_3DLIGHT));    // Light gray
    CPen    penBtnShadow (PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW));  // Dark gray
    CPen    pen3DDKShadow(PS_SOLID, 0, GetSysColor(COLOR_3DDKSHADOW)); // Black
    CPen    *pOldPen = NULL;

    ASSERT_NULL(pDC);

    /* Flat Button */
    if (m_bIsFlat)
    {
        if (m_bMouseOnButton && m_bDrawBorder)
        {
            OnDrawBorder(pDC, &rcItem);
        }

        return;
    }

    /* Not Flat Button */

    // Draw top-left borders
    // White line
    pOldPen = pDC->SelectObject(&penBtnHiLight);
    pDC->MoveTo(rcItem.left, rcItem.bottom-1);
    pDC->LineTo(rcItem.left, rcItem.top);
    pDC->LineTo(rcItem.right, rcItem.top);
    // Light gray line
    pDC->SelectObject(pen3DLight);
    pDC->MoveTo(rcItem.left+1, rcItem.bottom-1);
    pDC->LineTo(rcItem.left+1, rcItem.top+1);
    pDC->LineTo(rcItem.right, rcItem.top+1);
    // Draw bottom-right borders
    // Black line
    pDC->SelectObject(pen3DDKShadow);
    pDC->MoveTo(rcItem.left, rcItem.bottom-1);
    pDC->LineTo(rcItem.right-1, rcItem.bottom-1);
    pDC->LineTo(rcItem.right-1, rcItem.top-1);
    // Dark gray line
    pDC->SelectObject(penBtnShadow);
    pDC->MoveTo(rcItem.left+1, rcItem.bottom-2);
    pDC->LineTo(rcItem.right-2, rcItem.bottom-2);
    pDC->LineTo(rcItem.right-2, rcItem.top);
    //
    pDC->SelectObject(pOldPen);

    return;
}

/* Write the button title */
void CButtonST::DrawBtnTitle(CDC *pDC, CString strTitle, CRect rcCap)
{
    CRect rcCenter;

    ASSERT_NULL(pDC);
    
    // Draw the button's title
    // If button is pressed then "press" title also
    if (m_bIsPressed && m_bIsCheckBox == FALSE)
    {
        rcCap.OffsetRect(1, 1);
    }

    // ONLY FOR DEBUG
    //CBrush brBtnShadow(RGB(255, 0, 0));
    //pDC->FrameRect(&capRect, &brBtnShadow);

    // Center text
    rcCenter = rcCap;
    pDC->DrawText(strTitle, -1, rcCap, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);
    rcCap.OffsetRect((rcCenter.Width() - rcCap.Width())/2, (rcCenter.Height() - rcCap.Height())/2);
    /* RFU
    capRect.OffsetRect(0, (centerRect.Height() - capRect.Height())/2);
    capRect.OffsetRect((centerRect.Width() - capRect.Width())-4, (centerRect.Height() - capRect.Height())/2);
    */

    pDC->SetBkMode(TRANSPARENT);
    /*
    pDC->DrawState(capRect.TopLeft(), capRect.Size(), (LPCTSTR)sTitle, (bIsDisabled ? DSS_DISABLED : DSS_NORMAL),
                    TRUE, 0, (CBrush*)NULL);
    */
    if (m_bIsDisabled)
    {
        rcCap.OffsetRect(1, 1);
        pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
        pDC->DrawText(strTitle, -1, rcCap, DT_WORDBREAK | DT_CENTER);
        rcCap.OffsetRect(-1, -1);
        pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
        pDC->DrawText(strTitle, -1, rcCap, DT_WORDBREAK | DT_CENTER);
        return;
    } // if

    // !m_bIsDisabled
    if (m_bMouseOnButton || m_bIsPressed)
    {
        pDC->SetTextColor(m_crColors[BTNST_COLOR_FG_IN]);
        pDC->SetBkColor(m_crColors[BTNST_COLOR_BK_IN]);
    } // if
    else
    {
        pDC->SetTextColor(m_crColors[BTNST_COLOR_FG_OUT]);
        pDC->SetBkColor(m_crColors[BTNST_COLOR_BK_OUT]);
    } // else
    pDC->DrawText(strTitle, -1, rcCap, DT_WORDBREAK | DT_CENTER);

    return;
}

// Draw the focus rect
void CButtonST::DrawFocusRect(CDC *pDC, CRect rcItem)
{
    CRect rcFocus;

    ASSERT_NULL(pDC);
    
    if (m_bIsFlat == FALSE || (m_bIsFlat && m_bDrawFlatFocus))
    {
        // Draw the focus rect
        if (m_bIsFocused)
        {
            rcFocus = rcItem;
            rcFocus.DeflateRect(3, 3);
            pDC->DrawFocusRect(&rcFocus);
        } // if
    } // if

    return;
}

void CButtonST::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
    CRect   rcItem;
    CRect   rcCaption;
    CString strTitle;
    CDC*    pDC = NULL;

    ASSERT_NULL(lpDIS);

    pDC = CDC::FromHandle(lpDIS->hDC);
    ASSERT_NULL(pDC);

    /* 获取按钮的属性 */
    GetBtnProperty(lpDIS);

    rcItem = lpDIS->rcItem;

    pDC->SetBkMode(TRANSPARENT);

    DeflateRect(pDC, &rcItem);

    // Prepare draw... paint button background

    // Draw transparent?
    if (m_bDrawTransparent)
        PaintBk(pDC);
    else
        OnDrawBackground(pDC, &rcItem);

    // Draw pressed button
    if (m_bIsPressed)
    {
        DrawNoPressButton(pDC, rcItem);
    }
    else // ...else draw non pressed button
    {
        DrawNoPressButton(pDC, rcItem);
    } // else

    // Read the button's title
    GetWindowText(strTitle);
    rcCaption = lpDIS->rcItem;

    // Draw the icon
    if (m_csIcons[0].hIcon)
    {
        DrawTheIcon(pDC, !strTitle.IsEmpty(), &lpDIS->rcItem, &rcCaption, m_bIsPressed, m_bIsDisabled);
    } // if

    if (m_csBitmaps[0].hBitmap)
    {
        pDC->SetBkColor(RGB(255,255,255));
        DrawTheBitmap(pDC, !strTitle.IsEmpty(), &lpDIS->rcItem, &rcCaption, m_bIsPressed, m_bIsDisabled);
    } // if

    // Write the button title (if any)
    if (strTitle.IsEmpty() == FALSE)
    {
        DrawBtnTitle(pDC, strTitle, rcCaption);
    } // if

    DrawFocusRect(pDC, rcItem);
} // End of DrawItem

void CButtonST::PaintBk(CDC *pDC)
{
    CRect       rcClt;
    CRect       rcWin;
    CClientDC   clDC(GetParent());

    ASSERT_NULL(pDC);

    GetClientRect(&rcClt);
    GetWindowRect(&rcWin);
    GetParent()->ScreenToClient(rcWin);

    if (m_dcBk.m_hDC == NULL)
    {
        m_dcBk.CreateCompatibleDC(&clDC);
        m_bmpBk.CreateCompatibleBitmap(&clDC, rcClt.Width(), rcClt.Height());
        m_pbmpOldBk = m_dcBk.SelectObject(&m_bmpBk);
        m_dcBk.BitBlt(0, 0, rcClt.Width(), rcClt.Height(), &clDC, rcWin.left, rcWin.top, SRCCOPY);
    } // if

    pDC->BitBlt(0, 0, rcClt.Width(), rcClt.Height(), &m_dcBk, 0, 0, SRCCOPY);
} // End of PaintBk

HBITMAP CButtonST::CreateBitmapMask(HBITMAP hSourceBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF crTransColor)
{
    HBITMAP     hMask       = NULL;
    HDC         hdcSrc      = NULL;
    HDC         hdcDest     = NULL;
    HBITMAP     hbmSrcT     = NULL;
    HBITMAP     hbmDestT    = NULL;
    COLORREF    crSaveBk;
    COLORREF    crSaveDestText;

    hMask = ::CreateBitmap(dwWidth, dwHeight, 1, 1, NULL);
    ASSERT_NULL_R(hMask, NULL);

    hdcSrc  = ::CreateCompatibleDC(NULL);
    ASSERT_NULL_R(hdcSrc, NULL);
    hdcDest = ::CreateCompatibleDC(NULL);
    ASSERT_NULL_R(hdcDest, NULL);

    hbmSrcT = (HBITMAP)::SelectObject(hdcSrc, hSourceBitmap);
    ASSERT_NULL_R(hbmSrcT, NULL);
    hbmDestT= (HBITMAP)::SelectObject(hdcDest, hMask);
    ASSERT_NULL_R(hbmDestT, NULL);

    crSaveBk = ::SetBkColor(hdcSrc, crTransColor);

    ::BitBlt(hdcDest, 0, 0, dwWidth, dwHeight, hdcSrc, 0, 0, SRCCOPY);

    crSaveDestText = ::SetTextColor(hdcSrc, RGB(255, 255, 255));
    ::SetBkColor(hdcSrc,RGB(0, 0, 0));

    ::BitBlt(hdcSrc, 0, 0, dwWidth, dwHeight, hdcDest, 0, 0, SRCAND);

    SetTextColor(hdcDest, crSaveDestText);

    ::SetBkColor(hdcSrc, crSaveBk);
    ::SelectObject(hdcSrc, hbmSrcT);
    ::SelectObject(hdcDest, hbmDestT);

    ::DeleteDC(hdcSrc);
    ::DeleteDC(hdcDest);

    return hMask;
} // End of CreateBitmapMask

// ST_ALIGN_HORIZ
void CButtonST::PrepareHorizRect(BOOL bHasTitle, CRect *prcTitle, DWORD dwWidth, DWORD dwHeight, CRect *prcImage)
{
    ASSERT_NULL(prcImage);
    
    // Center image vertically
    prcImage->top += ((prcImage->Height() - (long)dwHeight)/2);

    if (bHasTitle == FALSE)
    {
        // Center image horizontally
        prcImage->left += ((prcImage->Width() - (long)dwWidth)/2);
        return;
    }

    /* Has Title */
    ASSERT_NULL(prcTitle);
    // Image must be placed just inside the focus rect
    prcImage->left += 3;
    prcTitle->left += dwWidth + 3;

    return;
}

// ST_ALIGN_HORIZ_RIGHT
void CButtonST::PrepareHorizRightRect(BOOL bHasTitle, CRect *prcTitle, DWORD dwWidth, DWORD dwHeight, CRect *prcImage)
{
    CRect   rcBtn;

    ASSERT_NULL(prcImage);

    if (bHasTitle == FALSE)
    {
        // Center image horizontally
        prcImage->left += ((prcImage->Width() - (long)dwWidth)/2);
        return;
    }

    /* Has Title */
    ASSERT_NULL(prcTitle);
    GetClientRect(&rcBtn);
    // Image must be placed just inside the focus rect
    prcTitle->right = prcTitle->Width() - dwWidth - 3;
    prcTitle->left = 3;
    prcImage->left = rcBtn.right - dwWidth - 3;
    // Center image vertically
    prcImage->top += ((prcImage->Height() - (long)dwHeight)/2);

    return;
}

// ST_ALIGN_VERT
void CButtonST::PrepareVertRect(BOOL bHasTitle, CRect *prcTitle, DWORD dwWidth, DWORD dwHeight, CRect *prcImage)
{
    ASSERT_NULL(prcImage);

    // Center image horizontally
    prcImage->left += ((prcImage->Width() - (long)dwWidth)/2);
    if (bHasTitle == FALSE)
    {
        // Center image vertically
        prcImage->top += ((prcImage->Height() - (long)dwHeight)/2);
        return;
    }

    /* Has Title */
    ASSERT_NULL(prcTitle);
    prcImage->top = 3;
    prcTitle->top += dwHeight;
    return;
}

//
// Parameters:
//      [IN]    bHasTitle
//              TRUE if the button has a text
//      [IN]    prcItem
//              A pointer to a RECT structure indicating the allowed paint area
//      [IN/OUT]prcTitle
//              A pointer to a CRect object indicating the paint area reserved for the
//              text. This structure will be modified if necessary.
//      [IN]    bIsPressed
//              TRUE if the button is currently pressed
//      [IN]    dwWidth
//              Width of the image (icon or bitmap)
//      [IN]    dwHeight
//              Height of the image (icon or bitmap)
//      [OUT]   prcImage
//              A pointer to a CRect object that will receive the area available to the image
//
void CButtonST::PrepareImageRect(BOOL bHasTitle, RECT *prcItem, CRect *prcTitle, BOOL bIsPressed, DWORD dwWidth, DWORD dwHeight, CRect *prcImage)
{
    ASSERT_NULL(prcItem);
    ASSERT_NULL(prcImage);

    prcImage->CopyRect(prcItem);

    switch (m_ucAlign)
    {
        case ST_ALIGN_HORIZ:
            PrepareHorizRect(bHasTitle, prcTitle, dwWidth, dwHeight, prcImage);
            break;

        case ST_ALIGN_HORIZ_RIGHT:
            PrepareHorizRightRect(bHasTitle, prcTitle, dwWidth, dwHeight, prcImage);
            break;

        case ST_ALIGN_VERT:
            PrepareVertRect(bHasTitle, prcTitle, dwWidth, dwHeight, prcImage);
            break;
    }

    // If button is pressed then press image also
    if (bIsPressed && m_bIsCheckBox == FALSE)
        prcImage->OffsetRect(1, 1);
} // End of PrepareImageRect

void CButtonST::DrawTheIcon(CDC *pDC, BOOL bHasTitle, RECT *prcItem, CRect *prcTitle, BOOL bIsPressed, BOOL bIsDisabled)
{
    BYTE    ucIndex = 0;
    CRect   rcImage;
    
    ASSERT_NULL(pDC);
    ASSERT_NULL(prcItem);

    // Select the icon to use
    GetDrawIndex(bIsPressed, 1);

    PrepareImageRect(bHasTitle, prcItem, prcTitle, bIsPressed, m_csIcons[ucIndex].dwWidth, m_csIcons[ucIndex].dwHeight, &rcImage);

    // Ole'!
    pDC->DrawState( rcImage.TopLeft(),
                    rcImage.Size(),
                    m_csIcons[ucIndex].hIcon,
                    (bIsDisabled ? DSS_DISABLED : DSS_NORMAL),
                    (CBrush*)NULL);
} // End of DrawTheIcon

/* iType: 0:Bitmap; 1:Icon */
BYTE CButtonST::GetDrawIndex(BOOL bIsPressed, int iType)
{
    BOOL bTemp = FALSE;
    
    // Select the bitmap to use
    //if (( m_bIsCheckBox &&  bIsPressed) || 
    //    (!m_bIsCheckBox && (bIsPressed  || m_bMouseOnButton)))
    //    return 0;
    bTemp = bIsPressed  || m_bMouseOnButton;
    bTemp = bTemp && (!m_bIsCheckBox);
    bTemp = bTemp || (m_bIsCheckBox &&  bIsPressed);
    COMP_BTRUE_R(bTemp, 0);

    if(iType == 0)
    {
        ASSERT_NULL_R(m_csBitmaps[1].hBitmap, 0);

        return 1;
    }
    
    ASSERT_NULL_R(m_csIcons[1].hIcon, 0);

    return 1;
}

void CButtonST::DrawTheBitmap(CDC *pDC, BOOL bHasTitle, RECT *rItem, CRect *prcCaption, BOOL bIsPressed, BOOL bIsDisabled)
{
    HDC         hdcBmpMem   = NULL;
    HBITMAP     hbmOldBmp   = NULL;
    HDC         hdcMskMem   = NULL;
    HBITMAP     hbmOldMsk   = NULL;
    HDC         hDisableDC  = NULL;
    HBITMAP     hDisableBmp = NULL;
    HBITMAP     hOldDisBmp  = NULL;
    BYTE        ucIndex     = 0;
    COLORREF    crOldColor;
    CRect       rcImage;
    RECT        rcDisRect;

    ASSERT_NULL(pDC);
    ASSERT_NULL(rItem);

    // Select the bitmap to use
    ucIndex = GetDrawIndex(bIsPressed, 0);

    PrepareImageRect(bHasTitle, rItem, prcCaption, bIsPressed, m_csBitmaps[ucIndex].dwWidth,
                     m_csBitmaps[ucIndex].dwHeight, &rcImage);

    hdcBmpMem = ::CreateCompatibleDC(pDC->m_hDC);
    hbmOldBmp = (HBITMAP)::SelectObject(hdcBmpMem, m_csBitmaps[ucIndex].hBitmap);

    hdcMskMem = ::CreateCompatibleDC(NULL);
    hbmOldMsk = (HBITMAP)::SelectObject(hdcMskMem, m_csBitmaps[ucIndex].hMask);

    if (bIsDisabled && m_bShowDisabledBitmap)
    {
        hDisableDC  = ::CreateCompatibleDC(pDC->m_hDC);
        hDisableBmp = ::CreateCompatibleBitmap(pDC->m_hDC, m_csBitmaps[ucIndex].dwWidth,
                                               m_csBitmaps[ucIndex].dwHeight);
        hOldDisBmp  = (HBITMAP)::SelectObject(hDisableDC, hDisableBmp);

        rcDisRect.left   = 0;
        rcDisRect.top    = 0;
        rcDisRect.right  = rcImage.right + 1;
        rcDisRect.bottom = rcImage.bottom + 1;
        ::FillRect(hDisableDC, &rcDisRect, (HBRUSH)RGB(255, 255, 255));

        crOldColor = ::SetBkColor(hDisableDC, RGB(255,255,255));

        ::BitBlt(hDisableDC, 0, 0, m_csBitmaps[ucIndex].dwWidth, m_csBitmaps[ucIndex].dwHeight,
                 hdcMskMem, 0, 0, SRCAND);
        ::BitBlt(hDisableDC, 0, 0, m_csBitmaps[ucIndex].dwWidth, m_csBitmaps[ucIndex].dwHeight,
                 hdcBmpMem, 0, 0, SRCPAINT);

        ::SetBkColor(hDisableDC, crOldColor);
        ::SelectObject(hDisableDC, hOldDisBmp);
        ::DeleteDC(hDisableDC);

        pDC->DrawState( CPoint(rcImage.left/*+1*/, rcImage.top),
                        CSize(m_csBitmaps[ucIndex].dwWidth, m_csBitmaps[ucIndex].dwHeight),
                        hDisableBmp, DST_BITMAP | DSS_DISABLED);

        DELETE_OBJECT(hDisableBmp);
    } // if
    else
    {
        ::BitBlt(pDC->m_hDC, rcImage.left, rcImage.top, m_csBitmaps[ucIndex].dwWidth,
                 m_csBitmaps[ucIndex].dwHeight, hdcMskMem, 0, 0, SRCAND);
        ::BitBlt(pDC->m_hDC, rcImage.left, rcImage.top, m_csBitmaps[ucIndex].dwWidth,
                 m_csBitmaps[ucIndex].dwHeight, hdcBmpMem, 0, 0, SRCPAINT);
    } // else

    ::SelectObject(hdcMskMem, hbmOldMsk);
    ::DeleteDC(hdcMskMem);

    ::SelectObject(hdcBmpMem, hbmOldBmp);
    ::DeleteDC(hdcBmpMem);
} // End of DrawTheBitmap

// This function creates a grayscale icon starting from a given icon.
// The resulting icon will have the same size of the original one.
//
// Parameters:
//      [IN]    hIcon
//              Handle to the original icon.
//
// Return value:
//      If the function succeeds, the return value is the handle to the newly created
//      grayscale icon.
//      If the function fails, the return value is NULL.
//
HICON CButtonST::CreateGrayscaleIcon(HICON hIcon)
{
    HICON       hGrayIcon   = NULL;
    HDC         hMainDC     = NULL;
    HDC         hMemDC1     = NULL;
    HDC         hMemDC2     = NULL;
    BITMAP      bmpOld;
    HBITMAP     hOldBmp1    = NULL;
    HBITMAP     hOldBmp2    = NULL;
    ICONINFO    csII;
    ICONINFO    csGrayII;
    BOOL        bRetValue   = FALSE;
    DWORD       dwLoopY     = 0;
    DWORD       dwLoopX     = 0;
    COLORREF    crPixel     = 0;
    BYTE        ucNewPixel  = 0;

    bRetValue = ::GetIconInfo(hIcon, &csII);
    COMP_BFALSE_R(bRetValue, NULL);

    hMainDC = ::GetDC(m_hWnd);
    ASSERT_NULL_R(hMainDC,NULL);
    hMemDC1 = ::CreateCompatibleDC(hMainDC);
    hMemDC2 = ::CreateCompatibleDC(hMainDC);
    ASSERT_NULL_R(hMemDC1,NULL);
    ASSERT_NULL_R(hMemDC2,NULL);

    if (::GetObject(csII.hbmColor, sizeof(BITMAP), &bmpOld))
    {
        csGrayII.hbmColor = ::CreateBitmap(csII.xHotspot*2, csII.yHotspot*2,
                                           bmpOld.bmPlanes, bmpOld.bmBitsPixel, NULL);
        if (csGrayII.hbmColor)
        {
            hOldBmp1 = (HBITMAP)::SelectObject(hMemDC1, csII.hbmColor);
            hOldBmp2 = (HBITMAP)::SelectObject(hMemDC2, csGrayII.hbmColor);

            ::BitBlt(hMemDC2, 0, 0, csII.xHotspot*2, csII.yHotspot*2, hMemDC1, 0, 0, SRCCOPY);

            for (dwLoopY = 0; dwLoopY < csII.yHotspot*2; dwLoopY++)
            {
                for (dwLoopX = 0; dwLoopX < csII.xHotspot*2; dwLoopX++)
                {
                    crPixel = ::GetPixel(hMemDC2, dwLoopX, dwLoopY);

                    ucNewPixel = (BYTE)((GetRValue(crPixel) * 0.299) +
                                        (GetGValue(crPixel) * 0.587) +
                                        (GetBValue(crPixel) * 0.114));
                    if (crPixel)
                        ::SetPixel(hMemDC2, dwLoopX, dwLoopY, RGB(ucNewPixel, ucNewPixel, ucNewPixel));
                } // for
            } // for

            ::SelectObject(hMemDC1, hOldBmp1);
            ::SelectObject(hMemDC2, hOldBmp2);

            csGrayII.hbmMask = csII.hbmMask;

            csGrayII.fIcon = TRUE;
            hGrayIcon = ::CreateIconIndirect(&csGrayII);
        } // if

        DELETE_OBJECT(csGrayII.hbmColor);
    } // if

    DELETE_OBJECT(csII.hbmColor);
    DELETE_OBJECT(csII.hbmMask);
    ::DeleteDC(hMemDC1);
    ::DeleteDC(hMemDC2);
    ::ReleaseDC(m_hWnd, hMainDC);

    return hGrayIcon;
} // End of CreateGrayscaleIcon

// This function assigns icons to the button.
// Any previous icon or bitmap will be removed.
//
// Parameters:
//      [IN]    iIcon  
//              ID number of the icon resource to show when the mouse is over/outside the button.
//              Pass NULL to remove any icon from the button.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//      BTNST_INVALIDRESOURCE
//          Failed loading the specified resource.
//
DWORD CButtonST::SetSameIcon(int iIcon)
{
    HICON       hIcon           = NULL;
    HINSTANCE   hInstResource   = NULL;

    // Find correct resource handle
    hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(iIcon), RT_GROUP_ICON);

    // Set icon
    hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(iIcon), IMAGE_ICON, 0, 0, 0);
    m_bIconSame = TRUE;
    m_iIconSame = iIcon;
    return SetIcon(hIcon, hIcon);
}

// This function assigns icons to the button.
// Any previous icon or bitmap will be removed.
//
// Parameters:
//      [IN]    iIconIn
//              ID number of the icon resource to show when the mouse is over the button.
//              Pass NULL to remove any icon from the button.
//      [IN]    iIconOut
//              ID number of the icon resource to show when the mouse is outside the button.
//              Can be NULL.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//      BTNST_INVALIDRESOURCE
//          Failed loading the specified resource.
//
DWORD CButtonST::SetIcon(int iIconIn, int iIconOut)
{
    HICON       hIconIn         = NULL;
    HICON       hIconOut        = NULL;
    HINSTANCE   hInstResource   = NULL;

    // Find correct resource handle
    hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(iIconIn), RT_GROUP_ICON);

    // Set icon when the mouse is IN the button
    hIconIn = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(iIconIn), IMAGE_ICON, 0, 0, 0);

    // Set icon when the mouse is OUT the button
    if (iIconOut)
    {
        hIconOut = BTNST_AUTO_GRAY;
        if (iIconOut != (int)BTNST_AUTO_GRAY)
            hIconOut = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(iIconOut), IMAGE_ICON, 0, 0, 0);
    } // if

    m_bIconSame = FALSE;
    if(iIconIn == iIconOut)
        m_bIconSame = TRUE;

    return SetIcon(hIconIn, hIconOut);
} // End of SetIcon

// This function assigns icons to the button.
// Any previous icon or bitmap will be removed.
//
// Parameters:
//      [IN]    hIconIn
//              Handle fo the icon to show when the mouse is over the button.
//              Pass NULL to remove any icon from the button.
//      [IN]    hIconOut
//              Handle to the icon to show when the mouse is outside the button.
//              Can be NULL.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//      BTNST_INVALIDRESOURCE
//          Failed loading the specified resource.
//
DWORD CButtonST::SetIcon(HICON hIconIn, HICON hIconOut)
{
    BOOL        bRetValue = FALSE;
    ICONINFO    ii;

    // Free any loaded resource
    FreeResources();

    if (hIconIn)
    {
        // Icon when mouse over button?
        m_csIcons[0].hIcon = hIconIn;
        // Get icon dimension
        ::ZeroMemory(&ii, sizeof(ICONINFO));
        bRetValue = ::GetIconInfo(hIconIn, &ii);
        if (bRetValue == FALSE)
        {
            FreeResources();
            return BTNST_INVALIDRESOURCE;
        } // if

        m_csIcons[0].dwWidth    = (DWORD)(ii.xHotspot * 2);
        m_csIcons[0].dwHeight   = (DWORD)(ii.yHotspot * 2);
        DELETE_OBJECT(ii.hbmMask);
        DELETE_OBJECT(ii.hbmColor);

        // Icon when mouse outside button?
        if (hIconOut)
        {
            if (hIconOut == BTNST_AUTO_GRAY)
            {
                hIconOut = CreateGrayscaleIcon(hIconIn);
            } // if

            m_csIcons[1].hIcon = hIconOut;
            // Get icon dimension
            ::ZeroMemory(&ii, sizeof(ICONINFO));
            bRetValue = ::GetIconInfo(hIconOut, &ii);
            if (bRetValue == FALSE)
            {
                FreeResources();
                return BTNST_INVALIDRESOURCE;
            } // if

            m_csIcons[1].dwWidth    = (DWORD)(ii.xHotspot * 2);
            m_csIcons[1].dwHeight   = (DWORD)(ii.yHotspot * 2);
            DELETE_OBJECT(ii.hbmMask);
            DELETE_OBJECT(ii.hbmColor);
        } // if
    } // if

    Invalidate();

    return BTNST_OK;
} // End of SetIcon

// This function assigns bitmaps to the button.
// Any previous icon or bitmap will be removed.
//
// Parameters:
//      [IN]    iBitmapIn
//              ID number of the bitmap resource to show when the mouse is over the button.
//              Pass NULL to remove any bitmap from the button.
//      [IN]    crTransColorIn
//              Color (inside iBitmapIn) to be used as transparent color.
//      [IN]    iBitmapOut
//              ID number of the bitmap resource to show when the mouse is outside the button.
//              Can be NULL.
//      [IN]    crTransColorOut
//              Color (inside iBitmapOut) to be used as transparent color.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//      BTNST_INVALIDRESOURCE
//          Failed loading the specified resource.
//      BTNST_FAILEDMASK
//          Failed creating mask bitmap.
//
DWORD CButtonST::SetBitmaps(int iBitmapIn, COLORREF crTransColorIn, int iBitmapOut, COLORREF crTransColorOut)
{
    HBITMAP     hBitmapIn       = NULL;
    HBITMAP     hBitmapOut      = NULL;
    HINSTANCE   hInstResource   = NULL;

    // Find correct resource handle
    hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(iBitmapIn), RT_BITMAP);

    // Load bitmap In
    hBitmapIn = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(iBitmapIn), IMAGE_BITMAP, 0, 0, 0);

    // Load bitmap Out
    if (iBitmapOut)
        hBitmapOut = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(iBitmapOut), IMAGE_BITMAP, 0, 0, 0);

    return SetBitmaps(hBitmapIn, crTransColorIn, hBitmapOut, crTransColorOut);
} // End of SetBitmaps

// This function assigns bitmaps to the button.
// Any previous icon or bitmap will be removed.
//
// Parameters:
//      [IN]    hBitmapIn
//              Handle fo the bitmap to show when the mouse is over the button.
//              Pass NULL to remove any bitmap from the button.
//      [IN]    crTransColorIn
//              Color (inside hBitmapIn) to be used as transparent color.
//      [IN]    hBitmapOut
//              Handle to the bitmap to show when the mouse is outside the button.
//              Can be NULL.
//      [IN]    crTransColorOut
//              Color (inside hBitmapOut) to be used as transparent color.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//      BTNST_INVALIDRESOURCE
//          Failed loading the specified resource.
//      BTNST_FAILEDMASK
//          Failed creating mask bitmap.
//
DWORD CButtonST::SetBitmaps(HBITMAP hBitmapIn, COLORREF crTransColorIn, HBITMAP hBitmapOut, COLORREF crTransColorOut)
{
    int     iRetValue = 0;
    BITMAP  csBitmapSize;

    // Free any loaded resource
    FreeResources();

    if (hBitmapIn)
    {
        m_csBitmaps[0].hBitmap = hBitmapIn;
        m_csBitmaps[0].crTransparent = crTransColorIn;
        // Get bitmap size
        iRetValue = ::GetObject(hBitmapIn, sizeof(csBitmapSize), &csBitmapSize);
        if (iRetValue == 0)
        {
            FreeResources();
            return BTNST_INVALIDRESOURCE;
        } // if
        m_csBitmaps[0].dwWidth = (DWORD)csBitmapSize.bmWidth;
        m_csBitmaps[0].dwHeight = (DWORD)csBitmapSize.bmHeight;

        // Create mask for bitmap In
        m_csBitmaps[0].hMask = CreateBitmapMask(hBitmapIn, m_csBitmaps[0].dwWidth, m_csBitmaps[0].dwHeight, crTransColorIn);
        if (m_csBitmaps[0].hMask == NULL)
        {
            FreeResources();
            return BTNST_FAILEDMASK;
        } // if

        if (hBitmapOut)
        {
            m_csBitmaps[1].hBitmap = hBitmapOut;
            m_csBitmaps[1].crTransparent = crTransColorOut;
            // Get bitmap size
            iRetValue = ::GetObject(hBitmapOut, sizeof(csBitmapSize), &csBitmapSize);
            if (iRetValue == 0)
            {
                FreeResources();
                return BTNST_INVALIDRESOURCE;
            } // if
            m_csBitmaps[1].dwWidth = (DWORD)csBitmapSize.bmWidth;
            m_csBitmaps[1].dwHeight = (DWORD)csBitmapSize.bmHeight;

            // Create mask for bitmap Out
            m_csBitmaps[1].hMask = CreateBitmapMask(hBitmapOut, m_csBitmaps[1].dwWidth, m_csBitmaps[1].dwHeight, crTransColorOut);
            if (m_csBitmaps[1].hMask == NULL)
            {
                FreeResources();
                return BTNST_FAILEDMASK;
            } // if
        } // if
    } // if

    Invalidate();

    return BTNST_OK;
} // End of SetBitmaps

// This functions sets the button to have a standard or flat style.
//
// Parameters:
//      [IN]    bFlat
//              If TRUE the button will have a flat style, else
//              will have a standard style.
//              By default, CButtonST buttons are flat.
//      [IN]    bRepaint
//              If TRUE the control will be repainted.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//
DWORD CButtonST::SetFlat(BOOL bFlat, BOOL bRepaint)
{
    m_bIsFlat = bFlat;
    REPAINT_STBTN(bRepaint);

    return BTNST_OK;
} // End of SetFlat

// This function sets the alignment type between icon/bitmap and text.
//
// Parameters:
//      [IN]    ucAlign
//              Alignment type. Can be one of the following values:
//              ST_ALIGN_HORIZ          Icon/bitmap on the left, text on the right
//              ST_ALIGN_VERT           Icon/bitmap on the top, text on the bottom
//              ST_ALIGN_HORIZ_RIGHT    Icon/bitmap on the right, text on the left
//              By default, CButtonST buttons have ST_ALIGN_HORIZ alignment.
//      [IN]    bRepaint
//              If TRUE the control will be repainted.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//      BTNST_INVALIDALIGN
//          Alignment type not supported.
//
DWORD CButtonST::SetAlign(BYTE ucAlign, BOOL bRepaint)
{
    switch (ucAlign)
    {
        case ST_ALIGN_HORIZ:
        case ST_ALIGN_HORIZ_RIGHT:
        case ST_ALIGN_VERT:
            m_ucAlign = ucAlign;
            REPAINT_STBTN(bRepaint);
            return BTNST_OK;
            break;
    } // switch

    return BTNST_INVALIDALIGN;
} // End of SetAlign

// This function sets the state of the checkbox.
// If the button is not a checkbox, this function has no meaning.
//
// Parameters:
//      [IN]    nCheck
//              1 to check the checkbox.
//              0 to un-check the checkbox.
//      [IN]    bRepaint
//              If TRUE the control will be repainted.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//
DWORD CButtonST::SetCheck(int iCheck, BOOL bRepaint)
{
    if (m_bIsCheckBox)
    {
        m_iCheck = 1;
        if (iCheck == 0)
            m_iCheck = 0;

        REPAINT_STBTN(bRepaint);
    } // if

    return BTNST_OK;
} // End of SetCheck

// This function returns the current state of the checkbox.
// If the button is not a checkbox, this function has no meaning.
//
// Return value:
//      The current state of the checkbox.
//          1 if checked.
//          0 if not checked or the button is not a checkbox.
//
int CButtonST::GetCheck()
{
    return m_iCheck;
} // End of GetCheck

// This function sets all colors to a default value.
//
// Parameters:
//      [IN]    bRepaint
//              If TRUE the control will be repainted.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//
DWORD CButtonST::SetDefaultColors(BOOL bRepaint)
{
    m_crColors[BTNST_COLOR_BK_IN]       = ::GetSysColor(COLOR_BTNFACE);
    m_crColors[BTNST_COLOR_FG_IN]       = ::GetSysColor(COLOR_BTNTEXT);
    m_crColors[BTNST_COLOR_BK_OUT]      = ::GetSysColor(COLOR_BTNFACE);
    m_crColors[BTNST_COLOR_FG_OUT]      = ::GetSysColor(COLOR_BTNTEXT);
    m_crColors[BTNST_COLOR_BK_FOCUS]    = ::GetSysColor(COLOR_BTNFACE);
    m_crColors[BTNST_COLOR_FG_FOCUS]    = ::GetSysColor(COLOR_BTNTEXT);

    REPAINT_STBTN(bRepaint);

    return BTNST_OK;
} // End of SetDefaultColors

// This function sets the color to use for a particular state.
//
// Parameters:
//      [IN]    ucColorIndex
//              Index of the color to set. Can be one of the following values:
//              BTNST_COLOR_BK_IN       Background color when mouse is over the button
//              BTNST_COLOR_FG_IN       Text color when mouse is over the button
//              BTNST_COLOR_BK_OUT      Background color when mouse is outside the button
//              BTNST_COLOR_FG_OUT      Text color when mouse is outside the button
//              BTNST_COLOR_BK_FOCUS    Background color when the button is focused
//              BTNST_COLOR_FG_FOCUS    Text color when the button is focused
//      [IN]    crColor
//              New color.
//      [IN]    bRepaint
//              If TRUE the control will be repainted.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//      BTNST_INVALIDINDEX
//          Invalid color index.
//
DWORD CButtonST::SetColor(BYTE ucColorIndex, COLORREF crColor, BOOL bRepaint)
{
    COMP_BGE_R(ucColorIndex, BTNST_MAX_COLORS, BTNST_INVALIDINDEX);

    // Set new color
    m_crColors[ucColorIndex] = crColor;

    REPAINT_STBTN(bRepaint);

    return BTNST_OK;
} // End of SetColor

// This functions returns the color used for a particular state.
//
// Parameters:
//      [IN]    ucColorIndex
//              Index of the color to get.
//              See SetColor for the list of available colors.
//      [OUT]   crpColor
//              A pointer to a COLORREF that will receive the color.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//      BTNST_INVALIDINDEX
//          Invalid color index.
//
DWORD CButtonST::GetColor(BYTE ucColorIndex, COLORREF *crpColor)
{
    ASSERT_NULL_R(crpColor, BTNST_INVALIDINDEX);

    COMP_BGE_R(ucColorIndex, BTNST_MAX_COLORS, BTNST_INVALIDINDEX);

    // Get color
    *crpColor = m_crColors[ucColorIndex];

    return BTNST_OK;
} // End of GetColor

// This function applies an offset to the RGB components of the specified color.
// This function can be seen as an easy way to make a color darker or lighter than
// its default value.
//
// Parameters:
//      [IN]    ucColorIndex
//              Index of the color to set.
//              See SetColor for the list of available colors.
//      [IN]    shOffsetColor
//              A short value indicating the offset to apply to the color.
//              This value must be between -255 and 255.
//      [IN]    bRepaint
//              If TRUE the control will be repainted.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//      BTNST_INVALIDINDEX
//          Invalid color index.
//      BTNST_BADPARAM
//          The specified offset is out of range.
//
DWORD CButtonST::OffsetColor(BYTE ucColorIndex, short shOffset, BOOL bRepaint)
{
    BYTE    ucRed       = 0;
    BYTE    ucGreen     = 0;
    BYTE    ucBlue      = 0;
    short   shOffsetR   = shOffset;
    short   shOffsetG   = shOffset;
    short   shOffsetB   = shOffset;

    COMP_BGE_R(ucColorIndex, BTNST_MAX_COLORS, BTNST_INVALIDINDEX);
    COMP_BL_R (shOffset, -255, BTNST_BADPARAM);
    COMP_BG_R (shOffset,  255, BTNST_BADPARAM);

    // Get RGB components of specified color
    ucRed   = GetRValue(m_crColors[ucColorIndex]);
    ucGreen = GetGValue(m_crColors[ucColorIndex]);
    ucBlue  = GetBValue(m_crColors[ucColorIndex]);

    // Calculate max. allowed real offset
    if (shOffset > 0)
    {
        MIN_OFFSET(ucRed, shOffset, shOffsetR);
        MIN_OFFSET(ucGreen, shOffset, shOffsetG);
        MIN_OFFSET(ucBlue, shOffset, shOffsetB);

        shOffset = min(min(shOffsetR, shOffsetG), shOffsetB);
    } // if
    else
    {
        MAX_OFFSET(ucRed, shOffset, shOffsetR);
        MAX_OFFSET(ucGreen, shOffset, shOffsetG);
        MAX_OFFSET(ucBlue, shOffset, shOffsetB);

        shOffset = max(max(shOffsetR, shOffsetG), shOffsetB);
    } // else

    // Set new color
    m_crColors[ucColorIndex] = RGB(ucRed + shOffset, ucGreen + shOffset, ucBlue + shOffset);

    REPAINT_STBTN(bRepaint);

    return BTNST_OK;
} // End of OffsetColor

// This function sets the hilight logic for the button.
// Applies only to flat buttons.
//
// Parameters:
//      [IN]    bAlwaysTrack
//              If TRUE the button will be hilighted even if the window that owns it, is
//              not the active window.
//              If FALSE the button will be hilighted only if the window that owns it,
//              is the active window.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//
DWORD CButtonST::SetAlwaysTrack(BOOL bAlwaysTrack)
{
    m_bAlwaysTrack = bAlwaysTrack;
    return BTNST_OK;
} // End of SetAlwaysTrack

// This function sets the cursor to be used when the mouse is over the button.
//
// Parameters:
//      [IN]    nCursorId
//              ID number of the cursor resource.
//              Pass NULL to remove a previously loaded cursor.
//      [IN]    bRepaint
//              If TRUE the control will be repainted.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//      BTNST_INVALIDRESOURCE
//          Failed loading the specified resource.
//
DWORD CButtonST::SetBtnCursor(int iCursorId, BOOL bRepaint)
{
    HINSTANCE   hInstResource = NULL;

    // Destroy any previous cursor
    if (m_hCursor)
    {
        ::DestroyCursor(m_hCursor);
        m_hCursor = NULL;
    } // if

    // Load cursor
    if (iCursorId)
    {
        hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(iCursorId), RT_GROUP_CURSOR);
        // Load cursor resource
        m_hCursor = (HCURSOR)::LoadImage(hInstResource, MAKEINTRESOURCE(iCursorId), IMAGE_CURSOR, 0, 0, 0);
        // Repaint the button
        REPAINT_STBTN(bRepaint);
        // If something wrong
        ASSERT_NULL_R(m_hCursor, BTNST_INVALIDRESOURCE);
    } // if

    return BTNST_OK;
} // End of SetBtnCursor

// This function sets if the button border must be drawn.
// Applies only to flat buttons.
//
// Parameters:
//      [IN]    bDrawBorder
//              If TRUE the border will be drawn.
//      [IN]    bRepaint
//              If TRUE the control will be repainted.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//
DWORD CButtonST::DrawBorder(BOOL bDrawBorder, BOOL bRepaint)
{
    m_bDrawBorder = bDrawBorder;
    // Repaint the button
    REPAINT_STBTN(bRepaint);

    return BTNST_OK;
} // End of DrawBorder

// This function sets if the focus rectangle must be drawn for flat buttons.
//
// Parameters:
//      [IN]    bDrawFlatFocus
//              If TRUE the focus rectangle will be drawn also for flat buttons.
//      [IN]    bRepaint
//              If TRUE the control will be repainted.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//
DWORD CButtonST::DrawFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint)
{
    m_bDrawFlatFocus = bDrawFlatFocus;
    // Repaint the button
    REPAINT_STBTN(bRepaint);

    return BTNST_OK;
} // End of DrawFlatFocus

void CButtonST::InitToolTip()
{
    COMP_BNE(m_ToolTip.m_hWnd, NULL);

    /* if (m_ToolTip.m_hWnd == NULL) */
    // Create ToolTip control
    m_ToolTip.Create(this);
    // Create inactive
    m_ToolTip.Activate(FALSE);
    // Enable multiline
    m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
} // End of InitToolTip

// This function sets the text to show in the button tooltip.
//
// Parameters:
//      [IN]    nText
//              ID number of the string resource containing the text to show.
//      [IN]    bActivate
//              If TRUE the tooltip will be created active.
//
void CButtonST::SetTooltipText(int iText, BOOL bActivate)
{
    CString strText;

    // Load string resource
    strText.LoadString(iText);
    // If string resource is not empty
    if (strText.IsEmpty() == FALSE)
        SetTooltipText((LPCTSTR)strText, bActivate);
} // End of SetTooltipText

// This function sets the text to show in the button tooltip.
//
// Parameters:
//      [IN]    lpszText
//              Pointer to a null-terminated string containing the text to show.
//      [IN]    bActivate
//              If TRUE the tooltip will be created active.
//
void CButtonST::SetTooltipText(LPCTSTR lpszText, BOOL bActivate)
{
    CRect rcBtn;

    // We cannot accept NULL pointer
    ASSERT_NULL(lpszText);

    // Initialize ToolTip
    InitToolTip();

    // If there is no tooltip defined then add it
    if (m_ToolTip.GetToolCount() == 0)
    {
        GetClientRect(&rcBtn);
        m_ToolTip.AddTool(this, lpszText, rcBtn, 1);
    } // if

    // Set text for tooltip
    m_ToolTip.UpdateTipText(lpszText, this, 1);
    m_ToolTip.Activate(bActivate);
} // End of SetTooltipText

// This function enables or disables the button tooltip.
//
// Parameters:
//      [IN]    bActivate
//              If TRUE the tooltip will be activated.
//
void CButtonST::ActivateTooltip(BOOL bActivate)
{
    // If there is no tooltip then do nothing
    ASSERT_ZERO(m_ToolTip.GetToolCount());

    // Activate tooltip
    m_ToolTip.Activate(bActivate);
} // End of EnableTooltip

// This function returns if the button is the default button.
//
// Return value:
//      TRUE
//          The button is the default button.
//      FALSE
//          The button is not the default button.
//
BOOL CButtonST::GetDefault()
{
    return m_bIsDefault;
} // End of GetDefault

// This function enables the transparent mode.
// Note: this operation is not reversible.
// DrawTransparent should be called just after the button is created.
// Do not use trasparent buttons until you really need it (you have a bitmapped
// background) since each transparent button makes a copy in memory of its background.
// This may bring unnecessary memory use and execution overload.
//
// Parameters:
//      [IN]    bRepaint
//              If TRUE the control will be repainted.
//
void CButtonST::DrawTransparent(BOOL bRepaint)
{
    m_bDrawTransparent = TRUE;

    // Restore old bitmap (if any)
    if (m_dcBk.m_hDC != NULL && m_pbmpOldBk != NULL)
    {
        m_dcBk.SelectObject(m_pbmpOldBk);
    } // if

    m_bmpBk.DeleteObject();
    m_dcBk.DeleteDC();

    // Repaint the button
    REPAINT_STBTN(bRepaint);
} // End of DrawTransparent

// This function sets the URL that will be opened when the button is clicked.
//
// Parameters:
//      [IN]    lpszURL
//              Pointer to a null-terminated string that contains the URL.
//              Pass NULL to removed any previously specified URL.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//
DWORD CButtonST::SetURL(LPCTSTR lpszURL)
{
    // Remove any existing URL
    memset(m_szURL, 0, sizeof(m_szURL));

    ASSERT_NULL_R(lpszURL, BTNST_OK)

    // Store the URL
    _tcsncpy_s(m_szURL, lpszURL, _MAX_PATH);

    return BTNST_OK;
} // End of SetURL


// This function associates a menu to the button.
// The menu will be displayed clicking the button.
// The menu will be handled by the BCMenu class.
//
// Parameters:
//      [IN]    nMenu
//              ID number of the menu resource.
//              Pass NULL to remove any menu from the button.
//      [IN]    hParentWnd
//              Handle to the window that owns the menu.
//              This window receives all messages from the menu.
//      [IN]    bWinXPStyle
//              If TRUE the menu will be displayed using the new Windows XP style.
//              If FALSE the menu will be displayed using the standard style.
//      [IN]    nToolbarID
//              Resource ID of the toolbar to be associated to the menu.
//      [IN]    sizeToolbarIcon
//              A CSize object indicating the size (in pixels) of each icon into the toolbar.
//              All icons into the toolbar must have the same size.
//      [IN]    crToolbarBk
//              A COLORREF value indicating the color to use as background for the icons into the toolbar.
//              This color will be used as the "transparent" color.
//      [IN]    bRepaint
//              If TRUE the control will be repainted.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//      BTNST_INVALIDRESOURCE
//          Failed loading the specified resource.
//
DWORD CButtonST::SetMenu(UINT nMenu, HWND hParentWnd, BOOL bWinXPStyle, UINT nToolbarID,
                         CSize sizeToolbarIcon, COLORREF crToolbarBk, BOOL bRepaint)
{
    BOOL    bRetValue = FALSE;
/*
    // Destroy any previous menu
    if (m_menuPopup.m_hMenu)
    {
        m_menuPopup.DestroyMenu();
        m_hParentWndMenu = NULL;
        m_bMenuDisplayed = FALSE;
    } // if

    // Load menu
    if (nMenu)
    {
        m_menuPopup.SetMenuDrawMode(bWinXPStyle);
        // Load menu
        bRetValue = m_menuPopup.LoadMenu(nMenu);
        // If something wrong
        COMP_BFALSE_R(bRetValue, BTNST_INVALIDRESOURCE);

        // Load toolbar
        if (nToolbarID)
        {
            m_menuPopup.SetBitmapBackground(crToolbarBk);
            m_menuPopup.SetIconSize(sizeToolbarIcon.cx, sizeToolbarIcon.cy);

            bRetValue = m_menuPopup.LoadToolbar(nToolbarID);
            // If something wrong
            if (bRetValue == FALSE)
            {
                m_menuPopup.DestroyMenu();
                return BTNST_INVALIDRESOURCE;
            } // if
        } // if

        m_hParentWndMenu = hParentWnd;
    } // if
    */
    // Repaint the button
    REPAINT_STBTN(bRepaint);

    return BTNST_OK;
} // End of SetMenu

// This function is called every time the button background needs to be painted.
// If the button is in transparent mode this function will NOT be called.
// This is a virtual function that can be rewritten in CButtonST-derived classes
// to produce a whole range of buttons not available by default.
//
// Parameters:
//      [IN]    pDC
//              Pointer to a CDC object that indicates the device context.
//      [IN]    pRect
//              Pointer to a CRect object that indicates the bounds of the
//              area to be painted.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//
DWORD CButtonST::OnDrawBackground(CDC *pDC, LPCRECT pRect)
{
    COLORREF    crColor;
    CBrush      brBackground;

    ASSERT_NULL_R(pDC, BTNST_BADPARAM);
    ASSERT_NULL_R(pRect, BTNST_BADPARAM);

    crColor = m_crColors[BTNST_COLOR_BK_IN];
    if (!(m_bMouseOnButton || m_bIsPressed))
    {
        crColor = m_crColors[BTNST_COLOR_BK_OUT];
        if (m_bIsFocused)
            crColor = m_crColors[BTNST_COLOR_BK_FOCUS];
    } // else

    brBackground.CreateSolidBrush(crColor);

    pDC->FillRect(pRect, &brBackground);

    return BTNST_OK;
} // End of OnDrawBackground

// This function is called every time the button border needs to be painted.
// If the button is in standard (not flat) mode this function will NOT be called.
// This is a virtual function that can be rewritten in CButtonST-derived classes
// to produce a whole range of buttons not available by default.
//
// Parameters:
//      [IN]    pDC
//              Pointer to a CDC object that indicates the device context.
//      [IN]    pRect
//              Pointer to a CRect object that indicates the bounds of the
//              area to be painted.
//
// Return value:
//      BTNST_OK
//          Function executed successfully.
//
DWORD CButtonST::OnDrawBorder(CDC *pDC, LPCRECT prcRect)
{
    ASSERT_NULL_R(pDC, BTNST_BADPARAM);
    ASSERT_NULL_R(prcRect, BTNST_BADPARAM);

    if (m_bIsPressed)
        pDC->Draw3dRect(prcRect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
    else
        pDC->Draw3dRect(prcRect, ::GetSysColor(COLOR_BTNHILIGHT), ::GetSysColor(COLOR_BTNSHADOW));

    return BTNST_OK;
} // End of OnDrawBorder

void CButtonST::SetIconAndText(int iIconIn, LPCTSTR lpszText)
{
    SetIcon(iIconIn, (int)BTNST_AUTO_GRAY);
    OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
    SetTooltipText(lpszText);
}

void CButtonST::SetIconAndText(int iIconIn, WORD wTextID)
{
    CString  strText;
    
    strText.Format(wTextID);
    SetIconAndText(iIconIn, strText);
}
//#undef BS_TYPEMASK

