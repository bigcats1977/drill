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
    //vector<int> _lsIsFixTube;
    vector<int> _lsAliasID;

    vector<string> _lsShowPara;
    vector<string> _lsListNO;
    vector<string> _lsMainNO;
    //vector<string> _lsShowValue;

    //vector<int> _lsValParaID;
    //vector<vector<int>> _lsFixTube;

    CDBShowCfg();
    ~CDBShowCfg();

    BOOL UpdateShowCfg(SHOWCFG* ptShow);
    BOOL UpdateAlias(UINT Alias);
    //BOOL UpdateShowValue(string curOption, SHOWCFG* ptShow);


private:
    void Empty();
    void GetTable();
};

