#include "Global.h"
#include "Torque.h"
//#include "TubeCfg.h"
#include "DBAccess.h"

/*********************全局变量************************************/
//CTubeCfg        g_cTubing;

CDBAccess::CDBAccess()
{
}

CDBAccess::~CDBAccess()
{
}

bool CDBAccess::InitDBHandle()
{
    COMP_BFALSE_R(m_bValidDBFile, false);

    m_tDBGlbCfg = CDBGlbCfg();
    m_tDBShowName = CDBShowName();
    m_tDBShowCfg = CDBShowCfg();
    m_tDBShowOpt = CDBShowOption();

    m_tDBValTorque = CDBValTorque();
    m_tDBValTurn = CDBValTurn();
    m_tDBTorqueCfg = CDBTorqueCfg();
    m_tDBXlsStatCfg = CDBXlsStatCfg();
    m_tDBValveCfg = CDBValveCfg();

    return true;
}

bool CDBAccess::InitConfigFromDB(UINT &initstep)
{
    UINT i = 0;
    string strDbFile;
    string strPW;

    m_bValidDBFile = false;

    strDbFile = theApp.m_strAppPath + SQLITEFILE;
    strPW = theApp.LoadstringFromRes(IDS_STRDBPASSWORD);
    theApp.SaveMessage(strDbFile.c_str());
    //theApp.SaveMessage(strPW.c_str());
    // NO DB file, using default value
    if (!m_tSqlite.OpenDB(strDbFile, strPW))
    {
        theApp.SaveMessage("OpenDB fail!!");
        return false;
    }

    m_bValidDBFile = true;
    if (!InitDBHandle())
    {
        theApp.SaveMessage("InitDBHandle fail!!");
        return false;
    }

    initstep = 0;
    // global parameter
    if (!ReadGlobalPara())
    {
        initstep |= 1;
        theApp.SaveMessage("ReadGlobalPara fail!!");
        //return false;
    }

    for (i = 0; i < LANGUAGE_NUM; i++)
    {
        // Show parameter
        if (!ReadShowPara(&theApp.m_tShowCfg[i], i))
        {
            initstep |= 2;
            theApp.SaveMessage("ReadShowPara fail!!");
            //return false;
        }

        // excel statastic config
        if (!ReadXlsStatPara(&theApp.m_tXlsStatCfg[i], i))
        {
            initstep |= 4;
            theApp.SaveMessage("ReadXlsStatPara fail!!");
            // return false;
        }
    }

    // Torque&Turn parameter
    if (!ReadTorqCfgPara(theApp.m_tShowCfg[g_tGlbCfg.nLangType].nAlias, &theApp.m_tParaCfg))
    {
        initstep |= 8;
        theApp.SaveMessage("ReadTorqCfgPara fail!!");
        // return false;
    }

    if (!ReadValvePara(&theApp.m_tValveCfg))
    {
        initstep |= 16;
        theApp.SaveMessage("ReadValvePara fail!!");
        //return false;
    }
    if (initstep != 0)
        return false;

    /* 系统参数路径，写死，不存储 */
    //CheckParaFileName();
    return true;
}

bool CDBAccess::ReadGlobalPara()
{
    COMP_BFALSE_R(m_bValidDBFile, false);

    g_tGlbCfg.nLangType = m_tDBGlbCfg._LangType;
    if (g_tGlbCfg.nLangType >= LANGUAGE_NUM)
        g_tGlbCfg.nLangType = LANGUAGE_CHINESE;
    g_tGlbCfg.nPortNO = m_tDBGlbCfg._PortNO;
    if (g_tGlbCfg.nPortNO < 1)
        g_tGlbCfg.nPortNO = 1;
    g_tGlbCfg.nBaudRate = m_tDBGlbCfg._BaudRate;
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

    return true;
}

bool CDBAccess::UpdateGlobalPara()
{
    COMP_BFALSE_R(m_bValidDBFile, false);

    CHECK_PARA_ROUND(g_tGlbCfg.nLangType, LANGUAGE_CHINESE, LANGUAGE_NUM, LANGUAGE_CHINESE);
    CHECK_PARA_ROUND(g_tGlbCfg.fDiscount, 0.5, 2.0, 1.0);
    CHECK_PARA_ROUND(g_tGlbCfg.fMulti, 0, 2, 1);
    CHECK_PARA_ROUND(g_tGlbCfg.fRpmAdj, 0, 50, 3.5);

    if (!m_tDBGlbCfg.UpdateGlbCfg(&g_tGlbCfg))
        return false;
    m_tDBGlbCfg.Reload();
    return true;
}

vector<string> CDBAccess::GetNamesByIndexs(string indexs)
{
    vector<string> lsNames;

    COMP_BFALSE_R(m_bValidDBFile, lsNames);
    return m_tDBShowName.GetNamesByIndexs(indexs);

}

bool CDBAccess::ReadShowPara(SHOWCFG* ptShow, UINT nLang)
{
    string  strCond;
    string  strContent;
    vector<string> lsName;
    vector<int>  lsNO;
    UINT    i = 0;

    ASSERT_NULL_R(ptShow, false);
    COMP_BFALSE_R(m_bValidDBFile, false);
    COMP_BLE_R(m_tDBShowCfg._lsParaNum.size(), 0, false);

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

    return true;
}

bool CDBAccess::UpdateShowAlias(SHOWCFG* ptShow, UINT Alias)
{
    ASSERT_NULL_R(ptShow, false);
    COMP_BFALSE_R(m_bValidDBFile, false);

    if (!m_tDBShowCfg.UpdateAlias(Alias))
    {
        return false;
    }
    m_tDBShowCfg.Reload();

    ptShow->nAlias = Alias;

    return true;
}

