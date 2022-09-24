// Mylistctrl.cpp : implementation file
//

#include "stdafx.h"
#include "Mylistctrl.h"
#include "afxtempl.h"
#include "Torque.h"
#include "mySheet.h"
#include "DlgHisList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#ifdef _DEBUG
#define ASSERT_VALID_STRING( str ) ASSERT( !IsBadStringPtr( str, 0xfffff ) )
#else   //  _DEBUG
#define ASSERT_VALID_STRING( str ) ( (void)0 )
#endif  //  _DEBUG

#define columncount 8



/////////////////////////////////////////////////////////////////////////////
// CMylistctrl

CMylistctrl::CMylistctrl()
{
    m_iNumCols          = 0;
    m_iSortCol          = -1;
    m_bSortAscending    = TRUE;
    m_iHighlight        = 0;
}

CMylistctrl::~CMylistctrl()
{

}


BEGIN_MESSAGE_MAP(CMylistctrl, CListCtrl)
//{{AFX_MSG_MAP(CMylistctrl)
ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONDBLCLK()
ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
    ON_WM_RBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMylistctrl message handlers


BOOL CMylistctrl::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Add your specialized code here and/or call the base class
    cs.style &= ~LVS_TYPEMASK;
    cs.style &= ~LVS_SHOWSELALWAYS;
    cs.style |= LVS_REPORT;
    cs.cx    = GetSystemMetrics(0);
    cs.cy    = GetSystemMetrics(1);
    return CListCtrl::PreCreateWindow(cs);
}

void CMylistctrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    CListCtrl::OnLButtonDown(nFlags, point);
}

//for this notification, the structure is actually a
// NMLVCUSTOMDRAW that tells you what's going on with the custom
// draw action. So, we'll need to cast the generic pNMHDR pointer.
void CMylistctrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    DWORD_PTR iRow   = 0;
    DWORD    dwData = 0;
    LPNMLVCUSTOMDRAW  lplvcd = NULL;
    CPaintDC dc(this);

    ASSERT_NULL(pNMHDR);
    ASSERT_NULL(pResult);

    lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

    dc.SetBkMode(TRANSPARENT);

    switch(lplvcd->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW;          // ask for item notifications.
        break;
    case CDDS_ITEMPREPAINT:
        *pResult = CDRF_DODEFAULT;
        iRow = lplvcd->nmcd.dwItemSpec;
        /* 隔行设置不同的背景色 */
        if(iRow%2 != 0)
            lplvcd->clrTextBk = CLR_ALTERBACKG;
        dwData = GetItemData(iRow);
        /* 异常记录，文字设置为红色 */
        if(dwData == TORQ_BAD_QUALITY)
        {
            lplvcd->clrText = CLR_ABNORMDATA;
        }
        /* 工具扣为蓝色 */
        else if(dwData == TORQ_TOOLBUCKLE)
        {
            lplvcd->clrText = CLR_TOOLBUCKLE;
        }
        *pResult = CDRF_NEWFONT;
        break;
    default:
        *pResult = CDRF_DODEFAULT;
    }
}

BOOL CMylistctrl::GetCellRect(int iRow, int iCol, int iArea, CRect &rc)
{
    CRect rcCol;

    if(iCol)
        return GetSubItemRect(iRow, iCol, iArea, rc);

    iCol = 1;
    COMP_BFALSE_R(GetSubItemRect(iRow, iCol, iArea, rcCol), FALSE);

    COMP_BFALSE_R(GetItemRect(iRow, rc, iArea), FALSE);

    rc.right = rcCol.left;

    return TRUE;
}

void CMylistctrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    int         iItem = 0;
    CString     strTemp;
    POSITION    pos = GetFirstSelectedItemPosition();

    iItem = GetNextSelectedItem(pos);
    COMP_BE(iItem, -1);

    strTemp = GetItemText(iItem,0);

    ((CDlgHisList*)this->GetParent())->SetDataPlace(atoi(strTemp));

    ((CMySheet *)this->GetParent()->GetParent())-> SetActivePage(1); 
    /*((CMySheet *)this->GetParent())-> SetActivePage(1);*/

    CListCtrl::OnLButtonDblClk(nFlags, point);
}

