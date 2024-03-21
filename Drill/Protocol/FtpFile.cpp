#include "FtpFile.h"


CFtpFile::CFtpFile()
{
    m_Internet = NULL;
    m_Connect = NULL;
    m_HostName.clear();
    m_UserName.clear();
    m_Password.clear();
}

CFtpFile::CFtpFile(string host, string user, string pass)
{
    m_Internet = NULL;
    m_Connect = NULL;

    m_HostName = host;
    m_UserName = user;
    m_Password = pass;
    if (m_HostName.empty())
        return;

    m_Internet = InternetOpen("WinInet Ftp", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!m_Internet)
        return;
    m_Connect = InternetConnect(m_Internet, m_HostName.c_str(), INTERNET_INVALID_PORT_NUMBER,
        m_UserName.c_str(), m_Password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
}
CFtpFile::~CFtpFile()
{
    InternetCloseHandle(m_Connect);
    InternetCloseHandle(m_Internet);
}

bool CFtpFile::Ftp_SaveToFile(string FileName, BYTE* pData, DWORD dwDataSize)
{
    HANDLE hFile = ::CreateFile(FileName.c_str(), GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_ARCHIVE, NULL);
    if (INVALID_HANDLE_VALUE == hFile)
        return false;

    DWORD dwRet = 0;
    WriteFile(hFile, pData, dwDataSize, &dwRet, NULL);
    CloseHandle(hFile);
    return true;
}

bool CFtpFile::FTP_Download(string UrlPath, string SavePath)
{
    HINTERNET hFTPFile = NULL;
    BYTE* pDownloadData = NULL;
    DWORD dwDownloadDataSize = 0;
    BYTE* pBuf = NULL;
    DWORD dwBytesReturn = 0;
    DWORD dwOffset = 0;
    BOOL bRet = FALSE;

    if (!m_Connect || m_HostName.empty())
        return false;

    hFTPFile = FtpOpenFile(m_Connect, UrlPath.c_str(), GENERIC_READ, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, NULL);
    dwDownloadDataSize = FtpGetFileSize(hFTPFile, NULL);
    pDownloadData = new BYTE[dwDownloadDataSize];

    RtlZeroMemory(pDownloadData, dwDownloadDataSize);
    pBuf = new BYTE[4096];
    RtlZeroMemory(pBuf, 4096);
    do
    {
        bRet = InternetReadFile(hFTPFile, pBuf, 4096, &dwBytesReturn);
        if (FALSE == bRet)
            break;
        RtlCopyMemory((pDownloadData + dwOffset), pBuf, dwBytesReturn);
        dwOffset = dwOffset + dwBytesReturn;
    } while (dwDownloadDataSize > dwOffset);

    Ftp_SaveToFile(SavePath, pDownloadData, dwDownloadDataSize);
    delete[]pDownloadData;
    pDownloadData = NULL;
    return true;
}

// 逐级创建文件夹目录
bool CFtpFile::FTP_CreateDirectory(string path)
{
    int pos = 1;
    string tempPath;
    //WIN32_FIND_DATA pDirInfo;
    //HINTERNET hDir;
    //DWORD dwFlag = 0;

    if (!m_Connect || path.empty())
        return false;

    while (pos > 0)
    {
        // 从第二个“/”开始依次找到目的路径中的“/”位置
        pos = path.find_first_of('/', pos + 1);
        if (pos == -1)
            break;
        tempPath = path.substr(0, pos + 1);
        /*hDir = FtpFindFirstFile(m_Connect, tempPath.c_str(),
            &pDirInfo, dwFlag, 0);
        if (!hDir)*/
        if (_access(tempPath.c_str(), 0) == -1)
        {
            FtpCreateDirectory(m_Connect, tempPath.c_str());
        }
    }

    return true;
}

bool CFtpFile::FTP_Upload(string UrlPath, string FilePath)
{
    HINTERNET hFTPFile = NULL;
    DWORD dwBytesReturn = 0;
    DWORD UploadDataSize = 0;
    BYTE* pUploadData = NULL;
    DWORD dwRet, bRet = 0;


    if (!m_Connect || UrlPath.empty() || FilePath.empty())
        return false;

    hFTPFile = ::FtpOpenFile(m_Connect, UrlPath.c_str(), GENERIC_WRITE, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, NULL);
    HANDLE hFile = ::CreateFile(FilePath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ |
        FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
    if (INVALID_HANDLE_VALUE == hFile)
        return false;

    UploadDataSize = ::GetFileSize(hFile, NULL);
    pUploadData = new BYTE[UploadDataSize];
    ::ReadFile(hFile, pUploadData, UploadDataSize, &dwRet, NULL);
    UploadDataSize = dwRet;

    bRet = ::InternetWriteFile(hFTPFile, pUploadData, UploadDataSize, &dwBytesReturn);
    if (FALSE == bRet)
    {
        delete[]pUploadData;
        return false;
    }
    delete[]pUploadData;
    return true;
}
