// ListCtrlEditBase.h: interface for the CListCtrlEditBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTCTRLEDITBASE_H__C64C191A_EDE7_42CA_A824_74129CE641A2__INCLUDED_)
#define AFX_LISTCTRLEDITBASE_H__C64C191A_EDE7_42CA_A824_74129CE641A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ListCtrlMap.H"
#include "DataMod.h"

#ifndef EA_DEFAULT
#define EA_DEFAULT ES_AUTOHSCROLL|WS_CHILD|ES_LEFT|ES_WANTRETURN|WS_BORDER
#endif

class CListCtrlEditBase
{
    // Construction
public:
    int          m_iCurItem;
    int          m_iCurSubItem;
    BOOL         m_bExchange;
    CWnd* m_pInEdit;
    CListCtrl* m_pParentList;
    CListCtrlMap m_CtrlMap;
public:
    static  BOOL OnOpen(CListCtrl* pCtrl, NMHDR* pNMHDR);
    static  BOOL HitTestEx(CListCtrl* pCtrl, NMHDR* pNMHDR, int& iItem, int& iSubItem);
    virtual void OnEditEnd();
    virtual void ShowEdit(int iItem, int iSubItem);
    virtual BOOL PreTranslateMessageEx(MSG* pMsg);

    BOOL Key_Shift(int& iItem, int& iSub);
    BOOL Key_Ctrl(int& iItem, int& iSub);
    void Delete(int iIndex);
    void Insert(int iIndex);
    void SetListCtrl(CListCtrl* pCtrl);
    void SetSelectPos();
    void SetListCtrl(CListCtrl* pCtrl, CWnd* pEdit);
    void OnDestroyEx();
    void OnKillFocusEx();

    BOOL ReturnKeyProc();
    BOOL TabKeyProc();
    BOOL UpKeyProc();
    BOOL DownKeyProc();
    BOOL EscKeyProc();
protected:
    CListCtrlEditBase();
    //  virtual ~CListCtrlEditBase();

private:
    BOOL VKEditProc(WORD wKey);
    BOOL Key_Shift_TAB(int& iItem, int& iSub);
    BOOL Key_Shift_Other(int& iItem, int& iSub);
};

#endif // !defined(AFX_LISTCTRLEDITBASE_H__C64C191A_EDE7_42CA_A824_74129CE641A2__INCLUDED_)

