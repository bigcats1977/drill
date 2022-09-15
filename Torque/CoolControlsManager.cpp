/******************************************************************

$Archive: /MfcExt/Source/CoolControlsManager.cpp $
$Workfile: CoolControlsManager.cpp $
$Author: Bogdan Ledwig $
$Date: 99-04-26 22:12 $
$Revision: 13 $

*******************************************************************/

#include "StdAfx.h"
#include "CoolControlsManager.h"

// If you don't want to see extra TRACE diagnostics,
// modify the line below to: #define CCM_TRACE
#define CCM_TRACE //TRACE

#define CCM_TIMER_VAL 100        // 100 ms timer period seems to be good enough...
#define MAX_CLASSNAME 64         // Length of buffer for retrieving the class name


CString g_strCtrlName[CC_MAXNUM] = {
                                    _T("Button"),
                                    _T("ComboBox"),
                                    _T("Edit"),
                                    _T("ListBox"),
                                    _T("SysListView32"),
                                    _T("SHELLDLL_DefView"),
                                    _T("SysTreeView32"),
                                    _T("SysDateTimePick32"),
                                    _T("SysMonthCal32"),
                                    _T("msctls_updown32"),
                                    _T("ComboLBox"),
                                    _T("ScrollBar"),
                                    _T("ComboBoxEx32"),
                                    _T("msctls_hotkey32"),
                                    _T("SysIPAddress32"),
                                    _T("msctls_trackbar32"),
                                    _T("RichEdit"),
                                    _T("RichEdit20W"),
                                    _T("SysHeader32"),
                                    _T("ToolbarWindow32"),
                                    _T("SysTabControl32")
};

BOOL (CCoolControlsManager::*g_tAddCtrlProc[CC_MAXNUM])(DWORD dwStyle, DWORD dwExStyle,
                                                        CCoolControlsManager::CCMControl *pCtl,
                                                        HWND hWnd, TCHAR *szType) = {
            /*  0 */            &CCoolControlsManager::AddButtonCtrl,
            /*  1 */            &CCoolControlsManager::AddComboBoxCtrl,
            /*  2 */            &CCoolControlsManager::AddEditCtrl,
            /*  3 */            &CCoolControlsManager::AddListBoxCtrl,
            /*  4 */            &CCoolControlsManager::AddSysListViewCtrl,
            /*  5 */            &CCoolControlsManager::AddShellDllViewCtrl,
            /*  6 */            &CCoolControlsManager::AddSysTreeViewCtrl,
            /*  7 */            &CCoolControlsManager::AddSysDateTimeCtrl,
            /*  8 */            &CCoolControlsManager::AddSysMonthCalCtrl,
            /*  9 */            &CCoolControlsManager::AddMSUpDownCtrl,
            /* 10 */            &CCoolControlsManager::AddComboLBoxCtrl,
            /* 11 */            &CCoolControlsManager::AddScrollBarCtrl,
            /* 12 */            &CCoolControlsManager::AddComboBoxExCtrl,
            /* 13 */            &CCoolControlsManager::AddMSHotKeyCtrl,
            /* 14 */            &CCoolControlsManager::AddSysIPAddrCtrl,
            /* 15 */            &CCoolControlsManager::AddMSTrackBarCtrl,
            /* 16 */            &CCoolControlsManager::AddRichEditCtrl,
            /* 17 */            &CCoolControlsManager::AddRichEditWCtrl,
            /* 18 */            &CCoolControlsManager::AddSysHeaderCtrl,
            /* 19 */            &CCoolControlsManager::AddToolbarWinCtrl,
            /* 20 */            &CCoolControlsManager::AddSysTabCtrl
}; 

////////////////////////////////////////////////////////////////////////
// CCMControl static members initialization

HWND CCoolControlsManager::CCMControl::m_hWndOld = NULL;
CMapPtrToPtr CCoolControlsManager::m_ctrlMap;// = 10;
CMapPtrToPtr CCoolControlsManager::m_dlgMap;// = 10;
BOOL CCoolControlsManager::m_bEnabled = TRUE;

// Changed 02.03.1999 Mike Walter
CMapWordToPtr CCoolControlsManager::m_threadMap;// = 10;

///////////////////////////////////////////////////////////////////////
// Here is the one and only CCoolControlsManager object
static CCoolControlsManager g_ctrlManager;

CCoolControlsManager& GetCtrlManager()
{
    return g_ctrlManager;
}

////////////////////////////////////////////////////////////////////////
// WH_CALLWNDPROC hook procedure

LRESULT CALLBACK CCM_CallWndProc( int iCode, WPARAM wParam, LPARAM lParam )
{
    HOOKPROC    hHookProc;
    CWPSTRUCT   *pwp = NULL;
    TCHAR       szBuf[MAX_CLASSNAME];

    COMP_BFALSE_R(g_ctrlManager.m_threadMap.Lookup( (WORD)GetCurrentThreadId(), (void*&)hHookProc ), 0);

    if ( iCode == HC_ACTION )
    {
        ASSERT_NULL_R(lParam, 0);
        pwp = (CWPSTRUCT*)lParam;
        if ( g_ctrlManager.IsEnabled() )
        {
            if ( g_ctrlManager.m_bDialogOnly == TRUE )
            {
                if ( pwp->message == WM_INITDIALOG )
                    g_ctrlManager.Install( pwp->hwnd );
            }
            else if ( pwp->message == WM_CREATE && g_ctrlManager.IsEnabled() )
            {
                if ( GetWindowLong( pwp->hwnd, GWL_STYLE ) & WS_CHILD )
                {
                    GetClassName( pwp->hwnd, szBuf, MAX_CLASSNAME );
                    if ( lstrcmp( szBuf, _T( "ScrollBar" ) ) ) // Don't add scrollbars
                        g_ctrlManager.AddControl( pwp->hwnd );
                }
            }
        }
    }
    // Changed 02.03.1999 Mike Walter
    return CallNextHookEx( (HHOOK)hHookProc, iCode, wParam, lParam );
}

// Install a hook for the current thread only
void CCoolControlsManager::InstallHook( DWORD dwThreadID, BOOL bDialogOnly )
{
    // Changes 02.03.1999 Mike Walter
    HOOKPROC hNewHook;

    // ASSERT( m_hkWndProc == NULL );

    m_bDialogOnly = bDialogOnly;

    if ( m_threadMap.Lookup( (WORD)( dwThreadID == -1 ? GetCurrentThreadId() : dwThreadID ), (void*&)hNewHook ) == FALSE )
    {
        hNewHook = (HOOKPROC)SetWindowsHookEx(  WH_CALLWNDPROC,
                                                (HOOKPROC)CCM_CallWndProc,
                                                NULL,
                                                ( dwThreadID == -1 ? GetCurrentThreadId() : dwThreadID ) );

        m_threadMap.SetAt( (WORD)( dwThreadID == -1 ? GetCurrentThreadId() : dwThreadID ), hNewHook );

        //CCM_TRACE( "CCoolControlsManager: WH_CALLWNDPROC hook installed for thread: %d\n", ( dwThreadID == -1 ? GetCurrentThreadId() : dwThreadID ) );
        return;
    }
    /*else*/
    CCM_TRACE( "CCoolControlsManager: WH_CALLWNDPROC hook already installed for thread: %d!\n", ( dwThreadID == -1 ? GetCurrentThreadId() : dwThreadID ) );
}

// Install a global hook for all windows in the system.
// This function may be called only when is put in a DLL.
void CCoolControlsManager::InstallGlobalHook( HINSTANCE hInstance, BOOL bDialogOnly )
{
    HOOKPROC hkProc;

    ASSERT( hInstance );      // hInstance must not be NULL!
    ASSERT( m_hkWndProc == NULL );

    m_bDialogOnly = bDialogOnly;

    hkProc = (HOOKPROC)GetProcAddress( hInstance, "CCM_CallWndProc" );

    m_hkWndProc = (HOOKPROC)SetWindowsHookEx(   WH_CALLWNDPROC,
                                                (HOOKPROC)hkProc,
                                                hInstance,
                                                0 );

    CCM_TRACE( _T( "CCoolControlsManager: WH_CALLWNDPROC global hook installed\n" ) );
}

void CCoolControlsManager::UninstallHook( DWORD dwThreadID )
{
    // ASSERT( m_hkWndProc != NULL );

    // Changes 02.03.1999 Mike Walter
    HOOKPROC hHookProc;

    if ( dwThreadID == -1 )
    {
        if ( g_ctrlManager.m_threadMap.Lookup( (WORD)GetCurrentThreadId(), (void*&)hHookProc ) == FALSE )
        {
            CCM_TRACE( "CCoolControlsManager: No hook installed for thread: %d!\n", GetCurrentThreadId() );
            return;
        }

        UnhookWindowsHookEx( (HHOOK)hHookProc );
        m_threadMap.RemoveKey( (WORD)GetCurrentThreadId() );

        CCM_TRACE( "CCoolControlsManager: Hook uninstalled for thread: %d\n", GetCurrentThreadId() );
        CCM_TRACE( "CCoolControlsManager: Thread map has %d items\n",g_ctrlManager.m_threadMap.GetCount() );
    }
    else
    {
        if ( g_ctrlManager.m_threadMap.Lookup( (WORD)dwThreadID, (void*&)hHookProc) == FALSE )
        {
            CCM_TRACE( "CCoolControlsManager: No hook installed for thread: %d!\n", dwThreadID );
            return;
        }

        UnhookWindowsHookEx( (HHOOK)hHookProc );
        m_threadMap.RemoveKey( (WORD)dwThreadID );

        CCM_TRACE( "CCoolControlsManager: Hook uninstalled for thread: %d\n", dwThreadID );
        CCM_TRACE( "CCoolControlsManager: Thread map has %d items\n", g_ctrlManager.m_threadMap.GetCount() );
    }

    if ( m_nTimerID && g_ctrlManager.m_threadMap.IsEmpty() == TRUE )
        KillTimer( NULL, m_nTimerID );
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCoolControlsManager::CCoolControlsManager()
{
    m_hkWndProc = NULL;
    m_nTimerID = 0;
    CCM_TRACE( _T( "CCoolControlsManager::CCoolControlsManager()\n" ) );
}

CCoolControlsManager::~CCoolControlsManager()
{
    // Changed 02.03.1999 Mike Walter
    POSITION    pos = m_threadMap.GetStartPosition();
    HOOKPROC    hHook;
    DWORD       dwThreadID = 0;
    HWND        hWnd;
    CCMControl  *pCtl = NULL;
    CCMDialog   *pDlg = NULL;

    while ( pos )
    {
        m_threadMap.GetNextAssoc( pos, (WORD&)dwThreadID, (void*&)hHook );
        UninstallHook( dwThreadID );
    }

    // If we have any elements in the map (normally impossible), unsubclass they and remove
    pos = m_ctrlMap.GetStartPosition();

    while ( pos )
    {
        m_ctrlMap.GetNextAssoc( pos, (void*&)hWnd, (void*&)pCtl );
        pCtl->Unsubclass();
        m_ctrlMap.RemoveKey( hWnd );
        delete pCtl;
    }

    // Now do the same things for dialog map
    pos = m_dlgMap.GetStartPosition();

    while ( pos )
    {
        m_dlgMap.GetNextAssoc( pos, (void*&)hWnd, (void*&)pDlg );
        pDlg->Unsubclass();
        m_dlgMap.RemoveKey( hWnd );
        delete pCtl;
    }

    CCM_TRACE( "CCoolControlsManager::~CCoolControlsManager()\n" );
}

void CCoolControlsManager::Install( HWND hWnd )
{
    HWND        hCtrl;
    CCMControl  *pCtl = NULL;
    TCHAR       szBuf[MAX_CLASSNAME];

    // Already in the dialog map
    COMP_BTRUE(m_dlgMap.Lookup( hWnd, (void*&)pCtl ));

    // Iterate through all child windows
    hCtrl = GetTopWindow( hWnd );

    while ( hCtrl )
    {
        if ( GetWindowLong( hCtrl, GWL_STYLE ) & WS_CHILD )
        {
            GetClassName( hCtrl, szBuf, MAX_CLASSNAME );
            if ( lstrcmpi( szBuf, _T( "#32770" ) ) ) // Never add child dialogs!
                AddControl( hCtrl );
        }
        hCtrl = GetNextWindow( hCtrl, GW_HWNDNEXT );
    }

    AddDialog( hWnd ); // Add parent window as well

    // Now redraw all recently inserted controls
    hCtrl = GetTopWindow( hWnd );
    while ( hCtrl )
    {
        if ( m_ctrlMap.Lookup( hCtrl, (void*&)pCtl ) )
            pCtl->DrawBorder();
        hCtrl = GetNextWindow( hCtrl, GW_HWNDNEXT );
    }
}

void CCoolControlsManager::Uninstall( HWND hWnd )
{
    // Remove all window controls from the map
    // when the window is about to destroy
    CCM_TRACE( _T( "CCoolControlsManager: Uninstall, handle: %X\n" ), hWnd );

    HWND hCtrl = GetTopWindow( hWnd );
    while ( hCtrl )
    {
        if ( GetWindowLong( hCtrl, GWL_STYLE ) & WS_CHILD )
            RemoveControl( hCtrl );
        hCtrl = GetNextWindow( hCtrl, GW_HWNDNEXT );
    }
}

BOOL CCoolControlsManager::AddCtrl2Map(CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    ASSERT_NULL_R(pCtl, FALSE);
    ASSERT_NULL_R(hWnd, FALSE);
    ASSERT_NULL_R(szType, FALSE);

    CCM_TRACE( _T( "CCoolControlsManager::AddControl, handle: %X, type: %s\n" ),
               hWnd, szType );

    // Perform window subclassing
    pCtl->Subclass( hWnd, CCM_ControlProc );

    // Add the control to the map
    m_ctrlMap.SetAt( hWnd, pCtl );

    if ( m_nTimerID == 0 ) // If timer is not initialized yet
    {
        m_nTimerID = SetTimer( NULL, COOLCONTROL_TIMER, CCM_TIMER_VAL, (TIMERPROC)CCM_TimerProc );
        CCM_TRACE( _T( "CControlManager: Timer created\n" ) );
        ASSERT( m_nTimerID );    // Failed to create the timer
    }
    return TRUE;
}

BOOL CCoolControlsManager::AddButtonCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    // Do not subclass ownerdraw buttons
    COMP_BE_R(( dwStyle & BS_OWNERDRAW ), BS_OWNERDRAW, FALSE);

    // Skip all group boxes and flat buttons
    COMP_BE_R(( dwStyle & BS_GROUPBOX ),  BS_GROUPBOX, FALSE);
    COMP_BE_R(( dwStyle & BS_FLAT ),      BS_FLAT,     FALSE);

    /* new check box */
    if ( ( dwStyle & BS_AUTOCHECKBOX )  == BS_AUTOCHECKBOX  ||
         ( dwStyle & BS_CHECKBOX )      == BS_CHECKBOX      ||
         ( dwStyle & BS_3STATE )        == BS_3STATE )
    {
        pCtl = new CCMCheckBox;
        return AddCtrl2Map(pCtl, hWnd, szType);
    }
    /* new radio button */
    if ( ( dwStyle & BS_AUTORADIOBUTTON )   == BS_AUTORADIOBUTTON ||
         ( dwStyle & BS_RADIOBUTTON )       == BS_RADIOBUTTON )
    {
        pCtl = new CCMRadioButton;
        return AddCtrl2Map(pCtl, hWnd, szType);
    }

    // If none of the above then it must be a pushbutton!
    pCtl = new CCMPushButton;     
    return AddCtrl2Map(pCtl, hWnd, szType);
}

