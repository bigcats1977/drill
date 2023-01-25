// English.h : main header file for the English DLL
//

#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CEnglishApp
// See English.cpp for the implementation of this class
//

class CEnglishApp : public CWinApp
{
public:
    CEnglishApp();

    // Overrides
public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};
