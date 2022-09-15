/******************************************************************

$Archive: /MfcExt/Source/CoolControlsManager.h $
$Workfile: CoolControlsManager.h $
$Author: Bogdan Ledwig $
$Date: 99-04-26 22:11 $
$Revision: 13 $

*******************************************************************/

/*
Written by Bogdan Ledwig
e-mail:   bogdanle@opole.mtl.pl
web site: http://bogdanle.cjb.net/
Copyright ?1999 Bogdan Ledwig. All Rights Reserved.

This code may be used in compiled form in any way you wish. This
file may be redistributed unmodified by any means PROVIDING it is
not sold for profit without the authors written consent, and
providing that this notice and the authors name is included. If the
source code is used in any commercial product then a statement
along the lines of "Portions Copyright (C) 1999 Bogdan Ledwig"
must be included in the startup banner, "About" box or printed
documentation. An email letting me know that you are using it would be
nice as well.

This software is provided "as is" without express or implied warranty.
Use it at you own risk! The author accepts no liability for any damages
to your computer or data these products may cause.

History:
1.0     1999-02-24   First public release
1.1     1999-05-16   LVS_EX_HEADERDRAGDROP list control bug fixed (thanks to Vlad Bychkoff for pointing this out)
UNICODE support added
WH_CALLWNDPROCRET is no longer supported due to some weird problems with that
Added support for multiple UI threads - (thanks for Mike Walter for the code)
Class name has been changed to CCoolControlsManager (my own idea)
Added support for SysTabControl32
*/

#if !defined (__CoolControlsManager_h)
#define __CoolControlsManager_h

#include <afxtempl.h>   // For CMapPtrToPtr

//////////////////////////////////////////////////////////////////////
// CCoolControlsManager class

/* Cool Control type reDefine */
#define CC_BUTTON           0       /* Button */
#define CC_COMBOBOX         1       /* ComboBox */
#define CC_EDIT             2       /* Edit */
#define CC_LISTBOX          3       /* ListBox */
#define CC_SYSLISTVIEW      4       /* SysListView32 */
#define CC_SHELLDLLVIEW     5       /* SHELLDLL_DefView */
#define CC_SYSTREEVIEW      6       /* SysTreeView32 */
#define CC_SYSDTPICK        7       /* SysDateTimePick32 */
#define CC_SYSMONTHCAL      8       /* SysMonthCal32 */
#define CC_MSUPDOWN         9       /* msctls_updown32 */
#define CC_COMBOLBOX        10      /* ComboLBox */
#define CC_SCROLLBAR        11      /* ScrollBar */
#define CC_COMBOBOXEX       12      /* ComboBoxEx32 */
#define CC_MSHOTKEY         13      /* msctls_hotkey32 */
#define CC_SYSIPADDR        14      /* SysIPAddress32 */
#define CC_MSTRACKBAR       15      /* msctls_trackbar32 */
#define CC_RICHEDIT         16      /* RichEdit */
#define CC_RICHEDITW        17      /* RichEdit20W */
#define CC_SYSHEADER        18      /* SysHeader32 */
#define CC_TOOLBARWIN       19      /* ToolbarWindow32 */
#define CC_SYSTABCTRL       20      /* SysTabControl32 */
#define CC_MAXNUM           (CC_SYSTABCTRL+1)

class CCoolControlsManager 
{
public:

    enum ButtonStatesEnum 
    {
        bsNormal,
        bsHover,
        bsChecked,
    };

    enum DrawStatesEnum 
    {
        dsNormal,
        dsHover,
        dsAlternate,
        dsFocus = 0x04,
        dsHoverMask = 0x05,
        dsDisabled = 0x08,
    };

    // Base class for all controls and dialogs
    class CCMCore 
    {
    public:
        void Subclass( HWND hWnd, WNDPROC wndNewProc );
        void Unsubclass();

        // New window procedure (must be implemented by derived classes)
        virtual LRESULT WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam ) = 0;

