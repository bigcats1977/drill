#pragma once
#include "DB/DBBase.h"
class CDBWITSCfg :  public CDBBase
{
public:
    UINT  _TCPPort;
    vector<int>  _ShowParas;
    vector<int>  _FixItems;
    vector<int>  _RepeatItems;
    vector<int>  _CalItems;
    vector<int>  _ShowItems;

    CDBWITSCfg();
    ~CDBWITSCfg();

    BOOL   UpdateWITSCfg(WITSCFG* ptfg);

private:

    void Empty();
    void GetTable();
};
