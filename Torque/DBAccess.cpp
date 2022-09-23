#include "Global.h"
#include "Torque.h"
#include "TubeCfg.h"
#include "DBAccess.h"

/*********************全局变量************************************/
CTubeCfg        g_cTubing;

CDBAccess::CDBAccess()
{

}

CDBAccess::~CDBAccess()
{

}

BOOL CDBAccess::InitDBHandle()
{
    UINT i = 0;

    COMP_BFALSE_R(m_bValidDBFile, FALSE);

    m_tDBGlbCfg = CDBGlbCfg();
    m_tDBShowName = CDBShowName();
    m_tDBShowCfg = CDBShowCfg();
    m_tDBShowOpt = CDBShowOption();

    for (i = 0; i < MAXTUBECFGNUM; i++)
        m_tDBTubeText[i] = CDBText(T_TUBEFACTORY + i);
    m_tDBTubeCfg = CDBTubeCfg();

    m_tDBValTorque = CDBValTorque();
    m_tDBValTurn = CDBValTurn();
    m_tDBTorqueCfg = CDBTorqueCfg();
    m_tDBXlsStatCfg = CDBXlsStatCfg();
    m_tDBValveCfg = CDBValveCfg();

    return TRUE;
}

BOOL CDBAccess::InitConfigFromDB(int &initstep)
{
    UINT i = 0;
    string strDbFile;

    m_bValidDBFile = FALSE;

    //return FALSE;

    initstep = 0;
    strDbFile = theApp.m_strAppPath + SQLITEFILE;
    // NO DB file, using default value
    COMP_BFALSE_R(m_tSqlite.OpenDB(strDbFile), FALSE);

    m_bValidDBFile = TRUE;
    COMP_BFALSE_R(InitDBHandle(), FALSE);

    // global parameter
    COMP_BFALSE_R(ReadGlobalPara(), FALSE);
    initstep++;

    for (i = 0; i < LANGUAGE_NUM; i++)
    {
        // Show parameter
        COMP_BFALSE_R(ReadShowPara(&theApp.m_tShowCfg[i], i), FALSE);

        // excel statastic config
        COMP_BFALSE_R(ReadXlsStatPara(&theApp.m_tXlsStatCfg[i], i), FALSE);
    }
    initstep++;

    // Torque&Turn parameter
    COMP_BFALSE_R(ReadTorqCfgPara(theApp.m_tShowCfg[g_tGlbCfg.nLangType].nAlias, &theApp.m_tParaCfg), FALSE);
    initstep++;

    COMP_BFALSE_R(LoadTubingInfo(), FALSE);
    COMP_BFALSE_R(LoadTubingCfg(), FALSE);
    initstep++;

    COMP_BFALSE_R(ReadValvePara(&theApp.m_tValveCfg), FALSE);
    initstep++;
    /* 系统参数路径，写死，不存储 */
    //CheckParaFileName();
    return TRUE;
}

