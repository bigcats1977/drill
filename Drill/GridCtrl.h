#if !defined(AFX_GRIDCTRL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_GRIDCTRL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// GridCtrl.h : header file
//
// MFC Grid Control header file
//
//The code added function for combine or unCombine cell
//the added function  written by daishaozhong(daishaozhong@china.com)
//added time 2001-8-18.
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Copyright (c) 1998.
//
// The code contained in this file is based on the original
// WorldCom Grid control written by Joe Willcoxson,
//        E-mail:  chinajoe@aol.com
//        URL:  http://users.aol.com/chinajoe
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
// Please use and enjoy. Please let me know of any bugs/mods/improvements
// that you have found/implemented and I will fix/incorporate them into this
// file.
//
/////////////////////////////////////////////////////////////////////////////

#include "CellRange.h"
#include <afxtempl.h>

#ifdef _WIN32_WCE
#define GRIDCONTROL_NO_TITLETIPS   // Do not use titletips for cells with large data
#define GRIDCONTROL_NO_DRAGDROP    // Do not use OLE drag and drop
#define GRIDCONTROL_NO_CLIPBOARD   // Do not use clipboard routines
#define GRIDCONTROL_NO_PRINTING    // Do not use printing routines

#ifdef WCE_NO_PRINTING
#define _WIN32_WCE_NO_PRINTING
#endif
#ifdef WCE_NO_CURSOR
#define _WIN32_WCE_NO_CURSOR
#endif

#else
// Use C++ exception handling instead of structured.
#undef TRY
#undef CATCH
#undef END_CATCH
#define TRY try
#define CATCH(ex_class, ex_object) catch(ex_class* ex_object)
#define END_CATCH
#endif  // _WIN32_WCE

#ifndef GRIDCONTROL_NO_TITLETIPS
#include "TitleTip.h"
#endif

#ifndef GRIDCONTROL_NO_DRAGDROP
#include "GridDropTarget.h"
#undef GRIDCONTROL_NO_CLIPBOARD     // Force clipboard functions on
#endif

#ifndef GRIDCONTROL_NO_CLIPBOARD
#include <afxole.h>
#endif

// Use this as the classname when inserting this control as a custom control
// in the MSVC++ dialog editor
#define GRIDCTRL_CLASSNAME    _T("MFCGridCtrl")  // Window class name
#define IDC_INPLACE_CONTROL   8                  // ID of inplace edit controls

#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )

// Used for Get/SetItem calls.
typedef struct _GV_ITEM
{
    int      iRow;        // Row and Column of item
    int      iCol;
    UINT     nMask;       // Mask for use in getting/setting cell data
    UINT     nState;      // cell state (focus/hilighted etc)
    UINT     nFormat;     // Format of cell
    CString  szText;      // Text in cell
    int      iImage;      // index of the list view item抯 icon
    COLORREF clrBk;       // Background colour (or CLR_DEFAULT)
    COLORREF clrFg;       // Forground colour (or CLR_DEFAULT)
    LPARAM   lParam;      // 32-bit value to associate with item
    LOGFONT  lfFont;      // Cell font
} GV_ITEM;

// Grid line selection
#define GVL_NONE                0
#define GVL_HORZ                1
#define GVL_VERT                2
#define GVL_BOTH                3

// Cell data mask
#define GVIF_TEXT               LVIF_TEXT
#define GVIF_IMAGE              LVIF_IMAGE
#define GVIF_PARAM              LVIF_PARAM
#define GVIF_STATE              LVIF_STATE
#define GVIF_BKCLR              (GVIF_STATE<<1)
#define GVIF_FGCLR              (GVIF_STATE<<2)
#define GVIF_FORMAT             (GVIF_STATE<<3)
#define GVIF_FONT               (GVIF_STATE<<4)

// Cell states
#define GVIS_FOCUSED            0x0001
#define GVIS_SELECTED           0x0002
#define GVIS_DROPHILITED        0x0004
#define GVIS_READONLY           0x0008
#define GVIS_FIXED              0x0010  // not yet used
#define GVIS_MODIFIED           0x0020

// Cell Searching options
#define GVNI_FOCUSED            0x0001
#define GVNI_SELECTED           0x0002
#define GVNI_DROPHILITED        0x0004
#define GVNI_READONLY           0x0008
#define GVNI_FIXED              0x0010  // not yet used
#define GVNI_MODIFIED           0x0020

#define GVNI_ABOVE              LVNI_ABOVE
#define GVNI_BELOW              LVNI_BELOW
#define GVNI_TOLEFT             LVNI_TOLEFT
#define GVNI_TORIGHT            LVNI_TORIGHT
#define GVNI_ALL                (LVNI_BELOW|LVNI_TORIGHT|LVNI_TOLEFT)
#define GVNI_AREA               (LVNI_BELOW|LVNI_TORIGHT)

// Hit test values (not yet implemented)
#define GVHT_DATA               0x0000
#define GVHT_TOPLEFT            0x0001
#define GVHT_COLHDR             0x0002
#define GVHT_ROWHDR             0x0004
#define GVHT_COLSIZER           0x0008
#define GVHT_ROWSIZER           0x0010
#define GVHT_LEFT               0x0020
#define GVHT_RIGHT              0x0040
#define GVHT_ABOVE              0x0080
#define GVHT_BELOW              0x0100

