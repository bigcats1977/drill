#pragma once
#include "stdafx.h"
#include "sqlite3.h"

class SqliteProc
{
public:
    SqliteProc();
    ~SqliteProc();

    bool OpenDB(string filename, string pw);
    bool isOpen();
    void CloseDB();
    bool GetTable(string tablename, int& row, int& col, char*** pazResult);
    bool QueryTable(string tablename, string condition, int& row, int& col, char*** pazResult);
    bool QueryTableOrder(string tablename, string condition, string ordField, bool bASC, int& row, int& col, char*** pazResult);
    bool InsertRow(string tablename, vector<string> values);
    void FreeResult(char*** pazResult);
    bool UpdateField(string tablename, string condition, string field, string value);
    bool UpdateFields(string tablename, string condition, vector<string> fields, vector<string> values);
    bool UpdateRows(string tablename, vector<string> condition, string field, vector<string> value);
    bool DeleteRow(string tablename, string condition);
    bool GetValue(char* pResult, bool& Value);
    bool GetValue(char* pResult, int& Value);
    bool GetValue(char* pResult, UINT& Value);
    bool GetValue(char* pResult, WORD& Value);
    bool GetValue(char* pResult, double& Value);
    bool GetValue(char* pResult, string& Value);
    bool GetValue(char* pResult, BYTE& Value);
    bool ExecuteSQL(string sql);
    int  GetLastInsertID();


private:
    sqlite3* ptCfgDB;              /* SQLite3 数据库配置文件 */
    string   DBFile;
};