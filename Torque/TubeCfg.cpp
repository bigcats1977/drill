#include "stdafx.h"
#include "TubeCfg.h"
#include "DBText.h"


CTubeCfg::CTubeCfg()
{
    InitVar();
}

CTubeCfg::~CTubeCfg()
{
    int i = 0;
    for (i = 0; i < MAXTUBECFGNUM; i++)
    {
        if (m_tTubInfo[i].bDbData && m_tTubInfo[i].ptPara != NULL)
        {
            delete[]  m_tTubInfo[i].ptPara;
        }
    }

    if (m_tTubCfg.bDbData && m_tTubCfg.ptCfg != NULL)
        delete[] m_tTubCfg.ptCfg;
}

void CTubeCfg::InitVar()
{
    int i = 0;
    for (i = 0; i < MAXTUBECFGNUM; i++)
    {
        m_tTubInfo[i].bDbData = FALSE;
        m_tTubInfo[i].nNum = 0;
        m_tTubInfo[i].ptPara = NULL;
    }

    m_tTubCfg.bDbData = FALSE;
    m_tTubCfg.nNum = 0;
    m_tTubCfg.ptCfg = NULL;
}

void CTubeCfg::GetTubParaAddr(int index, FIXTUBEINFO* ptTub)
{
    ASSERT_NULL(ptTub);

    ptTub->bDbData = FALSE;

    switch (index)
    {
    case INDEX_TUBE_FACTORY:
        ptTub->nNum = MAXDEFFACTORYNUM;
        ptTub->ptPara = &g_tDefFactory[0];
        break;
    case INDEX_TUBE_OEM:
        ptTub->nNum = MAXDEFOEMNUM;
        ptTub->ptPara = &g_tDefOEM[0];
        break;
    case INDEX_TUBE_SIZE:
        ptTub->nNum = MAXDEFSIZENUM;
        ptTub->ptPara = &g_tDefSize[0];
        break;
    case INDEX_TUBE_MATER:
        ptTub->nNum = MAXDEFMATERNUM;
        ptTub->ptPara = &g_tDefMater[0];
        break;
    case INDEX_TUBE_COUPL:
        ptTub->nNum = MAXDEFCOUPLNUM;
        ptTub->ptPara = &g_tDefCoupl[0];
        break;
    default:
        ptTub->nNum = 0;
        ptTub->ptPara = NULL;
        break;
    }
}

void CTubeCfg::InitConfig()
{
    for (int i = 0; i < MAXTUBECFGNUM; i++)
    {
        GetTubParaAddr(i, &m_tTubInfo[i]);
    }

    m_tTubCfg.nNum = MAXDEFTUBECFGNUM;
    m_tTubCfg.ptCfg = &g_tDefTubeCfg[0];
}

BOOL CTubeCfg::CheckIndex(UINT nIndex)
{
    COMP_BL_R(nIndex, INDEX_TUBE_FACTORY, FALSE);
    COMP_BG_R(nIndex, INDEX_TUBE_COUPL, FALSE);
    return TRUE;
}

BOOL CTubeCfg::CheckInFactory(UINT nIndex, UINT nFactory)
{
    COMP_BL_R(nIndex, (256 * nFactory), FALSE);
    COMP_BGE_R(nIndex, (256 * (nFactory + 1)), FALSE);
    return TRUE;
}

void CTubeCfg::SetComboBoxValues(CComboBox* ptCCB, int index, set<UINT> &lsNO, UINT nCurVal, UINT nLang)
{
    UINT i = 0;
    int iCur = 0;
    set<UINT>::iterator it;
    FIXTUBEPARA* ptPara = NULL;
    
    ASSERT_NULL(ptCCB);
    COMP_BLE(lsNO.size(), 0);
    COMP_BL(index, INDEX_TUBE_FACTORY);
    COMP_BG(index, INDEX_TUBE_COUPL);
    CheckLanguage(nLang);

    ptCCB->ResetContent();
    for (i = 0; i < m_tTubInfo[index].nNum; i++)
    {
        ptPara = &m_tTubInfo[index].ptPara[i];
        it = lsNO.find(ptPara->nNO);
        if (it == lsNO.end())
            continue;

        ptCCB->AddString(ptPara->strName[nLang].c_str());
        if (nCurVal == ptPara->nNO)
            iCur = ptCCB->GetCount()-1;
    }
    ptCCB->SetCurSel(iCur);
    lsNO.clear();
}

