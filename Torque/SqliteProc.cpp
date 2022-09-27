#include "SqliteProc.h"

SqliteProc::SqliteProc()
{
    ptCfgDB = NULL;
    DBFile = "";
}

SqliteProc::~SqliteProc()
{
    CloseDB();
}

//UTF-8转Unicode 
wstring SqliteProc::UTF82Unicode(const string& utf8string)
{
    int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0);
    if (widesize == ERROR_NO_UNICODE_TRANSLATION)
    {
        throw exception("Invalid UTF-8 sequence.");
    }
    if (widesize == 0)
    {
        throw exception("Error in conversion.");
    }
    vector<wchar_t> resultstring(widesize);
    int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &resultstring[0], widesize);
    if (convresult != widesize)
    {
        throw exception("La falla!");
    }
    return wstring(&resultstring[0]);
}

//unicode 转为 ascii 
string SqliteProc::Unicode2ASCII(wstring& wstrcode)
{
    int asciisize = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, NULL, 0, NULL, NULL);
    if (asciisize == ERROR_NO_UNICODE_TRANSLATION)
    {
        throw exception("Invalid UTF-8 sequence.");
    }
    if (asciisize == 0)
    {
        throw exception("Error in conversion.");
    }
    vector<char> resultstring(asciisize);
    int convresult = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, &resultstring[0], asciisize, NULL, NULL);
    if (convresult != asciisize)
    {
        throw exception("La falla!");
    }
    return string(&resultstring[0]);
}

//utf-8 转 ascii 
string SqliteProc::UTF82ASCII(string& strUtf8Code)
{
    string strRet = "";
    //先把 utf8 转为 unicode 
    wstring wstr = UTF82Unicode(strUtf8Code);
    //最后把 unicode 转为 ascii 
    strRet = Unicode2ASCII(wstr);
    return strRet;
}

//ascii 转 Unicode 
wstring SqliteProc::ASCII2Unicode(string& strascii)
{
    int widesize = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, NULL, 0);
    if (widesize == ERROR_NO_UNICODE_TRANSLATION)
    {
        throw exception("Invalid UTF-8 sequence.");
    }
    if (widesize == 0)
    {
        throw exception("Error in conversion.");
    }
    vector<wchar_t> resultstring(widesize);
    int convresult = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, &resultstring[0], widesize);
    if (convresult != widesize)
    {
        throw exception("La falla!");
    }
    return wstring(&resultstring[0]);
}

//Unicode 转 Utf8 
string SqliteProc::Unicode2UTF8(const wstring& widestring)
{
    int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);
    if (utf8size == 0)
    {
        throw exception("Error in conversion.");
    }
    vector<char> resultstring(utf8size);
    int convresult = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);
    if (convresult != utf8size)
    {
        throw exception("La falla!");
    }
    return string(&resultstring[0]);
}

//ascii 转 Utf8 
string SqliteProc::ASCII2UTF8(string& strAsciiCode)
{
    string strRet("");
    //先把 ascii 转为 unicode 
    wstring wstr = ASCII2Unicode(strAsciiCode);
    //最后把 unicode 转为 utf8 
    strRet = Unicode2UTF8(wstr);
    return strRet;
}

BOOL SqliteProc::OpenDB(string filename, string pw)
{
    string inputkey;
    char* pErrMsg = NULL;

    if (SQLITE_OK != sqlite3_open(filename.c_str(), &ptCfgDB))
    {
        CloseDB();
        return FALSE;
    }

    // 设置密码
    //sqlite3_key(ptCfgDB, pw.c_str(), pw.size());

    // 输入密码
    inputkey = "PRAGMA key='" + pw + "';";
    ExecuteSQL(inputkey);

    int result = sqlite3_exec(ptCfgDB, "SELECT count(*) FROM sqlite_master;", NULL, 0, &pErrMsg);
    if (result != SQLITE_OK)
    {
        sqlite3_free(pErrMsg);
        return FALSE;
    }

    return TRUE;
}

BOOL SqliteProc::isOpen()
{
    return (NULL != ptCfgDB);
}

void SqliteProc::CloseDB()
{
    if (NULL != ptCfgDB)
    {
        sqlite3_close(ptCfgDB);
        ptCfgDB = NULL;
    }

    DBFile = "";
}

BOOL SqliteProc::ExecuteSQL(string sql)
{
    char* pErrMsg = NULL;

    if (sqlite3_exec(ptCfgDB, sql.c_str(), NULL, 0, &pErrMsg) != SQLITE_OK)
    {
        sqlite3_free(pErrMsg);
        return FALSE;
    }

    return TRUE;
}


int  SqliteProc::GetLastInsertID()
{
    ASSERT_NULL_R(ptCfgDB, DB_INVALID_VAL);
    return (int)sqlite3_last_insert_rowid(ptCfgDB);
}

BOOL SqliteProc::GetTable(string tablename, int& row, int& col, char*** pazResult)
{
    int res = SQLITE_OK;
    string sql;
    char* pErrMsg = NULL;

    if (!isOpen())
        return FALSE;

    sql = "SELECT * FROM " + tablename;

    res = sqlite3_get_table(ptCfgDB, sql.c_str(), pazResult, &row, &col, &pErrMsg);
    if (res != SQLITE_OK)
    {
        sqlite3_free(pErrMsg);
        return FALSE;
    }

    return TRUE;
}