BOOL CCoolControlsManager::AddComboBoxCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    ASSERT_NULL_R(hWnd, FALSE);

    // Special case for simple comboboxes
    if ( ( dwStyle & 0x03 ) == CBS_SIMPLE )
    {
        hWnd = GetTopWindow( hWnd );
        while ( hWnd )
        {
            AddControl( hWnd );
            hWnd = GetNextWindow( hWnd, GW_HWNDNEXT );
        }
        return FALSE;
    }

    pCtl = new CCMComboBox;
    return AddCtrl2Map(pCtl, hWnd, szType);
}

BOOL CCoolControlsManager::AddEditCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    TCHAR   szBuf[MAX_CLASSNAME];

    ASSERT_NULL_R(hWnd, FALSE);
    ASSERT_NULL_R(szType, FALSE);
    
#if defined _DEBUG
        lstrcpy( szType, _T( "Edit" ) );
#endif

    // Edit window in a simple combobox
    GetClassName( GetParent( hWnd ), szBuf, MAX_CLASSNAME );
    if ( !lstrcmpi( szBuf, _T( "ComboBox" ) ) &&
       ( GetWindowLong( GetParent( hWnd ), GWL_STYLE ) & 0x03 ) == CBS_SIMPLE )
    {
        pCtl = new CCMEditCombo;
        return AddCtrl2Map(pCtl, hWnd, szType);
    }

    COMP_BFALSE_R(dwExStyle & WS_EX_CLIENTEDGE, FALSE);
    /* if ( dwExStyle & WS_EX_CLIENTEDGE ) */
    pCtl = new CCMEdit;
    return AddCtrl2Map(pCtl, hWnd, szType);
}

BOOL CCoolControlsManager::AddListBoxCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    COMP_BFALSE_R(dwExStyle & WS_EX_CLIENTEDGE, FALSE);

    /* if ( dwExStyle & WS_EX_CLIENTEDGE ) */
    pCtl = new CCMControl;
    return AddCtrl2Map(pCtl, hWnd, szType);
}

BOOL CCoolControlsManager::AddSysListViewCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    COMP_BFALSE_R(dwExStyle & WS_EX_CLIENTEDGE, FALSE);

    /* if ( dwExStyle & WS_EX_CLIENTEDGE ) */
    pCtl = new CCMControl;
    // Don't forget to add the header control
    AddControl( GetTopWindow( hWnd ) );
    return AddCtrl2Map(pCtl, hWnd, szType);
}

/* In open/save common dialogs */
BOOL CCoolControlsManager::AddShellDllViewCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    // Add child ListView control
    AddControl( GetTopWindow( hWnd ) );
    return FALSE;
}

BOOL CCoolControlsManager::AddSysTreeViewCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    COMP_BFALSE_R(dwExStyle & WS_EX_CLIENTEDGE, FALSE);

    /* if ( dwExStyle & WS_EX_CLIENTEDGE ) */
    pCtl = new CCMControl;
    return AddCtrl2Map(pCtl, hWnd, szType);
}

BOOL CCoolControlsManager::AddSysDateTimeCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    ASSERT_NULL_R(hWnd, FALSE);

    COMP_BFALSE_R(dwExStyle & WS_EX_CLIENTEDGE, FALSE);

    /* if ( dwExStyle & WS_EX_CLIENTEDGE ) */
    pCtl = new CCMDateTime;
    if ( dwStyle & DTS_UPDOWN )
        AddControl( GetTopWindow( hWnd ) );  // Add up-down control as well
        
    return AddCtrl2Map(pCtl, hWnd, szType);
}

BOOL CCoolControlsManager::AddSysMonthCalCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    pCtl = new CCMControl;
    return AddCtrl2Map(pCtl, hWnd, szType);
}

BOOL CCoolControlsManager::AddMSUpDownCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    pCtl = new CCMUpDown;
    return AddCtrl2Map(pCtl, hWnd, szType);
}

BOOL CCoolControlsManager::AddComboLBoxCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    COMP_BFALSE_R(dwStyle & WS_EX_CLIENTEDGE, FALSE);

    /* if ( dwExStyle & WS_EX_CLIENTEDGE ) */
    pCtl = new CCMControl;
    return AddCtrl2Map(pCtl, hWnd, szType);
}

BOOL CCoolControlsManager::AddScrollBarCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    COMP_BTRUE_R(dwStyle & SBS_SIZEBOX, FALSE);
    COMP_BTRUE_R(dwStyle & SBS_SIZEGRIP, FALSE);

    /* if ( !( dwStyle & SBS_SIZEBOX ) && !( dwStyle & SBS_SIZEGRIP ) ) */
    pCtl = new CCMScrollBar;
    return AddCtrl2Map(pCtl, hWnd, szType);
}

BOOL CCoolControlsManager::AddComboBoxExCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{    
    AddControl( GetTopWindow( hWnd ) );
    return FALSE;
}

BOOL CCoolControlsManager::AddMSHotKeyCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    COMP_BFALSE_R(dwStyle & WS_EX_CLIENTEDGE, FALSE);

    /* if ( dwExStyle & WS_EX_CLIENTEDGE ) */
    pCtl = new CCMControl;
    return AddCtrl2Map(pCtl, hWnd, szType);
}

BOOL CCoolControlsManager::AddSysIPAddrCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    COMP_BFALSE_R(dwStyle & WS_EX_CLIENTEDGE, FALSE);

    /* if ( dwExStyle & WS_EX_CLIENTEDGE ) */
    pCtl = new CCMIPAddress;
    return AddCtrl2Map(pCtl, hWnd, szType);
}

BOOL CCoolControlsManager::AddMSTrackBarCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    pCtl = new CCMTrackbar;
    return AddCtrl2Map(pCtl, hWnd, szType);
}

BOOL CCoolControlsManager::AddRichEditCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    COMP_BFALSE_R(dwStyle & WS_EX_CLIENTEDGE, FALSE);
    
    /* if ( dwExStyle & WS_EX_CLIENTEDGE ) */
    pCtl = new CCMControl;
    return AddCtrl2Map(pCtl, hWnd, szType);
}

BOOL CCoolControlsManager::AddRichEditWCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    COMP_BFALSE_R(dwStyle & WS_EX_CLIENTEDGE, FALSE);

    /* if ( dwExStyle & WS_EX_CLIENTEDGE ) */
    pCtl = new CCMControl;
    return AddCtrl2Map(pCtl, hWnd, szType);
}

BOOL CCoolControlsManager::AddSysHeaderCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    COMP_BFALSE_R(dwStyle & HDS_BUTTONS, FALSE);

    /* if ( dwStyle & HDS_BUTTONS ) */
    pCtl = new CCMHeaderCtrl;
    return AddCtrl2Map(pCtl, hWnd, szType);
}

BOOL CCoolControlsManager::AddToolbarWinCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    HWND    hCtrl;

    // Skip the flat toolbars
    COMP_BTRUE_R(dwStyle & TBSTYLE_FLAT, FALSE);
    
    // Add additional toolbar controls
    hCtrl = GetTopWindow( hWnd );
    while ( hCtrl )
    {
        AddControl( hCtrl );
        hCtrl = GetNextWindow( hCtrl, GW_HWNDNEXT );
    }
    pCtl = new CCMToolbar;
    return AddCtrl2Map(pCtl, hWnd, szType);
}

BOOL CCoolControlsManager::AddSysTabCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType)
{
    HWND    hWndTop;
    
    pCtl = new CCMTabControl;
    hWndTop = GetTopWindow( hWnd );

    // Also add the up-down control (horizontal tabs only)
    if ( hWndTop )
        AddControl( hWndTop );
    
    return AddCtrl2Map(pCtl, hWnd, szType);
}

/*lint -e429*/
// In lpszClass you can specify class name, which will be used
// instead of true class name (useful for non-standard controls
// that are similar to the one of those we have supported)
BOOL CCoolControlsManager::AddControl( HWND hWnd, LPCTSTR lpszClass )
{
    int         i         = 0;
    DWORD       dwStyle   = 0;
    DWORD       dwExStyle = 0;
    CCMControl  *pCtl     = NULL;
    TCHAR       szBuf[MAX_CLASSNAME];

    // Must not be NULL or already in the map
    ASSERT_NULL_R(hWnd, FALSE);

    COMP_BTRUE_R(m_ctrlMap.Lookup( hWnd, (void*&)pCtl ), FALSE);

    if ( lpszClass == NULL )
        GetClassName( hWnd, szBuf, MAX_CLASSNAME );
    else
        lstrcpy( szBuf, lpszClass );

    dwStyle   = GetWindowLong( hWnd, GWL_STYLE );
    dwExStyle = GetWindowLong( hWnd, GWL_EXSTYLE );

    for(i=0; i<CC_MAXNUM; i++)
    {
        if ( !lstrcmpi( szBuf, g_strCtrlName[i] ) )
            return (this->* g_tAddCtrlProc[i])(dwStyle, dwExStyle, pCtl, hWnd, szBuf);
    }

    // Unknown control, do not process
    return FALSE;
}
/*lint +e429*/

BOOL CCoolControlsManager::RemoveControl( HWND hWnd )
{
    CCMControl  *pCtl = NULL;

    COMP_BFALSE_R(m_ctrlMap.Lookup( hWnd, (void*&)pCtl ), FALSE);

    // Unsubclass window and next remove it from the map
    pCtl->Unsubclass();
    m_ctrlMap.RemoveKey( hWnd );
    delete pCtl;      // Destroy the object

    if ( m_ctrlMap.IsEmpty() )
    {
        KillTimer( NULL, m_nTimerID );
        CCM_TRACE( _T( "CCoolControlsManager: Timer killed, map is empty\n" ) );
        m_nTimerID = 0;
    }
    else
        CCM_TRACE( _T( "CCoolControlsManager: map has %d items\n" ), m_ctrlMap.GetCount() );

    return TRUE;
}

void CCoolControlsManager::AddDialog( HWND hWnd )
{
    CCMDialog   *pCtl = NULL;

    ASSERT_NULL(hWnd);

    /*lint -e429*/

    /* if ( hWnd  ) */
    pCtl = new CCMDialog;
    // Perform window subclassing
    pCtl->Subclass( hWnd, CCM_DialogProc );
    // Add the dialog to the map
    m_dlgMap.SetAt( hWnd, pCtl );

    /*lint +e429*/
}

void CCoolControlsManager::RemoveDialog( HWND hWnd )
{
    CCMDialog   *pCtl = NULL;
    /*lint -e429*/

    COMP_BFALSE(m_dlgMap.Lookup( hWnd, (void*&)pCtl ));
    /* if ( m_dlgMap.Lookup( hWnd, (void*&)pCtl ) == TRUE ) */
    // Unsubclass window and next remove it from the map
    pCtl->Unsubclass();
    m_dlgMap.RemoveKey( hWnd );
    // Destroy the object
    delete pCtl;

    /*lint +e429*/
}

static void CALLBACK CCM_TimerProc( HWND /*hwnd*/, UINT /*nMsg*/,
                                   UINT /*nEventId*/, DWORD /*dwTime*/ )
{
    g_ctrlManager.TimerProc();
}

void CCoolControlsManager::TimerProc()
{
    CCMControl  *pCtl    = NULL;
    CCMControl  *pCtlOld = NULL;
    POINT       point;
    HWND        hWnd;

    // Do not process when the map is empty or the capture is set
    COMP_BTRUE(m_ctrlMap.IsEmpty());
    COMP_BNE(GetCapture(), NULL);

    GetCursorPos( &point );
    hWnd = WindowFromPoint( point );

    // Lookup for a window in the map
    if ( m_ctrlMap.Lookup( hWnd, (void*&)pCtl ) == FALSE ) // Not found
    {
        // If window does not exist in the map, it can be
        // a child of the control (e.g. edit control in ComboBox
        // or header control in ListView). If so, get the parent window and
        // carry on
        hWnd = GetParent( hWnd );
        // Not our window, so just reset previous control and exit
        if ( hWnd == NULL || m_ctrlMap.Lookup( hWnd, (void*&)pCtl ) == FALSE )
        {
            // Not our window, so just reset previous control and exit
            if ( m_ctrlMap.Lookup( CCMControl::m_hWndOld, (void*&)pCtl ) == TRUE )
            {
                pCtl->SetState( dsHover, 0 );
                CCMControl::m_hWndOld = NULL;
            }
            return;
        }
    }

    if ( pCtl->NeedRedraw( point ) ) // Window has been found and needs to be redrawn!
    {
        // First, reset old control (if any)
        if ( m_ctrlMap.Lookup( CCMControl::m_hWndOld, (void*&)pCtlOld ) == TRUE )
        {
            pCtlOld->SetState( dsHover, 0 );
            CCMControl::m_hWndOld = NULL;
        }

        // Redraw control under the cursor
        pCtl->SetState( 0, dsHover );
        CCMControl::m_hWndOld = hWnd;
    }
}

