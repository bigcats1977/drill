
#pragma once

#define WITSHEAD                _T("&&\r\n")
#define WITSTAIL                _T("!!\r\n")
#define WITSSPLIT               _T("\r\n")

#define RPTHISDATANUM			30

class WITSEnc
{
public:
	static string EncWITSTorqConfig(UINT SeqNO, WITSCFG* ptWITS, SHOWCFG *ptShow);
	static string EncWITSTorqData(UINT SeqNO, WITSCFG* ptWITS, WITSRPTDATA *ptData);
	static string EncWITSTorqQuality(UINT SeqNO, WITSCFG* ptWITS, WITSRPTDATA* ptData, TorqData::Torque *ptTorq);
	static string EncWITSHisTorq(UINT SeqNO, int Start, WITSCFG* ptWITS, DRAWTORQDATA* ptDraw);

private:
	static string EncWITSFixHead(UINT SeqNO, WITSCFG* ptWITS);
	static string EncOnlyTorqData(WITSCFG* ptWITS, WITSRPTDATA* ptData);
	static string EncWITSFixTail();
};
