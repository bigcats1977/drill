#pragma once

#include "LineChartCtrlStat.h"
#include "Ruler.h"

// CDlgScatter 对话框

class CDlgScatter : public CDialogEx
{
    DECLARE_DYNAMIC(CDlgScatter)

public:
    CDlgScatter(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~CDlgScatter();
    void    DrawScatterPlot(void);
    CString SaveScatterImg(void);

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DLGSCATTER };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()

protected:
    virtual BOOL OnInitDialog();

    CRuler              m_xScatAxis;
    CRuler              m_yScatAxis;
    CLineChartCtrlStat  m_wndLineScat;

private:
    void ResetScatLine(PARACFG* ptCfg);
    void ResetScatLineByData(TorqData::Torque* ptTorq);

    double              m_fFullCir;
    double              m_fFullTorq;
};
