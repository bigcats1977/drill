//  MenuEx.h: interface for the CMenuEx class.
//  CMenuEx
//  原作者：lbird(郑恒)
//  修改:(querw)阙荣文
//  新增接口:
//  void LoadMenu();
//  BOOL ModifyMenuEx()
//  BOOL AppendMenuEx()
//  BOOL RemoveMenuEx()
//  void LoadToolBar()
//  原接口:
//  void SetHighLightColor(COLORREF crColor)
//  void SetBackColor(COLORREF)
//  void SetTextColor(COLORREF)
//  void SetImageLeft(UINT nBmpLeftID)
//
//                      2004.7.23
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUEX_H__FE677F6B_2315_11D7_8869_BB2B2A4F4D45__INCLUDED_)
#define AFX_MENUEX_H__FE677F6B_2315_11D7_8869_BB2B2A4F4D45__INCLUDED_
#include "afxtempl.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct tagMENUITEM
{
    CString     strText;                //菜单文本
    UINT        nID;                    //菜单ID
    int         iIndex;                 //菜单图标的编号
    int         iLeftImagePos;          //菜单左边位图的位置
    int         iSubMenuIndex;          //若是子菜单,表示在数组中的位置
    tagMENUITEM()
    {
        strText = "";
        nID = 0;
        iIndex = -1;
        iLeftImagePos = -1;
        iSubMenuIndex = -1;
        pImageList = NULL;
        pLeftBitmap = NULL;
    };
    CImageList* pImageList;
    CBitmap* pLeftBitmap;
}MENUITEM, * LPMENUITEM;


///////////////////////////////////////////
class CMenuEx : public CMenu
{
    DECLARE_DYNAMIC(CMenuEx)
public:
    //读入菜单,调用这个函数以后,菜单就有了自画风格
    BOOL LoadMenu(UINT nMenu);

    //修改,添加,删除菜单项,自动指定为自画风格
    BOOL ModifyMenuEx(UINT nPosition, UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL);
    BOOL AppendMenuEx(UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL);
    BOOL RemoveMenuEx(UINT nPosition, UINT nFlags);

    //读入工具条,可以指定一张位图以替代工具条资源的16色位图
    void LoadToolBar(UINT nToolBar, UINT nFace);

    //其他各种设置
    void SetHighLightColor(COLORREF crColor);
    void SetBackColor(COLORREF crColor);
    void SetTextColor(COLORREF crColor);
    void SetImageLeft(UINT nBmpLeftID, BOOL bIsPopupMenu = FALSE);

    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);

    CMenuEx();
    virtual ~CMenuEx();

protected:
    void    CalculateLeftImagePos(CMenu* pPopupMenu); //重新计算左边位图的位置(最底部的菜单项的位图标号是 0 )
    void    ChangeMenuStyle(CMenu* pMenu, HMENU hNewMenu);
    int     GetImageFromToolBar(UINT nToolBar, CToolBar* pToolBar,
        COLORREF crMask = RGB(192, 192, 192),
        UINT nBitmap = 0);
    void    DrawImageLeft(CDC* pDC, CRect& rcItem, LPMENUITEM lpItem);
    void    TextMenu(CDC* pDC, CRect rcItem, CRect rcText, BOOL bSelected, BOOL bGrayed,
        LPMENUITEM lpItem);
    void    DrawMenuItemImage(CDC* pDC, CRect& rcItem, BOOL bSelected, BOOL bChecked,
        BOOL bGrayed, BOOL bHasImage, LPMENUITEM lpItem);
    void    GrayString(CDC* pDC, const CString& strGray, const CRect rcItem);


    int         m_iSeparator;               //分割条的高度

    CSize       m_szImageLeft;              //菜单左边为体的高度和宽度
    CBitmap     m_bmpImageLeft;             //菜单左边的位图
    int         m_iWidthLeftImage;          //菜单左边位图的宽度
    BOOL        m_bHasImageLeft;            //是否有侧边位图

    COLORREF    m_crMenu;                   //菜单背景色
    COLORREF    m_crTextSelected;           //菜单被选中时文本的颜色
    CImageList  m_ImageList;                //菜单项位图列表
    COLORREF    m_crText;                   //菜单项没被选中时文本的颜色
    CSize       m_szImage;                  //菜单项位图的大小

    CArray<CMenuEx*, CMenuEx*> m_SubMenuArr;
    CArray<MENUITEM*, MENUITEM*>m_MenuItemArr;

private:
    BOOL    DrawCheckedMenuItem(CDC* pDC, CRect& rcItem, BOOL bSelected,
        BOOL bChecked, BOOL bGrayed);
    void    DrawMenuImage(CDC* pDC, CRect& rcItem, BOOL bSelected,
        BOOL bGrayed, LPMENUITEM lpItem);
    void    ChangeSeparatorMenuStyle(HMENU hNewMenu, int iIndex);
    void    ChangeSubMenuStyle(CMenu* pMenu, HMENU hNewMenu, int iIndex);
    void    ChangeNormalMenuStyle(CMenu* pMenu, HMENU hNewMenu, UINT nID, int iIndex);
};

#endif // !defined(AFX_MENUEX_H__FE677F6B_2315_11D7_8869_BB2B2A4F4D45__INCLUDED_)
