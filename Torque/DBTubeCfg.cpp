#include "DBTubeCfg.h"

CDBTubeCfg::CDBTubeCfg()
{
    _TableIndex = T_TUBECFG;
    Reload();
}

CDBTubeCfg::~CDBTubeCfg()
{
    Empty();

    _lsFixTube.clear();
    _lsTorqVal.clear();
}

void CDBTubeCfg::Empty()
{
    _lsAutoIndex.clear();
    /*_lsFactory.clear();
    _lsOEM.clear();
    _lsSize.clear();
    _lsMaterial.clear();
    _lsCoupling.clear();*/
    _lsFixTube.clear();
    //_lsFixTube.resize(MAXTUBECFGNUM);

    _lsTorqVal.clear();
    //_lsTorqVal.resize(MAXTUBETORQNUM);
}

void CDBTubeCfg::GetTable()
{
    int row = 0, col = 0;
    char** pResult;
    UINT nIndex = 0;
    int m = 0, n = 0;
    int Value = 0;

    if (!_Sqlite->GetTable(g_tTableName[_TableIndex], row, col, &pResult))
        return;
    if (row <= 0)
    {
        _Sqlite->FreeResult(&pResult);
        return;
    }

    _Count = row;

    nIndex = col;

    _lsFixTube.resize(_Count);
    _lsTorqVal.resize(_Count);
    for (m = 0; m < row; m++)
    {
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsAutoIndex.push_back(Value);

        for (n = 0; n < MAXTUBECFGNUM; n++)
        {
            _Sqlite->GetValue(pResult[nIndex++], Value);
            _lsFixTube[m].push_back(Value);
        }

        for (n = 0; n < MAXTUBETORQNUM; n++)
        {
            _Sqlite->GetValue(pResult[nIndex++], Value);
            _lsTorqVal[m].push_back(Value);
        }
    }

    _Sqlite->FreeResult(&pResult);
}

BOOL CDBTubeCfg::CheckTubeValue(TUBECFG* ptTube)
{
    int i = 0;
    UINT nCount = 0;
    ASSERT_NULL_R(ptTube, FALSE);

    for (i = 0; i < MAXTUBECFGNUM; i++)
        nCount += ptTube->nFixTube[i];
    ASSERT_ZERO_R(nCount, FALSE);

    return TRUE;
}

BOOL CDBTubeCfg::QueryTubeByIndex(TUBECFG* ptTube)
{
    UINT  i = 0;
    int   iOffset;
    vector<int>::iterator it;

    COMP_BFALSE_R(_ValidDB, FALSE);
    ASSERT_NULL_R(ptTube, FALSE);

    it = find(_lsAutoIndex.begin(), _lsAutoIndex.end(), ptTube->nIndex);
    if (it == _lsAutoIndex.end())
        return FALSE;

    iOffset = it - _lsAutoIndex.begin();
    for (i = 0; i < MAXTUBECFGNUM; i++)
    {
        ptTube->nFixTube[i] = _lsFixTube[iOffset][i];
    }

    // Torq Value
    for (i = 0; i < MAXTUBETORQNUM; i++)
    {
        ptTube->nTorqVal[i] = _lsTorqVal[iOffset][i];
    }

    return TRUE;
}

int CDBTubeCfg::QueryTubeByInfo(TUBECFG* ptTube)
{
    string strCond;
    int row = 0, col = 0;
    char** pResult;
    UINT nIndex = 0;
    UINT  i = 0;

    COMP_BFALSE_R(_ValidDB, DB_INVALID_VAL);
    ASSERT_NULL_R(ptTube, DB_INVALID_VAL);
    COMP_BFALSE_R(CheckTubeValue(ptTube), 0);

    // GET torque value by fix tubing parameter
    for (i = 0; i < MAXTUBECFGNUM; i++)
    {
        if (!strCond.empty())
            strCond += " AND ";
        strCond += "FixTube" + to_string(i + 1) + " = " + to_string(ptTube->nFixTube[i]);
    }
    if (!_Sqlite->QueryTable(g_tTableName[_TableIndex], strCond, row, col, &pResult))
        return DB_INVALID_VAL;
    if (row <= 0)
    {
        _Sqlite->FreeResult(&pResult);
        return DB_INVALID_VAL;
    }

    nIndex = col;

    // autoindex
    _Sqlite->GetValue(pResult[nIndex++], ptTube->nIndex);

    // Tube Cfg ID
    nIndex += MAXTUBECFGNUM;

    // Torq Value
    for (i = 0; i < MAXTUBETORQNUM; i++)
    {
        _Sqlite->GetValue(pResult[nIndex++], ptTube->nTorqVal[i]);
    }

    _Sqlite->FreeResult(&pResult);

    return ptTube->nIndex;
}

int CDBTubeCfg::InsertTubeCfg(TUBECFG* ptTube)
{
    int i = 0;
    vector<string> strValues;

    COMP_BFALSE_R(_ValidDB, DB_INVALID_VAL);
    ASSERT_NULL_R(ptTube, DB_INVALID_VAL);
    COMP_BFALSE_R(CheckTubeValue(ptTube), 0);

    // Auto Index
    strValues.push_back(NULLSTR);
    for (i = 0; i < MAXTUBECFGNUM; i++)
    {
        strValues.push_back(to_string(ptTube->nFixTube[i]));
    }
    for (i = 0; i < MAXTUBETORQNUM; i++)
    {
        strValues.push_back(to_string(ptTube->nTorqVal[i]));
    }

    if(! _Sqlite->InsertRow(g_tTableName[_TableIndex], strValues))
        return DB_INVALID_VAL;

    ptTube->nIndex = _Sqlite->GetLastInsertID();
    return ptTube->nIndex;
}

BOOL CDBTubeCfg::DeleteTubeCfg(TUBECFG* ptTube)
{
    string strCond;

    COMP_BFALSE_R(_ValidDB, FALSE);
    ASSERT_NULL_R(ptTube, FALSE);
    COMP_BFALSE_R(CheckTubeValue(ptTube), TRUE);

    strCond = "AutoIndex=" + to_string(ptTube->nIndex);
    return _Sqlite->DeleteRow(g_tTableName[_TableIndex], strCond);
}

BOOL CDBTubeCfg::UpdateTubeCfg(TUBECFG* ptTube)
{
    UINT i = 0;
    string strCond;
    vector<string> strFields;
    vector<string> strValues;

    COMP_BFALSE_R(_ValidDB, FALSE);
    ASSERT_NULL_R(ptTube, FALSE);
    COMP_BFALSE_R(CheckTubeValue(ptTube), TRUE);

    strCond = "AutoIndex=" + to_string(ptTube->nIndex);

    for (i = 0; i < MAXTUBETORQNUM; i++)
    {
        strFields.push_back("TorqVal" + to_string(i+1));
        strValues.push_back(to_string(ptTube->nTorqVal[i]));
    }

    return _Sqlite->UpdateFields(g_tTableName[_TableIndex], strCond, strFields, strValues);
}

int CDBTubeCfg::GetTubeIndexByInfo(TUBECFG* ptTube)
{
    int index = 0;
    COMP_BFALSE_R(_ValidDB, DB_INVALID_VAL);
    ASSERT_NULL_R(ptTube, DB_INVALID_VAL);
    COMP_BFALSE_R(CheckTubeValue(ptTube), 0);

    index = QueryTubeByInfo(ptTube);
    if (index >= 0)
        return index;

    // insert
    return InsertTubeCfg(ptTube);
}

