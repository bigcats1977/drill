// GridCtrl.cpp : implementation file
//
// MFC Grid Control
//
// Written by Chris Maunder
//        mailto:chrismaunder@codeguru.com
//
// Copyright (c) 1998-1999.
//
// The code contained in this file is based on the original
// WorldCom Grid control written by Joe Willcoxson,
//        mailto:chinajoe@aol.com
//        http://users.aol.com/chinajoe
// The code has gone through so many modifications that I'm
// not sure if there is even a single original line of code.
// In any case Joe's code was a great framewaork on which to
// build.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name and all copyright
// notices remains intact. If the source code in this file is used in
// any  commercial application then a statement along the lines of
// "Portions copyright (c) Chris Maunder, 1998" must be included in
// the startup banner, "About" box or printed documentation. An email
// letting me know that you are using it would be nice as well. That's
// not much to ask considering the amount of work that went into this.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// Expect bugs!
//
// Please use and enjoy, and let me know of any bugs/mods/improvements
// that you have found/implemented and I will fix/incorporate them into
// this file.
//
//  History:
//  --------
//  This control is constantly evolving, sometimes due to new features that I
//  feel are necessary, and sometimes due to existing bugs. Where possible I
//  have credited the changes to those who contributed code corrections or
//  enhancements (names in brackets) or code suggestions (suggested by...)
//
//          1.0     20 Feb 1998   First release version.
//          1.01    24 Feb 1998   Memory leak fix (Jens Bohlmann)
//                                Fixec typo (my fault!) in CMemDC.h - Claus Arend-Schneider)
//                                Bug in GetSelectedCount (Lyn Newton)
//          1.02    4  Mar 1998   Scrolling a little neater (less dead area)
//                                Cell selection via OnTimer correctly updates Focus cell (Suggested by Lyn Newton)
//          1.03    17 Mar 1998   Clipboard functions added, Intellimouse support
//                                Using 32 bit scroll pos functions instead of 16 bit ("cronos")
//                                Added OLE drag and drop.
//          1.04     6 Apr 1998   Added Ctrl-A = Select All, fixed CGridDropTarget
//                                problem, minor bug in CopyTextFromGrid (assert on
//                                empty string). Cleaned up reponse to m_bEditable
//                                (OnDrop and Ctrl-X disabled)
//          1.05    10 May 1998   Memory leak fixed. (Yuheng Zhao)
//                                Changed OLE initialisation (Carlo Comino)
//                                Added separate fore + background cell colours (Suggested by John Crane)
//                                ExpandToFit etc cleaned up - now decreases and
//                                increases cell sizes to fit client area.
//                                Added notification messages for the grid's parent (Suggested by
//                                Added GVIS_READONLY state
//          1.06    20 May 1998   Added TAB key handling. (Daniela Rybarova)
//                                Intellimouse code correction for whole page scrolling (Paul Grant)
//                                Fixed 16 bit thumb track problems (now 32 bit) (Paul Grant)
//                                Fixed accelerator key problem in CInPlaceEdit (Matt Weagle)
//                                Fixed Stupid ClassWizard code parsing problem (Michael A. Barnhart)
//                                Double buffering now programmatically selectable
//                                Workaround for win95 drag and drop registration problem
//                                Corrected UNICODE implementation of clipboard stuff
//                                Dragging and dropping from a selection onto itself no
//                                no longer causes the cells to be emptied
//          1.07    28 Jul 1998   Added EnsureVisible. (Roelf Werkman)
//                                Fixed delete key problem on read-only cells. (Serge Weinstock)
//                                OnEndInPlaceEdit sends notification AFTER storing
//                                the modified text in the cell.
//                                Added CreateInPlaceEditControl to make it easier to
//                                change the way cells are edited. (suggested by Chris Clark)
//                                Added Set/GetGridColor.
//                                CopyTextToClipboard and PasteTextToGrid problem with
//                                blank cells fixed, and CopyTextToClipboard tweaked.
//                                SetModified called when cutting text or hitting DEL. (Jonathan Watters)
//                                Focus cell made visible when editing begins.
//                                Blank lines now treated correctly when pasting data.
//                                Removed ES_MULTILINE style from the default edit control.
//                                Added virtual CreateCell(row, col) function.
//                                Fonts now specified on a per-cell basis using Get/SetItemFont.
//          1.08     6 Aug 1998   Ctrl+arrows now allows cell navigation. Modified
//                                CreateInPlaceEditControl to accept ID of control.
//                                Added Titletips to grid cells. (Added EnableTitleTips / GetTitleTips)
//          1.09    12 Sep 1998   When printing, parent window title is printed in header - Gert Rijs
//                                GetNextItem search with GVNI_DROPHILITED now returns
//                                cells with GVIS_DROPHILITED set, instead of GVIS_FOCUSED (Franco Bez)
//                                (Also fixed minor bug in GetNextItem) (Franco Bez)
//                                Cell selection using Shift+arrows works - Franco Bez
//                                SetModified called after edits ONLY if contents changed (Franco Bez)
//                                Cell colours now dithered in 256 colour screens.
//                                Support for MSVC 4.2 (Graham Cheetham)
//          1.10    30 Nov 1998   Titletips now disappear on a scroll event. Compiler errors
//                                fixed. Grid lines drawing fixed (Graham Cheetham).
//                                Cell focus fix on Isert Row/Col (Jochen Kauffmann)
//                                Added DeleteNonFixedRows() (John Rackley)
//                                Message #define conflict fixed (Oskar Wieland)
//                                Titletips & cell insert/delete fix (Ramesh Dhar)
//                                Titletips repeat-creation bug fixed.
//                                GVN_SELCHANGED message now sends current cell ID
//                                Font resource leak in GetTextExtent fixed (Gavin Jerman)
//                                More TAB fixes (Andreas Ruh)
//          1.11    1 Dec 1998    GetNextItem bug fix (suggested by Francis Fu)
//                                InsertColumn (-1) fix (Roy Hopkins)
//                                Was too liberal with the "IsEditable"'s. oops. (Michel Hete)
//          1.11a   4 Jan 1999    Compiler errors in VC6 fixed.
//          1.12    10 Apr 1999   Cleanup to allow GRIDCONTROL_NO_CLIPBOARD define
//                                CE #defines added. (Thanks to Frank Uzzolino for a start on this)
//                                TitleTip display fixed for cells with images, plus it now uses cell font
//                                Added GetTextRect and IsCellFixed
//                                Focus change problem when resizing columns fixed (Sergey Nikiforenko)
//                                Grid line drawing problem in fixed cells fixed (Sergey Nikiforenko)
//                                CreateCell format persistance bug fixed (Sergey Nikiforenko)
//                                DeleteColumn now returns TRUE (oops) (R. Elmer)
//                                Enter, Tab and Esc key problem (finally) fixed - Darren Webb and Koay Kah Hoe
//                                OnSize infinite loop fixed - Steve Kowald
//                                GVN_SELCHANGING and GVN_SELCHANGED values changed to avoid conflicts (Hiroaki Watanabe)
//                                Added single row selection mode (Yao Cai)
//                                Fixed image drawing clip problem
//                                Reduced unnecessary redraws significantly
//                                GetNextItem additions and bug fix, and GVNI_AREA search option (Franco Bez)
//                                Added GVIS_MODIFIED style for cells, so individual cells can have their
//                                modification status queried. (Franco Bez)
//          1.12a   15 Apr 1999   Removed the SetModified/GetModified inlines (no compiler warning!)
//                                Renamed IDC_INPLACE_CONTROL to IDC_INPLACE_CONTROL and moved
//                                to the header
//
//    - OnOutOfMemory function instead of exceptions
//    - Decrease timer interval over time to speed up selection over time
//
// NOTE: Grid data is stored row-by-row, so all operations on large numbers
//       of cells should be done row-by-row as well.
//
// KNOWN ISSUES TO BE ADDRESSED (Please don't send bug reports):
// * Killfocus comes to late when a command is selected by the Menu.
//   When you are editing a cell and choose a Menuitem that searches for all the
//   modified cells it is not found. When you chose the menu a second time it is
//   found. I assume that the Menu command is executed before the cell receives the
//   KillFocus event. Expect similar Problems with accelerators. (Franco Bez)
// * When you select a cell and move the mouse around (with the Left button down
//   i.e continuing with your selection) - if the mouse is over the Fixed column
//   or Row the drawing of the selected region is strange - in particular as you
//   move up and down say the Left Fixed Column notice the behaviour of the Focus
//   Cell - it is out of sync. (Vinay Desai)
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCtrl.h"
#include "InPlaceEdit.h"
#include "math.h"
#include "Torque.h"
// OLE stuff for clipboard operations
#include <afxadv.h>            // For CSharedFile
#include <afxconv.h>           // For LPTSTR -> LPSTR macros

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define HEADER_HEIGHT       2    // For printing
#define FOOTER_HEIGHT       2
#define LEFT_MARGIN         4
#define RIGHT_MARGIN        4
#define TOP_MARGIN          1
#define BOTTOM_MARGIN       1
#define GAP                 1

#define SELECTED_CELL_FONT_WEIGHT 600    // weight of text for selected items

/* Cell落在行或者列的位置 */
#define OVERLAP_CELL(ciCheck, iRow, iCol)                               \
        ((ciCheck.m_iRow == iRow) ||                                    \
         (ciCheck.m_iCol == iCol))

#define SHOWGRIDCTRL (!defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING))

#define CELLHIDE(pgcCell)  (((pgcCell)->m_bHide == TRUE)    ||          \
                            ((pgcCell)->m_iNumHhide != 0)   ||          \
                            ((pgcCell)->m_iNumVhide != 0))

#define GETNEXTTOKEN()                      {                           \
    for (; *pEnd && (*pEnd != _T(',')) && (*pEnd != _T('\n')); pEnd++); \
    {                                                                   \
        if ((*pEnd == _T('\0')) && (pToken == pEnd))                    \
            pToken = NULL;                                              \
    }                                                                   \
    *pEnd = _T('\0');                                                   \
}


/* 定义CGridCtrl的KeyDown处理函数数组 
   返回行是否发生变化 */
typedef void (CGridCtrl::*KeyDownFun)(UINT nRepCnt, UINT nFlags, CCellID *pciNext);
typedef struct tagGCKEYDOWNFUNC
{
    UINT        nChar;
    KeyDownFun  fKeyDownFunc;
}GCKEYDOWNFUNC;
GCKEYDOWNFUNC  g_tGCKeyDownProc[] = 
{
    {VK_DELETE,         &CGridCtrl::KD_Delete   },
    {VK_TAB,            &CGridCtrl::KD_Tab      },
    {VK_DOWN,           &CGridCtrl::KD_Down     },
    {VK_UP,             &CGridCtrl::KD_Up       },
    {VK_RIGHT,          &CGridCtrl::KD_Right    },
    {VK_LEFT,           &CGridCtrl::KD_Left     },
    {VK_NEXT,           &CGridCtrl::KD_Next     },
    {VK_PRIOR,          &CGridCtrl::KD_Prior    },
    {VK_HOME,           &CGridCtrl::KD_Home     },
    {VK_END,            &CGridCtrl::KD_End      }
};

/* 定义CGridCtrl的OnHScroll处理函数数组 */
typedef void (CGridCtrl::*HScrollFun)(int iScrPos, CCellID ciTopLeft, CRect rcTmp);
typedef struct tagGCHSCROLLFUNC
{
    UINT        nSBCode;
    HScrollFun  fHScrollFunc;
}GCHSCROLLFUNC;
GCHSCROLLFUNC  g_tGCHScrollProc[] = 
{
    {SB_LINERIGHT,      &CGridCtrl::HS_LineRight},
    {SB_LINELEFT,       &CGridCtrl::HS_LineLeft },
    {SB_PAGERIGHT,      &CGridCtrl::HS_PageRight},
    {SB_PAGELEFT,       &CGridCtrl::HS_PageLeft },
    {SB_THUMBPOSITION,  &CGridCtrl::HS_Thumb    },
    {SB_THUMBTRACK,     &CGridCtrl::HS_Thumb    },
    {SB_RIGHT,          &CGridCtrl::HS_Right    },
    {SB_LEFT,           &CGridCtrl::HS_Left     }
};

/* 定义CGridCtrl的OnVScroll处理函数数组 */
typedef void (CGridCtrl::*VScrollFun)(int iScrPos, CCellID ciTopLeft, CRect rcTmp);
typedef struct tagGCVSCROLLFUNC
{
    UINT        nSBCode;
    VScrollFun  fVScrollFunc;
}GCVSCROLLFUNC;
GCVSCROLLFUNC  g_tGCVScrollProc[] = 
{
    {SB_LINEDOWN,       &CGridCtrl::VS_LineDown },
    {SB_LINEUP,         &CGridCtrl::VS_LineUp   },
    {SB_PAGEDOWN,       &CGridCtrl::VS_PageDown },
    {SB_PAGEUP,         &CGridCtrl::VS_PageUp   },
    {SB_THUMBPOSITION,  &CGridCtrl::VS_Thumb    },
    {SB_THUMBTRACK,     &CGridCtrl::VS_Thumb    },
    {SB_TOP,            &CGridCtrl::VS_Top      },
    {SB_BOTTOM,         &CGridCtrl::VS_Bottom   }
};

IMPLEMENT_DYNCREATE(CGridCtrl, CWnd)

void AFXAPI DDX_GridControl(CDataExchange* pDX, int iIDC, CGridCtrl& rControl)
{
    HWND hWndCtrl = NULL;

    // subclassed yet
    COMP_BNE(rControl.GetSafeHwnd(), NULL);

    // not subclassed yet
    ASSERT_NULL(pDX);

    ASSERT(!pDX->m_bSaveAndValidate);

    hWndCtrl = pDX->PrepareCtrl(iIDC);

    if (!rControl.SubclassWindow(hWndCtrl))
    {
        ASSERT(FALSE);      // possibly trying to subclass twice?
        AfxThrowNotSupportedException();
    }
#ifndef _AFX_NO_OCC_SUPPORT
    else
    {
        // If the control has reparented itself (e.g., invisible control),
        // make sure that the CWnd gets properly wired to its control site.
        if (pDX->m_pDlgWnd->GetSafeHwnd() != ::GetParent(rControl.GetSafeHwnd()))
            rControl.AttachControlSite(pDX->m_pDlgWnd);
    }
#endif //!_AFX_NO_OCC_SUPPORT

}

// Get the number of lines to scroll with each mouse wheel notch
// Why doesn't windows give us this function???
UINT GetMouseScrollLines()
{
    UINT    nScrollLines    = 3;   // reasonable default
    DWORD   dwKeyDataType   = 0;
    DWORD   dwDataBufSize   = 0;
    TCHAR   szData[128];
    HKEY    hKey;

    if (RegOpenKeyEx(HKEY_CURRENT_USER,  _T("Control Panel\\Desktop"),
                     0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        dwDataBufSize = sizeof(szData);

        if (RegQueryValueEx(hKey, _T("WheelScrollLines"), NULL, &dwKeyDataType,
                           (LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
        {
            nScrollLines = _tcstoul(szData, NULL, 10);
        }
        RegCloseKey(hKey);
    }

    return nScrollLines;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl
void CGridCtrl::InitOLELib(void)
{
    _AFX_THREAD_STATE   *pState = NULL;
    SCODE               sc;

    // Initialize OLE libraries
    m_bMustUninitOLE = FALSE;

#if !defined(GRIDCONTROL_NO_DRAGDROP) || !defined(GRIDCONTROL_NO_CLIPBOARD)
    pState = AfxGetThreadState();
    ASSERT_NULL(pState);
    if (!pState->m_bNeedTerm)
    {
        sc = ::OleInitialize(NULL);
        if (FAILED(sc))
            AfxMessageBox(IDS_STRINFINITOLEFAIL);
        else
            m_bMustUninitOLE = TRUE;
    }
#endif
}

CGridCtrl::CGridCtrl(int iRows, int iCols, int iFixedRows, int iFixedCols)
{
#ifndef _WIN32_WCE
    NONCLIENTMETRICS ncm;
#else
    LOGFONT          lf;
#endif

    RegisterWindowClass();

    InitOLELib();

    // Store the system colours in case they change. The gridctrl uses
    // these colours, and in OnSysColorChange we can check to see if
    // the gridctrl colours have been changed from the system colours.
    // If they have, then leave them, otherwise change them to reflect
    // the new system colours.
    m_clrWindowText         = ::GetSysColor(COLOR_WINDOWTEXT);
    m_clrWindow             = ::GetSysColor(COLOR_WINDOW);
    m_clr3DFace             = ::GetSysColor(COLOR_3DFACE);
    m_clrShadow             = ::GetSysColor(COLOR_3DSHADOW);
    m_clrGrid               = RGB(192,192,192);

    m_iRows                 = 0;
    m_iCols                 = 0;
    m_iFixedRows            = 0;
    m_iFixedCols            = 0;

    m_iDefCellHeight        = 10;        // These will get changed to something meaningful
    m_iDefCellWidth         = 30;        //    when the window is created or subclassed

    m_iVScrollMax           = 0;         // Scroll position
    m_iHScrollMax           = 0;
    m_iMargin               = 0;         // cell padding
    m_iRowsPerWheelNotch    = GetMouseScrollLines(); // Get the number of lines
                                                  // per mouse wheel notch to scroll

    m_iMouseMode            = MOUSE_NOTHING;
    m_iGridLines            = GVL_NONE;
    m_bEditable             = TRUE;
    m_bListMode             = FALSE;
    m_bSingleRowSelection   = FALSE;
    m_bAllowDraw            = TRUE;      // allow draw updates
    m_bEnableSelection      = TRUE;
    m_bAllowRowResize       = TRUE;
    m_bAllowColumnResize    = TRUE;
    m_bSortOnClick          = TRUE;      // Sort on header row click if in list mode
    m_bHandleTabKey         = TRUE;
    m_bDoubleBuffer         = FALSE;      // Use double buffering to avoid flicker?
    m_bTitleTips            = TRUE;      // show cell title tips

    m_bAscending            = TRUE;      // sorting stuff
    m_iSortCol              = -1;

    m_iTimerID              = 0;         // For drag-selection
    m_iTimerInterval        = 25;        // (in milliseconds)
    m_iResizeCaptureRange   = 3;        // When resizing columns/row, the cursor has to be
                                      // within +/-3 pixels of the dividing line for
                                      // resizing to be possible
    m_pImageList            = NULL;
    m_bAllowDragAndDrop     = FALSE;     // for drag and drop

#ifndef _WIN32_WCE
    // Initially use the system message font for the GridCtrl font
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));
    memcpy(&m_Logfont, &(ncm.lfMessageFont), sizeof(LOGFONT));
#else
    GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);
    memcpy(&m_Logfont, &lf, sizeof(LOGFONT));
#endif

    // Set up the initial grid size
    SetRowCount(iRows);
    SetColumnCount(iCols);
    SetFixedRowCount(iFixedRows);
    SetFixedColumnCount(iFixedCols);

    // Set the colours
    SetTextColor(m_clrWindowText);
    SetTextBkColor(m_clrWindow);
    SetBkColor(m_clrShadow);
    SetFixedTextColor(m_clrWindowText);
    SetFixedBkColor(m_clr3DFace);

    // set initial selection range (ie. none)
    m_tSelectedCellMap.RemoveAll();
    m_tPrevSelectedCellMap.RemoveAll();
}

CGridCtrl::~CGridCtrl()
{
    DeleteAllItems();

    DestroyWindow();

    m_ftGrid.DeleteObject();

#if !defined(GRIDCONTROL_NO_DRAGDROP) || !defined(GRIDCONTROL_NO_CLIPBOARD)
    // Uninitialize OLE support
    if (m_bMustUninitOLE)
        ::OleUninitialize();
#endif
}

// Register the window class if it has not already been registered.
BOOL CGridCtrl::RegisterWindowClass()
{
    WNDCLASS  wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, GRIDCTRL_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = 0;
        wndcls.cbWndExtra       = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = GRIDCTRL_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CGridCtrl::Create(const RECT& rc, CWnd* pParentWnd, UINT nID, DWORD dwStyle)
{
    int     i = 0;

    ASSERT_NULL_R(pParentWnd, FALSE);
    ASSERT(pParentWnd->GetSafeHwnd());

    COMP_BFALSE_R(CWnd::Create(GRIDCTRL_CLASSNAME, NULL, dwStyle, rc, pParentWnd, nID),
                  FALSE);

#ifndef GRIDCONTROL_NO_DRAGDROP
    m_tDropTarget.Register(this);
#endif

    // Create titletips
#ifndef GRIDCONTROL_NO_TITLETIPS
    if (m_bTitleTips)
        m_tTitleTip.Create(this);
#endif

    // The number of rows and columns will only be non-zero if the constructor
    // was called with non-zero initialising parameters. If this window was created
    // using a dialog template then the number of rows and columns will be 0 (which
    // means that the code below will not be needed - which is lucky 'cause it ain't
    // gonna get called in a dialog-template-type-situation.
    TRY
    {
        m_arRowHeights.SetSize(m_iRows);    // initialize row heights
        m_arColWidths.SetSize(m_iCols);     // initialize column widths
    }
    CATCH (CMemoryException, e)
    {
        e->ReportError();
        e->Delete();
        return FALSE;
    }
    END_CATCH

    for (i = 0; i < m_iRows; i++)
        m_arRowHeights[i] = m_iDefCellHeight;
    for (i = 0; i < m_iCols; i++)
        m_arColWidths[i]  = m_iDefCellWidth;

    ResetScrollBars();
    return TRUE;
}

void CGridCtrl::PreSubclassWindow()
{
    HFONT hFont = NULL;

    CWnd::PreSubclassWindow();

    hFont = ::CreateFontIndirect(&m_Logfont);
    OnSetFont((LPARAM)hFont, 0);
    DeleteObject(hFont);

    ResetScrollBars();
}

BOOL CGridCtrl::SubclassWindow(HWND hWnd)
{
    COMP_BFALSE_R(CWnd::SubclassWindow(hWnd), FALSE);

#ifndef GRIDCONTROL_NO_DRAGDROP
    m_tDropTarget.Register(this);
#endif

#ifndef GRIDCONTROL_NO_TITLETIPS
    if (m_bTitleTips && !IsWindow(m_tTitleTip.m_hWnd))
        m_tTitleTip.Create(this);
#endif

    return TRUE;
}

LRESULT CGridCtrl::SendMessageToParent(int iRow, int iCol, int iMessage)
{
    CWnd        *pOwner  = NULL;
    NM_GRIDVIEW nmgv;

    COMP_BFALSE_R(IsWindow(m_hWnd), 0);

    nmgv.iRow         = iRow;
    nmgv.iColumn      = iCol;
    nmgv.hdr.hwndFrom = m_hWnd;
    nmgv.hdr.idFrom   = GetDlgCtrlID();
    nmgv.hdr.code     = iMessage;

    pOwner = GetOwner();
    ASSERT_NULL_R(pOwner, 0);
    if (IsWindow(pOwner->m_hWnd))
        return pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);

    return 0;
}

BEGIN_MESSAGE_MAP(CGridCtrl, CWnd)
    //{{AFX_MSG_MAP(CGridCtrl)
    ON_WM_PAINT()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_SIZE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
    ON_WM_GETDLGCODE()
    ON_WM_KEYDOWN()
    ON_WM_CHAR()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_ERASEBKGND()
    ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
    ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
    //}}AFX_MSG_MAP
#ifndef _WIN32_WCE_NO_CURSOR
    ON_WM_SETCURSOR()
#endif
#ifndef _WIN32_WCE
    ON_WM_SYSCOLORCHANGE()
    ON_WM_CAPTURECHANGED()
#endif
#ifndef GRIDCONTROL_NO_CLIPBOARD
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
    ON_COMMAND(ID_EDIT_CUT, OnEditCut)
    ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
    ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
#endif
#if !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)
    ON_WM_MOUSEWHEEL()
#endif
#if (_WIN32_WCE >= 210)
    ON_WM_SETTINGCHANGE()
#endif
    ON_MESSAGE(WM_SETFONT, OnSetFont)
    ON_MESSAGE(WM_GETFONT, OnGetFont)
    ON_NOTIFY(GVN_ENDLABELEDIT, IDC_INPLACE_CONTROL, OnEndInPlaceEdit)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGridCtrl message handlers

void CGridCtrl::OnPaint()
{
    CPaintDC dc(this);      // device context for painting

    OnDraw(&dc);
}

BOOL CGridCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;    // Don't erase the background.
}

// Custom background erasure. This gets called from within the OnDraw function,
// since we will (most likely) be using a memory DC to stop flicker. If we just
// erase the background normally through OnEraseBkgnd, and didn't fill the memDC's
// selected bitmap with colour, then all sorts of vis problems would occur
void CGridCtrl::EraseBkgnd(CDC* pDC)
{
    int     iFixedColWidth  = 0;
    int     iFixedRowHeight = 0;
    CRect   rcVis;        // visible
    CRect   rcClip;
    CRect   rcTmp;
    CRect   rcCell;
    CBrush  brFixedBk;
    CBrush  brTextBk;
    CBrush  brBk;

    ASSERT_NULL(pDC);

    COMP_BE(pDC->GetClipBox(rcClip), ERROR);

    GetVisibleNonFixedCellRange(rcVis);

    brFixedBk.CreateSolidBrush(GetFixedBkColor());
    brTextBk.CreateSolidBrush(GetTextBkColor());
    brBk.CreateSolidBrush(GetBkColor());

    // Draw Fixed columns background
    iFixedColWidth = GetFixedColumnWidth();
    if (rcClip.left < iFixedColWidth &&
        rcClip.top  < rcVis.bottom)
    {
        pDC->FillRect(CRect(rcClip.left, rcClip.top,
                            iFixedColWidth, rcVis.bottom),
                      &brFixedBk);
    }

    // Draw Fixed rows background
    iFixedRowHeight = GetFixedRowHeight();
    if (rcClip.top   < iFixedRowHeight &&
        rcClip.right > iFixedColWidth  &&
        rcClip.left  < rcVis.right)
    {
        pDC->FillRect(CRect(iFixedColWidth-1, rcClip.top,
                            rcVis.right, iFixedRowHeight),
                      &brFixedBk);
    }

    // Draw non-fixed cell background
    if (rcTmp.IntersectRect(rcVis, rcClip))
    {
        rcCell.SetRect(max(iFixedColWidth,  rcTmp.left),
                       max(iFixedRowHeight, rcTmp.top),
                       rcTmp.right,
                       rcTmp.bottom);
        pDC->FillRect(rcCell, &brTextBk);
    }

    // Draw right hand side of window outside grid
    if (rcVis.right < rcClip.right)
    {
        pDC->FillRect(CRect(rcVis.right, rcClip.top,
                            rcClip.right, rcClip.bottom),
                      &brBk);
    }

    // Draw bottom of window below grid
    if (rcVis.bottom < rcClip.bottom &&
        rcClip.left  < rcVis.right)
    {
        pDC->FillRect(CRect(rcClip.left, rcVis.bottom,
                            rcVis.right, rcClip.bottom),
                      &brBk);
    }
}

void CGridCtrl::OnSize(UINT nType, int cx, int cy)
{
    static BOOL bAlreadyInsideThisProcedure = FALSE;

    COMP_BTRUE(bAlreadyInsideThisProcedure);

    COMP_BFALSE(::IsWindow(m_hWnd));

    // Start re-entry blocking
    bAlreadyInsideThisProcedure = TRUE;

    // Auto-destroy any InPlaceEdit's
    SetFocus();

    CWnd::OnSize(nType, cx, cy);
    ResetScrollBars();

    // End re-entry blocking
    bAlreadyInsideThisProcedure = FALSE;
}

UINT CGridCtrl::OnGetDlgCode()
{
    UINT nCode = DLGC_WANTARROWS | DLGC_WANTCHARS; // DLGC_WANTALLKEYS; //

    if (m_bHandleTabKey && !IsCTRLpressed())
        nCode |= DLGC_WANTTAB;

    return nCode;
}

void CGridCtrl::UpdateSysColor()
{
    if (GetTextColor() == m_clrWindowText)                  // Still using system colours
        SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetTextBkColor() == m_clrWindow)
        SetTextBkColor(::GetSysColor(COLOR_WINDOW));
    if (GetBkColor() == m_clrShadow)
        SetBkColor(::GetSysColor(COLOR_3DSHADOW));
    if (GetFixedTextColor() == m_clrWindowText)
        SetFixedTextColor(::GetSysColor(COLOR_WINDOWTEXT));
    if (GetFixedBkColor() == m_clr3DFace)
        SetFixedBkColor(::GetSysColor(COLOR_3DFACE));

    m_clrWindowText = ::GetSysColor(COLOR_WINDOWTEXT);
    m_clrWindow     = ::GetSysColor(COLOR_WINDOW);
    m_clr3DFace     = ::GetSysColor(COLOR_3DFACE);
    m_clrShadow     = ::GetSysColor(COLOR_3DSHADOW);
}

#ifndef _WIN32_WCE
// If system colours change, then redo colours
void CGridCtrl::OnSysColorChange()
{
    CWnd::OnSysColorChange();

    UpdateSysColor();    
}
#endif

#ifndef _WIN32_WCE_NO_CURSOR
// If we are drag-selecting cells, or drag and dropping, stop now
void CGridCtrl::OnCaptureChanged(CWnd *pWnd)
{
    ASSERT_NULL(pWnd);
    COMP_BE(pWnd->GetSafeHwnd(), GetSafeHwnd());

    // kill timer if active
    if (m_iTimerID != 0)
    {
        (m_iTimerID);
        m_iTimerID = 0;
    }

#ifndef GRIDCONTROL_NO_DRAGDROP
    // Kill drag and drop if active
    if (m_iMouseMode == MOUSE_DRAGGING)
        m_iMouseMode = MOUSE_NOTHING;
#endif
}
#endif

#if (_MFC_VER >= 0x0421) || (_WIN32_WCE >= 210)
// If system settings change, then redo colours
void CGridCtrl::OnSettingChange(UINT nFlags, LPCTSTR lpszSection)
{
    CWnd::OnSettingChange(nFlags, lpszSection);

    UpdateSysColor();

    m_iRowsPerWheelNotch = GetMouseScrollLines(); // Get the number of lines
}
#endif

void CGridCtrl::TimerSelectXCell(CPoint ptOrig)
{
    CRect   rcTmp;
    int     iFixedColWidth = 0;
    
    GetClientRect(&rcTmp);

    if (ptOrig.x > rcTmp.right)
    {
        // SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
        SendMessage(WM_KEYDOWN, VK_RIGHT, 0);

        ptOrig.y = max(ptOrig.y, rcTmp.top);
        ptOrig.y = min(ptOrig.y, rcTmp.bottom);
        ptOrig.x = rcTmp.right;
        OnSelecting(GetCellFromPt(ptOrig));
    }
    else if (ptOrig.x < iFixedColWidth)
    {
        //SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
        SendMessage(WM_KEYDOWN, VK_LEFT, 0);

        ptOrig.y = max(ptOrig.y, rcTmp.top);
        ptOrig.y = min(ptOrig.y, rcTmp.bottom);
        ptOrig.x = iFixedColWidth + 1;
        OnSelecting(GetCellFromPt(ptOrig));
    }
}