BOOL SqliteProc::QueryTable(string tablename, string condition, int& row, int& col, char*** pazResult)
{
    int res = SQLITE_OK;
    string sql;
    char* errmsg = NULL;

    if (!isOpen())
        return FALSE;

    sql = "SELECT * FROM " + tablename;
    if (!condition.empty())
        sql += " WHERE " + ASCII2UTF8(condition) +";";

    res = sqlite3_get_table(ptCfgDB, sql.c_str(), pazResult, &row, &col, &errmsg);
    if (res != SQLITE_OK)
    {
        sqlite3_free(errmsg);
        return FALSE;
    }

    return TRUE;
}

BOOL SqliteProc::InsertRow(string tablename, vector<string> values)
{
    string content;
    string sql;
    char* pErrMsg = 0;
    int i = 0;

    ASSERT_ZERO_R(tablename.size(), FALSE);
    ASSERT_ZERO_R(values.size(), FALSE);

    for (i = 0; i < (int)values.size(); i++)
    {
        if (!content.empty())
            content += ",";
        if (values[i].empty())
            content += "NULL";
        else
            content += "'" + ASCII2UTF8(values[i]) + "'";
    }

    sql = "INSERT INTO " + tablename + " VALUES(" + content + ");";
    if (sqlite3_exec(ptCfgDB, sql.c_str(), NULL, 0, &pErrMsg) != SQLITE_OK)
    {
        sqlite3_free(pErrMsg);
        return FALSE;
    }

    return TRUE;
}

BOOL SqliteProc::GetValue(char* pResult, int& Value)
{
    ASSERT_NULL_R(pResult, FALSE);

    Value = atoi(pResult);

    return TRUE;
}

BOOL SqliteProc::GetValue(char* pResult, UINT& Value)
{
    ASSERT_NULL_R(pResult, FALSE);

    Value = atoi(pResult);

    return TRUE;
}

BOOL SqliteProc::GetValue(char* pResult, double& Value)
{
    ASSERT_NULL_R(pResult, FALSE);

    Value = atof(pResult);

    return TRUE;
}

BOOL SqliteProc::GetValue(char* pResult, WORD& Value)
{
    ASSERT_NULL_R(pResult, FALSE);

    Value = atoi(pResult);

    return TRUE;
}

BOOL SqliteProc::GetValue(char* pResult, string& Value)
{
    Value.clear();
    ASSERT_NULL_R(pResult, FALSE);

    Value = pResult;
    Value = UTF82ASCII(Value);

    return TRUE;
}

void SqliteProc::FreeResult(char*** pazResult)
{
    sqlite3_free_table(*pazResult);
}

BOOL SqliteProc::UpdateField(string tablename, string condition, string field, string value)
{
    string content;
    string sql;
    char* pErrMsg = 0;
    if (field.empty() )
        return false;
    if (value.empty())
        content = field + "=NULL";
    else
        content = field + "='" + ASCII2UTF8(value) + "'";
    sql = "UPDATE " + tablename + " Set " + content;
    if (!condition.empty())
        sql += " WHERE " + ASCII2UTF8(condition);
    if (sqlite3_exec(ptCfgDB, sql.c_str(), NULL, 0, &pErrMsg) != SQLITE_OK)
    {
        sqlite3_free(pErrMsg);
        return FALSE;
    }

    return TRUE;
}
BOOL SqliteProc::UpdateFields(string tablename, string condition, vector<string> fields, vector<string> values)
{
    string content;
    string sql;
    char* pErrMsg = 0;
    int i = 0;

    ASSERT_ZERO_R(fields.size(), FALSE);
    if (fields.size() != values.size())
        return FALSE;

    for(i=0;i<(int)fields.size();i++)
    {
        if (!content.empty())
            content += ",";

        if (values[i].empty())
            content += fields[i] + "=NULL";
        else
            content += fields[i] + "='" + ASCII2UTF8(values[i]) + "'";
    }

    sql = "UPDATE " + tablename + " Set " + content;
    if (!condition.empty())
        sql += " WHERE " + ASCII2UTF8(condition);
    if (sqlite3_exec(ptCfgDB, sql.c_str(), NULL, 0, &pErrMsg) != SQLITE_OK)
    {
        sqlite3_free(pErrMsg);
        return FALSE;
    }

    return TRUE;
}

/* 更新多行数据的同一个field的值 */
BOOL SqliteProc::UpdateRows(string tablename, vector<string> conditions, string field, vector<string> values)
{
    string content;
    string sql;
    char* pErrMsg = 0;
    int i;

    ASSERT_ZERO_R(conditions.size(), FALSE);
    if (conditions.size() != values.size())
        return FALSE;

    sql = "BEGIN;\n";
    for(i=0;i<(int)conditions.size();i++)
    {
        if (values[i].empty())
            content = field + "=NULL";
        else
            content = field + "='" + ASCII2UTF8(values[i]) + "'";
        sql += "UPDATE " + tablename + " Set " + content + " WHERE " + ASCII2UTF8(conditions[i]) + ";\n";
    }
    sql = "COMMIT;";

    if (sqlite3_exec(ptCfgDB, sql.c_str(), NULL, 0, &pErrMsg) != SQLITE_OK)
    {
        sqlite3_free(pErrMsg);
        return FALSE;
    }
    return TRUE;
}

/* 删除满足条件的数据 */
BOOL SqliteProc::DeleteRow(string tablename, string condition)
{
    string sql;
    char* pErrMsg = 0;
    int i = 0;

    ASSERT_ZERO_R(tablename.size(), FALSE);

    sql = "DELETE FROM " + tablename + " WHERE " + ASCII2UTF8(condition) + ";\n";
    if (sqlite3_exec(ptCfgDB, sql.c_str(), NULL, 0, &pErrMsg) != SQLITE_OK)
    {
        sqlite3_free(pErrMsg);
        return FALSE;
    }

    return TRUE;
}