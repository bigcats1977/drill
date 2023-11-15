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
    lpDisp = NULL;
}

Excel::~Excel()
{
    close();
    release();
}

bool Excel::initExcel()
{
    //创建Excel 2000服务器(启动Excel)
    if (!application.CreateDispatch(_T("Excel.Application"), nullptr))
    {
        //MessageBox(nullptr, _T("创建Excel服务失败,你可能没有安装EXCEL，请检查!"), _T("错误"), MB_OK);
        return false;
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

bool Excel::open(CString fileName)
{
    //先关闭文件
    close();

    //利用模板建立新文档
    books.AttachDispatch(application.get_Workbooks());

    try {
        //lpDisp = books.Add(COleVariant(fileName));
        /*打开一个工作簿*/
        lpDisp = books.Open(fileName,
            vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
            vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
            vtMissing, vtMissing, vtMissing, vtMissing);
    }
    catch (...)
    {
        // 找不到则新创建一个
        lpDisp = books.Add(vtMissing);
    }

    if (!lpDisp)
        return false;

    workBook.AttachDispatch(lpDisp);
    sheets.AttachDispatch(workBook.get_Worksheets());
    openFileName = fileName;
    return true;
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

    wholeRange.ReleaseDispatch();
    workSheet.ReleaseDispatch();
    sheets.ReleaseDispatch();
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
    wholeRange.ReleaseDispatch();
    try
    {
        lpDisp = sheets.get_Item(COleVariant((long)tableId));
        workSheet.AttachDispatch(lpDisp);
    }
    catch (...)
    {
        lpDisp = sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
        workSheet.AttachDispatch(lpDisp);
        workSheet.put_Name("Sheet1");
    }
    if (!lpDisp)
        return false;

    wholeRange.AttachDispatch(workSheet.get_Cells());

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
    wholeRange.ReleaseDispatch();

    try
    {
        lpDisp = sheets.get_Item(COleVariant(sheet));
        workSheet.AttachDispatch(lpDisp);
    }
    catch (...)
    {
        lpDisp = sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
        workSheet.AttachDispatch(lpDisp);
        workSheet.put_Name(sheet);
    }
    if (!lpDisp)
        return false;

    wholeRange.AttachDispatch(workSheet.get_Cells());

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
    range.AttachDispatch(wholeRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
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
    range.AttachDispatch(wholeRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
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
        range.AttachDispatch(wholeRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
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
        range.AttachDispatch(wholeRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
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
        range.AttachDispatch(wholeRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
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

CShape Excel::addCellPicture(CString strFileName, CString top, CString bottom)
{
    VARIANT v[4];
    float fLoc[4];
    CRange range;

    range.AttachDispatch(workSheet.get_Range(COleVariant(top), COleVariant(bottom)));

    v[0] = range.get_Left();
    v[1] = range.get_Top();
    v[2] = range.get_Width();
    v[3] = range.get_Height();
    for (int i = 0; i < 4; i++) {
        if (V_VT(&v[i]) == VT_R4) {
            // 如果 VARIANT 包含一个 float 值
            fLoc[i] = V_R4(&v[i]);
        }
        else if (V_VT(&v[i]) == VT_R8) {
            // 如果 VARIANT 包含一个 double 值
            fLoc[i] = static_cast<float>(V_R8(&v[i]));
        }
    }

    return addCellPicture(strFileName, fLoc[0], fLoc[1], fLoc[2], fLoc[3]);
}

void Excel::copyMultiRow(UINT nSrcRow, UINT nDestRow, UINT nBeginCol, UINT nEndCol, UINT nNum)
{
    CRange      rangeBegin, rangeEnd;
    CString     StartCell, EndCell;

    ASSERT_ZERO(nSrcRow);
    ASSERT_ZERO(nDestRow);
    ASSERT_ZERO(nBeginCol);
    ASSERT_ZERO(nEndCol);
    ASSERT_ZERO(nNum);

    StartCell.Format("%s%d", columnToTitle(nBeginCol), nSrcRow);// 1-->A
    EndCell.Format("%s%d", columnToTitle(nEndCol), nSrcRow + nNum);  // 13-->M
    rangeBegin.AttachDispatch(workSheet.get_Range(COleVariant(StartCell), COleVariant(EndCell)));


    /* 拷贝目的位置 */
    StartCell.Format("%s%d", columnToTitle(nBeginCol), nDestRow);// 1-->A
    EndCell.Format("%s%d", columnToTitle(nEndCol), nDestRow + nNum);// 13-->M
    rangeEnd.AttachDispatch(workSheet.get_Range(COleVariant(StartCell), COleVariant(EndCell)));
    COleVariant varRange;
    varRange.vt = VT_DISPATCH;
    V_DISPATCH(&varRange) = (LPDISPATCH)rangeEnd;
    V_DISPATCH(&varRange)->AddRef();
    rangeBegin.Copy(varRange);

    rangeEnd.ReleaseDispatch();
    rangeBegin.ReleaseDispatch();
}

void Excel::setMultiRowHeight(UINT nRow, UINT nBeginCol, UINT nEndCol, float fMulti)
{
    CRange      range;
    CString     StartCell, EndCell;

    StartCell.Format("%s%d", columnToTitle(nBeginCol), nRow);// 1-->A
    EndCell.Format("%s%d", columnToTitle(nEndCol), nRow);// 13-->M
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


int Excel::titleToColumn(CString title)
{
    int sum = 0, i = 0;

    for (i = 0; i < title.GetLength(); i++)
    {
        sum += (title.GetAt(i) - 'A' + 1) * 26;
    }
    return sum;
}

CString Excel::columnToTitle(int column)
{
    CString strTitle;
    column = column <= 0 ? 1 : column;
    while (column > 0)
    {
        int m = column % 26 == 0 ? 26 : column % 26;
        strTitle.Insert(0, 'A' + m - 1);
        column = (column - m) / 26;
    }
    return strTitle;
}

bool Excel::SetMultiCellContent(UINT nBeginRow, UINT nEndRow, UINT nBeginCol, UINT nEndCol, CStringList& slContent)
{
    int             i, j;
    int             rows, cols;
    CString         StartCell, EndCell, strValue;
    CRange          curRange;
    COleSafeArray   saRet;
    DWORD           numElements[2];
    long            index[2];
    VARIANT         v;
    POSITION        rPos;

    ASSERT_ZERO_R(nBeginRow, false);
    ASSERT_ZERO_R(nEndRow, false);
    ASSERT_ZERO_R(nBeginCol, false);
    ASSERT_ZERO_R(nEndCol, false);

    rows = nEndRow - nBeginRow + 1;
    cols = nEndCol - nBeginCol + 1;
    if (slContent.GetSize() < rows * cols)
        return false;

    /*向Sheet中写入多个单元格,规模为10*10 */
    StartCell.Format("%s%d", columnToTitle(nBeginCol), nBeginRow);
    EndCell.Format("%s%d", columnToTitle(nEndCol), nEndRow);
    lpDisp = workSheet.get_Range(_variant_t(StartCell), _variant_t(EndCell));
    curRange.AttachDispatch(lpDisp);

    //Number of rows in the range.
    numElements[0] = rows;
    //Number of columns in the range.
    numElements[1] = cols;
    saRet.Create(VT_BSTR, 2, numElements);
    //saRet.Create(VT_R8, 2, numElements);

    //Fill the SAFEARRAY.
    rPos = slContent.GetHeadPosition();
    for (i = 0; i < rows; i++)
    {
        index[0] = i;
        for (j = 0; j < cols; j++)
        {
            index[1] = j;

            //Fill with Strings.
            VariantInit(&v);
            v.vt = VT_BSTR;
            strValue = slContent.GetNext(rPos);
            v.bstrVal = strValue.AllocSysString();
            saRet.PutElement(index, v.bstrVal);
            SysFreeString(v.bstrVal);
            VariantClear(&v);

            ////Fill with Numbers.
            //double d;
            //d = (iRow * 1000) + iCol;
            //saRet.PutElement(index, &d);
        }
    }

    //Set the range value to the SAFEARRAY.
    curRange.put_Value2(COleVariant(saRet));
    saRet.Detach();
    curRange.ReleaseDispatch();

    return true;
}