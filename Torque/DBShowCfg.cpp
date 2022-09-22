#include "DBShowCfg.h"

CDBShowCfg::CDBShowCfg()
{
    _TableIndex = T_SHOWCFG;

    Reload();
}

CDBShowCfg::~CDBShowCfg()
{
    Empty();

    //_lsFixTube.clear();
}

void CDBShowCfg::Empty()
{
    _lsLangType.clear();
    _lsParaNum.clear();
    _lsListNum.clear();
    _lsMainNum.clear();
    _lsFileName.clear();
    _lsStatType.clear();
    //_lsIsFixTube.clear();
    //_lsValParaID.clear();
    _lsAliasID.clear();

    _lsShowPara.clear();
    _lsListNO.clear();
    _lsMainNO.clear();
    //_lsShowValue.clear();

    //_lsFixTube.clear();
    //_lsFixTube.resize(MAXTUBECFGNUM);
    /*_lsFactory.clear();
    _lsOEM.clear();
    _lsSize.clear();
    _lsMaterial.clear();
    _lsCoupling.clear();*/
}

void CDBShowCfg::GetTable()
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

    //_lsFixTube.resize(_Count);
    for (m = 0; m < row; m++)
    {
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsLangType.push_back(Value);
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsParaNum.push_back(Value);
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsListNum.push_back(Value);
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsMainNum.push_back(Value);
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsFileName.push_back(Value);
        _Sqlite->GetValue(pResult[nIndex++], Value);
        _lsStatType.push_back(Value);
        //_Sqlite->GetValue(pResult[nIndex++], Value);
        //_lsIsFixTube.push_back(Value);

        if (!_Sqlite->GetValue(pResult[nIndex++], Value))
            Value = 0;
        _lsAliasID.push_back(Value);

        _Sqlite->GetValue(pResult[nIndex++], Content);
        _lsShowPara.push_back(Content);
        _Sqlite->GetValue(pResult[nIndex++], Content);
        _lsListNO.push_back(Content);
        _Sqlite->GetValue(pResult[nIndex++], Content);
        _lsMainNO.push_back(Content);
        //_Sqlite->GetValue(pResult[nIndex++], Content);
        //_lsShowValue.push_back(Content);
        
        /*for (n = 0; n < MAXTUBECFGNUM; n++)
        {
            _Sqlite->GetValue(pResult[nIndex++], Value);
            _lsFixTube[m].push_back(Value);
        }*/
    }

    _Sqlite->FreeResult(&pResult);
}

BOOL CDBShowCfg::UpdateShowCfg(SHOWCFG* ptShow)
{
    int i = 0;
    string condition;
    string lsNO;
    vector<string> fields;
    vector<string> values;

    COMP_BFALSE_R(_ValidDB, FALSE);
    ASSERT_NULL_R(ptShow, FALSE);

    condition = "LangType=" + to_string(*_CurLang);

    fields.push_back("ParaNum");
    values.push_back(to_string(ptShow->nParaNum));
    fields.push_back("ListNum");
    values.push_back(to_string(ptShow->nListNum));
    fields.push_back("MainNum");
    values.push_back(to_string(ptShow->nMainNum));
    fields.push_back("FileName");
    values.push_back(to_string(ptShow->nFileName));
    fields.push_back("StatType");
    values.push_back(to_string(ptShow->nStatType));

    fields.push_back("AliasID");
    if(ptShow->nAlias == 0)
        values.push_back(NULLSTR);
    else
        values.push_back(to_string(ptShow->nAlias));

    lsNO = GetListFromArray(&ptShow->nShow[0], ptShow->nParaNum);
    fields.push_back("ShowParas");
    values.push_back(lsNO);

    lsNO = GetListFromArray(&ptShow->nList[0], ptShow->nListNum);
    fields.push_back("ListNOs");
    values.push_back(lsNO);

    lsNO = GetListFromArray(&ptShow->nMain[0], ptShow->nMainNum);
    fields.push_back("MainNOs");
    values.push_back(lsNO);

    return _Sqlite->UpdateFields(g_tTableName[_TableIndex], condition, fields, values);
}

BOOL CDBShowCfg::UpdateAlias(UINT Alias)
{
    string condition;
    vector<string> fields;
    vector<string> values;

    COMP_BFALSE_R(_ValidDB, FALSE);

    condition = "LangType=" + to_string(*_CurLang);


    fields.push_back("AliasID");
    if (Alias == 0)
        values.push_back(NULLSTR);
    else
        values.push_back(to_string(Alias));

    return _Sqlite->UpdateFields(g_tTableName[_TableIndex], condition, fields, values);
}
#if 0
BOOL CDBShowCfg::UpdateShowValue(string curOption, SHOWCFG* ptShow)
{
    int i = 0;
    string condition;
    vector<string> fields;
    vector<string> values;

    COMP_BFALSE_R(_ValidDB, FALSE);
    ASSERT_NULL_R(ptShow, FALSE);

    condition = "LangType=" + to_string(*_CurLang);

    fields.push_back("IsFixTube");
    values.push_back(to_string(ptShow->bFixTube));

    fields.push_back("ValParaID");
    values.push_back(to_string(ptShow->iValParaID));

    if (curOption.size() > 0)
    {
        fields.push_back("ShowValues");
        values.push_back(curOption);
    }
    if (ptShow->bFixTube)
    {
        for (i = 0; i < MAXTUBECFGNUM; i++)
        {
            fields.push_back("FixTube" + to_string(i+1));
            values.push_back(to_string(ptShow->tTubeCfg.nFixTube[i]));
        }
    }

    return _Sqlite->UpdateFields(g_tTableName[_TableIndex], condition, fields, values);
}
#endif