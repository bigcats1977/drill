#include "DBTorqueCfg.h"

CDBTorqueCfg::CDBTorqueCfg()
{
    _TableIndex = T_TORQUECFG;

    GetTable();
}

CDBTorqueCfg::~CDBTorqueCfg()
{
    Empty();
}

void CDBTorqueCfg::Empty()
{
    _lsAutoIndex.clear();
    _lsAlias.clear();
    _lsLangType.clear();
    _lsTorqueID.clear();
    _lsTurnID.clear();
    _lsFullRPM.clear();
    //_lsMinShlSlope.clear();
    _lsSingleSTD.clear();

    _lsOptionID.clear();
    _lsComment.clear();
}

void CDBTorqueCfg::GetTable()
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

    for (m = 0; m < row; m++)
    {
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsAutoIndex.push_back(Value);
        _Sqlite->GetValue(pResult[nIndex++], Content);
        _lsAlias.push_back(Content);
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsLangType.push_back(Value);
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsTorqueID.push_back(Value);
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsTurnID.push_back(Value);
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsFullRPM.push_back(Value);
        //_Sqlite->GetValue(pResult[nIndex++], fVal);
        //_lsMinShlSlope.push_back(fVal);
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsSingleSTD.push_back(Value);

        _Sqlite->GetValue(pResult[nIndex++], Content);
        _lsOptionID.push_back(Content);
        _Sqlite->GetValue(pResult[nIndex++], Content);
        _lsComment.push_back(Content);
    }

    _Sqlite->FreeResult(&pResult);
}

bool CDBTorqueCfg::GetParaCfgById(int index, PARACFG* ptCfg, TORQCFGID* ptID)
{
    int iOffset = 0;
    vector<int>::iterator it;
    CONTROLPARA* ptCtrl = NULL;

    COMP_BFALSE_R(Valid(), false);
    ASSERT_NULL_R(ptCfg, false);
    ASSERT_NULL_R(ptID, false);

    it = find(_lsAutoIndex.begin(), _lsAutoIndex.end(), index);
    if (it == _lsAutoIndex.end())
        return false;

    iOffset = it - _lsAutoIndex.begin();
    ptCtrl = &ptCfg->tCtrl;

    ptCfg->strAlias = _lsAlias[iOffset];
    ptID->nTorqueID = _lsTorqueID[iOffset];
    ptID->nTurnID = _lsTurnID[iOffset];

    ptCtrl->fFullRPM = _lsFullRPM[iOffset];
    //ptCtrl->fMinShlSlope = _lsMinShlSlope[iOffset];
    ptCtrl->iSingleSTD = _lsSingleSTD[iOffset];

    ptID->strOptionID = _lsOptionID[iOffset];
    ptCfg->strMemo = _lsComment[iOffset];

    return true;
}

int CDBTorqueCfg::UpdateParaByAlias(PARACFG* ptCfg, TORQCFGID* ptID)
{
    bool bInsert = false;
    string strCond;
    vector<string> strFields;
    vector<string> strValues;
    vector<string>::iterator it;

    COMP_BFALSE_R(Valid(), DB_INVALID_VAL);
    ASSERT_NULL_R(ptCfg, DB_INVALID_VAL);
    ASSERT_NULL_R(ptID, DB_INVALID_VAL);
    if (ptCfg->strAlias.size() <= 0)
        return DB_INVALID_VAL;

    it = find(_lsAlias.begin(), _lsAlias.end(), ptCfg->strAlias);
    if (it == _lsAlias.end())
        bInsert = TRUE;

    if (bInsert)
    {
        strFields.push_back("Alias");
        strValues.push_back(ptCfg->strAlias);
    }
    strFields.push_back("LangType");
    strValues.push_back(to_string(*_CurLang));
    strFields.push_back("TorqueID");
    strValues.push_back(to_string(ptID->nTorqueID));
    strFields.push_back("TurnID");
    strValues.push_back(to_string(ptID->nTurnID));
    strFields.push_back("FullRPM");
    strValues.push_back(to_string(ptCfg->tCtrl.fFullRPM));
    //strFields.push_back("MinShlSlope");
    //strValues.push_back(to_string(ptCfg->tCtrl.fMinShlSlope));
    strFields.push_back("SingleSTD");
    strValues.push_back(to_string(ptCfg->tCtrl.iSingleSTD));
    strFields.push_back("OptionID");
    strValues.push_back(ptID->strOptionID);
    strFields.push_back("Comment");
    strValues.push_back(ptCfg->strMemo);

    if (bInsert)
    {
        strValues.insert(strValues.begin(), NULLSTR);

        if (!_Sqlite->InsertRow(g_tTableName[_TableIndex], strValues))
            return DB_INVALID_VAL;

        return _Sqlite->GetLastInsertID();
    }
    else
    {
        strCond = "Alias='" + ptCfg->strAlias + "'";

        if (!_Sqlite->UpdateFields(g_tTableName[_TableIndex], strCond, strFields, strValues))
            return DB_INVALID_VAL;

        return _lsAutoIndex[it - _lsAlias.begin()];
    }
}

int CDBTorqueCfg::GetIndexByAlias(string alias)
{
    vector<string>::iterator it;

    if (alias.size() <= 0)
        return DB_INVALID_VAL;

    it = find(_lsAlias.begin(), _lsAlias.end(), alias);
    if (it == _lsAlias.end())
        return DB_INVALID_VAL;

    return _lsAutoIndex[it - _lsAlias.begin()];
}

int CDBTorqueCfg::GetParaCfgByAlias(string alias, PARACFG* ptCfg, TORQCFGID* ptID)
{
    int index = 0;
    vector<string>::iterator it;

    if (alias.size() <= 0)
        return DB_INVALID_VAL;

    it = find(_lsAlias.begin(), _lsAlias.end(), alias);
    if (it == _lsAlias.end())
        return DB_INVALID_VAL;

    index = _lsAutoIndex[it - _lsAlias.begin()];
    if (!GetParaCfgById(index, ptCfg, ptID))
        return DB_INVALID_VAL;

    return index;
}

bool CDBTorqueCfg::DeleteParaCfgByID(int Index)
{
    string strCond;

    COMP_BFALSE_R(Valid(), false);
    COMP_BLE_R(Index, 0, false);

    strCond = "AutoIndex=" + to_string(Index) + " And LangType = " + to_string(*_CurLang);
    return _Sqlite->DeleteRow(g_tTableName[_TableIndex], strCond);
}
