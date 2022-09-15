// MenuEx.cpp: implementation of the CMenuEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "torque.h"
#include "MenuEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC( CMenuEx, CMenu )
CMenuEx::CMenuEx():m_szImage(16,15)
{
    m_crMenu         =::GetSysColor(COLOR_MENU);
    m_crText         =::GetSysColor(COLOR_MENUTEXT);
    m_crTextSelected =::GetSysColor(COLOR_HIGHLIGHTTEXT);

    m_bHasImageLeft  = FALSE;
    m_iSeparator     = 10;  //sparator��Ĭ�ϸ߶�
}

CMenuEx::~CMenuEx()
{
    int         i           = 0;
    CMenuEx     *pSubMenu   = NULL;
    MENUITEM    *pMenuItem  = NULL;

    m_ImageList.DeleteImageList();

    if(m_bHasImageLeft)
        m_bmpImageLeft.DeleteObject();

    //�ͷ����е��Ӳ˵�
    for(i = 0; i < m_SubMenuArr.GetSize(); i++)
    {
        pSubMenu = m_SubMenuArr.GetAt(i);
        delete pSubMenu;
    }

    //�ͷ����еĲ˵���
    for(i = 0; i < m_MenuItemArr.GetSize(); i++)
    {
        pMenuItem = m_MenuItemArr.GetAt(i);
        delete pMenuItem;
    }

    //���ٲ˵�
    DestroyMenu();
}

/////////////////////////////////////////////////
//���˵���Ϊ������ʱ���ƻ�ɫ���ı�
void CMenuEx::GrayString(CDC *pDC, const CString &strGray, const CRect rcItem)
{
    CRect   rcTmp(rcItem);

    ASSERT_NULL(pDC);

    rcTmp.left +=1;
    rcTmp.top  +=1;

    pDC->SetTextColor(RGB(255,255,255));
    pDC->DrawText(strGray,&rcTmp,DT_EXPANDTABS|DT_VCENTER|DT_SINGLELINE);

    rcTmp.left -=1;
    rcTmp.top  -=1;
    pDC->SetTextColor(RGB(127,127,127));
    pDC->DrawText(strGray,&rcTmp,DT_EXPANDTABS|DT_VCENTER|DT_SINGLELINE);
}

BOOL CMenuEx::DrawCheckedMenuItem(CDC *pDC, CRect &rcItem, BOOL bSelected,
                                  BOOL bChecked, BOOL bGrayed)
{
    CRect   rcTmp(rcItem.left,
                  rcItem.top,
                  rcItem.left + m_szImage.cx + 4,
                  rcItem.top  + m_szImage.cy + 4);

    ASSERT_NULL_R(pDC, FALSE);
    COMP_BFALSE_R(bChecked, FALSE);

    if(bGrayed)
    {
        //�˵�������
        GrayString(pDC,"��", rcTmp);
    }
    else
    {
        if(bSelected)
        {
            //�˵�ѡ��
            //�����ѡ�н������һ���������
            //pDC->Draw3dRect(&rcTmp,RGB(255,255,255),RGB(127,127,127));
        }

        rcTmp.InflateRect(-2,-2);

        //����"��"
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(m_crText);
        pDC->DrawText("��",&rcTmp,DT_EXPANDTABS|DT_VCENTER|DT_SINGLELINE);
    }

    rcItem.left += m_szImage.cx + 4 +2 ;

    return TRUE;
}

void CMenuEx::DrawMenuImage(CDC *pDC, CRect &rcItem, BOOL bSelected,
                            BOOL bGrayed, LPMENUITEM lpItem)
{
    CRect   rcImage(rcItem.left,
                    rcItem.top,
                    rcItem.left + m_szImage.cx + 4,
                    rcItem.top  + m_szImage.cy + 4);
    CPoint  ptImage(rcImage.left+2 , rcImage.top+2 );
    UINT    nStyle = ILD_TRANSPARENT;    //CImageList::Draw()����λͼ�ķ��

    ASSERT_NULL(pDC);
    ASSERT_NULL(lpItem);

    if(bSelected)
    {
        //ptImage.x = rcImage.left + 1;
        ptImage.y   = rcImage.top + 1;
    }

    if(bGrayed)
    {
        nStyle |= ILD_BLEND50;   //�˵�����������λͼ�ϰ�
    }
/*
    else
    {
        if(bSelected)
        {
            //�����ѡ�н������һ���������
            //pDC->Draw3dRect(&rcTmp,RGB(255,255,255),RGB(127,127,127));
        }
    }
*/
//      pDC->FillSolidRect(rect.left + 2,rect.top + 2,m_szImage.cx,m_szImage.cy,0);
    lpItem->pImageList->Draw(pDC, lpItem->iIndex, ptImage, nStyle); //�ڲ˵����л���λͼ

    //�����ɻ��ƾ��εĴ�С
    //4��λͼ��Ӿ��α�λͼ��4
    //2���˵��ı���λͼ��Ӿ��εļ��Ϊ2
    //rect.left  +=m_szImage.cx + 4 + 2;

    return;
}

