
#include "DBText.h"

CDBText::CDBText()
{
    Empty();
}

CDBText::CDBText(UINT table)
{
    if (table< T_TUBEFACTORY || table > T_TUBECOUPL)
        return;

    _TableIndex = table;

    Reload();
}
CDBText::~CDBText()
{
    Empty();
}

void CDBText::Empty()
{
    _Count = 0;
    _lsNO.clear();
    for (int i = 0; i < LANGUAGE_NUM; i++)
        _lsName[i].clear();
}

void CDBText::GetTable()
{
    int row = 0, col = 0;
    char** pResult;
    int m = 0, n = 0;
    UINT nIndex = 0;
    UINT NO;
    string Name;

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
        for (n = 0; n < col; n++)
        {
            if (n == 0)
            {
                _Sqlite->GetValue(pResult[nIndex++], NO);
                _lsNO.push_back(NO);
            }
            else
            {
                _Sqlite->GetValue(pResult[nIndex++], Name);
                _lsName[n - 1].push_back(Name);
            }
        }
    }

    _Sqlite->FreeResult(&pResult);
}

BOOL CDBText::CheckNO(int NO)
{
    vector<int>::iterator it;

    it = find(_lsNO.begin(), _lsNO.end(), NO);

    if (it != _lsNO.end())
        return TRUE;

    return FALSE;
}

vector<string> CDBText::GetAllName()
{
    COMP_BFALSE_R(_ValidDB, vector<string>());

    return _lsName[*_CurLang];
}

string CDBText::GetNameByNO(int NO)
{
    int i = 0;
    vector<int>::iterator it;

    COMP_BFALSE_R(_ValidDB, NULLSTR);

    it = find(_lsNO.begin(), _lsNO.end(), NO);
    if (it != _lsNO.end())
    {
        return _lsName[*_CurLang][it - _lsNO.begin()];
    }

    return NULLSTR;
}

BOOL CDBText::UpdateNameByNO(int NO, string name)
{
    vector<int>::iterator it;
    string strField;
    string strCond;
    vector<string> lsField;
    vector<string> lsValue;

    COMP_BFALSE_R(_ValidDB, FALSE);
    if (name.empty())
        return FALSE;
    it = find(_lsNO.begin(), _lsNO.end(), NO);
    if (it == _lsNO.end())
        return FALSE;

    strCond = "NO=" + to_string(NO);
    lsField.push_back("Name" + to_string(*_CurLang + 1));
    lsValue.push_back(name);

    return _Sqlite->UpdateFields(g_tTableName[_TableIndex], strCond, lsField, lsValue);
}

BOOL CDBText::UpdateNames(int NO, vector<string> Names)
{
    int i = 0;
    string strCond;
    vector<string> strFields;
    vector<string> strValues;

    COMP_BFALSE_R(_ValidDB, FALSE);
    if (!CheckNO(NO))
        return FALSE;

    strCond = "NO=" + to_string(NO);
    for (i = 0; i < (int)Names.size(); i++)
    {
        strFields.push_back("Name" + to_string(i + 1));
        strValues.push_back(Names[i]);
    }

    return _Sqlite->UpdateFields(g_tTableName[_TableIndex], strCond, strFields, strValues);
}

int CDBText::InsertNames(int NO, vector<string> Names)
{
    int i = 0;
    vector<string> strValues;

    COMP_BFALSE_R(_ValidDB, DB_INVALID_VAL);

    if (CheckNO(NO))
        return DB_INVALID_VAL;

    strValues.push_back(to_string(NO));
    for(i=0;i<LANGUAGE_NUM;i++)
        strValues.push_back(Names[i]);

    if(! _Sqlite->InsertRow(g_tTableName[_TableIndex], strValues))
        return DB_INVALID_VAL;

    return _Sqlite->GetLastInsertID();
}

BOOL CDBText::DeleteNO(int NO)
{
    string strCond;

    COMP_BFALSE_R(_ValidDB, FALSE);

    if (!CheckNO(NO))
        return FALSE;

    strCond = "NO=" + to_string(NO);
    return _Sqlite->DeleteRow(g_tTableName[_TableIndex], strCond);
}