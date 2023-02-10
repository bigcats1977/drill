#pragma once

#include <string>
#include <afxdb.h>

using namespace std;

#define REGNAME         _T("reg.dat")

#define MAXREGYEAR      200
#define REGCODEVALUE    0x55    /* 保存的注册码的差值 */
#define REGCODESEGNUM   6       /* 注册码分为6段，长度分别定义如下 */
#define REGCODELEN      6
#define MACHVOL1LEN		8
#define MACHVOL2LEN		8
#define MACHMAC1LEN		6
#define MACHMAC2LEN		4

/* REGCODE计算的常数定义 */
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
    BYTE    strGenDate[TIMESTRLEN];     // 填写注册码时间，检查日期往前随机30天
                                        // 如果当前日期小于检查日期，说明修改过日期，注册无效，除非重新注册
    BYTE    bRsv1 : 3;
    BYTE    bReged : 1;
    BYTE    bMach : 1;                  // 机器码有效
    BYTE    bRsv2 : 3;
    BYTE    ucYear;
    BYTE    strRegDate[TIMESTRLEN];     // 软件有效日期，注册日期往后××年
    BYTE    strCode2[HALFREGCODE];
    BYTE    strUseDate[TIMESTRLEN];     // 软件最新使用日期

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

    // 软件注册相关函数
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

