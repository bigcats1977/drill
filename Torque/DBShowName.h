#pragma once
#include "DBBase.h"
class CDBShowName :  public CDBBase
{
public:
    vector<int>     _lsAutoIndex;
    vector<int>     _lsLangType;
    vector<int>     _lsNO;
    vector<string>  _lsName;

    CDBShowName();
    ~CDBShowName();
    int     InsertShowName(int NO, string Name);
    int     GetIndexByNO(int NO);
    vector<string> GetNamesByIndexs(string indexs);
    vector<string> GetNamesByNOs(string NOs, UINT nLang=LANGUAGE_CURRENT);
    vector<int> GetIndexsByNOs(string NOs);
    //BOOL    UpdateBulkNames(WORD wNum, string names[]);
    int     UpdateShowName(int NO, string Name);

private:
    void Empty();
    void GetTable();
    //string GetNameByLangNO(UINT NO);
    int GetNOByName(string Name, int &Index);
};