/* 厂家 */
void CTubeCfg::SetFactoryBox(CComboBox* ptCCB, TUBECFG* ptTubing, UINT nLang)
{
    UINT    i = 0;
    int     iCur = 0;

    ASSERT_NULL(ptCCB);
    ASSERT_NULL(ptTubing);
    CheckLanguage(nLang);

    ptCCB->ResetContent();
    for (i = 0; i < m_tTubInfo[INDEX_TUBE_FACTORY].nNum; i++)
    {
        if (ptTubing->nFixTube[INDEX_TUBE_FACTORY] == m_tTubInfo[INDEX_TUBE_FACTORY].ptPara[i].nNO)
            iCur = i;
        //ptCCB->AddString(m_tTubInfo[INDEX_TUBE_FACTORY].ptPara[i].aucName[nLang]);
        ptCCB->AddString(m_tTubInfo[INDEX_TUBE_FACTORY].ptPara[i].strName[nLang].c_str());
    }
    ptCCB->SetCurSel(iCur);
}

void CTubeCfg::SetAllComboBox(UINT nIndex, CComboBox* ptCCB, TUBECFG* ptTubing, UINT nLang)
{
    UINT    i = 0;
    set<UINT>  OEMs;
    FIXTUBEINFO* ptTubInfo = NULL;

    ASSERT_NULL(ptCCB);
    ASSERT_NULL(ptTubing);
    CheckLanguage(nLang);

    ptTubInfo = &m_tTubInfo[nIndex];

    for (i = 0; i < ptTubInfo->nNum; i++)
    {
        if (!CheckInFactory(ptTubInfo->ptPara[i].nNO, ptTubing->nFixTube[INDEX_TUBE_FACTORY]))
            continue;

        OEMs.insert(ptTubInfo->ptPara[i].nNO);
    }

    SetComboBoxValues(ptCCB, nIndex, OEMs, ptTubing->nFixTube[nIndex], nLang);
}

/* 厂家规格 */
void CTubeCfg::SetOEMBox(CComboBox *ptCCB, TUBECFG *ptTubing, UINT nLang)
{
    UINT    i       = 0;
    set<UINT>  OEMs;

    ASSERT_NULL(ptCCB);
    ASSERT_NULL(ptTubing);
    CheckLanguage(nLang);

    for(i=0; i< m_tTubCfg.nNum; i++)
    {
        if(m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_FACTORY] != ptTubing->nFixTube[INDEX_TUBE_FACTORY])
            continue;

        OEMs.insert(m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_OEM]);
    }

    SetComboBoxValues(ptCCB, INDEX_TUBE_OEM, OEMs, ptTubing->nFixTube[INDEX_TUBE_OEM], nLang);
}

/* 管件规格 */
void CTubeCfg::SetSizeBox(CComboBox *ptCCB, TUBECFG *ptTubing, UINT nLang)
{
    UINT     i       = 0;
    set<UINT>  SIZEs;

    ASSERT_NULL(ptCCB);
    ASSERT_NULL(ptTubing);
    CheckLanguage(nLang);

    for(i=0; i< m_tTubCfg.nNum; i++)
    {
        if(m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_OEM] != ptTubing->nFixTube[INDEX_TUBE_OEM])
            continue;

        SIZEs.insert(m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_SIZE]);
    }

    SetComboBoxValues(ptCCB, INDEX_TUBE_SIZE, SIZEs, ptTubing->nFixTube[INDEX_TUBE_SIZE], nLang);
}

/* 扣型材质 */
void CTubeCfg::SetMatBox(CComboBox *ptCCB, TUBECFG *ptTubing, UINT nLang)
{
    UINT    i       = 0;
    set<UINT>  MATs;

    ASSERT_NULL(ptCCB);
    ASSERT_NULL(ptTubing);
    CheckLanguage(nLang);

    ptCCB->ResetContent();
    for(i=0; i< m_tTubCfg.nNum; i++)
    {
        if( m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_OEM] != ptTubing->nFixTube[INDEX_TUBE_OEM] ||
            m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_SIZE] != ptTubing->nFixTube[INDEX_TUBE_SIZE])
            continue;

        MATs.insert(m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_MATER]);
    }

    SetComboBoxValues(ptCCB, INDEX_TUBE_MATER, MATs, ptTubing->nFixTube[INDEX_TUBE_MATER], nLang);
}

