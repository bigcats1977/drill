#pragma once

#include "afxwin.h"
#include "Torque.h"



class CTubeCfg
{
public:
    CTubeCfg();
    ~CTubeCfg();
    void InitConfig();
    //void LoadTubingInfo();
    //void LoadTubingCfg();
    void GetTubParaAddr(int index, FIXTUBEINFO* ptTub);

    FIXTUBEINFO  m_tTubInfo[MAXTUBECFGNUM];
    /*FIXTUBINFO  m_tFactory;
    FIXTUBINFO  m_tOEM;
    FIXTUBINFO  m_tSize;
    FIXTUBINFO  m_tMater;
    FIXTUBINFO  m_tCoupl;*/
    FIXTUBECFG   m_tTubCfg;
   
    void SetFactoryBox(CComboBox *ptCCB, TUBECFG *ptTubing, UINT nLang=LANGUAGE_CURRENT);
    void SetOEMBox(CComboBox *ptCCB, TUBECFG *ptTubing, UINT nLang=LANGUAGE_CURRENT);
    void SetSizeBox(CComboBox *ptCCB, TUBECFG *ptTubing, UINT nLang=LANGUAGE_CURRENT);
    void SetMatBox(CComboBox *ptCCB, TUBECFG *ptTubing, UINT nLang=LANGUAGE_CURRENT);
    void SetCouplBox(CComboBox *ptCCB, TUBECFG *ptTubing, UINT nLang=LANGUAGE_CURRENT);
    void SetAllComboBox(UINT nIndex, CComboBox* ptCCB, TUBECFG* ptTubing, UINT nLang=LANGUAGE_CURRENT);
    UINT GetTubeNOByName(UINT nIndex, UINT nFactory, CComboBox* ptCCB, UINT nLang=LANGUAGE_CURRENT);
    BOOL CheckTubeNOChanged(UINT nIndex, UINT nCurNO, TUBECFG* ptOld, TUBECFG* ptCurrent);
    BOOL CheckIndex(UINT nIndex);
    void InitOEMValue(TUBECFG *ptTubing);
    void InitSizeValue(TUBECFG *ptTubing);
    void InitMatValue(TUBECFG *ptTubing);
    void InitCouplValue(TUBECFG *ptTubing);

private:
    void InitVar();
    void SetComboBoxValues(CComboBox* ptCCB, int index, set<UINT> &lsNO, UINT nCurVal, UINT nLang=LANGUAGE_CURRENT);
    BOOL CheckInFactory(UINT nIndex, UINT nFactory);
    void ClearParaString(FIXTUBEINFO *ptTube);
};