void CMylistctrl::OnRButtonDown(UINT nFlags, CPoint point)
{
    CListCtrl::OnRButtonDown(nFlags, point);
}

/*  不允许排序
    因为需要根据位置获取数据的序号，排序将导致处理复杂
   */
void CMylistctrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
{
    *pResult = 0;
}

BOOL CMylistctrl::SetItemData( int iItem, DWORD dwData )
{
    ItemData *pItemData = NULL;

    COMP_BGE_R(iItem, GetItemCount(), FALSE);

    pItemData = (ItemData*)( CListCtrl::GetItemData( iItem ) );
    ASSERT_NULL_R(pItemData, FALSE);
    pItemData->dwData = dwData;

    return TRUE;
}

BOOL CMylistctrl::SetTextArray( int iItem, LPTSTR* arrpsz )
{
    ItemData* pItemData = NULL;

    ASSERT_NULL_R(arrpsz, FALSE);
    
    ASSERT( CListCtrl::GetItemData( iItem ) == NULL );
    pItemData = new ItemData;
    ASSERT_NULL_R(pItemData, FALSE);
    pItemData->arrpsz = arrpsz;

    return CListCtrl::SetItemData( iItem, reinterpret_cast<DWORD_PTR>( pItemData ) );
}

int CMylistctrl::AddItem( LPCTSTR pszText, ... )
{
    int     iCol    = 0;
    va_list list;
    LPTSTR *arrpsz  = NULL;
    const int iIndex = InsertItem( GetItemCount(), pszText );

    arrpsz = new LPTSTR[ m_iNumCols ];
    ASSERT_NULL_R(arrpsz, -1);
    arrpsz[ 0 ] = new TCHAR[ lstrlen( pszText ) + 1 ];
    ASSERT_NULL_R(arrpsz[ 0 ], -1);
    (void)lstrcpy( arrpsz[ 0 ], pszText );

    va_start( list, pszText );

    for(iCol = 1; iCol < m_iNumCols; iCol++ )
    {
        pszText = va_arg( list, LPCTSTR );
        ASSERT_VALID_STRING( pszText );
        VERIFY( CListCtrl::SetItem( iIndex, iCol, LVIF_TEXT, pszText, 0, 0, 0, 0 ) );

        arrpsz[ iCol ] = new TCHAR[ lstrlen( pszText ) + 1 ];
        ASSERT_NULL_R(arrpsz[ iCol ], -1);
        (void)lstrcpy( arrpsz[ iCol ], pszText );
    }

    va_end( list );

    VERIFY( SetTextArray( iIndex, arrpsz ) );

    return iIndex;
}

int CMylistctrl::AddItemList( CStringList& slItem )
{
    POSITION    tPos;
    int     iCol = 0;
    //va_list list;
    LPTSTR *arrpsz  = NULL;
    CString strItem;
    
    tPos = slItem.GetHeadPosition();
    if (tPos == NULL)
        return -1;

    strItem = slItem.GetNext(tPos);
    const int iIndex = InsertItem(GetItemCount(), strItem);

    arrpsz = new LPTSTR[ m_iNumCols ];
    ASSERT_NULL_R(arrpsz, -1);
    arrpsz[ 0 ] = new TCHAR[ strItem.GetLength() + 1 ];
    ASSERT_NULL_R(arrpsz[ 0 ], -1);
    (void)lstrcpy( arrpsz[ 0 ], strItem );
    iCol++;

    //va_start( list, slItem);

    while(tPos != NULL)
    {
        strItem = slItem.GetNext(tPos);
        VERIFY( CListCtrl::SetItem( iIndex, iCol, LVIF_TEXT, strItem, 0, 0, 0, 0 ) );

        arrpsz[ iCol ] = new TCHAR[ strItem.GetLength() + 1 ];
        ASSERT_NULL_R(arrpsz[ iCol ], -1);
        (void)lstrcpy( arrpsz[ iCol ], strItem );
        iCol++;
    }
    if (iCol < m_iNumCols)
        int kkk = 1;

    //va_end( list );

    VERIFY( SetTextArray( iIndex, arrpsz ) );

    return iIndex;
}