typedef struct tagNM_GRIDVIEW
{
    NMHDR hdr;
    int   iRow;
    int   iColumn;
} NM_GRIDVIEW;

typedef struct tagGV_DISPINFO
{
    NMHDR   hdr;
    GV_ITEM item;
} GV_DISPINFO;

/* Cell's Valid Range */
typedef struct tagCELL_RANGE
{
    int     iMinRow;
    int     iMinCol;
    int     iMaxRow;
    int     iMaxCol;
} CELL_RANGE;

// Messages sent to the grid's parent (More will be added in future)
#define GVN_BEGINDRAG           LVN_BEGINDRAG        // LVN_FIRST-9
#define GVN_BEGINLABELEDIT      LVN_BEGINLABELEDIT   // LVN_FIRST-5
#define GVN_BEGINRDRAG          LVN_BEGINRDRAG
#define GVN_COLUMNCLICK         LVN_COLUMNCLICK
#define GVN_DELETEITEM          LVN_DELETEITEM
#define GVN_ENDLABELEDIT        LVN_ENDLABELEDIT     // LVN_FIRST-6
#define GVN_SELCHANGING         LVN_ITEMCHANGING
#define GVN_SELCHANGED          LVN_ITEMCHANGED



// Each cell contains one of these. Fields "row" and "column" are not stored since we
// will usually have acces to them in other ways, and they are an extra 8 bytes per
// cell that is probably unnecessary.

class CGridCell : public CObject
{
public:
    BOOL m_bSkipV;      //前一个表明去掉单元格的垂直线段,后一个表明去掉单元格的水平线段
    BOOL m_bSkipH;
    BOOL m_bHide;//单元格是否被合并(需要隐藏)
    BOOL m_bNoEdit;
    int  m_iNumHhide;   //水平需要隐藏的单元格;只针对TOPLEFT赋值.即列数
    int  m_iNumVhide;   //垂直需要隐藏的单元格;只针对TOPLEFT赋值,即行数
    int  m_iTopleftRow; //被隐藏的单元格需要输入最左上的行和列值
    int  m_iTopleftCol;

    CGridCell()
    {
        m_nState        = 0;
        m_nFormat       = 0;
        m_iImage        = -1;
        m_clrBk         = CLR_DEFAULT;
        m_clrFg         = CLR_DEFAULT;
        m_lParam        = 0;
       //for combine,do you want remove it?
        m_bSkipV        = FALSE;
        m_bSkipH        = FALSE;
        m_bHide         = FALSE;
        m_bNoEdit       = FALSE;
        m_iNumHhide     = 0;
        m_iNumVhide     = 0;
        m_iTopleftRow   = 0;
        m_iTopleftCol   = 0;
        m_szText.Empty();
        //above added for combine ;
    }

    UINT     m_nState;       // Cell state (selected/focus etc)
    UINT     m_nFormat;     // Cell format
    CString  m_szText;      // Cell text (or binary data if you wish...)
    int      m_iImage;      // Index of the list view item抯 icon
    COLORREF m_clrBk;     // Background colour (or CLR_DEFAULT)
    COLORREF m_clrFg;     // Forground colour (or CLR_DEFAULT)
    LOGFONT  m_lfFont;      // Cell font
    LPARAM   m_lParam;      // 32-bit value to associate with item
};


// storage typedef for each row in the grid
typedef CTypedPtrArray<CObArray, CGridCell*> GRID_ROW;

// DDX_GridControl is used where a DDX_Control call is needed. In some strange
// situations the usual DDX_Control does not result in CGridCtrl::SubclassWindow
// or CGridCtrl::PreSubclassWindow being called. Using this version calls
// CGridCtrl::SubclassWindow directly - ensuring that cell metrics are set properly
class CGridCtrl;
void AFXAPI DDX_GridControl(CDataExchange* pDX, int iIDC, CGridCtrl& rControl);

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl window

class CGridCtrl : public CWnd
{
    DECLARE_DYNCREATE(CGridCtrl)

// Construction
public:
    CGridCtrl(int iRows = 0, int iCols = 0, int iFixedRows = 0, int iFixedCols = 0);

    BOOL Create(const RECT& rc, CWnd* pParentWnd, UINT nID,
                DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);
    BOOL SubclassWindow(HWND hWnd);

// Attributes
public:
    int  GetRowCount() const                    { return m_iRows;                   }
    int  GetColumnCount() const                 { return m_iCols;                   }
    int  GetFixedRowCount() const               { return m_iFixedRows;              }
    int  GetFixedColumnCount() const            { return m_iFixedCols;              }
    BOOL SetRowCount(int iRows = 15);
    BOOL SetColumnCount(int iCols = 15);
    BOOL SetFixedRowCount(int iFixedRows = 1);
    BOOL SetFixedColumnCount(int iFixedCols = 1);

    int  GetRowHeight(int iRow) const;
    BOOL SetRowHeight(int iRow, int iHeight);
    int  GetColumnWidth(int iCol) const;
    BOOL SetColumnWidth(int iCol, int iWidth);

