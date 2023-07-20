#include "Global.h"
#include "Drill.h"
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
    string  strInfo;

    try {
        m_tDBGlbCfg = CDBGlbCfg();
        m_tDBShowName = CDBShowName();
        m_tDBShowCfg = CDBShowCfg();
        m_tDBShowOpt = CDBShowOption();

        m_tDBValTorque = CDBValTorque();
        m_tDBValTurn = CDBValTurn();
        m_tDBTorqueCfg = CDBTorqueCfg();
        m_tDBXlsStatCfg = CDBXlsStatCfg();
        m_tDBValveCfg = CDBValveCfg();
        m_tDBServCfg = CDBServerCfg();
        m_tDBWITSCfg = CDBWITSCfg();
    }
    catch (exception& e)
    {
        strInfo = string_format("%s fail(%s)!!", __FUNCTION__, e.what());
        theApp.SaveMessage(strInfo);
        return false;
    }

    return true;
}

bool CDBAccess::InitConfigFromDB(UINT& initstep)
{
    UINT i = 0;
    string strDbFile;
    string strPW;

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

    if (!InitDBHandle())
    {
        theApp.SaveMessage("InitDBHandle fail!!");
        return false;
    }

    initstep = DB_INIT_SUCCESS;
    // global parameter
    if (!ReadGlobalPara())
    {
        initstep |= DB_INIT_GLOBAL;
        theApp.SaveMessage("ReadGlobalPara fail!!");
        //return false;
    }

    for (i = 0; i < LANGUAGE_NUM; i++)
    {
        // Show parameter
        if (!ReadShowPara(&theApp.m_tShowCfg[i], i))
        {
            initstep |= DB_INIT_SHOW;
            theApp.SaveMessage("ReadShowPara fail!!");
            //return false;
        }

        // excel statastic config
        if (!ReadXlsStatPara(&theApp.m_tXlsStatCfg[i], i))
        {
            initstep |= DB_INIT_XLS_STAT;
            theApp.SaveMessage("ReadXlsStatPara fail!!");
            // return false;
        }
    }

    // Torque&Turn parameter
    if (!ReadTorqCfgPara(theApp.m_tShowCfg[g_tGlbCfg.nLangType].nAlias, &theApp.m_tParaCfg))
    {
        initstep |= DB_INIT_TORQUE_CFG;
        theApp.SaveMessage("ReadTorqCfgPara fail!!");
        // return false;
    }

    if (!ReadValvePara(&theApp.m_tValveCfg))
    {
        initstep |= DB_INIT_VALVE_CFG;
        theApp.SaveMessage("ReadValvePara fail!!");
        //return false;
    }

    if (!ReadServerCfg(&theApp.m_tServCfg))
    {
        initstep |= DB_INIT_SERVER_CFG;
        theApp.SaveMessage("ReadServerCfg fail!!");
    }

    if (!ReadWITSCfg(&theApp.m_tWITSCfg))
    {
        initstep |= DB_INIT_WITS_CFG;
        theApp.SaveMessage("ReadWITSCfg fail!!");
    }

    if (initstep != DB_INIT_SUCCESS)
        return false;

    /* 系统参数路径，写死，不存储 */
    //CheckParaFileName();
    return true;
}

