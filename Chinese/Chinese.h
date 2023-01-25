// Chinese.h : main header file for the Chinese DLL
//

#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CChineseApp
// See Chinese.cpp for the implementation of this class
//

class CChineseApp : public CWinApp
{
public:
    CChineseApp();

    // Overrides
public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};
