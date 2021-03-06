#pragma once

#include <string>
#include <vector>

namespace CSV
{
    typedef std::vector<std::string> Row;
    std::vector<Row> Parse(const char* data, size_t length);
} // namespace CSV
