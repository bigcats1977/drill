// CellRange.h: interface for the CCellRange class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CELLRANGE_H__F86EF761_725A_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_CELLRANGE_H__F86EF761_725A_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Macro.h"
// The code contained in this file is based on the original
// WorldCom Grid control written by Joe Willcoxson,
//      mailto:chinajoe@aol.com
//      http://users.aol.com/chinajoe

class CCellID//仅仅提供一个最基本的单元格
{    
// Attributes
public:
    int  m_iRow;
    int  m_iCol;

// Operations
public:
    CCellID(int iRow = -1, int iCol = -1) : m_iRow(iRow), m_iCol(iCol) {}

    int  IsValid() const { return (m_iRow >= 0 && m_iCol >= 0); }
    int  operator==(const CCellID& rhs)    { return (m_iRow == rhs.m_iRow && m_iCol == rhs.m_iCol);}
    int  operator!=(const CCellID& rhs)    { return !operator==(rhs); }
};

class CCellRange//可以判断二个range是否相等并且可以求他们的相交区间
//判断一个单元格是否在此区间
{ 
public:
    CCellRange(int iMinRow = -1, int iMinCol = -1, int iMaxRow = -1, int iMaxCol = -1)
    {
        Set(iMinRow, iMinCol, iMaxRow, iMaxCol);
    }

    void Set(int iMinRow = -1, int iMinCol = -1, int iMaxRow = -1, int iMaxCol = -1);
    
    int  IsValid() const;
    int  InRange(int iRow, int iCol) const;
    int  InRange(const CCellID& cellID) const;
    int  Count() { return (m_iMaxRow - m_iMinRow + 1) * (m_iMaxCol - m_iMinCol + 1); }
    
    CCellID     GetTopLeft() const;
    CCellRange  Intersect(const CCellRange& rhs) const;
    
    int  GetMinRow() const {return m_iMinRow;}
    void SetMinRow(int iMinRow) {m_iMinRow = iMinRow;}
    
    int  GetMinCol() const {return m_iMinCol;}
    void SetMinCol(int iMinCol) {m_iMinCol = iMinCol;}
    
    int  GetMaxRow() const {return m_iMaxRow;}
    void SetMaxRow(int iMaxRow) {m_iMaxRow = iMaxRow;}
    
    int  GetMaxCol() const {return m_iMaxCol;}
    void SetMaxCol(int iMaxCol) {m_iMaxCol = iMaxCol;}

    int  GetRowSpan() const {return m_iMaxRow - m_iMinRow + 1;}
    int  GetColSpan() const {return m_iMaxCol - m_iMinCol + 1;}
    
    int  operator==(const CCellRange& rhs);
    int  operator!=(const CCellRange& rhs);
    
protected:
    int  m_iMinRow;
    int  m_iMinCol;
    int  m_iMaxRow;
    int  m_iMaxCol;
};

inline void CCellRange::Set(int iMinRow, int iMinCol, int iMaxRow, int iMaxCol)
{
     m_iMinRow = iMinRow;
     m_iMinCol = iMinCol;
     m_iMaxRow = iMaxRow;
     m_iMaxCol = iMaxCol;
}

inline int CCellRange::operator==(const CCellRange& rhs)
{
     return ((m_iMinRow == rhs.m_iMinRow) &&
             (m_iMinCol == rhs.m_iMinCol) &&
             (m_iMaxRow == rhs.m_iMaxRow) &&
             (m_iMaxCol == rhs.m_iMaxCol));
}

inline int CCellRange::operator!=(const CCellRange& rhs)
{
     return !operator==(rhs);
}

inline int CCellRange::IsValid() const
{
     return (m_iMinRow >= 0 &&
             m_iMinCol >= 0 &&
             m_iMaxRow >= 0 &&
             m_iMaxCol >= 0 &&
             m_iMinRow <= m_iMaxRow &&
             m_iMinCol <= m_iMaxCol);
}

inline int CCellRange::InRange(int iRow, int iCol) const
{
     return (iRow >= m_iMinRow &&
             iRow <= m_iMaxRow &&
             iCol >= m_iMinCol &&
             iCol <= m_iMaxCol);
}

inline int CCellRange::InRange(const CCellID& ciCell) const
{
     return InRange(ciCell.m_iRow, ciCell.m_iCol);
}

inline CCellID CCellRange::GetTopLeft() const
{
     return CCellID(m_iMinRow, m_iMinCol);
}

inline CCellRange CCellRange::Intersect(const CCellRange& rhs) const
{
     return CCellRange(max(m_iMinRow,rhs.m_iMinRow),
                       max(m_iMinCol,rhs.m_iMinCol),
                       min(m_iMaxRow,rhs.m_iMaxRow),
                       min(m_iMaxCol,rhs.m_iMaxCol));
}

#endif // !defined(AFX_CELLRANGE_H__F86EF761_725A_11D1_ABBA_00A0243D1382__INCLUDED_)
