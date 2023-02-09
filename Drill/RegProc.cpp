#include "RegProc.h"
#include "Drill.h"

CRegProc::CRegProc()
{
    InitValue();
}

CRegProc::CRegProc(string regFile)
{
    CFile           file;
    CFileFind       find;
    int             reglen = 0;
    char            *buffer;
    string          info;

    InitValue();

    if (regFile.size() < sizeof(REGNAME))
        return;

    m_strRegFile = regFile;

    // bMach == 0
    COMP_BFALSE(find.FindFile(regFile.c_str(), 0));

    file.Open(regFile.c_str(), CFile::modeRead | CFile::shareDenyNone, NULL);
    reglen = (int)file.GetLength();
    if (reglen < DBREGLEN)
    {
        file.Close();
        return;
    }

    buffer = (char*)calloc(reglen + 1, 1);
    file.Read(buffer, reglen);
    file.Close();

    info = base64_decode(buffer);
    free(buffer);

    memcpy(&m_tdbReg, info.c_str(), DBREGLEN);

    GetRegInfoFromDBREG();
}

CRegProc::~CRegProc()
{
}

void CRegProc::InitValue()
{
    m_dwTotalTorqNum = 0;

    memset(&m_tdbReg, 0, DBREGLEN);
    m_tdbReg.bRsv1 = 6;
    m_tdbReg.bRsv2 = 5;

    m_strRegFile.clear();

    m_strMachine.clear();
    m_strRegCode.clear();
    m_strRegDate.clear();
    m_strGenDate.clear();
    m_strUseDate.clear();

    m_base64chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
}

bool CRegProc::Reged()
{
    return m_tdbReg.bReged;
}
bool CRegProc::Machine()
{
    return m_tdbReg.bMach;
}
string CRegProc::GetMachineCode()
{
    if (m_tdbReg.bMach)
        return m_strMachine;
    return "";
}
string  CRegProc::GetRegisterCode()
{
    if (m_tdbReg.bReged)
        return m_strRegCode;
    return "";
}

string CRegProc::GenMachineCode()
{
    CTime   tNowDate = CTime::GetCurrentTime();
    int     iYear, iMonth, iDay;
    DWORD   adwPCVol[2];
    DWORD   adwPCMac[2];
    char    buffer[50] = { 0 };

    iYear = tNowDate.GetYear();
    iMonth = tNowDate.GetMonth();
    iDay = tNowDate.GetDay();

    /* 获取机器上的卷和MAC信息 */
    GenVolMacInfo(adwPCVol, adwPCMac, iYear, iMonth, iDay);

    adwPCMac[0] ^= MAC0XOR;
    adwPCMac[1] ^= MAC1XOR;
    adwPCMac[0] &= MAC0AND;
    adwPCMac[1] &= MAC1AND;
    adwPCVol[0] ^= VOL0XOR;
    adwPCVol[1] ^= VOL1XOR;

    m_strMachine = string_format("%08X%04X%08X%06X", adwPCVol[0], adwPCMac[1], adwPCVol[1], adwPCMac[0]);
    m_strUseDate = string_format("%04d%02d%02d", iYear, iMonth, iDay);
    m_strGenDate = string_format("%04d%02d%02d", iYear, iMonth, iDay);
    m_strRegCode = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    m_strRegDate = "20000101";
    m_tdbReg.ucYear = 0;
    m_tdbReg.bReged = 0;
    SaveRegFile();

    return m_strMachine;
}

