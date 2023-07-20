#include "DBServerCfg.h"

CDBServerCfg::CDBServerCfg()
{
    _TableIndex = T_SERVERCFG;
    GetTable();
}

CDBServerCfg::~CDBServerCfg()
{
    Empty();
}

void CDBServerCfg::Empty()
{
    _FTPPort = 21;
    _TCPPort = 9600;
    _FTPAddr.clear();
    _UserName.clear();
    _Password.clear();
    _TargetPath.clear();
}

void CDBServerCfg::GetTable()
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

    _Sqlite->GetValue(pResult[nIndex++], _FTPAddr);
    _Sqlite->GetValue(pResult[nIndex++], _FTPPort);
    _Sqlite->GetValue(pResult[nIndex++], _UserName);
    _Sqlite->GetValue(pResult[nIndex++], _Password);
    _Sqlite->GetValue(pResult[nIndex++], _TargetPath);
    _Sqlite->GetValue(pResult[nIndex++], _TCPPort);

    _Sqlite->FreeResult(&pResult);
}

BOOL CDBServerCfg::UpdateServerCfg(SERVERCFG* ptfg)
{
    BOOL bRes = FALSE;
    vector<string> fields;
    vector<string> values;

    COMP_BFALSE_R(Valid(), FALSE);
    ASSERT_NULL_R(ptfg, FALSE);

    fields.push_back("FTPAddr");
    values.push_back(ptfg->strFTPAddr);
    fields.push_back("FTPPort");
    values.push_back(to_string(ptfg->nFTPPort));
    fields.push_back("UserName");
    values.push_back(ptfg->strUserName);
    fields.push_back("Password");
    values.push_back(ptfg->strPassword);
    fields.push_back("TargetPath");
    values.push_back(ptfg->strTargetPath);
    fields.push_back("TCPPort");
    values.push_back(to_string(ptfg->nTCPPort));

    return _Sqlite->UpdateFields(g_tTableName[_TableIndex], NULLSTR, fields, values);
}