    BOOL GetCellOrigin(int iRow, int iCol, LPPOINT pPoint) const;
    BOOL GetCellOrigin(const CCellID& ciCell, LPPOINT pPoint) const;
    BOOL GetCellRect(int iRow, int iCol, LPRECT pRect) const;
    BOOL GetCellRect(const CCellID& ciCell, LPRECT pRect) const;

    BOOL GetTextRect(const CCellID& ciCell, LPRECT pRect);
    BOOL GetTextRect(int iRow, int iCol, LPRECT pRect);

    int  GetFixedRowHeight() const;
    int  GetFixedColumnWidth() const;
    long GetVirtualWidth() const;
    long GetVirtualHeight() const;

    void     SetTextColor(COLORREF clr)           { m_clrText = clr;                }
    COLORREF GetTextColor() const                 { return m_clrText;               }
    void     SetTextBkColor(COLORREF clr)         { m_clrTextBk = clr;              }
    COLORREF GetTextBkColor() const               { return m_clrTextBk;             }
    void     SetBkColor(COLORREF clr)             { m_clrBk = clr;                  }
    COLORREF GetBkColor() const                   { return m_clrBk;                 }
    void     SetFixedTextColor(COLORREF clr)      { m_clrFixedText = clr;           }
    COLORREF GetFixedTextColor() const            { return m_clrFixedText;          }
    void     SetFixedBkColor(COLORREF clr)        { m_clrFixedBk = clr;             }
    COLORREF GetFixedBkColor() const              { return m_clrFixedBk;            }
    void     SetGridColor(COLORREF clr)           { m_clrGrid = clr;                }
    COLORREF GetGridColor() const                 { return m_clrGrid;               }

    int      GetSelectedCount() const             { return m_tSelectedCellMap.GetCount(); }

    CCellID  GetFocusCell() const                 { return m_ciCurrent;             }

    void        SetImageList(CImageList* pList)   { m_pImageList = pList;           }
    CImageList* GetImageList() const              { return m_pImageList;            }

    void SetGridLines(int iWhichLines = GVL_BOTH) { m_iGridLines = iWhichLines;
                                                    if (::IsWindow(GetSafeHwnd()))
                                                        Invalidate(); 
                                                  }
    int  GetGridLines() const                     { return m_iGridLines;            }

    void SetEditable(BOOL bEditable = TRUE)       { m_bEditable = bEditable;        }
    BOOL IsEditable() const                       { return m_bEditable;             }
    void SetListMode(BOOL bEnableListMode = TRUE) { m_bListMode = bEnableListMode;  }
    BOOL GetListMode() const                      { return m_bListMode;             }
    void SetSingleRowSelection(BOOL bSing = TRUE) { m_bSingleRowSelection = bSing;  }
    BOOL GetSingleRowSelection()                  { return m_bSingleRowSelection;   }
    void EnableSelection(BOOL bEnable = TRUE)     { ResetSelectedRange();
                                                    m_bEnableSelection = bEnable;
                                                    ResetSelectedRange();
                                                  }
    BOOL IsSelectable() const                     { return m_bEnableSelection;      }
    void EnableDragAndDrop(BOOL bAllow = TRUE)    { m_bAllowDragAndDrop = bAllow;   }
    BOOL GetDragAndDrop() const                   { return m_bAllowDragAndDrop;     }
    void SetRowResize(BOOL bResize = TRUE)        { m_bAllowRowResize = bResize;    }
    BOOL GetRowResize() const                     { return m_bAllowRowResize;       }
    void SetColumnResize(BOOL bResize = TRUE)     { m_bAllowColumnResize = bResize; }
    BOOL GetColumnResize() const                  { return m_bAllowColumnResize;    }
    void SetHeaderSort(BOOL bSortOnClick = TRUE)  { m_bSortOnClick = bSortOnClick;  }
    BOOL GetHeaderSort() const                    { return m_bSortOnClick;          }
    void SetHandleTabKey(BOOL bHandleTab = TRUE)  { m_bHandleTabKey = bHandleTab;   }
    BOOL GetHandleTabKey() const                  { return m_bHandleTabKey;         }
    void SetDoubleBuffering(BOOL bBuffer = FALSE)  { m_bDoubleBuffer = bBuffer;     }
    BOOL GetDoubleBuffering() const               { return m_bDoubleBuffer;         }
    void EnableTitleTips(BOOL bEnable = TRUE)     { m_bTitleTips = bEnable;         }
    BOOL GetTitleTips()                           { return m_bTitleTips;            }

    void SetModified(BOOL bModified = TRUE, int iRow = -1, int iCol = -1);
    BOOL GetModified(int iRow = -1, int iCol = -1);
    BOOL IsCellFixed(int iRow, int iCol);

