#pragma once

#include "DBBase.h"

class CDBText : public CDBBase
{
public:
    vector<int>     _lsNO;
    vector<string>  _lsName[LANGUAGE_NUM];

    CDBText();
    CDBText(UINT table);
    ~CDBText();

    vector<string> GetAllName();
    string  GetNameByNO(int NO);
    BOOL    CheckNO(int NO);
    BOOL    UpdateNameByNO(int NO, string name);
    BOOL    UpdateNames(int NO, vector<string> Names);
    int     InsertNames(int NO, vector<string> Names);
    BOOL    DeleteNO(int NO);

private:
    void Empty();
    void GetTable();
};

