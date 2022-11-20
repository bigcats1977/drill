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
    _GenPara.clear();
    _JobPara.clear();
    _InfoPara.clear();
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
        _Sqlite->GetValue(pResult[nIndex++], Content);
        _GenPara.push_back(Content);
        _Sqlite->GetValue(pResult[nIndex++], Content);
        _JobPara.push_back(Content);
        _Sqlite->GetValue(pResult[nIndex++], Content);
        _InfoPara.push_back(Content);
    }

    _Sqlite->FreeResult(&pResult);
}

bool CDBXlsStatCfg::SetItemValue(vector<int> lsVals, int* pVals, int num)
{
    ASSERT_NULL_R(pVals, false);
    if (lsVals.size() != num)
        return false;
    for (int i = 0; i < num; i++)
        pVals[i] = lsVals[i];
    return true;
}

bool CDBXlsStatCfg::GetInfoByLang(XLSSTATCFG *ptCfg, UINT nLang)
{
    vector<int> lsVals;
    vector<int>::iterator it;
    int iOffset = 0;

    COMP_BFALSE_R(_ValidDB, false);
    ASSERT_NULL_R(ptCfg, false);
    CheckLanguage(nLang);

    it = find(_lsLangType.begin(), _lsLangType.end(), nLang);
    if (it == _lsLangType.end())
        return false;

    iOffset = it - _lsLangType.begin();

    lsVals = GetIDFromList(_GenPara[iOffset]);
    if (!SetItemValue(lsVals, &ptCfg->GenPara[0], STATPARA_GENNUM))
        return false;
    lsVals = GetIDFromList(_JobPara[iOffset]);
    if (!SetItemValue(lsVals, &ptCfg->JobPara[0], STATPARA_JOBNUM))
        return false;
    lsVals = GetIDFromList(_InfoPara[iOffset]);
    if (!SetItemValue(lsVals, &ptCfg->InfoPara[0], STATPARA_INFONUM))
        return false;

    return true;
}
BOOL CDBXlsStatCfg::UpdateInfo(XLSSTATCFG* ptCfg)
{
    int i = 0;
    string condition;
    string lsCheck;
    vector<string> fields;
    vector<string> values;

    COMP_BFALSE_R(_ValidDB, false);
    ASSERT_NULL_R(ptCfg, false);

    condition = "LangType=" + to_string(*_CurLang);

    lsCheck = GetListFromArray(&ptCfg->GenPara[0], STATPARA_GENNUM);
    fields.push_back("GenPara");
    values.push_back(lsCheck);

    lsCheck = GetListFromArray(&ptCfg->JobPara[0], STATPARA_JOBNUM);
    fields.push_back("JobPara");
    values.push_back(lsCheck);

    lsCheck = GetListFromArray(&ptCfg->InfoPara[0], STATPARA_INFONUM);
    fields.push_back("InfoPara");
    values.push_back(lsCheck);

    return _Sqlite->UpdateFields(g_tTableName[_TableIndex], condition, fields, values);
}