void CGridCtrl::TimerSelectYCell(CPoint ptOrig)
{
    CRect   rcTmp;
    int     iFixedRowHeight = 0;
    
    GetClientRect(&rcTmp);

    iFixedRowHeight = GetFixedRowHeight();

    if (ptOrig.y > rcTmp.bottom)
    {
        //SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
        SendMessage(WM_KEYDOWN, VK_DOWN, 0);

        ptOrig.x = max(ptOrig.x, rcTmp.left);
        ptOrig.x = min(ptOrig.x, rcTmp.right);
        ptOrig.y = rcTmp.bottom;
        OnSelecting(GetCellFromPt(ptOrig));
    }
    else if (ptOrig.y < iFixedRowHeight)
    {
        //SendMessage(WM_VSCROLL, SB_LINEUP, 0);
        SendMessage(WM_KEYDOWN, VK_UP, 0);

        ptOrig.x = max(ptOrig.x, rcTmp.left);
        ptOrig.x = min(ptOrig.x, rcTmp.right);
        ptOrig.y = iFixedRowHeight + 1;
        OnSelecting(GetCellFromPt(ptOrig));
    }

    return;
}

// For drag-selection. Scrolls hidden cells into view
// TODO: decrease timer interval over time to speed up selection over time
void CGridCtrl::OnTimer(UINT_PTR nIDEvent)
{
    CPoint  ptOrig;

    ASSERT(nIDEvent == WM_LBUTTONDOWN);
    COMP_BNE(nIDEvent, WM_LBUTTONDOWN);

#ifdef _WIN32_WCE
    COMP_BE(m_iMouseMode, MOUSE_NOTHING);
    ptOrig = GetMessagePos();
#else
    COMP_BFALSE(GetCursorPos(&ptOrig));
#endif

    ScreenToClient(&ptOrig);

    TimerSelectYCell(ptOrig);

    TimerSelectXCell(ptOrig);
}

/* CTRL + Key */
void CGridCtrl::CTRLKeyDown(UINT nChar)
{
    COMP_BFALSE(IsCTRLpressed());

    switch (nChar)
    {
        case 'A':
            OnEditSelectAll();
            break;
#ifndef GRIDCONTROL_NO_CLIPBOARD
        case 'X':
            OnEditCut();
            break;
        case 'C':
            OnEditCopy();
            break;
        case 'V':
            OnEditPaste();
            break;
#endif
    }
}

void CGridCtrl::KD_Delete(UINT nRepCnt, UINT nFlags, CCellID *pciNext)
{
    COMP_BFALSE(IsCellEditable(m_ciCurrent.m_iRow, m_ciCurrent.m_iCol));

    SendMessageToParent(m_ciCurrent.m_iRow, m_ciCurrent.m_iCol, GVN_BEGINLABELEDIT);
    SetItemText(m_ciCurrent.m_iRow, m_ciCurrent.m_iCol, _T(""));
    SetModified(TRUE, m_ciCurrent.m_iRow, m_ciCurrent.m_iCol);
    SendMessageToParent(m_ciCurrent.m_iRow, m_ciCurrent.m_iCol, GVN_ENDLABELEDIT);
    RedrawCell(m_ciCurrent);

    return;
}

void CGridCtrl::KD_Tab(UINT nRepCnt, UINT nFlags, CCellID *pciNext)
{
    ASSERT_NULL(pciNext);

    if (IsSHIFTpressed())
    {
        if (pciNext->m_iCol > m_iFixedCols)
        {
            pciNext->m_iCol--;
            return;
        }
        if (pciNext->m_iCol == m_iFixedCols &&
            pciNext->m_iRow >  m_iFixedRows)
        {
            pciNext->m_iRow--;
            pciNext->m_iCol = GetColumnCount() - 1;
            return;
        }

        CWnd::OnKeyDown(VK_TAB, nRepCnt, nFlags);
        return;
    }

    /* SHIFT isn't pressed */
    if (pciNext->m_iCol < (GetColumnCount() - 1))
    {
        pciNext->m_iCol++;
        return;
    }
    if (pciNext->m_iCol == (GetColumnCount() - 1) &&
        pciNext->m_iRow <  (GetRowCount()    - 1) )
    {
        pciNext->m_iRow++;
        pciNext->m_iCol = m_iFixedCols;
        return;
    }

    CWnd::OnKeyDown(VK_TAB, nRepCnt, nFlags);
    return;
}

void CGridCtrl::KD_Down(UINT nRepCnt, UINT nFlags, CCellID *pciNext)
{
    ASSERT_NULL(pciNext);

    if (pciNext->m_iRow < (GetRowCount() - 1))
    {
        pciNext->m_iRow++;
    }

    return ;
}

void CGridCtrl::KD_Up(UINT nRepCnt, UINT nFlags, CCellID *pciNext)
{
    ASSERT_NULL(pciNext);

    if (pciNext->m_iRow > m_iFixedRows)
    {
        pciNext->m_iRow--;
    }

    return;
}

void CGridCtrl::KD_Right(UINT nRepCnt, UINT nFlags, CCellID *pciNext)
{
    ASSERT_NULL(pciNext);

    if (pciNext->m_iCol < (GetColumnCount() - 1))
    {
        pciNext->m_iCol++;
    }

    return;
}

void CGridCtrl::KD_Left(UINT nRepCnt, UINT nFlags, CCellID *pciNext)
{
    ASSERT_NULL(pciNext);

    if (pciNext->m_iCol > m_iFixedCols)
    {
        pciNext->m_iCol--;
    }

    return;
}

void CGridCtrl::KD_Next(UINT nRepCnt, UINT nFlags, CCellID *pciNext)
{
    int     iIncrement  = 0;
    CCellID ciOldTopLeft;
    CCellID ciNewTopLeft;

    ASSERT_NULL(pciNext);

    ciOldTopLeft = GetTopleftNonFixedCell();
    SendMessage(WM_VSCROLL, SB_PAGEUP, 0);
    ciNewTopLeft = GetTopleftNonFixedCell();

    iIncrement = ciNewTopLeft.m_iRow - ciOldTopLeft.m_iRow;
    if (iIncrement)
    {
        pciNext->m_iRow += iIncrement;
        if (pciNext->m_iRow < m_iFixedRows)
            pciNext->m_iRow = m_iFixedRows;
        return;
    }

    /* iIncrement is 0 */
    pciNext->m_iRow = m_iFixedRows;

    return;
}

void CGridCtrl::KD_Prior(UINT nRepCnt, UINT nFlags, CCellID *pciNext)
{
    int     iIncrement  = 0;
    CCellID ciOldTopLeft;
    CCellID ciNewTopLeft;

    ASSERT_NULL(pciNext);

    ciOldTopLeft = GetTopleftNonFixedCell();
    SendMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
    ciNewTopLeft = GetTopleftNonFixedCell();

    iIncrement = ciNewTopLeft.m_iRow - ciOldTopLeft.m_iRow;
    if (iIncrement)
    {
        pciNext->m_iRow += iIncrement;
        if (pciNext->m_iRow > (GetRowCount() - 1))
            pciNext->m_iRow = GetRowCount() - 1;
        return;
    }

    /* iIncrement is 0 */
    pciNext->m_iRow = GetRowCount() - 1;

    return;
}

void CGridCtrl::KD_Home(UINT nRepCnt, UINT nFlags, CCellID *pciNext)
{
    ASSERT_NULL(pciNext);

    SendMessage(WM_VSCROLL, SB_TOP, 0);
    pciNext->m_iRow = m_iFixedRows;

    return;
}

void CGridCtrl::KD_End(UINT nRepCnt, UINT nFlags, CCellID *pciNext)
{
    ASSERT_NULL(pciNext);

    SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
    pciNext->m_iRow = GetRowCount() - 1;

    return;
}

void CGridCtrl::KD_TabVisible(BOOL bChangeLine)
{
    if (IsSHIFTpressed())
    {
        if (bChangeLine)
        {
            SendMessage(WM_VSCROLL, SB_LINEUP, 0);
            SetScrollPos32(SB_HORZ, m_iHScrollMax);
            return;
        }

        SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
        return;
    }

    /* SHIFT isn't pressed */
    if (bChangeLine)
    {
        SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
        SetScrollPos32(SB_HORZ, 0);
        return;
    }

    SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
    return;
}

void CGridCtrl::KD_SetVisible(UINT nChar, CCellID ciNext, BOOL bChangeLine)
{
    COMP_BTRUE(IsCellVisible(ciNext));

    // Make sure cell is visible
    EnsureVisible(ciNext);

    switch (nChar)
    {
        case VK_RIGHT:
            SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
            break;

        case VK_LEFT:
            SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
            break;

        case VK_DOWN:
            SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
            break;

        case VK_UP:
            SendMessage(WM_VSCROLL, SB_LINEUP, 0);
            break;

        case VK_TAB:
            KD_TabVisible(bChangeLine);
            break;

    }
    Invalidate();
}

// move about with keyboard
void CGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    int     i           = 0;
    int     iFunNum     = 0;
    BOOL    bChangeLine = FALSE;
    CCellID ciNext;

    if (!IsValid(m_ciCurrent))
    {
        CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
        return;
    }

    ciNext = m_ciCurrent;

    /* CTRL + Key 特殊处理*/
    CTRLKeyDown(nChar);

    iFunNum = sizeof(g_tGCKeyDownProc)/sizeof(GCKEYDOWNFUNC);

    for (i=0; i<iFunNum; i++)
    {
        if ((g_tGCKeyDownProc[i].nChar == nChar))
        {
            (this->* g_tGCKeyDownProc[i].fKeyDownFunc)(nRepCnt, nFlags, &ciNext);
            break;
        }
    }
    if(i == iFunNum)
    {
        CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
    }
 
    /* 位置没有变化，直接返回 */
    COMP_BE(ciNext, m_ciCurrent);

    if(ciNext.m_iRow != m_ciCurrent.m_iRow)
        bChangeLine = TRUE;

    // While moving with the Cursorkeys the current ROW/CELL will get selected
    // OR Selection will get expanded when SHIFT is pressed
    // Cut n paste from OnLButtonDown - Franco Bez
    // Added check for NULL mouse mode - Chris Maunder.
    if (m_iMouseMode == MOUSE_NOTHING)
    {
        m_tPrevSelectedCellMap.RemoveAll();
        m_iMouseMode = m_bListMode? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
        if (!IsSHIFTpressed() || nChar == VK_TAB)
            m_ciSelectionStart = ciNext;
        OnSelecting(ciNext);
        m_iMouseMode = MOUSE_NOTHING;
    }

    SetFocusCell(ciNext);

    KD_SetVisible(nChar,ciNext, bChangeLine);
}

// Instant editing of cells when keys are pressed
void CGridCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (!IsCTRLpressed() && m_iMouseMode == MOUSE_NOTHING)
    {
        if (!m_bHandleTabKey || (m_bHandleTabKey && nChar != VK_TAB))
            OnEditCell(m_ciCurrent.m_iRow, m_ciCurrent.m_iCol, nChar);
    }

    CWnd::OnChar(nChar, nRepCnt, nFlags);
}

// Callback from any CInPlaceEdits that ended. This just calls OnEndEditCell,
// refreshes the edited cell and moves onto next cell if the return character
// from the edit says we should.
void CGridCtrl::OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
    GV_DISPINFO *pgvDispInfo = NULL;
    GV_ITEM     *pgvItem     = NULL;
    int         i            = 0;
    UINT        nValidVKey[9]= {VK_TAB,
                                VK_DOWN,
                                VK_UP,
                                VK_RIGHT,
                                VK_LEFT,
                                VK_NEXT,
                                VK_PRIOR,
                                VK_HOME,
                                VK_END
                                };

    ASSERT_NULL(pNMHDR);
    ASSERT_NULL(pResult);

    pgvDispInfo = (GV_DISPINFO *)pNMHDR;
    pgvItem     = &pgvDispInfo->item;

    // In case OnEndInPlaceEdit called as window is being destroyed
    COMP_BFALSE(IsWindow(GetSafeHwnd()));

    OnEndEditCell(pgvItem->iRow, pgvItem->iCol, pgvItem->szText);
    InvalidateCellRect(CCellID(pgvItem->iRow, pgvItem->iCol));

    SendMessageToParent(pgvItem->iRow, pgvItem->iCol, GVN_ENDLABELEDIT);

    *pResult = 0;

    for(i=0; i<9; i++)
    {
        if(pgvItem->lParam == nValidVKey[i])
            break;
    }
    COMP_BE(i, 9);

    OnKeyDown(pgvItem->lParam, 0, 0);
    OnEditCell(m_ciCurrent.m_iRow, m_ciCurrent.m_iCol, pgvItem->lParam);
}

void CGridCtrl::HS_LineRight(int iScrPos, CCellID ciTopLeft, CRect rcTmp)
{
    int     iXScroll = 0;

    COMP_BGE(iScrPos, m_iHScrollMax);

    iXScroll = GetColumnWidth(ciTopLeft.m_iCol);
    SetScrollPos32(SB_HORZ, iScrPos + iXScroll);

    // didn't work
    COMP_BE(GetScrollPos32(SB_HORZ), iScrPos);

    rcTmp.left = GetFixedColumnWidth() + iXScroll;
    ScrollWindow(-iXScroll, 0, rcTmp);
    rcTmp.left = rcTmp.right - iXScroll;
    InvalidateRect(rcTmp);
}

void CGridCtrl::HS_LineLeft(int iScrPos, CCellID ciTopLeft, CRect rcTmp)
{
    int     iXScroll = 0;

    COMP_BLE(iScrPos, 0);
    COMP_BLE(ciTopLeft.m_iCol, GetFixedColumnCount());

    iXScroll = GetColumnWidth(ciTopLeft.m_iCol-1);
    SetScrollPos32(SB_HORZ, max(0,iScrPos - iXScroll));
    rcTmp.left = GetFixedColumnWidth();
    ScrollWindow(iXScroll, 0, rcTmp);
    rcTmp.right = rcTmp.left + iXScroll;
    InvalidateRect(rcTmp);
}

void CGridCtrl::HS_PageRight(int iScrPos, CCellID ciTopLeft, CRect rcTmp)
{
    int     iPos    = 0;

    COMP_BGE(iScrPos, m_iHScrollMax);

    rcTmp.left = GetFixedColumnWidth();
    iPos       = min(m_iHScrollMax, iScrPos + rcTmp.Width());
    SetScrollPos32(SB_HORZ, iPos);
    rcTmp.left = GetFixedColumnWidth();
    InvalidateRect(rcTmp);
}

void CGridCtrl::HS_PageLeft(int iScrPos, CCellID ciTopLeft, CRect rcTmp)
{
    int     iPos    = 0;

    COMP_BLE(iScrPos, 0);

    rcTmp.left = GetFixedColumnWidth();
    iPos       = max(0, iScrPos -rcTmp.Width());
    SetScrollPos32(SB_HORZ, iPos);
    rcTmp.left = GetFixedColumnWidth();
    InvalidateRect(rcTmp);
}

void CGridCtrl::HS_Thumb(int iScrPos, CCellID ciTopLeft, CRect rcTmp)
{
    CCellID ciNewTopLeft;

    SetScrollPos32(SB_HORZ, GetScrollPos32(SB_HORZ, TRUE));

    ciNewTopLeft = GetTopleftNonFixedCell();
    COMP_BE(ciNewTopLeft, ciTopLeft);

    rcTmp.left = GetFixedColumnWidth();
    InvalidateRect(rcTmp);
}

void CGridCtrl::HS_Right(int iScrPos, CCellID ciTopLeft, CRect rcTmp)
{
    COMP_BGE(iScrPos, m_iHScrollMax);

    SetScrollPos32(SB_HORZ, m_iHScrollMax);
    Invalidate();
}

void CGridCtrl::HS_Left(int iScrPos, CCellID ciTopLeft, CRect rcTmp)
{
    COMP_BLE(iScrPos, 0);

    SetScrollPos32(SB_HORZ, 0);
    Invalidate();
}


// Handle horz scrollbar notifications
void CGridCtrl::OnHScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
    int     i       = 0;
    int     iFunNum = 0;
    int     iScrPos = 0;
    CCellID ciTopLeft;
    CRect   rcTmp;

    // Auto-destroy any InPlaceEdit's
    if (GetFocus()->GetSafeHwnd() != GetSafeHwnd())
        SetFocus();

#ifndef GRIDCONTROL_NO_TITLETIPS
    m_tTitleTip.Hide();  // hide any titletips
#endif

    iScrPos   = GetScrollPos32(SB_HORZ);
    ciTopLeft = GetTopleftNonFixedCell();
    GetClientRect(&rcTmp);

    iFunNum = sizeof(g_tGCHScrollProc)/sizeof(GCHSCROLLFUNC);

    for (i=0; i<iFunNum; i++)
    {
        if ((g_tGCHScrollProc[i].nSBCode == nSBCode))
        {
            (this->* g_tGCHScrollProc[i].fHScrollFunc)(iScrPos, ciTopLeft, rcTmp);
            break;
        }
    }
}

void CGridCtrl::VS_LineDown (int iScrPos, CCellID ciTopLeft, CRect rcTmp)
{
    int iYScroll    = 0;

    COMP_BGE(iScrPos, m_iVScrollMax);

    iYScroll = GetRowHeight(ciTopLeft.m_iRow);
    SetScrollPos32(SB_VERT, iScrPos + iYScroll);

    // didn't work
    COMP_BE(GetScrollPos32(SB_VERT), iScrPos);

    rcTmp.top = GetFixedRowHeight() + iYScroll;

    ScrollWindow( 0, -iYScroll, rcTmp);
    rcTmp.top = rcTmp.bottom - iYScroll;
    InvalidateRect(rcTmp);
}

void CGridCtrl::VS_LineUp   (int iScrPos, CCellID ciTopLeft, CRect rcTmp)
{
    int iYScroll    = 0;

    COMP_BLE(iScrPos, 0);
    COMP_BLE(ciTopLeft.m_iRow, GetFixedRowCount());

    iYScroll = GetRowHeight(ciTopLeft.m_iRow-1);
    SetScrollPos32(SB_VERT, max(0, iScrPos - iYScroll));
    rcTmp.top = GetFixedRowHeight();

    ScrollWindow(0, iYScroll, rcTmp);
    rcTmp.bottom = rcTmp.top + iYScroll;
    InvalidateRect(rcTmp);
}

void CGridCtrl::VS_PageDown (int iScrPos, CCellID ciTopLeft, CRect rcTmp)
{
    COMP_BGE(iScrPos, m_iVScrollMax);

    rcTmp.top = GetFixedRowHeight();
    iScrPos = min(m_iVScrollMax, iScrPos + rcTmp.Height());
    SetScrollPos32(SB_VERT, iScrPos);
    rcTmp.top = GetFixedRowHeight();
    InvalidateRect(rcTmp);
}

void CGridCtrl::VS_PageUp   (int iScrPos, CCellID ciTopLeft, CRect rcTmp)
{
    int iPos = 0;

    COMP_BLE(iScrPos, 0);

    rcTmp.top = GetFixedRowHeight();
    iPos      = max(0, iScrPos -rcTmp.Height());
    SetScrollPos32(SB_VERT, iPos);
    rcTmp.top = GetFixedRowHeight();
    InvalidateRect(rcTmp);
}

void CGridCtrl::VS_Thumb    (int iScrPos, CCellID ciTopLeft, CRect rcTmp)
{
    CCellID ciNewTopLeft;

    SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT, TRUE));

    ciNewTopLeft = GetTopleftNonFixedCell();
    COMP_BE(ciNewTopLeft, ciTopLeft);

    rcTmp.top = GetFixedRowHeight();
    InvalidateRect(rcTmp);
}

void CGridCtrl::VS_Top      (int iScrPos, CCellID ciTopLeft, CRect rcTmp)
{
    COMP_BLE(iScrPos, 0);

    SetScrollPos32(SB_VERT, 0);
    Invalidate();
}

void CGridCtrl::VS_Bottom   (int iScrPos, CCellID ciTopLeft, CRect rcTmp)
{
    COMP_BGE(iScrPos, m_iVScrollMax);

    SetScrollPos32(SB_VERT, m_iVScrollMax);
    Invalidate();
}

// Handle vert scrollbar notifications
void CGridCtrl::OnVScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
    int     i       = 0;
    int     iFunNum = 0;
    int     iScrPos = 0;
    CCellID ciTopLeft;
    CRect   rcTmp;

    if (GetFocus()->GetSafeHwnd() != GetSafeHwnd())
        SetFocus();        // Auto-destroy any InPlaceEdit's

#ifndef GRIDCONTROL_NO_TITLETIPS
    m_tTitleTip.Hide();  // hide any titletips