BOOL CMylistctrl::SetHeadings( UINT nStringID )
{
    CString strHeadings;

    VERIFY( strHeadings.LoadString( nStringID ) );
    return SetHeadings( strHeadings );
}

BOOL CMylistctrl::SetHeadings( const CString& strHeadings )
{
    int iStart      = 0;
    int iComma      = 0;
    int iSemiColon  = 0;
    int iWidth      = 0;
    CString strHeading;

    for(int nIndex=0;nIndex<m_iNumCols;nIndex++)   
        DeleteColumn(0);
    m_iNumCols = 0;

    for( ;; )
    {
        iComma = strHeadings.Find( _T(','), iStart );

        if( iComma == -1 )
            break;

        strHeading = strHeadings.Mid( iStart, iComma - iStart );

        iStart = iComma + 1;

        iSemiColon = strHeadings.Find( _T(';'), iStart );

        if( iSemiColon == -1 )
            iSemiColon = strHeadings.GetLength();

        iWidth = atoi( strHeadings.Mid( iStart, iSemiColon - iStart ) );

        iStart = iSemiColon + 1;

        COMP_BE_R(InsertColumn( m_iNumCols++, strHeading, LVCFMT_LEFT, iWidth ), -1, FALSE);
    }

    return TRUE;
}

BOOL CMylistctrl::DeleteAllItems()
{
    int iItem = 0;

    for(iItem  = 0; iItem < GetItemCount(); iItem ++ )
        FreeItemMemory( iItem );

    return CListCtrl::DeleteAllItems();
}

BOOL CMylistctrl::SetItemText( int iItem, int iSubItem, LPCTSTR lpszText )
{
    LPTSTR  *arrpsz = NULL;
    LPTSTR  pszText = NULL;

    COMP_BFALSE_R(CListCtrl::SetItemText( iItem, iSubItem, lpszText ), FALSE);

    arrpsz = GetTextArray( iItem );
    ASSERT_NULL_R(arrpsz, FALSE);

    pszText = arrpsz[ iSubItem ];
    delete[] pszText;

    pszText = new TCHAR[ lstrlen( lpszText ) + 1 ];
    ASSERT_NULL_R(pszText, FALSE);
    (void)lstrcpy( pszText, lpszText );
    arrpsz[ iSubItem ] = pszText;

    return TRUE;
}

void CMylistctrl::FreeItemMemory( const int iItem )
{
    int     i = 0;
    ItemData    *pItemData  = NULL;
    LPTSTR      *arrpsz     = NULL;

    pItemData = reinterpret_cast<ItemData*>( CListCtrl::GetItemData( iItem ) );
    ASSERT_NULL(pItemData);
    arrpsz = pItemData->arrpsz;
    ASSERT_NULL(arrpsz);

    for(i = 0; i < m_iNumCols; i++ )
        delete[] arrpsz[ i ];

    delete[] arrpsz;
    delete pItemData;

    VERIFY( CListCtrl::SetItemData( iItem, NULL ) );
}

LPTSTR* CMylistctrl::GetTextArray( int iItem ) const
{
    ItemData *pItemData = NULL;
    ASSERT( iItem < GetItemCount() );

    pItemData = reinterpret_cast<ItemData*>( CListCtrl::GetItemData( iItem ) );
    ASSERT_NULL_R(pItemData, NULL);
    return pItemData->arrpsz;
}

DWORD CMylistctrl::GetItemData( int iItem ) const
{
    ItemData *pItemData = NULL;
    ASSERT( iItem < GetItemCount() );

    pItemData = reinterpret_cast<ItemData*>( CListCtrl::GetItemData( iItem ) );
    ASSERT_NULL_R(pItemData, 0);
    return pItemData->dwData;
}

void CMylistctrl::LoadColumnInfo()
{
}

