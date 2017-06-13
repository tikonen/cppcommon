#ifdef _WIN32

#include <strsafe.h>

#include "LogUtil.h"

void DebugMsg(PCTSTR szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);

    TCHAR msg[2048];

    HRESULT hr = StringCbVPrintf(msg, sizeof(msg), szFormat, args);
    va_end(args);

    // Ensure that the formatted string is NULL-terminated
    msg[sizeof(msg) / sizeof(TCHAR) - 1] = TEXT('\0');

    OutputDebugString(msg);
    OutputDebugStringA("\n");

    /*
    MessageBox(ghwndApp, szBuffer, NULL,
    MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
    */
}

#endif
