#pragma once
#include "stdafx.h"
#include "sqlite3.h"

class SqliteProc
{
public:
    SqliteProc();
    ~SqliteProc();

    int OpenDB(string filename, string pw);
    BOOL isOpen();
    void CloseDB();
    BOOL GetTable(string tablename, int &row, int &col, char*** pazResult);
    BOOL QueryTable(string tablename, string condition, int& row, int& col, char*** pazResult);
    BOOL QueryTableOrder(string tablename, string condition, string ordField, bool bASC, int& row, int& col, char*** pazResult);
    BOOL InsertRow(string tablename, vector<string> values);
    void FreeResult(char*** pazResult);
    BOOL UpdateField(string tablename, string condition, string field, string value);
    BOOL UpdateFields(string tablename, string condition, vector<string> fields, vector<string> values);
    BOOL UpdateRows(string tablename, vector<string> condition, string field, vector<string> value);
    BOOL DeleteRow(string tablename, string condition);
    BOOL GetValue(char* pResult, int &Value);
    BOOL GetValue(char* pResult, UINT & Value);
    BOOL GetValue(char* pResult, WORD & Value); 
    BOOL GetValue(char* pResult, double& Value);
    BOOL GetValue(char* pResult, string &Value);
    BOOL GetValue(char* pResult, BYTE& Value);
    BOOL ExecuteSQL(string sql);
    int  GetLastInsertID();

    string UTF82ASCII(string& strUtf8Code);         //utf-8 ת ascii 
    string ASCII2UTF8(string& strAsciiCode);       //ascii ת Utf8

private:
    sqlite3* ptCfgDB;              /* SQLite3 ���ݿ������ļ� */
    string   DBFile;

    wstring UTF82Unicode(const string& utf8string); //UTF-8תUnicode 
    string  Unicode2ASCII(wstring& wstrcode);        //unicode תΪ ascii 
    wstring ASCII2Unicode(string& strascii);        //ascii ת Unicode 
    string Unicode2UTF8(const wstring& widestring); //Unicode ת Utf8 
};