bool CDBAccess::ReadGlobalPara()
{
    COMP_BFALSE_R(m_tDBGlbCfg.Valid(), false);

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
        //g_tGlbCfg.strUnit = theApp.LoadstringFromRes(IDS_STRTORQLBFTUNIT, BIGPOINT);
        g_tGlbCfg.strUnit = string_format(theApp.LoadstringFromRes(IDS_STRTORQLBFTUNIT).c_str(), BIGPOINT);
    }
    else
    {
        //g_tGlbCfg.strUnit = theApp.LoadstringFromRes(IDS_STRTORQNMUNIT, BIGPOINT);
        g_tGlbCfg.strUnit = string_format(theApp.LoadstringFromRes(IDS_STRTORQNMUNIT).c_str(), BIGPOINT);
    }

    g_tGlbCfg.nCollectDur = m_tDBGlbCfg._CollectDur;
    g_tGlbCfg.nResetDur = m_tDBGlbCfg._ResetDur;
    g_tGlbCfg.nSaveDur = m_tDBGlbCfg._SaveDur;
    //g_tGlbCfg.nIPShowMode = m_tDBGlbCfg._IPShowMode;
    g_tGlbCfg.nZoomIn = m_tDBGlbCfg._ZoomIn;
    g_tGlbCfg.nImgNum = m_tDBGlbCfg._ImgNum;
    //g_tGlbCfg.nTest = m_tDBGlbCfg._Test;

    g_tGlbCfg.fDiscount = m_tDBGlbCfg._Discount;
    g_tGlbCfg.fMulti = m_tDBGlbCfg._Multi;
    g_tGlbCfg.fRpmAdj = m_tDBGlbCfg._RpmAdj;
    //g_tGlbCfg.fIPDeltaVal = m_tDBGlbCfg._IPDeltaVal;

    //g_tGlbCfg.bCheckIP = m_tDBGlbCfg._CheckIP;
    g_tGlbCfg.bBigTorq = m_tDBGlbCfg._BigTorq;
    g_tGlbCfg.bDateBehind = m_tDBGlbCfg._DateBehind;

    g_tGlbCfg.strPassWord = m_tDBGlbCfg._Password;
    g_tGlbCfg.strDataPath = m_tDBGlbCfg._DataPath;

    return true;
}

bool CDBAccess::UpdateGlobalPara()
{
    COMP_BFALSE_R(m_tDBGlbCfg.Valid(), false);

    CHECK_PARA_ROUND(g_tGlbCfg.nLangType, LANGUAGE_CHINESE, LANGUAGE_NUM, LANGUAGE_CHINESE);
    CHECK_PARA_ROUND(g_tGlbCfg.fDiscount, 0.5, 2.0, 1.0);
    CHECK_PARA_ROUND(g_tGlbCfg.fMulti, 0, 2, 1);
    CHECK_PARA_ROUND(g_tGlbCfg.fRpmAdj, 0, 50, 3.5);

    if (!m_tDBGlbCfg.UpdateGlbCfg(&g_tGlbCfg))
        return false;
    m_tDBGlbCfg.Reload();
    return true;
}

bool CDBAccess::ReadServerCfg(SERVERCFG* ptServer)
{
    ASSERT_NULL_R(ptServer, false);
    COMP_BFALSE_R(m_tDBServCfg.Valid(), false);

    ptServer->strFTPAddr = m_tDBServCfg._FTPAddr;
    ptServer->nFTPPort = m_tDBServCfg._FTPPort;
    ptServer->strUserName = m_tDBServCfg._UserName;
    ptServer->strPassword = m_tDBServCfg._Password;
    ptServer->strTargetPath = m_tDBServCfg._TargetPath;
    ptServer->nTCPPort = m_tDBServCfg._TCPPort;

    return true;
}

bool CDBAccess::UpdateServerCfg(SERVERCFG* ptServer)
{
    ASSERT_NULL_R(ptServer, false);
    COMP_BFALSE_R(m_tDBServCfg.Valid(), false);

    if (!m_tDBServCfg.UpdateServerCfg(ptServer))
        return false;
    m_tDBServCfg.Reload();
    return true;
}

bool CDBAccess::ReadWITSCfg(WITSCFG* ptWITSCfg)
{
    ASSERT_NULL_R(ptWITSCfg, false);
    COMP_BFALSE_R(m_tDBWITSCfg.Valid(), false);

    COMP_BNE_R(m_tDBWITSCfg._FixItems.size(), WITSRPT_FIXHEADNUM, false);
    COMP_BNE_R(m_tDBWITSCfg._RepeatItems.size(), WITSRPT_REPEATNUM, false);
    COMP_BNE_R(m_tDBWITSCfg._CalItems.size(), WITSRPT_CALPARANUM, false);
    COMP_BNE_R(m_tDBWITSCfg._ShowParas.size(), m_tDBWITSCfg._ShowItems.size(), false);

    ptWITSCfg->ShowParas = m_tDBWITSCfg._ShowParas;
    ptWITSCfg->FixItems = m_tDBWITSCfg._FixItems;
    ptWITSCfg->RepeatItems = m_tDBWITSCfg._RepeatItems;
    ptWITSCfg->CalItems = m_tDBWITSCfg._CalItems;
    ptWITSCfg->ShowItems = m_tDBWITSCfg._ShowItems;

    return true;
}

