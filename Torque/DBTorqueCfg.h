#pragma once
#include "DBBase.h"

class CDBTorqueCfg :  public CDBBase
{
public:
    vector<int>     _lsAutoIndex;
    vector<string>  _lsAlias;
    vector<int>     _lsLangType;
    vector<int>     _lsTorqueID;
    vector<int>     _lsTurnID;
    vector<double>  _lsFullRPM;
    //vector<double>  _lsMinShlSlope;

    vector<string>  _lsOptionID;
    vector<string>  _lsComment;

    CDBTorqueCfg();
    ~CDBTorqueCfg();


    bool    GetParaCfgById(int index, PARACFG* ptCfg, TORQCFGID* ptID);
    int     UpdateParaByAlias(PARACFG* ptCfg, TORQCFGID* ptID);
    int     GetIndexByAlias(string alias);
    int     GetParaCfgByAlias(string alias, PARACFG* ptCfg, TORQCFGID* ptID);
    bool    DeleteParaCfgByID(int Index);

private:
    void Empty();
    void GetTable();
};