bool CRegProc::CheckRegCode(string strMach, string strReg)
{
    int     machYear, machMonth, machDay;
    int     regYear, regMonth, regDay, iYears = 0;
    DWORD   adwRegVol[2];
    DWORD   adwRegMac[2];
    DWORD   adwPCVol[2];
    DWORD   adwPCMac[2];
    CTime   curTime = CTime::GetCurrentTime();

    COMP_BFALSE_R(GetVolMacFromMachine(strMach, adwPCVol, adwPCMac, machYear, machMonth, machDay), false);

    /* 获取注册码中的卷和MAC信息 */
    COMP_BFALSE_R(GetVolMacFromRegStr(strReg, adwRegVol, adwRegMac, regYear, regMonth, regDay), false);

    // 机器码中的日期无效
    // 注册码中的日期应该和机器码的日期相等，年份差整数倍
    if (machYear > regYear || machMonth != regMonth || machDay != regDay)
    {
        return false;
    }
    iYears = regYear - machYear;
    COMP_BG_R(iYears, MAXREGYEAR, false);

    COMP_BFALSE_R(GenVolMacInfo(adwPCVol, adwPCMac, regYear, regMonth, regDay), false);

    m_strRegDate.clear();
    if (memcmp(adwRegVol, adwPCVol, 2 * sizeof(DWORD)) == 0 &&
        memcmp(adwRegMac, adwPCMac, 2 * sizeof(DWORD)) == 0)
    {
        m_tdbReg.bMach = 1;
        m_tdbReg.bReged = 1;
        m_tdbReg.ucYear = (BYTE)iYears;
        m_strMachine = strMach;
        m_strRegCode = strReg;
        m_strGenDate = string_format("%04d%02d%02d", machYear, machMonth, machDay);
        m_strRegDate = string_format("%04d%02d%02d", regYear, regMonth, regDay);
        m_strUseDate = string_format("%04d%02d%02d", curTime.GetYear(), curTime.GetMonth(), curTime.GetDay());
        SaveRegFile();
        return true;
    }

    return false;
}

void CRegProc::SaveRegFile()
{
    HANDLE          hDir;
    CFile           newfile;
    //DWORD           dwSaveNum;
    string          strSaveNum;
    string          strStream;

    if (m_strRegCode.size() != MAXREGCODE)
        return;

    hDir = CreateFile(m_strRegFile.c_str(),//folder
        GENERIC_READ | GENERIC_WRITE,//readwrite
        0,//share
        NULL,//security attribute
        CREATE_ALWAYS,//创建
        FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN,//dwFlagsAndAttributes,
        NULL);//temphandle
    if (hDir == INVALID_HANDLE_VALUE)
    {
        AfxMessageBox(theApp.LoadstringFromRes(IDS_STRINFSAVEREGFAIL).c_str());
        CloseHandle(hDir);
        return;
    }
    CloseHandle(hDir);

    GenRegArrayFromStr(m_tdbReg.strMachCode, MAXMACHCODE, m_strMachine);
    GenRegArrayFromStr(m_tdbReg.strCode1, HALFREGCODE, m_strRegCode.substr(0, HALFREGCODE));
    GenRegArrayFromStr(m_tdbReg.strCode2, HALFREGCODE, m_strRegCode.substr(HALFREGCODE, HALFREGCODE));

    GenRegArrayFromStr(m_tdbReg.strGenDate, TIMESTRLEN, m_strGenDate);
    GenRegArrayFromStr(m_tdbReg.strRegDate, TIMESTRLEN, m_strRegDate);
    GenRegArrayFromStr(m_tdbReg.strUseDate, TIMESTRLEN, m_strUseDate);
    strSaveNum = string_format("%08X", m_dwTotalTorqNum);
    GenRegArrayFromStr(m_tdbReg.strTotalNum, TORQNUMLEN, strSaveNum);

    strStream = base64_encode((char*)&m_tdbReg, DBREGLEN);

    newfile.Open(m_strRegFile.c_str(), CFile::modeWrite, NULL);
    newfile.Write(strStream.c_str(), strStream.size());
    newfile.Close();
}

