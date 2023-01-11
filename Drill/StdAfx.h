// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__F3E0F30D_EE96_49EF_AD96_BE3F827D3F14__INCLUDED_)
#define AFX_STDAFX_H__F3E0F30D_EE96_49EF_AD96_BE3F827D3F14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef WINVER // Specifies that the minimum required platform is Windows Vista.   
#define WINVER 0x0601 // WIN7   
#endif   

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxdlgs.h>
#include <afxcontrolbars.h>
#include <afxdisp.h>
#include <afxcontrolbars.h>


#include "Global.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__F3E0F30D_EE96_49EF_AD96_BE3F827D3F14__INCLUDED_)