    protected:
        HWND    m_hWnd;                       // Handle of the window
        WNDPROC m_oldWndProc;                 // Address of original window procedure
    };

    // Class that holds important control informations and is
    // responsible for drawing the control
    class CCMControl : public CCMCore 
    {
        // Construction/destruction
    public:
        CCMControl();

        // Operations
    public:
        // Main drawing routine
        void DrawBorder();
        // Returns TRUE if control needs repainting
        virtual BOOL NeedRedraw( const POINT& point );

        // Give acces to the control state
        void SetState( int iFlagRemove, int iFlagAdd, BOOL bRedraw = TRUE );
        int  GetState() const;

        // New window procedure
        virtual LRESULT WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam );

        static HWND m_hWndOld;           // Previously 'hovered' or 'focused' window

        // Implementation
    protected:
        // Paint the scrollbars if a window contains at least one
        void DrawScrollBars( HDC hDC, const RECT& rc );
        // Paint the scrollbar control or an embedded window scrollbar (horiz or vert)
        void DrawScrollBar( HDC hDC, const RECT& rc,
                            int iType, BOOL bScrollbarCtrl = FALSE );
        // Draw the scrollbar thumb
        void DrawScrollbarThumb( HDC hDC, const RECT& rc );

        // There are helper functions for drawing 3D frames
        static void Draw3dBorder(   HDC hDC, const RECT& rc,
                                    int iColor1, int iColor2,
                                    int iColor3, int iColor4 );
        static void Draw3dBorder(   HDC hDC, const RECT& rc,
                                    int iColor1, int iColor2,
                                    int iColor3, int iColor4,
                                    int iColor5, int iColor6 );
        static void FillSolidRect( HDC hDC, const RECT& rect, COLORREF clr );
        static void FillSolidRect( HDC hDC, int x, int y, int cx, int cy, COLORREF clr );
        static void Draw3dRect( HDC hDC, int x, int y, int cx, int cy,
                                COLORREF clrTopLeft, COLORREF clrBottomRight );
        static void Draw3dRect( HDC hDC, const RECT& rect,
                                COLORREF clrTopLeft, COLORREF clrBottomRight );

        // Overrides
    protected:
        // Prepares DC and RECT for further drawings
        virtual void PrepareDraw( HDC& hDC, RECT& rc );

        // Drawing code which is specific to the control
        virtual void DrawControl( HDC hDC, const RECT& rc );

        // Returns TRUE if control (or one of its children) has a focus
        virtual BOOL IsFocused();

        short m_nState;                       // Current control state
        short m_nOldState;                    // Previous control state

    private:
        void DrawScrollArrow( HDC hDC, const RECT& rect, int iType, RECT& rcArrow1);
        void DrawScrollVThumb(HDC hDC, const RECT& rect, RECT rcArrow1, BOOL bScrollbarCtrl);
        void DrawScrollHThumb(HDC hDC, const RECT& rect, RECT rcArrow1, BOOL bScrollbarCtrl);
    };

    // Edit windows
    class CCMEdit : public CCMControl 
    {
    public:
        virtual LRESULT WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam );
    protected:
        virtual void DrawControl( HDC hDC, const RECT& rc );
    private:
        void DrawMSUpDownCtrl(HWND hWnd, RECT rect);
    };

    // ComboBoxes (all styles are suported)
    class CCMComboBox : public CCMControl 
    {
    public:
        virtual LRESULT WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam );
    protected:
        virtual void DrawControl( HDC hDC, const RECT& rc );
        virtual BOOL IsFocused();
    };

    // Date/Time pickers
    class CCMDateTime : public CCMComboBox 
    {
    protected:
        virtual void DrawControl( HDC hDC, const RECT& rc );
    };

    // Pushbuttons
    class CCMPushButton : public CCMControl 
    {
    public:
        virtual LRESULT WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam );
    protected:
        virtual void DrawControl( HDC hDC, const RECT& rc );
    };

    // Checkboxes
    class CCMCheckBox : public CCMPushButton 
    {
    protected:
        virtual void DrawControl( HDC hDC, const RECT& rc );

        // Check boxes and radio buttons are always drawn
        // in the same way regardless of focus
        virtual BOOL IsFocused() { return FALSE; }
    private:
        RECT         ReCalcRect(RECT rectSrc, DWORD   dwStyle);
    };

    // Radiobuttons
    class CCMRadioButton : public CCMPushButton 
    {
    protected:
        virtual void DrawControl( HDC hDC, const RECT& rc );
        void DrawFrame( POINT* ptArr, int iColor,
                        HDC hDC, int xOff, int yOff );
        virtual BOOL IsFocused() { return FALSE; }
    };

    // Spin Buttons
    class CCMUpDown : public CCMControl 
    {
    protected:
        virtual void DrawControl( HDC hDC, const RECT& rc );
        void DrawButton( HDC hDC, const RECT& rc );
    private:
        void DrawAlign(HDC hDC, RECT& rect, HWND hWnd, DWORD dwStyle);
        void DrawAlignLeft (HDC hDC, RECT& rect, CCMControl *pCtl, BOOL bEnable);
        void DrawAlignRight(HDC hDC, RECT& rect, CCMControl *pCtl, BOOL bEnable);
    };

    // Edit control in simple combobox
    class CCMEditCombo : public CCMEdit 
    {
    protected:
        virtual void PrepareDraw( HDC& hDC, RECT& rc );
    };

    // Stand-alone scrollbar controls
    class CCMScrollBar : public CCMControl 
    {
    public:
        virtual LRESULT WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam );
    protected:
        virtual void DrawControl( HDC hDC, const RECT& rc );
    };

    // Header control
    class CCMHeaderCtrl : public CCMControl 
    {
    public:
        virtual BOOL NeedRedraw( const POINT& point );
    protected:
        virtual void DrawControl( HDC hDC, const RECT& rc );
        void    DrawButton( HDC hDC, const RECT& rc, int iState );
    private:
        int     m_iOldItem;        // Recently selected item
    };

    // Slider control
    class CCMTrackbar : public CCMControl 
    {
    protected:
        virtual void DrawControl( HDC hDC, const RECT& rc );
        void DrawThumb( HDC hDC, const RECT& rc );
    private:
        void CreateThumbPen( HPEN *pPen );
        void DeleteThumbPen( HPEN *pPen );
        void DrawBothThumb( HDC hDC, const RECT& rc );
        void DrawVLThumb( HDC hDC, const RECT& rc );
        void DrawVRThumb( HDC hDC, const RECT& rc );
        void DrawHTThumb( HDC hDC, const RECT& rc );
        void DrawHBThumb( HDC hDC, const RECT& rc );
        void CorrectVRect(DWORD dwStyle, RECT& VRect );
    };

    // Toolbars
    class CCMToolbar : public CCMControl 
    {
    public:
        virtual BOOL NeedRedraw( const POINT& point );
        virtual LRESULT WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam );
    protected:
        virtual void DrawControl( HDC hDC, const RECT& rc );
        void    DrawButton( HDC hDC, const RECT& rc, int iState );
    private:
        int     m_iOldItem;        // Recently selected item
    };

    // IP Address control
    class CCMIPAddress : public CCMControl 
    {
    public:
        virtual LRESULT WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam );
    protected:
        virtual BOOL IsFocused();
    };

    // Tab control
    class CCMTabControl : public CCMControl 
    {
    public:
        enum OrientationsEnum 
        {
            tabTop,
            tabLeft,
            tabRight,
            tabBottom,
        };

        CCMTabControl()
        {
            m_iOldItem = -1;
        }
        virtual LRESULT WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam );
        virtual BOOL NeedRedraw( const POINT& point );
    protected:
        virtual void DrawControl( HDC hDC, const RECT& rc );
        void DrawTab( HDC hDC, const RECT& rc, int iItem, int iState );
        virtual BOOL IsFocused() { return FALSE; }
        OrientationsEnum GetOrientation() const;
    private:
        int     m_iOldItem;         // Recently selected item
        void    DrawTopTab   (HDC hDC, const RECT& rect, int iItem, int iState, int iCurSel);
        void    DrawLeftTab  (HDC hDC, const RECT& rect, int iItem, int iState, int iCurSel);
        void    DrawRightTab (HDC hDC, const RECT& rect, int iItem, int iState, int iCurSel);
        void    DrawBottomTab(HDC hDC, const RECT& rect, int iItem, int iState, int iCurSel);
        void    FillDrawTab  (HDC hDC, RECT rc, RECT rcSel, int iCurSel);
    };

    // Dialog (parent of controls)
    class CCMDialog : public CCMCore 
    {
    public:
        virtual LRESULT WindowProc( UINT nMsg, WPARAM wParam, LPARAM lParam );
    };