/* 接箍规格 */
void CTubeCfg::SetCouplBox(CComboBox *ptCCB, TUBECFG *ptTubing, UINT nLang)
{
    UINT    i       = 0;
    set<UINT>  COUPLs;

    ASSERT_NULL(ptCCB);
    ASSERT_NULL(ptTubing);
    CheckLanguage(nLang);

    for(i=0; i< m_tTubCfg.nNum; i++)
    {
        if( m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_OEM] != ptTubing->nFixTube[INDEX_TUBE_OEM] ||
            m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_SIZE] != ptTubing->nFixTube[INDEX_TUBE_SIZE] ||
            m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_MATER] != ptTubing->nFixTube[INDEX_TUBE_MATER])
            continue;

        COUPLs.insert(m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_COUPL]);
    }

    SetComboBoxValues(ptCCB, INDEX_TUBE_COUPL, COUPLs, ptTubing->nFixTube[INDEX_TUBE_COUPL], nLang);
}

UINT CTubeCfg::GetTubeNOByName(UINT nIndex, UINT nFactory, CComboBox* ptCCB, UINT nLang)
{
    UINT i = 0;
    FIXTUBEPARA* ptPara = NULL;
    CString strTemp;

    COMP_BL_R(nIndex, INDEX_TUBE_FACTORY, DB_INVALID_UINT);
    COMP_BG_R(nIndex, INDEX_TUBE_COUPL, DB_INVALID_UINT);
    ASSERT_NULL_R(ptCCB, DB_INVALID_UINT);
    
    CheckLanguage(nLang);

    ptCCB->GetWindowText(strTemp);
    for (i = 0; i < m_tTubInfo[nIndex].nNum; i++)
    {
        ptPara = &m_tTubInfo[nIndex].ptPara[i];
        if ((INDEX_TUBE_FACTORY != nIndex) && (!CheckInFactory(ptPara->nNO, nFactory)))
            continue;
        if (0 == strTemp.CompareNoCase(ptPara->strName[nLang].c_str()))
        //if (0 == strTemp.CompareNoCase(ptPara->aucName[nLang]))
        {
            return  ptPara->nNO;
        }
    }

    return DB_INVALID_UINT;
}

BOOL CTubeCfg::CheckTubeNOChanged(UINT nIndex, UINT nCurNO, TUBECFG* ptOld, TUBECFG* ptCurrent)
{
    COMP_BL_R(nIndex, INDEX_TUBE_FACTORY, FALSE);
    COMP_BG_R(nIndex, INDEX_TUBE_COUPL, FALSE);
    ASSERT_NULL_R(ptOld, FALSE);
    ASSERT_NULL_R(ptCurrent, FALSE);

    if (nCurNO == ptOld->nFixTube[nIndex])
        return FALSE;

    ptCurrent->nFixTube[nIndex] = nCurNO;
    return TRUE;
}

void CTubeCfg::InitOEMValue(TUBECFG *ptTubing)
{
    ASSERT_NULL(ptTubing);

    ptTubing->nFixTube[INDEX_TUBE_OEM] = ptTubing->nFixTube[INDEX_TUBE_FACTORY] * 256;
}

void CTubeCfg::InitSizeValue(TUBECFG *ptTubing)
{
    UINT     i = 0;

    ASSERT_NULL(ptTubing);

    for(i=0; i< m_tTubCfg.nNum; i++)
    {
        if(m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_OEM]     == ptTubing->nFixTube[INDEX_TUBE_OEM])
        {
            ptTubing->nFixTube[INDEX_TUBE_SIZE] = m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_SIZE];
            break;
        }
    }
}

void CTubeCfg::InitMatValue(TUBECFG *ptTubing)
{
    UINT     i = 0;

    ASSERT_NULL(ptTubing);

    for(i=0; i< m_tTubCfg.nNum; i++)
    {
        if( m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_OEM]     == ptTubing->nFixTube[INDEX_TUBE_OEM]     &&
            m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_SIZE]    == ptTubing->nFixTube[INDEX_TUBE_SIZE])
        {
            ptTubing->nFixTube[INDEX_TUBE_MATER] = m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_MATER];
            break;
        }
    }
}

void CTubeCfg::InitCouplValue(TUBECFG *ptTubing)
{
    UINT     i = 0;

    ASSERT_NULL(ptTubing);

    for(i=0; i< m_tTubCfg.nNum; i++)
    {
        if( m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_OEM]     == ptTubing->nFixTube[INDEX_TUBE_OEM]     &&
            m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_SIZE]    == ptTubing->nFixTube[INDEX_TUBE_SIZE]    &&
            m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_MATER]     == ptTubing->nFixTube[INDEX_TUBE_MATER])
        {
            ptTubing->nFixTube[INDEX_TUBE_COUPL] = m_tTubCfg.ptCfg[i].nFixTube[INDEX_TUBE_COUPL];
            break;
        }
    }
}
