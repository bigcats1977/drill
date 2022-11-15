#include "DBXlsStatCfg.h"

CDBXlsStatCfg::CDBXlsStatCfg()
{
    _TableIndex = T_XLSSTATCFG;

    Reload();
}

CDBXlsStatCfg::~CDBXlsStatCfg()
{
    Empty();
}

void CDBXlsStatCfg::Empty()
{
    _lsLangType.clear();
    _lsOperator.clear();
    _lsTally.clear();
    _lsSummary.clear();
    _lsReport.clear();
}

void CDBXlsStatCfg::GetTable()
{
    int row = 0, col = 0;
    char** pResult;
    UINT nIndex = 0;
    int m = 0, n = 0;
    int Value = 0;
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
        _lsLangType.push_back(Value);
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsOperator.push_back(Value);
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsTally.push_back(Value);


        _Sqlite->GetValue(pResult[nIndex++], Content);
        _lsSummary.push_back(Content);
        _Sqlite->GetValue(pResult[nIndex++], Content);
        _lsReport.push_back(Content);
    }

    _Sqlite->FreeResult(&pResult);
}


BOOL CDBXlsStatCfg::SetItemValue(vector<int> lsVals, int type, BOOL* pVals)
{
    int i = 0;
    int count = 0;
    ASSERT_NULL_R(pVals, FALSE);

    if (lsVals.size() != MAXPARANUM)
        return FALSE;
    for (i = 0; i < MAXPARANUM; i++)
    {
        if (lsVals[i] == 0)
            pVals[i] = FALSE;
        else
        {
            count++;
            pVals[i] = TRUE;
        }
    }

    switch (type)
    {
    case 0:
        if (count > MAXSUMMARYPARA)
            return FALSE;
        break;
    case 1:
        if (count > MAXREPORTPARA)
            return FALSE;
        break;
    default:
        break;
    }

    return TRUE;
}

BOOL CDBXlsStatCfg::GetInfoByLang(XLSSTATCFG *ptCfg, UINT nLang)
{
    vector<int> lsVals;
    vector<int>::iterator it;
    int iOffset = 0;

    COMP_BFALSE_R(_ValidDB, FALSE);
    ASSERT_NULL_R(ptCfg, FALSE);
    CheckLanguage(nLang);

    it = find(_lsLangType.begin(), _lsLangType.end(), nLang);
    if (it == _lsLangType.end())
        return FALSE;

    iOffset = it - _lsLangType.begin();

    lsVals = GetIDFromList(_lsSummary[iOffset]);
    if (!SetItemValue(lsVals, 0, &ptCfg->bSummary[0]))
        return FALSE;
    lsVals = GetIDFromList(_lsReport[iOffset]);
    if (!SetItemValue(lsVals, 1, &ptCfg->bReport[0]))
        return FALSE;

    ptCfg->ucOperator = _lsOperator[iOffset];
    ptCfg->ucTally = _lsTally[iOffset];

    return TRUE;
}
BOOL CDBXlsStatCfg::UpdateInfo(XLSSTATCFG* ptCfg)
{
    int i = 0;
    string condition;
    string lsCheck;
    vector<string> fields;
    vector<string> values;

    COMP_BFALSE_R(_ValidDB, FALSE);
    ASSERT_NULL_R(ptCfg, FALSE);

    condition = "LangType=" + to_string(*_CurLang);

    fields.push_back("Operator");
    values.push_back(to_string(ptCfg->ucOperator));
    fields.push_back("Tally");
    values.push_back(to_string(ptCfg->ucTally));

    lsCheck = GetListFromArray(&ptCfg->bSummary[0], MAXPARANUM);
    fields.push_back("Summary");
    values.push_back(lsCheck);

    lsCheck = GetListFromArray(&ptCfg->bReport[0], MAXPARANUM);
    fields.push_back("Report");
    values.push_back(lsCheck);

    return _Sqlite->UpdateFields(g_tTableName[_TableIndex], condition, fields, values);
}
