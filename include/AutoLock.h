#pragma once

#include <Windows.h>

class CCriticalSection
{
public:
    CCriticalSection() { InitializeCriticalSection(&mCrit); }
    ~CCriticalSection() { DeleteCriticalSection(&mCrit); }

    CRITICAL_SECTION& Get() { return mCrit; }

protected:
    CRITICAL_SECTION mCrit;
};

class CAutoLock
{
public:
    CAutoLock(CCriticalSection& section)
    : mSection(section)
    {
        EnterCriticalSection(&mSection.Get());
    }
    ~CAutoLock() { LeaveCriticalSection(&mSection.Get()); }

private:
    CCriticalSection& mSection;
};
