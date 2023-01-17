//Excel.cpp
#include "stdafx.h"
#include <tchar.h>
#include "Excel.h"

#if 1
COleVariant
covTrue((short)TRUE),
covFalse((short)FALSE),
covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
#endif
CApplication Excel::application;

Excel::Excel() :isLoad(false)
{
}


Excel::~Excel()
{
    close();
}


bool Excel::initExcel()
{
    //创建Excel 2000服务器(启动Excel)
    if (!application.CreateDispatch(_T("Excel.Application"), nullptr))
    {
        //MessageBox(nullptr, _T("创建Excel服务失败,你可能没有安装EXCEL，请检查!"), _T("错误"), MB_OK);
        return FALSE;
    }

    application.put_DisplayAlerts(FALSE);
    return true;
}


void Excel::release()
{
    application.Quit();
    application.ReleaseDispatch();
    application = nullptr;
}

bool Excel::open(const char* fileName)
{

    //先关闭文件
    close();

    //利用模板建立新文档
    books.AttachDispatch(application.get_Workbooks(), true);


    LPDISPATCH lpDis = nullptr;
    try {
        lpDis = books.Add(COleVariant(CString(fileName)));
    }
    catch (CException* e)
    {
        // 找不到则新创建一个
        lpDis = books.Add(vtMissing);
    }

    if (lpDis)
    {
        workBook.AttachDispatch(lpDis);

        sheets.AttachDispatch(workBook.get_Worksheets());

        openFileName = fileName;
        return true;
    }

    return false;
}

void Excel::close(bool ifSave)
{
    //如果文件已经打开，关闭文件
    if (!openFileName.IsEmpty())
    {
        //如果保存,交给用户控制,让用户自己存，如果自己SAVE，会出现莫名的等待
        if (ifSave)
        {
            //show(true);
        }
        else
        {
            workBook.Close(COleVariant(short(FALSE)), COleVariant(openFileName), covOptional);
            books.Close();
        }

        //清空打开文件名称
        openFileName.Empty();
    }


    sheets.ReleaseDispatch();
    workSheet.ReleaseDispatch();
    currentRange.ReleaseDispatch();
    workBook.ReleaseDispatch();
    books.ReleaseDispatch();
}

void Excel::saveAsXLSFile(const string& xlsFile)
{
    // 56: xlExcel8    xls 
    // 51: xlWorkbookDefault   xlsx  
    workBook.SaveAs(COleVariant(xlsFile.c_str()),
        _variant_t(51),
        covOptional,
        covOptional,
        covOptional,
        covOptional,
        0,
        covOptional,
        covOptional,
        covOptional,
        covOptional,
        covOptional);
    return;
}


int Excel::getSheetCount()
{
    return sheets.get_Count();
}

CString Excel::getSheetName(long tableID)
{
    CWorksheet sheet;
    sheet.AttachDispatch(sheets.get_Item(COleVariant((long)tableID)));
    CString name = sheet.get_Name();
    sheet.ReleaseDispatch();
    return name;
}

void Excel::preLoadSheet()
{
    CRange used_range;

    used_range = workSheet.get_UsedRange();


    VARIANT ret_ary = used_range.get_Value2();
    if (!(ret_ary.vt & VT_ARRAY))
    {
        return;
    }
    //
    safeArray.Clear();
    safeArray.Attach(ret_ary);
}

//按照名称加载sheet表格，也可提前加载所有表格
bool Excel::loadSheet(long tableId, bool preLoaded)
{
    LPDISPATCH lpDis = nullptr;
    currentRange.ReleaseDispatch();
    currentRange.ReleaseDispatch();
    lpDis = sheets.get_Item(COleVariant((long)tableId));
    if (lpDis)
    {
        workSheet.AttachDispatch(lpDis, true);
        currentRange.AttachDispatch(workSheet.get_Cells(), true);
    }
    else
    {
        return false;
    }

    isLoad = false;
    //如果进行预先加载
    if (preLoaded)
    {
        preLoadSheet();
        isLoad = true;
    }

    return true;
}


bool Excel::loadSheet(CString sheet, bool preLoaded)
{
    LPDISPATCH lpDis = nullptr;
    currentRange.ReleaseDispatch();
    currentRange.ReleaseDispatch();

    lpDis = sheets.get_Item(COleVariant(sheet));
    if (lpDis)
    {
        workSheet.AttachDispatch(lpDis, true);
        currentRange.AttachDispatch(workSheet.get_Cells(), true);
    }
    else
    {
        return false;
    }

    isLoad = false;
    //如果进行预先加载
    if (preLoaded)
    {
        preLoadSheet();
        isLoad = true;
    }

    return true;
}


int Excel::getColumnCount()
{
    CRange range;
    CRange usedRange;

    usedRange.AttachDispatch(workSheet.get_UsedRange(), true);
    range.AttachDispatch(usedRange.get_Columns(), true);
    int count = range.get_Count();

    usedRange.ReleaseDispatch();
    range.ReleaseDispatch();

    return count;
}

