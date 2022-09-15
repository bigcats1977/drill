#if !defined(AFX_MYLISTCTRL_H__1FBC15D9_9388_4CAC_8B5C_5F756C2FC0B1__INCLUDED_)
#define AFX_MYLISTCTRL_H__1FBC15D9_9388_4CAC_8B5C_5F756C2FC0B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Mylistctrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMylistctrl window

class CMylistctrl : public CListCtrl
{
struct ItemData
{
public:
    ItemData() : arrpsz( NULL ), dwData( NULL ) {}

    LPTSTR  *arrpsz;
    DWORD   dwData;

private:
    // ban copying.
    ItemData( const ItemData& );
    ItemData& operator=( const ItemData& );
};
// Construction
public:
    CMylistctrl();

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMylistctrl)
    protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL

// Implementation
public:
    BOOL    SetHeadings( UINT nStringID );
    BOOL    SetHeadings( const CString& strHeadings );
    BOOL    GetCellRect(int iRow, int iCol, int iArea, CRect &rc);
    BOOL    SetItemData(int iItem, DWORD dwData);
    int     AddItemList(CStringList& slItem);
    int     AddItem( LPCTSTR pszText, ... );
    void    LoadColumnInfo();
    BOOL    DeleteAllItems();
    BOOL    SetItemText( int iItem, int iSubItem, LPCTSTR lpszText );
    DWORD   GetItemData(int iItem) const;
    virtual ~CMylistctrl();

    // Generated message map functions
protected:
    int     m_iHighlight;
    int     m_iNumCols;
    int     m_iSortCol;
    BOOL    m_bSortAscending;

    BOOL    SetTextArray( int iItem, LPTSTR* arrpsz );
    void    FreeItemMemory( const int iItem );
    LPTSTR  *GetTextArray( int iItem ) const;
    //{{AFX_MSG(CMylistctrl)
    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTCTRL_H__1FBC15D9_9388_4CAC_8B5C_5F756C2FC0B1__INCLUDED_)
