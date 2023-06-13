#pragma once

#include "stdafx.h"
#include "SqliteProc.h"


class CDBBase
{
public:
    CDBBase();
    ~CDBBase();

public:
    int   _Count;

    bool Valid();
    void Reload();

protected:
    UINT        _TableIndex;

    //bool        Valid();
    SqliteProc* _Sqlite;
    UINT* _CurLang;

    virtual void Empty();
    virtual void GetTable();
    //void SetModified();

private:
    //BOOL        _Update;
};

