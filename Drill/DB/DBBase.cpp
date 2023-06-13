#include "DBBase.h"
#include "Drill.h"


CDBBase::CDBBase()
{
    _Sqlite = &theDB.m_tSqlite;
    /*if (!theDB.m_bValidDBFile)
    {
        Valid() = false;
        return;
    }*/
    _CurLang = &g_tGlbCfg.nLangType;
    /*Valid() = true;*/
    //_Update = TRUE;

    GetTable();
}

CDBBase::~CDBBase()
{
    Empty();
}

void CDBBase::Empty()
{
    _Count = 0;
}

void CDBBase::GetTable()
{
    _Count = 0;
    //Valid() = false;
    //COMP_BFALSE(Valid());
}

void CDBBase::Reload()
{
    //COMP_BFALSE(_Update);

    Empty();
    GetTable();

    //_Update = FALSE;
}

bool CDBBase::Valid()
{
    return (_Count > 0);
}


//void CDBBase::SetModified()
//{
//    _Update = TRUE;
//}

