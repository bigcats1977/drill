#include "DBShowName.h"

CDBShowName::CDBShowName()
{
    _TableIndex = T_SHOWNAME;
    Reload();
}

CDBShowName::~CDBShowName()
{
    Empty();
}

void CDBShowName::Empty()
{
    _Count = 0;

    _lsAutoIndex.clear();
    _lsLangType.clear();
    _lsNO.clear();
    _lsName.clear();
}

void CDBShowName::GetTable()
{
    int row = 0, col = 0;
    char** pResult;
    int m = 0;
    UINT nIndex = 0;
    int Value = 0;
    string Name = "";

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
        _lsAutoIndex.push_back(Value);
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsLangType.push_back(Value);
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsNO.push_back(Value);

        _Sqlite->GetValue(pResult[nIndex++], Name);
        _lsName.push_back(Name);
    }

    _Sqlite->FreeResult(&pResult);
}


int  CDBShowName::GetIndexByNO(int NO)
{
    vector<int>::iterator it;
    int iOffset = 0;

    COMP_BFALSE_R(_ValidDB, DB_INVALID_VAL);

    it = find(_lsNO.begin(), _lsNO.end(), NO);
    while (it != _lsNO.end())
    {
        iOffset = it - _lsNO.begin();
        if (_lsLangType[iOffset] == *_CurLang)
        {
            return _lsAutoIndex[iOffset];
        }
        it++;
        it = find(it, _lsNO.end(), NO);
    }

    return DB_INVALID_VAL;
}

vector<string> CDBShowName::GetNamesByNOs(string NOs, UINT nLang)
{
    int i = 0;
    vector<string> lsNames;
    vector<int> lsNO;
    vector<int>::iterator it;
    int iOffset = 0;

    COMP_BFALSE_R(_ValidDB, lsNames);
    if (NOs.empty())
        return lsNames;
    CheckLanguage(nLang);

    lsNO = GetIDFromList(NOs);
    if (lsNO.size() <= 0)
        return lsNames;

    // get ID's text
    for (i = 0; i < (int)lsNO.size(); i++)
    {
        it = find(_lsNO.begin(), _lsNO.end(), lsNO[i]);
        while (it != _lsNO.end())
        {
            iOffset = it - _lsNO.begin();
            if (_lsLangType[iOffset] == nLang)
            {
                lsNames.push_back(_lsName[iOffset]);
                break;
            }
            it++;
            it = find(it, _lsNO.end(), lsNO[i]);
        }
    }
    return lsNames;
}

vector<string> CDBShowName::GetNamesByIndexs(string indexs)
{
    int i = 0;
    vector<string> lsNames;
    vector<int> lsIndex;
    vector<int>::iterator it;

    COMP_BFALSE_R(_ValidDB, lsNames);
    if (indexs.empty())
        return lsNames;

    lsIndex = GetIDFromList(indexs);
    if (lsIndex.size() <= 0)
        return lsNames;

    // get index's text
    for (i = 0; i < (int)lsIndex.size(); i++)
    {
        it = find(_lsAutoIndex.begin(), _lsAutoIndex.end(), lsIndex[i]);
        if (it == _lsAutoIndex.end())
        {
            lsNames.push_back(NULLSTR);
        }
        else
        {
            lsNames.push_back(_lsName[it - _lsAutoIndex.begin()]);
        }
    }
    return lsNames;
}

vector<int> CDBShowName::GetIndexsByNOs(string NOs)
{
    int i = 0;
    vector<int> lsIndexs;
    vector<int> lsNO;
    char delimiter = ',';
    vector<int>::iterator it;
    int iOffset = 0;

    COMP_BFALSE_R(_ValidDB, lsIndexs);
    if (NOs.empty())
        return lsIndexs;

    lsNO = GetIDFromList(NOs);

    // get ID's text
    for (i = 0; i < (int)lsNO.size(); i++)
    {
        it = find(_lsNO.begin(), _lsNO.end(), lsNO[i]);
        while (it != _lsNO.end())
        {
            iOffset = it - _lsNO.begin();
            if (_lsLangType[iOffset] == *_CurLang)
            {
                lsIndexs.push_back(_lsAutoIndex[iOffset]);
                break;
            }
            it++;
            it = find(it, _lsNO.end(), lsNO[i]);
        }
    }
    return lsIndexs;
}