///////////////////////////////////////////////////////////////////////
// All messages from subclassed dialogs will come here

static LRESULT CALLBACK CCM_DialogProc( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam )
{
    CCoolControlsManager::CCMDialog *pCtl = NULL;

    // Try to find dialog in the dialog map
    if ( g_ctrlManager.m_dlgMap.Lookup( hWnd, (void*&)pCtl ) == FALSE )
    {
        // This is not our dialog, so just apply default processing
        return DefWindowProc( hWnd, nMsg, wParam, lParam );
    }

    // Otherwise, let the dialog to process this message
    return pCtl->WindowProc( nMsg, wParam, lParam );
}

///////////////////////////////////////////////////////////////////////
// All messages from subclassed controls will come here

static LRESULT CALLBACK CCM_ControlProc( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam )
{
    CCoolControlsManager::CCMControl *pCtl = NULL;

    // Try to find window in the control map
    if ( g_ctrlManager.m_ctrlMap.Lookup( hWnd, (void*&)pCtl ) == FALSE )
    {
        // This is not our window, so just apply default processing
        return DefWindowProc( hWnd, nMsg, wParam, lParam );
    }

    // Otherwise, let the control to process this message
    return pCtl->WindowProc( nMsg, wParam, lParam );
}

//////////////////////////////////////////////////////////////////////////////
// CCMControl and derived classes

CCoolControlsManager::CCMControl::CCMControl()
{
    m_hWnd      = NULL;
    m_oldWndProc= NULL;
    m_nState    = dsNormal;
    m_nOldState = dsNormal;
}

void CCoolControlsManager::CCMControl::PrepareDraw( HDC& hDC, RECT& rect )
{
    GetWindowRect( m_hWnd, &rect );
    OffsetRect( &rect, -rect.left, -rect.top );
    hDC = GetWindowDC( m_hWnd );
}

void CCoolControlsManager::CCMControl::DrawBorder()
{
    HDC     hDC;
    RECT    rect;
    RECT    rc;
    POINT   point = { 0, 0 };

    PrepareDraw( hDC, rect );

    if ( GetWindowLong( m_hWnd, GWL_EXSTYLE ) & WS_EX_CLIENTEDGE )
    {
        GetWindowRect( m_hWnd, &rc );
        ClientToScreen( m_hWnd, &point );
        if ( point.x == rc.left + 3 )
            InflateRect( &rect, -1, -1 );
    }

    m_nState &= ~dsFocus;
    if ( IsFocused() == TRUE )
        m_nState |= dsFocus;

    // Single line control looks better when this style is added
    m_nState &= ~dsAlternate;
    if ( ( rect.bottom - rect.top ) < 30 )
        m_nState |= dsAlternate;

    if ( ( m_nOldState & dsHover && m_nState & dsHover ) ||
         ( m_nOldState & dsFocus && m_nState & dsFocus ) )
        ; // If previous state is the same as current state, do nothing
    else
    {
        // Perform control-specific drawing routines
        CCM_TRACE( _T( "CCoolControlsManager::DrawBorder, handle: %X state: %d\n" ), m_hWnd, m_nState );
        DrawControl( hDC, rect );
    }

    // Update old state
    m_nOldState = m_nState;

    ReleaseDC( WindowFromDC( hDC ), hDC );
}

BOOL CCoolControlsManager::CCMControl::NeedRedraw( const POINT& /*point*/ )
{
    return m_hWnd != m_hWndOld ? TRUE : FALSE;
}

void CCoolControlsManager::CCMCore::Subclass( HWND hWnd, WNDPROC wndNewProc )
{
    ASSERT( hWnd );      // Do you really want to subclass a window that has a NULL handle?
    m_hWnd = hWnd;

    // Store address of the original window procedure
    m_oldWndProc = (WNDPROC)GetWindowLongPtr( m_hWnd, GWLP_WNDPROC);

    // And set the new one
    SetWindowLongPtr( m_hWnd, GWLP_WNDPROC, (LONG_PTR)wndNewProc );
}

void CCoolControlsManager::CCMCore::Unsubclass()
{
    SetWindowLongPtr( m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_oldWndProc );
}

LRESULT CCoolControlsManager::CCMControl::WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( nMsg )
    {
        // Generic messages
    case WM_KILLFOCUS:
    case WM_SETFOCUS:
        DrawBorder();
        break;

    case WM_PAINT:
    case WM_NCPAINT:
    case WM_ENABLE:
        CallWindowProc( m_oldWndProc, m_hWnd, nMsg, wParam, lParam );
        DrawBorder();
        return 0;

    case WM_NCDESTROY:
        // Unsubclass window and remove it from the map
        CallWindowProc( m_oldWndProc, m_hWnd, nMsg, wParam, lParam );
        g_ctrlManager.RemoveControl( m_hWnd );
        return 0;
    }

    return CallWindowProc( m_oldWndProc, m_hWnd, nMsg, wParam, lParam );
}

void CCoolControlsManager::CCMControl::DrawControl( HDC hDC, const RECT& rc )
{
    if ( m_nState & dsHoverMask )
    {
        Draw3dBorder( hDC, rc, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT,
            COLOR_3DDKSHADOW, COLOR_3DLIGHT );
    }
    else
    {
        if ( IsWindowEnabled( m_hWnd ) == FALSE || m_nState & dsDisabled )
            Draw3dBorder( hDC, rc, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT,
                          COLOR_3DFACE, COLOR_3DFACE );
        else
            Draw3dBorder( hDC, rc, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT,
                          m_nState & dsAlternate ? COLOR_3DHIGHLIGHT : COLOR_3DLIGHT, COLOR_3DLIGHT );
    }

    DrawScrollBars( hDC, rc );
}

void CCoolControlsManager::CCMControl::DrawScrollbarThumb( HDC hDC, const RECT& rc )
{
    /* Hover */
    if ( m_nState & dsHoverMask )
    {
        Draw3dBorder( hDC, rc, COLOR_3DFACE, COLOR_3DDKSHADOW,
                      COLOR_3DHIGHLIGHT, COLOR_3DSHADOW );
        return;
    }

    /* Not Hover */
    Draw3dBorder( hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DSHADOW,
                  COLOR_3DFACE, COLOR_3DFACE );
}

void CCoolControlsManager::CCMControl::DrawScrollBars( HDC hDC, const RECT& rect )
{
    const int iFrameSize  = GetSystemMetrics( SM_CXEDGE );
    const int iScrollSize = GetSystemMetrics( SM_CXHSCROLL );

    RECT rc;
    DWORD dwStyle = GetWindowLong( m_hWnd, GWL_STYLE );

    if ( dwStyle & WS_HSCROLL &&  dwStyle & WS_VSCROLL )
    {
        rc.left     = rect.left   + iFrameSize;
        rc.top      = rect.bottom - iFrameSize - iScrollSize;
        rc.right    = rect.right  - iFrameSize - iScrollSize;
        rc.bottom   = rect.bottom - iFrameSize;
        DrawScrollBar( hDC, rc, SB_HORZ );

        rc.left     = rect.right  - iFrameSize - iScrollSize;
        rc.top      = rect.top    + iFrameSize;
        rc.right    = rect.right  - iFrameSize;
        rc.bottom   = rect.bottom - iFrameSize - iScrollSize;
        DrawScrollBar( hDC, rc, SB_VERT );
    }
    else if ( dwStyle & WS_VSCROLL )
    {
        rc.left     = rect.right  - iFrameSize - iScrollSize;
        rc.top      = rect.top    + iFrameSize;
        rc.right    = rect.right  - iFrameSize;
        rc.bottom   = rect.bottom - iFrameSize;
        DrawScrollBar( hDC, rc, SB_VERT );
    }
    else if ( dwStyle & WS_HSCROLL )
    {
        rc.left     = rect.left   + iFrameSize;
        rc.top      = rect.bottom - iFrameSize - iScrollSize;
        rc.right    = rect.right  - iFrameSize;
        rc.bottom   = rect.bottom - iFrameSize;
        DrawScrollBar( hDC, rc, SB_HORZ );
    }
}

void CCoolControlsManager::CCMControl::DrawScrollArrow( HDC hDC, const RECT& rect, int iType, RECT& rcArrow1)
{
    int         iScrollSize = 0;
    RECT        rcArrow2;  /* 右或者下箭头范围 */
    
    iScrollSize = GetSystemMetrics( SM_CXHSCROLL );

    // Calculate the arrow rectangles
    rcArrow1 = rect;
    rcArrow2 = rect;
    if ( iType == SB_HORZ )
    {
        if ( ( rect.right - rect.left ) < 2 * iScrollSize )
            iScrollSize = ( rect.right - rect.left ) / 2;

        rcArrow1.right = rect.left  + iScrollSize;
        rcArrow2.left  = rect.right - iScrollSize;
    }
    else // SB_VERT
    {
        if ( ( rect.bottom - rect.top ) < 2 * iScrollSize )
            iScrollSize = ( rect.bottom - rect.top ) / 2;

        rcArrow1.bottom = rect.top    + iScrollSize;
        rcArrow2.top    = rect.bottom - iScrollSize;
    }

    // Draw the scrollbar arrows
    DrawScrollbarThumb( hDC, rcArrow1 );
    DrawScrollbarThumb( hDC, rcArrow2 );

    return;
}

void CCoolControlsManager::CCMControl::DrawScrollVThumb( HDC hDC, const RECT& rect,
                                                         RECT rcArrow1, BOOL bScrollbarCtrl )
{
    // The minimal thumb size depends on the system version
    // For Windows 98 minimal thumb size is a half of scrollbar size
    // and for Windows NT is always 8 pixels regardless of system metrics.
    // I really don't know why.
    int         iMinThumbSize = 8; /* 非win98大小 */
    int         iScrollSize = 0;
    int         iRange      = 0;
    int         iScrollArea = 0;
    int         iThumbSize  = 0;
    int         iThumbPos   = 0;
    int         iMulDiv     = 0;
    RECT        rcThumb;    /* 滚动条位置 */
    SCROLLINFO  si;

    rcThumb = rcArrow1;
    
    iScrollSize = GetSystemMetrics( SM_CXHSCROLL );

    //if ( GetVersion() & 0x80000000 ) // Windows 98 code
    //    iMinThumbSize = iScrollSize / 2;

    si.cbSize = sizeof( SCROLLINFO );
    si.fMask = SIF_ALL;
    GetScrollInfo( m_hWnd, bScrollbarCtrl ? SB_CTL : SB_VERT, &si );

    // Calculate the size and position of the thumb
    iRange = si.nMax - si.nMin + 1;
    ASSERT_ZERO(iRange);
    
    iScrollArea = ( rect.bottom - rect.top ) - 2 * iScrollSize;

    iThumbSize = GetSystemMetrics( SM_CXHTHUMB );
    // If nPage is not set then thumb has default size
    if ( si.nPage != 0 ) 
    {
        iMulDiv    = MulDiv( si.nPage ,iScrollArea, iRange );
        iThumbSize = max( iMulDiv, iMinThumbSize );
    }

    if ( iThumbSize >= iScrollArea )
    {
        iThumbSize = iScrollArea;

        COMP_BFALSE(bScrollbarCtrl);
    }

    if ( UINT( iRange ) == si.nPage )
    {
        iThumbPos = 0;
        iThumbSize--;
    }
    else
        iThumbPos = MulDiv( si.nPos - si.nMin, iScrollArea - iThumbSize, iRange - si.nPage );

    rcThumb.top    += iScrollSize  + iThumbPos;
    rcThumb.bottom  = rcArrow1.top + iThumbSize;

    // Don't draw the thumb when it's larger than the scroll area
    if ( iThumbSize <= iScrollArea )
        DrawScrollbarThumb( hDC, rcThumb );
}

void CCoolControlsManager::CCMControl::DrawScrollHThumb( HDC hDC, const RECT& rect, 
                                                         RECT rcArrow1, BOOL bScrollbarCtrl )
{
    // The minimal thumb size depends on the system version
    // For Windows 98 minimal thumb size is a half of scrollbar size
    // and for Windows NT is always 8 pixels regardless of system metrics.
    // I really don't know why.
    int         iMinThumbSize = 8; /* 非win98大小 */
    int         iScrollSize = 0;
    int         iRange      = 0;
    int         iScrollArea = 0;
    int         iThumbSize  = 0;
    int         iThumbPos   = 0;
    int         iMulDiv     = 0;
    RECT        rcThumb;    /* 滚动条位置 */
    SCROLLINFO  si;

    rcThumb = rcArrow1;
    
    iScrollSize = GetSystemMetrics( SM_CXHSCROLL );
    //if ( GetVersion() & 0x80000000 ) // Windows 98 code
    //    iMinThumbSize = iScrollSize / 2;

    si.cbSize = sizeof( SCROLLINFO );
    si.fMask = SIF_ALL;
    GetScrollInfo( m_hWnd, bScrollbarCtrl ? SB_CTL : SB_HORZ, &si );

    // Calculate the size and position of the thumb
    iRange = si.nMax - si.nMin + 1;
    ASSERT_ZERO(iRange);
    
    iScrollArea = ( rect.right - rect.left ) - 2 * iScrollSize;

    iThumbSize = GetSystemMetrics( SM_CXHTHUMB );
    // If nPage is not set then thumb has default size
    if ( si.nPage != 0 )
    {
        iMulDiv    = MulDiv( si.nPage ,iScrollArea, iRange );
        iThumbSize = max( iMulDiv, iMinThumbSize );
    }

    if ( iThumbSize >= iScrollArea )
    {
        iThumbSize = iScrollArea;

        COMP_BFALSE(bScrollbarCtrl);
    }

    if ( UINT( iRange ) == si.nPage )
    {
        iThumbPos = 0;
        iThumbSize--;
    }
    else
        iThumbPos = MulDiv( si.nPos - si.nMin, iScrollArea - iThumbSize, iRange - si.nPage );

    rcThumb.left += iScrollSize + iThumbPos;
    rcThumb.right = rcArrow1.left + iThumbSize;

    // Don't draw the thumb when it's larger than the scroll area
    if ( iThumbSize <= iScrollArea )
        DrawScrollbarThumb( hDC, rcThumb );
}

