#include "DBWITSCfg.h"

CDBWITSCfg::CDBWITSCfg()
{
    _TableIndex = T_WITSCFG;
    GetTable();
}

CDBWITSCfg::~CDBWITSCfg()
{
    Empty();
}

void CDBWITSCfg::Empty()
{
    //_TCPPort = 9600;
    _ShowParas.clear();
    _FixItems.clear();
    _RepeatItems.clear();
    _CalItems.clear();
    _ShowItems.clear();
}

void CDBWITSCfg::GetTable()
{
    int row = 0, col = 0;
    char** pResult;
    string lsItem;
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

    //_Sqlite->GetValue(pResult[nIndex++], _TCPPort);
    _Sqlite->GetValue(pResult[nIndex++], lsItem);
    _ShowParas = GetIDFromList(lsItem);
    _Sqlite->GetValue(pResult[nIndex++], lsItem);
    _FixItems = GetIDFromList(lsItem);
    _Sqlite->GetValue(pResult[nIndex++], lsItem);
    _RepeatItems = GetIDFromList(lsItem);
    _Sqlite->GetValue(pResult[nIndex++], lsItem);
    _CalItems = GetIDFromList(lsItem);
    _Sqlite->GetValue(pResult[nIndex++], lsItem);
    _ShowItems = GetIDFromList(lsItem);

    _Sqlite->FreeResult(&pResult);
}

BOOL CDBWITSCfg::UpdateWITSCfg(WITSCFG* ptfg)
{
    BOOL bRes = FALSE;
    vector<string> fields;
    vector<string> values;

    COMP_BFALSE_R(Valid(), FALSE);
    ASSERT_NULL_R(ptfg, FALSE);

    /*fields.push_back("TCPPort");
    values.push_back(to_string(ptfg->nTCPPort));*/
    fields.push_back("ShowParas");
    values.push_back(GetListFromVector(ptfg->ShowParas));
    fields.push_back("FixItems");
    values.push_back(GetListFromVector(ptfg->FixItems));
    fields.push_back("RepeatItems");
    values.push_back(GetListFromVector(ptfg->RepeatItems));
    fields.push_back("CalItems");
    values.push_back(GetListFromVector(ptfg->CalItems));
    fields.push_back("ShowItems");
    values.push_back(GetListFromVector(ptfg->ShowItems));

    return _Sqlite->UpdateFields(g_tTableName[_TableIndex], NULLSTR, fields, values);
}