bool CDBAccess::UpdateWITSCfg(WITSCFG* ptWITSCfg)
{
    ASSERT_NULL_R(ptWITSCfg, false);
    COMP_BFALSE_R(m_tDBWITSCfg.Valid(), false);

    if (!m_tDBWITSCfg.UpdateWITSCfg(ptWITSCfg))
        return false;
    m_tDBWITSCfg.Reload();
    return true;
}

vector<string> CDBAccess::GetNamesByIndexs(string indexs)
{
    vector<string> lsNames;

    COMP_BFALSE_R(m_tDBShowName.Valid(), lsNames);
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
    COMP_BFALSE_R(m_tDBShowCfg.Valid(), false);
    COMP_BLE_R(m_tDBShowCfg._lsParaNum.size(), 0, false);

    CheckLanguage(nLang);

    theApp.ClearShowPara(ptShow);

    ptShow->nParaNum = m_tDBShowCfg._lsParaNum[nLang];
    //ptShow->nListNum = m_tDBShowCfg._lsListNum[nLang];
    ptShow->nMainNum = m_tDBShowCfg._lsMainNum[nLang];
    ptShow->nFileName = m_tDBShowCfg._lsFileName[nLang];
    ptShow->nStatType = m_tDBShowCfg._lsStatType[nLang];
    ptShow->nJointOD = m_tDBShowCfg._lsJointOD[nLang];
    ptShow->nAlias = m_tDBShowCfg._lsAliasID[nLang];

    // Show Paras Name
    strContent = m_tDBShowCfg._lsShowPara[nLang];
    theApp.SetShowNameFromID(strContent, ptShow, nLang);

    // List Paras Name
    /*strContent = m_tDBShowCfg._lsListNO[nLang];
    theApp.SetShowNOFromID(0, strContent, ptShow);*/

    // Main Paras Name
    strContent = m_tDBShowCfg._lsMainNO[nLang];
    theApp.SetShowNOFromID(1, strContent, ptShow);

    return true;
}

bool CDBAccess::UpdateShowAlias(SHOWCFG* ptShow, UINT Alias)
{
    ASSERT_NULL_R(ptShow, false);
    COMP_BFALSE_R(m_tDBShowCfg.Valid(), false);

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
    COMP_BFALSE_R(m_tDBShowCfg.Valid(), false);

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
    COMP_BFALSE_R(m_tDBShowName.Valid(), false);

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
    COMP_BFALSE_R(m_tDBShowOpt.Valid(), lsOptions);
    return m_tDBShowOpt.GetOptionsByNameIndex(showIndex);
}

vector<int> CDBAccess::ReadCurOptionIndex(int type)
{
    size_t i = 0;
    vector<int> lsIndexs;
    vector<int> lsOpt;
    COMP_BFALSE_R(m_tDBShowCfg.Valid(), lsIndexs);

    lsOpt = GetIDFromList(m_tDBShowCfg._lsShowPara[g_tGlbCfg.nLangType]);
    if (lsOpt.size() <= 0)
        return lsOpt;

    switch (type) {
    case 0: //showparas
        return lsOpt;
        break;

    //case 1: //listno
    //    lsIndexs = GetIDFromList(m_tDBShowCfg._lsListNO[g_tGlbCfg.nLangType]);
    //    break;

    case 1: //mainno
        lsIndexs = GetIDFromList(m_tDBShowCfg._lsMainNO[g_tGlbCfg.nLangType]);
        break;
    }
    for (i = 0; i < lsIndexs.size(); i++) {
        lsIndexs[i] = lsOpt[lsIndexs[i]];
    }

    return lsIndexs;
}

