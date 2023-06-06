#pragma once
#include "DBBase.h"

class CDBValveCfg : public CDBBase
{
public:
    string    _V1Torq;
    string    _V1Press;
    string    _V2Torq;
    string    _V2Flow;

    CDBValveCfg();
    ~CDBValveCfg();

    bool UpdateGlbCfg(VALVECFG* ptCfg);
    bool GetGlbCfg(VALVECFG* ptCfg);

private:
    void Empty();
    void GetTable();

    bool GetValues(string strVal, BYTE* ptAddr);
};