void CCoolControlsManager::CCMControl::DrawScrollBar( HDC hDC, const RECT& rect,
                                                      int iType, BOOL bScrollbarCtrl )
{
    RECT        rcArrow1;    /* 左或者上箭头范围 */

    // Draw the scrollbar arrows
    DrawScrollArrow(hDC, rect, iType, rcArrow1);

    if ( bScrollbarCtrl == TRUE && IsWindowEnabled( m_hWnd ) == FALSE )
        return;

    // Draw the scrollbar thumb
    if(iType == SB_VERT)
    {
        DrawScrollVThumb(hDC, rect, rcArrow1, bScrollbarCtrl);
        return;
    }

    DrawScrollHThumb(hDC, rect, rcArrow1, bScrollbarCtrl);
    return;
}

BOOL CCoolControlsManager::CCMControl::IsFocused()
{
    return m_hWnd == GetFocus() ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////////
// CCMEdit class

void CCoolControlsManager::CCMEdit::DrawMSUpDownCtrl(HWND hWnd, RECT rect)
{
    RECT        rcTemp;
    DWORD       dwStyle = 0;
    HDC         hDCTemp;
    COLORREF    clr;
    int         iEdge;

    dwStyle = GetWindowLong( hWnd, GWL_STYLE );
    if ( ( dwStyle & UDS_ALIGNRIGHT || dwStyle & UDS_ALIGNLEFT ) &&
        SendMessage( hWnd, UDM_GETBUDDY, 0, 0L ) == (LONG)m_hWnd )
    {
        GetWindowRect( hWnd, &rcTemp );
        iEdge = GetSystemMetrics( SM_CXEDGE );
        if ( dwStyle & UDS_ALIGNRIGHT )
            rect.right += ( rcTemp.right - rcTemp.left ) - iEdge;
        else // UDS_ALIGNLEFT
            rect.left  -= ( rcTemp.right - rcTemp.left ) - iEdge;

        hDCTemp = GetDC( hWnd );   // We must draw the lines onto spin control DC

        clr = GetSysColor( m_nState & dsHoverMask ? COLOR_3DDKSHADOW : COLOR_3DHIGHLIGHT );
        if ( !IsWindowEnabled( m_hWnd ) )
            clr = GetSysColor( COLOR_3DFACE );
        FillSolidRect( hDCTemp, 1, 1, rcTemp.right - rcTemp.left - iEdge - 1, 1, clr );
        if ( dwStyle & UDS_ALIGNLEFT )
            FillSolidRect( hDCTemp, 1, 1, 1, rcTemp.bottom - rcTemp.top - iEdge - 1, clr );

        ReleaseDC( hWnd, hDCTemp );
    }

    return;
}

void CCoolControlsManager::CCMEdit::DrawControl( HDC hDC, const RECT& rc )
{
    RECT    rect = rc;
    TCHAR   szBuf[MAX_CLASSNAME];
    
    // Check if edit window has an associated up-down control.
    // If so draw a border around both controls
    HWND    hWnd;

    hWnd = GetNextWindow( m_hWnd, GW_HWNDNEXT );

    if ( hWnd )
    {
        // Retrieve window class name
        GetClassName( hWnd, szBuf, MAX_CLASSNAME );
        // Up-down is found
        if ( lstrcmpi( szBuf, _T( "msctls_updown32" ) ) == 0 )
        {
            DrawMSUpDownCtrl(hWnd, rect);
        }
    }

    if ( GetWindowLong( m_hWnd, GWL_STYLE ) & ES_READONLY )
        m_nState |= dsDisabled;
    else
        m_nState &= ~dsDisabled;
    CCMControl::DrawControl( hDC, rect );
}

LRESULT CCoolControlsManager::CCMEdit::WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam )
{
    HWND    hWnd;
    TCHAR   szBuf[MAX_CLASSNAME];

    switch ( nMsg )
    {
    case WM_ENABLE:
        {
            CallWindowProc( m_oldWndProc, m_hWnd, nMsg, wParam, lParam );
            DrawBorder();
            hWnd = GetNextWindow( m_hWnd, GW_HWNDNEXT );
            if ( hWnd )
            {
                // Retrieve window class name
                GetClassName( hWnd, szBuf, MAX_CLASSNAME );
                if ( lstrcmpi( szBuf, _T( "msctls_updown32" ) ) == 0 &&   // Up-down is found
                    SendMessage( hWnd, UDM_GETBUDDY, 0, 0L ) == (LONG)m_hWnd )
                    SendMessage( hWnd, WM_PAINT, 0, 0L );  // Repaint up-down too
            }
        }
        return 0;

    default:
        return CCMControl::WindowProc( nMsg, wParam, lParam );
    }
}

//////////////////////////////////////////////////////////////////////////////
// CCMComboBox class

void CCoolControlsManager::CCMComboBox::DrawControl( HDC hDC, const RECT& rect )
{
    RECT    rc;

    // First, draw borders around the control
    CCMControl::DrawControl( hDC, rect );

    // Now, we have to draw borders around the drop-down arrow
    rc = rect;
    InflateRect( &rc, -2, -2 );
    rc.left = rc.right - GetSystemMetrics( SM_CXHSCROLL );

    if ( IsWindowEnabled( m_hWnd ) == TRUE )
    {
        if ( m_nState & dsHoverMask )
            Draw3dBorder( hDC, rc, COLOR_3DFACE, COLOR_3DDKSHADOW,
                          COLOR_3DHIGHLIGHT, COLOR_3DSHADOW );
        else
            Draw3dBorder( hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DSHADOW,
                          COLOR_3DFACE, COLOR_3DFACE );
    }
    else
        Draw3dBorder( hDC, rc, COLOR_3DFACE, COLOR_3DFACE,
                      COLOR_3DFACE, COLOR_3DFACE );
}

BOOL CCoolControlsManager::CCMComboBox::IsFocused()
{
    HWND    hWnd = NULL;

    // Special case for drop-down and simple ComboBoxes
    // which contain child edit control and focus always
    // goes to that edit window
    if ( ( GetWindowLong( m_hWnd, GWL_STYLE ) & 0x03 ) == CBS_DROPDOWN )
    {
        hWnd = GetTopWindow( m_hWnd );
        if ( hWnd && hWnd == GetFocus() )
            return TRUE;
    }

    return CCMControl::IsFocused();
}

LRESULT CCoolControlsManager::CCMComboBox::WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( nMsg )
    {
        // Drop-down ComboBox receives neither WM_SETFOCUS nor WM_KILLFOCUS
        // Instead, it receives these next two messages
    case WM_LBUTTONUP:      // For kill focus
        if ( lParam == -1 )
            DrawBorder();
        break;

    case WM_COMMAND:
        if ( HIWORD( wParam ) == EN_SETFOCUS )
            DrawBorder();
        break;

    default:
        return CCMControl::WindowProc( nMsg, wParam, lParam );
    }

    return CallWindowProc( m_oldWndProc, m_hWnd, nMsg, wParam, lParam );
}

//////////////////////////////////////////////////////////////////////////////
// CCMDateTime class

void CCoolControlsManager::CCMDateTime::DrawControl( HDC hDC, const RECT& rc )
{
    if ( GetWindowLong( m_hWnd, GWL_STYLE ) & DTS_UPDOWN )
        CCMControl::DrawControl( hDC, rc );
    else
        CCMComboBox::DrawControl( hDC, rc );
}

//////////////////////////////////////////////////////////////////////////////
// CCMPushButton class

void CCoolControlsManager::CCMPushButton::DrawControl( HDC hDC, const RECT& rc )
{
    BOOL    bDefault = FALSE;
    DWORD   dwStyle  = 0;
    int     iCheck   = 0;

    // Unfortunately BS_DEFPUSHBUTTON is defined as 0x00000001L,
    // and BS_OWNERDRAW as 0x0000000BL (see winuser.h) so we cannot
    // determine the default button for owner-draw controls
    dwStyle = GetWindowLong( m_hWnd, GWL_STYLE ) & BS_OWNERDRAW;

    if ( dwStyle != BS_OWNERDRAW )
    {
        if ( dwStyle == BS_DEFPUSHBUTTON && IsWindowEnabled( m_hWnd ) )
            bDefault = TRUE;
    }

    iCheck = SendMessage( m_hWnd, BM_GETCHECK, 0, 0 );
    /* Hover */
    if ( m_nState & dsHoverMask )
    {
        if ( bDefault == TRUE )
        {
            Draw3dBorder( hDC, rc, COLOR_3DDKSHADOW, COLOR_3DDKSHADOW,
                          COLOR_3DHIGHLIGHT, COLOR_3DDKSHADOW,
                          COLOR_3DLIGHT, COLOR_3DSHADOW );
            return;
        }
        
        if ( iCheck )
        {
            Draw3dBorder( hDC, rc, COLOR_3DDKSHADOW, COLOR_3DHIGHLIGHT,
                          COLOR_3DSHADOW, COLOR_3DLIGHT );
            return;
        }
        Draw3dBorder( hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DDKSHADOW,
                      COLOR_3DLIGHT, COLOR_3DSHADOW,
                      COLOR_3DFACE, COLOR_3DFACE );
        return;
    }

    if ( bDefault == TRUE )
    {
        Draw3dBorder( hDC, rc, COLOR_3DDKSHADOW, COLOR_3DDKSHADOW,
                      COLOR_3DHIGHLIGHT, COLOR_3DSHADOW,
                      COLOR_3DFACE, COLOR_3DFACE );
        return;
    }
    if ( iCheck )
    {
        Draw3dBorder( hDC, rc, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT,
                      COLOR_3DFACE, COLOR_3DFACE );
        return;
    }
    
    Draw3dBorder( hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DSHADOW,
                  COLOR_3DFACE, COLOR_3DFACE,
                  COLOR_3DFACE, COLOR_3DFACE );
    return;

}

LRESULT CCoolControlsManager::CCMPushButton::WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( nMsg )
    {
    // Button messages
    case BM_SETCHECK:
    case WM_SETTEXT:
        CallWindowProc( m_oldWndProc, m_hWnd, nMsg, wParam, lParam );
        DrawBorder();
        return 0;

    default:
        return CCMControl::WindowProc( nMsg, wParam, lParam );
    }
}

//////////////////////////////////////////////////////////////////////////////
// CCMCheckBox class

// Checkmark square size, hard coded here because I couldn't find any
// method to obtain this value from the system.
// Maybe someone else knows how to do it? If so, please let me know!
RECT CCoolControlsManager::CCMCheckBox::ReCalcRect( RECT rectSrc, DWORD   dwStyle)
{
    RECT    rc;
    int     iCheckSize  = 13;
    
    rc.top = rectSrc.top + ( ( rectSrc.bottom - rectSrc.top ) - iCheckSize ) / 2;    // Default top
    if ( ( dwStyle & BS_VCENTER ) == BS_VCENTER )
        rc.top = rectSrc.top + ( ( rectSrc.bottom - rectSrc.top ) - iCheckSize ) / 2;
    else if ( dwStyle & BS_TOP )
        rc.top = rectSrc.top + 1;
    else if ( dwStyle & BS_BOTTOM )
        rc.top = rectSrc.bottom - iCheckSize - 2;
        
    rc.left = rectSrc.left;    // default left
    if ( dwStyle & BS_LEFTTEXT )
        rc.left = rectSrc.right - iCheckSize;

    rc.right = rc.left + iCheckSize;
    rc.bottom = rc.top + iCheckSize;

    return rc;
}

void CCoolControlsManager::CCMCheckBox::DrawControl( HDC hDC, const RECT& rect )
{
    RECT    rcNew;
    int     iState      = 0;
    DWORD   dwStyle     = GetWindowLong( m_hWnd, GWL_STYLE );

    if ( dwStyle & BS_PUSHLIKE )
    {
        CCMPushButton::DrawControl( hDC, rect );
        return;
    }

    rcNew = ReCalcRect(rect, dwStyle);

    /* Hover */
    if ( m_nState & dsHoverMask )
    {
        Draw3dBorder( hDC, rcNew, COLOR_3DDKSHADOW, COLOR_3DHIGHLIGHT,
                      COLOR_3DSHADOW, COLOR_3DFACE );
        return;
    }

    /* Enable */
    if ( IsWindowEnabled( m_hWnd ) == TRUE )
    {
        iState = SendMessage( m_hWnd, BM_GETCHECK, 0, 0L );
        Draw3dBorder( hDC, rcNew, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT,
                      iState == 2 ? COLOR_3DHIGHLIGHT : COLOR_WINDOW,
                      iState == 2 ? COLOR_3DHIGHLIGHT : COLOR_WINDOW );
        return;
    }

    /* Normal */
    Draw3dBorder( hDC, rcNew, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT,
                  COLOR_3DFACE, COLOR_3DFACE );
    return;
}

//////////////////////////////////////////////////////////////////////////////
// CCMRadioButton class

void CCoolControlsManager::CCMRadioButton::DrawFrame( POINT* ptArr, int iColor,
                                                      HDC hDC, int xOff, int yOff )
{
    int i = 0;

    ASSERT_NULL(ptArr);
    
    for ( i = 0; ; i++ )
    {
        if ( !ptArr[i].x && !ptArr[i].y )
            return;

        SetPixel( hDC, ptArr[i].x + xOff, ptArr[i].y + yOff, GetSysColor( iColor ) );
    }
}

