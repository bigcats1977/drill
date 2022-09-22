#pragma once
#include "DBBase.h"
class CDBShowOption :public CDBBase
{
public:
    vector<int> _lsNO;
    vector<int> _lsShowIndex;
    vector<int> _lsLangType;

    vector<string> _lsOption;
    vector<string> _lsLastUsedTime;

    CDBShowOption();
    ~CDBShowOption();

    int  InsertOption(int index, string Value);
    string GetOptionByNO(int NO);
    vector<string> GetOptionsByNOs(string NOs);
    BOOL GetOptionsByNOS(string NOs, string Values[]);
    vector<string> GetOptionsByNameIndex(int index);
    string GetNOsByOptions(UINT *NameID, string *Values);
    int GetNOByOption(int Name, string value, BOOL bIns = FALSE);

private:
    void Empty();
    void GetTable();
};