BOOL CDBAccess::ReadGlobalPara()
{
    COMP_BFALSE_R(m_bValidDBFile, FALSE);

    g_tGlbCfg.nLangType = m_tDBGlbCfg._LangType;
    if (g_tGlbCfg.nLangType >= LANGUAGE_NUM)
        g_tGlbCfg.nLangType = LANGUAGE_CHINESE;
    g_tGlbCfg.nPortNO = m_tDBGlbCfg._PortNO;
    if (g_tGlbCfg.nPortNO < 1)
        g_tGlbCfg.nPortNO = 1;
    g_tGlbCfg.nPlusPerTurn = m_tDBGlbCfg._PlusPerTurn;
    g_tGlbCfg.nTorqUnit = m_tDBGlbCfg._TorqUnit;
    if (g_tGlbCfg.nTorqUnit == 1)
    {
        g_tGlbCfg.strUnit = theApp.LoadstringFromRes(IDS_STRTORQLBFTUNIT, BIGPOINT);
    }
    else
    {
        g_tGlbCfg.strUnit = theApp.LoadstringFromRes(IDS_STRTORQNMUNIT, BIGPOINT);
    }

    g_tGlbCfg.nCollectDur = m_tDBGlbCfg._CollectDur;
    g_tGlbCfg.nResetDur = m_tDBGlbCfg._ResetDur;
    g_tGlbCfg.nSaveDur = m_tDBGlbCfg._SaveDur;
    g_tGlbCfg.nIPShowMode = m_tDBGlbCfg._IPShowMode;
    g_tGlbCfg.nZoomIn = m_tDBGlbCfg._ZoomIn;
    g_tGlbCfg.nImgNum = m_tDBGlbCfg._ImgNum;
    g_tGlbCfg.nTest = m_tDBGlbCfg._Test;

    g_tGlbCfg.fDiscount = m_tDBGlbCfg._Discount;
    g_tGlbCfg.fMulti = m_tDBGlbCfg._Multi;
    g_tGlbCfg.fRpmAdj = m_tDBGlbCfg._RpmAdj;
    g_tGlbCfg.fIPDeltaVal = m_tDBGlbCfg._IPDeltaVal;

    g_tGlbCfg.bCheckIP = m_tDBGlbCfg._CheckIP;
    g_tGlbCfg.bBigTorq = m_tDBGlbCfg._BigTorq;
    g_tGlbCfg.bShackle = m_tDBGlbCfg._Shackle;
    g_tGlbCfg.bDateBehind = m_tDBGlbCfg._DateBehind;

    g_tGlbCfg.strPassWord = m_tDBGlbCfg._Password;
    g_tGlbCfg.strDataPath = m_tDBGlbCfg._DataPath;

    return TRUE;
}

BOOL CDBAccess::UpdateGlobalPara()
{
    COMP_BFALSE_R(m_bValidDBFile, FALSE);

    CHECK_PARA_ROUND(g_tGlbCfg.nLangType, LANGUAGE_CHINESE, LANGUAGE_NUM, LANGUAGE_CHINESE);
    CHECK_PARA_ROUND(g_tGlbCfg.fDiscount, 0.5, 2.0, 1.0);
    CHECK_PARA_ROUND(g_tGlbCfg.fMulti, 0, 2, 1);
    CHECK_PARA_ROUND(g_tGlbCfg.fRpmAdj, 0, 50, 3.5);

    if (!m_tDBGlbCfg.UpdateGlbCfg(&g_tGlbCfg))
        return FALSE;
    m_tDBGlbCfg.Reload();
    return TRUE;
}


vector<string> CDBAccess::GetNamesByIndexs(string indexs)
{
    vector<string> lsNames;

    COMP_BFALSE_R(m_bValidDBFile, lsNames);
    return m_tDBShowName.GetNamesByIndexs(indexs);

}

BOOL CDBAccess::ReadShowPara(SHOWCFG* ptShow, UINT nLang)
{
    string  strCond;
    string  strContent;
    vector<string> lsName;
    vector<int>  lsNO;
    UINT    i = 0;

    ASSERT_NULL_R(ptShow, FALSE);
    COMP_BFALSE_R(m_bValidDBFile, FALSE);
    COMP_BLE_R(m_tDBShowCfg._lsParaNum.size(), 0, FALSE);

    CheckLanguage(nLang);

    theApp.ClearShowPara(ptShow);

    ptShow->nParaNum = m_tDBShowCfg._lsParaNum[nLang];
    ptShow->nListNum = m_tDBShowCfg._lsListNum[nLang];
    ptShow->nMainNum = m_tDBShowCfg._lsMainNum[nLang];
    ptShow->nFileName = m_tDBShowCfg._lsFileName[nLang];
    ptShow->nStatType = m_tDBShowCfg._lsStatType[nLang];
    ptShow->nAlias = m_tDBShowCfg._lsAliasID[nLang];

    // Show Paras Name
    strContent = m_tDBShowCfg._lsShowPara[nLang];
    theApp.SetShowNameFromID(strContent, ptShow, nLang);

    // List Paras Name
    strContent = m_tDBShowCfg._lsListNO[nLang];
    theApp.SetShowNOFromID(0, strContent, ptShow);

    // Main Paras Name
    strContent = m_tDBShowCfg._lsMainNO[nLang];
    theApp.SetShowNOFromID(1, strContent, ptShow);

    return TRUE;
}