string CRegProc::GetRegStrFromArray(BYTE* pData, int len)
{
    char    buffer[50] = { 0 };
    string  result;

    ASSERT_NULL_R(pData, result);
    COMP_BLE_R(len, 0, result);

    memcpy(&buffer[0], pData, len);
    buffer[len] = '\0';
    result = buffer;

    StringSubtract(result, REGCODEVALUE);

    return result;

}
bool CRegProc::GenRegArrayFromStr(BYTE* pData, int len, string str)
{
    ASSERT_NULL_R(pData, false);
    COMP_BG_R(len, (int)str.size(), false);

    for (int i = 0; i < len; i++)
    {
        pData[i] = str[i] + REGCODEVALUE;
    }

    return true;
}
// 从注册文件获取实际信息
bool CRegProc::GetRegInfoFromDBREG()
{
    string strTorqNum;

    m_strMachine = GetRegStrFromArray(m_tdbReg.strMachCode, MAXMACHCODE);
    m_strRegCode = GetRegStrFromArray(m_tdbReg.strCode1, HALFREGCODE);
    m_strRegCode += GetRegStrFromArray(m_tdbReg.strCode2, HALFREGCODE);
    m_strGenDate = GetRegStrFromArray(m_tdbReg.strGenDate, TIMESTRLEN);
    m_strRegDate = GetRegStrFromArray(m_tdbReg.strRegDate, TIMESTRLEN);
    m_strUseDate = GetRegStrFromArray(m_tdbReg.strUseDate, TIMESTRLEN);

    strTorqNum = GetRegStrFromArray(m_tdbReg.strTotalNum, TIMESTRLEN);
    m_dwTotalTorqNum = strtoul(strTorqNum.c_str(), NULL, 16);
    return true;
}


void CRegProc::StringSubtract(CString& strValue, BYTE ucChar)
{
    for (int i = 0; i < strValue.GetLength(); i++)
    {
        strValue.SetAt(i, strValue.GetAt(i) - ucChar);
    }
}

void CRegProc::StringSubtract(string& strValue, BYTE ucChar)
{
    for (size_t i = 0; i < strValue.length(); i++)
    {
        strValue[i] -= ucChar;
        //strValue.SetAt(i, strValue.GetAt(i) - ucChar);
    }
}

bool CRegProc::CheckRegDate()
{
    CTime   genTime, regTime, UseTime;
    CTime   curTime = CTime::GetCurrentTime();

    if (m_tdbReg.ucYear > MAXREGYEAR)
        return false;

    if (m_strGenDate.size() != TIMESTRLEN ||
        m_strRegDate.size() != TIMESTRLEN ||
        m_strUseDate.size() != TIMESTRLEN)
        return false;

    if (m_strUseDate > m_strRegDate || m_strUseDate < m_strGenDate)
        return false;

    if (!GetDateFromString(m_strGenDate, genTime))
        return false;
    if (!GetDateFromString(m_strRegDate, regTime))
        return false;
    if (!GetDateFromString(m_strUseDate, UseTime))
        return false;

    // 生成日期和注册码日期相差整年
    if ((genTime.GetYear() + m_tdbReg.ucYear != regTime.GetYear()) ||
        (genTime.GetMonth() != regTime.GetMonth()) ||
        (genTime.GetMonth() != regTime.GetMonth()))
        return false;

    // 修改了系统日期，返回失败
    if (curTime < genTime)
        return false;

    // 系统当前日期小于最后使用时间
    if (curTime < UseTime)
        return false;

    // 超过使用期限
    // 加上使用当天
    curTime -= CTimeSpan(1, 0, 0, 0);
    if (curTime > regTime)
        return false;
    return true;
}

/* 验证注册码是否有效 */
bool CRegProc::CheckRegCode()
{
    DWORD adwRegVol[2];
    DWORD adwRegMac[2];
    DWORD adwPCVol[2];
    DWORD adwPCMac[2];
    int   iYear, iMonth, iDay;
    CTime   tNowDate;
    string  strRegDate;
    string  strNowDate;

    if (m_strRegCode.size() != MAXREGCODE)
        return false;

    /* 获取注册码中的卷和MAC信息 */
    GetVolMacFromRegStr(m_strRegCode, adwRegVol, adwRegMac, iYear, iMonth, iDay);

    /* 获取机器上的卷和MAC信息 */
    GenVolMacInfo(adwPCVol, adwPCMac, iYear, iMonth, iDay);

    if (memcmp(adwRegVol, adwPCVol, 2 * sizeof(DWORD)) != 0 ||
        memcmp(adwRegMac, adwPCMac, 2 * sizeof(DWORD)) != 0)
    {
        return false;
    }

    // 检查日期是否超过5年
    tNowDate = CTime::GetCurrentTime();
    tNowDate = CTime(tNowDate.GetYear() - VALID_YEAR,
        tNowDate.GetMonth(),
        tNowDate.GetDay(),
        0, 0, 0);
    strNowDate = tNowDate.Format("%Y%m%d");
    strRegDate = string_format("%4d%02d%02d", iYear, iMonth, iDay);

    if (strNowDate > strRegDate)
    {
        theApp.SaveMessage(strRegDate);
        return false;
    }

    return true;
}

