#pragma once

#include "LineChartCtrlEx.h"
#include "Ruler.h"

// CDlgZoomIn dialog

#define MINTORQRANGE    1000

class CDlgZoomIn : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgZoomIn)

public:
	CDlgZoomIn(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgZoomIn();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGZOOMIN };
#endif
    CRuler      m_xZoomAxis;
    CRuler      m_yZoomAxis;
    UINT        m_nPos; //src pos
    SPLITPOINT  m_tSplit;

    CLineChartCtrlEx    m_wndLineZoom;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    //afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg LRESULT SelPosChange(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
    void DrawZoomPnt();
    void DrawZoomLine(DRAWTORQDATA *ptDraw);
    void GetZoomRange(DRAWTORQDATA *ptDraw);
    void GetTorqueRange(DRAWTORQDATA *ptDraw);
    void AdjustShowCir(DRAWTORQDATA *ptDraw, double  fSrcMaxCir);

    int     m_iBegin, m_iEnd;
    int     m_iZoomPos;  /* 放大后位置 */
    UINT    m_nZoomTorq; /* 放大点扭矩 */
    double  m_fMinCir, m_fMaxCir;
    double  m_fMinTorq, m_fMaxTorq;

    UINT    m_nCurZoom;

    CPoint  m_pointDown;
    BOOL    m_bDragTitle;
};
