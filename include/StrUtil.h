#pragma once

#include <sstream>
#include <string>

namespace strutil
{
    // typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> >  tstring;

    void replace_all(std::string& str, const std::string& from, const std::string& to);

    // Convert to a string
    inline std::string str(const std::string&& str) { return str; }

    template <typename T> std::string str(const T& value)
    {
        std::ostringstream os;
        os << value;
        return os.str();
    }

    inline std::string str_r(std::ostringstream& os) { return os.str(); }

    template <typename T, typename... ARGS>
    std::string str_r(std::ostringstream& os, const T& value, ARGS... args)
    {
        os << value;
        return str_r(os, std::forward<ARGS>(args)...);
    }

    template <typename T, typename... ARGS> std::string str(const T& value, ARGS... args)
    {
        std::ostringstream os;
        os << value;
        return str_r(os, std::forward<ARGS>(args)...);
    }

    // String formatting
    inline std::string format_r(int /*pos*/, std::string format) { return format; }
    inline std::string format(const std::string format) { return format; }

    template <typename T, typename... ARGS>
    std::string format_r(int pos, std::string format, const T& value, ARGS... args);

    // convenience short hand for building a format parameter
    template <typename... ARGS> std::pair<ARGS...> _p(ARGS... args)
    {
        return std::make_pair(std::forward<ARGS>(args)...);
    };

    template <typename K, typename T, typename... ARGS>
    std::string format_r(int pos, std::string format, const std::pair<K, T>& value, ARGS... args)
    {
        std::ostringstream os;
        os << value.second;
        auto parameter = str("{", value.first, "}");
        replace_all(format, parameter, std::string(os.str()));
        return format_r(pos + 1, format, std::forward<ARGS>(args)...);
    }

    template <typename T, typename... ARGS>
    std::string format_r(int pos, std::string format, const T& value, ARGS... args)
    {
        return format_r(pos, format, std::make_pair(str(pos), value), std::forward<ARGS>(args)...);
    }

    template <typename T, typename... ARGS>
    std::string format(const std::string format, const T& value, ARGS... args)
    {
        return format_r(0, format, value, std::forward<ARGS>(args)...);
    }

    // Conversions between UTF-8 and UTF-16
    std::string to_utf8(const std::wstring& str);
    std::wstring to_utf16(const std::string& str);
} // namespace strutil