void CCoolControlsManager::CCMRadioButton::DrawControl( HDC hDC, const RECT& rect )
{
    RECT    rc;
    int     iRadioSize  = 12;
    DWORD   dwStyle     = GetWindowLong( m_hWnd, GWL_STYLE );
    POINT pt1[] = {
                    { 1,9 },{ 1,8 },
                    { 0,7 },{ 0,6 },{ 0,5 },{ 0,4 },
                    { 1,3 },{ 1,2 },
                    { 2,1 },{ 3,1 },
                    { 4,0 },{ 5,0 },{ 6,0 },{ 7,0 },
                    { 8,1 },{ 9,1 },
                    { 0,0 }
    };
    POINT pt2[] = {
                    { 2,8 },
                    { 1,7 },{ 1,6 },{ 1,5 },{ 1,4 },
                    { 2,3 },{ 2,2 },
                    { 3,2 },
                    { 4,1 },{ 5,1 },{ 6,1 },{ 7,1 },
                    { 8,2 },{ 9,2 },
                    { 0,0 }
    };
    POINT pt3[] = {
                    { 2,9 },{ 3,9 },
                    { 4,10 },{ 5,10 },{ 6,10 },{ 7,10 },
                    { 8,9 },{ 9,9 },
                    { 9,8 },
                    { 10,7 },{ 10,6 },{ 10,5 },{ 10,4 },
                    { 9,3 },
                    { 0,0 }
    };
    POINT pt4[] = {
                    { 2,10 },{ 3,10 },
                    { 4,11 },{ 5,11 },{ 6,11 },{ 7,11 },
                    { 8,10 },{ 9,10 },
                    { 10,9 },{ 10,8 },
                    { 11,7 },{ 11,6 },{ 11,5 },{ 11,4 },
                    { 10,3 },{ 10,2 },
                    { 0,0 }
    };

    if ( dwStyle & BS_PUSHLIKE )
    {
        CCMPushButton::DrawControl( hDC, rect );
        return;
    }

    rc.top = rect.top + ( ( rect.bottom - rect.top ) - iRadioSize - 1 ) / 2; // Default top
    if ( ( dwStyle & BS_VCENTER ) == BS_VCENTER )
        rc.top = rect.top + ( ( rect.bottom - rect.top ) - iRadioSize - 1 ) / 2;
    else if ( dwStyle & BS_TOP )
        rc.top = rect.top + 1;
    else if ( dwStyle & BS_BOTTOM )
        rc.top = rect.bottom - iRadioSize - 3;

    rc.left = rect.left + 1;    // Default left
    if ( dwStyle & BS_LEFTTEXT )
        rc.left = rect.right - iRadioSize;

    rc.right = rc.left + iRadioSize;
    rc.bottom = rc.top + iRadioSize;

    /* Hover */
    if ( m_nState & dsHoverMask )
    {
        DrawFrame(  pt1, COLOR_3DSHADOW,   hDC, rc.left, rc.top );
        DrawFrame(  pt2, COLOR_3DDKSHADOW, hDC, rc.left, rc.top );
        DrawFrame(  pt3, COLOR_3DFACE,     hDC, rc.left, rc.top );
        DrawFrame(  pt4, COLOR_WINDOW,     hDC, rc.left, rc.top );
        return;
    }

    /* Enable */
    if ( IsWindowEnabled( m_hWnd ) == TRUE )
    {
        DrawFrame(  pt1, COLOR_3DSHADOW, hDC, rc.left, rc.top );
        DrawFrame(  pt2, COLOR_WINDOW,   hDC, rc.left, rc.top );
        DrawFrame(  pt3, COLOR_3DFACE,   hDC, rc.left, rc.top );
        DrawFrame(  pt4, COLOR_WINDOW,   hDC, rc.left, rc.top );
        return;
    }

    /* Normal */
    DrawFrame(  pt1, COLOR_3DSHADOW,    hDC, rc.left, rc.top );
    DrawFrame(  pt2, COLOR_3DFACE,      hDC, rc.left, rc.top );
    DrawFrame(  pt3, COLOR_3DFACE,      hDC, rc.left, rc.top );
    DrawFrame(  pt4, COLOR_3DHIGHLIGHT, hDC, rc.left, rc.top );
    return;
}

//////////////////////////////////////////////////////////////////////////////
// CCMUpDown class

void CCoolControlsManager::CCMUpDown::DrawButton( HDC hDC, const RECT& rc )
{
    BOOL    bEnabled = FALSE;
    HWND    hWnd;

    // If associated edit control is disabled
    // draw the up-down as disabled too
    bEnabled = IsWindowEnabled( m_hWnd );
    hWnd = (HWND)SendMessage( m_hWnd, UDM_GETBUDDY, 0, 0L );
    if ( hWnd )
        bEnabled = IsWindowEnabled( hWnd );

    /* Enable & Hover */
    if ( bEnabled && m_nState & dsHoverMask )
    {
        Draw3dBorder( hDC, rc, COLOR_3DFACE, COLOR_3DDKSHADOW,
                      COLOR_3DHIGHLIGHT, COLOR_3DSHADOW );
        return;
    }

    /*Normal*/
    Draw3dBorder( hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DSHADOW,
                  COLOR_3DFACE, COLOR_3DFACE );
}

void CCoolControlsManager::CCMUpDown::DrawAlignLeft(HDC hDC, RECT& rect, CCMControl *pCtl, BOOL bEnable)
{
    COLORREF    clr1;
    COLORREF    clr2;

    ASSERT_NULL(pCtl);

    COMP_BTRUE(pCtl->GetState() & dsHoverMask);

    rect.left   += 2;
    rect.top    += 2;
    rect.bottom -= 2;

    clr1 = clr2 = GetSysColor(COLOR_3DFACE);
    if(bEnable)
    {
        clr1 = GetSysColor(COLOR_3DHIGHLIGHT);
        clr2 = GetSysColor(COLOR_3DLIGHT);
    }
    FillSolidRect(  hDC, rect.left, rect.top - 1,
                    rect.right, 1,
                    clr1 );
    FillSolidRect(  hDC, rect.left, rect.bottom,
                    rect.right, 1,
                    clr2 );
    FillSolidRect(  hDC, rect.left - 1, rect.top - 1,
                    1, rect.bottom,
                    clr1 );
}

void CCoolControlsManager::CCMUpDown::DrawAlignRight(HDC hDC, RECT& rect, CCMControl *pCtl, BOOL bEnable)
{
    COLORREF    clr1;
    COLORREF    clr2;
    
    ASSERT_NULL(pCtl);
    COMP_BTRUE(pCtl->GetState() & dsHoverMask);

    rect         = rect;
    rect.right  -= 2;
    rect.top    += 2;
    rect.bottom -= 2;

    clr1 = clr2 = GetSysColor(COLOR_3DFACE);
    if(bEnable)
    {
        clr1 = GetSysColor(COLOR_3DHIGHLIGHT);
        clr2 = GetSysColor(COLOR_3DLIGHT);
    }
    FillSolidRect(  hDC, rect.left, rect.top - 1,
                    rect.right, 1,
                    clr1 );
    FillSolidRect(  hDC, rect.left, rect.bottom,
                    rect.right, 1,
                    clr2 );
    FillSolidRect(  hDC, rect.right, rect.top - 1,
                    1, rect.bottom,
                    clr2 );
}

void CCoolControlsManager::CCMUpDown::DrawAlign(HDC hDC, RECT& rect, HWND hWnd, DWORD dwStyle)
{
    CCMControl  *pCtl   = NULL;
    BOOL        bEnable = FALSE;

    ASSERT_NULL(hWnd);

    g_ctrlManager.m_ctrlMap.Lookup( hWnd, (void*&)pCtl);
    bEnable = IsWindowEnabled( hWnd );

    if(dwStyle & UDS_ALIGNLEFT)
        DrawAlignLeft(hDC, rect, pCtl, bEnable);
    else if(dwStyle & UDS_ALIGNRIGHT)
        DrawAlignRight(hDC, rect, pCtl, bEnable);
    
    return;
}

void CCoolControlsManager::CCMUpDown::DrawControl( HDC hDC, const RECT& rect )
{
    RECT        AlignRC;
    RECT        rc;
    CCMControl  *pCtl   = NULL;
    DWORD       dwStyle = GetWindowLong( m_hWnd, GWL_STYLE );
    HWND        hWnd    = (HWND)SendMessage( m_hWnd, UDM_GETBUDDY, 0, 0L );

    AlignRC = rect;
    DrawAlign(hDC, AlignRC, hWnd, dwStyle);

    rc = AlignRC;
    if ( dwStyle & UDS_HORZ )
    {
        rc.right = rc.left + ( AlignRC.right - AlignRC.left ) / 2;
        DrawButton( hDC, rc );

        rc.left = rc.right + ( AlignRC.right - AlignRC.left ) % 2;
        rc.right = AlignRC.right;
        DrawButton( hDC, rc );
    }
    else
    {
        rc.bottom = rc.top + ( AlignRC.bottom - AlignRC.top ) / 2;
        DrawButton( hDC, rc );

        rc.top = rc.bottom + ( AlignRC.bottom - AlignRC.top ) % 2;
        rc.bottom = AlignRC.bottom;
        DrawButton( hDC, rc);
    }

    hWnd = GetParent( m_hWnd );
    g_ctrlManager.m_ctrlMap.Lookup( hWnd, (void*&)pCtl );

    if ( pCtl && IsWindowEnabled( hWnd ) )   // Redraw parent or buddy if neccesary
    {
        if ( m_nState & dsHoverMask )
        {
            pCtl->SetState( 0, dsHover );
            return;
        }
        /* Not Hover */
        pCtl->SetState( dsHover, 0 );
    }
}

//////////////////////////////////////////////////////////////////////////////
// CCMEditCombo class

void CCoolControlsManager::CCMEditCombo::PrepareDraw( HDC& hDC, RECT& rect )
{
    GetWindowRect( m_hWnd, &rect );
    InflateRect( &rect, 3, 3 );
    OffsetRect( &rect, -rect.left, -rect.top );
    // Draw onto that DC that is most suitable for given class
    hDC = GetWindowDC( GetParent( m_hWnd ) );
}

//////////////////////////////////////////////////////////////////////////////
// CCMScrollBar class

void CCoolControlsManager::CCMScrollBar::DrawControl( HDC hDC, const RECT& rc )
{
    DrawScrollBar( hDC, rc,
                   ( GetWindowLong( m_hWnd, GWL_STYLE ) & SBS_VERT ) ? SB_VERT : SB_HORZ,
                   TRUE );
}

LRESULT CCoolControlsManager::CCMScrollBar::WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( nMsg )
    {
        // Scrollbar messages
    case SBM_SETPOS:
        if ( !lParam )  // redraw flag
            break;
        break;

    case SBM_SETSCROLLINFO:
        if ( !wParam )  // redraw flag
            break;
        break;

    case SBM_SETRANGEREDRAW:
        CallWindowProc( m_oldWndProc, m_hWnd, nMsg, wParam, lParam );
        DrawBorder();
        return 0;

    default:
        return CCMControl::WindowProc( nMsg, wParam, lParam );
    }

    return CallWindowProc( m_oldWndProc, m_hWnd, nMsg, wParam, lParam );
}

//////////////////////////////////////////////////////////////////////////////
// CCMHeaderCtrl class

void CCoolControlsManager::CCMHeaderCtrl::DrawButton( HDC hDC, const RECT& rc, int iState )
{
    if ( iState & dsHoverMask )
    {
        Draw3dBorder( hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DDKSHADOW,
                      COLOR_3DLIGHT, COLOR_3DSHADOW );
        return;
    }

    /* else */
    Draw3dBorder( hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DSHADOW,
                  COLOR_3DFACE, COLOR_3DFACE );
}

void CCoolControlsManager::CCMHeaderCtrl::DrawControl( HDC hDC, const RECT& /*rc*/ )
{
    int     i           = 0;
    int     iTmp        = 0;
    int     iState      = 0;
    int     iCount      = 0;
    int     iOldItem    = 0;
    BOOL    bInView     = FALSE;
    RECT    rc;
    POINT   point;
    HWND    hWnd;
    HDITEM  hi;
    CCMControl  *pCtl   = NULL;

    iOldItem = m_iOldItem;
    m_iOldItem = -1;

    GetCursorPos( &point );

    // This code fails if we will have standalone header control but such cases are rare...
    hWnd = GetParent( m_hWnd );
    GetClientRect( GetParent( m_hWnd ), &rc );
    ScreenToClient( GetParent( m_hWnd ), &point );
    // Test if mouse pointer is within the client area of the list control
    bInView = PtInRect( &rc, point );

    GetClientRect( m_hWnd, &rc );
    rc.right = 0;
    GetCursorPos( &point );
    ScreenToClient( m_hWnd, &point );
    hDC = GetDC( m_hWnd );

    iCount = SendMessage( m_hWnd, HDM_GETITEMCOUNT, 0, 0L );

    for ( i = 0; i < iCount; i++ )
    {
#if (_WIN32_IE >= 0x0300)
        hi.mask = HDI_ORDER;
        SendMessage( m_hWnd, HDM_GETITEM, i, (LPARAM)&hi );
        SendMessage( m_hWnd, HDM_GETITEMRECT, hi.iOrder, (LPARAM)&rc );
#else
        SendMessage( m_hWnd, HDM_GETITEMRECT, i, (LPARAM)&rc );
#endif
        iState = 0;
        if ( bInView & PtInRect( &rc, point ) )
        {
            iState = dsHover;
#if (_WIN32_IE >= 0x0300)
            m_iOldItem = hi.iOrder;
#else
            m_iOldItem = i;
#endif
        }
        DrawButton( hDC, rc, iState );
    }

    iTmp    = rc.right;
    GetClientRect( m_hWnd, &rc );
    rc.left = iTmp;
    DrawButton( hDC, rc, 0 );

    // If header is a child of ListView, redraw the list so
    // it will indicate proper state
    if ( g_ctrlManager.m_ctrlMap.Lookup( hWnd, (void*&)pCtl ) )
    {
        if ( m_iOldItem >= 0 )
            pCtl->SetState( 0, dsHover );
        else if ( iOldItem >= 0 )
            pCtl->SetState( dsHover, 0 );
    }

    ReleaseDC( m_hWnd, hDC );
}