#endif

    // Get the scroll position ourselves to ensure we get a 32 bit value
    iScrPos = GetScrollPos32(SB_VERT);
    ciTopLeft = GetTopleftNonFixedCell();
    GetClientRect(&rcTmp);

    iFunNum = sizeof(g_tGCVScrollProc)/sizeof(GCVSCROLLFUNC);

    for (i=0; i<iFunNum; i++)
    {
        if ((g_tGCVScrollProc[i].nSBCode == nSBCode))
        {
            (this->* g_tGCVScrollProc[i].fVScrollFunc)(iScrPos, ciTopLeft, rcTmp);
            break;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl implementation functions
/* draw top-left cells 0..m_iFixedRows-1, 0..m_iFixedCols-1 */
void CGridCtrl::DrawFixedTopLeftCell(CDC* pDC)
{
    int     iRow  = 0;
    int     iCol  = 0;
    CRect   rcTmp;

    ASSERT_NULL(pDC);
    
    rcTmp.bottom = -1;
    for (iRow = 0; iRow < m_iFixedRows; iRow++)
    {
        rcTmp.top    = rcTmp.bottom + 1;
        rcTmp.bottom = rcTmp.top + GetRowHeight(iRow) - 1;
        rcTmp.right  = -1;
        for (iCol = 0; iCol < m_iFixedCols; iCol++)
        {
            rcTmp.left  = rcTmp.right + 1;
            rcTmp.right = rcTmp.left  + GetColumnWidth(iCol)-1;

            DrawFixedCell(pDC, iRow, iCol, rcTmp);
        }
    }

    return;
}

/* draw fixed column cells:  m_iFixedRows..n, 0..m_iFixedCols-1 */
void CGridCtrl::DrawFixedColCell(CDC* pDC, CRect rcClip, int iFixedRowHeight,
                                 int iMinVisRow, int iMaxVisRow)
{
    int     iRow  = 0;
    int     iCol  = 0;
    CRect   rcTmp;

    ASSERT_NULL(pDC);
    
    rcTmp.bottom = iFixedRowHeight-1;
    for (iRow = iMinVisRow; iRow <= iMaxVisRow; iRow++)
    {
        rcTmp.top    = rcTmp.bottom + 1;
        rcTmp.bottom = rcTmp.top    + GetRowHeight(iRow)-1;

        // rcTmp.bottom = bottom pixel of previous row
        if (rcTmp.top > rcClip.bottom)
            break;                // Gone past cliprect

        COMP_BTRUE_CONTINUE((rcTmp.bottom < rcClip.top)); // Reached cliprect yet?

        rcTmp.right = -1;
        for (iCol = 0; iCol < m_iFixedCols; iCol++)
        {
            rcTmp.left  = rcTmp.right + 1;
            rcTmp.right = rcTmp.left  + GetColumnWidth(iCol)-1;

            if (rcTmp.left  > rcClip.right)
                break;            // gone past cliprect
            COMP_BTRUE_CONTINUE((rcTmp.right < rcClip.left)); // Reached cliprect yet?

            DrawFixedCell(pDC, iRow, iCol, rcTmp);
        }
    }
}

/* draw fixed row cells  0..m_iFixedRows, m_iFixedCols..n */
void CGridCtrl::DrawFixedRowCell(CDC* pDC, CRect rcClip, int iFixedColWidth,
                                 int iMinVisCol, int iMaxVisCol)
{
    int     iRow  = 0;
    int     iCol  = 0;
    CRect   rcTmp;

    ASSERT_NULL(pDC);

    rcTmp.bottom = -1;
    for (iRow = 0; iRow < m_iFixedRows; iRow++)
    {
        rcTmp.top = rcTmp.bottom+1;
        rcTmp.bottom = rcTmp.top + GetRowHeight(iRow)-1;

        // rcTmp.bottom = bottom pixel of previous row
        if (rcTmp.top > rcClip.bottom)
            break;                // Gone past cliprect
        COMP_BTRUE_CONTINUE((rcTmp.bottom < rcClip.top)); // Reached cliprect yet?

        rcTmp.right = iFixedColWidth-1;
        for (iCol = iMinVisCol; iCol <= iMaxVisCol; iCol++)
        {
            rcTmp.left = rcTmp.right+1;
            rcTmp.right = rcTmp.left + GetColumnWidth(iCol)-1;

            if (rcTmp.left > rcClip.right)
                break;        // gone past cliprect
            COMP_BTRUE_CONTINUE((rcTmp.right < rcClip.left)); // Reached cliprect yet?

            DrawFixedCell(pDC, iRow, iCol, rcTmp);
        }
    }
}

/* draw rest of non-fixed cells */
void CGridCtrl::DrawNonFixedCell( CDC* pDC, CRect rcClip,
                                  int iFixedRowHeight, int iFixedColWidth,
                                  int iMinVisRow, int iMaxVisRow,
                                  int iMinVisCol, int iMaxVisCol)
{
    int     iRow  = 0;
    int     iCol  = 0;
    CRect   rcTmp;

    ASSERT_NULL(pDC);
    
    rcTmp.bottom = iFixedRowHeight-1;
    for (iRow = iMinVisRow; iRow <= iMaxVisRow; iRow++)
    {

        rcTmp.top    = rcTmp.bottom + 1;
        rcTmp.bottom = rcTmp.top    + GetRowHeight(iRow)-1;

        // rcTmp.bottom = bottom pixel of previous row
        if (rcTmp.top > rcClip.bottom)
            break;                // Gone past cliprect
        COMP_BTRUE_CONTINUE((rcTmp.bottom < rcClip.top)); // Reached cliprect yet?

        rcTmp.right = iFixedColWidth-1;
        for (iCol = iMinVisCol; iCol <= iMaxVisCol; iCol++)
        {
            rcTmp.left  = rcTmp.right + 1;
            rcTmp.right = rcTmp.left  + GetColumnWidth(iCol)-1;

            if (rcTmp.left > rcClip.right)
                break;        // gone past cliprect
            COMP_BTRUE_CONTINUE((rcTmp.right < rcClip.left)); // Reached cliprect yet?

           DrawCell(pDC, iRow, iCol, rcTmp);//this is needed anytime
        }
    }
}

/* draw vertical lines (drawn at ends of cells) */
void CGridCtrl::DrawVertLine(CDC* pDC, CRect rcVisible,
                             int iFixedRowHeight, int iFixedColWidth,
                             int iMinVisRow, int iMaxVisRow,
                             int iMinVisCol, int iMaxVisCol)
{
    int       iRow      = 0;
    int       iCol      = 0;
    int       x         = 0;
    CGridCell *pgcCell  = NULL;
    CRect     rcRemoveLine;

    /* if (m_iGridLines == GVL_BOTH || m_iGridLines == GVL_VERT) */
    COMP_BE(m_iGridLines, GVL_NONE);
    COMP_BE(m_iGridLines, GVL_HORZ);

    //above is added for combine cell
    x = iFixedColWidth;
    for (iCol = iMinVisCol; iCol <= iMaxVisCol; iCol++)
    {
        x += GetColumnWidth(iCol);

        pDC->MoveTo(x-1, iFixedRowHeight);
        pDC->LineTo(x-1, rcVisible.bottom);

        //want remove it?
        for(iRow = iMinVisRow; iRow <= iMaxVisRow; iRow++)
        {
            pgcCell = GetCell(iRow,iCol);
            COMP_BNULL_CONTINUE(pgcCell);

           if(pgcCell->m_bSkipV==TRUE)
           {
               GetCellRect(iRow,iCol,rcRemoveLine);
               pDC->MoveTo(x-1, rcRemoveLine.top );
               pDC->LineTo(x-1, rcRemoveLine.bottom+1);
           }
       }
        //above is added for combine;
    }
}

/* draw horizontal lines (drawn at ends of cells) */
void CGridCtrl::DrawHorzLine(CDC* pDC, CRect rcVisible,
                             int iFixedRowHeight, int iFixedColWidth,
                             int iMinVisRow, int iMaxVisRow,
                             int iMinVisCol, int iMaxVisCol)
{
    int       iRow      = 0;
    int       iCol      = 0;
    int       y         = 0;
    CGridCell *pgcCell  = NULL;
    CRect     rcRemoveLine;

    /* if (m_iGridLines == GVL_BOTH || m_iGridLines == GVL_HORZ) */
    COMP_BE(m_iGridLines, GVL_NONE);
    COMP_BE(m_iGridLines, GVL_VERT);

    //above is added for combine cell
    y = iFixedRowHeight;
    for (iRow = iMinVisRow; iRow <= iMaxVisRow; iRow++)
    {
        y += GetRowHeight(iRow);
        pDC->MoveTo(iFixedColWidth,   y-1);
        pDC->LineTo(rcVisible.right,  y-1);

        //want remove it?
        for(iCol = iMinVisCol; iCol <= iMaxVisCol; iCol++)
        {
            pgcCell = GetCell(iRow,iCol);
            COMP_BNULL_CONTINUE(pgcCell);

           if(pgcCell->m_bSkipH==TRUE)
           {
               GetCellRect(iRow,iCol,rcRemoveLine);
               pDC->MoveTo(rcRemoveLine.left-1,y-1 );
               pDC->LineTo(rcRemoveLine.right, y-1);
           }
        }
    }
}

void CGridCtrl::OnDraw(CDC* pDC)
{
    int         iFixedRowHeight = 0;
    int         iFixedColWidth  = 0;
    int         iMinVisRow      = 0;
    int         iMinVisCol      = 0;
    int         iMaxVisRow      = 0;
    int         iMaxVisCol      = 0;
    int         iOrientedROP    = 0;
    CCellID     ciTopLeft;
    CCellRange  crVisible;
    CRect       rcClip;
    CRect       rcVis;
    CPen        penGrid;

    COMP_BE(pDC->GetClipBox(&rcClip), ERROR);

    EraseBkgnd(pDC);            // OnEraseBkgnd does nothing, so erase bkgnd here.
                                // This necessary since we may be using a Memory DC.

    iFixedRowHeight = GetFixedRowHeight();
    iFixedColWidth  = GetFixedColumnWidth();

    ciTopLeft  = GetTopleftNonFixedCell();
    iMinVisRow = ciTopLeft.m_iRow;
    iMinVisCol = ciTopLeft.m_iCol;

    crVisible  = GetVisibleNonFixedCellRange(rcVis);
    iMaxVisRow = crVisible.GetMaxRow();
    iMaxVisCol = crVisible.GetMaxCol();

    // draw top-left cells 0..m_iFixedRows-1, 0..m_iFixedCols-1
    DrawFixedTopLeftCell(pDC);

    // draw fixed column cells:  m_iFixedRows..n, 0..m_iFixedCols-1
    DrawFixedColCell(pDC, rcClip, iFixedRowHeight, iMinVisRow, iMaxVisRow);

    // draw fixed row cells  0..m_iFixedRows, m_iFixedCols..n
    DrawFixedRowCell(pDC, rcClip, iFixedColWidth,  iMinVisCol, iMaxVisCol);

    // draw rest of non-fixed cells
    DrawNonFixedCell(pDC, rcClip, iFixedRowHeight,iFixedColWidth,
                     iMinVisRow, iMaxVisRow, iMinVisCol, iMaxVisCol);


    penGrid.CreatePen(PS_SOLID, 0, m_clrGrid);
    pDC->SelectObject(&penGrid);
    iOrientedROP = pDC->SetROP2(R2_NOT);

    // draw vertical lines (drawn at ends of cells)
    DrawVertLine(pDC, rcVis, iFixedRowHeight,iFixedColWidth,
                 iMinVisRow, iMaxVisRow, iMinVisCol, iMaxVisCol);

    // draw horizontal lines (drawn at bottom of each cell)
    DrawHorzLine(pDC, rcVis, iFixedRowHeight,iFixedColWidth,
                 iMinVisRow, iMaxVisRow, iMinVisCol, iMaxVisCol);

    pDC->SetROP2(iOrientedROP);//added for combine;
    pDC->SelectStockObject(NULL_PEN);
}

////////////////////////////////////////////////////////////////////////////////////////
// CGridCtrl Cell selection stuff

BOOL CGridCtrl::IsValid(int iRow, int iCol) const
{
    COMP_BL_R(iRow, 0, FALSE);
    COMP_BL_R(iCol, 0, FALSE);
    COMP_BGE_R(iRow, m_iRows, FALSE);
    COMP_BGE_R(iCol, m_iCols, FALSE);

    return TRUE;
}

BOOL CGridCtrl::IsValid(const CCellID& ciCell) const
{
    return IsValid(ciCell.m_iRow, ciCell.m_iCol);
}

BOOL CGridCtrl::IsValid(const CCellRange& crRange) const
{
    COMP_BL_R(crRange.GetMinRow(), 0, FALSE);
    COMP_BL_R(crRange.GetMinCol(), 0, FALSE);
    COMP_BL_R(crRange.GetMaxRow(), 0, FALSE);
    COMP_BL_R(crRange.GetMaxCol(), 0, FALSE);
    COMP_BGE_R(crRange.GetMaxRow(), m_iRows, FALSE);
    COMP_BGE_R(crRange.GetMaxCol(), m_iCols, FALSE);
    COMP_BG_R(crRange.GetMinRow(), crRange.GetMaxRow(), FALSE);
    COMP_BG_R(crRange.GetMinCol(), crRange.GetMaxCol(), FALSE);

    return TRUE;
    /*
     return (crRange.GetMinRow() >= 0 && crRange.GetMinCol() >= 0 &&
             crRange.GetMaxRow() >= 0 && crRange.GetMaxCol() >= 0 &&
             crRange.GetMaxRow() < m_iRows && crRange.GetMaxCol() < m_iCols &&
             crRange.GetMinRow() <= crRange.GetMaxRow() && crRange.GetMinCol() <= crRange.GetMaxCol());
    */
}

// Enables/Disables redraw for certain operations like columns auto-sizing etc,
// but not for user caused things such as selection changes.
void CGridCtrl::SetRedraw(BOOL bAllowDraw, BOOL bResetScrollBars /* = FALSE */)
{
    TRACE(_T("%s: Setting redraw to %s\n"),
             GetRuntimeClass()->m_lpszClassName, bAllowDraw? _T("TRUE") : _T("FALSE"));

    if (bAllowDraw && !m_bAllowDraw)
        Invalidate();
    m_bAllowDraw = bAllowDraw;
    if (bResetScrollBars)
        ResetScrollBars();
}

// Forces a redraw of a cell immediately (using a direct DC construction,
// or the supplied dc)
BOOL CGridCtrl::RedrawCell(const CCellID& ciCell, CDC* pDC /* = NULL */)
{
    return RedrawCell(ciCell.m_iRow, ciCell.m_iCol, pDC);
}

BOOL CGridCtrl::RedrawFixedCell(CDC* pDC, int iRow, int iCol, CRect &rcCell)
{
    CGridCell   *pgcCell    = NULL;

    ASSERT_NULL_R(pDC, FALSE);

    pgcCell = GetCell(iRow,iCol);
    ASSERT_NULL_R(pgcCell, FALSE);

    if(pgcCell->m_bHide==TRUE)
    {
        iRow = pgcCell->m_iTopleftRow;
        iCol = pgcCell->m_iTopleftCol;
    }

    //above added for combine fixedcell
    return DrawFixedCell(pDC, iRow, iCol, rcCell, TRUE);
}

//there is bug in the combining procedure if you want to use\
//the GetCellRangeRect(),so i manpower adjust the "rcTmp" variable
BOOL CGridCtrl::RedrawNormalCell(CDC* pDC, int iRow, int iCol, CRect &rcCell)
{
    CGridCell   *pgcCell        = NULL;
    CGridCell   *pgcTopLeft     = NULL;
    CRect       rcTmp2;
    CCellID     ciTopLeft;
    int         iRightCol       = 0;
    int         iBottomRow      = 0;
    int         iMinVisibleRow  = 0;
    int         iMinVisibleCol  = 0;

    ASSERT_NULL_R(pDC, FALSE);

    pgcCell = GetCell(iRow,iCol);
    ASSERT_NULL_R(pgcCell, FALSE);

    if(pgcCell->m_bHide==TRUE)
    {
        iRow = pgcCell->m_iTopleftRow;
        iCol = pgcCell->m_iTopleftCol;
        pgcTopLeft = GetCell(pgcCell->m_iTopleftRow,pgcCell->m_iTopleftCol);
        if(pgcTopLeft != NULL)
        {
           iRightCol  = iCol + pgcTopLeft->m_iNumVhide;
           iBottomRow = iRow + pgcTopLeft->m_iNumHhide;
        }
    }
    else if(pgcCell->m_iNumHhide != 0 || pgcCell->m_iNumVhide != 0)
    {
        iRightCol  = iCol + pgcCell->m_iNumVhide;
        iBottomRow = iRow + pgcCell->m_iNumHhide;
    }

    if(CELLHIDE(pgcCell))
    {
        if(GetCellRect(iBottomRow, iRightCol, rcTmp2))
        {
            rcCell.right  = rcTmp2.right;
            rcCell.bottom = rcTmp2.bottom;


            ciTopLeft      = GetTopleftNonFixedCell();
            iMinVisibleRow = ciTopLeft.m_iRow;
            iMinVisibleCol = ciTopLeft.m_iCol;
            iMinVisibleRow = iMinVisibleRow>iRow?iMinVisibleRow:iRow;
            iMinVisibleCol = iMinVisibleCol>iCol?iMinVisibleCol:iCol;

            GetCellRect(iMinVisibleRow, iMinVisibleCol, rcTmp2);
            rcCell.left = rcTmp2.left;
            rcCell.top  = rcTmp2.top;
        }
    }

    //above added for combine cell;
    return DrawCell(pDC, iRow, iCol, rcCell, TRUE);
}

void CGridCtrl::RedrawGridLines(CDC* pDC, CRect rcCell)
{
    CPen    penGrid;
    CPen    *pOldPen = NULL;

    ASSERT_NULL(pDC);

    penGrid.CreatePen(PS_SOLID, 0, m_clrGrid);
    pOldPen = (CPen*)pDC->SelectObject(&penGrid);
    
    if (m_iGridLines == GVL_BOTH || m_iGridLines == GVL_HORZ)
    {
        pDC->MoveTo(rcCell.left,    rcCell.bottom);
        pDC->LineTo(rcCell.right+1, rcCell.bottom);
    }
    if (m_iGridLines == GVL_BOTH || m_iGridLines == GVL_VERT)
    {
        pDC->MoveTo(rcCell.right, rcCell.top);
        pDC->LineTo(rcCell.right, rcCell.bottom+1);
    }
    pDC->SelectObject(pOldPen);
}

BOOL CGridCtrl::RedrawCellWithDC(CDC* pDC, int iRow, int iCol, CRect rcCell)
{
    BOOL bResult = TRUE;

    ASSERT_NULL_R(pDC, FALSE);

    // Redraw cells directly
    if (iRow < m_iFixedRows || iCol < m_iFixedCols)
    {
        return RedrawFixedCell(pDC, iRow, iCol, rcCell);
    }

    bResult = RedrawNormalCell(pDC, iRow, iCol, rcCell);

    // Since we have erased the background, we will need to redraw the gridlines
    RedrawGridLines(pDC, rcCell);

    return bResult;
}

BOOL CGridCtrl::RedrawCell(int iRow, int iCol, CDC* pDC /* = NULL */)
{
    BOOL bResult        = TRUE;
    BOOL bMustReleaseDC = FALSE;
    CRect rcTmp;

    COMP_BFALSE_R(m_bAllowDraw, FALSE);
    COMP_BFALSE_R(IsCellVisible(iRow, iCol), FALSE);
    COMP_BFALSE_R(GetCellRect(iRow, iCol, rcTmp), FALSE);

    if (!pDC)
    {
        pDC = GetDC();
        if (!pDC)
        {
            InvalidateRect(rcTmp, TRUE);    // Could not get a DC - invalidate it anyway
                                            // and hope that OnPaint manages to get one
            return TRUE;
        }

        bMustReleaseDC = TRUE;
    }

    bResult = RedrawCellWithDC(pDC, iRow, iCol, rcTmp);

    if (bMustReleaseDC)
        ReleaseDC(pDC);

    return bResult;
}

// redraw a complete row
BOOL CGridCtrl::RedrawRow(int iRow)
{
    BOOL bResult = TRUE;
    int  iCol    = 0;
    CDC  *pDC    = NULL;

    pDC = GetDC();

    for (iCol = 0; iCol < GetColumnCount(); iCol++)
    {
        bResult = RedrawCell(iRow, iCol, pDC) && bResult;
    }

    if (pDC)
        ReleaseDC(pDC);

    return bResult;
}

// redraw a complete column
BOOL CGridCtrl::RedrawColumn(int iCol)
{
    BOOL bResult = TRUE;
    int  iRow    = 0;
    CDC  *pDC    = NULL;

    pDC = GetDC();

    for (iRow = 0; iRow < GetRowCount(); iRow++)
    {
        bResult = RedrawCell(iRow, iCol, pDC) && bResult;
    }

    if (pDC)
        ReleaseDC(pDC);

    return bResult;
}

// Sets the currently selected cell, returning the previous current cell
CCellID CGridCtrl::SetFocusCell(int iRow, int iCol)
{
    return SetFocusCell(CCellID(iRow, iCol));
}

void CGridCtrl::SelectedCell(CCellID ciPreCell, CCellID ciCurCel)
{
    int     iRow    = 0;
    int     iCol    = 0;

    COMP_BFALSE(IsValid(ciCurCel));

    SetItemState(ciCurCel.m_iRow, ciCurCel.m_iCol,
                 GetItemState(ciCurCel.m_iRow, ciCurCel.m_iCol) | GVIS_FOCUSED);

    RedrawCell(ciCurCel);

    if (ciPreCell.m_iCol != ciCurCel.m_iCol)
    {
        for (iRow = 0; iRow < m_iFixedRows; iRow++)
           RedrawCell(iRow, ciCurCel.m_iCol);
    }
    if (ciPreCell.m_iRow != ciCurCel.m_iRow)
    {
        for (iCol = 0; iCol < m_iFixedCols; iCol++)
            RedrawCell(ciCurCel.m_iRow, iCol);
    }

    SendMessageToParent(ciCurCel.m_iRow, ciCurCel.m_iCol, GVN_SELCHANGED);
}

void CGridCtrl::UnSelectCell(CCellID ciPreCell, CCellID ciCurCel)
{
    int     iRow    = 0;
    int     iCol    = 0;

    SendMessageToParent(ciPreCell.m_iRow, ciPreCell.m_iCol, GVN_SELCHANGING);

    SetItemState(ciPreCell.m_iRow, ciPreCell.m_iCol,
                 GetItemState(ciPreCell.m_iRow, ciPreCell.m_iCol) & ~GVIS_FOCUSED);
    RedrawCell(ciPreCell);

    if (ciPreCell.m_iCol != ciCurCel.m_iCol)
    {
        for (iRow = 0; iRow < m_iFixedRows; iRow++)
            RedrawCell(iRow, ciPreCell.m_iCol);
    }
    if (ciPreCell.m_iRow != ciCurCel.m_iRow)
    {
        for (iCol = 0; iCol < m_iFixedCols; iCol++)
            RedrawCell(ciPreCell.m_iRow, iCol);
    }
}

CCellID CGridCtrl::SetFocusCell(CCellID ciCell)
{
    int     iRow    = 0;
    int     iCol    = 0;
    CCellID ciPrev;

    COMP_BE_R(ciCell, m_ciCurrent, m_ciCurrent);

    ciPrev      = m_ciCurrent;
    m_ciCurrent = ciCell;

    UnSelectCell(ciPrev, m_ciCurrent);

    SelectedCell(ciPrev, m_ciCurrent);

    return ciPrev;
}

// Sets the range of currently selected cells
void CGridCtrl::SetSelectedRange(const CCellRange& crRange,
                                 BOOL bForceRepaint /* = FALSE */)
{
    SetSelectedRange(crRange.GetMinRow(), crRange.GetMinCol(),
                     crRange.GetMaxRow(), crRange.GetMaxCol(),
                     bForceRepaint);
}


CELL_RANGE  CGridCtrl::InitCellRange(int iMinRow, int iMinCol, int iMaxRow, int iMaxCol)
{
    CELL_RANGE  crTmp;

    crTmp.iMinRow = iMinRow;
    crTmp.iMinCol = iMinCol;
    crTmp.iMaxRow = iMaxRow;
    crTmp.iMaxCol = iMaxCol;

    return crTmp;
}

BOOL CGridCtrl::CellInRange(CCellID ciCell, CELL_RANGE crRange)
{
    COMP_BL_R(ciCell.m_iRow, crRange.iMinRow, FALSE);
    COMP_BL_R(ciCell.m_iCol, crRange.iMinCol, FALSE);
    COMP_BG_R(ciCell.m_iRow, crRange.iMaxRow, FALSE);
    COMP_BG_R(ciCell.m_iCol, crRange.iMaxCol, FALSE);

    return TRUE;
}

BOOL CGridCtrl::IsValidRange(CELL_RANGE crRange)
{
    COMP_BL_R(crRange.iMinRow, 0, FALSE);
    COMP_BL_R(crRange.iMinCol, 0, FALSE);
    COMP_BL_R(crRange.iMaxRow, 0, FALSE);
    COMP_BL_R(crRange.iMaxCol, 0, FALSE);
    COMP_BGE_R(crRange.iMaxRow, m_iRows, FALSE);
    COMP_BGE_R(crRange.iMaxCol, m_iCols, FALSE);
    COMP_BG_R(crRange.iMinRow, crRange.iMaxRow, FALSE);
    COMP_BG_R(crRange.iMinCol, crRange.iMaxCol, FALSE);

    return TRUE;
}

void CGridCtrl::UnselectedPreCells(CELL_RANGE crRange, CDC *pDC)
{
    POSITION    pos;
    DWORD       dwKey   = 0;
    CCellID     ciCell;

    for (pos = m_tSelectedCellMap.GetStartPosition(); pos != NULL; )
    {
        m_tSelectedCellMap.GetNextAssoc(pos, dwKey, (CCellID&)ciCell);

        // Reset the selection flag on the cell
        COMP_BFALSE_CONTINUE(IsValid(ciCell));

        SetItemState(ciCell.m_iRow, ciCell.m_iCol,
                     GetItemState(ciCell.m_iRow, ciCell.m_iCol) & ~GVIS_SELECTED);

        // If this is to be reselected, continue on past the redraw
        COMP_BTRUE_CONTINUE(CellInRange(ciCell, crRange));

        // Redraw NOW
        if (pDC)                    
            RedrawCell(ciCell.m_iRow, ciCell.m_iCol, pDC);
        // Redraw at leisure
        else
            InvalidateCellRect(ciCell);                
    }
}

void CGridCtrl::SetSelectedPreCells(CDC *pDC)
{
    POSITION    pos;
    DWORD       dwKey   = 0;
    UINT        nState  = 0;
    CCellID     ciCell;

    COMP_BTRUE(GetSingleRowSelection());

    for (pos = m_tPrevSelectedCellMap.GetStartPosition(); pos != NULL; /* nothing */)
    {
        m_tPrevSelectedCellMap.GetNextAssoc(pos, dwKey, (CCellID&)ciCell);

        COMP_BFALSE_CONTINUE(IsValid(ciCell));

        nState = GetItemState(ciCell.m_iRow, ciCell.m_iCol);

        // Set state as Selected. This will add the cell to m_SelectedCells[]
        SetItemState(ciCell.m_iRow, ciCell.m_iCol, nState | GVIS_SELECTED);

        // Redraw (immediately or at leisure)
        if (pDC)
            RedrawCell(ciCell.m_iRow, ciCell.m_iCol, pDC);
        else
            InvalidateCellRect(ciCell);
    }
}

void CGridCtrl::SetSelectedState(CELL_RANGE crRange, CDC *pDC)
{
    int     iRow    = 0;
    int     iCol    = 0;
    UINT    nState  = 0;
    CCellID ciCell;

    COMP_BFALSE(IsValidRange(crRange));
    
    for (iRow = crRange.iMinRow; iRow <= crRange.iMaxRow; iRow++)
        for (iCol = crRange.iMinCol; iCol <= crRange.iMaxCol; iCol++)
        {
            nState = GetItemState(iRow, iCol);

            // Already selected - ignore
            COMP_BTRUE_CONTINUE(nState & GVIS_SELECTED); 

            // Add to list of selected cells
            ciCell = CCellID(iRow,iCol);

            // Set state as Selected. This will add the cell to m_SelectedCells[]
            SetItemState(iRow, iCol, nState | GVIS_SELECTED);

            // Redraw (immediately or at leisure)
            if (pDC)
                RedrawCell(iRow, iCol, pDC);
            else
                InvalidateCellRect(ciCell);
        }
//    TRACE(_T("%d cells selected.\n"), m_tSelectedCellMap.GetCount());
}

void CGridCtrl::SetSelectedRange(int iMinRow, int iMinCol, int iMaxRow, int iMaxCol,
                                 BOOL bForceRepaint /* = FALSE */)
{
    CELL_RANGE  crRange;
    CDC         *pDC = NULL;

    COMP_BFALSE(m_bEnableSelection);

    crRange = InitCellRange(iMinRow, iMinCol, iMaxRow, iMaxCol);

    if (bForceRepaint)
        pDC = GetDC();

    // Unselect all previously selected cells
    UnselectedPreCells(crRange, pDC);

    // if any previous selected cells are to be retained (eg Ctrl is being held down)
    // then copy them to the newly created list, and mark all these cells as selected
    SetSelectedPreCells(pDC);

    // Now select all cells in the cell range specified. If the cell has already
    // been marked as selected (above) then ignore it.
    SetSelectedState(crRange, pDC);

    if (pDC != NULL)
        ReleaseDC(pDC);
}

// selects all cells
void CGridCtrl::SelectAllCells()
{
    COMP_BFALSE(m_bEnableSelection);

    SetSelectedRange(m_iFixedRows, m_iFixedCols, GetRowCount()-1, GetColumnCount()-1);
}

// selects columns
void CGridCtrl::SelectColumns(CCellID ciCurrent)
{
    COMP_BFALSE(m_bEnableSelection);

    //if (currentcell.m_iCol == m_ciCurrent.m_iCol) return;
    COMP_BL(ciCurrent.m_iCol, m_iFixedCols);
    COMP_BFALSE(IsValid(ciCurrent));

    SetSelectedRange(GetFixedRowCount(),
                     min(m_ciSelectionStart.m_iCol, ciCurrent.m_iCol),
                     GetRowCount()-1,
                     max(m_ciSelectionStart.m_iCol, ciCurrent.m_iCol));
}

// selects rows
void CGridCtrl::SelectRows(CCellID ciCurrent)
{
    COMP_BFALSE(m_bEnableSelection);

    //if (currentcell.m_iRow; == m_ciCurrent.m_iRow) return;
    COMP_BL(ciCurrent.m_iRow, m_iFixedRows);
    COMP_BFALSE(IsValid(ciCurrent));

    if (GetListMode() && GetSingleRowSelection())
        SetSelectedRange(ciCurrent.m_iRow, GetFixedColumnCount(),
                         ciCurrent.m_iRow, GetColumnCount()-1);
    else
        SetSelectedRange(min(m_ciSelectionStart.m_iRow, ciCurrent.m_iRow),
                         GetFixedColumnCount(),
                         max(m_ciSelectionStart.m_iRow, ciCurrent.m_iRow),
                         GetColumnCount()-1);
}

// selects cells
void CGridCtrl::SelectCells(CCellID ciCurrent)
{
    int     iRow    = 0;
    int     iCol    = 0;

    COMP_BFALSE(m_bEnableSelection);

    iRow = ciCurrent.m_iRow;
    iCol = ciCurrent.m_iCol;
    COMP_BL(iRow, m_iFixedRows);
    COMP_BL(iCol, m_iFixedCols);
    COMP_BFALSE(IsValid(ciCurrent));

    // Prevent unnecessary redraws
    //if (ciCurrent == m_ciLeftClickDown)  return;
    //else if (ciCurrent == m_ciCurrent) return;

    SetSelectedRange(min(m_ciSelectionStart.m_iRow, iRow),
                     min(m_ciSelectionStart.m_iCol, iCol),
                     max(m_ciSelectionStart.m_iRow, iRow),
                     max(m_ciSelectionStart.m_iCol, iCol));
}

void CGridCtrl::OnSelecting(const CCellID& ciCurrent)
{
    COMP_BFALSE(m_bEnableSelection);

    switch(m_iMouseMode)
    {
        case MOUSE_SELECT_ALL:
            SelectAllCells();
            break;

        case MOUSE_SELECT_COL:
            SelectColumns(ciCurrent);
            break;

        case MOUSE_SELECT_ROW:
            SelectRows(ciCurrent);
            break;

        case MOUSE_SELECT_CELLS:
            SelectCells(ciCurrent);
            break;
    }
}

#ifndef GRIDCONTROL_NO_CLIPBOARD

////////////////////////////////////////////////////////////////////////////////////////
// Clipboard functions

void CGridCtrl::CutSelectedText()
{
    POSITION    pos;
    DWORD       dwKey   = 0;
    CCellID     ciCell;
    CGridCell   *pgcCell= NULL;

    COMP_BFALSE(IsEditable());

    // Clear contents of selected cells.
    for (pos = m_tSelectedCellMap.GetStartPosition(); pos != NULL; )
    {
        m_tSelectedCellMap.GetNextAssoc(pos, dwKey, (CCellID&)ciCell);

        COMP_BFALSE_CONTINUE(IsCellEditable(ciCell));

        pgcCell = GetCell(ciCell.m_iRow, ciCell.m_iCol);
        if (pgcCell)
        {
            SendMessageToParent(ciCell.m_iRow, ciCell.m_iCol, GVN_BEGINLABELEDIT);
            EmptyCell(pgcCell, ciCell.m_iRow, ciCell.m_iCol);
            SetModified(TRUE, ciCell.m_iRow, ciCell.m_iCol);
            SendMessageToParent(ciCell.m_iRow, ciCell.m_iCol, GVN_ENDLABELEDIT);
        }
    }
    Invalidate();
}

COleDataSource* CGridCtrl::CopyTextFromGrid()
{
    char        cTmp;
    CString     strTmp;
    CCellRange  crSelection;
    CGridCell   *pgcCell = NULL;
    int         iRow     = 0;
    int         iCol     = 0;
    DWORD       dwLen    = 0;
    HGLOBAL     hMem     = NULL;
    COleDataSource  *pSource = NULL;

    USES_CONVERSION;

    crSelection = GetSelectedCellRange();
    COMP_BFALSE_R(IsValid(crSelection), NULL);

    // Write to shared file (REMEBER: CF_TEXT is ANSI, not UNICODE, so we need to convert)
    CSharedFile sfTmp(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

    // Get a tab delimited string to copy to cache
    for (iRow = crSelection.GetMinRow(); iRow <= crSelection.GetMaxRow(); iRow++)
    {
        strTmp.Empty();
        for (iCol = crSelection.GetMinCol(); iCol <= crSelection.GetMaxCol(); iCol++)
        {
            pgcCell = GetCell(iRow,iCol);
            if (pgcCell && (pgcCell->m_nState & GVIS_SELECTED))
            {
                if (pgcCell->m_szText.IsEmpty())
                    strTmp += _T(" ");
                else
                   strTmp += pgcCell->m_szText;
            }
            if (iCol != crSelection.GetMaxCol())
                strTmp += _T("\t");
        }
        if (iRow != crSelection.GetMaxRow())
            strTmp += _T("\n");

        sfTmp.Write(T2A(strTmp.GetBuffer(1)), strTmp.GetLength());
        strTmp.ReleaseBuffer();
    }

    cTmp = '\0';
    sfTmp.Write(&cTmp, 1);

    dwLen = (DWORD)sfTmp.GetLength();
    hMem = sfTmp.Detach();
    ASSERT_NULL_R(hMem, NULL);

    hMem = ::GlobalReAlloc(hMem, dwLen, GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
    ASSERT_NULL_R(hMem, NULL);

    // Cache data
    pSource = new COleDataSource();
    ASSERT_NULL_R(pSource, NULL);
    pSource->CacheGlobalData(CF_TEXT, hMem);

    return pSource;
}

void CGridCtrl::SetTextInCell(CCellID ciCell, CString strText)
{
    int     iIndex  = 0;
    int     iLine   = 0;
    int     iColumn = 0;
    CString strLine;
    CString strNext;
    CString strCellText;
    LPTSTR  pszLine     = NULL;
    LPTSTR  pszCellText = NULL;
    CGridCell *pgcCell  = NULL;//added for combine
    CCellID ciTarget;
    
    strText.LockBuffer();
    strLine = strText;

    // Find the end of the first line
    do {
        iIndex = strLine.Find(_T("\n"));

        // Store the remaining chars after the newline
        strNext = (iIndex < 0)? _T("")  : strLine.Mid(iIndex+1);

        // Remove all chars after the newline
        if (iIndex >= 0)
            strLine = strLine.Left(iIndex);

        // Make blank entries a "space"
        if (strLine.IsEmpty() && iIndex >= 0)
            strLine = _T(" ");

        pszLine = strLine.GetBuffer(1);

        // Break the current line into tokens (tab or comma delimited)
        pszCellText = _tcstok(pszLine, _T("\t,\n"));
        while (pszCellText != NULL)
        {
            ciTarget = CCellID(ciCell.m_iRow + iLine, ciCell.m_iCol + iColumn);
            pgcCell  = GetCell(ciCell.m_iRow + iLine, ciCell.m_iCol + iColumn);//added for combine
            if (IsValid(ciTarget) && !pgcCell->m_bNoEdit)//added (&&!pgcCell->m_bNoEdit)for combine
            {
                strCellText = pszCellText;
                strCellText.TrimLeft();
                strCellText.TrimRight();

                SendMessageToParent(ciTarget.m_iRow, ciTarget.m_iCol, GVN_BEGINLABELEDIT);
                SetItemText(ciTarget.m_iRow, ciTarget.m_iCol, strCellText);
                SetModified(TRUE, ciTarget.m_iRow, ciTarget.m_iCol);
                SendMessageToParent(ciTarget.m_iRow, ciTarget.m_iCol, GVN_ENDLABELEDIT);

                // Make sure cell is not selected to avoid data loss
                SetItemState(ciTarget.m_iRow, ciTarget.m_iCol,
                             GetItemState(ciTarget.m_iRow, ciTarget.m_iCol) & ~GVIS_SELECTED);
            }

            pszCellText = _tcstok(NULL, _T("\t,\n"));
            iColumn++;
        }

        strLine.ReleaseBuffer();
        strLine = strNext;
        iLine++;

    } while (iIndex >= 0);

    strText.UnlockBuffer();
    return;
}

BOOL CGridCtrl::PasteTextToGrid(CCellID ciCell, COleDataObject* pDataObject)
{
    HGLOBAL hMem     = NULL;
    LPSTR   szBuffer = NULL;
    CString strText;

    COMP_BFALSE_R(IsValid(ciCell), FALSE);
    COMP_BFALSE_R(IsCellEditable(ciCell), FALSE);
    ASSERT_NULL_R(pDataObject, FALSE);
    COMP_BFALSE_R(pDataObject->IsDataAvailable(CF_TEXT), FALSE);

    // Get the text from the COleDataObject
    hMem = pDataObject->GetGlobalData(CF_TEXT);
    ASSERT_NULL_R(hMem, FALSE);
    CMemFile sf((BYTE*) ::GlobalLock(hMem), ::GlobalSize(hMem));

    // CF_TEXT is ANSI text, so we need to allocate a char* buffer
    // to hold this.
    szBuffer = new char[::GlobalSize(hMem)];
    ASSERT_NULL_R(szBuffer, FALSE);

    sf.Read(szBuffer, ::GlobalSize(hMem));
    ::GlobalUnlock(hMem);

    // Now store in generic TCHAR form so we no longer have to deal with
    // ANSI/UNICODE problems
    strText = szBuffer;
    delete szBuffer;

    // Parse text data and set in cells...
    SetTextInCell(ciCell, strText);

    Invalidate();

    return TRUE;
}
#endif

#ifndef GRIDCONTROL_NO_DRAGDROP

void CGridCtrl::OnBeginDrag()
{
    COleDataSource *pSource = NULL;
    DROPEFFECT      dropEffect;

    COMP_BFALSE(m_bAllowDragAndDrop);

    pSource = CopyTextFromGrid();
    ASSERT_NULL(pSource);

    SendMessageToParent(GetSelectedCellRange().GetTopLeft().m_iRow,
                        GetSelectedCellRange().GetTopLeft().m_iCol,
                        GVN_BEGINDRAG);

    m_iMouseMode = MOUSE_DRAGGING;
    dropEffect = pSource->DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE);

    if (dropEffect & DROPEFFECT_MOVE)
        CutSelectedText();

    delete pSource;    // Did not pass source to clipboard, so must delete
}

DROPEFFECT CGridCtrl::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState,
                                 CPoint ptOpr)
{
    CCellID ciCell;
    UINT    nState  = 0;

    // Any text data available for us?
    COMP_BFALSE_R(m_bAllowDragAndDrop, DROPEFFECT_NONE);
    COMP_BFALSE_R(IsEditable(),        DROPEFFECT_NONE);
    ASSERT_NULL_R(pDataObject, DROPEFFECT_NONE);
    COMP_BFALSE_R(pDataObject->IsDataAvailable(CF_TEXT), DROPEFFECT_NONE);

    // Find which cell we are over and drop-highlight it
    ciCell = GetCellFromPt(ptOpr, FALSE);

    // If not valid, set the previously drop-highlighted cell as no longer drop-highlighted
    if (!IsValid(ciCell))
    {
        OnDragLeave();
        m_ciLastDragOver = CCellID(-1,-1);
        return DROPEFFECT_NONE;
    }

    COMP_BFALSE_R(IsCellEditable(ciCell), DROPEFFECT_NONE);

    // Have we moved over a different cell than last time?
    if (ciCell != m_ciLastDragOver)
    {
        // Set the previously drop-highlighted cell as no longer drop-highlighted
        if (IsValid(m_ciLastDragOver))
        {
            nState = GetItemState(m_ciLastDragOver.m_iRow, m_ciLastDragOver.m_iCol);
            SetItemState(m_ciLastDragOver.m_iRow, m_ciLastDragOver.m_iCol,
                         nState & ~GVIS_DROPHILITED);
            RedrawCell(m_ciLastDragOver);
        }

        m_ciLastDragOver = ciCell;

        // Set the new cell as drop-highlighted
        if (IsValid(m_ciLastDragOver))
        {
            nState = GetItemState(m_ciLastDragOver.m_iRow, m_ciLastDragOver.m_iCol);
            SetItemState(m_ciLastDragOver.m_iRow, m_ciLastDragOver.m_iCol,
                         nState | GVIS_DROPHILITED);
            RedrawCell(m_ciLastDragOver);
        }
    }

    // Return an appropraite value of DROPEFFECT so mouse cursor is set properly
    COMP_BTRUE_R(dwKeyState & MK_CONTROL, DROPEFFECT_COPY)

    /* else */
    return DROPEFFECT_MOVE;
}

DROPEFFECT CGridCtrl::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState,
                                  CPoint ptOpr)
{
    UINT    nState  = 0;

    // Any text data available for us?
    COMP_BFALSE_R(m_bAllowDragAndDrop, DROPEFFECT_NONE);
    ASSERT_NULL_R(pDataObject, DROPEFFECT_NONE);
    COMP_BFALSE_R(pDataObject->IsDataAvailable(CF_TEXT), DROPEFFECT_NONE);

    // Find which cell we are over and drop-highlight it
    m_ciLastDragOver = GetCellFromPt(ptOpr, FALSE);
    COMP_BFALSE_R(IsValid(m_ciLastDragOver), DROPEFFECT_NONE);

    COMP_BFALSE_R(IsCellEditable(m_ciLastDragOver), DROPEFFECT_NONE);

    if (IsValid(m_ciLastDragOver))
    {
        nState = GetItemState(m_ciLastDragOver.m_iRow, m_ciLastDragOver.m_iCol);
        SetItemState(m_ciLastDragOver.m_iRow, m_ciLastDragOver.m_iCol,
                     nState | GVIS_DROPHILITED);
        RedrawCell(m_ciLastDragOver);
    }

    // Return an appropraite value of DROPEFFECT so mouse cursor is set properly
    COMP_BTRUE_R(dwKeyState & MK_CONTROL, DROPEFFECT_COPY)

    /* else */
    return DROPEFFECT_MOVE;
}

void CGridCtrl::OnDragLeave()
{
    UINT    nState  = 0;

    COMP_BFALSE(IsValid(m_ciLastDragOver));

    // Set the previously drop-highlighted cell as no longer drop-highlighted
    nState = GetItemState(m_ciLastDragOver.m_iRow, m_ciLastDragOver.m_iCol);
    SetItemState(m_ciLastDragOver.m_iRow, m_ciLastDragOver.m_iCol,
                 nState & ~GVIS_DROPHILITED);
    RedrawCell(m_ciLastDragOver);
}

BOOL CGridCtrl::OnDrop(COleDataObject* pDataObject, DROPEFFECT /*dropEffect*/,
                       CPoint /* ptOpr */)
{
    COMP_BFALSE_R(m_bAllowDragAndDrop, FALSE);
    COMP_BFALSE_R(IsCellEditable(m_ciLastDragOver), FALSE);

    m_iMouseMode = MOUSE_NOTHING;
    OnDragLeave();

    return PasteTextToGrid(m_ciLastDragOver, pDataObject);
}
#endif

#ifndef GRIDCONTROL_NO_CLIPBOARD
void CGridCtrl::OnEditCut()
{
    COleDataSource *pSource = NULL;

    COMP_BFALSE(IsEditable());

    pSource = CopyTextFromGrid();
    ASSERT_NULL(pSource);

    pSource->SetClipboard();
    CutSelectedText();
}

void CGridCtrl::OnEditCopy()
{
    COleDataSource* pSource = NULL;

    pSource = CopyTextFromGrid();
    ASSERT_NULL(pSource);

    pSource->SetClipboard();
}

void CGridCtrl::OnEditPaste()
{
    CCellID ciCell;
    COleDataObject tObject;

    COMP_BFALSE(IsEditable());

    // Get the Focus cell, or if none, get the topleft (non-fixed) cell
    ciCell = GetFocusCell();
    if (!IsValid(ciCell))
        ciCell = GetTopleftNonFixedCell();

    COMP_BFALSE(IsValid(ciCell));

    // Attach a COleDataObject to the clipboard and paste the data to the grid
    if (tObject.AttachClipboard())
        PasteTextToGrid(ciCell, &tObject);
}
#endif

void CGridCtrl::OnEditSelectAll()
{
    SelectAllCells();
}

#ifndef GRIDCONTROL_NO_CLIPBOARD
void CGridCtrl::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
    CCellRange crSelection;

    ASSERT_NULL(pCmdUI);

    crSelection = GetSelectedCellRange();
    pCmdUI->Enable(crSelection.Count() && IsValid(crSelection));
}