BOOL CDBAccess::UpdateShowAlias(SHOWCFG* ptShow, UINT Alias)
{
    ASSERT_NULL_R(ptShow, FALSE);
    COMP_BFALSE_R(m_bValidDBFile, FALSE);

    if (!m_tDBShowCfg.UpdateAlias(Alias))
    {
        return FALSE;
    }
    m_tDBShowCfg.Reload();

    ptShow->nAlias = Alias;

    return TRUE;
}

BOOL CDBAccess::UpdateShowPara(SHOWCFG* ptShow)
{
    ASSERT_NULL_R(ptShow, FALSE);
    COMP_BFALSE_R(m_bValidDBFile, FALSE);

    if (!m_tDBShowCfg.UpdateShowCfg(ptShow))
        return FALSE;

    m_tDBShowCfg.Reload();

    ReadShowPara(theApp.m_ptCurShow);
    return TRUE;
}


BOOL CDBAccess::UpdateShowName(SHOWCFG* ptShow)
{
    int i = 0;
    int index = 0;

    ASSERT_NULL_R(ptShow, FALSE);
    COMP_BFALSE_R(m_bValidDBFile, FALSE);

    // update show name
    for (i = 1; i < MAXPARANUM; i++)
    {
        index = m_tDBShowName.UpdateShowName(i, ptShow->strShow[i]);
        if (index == DB_INVALID_VAL)
        {
            return FALSE;
        }
        ptShow->nShow[i] = index;
    }

    m_tDBShowName.Reload();

    return TRUE;
}

vector<string> CDBAccess::GetOptionsByIndex(UINT showIndex)
{
    vector<string> lsOptions;
    COMP_BFALSE_R(m_bValidDBFile, lsOptions);
    return m_tDBShowOpt.GetOptionsByNameIndex(showIndex);
}

BOOL CDBAccess::ReadTubeInfo(TUBECFG* ptTube, BOOL bIndex)
{
    int index = 0;
    ASSERT_NULL_R(ptTube, FALSE);
    COMP_BFALSE_R(m_bValidDBFile, FALSE);

    // by index
    if (bIndex == 0)
    {
        return m_tDBTubeCfg.QueryTubeByIndex(ptTube);
    }

    index = m_tDBTubeCfg.QueryTubeByInfo(ptTube);
    if (index > 0)
        return TRUE;

    return FALSE;
}

vector<int> CDBAccess::ReadCurShowIndex()
{
    vector<int> lsIndexs;
    COMP_BFALSE_R(m_bValidDBFile, lsIndexs);

    if (m_tDBShowCfg._lsShowPara.size() > 0)
        lsIndexs = m_tDBShowName.GetIndexsByNOs(m_tDBShowCfg._lsShowPara[g_tGlbCfg.nLangType]);

    return lsIndexs;
}

vector<string> CDBAccess::ReadOptionsByShowIndex(int index)
{
    vector<string> lsOptions;
    COMP_BFALSE_R(m_bValidDBFile, lsOptions);
    return m_tDBShowOpt.GetOptionsByNameIndex(index);
}

BOOL  CDBAccess::ReadXlsStatPara(XLSSTATCFG* ptStat, UINT nLang)
{
    ASSERT_NULL_R(ptStat, FALSE);
    COMP_BFALSE_R(m_bValidDBFile, FALSE);

    return m_tDBXlsStatCfg.GetInfoByLang(ptStat, nLang);
}

BOOL CDBAccess::UpdateXlsStatPara(XLSSTATCFG* ptStat)
{
    ASSERT_NULL_R(ptStat, FALSE);
    COMP_BFALSE_R(m_bValidDBFile, FALSE);
    if (!m_tDBXlsStatCfg.UpdateInfo(ptStat))
        return FALSE;

    m_tDBXlsStatCfg.Reload();

    return TRUE;
}

