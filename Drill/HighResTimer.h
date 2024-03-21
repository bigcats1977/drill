#pragma once


// CHighResTimer
//High-Resolution Timer   
//多媒体高精度时钟应用类   
/////////////////////////////////////////////  

#include <MMSYSTEM.H>   
#pragma comment(lib, "WINMM.LIB")  

class CHighResTimer : public CWnd
{
    typedef void (*TIMERCALLBACK)(CWnd*);

public:
    CHighResTimer();
    virtual ~CHighResTimer();
    BOOL CreateTimer(CWnd* pUser, UINT uDelay, TIMERCALLBACK lpTimeProc);
    void KillTimer();
    BOOL ValidTimer();

    static void CALLBACK TimeProc(UINT uID, UINT uMsg, CWnd* pUser, DWORD dw1, DWORD dw2)
    {
        // dwUser contains ptr to Timer object
        CHighResTimer* ptimer = (CHighResTimer*)pUser;
        // Call user-specified callback and pass back user specified data
        if (NULL != ptimer)
            (ptimer->m_pfnCallback) (ptimer->m_pUser);
    }

private:
    CWnd* m_pUser;
    MMRESULT        m_nIDTimer;
    UINT            m_uDelay;
    TIMERCALLBACK   m_pfnCallback;
};


