#include "DBGlbCfg.h"

CDBGlbCfg::CDBGlbCfg()
{
    _TableIndex = T_GLBCFG;
    Reload();
}

CDBGlbCfg::~CDBGlbCfg()
{
    Empty();
}

void CDBGlbCfg::Empty()
{
    _LangType = 0;
    _PortNO = 1;
    _BaudRate = 115200;
    _PlusPerTurn = 4500;
    _TorqUnit = 0;
    _CollectDur = 100;
    _ResetDur = 10;
    _SaveDur = 30;
    _IPShowMode = 1;
    _ZoomIn = 5;
    _ImgNum = 8;
    //_Test = 0;

    _Discount = 1;
    _Multi = 1;
    _RpmAdj = 3.5;
    _IPDeltaVal = 0.1;

    _CheckIP = 1;
    _BigTorq = 0;
    _Shackle = 0;
    _DateBehind = 0;

    _Password.clear();
    _DataPath.clear();
}

void CDBGlbCfg::GetTable()
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

    _Sqlite->GetValue(pResult[nIndex++], _LangType);
    _Sqlite->GetValue(pResult[nIndex++], _PortNO);
    _Sqlite->GetValue(pResult[nIndex++], _BaudRate);
    _Sqlite->GetValue(pResult[nIndex++], _PlusPerTurn);
    _Sqlite->GetValue(pResult[nIndex++], _TorqUnit);
    _Sqlite->GetValue(pResult[nIndex++], _CollectDur);
    _Sqlite->GetValue(pResult[nIndex++], _ResetDur);
    _Sqlite->GetValue(pResult[nIndex++], _SaveDur);
    _Sqlite->GetValue(pResult[nIndex++], _IPShowMode);
    _Sqlite->GetValue(pResult[nIndex++], _ZoomIn);
    _Sqlite->GetValue(pResult[nIndex++], _ImgNum);
    //_Sqlite->GetValue(pResult[nIndex++], _Test);

    _Sqlite->GetValue(pResult[nIndex++], _Discount);
    _Sqlite->GetValue(pResult[nIndex++], _Multi);
    _Sqlite->GetValue(pResult[nIndex++], _RpmAdj);
    _Sqlite->GetValue(pResult[nIndex++], _IPDeltaVal);

    _Sqlite->GetValue(pResult[nIndex++], _CheckIP);
    _Sqlite->GetValue(pResult[nIndex++], _BigTorq);
    _Sqlite->GetValue(pResult[nIndex++], _Shackle);
    _Sqlite->GetValue(pResult[nIndex++], _DateBehind);

    _Sqlite->GetValue(pResult[nIndex++], _Password);
    _Sqlite->GetValue(pResult[nIndex++], _DataPath);

    _Sqlite->FreeResult(&pResult);
}

BOOL CDBGlbCfg::UpdateGlbCfg(GLBCFG *ptfg)
{
    BOOL bRes = FALSE;
    vector<string> fields;
    vector<string> values;

    COMP_BFALSE_R(_ValidDB, FALSE);
    ASSERT_NULL_R(ptfg, FALSE);
    
    fields.push_back("LangType");
    values.push_back(to_string(ptfg->nLangType));
    fields.push_back("PortNO");
    values.push_back(to_string(ptfg->nPortNO));
    fields.push_back("BaudRate");
    values.push_back(to_string(ptfg->nBaudRate));
    fields.push_back("PlusPerTurn");
    values.push_back(to_string(ptfg->nPlusPerTurn));
    fields.push_back("TorqUnit");
    values.push_back(to_string(ptfg->nTorqUnit));
    fields.push_back("CollectDur");
    values.push_back(to_string(ptfg->nCollectDur));
    fields.push_back("ResetDur");
    values.push_back(to_string(ptfg->nResetDur));
    fields.push_back("SaveDur");
    values.push_back(to_string(ptfg->nSaveDur));
    fields.push_back("IPShowMode");
    values.push_back(to_string(ptfg->nIPShowMode));
    fields.push_back("ZoomIn");
    values.push_back(to_string(ptfg->nZoomIn));
    fields.push_back("ImgNum");
    values.push_back(to_string(ptfg->nImgNum));
    fields.push_back("Test");
    values.push_back(to_string(ptfg->nTest));

    fields.push_back("Discount");
    values.push_back(to_string(ptfg->fDiscount));
    fields.push_back("Multi");
    values.push_back(to_string(ptfg->fMulti));
    fields.push_back("RpmAdj");
    values.push_back(to_string(ptfg->fRpmAdj));
    fields.push_back("IPDeltaVal");
    values.push_back(to_string(ptfg->fIPDeltaVal));

    fields.push_back("CheckIP");
    values.push_back(to_string(ptfg->bCheckIP));
    fields.push_back("BigTorq");
    values.push_back(to_string(ptfg->bBigTorq));
    fields.push_back("Shackle");
    values.push_back(to_string(ptfg->bShackle));
    fields.push_back("DateBehind");
    values.push_back(to_string(ptfg->bDateBehind));

    fields.push_back("Password");
    values.push_back(ptfg->strPassWord);
    fields.push_back("DataPath");
    values.push_back(ptfg->strDataPath);

    return _Sqlite->UpdateFields(g_tTableName[_TableIndex], NULLSTR, fields, values);
}