    BOOL     SetItem(const GV_ITEM* pItem);
    BOOL     GetItem(GV_ITEM* pItem);
    BOOL     SetItemText(int iRow, int iCol, LPCTSTR strText);
    virtual  CString GetItemText(int iRow, int iCol);
    BOOL     SetItemData(int iRow, int iCol, LPARAM lParam);
    LPARAM   GetItemData(int iRow, int iCol) const;
    BOOL     SetItemImage(int iRow, int iCol, int iImage);
    int      GetItemImage(int iRow, int iCol) const;
    BOOL     SetItemState(int iRow, int iCol, UINT nState);
    UINT     GetItemState(int iRow, int iCol) const;
    BOOL     SetItemFormat(int iRow, int iCol, UINT nFormat);
    UINT     GetItemFormat(int iRow, int iCol) const;
    BOOL     SetItemBkColour(int iRow, int iCol, COLORREF clrBk = CLR_DEFAULT);
    COLORREF GetItemBkColour(int iRow, int iCol) const;
    BOOL     SetItemFgColour(int iRow, int iCol, COLORREF clrFg = CLR_DEFAULT);
    COLORREF GetItemFgColour(int iRow, int iCol) const;
    BOOL     SetItemFont(int iRow, int iCol, LOGFONT* lf);
    LOGFONT  *GetItemFont(int iRow, int iCol);

// Operations
public:
    int  InsertColumn(LPCTSTR strHeading, UINT nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE,
                      int iCol = -1);
    int  InsertRow(LPCTSTR strHeading, int iRow = -1);
    BOOL DeleteColumn(int iCol);
    BOOL DeleteRow(int iRow);
    BOOL DeleteNonFixedRows();
    BOOL DeleteAllItems();

    BOOL AutoSizeRow(int iRow);
    BOOL AutoSizeColumn(int iCol);
    void AutoSizeRows();
    void AutoSizeColumns();
    void AutoSize();
    void ExpandColumnsToFit();
    void ExpandRowsToFit();
    void ExpandToFit();

    CSize GetTextExtent(LPCTSTR strText, BOOL bUseSelectedFont = TRUE);
    void  EnsureVisible(CCellID &ciCell) { EnsureVisible(ciCell.m_iRow, ciCell.m_iCol); }
    void  EnsureVisible(int iRow, int iCol);
    BOOL  IsCellVisible(int iRow, int iCol) const;
    BOOL  IsCellVisible(CCellID ciCell) const;
    BOOL  IsCellEditable(int iRow, int iCol) const;
    BOOL  IsCellEditable(CCellID &ciCell) const;

    // SetRedraw stops/starts redraws on things like changing the # rows/columns
    // and autosizing, but not for user-intervention such as resizes
    void  SetRedraw(BOOL bAllowDraw, BOOL bResetScrollBars = FALSE);
    BOOL  RedrawCell(int iRow, int iCol, CDC* pDC = NULL);
    BOOL  RedrawCell(const CCellID& ciCell, CDC* pDC = NULL);
    BOOL  RedrawRow(int iRow);
    BOOL  RedrawColumn(int iCol);

#if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
    void Print();
#endif
#ifndef _WIN32_WCE
    BOOL Save(LPCTSTR strFileName);
    BOOL Load(LPCTSTR strFileName);
#endif

    CCellRange  GetCellRange() const;
    CCellRange  GetSelectedCellRange() const;
    void        SetSelectedRange(const CCellRange& crRange, BOOL bForceRepaint = FALSE);
    void        SetSelectedRange(int iMinRow, int iMinCol, int iMaxRow, int iMaxCol,
                                 BOOL bForceRepaint = FALSE);
    BOOL        IsValid(int iRow, int iCol) const;
    BOOL        IsValid(const CCellID& ciCell) const;
    BOOL        IsValid(const CCellRange& crRange) const;

#ifndef GRIDCONTROL_NO_CLIPBOARD
    // Clipboard and cut n' paste operations
    virtual void CutSelectedText();
    virtual COleDataSource* CopyTextFromGrid();
    virtual BOOL PasteTextToGrid(CCellID ciCell, COleDataObject* pDataObject);
#endif

#ifndef GRIDCONTROL_NO_DRAGDROP
    void OnBeginDrag();
    DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint ptOpr);
    DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint ptOpr);
    void OnDragLeave();
    BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint ptOpr);
#endif

#ifndef GRIDCONTROL_NO_CLIPBOARD
    virtual void OnEditCut();
    virtual void OnEditCopy();
    virtual void OnEditPaste();
#endif
    virtual void OnEditSelectAll();

    BOOL SortTextItems(int iCol, BOOL bAscending);
    BOOL SortItems(PFNLVCOMPARE pfnCompare, int iCol, BOOL bAscending, LPARAM data = 0);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGridCtrl)
    protected:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL
#if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
public:
    virtual void OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo);
    virtual void OnPrint(CDC *pDC, CPrintInfo *pInfo);
    virtual void OnEndPrinting(CDC *pDC, CPrintInfo *pInfo);
#endif

