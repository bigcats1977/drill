// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "contourlevels.h"

// Dispatch interfaces referenced by this interface
#include "contourlevel.h"


/////////////////////////////////////////////////////////////////////////////
// CContourLevels properties

/////////////////////////////////////////////////////////////////////////////
// CContourLevels operations

CContourLevel CContourLevels::GetItems(long Index)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, parms,
		Index);
	return CContourLevel(pDispatch);
}
