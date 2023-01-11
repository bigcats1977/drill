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


bool SqliteProc::OpenDB(string filename, string pw)
{
    string inputkey;
    char* pErrMsg = NULL;

    if (SQLITE_OK != sqlite3_open(ASCII2UTF8(filename).c_str(), &ptCfgDB))
    {
        CloseDB();
        return false;
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
        return false;
    }

    return true;
}

bool SqliteProc::isOpen()
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

bool SqliteProc::ExecuteSQL(string sql)
{
    char* pErrMsg = NULL;

    if (sqlite3_exec(ptCfgDB, sql.c_str(), NULL, 0, &pErrMsg) != SQLITE_OK)
    {
        sqlite3_free(pErrMsg);
        return false;
    }

    return true;
}


int  SqliteProc::GetLastInsertID()
{
    ASSERT_NULL_R(ptCfgDB, DB_INVALID_VAL);
    return (int)sqlite3_last_insert_rowid(ptCfgDB);
}

bool SqliteProc::GetTable(string tablename, int& row, int& col, char*** pazResult)
{
    int res = SQLITE_OK;
    string sql;
    char* pErrMsg = NULL;

    if (!isOpen())
        return false;

    sql = "SELECT * FROM " + tablename;

    res = sqlite3_get_table(ptCfgDB, sql.c_str(), pazResult, &row, &col, &pErrMsg);
    if (res != SQLITE_OK)
    {
        sqlite3_free(pErrMsg);
        return false;
    }

    return true;
}

bool SqliteProc::QueryTable(string tablename, string condition, int& row, int& col, char*** pazResult)
{
    int res = SQLITE_OK;
    string sql;
    char* errmsg = NULL;

    if (!isOpen())
        return false;

    sql = "SELECT * FROM " + tablename;
    if (!condition.empty())
        sql += " WHERE " + ASCII2UTF8(condition);
    sql += ";";

    res = sqlite3_get_table(ptCfgDB, sql.c_str(), pazResult, &row, &col, &errmsg);
    if (res != SQLITE_OK)
    {
        sqlite3_free(errmsg);
        return false;
    }

    return true;
}

bool SqliteProc::QueryTableOrder(string tablename, string condition, string ordField, bool bASC, int& row, int& col, char*** pazResult)
{
    int res = SQLITE_OK;
    string sql;
    char* errmsg = NULL;

    if (!isOpen())
        return false;

    sql = "SELECT * FROM " + tablename;
    if (!condition.empty())
        sql += " WHERE " + ASCII2UTF8(condition) + " ";
    if (bASC)
        sql += "ORDER BY " + ordField + " ASC";
    else
        sql += "ORDER BY " + ordField + " DESC";
    sql += ";";

    res = sqlite3_get_table(ptCfgDB, sql.c_str(), pazResult, &row, &col, &errmsg);
    if (res != SQLITE_OK)
    {
        sqlite3_free(errmsg);
        return false;
    }

    return true;
}

bool SqliteProc::InsertRow(string tablename, vector<string> values)
{
    string content;
    string sql;
    char* pErrMsg = 0;
    int i = 0;

    ASSERT_ZERO_R(tablename.size(), false);
    ASSERT_ZERO_R(values.size(), false);

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
        return false;
    }

    return true;
}

bool SqliteProc::GetValue(char* pResult, bool& Value)
{
    ASSERT_NULL_R(pResult, false);

    if (atoi(pResult) == 0)
        Value = false;
    else
        Value = true;

    return true;
}

bool SqliteProc::GetValue(char* pResult, BYTE& Value)
{
    ASSERT_NULL_R(pResult, false);

    Value = (BYTE)atoi(pResult);

    return true;
}

bool SqliteProc::GetValue(char* pResult, int& Value)
{
    ASSERT_NULL_R(pResult, false);

    Value = atoi(pResult);

    return true;
}

bool SqliteProc::GetValue(char* pResult, UINT& Value)
{
    ASSERT_NULL_R(pResult, false);

    Value = atoi(pResult);

    return true;
}

bool SqliteProc::GetValue(char* pResult, double& Value)
{
    ASSERT_NULL_R(pResult, false);

    Value = atof(pResult);

    return true;
}

bool SqliteProc::GetValue(char* pResult, WORD& Value)
{
    ASSERT_NULL_R(pResult, false);

    Value = atoi(pResult);

    return true;
}

bool SqliteProc::GetValue(char* pResult, string& Value)
{
    Value.clear();
    ASSERT_NULL_R(pResult, false);

    Value = pResult;
    Value = UTF82ASCII(Value);

    return true;
}

void SqliteProc::FreeResult(char*** pazResult)
{
    sqlite3_free_table(*pazResult);
}

bool SqliteProc::UpdateField(string tablename, string condition, string field, string value)
{
    string content;
    string sql;
    char* pErrMsg = 0;
    if (field.empty())
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
        return false;
    }

    return true;
}
bool SqliteProc::UpdateFields(string tablename, string condition, vector<string> fields, vector<string> values)
{
    string content;
    string sql;
    char* pErrMsg = 0;
    int i = 0;

    ASSERT_ZERO_R(fields.size(), false);
    if (fields.size() != values.size())
        return false;

    for (i = 0; i < (int)fields.size(); i++)
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
        return false;
    }

    return true;
}

/* 更新多行数据的同一个field的值 */
bool SqliteProc::UpdateRows(string tablename, vector<string> conditions, string field, vector<string> values)
{
    string content;
    string sql;
    char* pErrMsg = 0;
    int i;

    ASSERT_ZERO_R(conditions.size(), false);
    if (conditions.size() != values.size())
        return false;

    sql = "BEGIN;\n";
    for (i = 0; i < (int)conditions.size(); i++)
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
        return false;
    }
    return true;
}

/* 删除满足条件的数据 */
bool SqliteProc::DeleteRow(string tablename, string condition)
{
    string sql;
    char* pErrMsg = 0;
    int i = 0;

    ASSERT_ZERO_R(tablename.size(), false);

    sql = "DELETE FROM " + tablename + " WHERE " + ASCII2UTF8(condition) + ";\n";
    if (sqlite3_exec(ptCfgDB, sql.c_str(), NULL, 0, &pErrMsg) != SQLITE_OK)
    {
        sqlite3_free(pErrMsg);
        return false;
    }

    return true;
}