// Implementation
public:
    void    SetFixedCellCombine(int iRow,int iCol, int iXNum,int iYNum);
    void    UndoCellCombine(int iRow, int iCol);
    void    SetCellCombine(int iRow, int iCol, int  iXNum, int  iYNum, BOOL bNoEdit=FALSE);
    CGridCell*  GetCellP(int iRow, int iCol);
    virtual ~CGridCtrl();

    /* for OnKeyDown */
    void    KD_Delete(UINT nRepCnt, UINT nFlags, CCellID *pciNext);
    void    KD_Tab   (UINT nRepCnt, UINT nFlags, CCellID *pciNext);
    void    KD_Down  (UINT nRepCnt, UINT nFlags, CCellID *pciNext);
    void    KD_Up    (UINT nRepCnt, UINT nFlags, CCellID *pciNext);
    void    KD_Right (UINT nRepCnt, UINT nFlags, CCellID *pciNext);
    void    KD_Left  (UINT nRepCnt, UINT nFlags, CCellID *pciNext);
    void    KD_Next  (UINT nRepCnt, UINT nFlags, CCellID *pciNext);
    void    KD_Prior (UINT nRepCnt, UINT nFlags, CCellID *pciNext);
    void    KD_Home  (UINT nRepCnt, UINT nFlags, CCellID *pciNext);
    void    KD_End   (UINT nRepCnt, UINT nFlags, CCellID *pciNext);
    /* for OnHScroll */
    void    HS_LineRight(int iScrPos, CCellID ciTopLeft, CRect rcTmp);
    void    HS_LineLeft (int iScrPos, CCellID ciTopLeft, CRect rcTmp);
    void    HS_PageRight(int iScrPos, CCellID ciTopLeft, CRect rcTmp);
    void    HS_PageLeft (int iScrPos, CCellID ciTopLeft, CRect rcTmp);
    void    HS_Thumb    (int iScrPos, CCellID ciTopLeft, CRect rcTmp);
    void    HS_Right    (int iScrPos, CCellID ciTopLeft, CRect rcTmp);
    void    HS_Left     (int iScrPos, CCellID ciTopLeft, CRect rcTmp);
    /* for OnVScroll */
    void    VS_LineDown (int iScrPos, CCellID ciTopLeft, CRect rcTmp);
    void    VS_LineUp   (int iScrPos, CCellID ciTopLeft, CRect rcTmp);
    void    VS_PageDown (int iScrPos, CCellID ciTopLeft, CRect rcTmp);
    void    VS_PageUp   (int iScrPos, CCellID ciTopLeft, CRect rcTmp);
    void    VS_Thumb    (int iScrPos, CCellID ciTopLeft, CRect rcTmp);
    void    VS_Top      (int iScrPos, CCellID ciTopLeft, CRect rcTmp);
    void    VS_Bottom   (int iScrPos, CCellID ciTopLeft, CRect rcTmp);

protected:
    BOOL    RegisterWindowClass();
    LRESULT SendMessageToParent(int iRow, int iCol, int iMessage);

    BOOL InvalidateCellRect(const CCellID& ciCell);
    BOOL InvalidateCellRect(const CCellRange& crRange);
    void EraseBkgnd(CDC* pDC);

    BOOL GetCellRangeRect(const CCellRange& crRange, LPRECT lpRect) const;

    CGridCell*  GetCell(int iRow, int iCol) const;
    BOOL        SetCell(int iRow, int iCol, CGridCell* pgcCell);

    int  SetMouseMode(int iMode)
    {
        int iOldMode = 0;
        iOldMode     = m_iMouseMode;
        m_iMouseMode = iMode;
        return iOldMode;
    }
    int  GetMouseMode() const
    {
        return m_iMouseMode;
    }

    BOOL MouseOverRowResizeArea(CPoint& ptOpr) const;
    BOOL MouseOverColumnResizeArea(CPoint& ptOpr) const;

    CCellID    GetCellFromPt(CPoint ptOpr, BOOL bAllowFixedCellCheck = TRUE) const;
    CCellID    GetTopleftNonFixedCell() const;
    CCellRange GetUnobstructedNonFixedCellRange() const;
    CCellRange GetVisibleNonFixedCellRange(LPRECT pRect = NULL) const;

    CCellID     SetFocusCell(CCellID ciCell);
    CCellID     SetFocusCell(int iRow, int iCol);

    void        ResetSelectedRange();
    void        ResetScrollBars();
    int         GetScrollPos32(int iBar, BOOL bGetTrackPos = FALSE);
    BOOL        SetScrollPos32(int iBar, int iPos, BOOL bRedraw = TRUE);

    BOOL        SortTextItems(int iCol, BOOL bAscending, int iLow, int iHigh);
    BOOL        SortItems(PFNLVCOMPARE pfnCompare, int iCol, BOOL bAscending, LPARAM pData,
                          int iLow, int iHigh);

// Overrrides
protected:
    // Printing
#if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
    virtual void PrintColumnHeadings(CDC *pDC, CPrintInfo *pInfo);
    virtual void PrintHeader(CDC *pDC, CPrintInfo *pInfo);
    virtual void PrintFooter(CDC *pDC, CPrintInfo *pInfo);
#endif

#ifndef GRIDCONTROL_NO_DRAGDROP
    // Drag n' drop
    virtual CImageList* CreateDragImage(CPoint *pHotSpot);    // no longer necessary
