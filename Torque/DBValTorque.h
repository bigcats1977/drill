#pragma once
#include "DBBase.h"
class CDBValTorque : public CDBBase
{
public:
    vector<int>     _lsAutoIndex;

    vector<vector<double>> _lsTorqueVal;

    CDBValTorque();
    ~CDBValTorque();


    bool GetTorqValById(int index, double* pVal);
    int  QueryIDByInfo(CONTROLPARA* ptCtrl);
    int  GetIndexByInfo(CONTROLPARA* ptCtrl);

private:
    void Empty();
    void GetTable();
};

