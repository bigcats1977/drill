//  MenuEx.h: interface for the CMenuEx class.
//  CMenuEx
//  ԭ���ߣ�lbird(֣��)
//  �޸�:(querw)������
//  �����ӿ�:
//  void LoadMenu();
//  BOOL ModifyMenuEx()
//  BOOL AppendMenuEx()
//  BOOL RemoveMenuEx()
//  void LoadToolBar()
//  ԭ�ӿ�:
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
    CString     strText;                //�˵��ı�
    UINT        nID;                    //�˵�ID
    int         iIndex;                 //�˵�ͼ��ı��
    int         iLeftImagePos;          //�˵����λͼ��λ��
    int         iSubMenuIndex;          //�����Ӳ˵�,��ʾ�������е�λ��
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
    //����˵�,������������Ժ�,�˵��������Ի����
    BOOL LoadMenu(UINT nMenu);

    //�޸�,���,ɾ���˵���,�Զ�ָ��Ϊ�Ի����
    BOOL ModifyMenuEx(UINT nPosition, UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL);
    BOOL AppendMenuEx(UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL);
    BOOL RemoveMenuEx(UINT nPosition, UINT nFlags);

    //���빤����,����ָ��һ��λͼ�������������Դ��16ɫλͼ
    void LoadToolBar(UINT nToolBar, UINT nFace);

    //������������
    void SetHighLightColor(COLORREF crColor);
    void SetBackColor(COLORREF crColor);
    void SetTextColor(COLORREF crColor);
    void SetImageLeft(UINT nBmpLeftID, BOOL bIsPopupMenu = FALSE);

    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);

    CMenuEx();
    virtual ~CMenuEx();

protected:
    void    CalculateLeftImagePos(CMenu* pPopupMenu); //���¼������λͼ��λ��(��ײ��Ĳ˵����λͼ����� 0 )
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


    int         m_iSeparator;               //�ָ����ĸ߶�

    CSize       m_szImageLeft;              //�˵����Ϊ��ĸ߶ȺͿ��
    CBitmap     m_bmpImageLeft;             //�˵���ߵ�λͼ
    int         m_iWidthLeftImage;          //�˵����λͼ�Ŀ��
    BOOL        m_bHasImageLeft;            //�Ƿ��в��λͼ

    COLORREF    m_crMenu;                   //�˵�����ɫ
    COLORREF    m_crTextSelected;           //�˵���ѡ��ʱ�ı�����ɫ
    CImageList  m_ImageList;                //�˵���λͼ�б�
    COLORREF    m_crText;                   //�˵���û��ѡ��ʱ�ı�����ɫ
    CSize       m_szImage;                  //�˵���λͼ�Ĵ�С

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
