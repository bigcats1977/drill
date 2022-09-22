#pragma once
#include "DBBase.h"
class CDBXlsStatCfg :  public CDBBase
{
public:
    vector<int> _lsLangType;
    vector<int> _lsOperator;
    vector<int> _lsTally;

    vector<string> _lsSummary;
    vector<string> _lsReport;

    CDBXlsStatCfg();
    ~CDBXlsStatCfg();
    BOOL GetInfoByLang(XLSSTATCFG* ptCfg, UINT nLang= LANGUAGE_CURRENT);
    BOOL UpdateInfo(XLSSTATCFG* ptCfg);

private:
    void Empty();
    void GetTable();
    BOOL SetItemValue(vector<int> lsVals, int type, BOOL *pVals);
};

