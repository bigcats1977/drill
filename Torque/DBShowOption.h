#pragma once
#include "DBBase.h"
#define MAX_SHOWOPTNUM      50
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
    bool GetOptionsByNOS(string NOs, string Values[]);
    vector<string> GetOptionsByNameIndex(int index);
    string GetNOsByOptions(UINT *NameID, string *Values);
    int GetNOByOption(int Name, string value, bool bIns = false);

private:
    void Empty();
    void GetTable();
};