/////////////////////////////////////////////////
//���Ʋ˵���λͼ
void CMenuEx::DrawMenuItemImage(CDC *pDC, CRect &rcItem, BOOL bSelected, BOOL bChecked,
                                BOOL bGrayed, BOOL bHasImage, LPMENUITEM lpItem)
{
    ASSERT_NULL(pDC);

    COMP_BTRUE(DrawCheckedMenuItem(pDC, rcItem, bSelected, bChecked, bGrayed));

    COMP_BFALSE(bHasImage);
    DrawMenuImage(pDC, rcItem, bSelected, bGrayed, lpItem);
}

/////////////////////////////////////////////////
//���Ʋ˵����ı�
//������rect:������ε�RECT
//      rtText:�˵��ı���RECT
void CMenuEx::TextMenu(CDC *pDC, CRect rcItem, CRect rcText, BOOL bSelected, BOOL bGrayed,
                       LPMENUITEM lpItem)
{
    CRect   rcTmp       = rcItem;
    CPen    penBorder;
    CPen    *pOldPen    = NULL;
    CBrush  brBk;
    CBrush  *pOldBrush  = NULL;
    CPoint  ptTmp(rcText.left,rcText.top);
    CSize   szTmp(rcText.Width(),rcText.Height());

    ASSERT_NULL(pDC);
    ASSERT_NULL(lpItem);

    //ѡ��״̬�Ĳ˵���Ҫ�Ȼ����������

    if(bSelected)
    {
        penBorder.CreatePen(PS_SOLID,1,RGB(49,106,197));
        brBk.CreateSolidBrush(RGB(193,210,238));
        pOldPen = pDC->SelectObject(&penBorder);
        pOldBrush = pDC->SelectObject(&brBk);

        pDC->Rectangle(&rcTmp);

        pDC->SelectObject(pOldPen);
        pDC->SelectObject(pOldBrush);
    }

    if(bGrayed)
    {
        //GrayString(pDC,lpItem->strText,rtText);
        pDC->DrawState(ptTmp, szTmp, lpItem->strText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
        //pDC->DrawText(lpItem->strText,rtText,DT_LEFT | DT_EXPANDTABS | DT_VCENTER | DT_GRAY);
    }
    else
    {
        pDC->DrawText(lpItem->strText,rcText,DT_LEFT | DT_EXPANDTABS | DT_VCENTER);
    }
}

void CMenuEx::DrawImageLeft(CDC *pDC, CRect &rcItem,LPMENUITEM lpItem)
{
    CDC     memDC;
    CBitmap *pOldBmp = NULL;
    int     cy       = 0; //�趨�ò˵���Ӧ���Ļ���

    ASSERT_NULL(pDC);

    COMP_BFALSE(m_bHasImageLeft);
    COMP_BE(lpItem->iLeftImagePos, -1);

    COMP_BFALSE(memDC.CreateCompatibleDC(pDC));

    pOldBmp = (CBitmap *) memDC.SelectObject(&m_bmpImageLeft);

    //cy = m_szImageLeft.cy - rc.Height() * lpItem->iLeftImagePos;
    cy = 0;
    pDC->BitBlt(rcItem.left ,rcItem.top ,m_szImageLeft.cx ,rcItem.Height(),&memDC,0,cy,SRCCOPY);

    memDC.SelectObject(pOldBmp);
    memDC.DeleteDC();

    rcItem.left +=m_szImageLeft.cx+1;
}

void CMenuEx::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
    CDC         dc;
    CRect       rcTmp;
    CBrush      brBk(m_crMenu);
    CRect       rcLeft;
    CRect       rcText;
    LPMENUITEM  lpItem      = NULL;
    BOOL        bSelected   = FALSE;
    BOOL        bChecked    = FALSE;
    BOOL        bGrayed     = FALSE;
    BOOL        bHasImage   = FALSE;

    ASSERT_NULL(lpDIS);

    dc.Attach(lpDIS->hDC);
    lpItem = (LPMENUITEM)lpDIS->itemData;
    ASSERT_NULL(lpItem);

    rcTmp = CRect(lpDIS->rcItem);

    bSelected = lpDIS->itemState & ODS_SELECTED;
    bChecked  = lpDIS->itemState & ODS_CHECKED;
    bGrayed   = lpDIS->itemState & ODS_GRAYED;
    bHasImage = (lpItem->iIndex != -1);

    //��ѡ�еĻ�,ʹ�ñ�ѡ�е��ı���ɫ
    if(bSelected)
        dc.SetTextColor(m_crTextSelected);
    else
        dc.SetTextColor(m_crText);

    //�趨����ɫ
    dc.FillRect(&rcTmp, &brBk);

    rcLeft = rcTmp;
    rcLeft.right = rcLeft.left + m_szImage.cx + 5;

    rcText = rcTmp;
    rcText.left += (m_szImage.cx + 10);

    dc.FillSolidRect(&rcLeft,/*RGB(49,105,222)*/RGB(210,210,210));

    //�趨��ʾģʽ
    dc.SetBkMode(TRANSPARENT);

    //���Ʋ��λͼ
    //DrawImageLeft(&dc,rcTmp,lpItem);

    if(lpItem->nID ==0 )//�ָ���
    {
        rcText.top =rcText.Height()/2+rcText.top ;
        rcText.bottom =rcText.top +2;
        rcText.right -=2;
        dc.Draw3dRect(rcText,RGB(64,0,128),RGB(255,255,255));
    }
    else
    {
        //���Ʋ˵��ı�
        TextMenu(&dc,rcTmp,rcText,bSelected,bGrayed,lpItem);

        //���Ʋ˵�λͼ
        DrawMenuItemImage(&dc,rcTmp,bSelected,bChecked,bGrayed,bHasImage,lpItem);
    }
    dc.SetBkMode(OPAQUE);
    dc.Detach();
}

