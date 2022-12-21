#include "DBValTurn.h"

CDBValTurn::CDBValTurn()
{
    _TableIndex = T_VALTURN;

    Reload();
}

CDBValTurn::~CDBValTurn()
{
    Empty();

    _lsTurnVal.clear();
}

void CDBValTurn::Empty()
{
    _lsAutoIndex.clear();

    _lsTurnVal.clear();
}

void CDBValTurn::GetTable()
{
    int row = 0, col = 0;
    char** pResult;
    UINT nIndex = 0;
    int m = 0, n = 0;
    int  Value = 0;
    double fVal = 0;
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
    _lsTurnVal.resize(_Count);
    for (m = 0; m < row; m++)
    {
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsAutoIndex.push_back(Value);

        for (n = 0; n < MAXTURNCONFNUM; n++)
        {
            _Sqlite->GetValue(pResult[nIndex++], fVal);
            _lsTurnVal[m].push_back(fVal);
        }
    }

    _Sqlite->FreeResult(&pResult);
}

bool CDBValTurn::GetTurnValById(int index, double* pVal)
{
    vector<int>::iterator it;
    int iOffset = 0;
    int i = 0;

    ASSERT_NULL_R(pVal, false);

    it = find(_lsAutoIndex.begin(), _lsAutoIndex.end(), index);
    if (it == _lsAutoIndex.end())
        return false;

    iOffset = it - _lsAutoIndex.begin();

    for (i = 0; i < MAXTURNCONFNUM; i++)
    {
        pVal[i] = _lsTurnVal[iOffset][i];
    }

    return true;
}


int CDBValTurn::QueryIDByInfo(CONTROLPARA* ptCtrl)
{
    string strCond;
    int row = 0, col = 0;
    char** pResult;
    UINT nIndex = 0;
    int   i = 0;
    int Autoindex = 0;

    COMP_BFALSE_R(_ValidDB, DB_INVALID_VAL);
    ASSERT_NULL_R(ptCtrl, DB_INVALID_VAL);

    // GET Turn value by fix tubing parameter
    for (i = 0; i < MAXTURNCONFNUM; i++)
    {
        if (!strCond.empty())
            strCond += " AND ";
        strCond += "TurnVal" + to_string(i + 1) + " = " + to_string(ptCtrl->fTurnConf[i]);
    }
    if (!_Sqlite->QueryTable(g_tTableName[_TableIndex], strCond, row, col, &pResult))
        return DB_INVALID_VAL;
    if (row <= 0)
    {
        _Sqlite->FreeResult(&pResult);
        return DB_INVALID_VAL;
    }

    nIndex = col;

    // NO
    _Sqlite->GetValue(pResult[nIndex++], Autoindex);

    _Sqlite->FreeResult(&pResult);

    return Autoindex;
}

int CDBValTurn::GetIndexByInfo(CONTROLPARA* ptCtrl)
{
    int i = 0;
    vector<string> strValues;
    int index = 0;

    COMP_BFALSE_R(_ValidDB, DB_INVALID_VAL);
    ASSERT_NULL_R(ptCtrl, DB_INVALID_VAL);

    index = QueryIDByInfo(ptCtrl);
    if (index > 0)
        return index;

    // insert current data and return index
    // Auto Index
    strValues.push_back(NULLSTR);
    for (i = 0; i < MAXTURNCONFNUM; i++)
    {
        strValues.push_back(to_string(ptCtrl->fTurnConf[i]));
    }

    if (!_Sqlite->InsertRow(g_tTableName[_TableIndex], strValues))
        return FALSE;

    return _Sqlite->GetLastInsertID();
}