BOOL CDBAccess::ReadTorqCfgPara(int iAlias, PARACFG* ptCfg)
{
    TORQCFGID  tCfgID = { 0 };

    ASSERT_NULL_R(ptCfg, FALSE);
    COMP_BFALSE_R(m_bValidDBFile, FALSE);
    COMP_BLE_R(iAlias, 0, FALSE);

    theApp.ClearTorqCfgPara(ptCfg);

    COMP_BFALSE_R(m_tDBTorqueCfg.GetParaCfgById(iAlias, ptCfg, &tCfgID), FALSE);
    COMP_BFALSE_R(m_tDBValTorque.GetTorqValById(tCfgID.nTorqueID, &ptCfg->tCtrl.fTorqConf[0]), FALSE);
    COMP_BFALSE_R(m_tDBValTurn.GetTurnValById(tCfgID.nTurnID, &ptCfg->tCtrl.fTurnConf[0]), FALSE);
    COMP_BFALSE_R(m_tDBShowOpt.GetOptionsByNOS(tCfgID.strOptionID, ptCfg->strValue), FALSE);
    if (tCfgID.nTubeID > 0)
    {
        COMP_BFALSE_R(m_tDBTubeCfg.QueryTubeByInfo(&ptCfg->tTubeCfg), FALSE);
    }

    return TRUE;
}


int CDBAccess::ReadTorqCfgPara(string sAlias, PARACFG* ptCfg)
{
    int index = 0;
    
    ASSERT_NULL_R(ptCfg, DB_INVALID_VAL);
    COMP_BFALSE_R(m_bValidDBFile, DB_INVALID_VAL);

    index = m_tDBTorqueCfg.GetIndexByAlias(sAlias);
    COMP_BLE_R(index,0, index);

    ReadTorqCfgPara(index, ptCfg);

    return index;
}

BOOL CDBAccess::DeleteAlias(string sAlias)
{
    int index = 0;

    COMP_BFALSE_R(m_bValidDBFile, FALSE);
    
    index = m_tDBTorqueCfg.GetIndexByAlias(sAlias);
    COMP_BLE_R(index, 0, FALSE);

    COMP_BFALSE_R(m_tDBTorqueCfg.DeleteParaCfgByID(index), FALSE);
    
    m_tDBTorqueCfg.Reload();
    
    return TRUE;
}

BOOL CDBAccess::InsertTubeName(UINT TubeKind, int NO, vector<string> Names)
{
    BOOL bRes = FALSE;

    COMP_BFALSE_R(m_bValidDBFile, FALSE);
    COMP_BGE_R(TubeKind, MAXTUBECFGNUM, FALSE);

    bRes = m_tDBTubeText[TubeKind].InsertNames(NO, Names);
    COMP_BFALSE_R(bRes, FALSE);

    m_tDBTubeText[TubeKind].Reload();
    LoadTubingInfo();

    return TRUE;
}

BOOL CDBAccess::UpdateTubeName(UINT TubeKind, int NO, vector<string> Names)
{
    BOOL bRes = FALSE;

    COMP_BFALSE_R(m_bValidDBFile, FALSE);
    COMP_BGE_R(TubeKind, MAXTUBECFGNUM, FALSE);

    bRes = m_tDBTubeText[TubeKind].UpdateNames(NO, Names);
    COMP_BFALSE_R(bRes, FALSE);

    m_tDBTubeText[TubeKind].Reload();
    LoadTubingInfo();
    return TRUE;
}

BOOL CDBAccess::CheckTubeNO(UINT TubeKind, int NO)
{
    COMP_BFALSE_R(m_bValidDBFile, FALSE);
    COMP_BGE_R(TubeKind, MAXTUBECFGNUM, FALSE);

    return m_tDBTubeText[TubeKind].CheckNO(NO);
}

int CDBAccess::InsertTubeCfg(TUBECFG* ptTube)
{
    int index = DB_INVALID_VAL;
    COMP_BFALSE_R(m_bValidDBFile, DB_INVALID_VAL);
    ASSERT_NULL_R(ptTube, DB_INVALID_VAL);

    index = m_tDBTubeCfg.InsertTubeCfg(ptTube);
    COMP_BLE_R(index, 0, index);

    m_tDBTubeCfg.Reload();
    LoadTubingCfg();

    return index;
}

BOOL CDBAccess::DeleteTubeCfg(TUBECFG* ptTube)
{
    BOOL bRes = FALSE;

    ASSERT_NULL_R(ptTube, FALSE);
    COMP_BFALSE_R(m_bValidDBFile, FALSE);

    bRes = m_tDBTubeCfg.DeleteTubeCfg(ptTube);
    COMP_BFALSE_R(bRes, FALSE);
    
    m_tDBTubeCfg.Reload();
    LoadTubingCfg();

    return TRUE;
}

