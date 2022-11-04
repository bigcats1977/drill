#pragma once
#include "DBBase.h"
class CDBShowCfg : public CDBBase
{
public:
    vector<int> _lsLangType;
    vector<int> _lsParaNum;
    vector<int> _lsListNum;
    vector<int> _lsMainNum;
    vector<int> _lsFileName;
    vector<int> _lsStatType;
    vector<int> _lsAliasID;

    vector<string> _lsShowPara;
    vector<string> _lsListNO;
    vector<string> _lsMainNO;

    CDBShowCfg();
    ~CDBShowCfg();

    BOOL UpdateShowCfg(SHOWCFG* ptShow);
    BOOL UpdateAlias(UINT Alias);

private:
    void Empty();
    void GetTable();
};

