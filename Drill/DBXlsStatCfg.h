#pragma once
#include "DBBase.h"
class CDBXlsStatCfg : public CDBBase
{
public:
    vector<int> _lsLangType;

    vector<string> _GenPara;
    vector<string> _JobPara;
    vector<string> _InfoPara;

    CDBXlsStatCfg();
    ~CDBXlsStatCfg();
    bool GetInfoByLang(XLSSTATCFG* ptCfg, UINT nLang = LANGUAGE_CURRENT);
    bool UpdateInfo(XLSSTATCFG* ptCfg);

private:
    void Empty();
    void GetTable();
    bool SetItemValue(vector<int> lsVals, int* pVals, int num);
};