bool CRegProc::GetVolMacFromMachine(string strMachine, DWORD pdwVol[], DWORD pdwMac[], int& iYear, int& iMonth, int& iDay)
{
    int		iBegin = 0;
    string  strTemp;

    ASSERT_NULL_R(pdwVol, false);
    ASSERT_NULL_R(pdwMac, false);
    if (strMachine.length() != MAXMACHCODE)
        return false;

    iBegin = 0;
    strTemp = strMachine.substr(iBegin, MACHVOL1LEN);
    pdwVol[0] = strtoul(strTemp.c_str(), NULL, 16);
    pdwVol[0] ^= VOL0XOR; // restore source data

    iBegin += MACHVOL1LEN;
    strTemp = strMachine.substr(iBegin, MACHMAC2LEN);
    pdwMac[1] = strtoul(strTemp.c_str(), NULL, 16);
    pdwMac[1] ^= MAC1XOR;

    iBegin += MACHMAC2LEN;
    strTemp = strMachine.substr(iBegin, MACHVOL2LEN);
    pdwVol[1] = strtoul(strTemp.c_str(), NULL, 16);
    pdwVol[1] ^= VOL1XOR;

    iBegin += MACHVOL2LEN;
    strTemp = strMachine.substr(20, MACHMAC1LEN);
    pdwMac[0] = strtoul(strTemp.c_str(), NULL, 16);
    pdwMac[0] ^= MAC0XOR;

    iYear = (pdwVol[1] & 0x00FFFF00) >> 8;
    iMonth = (pdwVol[1] & 0x000000FF);
    iDay = (pdwVol[1] & 0xFF000000) >> 24;

    if (iYear < 2020)
        return false;
    if (iMonth < 1 || iMonth > 12)
        return false;
    if (iDay < 1 || iDay > 31)
        return false;

    return true;
}

/* 从原始注册码解密运算 */
bool CRegProc::GetVolMacFromRegStr(string strRegCode, DWORD pdwVol[], DWORD pdwMac[], int& iYear, int& iMonth, int& iDay)
{
    string  strMachine;

    ASSERT_NULL_R(pdwVol, false);
    ASSERT_NULL_R(pdwMac, false);
    if (strRegCode.length() != MAXREGCODE)
        return false;

    strRegCode.pop_back();
    strRegCode.push_back('=');
    strMachine = base64_decode(strRegCode);

    return GetVolMacFromMachine(strMachine, pdwVol, pdwMac, iYear, iMonth, iDay);
}

bool CRegProc::GenVolMacInfo(DWORD pdwVol[], DWORD pdwMac[], int iYear, int iMonth, int iDay)
{
    UCHAR ucMac[5];

    ASSERT_NULL_R(pdwVol, false);
    ASSERT_NULL_R(pdwMac, false);

    GetVolumeInformation("C:\\", NULL, NULL, &pdwVol[0], NULL, NULL, NULL, 0);
    pdwVol[1] = (iDay << 24) + (iYear << 8) + iMonth;

    theApp.GetMACAddr(ucMac);
    pdwMac[0] = ((ucMac[0] * 256) + ucMac[1]) * 256 + ucMac[2];
    pdwMac[1] = (ucMac[3] * 256) + ucMac[4];

    return true;
}

