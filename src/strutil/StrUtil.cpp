#include <codecvt>
#include <locale>

#include "StrUtil.h"

namespace strutil
{
    void replace_all(std::string& str, const std::string& from, const std::string& to)
    {
        size_t pos = 0;
        while ((pos = str.find(from, pos)) != std::string::npos) {
            str.replace(pos, from.length(), to);
            pos += to.length();
        }
    }

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utf8converter;

    std::string to_utf8(const std::wstring& str) { return utf8converter.to_bytes(str); }

    std::wstring to_utf16(const std::string& str) { return utf8converter.from_bytes(str); }
} // namespace strutil
