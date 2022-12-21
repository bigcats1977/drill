#include "DBValveCfg.h"

CDBValveCfg::CDBValveCfg()
{
    _TableIndex = T_VALVECFG;
    Reload();
}

CDBValveCfg::~CDBValveCfg()
{
    Empty();
}

void CDBValveCfg::Empty()
{
    _V1Torq.clear();
    _V1Press.clear();
    _V2Torq.clear();
    _V2Flow.clear();
}

void CDBValveCfg::GetTable()
{
    int row = 0, col = 0;
    char** pResult;
    UINT nIndex = 0;

    if (!_Sqlite->GetTable(g_tTableName[_TableIndex], row, col, &pResult))
        return;
    if (row <= 0)
    {
        _Sqlite->FreeResult(&pResult);
        return;
    }

    _Count = row;

    nIndex = col;

    _Sqlite->GetValue(pResult[nIndex++], _V1Torq);
    _Sqlite->GetValue(pResult[nIndex++], _V1Press);
    _Sqlite->GetValue(pResult[nIndex++], _V2Torq);
    _Sqlite->GetValue(pResult[nIndex++], _V2Flow);

    _Sqlite->FreeResult(&pResult);
}

bool CDBValveCfg::UpdateGlbCfg(VALVECFG* ptCfg)
{
    vector<string> fields;
    vector<string> values;
    string value;

    COMP_BFALSE_R(_ValidDB, false);
    ASSERT_NULL_R(ptCfg, false);

    value = GetListFromArray(&ptCfg->ucTorq[0][0], VALVERATIONUM);
    fields.push_back("V1Torq");
    values.push_back(value);
    value = GetListFromArray(&ptCfg->ucRatio[0][0], VALVERATIONUM);
    fields.push_back("V1Press");
    values.push_back(value);

    value = GetListFromArray(&ptCfg->ucTorq[1][0], VALVERATIONUM);
    fields.push_back("V2Torq");
    values.push_back(value);
    value = GetListFromArray(&ptCfg->ucRatio[1][0], VALVERATIONUM);
    fields.push_back("V2Flow");
    values.push_back(value);

    return _Sqlite->UpdateFields(g_tTableName[_TableIndex], NULLSTR, fields, values);
}


bool CDBValveCfg::GetValues(string strVal, BYTE* ptAddr)
{
    int i = 0;
    vector<int> values;
    ASSERT_NULL_R(ptAddr, false);
    if (strVal.empty())
        return false;

    values = GetIDFromList(strVal);
    if (values.size() != VALVERATIONUM)
        return false;

    for (i = 0; i < VALVERATIONUM; i++)
    {
        ptAddr[i] = (BYTE)values[i];
    }
    return true;
}

bool CDBValveCfg::GetGlbCfg(VALVECFG* ptCfg)
{
    bool bRes = false;

    COMP_BFALSE_R(_ValidDB, FALSE);
    ASSERT_NULL_R(ptCfg, FALSE);

    bRes = GetValues(_V1Torq, &ptCfg->ucTorq[0][0]);
    bRes &= GetValues(_V1Press, &ptCfg->ucRatio[0][0]);
    bRes &= GetValues(_V2Torq, &ptCfg->ucTorq[1][0]);
    bRes &= GetValues(_V2Flow, &ptCfg->ucRatio[1][0]);
    return bRes;
}