BOOL CCoolControlsManager::CCMHeaderCtrl::NeedRedraw( const POINT& point )
{
    int     i     = 0;
    int     iCount= 0;
    RECT    rc;
    POINT   pt;
    HDITEM  hi;

    COMP_BNE_R(m_hWnd, m_hWndOld, TRUE);

    GetClientRect( m_hWnd, &rc );
    rc.right = 0;

    pt = point;
    ScreenToClient( m_hWnd, &pt );

    iCount = SendMessage( m_hWnd, HDM_GETITEMCOUNT, 0, 0L );

    for ( i = 0; i < iCount; i++ )
    {
        hi.mask = HDI_WIDTH;
        SendMessage( m_hWnd, HDM_GETITEM, i, (LPARAM)&hi );
        rc.left  = rc.right;
        rc.right = rc.left + hi.cxy;
        if ( PtInRect( &rc, pt ) )
        {
            COMP_BNE_R(m_iOldItem, i, TRUE);
        }
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////
// CCMTrackbar class
void CCoolControlsManager::CCMTrackbar::CreateThumbPen( HPEN *pPen )
{
    ASSERT_NULL(pPen);

    /* Highlight Pen */
    pPen[0] = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_3DHIGHLIGHT ) );  
    
    /* Get Hover Mask Pen */
    if(m_nState & dsHoverMask)
    {
        pPen[1] = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_3DLIGHT ) );   /* light */
        pPen[2] = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_3DDKSHADOW ) );/* DkShadow */
        pPen[3] = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_3DSHADOW ) );  /* Shadow */
        return;
    }

    /* Get Not Hover Mask Pen Color*/
    pPen[1] = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_3DFACE ) );    /* light */
    pPen[2] = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_3DSHADOW ) );  /* DkShadow */
    pPen[3] = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_3DFACE ) );    /* Shadow */

    return;
}

void CCoolControlsManager::CCMTrackbar::DeleteThumbPen( HPEN *pPen )
{
    int i = 0;

    ASSERT_NULL(pPen);

    for(i=0; i<4; i++)
        DeleteObject( pPen[i] );

    return;
}


/* Draw TBS_BOTH Thumb */
void CCoolControlsManager::CCMTrackbar::DrawBothThumb( HDC hDC, const RECT& rc )
{
    FillSolidRect( hDC, rc, GetSysColor( COLOR_3DFACE ) );
    /* Hover Mask */
    if ( m_nState & dsHoverMask )
    {
        Draw3dBorder( hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DDKSHADOW,
                      COLOR_3DLIGHT, COLOR_3DSHADOW );
        return;
    }
    /* Not Hover Mask */
    Draw3dBorder( hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DSHADOW,
                  COLOR_3DFACE, COLOR_3DFACE );
    return;
}

/* Draw TBS_VERT and  TBS_LEFT Thumb */
void CCoolControlsManager::CCMTrackbar::DrawVLThumb( HDC hDC, const RECT& rc )
{
    int     n = 0;
    HPEN    penThumb[4];
    HPEN    hOldPen;

    /* 创建画Thumb笔 */
    CreateThumbPen(penThumb);

    n = ( rc.bottom - rc.top ) / 2 + 1;

    FillSolidRect( hDC, rc, GetSysColor( COLOR_3DFACE ) );

    hOldPen = (HPEN)SelectObject( hDC, penThumb[0] );
    MoveToEx( hDC, rc.right - 2, rc.top, NULL );
    LineTo( hDC, rc.left + n - 1, rc.top );
    LineTo( hDC, rc.left, rc.top + n - 1 );

    SelectObject( hDC, penThumb[2] );
    LineTo( hDC, rc.left + n - 1, rc.bottom - 1 );
    LineTo( hDC, rc.right - 1, rc.bottom - 1 );
    LineTo( hDC, rc.right - 1, rc.top - 1 );

    SelectObject( hDC, penThumb[1] );
    MoveToEx( hDC, rc.right - 3, rc.top + 1, NULL );
    LineTo( hDC, rc.left + n - 1, rc.top + 1 );
    LineTo( hDC, rc.left + 1, rc.top + n - 1 );

    SelectObject( hDC, penThumb[3] );
    LineTo( hDC, rc.left + n - 1, rc.bottom - 2 );
    LineTo( hDC, rc.right - 2, rc.bottom - 2 );
    LineTo( hDC, rc.right - 2, rc.top );

    SelectObject( hDC, hOldPen );

    /* 创建画Thumb笔 */
    DeleteThumbPen(penThumb);

}

/* Draw TBS_VERT and  TBS_RIGHT Thumb */
void CCoolControlsManager::CCMTrackbar::DrawVRThumb( HDC hDC, const RECT& rc )
{
    int     n = 0;
    HPEN    penThumb[4];
    HPEN    hOldPen;
    
    /* 创建画Thumb笔 */
    CreateThumbPen(penThumb);
    
    n = ( rc.bottom - rc.top ) / 2 + 1;

    FillSolidRect( hDC, rc, GetSysColor( COLOR_3DFACE ) );

    hOldPen = (HPEN)SelectObject( hDC, penThumb[0] );
    MoveToEx( hDC, rc.left, rc.bottom - 2, NULL );
    LineTo( hDC, rc.left, rc.top );
    LineTo( hDC, rc.right - n, rc.top );
    LineTo( hDC, rc.right - 1, rc.top + n - 1 );

    SelectObject( hDC, penThumb[2] );
    MoveToEx( hDC, rc.left, rc.bottom - 1, NULL );
    LineTo( hDC, rc.right - n, rc.bottom - 1 );
    LineTo( hDC, rc.right, rc.top + n - 2 );

    SelectObject( hDC, penThumb[1] );
    MoveToEx( hDC, rc.left + 1, rc.bottom - 3, NULL );
    LineTo( hDC, rc.left + 1, rc.top + 1 );
    LineTo( hDC, rc.right - n, rc.top + 1 );
    LineTo( hDC, rc.right - 2, rc.top + n - 1 );

    SelectObject( hDC, penThumb[3] );
    MoveToEx( hDC, rc.left + 1, rc.bottom - 2, NULL );
    LineTo( hDC, rc.right - n, rc.bottom - 2 );
    LineTo( hDC, rc.right - 1, rc.top + n - 2 );

    SelectObject( hDC, hOldPen );
    
    /* 删除画Thumb笔 */
    DeleteThumbPen(penThumb);
    
}

/* Draw TBS_HORZ and  TBS_TOP Thumb */
void CCoolControlsManager::CCMTrackbar::DrawHTThumb( HDC hDC, const RECT& rc )
{
    int     n = 0;
    HPEN    penThumb[4];
    HPEN    hOldPen;

    /* 创建画Thumb笔 */
    CreateThumbPen(penThumb);

    n = ( rc.right - rc.left ) / 2 + 1;

    FillSolidRect( hDC, rc, GetSysColor( COLOR_3DFACE ) );

    hOldPen = (HPEN)SelectObject( hDC, penThumb[0] );
    MoveToEx( hDC, rc.left + n - 2, rc.top + 1, NULL );
    LineTo( hDC, rc.left, rc.top + n - 1 );
    LineTo( hDC, rc.left, rc.bottom - 1 );

    SelectObject( hDC, penThumb[2] );
    LineTo( hDC, rc.right - 1, rc.bottom - 1 );
    LineTo( hDC, rc.right - 1, rc.top + n - 1 );
    LineTo( hDC, rc.left + n - 2, rc.top - 1 );

    SelectObject( hDC, penThumb[1] );
    MoveToEx( hDC, rc.left + n - 2, rc.top + 2, NULL );
    LineTo( hDC, rc.left + 1, rc.top + n - 1 );
    LineTo( hDC, rc.left + 1, rc.bottom - 2 );

    SelectObject( hDC, penThumb[3] );
    LineTo( hDC, rc.right - 2, rc.bottom - 2 );
    LineTo( hDC, rc.right - 2, rc.top + n - 1 );
    LineTo( hDC, rc.left + n - 2, rc.top );

    SelectObject( hDC, hOldPen );

    /* 删除画Thumb笔 */
    DeleteThumbPen(penThumb);
}

/* Draw TBS_HORZ and  TBS_BOTTOM Thumb */
void CCoolControlsManager::CCMTrackbar::DrawHBThumb( HDC hDC, const RECT& rc )
{
    int     n = 0;
    HPEN    penThumb[4];
    HPEN    hOldPen;

    /* 创建画Thumb笔 */
    CreateThumbPen(penThumb);

    n = ( rc.right - rc.left ) / 2 + 1;

    FillSolidRect( hDC, rc, GetSysColor( COLOR_3DFACE ) );

    hOldPen = (HPEN)SelectObject( hDC, penThumb[0] );
    MoveToEx( hDC, rc.left + n - 2, rc.bottom - 2, NULL );
    LineTo( hDC, rc.left, rc.bottom - n );
    LineTo( hDC, rc.left, rc.top );
    LineTo( hDC, rc.right - 1, rc.top );

    SelectObject( hDC, penThumb[2] );
    LineTo( hDC, rc.right - 1, rc.bottom - n );
    LineTo( hDC, rc.left + n - 2, rc.bottom );

    SelectObject( hDC, penThumb[1] );
    MoveToEx( hDC, rc.left + n - 2, rc.bottom - 3, NULL );
    LineTo( hDC, rc.left + 1, rc.bottom - n );
    LineTo( hDC, rc.left + 1, rc.top + 1 );
    LineTo( hDC, rc.right - 2, rc.top + 1 );

    SelectObject( hDC, penThumb[3] );
    LineTo( hDC, rc.right - 2, rc.bottom - n );
    LineTo( hDC, rc.left + n - 2, rc.bottom - 1 );

    SelectObject( hDC, hOldPen );

    /* 删除画Thumb笔 */
    DeleteThumbPen(penThumb);
}

void CCoolControlsManager::CCMTrackbar::DrawThumb( HDC hDC, const RECT& rc )
{
    DWORD dwStyle = GetWindowLong( m_hWnd, GWL_STYLE );

    /* BOTH */
    if ( dwStyle & TBS_BOTH )
    {
        DrawBothThumb(hDC, rc);
        return;
    }

    if ( dwStyle & TBS_VERT )
    {
        //  TBS_VERT & TBS_LEFT
        if ( dwStyle & TBS_LEFT )
        {
            DrawVLThumb(hDC, rc);
            return;
        }

        // TBS_VERT & TBS_RIGHT
        DrawVRThumb(hDC, rc);
        return;
    }
    
    //  TBS_HORZ & TBS_TOP
    if ( dwStyle & TBS_TOP )
    {
        DrawHTThumb(hDC, rc);
        return;
    }
    
    // TBS_HORZ & TBS_BOTTOM
    DrawHBThumb(hDC, rc);
    return;

}

// BUG!: Windows incorrectly calculates the channel rectangle for
// sliders with TBS_VERT style, so we have to calculate the rectangle
// in different manner. This bug appears on all Windows platforms!
void CCoolControlsManager::CCMTrackbar::CorrectVRect(DWORD dwStyle, RECT& VRect )
{
    RECT    rcThumb;
    int     iWidth  = 0;
    int     iHeight = 0;

    iWidth   = ( VRect.right  - VRect.left );
    iHeight  = ( VRect.bottom - VRect.top );
    VRect.top    = VRect.left;
    VRect.bottom = VRect.left + iWidth;

    SendMessage( m_hWnd, TBM_GETTHUMBRECT, 0, (LPARAM)&rcThumb );

    VRect.left = rcThumb.left + ( ( rcThumb.right - rcThumb.left ) / 2 + 1 ) - iHeight / 2;
    if ( dwStyle & TBS_LEFT )
        ;
    else if ( dwStyle & TBS_BOTH )
        VRect.left -= 1;
    else
        VRect.left -= 2;

    VRect.right = VRect.left + iHeight;

    return;
}

void CCoolControlsManager::CCMTrackbar::DrawControl( HDC hDC, const RECT& /*rect*/ )
{
    RECT    rcChannel;

    hDC = GetDC( m_hWnd );
    DWORD dwStyle = GetWindowLong( m_hWnd, GWL_STYLE );

    SendMessage( m_hWnd, TBM_GETCHANNELRECT, 0, (LPARAM)&rcChannel );

    // Correct RECT
    if ( dwStyle & TBS_VERT )
    {
        CorrectVRect(dwStyle, rcChannel);
    }

    // Draw the channel rect
    if ( m_nState & dsHoverMask )
        Draw3dBorder( hDC, rcChannel, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT,
                      COLOR_3DDKSHADOW, COLOR_3DLIGHT );
    else
        Draw3dBorder( hDC, rcChannel, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT,
                      COLOR_3DFACE, COLOR_3DFACE );

    // Draw the slider thumb
    if ( !( dwStyle & TBS_NOTHUMB ) )
    {
        SetRectEmpty( &rcChannel );
        SendMessage( m_hWnd, TBM_GETTHUMBRECT, 0, (LPARAM)&rcChannel );
        DrawThumb( hDC, rcChannel );
    }

    ReleaseDC( m_hWnd, hDC );
}

//////////////////////////////////////////////////////////////////////////////
// CCMToolbar class

