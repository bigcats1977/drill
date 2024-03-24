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
    //����Excel 2000������(����Excel)
    if (!application.CreateDispatch(_T("Excel.Application"), nullptr))
    {
        //MessageBox(nullptr, _T("����Excel����ʧ��,�����û�а�װEXCEL������!"), _T("����"), MB_OK);
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
    //�ȹر��ļ�
    close();

    //����ģ�彨�����ĵ�
    books.AttachDispatch(application.get_Workbooks());

    try {
        //lpDisp = books.Add(COleVariant(fileName));
        /*��һ��������*/
        lpDisp = books.Open(fileName,
            vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
            vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
            vtMissing, vtMissing, vtMissing, vtMissing);
    }
    catch (...)
    {
        // �Ҳ������´���һ��
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
    //����ļ��Ѿ��򿪣��ر��ļ�
    if (!openFileName.IsEmpty())
    {
        //�������,�����û�����,���û��Լ��棬����Լ�SAVE�������Ī���ĵȴ�
        if (ifSave)
        {
            //show(true);
        }
        else
        {
            workBook.Close(COleVariant(short(FALSE)), COleVariant(openFileName), covOptional);
            books.Close();
        }

        //��մ��ļ�����
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

//�������Ƽ���sheet���Ҳ����ǰ�������б��
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
    //�������Ԥ�ȼ���
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
    //�������Ԥ�ȼ���
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
    //VT_BSTR��ʾ�ַ���
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
    //VT_BSTR��ʾ�ַ���
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
    //�ַ���
    if (isLoad == false)
    {
        CRange range;
        range.AttachDispatch(wholeRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
        vResult = range.get_Value2();
        range.ReleaseDispatch();
    }
    //�����������Ԥ�ȼ�����
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
    //����
    else if (vResult.vt == VT_INT)
    {
        str.Format(_T("%d"), vResult.pintVal);
    }
    //8�ֽڵ�����
    else if (vResult.vt == VT_R8)
    {
        str.Format(_T("%0.0f"), vResult.dblVal);
    }
    //ʱ���ʽ
    else if (vResult.vt == VT_DATE)
    {
        SYSTEMTIME st;
        VariantTimeToSystemTime(vResult.date, &st);
        CTime tm(st);
        str = tm.Format(_T("%Y-%m-%d"));

    }
    //��Ԫ��յ�
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
    //�ַ���
    if (isLoad == false)
    {
        CRange range;
        range.AttachDispatch(wholeRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
        vresult = range.get_Value2();
        range.ReleaseDispatch();
    }
    //�����������Ԥ�ȼ�����
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
            // ��� VARIANT ����һ�� float ֵ
            fLoc[i] = V_R4(&v[i]);
        }
        else if (V_VT(&v[i]) == VT_R8) {
            // ��� VARIANT ����һ�� double ֵ
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


    /* ����Ŀ��λ�� */
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
    /* �����͹������и�Ϊ2�� */
    VARIANT rowHeight = range.get_RowHeight();
    range.put_RowHeight((_variant_t)(fMulti * rowHeight.dblVal));
    range.ReleaseDispatch();
}

void Excel::setMultiColFont(UINT nRow, UINT nCol, UINT nNUm, CString strFontName, long nSize, long nColor)
{
    UINT        i = 0;
    CRange      range;
    CFont0      ftColor;    // ����������ɫ
    CString     strCell;
    _variant_t  vRange1;    // ���õ�Ԫ��ı߿�
    _variant_t  vRange2;
    _variant_t  vRange3;
    _variant_t  vRange4;

    vRange1.vt = VT_I2;
    vRange1.lVal = 1;       // �ߵ���ʽ��0- no line; 1-solid; 2-big dot;3-small dot;4-dash dot; 5-dash dot dot; xlContinuous 1
    vRange2.vt = VT_I2;
    vRange2.lVal = 1;       // �ߵĴ�ϸ�̶ȣ�
    vRange3.vt = VT_I2;
    vRange3.lVal = 1;       // 1-black;2-white;3-red;4-green;5-blue; 6-yellow; 7-pink;8-dark blue;
    vRange4.vt = VT_UI4;
    vRange4.uintVal = RGB(0, 0, 0);     // �Ҳ��Ժ���Ϊ��û��ʵ�����壬ֻ��vRange3������

    for (i = 0; i < nNUm; i++)
    {
        strCell.Format("%c%d", 'A' + i + nCol - 1, nRow);

        range.AttachDispatch(workSheet.get_Range(COleVariant(strCell), COleVariant(strCell)), TRUE);
        ftColor.AttachDispatch(range.get_Font());
        ftColor.put_Name(_variant_t(strFontName));      // ���� 
        ftColor.put_Size(_variant_t(nSize));           // �ֺ�
        ftColor.put_ColorIndex(_variant_t(nColor));     //  �ֵ���ɫ red
        // ���õ�Ԫ����ߣ�
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

//ɾ��ָ�����Ƶ�sheet
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
    //��ת
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

    /*��Sheet��д������Ԫ��,��ģΪ10*10 */
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