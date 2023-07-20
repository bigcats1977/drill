#include "stdafx.h"
#include "WITSEnc.h"
#include "Drill.h"

string WITSEnc::EncWITSFixHead(UINT SeqNO, WITSCFG* ptWITS)
{
    string  strData;
    string  strTemp;
    //__time64_t  curTime;
    int   index = 0;
    UINT  ItemNO;
    SYSTEMTIME  ts;

    ASSERT_NULL_R(ptWITS, strData);

    strData += WITSHEAD;
    GetLocalTime(&ts);

    // 8001 Date
    ItemNO = ptWITS->FixItems[index++];
    strTemp = string_format("%04d-%02d-%02d", ts.wYear, ts.wMonth, ts.wDay);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    // 8002 Time
    ItemNO = ptWITS->FixItems[index++];
    strTemp = string_format("%02d:%02d:%02d.%03d", ts.wHour, ts.wMinute, ts.wSecond, ts.wMilliseconds);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    // 8003 SeqNO
    ItemNO = ptWITS->FixItems[index++];
    strTemp = string_format("%04d", SeqNO);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    return strData;
}

string WITSEnc::EncHisFixHead(UINT SeqNO, WITSCFG* ptWITS, TorqData::Torque* ptTorq, bool bBreakout, int diff)
{
    string  strData;
    string  strTemp;
    //__time64_t  curTime;
    int   index = 0;
    UINT  ItemNO;
    __time64_t  colTime;

    ASSERT_NULL_R(ptWITS, strData);
    ASSERT_NULL_R(ptTorq, strData);

    strData += WITSHEAD;

    colTime = ptTorq->mucoltime();
    if (bBreakout)
        colTime = ptTorq->bocoltime();
    colTime -= diff;
    CTime histime(colTime);

    // 8001 Date
    ItemNO = ptWITS->FixItems[index++];
    //strTemp = string_format("%04d-%02d-%02d", ts.wYear, ts.wMonth, ts.wDay);
    strTemp = string_format("%04d-%02d-%02d", histime.GetYear(), histime.GetMonth(), histime.GetDay());
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    // 8002 Time
    ItemNO = ptWITS->FixItems[index++];
    //strTemp = string_format("%02d:%02d:%02d.%03d", ts.wHour, ts.wMinute, ts.wSecond, ts.wMilliseconds);
    strTemp = string_format("%02d:%02d:%02d.%03d", histime.GetHour(), histime.GetMinute(), histime.GetSecond(), 0);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    // 8003 SeqNO
    ItemNO = ptWITS->FixItems[index++];
    strTemp = string_format("%04d", SeqNO);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    return strData;

}

string WITSEnc::EncWITSFixTail()
{
    return WITSTAIL;
}

// 只上报扭矩最大的数据（最新的数据）
string WITSEnc::EncOnlyTorqData(WITSCFG* ptWITS, WITSRPTDATA* ptData)
{
    string  strData;
    string  strTemp;
    UINT    ItemNO = 0;
    int     index = 0;
    int     rptIndex = ptData->nCount - 1;
    double  fMaxTorq = INT_MIN;

    ASSERT_NULL_R(ptWITS, strData);
    ASSERT_NULL_R(ptData, strData);
    ASSERT_ZERO_R(ptData->nCount, strData);
    COMP_BGE_R(ptData->nRptIdx, ptData->nCount, strData);

    for (UINT i = ptData->nRptIdx; i < ptData->nCount; i++)
    {
        if (ptData->fTorque[i] > fMaxTorq)
        {
            rptIndex = i;
            fMaxTorq = ptData->fTorque[i];
        }
    }

    index = 0;

    // 8051 Torque
    ItemNO = ptWITS->RepeatItems[index++];
    strTemp = string_format("%d", (int)ptData->fTorque[rptIndex]);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    // 8052 圈数
    ItemNO = ptWITS->RepeatItems[index++];
    strTemp = string_format("%.2f", ptData->fTurn[rptIndex]);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    // 8053 时间
    ItemNO = ptWITS->RepeatItems[index++];
    strTemp = string_format("%.2f", ptData->fDuration[rptIndex]);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    ptData->nRptIdx = ptData->nCount;

    return strData;
}

string WITSEnc::EncWITSTorqConfig(UINT SeqNO, WITSCFG* ptWITS, SHOWCFG* ptShow)
{
    string  strData;
    string  strTemp;
    vector<int> items;
    vector<int>::iterator it;
    UINT  ItemNO = 0;
    int index = 0;

    ASSERT_NULL_R(ptWITS, strData);
    ASSERT_NULL_R(ptShow, strData);

    strData += EncWITSFixHead(SeqNO, ptWITS);

    items = ptWITS->ShowParas;
    for (int i = 0; i < WITSRPT_SHOWPARANUM; i++)
    {
        it = find(items.begin(), items.end(), i + 1);
        if (it != items.end())
        {
            index = it - items.begin();
            ItemNO = ptWITS->ShowItems[index];

            strTemp = string_format("%s", ptShow->strShow[i + 1].c_str());
            strData += to_string(ItemNO) + strTemp + WITSSPLIT;
        }
    }

    strData += EncWITSFixTail();
    return strData;
}