void CGridCtrl::OnUpdateEditCut(CCmdUI* pCmdUI)
{
    CCellRange crSelection;

    ASSERT_NULL(pCmdUI);

    crSelection = GetSelectedCellRange();
    pCmdUI->Enable(IsEditable() && crSelection.Count() && IsValid(crSelection));
}

void CGridCtrl::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
    CCellID ciCell;
    BOOL    bCanPaste = FALSE;

    ASSERT_NULL(pCmdUI);

    ciCell = GetFocusCell();

    bCanPaste = IsValid(ciCell)         &&
                IsCellEditable(ciCell)  &&
                ::IsClipboardFormatAvailable(CF_TEXT);

    pCmdUI->Enable(bCanPaste);
}
#endif

void CGridCtrl::OnUpdateEditSelectAll(CCmdUI* pCmdUI)
{
    ASSERT_NULL(pCmdUI);

    pCmdUI->Enable(m_bEnableSelection);
}

////////////////////////////////////////////////////////////////////////////////////////
// hittest-like functions

BOOL CGridCtrl::MouseOverRowResizeArea(CPoint& ptOpr) const
{
    CCellID     ciCurrent;
    CPoint      ptStart;
    int         iEndY       = 0;
    CGridCell   *pgcCell    = NULL;
    CGridCell   *pgcTopLeft = NULL;

    COMP_BLE_R(ptOpr.x, GetFixedColumnWidth(), FALSE);

    ciCurrent = GetCellFromPt(ptOpr);

    //want remove it?I'm not satisfied with this change.as if existing bug!
    pgcCell = GetCell(ciCurrent.m_iRow,ciCurrent.m_iCol);
    ASSERT_NULL_R(pgcCell, FALSE);

    //the source code existing bug,adjust it!
    COMP_BFALSE_R(IsValid(ciCurrent.m_iRow,ciCurrent.m_iCol), FALSE);

    if(pgcCell->m_bHide==TRUE)
    {
        pgcTopLeft = GetCell(pgcCell->m_iTopleftRow,pgcCell->m_iTopleftCol);
        ASSERT_NULL_R(pgcTopLeft, FALSE);

        if(pgcTopLeft->m_iNumHhide == 0)
            ciCurrent.m_iCol = 0;
        if(pgcTopLeft->m_iNumVhide == 0)
            ciCurrent.m_iRow = 0;
    }

    //added for combine the fixdecell;
    if (!GetCellOrigin(ciCurrent, &ptStart))
        return FALSE;

    iEndY = ptStart.y + GetRowHeight(ciCurrent.m_iRow);

    COMP_BTRUE_R((ptOpr.y - ptStart.y <= m_iResizeCaptureRange && ciCurrent.m_iRow != 0), TRUE);
    COMP_BLE_R((iEndY - ptOpr.y), m_iResizeCaptureRange, TRUE);

    return FALSE;
}

BOOL CGridCtrl::MouseOverColumnResizeArea(CPoint& ptOpr) const
{
    CCellID     ciCurrent;
    CPoint      ptStart;
    int         iEndX       = 0;
    CGridCell   *pgcCell    = NULL;
    CGridCell   *pgcTopLeft = NULL;

    COMP_BLE_R(ptOpr.y, GetFixedRowHeight(), FALSE);

    ciCurrent = GetCellFromPt(ptOpr);
    //want remove it?I'm not satisfied with this change.as if existing bug!
    pgcCell = GetCell(ciCurrent.m_iRow,ciCurrent.m_iCol);
    ASSERT_NULL_R(pgcCell, FALSE);

    //the source code existing bug,adjust it!
    COMP_BFALSE_R(IsValid(ciCurrent.m_iRow,ciCurrent.m_iCol), FALSE);
        
    if(pgcCell->m_bHide==TRUE)
    {
        pgcTopLeft = GetCell(pgcCell->m_iTopleftRow,pgcCell->m_iTopleftCol);
        ASSERT_NULL_R(pgcTopLeft, FALSE);

        if(pgcTopLeft->m_iNumHhide == 0)
            ciCurrent.m_iCol = 0;
        if(pgcTopLeft->m_iNumVhide == 0)
            ciCurrent.m_iRow = 0;
    }

    //added for combine the fixdecell;
    if (!GetCellOrigin(ciCurrent, &ptStart))
        return FALSE;

    iEndX = ptStart.x + GetColumnWidth(ciCurrent.m_iCol);

    COMP_BTRUE_R((ptOpr.x - ptStart.x <= m_iResizeCaptureRange && ciCurrent.m_iCol != 0), TRUE);
    COMP_BLE_R((iEndX - ptOpr.x), m_iResizeCaptureRange, TRUE);

    return FALSE;
}

// calculate column index
int     CGridCtrl::GetCellColFromPt(CPoint ptOpr, BOOL bAllowFixedCellCheck /*=TRUE*/) const
{
    int     iCol      = 0;
    int     iXPos     = 0;
    int     iFixedColWidth  = 0;
    CCellID ciTopLeft;

    ciTopLeft = GetTopleftNonFixedCell();

    iFixedColWidth = GetFixedColumnWidth();

    // not in window
    COMP_BL_R(ptOpr.x, 0, -1);

    // in fixed col
    if (ptOpr.x < iFixedColWidth) 
    {
        // not in window
        COMP_BFALSE_R(bAllowFixedCellCheck, -1);

        iXPos = 0;
        for (iCol = 0; iCol < m_iFixedCols; iCol++)
        {
            iXPos += GetColumnWidth(iCol);
            COMP_BG_R(iXPos, ptOpr.x, iCol);
        }
        return m_iFixedCols;
    }
    // in non-fixed col
    iXPos = iFixedColWidth;
    for ( iCol = ciTopLeft.m_iCol; iCol < GetColumnCount(); iCol++)
    {
        iXPos += GetColumnWidth(iCol);
        COMP_BG_R(iXPos, ptOpr.x, iCol);
    }

    return -1;
}

// calculate row index
int     CGridCtrl::GetCellRowFromPt(CPoint ptOpr, BOOL bAllowFixedCellCheck /*=TRUE*/) const
{
    int     iRow      = 0;
    int     iYPos     = 0;
    int     iFixedRowHeight  = 0;
    CCellID ciTopLeft;

    ciTopLeft = GetTopleftNonFixedCell();

    iFixedRowHeight = GetFixedRowHeight();

    // not in window
    COMP_BL_R(ptOpr.y, 0, -1);
    
    // in fixed col
    if (ptOpr.y < iFixedRowHeight)
    {
        // not in window
        COMP_BFALSE_R(bAllowFixedCellCheck, -1);

        iYPos = 0;
        for (iRow = 0; iRow < m_iFixedRows; iRow++)
        {
            iYPos += GetRowHeight(iRow);
            COMP_BG_R(iYPos, ptOpr.y, iRow);
        }
        return m_iFixedRows;
    }

    iYPos = iFixedRowHeight;
    for ( iRow = ciTopLeft.m_iRow; iRow < GetRowCount(); iRow++)
    {
        iYPos += GetRowHeight(iRow);
        COMP_BG_R(iYPos, ptOpr.y, iRow);
    }

    return -1;
}

// Get cell from point
CCellID CGridCtrl::GetCellFromPt(CPoint ptOpr, BOOL bAllowFixedCellCheck /*=TRUE*/) const
{
    CCellID ciOpr;

    // calculate column index
    ciOpr.m_iCol = GetCellColFromPt(ptOpr, bAllowFixedCellCheck);

    // calculate row index
    ciOpr.m_iRow = GetCellRowFromPt(ptOpr, bAllowFixedCellCheck);

    return ciOpr;
}

////////////////////////////////////////////////////////////////////////////////
// CGridCtrl cellrange functions

CCellID CGridCtrl::GetTopleftNonFixedCell() const
{
    int iVertScroll = 0;
    int iHorzScroll = 0;
    int iCol        = 0;
    int iRight      = 0;
    int iRow        = 0;
    int iTop        = 0;

    iVertScroll = GetScrollPos(SB_VERT);
    iHorzScroll = GetScrollPos(SB_HORZ);

    iCol = m_iFixedCols;
    while (iRight < iHorzScroll && iCol < (GetColumnCount()-1))
        iRight += GetColumnWidth(iCol++);

    iRow = m_iFixedRows;
    while (iTop < iVertScroll && iRow < (GetRowCount()-1))
        iTop += GetRowHeight(iRow++);

    //TRACE("TopLeft cell is row %d, col %d\n",iRow, nColumn);
    return CCellID(iRow, iCol);
}

// This gets even partially visible cells
CCellRange CGridCtrl::GetVisibleNonFixedCellRange(LPRECT pRect /*=NULL*/) const
{
    int     i           = 0;
    int     iBottom     = 0;
    int     iRight      = 0;
    int     iMaxVisRow  = 0;
    int     iMaxVisCol  = 0;
    CRect   rcTmp;
    CCellID ciTopLeft;

    GetClientRect(&rcTmp);

    ciTopLeft = GetTopleftNonFixedCell();

    // calc bottom
    iBottom = GetFixedRowHeight();
    for (i = ciTopLeft.m_iRow; i < GetRowCount(); i++)
    {
        iBottom += GetRowHeight(i);
        if (iBottom >= rcTmp.bottom)
        {
            iBottom = rcTmp.bottom;
            break;
        }
    }
    iMaxVisRow = min(i, GetRowCount() - 1);

    // calc right
    iRight = GetFixedColumnWidth();
    for (i = ciTopLeft.m_iCol; i < GetColumnCount(); i++)
    {
        iRight += GetColumnWidth(i);
        if (iRight >= rcTmp.right)
        {
            iRight = rcTmp.right;
            break;
        }
    }
    iMaxVisCol = min(i, GetColumnCount() - 1);

    if (pRect)
    {
        pRect->left   = 0;
        pRect->top    = 0;
        pRect->right  = iRight;
        pRect->bottom = iBottom;
    }

    return CCellRange(ciTopLeft.m_iRow, ciTopLeft.m_iCol, iMaxVisRow, iMaxVisCol);
}

// used by ResetScrollBars() - This gets only fully visible cells
CCellRange CGridCtrl::GetUnobstructedNonFixedCellRange() const
{
    int     i           = 0;
    int     iBottom     = 0;
    int     iRight      = 0;
    int     iMaxVisRow  = 0;
    int     iMaxVisCol  = 0;
    CRect   rcTmp;
    CCellID ciTopLeft;
    GetClientRect(&rcTmp);

    ciTopLeft = GetTopleftNonFixedCell();

    // calc bottom
    iBottom = GetFixedRowHeight();
    for (i = ciTopLeft.m_iRow; i < GetRowCount(); i++)
    {
        iBottom += GetRowHeight(i);
        if (iBottom >= rcTmp.bottom)
            break;
    }
    iMaxVisRow = min(i, GetRowCount() - 1);
    if (iMaxVisRow > 0 && iBottom > rcTmp.bottom)
        iMaxVisRow--;

    // calc right
    iRight = GetFixedColumnWidth();
    for (i = ciTopLeft.m_iCol; i < GetColumnCount(); i++)
    {
        iRight += GetColumnWidth(i);
        if (iRight >= rcTmp.right)
            break;
    }
    iMaxVisCol = min(i, GetColumnCount() - 1);
    if (iMaxVisCol > 0 && iRight > rcTmp.right)
        iMaxVisCol--;

    return CCellRange(ciTopLeft.m_iRow, ciTopLeft.m_iCol, iMaxVisRow, iMaxVisCol);
}

// Returns the minimum bounding range of the current selection
// If no selection, then the returned CCellRange will be invalid
CCellRange CGridCtrl::GetSelectedCellRange() const
{
    POSITION    pos;
    DWORD       dwKey   = 0;
    CCellID     ciCell;
    CCellRange  crSelection;

    crSelection = CCellRange(GetRowCount(), GetColumnCount(), -1,-1);

    for (pos = m_tSelectedCellMap.GetStartPosition(); pos != NULL; )
    {
        m_tSelectedCellMap.GetNextAssoc(pos, dwKey, (CCellID&)ciCell);

        crSelection.SetMinRow( min(crSelection.GetMinRow(), ciCell.m_iRow) );
        crSelection.SetMinCol( min(crSelection.GetMinCol(), ciCell.m_iCol) );
        crSelection.SetMaxRow( max(crSelection.GetMaxRow(), ciCell.m_iRow) );
        crSelection.SetMaxCol( max(crSelection.GetMaxCol(), ciCell.m_iCol) );
    }

    return crSelection;
}

// Returns ALL the cells in the grid
CCellRange CGridCtrl::GetCellRange() const
{
    return CCellRange(0, 0, GetRowCount() - 1, GetColumnCount() - 1);
}

void CGridCtrl::ResetSelectedRange()
{
    SetSelectedRange(-1,-1,-1,-1);
    SetFocusCell(-1,-1);
}

// Get/Set scroll position using 32 bit functions
int CGridCtrl::GetScrollPos32(int nBar, BOOL bGetTrackPos /* = FALSE */)
{
    SCROLLINFO si;

    si.cbSize = sizeof(SCROLLINFO);

    if (bGetTrackPos)
    {
        if (GetScrollInfo(nBar, &si, SIF_TRACKPOS))
            return si.nTrackPos;
    }

    /* else */
    if (GetScrollInfo(nBar, &si, SIF_POS))
        return si.nPos;

    return 0;
}

BOOL CGridCtrl::SetScrollPos32(int nBar, int nPos, BOOL bRedraw /* = TRUE */)
{
    SCROLLINFO si;

    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_POS;
    si.nPos   = nPos;
    return SetScrollInfo(nBar, &si, bRedraw);
}

