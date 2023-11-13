
#pragma once

#define WITSHEAD                _T("&&\r\n")
#define WITSTAIL                _T("!!\r\n")
#define WITSSPLIT               _T("\r\n")

#define RPTHISDATANUM			50	//每次最多上报5个，历史记录每秒1条记录，5×10=50
#define HISDATAINTER			10
#define RPTTORQMULTI			1	//上报扭矩单位，1表示直接上报扭矩值

class WITSEnc
{
public:
	static string EncWITSTorqConfig(UINT SeqNO, bool bBreakout, double fCtrlTorq, WITSCFG* ptWITS, SHOWCFG *ptShow);
	static string EncWITSTorqData(UINT SeqNO, WITSCFG* ptWITS, WITSRPTDATA *ptData);
	static string EncWITSTorqQuality(UINT SeqNO, WITSCFG* ptWITS, WITSRPTDATA* ptData, TorqData::Torque *ptTorq);

	static string EncHisTorqConfig(UINT SeqNO, bool bBreakout, double fCtrlTorq, WITSCFG* ptWITS, TorqData::Torque* ptTorq);
	static string EncHisTorqData(UINT SeqNO, bool bBreakout, int Start, WITSCFG* ptWITS, TorqData::Torque* ptTorq);
	static string EncHisTorqQuality(UINT SeqNO, WITSCFG* ptWITS, TorqData::Torque* ptTorq);
private:
	static string EncWITSFixHead(UINT SeqNO, WITSCFG* ptWITS, int diff = 0);
	static string EncWITSFixTail();

	static string EncHisFixHead(UINT SeqNO, bool bBreakout, WITSCFG* ptWITS, TorqData::Torque* ptTorq, int diff = 0);
};
