#pragma once
#include <afxwin.h>
class CNumEdit :
    public CEdit
{
public:
    CNumEdit();
    ~CNumEdit();

    virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
    BOOL CheckUnique(char nChar);

};

