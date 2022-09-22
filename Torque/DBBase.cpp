#include "DBBase.h"
#include "torque.h"


CDBBase::CDBBase()
{
    _Sqlite = &theDB.m_tSqlite;
    if (!theDB.m_bValidDBFile)
    {
        _ValidDB = FALSE;
        return;
    }
    _CurLang = &g_tGlbCfg.nLangType;
    _ValidDB = TRUE;
    //_Update = TRUE;

    Reload();
}

CDBBase::~CDBBase()
{
    Empty();
}

void CDBBase::Empty()
{
    _Count = -1;
}

void CDBBase::GetTable()
{
    COMP_BFALSE(_ValidDB);
}

void CDBBase::Reload()
{
    //COMP_BFALSE(_Update);

    Empty();
    GetTable();

    //_Update = FALSE;
}

BOOL CDBBase::Valid()
{
    return (_Count >= 0);
}


//void CDBBase::SetModified()
//{
//    _Update = TRUE;
//}

