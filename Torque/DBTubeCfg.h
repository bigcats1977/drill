#pragma once
#include "DBBase.h"
class CDBTubeCfg : public CDBBase
{
public:
    vector<int> _lsAutoIndex;

    vector<vector<int>> _lsFixTube;
    /*vector<int> _lsFactory;
    vector<int> _lsOEM;
    vector<int> _lsSize;
    vector<int> _lsMaterial;
    vector<int> _lsCoupling;*/

    vector<vector<int>> _lsTorqVal;
    /*vector<int> _lsOptTorq;
    vector<int> _lsMinTorq;
    vector<int> _lsMaxTorqLB;
    vector<int> _lsOptTorqLB;
    vector<int> _lsMinTorqLB;*/

    CDBTubeCfg();
    ~CDBTubeCfg();

    BOOL    QueryTubeByIndex(TUBECFG* ptTube);
    int     QueryTubeByInfo(TUBECFG* ptTube);
    int     InsertTubeCfg(TUBECFG* ptTube);
    BOOL    DeleteTubeCfg(TUBECFG* ptTube);
    BOOL    UpdateTubeCfg(TUBECFG* ptTube);
    int     GetTubeIndexByInfo(TUBECFG* ptTube);

private:
    void Empty();
    void GetTable();
    BOOL CheckTubeValue(TUBECFG* ptTube);
};

