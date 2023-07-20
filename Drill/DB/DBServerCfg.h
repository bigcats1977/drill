#pragma once
#include "DBBase.h"
class CDBServerCfg : public CDBBase
{
public:
    string  _FTPAddr;
    UINT    _FTPPort;
    string  _UserName;
    string  _Password;
    string  _TargetPath;
    UINT    _TCPPort;

    CDBServerCfg();
    ~CDBServerCfg();

    BOOL   UpdateServerCfg(SERVERCFG* ptfg);

private:
    void Empty();
    void GetTable();
};

