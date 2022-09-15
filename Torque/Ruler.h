#if !defined(AFX_RULER_H__2DB50BEC_4601_43D7_8479_55F292120FED__INCLUDED_)
#define AFX_RULER_H__2DB50BEC_4601_43D7_8479_55F292120FED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Ruler.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRuler window

// ruler types
#define RT_VERTICAL                 0x00000000
#define RT_HORIZONTAL               0x00000001

#define HORNUM                      10
#define VORNUM                      20
#define MAX_SCALE_LEN               7
#define Med_SCALE_LEN               5
#define Min_SCALE_LEN               3
/* 一个大刻度里面包含小刻度数目 */
#define SCALE_NUM                   5

// hint information
#define VW_HSCROLL                  0x00000001
#define VW_VSCROLL                  0x00000002
#define VW_HPOSITION                0x00000003
#define VW_VPOSITION                0x00000004

class CRuler : public CStatic
{
// Construction
public:
    CRuler();

// Attributes
public:
    UINT        m_nRulerType;
    int         m_iTickNum;
    double      m_fTickValue;
    double      m_fMinTickVal;

// Operations
public:
    void SetRulerType(UINT nRulerType =RT_HORIZONTAL);
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CRuler)
    //}}AFX_VIRTUAL

// Implementation
public:
    void    SetTickPara(int iTickNum, double fMaxVal, double fMinVal = 0);
    void    SetXAxisDecNum(UINT nNum);
    void    InvalidateCtrl();
    virtual ~CRuler();

    // Generated message map functions
protected:
    //{{AFX_MSG(CRuler)
    afx_msg void OnPaint();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

protected:
    CBitmap     m_Bitmap;
    CDC         m_MemDC;
    //CFont       m_hFont;
    //CFont       m_vFont;
    void        DrawVerRuler();
    void        DrawHorRuler();
    void        DrawVerScale(int iBeginY, CRect rcRuler);
    void        DrawHorScale(int iBeginX, CRect rcRuler);

private:
    UINT        m_nXDecNum;         /* X坐标轴的小数位数 */
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RULER_H__2DB50BEC_4601_43D7_8479_55F292120FED__INCLUDED_)
