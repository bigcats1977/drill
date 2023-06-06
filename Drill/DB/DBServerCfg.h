#pragma once
#include "DBBase.h"
class CDBServerCfg : public CDBBase
{
public:
    string  _IPAddr;
    UINT    _FTPPort;
    string  _UserName;
    string  _Password;
    string  _TargetPath;

    CDBServerCfg();
    ~CDBServerCfg();

    BOOL   UpdateServerCfg(SERVERCFG* ptfg);

private:
    void Empty();
    void GetTable();
};