//////////////////////////////////////////////////////////
//�ɹ�������λͼ�������˵����õ�λͼ�б�m_ImageList
//crMask: ������λͼ������
int CMenuEx::GetImageFromToolBar(UINT nToolBar, CToolBar *pToolBar, COLORREF crMask, UINT nBitmap)
{
    int     i           = 0;
    int     j           = 0;
    int     iWidth      = 0;
    int     iHeight     = 0;
    int     iImgCount   = 0;
    int     iBtnCount   = 0;
    UINT    nItemID     = 0;
    CBitmap bmp;
    BITMAP  bmpInfo;
    MENUITEM *pMenuItem = NULL;

    ASSERT_NULL_R(pToolBar, 0);

    bmp.LoadBitmap(nToolBar);
    bmp.GetBitmap(&bmpInfo);

    //�õ�λͼ�ĸ߶�
    iHeight =bmpInfo.bmHeight;

    iBtnCount =pToolBar->GetToolBarCtrl().GetButtonCount();

    //�õ���������λͼ�ĸ���nCount
    for(i=0; i<iBtnCount; i++)
    {
        nItemID = pToolBar->GetItemID(i);
        for(j = 0; j < m_MenuItemArr.GetSize(); j++)
        {
            pMenuItem = m_MenuItemArr.GetAt(j);
            COMP_BNULL_CONTINUE(pMenuItem);

            if(pMenuItem->nID == nItemID)
            {
                pMenuItem->iIndex = i;
            }
        }

        if(nItemID != ID_SEPARATOR)
            iImgCount ++;
    }

/*
    for(int j = 0; j < m_MenuItemArr.GetSize(); j++)
    {
        MENUITEM *pMenuItem = m_MenuItemArr.GetAt(j);
        pMenuItem->iIndex = 2;
    }
*/

    //�����λͼ�Ŀ��
    iWidth = bmpInfo.bmWidth;
    if(iImgCount != 0)
        iWidth = bmpInfo.bmWidth/iImgCount;

    bmp.DeleteObject();

    TRACE("Menu Bitmap--width:%d\theight:%d\n", iWidth, iHeight);

    //����λͼ�б�
    if(nBitmap)
    {
        m_ImageList.Create(nBitmap,iWidth,iHeight,crMask);
    }
    else
    {
        m_ImageList.Create(nToolBar,iWidth,5,crMask);
    }
    m_szImage.cx = iWidth;
    m_szImage.cy = iHeight;

    return iImgCount;
}

