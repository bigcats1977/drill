#pragma once

#include "SqliteProc.h"
#include "DBGlbCfg.h"
//#include "DBText.h"
#include "DBShowName.h"
#include "DBShowCfg.h"
#include "DBShowOption.h"
//#include "DBTubeCfg.h"
#include "DBValTurn.h"
#include "DBTorqueCfg.h"
#include "DBValTorque.h"
#include "DBXlsStatCfg.h"
#include "DBValveCfg.h"

class CDBAccess
{
public:
    CDBAccess();
    ~CDBAccess();

    bool    InitDBHandle();
    bool    InitConfigFromDB(UINT& initstep);

    bool    ReadGlobalPara();
    bool    UpdateGlobalPara();

    vector<string> GetNamesByIndexs(string indexs);

    bool    ReadShowPara(SHOWCFG* ptShow, UINT nLang = LANGUAGE_CURRENT);
    bool    UpdateShowAlias(SHOWCFG* ptShow, UINT Alias);
    bool    UpdateShowPara(SHOWCFG* ptShow);
    bool    UpdateShowName(SHOWCFG* ptShow);
    vector<string> GetOptionsByIndex(UINT showIndex);
    //bool    ReadTubeInfo(TUBECFG* ptTube, bool bIndex = true);
    vector<int> ReadCurShowIndex();
    vector<string> ReadOptionsByShowIndex(int index);

    bool    ReadTorqCfgPara(int iAlias, PARACFG* ptCfg);
    int     ReadTorqCfgPara(string sAlias, PARACFG* ptCfg);
    int     UpdateTorqCfgPara(PARACFG* ptCfg, SHOWCFG* ptShow);
    bool    DeleteAlias(string sAlias);
    vector<string> ReadAllAlias();

    bool    InsertTubeName(UINT TubeKind, int NO, vector<string> Names);
    bool    UpdateTubeName(UINT TubeKind, int NO, vector<string> Names);
    bool    CheckTubeNO(UINT TubeKind, int NO);
    //int     InsertTubeCfg(TUBECFG* ptTube);
    //bool    DeleteTubeCfg(TUBECFG* ptTube);
    //bool    UpdateTubeCfg(TUBECFG* ptTube);
    //bool    GetInitTube(TUBECFG* ptTube);
    //int     QueryIndexByInfo(TUBECFG* ptTube);

    bool    ReadValvePara(VALVECFG* ptCfg);
    bool    UpdateValvePara(VALVECFG* ptCfg);

    bool    ReadXlsStatPara(XLSSTATCFG* ptStat, UINT nLang = LANGUAGE_CURRENT);
    bool    UpdateXlsStatPara(XLSSTATCFG* ptStat);

    SqliteProc      m_tSqlite;              /* SQLite3 数据库配置文件 */
    bool            m_bValidDBFile;

private:
    /*bool    LoadTubingInfo();
    bool    LoadTubingCfg();*/
    
    CDBGlbCfg       m_tDBGlbCfg;
    CDBShowName     m_tDBShowName;
    CDBShowCfg      m_tDBShowCfg;
    CDBShowOption   m_tDBShowOpt;
    //CDBText         m_tDBTubeText[MAXTUBECFGNUM];
    //CDBTubeCfg      m_tDBTubeCfg;
    CDBValTorque    m_tDBValTorque;
    CDBValTurn      m_tDBValTurn;
    CDBTorqueCfg    m_tDBTorqueCfg;
    CDBXlsStatCfg   m_tDBXlsStatCfg;
    CDBValveCfg     m_tDBValveCfg;
};