void CCoolControlsManager::CCMToolbar::DrawButton( HDC hDC, const RECT& rc, int iState )
{
    if ( iState & bsChecked )
    {
        /* checked & Hover */
        if ( iState & bsHover )
        {
            Draw3dBorder( hDC, rc,
                          COLOR_3DDKSHADOW, COLOR_3DHIGHLIGHT,
                          COLOR_3DSHADOW, COLOR_3DSHADOW );
            return;
        }

        /* checked & Not Hover */
        Draw3dBorder( hDC, rc,
                      COLOR_3DSHADOW, COLOR_3DHIGHLIGHT,
                      COLOR_3DFACE, COLOR_3DFACE );
        return;
    }
    // Not checked & Hover
    if ( iState & bsHover )
    {
        Draw3dBorder( hDC, rc,
                      COLOR_3DHIGHLIGHT, COLOR_3DDKSHADOW,
                      COLOR_3DLIGHT, COLOR_3DSHADOW );
        return;
    }

    // Not checked & Not Hover
    Draw3dBorder( hDC, rc,
                  COLOR_3DHIGHLIGHT, COLOR_3DSHADOW,
                  COLOR_3DFACE, COLOR_3DFACE );
}

void CCoolControlsManager::CCMToolbar::DrawControl( HDC hDC, const RECT& /*rc*/ )
{
    int     i       = 0;
    int     iCount  = 0;
    int     iState  = 0;
    POINT   point;
    RECT    rc;
    TBBUTTON ti;
    
    // Skip flat toolbars
    COMP_BTRUE(GetWindowLong( m_hWnd, GWL_STYLE ) & TBSTYLE_FLAT);

    iCount = SendMessage( m_hWnd, TB_BUTTONCOUNT, 0, 0L );

    hDC = GetDC( m_hWnd );  // We will draw toolbar buttons on the client DC

    GetCursorPos( &point );
    ScreenToClient( m_hWnd, &point );

    m_iOldItem = -1;

    for ( i = 0; i < iCount; i++ )
    {
        SendMessage( m_hWnd, TB_GETBUTTON, i, (LPARAM)&ti );
        SendMessage( m_hWnd, TB_GETITEMRECT, i, (LPARAM)&rc );

        if ( !( ti.fsStyle & TBSTYLE_SEP ) )
        {
            iState = ( ti.fsState & TBSTATE_CHECKED ) ? bsChecked : bsNormal;
            if ( PtInRect( &rc, point ) == TRUE )
            {
                if ( ti.fsState & TBSTATE_ENABLED )
                    iState |= bsHover;
                m_iOldItem = i;
            }
            DrawButton( hDC, rc, iState );
        }
    }

    ReleaseDC( m_hWnd, hDC );
}

BOOL CCoolControlsManager::CCMToolbar::NeedRedraw( const POINT& point )
{
    int     i       = 0;
    int     iItem   = -1;
    int     iCount  = 0;
    POINT   pt;
    RECT    rc;
    TBBUTTON ti;

    iCount = SendMessage( m_hWnd, TB_BUTTONCOUNT, 0, 0L );

    pt = point;
    ScreenToClient( m_hWnd, &pt );
    for (i = 0; i < iCount; i++ )
    {
        SendMessage( m_hWnd, TB_GETBUTTON, i, (LPARAM)&ti );

        if ( !( ti.fsStyle & TBSTYLE_SEP ) )
        {
            SendMessage( m_hWnd, TB_GETITEMRECT, i, (LPARAM)&rc );
            if ( PtInRect( &rc, pt ) )
            {
                iItem = i;
                break;
            }
        }
    }

    COMP_BNE_R(m_hWnd, m_hWndOld, TRUE);

    // m_hWnd == m_hWndOld
    COMP_BNE_R(m_iOldItem, iItem, TRUE);

    return FALSE;
}

LRESULT CCoolControlsManager::CCMToolbar::WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( nMsg )
    {
    case WM_PAINT:
    case WM_NCPAINT:
        m_iOldItem = -1;   // Redraw the whole toolbar unconditionally

    default:
        return CCMControl::WindowProc( nMsg, wParam, lParam );
    }
}

//////////////////////////////////////////////////////////////////////////////
// CCMTabControl class

void CCoolControlsManager::CCMTabControl::DrawLeftTab( HDC hDC, const RECT& rect,
                                                       int iItem, int iState, int iCurSel )
{
    RECT rc = rect;

    if ( iState & bsChecked )
    {
        rc.top      -= 2;
        rc.bottom   += 2;
        rc.left     -= 2;
        rc.right    += 1;
    }

    if ( iState & bsHover )
    {
        if ( iCurSel != iItem - 1 )
        {
            FillSolidRect( hDC, rc.left+2, rc.top, rc.right-rc.left-2, 1, GetSysColor( COLOR_3DHIGHLIGHT ) );
            FillSolidRect( hDC, rc.left+2, rc.top+1, rc.right-rc.left-2, 1, GetSysColor( COLOR_3DLIGHT ) );
            SetPixel( hDC, rc.left+1, rc.top+1, GetSysColor( COLOR_3DHIGHLIGHT ) );
        }

        FillSolidRect( hDC, rc.left, rc.top+2, 1, rc.bottom-rc.top-4, GetSysColor( COLOR_3DHIGHLIGHT ) );
        FillSolidRect( hDC, rc.left+1, rc.top+2, 1, rc.bottom-rc.top-4, GetSysColor( COLOR_3DLIGHT ) );

        if ( iCurSel != iItem + 1 )
        {
            FillSolidRect( hDC, rc.left+2, rc.bottom-1, rc.right-rc.left-2, 1, GetSysColor( COLOR_3DDKSHADOW ) );
            FillSolidRect( hDC, rc.left+2, rc.bottom-2, rc.right-rc.left-2, 1, GetSysColor( COLOR_3DSHADOW ) );
            SetPixel( hDC, rc.left+1, rc.bottom-2, GetSysColor( COLOR_3DDKSHADOW ) );
        }
        return;
    }

    // Not Hover
    if ( iCurSel != iItem - 1 )
    {
        FillSolidRect( hDC, rc.left+2, rc.top, rc.right-rc.left-2, 1, GetSysColor( COLOR_3DHIGHLIGHT ) );
        FillSolidRect( hDC, rc.left+2, rc.top+1, rc.right-rc.left-2, 1, GetSysColor( COLOR_3DFACE ) );
        SetPixel( hDC, rc.left+1, rc.top+1, GetSysColor( COLOR_3DHIGHLIGHT ) );
    }

    FillSolidRect( hDC, rc.left, rc.top+2, 1, rc.bottom-rc.top-4, GetSysColor( COLOR_3DHIGHLIGHT ) );
    FillSolidRect( hDC, rc.left+1, rc.top+2, 1, rc.bottom-rc.top-4, GetSysColor( COLOR_3DFACE ) );

    if ( iCurSel != iItem + 1 )
    {
        FillSolidRect( hDC, rc.left+2, rc.bottom-1, rc.right-rc.left-2, 1, GetSysColor( COLOR_3DSHADOW ) );
        FillSolidRect( hDC, rc.left+2, rc.bottom-2, rc.right-rc.left-2, 1, GetSysColor( COLOR_3DFACE ) );
        SetPixel( hDC, rc.left+1, rc.bottom-2, GetSysColor( COLOR_3DSHADOW ) );
    }
}

void CCoolControlsManager::CCMTabControl::DrawTopTab( HDC hDC, const RECT& rect,
                                                      int iItem, int iState, int iCurSel )
{
    RECT rc = rect;

    if ( iState & bsChecked )
    {
        rc.top      -= 2;
        rc.bottom   += 1;
        rc.left     -= 2;
        rc.right    += 2;
    }

    if ( iState & bsHover )
    {
        if ( iCurSel != iItem - 1 )
        {
            FillSolidRect( hDC, rc.left, rc.top+2, 1, rc.bottom-rc.top-2, GetSysColor( COLOR_3DHIGHLIGHT ) );
            FillSolidRect( hDC, rc.left+1, rc.top+2, 1, rc.bottom-rc.top-2, GetSysColor( COLOR_3DLIGHT ) );
            SetPixel( hDC, rc.left+1, rc.top+1, GetSysColor( COLOR_3DHIGHLIGHT ) );
        }

        FillSolidRect( hDC, rc.left+2, rc.top, rc.right-rc.left-4, 1, GetSysColor( COLOR_3DHIGHLIGHT ) );
        FillSolidRect( hDC, rc.left+2, rc.top+1, rc.right-rc.left-4, 1, GetSysColor( COLOR_3DLIGHT ) );

        if ( iCurSel != iItem + 1 )
        {
            FillSolidRect( hDC, rc.right-1, rc.top+2, 1, rc.bottom-rc.top-2, GetSysColor( COLOR_3DDKSHADOW ) );
            FillSolidRect( hDC, rc.right-2, rc.top+2, 1, rc.bottom-rc.top-2, GetSysColor( COLOR_3DSHADOW ) );
            SetPixel( hDC, rc.right-2, rc.top+1, GetSysColor( COLOR_3DDKSHADOW ) );
        }
        return;
    }

    // Not Hover
    if ( iCurSel != iItem - 1 )
    {
        FillSolidRect( hDC, rc.left, rc.top+2, 1, rc.bottom-rc.top-2, GetSysColor( COLOR_3DHIGHLIGHT ) );
        FillSolidRect( hDC, rc.left+1, rc.top+2, 1, rc.bottom-rc.top-2, GetSysColor( COLOR_3DFACE ) );
        SetPixel( hDC, rc.left+1, rc.top+1, GetSysColor( COLOR_3DHIGHLIGHT ) );
    }

    FillSolidRect( hDC, rc.left+2, rc.top, rc.right-rc.left-4, 1, GetSysColor( COLOR_3DHIGHLIGHT ) );
    FillSolidRect( hDC, rc.left+2, rc.top+1, rc.right-rc.left-4, 1, GetSysColor( COLOR_3DFACE ) );

    if ( iCurSel != iItem + 1 )
    {
        FillSolidRect( hDC, rc.right-1, rc.top+2, 1, rc.bottom-rc.top-2, GetSysColor( COLOR_3DSHADOW ) );
        FillSolidRect( hDC, rc.right-2, rc.top+2, 1, rc.bottom-rc.top-2, GetSysColor( COLOR_3DFACE ) );
        SetPixel( hDC, rc.right-2, rc.top+1, GetSysColor( COLOR_3DSHADOW ) );
    }
}

void CCoolControlsManager::CCMTabControl::DrawRightTab( HDC hDC, const RECT& rect,
                                                        int iItem, int iState, int iCurSel )
{
    RECT rc = rect;

    if ( iState & bsChecked )
    {
        rc.top      -= 2;
        rc.bottom   += 2;
        rc.right    += 2;
        rc.left     -= 1;
    }

    if ( iState & bsHover )
    {
        if ( iCurSel != iItem - 1 )
        {
            FillSolidRect( hDC, rc.left, rc.top, rc.right-rc.left-2, 1, GetSysColor( COLOR_3DHIGHLIGHT ) );
            FillSolidRect( hDC, rc.left, rc.top+1, rc.right-rc.left-2, 1, GetSysColor( COLOR_3DLIGHT ) );
            SetPixel( hDC, rc.right-2, rc.top+1, GetSysColor( COLOR_3DHIGHLIGHT ) );
        }

        FillSolidRect( hDC, rc.right-1, rc.top+2, 1, rc.bottom-rc.top-4, GetSysColor( COLOR_3DDKSHADOW ) );
        FillSolidRect( hDC, rc.right-2, rc.top+2, 1, rc.bottom-rc.top-4, GetSysColor( COLOR_3DSHADOW ) );

        if ( iCurSel != iItem + 1 )
        {
            FillSolidRect( hDC, rc.left, rc.bottom-2, rc.right-rc.left-2, 1, GetSysColor( COLOR_3DSHADOW ) );
            FillSolidRect( hDC, rc.left, rc.bottom-1, rc.right-rc.left-2, 1, GetSysColor( COLOR_3DDKSHADOW ) );
            SetPixel( hDC, rc.right-2, rc.bottom-2, GetSysColor( COLOR_3DDKSHADOW ) );
        }
        return;
    }

    // Not Hover
    if ( iCurSel != iItem - 1 )
    {
        FillSolidRect( hDC, rc.left, rc.top, rc.right-rc.left-2, 1, GetSysColor( COLOR_3DHIGHLIGHT ) );
        FillSolidRect( hDC, rc.left, rc.top+1, rc.right-rc.left-2, 1, GetSysColor( COLOR_3DFACE ) );
        SetPixel( hDC, rc.right-2, rc.top+1, GetSysColor( COLOR_3DHIGHLIGHT ) );
    }

    FillSolidRect( hDC, rc.right-1, rc.top+2, 1, rc.bottom-rc.top-4, GetSysColor( COLOR_3DSHADOW ) );
    FillSolidRect( hDC, rc.right-2, rc.top+2, 1, rc.bottom-rc.top-4, GetSysColor( COLOR_3DFACE ) );

    if ( iCurSel != iItem + 1 )
    {
        FillSolidRect( hDC, rc.left, rc.bottom-1, rc.right-rc.left-2, 1, GetSysColor( COLOR_3DSHADOW ) );
        FillSolidRect( hDC, rc.left, rc.bottom-2, rc.right-rc.left-2, 1, GetSysColor( COLOR_3DFACE ) );
        SetPixel( hDC, rc.right-2, rc.bottom-2, GetSysColor( COLOR_3DSHADOW ) );
    }
}