bool CRegProc::CheckMachineCode()
{
    DWORD   adwPCVol[2];
    DWORD   adwPCMac[2];
    DWORD   adwMachVol[2];
    DWORD   adwMachMac[2];
    int iYear, iMonth, iDay;

    if (!GetVolMacFromMachine(m_strMachine, adwMachVol, adwMachMac, iYear, iMonth, iDay))
        return false;

    if (!GenVolMacInfo(adwPCVol, adwPCMac, iYear, iMonth, iDay))
        return false;

    if (memcmp(adwMachVol, adwPCVol, 2 * sizeof(DWORD)) == 0 &&
        memcmp(adwMachMac, adwPCMac, 2 * sizeof(DWORD)) == 0)
        return true;

    return false;
}

bool CRegProc::GetDateFromString(string date, CTime& time)
{
    string temp;
    int iYear, iMonth, iDay;
    if (date.length() != 8)
        return false;

    temp = date.substr(0, 4);
    iYear = strtoul(temp.c_str(), NULL, 10);
    if (iYear < 2020)
        return false;

    temp = date.substr(4, 2);
    iMonth = strtoul(temp.c_str(), NULL, 10);
    if (iMonth < 1 || iMonth > 12)
        return false;

    temp = date.substr(6, 2);
    iDay = strtoul(temp.c_str(), NULL, 10);
    if (iDay < 1 || iDay > 31)
        return false;

    time = CTime(iYear, iMonth, iDay, 0, 0, 0);
    return true;
}

bool CRegProc::CheckProductDate()
{
    bool    bRet = false;
    CTime   tNowDate;
    int     iRandDay = 0;
    CString strProductDate;
    CString strNowDate;

    m_tdbReg.bReged = 0;
    bRet = theApp.GetProductVersion(strProductDate);
    COMP_BE_R(bRet, false, false);

    /* 有效期5年+(31以内)随机天数 */
    tNowDate = CTime::GetCurrentTime();

    iRandDay = rand() % 31;
    tNowDate += CTimeSpan(iRandDay, 0, 0, 0);

    tNowDate = CTime(tNowDate.GetYear() - VALID_YEAR,
        tNowDate.GetMonth(),
        tNowDate.GetDay(),
        0, 0, 0);
    strNowDate = tNowDate.Format("%Y%m%d");

    if (strNowDate > strProductDate)
    {
        theApp.SaveMessage(strProductDate.GetBuffer(0));
        return false;
    }

    m_tdbReg.bReged = 1;
    return true;
}

/* 获取注册信息和校验 */
bool CRegProc::CheckAppReg()
{
    CFile           file;
    CTime           curTime = CTime::GetCurrentTime();
    DWORD           dwSaveNum;

    m_tdbReg.bReged = 0;
    m_tdbReg.bMach = 0;

    // 机器码无效
    if (!CheckMachineCode())
        return false;
    m_tdbReg.bMach = 1;

    if (!CheckRegDate())
        return false;

    /* 校验注册码 */
    /* 免注册版，直接注释掉Check */
    if (!CheckRegCode())
        return false;

    m_tdbReg.bReged = 1;

    m_strUseDate = string_format("%04d%02d%02d", curTime.GetYear(), curTime.GetMonth(), curTime.GetDay());
    GenRegArrayFromStr(m_tdbReg.strUseDate, TIMESTRLEN, m_strUseDate);

    SaveRegFile();

    return true;
}

bool CRegProc::is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

string CRegProc::base64_encode(char const* bytes_to_encode, int in_len) {
    string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += m_base64chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += m_base64chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';

    }

    return ret;
}

string CRegProc::base64_decode(string encoded_string)
{
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = (unsigned char)m_base64chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = (unsigned char)m_base64chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}

void CRegProc::SaveTorqNum()
{
    CFile           file;
    CFileFind       find;

    COMP_BFALSE(find.FindFile(m_strRegFile.c_str(), 0));

    file.Open(m_strRegFile.c_str(), CFile::modeRead | CFile::modeWrite | CFile::shareDenyNone, NULL);
    if (file.GetLength() < DBREGLEN)
    {
        file.Close();
        return;
    }

    SaveRegFile();
}