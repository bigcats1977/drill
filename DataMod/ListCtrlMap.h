// ListCtrlMap.h: interface for the CListCtrlMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTCTRLMAP_H__6CB8880F_7675_4BE6_A705_2A6704E5C618__INCLUDED_)
#define AFX_LISTCTRLMAP_H__6CB8880F_7675_4BE6_A705_2A6704E5C618__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define IT_MAX 256

typedef struct _tagListCtrlMap
{
    CListCtrl* pCtrl;
    CWnd* szWnd[IT_MAX];
    struct _tagListCtrlMap* pNext;
}LISTCTRLMAP, * PLISTCTRLMAP;

class CListCtrlMap
{
public:
    static PLISTCTRLMAP GetCtrlMap(CListCtrl* pCtrl);
    static CWnd* GetSubWnd(CListCtrl* pCtrl, int iSubItem);
    void    Insert(CListCtrl* pCtrl, CWnd* pWnd, int iIndex);
    void    Delete(CListCtrl* pCtrl, CWnd* pWnd, int inIndex);
    void    DeleteWnd(PLISTCTRLMAP pMap, int iIndex);
    void    InsertWnd(PLISTCTRLMAP pMap, CWnd* pWnd, int iIndex);

    CListCtrlMap();
    virtual ~CListCtrlMap();

    static PLISTCTRLMAP m_pMap;
};

#endif // !defined(AFX_LISTCTRLMAP_H__6CB8880F_7675_4BE6_A705_2A6704E5C618__INCLUDED_)
