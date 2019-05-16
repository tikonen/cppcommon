
#include <CppCommonConfig.h>

#include <regex>

#include <CSVReader.h>

// used to split the file in lines
const std::regex linesregx("\\r\\n|\\n\\r|\\n|\\r");

// used to split each line to tokens, assuming ',' as column separator
const std::regex fieldsregx(",(?=(?:[^\"]*\"[^\"]*\")*(?![^\"]*\"))");

namespace CSV
{
    std::vector<Row> Parse(const char* data, size_t length)
    {
        std::vector<Row> result;

        // iterator splits data to lines
        std::cregex_token_iterator li(data, data + length, linesregx, -1);
        std::cregex_token_iterator end;

        while (li != end) {
            std::string line = li->str();
            ++li;
            if (line.length() == 0) {
                break;
            }

            // Split line to tokens
            std::sregex_token_iterator ti(line.begin(), line.end(), fieldsregx, -1);
            std::sregex_token_iterator end2;

            std::vector<std::string> row;
            while (ti != end2) {
                std::string token = ti->str();
                ++ti;
                row.push_back(token);
            }
            if (line.back() == ',') {
                // last character was a separator
                row.push_back("");
            }
            result.push_back(row);
        }
        return result;
    }
} // namespace CSV
