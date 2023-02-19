#pragma once

#include <string>
#include <stdio.h>
#include <Windows.h>
#include <WinInet.h>
#include <io.h>


using namespace std;

#pragma comment(lib, "WinInet.lib")

class CFtpFile
{
public:
    CFtpFile();
    CFtpFile(string host, string user, string pass);
    ~CFtpFile();

    bool FTP_CreateDirectory(string path);
    bool Ftp_SaveToFile(string FileName, BYTE* pData, DWORD dwDataSize);
    bool FTP_Download(string UrlPath, string SavePath);
    bool FTP_Upload(string UrlPath, string FilePath);

private:
    HINTERNET m_Connect;
    HINTERNET m_Internet;

    string m_HostName;
    string m_UserName;
    string m_Password;
};

