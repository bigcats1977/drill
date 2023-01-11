#pragma once

#include "LineChartCtrlStat.h"
#include "Ruler.h"

// CDlgScatter �Ի���

class CDlgScatter : public CDialogEx
{
    DECLARE_DYNAMIC(CDlgScatter)

public:
    CDlgScatter(CWnd* pParent = nullptr);   // ��׼���캯��
    virtual ~CDlgScatter();
    void    DrawScatterPlot(void);
    CString SaveScatterImg(void);

    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DLGSCATTER };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
