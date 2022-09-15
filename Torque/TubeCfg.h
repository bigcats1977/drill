#pragma once

#include "afxwin.h"
#include "Torque.h"


class CTubeCfg
{
public:
    CTubeCfg();
    ~CTubeCfg(); 

    FIXTUBINFO  m_tFactory;
    FIXTUBINFO  m_tOEM;
    FIXTUBINFO  m_tSize;
    FIXTUBINFO  m_tMater;
    FIXTUBINFO  m_tCoupl;
    AllTUBCFG   m_tAllTub;
   
    void SetFactoryBox(CComboBox *ptCCB, TUBINGCFG *ptTubing, UINT nLang);
    void SetOEMBox(CComboBox *ptCCB, TUBINGCFG *ptTubing, UINT nLang);
    void SetSizeBox(CComboBox *ptCCB, TUBINGCFG *ptTubing, UINT nLang);
    void SetMatBox(CComboBox *ptCCB, TUBINGCFG *ptTubing, UINT nLang);
    void SetCouplBox(CComboBox *ptCCB, TUBINGCFG *ptTubing, UINT nLang);
    void InitOEMValue(TUBINGCFG *ptTubing);
    void InitSizeValue(TUBINGCFG *ptTubing);
    void InitMatValue(TUBINGCFG *ptTubing);
    void InitCouplValue(TUBINGCFG *ptTubing);
};