#endif

    // Mouse Clicks
    virtual void  OnFixedColumnClick(CCellID& ciCell);
    virtual void  OnFixedRowClick(CCellID& ciCell);

    // Editing
    virtual CSize GetCellExtent(int iRow, int iCol, CDC* pDC);
    virtual void  OnEndEditCell(int iRow, int iCol, CString strText);
    virtual void  OnEditCell(int iRow, int iCol, UINT nChar);
    virtual void  CreateInPlaceEditControl(CRect& rc, DWORD dwStyle, UINT nID, int iRow, int iCol,
                                           LPCTSTR szText, UINT nChar);

    // Drawing
    virtual void  OnDraw(CDC* pDC);
    virtual BOOL  DrawFixedCell(CDC* pDC, int iRow, int iCol, CRect rcCell, BOOL bEraseBk=FALSE);
    virtual BOOL  DrawCell(CDC* pDC, int iRow, int iCol, CRect rcCell, BOOL bEraseBk=FALSE);

    // GridCell Creation and Cleanup
    virtual CGridCell* CreateCell(int iRow, int iCol);
    virtual void  EmptyCell(CGridCell* pgcCell, int iRow, int iCol);

// Attributes
protected:
    // General attributes
    COLORREF    m_clrText;          // Grid colours
    COLORREF    m_clrTextBk;
    COLORREF    m_clrBk;
    COLORREF    m_clrFixedText;
    COLORREF    m_clrFixedBk;
    COLORREF    m_clrGrid;
    COLORREF    m_clrWindowText;    // System colours
    COLORREF    m_clrWindow;
    COLORREF    m_clr3DFace;
    COLORREF    m_clrShadow;

    BOOL        m_bEditable;
    BOOL        m_bModified;
    BOOL        m_bAllowDragAndDrop;
    BOOL        m_bListMode;
    BOOL        m_bSingleRowSelection;
    BOOL        m_bAllowDraw;
    BOOL        m_bEnableSelection;
    BOOL        m_bSortOnClick;
    BOOL        m_bHandleTabKey;
    BOOL        m_bDoubleBuffer;
    BOOL        m_bTitleTips;

    // Cell size details
    int         m_iGridLines;
    int         m_iRows;
    int         m_iFixedRows;
    int         m_iCols;
    int         m_iFixedCols;
    CUIntArray  m_arRowHeights;
    CUIntArray  m_arColWidths;
    int         m_iMargin;
    int         m_iDefCellWidth;
    int         m_iDefCellHeight;
    int         m_iVScrollMax;
    int         m_iHScrollMax;

    // Fonts and images
    LOGFONT     m_Logfont;
    CFont       m_ftPrinter;        // for the printer
    CFont       m_ftGrid;           // for the grid
    CImageList* m_pImageList;

    // Cell data
    CTypedPtrArray<CObArray, GRID_ROW*> m_tRowData;

    // Mouse operations such as cell selection
    int         m_iMouseMode;
    CPoint      m_ptLeftClickDown;
    CPoint      m_ptLastMouse;
    CCellID     m_ciLeftClickDown;
    CCellID     m_ciSelectionStart;
    CCellID     m_ciCurrent;
    int         m_iTimerID;
    int         m_iTimerInterval;
    int         m_iResizeCaptureRange;
    BOOL        m_bAllowRowResize;
    BOOL        m_bAllowColumnResize;
    int         m_iRowsPerWheelNotch;
    CMap<DWORD,DWORD, CCellID, CCellID&> m_tSelectedCellMap;
    CMap<DWORD,DWORD, CCellID, CCellID&> m_tPrevSelectedCellMap;

#ifndef GRIDCONTROL_NO_TITLETIPS
    CTitleTip   m_tTitleTip;            // Title tips for cells
#endif

    // Drag and drop
    BOOL        m_bMustUninitOLE;       // Do we need to uninitialise OLE?
    CCellID     m_ciLastDragOver;
#ifndef GRIDCONTROL_NO_DRAGDROP
    CGridDropTarget m_tDropTarget;       // OLE Drop target for the grid
#endif

    // Printing information
    CSize       m_szChar;
    int         m_iPageHeight;
    CSize       m_szLogicalPage;        // Page size in gridctrl units.
    CSize       m_szPaper;              // Page size in device units.

    // sorting
    BOOL        m_bAscending;
    int         m_iSortCol;

protected:
    void SelectAllCells();
    void SelectColumns(CCellID ciCurrent);
    void SelectRows(CCellID ciCurrent);
    void SelectCells(CCellID ciCurrent);
    void OnSelecting(const CCellID& ciCurrent);

    // Generated message map functions
    //{{AFX_MSG(CGridCtrl)
    afx_msg void OnPaint();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint ptOpr);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint ptOpr);
    afx_msg void OnMouseMove(UINT nFlags, CPoint ptOpr);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg UINT OnGetDlgCode();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint ptOpr);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT nMessage);
    afx_msg void OnSysColorChange();
    afx_msg void OnCaptureChanged(CWnd *pWnd);
    afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
    //}}AFX_MSG
#if (_MFC_VER >= 0x0421) || (_WIN32_WCE >= 210)
    afx_msg void OnSettingChange(UINT nFlags, LPCTSTR lpszSection);
#endif
#if !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)
    afx_msg BOOL OnMouseWheel(UINT nFlags, short iDelta, CPoint ptOpr);