string WITSEnc::EncWITSTorqData(UINT SeqNO, WITSCFG* ptWITS, WITSRPTDATA* ptData)
{
    string  strData;
    string  strTorq;

    ASSERT_NULL_R(ptWITS, strData);
    ASSERT_NULL_R(ptData, strData);

    strTorq = EncOnlyTorqData(ptWITS, ptData);
    ASSERT_ZERO_R(strTorq.size(), strData);

    strData += EncWITSFixHead(SeqNO, ptWITS);

    strData += strTorq;

    strData += EncWITSFixTail();

    return strData;
}

string WITSEnc::EncWITSTorqQuality(UINT SeqNO, WITSCFG* ptWITS, WITSRPTDATA* ptData, TorqData::Torque* ptTorq)
{
    string  strData;
    string  strTemp;
    UINT    ItemNO;
    int     index = 0;
    WORD    wIPPos = 0;
    WORD    wSchPos = 0;

    ASSERT_NULL_R(ptWITS, strData);
    ASSERT_NULL_R(ptTorq, strData);

    strData += EncWITSFixHead(SeqNO, ptWITS);

    // reserve report data
    if (ptData)
        strData += EncOnlyTorqData(ptWITS, ptData);

    // calculate items
    int IPTorq = 0, DeltaTorq = 0;
    double fSlopeFactor = 0, fIPTime = 0;
    index = 0;
    // 8024 Control Torque
    ItemNO = ptWITS->CalItems[index++];
    strTemp = string_format("%d", (int)ptTorq->fmumaxtorq());
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    // 8025 IP Torque
    ItemNO = ptWITS->CalItems[index++];
    strTemp = string_format("%d", IPTorq);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;
    // 8026 Delta Torque
    ItemNO = ptWITS->CalItems[index++];
    strTemp = string_format("%d", DeltaTorq);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;
    // 8027 台阶比
    ItemNO = ptWITS->CalItems[index++];
    strTemp = string_format("%.2f", fSlopeFactor);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;
    // 8028 台阶时间
    ItemNO = ptWITS->CalItems[index++];
    strTemp = string_format("%.2f", fIPTime);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    strData += EncWITSFixTail();
    return strData;
}

string WITSEnc::EncHisTorqConfig(UINT SeqNO, WITSCFG* ptWITS, TorqData::Torque* ptTorq)
{
    string  strData;
    string  strTemp;
    vector<int> items;
    vector<int>::iterator it;
    UINT  ItemNO = 0;
    int index = 0;

    ASSERT_NULL_R(ptWITS, strData);
    ASSERT_NULL_R(ptTorq, strData);

    strData += EncHisFixHead(SeqNO, ptWITS, ptTorq, theApp.HaveBreakout(ptTorq), 0);

    items = ptWITS->ShowParas;
    for (int i = 0; i < WITSRPT_SHOWPARANUM; i++)
    {
        it = find(items.begin(), items.end(), i + 1);
        if (it != items.end())
        {
            index = it - items.begin();
            ItemNO = ptWITS->ShowItems[index];

            //strTemp = string_format("%s", ptShow->strShow[i + 1].c_str());
            strTemp = string_format("%s", theApp.GetTorqShowValue(ptTorq, i+1));
            strData += to_string(ItemNO) + strTemp + WITSSPLIT;
        }
    }

    strData += EncWITSFixTail();
    return strData;
}

string WITSEnc::EncHisTorqData(UINT SeqNO, int Start, WITSCFG* ptWITS, TorqData::Torque* ptTorq)
{
    int i = 0;
    int index = 0;
    double fTemp = 0;
    UINT  ItemNO;
    string  strTemp;
    string  strData;
    string  strTorq;
    int plus = 0;

    ASSERT_NULL_R(ptWITS, strData);
    ASSERT_NULL_R(ptTorq, strData);
    ASSERT_ZERO_R(ptTorq->ftorque_size(), strData);
    COMP_BGE_R(Start, ptTorq->ftorque_size(), strData);

    for (int i = Start; i < Start + RPTHISDATANUM && i < ptTorq->ftorque_size(); i += HISDATAINTER)
    {
        index = 0;
        // 8051 Torque
        ItemNO = ptWITS->RepeatItems[index++];
        strTemp = string_format("%d", (int)ptTorq->ftorque(i));
        strTorq += to_string(ItemNO) + strTemp + WITSSPLIT;

        // 8052 圈数
        for (int j = 0; j < i + HISDATAINTER && j < ptTorq->dwdelplus_size(); j++)
        {
            plus += ptTorq->dwdelplus(j);
        }
        ItemNO = ptWITS->RepeatItems[index++];
        //fTemp = i * ptDraw->ptOrgTorq->fmaxcir() / MAXLINEITEM;
        fTemp = plus / ptTorq->fplus();
        strTemp = string_format("%.2f", fTemp);
        strTorq += to_string(ItemNO) + strTemp + WITSSPLIT;

        // 8053 时间
        ItemNO = ptWITS->RepeatItems[index++];
        fTemp = i * 0.1;
        strTemp = string_format("%.2f", fTemp);
        strTorq += to_string(ItemNO) + strTemp + WITSSPLIT;
    }

    ASSERT_ZERO_R(strTorq.size(), strData);

    strData += EncWITSFixHead(SeqNO, ptWITS);

    strData += strTorq;

    strData += EncWITSFixTail();

    return strData;
}
