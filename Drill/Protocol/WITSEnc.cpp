#include "stdafx.h"
#include <chrono>
#include "WITSEnc.h"
#include "Drill.h"

using namespace chrono;

string WITSEnc::EncWITSFixHead(UINT SeqNO, WITSCFG* ptWITS, int diff)
{
    string  strData;
    string  strTemp;
    int   index = 0;
    UINT  ItemNO;

    ASSERT_NULL_R(ptWITS, strData);

    strData += WITSHEAD;
    // ��ȡ��ǰʱ���
    system_clock::time_point currentTime = system_clock::now();
    currentTime += milliseconds(diff);
    // ��ʱ���ת��Ϊʱ��ṹ
    time_t currentTime_t = system_clock::to_time_t(currentTime);
    tm* timeInfo = localtime(&currentTime_t);
    // ��ȡ�����ֵ
    auto ms = duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count() % 1000;

    // ��ӡ�ꡢ�¡��ա�ʱ���֡���
    /*TRACE("%d-%d-%d %d:%d:%d.%d\n", timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday,
        timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec, ms);*/

    // 8001 Date
    ItemNO = ptWITS->FixItems[index++];
    strTemp = string_format("%04d-%02d-%02d", timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    // 8002 Time
    ItemNO = ptWITS->FixItems[index++];
    strTemp = string_format("%02d:%02d:%02d.%03d", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec, ms);
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

string WITSEnc::EncWITSTorqConfig(UINT SeqNO, int iBreakout, double fCtrlTorq, WITSCFG* ptWITS, SHOWCFG* ptShow)
{
    string  strData;
    string  strTemp;
    CString strValue;
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
        strValue = ptShow->strShow[i + 1].c_str();
        if (strValue.IsEmpty())
            continue;
        it = find(items.begin(), items.end(), i + 1);
        if (it != items.end())
        {
            index = it - items.begin();
            ItemNO = ptWITS->ShowItems[index];

            strTemp = string_format("%s", strValue);
            strData += to_string(ItemNO) + strTemp + WITSSPLIT;
        }
    }

    index = 0;
    // 8601 Torque Type
    ItemNO = ptWITS->ConfigItems[index++];
    strTemp = string_format("%d", iBreakout == 0 ? 0 : 1);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    // 8602 Max Torq
    ItemNO = ptWITS->ConfigItems[index++];
    strTemp = string_format("%.0f", HAND_CEIL(fCtrlTorq * RATIO_UPPERLIMIT));
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    // 8603 Min Torq
    ItemNO = ptWITS->ConfigItems[index++];
    strTemp = string_format("%.0f", HAND_CEIL(fCtrlTorq * RATIO_LOWERLIMIT));
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    strData += EncWITSFixTail();
    return strData;
}

// ֻ�ϱ�Ť���������ݣ����µ����ݣ�
// ����ϱ�5������
string WITSEnc::EncWITSTorqData(UINT SeqNO, WITSCFG* ptWITS, WITSRPTDATA* ptData)
{
    string  strHead, strTail, strTorq;
    string  strData;
    string  strTemp;
    UINT    ItemNO = 0;
    UINT    begin = 0;
    int     index = 0;
    int     diff = 0;
    int     rptIndex = ptData->nCount - 1;
    double  fMaxTorq = INT_MIN;

    ASSERT_NULL_R(ptWITS, strData);
    ASSERT_NULL_R(ptData, strData);
    ASSERT_ZERO_R(ptData->nCount, strData);
    COMP_BGE_R(ptData->nRptIdx, ptData->nCount, strData);

    strTail = EncWITSFixTail();

    begin = ptData->nRptIdx;
    if (ptData->nCount - ptData->nRptIdx > WITSRPT_TORQUECOUNT)
        begin = ptData->nCount - WITSRPT_TORQUECOUNT;
    diff = -1 * g_tGlbCfg.nCollectDur * (ptData->nCount - begin);
    for (; begin < ptData->nCount; begin++)
    {
        index = 0;
        strTorq.clear();
        diff += g_tGlbCfg.nCollectDur;

        // 8051 Torque
        ItemNO = ptWITS->RepeatItems[index++];
        strTemp = string_format("%d", (int)ptData->fTorque[begin] / RPTTORQMULTI);
        strTorq += to_string(ItemNO) + strTemp + WITSSPLIT;

        // 8052 Ȧ��
        ItemNO = ptWITS->RepeatItems[index++];
        strTemp = string_format("%.2f", ptData->fTurn[begin]);
        strTorq += to_string(ItemNO) + strTemp + WITSSPLIT;

        // 8053 ʱ��
        ItemNO = ptWITS->RepeatItems[index++];
        strTemp = string_format("%.2f", ptData->fDuration[begin]);
        strTorq += to_string(ItemNO) + strTemp + WITSSPLIT;

        strHead = EncWITSFixHead(SeqNO, ptWITS, diff);
        strData += strHead + strTorq + strTail;
    }
#if 0
    index = 0;

    // 8051 Torque
    ItemNO = ptWITS->RepeatItems[index++];
    strTemp = string_format("%d", (int)ptData->fTorque[rptIndex] / RPTTORQMULTI);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    // 8052 Ȧ��
    ItemNO = ptWITS->RepeatItems[index++];
    strTemp = string_format("%.2f", ptData->fTurn[rptIndex]);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    // 8053 ʱ��
    ItemNO = ptWITS->RepeatItems[index++];
    strTemp = string_format("%.2f", ptData->fDuration[rptIndex]);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;
#endif
    ptData->nRptIdx = ptData->nCount;

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

    // reserve report data
    if (ptData)
        strData += EncWITSTorqData(SeqNO, ptWITS, ptData);

    strData += EncWITSFixHead(SeqNO, ptWITS);
    // calculate items
    //int IPTorq = 0, DeltaTorq = 0;
    //double fSlopeFactor = 0, fIPTime = 0;
    index = 0;
    // 8024 Control Torque
    ItemNO = ptWITS->CalItems[index++];
    strTemp = string_format("%d", (int)ptTorq->fmumaxtorq() / RPTTORQMULTI);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;
#if 0
    // 8025 IP Torque
    ItemNO = ptWITS->CalItems[index++];
    strTemp = string_format("%d", IPTorq / RPTTORQMULTI);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;
    // 8026 Delta Torque
    ItemNO = ptWITS->CalItems[index++];
    strTemp = string_format("%d", DeltaTorq / RPTTORQMULTI);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;
    // 8027 ̨�ױ�
    ItemNO = ptWITS->CalItems[index++];
    strTemp = string_format("%.2f", fSlopeFactor);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;
    // 8028 ̨��ʱ��
    ItemNO = ptWITS->CalItems[index++];
    strTemp = string_format("%.2f", fIPTime);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;
#endif
    strData += EncWITSFixTail();
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

string WITSEnc::EncHisTorqConfig(UINT SeqNO, bool bBO, double fCtrlTorq, WITSCFG* ptWITS, TorqData::Torque* ptTorq)
{
    string  strData;
    string  strTemp;
    CString strValue;
    vector<int> items;
    vector<int>::iterator it;
    UINT  ItemNO = 0;
    int index = 0;

    ASSERT_NULL_R(ptWITS, strData);
    ASSERT_NULL_R(ptTorq, strData);

    strData += EncHisFixHead(SeqNO, ptWITS, ptTorq, bBO, 0);

    items = ptWITS->ShowParas;
    for (int i = 0; i < WITSRPT_SHOWPARANUM; i++)
    {
        it = find(items.begin(), items.end(), i + 1);
        if (it != items.end())
        {
            strValue = theApp.GetTorqShowValue(ptTorq, i + 1);
            if (strValue.IsEmpty())
                continue;
            index = it - items.begin();
            ItemNO = ptWITS->ShowItems[index];

            //strTemp = string_format("%s", ptShow->strShow[i + 1].c_str());
            strTemp = string_format("%s", strValue);
            strData += to_string(ItemNO) + strTemp + WITSSPLIT;
        }
    }

    index = 0;
    // 8601 Torque Type
    ItemNO = ptWITS->ConfigItems[index++];
    strTemp = string_format("%d", bBO);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    // 8602 Max Torq
    ItemNO = ptWITS->ConfigItems[index++];
    strTemp = string_format("%.0f", HAND_CEIL(fCtrlTorq * RATIO_UPPERLIMIT));
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    // 8603 Min Torq
    ItemNO = ptWITS->ConfigItems[index++];
    strTemp = string_format("%.0f", HAND_CEIL(fCtrlTorq * RATIO_LOWERLIMIT));
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;

    strData += EncWITSFixTail();
    return strData;
}

string WITSEnc::EncHisTorqData(UINT SeqNO, int Start, WITSCFG* ptWITS, TorqData::Torque* ptTorq, bool bBreakout)
{
    int i = 0, j = 0;
    int index = 0;
    double fTemp = 0;
    UINT  ItemNO;
    string  strTemp;
    string  strData;
    string  strTorq;
    int plus = 0;
    double  duration = 0;

    ASSERT_NULL_R(ptWITS, strData);
    ASSERT_NULL_R(ptTorq, strData);
    ASSERT_ZERO_R(ptTorq->ftorque_size(), strData);
    COMP_BGE_R(Start, ptTorq->ftorque_size(), strData);

    int begin = 0, end = ptTorq->dwmucount();
    duration = ptTorq->dwmucount() / 10.0;
    if (bBreakout)
    {
        begin = ptTorq->dwmucount();
        end = ptTorq->dwmucount() + ptTorq->dwbocount();
        duration = ptTorq->dwbocount() / 10.0;
    }

    for (i = Start + begin; i < Start + RPTHISDATANUM && i < end; i += HISDATAINTER)
    {
        index = 0;
        strTorq.clear();

        // 8051 Torque
        fTemp = 0;
        plus = 0;
        for (j = i; j < i + HISDATAINTER && j < end; j++)
        {
            fTemp = MAX(fTemp, ptTorq->ftorque(j));
        }
        ItemNO = ptWITS->RepeatItems[index++];
        //strTemp = string_format("%d", (int)ptTorq->ftorque(i));
        strTemp = string_format("%d", (int)fTemp / RPTTORQMULTI);
        strTorq += to_string(ItemNO) + strTemp + WITSSPLIT;

        // 8052 Ȧ��
        for (int j = begin; j < i + HISDATAINTER && j < end; j++)
        {
            plus += ptTorq->dwdelplus(j);
        }
        ItemNO = ptWITS->RepeatItems[index++];
        //fTemp = i * ptDraw->ptOrgTorq->fmaxcir() / MAXLINEITEM;
        fTemp = plus / ptTorq->fplus();
        strTemp = string_format("%.2f", fTemp);
        strTorq += to_string(ItemNO) + strTemp + WITSSPLIT;

        // 8053 ʱ��
        ItemNO = ptWITS->RepeatItems[index++];
        fTemp = (i - begin) * 0.1;
        strTemp = string_format("%.2f", fTemp);
        strTorq += to_string(ItemNO) + strTemp + WITSSPLIT;

        int diff = (int)floor(duration - fTemp);
        strData += EncHisFixHead(SeqNO, ptWITS, ptTorq, bBreakout, diff);

        strData += strTorq;

        strData += EncWITSFixTail();
    }

    return strData;
}

string WITSEnc::EncHisTorqQuality(UINT SeqNO, WITSCFG* ptWITS, TorqData::Torque* ptTorq)
{
    string  strData;
    string  strTemp;
    UINT    ItemNO;
    int     index = 0;
    WORD    wIPPos = 0;
    WORD    wSchPos = 0;

    ASSERT_NULL_R(ptWITS, strData);
    ASSERT_NULL_R(ptTorq, strData);

    strData += EncHisFixHead(SeqNO, ptWITS, ptTorq, theApp.HaveBreakout(ptTorq), 0);

    // calculate items
    int IPTorq = 0, DeltaTorq = 0;
    double fSlopeFactor = 0, fIPTime = 0;
    index = 0;
    // 8024 Control Torque
    ItemNO = ptWITS->CalItems[index++];
    strTemp = string_format("%d", (int)ptTorq->fmumaxtorq() / RPTTORQMULTI);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;
#if 0
    // 8025 IP Torque
    ItemNO = ptWITS->CalItems[index++];
    strTemp = string_format("%d", IPTorq / RPTTORQMULTI);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;
    // 8026 Delta Torque
    ItemNO = ptWITS->CalItems[index++];
    strTemp = string_format("%d", DeltaTorq / RPTTORQMULTI);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;
    // 8027 ̨�ױ�
    ItemNO = ptWITS->CalItems[index++];
    strTemp = string_format("%.2f", fSlopeFactor);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;
    // 8028 ̨��ʱ��
    ItemNO = ptWITS->CalItems[index++];
    strTemp = string_format("%.2f", fIPTime);
    strData += to_string(ItemNO) + strTemp + WITSSPLIT;
#endif
    strData += EncWITSFixTail();
    return strData;
}