void CMenuEx::MeasureItem(LPMEASUREITEMSTRUCT  lpMIS)
{
    MENUITEM *lpItem = NULL;
    CDC      *pDC    = NULL;
    CSize    szText;

    ASSERT_NULL(lpMIS);

    lpItem =(LPMENUITEM)lpMIS->itemData;
    ASSERT_NULL(lpItem);

    if(lpItem->nID == 0)//�ָ���
    {
        lpMIS->itemHeight =m_iSeparator;
        return;
    }
    
    /* else */
    pDC = AfxGetMainWnd()->GetDC();
    ASSERT_NULL(pDC);

    szText = pDC->GetTextExtent(lpItem->strText);
    lpMIS->itemWidth = szText.cx /*+m_szImage.cx+6 */ ;
    lpMIS->itemWidth += m_szImage.cx;

    //lpMIS->itemWidth += 30;
    if(/*m_bHasImageLeft &&*/ lpItem->iLeftImagePos >= 0)
        lpMIS->itemWidth += 30;

    if(lpItem->nID == -1)
    {
        lpMIS->itemWidth += 15;
    }


    lpMIS->itemHeight = m_szImage.cy + 4;

    AfxGetMainWnd()->ReleaseDC(pDC);
}

void CMenuEx::SetImageLeft(UINT nBmpLeftID,BOOL bIsPopupMenu)
{
    int         i           = 0;
    BITMAP      bmpInfo;
    MENUITEM    *pMenuItem  = NULL;

    if(m_bHasImageLeft)
        m_bmpImageLeft.DeleteObject();

    m_bmpImageLeft.LoadBitmap(nBmpLeftID);
    m_bHasImageLeft = TRUE;
    m_bmpImageLeft.GetBitmap(&bmpInfo);
    m_szImageLeft.cx =bmpInfo.bmWidth;
    m_szImageLeft.cy =bmpInfo.bmHeight;

    for(i=0; i < m_MenuItemArr.GetSize(); i++)
    {
        pMenuItem = m_MenuItemArr.GetAt(i);
        if(pMenuItem != NULL)
            pMenuItem->iLeftImagePos = 1;
    }

    //���¼���˵������λͼ��λ��
/*
    int MenuItemCount = this->GetMenuItemCount();
    if(bIsPopupMenu)
    {
        CalculateLeftImagePos(this);
    }
    else
    {
        int nMenuItemCount = this->GetMenuItemCount();
        for(int i = 0; i < nMenuItemCount; i++)
        {
            int nID = this->GetMenuItemID(i);
            if(nID == -1)
            {
                CMenu *pSubMenu = this->GetSubMenu(i);
                CalculateLeftImagePos(pSubMenu);
            }
        }
    }*/

}

void CMenuEx::SetTextColor(COLORREF crColor)
{
    int     i         = 0;
    CMenuEx *pSubMenu = NULL;

    m_crText = crColor;
    for(i = 0; i < m_SubMenuArr.GetSize(); i++)
    {
        pSubMenu = m_SubMenuArr.GetAt(i);
        if(pSubMenu != NULL)
            pSubMenu->SetTextColor(crColor);
    }
}

void CMenuEx::SetBackColor(COLORREF crColor)
{
    int     i         = 0;
    CMenuEx *pSubMenu = NULL;

    m_crMenu = crColor;
    for(i = 0; i < m_SubMenuArr.GetSize(); i++)
    {
        pSubMenu = m_SubMenuArr.GetAt(i);
        if(pSubMenu != NULL)
            pSubMenu->SetBackColor(crColor);
    }
}

void CMenuEx::SetHighLightColor(COLORREF crColor)
{
    int     i         = 0;
    CMenuEx *pSubMenu = NULL;

    m_crTextSelected = crColor;
    for(i = 0; i < m_SubMenuArr.GetSize(); i++)
    {
        pSubMenu = m_SubMenuArr.GetAt(i);
        if(pSubMenu != NULL)
            pSubMenu->SetHighLightColor(crColor);
    }
}

