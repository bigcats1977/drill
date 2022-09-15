// Russian.h : main header file for the Russian DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CRussianApp
// See Russian.cpp for the implementation of this class
//

class CRussianApp : public CWinApp
{
public:
	CRussianApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