int Excel::getRowCount()
{
    CRange range;
    CRange usedRange;

    usedRange.AttachDispatch(workSheet.get_UsedRange(), true);
    range.AttachDispatch(usedRange.get_Rows(), true);

    int count = range.get_Count();

    usedRange.ReleaseDispatch();
    range.ReleaseDispatch();

    return count;
}

bool Excel::isCellString(long iRow, long iColumn)
{
    CRange range;
    range.AttachDispatch(currentRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
    COleVariant vResult = range.get_Value2();
    //VT_BSTR标示字符串
    if (vResult.vt == VT_BSTR)
    {
        return true;
    }
    return false;
}


bool Excel::isCellInt(long iRow, long iColumn)
{

    CRange range;
    range.AttachDispatch(currentRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
    COleVariant vResult = range.get_Value2();
    //VT_BSTR标示字符串
    if (vResult.vt == VT_INT || vResult.vt == VT_R8)
    {
        return true;
    }
    return false;
}

CString Excel::getCellString(long iRow, long iColumn)
{
    COleVariant vResult;
    CString str;
    //字符串
    if (isLoad == false)
    {
        CRange range;
        range.AttachDispatch(currentRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
        vResult = range.get_Value2();
        range.ReleaseDispatch();
    }
    //如果数据依据预先加载了
    else
    {
        long read_address[2];
        VARIANT val;
        read_address[0] = iRow;
        read_address[1] = iColumn;
        safeArray.GetElement(read_address, &val);
        vResult = val;
    }

    if (vResult.vt == VT_BSTR)
    {
        str = vResult.bstrVal;
    }
    //整数
    else if (vResult.vt == VT_INT)
    {
        str.Format(_T("%d"), vResult.pintVal);
    }
    //8字节的数字
    else if (vResult.vt == VT_R8)
    {
        str.Format(_T("%0.0f"), vResult.dblVal);
    }
    //时间格式
    else if (vResult.vt == VT_DATE)
    {
        SYSTEMTIME st;
        VariantTimeToSystemTime(vResult.date, &st);
        CTime tm(st);
        str = tm.Format(_T("%Y-%m-%d"));

    }
    //单元格空的
    else if (vResult.vt == VT_EMPTY)
    {
        str = "";
    }

    return str;
}

double Excel::getCellDouble(long iRow, long iColumn)
{
    double rtn_value = 0;
    COleVariant vresult;
    //字符串
    if (isLoad == false)
    {
        CRange range;
        range.AttachDispatch(currentRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
        vresult = range.get_Value2();
        range.ReleaseDispatch();
    }
    //如果数据依据预先加载了
    else
    {
        long read_address[2];
        VARIANT val;
        read_address[0] = iRow;
        read_address[1] = iColumn;
        safeArray.GetElement(read_address, &val);
        vresult = val;
    }

    if (vresult.vt == VT_R8)
    {
        rtn_value = vresult.dblVal;
    }

    return rtn_value;
}

int Excel::getCellInt(long iRow, long iColumn)
{
    int num;
    COleVariant vresult;

    if (isLoad == FALSE)
    {
        CRange range;
        range.AttachDispatch(currentRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
        vresult = range.get_Value2();
        range.ReleaseDispatch();
    }
    else
    {
        long read_address[2];
        VARIANT val;
        read_address[0] = iRow;
        read_address[1] = iColumn;
        safeArray.GetElement(read_address, &val);
        vresult = val;
    }
    //
    num = static_cast<int>(vresult.dblVal);

    return num;
}

void Excel::setCellString(long iRow, long iColumn, CString newString)
{
    COleVariant new_value(newString);
    CRange start_range = workSheet.get_Range(COleVariant(_T("A1")), covOptional);
    CRange write_range = start_range.get_Offset(COleVariant((long)iRow - 1), COleVariant((long)iColumn - 1));
    write_range.put_Value2(new_value);
    start_range.ReleaseDispatch();
    write_range.ReleaseDispatch();
}

void Excel::setCellInt(long iRow, long iColumn, int newInt)
{
    COleVariant new_value((long)newInt);
    CRange start_range = workSheet.get_Range(COleVariant(_T("A1")), covOptional);
    CRange write_range = start_range.get_Offset(COleVariant((long)iRow - 1), COleVariant((long)iColumn - 1));
    write_range.put_Value2(new_value);
    start_range.ReleaseDispatch();
    write_range.ReleaseDispatch();
}

CShape Excel::addCellPicture(CString strFileName, float fLeft, float fTop, float fWidth, float fHeight)
{
    CShapes     shapes;
    CShape      shape;
    shapes.AttachDispatch(workSheet.get_Shapes(), TRUE);
    shape.AttachDispatch(shapes.AddPicture(strFileName, false, true, fLeft, fTop, fWidth, fHeight), TRUE);
    return shape;
}

void Excel::copyMultiRow(UINT nSrcRow, UINT nDestRow, UINT nNum)
{
    CRange      rangeBegin, rangeEnd;
    CString     StartCell, EndCell;

    StartCell.Format("A%d", nSrcRow);
    EndCell.Format("M%d", nSrcRow + nNum);
    rangeBegin.AttachDispatch(workSheet.get_Range(COleVariant(StartCell), COleVariant(EndCell)));


    /* 拷贝目的位置 */
    StartCell.Format("A%d", nDestRow);
    EndCell.Format("M%d", nDestRow + 4);
    rangeEnd.AttachDispatch(workSheet.get_Range(COleVariant(StartCell), COleVariant(EndCell)));
    COleVariant varRange;
    varRange.vt = VT_DISPATCH;
    V_DISPATCH(&varRange) = (LPDISPATCH)rangeEnd;
    V_DISPATCH(&varRange)->AddRef();
    rangeBegin.Copy(varRange);

    rangeEnd.ReleaseDispatch();
    rangeBegin.ReleaseDispatch();
}

void Excel::setMultiRowHeight(UINT nRow, float fMulti)
{
    CRange      range;
    CString     StartCell, EndCell;

    StartCell.Format("A%d", nRow);
    EndCell.Format("M%d", nRow);
    range.AttachDispatch(workSheet.get_Range(COleVariant(StartCell), COleVariant(EndCell)));
    /* 设置油管详情行高为2倍 */
    VARIANT rowHeight = range.get_RowHeight();
    range.put_RowHeight((_variant_t)(fMulti * rowHeight.dblVal));
    range.ReleaseDispatch();
}

void Excel::setMultiColFont(UINT nRow, UINT nCol, UINT nNUm, CString strFontName, long nSize, long nColor)
{
    UINT        i = 0;
    CRange      range;
    CFont0      ftColor;    // 设置字体颜色
    CString     strCell;
    _variant_t  vRange1;    // 设置单元格的边框
    _variant_t  vRange2;
    _variant_t  vRange3;
    _variant_t  vRange4;

    vRange1.vt = VT_I2;
    vRange1.lVal = 1;       // 线的样式：0- no line; 1-solid; 2-big dot;3-small dot;4-dash dot; 5-dash dot dot; xlContinuous 1
    vRange2.vt = VT_I2;
    vRange2.lVal = 1;       // 线的粗细程度；
    vRange3.vt = VT_I2;
    vRange3.lVal = 1;       // 1-black;2-white;3-red;4-green;5-blue; 6-yellow; 7-pink;8-dark blue;
    vRange4.vt = VT_UI4;
    vRange4.uintVal = RGB(0, 0, 0);     // 我测试后认为，没有实际意义，只有vRange3起作用

    for (i = 0; i < nNUm; i++)
    {
        strCell.Format("%c%d", 'A' + i + nCol - 1, nRow);

        range.AttachDispatch(workSheet.get_Range(COleVariant(strCell), COleVariant(strCell)), TRUE);
        ftColor.AttachDispatch(range.get_Font());
        ftColor.put_Name(_variant_t(strFontName));      // 字体 
        ftColor.put_Size(_variant_t(nSize));           // 字号
        ftColor.put_ColorIndex(_variant_t(nColor));     //  字的颜色 red
        // 设置单元格的线；
        range.BorderAround(vRange1, vRange2, vRange3, vRange4);
        range.ReleaseDispatch();
    }
}

void Excel::delNOTNameSheet(CString strSheetName)
{
    int         i = 0;
    CWorksheet  tmpSheet;

    for (i = sheets.get_Count(); i > 0; i--)
    {
        tmpSheet.AttachDispatch(sheets.get_Item(_variant_t((long)i)), TRUE);
        if (0 != strSheetName.CompareNoCase(tmpSheet.get_Name()))
        {
            tmpSheet.Delete();
        }
    }
}

//删除指定名称的sheet
void Excel::delNameSheet(CString strSheetName)
{
    int         i = 0;
    CWorksheet  tmpSheet;

    for (i = sheets.get_Count(); i > 0; i--)
    {
        tmpSheet.AttachDispatch(sheets.get_Item(_variant_t((long)i)), TRUE);
        if (0 == strSheetName.CompareNoCase(tmpSheet.get_Name()))
        {
            tmpSheet.Delete();
            return;
        }
    }
}

void Excel::show(bool bShow)
{
    application.put_Visible(bShow);
    application.put_UserControl(bShow);
}

CString Excel::getOpenFileName()
{
    return openFileName;
}

CString Excel::getOpenSheelName()
{
    return workSheet.get_Name();
}

char* Excel::getColumnName(long iColumn)
{
    static char column_name[64];
    size_t str_len = 0;

    while (iColumn > 0)
    {
        int num_data = iColumn % 26;
        iColumn /= 26;
        if (num_data == 0)
        {
            num_data = 26;
            iColumn--;
        }
        column_name[str_len] = (char)((num_data - 1) + 'A');
        str_len++;
    }
    column_name[str_len] = '\0';
    //反转
    _strrev(column_name);

    return column_name;
}