public:
    CCoolControlsManager();
    virtual ~CCoolControlsManager();

    void Install( HWND hWnd );            // Installs control manager for given window only
    void Uninstall( HWND hWnd );          // Removes all window controls from the map

    // Installs WH_CALLWNDPROC hook, which automatically
    // handles all dialogs in the givent thread. If dwThreadID = -1, then
    // current thread is used
    void InstallHook( DWORD dwThreadID = -1, BOOL bDialogOnly = TRUE );

    // Installs WH_CALLWNDPROC hook for
    // all dialogs in system (valid only for a DLL)
    void InstallGlobalHook( HINSTANCE hInstance, BOOL bDialogOnly = TRUE );

    // Uninstals hook
    void UninstallHook( DWORD dwThreadID = -1 );

    /* for AddControl function add Control */
    BOOL AddButtonCtrl      (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddComboBoxCtrl    (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddEditCtrl        (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddListBoxCtrl     (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddSysListViewCtrl (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddShellDllViewCtrl(DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddSysTreeViewCtrl (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddSysDateTimeCtrl (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddSysMonthCalCtrl (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddMSUpDownCtrl    (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddComboLBoxCtrl   (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddScrollBarCtrl   (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddComboBoxExCtrl  (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddMSHotKeyCtrl    (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddSysIPAddrCtrl   (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddMSTrackBarCtrl  (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddRichEditCtrl    (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddRichEditWCtrl   (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddSysHeaderCtrl   (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddToolbarWinCtrl  (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddSysTabCtrl      (DWORD dwStyle, DWORD dwExStyle, CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    BOOL AddCtrl2Map(CCMControl *pCtl, HWND hWnd, TCHAR *szType);
    // Adds single control to the map
    BOOL AddControl( HWND hWnd, LPCTSTR lpszClass = NULL  );
    // Removes single control from the map
    BOOL RemoveControl( HWND hWnd );

    void AddDialog( HWND hWnd );          // Add dialog
    void RemoveDialog( HWND hWnd );       // Removes dialog window from the map

    void Enable( BOOL bEnable = TRUE );   // Temporary enable/disable control manager
    BOOL IsEnabled() const;               // Returns TRUE if control manager is enabled

    void TimerProc();                     // Timer procedure

    static CMapPtrToPtr m_ctrlMap;        // Main control map
    static CMapWordToPtr m_threadMap;     // Main thread map

protected:
    // Other class members
    static BOOL m_bEnabled;               // TRUE if control manager is enabled
    BOOL    m_bDialogOnly;                // Process only controls in dialogs
    HOOKPROC m_hkWndProc;                 // Old WH_WNDPROC hook procedure
    UINT_PTR m_nTimerID;                   // Our timer identifier
    static CMapPtrToPtr m_dlgMap;         // Map for dialog window (in general - for owner of controls)

    // Give access to the protected members for these functions
    friend LRESULT CALLBACK CCM_CallWndProc( int iCode, WPARAM wParam, LPARAM lParam );
    friend static LRESULT CALLBACK CCM_DialogProc( HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam );
    friend static LRESULT CALLBACK CCM_ControlProc( HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam );
    friend static void CALLBACK CCM_TimerProc( HWND hwnd, UINT nMsg, UINT nEventId, DWORD dwTime );
};

//////////////////////////////////////////////////////////////////////////////////////////
// CCoolControlsManager inlines

inline
void CCoolControlsManager::Enable( BOOL bEnable )
{
    m_bEnabled = bEnable;
}

inline
BOOL CCoolControlsManager::IsEnabled() const
{
    return m_bEnabled;
}

/////////////////////////////////////////////////////////////////////////////
// CCMControl inlines

inline
void CCoolControlsManager::CCMControl::SetState( int iFlagRemove, int iFlagAdd, BOOL bRedraw )
{
    m_nState &= ~iFlagRemove;
    m_nState |= iFlagAdd;
    if ( bRedraw )
        DrawBorder();
}

inline
int CCoolControlsManager::CCMControl::GetState() const
{
    return m_nState;
}

// Gives access to the one and only CCoolControlsManager object
CCoolControlsManager& GetCtrlManager();

#endif // __CoolControlsManager_h