void CMenuEx::LoadToolBar(UINT nToolBar, UINT nFace)
{
    CToolBar m_wndToolBar;

    COMP_BFALSE(m_wndToolBar.CreateEx(AfxGetMainWnd(),
                                      TBSTYLE_FLAT,
                                      WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |
                                      CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC));
    COMP_BFALSE(m_wndToolBar.LoadToolBar(nToolBar));

    GetImageFromToolBar(nToolBar,&m_wndToolBar,RGB(192,192,192),0);
    //InitMenu(this,nToolBar,&m_wndToolBar,nFace);
}

BOOL CMenuEx::RemoveMenuEx(UINT nPosition, UINT nFlags)
{
    int      i          = 0;
    UINT     nItemID    = 0;
    MENUITEM *pMenuItem = NULL;

    nItemID = nPosition;
    if(nFlags & MF_BYPOSITION)
    {
        nItemID = GetMenuItemID((int)nPosition);
    }

    if((int) nItemID >= 0)          //��ͨ�˵����ָ���
    {
        for(i = 0; i < m_MenuItemArr.GetSize(); i++)
        {
            pMenuItem = m_MenuItemArr.GetAt(i);
            COMP_BNULL_CONTINUE(pMenuItem);

            if(pMenuItem->nID == nItemID)
            {
                m_MenuItemArr.RemoveAt(i);
                break;
            }
        }
    }
/*
    else                            //�Ӳ˵�
    {
    }
*/

    return CMenu::RemoveMenu(nPosition,nFlags);
}

/*lint -e429*/
BOOL CMenuEx::AppendMenuEx(UINT nFlags, UINT nIDNewItem, LPCTSTR lpszNewItem)
{
    MENUITEM *pMenuItem = NULL;

    pMenuItem = new MENUITEM;
    ASSERT_NULL_R(pMenuItem, FALSE);

    pMenuItem->strText       = lpszNewItem;
    pMenuItem->nID           = nIDNewItem;
    pMenuItem->iIndex        = -1;
    pMenuItem->iLeftImagePos = -1;
/*
    UINT uNewFlags = 0;
    if(nFlags & MF_CHECKED) uNewFlags |= MF_CHECKED;
    if(nFlags & MF_UNCHECKED) uNewFlags |= MF_UNCHECKED;
    if(nFlags & MF_SEPARATOR) uNewFlags |= MF_SEPARATOR;
    uNewFlags |= MF_OWNERDRAW;*/

    nFlags &= ~MF_STRING;
    nFlags |= MF_OWNERDRAW;

    m_MenuItemArr.Add(pMenuItem);
    return CMenu::AppendMenu(nFlags,nIDNewItem,(LPCTSTR)pMenuItem);
}
/*lint +e429*/

BOOL CMenuEx::LoadMenu(UINT nMenu)
{
    int     i           = 0;
    UINT    nID         = 0;
    CMenu   Menu;     //��ʱ�˵�
    CMenu   *pSubMenu   = NULL;
    HMENU   hMenu;
    HMENU   hSubMenu;
    CString strPopup;
    CString strText;

    //���¶���˵�,����Ϊpopup�˵�,�����Ի�(�ɿ�ܵ���MesureItem() �� DrawItem()
    hMenu = ::CreateMenu();
    this->Attach(hMenu);

    Menu.LoadMenu(nMenu);
    for(i = 0; i < (int)Menu.GetMenuItemCount(); i++)
    {
        nID = Menu.GetMenuItemID(i);
        if(nID == 0)            //�ָ���
        {
            ::AppendMenu(hMenu,MF_SEPARATOR,0,NULL);
        }
        else if((int)nID == -1)     //�����˵�(���Ӳ˵�)
        {
            pSubMenu = Menu.GetSubMenu(i);
            ASSERT_NULL_R(pSubMenu, FALSE);

            //�����Ӳ˵�
            hSubMenu = ::CreatePopupMenu();
            Menu.GetMenuString(i,strPopup,MF_BYPOSITION);
            ::InsertMenu(hMenu,i,MF_BYPOSITION | MF_POPUP | MF_STRING,(UINT)hSubMenu,strPopup);

            //���Ӳ˵��ݹ����ChangeMenuStyle(),���Ӳ˵���ΪMF_OWNERDRAW���
            ChangeMenuStyle(pSubMenu,hSubMenu);
        }
        else                    //�����Ĳ˵���
        {
            Menu.GetMenuString(nID,strText,MF_BYCOMMAND);
            AppendMenu(MF_STRING,nID,strText);
        }
    }

    Menu.DestroyMenu();         //������ʱ�˵�
    return TRUE;
}

