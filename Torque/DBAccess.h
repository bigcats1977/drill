#pragma once

#include "SqliteProc.h"
#include "DBGlbCfg.h"
#include "DBText.h"
#include "DBShowName.h"
#include "DBShowCfg.h"
#include "DBShowOption.h"
#include "DBTubeCfg.h"
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

    BOOL    InitDBHandle();
    BOOL    InitConfigFromDB(int& initstep);

    BOOL    ReadGlobalPara();
    BOOL    UpdateGlobalPara();

    vector<string> GetNamesByIndexs(string indexs);

    BOOL    ReadShowPara(SHOWCFG* ptShow, UINT nLang = LANGUAGE_CURRENT);
    BOOL    UpdateShowAlias(SHOWCFG* ptShow, UINT Alias);
    BOOL    UpdateShowPara(SHOWCFG* ptShow);
    BOOL    UpdateShowName(SHOWCFG* ptShow);
    vector<string> GetOptionsByIndex(UINT showIndex);
    BOOL    ReadTubeInfo(TUBECFG* ptTube, BOOL bIndex = TRUE);
    vector<int> ReadCurShowIndex();
    vector<string> ReadOptionsByShowIndex(int index);

    BOOL    ReadTorqCfgPara(int iAlias, PARACFG* ptCfg);
    int     ReadTorqCfgPara(string sAlias, PARACFG* ptCfg);
    int     UpdateTorqCfgPara(PARACFG* ptCfg, SHOWCFG* ptShow);
    BOOL    DeleteAlias(string sAlias);
    vector<string> ReadAllAlias();

    BOOL    InsertTubeName(UINT TubeKind, int NO, vector<string> Names);
    BOOL    UpdateTubeName(UINT TubeKind, int NO, vector<string> Names);
    BOOL    CheckTubeNO(UINT TubeKind, int NO);
    int     InsertTubeCfg(TUBECFG* ptTube);
    BOOL    DeleteTubeCfg(TUBECFG* ptTube);
    BOOL    UpdateTubeCfg(TUBECFG* ptTube);
    BOOL    GetInitTube(TUBECFG* ptTube);
    int     QueryIndexByInfo(TUBECFG* ptTube);

    BOOL    ReadValvePara(VALVECFG* ptCfg);
    BOOL    UpdateValvePara(VALVECFG* ptCfg);

    BOOL    ReadXlsStatPara(XLSSTATCFG* ptStat, UINT nLang = LANGUAGE_CURRENT);
    BOOL    UpdateXlsStatPara(XLSSTATCFG* ptStat);

    SqliteProc      m_tSqlite;              /* SQLite3 数据库配置文件 */
    BOOL            m_bValidDBFile;

private:
    BOOL    LoadTubingInfo();
    BOOL    LoadTubingCfg();
    
    CDBGlbCfg       m_tDBGlbCfg;
    CDBShowName     m_tDBShowName;
    CDBShowCfg      m_tDBShowCfg;
    CDBShowOption   m_tDBShowOpt;
    CDBText         m_tDBTubeText[MAXTUBECFGNUM];
    CDBTubeCfg      m_tDBTubeCfg;
    CDBValTorque    m_tDBValTorque;
    CDBValTurn      m_tDBValTurn;
    CDBTorqueCfg    m_tDBTorqueCfg;
    CDBXlsStatCfg   m_tDBXlsStatCfg;
    CDBValveCfg     m_tDBValveCfg;
};

