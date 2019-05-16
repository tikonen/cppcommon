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

LPCSTR GetErrorString(const DWORD error)
{
    static CHAR buffer[MAX_PATH];

    // Call FormatMessage to get error string from winapi
    LPWSTR _buffer = nullptr;
    DWORD size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                (LPWSTR)&_buffer, 0, nullptr);

    if (size > 0) {
        char* descbuffer = new char[size * 2];
        size_t convertCount;
        wcstombs_s(&convertCount, descbuffer, size * 2, _buffer, size);
        snprintf(buffer, sizeof(buffer), "%x : %s", error, descbuffer);
    } else {
        snprintf(buffer, sizeof(buffer), "%x", error);
    }
    LocalFree(_buffer);
    return buffer;
}

LPCSTR GetLastErrorString() { return GetErrorString(GetLastError()); }

LPCSTR HResultToString(const HRESULT result)
{
    // Handle success case
    if (!FAILED(result))
        return "Success";

    return GetErrorString(result); // Call other helper to convert win32 error to string
}

#endif