BOOL CDBAccess::UpdateTubeCfg(TUBECFG* ptTube)
{
    BOOL bRes = FALSE;

    ASSERT_NULL_R(ptTube, FALSE);
    COMP_BFALSE_R(m_bValidDBFile, FALSE);

    bRes = m_tDBTubeCfg.UpdateTubeCfg(ptTube);
    COMP_BFALSE_R(bRes, FALSE);

    m_tDBTubeCfg.Reload();
    LoadTubingCfg();

    return TRUE;
}

BOOL CDBAccess::GetInitTube(TUBECFG* ptTube)
{
    UINT i = 0;

    ASSERT_NULL_R(ptTube, FALSE);
    COMP_BFALSE_R(m_bValidDBFile, FALSE);

    for (i = 0; i < MAXTUBECFGNUM; i++)
    {
        ptTube->nFixTube[i] = m_tDBTubeCfg._lsFixTube[i][0];
    }
    for (i = 0; i < MAXTUBETORQNUM; i++)
    {
        ptTube->nTorqVal[i] = m_tDBTubeCfg._lsTorqVal[i][0];
    }
    return TRUE;
}

int CDBAccess::QueryIndexByInfo(TUBECFG* ptTube)
{
    ASSERT_NULL_R(ptTube, DB_INVALID_VAL);
    COMP_BFALSE_R(m_bValidDBFile, DB_INVALID_VAL);

    return m_tDBTubeCfg.QueryTubeByInfo(ptTube);
}

BOOL CDBAccess::LoadTubingInfo()
{
    int i = 0;
    int count = 0;
    int m = 0, n = 0;
    FIXTUBEPARA* ptPara = NULL;
    FIXTUBEINFO* ptFixInfo = &g_cTubing.m_tTubInfo[0];

    COMP_BFALSE_R(m_bValidDBFile, FALSE);

    // load tubbing Info data
    for (i = 0; i < MAXTUBECFGNUM; i++)
    {
        if (!m_tDBTubeText[i].Valid())
        {
            g_cTubing.GetTubParaAddr(i, &ptFixInfo[i]);
            continue;
        }

        if (ptFixInfo[i].bDbData && ptFixInfo[i].ptPara != NULL)
        {
            delete[]  ptFixInfo[i].ptPara;
            ptFixInfo[i].nNum = 0;
            ptFixInfo[i].ptPara = NULL;
        }
        count = m_tDBTubeText[i]._Count;
        ptPara = new FIXTUBEPARA[count];
        // memset string 变量，导致内存泄漏
        // memset(ptPara, 0, count * sizeof(FIXTUBEPARA));
        ptFixInfo[i].bDbData = TRUE;
        ptFixInfo[i].nNum = count;
        ptFixInfo[i].ptPara = ptPara;
        for (m = 0; m < count; m++)
        {
            for (n = 0; n <= LANGUAGE_NUM; n++)
            {
                if (n == 0)
                {
                    ptPara->nNO = m_tDBTubeText[i]._lsNO[m];
                }
                else
                {
                    //lstrcpyn(ptPara->aucName[n - 1], theApp.m_tDBTubeText[i]._lsName[n - 1][m].c_str(), MAXTUBEPARALEN);
                    ptPara->strName[n - 1] = m_tDBTubeText[i]._lsName[n - 1][m];
                }
            }
            ptPara++;
        }
    }

    return TRUE;
}