void CGridCtrl::ResetScrollBars()
{
    CRect       rcTmp;
    CRect       rcVisible;
    CRect       rcVirtual;
    CCellRange  crVisibles;
    SCROLLINFO  si;

    COMP_BFALSE(m_bAllowDraw);
    COMP_BFALSE(::IsWindow(GetSafeHwnd()));

    GetClientRect(&rcTmp);
    rcTmp.left  += GetFixedColumnWidth();
    rcTmp.top   += GetFixedRowHeight();
    COMP_BGE(rcTmp.left, rcTmp.right);
    COMP_BGE(rcTmp.top,  rcTmp.bottom);

    rcVisible.SetRect(GetFixedColumnWidth(), GetFixedRowHeight(), rcTmp.right, rcTmp.bottom);
    rcVirtual.SetRect(GetFixedColumnWidth(), GetFixedRowHeight(), GetVirtualWidth(), GetVirtualHeight());

    crVisibles = GetUnobstructedNonFixedCellRange();
    COMP_BFALSE(IsValid(crVisibles));

    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_PAGE;
    si.nPage  = rcVisible.Width();    SetScrollInfo(SB_HORZ, &si, FALSE);
    si.nPage  = rcVisible.Height();   SetScrollInfo(SB_VERT, &si, FALSE);

    m_iVScrollMax = 0;
    if (rcVisible.Height() < rcVirtual.Height())
        m_iVScrollMax = rcVirtual.Height()-1; //+ GetRowHeight(visibleCells.GetTopLeft().row);
        
    m_iHScrollMax = 0;
    if (rcVisible.Width() < rcVirtual.Width())
        m_iHScrollMax = rcVirtual.Width()-1; //+ GetColumnWidth(visibleCells.GetTopLeft().col);

    ASSERT(m_iVScrollMax < INT_MAX && m_iHScrollMax < INT_MAX); // This should be fine :)
    SetScrollRange(SB_VERT, 0, m_iVScrollMax, TRUE);
    SetScrollRange(SB_HORZ, 0, m_iHScrollMax, TRUE);
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column position functions
int  CGridCtrl::GetCellOriginLeft(int iCol) const
{
    int     i        = 0;
    int     iLeftPos = 0;
    CCellID ciTopLeft;

    COMP_BL_R (iCol, 0, -1);
    COMP_BGE_R(iCol, m_iCols, -1);

    // is a fixed column
    if (iCol < m_iFixedCols)
    {
        for (i = 0; i < iCol; i++)
            iLeftPos += GetColumnWidth(i);
        return iLeftPos;
    }

    // is a scrollable data column
    ciTopLeft = GetTopleftNonFixedCell();
    COMP_BL_R(iCol, ciTopLeft.m_iCol, -1);

    for (i = 0; i < m_iFixedCols; i++)
        iLeftPos += GetColumnWidth(i);
    for (i = ciTopLeft.m_iCol; i < iCol; i++)
        iLeftPos += GetColumnWidth(i);
    return iLeftPos;
}

int  CGridCtrl::GetCellOriginTop (int iRow) const
{
    int     i        = 0;
    int     iTopPos  = 0;
    CCellID ciTopLeft;

    COMP_BL_R (iRow, 0, -1);
    COMP_BGE_R(iRow, m_iRows, -1);

    // is a fixed row
    if (iRow < m_iFixedRows)
    {
        for (i = 0; i < iRow; i++)
            iTopPos += GetRowHeight(i);
        return iTopPos;
    }

    // is a scrollable data row
    ciTopLeft = GetTopleftNonFixedCell();
    COMP_BL_R(iRow, ciTopLeft.m_iRow, -1);

    for (i = 0; i < m_iFixedRows; i++)
        iTopPos += GetRowHeight(i);
    for (i = ciTopLeft.m_iRow; i < iRow; i++)
        iTopPos += GetRowHeight(i);

    return iTopPos;
}

// returns the top left point of the cell. Returns FALSE if cell not visible.
BOOL CGridCtrl::GetCellOrigin(int iRow, int iCol, LPPOINT pPoint) const
{
    int     iLeftPos = 0;
    int     iTopPos  = 0;

    ASSERT_NULL_R(pPoint, FALSE);

    iLeftPos = GetCellOriginLeft(iCol);
    iTopPos  = GetCellOriginTop (iRow);
    COMP_BL_R(iLeftPos, 0, FALSE);
    COMP_BL_R(iTopPos,  0, FALSE);

    pPoint->x = iLeftPos;
    pPoint->y = iTopPos;

    return TRUE;
}

BOOL CGridCtrl::GetCellOrigin(const CCellID& ciCell, LPPOINT pPoint) const
{
    return GetCellOrigin(ciCell.m_iRow, ciCell.m_iCol, pPoint);
}

// Returns the bounding box of the cell
BOOL CGridCtrl::GetCellRect(const CCellID& ciCell, LPRECT pRect) const
{
    return GetCellRect(ciCell.m_iRow, ciCell.m_iCol, pRect);
}

BOOL CGridCtrl::GetCellRect(int iRow, int iCol, LPRECT pRect) const
{
    CPoint ptCellOrigin;

    COMP_BFALSE_R(GetCellOrigin(iRow, iCol, &ptCellOrigin), FALSE);

    pRect->left   = ptCellOrigin.x;
    pRect->top    = ptCellOrigin.y;
    pRect->right  = ptCellOrigin.x + GetColumnWidth(iCol)-1;
    pRect->bottom = ptCellOrigin.y + GetRowHeight(iRow)-1;

    //TRACE("Row %d, col %d: L %d, T %d, W %d, H %d:  %d,%d - %d,%d\n",
    //      iRow,iCol, CellOrigin.x, CellOrigin.y, GetColumnWidth(iCol), GetRowHeight(iRow),
    //      pRect->left, pRect->top, pRect->right, pRect->bottom);

    return TRUE;
}

BOOL CGridCtrl::GetTextRect(const CCellID& ciCell, LPRECT pRect)
{
    return GetTextRect(ciCell.m_iRow, ciCell.m_iCol, pRect);
}

BOOL CGridCtrl::GetTextRect(int iRow, int iCol, LPRECT pRect)
{
    GV_ITEM     tItem;
    IMAGEINFO   tInfo;
    int         iImageWidth = 0;

    tItem.nMask = GVIF_IMAGE;
    tItem.iRow = iRow;
    tItem.iCol = iCol;
    COMP_BFALSE_R(GetItem(&tItem), FALSE);

    COMP_BFALSE_R(GetCellRect(iRow, iCol, pRect), FALSE);

    //pRect->left += m_iMargin;
    //pRect->right -= m_iMargin;

    ASSERT_NULL_R(m_pImageList, TRUE);
    COMP_BL_R(tItem.iImage, 0, TRUE);

    if (m_pImageList->GetImageInfo(tItem.iImage, &tInfo))
    {
        iImageWidth  = tInfo.rcImage.right-tInfo.rcImage.left+1;
        pRect->left += iImageWidth+m_iMargin;
    }

    return TRUE;
}

// Returns the bounding box of a range of cells
BOOL CGridCtrl::GetCellRangeRect(const CCellRange& crRange, LPRECT lpRect) const
{
    CPoint  ptMinOrigin;
    CPoint  ptMaxOrigin;

    COMP_BFALSE_R(GetCellOrigin(crRange.GetMinRow(), crRange.GetMinCol(), &ptMinOrigin),
                  FALSE);
    COMP_BFALSE_R(GetCellOrigin(crRange.GetMaxRow(), crRange.GetMaxCol(), &ptMaxOrigin),
                  FALSE);

    lpRect->left   = ptMinOrigin.x;
    lpRect->top    = ptMinOrigin.y;
    lpRect->right  = ptMaxOrigin.x + GetColumnWidth(crRange.GetMaxCol()-1);
    lpRect->bottom = ptMaxOrigin.y + GetRowHeight(crRange.GetMaxRow()-1);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// Grid attribute functions

LRESULT CGridCtrl::OnSetFont(WPARAM hFont, LPARAM /*lParam */)
{
    int     iRow        = 0;
    int     iCol        = 0;
    CDC     *pDC        = NULL;
    CFont   *pOldFont   = NULL;
    LRESULT lResult;
    LOGFONT lf;
    TEXTMETRIC tm;

    lResult = Default();

    // Get the logical font
    COMP_BFALSE_R(GetObject((HFONT) hFont, sizeof(LOGFONT), &lf), lResult);

    // Store font as the global default
    memcpy(&m_Logfont, &lf, sizeof(LOGFONT));

    // reset all cells' fonts
    for (iRow = 0; iRow < GetRowCount(); iRow++)
        for (iCol = 0; iCol < GetColumnCount(); iCol++)
            SetItemFont(iRow, iCol, &lf);

    // Get the font size and hence the default cell size
    pDC = GetDC();
    if (pDC)
    {
        m_ftGrid.DeleteObject();
        m_ftGrid.CreateFontIndirect(&m_Logfont);
        pOldFont = pDC->SelectObject(&m_ftGrid);

        pDC->GetTextMetrics(&tm);

        m_iMargin = pDC->GetTextExtent(_T(" "),1).cx;
        pDC->SelectObject(pOldFont);
        ReleaseDC(pDC);

        m_iDefCellHeight = tm.tmHeight+tm.tmExternalLeading + 2*m_iMargin;
        m_iDefCellWidth  = tm.tmAveCharWidth*12 + 2*m_iMargin;
    }

    if (::IsWindow(GetSafeHwnd()))
        Invalidate();

    return lResult;
}

LRESULT CGridCtrl::OnGetFont(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    return (LRESULT) (HFONT) m_ftGrid;
}

#ifndef _WIN32_WCE_NO_CURSOR
BOOL CGridCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (nHitTest == HTCLIENT)
    {
        switch (m_iMouseMode)
        {
            case MOUSE_OVER_COL_DIVIDE:
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
                break;

            case MOUSE_OVER_ROW_DIVIDE:
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
                break;

#ifndef GRIDCONTROL_NO_DRAGDROP
            case MOUSE_DRAGGING:
                break;
#endif
            default:
            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        }
        return TRUE;
    }

    return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
#endif

////////////////////////////////////////////////////////////////////////////////////
// Row/Column count functions

BOOL CGridCtrl::SetFixedRowCount(int iFixedRows)
{
    ASSERT(iFixedRows >= 0);

    if (iFixedRows > GetRowCount())
    {
        COMP_BFALSE_R(SetRowCount(iFixedRows), FALSE);
    }

    if (m_ciCurrent.m_iRow < iFixedRows)
        SetFocusCell(-1,-1);

    m_iFixedRows = iFixedRows;

    if (GetSafeHwnd() && m_bAllowDraw)
        Invalidate();

    return TRUE;
}

BOOL CGridCtrl::SetFixedColumnCount(int iFixedCols)
{
    ASSERT(iFixedCols >= 0);
    if (iFixedCols > GetColumnCount())
    {
        COMP_BFALSE_R(SetColumnCount(iFixedCols), FALSE);
    }

    if (m_ciCurrent.m_iCol < iFixedCols)
        SetFocusCell(-1,-1);

    m_iFixedCols = iFixedCols;

    if (GetSafeHwnd() && m_bAllowDraw)
        Invalidate();

    return TRUE;
}

void CGridCtrl::DelLosRow(int iDelRow)
{
    int         iRow     = 0;
    int         iCol     = 0;
    CGridCell   *pgcCell = NULL;
    GRID_ROW    *pRow    = NULL;
    
    for (iRow = iDelRow; iRow < m_iRows; iRow++)
    {
        // Delete cells
        for (iCol = 0; iCol < m_iCols; iCol++)
        {
            pgcCell = GetCell(iRow, iCol);
            if (pgcCell)
            {
                EmptyCell(pgcCell, iRow, iCol);
                delete pgcCell;
            }
        }

        // Delete rows
        pRow = m_tRowData[iRow];
        DELETE_POINT(pRow);
    }
}

void CGridCtrl::InsertNewRow(int iStartRow)
{
    int         iRow     = 0;
    int         iCol     = 0;

    COMP_BL(iStartRow, 0);

    // initialize row heights and data
    for (iRow = iStartRow; iRow < GetRowCount(); iRow++)
    {
        m_arRowHeights[iRow] = m_iDefCellHeight;
        m_tRowData[iRow] = new GRID_ROW;
        COMP_BNULL_CONTINUE(m_tRowData[iRow]);

        m_tRowData[iRow]->SetSize(m_iCols);
        for (iCol = 0; iCol < m_iCols; iCol++)
        {
            m_tRowData[iRow]->SetAt(iCol, CreateCell(iRow, iCol));
        }
    }
}

BOOL CGridCtrl::SetRowCount(int iRows)
{
    int     i           = 0;
    int     iRow        = 0;
    int     iCol        = 0;
    int     iAddedRows  = 0;

    COMP_BL_R(iRows, 0, FALSE);
    COMP_BE_R(iRows, GetRowCount(), TRUE);

    if (iRows < m_iFixedRows)
        m_iFixedRows = iRows;

    if (m_ciCurrent.m_iRow >= iRows)
        SetFocusCell(-1,-1);

    iAddedRows = iRows - GetRowCount();

    // If we are about to lose rows, then we need to delete the GridCell objects
    // in each column within each row
    if (iAddedRows < 0)
    {
        DelLosRow(iRows);
    }

    // Change the number of rows.
    m_iRows = iRows;
    m_tRowData.SetSize(m_iRows);
    m_arRowHeights.SetSize(iRows);

    // If we have just added rows, we need to construct new elements for each cell
    // and set the default row height
    if (iAddedRows > 0)
    {
        InsertNewRow(iRows - iAddedRows);
    }
    //else
    //    ResetSelectedRange();

    if (GetSafeHwnd() && m_bAllowDraw)
    {
        ResetScrollBars();
        Invalidate();
    }
    return TRUE;
}

void CGridCtrl::DelLosColumn(int iDelCol)
{
    int         iRow     = 0;
    int         iCol     = 0;
    CGridCell   *pgcCell = NULL;
    
    for ( iRow = 0; iRow < m_iRows; iRow++)
        for ( iCol = iDelCol; iCol < GetColumnCount(); iCol++)
        {
            pgcCell = GetCell(iRow, iCol);
            if (pgcCell)
            {
                EmptyCell(pgcCell, iRow, iCol);
                delete pgcCell;
            }
        }

    return;
}

void CGridCtrl::InsertNewColumn(int iStartCol)
{
    int         iRow    = 0;
    int         iCol    = 0;
    GRID_ROW    *pRow   = NULL;

    COMP_BL(iStartCol, 0);

    // initialized column widths
    //iStartCol = iColNum - iAddCols;
    for ( iCol = iStartCol; iCol < GetColumnCount(); iCol++)
        m_arColWidths[iCol] = m_iDefCellWidth;

    // initialise column data
    for (iRow = 0; iRow < m_iRows; iRow++)
        for (iCol = iStartCol; iCol < GetColumnCount(); iCol++)
        {
            pRow = m_tRowData[iRow];
            if (pRow)
                pRow->SetAt(iCol, CreateCell(iRow,iCol));
        }
}

BOOL CGridCtrl::SetColumnCount(int iCols)
{
    int     i           = 0;
    int     iRow        = 0;
    int     iCol        = 0;
    int     iAddedCols  = 0;

    COMP_BL_R(iCols, 0, FALSE);
    COMP_BE_R(iCols, GetColumnCount(), TRUE);

    if (iCols < m_iFixedCols)
        m_iFixedCols = iCols;

    if (m_ciCurrent.m_iCol >= iCols)
        SetFocusCell(-1,-1);

    iAddedCols = iCols - GetColumnCount();

    // If we are about to lose columns, then we need to delete the GridCell objects
    // within each column
    if (iAddedCols < 0)
        DelLosColumn(iCols);

    // Change the number of columns.
    m_iCols = iCols;
    m_arColWidths.SetSize(iCols);

    // Change the number of columns in each row.
    for (i = 0; i < m_iRows; i++)
    {
        if (m_tRowData[i])
            m_tRowData[i]->SetSize(iCols);
    }

    // If we have just added columns, we need to construct new elements for each cell
    // and set the default column width
    if (iAddedCols > 0)
    {
        InsertNewColumn((iCols - iAddedCols));
    }
    //else    // check for selected cell ranges
    //    ResetSelectedRange();

    /* 不允许画的话直接返回 */
    COMP_BFALSE_R(m_bAllowDraw, TRUE);

    if (GetSafeHwnd())
    {
        ResetScrollBars();
        Invalidate();
    }
    return TRUE;
}

void CGridCtrl::InitColumnData(int iCol, LPCTSTR strHeading, UINT nFormat)
{
    int      iRow    = 0;

    COMP_BL(iCol, 0);

    // Initialise column data
    SetItemText(0, iCol, strHeading);
    for (iRow = 0; iRow < m_iRows; iRow++)
    {
        SetItemFormat(iRow, iCol, nFormat);
    }

    // initialized column width
    m_arColWidths[iCol] = GetTextExtent(strHeading).cx;

    return;
}

/* add to end of columns (if iColumn = -1) */
int CGridCtrl::InsertEndColumn()
{
    int      iRow    = 0;
    int      iCol    = -1;
    GRID_ROW *pRow   = NULL;

    ResetSelectedRange();

    if (m_iRows < 1)
        SetRowCount(1);

    iCol = m_iCols;
    m_arColWidths.Add(0);
    for (iRow = 0; iRow < m_iRows; iRow)
    {
        pRow = m_tRowData[iRow];
        ASSERT_NULL_R(pRow, -1);

        pRow->Add(CreateCell(iRow, iCol));
    }
    m_iCols++;

    return iCol;
}

/* Insert a column at a given position */
int CGridCtrl::InsertGivenColumn(int iCol)
{
    int      iRow    = 0;
    GRID_ROW *pRow   = NULL;

    // If the insertion is for a specific column, check it's within range.
    COMP_BL_R(iCol, 0, -1);
    COMP_BGE_R(iCol, GetColumnCount(), -1);

    ResetSelectedRange();

    // Gotta be able to at least _see_ some of the column.
    if (m_iRows < 1)
        SetRowCount(1);

    m_arColWidths.InsertAt(iCol, 0, (int)0);
    for (iRow = 0; iRow < m_iRows; iRow++)
    {
        pRow = m_tRowData[iRow];
        ASSERT_NULL_R(pRow, -1);

        pRow->InsertAt(iCol, CreateCell(iRow, iCol));
    }

    m_iCols++;

    return iCol;
}

// Insert a column at a given position, or add to end of columns (if iColumn = -1)
int CGridCtrl::InsertColumn(LPCTSTR strHeading,
                            UINT    nFormat /* = DT_CENTER|DT_VCENTER|DT_SINGLELINE */,
                            int     iCol    /* = -1 */)
{
    int      iRow    = 0;
    GRID_ROW *pRow   = NULL;

    /* iCol == -1 */
    if(iCol < 0)
    {
        iCol = InsertEndColumn();
    }
    else
    {
        iCol = InsertGivenColumn(iCol);
    }
    COMP_BL_R(iCol, 0, -1);

    InitColumnData(iCol, strHeading, nFormat);

    if (m_ciCurrent.m_iCol != -1 && iCol < m_ciCurrent.m_iCol)
        m_ciCurrent.m_iCol++;

    ResetScrollBars();

    return iCol;
}


int CGridCtrl::InsertEndRow()
{
    int      iRow    = -1;
    GRID_ROW *pRow   = NULL;

    ResetSelectedRange();

    // Gotta be able to at least _see_ some of the row.
    if (m_iCols < 1)
        SetColumnCount(1);

    // Adding a row to the bottom
    iRow = m_iRows;
    m_arRowHeights.Add(0);
    m_tRowData.Add(new GRID_ROW);

    m_iRows++;
    m_tRowData[iRow]->SetSize(m_iCols);

    return iRow;
}

int CGridCtrl::InsertGivenRow(int iRow)
{
    // If the insertion is for a specific row, check it's within range.
    COMP_BL_R(iRow, 0, -1);
    COMP_BGE_R(iRow, GetRowCount(), -1);

    ResetSelectedRange();

    // Gotta be able to at least _see_ some of the row.
    if (m_iCols < 1)
        SetColumnCount(1);

    // Adding a row to the bottom
    m_arRowHeights.InsertAt(iRow, 0, (int)0);
    m_tRowData.InsertAt(iRow, new GRID_ROW);

    m_iRows++;
    m_tRowData[iRow]->SetSize(m_iCols);

    return iRow;
}

// Insert a row at a given position, or add to end of rows (if iRow = -1)
int CGridCtrl::InsertRow(LPCTSTR strHeading, int iRow /* = -1 */)
{
    int      iCol    = 0;
    GRID_ROW *pRow   = NULL;

    /* iRow == -1 */
    if(iRow < 0)
    {
        iRow = InsertEndRow();
    }
    else
    {
        iRow = InsertGivenRow(iRow);
    }
    COMP_BL_R(iRow, 0, -1);

    // Initialise cell data
    for (iCol = 0; iCol < m_iCols; iCol++)
    {
        pRow = m_tRowData[iRow];
        ASSERT_NULL_R(pRow, -1);
        pRow->SetAt(iCol, CreateCell(iRow, iCol));
    }

    // Set row title
    SetItemText(iRow, 0, strHeading);

    // initialized row height
    m_arRowHeights[iRow] = GetTextExtent(strHeading).cy;

    if (m_ciCurrent.m_iRow != -1 && iRow < m_ciCurrent.m_iRow)
        m_ciCurrent.m_iRow++;

    ResetScrollBars();

    return iRow;
}

// Creates a new grid cell and performs any necessary initialisation
CGridCell* CGridCtrl::CreateCell(int iRow, int iCol)
{
    CGridCell* pgcCell = NULL;

    pgcCell= new CGridCell;
    ASSERT_NULL_R(pgcCell, NULL);

    // Make format same as cell above
    if (iRow > 0 && iCol >= 0 && iCol < m_iCols)
        pgcCell->m_nFormat = GetItemFormat(iRow-1, iCol);

    // Make font default grid font
    memcpy(&(pgcCell->m_lfFont), &m_Logfont, sizeof(LOGFONT));

    return pgcCell;
}

// Performs any cell cleanup necessary to maintain grid integrity
void CGridCtrl::EmptyCell(CGridCell* pgcCell, int iRow, int iCol)
{
    // Set the cells state to 0. If the cell is selected, this
    // will remove the cell from the selected list.
    SetItemState(iRow, iCol, 0);

    // Empty strings
    pgcCell->m_szText.Empty();
}

BOOL CGridCtrl::DeleteColumn(int iCol)
{
    int         iRow    = 0;
    GRID_ROW    *pRow   = NULL;
    CGridCell   *pgcCell= NULL;

    COMP_BL_R (iCol, 0, FALSE);
    COMP_BGE_R(iCol, GetColumnCount(), FALSE);

    ResetSelectedRange();

    for (iRow = 0; iRow < GetRowCount(); iRow++)
    {
        pRow = m_tRowData[iRow];
        ASSERT_NULL_R(pRow, FALSE);

        pgcCell = pRow->GetAt(iCol);
        if (pgcCell)
        {
            EmptyCell(pgcCell, iRow, iCol);
            delete pgcCell;
        }
        pRow->RemoveAt(iCol);
    }
    m_arColWidths.RemoveAt(iCol);
    m_iCols--;

    if (iCol < m_iFixedCols)
        m_iFixedCols--;

    if (iCol == m_ciCurrent.m_iCol)
        m_ciCurrent.m_iRow = m_ciCurrent.m_iCol = -1;
    else if (iCol < m_ciCurrent.m_iCol)
        m_ciCurrent.m_iCol--;

    ResetScrollBars();

    return TRUE;
}

BOOL CGridCtrl::DeleteRow(int iRow)
{
    int         iCol    = 0;
    GRID_ROW    *pRow   = NULL;
    CGridCell   *pgcCell= NULL;

    COMP_BL_R (iRow, 0, FALSE);
    COMP_BGE_R(iRow, GetRowCount(), FALSE);

    pRow = m_tRowData[iRow];
    ASSERT_NULL_R(pRow, FALSE);

    ResetSelectedRange();

    for (iCol = 0; iCol < GetColumnCount(); iCol++)
    {
        pgcCell = pRow->GetAt(iCol);
        if (pgcCell)
        {
            EmptyCell(pgcCell, iRow, iCol);
            delete pgcCell;
        }
    }
    delete pRow;
    m_tRowData.RemoveAt(iRow);
    m_arRowHeights.RemoveAt(iRow);

    m_iRows--;
    if (iRow < m_iFixedRows)
        m_iFixedRows--;

    if (iRow == m_ciCurrent.m_iRow)
        m_ciCurrent.m_iRow = m_ciCurrent.m_iCol = -1;
    else if (iRow < m_ciCurrent.m_iRow)
        m_ciCurrent.m_iRow--;

    ResetScrollBars();

    return TRUE;
}

// Handy function that removes all non-fixed rows
BOOL CGridCtrl::DeleteNonFixedRows()
{
    int iRow   = 0;

    // Delete all data rows
    for (iRow = GetRowCount(); iRow >= GetFixedRowCount(); iRow--)
        DeleteRow(iRow);

    return TRUE;
}

// Removes all rows, columns and data from the grid.
BOOL CGridCtrl::DeleteAllItems()
{
    int         iRow    = 0;
    int         iCol    = 0;
    GRID_ROW    *pRow   = NULL;
    CGridCell   *pgcCell= NULL;

    ResetSelectedRange();

    m_arColWidths.RemoveAll();
    m_arRowHeights.RemoveAll();

    // Delete all cells in the grid
    for (iRow = 0; iRow < m_iRows; iRow++)
    {
        pRow = m_tRowData[iRow];
        COMP_BNULL_CONTINUE(pRow);

        for (iCol = 0; iCol < m_iCols; iCol++)
        {
            pgcCell = pRow->GetAt(iCol);
            if (pgcCell)
            {
                EmptyCell(pgcCell, iRow, iCol);  // TODO - this is a bit of a performance hit.
                delete pgcCell;                // better to call m_SelectedCells.RemoveAll()
            }                                // instead. This is safer for changes though.
        }
        delete pRow;
    }

    // Remove all rows
    m_tRowData.RemoveAll();

    m_ciCurrent.m_iRow = m_ciCurrent.m_iCol = -1;
    m_iRows = m_iFixedRows = m_iCols = m_iFixedCols = 0;

    ResetScrollBars();

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl data functions

// Sorts on a given column using the cell text
BOOL CGridCtrl::SortTextItems(int iCol, BOOL bAscending)
{
    ResetSelectedRange();
    SetFocusCell(-1,-1);
    return SortTextItems(iCol, bAscending, GetFixedRowCount(),-1);
}

BOOL CGridCtrl::CheckSortInfo(int iCol, int iLow, int &iHigh)
{
    COMP_BGE_R(iCol, GetColumnCount(), FALSE);
    COMP_BLE_R(iHigh, iLow, FALSE);

    if (iHigh == -1)
        iHigh = GetRowCount() - 1;

    return TRUE;
}

void CGridCtrl::AscFindSwapTextItem(int iCol, int iLow, int iHigh, int &iLo, int &iHi)
{
    CString strMidItem;
    
    strMidItem = GetItemText( (iLo+iHi)/2, iCol );

    // Find the first element that is greater than or equal to the partition
    // element starting from the left Index.
    while (iLo < iHigh && GetItemText(iLo, iCol) < strMidItem)
        ++iLo;

    // Find an element that is smaller than or equal to  the partition
    // element starting from the right Index.
    while (iHi > iLow  && GetItemText(iHi, iCol) > strMidItem)
        --iHi;

    return;
}

void CGridCtrl::DecFindSwapTextItem(int iCol, int iLow, int iHigh, int &iLo, int &iHi)
{
    CString strMidItem;
    
    strMidItem = GetItemText( (iLo+iHi)/2, iCol );

    // Find the first element that is greater than or equal to the partition
    // element starting from the left Index.
    while (iLo < iHigh && GetItemText(iLo, iCol) > strMidItem)
        ++iLo;

    // Find an element that is smaller than or equal to  the partition
    // element starting from the right Index.
    while (iHi > iLow  && GetItemText(iHi, iCol) < strMidItem)
        --iHi;

    return;
}

void CGridCtrl::SwapTextItems(int iCol, int &iLo, int &iHi)
{
    int         iColIdx     = 0;
    UINT        nRowHeight  = 0;
    CGridCell   *pgcCell    = NULL;
    
    // swap only if the items are not equal
    if (GetItemText(iLo , iCol) != GetItemText(iHi, iCol))
    {
        for (iColIdx = 0; iColIdx < GetColumnCount(); iColIdx++)
        {
            pgcCell = GetCell(iLo, iColIdx);
            COMP_BNULL_CONTINUE(pgcCell);

            SetCell(iLo, iColIdx, GetCell(iHi, iColIdx));
            SetCell(iHi, iColIdx, pgcCell);
        }
        nRowHeight = m_arRowHeights[iLo];
        m_arRowHeights[iLo] = m_arRowHeights[iHi];
        m_arRowHeights[iHi] = nRowHeight;
    }

    ++iLo;
    --iHi;
}

// recursive sort implementation
BOOL CGridCtrl::SortTextItems(int iCol, BOOL bAscending, int iLow, int iHigh)
{
    int     iLo = 0;
    int     iHi = 0;

    COMP_BFALSE_R(CheckSortInfo(iCol, iLow, iHigh), FALSE);

    iLo = iLow;
    iHi = iHigh;

    // loop through the list until indices cross
    while( iLo <= iHi )
    {
        if( bAscending )
        {
            AscFindSwapTextItem(iCol, iLow, iHigh, iLo, iHi);
        }
        else
        {
            DecFindSwapTextItem(iCol, iLow, iHigh, iLo, iHi);
        }

        // If the indexes have not crossed, swap if the items are not equal
        if (iLo <= iHi)
        {
            SwapTextItems(iCol, iLo, iHi);
        }
    }

    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if( iLow < iHi )
        SortTextItems(iCol, bAscending, iLow , iHi);

    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if( iLo < iHigh )
        SortTextItems(iCol, bAscending, iLo, iHigh);

    return TRUE;
}

// Sorts on a given column using the supplied compare function (see CListCtrl::SortItems)
BOOL CGridCtrl::SortItems(PFNLVCOMPARE pfnCompare, int iCol, BOOL bAscending,
                          LPARAM data /* = 0 */)
{
    ResetSelectedRange();
    SetFocusCell(-1,-1);
    return SortItems(pfnCompare, iCol, bAscending, data, GetFixedRowCount(), -1);
}

void CGridCtrl::AscFindSwapItem(PFNLVCOMPARE pfnCompare, int iCol, LPARAM pData,
                                int iLow, int iHigh, int &iLo, int &iHi)
{
    LPARAM      pMidItem    = NULL;
    
    pMidItem = GetItemData( (iLo+iHi)/2, iCol );

    // Find the first element that is greater than or equal to the partition
    // element starting from the left Index.
    while (iLo < iHigh  && pfnCompare(GetItemData(iLo, iCol), pMidItem, pData) < 0)
        ++iLo;

    // Find an element that is smaller than or equal to  the partition
    // element starting from the right Index.
    while (iHi > iLow && pfnCompare(GetItemData(iHi, iCol), pMidItem, pData) > 0)
        --iHi;

    return;
}

void CGridCtrl::DecFindSwapItem(PFNLVCOMPARE pfnCompare, int iCol, LPARAM pData,
                                int iLow, int iHigh, int &iLo, int &iHi)
{
    LPARAM      pMidItem    = NULL;
    
    pMidItem = GetItemData( (iLo+iHi)/2, iCol );

    // Find the first element that is greater than or equal to the partition
    // element starting from the left Index.
    while (iLo < iHigh && pfnCompare(GetItemData(iLo, iCol), pMidItem, pData) > 0)
        ++iLo;

    // Find an element that is smaller than or equal to  the partition
    // element starting from the right Index.
    while (iHi > iLow  && pfnCompare(GetItemData(iHi, iCol), pMidItem, pData) < 0)
        --iHi;

    return;
}

void CGridCtrl::SwapItems(PFNLVCOMPARE pfnCompare, int iCol, LPARAM pData, int &iLo, int &iHi)
{
    int         iColIdx     = 0;
    UINT        nRowHeight  = 0;
    CGridCell   *pgcCell    = NULL;
    
    // swap only if the items are not equal
    if (pfnCompare(GetItemData(iLo, iCol), GetItemData(iHi, iCol), pData) != 0)
    {
        for (iColIdx = 0; iColIdx < GetColumnCount(); iColIdx++)
        {
            pgcCell = GetCell(iLo, iColIdx);
            SetCell(iLo, iColIdx, GetCell(iHi, iColIdx));
            SetCell(iHi, iColIdx, pgcCell);
        }
        nRowHeight = m_arRowHeights[iLo];
        m_arRowHeights[iLo] = m_arRowHeights[iHi];
        m_arRowHeights[iHi] = nRowHeight;
    }

    ++iLo;
    --iHi;
}

// recursive sort implementation
BOOL CGridCtrl::SortItems(PFNLVCOMPARE pfnCompare, int iCol, BOOL bAscending, LPARAM pData,
                          int iLow, int iHigh)
{
    int         iLo         = 0;
    int         iHi         = 0;
    
    COMP_BFALSE_R(CheckSortInfo(iCol, iLow, iHigh), FALSE);

    iLo = iLow;
    iHi = iHigh;

    // loop through the list until indices cross
    while( iLo <= iHi )
    {
        if( bAscending )
        {
            AscFindSwapItem(pfnCompare, iCol, pData, iLow, iHigh, iLo, iHi);
        }
        else
        {
            DecFindSwapItem(pfnCompare, iCol, pData, iLow, iHigh, iLo, iHi);
        }   

        // If the indexes have not crossed, swap if the items are not equal
        if (iLo <= iHi)
        {
            SwapItems(pfnCompare, iCol, pData, iLo, iHi);
        }
    }

    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if( iLow < iHi )
        SortItems(pfnCompare, iCol, bAscending, pData, iLow, iHi);

    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if( iLo < iHigh )
        SortItems(pfnCompare, iCol, bAscending, pData, iLo, iHigh);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl data functions

BOOL CGridCtrl::SetItem(const GV_ITEM* pItem)
{
    CGridCell *pgcCell = NULL;

    ASSERT_NULL_R(pItem, FALSE);

    pgcCell = GetCell(pItem->iRow, pItem->iCol);
    ASSERT_NULL_R(pgcCell, FALSE);

    if (pItem->nMask & GVIF_TEXT)
        pgcCell->m_szText  = pItem->szText;
    if (pItem->nMask & GVIF_PARAM)
        pgcCell->m_lParam  = pItem->lParam;
    if (pItem->nMask & GVIF_IMAGE)
        pgcCell->m_iImage  = pItem->iImage;
    if (pItem->nMask & GVIF_STATE)
        pgcCell->m_nState  = pItem->nState;
    if (pItem->nMask & GVIF_FORMAT)
        pgcCell->m_nFormat = pItem->nFormat;
    if (pItem->nMask & GVIF_BKCLR)
        pgcCell->m_clrBk   = pItem->clrBk;
    if (pItem->nMask & GVIF_FGCLR)
        pgcCell->m_clrFg   = pItem->clrFg;
    if (pItem->nMask & GVIF_FONT)
        memcpy(&(pgcCell->m_lfFont), &(pItem->lfFont), sizeof(LOGFONT));

    return TRUE;
}

BOOL CGridCtrl::GetItem(GV_ITEM* pItem)
{
    CGridCell *pgcCell = NULL;

    ASSERT_NULL_R(pItem, FALSE);

    pgcCell = GetCell(pItem->iRow, pItem->iCol);
    ASSERT_NULL_R(pgcCell, FALSE);

    if (pItem->nMask & GVIF_TEXT)
        pItem->szText  = GetItemText(pItem->iRow, pItem->iCol);
    if (pItem->nMask & GVIF_PARAM)
        pItem->lParam  = pgcCell->m_lParam;
    if (pItem->nMask & GVIF_IMAGE)
        pItem->iImage  = pgcCell->m_iImage;
    if (pItem->nMask & GVIF_STATE)
        pItem->nState  = pgcCell->m_nState;
    if (pItem->nMask & GVIF_FORMAT)
        pItem->nFormat = pgcCell->m_nFormat;
    if (pItem->nMask & GVIF_BKCLR)
        pItem->clrBk   = pgcCell->m_clrBk;
    if (pItem->nMask & GVIF_FGCLR)
        pItem->clrFg   = pgcCell->m_clrFg;
    if (pItem->nMask & GVIF_FONT)
        memcpy(&(pItem->lfFont), &(pgcCell->m_lfFont), sizeof(LOGFONT));

    return TRUE;
}

BOOL CGridCtrl::SetItemText(int iRow, int iCol, LPCTSTR str)
{
    CGridCell *pgcCell = NULL;

    pgcCell = GetCell(iRow, iCol);
    ASSERT_NULL_R(pgcCell, FALSE);

    pgcCell->m_szText = str;
    return TRUE;
}

BOOL CGridCtrl::SetItemData(int iRow, int iCol, LPARAM lParam)
{
    CGridCell *pgcCell = NULL;

    pgcCell = GetCell(iRow, iCol);
    ASSERT_NULL_R(pgcCell, FALSE);

    pgcCell->m_lParam = lParam;
    return TRUE;
}

LPARAM CGridCtrl::GetItemData(int iRow, int iCol) const
{
    CGridCell *pgcCell = NULL;

    pgcCell = GetCell(iRow, iCol);
    ASSERT_NULL_R(pgcCell, (LPARAM) 0);

    return pgcCell->m_lParam;
}

BOOL CGridCtrl::SetItemImage(int iRow, int iCol, int iImage)
{
    CGridCell *pgcCell = NULL;

    pgcCell = GetCell(iRow, iCol);
    ASSERT_NULL_R(pgcCell, FALSE);

    pgcCell->m_iImage = iImage;
    return TRUE;
}

int CGridCtrl::GetItemImage(int iRow, int iCol) const
{
    CGridCell *pgcCell = NULL;

    pgcCell = GetCell(iRow, iCol);
    ASSERT_NULL_R(pgcCell, -1);

    return pgcCell->m_iImage;
}

BOOL CGridCtrl::SetItemState(int iRow, int iCol, UINT state)
{
    CGridCell *pgcCell = NULL;
    CCellID   ciCell;
    DWORD     dwKey    = 0;

    pgcCell = GetCell(iRow, iCol);
    ASSERT_NULL_R(pgcCell, FALSE);

    // If the cell is being unselected, remove it from the selected list
    if ((pgcCell->m_nState & GVIS_SELECTED) && !(state & GVIS_SELECTED))
    {
        dwKey = MAKELONG(iRow, iCol);

        if (m_tSelectedCellMap.Lookup(dwKey, (CCellID&)ciCell))
            m_tSelectedCellMap.RemoveKey(dwKey);
    }

    // If cell is being selected, add it to the list of selected cells
    else if (!(pgcCell->m_nState & GVIS_SELECTED) && (state & GVIS_SELECTED))
    {
        ciCell = CCellID (iRow, iCol);
        m_tSelectedCellMap.SetAt(MAKELONG(iRow, iCol), ciCell);
    }

    // Set the cell's state
    pgcCell->m_nState = state;

    return TRUE;
}

UINT CGridCtrl::GetItemState(int iRow, int iCol) const
{
    CGridCell *pgcCell = NULL;

    pgcCell = GetCell(iRow, iCol);
    ASSERT_NULL_R(pgcCell, 0);

    return pgcCell->m_nState;
}

BOOL CGridCtrl::SetItemFormat(int iRow, int iCol, UINT nFormat)
{
    CGridCell *pgcCell = NULL;

    pgcCell = GetCell(iRow, iCol);
    ASSERT_NULL_R(pgcCell, FALSE);

    pgcCell->m_nFormat = nFormat;
    return TRUE;
}

UINT CGridCtrl::GetItemFormat(int iRow, int iCol) const
{
    CGridCell *pgcCell = NULL;

    pgcCell = GetCell(iRow, iCol);
    ASSERT_NULL_R(pgcCell, 0);

    return pgcCell->m_nFormat;
}

BOOL CGridCtrl::SetItemBkColour(int iRow, int iCol, COLORREF cr /* = CLR_DEFAULT */)
{
    CGridCell *pgcCell = NULL;

    pgcCell = GetCell(iRow, iCol);
    ASSERT_NULL_R(pgcCell, FALSE);

    pgcCell->m_clrBk = cr;
    return TRUE;
}

COLORREF CGridCtrl::GetItemBkColour(int iRow, int iCol) const
{
    CGridCell *pgcCell = NULL;

    pgcCell = GetCell(iRow, iCol);
    ASSERT_NULL_R(pgcCell, 0);

    return pgcCell->m_clrBk;
}

BOOL CGridCtrl::SetItemFgColour(int iRow, int iCol, COLORREF cr /* = CLR_DEFAULT */)
{
    CGridCell *pgcCell = NULL;

    pgcCell = GetCell(iRow, iCol);
    ASSERT_NULL_R(pgcCell, FALSE);

    pgcCell->m_clrFg = cr;
    return TRUE;
}

COLORREF CGridCtrl::GetItemFgColour(int iRow, int iCol) const
{
    CGridCell *pgcCell = NULL;

    pgcCell = GetCell(iRow, iCol);
    ASSERT_NULL_R(pgcCell, 0);

    return pgcCell->m_clrFg;
}

BOOL CGridCtrl::SetItemFont(int iRow, int iCol, LOGFONT* lf)
{
    CGridCell *pgcCell = NULL;

    pgcCell = GetCell(iRow, iCol);
    ASSERT_NULL_R(pgcCell, FALSE);

    memcpy(&(pgcCell->m_lfFont), lf, sizeof(LOGFONT));
    return TRUE;
}

LOGFONT* CGridCtrl::GetItemFont(int iRow, int iCol)
{
    CGridCell *pgcCell = NULL;

    pgcCell = GetCell(iRow, iCol);
    ASSERT_NULL_R(pgcCell, FALSE);

    return &(pgcCell->m_lfFont);
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column size functions

long CGridCtrl::GetVirtualWidth() const
{
    int     i             = 0;
    long    lVirtualWidth = 0;
    int     iColCount     = 0;

    iColCount = GetColumnCount();
    for (i = 0; i < iColCount; i++)
        lVirtualWidth += m_arColWidths[i];

    return lVirtualWidth;
}

long CGridCtrl::GetVirtualHeight() const
{
    int     i             = 0;
    long    lVirtualHeight = 0;
    int     iRowCount     = 0;

    iRowCount = GetRowCount();
    for (i = 0; i < iRowCount; i++)
        lVirtualHeight += m_arRowHeights[i];

    return lVirtualHeight;
}

int CGridCtrl::GetRowHeight(int iRow) const
{
    COMP_BL_R(iRow, 0, -1);
    COMP_BGE_R(iRow, m_iRows, -1);

    return m_arRowHeights[iRow];
}

int CGridCtrl::GetColumnWidth(int iCol) const
{
    COMP_BL_R(iCol, 0, -1);
    COMP_BGE_R(iCol, m_iCols, -1);

    return m_arColWidths[iCol];
}

BOOL CGridCtrl::SetRowHeight(int iRow, int iHeight)
{
    COMP_BL_R(iRow, 0, FALSE);
    COMP_BGE_R(iRow, m_iRows, FALSE);
    COMP_BLE_R(iHeight, 0, FALSE);

    m_arRowHeights[iRow] = iHeight;
    return TRUE;
}

BOOL CGridCtrl::SetColumnWidth(int iCol, int iWidth)
{
    COMP_BL_R(iCol, 0, FALSE);
    COMP_BGE_R(iCol, m_iCols, FALSE);
    COMP_BLE_R(iWidth, 0, FALSE);

    m_arColWidths[iCol] = iWidth;
    return TRUE;
}

int CGridCtrl::GetFixedRowHeight() const
{
    int i       = 0;
    int iHeight = 0;

    for (i = 0; i < m_iFixedRows; i++)
        iHeight += GetRowHeight(i);

    return iHeight;
}

int CGridCtrl::GetFixedColumnWidth() const
{
    int i       = 0;
    int iWidth  = 0;

    for (i = 0; i < m_iFixedCols; i++)
        iWidth += GetColumnWidth(i);

    return iWidth;
}

BOOL CGridCtrl::AutoSizeColumn(int iCol)
{   
    int     iWidth  = 0;
    int     iRow    = 0;
    CDC     *pDC    = NULL; 
    CSize   szTmp;

    COMP_BL_R(iCol, 0, FALSE);
    COMP_BGE_R(iCol, m_iCols, FALSE);

    pDC = GetDC();
    ASSERT_NULL_R(pDC, FALSE);

    for (iRow = 0; iRow < GetRowCount(); iRow++)
    {
        szTmp = GetCellExtent(iRow, iCol, pDC);
        if (szTmp.cx > iWidth)
            iWidth = szTmp.cx;
    }

    m_arColWidths[iCol] = iWidth;

    ReleaseDC(pDC);
    ResetScrollBars();

    return TRUE;
}

BOOL CGridCtrl::AutoSizeRow(int iRow)
{
    int     iHeight = 0;
    int     iCol    = 0;
    CDC     *pDC    = NULL; 
    CSize   szTmp;

    COMP_BL_R(iRow, 0, FALSE);
    COMP_BGE_R(iRow, m_iRows, FALSE);

    pDC = GetDC();
    ASSERT_NULL_R(pDC, FALSE);

    for (iCol = 0; iCol < GetColumnCount(); iCol++)
    {
        szTmp = GetCellExtent(iRow, iCol, pDC);
        if (szTmp.cy > iHeight)
            iHeight = szTmp.cy;
    }
    m_arRowHeights[iRow] = iHeight;

    ReleaseDC(pDC);
    ResetScrollBars();

    return TRUE;
}

void CGridCtrl::AutoSizeColumns()
{
    int iCol = 0;

    for (iCol = 0; iCol < GetColumnCount(); iCol++)
        AutoSizeColumn(iCol);
}

void CGridCtrl::AutoSizeRows()
{
    int iRow = 0;

    for (iRow = 0; iRow < GetRowCount(); iRow++)
        AutoSizeRow(iRow);
}

// sizes all rows and columns
// faster than calling both AutoSizeColumns() and AutoSizeRows()
void CGridCtrl::AutoSize()
{
    int     iCol    = 0;
    int     iRow    = 0;
    int     iNumCols= 0;
    int     iNumRows= 0;
    CDC     *pDC    = NULL;
    CSize   szTmp;

    pDC = GetDC();
    ASSERT_NULL(pDC);

    iNumCols = GetColumnCount();
    iNumRows = GetRowCount();

    // initialize column widths to zero
    for ( iCol = 0; iCol < iNumCols; iCol++)
        m_arColWidths[iCol] = 0;

    // initialize row heights to zero
    for ( iRow = 0; iRow < iNumRows; iRow++)
        m_arRowHeights[iRow] = 0;

    for (iCol = 0; iCol < iNumCols; iCol++)
        for (iRow = 0; iRow < iNumRows; iRow++)
        {
            szTmp = GetCellExtent(iRow, iCol, pDC);
            if (szTmp.cx > (int) m_arColWidths[iCol])
                m_arColWidths[iCol] = szTmp.cx;
            if (szTmp.cy > (int) m_arRowHeights[iRow])
                m_arRowHeights[iRow] = szTmp.cy;
        }

    ReleaseDC(pDC);

    COMP_BFALSE(m_bAllowDraw);

    ResetScrollBars();
    Invalidate();

}

void CGridCtrl::ExpandColumnsToFit()
{
    int     i   = 0;
    long    lVirtualWidth   = 0;
    int     iDifference     = 0;
    int     iColAdjustment  = 0;
    int     iLeftOver       = 0;
    CRect   rcTmp;

    COMP_BLE(GetColumnCount(), 0);

    GetClientRect(&rcTmp);

    lVirtualWidth   = GetVirtualWidth();
    iDifference     = rcTmp.Width() - (int) lVirtualWidth;
    iColAdjustment  = iDifference / GetColumnCount();

    for (i = 0; i < GetColumnCount(); i++)
       m_arColWidths[i] += iColAdjustment;

    if (iDifference > 0)
    {
        iLeftOver = iDifference % GetColumnCount();
        for (i = 0; i < iLeftOver; i++)
            m_arColWidths[i] += 1;
    }
    else
    {
        iLeftOver = (-iDifference) % GetColumnCount();
        for (i = 0; i < iLeftOver; i++)
            m_arColWidths[i] -= 1;
    }

    COMP_BFALSE(m_bAllowDraw);

    Invalidate();
}

void CGridCtrl::ExpandRowsToFit()
{
    int     i   = 0;
    long    lVirtualHeight  = 0;
    int     iDifference     = 0;
    int     iRowAdjustment  = 0;
    int     iLeftOver       = 0;
    CRect   rcTmp;

    COMP_BLE(GetRowCount(), 0);

    GetClientRect(&rcTmp);

    lVirtualHeight  = GetVirtualHeight();
    iDifference     = rcTmp.Height() - (int) lVirtualHeight;
    iRowAdjustment  = iDifference / GetRowCount();

    for (i = 0; i < GetRowCount(); i++)
       m_arRowHeights[i] += iRowAdjustment;

    if (iDifference > 0)
    {
        iLeftOver = iDifference % GetRowCount();
        for (i = 0; i < iLeftOver; i++)
            m_arRowHeights[i] += 1;
    }
    else
    {
        iLeftOver = (-iDifference) % GetRowCount();
        for (i = 0; i < iLeftOver; i++)
            m_arRowHeights[i] -= 1;
    }

    COMP_BFALSE(m_bAllowDraw);

    Invalidate();
}

void CGridCtrl::ExpandToFit()
{
    ExpandColumnsToFit();   // This will remove any existing horz scrollbar
    ExpandRowsToFit();      // This will remove any existing vert scrollbar
    ExpandColumnsToFit();   // Just in case the first adjustment was with a vert
                            // scrollbar in place
}

BOOL CGridCtrl::IsCellFixed(int iRow, int iCol)
{
    return (iRow < GetFixedRowCount() || iCol < GetFixedColumnCount());
}

void CGridCtrl::SetModified(BOOL bModified /*=TRUE*/, int iRow /*=-1*/, int iCol /*=-1*/)
{
    int iRowTmp = 0;
    int iColTmp = 0;

    if (iRow > 0 && iCol > 0)
    {
        if (bModified)
        {
            SetItemState(iRow, iCol, GetItemState(iRow, iCol) | GVIS_MODIFIED);
            m_bModified = TRUE;
        }
        else
            SetItemState(iRow, iCol, GetItemState(iRow, iCol) & ~GVIS_MODIFIED);
    }
    else
        m_bModified = bModified;

    COMP_BTRUE(m_bModified);

    for (iRowTmp = 0; iRowTmp < GetRowCount(); iRowTmp++)
        for (iColTmp = 0; iColTmp < GetColumnCount(); iColTmp++)
            SetItemState(iRowTmp, iColTmp, GetItemState(iRowTmp, iColTmp) & ~GVIS_MODIFIED);
}

BOOL CGridCtrl::GetModified(int iRow /*=-1*/, int iCol /*=-1*/)
{
    COMP_BLE_R(iRow, 0, m_bModified);
    COMP_BLE_R(iCol, 0, m_bModified);

    return ( (GetItemState(iRow, iCol) & GVIS_MODIFIED) == GVIS_MODIFIED );
}

/////////////////////////////////////////////////////////////////////////////////////
// GridCtrl cell visibility tests and invalidation/redraw functions

// EnsureVisible supplied by Roelf Werkman
void CGridCtrl::EnsureVisible(int iRow, int iCol)
{
    int         iRight      = 0;
    int         iLeft       = 0;
    int         iDown       = 0;
    int         iUp         = 0;
    CCellRange  crVisibles;
    // Move one more if we only see a snall bit of the cell
    CRect       rcCell;
    CRect       rcWindow;

    crVisibles  = GetVisibleNonFixedCellRange();
    iRight      = iCol - crVisibles.GetMaxCol();
    iLeft       = crVisibles.GetMinCol() - iCol;
    iDown       = iRow - crVisibles.GetMaxRow();
    iUp         = crVisibles.GetMinRow() - iRow;

    while (iRight > 0)
    {
        SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
        iRight--;
    }
    while (iLeft > 0)
    {
        SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
        iLeft--;
    }
    while (iDown > 0)
    {
        SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
        iDown--;
    }
    while (iUp > 0)
    {
        SendMessage(WM_VSCROLL, SB_LINEUP, 0);
        iUp--;
    }

   GetCellRect(iRow, iCol, rcCell);
   GetClientRect(&rcWindow);
   if (rcCell.right > rcWindow.right)
        SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
   if (rcCell.bottom > rcWindow.bottom)
        SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
}

BOOL CGridCtrl::IsCellEditable(CCellID &ciCell) const
{
    return IsCellEditable(ciCell.m_iRow, ciCell.m_iCol);
}

BOOL CGridCtrl::IsCellEditable(int iRow, int iCol) const
{
    //want remove it?
    CGridCell *pgcCell = NULL;

    pgcCell = GetCell(iRow,iCol);
    ASSERT_NULL_R(pgcCell, FALSE);
    COMP_BTRUE_R(pgcCell->m_bNoEdit, FALSE);

    //above added for combine cell
    return IsEditable() && ((GetItemState(iRow, iCol) & GVIS_READONLY) != GVIS_READONLY);
}

BOOL CGridCtrl::IsCellVisible(CCellID ciCell) const
{
    return IsCellVisible(ciCell.m_iRow, ciCell.m_iCol);
}

BOOL CGridCtrl::IsCellRowVis(int iRow) const
{
    int     i  = 0;
    int     y  = 0;
    int     iStart = 0;
    CRect   rcTmp;
    CCellID ciTopLeft;
    
    GetClientRect(&rcTmp);

    if (iRow >= GetFixedRowCount())
    {
        ciTopLeft = GetTopleftNonFixedCell();
        COMP_BL_R(iRow, ciTopLeft.m_iRow, FALSE);

        y      = GetFixedRowHeight();
        iStart = ciTopLeft.m_iRow;
    }

    for (i = iStart; i <= iRow; i++)
    {
        COMP_BGE_R(y, rcTmp.bottom, FALSE);

        y += GetRowHeight(i);
    }

    return TRUE;
}

BOOL CGridCtrl::IsCellColVis(int iCol) const
{
    int     i  = 0;
    int     x  = 0;
    int     iStart = 0;
    CRect   rcTmp;
    CCellID ciTopLeft;
    
    GetClientRect(&rcTmp);

    if (iCol >= GetFixedColumnCount())
    {
        ciTopLeft = GetTopleftNonFixedCell();
        COMP_BL_R(iCol, ciTopLeft.m_iCol, FALSE);

        x      = GetFixedColumnWidth();
        iStart = ciTopLeft.m_iCol;
    }

    for (i = iStart; i <= iCol; i++)
    {
        COMP_BGE_R(x, rcTmp.right, FALSE);

        x += GetColumnWidth(i);
    }

    return TRUE;
}

BOOL CGridCtrl::IsCellVisible(int iRow, int iCol) const
{
    COMP_BFALSE_R(IsWindow(m_hWnd), FALSE) ;

    return (IsCellRowVis(iRow) && IsCellColVis(iCol));
}

BOOL CGridCtrl::InvalidateCellRect(const CCellID& ciCell)
{
    CRect rcTmp;

    COMP_BFALSE_R(::IsWindow(GetSafeHwnd()), FALSE) ;
    COMP_BFALSE_R(m_bAllowDraw, FALSE) ;

    ASSERT(IsValid(ciCell));

    COMP_BFALSE_R(IsCellVisible(ciCell.m_iRow, ciCell.m_iCol), FALSE);

    COMP_BFALSE_R(GetCellRect(ciCell, rcTmp), FALSE);

    rcTmp.right++;
    rcTmp.bottom++;
    InvalidateRect(rcTmp, TRUE);

    return TRUE;
}

BOOL CGridCtrl::InvalidateCellRect(const CCellRange& crRange)
{
    CCellRange  crVisible;
    CRect       rcTmp;

    ASSERT(IsValid(crRange));

    COMP_BFALSE_R(::IsWindow(GetSafeHwnd()), FALSE) ;
    COMP_BFALSE_R(m_bAllowDraw, FALSE) ;

    crVisible = GetVisibleNonFixedCellRange().Intersect(crRange);

    COMP_BFALSE_R(GetCellRangeRect(crVisible, rcTmp), FALSE);

    rcTmp.right++;
    rcTmp.bottom++;
    InvalidateRect(rcTmp, TRUE);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl Mouse stuff

// Handles mouse wheel notifications
// Note - if this doesn't work for win95 then use OnRegisteredMouseWheel instead
#if !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)
BOOL CGridCtrl::OnMouseWheel(UINT nFlags, short iDelta, CPoint ptOpr)
{
    int     i           = 0;
    int     iScrolled   = 0;
    UINT    nSBCode     = 0;

    // A m_iRowsPerWheelNotch value less than 0 indicates that the mouse
    // wheel scrolls whole pages, not just lines.
    nSBCode   = SB_LINEUP;
    iScrolled = m_iRowsPerWheelNotch * iDelta / 120;
    if (m_iRowsPerWheelNotch == -1)
    {
        iScrolled = iDelta / 120;
        nSBCode   = SB_PAGEUP;
    }

    if(iScrolled > 0)
    {
        for (i = 0; i < iScrolled; i++)
            PostMessage(WM_VSCROLL, nSBCode, 0);

        return CWnd::OnMouseWheel(nFlags, iDelta, ptOpr);
    }
    
    /* iScrolled <= 0 */
    nSBCode = SB_LINEDOWN;
    if (m_iRowsPerWheelNotch == -1)
        nSBCode = SB_PAGEDOWN;

    for (i = 0; i > iScrolled; i--)
        PostMessage(WM_VSCROLL, nSBCode, 0);

    return CWnd::OnMouseWheel(nFlags, iDelta, ptOpr);
}
#endif // !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)

/* 根据鼠标位置，更新鼠标模式和光标 */
void CGridCtrl::UpdateMouseMode(CPoint& ptOpr)
{    
    if (m_bAllowColumnResize && MouseOverColumnResizeArea(ptOpr))
    {
        if (m_iMouseMode != MOUSE_OVER_COL_DIVIDE)
        {
            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
            m_iMouseMode = MOUSE_OVER_COL_DIVIDE;
        }
    }
    else if (m_bAllowRowResize && MouseOverRowResizeArea(ptOpr))
    {
        if (m_iMouseMode != MOUSE_OVER_ROW_DIVIDE)
        {
            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
            m_iMouseMode = MOUSE_OVER_ROW_DIVIDE;
        }
    }
    else if (m_iMouseMode != MOUSE_NOTHING)
    {
        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        m_iMouseMode = MOUSE_NOTHING;
    }

    return;
}

void CGridCtrl::MM_UpdateTitleTips(CPoint& ptOpr)
{
    int         iBottomRow  = 0;
    int         iRightCol   = 0;
    CRect       rcText;
    CRect       rcCell;
    CRect       rcTmp;
    CCellID     ciCurrent;
    CGridCell   *pgcCell    = NULL;
    CGridCell   *pgcTopLeft = NULL;
    CCellRange  crTopLeft;

    ciCurrent = GetCellFromPt(ptOpr);

    //want remove it?
    COMP_BFALSE(IsValid(ciCurrent));

    pgcCell = GetCell(ciCurrent.m_iRow,ciCurrent.m_iCol);
    ASSERT_NULL(pgcCell);

    if(pgcCell->m_bHide == TRUE)
    {
        pgcTopLeft =  GetCell(pgcCell->m_iTopleftRow,pgcCell->m_iTopleftCol);
        ASSERT_NULL(pgcTopLeft);
        crTopLeft.Set(pgcCell->m_iTopleftRow,
                      pgcCell->m_iTopleftCol,
                      pgcCell->m_iTopleftRow + pgcTopLeft->m_iNumHhide,
                      pgcCell->m_iTopleftCol + pgcTopLeft->m_iNumVhide);
        iBottomRow =  pgcCell->m_iTopleftRow + pgcTopLeft->m_iNumHhide;
        iRightCol  =  pgcCell->m_iTopleftCol + pgcTopLeft->m_iNumVhide;
    }

    if( CELLHIDE(pgcCell) )
    {
        if( GetCellRect(iBottomRow,iRightCol,rcTmp) &&
            GetCellRangeRect(crTopLeft, rcCell))
        {
            rcCell.right  = rcTmp.right;
            rcCell.bottom = rcTmp.bottom;
        }

        m_tTitleTip.Show(rcCell,
                         GetItemText(pgcCell->m_iTopleftRow,pgcCell->m_iTopleftCol),
                         0,
                         rcCell,
                         GetItemFont(pgcCell->m_iTopleftRow,pgcCell->m_iTopleftCol));
        return;
    }

    /* if(pgcCell->m_bHide!=TRUE&&pgcCell->m_iNumHhide==0&&pgcCell->m_iNumVhide==0) */
    if (GetTextRect(ciCurrent.m_iRow, ciCurrent.m_iCol, rcText) &&
        GetCellRect(ciCurrent.m_iRow, ciCurrent.m_iCol, rcCell))
    {
         m_tTitleTip.Show(rcText,
                          GetItemText(ciCurrent.m_iRow, ciCurrent.m_iCol),
                          0,
                          rcCell,
                          GetItemFont(ciCurrent.m_iRow, ciCurrent.m_iCol));
    }

    return;
}

void CGridCtrl::MM_WhenLeftBtnUp(CPoint& ptOpr)
{
    /* 根据鼠标位置，更新鼠标模式和光标 */
    UpdateMouseMode(ptOpr);

#ifndef GRIDCONTROL_NO_TITLETIPS
    if (m_iMouseMode == MOUSE_NOTHING && m_bTitleTips)
    {
        MM_UpdateTitleTips(ptOpr);
    }
#endif

    m_ptLastMouse = ptOpr;
}

void CGridCtrl::MM_MouseSelect(CPoint& ptOpr)
{
    CCellID ciCurrent;

    ciCurrent = GetCellFromPt(ptOpr);
    COMP_BFALSE(IsValid(ciCurrent));

    if (ciCurrent != GetFocusCell())
    {
        OnSelecting(ciCurrent);
        //SetFocusCell(max(ciCurrent.m_iRow, m_iFixedRows),
        //             max(ciCurrent.m_iCol, m_iFixedCols));
        if (ciCurrent.m_iRow >= m_iFixedRows &&
            ciCurrent.m_iCol >= m_iFixedCols)
        {
            SetFocusCell(ciCurrent);
        }
    }
}

void CGridCtrl::MM_MouseSizingCol(CPoint& ptOpr)
{
    CRect   rcOldInverted;
    CRect   rcNewInverted;
    CRect   rcTmp;
    CDC     *pDC        = NULL;
    
    pDC = GetDC();
    ASSERT_NULL(pDC);

    GetClientRect(&rcTmp);

    rcOldInverted.SetRect(m_ptLastMouse.x,     rcTmp.top,
                          m_ptLastMouse.x + 2, rcTmp.bottom);
    pDC->InvertRect(&rcOldInverted);
    rcNewInverted.SetRect (ptOpr.x,     rcTmp.top,
                           ptOpr.x + 2, rcTmp.bottom);
    pDC->InvertRect(&rcNewInverted);
    ReleaseDC(pDC);
}

void CGridCtrl::MM_MouseSizingRow(CPoint& ptOpr)
{
    CRect   rcOldInverted;
    CRect   rcNewInverted;
    CRect   rcTmp;
    CDC     *pDC        = NULL;
    
    pDC = GetDC();
    ASSERT_NULL(pDC);

    GetClientRect(&rcTmp);

    rcOldInverted.SetRect(rcTmp.left,  m_ptLastMouse.y,
                          rcTmp.right, m_ptLastMouse.y + 2);
    pDC->InvertRect(&rcOldInverted);
    rcNewInverted.SetRect (rcTmp.left,  ptOpr.y,
                           rcTmp.right, ptOpr.y + 2);
    pDC->InvertRect(&rcNewInverted);
    ReleaseDC(pDC);
}

void CGridCtrl::MM_WhenLeftBtnDown(CPoint& ptOpr)
{
    switch(m_iMouseMode)
    {
        case MOUSE_SELECT_ALL:
            break;

        case MOUSE_SELECT_COL:
        case MOUSE_SELECT_ROW:
        case MOUSE_SELECT_CELLS:
            MM_MouseSelect(ptOpr);
            break;

        case MOUSE_SIZING_COL:
            MM_MouseSizingCol(ptOpr);
            break;

        case MOUSE_SIZING_ROW:
            MM_MouseSizingRow(ptOpr);
            break;

#ifndef GRIDCONTROL_NO_DRAGDROP
        case MOUSE_PREPARE_DRAG:
            OnBeginDrag();
            break;
#endif
    }
}

void CGridCtrl::OnMouseMove(UINT nFlags, CPoint ptOpr)
{
    CRect   rcTmp;

    GetClientRect(&rcTmp);

#ifndef GRIDCONTROL_NO_DRAGDROP
    // If outside client area, return (unless we are drag n dropping)
    if (m_iMouseMode != MOUSE_DRAGGING && !rcTmp.PtInRect(ptOpr))
        return;
#endif

    // If the left mouse button is up, then test to see if row/column sizing is imminent
    if (!(nFlags & MK_LBUTTON)
        || (m_iMouseMode == MOUSE_NOTHING && (nFlags & MK_LBUTTON)))
    {
        MM_WhenLeftBtnUp(ptOpr);
        return;
    }

    if (!IsValid(m_ciLeftClickDown))
    {
        m_ptLastMouse = ptOpr;
        return;
    }

    // If the left mouse button is down, then process appropriately
    if (nFlags & MK_LBUTTON)
    {
        MM_WhenLeftBtnDown(ptOpr);
    }

    m_ptLastMouse = ptOpr;
}

BOOL CGridCtrl::LBDC_ColDivide(CPoint ptOpr)
{
    CCellID ciCell;
    CPoint  ptStart;

    ciCell = GetCellFromPt(ptOpr);
    ASSERT(IsValid(ciCell));

    COMP_BFALSE_R(GetCellOrigin(0, ciCell.m_iCol, &ptStart), FALSE);

    // Clicked right of border
    if (ptOpr.x - ptStart.x <= m_iResizeCaptureRange)
        ciCell.m_iCol--;

    AutoSizeColumn(ciCell.m_iCol);
    Invalidate();

    return TRUE;
}

BOOL CGridCtrl::LBDC_RowDivide(CPoint ptOpr)
{
    CCellID ciCell;
    CPoint  ptStart;

    ciCell = GetCellFromPt(ptOpr);
    ASSERT(IsValid(ciCell));

    COMP_BFALSE_R(GetCellOrigin(0, ciCell.m_iCol, &ptStart), FALSE);

    // Clicked below border
    if (ptOpr.y - ptStart.y <= m_iResizeCaptureRange)
        ciCell.m_iRow--;

     AutoSizeRow(ciCell.m_iRow);
    Invalidate();

    return TRUE;
}

BOOL CGridCtrl::LBDC_Normal(CPoint ptOpr)
{
    CCellID ciCell;

    if (m_ciLeftClickDown.m_iRow >= m_iFixedRows &&
        m_ciLeftClickDown.m_iCol >= m_iFixedCols &&
        IsValid(m_ciLeftClickDown))
    {
        OnEditCell(m_ciCurrent.m_iRow, m_ciCurrent.m_iCol, VK_LBUTTON);
    }
    else if (m_bListMode)
    {
        ciCell = GetCellFromPt(ptOpr);
        COMP_BFALSE_R(IsValid(ciCell), FALSE);

        if (ciCell.m_iRow >= m_iFixedRows &&
            ciCell.m_iCol < GetFixedColumnCount())
            OnEditCell(ciCell.m_iRow, ciCell.m_iCol, VK_LBUTTON);
    }

    return TRUE;
}

void CGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint ptOpr)
{
    BOOL    bRes    = FALSE;

    if (m_iMouseMode == MOUSE_OVER_COL_DIVIDE)
    {
        bRes = LBDC_ColDivide(ptOpr);
    }
    else if (m_iMouseMode == MOUSE_OVER_ROW_DIVIDE)
    {
        bRes = LBDC_RowDivide(ptOpr);
    }
    else if (m_iMouseMode == MOUSE_NOTHING)
    {
        bRes = LBDC_Normal(ptOpr);
    }

    COMP_BFALSE(bRes);

    CWnd::OnLButtonDblClk(nFlags, ptOpr);
}

/* return FALSE 表示处理完成直接返回，不需要后续的处理 */
BOOL CGridCtrl::LBD_UpdateStatus(UINT nFlags, CPoint& ptOpr)
{
    HWND hOldFocusWnd = ::GetFocus();

    m_ptLeftClickDown = ptOpr;
    m_ciLeftClickDown = GetCellFromPt(ptOpr);
    COMP_BFALSE_R(IsValid(m_ciLeftClickDown), FALSE);

    m_ciSelectionStart = (nFlags & MK_SHIFT)? m_ciCurrent : m_ciLeftClickDown;

    SetFocus();
    // Auto-destroy any InPlaceEdit's

    // If the user clicks on the current cell, then prepare to edit it.
    // (If the user moves the mouse, then dragging occurs)
    if (m_ciLeftClickDown == m_ciCurrent)
    {
        m_iMouseMode = MOUSE_PREPARE_EDIT;
        return FALSE;
    }
    
    if (m_iMouseMode != MOUSE_OVER_COL_DIVIDE &&
        m_iMouseMode != MOUSE_OVER_ROW_DIVIDE)
    {
        SetFocusCell(-1,-1);
        SetFocusCell(max(m_ciLeftClickDown.m_iRow, m_iFixedRows),
                     max(m_ciLeftClickDown.m_iCol, m_iFixedCols));
    }

#ifndef GRIDCONTROL_NO_DRAGDROP
    // If the user clicks on a selected cell, then prepare to drag it.
    // (If the user moves the mouse, then dragging occurs)
    if ((m_bAllowDragAndDrop)           &&
        (hOldFocusWnd == GetSafeHwnd()) &&
        (GetItemState(m_ciLeftClickDown.m_iRow, m_ciLeftClickDown.m_iCol) & GVNI_SELECTED))
    {
        m_iMouseMode = MOUSE_PREPARE_DRAG;
        return FALSE;
    }
#endif

    SetCapture();

    return TRUE;
}

void CGridCtrl::LBD_ColDivide(CPoint& ptOpr)
{
    CPoint  ptStart;
    CRect   rcTmp;
    CRect   rcInverted;
    CDC     *pDC    = NULL;

    m_iMouseMode = MOUSE_SIZING_COL;

    COMP_BFALSE(GetCellOrigin(0, m_ciLeftClickDown.m_iCol, &ptStart));

    GetClientRect(&rcTmp);

    pDC = GetDC();
    if (pDC)
    {
        rcInverted.SetRect(ptOpr.x, rcTmp.top, ptOpr.x + 2, rcTmp.bottom);
        pDC->InvertRect(&rcInverted);
        ReleaseDC(pDC);
    }

    // clicked right of border
    if (ptOpr.x - ptStart.x <= m_iResizeCaptureRange)
    {
        COMP_BFALSE(GetCellOrigin(0, --m_ciLeftClickDown.m_iCol, &ptStart));
    }

    rcTmp.left = ptStart.x;
    ClientToScreen(rcTmp);
#ifndef _WIN32_WCE_NO_CURSOR
    ClipCursor(rcTmp);
#endif

    return;
}

void CGridCtrl::LBD_RowDivide(CPoint& ptOpr)
{
    CPoint  ptStart;
    CRect   rcTmp;
    CRect   rcInverted;
    CDC     *pDC    = NULL;
    
    m_iMouseMode = MOUSE_SIZING_ROW;

    COMP_BFALSE(GetCellOrigin(m_ciLeftClickDown, &ptStart));

    GetClientRect(&rcTmp);

    pDC = GetDC();
    if (pDC)
    {
        rcInverted.SetRect(rcTmp.left, ptOpr.y, rcTmp.right, ptOpr.y + 2);
        pDC->InvertRect(&rcInverted);
        ReleaseDC(pDC);
    }

    // clicked below border
    if (ptOpr.y - ptStart.y <= m_iResizeCaptureRange)
    {
        COMP_BFALSE(GetCellOrigin(--m_ciLeftClickDown.m_iRow, 0, &ptStart));
    }

    rcTmp.top = ptStart.y;
    ClientToScreen(rcTmp);
#ifndef _WIN32_WCE_NO_CURSOR
    ClipCursor(rcTmp);
#endif

    return;
}

void CGridCtrl::LBD_Select(UINT nFlags)
{
    DWORD       dwKey   = 0;
    CCellID     ciCell;
    POSITION    pos;

    // If Ctrl pressed, save the current cell selection. This will get added
    // to the new cell selection at the end of the cell selection process
    m_tPrevSelectedCellMap.RemoveAll();
    if (nFlags & MK_CONTROL)
    {
        for (pos = m_tSelectedCellMap.GetStartPosition(); pos != NULL; )
        {
            m_tSelectedCellMap.GetNextAssoc(pos, dwKey, (CCellID&)ciCell);
            m_tPrevSelectedCellMap.SetAt(dwKey, ciCell);
        }
    }

    if (m_ciLeftClickDown.m_iRow < GetFixedRowCount())
    {
        OnFixedRowClick(m_ciLeftClickDown);
    }
    else if (m_ciLeftClickDown.m_iCol < GetFixedColumnCount())
    {
        OnFixedColumnClick(m_ciLeftClickDown);
    }
    else
    {
        m_iMouseMode = m_bListMode ? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
        OnSelecting(m_ciLeftClickDown);
    }

    m_iTimerID = SetTimer(WM_LBUTTONDOWN, m_iTimerInterval, 0);
}

void CGridCtrl::OnLButtonDown(UINT nFlags, CPoint ptOpr)
{
    COMP_BFALSE(LBD_UpdateStatus(nFlags, ptOpr));

    if (m_iMouseMode == MOUSE_NOTHING)
    {
        UpdateMouseMode(ptOpr);
    }

    if (m_iMouseMode == MOUSE_OVER_COL_DIVIDE) // sizing column
    {
        LBD_ColDivide(ptOpr);
    }
    else if (m_iMouseMode == MOUSE_OVER_ROW_DIVIDE) // sizing row
    {
        LBD_RowDivide(ptOpr);
    }
    else // not sizing or editing -- selecting
    {
        LBD_Select(nFlags);
    }
    m_ptLastMouse = ptOpr;
}

BOOL CGridCtrl::LBU_SizeCol(CPoint ptOpr)
{
    CRect   rcTmp;
    CRect   rcInverted;
    CPoint  ptStart;
    CDC     *pDC    = NULL;

    GetClientRect(&rcTmp);
    rcInverted.SetRect(m_ptLastMouse.x, rcTmp.top, m_ptLastMouse.x + 2, rcTmp.bottom);

    pDC = GetDC();
    if (pDC)
    {
        pDC->InvertRect(&rcInverted);
        ReleaseDC(pDC);
    }

    COMP_BE_R(m_ptLeftClickDown, ptOpr, TRUE);

    /* m_ptLeftClickDown != ptOpr */
    COMP_BFALSE_R(GetCellOrigin(m_ciLeftClickDown, &ptStart), FALSE);

    SetColumnWidth(m_ciLeftClickDown.m_iCol, ptOpr.x - ptStart.x);
    ResetScrollBars();
    Invalidate();

    return TRUE;
}

BOOL CGridCtrl::LBU_SizeRow(CPoint ptOpr)
{
    CRect   rcTmp;
    CRect   rcInverted;
    CPoint  ptStart;
    CDC     *pDC    = NULL;

    GetClientRect(&rcTmp);
    rcInverted.SetRect(rcTmp.left, m_ptLastMouse.y, rcTmp.right, m_ptLastMouse.y + 2);

    pDC = GetDC();
    if (pDC)
    {
        pDC->InvertRect(&rcInverted);
        ReleaseDC(pDC);
    }

    COMP_BE_R(m_ptLeftClickDown, ptOpr, TRUE);

    /* m_ptLeftClickDown != ptOpr */
    COMP_BFALSE_R(GetCellOrigin(m_ciLeftClickDown, &ptStart), FALSE);
    
    SetRowHeight(m_ciLeftClickDown.m_iRow, ptOpr.y - ptStart.y);
    ResetScrollBars();
    Invalidate();

    return TRUE;
}

void CGridCtrl::OnLButtonUp(UINT nFlags, CPoint ptOpr)
{
    BOOL    bRes    = FALSE;
    CWnd    *pOwner = NULL;

    CWnd::OnLButtonUp(nFlags, ptOpr);

#ifndef _WIN32_WCE_NO_CURSOR
    ClipCursor(NULL);
#endif

    if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
    {
        ReleaseCapture();
        KillTimer(m_iTimerID);
        m_iTimerID = 0;
    }

    // m_iMouseMode == MOUSE_PREPARE_EDIT only if user clicked down on current cell
    // and then didn't move mouse before clicking up (releasing button)
    if (m_iMouseMode == MOUSE_PREPARE_EDIT)
    {
        OnEditCell(m_ciCurrent.m_iRow, m_ciCurrent.m_iCol, VK_LBUTTON);
    }
#ifndef GRIDCONTROL_NO_DRAGDROP
    // m_iMouseMode == MOUSE_PREPARE_DRAG only if user clicked down on a selected cell
    // and then didn't move mouse before clicking up (releasing button)
    else if (m_iMouseMode == MOUSE_PREPARE_DRAG)
    {
        ResetSelectedRange();
    }
#endif
    else if (m_iMouseMode == MOUSE_SIZING_COL)
    {
        bRes = LBU_SizeCol(ptOpr);
        COMP_BFALSE(bRes);
    }
    else if (m_iMouseMode == MOUSE_SIZING_ROW)
    {
        bRes = LBU_SizeRow(ptOpr);
        COMP_BFALSE(bRes);
    }

    m_iMouseMode = MOUSE_NOTHING;

#ifndef _WIN32_WCE_NO_CURSOR
    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
#endif

    COMP_BFALSE(IsValid(m_ciLeftClickDown));

    pOwner = GetOwner();
    ASSERT_NULL(pOwner);

    if (IsWindow(pOwner->m_hWnd))
    {
        pOwner->PostMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), BN_CLICKED),
                            (LPARAM) GetSafeHwnd());
    }
}

#if SHOWGRIDCTRL  //!defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
/////////////////////////////////////////////////////////////////////////////
// CGridCtrl printing

void CGridCtrl::Print()
{
    UINT        nPage       = 0;
    BOOL        bPrintingOK = FALSE;
    CDC         dc;
    CString     strTitle;
    DOCINFO     di;                                 // Initialise print doc details
    CPrintInfo  Info;
    CPrintDialog printDlg(FALSE);

    // Get printer settings from user
    COMP_BNE(printDlg.DoModal(), IDOK);

    dc.Attach(printDlg.GetPrinterDC());         // attach a printer DC
    dc.m_bPrinting = TRUE;

    strTitle.LoadString(AFX_IDS_APP_TITLE);

    memset(&di, 0, sizeof (DOCINFO));
    di.cbSize = sizeof (DOCINFO);
    di.lpszDocName = strTitle;

    bPrintingOK = dc.StartDoc(&di);        // Begin a new print job

    Info.m_rectDraw.SetRect(0,0, dc.GetDeviceCaps(HORZRES), dc.GetDeviceCaps(VERTRES));

    OnBeginPrinting(&dc, &Info);                // Initialise printing
    for (nPage = Info.GetMinPage(); nPage <= Info.GetMaxPage() && bPrintingOK; nPage++)
    {
        dc.StartPage();                         // begin new page
        Info.m_nCurPage = nPage;
        OnPrint(&dc, &Info);                    // Print page
        bPrintingOK = (dc.EndPage() > 0);       // end page
    }
    OnEndPrinting(&dc, &Info);                  // Clean up after printing

    if (bPrintingOK)
        dc.EndDoc();                            // end a print job
    else
        dc.AbortDoc();                          // abort job.

    dc.Detach();                                // detach the printer DC
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl printing overridables - for Doc/View print/print preview framework

void CGridCtrl::OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo)
{
    int     iMaxRowCount = 0;
    int     iFontSize    = -9;
    int     iMargins     = 0;
    int     iTotalRowHeight = 0;
    int     iNumPages    = 1;
    int     iRow         = 0;
    CDC     *pCurrentDC  = NULL;
    CSize   szPaperPixelsPerInch;
    CSize   szScreenPixelsPerInch;
    CString strAlphabet;
    CString strFontName;
    CFont   *pOldFont    = NULL;

    // OnBeginPrinting() is called after the user has committed to
    // printing by OK'ing the Print dialog, and after the framework
    // has created a CDC object for the printer or the preview view.

    // This is the right opportunity to set up the page range.
    // Given the CDC object, we can determine how many rows will
    // fit on a page, so we can in turn determine how many printed
    // pages represent the entire document.

    ASSERT(pDC && pInfo);
    ASSERT_NULL(pDC);
    ASSERT_NULL(pInfo);

    iMaxRowCount = GetRowCount() - GetFixedRowCount();
    COMP_BE(iMaxRowCount, 0);

    // Get a DC for the current window (will be a screen DC for print previewing)
    pCurrentDC = GetDC();        // will have dimensions of the client area
    ASSERT_NULL(pCurrentDC);

    szPaperPixelsPerInch = CSize(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
    szScreenPixelsPerInch= CSize(pCurrentDC->GetDeviceCaps(LOGPIXELSX), pCurrentDC->GetDeviceCaps(LOGPIXELSY));

    // Create the printer font
    strFontName.Format(IDS_STRFONTTIMESNR);
    m_ftPrinter.CreateFont(iFontSize, 0,0,0, FW_NORMAL, 0,0,0, DEFAULT_CHARSET,
                             OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH | FF_DONTCARE, strFontName);

    pOldFont = pDC->SelectObject(&m_ftPrinter);

    // Get the average character width (in GridCtrl units) and hence the margins
    strAlphabet.Format(IDS_STRCGCALPHABET);
    m_szChar = pDC->GetTextExtent(strAlphabet, 52);
    m_szChar.cx /= 52;
    iMargins = (LEFT_MARGIN+RIGHT_MARGIN)*m_szChar.cx;

    // Get the page sizes (physical and logical)
    m_szPaper = CSize(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
    m_szLogicalPage.cx = GetVirtualWidth()+iMargins;

#ifdef _WIN32_WCE
    m_szLogicalPage.cy = (m_szLogicalPage.cx * m_szPaper.cy) / m_szPaper.cx;
#else
    m_szLogicalPage.cy = MulDiv(m_szLogicalPage.cx, m_szPaper.cy, m_szPaper.cx);
#endif

    m_iPageHeight = m_szLogicalPage.cy - GetFixedRowHeight()
                       - (HEADER_HEIGHT+FOOTER_HEIGHT + 2*GAP)*m_szChar.cy;

    // Get the number of pages. Assumes no row is bigger than the page size.
    for (iRow = GetFixedRowCount(); iRow < GetRowCount(); iRow++)
    {
        iTotalRowHeight += GetRowHeight(iRow);
        if (iTotalRowHeight > m_iPageHeight)
        {
            iNumPages++;
            iTotalRowHeight = GetRowHeight(iRow);
        }
    }

    // Set up the print info
    pInfo->SetMaxPage(iNumPages);
    pInfo->m_nCurPage = 1;                        // start printing at page# 1

    ReleaseDC(pCurrentDC);
    pDC->SelectObject(pOldFont);
}

void CGridCtrl::SetPrintInfo(CDC *pDC, CPrintInfo *pInfo)
{
    ASSERT_NULL(pDC);
    ASSERT_NULL(pInfo);

    // Set the page map mode to use GridCtrl units, and setup margin
    pDC->SetMapMode(MM_ANISOTROPIC);
    pDC->SetWindowExt(m_szLogicalPage);
    pDC->SetViewportExt(m_szPaper);
    pDC->SetWindowOrg(-LEFT_MARGIN*m_szChar.cx, 0);

    // Header
    pInfo->m_rectDraw.top    = 0;
    pInfo->m_rectDraw.left   = 0;
    pInfo->m_rectDraw.right  = m_szLogicalPage.cx - (LEFT_MARGIN+RIGHT_MARGIN)*m_szChar.cx;
    pInfo->m_rectDraw.bottom = HEADER_HEIGHT*m_szChar.cy;
    PrintHeader(pDC, pInfo);
    pDC->OffsetWindowOrg(0, -HEADER_HEIGHT*m_szChar.cy);

    // Gap between header and column headings
    pDC->OffsetWindowOrg(0, -GAP*m_szChar.cy);
}

int CGridCtrl::FindStartPrintRow(CPrintInfo *pInfo)
{
    int     iTotalRowHeight = 0;
    int     iCurPrintRow    = 0;
    UINT    nNumPages       = 1;

    ASSERT_NULL_R(pInfo, -1);

    iCurPrintRow = GetFixedRowCount();

    while (iCurPrintRow < GetRowCount() && nNumPages < pInfo->m_nCurPage)
    {
        iTotalRowHeight += GetRowHeight(iCurPrintRow);
        if (iTotalRowHeight > m_iPageHeight)
        {
            nNumPages++;
            if (nNumPages == pInfo->m_nCurPage)
                break;
            iTotalRowHeight = GetRowHeight(iCurPrintRow);
        }
        iCurPrintRow++;
    }

    COMP_BGE_R(iCurPrintRow, GetRowCount(), -1);

    return iCurPrintRow;
}

void CGridCtrl::PrintHorzLine(CDC *pDC, int iCol, int iCurPrintRow, CRect rectDraw)
{
    int     iOverlap= 0;

    ASSERT_NULL(pDC);
    COMP_BE(m_iGridLines, GVL_NONE);
    COMP_BE(m_iGridLines, GVL_VERT);

    iOverlap = (iCol == 0)? 0:1;
    pDC->MoveTo(rectDraw.left-iOverlap, rectDraw.bottom);
    pDC->LineTo(rectDraw.right, rectDraw.bottom);
    if (iCurPrintRow == 0)
    {
        pDC->MoveTo(rectDraw.left-iOverlap, rectDraw.top);
        pDC->LineTo(rectDraw.right, rectDraw.top);
    }
}

void CGridCtrl::PrintVertLine(CDC *pDC, int iCol, BOOL bFirstRow, CRect rectDraw)
{
    int     iOverlap= 0;

    ASSERT_NULL(pDC);
    COMP_BE(m_iGridLines, GVL_NONE);
    COMP_BE(m_iGridLines, GVL_HORZ);

    iOverlap = (bFirstRow)? 0:1;
    pDC->MoveTo(rectDraw.right, rectDraw.top-iOverlap);
    pDC->LineTo(rectDraw.right, rectDraw.bottom);
    if (iCol == 0)
    {
        pDC->MoveTo(rectDraw.left, rectDraw.top-iOverlap);
        pDC->LineTo(rectDraw.left, rectDraw.bottom);
    }
}

void CGridCtrl::PrintGridRow(CDC *pDC, int iCurPrintRow)
{
    BOOL    bFirstPrintedRow = TRUE;
    CRect   rcTmp;
    int     iCol    = 0;
    int     iOverlap= 0;

    ASSERT_NULL(pDC);
    COMP_BL(iCurPrintRow, 0);

    rcTmp.bottom = -1;
    while (iCurPrintRow < GetRowCount())
    {
        rcTmp.top    = rcTmp.bottom+1;
        rcTmp.bottom = rcTmp.top + GetRowHeight(iCurPrintRow) - 1;

        // Gone past end of page
        COMP_BG(rcTmp.bottom, m_iPageHeight);

        rcTmp.right  = -1;
        for (iCol = 0; iCol < GetColumnCount(); iCol++)
        {
            rcTmp.left  = rcTmp.right+1;
            rcTmp.right = rcTmp.left + GetColumnWidth(iCol) - 1;

            DrawCell(pDC, iCurPrintRow, iCol, rcTmp);

            PrintHorzLine(pDC, iCol, iCurPrintRow, rcTmp);
            PrintVertLine(pDC, iCol, bFirstPrintedRow, rcTmp);
        }
        iCurPrintRow++;
        bFirstPrintedRow = FALSE;
    }
}

void CGridCtrl::OnPrint(CDC *pDC, CPrintInfo *pInfo)
{
    int     iCurPrintRow = 0;
    CFont   *pOldFont    = NULL;

    ASSERT_NULL(pDC);
    ASSERT_NULL(pInfo);

    //CRect rcPage(pInfo->m_rectDraw);
    pOldFont = pDC->SelectObject(&m_ftPrinter);

    SetPrintInfo(pDC, pInfo);

    // Print the column headings
    pInfo->m_rectDraw.bottom = GetFixedRowHeight();
    PrintColumnHeadings(pDC, pInfo);
    pDC->OffsetWindowOrg(0, -GetFixedRowHeight());

    // We need to find out which row to start printing for this page.
    iCurPrintRow = FindStartPrintRow(pInfo);
    COMP_BL(iCurPrintRow, 0);

    // Draw as many rows as will fit on the printed page.
    // Clip the printed page so that there is no partially shown
    // row at the bottom of the page (the same row which will be fully
    // shown at the top of the next page).
    PrintGridRow(pDC, iCurPrintRow);

    // Footer
    pInfo->m_rectDraw.bottom = FOOTER_HEIGHT*m_szChar.cy;
    pDC->SetWindowOrg(-LEFT_MARGIN*m_szChar.cx, -m_szLogicalPage.cy + FOOTER_HEIGHT*m_szChar.cy);
    PrintFooter(pDC, pInfo);

    // SetWindowOrg back for next page
    pDC->SetWindowOrg(0,0);

    pDC->SelectObject(pOldFont);
}

void CGridCtrl::DrawHorzGridLine(CDC *pDC, int iRow, int iCol, CRect rcCell)
{
    int     iOverlap    = 0;

    ASSERT_NULL(pDC);
    COMP_BE(m_iGridLines, GVL_NONE);
    COMP_BE(m_iGridLines, GVL_VERT);

    iOverlap = (iCol == 0)? 0:1;
    pDC->MoveTo(rcCell.left-iOverlap, rcCell.bottom);
    pDC->LineTo(rcCell.right, rcCell.bottom);

    COMP_BNE(iRow, 0);

    /* iRow == 0 */
    pDC->MoveTo(rcCell.left-iOverlap, rcCell.top);
    pDC->LineTo(rcCell.right, rcCell.top);
}

void CGridCtrl::DrawVertGridLine(CDC *pDC, int iRow, int iCol, CRect rcCell)
{
    int     iOverlap    = 0;

    ASSERT_NULL(pDC);
    COMP_BE(m_iGridLines, GVL_NONE);
    COMP_BE(m_iGridLines, GVL_HORZ);

    iOverlap = (iRow == 0)? 0:1;
    pDC->MoveTo(rcCell.right, rcCell.top-iOverlap);
    pDC->LineTo(rcCell.right, rcCell.bottom);

    COMP_BNE(iCol, 0);

    /* iCol == 0 */
    pDC->MoveTo(rcCell.left, rcCell.top-iOverlap);
    pDC->LineTo(rcCell.left, rcCell.bottom);
}

void CGridCtrl::PrintColumnHeadings(CDC *pDC, CPrintInfo* /*pInfo*/)
{
    int     iRow        = 0;
    int     iCol        = 0;
    int     iOverlap    = 0;
    CRect   rcTmp;
    CFont   *pOldFont   = NULL;

    ASSERT_NULL(pDC);

    pOldFont = pDC->SelectObject(&m_ftPrinter);

    rcTmp.bottom = -1;
    for (iRow = 0; iRow < GetFixedRowCount(); iRow++)
    {
        rcTmp.top    = rcTmp.bottom+1;
        rcTmp.bottom = rcTmp.top + GetRowHeight(iRow) - 1;

        rcTmp.right = -1;

        for (iCol = 0; iCol < GetColumnCount(); iCol++)
        {
            rcTmp.left  = rcTmp.right+1;
            rcTmp.right = rcTmp.left + GetColumnWidth(iCol) - 1;

            DrawFixedCell(pDC, iRow, iCol, rcTmp);

            DrawHorzGridLine(pDC, iRow, iCol, rcTmp);

            DrawVertGridLine(pDC, iRow, iCol, rcTmp);
        }
    }

    pDC->SelectObject(pOldFont);
}

void CGridCtrl::PrintHeader(CDC *pDC, CPrintInfo *pInfo)
{
    CRect   rcTmp(pInfo->m_rectDraw);
    CString strHeaderString;
    int     iPrevBkMode  = 0;
    CFont   BoldFont;
    CFont   *pNormalFont = NULL;
    CWnd    *pParentWnd  = NULL;
    LOGFONT lf;

    //create bold font for header and footer
    VERIFY(m_ftPrinter.GetLogFont(&lf));
    lf.lfWeight = FW_BOLD;
    VERIFY(BoldFont.CreateFontIndirect(&lf));

    pNormalFont = pDC->SelectObject(&BoldFont);
    iPrevBkMode = pDC->SetBkMode(TRANSPARENT);

    // print App title on top right margin
    strHeaderString.LoadString(AFX_IDS_APP_TITLE);
    pDC->DrawText(strHeaderString, &rcTmp, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);

    // print parent window title in the centre (Gert Rijs)
    pParentWnd = GetParent();
    while (pParentWnd)
    {
        pParentWnd->GetWindowText(strHeaderString);
        if (strHeaderString.GetLength())  // can happen if it is a CView, CChildFrm has the title
            break;
        pParentWnd = pParentWnd->GetParent();
    }
    pDC->DrawText(strHeaderString, &rcTmp, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);


    pDC->SetBkMode(iPrevBkMode);
    pDC->SelectObject(pNormalFont);
    BoldFont.DeleteObject();

    pDC->SelectStockObject(BLACK_PEN);
    pDC->MoveTo(rcTmp.left, rcTmp.bottom);
    pDC->LineTo(rcTmp.right, rcTmp.bottom);
}

//print footer with a line and date, and page number
void CGridCtrl::PrintFooter(CDC *pDC, CPrintInfo *pInfo)
{
    CRect   rcTmp(pInfo->m_rectDraw);
    int     iPrevBkMode  = 0;
    CFont   BoldFont;
    CFont   *pNormalFont = NULL;
    LOGFONT lf;
    CString strTmp;
    CTime   t;

    //draw line
    pDC->MoveTo(rcTmp.left, rcTmp.top);
    pDC->LineTo(rcTmp.right, rcTmp.top);

    //create bold font for header and footer
    m_ftPrinter.GetLogFont(&lf);
    lf.lfWeight = FW_BOLD;
    BoldFont.CreateFontIndirect(&lf);

    pNormalFont = pDC->SelectObject(&BoldFont);
    iPrevBkMode = pDC->SetBkMode(TRANSPARENT);

    // draw page number
    rcTmp.OffsetRect(0, m_szChar.cy/2);
    strTmp.Format(IDS_STRCGCPAGENUM, pInfo->m_nCurPage, pInfo->GetMaxPage());
    pDC->DrawText(strTmp,-1,rcTmp, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

    t = CTime::GetCurrentTime();
    strTmp = t.Format(_T("%c"));
    pDC->DrawText(strTmp,-1,rcTmp, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

    pDC->SetBkMode(iPrevBkMode);
    pDC->SelectObject(pNormalFont);
    BoldFont.DeleteObject();
}

void CGridCtrl::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    m_ftPrinter.DeleteObject();
}

#endif  // !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)


#ifndef _WIN32_WCE
/////////////////////////////////////////////////////////////////////////////
// CGridCtrl persistance

BOOL CGridCtrl::Save(LPCTSTR strFileName)
{
    int             i        = 0;
    int             j        = 0;
    int             iNumCols = 0;
    CStdioFile      sfFile;
    CFileException  ex;

    if (!sfFile.Open(strFileName, CFile::modeWrite | CFile::modeCreate| CFile::typeText, &ex))
    {
        ex.ReportError();
        return FALSE;
    }

    TRY
    {
        iNumCols = GetColumnCount();
        for (i = 0; i < iNumCols-1; i++)
        {
            sfFile.WriteString(GetItemText(0,i));
            //sfFile.WriteString((i==(iNumCols-1))? _T("\n"):_T(","));
            sfFile.WriteString(_T(","));
        }
        /* iNumCols-1 */
        sfFile.WriteString(GetItemText(0,i));
        sfFile.WriteString(_T("\n"));

        for (i = 0; i < GetRowCount(); i++)
        {
            for (j = 0; j < iNumCols-1; j++)
            {
                sfFile.WriteString(GetItemText(i,j));
                //sfFile.WriteString((j==(iNumCols-1))? _T("\n"):_T(","));
                sfFile.WriteString(_T(","));
            }

            /* iNumCols-1 */
            sfFile.WriteString(GetItemText(i,j));
            sfFile.WriteString(_T("\n"));
        }

        sfFile.Close();
    }

    CATCH (CFileException, e)
    {
        AfxMessageBox(IDS_STRINFSAVEGRIDFAIL);
        e->Delete();
        return FALSE;
    }
    END_CATCH

    return TRUE;
}

BOOL CGridCtrl::Load(LPCTSTR strFileName)
{
    int             iItem   = 0;
    int             iSubItem= 0;
    TCHAR           *pToken = NULL;
    TCHAR           *pEnd   = NULL;
    TCHAR           pBuffer[1024];
    CStdioFile      sfFile;
    CFileException  ex;

    if (!sfFile.Open(strFileName, CFile::modeRead | CFile::typeText))
    {
        ex.ReportError();
        return FALSE;
    }

    DeleteAllItems();

    TRY
    {
        // Read Header off file
        sfFile.ReadString(pBuffer, 1024);

        // Get first token
        pToken = pBuffer;
        pEnd   = pBuffer;
        GETNEXTTOKEN();
/*
        for (pToken=pBuffer, pEnd=pBuffer;
             *pEnd && (*pEnd != _T(',')) && (*pEnd != _T('\n'));
             pEnd++);
        if ((*pEnd == _T('\0')) && (pToken == pEnd))
            pToken = NULL;
        *pEnd = _T('\0');
*/
        while (pToken)
        {
            InsertColumn(pToken);

            // Get next token
            pToken=++pEnd;
            GETNEXTTOKEN();
/*
            for (pToken=++pEnd; *pEnd && (*pEnd != _T(',')) && (*pEnd != _T('\n'));
                 pEnd++);
            if ((*pEnd == _T('\0')) && (pToken == pEnd))
                pToken = NULL;
            *pEnd = _T('\0');
*/
        }

        // Read in rest of data
        while (sfFile.ReadString(pBuffer, 1024))
        {
            // Get first token
            pToken = pBuffer;
            pEnd   = pBuffer;
            GETNEXTTOKEN();
/*
            for (pToken=pBuffer, pEnd=pBuffer;
                 *pEnd && (*pEnd != _T(',')) && (*pEnd != _T('\n')); pEnd++);
            if ((*pEnd == _T('\0')) && (pToken == pEnd))
                pToken = NULL;
            *pEnd = _T('\0');
*/
            iSubItem = 0;
            while (pToken)
            {
                if (!iSubItem)
                    InsertRow(pToken);
                else
                    SetItemText(iItem, iSubItem, pToken);

                // Get next token
                pToken=++pEnd;
                GETNEXTTOKEN();
/*
                for (pToken=++pEnd; *pEnd && (*pEnd != _T(',')) && (*pEnd != _T('\n'));
                     pEnd++);
                if ((*pEnd == _T('\0')) && (pToken == pEnd))
                    pToken = NULL;
                *pEnd = _T('\0');
*/
                iSubItem++;
            }
            iItem++;
        }

        AutoSizeColumns();
        sfFile.Close();
    }

    CATCH (CFileException, e)
    {
        AfxMessageBox(IDS_STRINFLOADGRIDFAIL);
        e->Delete();
        return FALSE;
    }
    END_CATCH

    return TRUE;
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl overrideables

#ifndef GRIDCONTROL_NO_DRAGDROP
// This is no longer needed since I've changed to OLE drag and drop - but it's
// still cool code. :)
CImageList* CGridCtrl::CreateDragImage(CPoint *pHotSpot)
{
    CRect       rcTmp;
    CCellID     ciCell;
    CDC         MemDC;
    CBitmap     bm;
    CDC         *pDC        = NULL;
    CImageList  *pList      = NULL;
    CBitmap     *pOldBitmap = NULL;

    ASSERT_NULL_R(pHotSpot, NULL);

    pDC = GetDC();
    ASSERT_NULL_R(pDC, NULL);

    ciCell = GetFocusCell();
    COMP_BFALSE_R(GetCellRect(ciCell.m_iRow, ciCell.m_iCol, rcTmp), NULL);

    // Translate coordinate system
    rcTmp.BottomRight() = CPoint(rcTmp.Width(), rcTmp.Height());
    rcTmp.TopLeft()     = CPoint(0,0);
    *pHotSpot = rcTmp.BottomRight();

    // Create a new imagelist (the caller of this function has responsibility
    // for deleting this list)
    pList = new CImageList;
    ASSERT_NULL_R(pList, NULL);

    if (!pList->Create(rcTmp.Width(), rcTmp.Height(), ILC_MASK, 1,1))
    {
        delete pList;
        return NULL;
    }

    // Create mem DC and bitmap
    MemDC.CreateCompatibleDC(pDC);
    bm.CreateCompatibleBitmap(pDC, rcTmp.Width(), rcTmp.Height());
    pOldBitmap = MemDC.SelectObject(&bm);
    MemDC.SetWindowOrg(0,0);

    // Draw cell onto bitmap in memDC
    DrawCell(&MemDC, ciCell.m_iRow, ciCell.m_iCol, rcTmp, TRUE);

    // Clean up
    MemDC.SelectObject(pOldBitmap);
    ReleaseDC(pDC);

    // Add the bitmap we just drew to the image list.
    pList->Add(&bm, GetTextBkColor());
    bm.DeleteObject();

    return pList;
}
#endif

void CGridCtrl::OnFixedRowClick(CCellID& ciCell)
{
    COMP_BFALSE(IsValid(ciCell));

    if (m_bListMode)
    {
        COMP_BFALSE(m_bSortOnClick);

        if (ciCell.m_iCol == m_iSortCol)
            m_bAscending = !m_bAscending;
        else
        {
            m_bAscending = TRUE;
            m_iSortCol = ciCell.m_iCol;
        }
        SortTextItems(m_iSortCol, m_bAscending);
        Invalidate();
    }
    else if (ciCell.m_iCol < GetFixedColumnCount())
    {
        m_iMouseMode = MOUSE_SELECT_ALL;
        OnSelecting(ciCell);
    }
    else
    {
        m_iMouseMode = MOUSE_SELECT_COL;
        OnSelecting(ciCell);
    }
}

void CGridCtrl::OnFixedColumnClick(CCellID& ciCell)
{
    COMP_BFALSE(IsValid(ciCell));

//    if (m_bListMode && (GetItemState(cell.m_iRow, m_iFixedCols) & GVNI_SELECTED))
//    {
//        OnEditCell(cell.m_iRow, cell.m_iCol, VK_LBUTTON);
//        return;
//    }

    if (ciCell.m_iRow < GetFixedRowCount())
    {
        m_iMouseMode = MOUSE_SELECT_ALL;
        OnSelecting(ciCell);
    }
    else
    {
        m_iMouseMode = MOUSE_SELECT_ROW;
        OnSelecting(ciCell);
    }
}

// Gets the extent of the text pointed to by str (no CDC needed)
// By default this uses the selected font (which is a bigger font)
CSize CGridCtrl::GetTextExtent(LPCTSTR str, BOOL bUseSelectedFont /* = TRUE */)
{
    LOGFONT lf;
    CFont   ftIndirect;
    CSize   szText;
    CDC     *pDC        = NULL;
    CFont   *pOldFont   = NULL;

    pDC = GetDC();
    ASSERT_NULL_R(pDC, CSize(0,0));

    if (bUseSelectedFont)
    {
        memcpy(&lf, &m_Logfont, sizeof(LOGFONT));
        lf.lfWeight = SELECTED_CELL_FONT_WEIGHT;

        ftIndirect.CreateFontIndirect(&lf);

        pOldFont = pDC->SelectObject(&ftIndirect);
    }
    else
        pOldFont = pDC->SelectObject(&m_ftGrid);

    szText = pDC->GetTextExtent(str);
    pDC->SelectObject(pOldFont);
    ReleaseDC(pDC);

    return szText + CSize(2*m_iMargin, 2*m_iMargin);
}

CSize CGridCtrl::GetCellExtent(int iRow, int iCol, CDC* pDC)
{
    LOGFONT lf;
    CFont   font;
    CSize   szTextExtent;
    CSize   szImageSize;
    int     iImage      = 0;
    IMAGEINFO tImgInfo;
    CFont   *pOldFont   = NULL;
    LOGFONT *pLF        = NULL;

    ASSERT_NULL_R(pDC, CSize(0,0));

    pLF = GetItemFont(iRow, iCol);
    
    // use selected font since it's thicker
    memcpy(&lf, pLF, sizeof(LOGFONT));

    if (iRow < m_iFixedRows || iCol < m_iFixedCols)
        lf.lfWeight = SELECTED_CELL_FONT_WEIGHT;

    font.CreateFontIndirect(&lf);

    pOldFont = pDC->SelectObject(&font);
    szTextExtent = pDC->GetTextExtent(GetItemText(iRow, iCol));
    pDC->SelectObject(pOldFont);

    szTextExtent += CSize(4*m_iMargin, 2*m_iMargin);

    szImageSize = CSize(0,0);
    if (m_pImageList)
    {
        iImage = GetItemImage(iRow, iCol);
        if (iImage >= 0)
        {
            if (m_pImageList->GetImageInfo(iImage, &tImgInfo))
            {
                szImageSize = CSize(tImgInfo.rcImage.right -tImgInfo.rcImage.left+1,
                                    tImgInfo.rcImage.bottom-tImgInfo.rcImage.top +1);
            }
        }
    }

    return CSize(szTextExtent.cx + szImageSize.cx, max(szTextExtent.cy, szImageSize.cy));
}

/* added for combine cell */
BOOL CGridCtrl::DrawHideFixedCell(int &iRow, int &iCol, CRect& rcCell, GV_ITEM* pItem)
{
    int         iRightCol   = 0;
    int         iBottomRow  = 0;
    int         iMinVisRow  = 0;
    int         iMinVisCol  = 0;   
    CRect       rcTmp;
    CGridCell   *pgcCell    = NULL;
    CGridCell   *pgcTopLeft = NULL;
    CCellID     ciTopLeft;

    ASSERT_NULL_R(pItem, FALSE);

    //want remove it?
    pgcCell = GetCell(iRow,iCol);
    ASSERT_NULL_R(pgcCell, FALSE);

    if(!CELLHIDE(pgcCell))
    {
        pItem->iRow = iRow;
        pItem->iCol = iCol;
        return GetItem(pItem);
    }
    
    if(pgcCell->m_bHide == TRUE)
    {
        pgcTopLeft = GetCell(pgcCell->m_iTopleftRow, pgcCell->m_iTopleftCol);
        ASSERT_NULL_R(pgcCell, FALSE);

        iRow = pgcCell->m_iTopleftRow;
        iCol = pgcCell->m_iTopleftCol;
        iRightCol  = pgcCell->m_iTopleftCol + pgcTopLeft->m_iNumVhide;
        iBottomRow = pgcCell->m_iTopleftRow + pgcTopLeft->m_iNumHhide;
    }
    /* if( pgcCell->m_iNumHhide!=0 || pgcCell->m_iNumVhide!=0) */
    else //扩充左上角到合适的位置
    {
        iBottomRow = iRow + pgcCell->m_iNumHhide;
        iRightCol  = iCol + pgcCell->m_iNumVhide;
    }

    if(GetCellRect(iBottomRow,iRightCol,rcTmp))
    {
        rcCell.right   = rcTmp.right;
        rcCell.bottom  = rcTmp.bottom;

        ciTopLeft  = GetTopleftNonFixedCell();
        iMinVisRow = ciTopLeft.m_iRow;
        iMinVisCol = ciTopLeft.m_iCol;
        iMinVisRow = max(iMinVisRow, iRow);
        iMinVisCol = max(iMinVisCol, iCol);

        GetCellRect(iRow,iCol,rcTmp);
        if( iRightCol > GetFixedColumnCount())
        {
            GetCellRect(iRow,iMinVisCol,rcTmp);
        }
        else if(iBottomRow > GetFixedRowCount())
        {
            GetCellRect(iMinVisRow,iCol,rcTmp);
        }

        rcCell.left = rcTmp.left;
        rcCell.top  = rcTmp.top;
    }

    pItem->iRow = iRow;
    pItem->iCol = iCol;
    COMP_BFALSE_R(GetItem(pItem), FALSE);

    if( !(iBottomRow >GetFixedRowCount()))
        pItem->nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;

    return TRUE;
}

void CGridCtrl::DrawCellImage(CDC* pDC, GV_ITEM *pItem, CRect& rcCell)
{
    IMAGEINFO   tImgInfo;
    int         iImageWidth = 0;

    ASSERT_NULL(pDC);
    ASSERT_NULL(pItem);

    ASSERT_NULL(m_pImageList);
    COMP_BL(pItem->iImage, 0);

    if (m_pImageList->GetImageInfo(pItem->iImage, &tImgInfo))
    {
        iImageWidth = tImgInfo.rcImage.right-tImgInfo.rcImage.left+1;
        m_pImageList->Draw(pDC, pItem->iImage, rcCell.TopLeft(), ILD_NORMAL);
        rcCell.left += iImageWidth + m_iMargin;
    }

    return;
}
BOOL CGridCtrl::DrawFixedCell(CDC* pDC, int iRow, int iCol, CRect rcCell, BOOL bEraseBk)
{
    int         iSavedDC    = 0;
    GV_ITEM     tItem;
    CBrush      brBk;
    CCellID     ciFocus;
    CFont       ftTmp;
    static LOGFONT lf;
    CPen        penLight;
    CPen        penDark;
    LOGFONT     *pLF        = NULL;
    CPen        *pOldPen = NULL;

    ASSERT_NULL_R(pDC, FALSE);
    COMP_BFALSE_R(m_bAllowDraw, FALSE);

    tItem.nMask = GVIF_TEXT | GVIF_FORMAT | GVIF_IMAGE | GVIF_BKCLR | GVIF_FGCLR;

    COMP_BFALSE_R(DrawHideFixedCell(iRow, iCol, rcCell, &tItem), FALSE);

    // Force redraw of background if custom colour
    //if (Item.m_clrBk != CLR_DEFAULT)
    bEraseBk = TRUE;

    if (bEraseBk
#if SHOWGRIDCTRL  //!defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
        && !pDC->IsPrinting()
#endif
        )
    {
        brBk.CreateSolidBrush((tItem.clrBk == CLR_DEFAULT)? GetFixedBkColor() : tItem.clrBk);
        pDC->FillRect(rcCell, &brBk);
    }
    pDC->SetTextColor((tItem.clrFg == CLR_DEFAULT)? GetFixedTextColor() : tItem.clrFg);

    iSavedDC = pDC->SaveDC();

    // Create the appropriate font and select into DC
    pLF = GetItemFont(iRow, iCol);
    ciFocus = GetFocusCell();

    if (OVERLAP_CELL(ciFocus, iRow, iCol))
    {
        memcpy(&lf, pLF, sizeof(LOGFONT));
        lf.lfWeight = SELECTED_CELL_FONT_WEIGHT;
        pLF = &lf;
    }

    ftTmp.CreateFontIndirect(pLF);
    pDC->SelectObject(&ftTmp);

    if (IsValid(ciFocus) && (OVERLAP_CELL(ciFocus, iRow, iCol)))
    {
        rcCell.right++;
        rcCell.bottom++;
        pDC->DrawEdge(rcCell, BDR_SUNKENINNER /*EDGE_RAISED*/, BF_RECT);
        rcCell.DeflateRect(1,1);
    }
    else
    {
        penLight.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DHIGHLIGHT));
        penDark.CreatePen (PS_SOLID, 1, ::GetSysColor(COLOR_3DDKSHADOW));
        pOldPen = pDC->GetCurrentPen();

        pDC->SelectObject(&penLight);
        pDC->MoveTo(rcCell.right, rcCell.top);
        pDC->LineTo(rcCell.left, rcCell.top);
        pDC->LineTo(rcCell.left, rcCell.bottom);

        pDC->SelectObject(&penDark);
        pDC->MoveTo(rcCell.right, rcCell.top);
        pDC->LineTo(rcCell.right, rcCell.bottom);
        pDC->LineTo(rcCell.left, rcCell.bottom);

        pDC->SelectObject(pOldPen);
        rcCell.DeflateRect(1,1);
    }

    pDC->SetBkMode(TRANSPARENT);
    rcCell.DeflateRect(m_iMargin, 0);

    DrawCellImage(pDC, &tItem, rcCell);

    DrawText(pDC->m_hDC, tItem.szText, -1, rcCell, tItem.nFormat);

    pDC->RestoreDC(iSavedDC);
    ftTmp.DeleteObject();

    return TRUE;
}

/* added for combine cell */
void CGridCtrl::DrawHideCell(int &iRow, int &iCol, CRect& rcCell)
{
    int         iRightCol   = 0;
    int         iBottomRow  = 0;
    int         iMinVisRow  = 0;
    int         iMinVisCol  = 0;
    CRect       rcTmp;
    CGridCell   *pgcCell    = NULL;
    CGridCell   *pgcTopLeft = NULL;
    CCellID     ciTopLeft;
    
    //want remove it?

    //CCellRange  crCombine;
    //there is bug in the combining procedure if you want to use\
    //the GetCellRangeRect(),so i manpower adjust the "rcCell" variable
    pgcCell = GetCell(iRow,iCol);
    ASSERT_NULL(pgcCell);

    COMP_BFALSE(CELLHIDE(pgcCell));

    if(pgcCell->m_bHide == TRUE)
    {
        pgcTopLeft = GetCell(pgcCell->m_iTopleftRow, pgcCell->m_iTopleftCol);
        ASSERT_NULL(pgcCell);

        /*crCombine.Set(pgcCell->m_iTopleftRow,pgcCell->m_iTopleftCol,\
        pgcCell->m_iTopleftRow+pgcTopLeft->m_iNumHhide,\
        pgcCell->m_iTopleftCol+pgcTopLeft->m_iNumVhide);
        GetCellRangeRect(crCombine,rcCell);*/

        iRow = pgcCell->m_iTopleftRow;
        iCol = pgcCell->m_iTopleftCol;
        iRightCol  = pgcCell->m_iTopleftCol + pgcTopLeft->m_iNumVhide;
        iBottomRow = pgcCell->m_iTopleftRow + pgcTopLeft->m_iNumHhide;
    }
    /* if( pgcCell->m_iNumHhide!=0 || pgcCell->m_iNumVhide!=0) */
    else //扩充左上角到合适的位置
    {
        /*crCombine.Set(iRow,iCol,iRow+pgcCell->m_iNumHhide,iCol+pgcCell->m_iNumVhide);
        GetCellRangeRect(crCombine,rcCell);*/

        iBottomRow = iRow + pgcCell->m_iNumHhide;
        iRightCol  = iCol + pgcCell->m_iNumVhide;
    }

    if(GetCellRect(iBottomRow,iRightCol,rcTmp))
    {
        //if( rcCell.right != rcTmp.right)
        rcCell.right =  rcTmp.right;
        //if( rcCell.bottom != rcTmp.bottom)
        rcCell.bottom  = rcTmp.bottom;

        ciTopLeft  = GetTopleftNonFixedCell();
        iMinVisRow = ciTopLeft.m_iRow;
        iMinVisCol = ciTopLeft.m_iCol;
        iMinVisRow = max(iMinVisRow, iRow);
        iMinVisCol = max(iMinVisCol, iCol);
        GetCellRect(iMinVisRow, iMinVisCol, rcTmp);
        //if(rcCell.left<rcTmp.left)
        rcCell.left = rcTmp.left;
        //if(rcCell.top<rcTmp.top)
        rcCell.top = rcTmp.top;
    }
}

void CGridCtrl::DrawFocusedCell(CDC *pDC, CRect rcCell, BOOL bEraseBk,
                                COLORREF clrTextBk, COLORREF clrText)
{
    CBrush brTextBk(clrTextBk);

    ASSERT_NULL(pDC);

#if !SHOWGRIDCTRL // defined(_WIN32_WCE_NO_PRINTING) || defined(GRIDCONTROL_NO_PRINTING)
    return;
#endif

    COMP_BTRUE(pDC->IsPrinting());

    // FillRect doesn't draw RHS or bottom
    if (bEraseBk)
    {
        rcCell.right++; 
        rcCell.bottom++;
        pDC->FillRect(rcCell, &brTextBk);
        rcCell.right--;
        rcCell.bottom--;
    }
    pDC->SelectStockObject(BLACK_PEN);
    pDC->SelectStockObject(NULL_BRUSH);
    pDC->Rectangle(rcCell);
    rcCell.DeflateRect(1,1);
    pDC->SetTextColor(clrText);

    return;
}

void CGridCtrl::DrawSelectedCell(CDC *pDC, CRect rcCell)
{
    ASSERT_NULL(pDC);

#if !SHOWGRIDCTRL // defined(_WIN32_WCE_NO_PRINTING) || defined(GRIDCONTROL_NO_PRINTING)
    return;
#endif

    COMP_BTRUE(pDC->IsPrinting());

    // FillRect doesn't draw RHS or bottom
    rcCell.right++;
    rcCell.bottom++;
    pDC->FillSolidRect(rcCell, ::GetSysColor(COLOR_HIGHLIGHT));
    rcCell.right--;
    rcCell.bottom--;
    pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
}

void CGridCtrl::DrawNormalCell(CDC *pDC, CRect rcCell, BOOL bEraseBk,
                               COLORREF clrTextBk, COLORREF clrText)
{
    CBrush brTextBk(clrTextBk);

    ASSERT_NULL(pDC);

    // FillRect doesn't draw RHS or bottom
    if (bEraseBk)
    {
        rcCell.right++;
        rcCell.bottom++;
        pDC->FillRect(rcCell, &brTextBk);
        rcCell.right--;
        rcCell.bottom--;
    }
    pDC->SetTextColor(clrText);
}

void CGridCtrl::DrawDrophilitedCell(CDC *pDC, CRect rcCell)
{
    ASSERT_NULL(pDC);

#if !SHOWGRIDCTRL // defined(_WIN32_WCE_NO_PRINTING) || defined(GRIDCONTROL_NO_PRINTING)
    return;
#endif

    COMP_BTRUE(pDC->IsPrinting());

    pDC->SelectStockObject(BLACK_PEN);
    pDC->SelectStockObject(NULL_BRUSH);

    pDC->Rectangle(rcCell);
}

BOOL CGridCtrl::DrawCell(CDC* pDC, int iRow, int iCol, CRect rcCell, BOOL bEraseBk)
{
    GV_ITEM     tItem;
    COLORREF    clrTextBk;
    COLORREF    clrText;
    int         iSavedDC    = 0;
    LOGFONT     *pLF        = NULL;
    CFont       ftTmp;

    COMP_BFALSE_R(m_bAllowDraw, FALSE);
    ASSERT_NULL_R(pDC, FALSE);

    tItem.nMask = GVIF_TEXT | GVIF_FORMAT | GVIF_STATE | GVIF_IMAGE | GVIF_BKCLR | GVIF_FGCLR;

    /* update iRow, iCol, rcCell for Hide/ combine cell */
    DrawHideCell(iRow, iCol, rcCell);

    tItem.iRow = iRow;
    tItem.iCol = iCol;
    COMP_BFALSE_R(GetItem(&tItem), FALSE);

    clrTextBk = (tItem.clrBk == CLR_DEFAULT)? GetTextBkColor() : tItem.clrBk;
    clrText   = (tItem.clrFg == CLR_DEFAULT)? GetTextColor()   : tItem.clrFg;

    // Force redraw of background if custom colour
    //if (Item.m_clrBk != CLR_DEFAULT)
    bEraseBk = TRUE;

    iSavedDC = pDC->SaveDC();

    pDC->SetBkMode(TRANSPARENT);

    if (tItem.nState & GVIS_FOCUSED)
    {
        DrawFocusedCell(pDC, rcCell, bEraseBk, clrTextBk, clrText);
    }
    else if (tItem.nState & GVIS_SELECTED)
    {
        DrawSelectedCell(pDC, rcCell);
    }
    else
    {
        DrawNormalCell(pDC, rcCell, bEraseBk, clrTextBk, clrText);
    }

    if (tItem.nState & GVIS_DROPHILITED)
    {
        DrawDrophilitedCell(pDC, rcCell);
    }

    // Create the appropriate font and select into DC
    pLF = GetItemFont(iRow, iCol);

    ftTmp.CreateFontIndirect(pLF);
    pDC->SelectObject(&ftTmp);

    rcCell.DeflateRect(m_iMargin, 0);

    DrawCellImage(pDC, &tItem, rcCell);

    DrawText(pDC->m_hDC, tItem.szText, -1, rcCell, tItem.nFormat);

    pDC->RestoreDC(iSavedDC);
    ftTmp.DeleteObject();

    return TRUE;
}

BOOL CGridCtrl::EditCombineCell(int &iRow, int &iCol, CRect &rcCell)
{
    int         iRightCol   = 0;
    int         iBottomRow  = 0;
    int         iMinVisRow  = 0;
    int         iMinVisCol  = 0;
    CGridCell   *pgcCell    = NULL;
    CGridCell   *pgcTopLeft = NULL;
    CCellID     ciTopLeft;
    CCellRange  crTopLeft;
    CRect       rcCombine;
    CRect       rcTmp;

    //want remove it?
    pgcCell = GetCell(iRow,iCol);
    ASSERT_NULL_R(pgcCell, FALSE);

    COMP_BFALSE_R(CELLHIDE(pgcCell), TRUE);
   
    /* 如果被隐藏，显然应当编辑最左上角 */
    if(pgcCell->m_bHide)
    {
        pgcTopLeft = GetCell(pgcCell->m_iTopleftRow,pgcCell->m_iTopleftCol);
        ASSERT_NULL_R(pgcTopLeft, FALSE);

        crTopLeft.Set(pgcCell->m_iTopleftRow,
                      pgcCell->m_iTopleftCol,
                      pgcTopLeft->m_iNumHhide + pgcCell->m_iTopleftRow,
                      pgcTopLeft->m_iNumVhide+pgcCell->m_iTopleftCol);

        iRow  = pgcCell->m_iTopleftRow;
        iCol  = pgcCell->m_iTopleftCol;

    }
    else
    /* if(pgcCell->m_iNumHhide!=0||pgcCell->m_iNumVhide!=0) */
    {
        crTopLeft.Set(iRow,
                      iCol,
                      pgcCell->m_iNumHhide+iRow,
                      pgcCell->m_iNumVhide+iCol);
    }

    GetCellRangeRect(crTopLeft,rcCombine);
    rcCell = rcCombine;
    iBottomRow = iRow + pgcTopLeft->m_iNumHhide;
    iRightCol  = iCol + pgcTopLeft->m_iNumVhide;

    /* adjust the border; */
    if(GetCellRect(iBottomRow,iRightCol,rcTmp))
    {
        rcCell.right =rcTmp.right;
        rcCell.bottom=rcTmp.bottom;

        ciTopLeft  = GetTopleftNonFixedCell();
        iMinVisRow = ciTopLeft.m_iRow;
        iMinVisCol = ciTopLeft.m_iCol;
        iMinVisRow = max(iMinVisRow, iRow);
        iMinVisCol = max(iMinVisCol, iCol);
        GetCellRect(iMinVisRow,iMinVisCol,rcTmp);
        if( rcCell.left < rcTmp.left)
            rcCell.left = rcTmp.left;
        rcCell.left--;
        if( rcCell.top < rcTmp.top)
            rcCell.top = rcTmp.top;
        rcCell.top--;
    }

    return TRUE;
}

void CGridCtrl::OnEditCell(int iRow, int iCol, UINT nChar)
{
    CCellID     ciCell(iRow, iCol);
    CRect       rcTmp;
    BOOL        bEditable   = TRUE;
    BOOL        bRes        = FALSE;
    GV_ITEM     tItem;
    DWORD       dwStyle     = ES_LEFT;

    EnsureVisible(iRow, iCol);

    bEditable &= IsValid(ciCell);
    bEditable &= IsCellEditable(iRow, iCol);
    bEditable &= IsCellVisible(iRow, iCol);
    COMP_BFALSE(bEditable);
    // if (!IsValid(ciCell) || !IsCellEditable(iRow, iCol) || !IsCellVisible(iRow, iCol))
    //     return;

    bRes = GetCellRect(ciCell, rcTmp);
    COMP_BFALSE(bRes);

    //want remove it?
    bRes = EditCombineCell(iRow, iCol, rcTmp);
    COMP_BFALSE(bRes);

    //above added for combine cell
    SendMessageToParent(iRow, iCol, GVN_BEGINLABELEDIT);

    tItem.nMask = GVIF_TEXT | GVIF_FORMAT;
    tItem.iRow = iRow;
    tItem.iCol = iCol;
    bRes = GetItem(&tItem);
    COMP_BFALSE(bRes);

    dwStyle = ES_LEFT;
    if (tItem.nFormat & DT_RIGHT)
        dwStyle = ES_RIGHT;
    else if (tItem.nFormat & DT_CENTER)
        dwStyle = ES_CENTER;

    CreateInPlaceEditControl(rcTmp, dwStyle, IDC_INPLACE_CONTROL,
                             iRow, iCol, tItem.szText, nChar);
}


void CGridCtrl::CreateInPlaceEditControl(CRect& rc, DWORD dwStyle, UINT nID,
                                         int iRow, int iCol,
                                         LPCTSTR szText, UINT nChar)
{
    // InPlaceEdit auto-deletes itself
    new CInPlaceEdit(this, rc, dwStyle, nID, iRow, iCol, szText, nChar);
}

void CGridCtrl::OnEndEditCell(int iRow, int iCol, CString str)
{
    if (GetItemText(iRow,iCol) != str)
    {
        SetModified(TRUE, iRow, iCol);
        SetItemText(iRow, iCol, str);
    }
}

CString CGridCtrl::GetItemText(int iRow, int iCol)
{
    CGridCell   *pgcCell= NULL;

    COMP_BL_R(iRow, 0, "");
    COMP_BL_R(iCol, 0, "");
    COMP_BGE_R(iRow, m_iRows, "");
    COMP_BGE_R(iCol, m_iCols, "");

    pgcCell = GetCell(iRow, iCol);
    ASSERT_NULL_R(pgcCell, "");

    return pgcCell->m_szText;
}

CGridCell* CGridCtrl::GetCellP(int iRow, int iCol)
{
    GRID_ROW    *pRow = NULL;

    COMP_BL_R(iRow, 0, NULL);
    COMP_BL_R(iCol, 0, NULL);
    COMP_BGE_R(iRow, m_iRows, NULL);
    COMP_BGE_R(iCol, m_iCols, NULL);

    pRow = m_tRowData[iRow];
    ASSERT_NULL_R(pRow, NULL);

    return pRow->GetAt(iCol);
}

BOOL CGridCtrl::CheckHideCell(int iRow, int iCol, int iXNum, int iYNum)
{
    int       i         = 0;
    int       j         = 0;
    CGridCell *pgcCell  = NULL;
    
    for (i=0; i<=iXNum; i++)
        for(j=0; j<=iYNum; j++)
        {
            pgcCell = GetCell(iRow+i,iCol+j);
            ASSERT_NULL_R(pgcCell, FALSE);

            COMP_BTRUE_R(CELLHIDE(pgcCell), FALSE);
        }

    return TRUE;
}

void CGridCtrl::SetCurCellHide(int iRow, int iCol, int iXNum, int iYNum, BOOL bNoEdit)
{
    CGridCell *pgcCell = NULL;

    pgcCell = GetCell(iRow,iCol);
    ASSERT_NULL(pgcCell);
    
    if(iXNum > 0)
    {
        pgcCell->m_bSkipH    = TRUE;
        pgcCell->m_iNumHhide = iXNum;
    }
    if(iYNum > 0)
    {
        pgcCell->m_bSkipV    = TRUE;
        pgcCell->m_iNumVhide = iYNum;
    }
    pgcCell->m_bNoEdit = bNoEdit;
}

void CGridCtrl::SetAbutCellHide(int iRow, int iCol, int iXNum, int iYNum, BOOL bNoEdit)
{
    int       i         = 0;
    int       j         = 0;
    CGridCell *pgcCell  = NULL;

    for (i=0; i<=iXNum; i++)
        for(j=0; j<=iYNum; j++)
        {
            COMP_BTRUE_CONTINUE((i+j) == 0);

            pgcCell = GetCell(iRow+i,iCol+j);
            ASSERT_NULL(pgcCell);

            pgcCell->m_bHide        = TRUE;
            pgcCell->m_iTopleftCol  = iCol;
            pgcCell->m_iTopleftRow  = iRow;
            pgcCell->m_bNoEdit      = bNoEdit;
            if(i<iXNum)
                pgcCell->m_bSkipH   = TRUE;
            if(j<iXNum)
                pgcCell->m_bSkipV   = TRUE;
        }
}

//设置单元格的合并特性
//iRow ,iCol  分别为欲合并的几个单元格的TOPLEFT位置,xNum为其紧临的水平方向的单元格数目
//yNum为TOPLEFT垂直方向的单元格数目
//本函数及相关的合并单元格功能代码均由daishaozhong@china.com提供
//合并功能已经通过了不很严格的调试，如果你要使用本功能,我不对本代码的正确性和安全性负责
//欢迎使用和改进。
void CGridCtrl::SetCellCombine(int iRow, int iCol, int iXNum, int iYNum, BOOL bNoEdit)
{
    iXNum = (int)fabs((double)iXNum);
    iYNum = (int)fabs((double)iYNum);

    COMP_BFALSE(IsValid(iRow, iCol));
    COMP_BFALSE(IsValid(iRow+iXNum, iCol+iYNum));

    COMP_BTRUE(IsCellFixed(iRow,iCol));

    COMP_BFALSE(CheckHideCell(iRow, iCol, iXNum, iYNum));

    SetCurCellHide (iRow, iCol, iXNum, iYNum, bNoEdit);

    SetAbutCellHide(iRow, iCol, iXNum, iYNum, bNoEdit);
}

void CGridCtrl::UndoCellCombine(int iRow, int iCol)//左上角的单元格座标
{
    int         i           = 0;
    int         j           = 0;
    CGridCell   *pgcCell    = NULL;
    CGridCell   *pgcOldCell = NULL;

    COMP_BFALSE(IsValid(iRow,iCol));

    pgcOldCell = GetCell(iRow,iCol);
    ASSERT_NULL(pgcOldCell);
    ASSERT_ZERO(pgcOldCell->m_iNumHhide);
    ASSERT_ZERO(pgcOldCell->m_iNumVhide);

    for (i=0; i<=pgcOldCell->m_iNumHhide; i++)
        for(j=0; j<=pgcOldCell->m_iNumVhide; j++)
        {
            COMP_BTRUE_CONTINUE((i+j) == 0);

            pgcCell = GetCell(iRow+i,iCol+j);
            ASSERT_NULL(pgcCell);
            
            pgcCell->m_bHide        = FALSE;
            pgcCell->m_iTopleftCol  = 0;
            pgcCell->m_iTopleftRow  = 0;
            pgcCell->m_bNoEdit      = FALSE;
            if(j<pgcOldCell->m_iNumVhide)
                pgcCell->m_bSkipV   = FALSE;
            if(i<pgcOldCell->m_iNumHhide)
                pgcCell->m_bSkipH   = FALSE;

        }

    pgcOldCell->m_bSkipH    = FALSE;
    pgcOldCell->m_bSkipV    = FALSE;
    pgcOldCell->m_bNoEdit   = FALSE;
    pgcOldCell->m_iNumHhide = 0;
    pgcOldCell->m_iNumVhide = 0;
}

void CGridCtrl::SetCurFixedCellHide(int iRow, int iCol, int iXNum, int iYNum)
{
    CGridCell *pgcCell = NULL;

    pgcCell = GetCell(iRow,iCol);
    ASSERT_NULL(pgcCell);

    if(iXNum > 0)
    {
        pgcCell->m_iNumHhide = iXNum;
    }
    if(iYNum > 0)
    {
        pgcCell->m_iNumVhide = iYNum;
    }
}

void CGridCtrl::SetAbutFixedCellHide(int iRow, int iCol, int iXNum, int iYNum)
{
    int       i         = 0;
    int       j         = 0;
    CGridCell *pgcCell  = NULL;

    for (i=0; i<=iXNum; i++)
        for(j=0; j<=iYNum; j++)
        {
            COMP_BTRUE_CONTINUE((i+j) == 0);

            pgcCell = GetCell(iRow+i,iCol+j);
            ASSERT_NULL(pgcCell);

            pgcCell->m_bHide        = TRUE;
            pgcCell->m_iTopleftCol  = iCol;
            pgcCell->m_iTopleftRow  = iRow;
        }
}

void CGridCtrl::SetFixedCellCombine(int iRow,int iCol, int iXNum,int iYNum)
{
    iXNum = (int)fabs((double)iXNum);
    iYNum = (int)fabs((double)iYNum);

    COMP_BFALSE(IsValid(iRow, iCol));
    COMP_BFALSE(IsValid(iRow+iXNum, iCol+iYNum));

    COMP_BFALSE(IsCellFixed(iRow+iXNum,iCol+iYNum));

    CheckHideCell(iRow, iCol, iXNum, iYNum);

    SetCurFixedCellHide (iRow, iCol, iXNum, iYNum);

    SetAbutFixedCellHide(iRow, iCol, iXNum, iYNum);
}

