#pragma once

#include <LogUtil.h>
#include <string>

class RegistryUtil
{
private:
    HKEY hKey;

public:
    RegistryUtil(LPCWSTR pPath, HKEY hParentKey = HKEY_CURRENT_USER)
    {
        auto err = RegCreateKeyExW(hParentKey, pPath, 0, NULL, REG_OPTION_NON_VOLATILE,
                                   KEY_ALL_ACCESS, NULL, &hKey, NULL);
        if (err != ERROR_SUCCESS) {
            throw std::runtime_error(std::string("Error opening key: ") + HResultToString(err));
        }
    }
    virtual ~RegistryUtil() { RegCloseKey(hKey); }

    LONG SetValue(LPCWSTR pKey, DWORD value)
    {
        return RegSetValueExW(hKey, pKey, 0, REG_DWORD, (BYTE*)&value, sizeof(value));
    }

    LONG GetValue(LPCWSTR pKey, DWORD* dValue)
    {
        DWORD len = sizeof(DWORD);
        return RegGetValueW(hKey, NULL, pKey, RRF_RT_DWORD, NULL, dValue, &len);
    }
};