void CCoolControlsManager::CCMTabControl::DrawBottomTab( HDC hDC, const RECT& rect,
                                                         int iItem, int iState, int iCurSel )
{
    RECT rc = rect;

    if ( iState & bsChecked )
    {
        rc.bottom   += 2;
        rc.left     -= 2;
        rc.right    += 2;
        rc.top      -=1;
    }

    if ( iState & bsHover )
    {
        if ( iCurSel != iItem - 1 )
        {
            FillSolidRect( hDC, rc.left, rc.top, 1, rc.bottom-rc.top-2, GetSysColor( COLOR_3DHIGHLIGHT ) );
            FillSolidRect( hDC, rc.left+1, rc.top, 1, rc.bottom-rc.top-2, GetSysColor( COLOR_3DLIGHT ) );
            SetPixel( hDC, rc.left+1, rc.bottom-2, GetSysColor( COLOR_3DHIGHLIGHT ) );
        }

        FillSolidRect( hDC, rc.left+2, rc.bottom-1, rc.right-rc.left-4, 1, GetSysColor( COLOR_3DDKSHADOW ) );
        FillSolidRect( hDC, rc.left+2, rc.bottom-2, rc.right-rc.left-4, 1, GetSysColor( COLOR_3DSHADOW ) );

        if ( iCurSel != iItem + 1 )
        {
            FillSolidRect( hDC, rc.right-1, rc.top, 1, rc.bottom-rc.top-2, GetSysColor( COLOR_3DDKSHADOW ) );
            FillSolidRect( hDC, rc.right-2, rc.top, 1, rc.bottom-rc.top-2, GetSysColor( COLOR_3DSHADOW ) );
            SetPixel( hDC, rc.right-2, rc.bottom-2, GetSysColor( COLOR_3DDKSHADOW ) );
        }
        return;
    }

    // Not Hover
    if ( iCurSel != iItem - 1 )
    {
        FillSolidRect( hDC, rc.left, rc.top, 1, rc.bottom-rc.top-2, GetSysColor( COLOR_3DHIGHLIGHT ) );
        FillSolidRect( hDC, rc.left+1, rc.top, 1, rc.bottom-rc.top-2, GetSysColor( COLOR_3DFACE ) );
        SetPixel( hDC, rc.left+1, rc.bottom-2, GetSysColor( COLOR_3DHIGHLIGHT ) );
    }

    FillSolidRect( hDC, rc.left+2, rc.bottom-1, rc.right-rc.left-4, 1, GetSysColor( COLOR_3DSHADOW ) );
    FillSolidRect( hDC, rc.left+2, rc.bottom-2, rc.right-rc.left-4, 1, GetSysColor( COLOR_3DFACE ) );

    if ( iCurSel != iItem + 1 )
    {
        FillSolidRect( hDC, rc.right-1, rc.top, 1, rc.bottom-rc.top-2, GetSysColor( COLOR_3DSHADOW ) );
        FillSolidRect( hDC, rc.right-2, rc.top, 1, rc.bottom-rc.top-2, GetSysColor( COLOR_3DFACE ) );
        SetPixel( hDC, rc.right-2, rc.bottom-2, GetSysColor( COLOR_3DSHADOW ) );
    }
}

void CCoolControlsManager::CCMTabControl::DrawTab( HDC hDC, const RECT& rect,
                                                   int iItem, int iState )
{
    int iCurSel = 0;

    iCurSel = SendMessage( m_hWnd, TCM_GETCURSEL, 0, 0L );
    if ( iCurSel == -1 )
         iCurSel =  -2;

    switch ( GetOrientation() )
    {
    case tabLeft:
        DrawLeftTab(hDC, rect, iItem, iState, iCurSel);
        break;

    case tabTop:
        DrawTopTab(hDC, rect, iItem, iState, iCurSel);
        break;

    case tabRight:
        DrawRightTab(hDC, rect, iItem, iState, iCurSel);
        break;

    case tabBottom:
        DrawBottomTab(hDC, rect, iItem, iState, iCurSel);
        break;
    }
}

CCoolControlsManager::CCMTabControl::OrientationsEnum CCoolControlsManager::CCMTabControl::GetOrientation() const
{
    DWORD dwStyle = GetWindowLong( m_hWnd, GWL_STYLE );

    if ( dwStyle & TCS_BOTTOM )
    {
        /*TCS_BOTTOM & TCS_VERTICAL*/
        COMP_BTRUE_R(( dwStyle & TCS_VERTICAL ), tabRight);

        /*TCS_BOTTOM & TCS_HOTTRACK*/
        return tabBottom;
    }

    /*TCS_TOP & TCS_VERTICAL*/
    COMP_BTRUE_R(( dwStyle & TCS_VERTICAL ), tabLeft);

    /*TCS_TOP & TCS_HOTTRACK*/
    return tabTop;
}

void CCoolControlsManager::CCMTabControl::FillDrawTab(HDC hDC, RECT rc, RECT rcSel, int iCurSel)
{
    int     i       = 0;
    int     iCount  = 0;
    int     iState  = 0;
    POINT   point;
    
    switch ( GetOrientation() )
    {
    case tabLeft:
        rc.left += 2;
        FillSolidRect( hDC, rc.left, rc.bottom, rc.right-rc.left, -1, GetSysColor( COLOR_3DSHADOW ) );
        FillSolidRect( hDC, rc.right, rc.top, -1, rc.bottom-rc.top, GetSysColor( COLOR_3DSHADOW ) );
        break;

    case tabTop:
        rc.top += 2;
        FillSolidRect( hDC, rc.left, rc.bottom, rc.right-rc.left, -1, GetSysColor( COLOR_3DSHADOW ) );
        FillSolidRect( hDC, rc.right, rc.top, -1, rc.bottom-rc.top, GetSysColor( COLOR_3DSHADOW ) );
        break;

    case tabRight:
        rc.right -= 2;
        FillSolidRect( hDC, rc.left, rc.bottom, rc.right-rc.left, -1, GetSysColor( COLOR_3DSHADOW ) );
        FillSolidRect( hDC, rc.right, rc.top, -1, rcSel.top-rc.top, GetSysColor( COLOR_3DSHADOW ) );
        FillSolidRect( hDC, rc.right, rcSel.bottom, -1, rc.bottom-rcSel.bottom, GetSysColor( COLOR_3DSHADOW ) );
        break;

    case tabBottom:
        rc.bottom -= 2;
        FillSolidRect( hDC, rc.left, rc.bottom, rcSel.left-rc.left, -1, GetSysColor( COLOR_3DSHADOW ) );
        FillSolidRect( hDC, rcSel.right, rc.bottom, rc.right-rcSel.right, -1, GetSysColor( COLOR_3DSHADOW ) );
        FillSolidRect( hDC, rc.right, rc.top, -1, rc.bottom-rc.top, GetSysColor( COLOR_3DSHADOW ) );
        break;
    }

    Draw3dRect( hDC, rc.left+1, rc.top+1, rc.right-rc.left-2, rc.bottom-rc.top-2,
                GetSysColor( COLOR_3DFACE ), GetSysColor( COLOR_3DFACE ) );

    m_iOldItem = -1;
    GetCursorPos( &point );
    ScreenToClient( m_hWnd, &point );
    iCount = SendMessage( m_hWnd, TCM_GETITEMCOUNT, 0, 0L );

    for ( i = 0; i < iCount; i++ )
    {
        SendMessage( m_hWnd, TCM_GETITEMRECT, i, (LPARAM)&rc );

        /* 设置Tab状态 */
        iState = bsNormal;
        if ( iCurSel == i )
            iState = bsChecked;
        if ( PtInRect( &rc, point ) == TRUE )
        {
            iState |= bsHover;
            m_iOldItem = i;
        }

        DrawTab( hDC, rc, i, iState );
    }

    return;
}

void CCoolControlsManager::CCMTabControl::DrawControl( HDC hDC, const RECT& rect )
{
    int     iCurSel = 0;
    RECT    rc;
    RECT    rcSel;
    DWORD   dwStyle = GetWindowLong( m_hWnd, GWL_STYLE );

    // Skip button-like tab controls
    COMP_BTRUE( dwStyle & TCS_BUTTONS );

    hDC = GetDC( m_hWnd );  // We will draw on the client DC

    rc = rect;
    SendMessage( m_hWnd, TCM_ADJUSTRECT, FALSE, (LPARAM)&rc );
    InflateRect( &rc, 4, 4 );

    
    iCurSel = SendMessage( m_hWnd, TCM_GETCURSEL, 0, 0L );
    SendMessage( m_hWnd, TCM_GETITEMRECT, iCurSel, (LPARAM)&rcSel );

    FillDrawTab(hDC, rc, rcSel, iCurSel);

    /* 非第一个选中，特殊处理 */
    if ( iCurSel != 0 )
    {
        switch ( GetOrientation() )
        {
        case tabTop:
            FillSolidRect( hDC, rect.left, rect.top, 2, rcSel.bottom-rcSel.top+2, GetSysColor( COLOR_3DFACE ) );
            break;
        case tabBottom:
            FillSolidRect( hDC, rect.left, rect.bottom, 2, -rcSel.bottom+rcSel.top-2, GetSysColor( COLOR_3DFACE ) );
            break;
        }
    }
    ReleaseDC( m_hWnd, hDC );
}

BOOL CCoolControlsManager::CCMTabControl::NeedRedraw( const POINT& point )
{
    int           iItem = 0;
    TCHITTESTINFO thti;

    SendMessage( m_hWnd, TCM_GETITEMCOUNT, 0, 0L );

    thti.pt = point;
    ScreenToClient( m_hWnd, &thti.pt );
    iItem = SendMessage( m_hWnd, TCM_HITTEST, 0, (LPARAM)&thti );

    COMP_BNE_R(m_hWnd, m_hWndOld, TRUE);

    /* m_hWnd == m_hWndOld */
    COMP_BNE_R(m_iOldItem, iItem, TRUE);

    return FALSE;
}

LRESULT CCoolControlsManager::CCMTabControl::WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( nMsg )
    {
    case WM_PAINT:
    case WM_NCPAINT:
        m_iOldItem = -1;   // Redraw the whole control unconditionally
        break;
    default:
        break;
    }
    return CCMControl::WindowProc( nMsg, wParam, lParam );
}

//////////////////////////////////////////////////////////////////////////////
// CCMIPAddress class

BOOL CCoolControlsManager::CCMIPAddress::IsFocused()
{
    HWND hWnd = GetTopWindow( m_hWnd );

    while ( hWnd )
    {
        COMP_BE_R(hWnd, GetFocus(), TRUE);

        hWnd = GetNextWindow( hWnd, GW_HWNDNEXT );
    }

    return FALSE;
}

LRESULT CCoolControlsManager::CCMIPAddress::WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( nMsg )
    {
    case WM_COMMAND:
        if ( HIWORD( wParam ) == EN_SETFOCUS || HIWORD( wParam ) == EN_KILLFOCUS )
            DrawBorder();
        break;

    default:
        return CCMControl::WindowProc( nMsg, wParam, lParam );
    }

    return CallWindowProc( m_oldWndProc, m_hWnd, nMsg, wParam, lParam );
}

//////////////////////////////////////////////////////////////////////////////
// CCMDialog class

LRESULT CCoolControlsManager::CCMDialog::WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( nMsg )
    {
    case WM_PARENTNOTIFY:
        if ( LOWORD( wParam ) == WM_CREATE )   // Add dynamically created controls to the map
            g_ctrlManager.AddControl( (HWND)lParam );
        break;

    case WM_NCDESTROY:
        // Unsubclass window and remove it from the map
        CallWindowProc( m_oldWndProc, m_hWnd, nMsg, wParam, lParam );
        g_ctrlManager.RemoveDialog( m_hWnd );
        return 0;
    default:
        break;
    }

    return CallWindowProc( m_oldWndProc, m_hWnd, nMsg, wParam, lParam );
}

///////////////////////////////////////////////////////////////////////////////////
// Helper functions for drawing 3D frames (borrowed from CDC class)

void CCoolControlsManager::CCMControl::FillSolidRect( HDC hDC, int x, int y, int cx, int cy, COLORREF clr )
{
    RECT rect;

    SetBkColor( hDC, clr );
    SetRect( &rect, x, y, x + cx, y + cy );
    ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
}

void CCoolControlsManager::CCMControl::FillSolidRect( HDC hDC, const RECT& rect, COLORREF clr )
{
    SetBkColor( hDC, clr );
    ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
}

void CCoolControlsManager::CCMControl::Draw3dRect( HDC hDC, int x, int y, int cx, int cy,
                                                  COLORREF clrTopLeft, COLORREF clrBottomRight )
{
    FillSolidRect( hDC, x, y, cx - 1, 1, clrTopLeft );
    FillSolidRect( hDC, x, y, 1, cy - 1, clrTopLeft );
    FillSolidRect( hDC, x + cx, y, -1, cy, clrBottomRight );
    FillSolidRect( hDC, x, y + cy, cx, -1, clrBottomRight );
}

void CCoolControlsManager::CCMControl::Draw3dRect( HDC hDC, const RECT& rect,
                                                  COLORREF clrTopLeft, COLORREF clrBottomRight )
{
    Draw3dRect( hDC, rect.left, rect.top, rect.right - rect.left,
                rect.bottom - rect.top, clrTopLeft, clrBottomRight );
}

void CCoolControlsManager::CCMControl::Draw3dBorder( HDC hDC, const RECT& rc,
                                                     int iColor1, int iColor2,
                                                     int iColor3, int iColor4 )
{
    Draw3dRect( hDC, rc, GetSysColor( iColor1 ), GetSysColor( iColor2 ) );

    Draw3dRect( hDC, rc.left + 1, rc.top + 1, rc.right - rc.left - 2, rc.bottom - rc.top - 2,
                GetSysColor( iColor3 ), GetSysColor( iColor4 ) );
}

void CCoolControlsManager::CCMControl::Draw3dBorder( HDC hDC, const RECT& rc,
                                                    int iColor1, int iColor2,
                                                    int iColor3, int iColor4,
                                                    int iColor5, int iColor6 )
{
    Draw3dRect( hDC, rc, GetSysColor( iColor1 ), GetSysColor( iColor2 ) );

    Draw3dRect( hDC, rc.left + 1, rc.top + 1, rc.right - rc.left - 2, rc.bottom - rc.top - 2,
                GetSysColor( iColor3 ), GetSysColor( iColor4 ) );
    Draw3dRect( hDC, rc.left + 2, rc.top + 2, rc.right - rc.left - 4, rc.bottom - rc.top - 4,
                GetSysColor( iColor5 ), GetSysColor( iColor6 ) );
}

