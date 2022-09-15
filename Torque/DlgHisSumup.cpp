// DlgHisSumup.cpp : implementation file
//

#include "stdafx.h"
#include "Torque.h"
#include "DlgHisSumup.h"

static DWORD CALLBACK StreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
    CFile* pFile = (CFile*)dwCookie;
    *pcb = pFile->Read(pbBuff, cb);
    return 0;
}

static DWORD CALLBACK StreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
    CFile*pFile = (CFile*)dwCookie;

    pFile->Write(pbBuff, cb);
    *pcb = cb;

    return 0;
}


// CDlgHisSumup dialog

IMPLEMENT_DYNAMIC(CDlgHisSumup, CPropertyPage)

CDlgHisSumup::CDlgHisSumup()
    : CPropertyPage(CDlgHisSumup::IDD)
{
    m_bReadOnly = TRUE;
}

CDlgHisSumup::~CDlgHisSumup()
{
}

void CDlgHisSumup::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REDSUM, m_richSum);
}


BEGIN_MESSAGE_MAP(CDlgHisSumup, CPropertyPage)
    ON_BN_CLICKED(IDC_BTNMODIFYSUM, &CDlgHisSumup::OnBnClickedBtnmodifysum)
    ON_BN_CLICKED(IDC_BTNSAVESUM, &CDlgHisSumup::OnBnClickedBtnsavesum)
    ON_BN_CLICKED(IDC_BTNMODFONT, &CDlgHisSumup::OnBnClickedBtnmodfont)
END_MESSAGE_MAP()


// CDlgHisSumup message handlers
BOOL CDlgHisSumup::OnInitDialog()
{
    // TODO: 调用 AfxInitRichEdit2() 以初始化 richedit2 库。\n"   
    int     iWidth   = 0;
    CString strHead;
    CRect   rcView;

    CPropertyPage::OnInitDialog();
    
    theApp.AdaptDlgCtrlSize(this,2);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgHisSumup::OnSetActive()
{
    CFile   file;
    EDITSTREAM es;

    SendMessageToDescendants(WM_SETFONT, (WPARAM)theApp.m_tRuleHFont.GetSafeHandle(), TRUE);

    m_richSum.SetReadOnly(TRUE);
    m_bReadOnly = TRUE;

    m_strSumFile = theApp.GetSaveDataPath();
    m_strSumFile += theApp.m_strFileTitle.c_str();
    m_strSumFile += ".rtf";
    
    if(file.Open(m_strSumFile, CFile::modeRead|CFile::shareDenyNone))
    {
        es.dwCookie = (DWORD)&file;
        es.pfnCallback = StreamInCallback;
        m_richSum.StreamIn(SF_RTF, es);
        file.Close();
    }
    
    UpdateData(FALSE);

    return CPropertyPage::OnSetActive();
}

void CDlgHisSumup::OnBnClickedBtnmodifysum()
{    
    m_richSum.SetReadOnly(TRUE);

    COMP_BFALSE(theApp.CheckPassWord());

    m_bReadOnly = FALSE;
    m_richSum.SetReadOnly(FALSE);
}

void CDlgHisSumup::OnBnClickedBtnsavesum()
{
    UpdateData(TRUE);
    
    CFile cFile(m_strSumFile, CFile::modeCreate|CFile::modeWrite);
    // 定义RichEdit的Stream 结构体
    EDITSTREAM es;
    // 设置stream结构体的文件句柄及回调函数
    es.dwCookie=(DWORD)&cFile;
    es.pfnCallback=StreamOutCallback;
    // 将当前edit控件里的字符串输出到rtf文件里。
    m_richSum.StreamOut(SF_RTF,es);

    m_bReadOnly = TRUE;
    m_richSum.SetReadOnly(TRUE);
}

void CDlgHisSumup::OnBnClickedBtnmodfont()
{
    CHARFORMAT cf;
    LOGFONT lf;
    memset(&cf, 0, sizeof(CHARFORMAT));
    memset(&lf, 0, sizeof(LOGFONT));

    COMP_BTRUE(m_bReadOnly);
    //判断是否选择了内容
    BOOL bSelect = (m_richSum.GetSelectionType() != SEL_EMPTY) ? TRUE : FALSE;
    if (bSelect)
    {
        m_richSum.GetSelectionCharFormat(cf);
    }
    else
    {
        m_richSum.GetDefaultCharFormat(cf);
    }

    //得到相关字体属性
    BOOL bIsBold = cf.dwEffects & CFE_BOLD;
    BOOL bIsItalic = cf.dwEffects & CFE_ITALIC;
    BOOL bIsUnderline = cf.dwEffects & CFE_UNDERLINE;
    BOOL bIsStrickout = cf.dwEffects & CFE_STRIKEOUT;

    //设置属性
    lf.lfCharSet = cf.bCharSet;
    lf.lfHeight = cf.yHeight / 15;
    lf.lfPitchAndFamily = cf.bPitchAndFamily;
    lf.lfItalic = bIsItalic;
    lf.lfWeight = (bIsBold ? FW_BOLD : FW_NORMAL);
    lf.lfUnderline = bIsUnderline;
    lf.lfStrikeOut = bIsStrickout;
    sprintf_s(lf.lfFaceName, cf.szFaceName);

    CFontDialog dlg(&lf);
    dlg.m_cf.rgbColors = cf.crTextColor;
    if (dlg.DoModal() == IDOK)
    {
        dlg.GetCharFormat(cf);//获得所选字体的属性
        if (bSelect)
            m_richSum.SetSelectionCharFormat(cf);     //为选定的内容设定所选字体
        else
            m_richSum.SetWordCharFormat(cf);         //为将要输入的内容设定字体
    }
}
