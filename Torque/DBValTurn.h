#pragma once
#include "DBBase.h"
class CDBValTurn : public CDBBase
{
public:
    vector<int>     _lsAutoIndex;

    vector<vector<double>> _lsTurnVal;

    CDBValTurn();
    ~CDBValTurn();

    BOOL GetTurnValById(int index, double* ptal);
    int  QueryIDByInfo(CONTROLPARA* ptCtrl);
    int  GetIndexByInfo(CONTROLPARA* ptCtrl);

private:
    void Empty();
    void GetTable();
};

