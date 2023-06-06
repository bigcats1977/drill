#pragma once

#include "SqliteProc.h"
#include "DBGlbCfg.h"
#include "DBShowName.h"
#include "DBShowCfg.h"
#include "DBShowOption.h"
#include "DBValTurn.h"
#include "DBTorqueCfg.h"
#include "DBValTorque.h"
#include "DBXlsStatCfg.h"
#include "DBValveCfg.h"
#include "DBServerCfg.h"

class CDBAccess
{
public:
    CDBAccess();
    ~CDBAccess();

    bool    InitDBHandle();
    bool    InitConfigFromDB(UINT& initstep);

    bool    ReadGlobalPara();
    bool    UpdateGlobalPara();

    bool    ReadServerPara(SERVERCFG* ptServer);
    bool    UpdateServerPara(SERVERCFG* ptServer);

    vector<string> GetNamesByIndexs(string indexs);

    bool    ReadShowPara(SHOWCFG* ptShow, UINT nLang = LANGUAGE_CURRENT);
    bool    UpdateShowAlias(SHOWCFG* ptShow, UINT Alias);
    bool    UpdateShowPara(SHOWCFG* ptShow);
    bool    UpdateShowName(SHOWCFG* ptShow);
    vector<string> GetOptionsByIndex(UINT showIndex);
    vector<int> ReadCurOptionIndex(int type = 0);
    vector<string> ReadOptionsByShowIndex(int index);

    bool    ReadTorqCfgPara(int iAlias, PARACFG* ptCfg);
    int     ReadTorqCfgPara(string sAlias, PARACFG* ptCfg);
    int     UpdateTorqCfgPara(PARACFG* ptCfg, SHOWCFG* ptShow);
    bool    DeleteAlias(string sAlias);
    vector<string> ReadAllAlias();

    bool    InsertTubeName(UINT TubeKind, int NO, vector<string> Names);
    bool    UpdateTubeName(UINT TubeKind, int NO, vector<string> Names);
    bool    CheckTubeNO(UINT TubeKind, int NO);

    bool    ReadValvePara(VALVECFG* ptCfg);
    bool    UpdateValvePara(VALVECFG* ptCfg);

    bool    ReadXlsStatPara(XLSSTATCFG* ptStat, UINT nLang = LANGUAGE_CURRENT);
    bool    UpdateXlsStatPara(XLSSTATCFG* ptStat);

    SqliteProc      m_tSqlite;              /* SQLite3 数据库配置文件 */
    bool            m_bValidDBFile;

private:

    CDBGlbCfg       m_tDBGlbCfg;
    CDBShowName     m_tDBShowName;
    CDBShowCfg      m_tDBShowCfg;
    CDBShowOption   m_tDBShowOpt;
    CDBValTorque    m_tDBValTorque;
    CDBValTurn      m_tDBValTurn;
    CDBTorqueCfg    m_tDBTorqueCfg;
    CDBXlsStatCfg   m_tDBXlsStatCfg;
    CDBValveCfg     m_tDBValveCfg;
    CDBServerCfg    m_tDBServCfg;
};