void CMenuEx::ChangeSeparatorMenuStyle(HMENU hNewMenu, int iIndex)
{
    MENUITEM *pMenuItem = NULL;

    ::AppendMenu(hNewMenu,MF_SEPARATOR,0,NULL);
    //pNewMenu->AppendMenu(MF_SEPARATOR,0,NULL);
    pMenuItem = new MENUITEM;
    ASSERT_NULL(pMenuItem);
    
    pMenuItem->nID           = 0;
    pMenuItem->iIndex        = -1;
    pMenuItem->iLeftImagePos = -1;
    pMenuItem->pImageList    = &m_ImageList;
    m_MenuItemArr.Add(pMenuItem);

    ::ModifyMenu(hNewMenu,iIndex,MF_BYPOSITION | MF_OWNERDRAW,-1,(LPCTSTR)pMenuItem);
    //pNewMenu->AppendMenuEx(MF_SEPARATOR,0,NULL);
}

void CMenuEx::ChangeSubMenuStyle(CMenu *pMenu, HMENU hNewMenu, int iIndex)
{
    CMenu       *pSubMenu  = NULL;
    HMENU       hPopMenu;
    MENUITEM    *pMenuItem = NULL;
    CString     strPopup;

    ASSERT_NULL(pMenu);

    pSubMenu = pMenu->GetSubMenu(iIndex);
    ASSERT_NULL(pSubMenu);
    hPopMenu = ::CreatePopupMenu();

    pMenu->GetMenuString(iIndex,strPopup,MF_BYPOSITION);
    ::InsertMenu(hNewMenu,iIndex,MF_BYPOSITION | MF_POPUP,(UINT)hPopMenu,strPopup);

    pMenuItem = new MENUITEM;
    ASSERT_NULL(pMenuItem);
    pMenuItem->nID           = -1;
    pMenuItem->strText       = strPopup;
    pMenuItem->iIndex        = -1;
    pMenuItem->iLeftImagePos = -1;
    pMenuItem->pImageList    = &m_ImageList;
    m_MenuItemArr.Add(pMenuItem);
    ::ModifyMenu(hNewMenu,iIndex,MF_BYPOSITION | MF_OWNERDRAW,-1,(LPCTSTR)pMenuItem);

    //pNewMenu->ModifyMenuEx(i,MF_BYPOSITION,-1,strPopup);


    ChangeMenuStyle(pSubMenu,hPopMenu);
}


void CMenuEx::ChangeNormalMenuStyle(CMenu *pMenu, HMENU hNewMenu, UINT nID, int iIndex)
{
    MENUITEM *pMenuItem = NULL;
    UINT     nState     = 0;
    CString  strText;

    ASSERT_NULL(pMenu);

    pMenu->GetMenuString(nID,strText,MF_BYCOMMAND);

    pMenuItem = new MENUITEM;
    ASSERT_NULL(pMenuItem);

    pMenuItem->nID = pMenu->GetMenuItemID(iIndex);
    pMenu->GetMenuString(pMenuItem->nID,pMenuItem->strText,MF_BYCOMMAND);
    pMenuItem->iIndex        = -1;
    pMenuItem->iLeftImagePos = -1;
    pMenuItem->pImageList    = &m_ImageList;
    //::AppendMenu(hNewMenu,MF_STRING,nID,(LPCTSTR)pMenuItem->strText);
    m_MenuItemArr.Add(pMenuItem);

    nState = pMenu->GetMenuState(iIndex,MF_BYPOSITION);
    ::AppendMenu(hNewMenu,MF_OWNERDRAW | MF_BYCOMMAND | nState, nID, (LPCTSTR)pMenuItem);

    //pNewMenu->AppendMenuEx(MF_STRING,nID,strText);
}

