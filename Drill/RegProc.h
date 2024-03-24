#pragma once

#include <string>
#include <afxdb.h>

using namespace std;

#define REGNAME         _T("reg.dat")

#define MAXREGYEAR      200
#define REGCODEVALUE    0x55    /* �����ע����Ĳ�ֵ */
#define REGCODESEGNUM   6       /* ע�����Ϊ6�Σ����ȷֱ������� */
#define REGCODELEN      6
#define MACHVOL1LEN		8
#define MACHVOL2LEN		8
#define MACHMAC1LEN		6
#define MACHMAC2LEN		4

/* REGCODE����ĳ������� */
#define MAC0XOR         0x789789
#define MAC0AND         0x00FFFFFF
//#define MAC0DEC         0x197712
#define MAC1XOR         0x7890
#define MAC1AND         0x0000FFFF
//#define MAC1DEC         0x1207
#define VOL0XOR         0x51685759
//#define VOL0DEC         0x21832475      // 0x66666666 change
#define VOL1XOR         0x13963349
//#define VOL1DEC         0x68497915      // 0x12345678 change
#define DPATHV2         0x1f5d876e

#define MAXMACHCODE     26
#define MAXREGCODE      36
#define HALFREGCODE     18
#define TIMESTRLEN      8
#define TORQNUMLEN      8
typedef struct tagDBREG
{
    BYTE    strMachCode[MAXMACHCODE];
    BYTE    strCode1[HALFREGCODE];
    BYTE    strGenDate[TIMESTRLEN];     // ��дע����ʱ�䣬���������ǰ���30��
                                        // �����ǰ����С�ڼ�����ڣ�˵���޸Ĺ����ڣ�ע����Ч����������ע��
    BYTE    bRsv1 : 3;
    BYTE    bReged : 1;
    BYTE    bMach : 1;                  // ��������Ч
    BYTE    bRsv2 : 3;
    BYTE    ucYear;
    BYTE    strRegDate[TIMESTRLEN];     // �����Ч���ڣ�ע���������������
    BYTE    strCode2[HALFREGCODE];
    BYTE    strUseDate[TIMESTRLEN];     // �������ʹ������

    BYTE    strTotalNum[TORQNUMLEN];
}DBREG;
#define DBREGLEN        (sizeof(DBREG))

class CRegProc
{
public:
    CRegProc();
    CRegProc(string regFile);
    ~CRegProc();
    DWORD   m_dwTotalTorqNum;

public:
    bool    Reged();
    bool    Machine();
    bool    CheckAppReg();
    bool    CheckProductDate();
    void    SaveTorqNum();

    string  GetMachineCode();
    string  GetRegisterCode();
    string  GenMachineCode();
    bool    CheckRegCode(string strMach, string strReg);

    void    SaveRegFile();


private:
    string          m_strRegFile;
    DBREG           m_tdbReg;

    string          m_strMachine;
    string          m_strRegCode;

    string          m_strRegDate;
    string          m_strGenDate;
    string          m_strUseDate;

    string          m_base64chars;
private:
    void        InitValue();
    bool        GetRegInfoFromDBREG();
    string      GetRegStrFromArray(BYTE* pData, int len);
    bool        GenRegArrayFromStr(BYTE* pData, int len, string str);

    // ���ע����غ���
    bool        CheckRegDate();
    bool        CheckRegCode();
    bool        CheckMachineCode();
    bool        GetVolMacFromMachine(string strMachine, DWORD pdwVol[], DWORD pdwMac[], int& iYear, int& iMonth, int& iDay);
    bool        GetVolMacFromRegStr(string strRegCode, DWORD pdwVol[], DWORD pdwMac[], int& iYear, int& iMonth, int& iDay);
    bool        GenVolMacInfo(DWORD pdwVol[], DWORD pdwMac[], int iYear, int iMonth, int iDay);

    bool        GetDateFromString(string date, CTime& time);

    void        StringSubtract(CString& strValue, BYTE ucChar);
    void        StringSubtract(string& strValue, BYTE ucChar);

    bool        is_base64(unsigned char c);
    string      base64_encode(char const* bytes_to_encode, int in_len);
    string      base64_decode(string encoded_string);
};