#endif
    afx_msg LRESULT OnSetFont(WPARAM hFont, LPARAM lParam);
    afx_msg LRESULT OnGetFont(WPARAM hFont, LPARAM lParam);
    afx_msg void OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult);
    DECLARE_MESSAGE_MAP()

    enum eMouseModes { MOUSE_NOTHING,
                       MOUSE_SELECT_ALL,
                       MOUSE_SELECT_COL,
                       MOUSE_SELECT_ROW,
                       MOUSE_SELECT_CELLS,
                       MOUSE_SCROLLING_CELLS,
                       MOUSE_OVER_ROW_DIVIDE,
                       MOUSE_SIZING_ROW,
                       MOUSE_OVER_COL_DIVIDE,
                       MOUSE_SIZING_COL,
                       MOUSE_PREPARE_EDIT,
#ifndef GRIDCONTROL_NO_DRAGDROP
                       MOUSE_PREPARE_DRAG,
                       MOUSE_DRAGGING
#endif
                    };
private:
    void    UpdateSysColor();
    /* for CGridCtrl */
    void    InitOLELib(void);
    /* for DrawFixedCell */
    void    DrawCellImage(CDC* pDC, GV_ITEM *pItem, CRect& rcCell);
    BOOL    DrawHideFixedCell(int &iRow, int &iCol, CRect& rcCell, GV_ITEM* pItem);
    /* for DrawCell */
    void    DrawHideCell(int &iRow, int &iCol, CRect& rcCell);
    void    DrawFocusedCell(CDC * pDC, CRect rcCell, BOOL bEraseBk,
                            COLORREF clrTextBk, COLORREF clrText);
    void    DrawNormalCell (CDC * pDC, CRect rcCell, BOOL bEraseBk,
                            COLORREF clrTextBk, COLORREF clrText);
    void    DrawSelectedCell(CDC *pDC, CRect rcCell);
    void    DrawDrophilitedCell(CDC *pDC, CRect rcCell);
    /* for GetCellFromPt */
    int     GetCellColFromPt(CPoint ptOpr, BOOL bAllowFixedCellCheck =TRUE) const;
    int     GetCellRowFromPt(CPoint ptOpr, BOOL bAllowFixedCellCheck =TRUE) const;
    /* for GetCellOrigin */
    int     GetCellOriginLeft(int iCol) const;
    int     GetCellOriginTop (int iRow) const;
    /* for IsCellVisible */
    BOOL    IsCellRowVis(int iRow) const;
    BOOL    IsCellColVis(int iCol) const;
    /* for OnEditCell */
    BOOL    EditCombineCell(int &iRow, int &iCol, CRect &rcCell);
    /* for OnDraw */
    void    DrawFixedTopLeftCell(CDC* pDC);
    void    DrawFixedColCell(CDC* pDC, CRect rcClip, int iFixedRowHeight,
                             int iMinVisRow, int iMaxVisRow);
    void    DrawFixedRowCell(CDC* pDC, CRect rcClip, int iFixedColWidth,
                             int iMinVisCol, int iMaxVisCol);
    void    DrawNonFixedCell(CDC* pDC, CRect rcClip,
                             int iFixedRowHeight, int iFixedColWidth,
                             int iMinVisRow, int iMaxVisRow,
                             int iMinVisCol, int iMaxVisCol);
    void    DrawVertLine(CDC* pDC, CRect rcVisible,
                         int iFixedRowHeight, int iFixedColWidth,
                         int iMinVisRow, int iMaxVisRow,
                         int iMinVisCol, int iMaxVisCol);
    void    DrawHorzLine(CDC* pDC, CRect rcVisible,
                         int iFixedRowHeight, int iFixedColWidth,
                         int iMinVisRow, int iMaxVisRow,
                         int iMinVisCol, int iMaxVisCol);
    /* for OnTimer */
    void    TimerSelectXCell(CPoint ptOrig);
    void    TimerSelectYCell(CPoint ptOrig);
    /* for OnKeyDown */
    void    CTRLKeyDown(UINT nChar);
    void    KD_TabVisible(BOOL bChangeLine);
    void    KD_SetVisible(UINT nChar, CCellID ciNext, BOOL bChangeLine);
    /* for OnMouseMove */
    void    UpdateMouseMode(CPoint& ptOpr);
    void    MM_UpdateTitleTips(CPoint& ptOpr);
    void    MM_WhenLeftBtnUp(CPoint& ptOpr);
    void    MM_MouseSelect(CPoint& ptOpr);
    void    MM_MouseSizingCol(CPoint& ptOpr);
    void    MM_MouseSizingRow(CPoint& ptOpr);
    void    MM_WhenLeftBtnDown(CPoint& ptOpr);
    /* for OnLButtonDown */
    BOOL    LBD_UpdateStatus(UINT nFlags, CPoint& ptOpr);
    void    LBD_ColDivide(CPoint& ptOpr);
    void    LBD_RowDivide(CPoint& ptOpr);
    void    LBD_Select(UINT nFlags);
    /* for OnLButtonUp */
    BOOL    LBU_SizeCol(CPoint ptOpr);
    BOOL    LBU_SizeRow(CPoint ptOpr);
    /* for OnLButtonDblClk */
    BOOL    LBDC_ColDivide(CPoint ptOpr);
    BOOL    LBDC_RowDivide(CPoint ptOpr);
    BOOL    LBDC_Normal(CPoint ptOpr);
    /* for SetSelectedRange */
    CELL_RANGE  InitCellRange(int iMinRow, int iMinCol, int iMaxRow, int iMaxCol);
    BOOL    CellInRange(CCellID ciCell, CELL_RANGE crRange);
    BOOL    IsValidRange(CELL_RANGE crRange);
    void    UnselectedPreCells(CELL_RANGE crRange, CDC *pDC);
    void    SetSelectedPreCells(CDC *pDC);
    void    SetSelectedState(CELL_RANGE crRange, CDC *pDC);
    /* for RedrawCell */
    BOOL    RedrawFixedCell(CDC* pDC, int iRow, int iCol, CRect &rcCell);
    BOOL    RedrawNormalCell(CDC* pDC, int iRow, int iCol, CRect &rcCell);
    void    RedrawGridLines(CDC* pDC, CRect rcCell);
    BOOL    RedrawCellWithDC(CDC* pDC, int iRow, int iCol, CRect rcCell);
    /* for SortTextItems */
    BOOL    CheckSortInfo(int iCol, int iLow, int &iHigh);
    void    AscFindSwapTextItem(int iCol, int iLow, int iHigh, int &iLo, int &iHi);
    void    DecFindSwapTextItem(int iCol, int iLow, int iHigh, int &iLo, int &iHi);
    void    SwapTextItems(int iCol, int &iLo, int &iHi);
    /* for SortItems */
    void    AscFindSwapItem(PFNLVCOMPARE pfnCompare, int iCol, LPARAM pData,
                            int iLow, int iHigh, int &iLo, int &iHi);
    void    DecFindSwapItem(PFNLVCOMPARE pfnCompare, int iCol, LPARAM pData,
                            int iLow, int iHigh, int &iLo, int &iHi);
    void    SwapItems(PFNLVCOMPARE pfnCompare, int iCol, LPARAM pData, int &iLo, int &iHi);
    /* for SetColumnCount */
    void    DelLosColumn(int iDelCol);
    void    InsertNewColumn(int iStartCol);
    /* for SetRowCount*/
    void    DelLosRow(int iDelRow);
    void    InsertNewRow(int iStartRow);
    /* for OnPrint */
    void    SetPrintInfo(CDC *pDC, CPrintInfo *pInfo);
    int     FindStartPrintRow(CPrintInfo *pInfo);
    void    PrintHorzLine(CDC *pDC, int iCol, int iCurPrintRow, CRect rectDraw);
    void    PrintVertLine(CDC *pDC, int iCol, BOOL bFirstRow, CRect rectDraw);
    void    PrintGridRow(CDC *pDC, int iCurPrintRow);
    /* for SetCellCombine */
    BOOL    CheckHideCell  (int iRow, int iCol, int iXNum, int iYNum);
    void    SetCurCellHide (int iRow, int iCol, int iXNum, int iYNum, BOOL bNoEdit);
    void    SetAbutCellHide(int iRow, int iCol, int iXNum, int iYNum, BOOL bNoEdit);
    /* for SetFixedCellCombine */
    void    SetCurFixedCellHide (int iRow, int iCol, int iXNum, int iYNum);
    void    SetAbutFixedCellHide(int iRow, int iCol, int iXNum, int iYNum);
    /* for SetFocusCell */
    void    SelectedCell(CCellID ciPreCell, CCellID ciCurCel);
    void    UnSelectCell(CCellID ciPreCell, CCellID ciCurCel);
    /* for PasteTextToGrid */
    void    SetTextInCell(CCellID ciCell, CString strText);
    /* for InsertColumn */
    void    InitColumnData(int iCol, LPCTSTR strHeading, UINT nFormat);
    int     InsertEndColumn();
    int     InsertGivenColumn(int iCol);
    /* for InsertRow */
    int     InsertEndRow();
    int     InsertGivenRow(int iRow);
    /* for PrintColumnHeadings */
    void    DrawHorzGridLine(CDC *pDC, int iRow, int iCol, CRect rcCell);
    void    DrawVertGridLine(CDC *pDC, int iRow, int iCol, CRect rcCell);
};

inline CGridCell* CGridCtrl::GetCell(int iRow, int iCol) const
{
    GRID_ROW    *pRow = NULL;

    COMP_BL_R (iRow, 0, NULL);
    COMP_BGE_R(iRow, m_iRows, NULL);
    COMP_BL_R (iCol, 0, NULL);
    COMP_BGE_R(iCol, m_iCols, NULL);

    pRow = m_tRowData[iRow];
    ASSERT_NULL_R(pRow, NULL);

    return pRow->GetAt(iCol);
}

inline BOOL CGridCtrl::SetCell(int iRow, int iCol, CGridCell* pgcCell)
{
    GRID_ROW    *pRow = NULL;

    ASSERT_NULL_R(pgcCell, FALSE);

    COMP_BL_R (iRow, 0, FALSE);
    COMP_BGE_R(iRow, m_iRows, FALSE);
    COMP_BL_R (iCol, 0, FALSE);
    COMP_BGE_R(iCol, m_iCols, FALSE);

    pRow = m_tRowData[iRow];
    ASSERT_NULL_R(pRow, FALSE);

    pRow->SetAt(iCol, pgcCell);
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCTRL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
