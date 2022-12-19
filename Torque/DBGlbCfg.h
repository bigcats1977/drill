#pragma once
#include "DBBase.h"

class CDBGlbCfg: public CDBBase
{
public:
    UINT    _LangType;
    UINT    _PortNO;
    UINT    _BaudRate;
    UINT    _PlusPerTurn;
    UINT    _TorqUnit;
    UINT    _CollectDur;
    UINT    _ResetDur;
    UINT    _SaveDur;
    UINT    _IPShowMode;
    UINT    _ZoomIn;
    UINT    _ImgNum;
    //UINT    _Test;

    double  _Discount;
    double  _Multi;
    double  _RpmAdj;
    double  _IPDeltaVal;

    BOOL    _CheckIP;
    BOOL    _BigTorq;
    BOOL    _BreakOut;
    BOOL    _DateBehind;

    string  _Password;
    string  _DataPath;

    CDBGlbCfg();
    ~CDBGlbCfg();

    BOOL   UpdateGlbCfg(GLBCFG * ptfg);

private:
    void Empty();
    void GetTable();
};

