// ListCtrlEditBase.cpp: implementation of the CListCtrlEditBaseBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ListCtrlEditBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define  PROKEYNUM      5
/* ����CListCtrlEditBase����������������� */
typedef BOOL (CListCtrlEditBase::*LCEBVKEditFun)(VOID);
typedef struct tagLCEBEDITFUNC
{
    WORD            wVKey;
    LCEBVKEditFun   fVKFunc;
}LCEBEDITFUNC;
LCEBEDITFUNC  g_tVKProcFunc[PROKEYNUM] = 
{
    {VK_RETURN,     &CListCtrlEditBase::ReturnKeyProc},
    {VK_TAB,        &CListCtrlEditBase::TabKeyProc},
    {VK_UP,         &CListCtrlEditBase::UpKeyProc},
    {VK_DOWN,       &CListCtrlEditBase::DownKeyProc},
    {VK_ESCAPE,     &CListCtrlEditBase::EscKeyProc}
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BOOL CListCtrlEditBase::Key_Shift_TAB(int &iItem,int &iSub)
{
    int     iItemCount  = 0;
    int     iSubCount   = 0;
    CWnd    *pWnd       = NULL;
    CHeaderCtrl *pHeader= NULL;

    ASSERT_NULL_R(m_pParentList, FALSE);

    iItemCount  = m_pParentList->GetItemCount();
    iItem       = m_iCurItem;
    iSub        = m_iCurSubItem;

    pHeader     = m_pParentList->GetHeaderCtrl();
    ASSERT_NULL_R(pHeader, FALSE);
    iSubCount   = pHeader->GetItemCount();

    for(;;)
    {
        iSub += 1;
        if(iSub >= iSubCount) //��ĩ
        {
            if(iItem == iItemCount-1) //��ĩ
            {
                iItem = m_iCurItem;
                iSub  = m_iCurSubItem;
                return FALSE;
            }

            //����һ��
            iSub   = 0;
            iItem += 1;
        }
        pWnd = m_CtrlMap.GetSubWnd(m_pParentList, iSub);
        if (pWnd)
        {
            pWnd->PostMessage(WM_USER_SHOW_EDIT, iItem, iSub);
            return TRUE;
        }
    }//end for

    if(iItem >= iItemCount)
        iItem = iItemCount-1;
    return FALSE;
}

BOOL CListCtrlEditBase::Key_Shift_Other(int &iItem,int &iSub)
{
    int     iItemCount  = 0;
    int     iOp         = 1;
    int     iSubCount   = 0;
    CWnd    *pWnd       = NULL;
    CHeaderCtrl *pHeader= NULL;

    ASSERT_NULL_R(m_pParentList, FALSE);

    iItemCount  = m_pParentList->GetItemCount();
    iItem       = m_iCurItem;
    iSub        = m_iCurSubItem;

    pHeader     = m_pParentList->GetHeaderCtrl();
    ASSERT_NULL_R(pHeader, FALSE);
    iSubCount   = pHeader->GetItemCount();

    for(;;)
    {
        iSub -= iOp;
        if( (iItem <  m_iCurItem && iSub >= iSubCount) ||
            (iItem == 0          && iSub <  0))
        {
            iItem= m_iCurItem;
            iSub = m_iCurSubItem;
            return FALSE;
        }
        if(iSub < 0)
        {
            iOp  = -1;
            iSub = 0;
            iItem --;
        }
        pWnd = m_CtrlMap.GetSubWnd(m_pParentList, iSub);
        if (pWnd )
        {
            pWnd->PostMessage(WM_USER_SHOW_EDIT, iItem, iSub);
            return TRUE;
        }
    }//end for

    if(iItem < 0)
        iItem = iItemCount-1;
    return TRUE;
}

BOOL CListCtrlEditBase::Key_Shift(int &iItem,int &iSub)
{
    short   sRet = 0;
    
    sRet = GetKeyState(VK_SHIFT);
    sRet = sRet >> 15;

    if(sRet == 0)
        return Key_Shift_TAB(iItem, iSub);

    return Key_Shift_Other(iItem, iSub);
}

BOOL CListCtrlEditBase::Key_Ctrl(int &iItem,int &iSub)
{
    short sRet          = 0;
    int   iItemCount    = 0;
    CWnd  *pWnd         = NULL;

    ASSERT_NULL_R(m_pParentList, FALSE);

    sRet  = GetKeyState(VK_CONTROL);
    iItem = m_iCurItem;
    iSub  = m_iCurSubItem;
    sRet  = sRet >> 15;
    iItemCount = m_pParentList->GetItemCount();

    ASSERT_ZERO_R(sRet, FALSE);

    iItem = (iItem >= iItemCount-1) ? 0 : (iItem+=1);
    pWnd  = m_CtrlMap.GetSubWnd(m_pParentList, iSub);
    if (pWnd)
    {
        pWnd->PostMessage(WM_USER_SHOW_EDIT, iItem, iSub);
    }

    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CListCtrlEditBase::CListCtrlEditBase()
{
    m_pParentList   = NULL;
    m_iCurItem      = -1;
    m_iCurSubItem   = -1;
    m_bExchange     = false;
    m_pInEdit       = NULL;
}

void CListCtrlEditBase::OnEditEnd()
{
    CString strText(_T(""));

    ASSERT_NULL(m_pParentList);
    ASSERT_NULL(m_pInEdit);

    m_pInEdit->GetWindowText(strText);
    m_pInEdit->ShowWindow(SW_HIDE);
    m_pParentList->SetItemText(m_iCurItem,m_iCurSubItem,strText);
}

////////////////////////////////////////////////////////////////
//����༭״̬
//IN :  BOOL bShow  �Ƿ���ʾ
//      int iItem   �к�
//      int iIndex  �к�
//      DWORD dwAddStyle ��������
////////////////////////////////////////////////////////////////
void CListCtrlEditBase::ShowEdit(int iItem,int iSubItem)
{
    int         i           = 0;
    int         iColCount   = 0;
    int         iOffset     = 0;
    DWORD       dwStyle;    // �еĶ��뷽ʽ
    CRect       rect;
    CRect       rcClient;   // ������ʱʹ��
    CSize       size;       // ������ʱʹ��
    CHeaderCtrl *pHeader    = NULL;
    LV_COLUMN   lvcol;
    CString     strItem;

    ASSERT_NULL(m_pParentList);
    ASSERT_NULL(m_pInEdit);

    COMP_BL(iItem, 0);
    COMP_BL(iSubItem, 0);

    // �пɼ�
    pHeader = (CHeaderCtrl*)m_pParentList->GetDlgItem(0);
    ASSERT_NULL(pHeader);
    iColCount = pHeader->GetItemCount();
    COMP_BGE(iSubItem, iColCount);
    if(m_pParentList->GetColumnWidth(iSubItem) < 5 )
        return;

    // ��ƫ��
    for( i = 0; i < iSubItem; i++ )
    {
        iOffset += m_pParentList->GetColumnWidth( i );
    }

    m_pParentList->GetItemRect(iItem, &rect, LVIR_BOUNDS);

    // �����У����ڲ���
    m_pParentList->GetClientRect( &rcClient );
    if( iOffset + rect.left < 0 || iOffset + rect.left > rcClient.right )
    {
        size.cx = iOffset + rect.left;
        size.cy = 0;
        m_pParentList->Scroll( size );
        rect.left -= size.cx;
    }

    // ��ȡ�еĶ��뷽ʽ
    lvcol.mask = LVCF_FMT;
    m_pParentList->GetColumn( iSubItem, &lvcol );
    // Ĭ������ΪCENTER
    dwStyle = ES_CENTER; 
    if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
        dwStyle = ES_LEFT;
    else if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
        dwStyle = ES_RIGHT;

    rect.left += iOffset + 1;
    rect.right = rect.left + m_pParentList->GetColumnWidth( iSubItem );
    if( rect.right > rcClient.right) 
        rect.right = rcClient.right;
    
    strItem = m_pParentList->GetItemText(iItem,iSubItem);
    m_pParentList->ClientToScreen(rect);
    m_pInEdit->GetParent()->ScreenToClient(rect);
    m_pInEdit->MoveWindow(rect);
    m_pInEdit->ShowWindow(SW_SHOW);
    m_pInEdit->SetWindowText(strItem);
    ::SetFocus(m_pInEdit->GetSafeHwnd());
    SendMessage(m_pInEdit->GetSafeHwnd(), EM_SETSEL, 0, -1);

    m_iCurItem = iItem;
    m_iCurSubItem = iSubItem;
}

//////////////////////////////////////////////////////////////////////
//���ص�ǰ�к���
//In: pCtrl ��ѡ��CListCtrl�ؼ�
//    pNMHDR ˫��ʱ�Ĳ���
//iItem ���ص��к�
//iSubItem ���ص��к�
//////////////////////////////////////////////////////////////////////
BOOL CListCtrlEditBase::HitTestEx(CListCtrl *pCtrl, NMHDR* pNMHDR, int &iItem, int &iSubItem)
{
    int         iColnum      = 0;
    int         iBottom      = 0;
    int         iColCount    = 0;
    int         iColWidth    = 0;
    CPoint      point;
    CRect       rect;
    NMLISTVIEW  *pNMListView = NULL;
    CHeaderCtrl *pHeader     = NULL;
    
    ASSERT_NULL_R(pCtrl, FALSE);
    ASSERT_NULL_R(pNMHDR, FALSE);
    
    pNMListView = (NM_LISTVIEW*)pNMHDR;
    point = pNMListView->ptAction;

    iItem = pCtrl->HitTest( point, NULL );
    iSubItem = 0;

    // ֻ���LVS_REPORT��ʽ
    if( (GetWindowLong(pCtrl->m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )
        return TRUE;

    // ��ȡ�ɼ��ĵ׶���
    iItem   = pCtrl->GetTopIndex();
    iBottom = iItem + pCtrl->GetCountPerPage();
    if( iBottom > pCtrl->GetItemCount() )
        iBottom = pCtrl->GetItemCount();

    // ��ȡ����
    pHeader   = (CHeaderCtrl*)pCtrl->GetDlgItem(0);
    ASSERT_NULL_R(pHeader, FALSE);
    iColCount = pHeader->GetItemCount();

    // �ӿɼ���������
    for( ;iItem <= iBottom; iItem++)
    {
        // ��ȡ������ڵ�λ�õ�BOUNDS
        pCtrl->GetItemRect( iItem, &rect, LVIR_BOUNDS );
        if( rect.PtInRect(point) )
        {
            // ͨ���ȽϿ�ȣ��ҵ���
            for( iColnum = 0; iColnum < iColCount; iColnum++ )
            {
                iColWidth = pCtrl->GetColumnWidth(iColnum);
                if( point.x >=  rect.left &&
                    point.x <= (rect.left + iColWidth ) )
                {
                    iSubItem = iColnum;
                    return TRUE;
                }
                rect.left += iColWidth;
            }
        }
    }
    return FALSE;
}

void CListCtrlEditBase::SetListCtrl(CListCtrl *pCtrl)
{
    ASSERT_NULL(pCtrl);

    m_pParentList = pCtrl;
}

void CListCtrlEditBase::Insert(int iIndex)
{
    ASSERT_NULL(m_pInEdit);
    ASSERT_NULL(m_pParentList);

    m_CtrlMap.Insert(m_pParentList, m_pInEdit, iIndex);
}

void CListCtrlEditBase::Delete(int iIndex)
{
    ASSERT_NULL(m_pInEdit);
    ASSERT_NULL(m_pParentList);

    m_CtrlMap.Delete(m_pParentList, m_pInEdit, iIndex);
}

void CListCtrlEditBase::OnKillFocusEx()
{
    ASSERT_NULL(m_pInEdit);

    if(m_bExchange)
    {
        OnEditEnd();
        return;
    }

    /* else */
    m_pInEdit->ShowWindow(SW_HIDE);
}

BOOL CListCtrlEditBase::ReturnKeyProc()
{
    int     iItem   = 0;
    int     iSub    = 0;
    DWORD   dwStyle = 0;
    CWnd    *pParent= NULL;

    ASSERT_NULL_R(m_pInEdit, FALSE);

    if(m_pInEdit->m_hWnd != NULL && m_pParentList != NULL)
    {
        dwStyle = m_pInEdit->GetStyle();
        COMP_BNE_R((dwStyle&WS_VISIBLE), WS_VISIBLE, FALSE);
        
        OnEditEnd();
        if(FALSE == Key_Ctrl(iItem,iSub))
            Key_Shift(iItem,iSub);
        
        SetSelectPos();
        return TRUE;
    }

    pParent = m_pInEdit->GetParent();
    m_bExchange = TRUE;
    OnEditEnd();
    m_pInEdit->ShowWindow(SW_HIDE);
    return true;
}

BOOL CListCtrlEditBase::TabKeyProc()
{
    int     iItem   = 0;
    int     iSub    = 0;
    DWORD   dwStyle = 0;
    
    ASSERT_NULL_R(m_pInEdit, FALSE);
    ASSERT_NULL_R(m_pParentList, FALSE);
    ASSERT_NULL_R(m_pInEdit->m_hWnd, FALSE);
    
    dwStyle = m_pInEdit->GetStyle();
    COMP_BNE_R((dwStyle&WS_VISIBLE), WS_VISIBLE, FALSE);
    
    OnEditEnd();
    if(FALSE == Key_Ctrl(iItem,iSub))
        Key_Shift(iItem,iSub);
    
    SetSelectPos();
    return TRUE;
}

BOOL CListCtrlEditBase::UpKeyProc()
{
    CWnd *pWnd = NULL;
    
    if (m_iCurItem > 0) 
        m_iCurItem--;

    pWnd = m_CtrlMap.GetSubWnd(m_pParentList, m_iCurSubItem);
    if (pWnd)
    {
        pWnd->PostMessage(WM_USER_SHOW_EDIT, m_iCurItem, m_iCurSubItem);
        SetSelectPos();
    }

    return TRUE;
}

BOOL CListCtrlEditBase::DownKeyProc()
{
    CWnd *pWnd = NULL;

    ASSERT_NULL_R(m_pParentList, FALSE);

    if (m_iCurItem < m_pParentList->GetItemCount() - 1) 
        m_iCurItem++;

    pWnd = m_CtrlMap.GetSubWnd(m_pParentList, m_iCurSubItem);
    if (pWnd )
    {
        pWnd->PostMessage(WM_USER_SHOW_EDIT, m_iCurItem, m_iCurSubItem);
        SetSelectPos();
    }

    return TRUE;
}

BOOL CListCtrlEditBase::EscKeyProc()
{    
    ASSERT_NULL_R(m_pInEdit, FALSE);
    m_bExchange = FALSE;
    m_pInEdit->ShowWindow(SW_HIDE);

    return TRUE;
}

BOOL CListCtrlEditBase::VKEditProc(WORD wKey)
{
    int     i = 0;
    
    for(i = 0; i < PROKEYNUM; i++)
    {
        if(wKey == g_tVKProcFunc[i].wVKey)
        {
            return (this->* g_tVKProcFunc[i].fVKFunc)();
        }
    }
    
    return FALSE;
}

BOOL CListCtrlEditBase::PreTranslateMessageEx(MSG *pMsg)
{
    ASSERT_NULL_R(pMsg, FALSE);

    switch (pMsg->message)
    {
    case  WM_USER_SHOW_EDIT:
        ShowEdit((int)pMsg->wParam, (int)pMsg->lParam);
        return TRUE;

    case  WM_KEYDOWN:
        return VKEditProc(pMsg->wParam);
    }//end switch

    return FALSE;
}

void CListCtrlEditBase::OnDestroyEx()
{
    ASSERT_NULL(m_pParentList);

    m_CtrlMap.Delete(m_pParentList, NULL, 0);
}

void CListCtrlEditBase::SetListCtrl(CListCtrl *pCtrl, CWnd *pEdit)
{
    m_pParentList = pCtrl;
    m_pInEdit     = pEdit;
}

BOOL CListCtrlEditBase::OnOpen(CListCtrl *pCtrl, NMHDR *pNMHDR)
{
    int          iItem    = 0;
    int          iSubItem = 0;
    CWnd         *pWnd    = NULL;
    
    ASSERT_NULL_R(pCtrl, FALSE);
    ASSERT_NULL_R(pNMHDR, FALSE);

    if (!HitTestEx(pCtrl, pNMHDR, iItem, iSubItem))
        return FALSE;
    
    pWnd = CListCtrlMap::GetSubWnd(pCtrl, iSubItem);
    ASSERT_NULL_R(pWnd, FALSE);

    pWnd->PostMessage(WM_USER_SHOW_EDIT, iItem, iSubItem);
    return TRUE;
}

void CListCtrlEditBase::SetSelectPos()
{
    int      iItem = 0;
    POSITION pos;

    ASSERT_NULL(m_pParentList);

    pos = m_pParentList->GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        while (pos)
        {
            iItem = m_pParentList->GetNextSelectedItem(pos);
            m_pParentList->SetItemState(iItem,0,LVIS_SELECTED);
        }
    }
    return;
}

