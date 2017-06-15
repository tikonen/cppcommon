
#include <CppCommonConfig.h>

#if HAS_BOOST

#include <boost/regex.hpp>

#include <CSVReader.h>

// used to split the file in lines
const boost::regex linesregx("\\r\\n|\\n\\r|\\n|\\r");

// used to split each line to tokens, assuming ',' as column separator
const boost::regex fieldsregx(",(?=(?:[^\"]*\"[^\"]*\")*(?![^\"]*\"))");

namespace CSV
{
    std::vector<Row> Parse(const char* data, size_t length)
    {
        std::vector<Row> result;

        // iterator splits data to lines
        boost::cregex_token_iterator li(data, data + length, linesregx, -1);
        boost::cregex_token_iterator end;

        while (li != end) {
            std::string line = li->str();
            ++li;
            if (line.length() == 0) {
                break;
            }

            // Split line to tokens
            boost::sregex_token_iterator ti(line.begin(), line.end(), fieldsregx, -1);
            boost::sregex_token_iterator end2;

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
}

#endif