vector<int> CDBAccess::ReadCurShowIndex()
{
    vector<int> lsIndexs;
    COMP_BFALSE_R(m_tDBShowCfg.Valid(), lsIndexs);

    if (m_tDBShowCfg._lsShowPara.size() > 0)
        lsIndexs = GetIDFromList(m_tDBShowCfg._lsShowPara[g_tGlbCfg.nLangType]);
    //lsIndexs = m_tDBShowName.GetIndexsByNOs(m_tDBShowCfg._lsShowPara[g_tGlbCfg.nLangType]);

    return lsIndexs;
}

vector<string> CDBAccess::ReadOptionsByShowIndex(int index)
{
    vector<string> lsOptions;
    COMP_BFALSE_R(m_tDBShowOpt.Valid(), lsOptions);
    return m_tDBShowOpt.GetOptionsByNameIndex(index);
}

bool  CDBAccess::ReadXlsStatPara(XLSSTATCFG* ptStat, UINT nLang)
{
    ASSERT_NULL_R(ptStat, false);
    COMP_BFALSE_R(m_tDBXlsStatCfg.Valid(), false);

    return m_tDBXlsStatCfg.GetInfoByLang(ptStat, nLang);
}

bool CDBAccess::UpdateXlsStatPara(XLSSTATCFG* ptStat)
{
    ASSERT_NULL_R(ptStat, false);
    COMP_BFALSE_R(m_tDBXlsStatCfg.Valid(), false);
    if (!m_tDBXlsStatCfg.UpdateInfo(ptStat))
        return false;

    m_tDBXlsStatCfg.Reload();

    return true;
}

bool CDBAccess::ReadTorqCfgPara(int iAlias, PARACFG* ptCfg)
{
    TORQCFGID  tCfgID = { 0 };

    ASSERT_NULL_R(ptCfg, false);
    COMP_BFALSE_R(m_tDBTorqueCfg.Valid(), false);
    COMP_BFALSE_R(m_tDBValTorque.Valid(), false);
    COMP_BFALSE_R(m_tDBValTurn.Valid(), false);
    COMP_BFALSE_R(m_tDBShowOpt.Valid(), false);
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
    COMP_BFALSE_R(m_tDBTorqueCfg.Valid(), DB_INVALID_VAL);

    index = m_tDBTorqueCfg.GetIndexByAlias(sAlias);
    COMP_BLE_R(index, 0, index);

    ReadTorqCfgPara(index, ptCfg);

    return index;
}

bool CDBAccess::DeleteAlias(string sAlias)
{
    int index = 0;

    COMP_BFALSE_R(m_tDBTorqueCfg.Valid(), false);

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
    COMP_BFALSE_R(m_tDBValTorque.Valid(), DB_INVALID_VAL);
    COMP_BFALSE_R(m_tDBValTurn.Valid(), DB_INVALID_VAL);
    COMP_BFALSE_R(m_tDBShowOpt.Valid(), DB_INVALID_VAL);
    COMP_BFALSE_R(m_tDBTorqueCfg.Valid(), DB_INVALID_VAL);

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

    COMP_BFALSE_R(m_tDBTorqueCfg.Valid(), strAlias);

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
    COMP_BFALSE_R(m_tDBValveCfg.Valid(), false);

    COMP_BFALSE_R(m_tDBValveCfg.GetGlbCfg(ptCfg), false);

    return true;
}

bool CDBAccess::UpdateValvePara(VALVECFG* ptCfg)
{
    ASSERT_NULL_R(ptCfg, false);
    COMP_BFALSE_R(m_tDBValveCfg.Valid(), false);

    if (!m_tDBValveCfg.UpdateGlbCfg(ptCfg))
        return false;

    m_tDBValveCfg.Reload();
    ReadValvePara(&theApp.m_tValveCfg);

    return true;
}