bool CDBAccess::UpdateShowPara(SHOWCFG* ptShow)
{
    ASSERT_NULL_R(ptShow, false);
    COMP_BFALSE_R(m_bValidDBFile, false);

    if (!m_tDBShowCfg.UpdateShowCfg(ptShow))
        return false;

    m_tDBShowCfg.Reload();

    ReadShowPara(theApp.m_ptCurShow);
    return true;
}

bool CDBAccess::UpdateShowName(SHOWCFG* ptShow)
{
    int i = 0;
    int index = 0;

    ASSERT_NULL_R(ptShow, false);
    COMP_BFALSE_R(m_bValidDBFile, false);

    // update show name
    for (i = 0; i < MAXPARANUM; i++)
    {
        index = m_tDBShowName.UpdateShowName(i, ptShow->strShow[i]);
        if (index == DB_INVALID_VAL)
        {
            return false;
        }
        ptShow->nShow[i] = index;
    }

    m_tDBShowName.Reload();

    return true;
}

vector<string> CDBAccess::GetOptionsByIndex(UINT showIndex)
{
    vector<string> lsOptions;
    COMP_BFALSE_R(m_bValidDBFile, lsOptions);
    return m_tDBShowOpt.GetOptionsByNameIndex(showIndex);
}

vector<int> CDBAccess::ReadCurShowIndex()
{
    vector<int> lsIndexs;
    COMP_BFALSE_R(m_bValidDBFile, lsIndexs);

    if (m_tDBShowCfg._lsShowPara.size() > 0)
        lsIndexs = GetIDFromList(m_tDBShowCfg._lsShowPara[g_tGlbCfg.nLangType]);

    return lsIndexs;
}

vector<string> CDBAccess::ReadOptionsByShowIndex(int index)
{
    vector<string> lsOptions;
    COMP_BFALSE_R(m_bValidDBFile, lsOptions);
    return m_tDBShowOpt.GetOptionsByNameIndex(index);
}

bool  CDBAccess::ReadXlsStatPara(XLSSTATCFG* ptStat, UINT nLang)
{
    ASSERT_NULL_R(ptStat, false);
    COMP_BFALSE_R(m_bValidDBFile, false);

    return m_tDBXlsStatCfg.GetInfoByLang(ptStat, nLang);
}

bool CDBAccess::UpdateXlsStatPara(XLSSTATCFG* ptStat)
{
    ASSERT_NULL_R(ptStat, false);
    COMP_BFALSE_R(m_bValidDBFile, false);
    if (!m_tDBXlsStatCfg.UpdateInfo(ptStat))
        return false;

    m_tDBXlsStatCfg.Reload();

    return true;
}

bool CDBAccess::ReadTorqCfgPara(int iAlias, PARACFG* ptCfg)
{
    TORQCFGID  tCfgID = { 0 };

    ASSERT_NULL_R(ptCfg, false);
    COMP_BFALSE_R(m_bValidDBFile, false);
    COMP_BLE_R(iAlias, 0, false);

    theApp.ClearTorqCfgPara(ptCfg);

    COMP_BFALSE_R(m_tDBTorqueCfg.GetParaCfgById(iAlias, ptCfg, &tCfgID), false);
    COMP_BFALSE_R(m_tDBValTorque.GetTorqValById(tCfgID.nTorqueID, &ptCfg->tCtrl.fTorqConf[0]), false);
    COMP_BFALSE_R(m_tDBValTurn.GetTurnValById(tCfgID.nTurnID, &ptCfg->tCtrl.fTurnConf[0]), false);
    COMP_BFALSE_R(m_tDBShowOpt.GetOptionsByNOS(tCfgID.strOptionID, ptCfg->strValue), false);

    return true;
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

bool CDBAccess::DeleteAlias(string sAlias)
{
    int index = 0;

    COMP_BFALSE_R(m_bValidDBFile, false);
    
    index = m_tDBTorqueCfg.GetIndexByAlias(sAlias);
    COMP_BLE_R(index, 0, false);

    COMP_BFALSE_R(m_tDBTorqueCfg.DeleteParaCfgByID(index), false);
    
    m_tDBTorqueCfg.Reload();
    
    return true;
}

int CDBAccess::UpdateTorqCfgPara(PARACFG* ptCfg, SHOWCFG* ptShow)
{
    bool bRes = false;
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

    // show value save
    tCfgID.strOptionID = m_tDBShowOpt.GetNOsByOptions(&ptShow->nShow[0], &ptCfg->strValue[0]);

    AliasID = m_tDBTorqueCfg.UpdateParaByAlias(ptCfg, &tCfgID);
    COMP_BLE_R(AliasID, 0, DB_INVALID_VAL);

    m_tDBValTorque.Reload();
    m_tDBValTurn.Reload();
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

bool CDBAccess::ReadValvePara(VALVECFG* ptCfg)
{
    ASSERT_NULL_R(ptCfg, false);
    COMP_BFALSE_R(m_bValidDBFile, false);

    COMP_BFALSE_R(m_tDBValveCfg.GetGlbCfg(ptCfg), false);

    return true;
}

bool CDBAccess::UpdateValvePara(VALVECFG* ptCfg)
{
    ASSERT_NULL_R(ptCfg, false);
    COMP_BFALSE_R(m_bValidDBFile, false);

    if (!m_tDBValveCfg.UpdateGlbCfg(ptCfg))
        return false;

    m_tDBValveCfg.Reload();
    ReadValvePara(&theApp.m_tValveCfg);

    return true;
}
