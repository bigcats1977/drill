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
        //只允许输入一个小数点
        if ((pMsg->wParam == '.') && CheckUnique('.'))
        {
            return TRUE;
        }

        //当前字符是’-’ && 当前光标在第一位 && 当前还没有’-’
        if ((pMsg->wParam == '-') && (LOWORD(GetSel()) == 0) && !CheckUnique('-'))
        {
            return CEdit::PreTranslateMessage(pMsg); //则接受这个消息，实现输入’-’
        }

        //当前光标在第一位 && 当前已经有’-’
        if ((LOWORD(GetSel()) == 0) && CheckUnique('-'))
        {
            return TRUE; //不允许在’-'前面再插入任何字符，否则会出现“12-23.467”这样的情况
        }

        //只允许输入’.’ && 数字 ’0′ 到 ’9′  && 退格
        if (pMsg->wParam != '.' && (pMsg->wParam > '9' || pMsg->wParam < '0') && pMsg->wParam != '\b')
        {
            return TRUE; //剩下的消息全部不处理
        }
    }

    return CEdit::PreTranslateMessage(pMsg); //能活着到这里的消息没几个了
}

/*
* Fuction: 用于判断当前Edit文本中是否包含某个字符
* Param: char nChar 要检查的字符
* return: TRUE-已存在要检查的字符 FALSE-不存在
*/
BOOL CNumEdit::CheckUnique(char nChar)
{
    CString str;
    int nPos = 0;
    GetWindowText(str);
    nPos = str.Find(nChar);
    return (nPos >= 0) ? TRUE : FALSE;
}
