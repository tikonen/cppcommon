#pragma once

#ifdef _WIN32

#include <windows.h>

// Returns a string representing the result, with additional details if possible
LPCSTR HResultToString(HRESULT result);
LPCSTR GetLastErrorString();

void DebugMsg(PCTSTR szFormat, ...);

#endif