/*lint -e429*/
void CMenuEx::ChangeMenuStyle(CMenu *pMenu, HMENU hNewMenu)
{
    int     i           = 0;
    int     iItemCount  = 0;
    UINT    nID         = 0;
    //����ΪCMenuEx(����ΪCMenuEx������Զ��ػ�
    //ԭ����(CMenu��װ�Ľ��?)
    CMenuEx *pNewMenu   = NULL;

    ASSERT_NULL(pMenu);

    pNewMenu = new CMenuEx;
    ASSERT_NULL(pNewMenu);
    pNewMenu->Attach(hNewMenu);
    m_SubMenuArr.Add(pNewMenu);

    iItemCount = pMenu->GetMenuItemCount();
    for(i = 0; i < iItemCount; i++)
    {
        nID = pMenu->GetMenuItemID(i);
        if(nID == 0)            //�ָ���
        {
            ChangeSeparatorMenuStyle(hNewMenu, i);
        }
        else if(nID == -1)      //�����˵�(���Ӳ˵�)
        {
            ChangeSubMenuStyle(pMenu, hNewMenu, i);
        }
        else                    //�����Ĳ˵���
        {
            ChangeNormalMenuStyle(pMenu, hNewMenu, nID, i);
        }
    }
}

BOOL CMenuEx::ModifyMenuEx(UINT nPosition, UINT nFlags, UINT nIDNewItem, LPCTSTR lpszNewItem)
{
    int      i          = 0;
    int      iMenuSize  = 0;
    UINT     nItemID    = 0;
    MENUITEM *pMenuItem = NULL;

    nItemID = nPosition;
    if(nFlags & MF_BYPOSITION)
    {
        nItemID = GetMenuItemID((int)nPosition);
    }

    iMenuSize = m_MenuItemArr.GetSize();
    //��ͨ�˵���͵���ʽ�˵�
    for(i = 0; i < iMenuSize; i++)
    {
        pMenuItem = m_MenuItemArr.GetAt(i);
        COMP_BNULL_CONTINUE(pMenuItem);

        if(pMenuItem->nID == nItemID)
        {
            pMenuItem->strText = lpszNewItem;
            if((int)nIDNewItem != 0)
                pMenuItem->nID = nIDNewItem;
            break;
        }
    }

    //û�ҵ���Ӧ�Ĳ˵���
    if(i >= iMenuSize)
    {
        pMenuItem = new MENUITEM;
        ASSERT_NULL_R(pMenuItem, FALSE);

        pMenuItem->strText    = lpszNewItem;
        pMenuItem->nID        = nIDNewItem;
        pMenuItem->pImageList = &m_ImageList;
        m_MenuItemArr.Add(pMenuItem);
    }

/*
    UINT uNewFlags = 0;
    if(nFlags & MF_CHECKED) uNewFlags |= MF_CHECKED;
    if(nFlags & MF_UNCHECKED) uNewFlags |= MF_UNCHECKED;
    if(nFlags & MF_BYPOSITION) uNewFlags |= MF_BYPOSITION;
    if(nFlags & MF_BYCOMMAND) uNewFlags |= MF_BYCOMMAND;
    uNewFlags |= MF_OWNERDRAW;*/

    nFlags &= ~MF_STRING;
    nFlags |= MF_OWNERDRAW;

    return CMenu::ModifyMenu(nPosition,nFlags,nIDNewItem,(LPCTSTR)pMenuItem);
}
/*lint +e429*/

void CMenuEx::CalculateLeftImagePos(CMenu *pPopupMenu)
{
    int     i           = 0;
    int     j           = 0;
    int     iItemCount  = 0;
    UINT    nItemID     = 0;
    CString strText;
    MENUITEM *pMenuItem = NULL;

    ASSERT_NULL(pPopupMenu);

    iItemCount = pPopupMenu->GetMenuItemCount();

    for(i = iItemCount - 1 ; i >= 0; i--)
    {
        nItemID = pPopupMenu->GetMenuItemID(i);
        pPopupMenu->GetMenuString(i,strText,MF_BYPOSITION);
        for(j = 0; j < m_MenuItemArr.GetSize(); j++)
        {
            pMenuItem = m_MenuItemArr.GetAt(j);
            COMP_BNULL_CONTINUE(pMenuItem);

            if((int)pMenuItem->nID == nItemID)
            {
                pMenuItem->iLeftImagePos = iItemCount - 1 - i;
            }
        }
    }
}

