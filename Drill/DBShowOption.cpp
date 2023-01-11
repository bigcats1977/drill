#include "DBShowOption.h"

CDBShowOption::CDBShowOption()
{
    _TableIndex = T_SHOWOPTION;

    Reload();
}

CDBShowOption::~CDBShowOption()
{
    Empty();
}

void CDBShowOption::Empty()
{
    _lsNO.clear();
    _lsLangType.clear();
    _lsShowIndex.clear();
    _lsOption.clear();
}

void CDBShowOption::GetTable()
{
    int row = 0, col = 0;
    char** pResult;
    UINT nIndex = 0;
    int m = 0, n = 0;
    int Value;
    string Content;

    if (!_Sqlite->GetTable(g_tTableName[_TableIndex], row, col, &pResult))
        return;
    if (row <= 0)
    {
        _Sqlite->FreeResult(&pResult);
        return;
    }
    _Count = row;

    nIndex = col;

    for (m = 0; m < row; m++)
    {
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsNO.push_back(Value);
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsLangType.push_back(Value);
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsShowIndex.push_back(Value);

        _Sqlite->GetValue(pResult[nIndex++], Content);
        _lsOption.push_back(Content);

        // last update time
        nIndex++;
    }

    _Sqlite->FreeResult(&pResult);
}

// 最多取前50个选项
vector<string> CDBShowOption::GetOptionsByNameIndex(int index)
{
    int i = 0;
    string condition;
    int row = 0, col = 0;
    char** pResult;
    UINT nIndex = 0;
    string option;
    vector<string> lsOptions;

    condition = "LangType=" + to_string(*_CurLang) + " AND ShowIndex=" + to_string(index);
    //if (!_Sqlite->QueryTable(g_tTableName[_TableIndex], condition, row, col, &pResult))
    if (!_Sqlite->QueryTableOrder(g_tTableName[_TableIndex], condition, "LastUsedTime", false, row, col, &pResult))
        return lsOptions;

    if (row <= 0)
    {
        _Sqlite->FreeResult(&pResult);
        return lsOptions;
    }

    nIndex = col;
    // 返回最新更新的50个选项
    for (i = 0; i < row && i < MAX_SHOWOPTNUM; i++)
    {
        // NO
        nIndex++;
        // LangType
        nIndex++;
        // ShowIndex
        nIndex++;
        // Option
        _Sqlite->GetValue(pResult[nIndex++], option);
        lsOptions.push_back(option);
        // last update time
        nIndex++;
    }
    _Sqlite->FreeResult(&pResult);
    return lsOptions;
}


string CDBShowOption::GetOptionByNO(int NO)
{
    vector<int>::iterator it;

    COMP_BFALSE_R(_ValidDB, NULLSTR);
    COMP_BL_R(NO, 0, NULLSTR);

    it = find(_lsNO.begin(), _lsNO.end(), NO);
    if (it != _lsNO.end())
    {
        return _lsOption[it - _lsNO.begin()];
    }

    return NULLSTR;
}
vector<string> CDBShowOption::GetOptionsByNOs(string NOs)
{
    int i = 0;
    vector<string> lsOptions;
    string ID;
    vector<int> lsID;
    char delimiter = ',';
    vector<int>::iterator it;

    COMP_BFALSE_R(_ValidDB, lsOptions);
    if (NOs.empty())
        return lsOptions;

    lsID = GetIDFromList(NOs);

    if (lsID.size() <= 0)
        return lsOptions;

    // get ID's text
    for (i = 0; i < (int)lsID.size(); i++)
    {
        it = find(_lsNO.begin(), _lsNO.end(), lsID[i]);
        if (it != _lsNO.end())
        {
            lsOptions.push_back(_lsOption[it - _lsNO.begin()]);
        }
    }
    return lsOptions;
}

bool CDBShowOption::GetOptionsByNOS(string NOs, string Values[])
{
    int i = 0;
    vector<string> lsOptions;
    string ID;
    vector<int> lsID;
    char delimiter = ',';
    vector<int>::iterator it;

    COMP_BFALSE_R(_ValidDB, false);
    if (NOs.empty())
        return false;

    lsID = GetIDFromList(NOs);

    if (lsID.size() <= 0)
        return false;

    // get ID's text
    for (i = 0; i < (int)lsID.size(); i++)
    {
        it = find(_lsNO.begin(), _lsNO.end(), lsID[i]);
        if (it != _lsNO.end())
        {
            Values[i] = _lsOption[it - _lsNO.begin()];
        }
    }
    return true;
}

int CDBShowOption::GetNOByOption(int Name, string value, bool bIns)
{
    int NO = DB_INVALID_VAL;
    string condition;
    int row = 0, col = 0;
    char** pResult;
    UINT nIndex = 0;

    if (value.empty())
        return DB_INVALID_VAL;

    condition = "LangType=" + to_string(*_CurLang) + " AND ShowIndex=" + to_string(Name) + " AND Option ='" + value + "'";
    if (!_Sqlite->QueryTable(g_tTableName[_TableIndex], condition, row, col, &pResult))
        return DB_INVALID_VAL;
    if (row <= 0)
    {
        _Sqlite->FreeResult(&pResult);

        COMP_BFALSE_R(bIns, DB_INVALID_VAL);
        COMP_BFALSE_R(InsertOption(Name, value), DB_INVALID_VAL);

        return _Sqlite->GetLastInsertID();
    }

    nIndex = col;
    // NO
    _Sqlite->GetValue(pResult[nIndex++], NO);

    _Sqlite->FreeResult(&pResult);

    // update used time by NO
    condition = "NO=" + to_string(NO);
    _Sqlite->UpdateField(g_tTableName[_TableIndex], condition, "LastUsedTime", GetCurTime());

    return NO;
}

int CDBShowOption::InsertOption(int index, string Value)
{
    vector<string> strValues;

    if (Value.empty())
        return DB_INVALID_VAL;

    strValues.push_back(NULLSTR);
    strValues.push_back(to_string(*_CurLang));
    strValues.push_back(to_string(index));
    strValues.push_back(Value);
    strValues.push_back(GetCurTime());

    if (!_Sqlite->InsertRow(g_tTableName[_TableIndex], strValues))
        return DB_INVALID_VAL;

    return _Sqlite->GetLastInsertID();
}

string CDBShowOption::GetNOsByOptions(UINT* NameID, string* Values)
{
    int index = DB_INVALID_VAL;
    int i = 0;
    string ValIndexs;

    COMP_BFALSE_R(_ValidDB, ValIndexs);
    ASSERT_NULL_R(NameID, ValIndexs);
    ASSERT_NULL_R(Values, ValIndexs);

    for (i = 0; i < MAXPARANUM; i++)
    {
        if (!Values[i].empty())
        {
            index = GetNOByOption(NameID[i], Values[i], true);
            if (DB_INVALID_VAL != index)
            {
                ValIndexs += to_string(index);
            }
        }
        else
        {
            ValIndexs += " ";
        }

        ValIndexs += ",";
    }

    return ValIndexs;
}