BOOL CDBAccess::LoadTubingCfg()
{
    int  i = 0;
    UINT j = 0;
    int count;
    TUBECFG* ptCfg = NULL;
    FIXTUBECFG* ptFixCfg = &g_cTubing.m_tTubCfg;

    COMP_BFALSE_R(m_bValidDBFile, FALSE);

    count = m_tDBTubeCfg._Count;
    if (!m_tDBTubeCfg.Valid() || count <= 0)
    {
        ptFixCfg->nNum = MAXDEFTUBECFGNUM;
        ptFixCfg->ptCfg = &g_tDefTubeCfg[0];
        return TRUE;
    }

    if (ptFixCfg->bDbData && ptFixCfg->ptCfg != NULL)
    {
        delete[] ptFixCfg->ptCfg;
        ptFixCfg->nNum = 0;
        ptFixCfg->ptCfg = NULL;
    }

    ptCfg = new TUBECFG[count];
    memset(ptCfg, 0, count * sizeof(TUBECFG));
    ptFixCfg->bDbData = TRUE;
    ptFixCfg->nNum = count;
    ptFixCfg->ptCfg = ptCfg;

    for (i = 0; i < count; i++)
    {
        for (j = 0; j < MAXTUBECFGNUM; j++)
        {
            ptCfg->nFixTube[j] = theDB.m_tDBTubeCfg._lsFixTube[i][j];
        }

        for (j = 0; j < MAXTUBETORQNUM; j++)
        {
            ptCfg->nTorqVal[j] = theDB.m_tDBTubeCfg._lsTorqVal[i][j];
        }

        ptCfg++;
    }

    return TRUE;
}

BOOL CDBAccess::ReadValvePara(VALVECFG* ptCfg)
{
    ASSERT_NULL_R(ptCfg, FALSE);
    COMP_BFALSE_R(m_bValidDBFile, FALSE);

    COMP_BFALSE_R(m_tDBValveCfg.GetGlbCfg(ptCfg), FALSE);

    return TRUE;
}


int CDBAccess::UpdateTorqCfgPara(PARACFG* ptCfg, SHOWCFG* ptShow)
{
    BOOL bRes = FALSE;
    TORQCFGID tCfgID;
    int AliasID = 0;

    ASSERT_NULL_R(ptCfg, DB_INVALID_VAL);
    ASSERT_NULL_R(ptShow, DB_INVALID_VAL);
    COMP_BFALSE_R(m_bValidDBFile, DB_INVALID_VAL);

    // torque config
    tCfgID.nTorqueID = m_tDBValTorque.GetIndexByInfo(&ptCfg->tCtrl);
    COMP_BLE_R(tCfgID.nTorqueID, 0, DB_INVALID_VAL);

    // turn config
    tCfgID.nTurnID = m_tDBValTurn.GetIndexByInfo(&ptCfg->tCtrl);
    COMP_BLE_R(tCfgID.nTurnID, 0, DB_INVALID_VAL);

    // fix tube config
    tCfgID.nTubeID = m_tDBTubeCfg.GetTubeIndexByInfo(&ptCfg->tTubeCfg);
    COMP_BL_R(tCfgID.nTubeID, 0, DB_INVALID_VAL);

    // show value save
    tCfgID.strOptionID = m_tDBShowOpt.GetNOsByOptions(&ptShow->nShow[0], &ptCfg->strValue[0]);

    AliasID = m_tDBTorqueCfg.UpdateParaByAlias(ptCfg, &tCfgID);
    COMP_BLE_R(AliasID, 0, DB_INVALID_VAL);

    m_tDBValTorque.Reload();
    m_tDBValTurn.Reload();
    m_tDBTubeCfg.Reload();
    m_tDBShowOpt.Reload();
    m_tDBTorqueCfg.Reload();

    ReadTorqCfgPara(theApp.m_ptCurShow->nAlias, &theApp.m_tParaCfg);

    return AliasID;
}


vector<string> CDBAccess::ReadAllAlias()
{
    size_t i = 0;
    vector<string> strAlias;

    COMP_BFALSE_R(m_bValidDBFile, strAlias);

    if (m_tDBTorqueCfg._lsAlias.size() <= 0)
        return strAlias;

    for (i = 0; i < m_tDBTorqueCfg._lsAlias.size(); i++)
    {
        if (m_tDBTorqueCfg._lsLangType[i] != g_tGlbCfg.nLangType)
            continue;
        strAlias.push_back(m_tDBTorqueCfg._lsAlias[i]);
    }

    return strAlias;
}

BOOL CDBAccess::UpdateValvePara(VALVECFG* ptCfg)
{
    ASSERT_NULL_R(ptCfg, FALSE);
    COMP_BFALSE_R(m_bValidDBFile, FALSE);

    if (!m_tDBValveCfg.UpdateGlbCfg(ptCfg))
        return FALSE;

    m_tDBValveCfg.Reload();
    ReadValvePara(&theApp.m_tValveCfg);

    return TRUE;
}
