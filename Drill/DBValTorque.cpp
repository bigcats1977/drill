#include "DBValTorque.h"


CDBValTorque::CDBValTorque()
{
    _TableIndex = T_VALTORQUE;

    Reload();
}

CDBValTorque::~CDBValTorque()
{
    Empty();

    _lsTorqueVal.clear();
}

void CDBValTorque::Empty()
{
    _lsAutoIndex.clear();

    _lsTorqueVal.clear();
}

void CDBValTorque::GetTable()
{
    int row = 0, col = 0;
    char** pResult;
    UINT nIndex = 0;
    int m = 0, n = 0;
    int Value = 0;
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

    _lsTorqueVal.resize(_Count);
    for (m = 0; m < row; m++)
    {
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsAutoIndex.push_back(Value);

        for (n = 0; n < MAXTORQCONFNUM; n++)
        {
            _Sqlite->GetValue(pResult[nIndex++], fVal);
            _lsTorqueVal[m].push_back(fVal);
        }
    }

    _Sqlite->FreeResult(&pResult);
}

bool CDBValTorque::GetTorqValById(int index, double* pVal)
{
    vector<int>::iterator it;
    int iOffset = 0;
    int i = 0;

    ASSERT_NULL_R(pVal, false);

    it = find(_lsAutoIndex.begin(), _lsAutoIndex.end(), index);
    if (it == _lsAutoIndex.end())
        return false;

    iOffset = it - _lsAutoIndex.begin();

    for (i = 0; i < MAXTORQCONFNUM; i++)
    {
        pVal[i] = _lsTorqueVal[iOffset][i];
    }

    return true;
}

int CDBValTorque::QueryIDByInfo(CONTROLPARA* ptCtrl)
{
    string strCond;
    int row = 0, col = 0;
    char** pResult;
    UINT nIndex = 0;
    int  autoindex = 0;
    int i = 0;

    COMP_BFALSE_R(_ValidDB, DB_INVALID_VAL);
    ASSERT_NULL_R(ptCtrl, DB_INVALID_VAL);

    // GET torque value by fix tubing parameter
    for (i = 0; i < MAXTORQCONFNUM; i++)
    {
        if (!strCond.empty())
            strCond += " AND ";
        strCond += "TorqVal" + to_string(i + 1) + " = " + to_string(ptCtrl->fTorqConf[i]);
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
    _Sqlite->GetValue(pResult[nIndex++], autoindex);

    _Sqlite->FreeResult(&pResult);

    return autoindex;
}

int CDBValTorque::GetIndexByInfo(CONTROLPARA* ptCtrl)
{
    int i = 0;
    vector<string> strValues;
    int index = DB_INVALID_VAL;

    COMP_BFALSE_R(_ValidDB, DB_INVALID_VAL);
    ASSERT_NULL_R(ptCtrl, DB_INVALID_VAL);

    index = QueryIDByInfo(ptCtrl);
    if (index > 0)
        return index;

    // insert current data and return index
    // Auto Index
    strValues.push_back(NULLSTR);
    for (i = 0; i < MAXTORQCONFNUM; i++)
    {
        strValues.push_back(to_string(ptCtrl->fTorqConf[i]));
    }

    if (!_Sqlite->InsertRow(g_tTableName[_TableIndex], strValues))
        return DB_INVALID_VAL;

    return _Sqlite->GetLastInsertID();
}