int CDBShowName::GetNOByName(string Name, int &Index)
{
    int iNO = DB_INVALID_VAL;
    BOOL res = FALSE;
    int row = 0, col = 0;
    char** pResult;
    UINT nIndex = 0;
    string strCond;

    Index = -1;

    COMP_BFALSE_R(_ValidDB, DB_INVALID_VAL);
    if (Name.empty())
        return DB_INVALID_VAL;

    // get index of Name
    strCond = "LangType='" + to_string(*_CurLang) + "' AND Name = '" + Name + "'";
    if (!_Sqlite->QueryTable(g_tTableName[_TableIndex], strCond, row, col, &pResult))
        return DB_INVALID_VAL;
    if (row <= 0)
    {
        _Sqlite->FreeResult(&pResult);
        return DB_INVALID_VAL;
    }

    nIndex = col;

    // AutoIndex
    _Sqlite->GetValue(pResult[nIndex++], Index);

    // LangType
    nIndex++;

    _Sqlite->GetValue(pResult[nIndex++], iNO);

    _Sqlite->FreeResult(&pResult);

    return iNO;
}

int CDBShowName::InsertShowName(int NO, string Name)
{
    int oldIndex = DB_INVALID_VAL;
    int i = 0;
    int iNO = -1;
    vector<string> strValues;

    iNO = GetNOByName(Name, oldIndex);
    if (iNO == NO)
        return DB_INVALID_VAL;

    strValues.push_back(NULLSTR);
    strValues.push_back(to_string(*_CurLang));
    strValues.push_back(to_string(NO));
    strValues.push_back(Name);

    if(! _Sqlite->InsertRow(g_tTableName[_TableIndex], strValues))
        return DB_INVALID_VAL;

    return _Sqlite->GetLastInsertID();
}

int CDBShowName::UpdateShowName(int NO, string Name)
{
    int Index = DB_INVALID_VAL;
    int oldIndex = DB_INVALID_VAL;
    int iNO = DB_INVALID_VAL;
    stringstream ss;
    string sql;
    string xxx;

    COMP_BFALSE_R(_ValidDB, FALSE);
    if (Name.empty())
        return DB_INVALID_VAL;
    iNO = GetNOByName(Name, Index);
    COMP_BE_R(iNO, NO, Index);

    ss << "BEGIN;\n";

    //Remove Old Index
    oldIndex = GetIndexByNO(NO);
    if(oldIndex != DB_INVALID_VAL)
        ss << "UPDATE " << g_tTableName[_TableIndex] << " SET " << "NO=NULL" << " WHERE " << "AutoIndex=" << oldIndex << ";\n";

    if (Index == DB_INVALID_VAL)
    {
        // why ÄÚ´æÐ¹Â©8Ö±½Ó 01 00 00 00 CD CD CD CD
        //ss << "INSERT INTO " << g_tTableName[_TableIndex] << " VALUES(NULL," <<nLang <<","<<NO <<",'"<< _Sqlite->ASCII2UTF8(Name) << "');\n";
        xxx = "INSERT INTO " + g_tTableName[_TableIndex] + " VALUES(NULL," + to_string(*_CurLang) + "," + to_string(NO) + ",'" + ASCII2UTF8(Name) + "');\n";
        ss << xxx;
    }
    else
    {
        ss << "UPDATE " << g_tTableName[_TableIndex] << " SET " << "NO=" <<NO<< " WHERE " << "AutoIndex=" << Index << ";\n";
    }

    ss << "COMMIT;";

    sql = ss.str();

    if (!_Sqlite->ExecuteSQL(sql))
        return DB_INVALID_VAL;

    if (DB_INVALID_VAL == Index)
    {
        Index = _Sqlite->GetLastInsertID();
    }

    return Index;
}
