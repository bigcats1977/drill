// ListCtrlMap.cpp: implementation of the CListCtrlMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ListCtrlMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define VALIDMAPINDEX(iIndex)   ((iIndex) >= 0 && (iIndex) < IT_MAX)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PLISTCTRLMAP CListCtrlMap::m_pMap = NULL;

CListCtrlMap::CListCtrlMap()
{
}

CListCtrlMap::~CListCtrlMap()
{
}

//////////////////////////////////////////////////////////////////////
//����
//////////////////////////////////////////////////////////////////////
void CListCtrlMap::Insert(CListCtrl *pCtrl, CWnd *pWnd, int iIndex)
{
    PLISTCTRLMAP pMap = NULL;

    ASSERT_NULL(pCtrl);
    ASSERT_NULL(pWnd);

    pMap = GetCtrlMap(pCtrl);
    if(pMap == NULL)//����
    {
        pMap = new LISTCTRLMAP;
        ASSERT_NULL(pMap);

        ZeroMemory(pMap,sizeof(LISTCTRLMAP));
        InsertWnd(pMap, pWnd, iIndex);
        pMap->pCtrl = pCtrl;
        pMap->pNext = m_pMap;
        m_pMap = pMap;
    }
    else //�޸�
    {
        InsertWnd(pMap, pWnd, iIndex);
    }
}

//////////////////////////////////////////////////////////////////////
//ɾ��
//////////////////////////////////////////////////////////////////////
void CListCtrlMap::Delete(CListCtrl *pCtrl, CWnd *pWnd, int iIndex)
{
    PLISTCTRLMAP pMap = NULL;
    PLISTCTRLMAP pDel = NULL;

    ASSERT_NULL(m_pMap);
    
    //��1�ڵ�
    if (m_pMap->pCtrl == pCtrl)
    {
        /* pWndΪ�գ�ֱ��ɾ��m_pMap�󷵻� */
        if (pWnd == NULL)
        {
            delete m_pMap;
            m_pMap = NULL;
            return;
        }

        DeleteWnd(m_pMap, iIndex);
        return ;
    }

    // ѭ�����Ҵ�ɾ���ڵ����һ���ڵ�
    for (pMap = m_pMap; pMap != NULL; pMap = pMap->pNext)
    {
        pDel = pMap->pNext;
        if  (pDel != NULL && pDel->pCtrl == pCtrl)
        {
            pMap->pNext = pDel->pNext;

            /* pWndΪ�գ�ֱ��ɾ��Map */
            if (pWnd == NULL)
            {
                delete pDel;
                break;
            }
            
            DeleteWnd(pDel, iIndex);
            break;
        }
    }
}

void CListCtrlMap::DeleteWnd(PLISTCTRLMAP pMap, int iIndex)
{
    ASSERT_NULL(pMap);

    if(VALIDMAPINDEX(iIndex))
    {
        pMap->szWnd[iIndex] = NULL;
    }
}

void CListCtrlMap::InsertWnd(PLISTCTRLMAP pMap, CWnd *pWnd, int iIndex)
{
    ASSERT_NULL(pMap);

    if(VALIDMAPINDEX(iIndex))
    {
        pMap->szWnd[iIndex] = pWnd;
    }
}

CWnd *CListCtrlMap::GetSubWnd(CListCtrl *pCtrl, int iSubItem)
{
    PLISTCTRLMAP pMap = NULL;
    
    ASSERT_NULL_R(pCtrl, NULL);

    if(VALIDMAPINDEX(iSubItem))
    {
        pMap = GetCtrlMap(pCtrl);
        ASSERT_NULL_R(pMap, NULL);

        return pMap->szWnd[iSubItem];
    }

    return NULL;
}

PLISTCTRLMAP CListCtrlMap::GetCtrlMap(CListCtrl *pCtrl)
{
    PLISTCTRLMAP pMap = NULL;

    ASSERT_NULL_R(pCtrl, NULL);

    //��1��
    for (pMap = m_pMap; pMap != NULL; pMap = pMap->pNext)
    {
        COMP_BE_R(pMap->pCtrl, pCtrl, pMap);
    }

    return NULL;
}

