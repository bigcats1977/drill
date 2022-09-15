#if !defined(AFX_DRAWLINELIST_H__B6959A17_D2D9_48D2_8B67_5DC05824A4EF__INCLUDED_)
#define AFX_DRAWLINELIST_H__B6959A17_D2D9_48D2_8B67_5DC05824A4EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CDrawLine;

/////////////////////////////////////////////////////////////////////////////
// CDrawLineList wrapper class

class CDrawLineList : public COleDispatchDriver
{
public:
	CDrawLineList() {}		// Calls COleDispatchDriver default constructor
	CDrawLineList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDrawLineList(const CDrawLineList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CDrawLine GetItems(long Index);
	VARIANT GetControlDefault(long Index);
	void Clear();
	void Delete(long Index);
	long GetCount();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWLINELIST_H__B6959A17_D2D9_48D2_8B67_5DC05824A4EF__INCLUDED_)
