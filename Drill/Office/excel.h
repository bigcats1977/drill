#pragma once
#include "CApplication.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "CChart.h"
#include "CShapes.h"
#include "CShape.h"
#include "CChartObjects.h"
#include "CChartObject.h"
#include "CFont0.h"

class Excel
{
public:
    Excel();
    ~Excel();

    void show(bool bShow);
    //检查一个cell是否为字符串
    bool isCellString(long iRow, long iColumn);
    //检查一个cell是否为数值
    bool isCellInt(long iRow, long iColumn);
    //得到一个cell的string
    CString getCellString(long iRow, long iColumn);
    //得到一个cell的总数
    int getCellInt(long iRow, long iColumn);
    //得到一个cell的double数据
    double getCellDouble(long iRow, long iColumn);
    //取得行的总数
    int getRowCount();
    //取得列的总数
    int getColumnCount();
    //使用某个shell
    bool loadSheet(long tableId, bool preLoaded = false);
    bool loadSheet(CString sheet, bool preLoaded = false);

    //通过序号取得某个sheet的名称
    CString getSheetName(long tableID);

    //得到sheet的总数
    int getSheetCount();

    //打开excel文件
    bool open(CString fileName);

    //关闭打开的excel文件
    void close(bool ifSave = false);

    //另存为一个excel文件
    void saveAsXLSFile(const string& xlsFile);

    //取得打开文件的名称
    CString getOpenFileName();

    //取得打开sheel的名称
    CString getOpenSheelName();

    //向cell中写入一个int值
    void setCellInt(long iRow, long iColumn, int newInt);

    //向cell中写入一个字符串
    void setCellString(long iRow, long iColumn, CString newString);

    //向cell中添加一个图片
    CShape addCellPicture(CString strFileName, float fLeft, float fTop, float fWidth, float fHeight);
    CShape addCellPicture(CString strFileName, CString top, CString bottom);

    // 拷贝多行
    void   copyMultiRow(UINT nSrcRow, UINT nDestRow, UINT nBeginCol, UINT nEndCol, UINT nNum);

    // 设置行高为当前的多少倍
    void   setMultiRowHeight(UINT nRow, UINT nBeginCol, UINT nEndCol, float fMulti);

    // 设置一行多个单元格的字体和颜色
    void   setMultiColFont(UINT nRow, UINT nCol, UINT nNUm, CString strFontName, long nSize, long nColor = 0);

    //删除非指定名称的sheet
    void delNOTNameSheet(CString strSheetName);

    //删除指定名称的sheet
    void delNameSheet(CString strSheetName);

    // 写入连续的多个单元格内容
    bool SetMultiCellContent(UINT nBeginRow, UINT nEndRow, UINT nBeginCol, UINT nEndCol, CStringList& slContent);

public:
    //初始化 Excel_OLE
    static bool initExcel();

    //释放Excel_OLE
    static void release();

    //取得列的名称
    static char* getColumnName(long iColumn);

protected:
    void preLoadSheet();

private:
    CString openFileName;
    CWorkbook workBook;           //当前处理的文件
    CWorkbooks books;             //ExcelBook集合，多文件时使用
    CWorksheet workSheet;         //当前使用sheet
    CWorksheets sheets;           //Excel的sheet集合
    CRange wholeRange;            //全部操作区域
    LPDISPATCH lpDisp;

    bool isLoad;                   //是否已经加载了某个sheet数据
    COleSafeArray safeArray;

    int titleToColumn(CString title);
    CString columnToTitle(int column);

protected:
    static CApplication application;   //Excel进程实例
};