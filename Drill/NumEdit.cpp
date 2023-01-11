#include "NumEdit.h"

CNumEdit::CNumEdit()
{
}


CNumEdit::~CNumEdit()
{
}

BOOL CNumEdit::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_CHAR)
    {
        //ֻ��������һ��С����
        if ((pMsg->wParam == '.') && CheckUnique('.'))
        {
            return TRUE;
        }

        //��ǰ�ַ��ǡ�-�� && ��ǰ����ڵ�һλ && ��ǰ��û�С�-��
        if ((pMsg->wParam == '-') && (LOWORD(GetSel()) == 0) && !CheckUnique('-'))
        {
            return CEdit::PreTranslateMessage(pMsg); //����������Ϣ��ʵ�����롯-��
        }

        //��ǰ����ڵ�һλ && ��ǰ�Ѿ��С�-��
        if ((LOWORD(GetSel()) == 0) && CheckUnique('-'))
        {
            return TRUE; //�������ڡ�-'ǰ���ٲ����κ��ַ����������֡�12-23.467�����������
        }

        //ֻ�������롯.�� && ���� ��0�� �� ��9��  && �˸�
        if (pMsg->wParam != '.' && (pMsg->wParam > '9' || pMsg->wParam < '0') && pMsg->wParam != '\b')
        {
            return TRUE; //ʣ�µ���Ϣȫ��������
        }
    }

    return CEdit::PreTranslateMessage(pMsg); //�ܻ��ŵ��������Ϣû������
}

/*
* Fuction: �����жϵ�ǰEdit�ı����Ƿ����ĳ���ַ�
* Param: char nChar Ҫ�����ַ�
* return: TRUE-�Ѵ���Ҫ�����ַ� FALSE-������
*/
BOOL CNumEdit::CheckUnique(char nChar)
{
    CString str;
    int nPos = 0;
    GetWindowText(str);
    nPos = str.Find(nChar);
    return (nPos >= 0) ? TRUE : FALSE;
}
