// HighResolutionTimer.cpp : implementation file
//

#include "stdafx.h"
#include "Torque.h"
#include "HighResTimer.h"


// CHighResTimer
CHighResTimer::CHighResTimer()
{
    m_nIDTimer = NULL;
    m_uDelay = 0;
    m_pfnCallback = 0;
}

CHighResTimer::~CHighResTimer()
{
    KillTimer();
}

// CHighResTimer message handlers
void CHighResTimer::KillTimer()
{
    if (m_nIDTimer != NULL)
    {
        timeKillEvent(m_nIDTimer);
        m_nIDTimer = NULL;
    }
}

BOOL CHighResTimer::CreateTimer(CWnd* pUser, UINT uDelay, TIMERCALLBACK lpTimeProc)
{
    ASSERT(pUser);
    ASSERT(lpTimeProc);

    ASSERT_NULL_R(lpTimeProc, FALSE);

    m_pUser = pUser;
    m_pfnCallback = lpTimeProc;
    KillTimer();
    m_nIDTimer = timeSetEvent(uDelay, 0, (LPTIMECALLBACK)TimeProc, (DWORD_PTR)this,
        TIME_PERIODIC | TIME_CALLBACK_FUNCTION);

    ASSERT_NULL_R(m_nIDTimer, FALSE);

    return TRUE;
}


BOOL CHighResTimer::ValidTimer()
{
    ASSERT_NULL_R(m_nIDTimer, FALSE);

    return TRUE;
}
