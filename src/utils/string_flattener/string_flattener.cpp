// string_flattener.cpp
#include "string_flattener.hpp"

#include <string>

namespace {
    void replace(std::string & str, char old_c, char new_c)
    {
        for (auto& c: str)
            if (c == old_c)
                c = new_c;
    }
}
namespace utils {
    auto StringFlattener::flatten(std::string str) -> std::string 
    {
        // str is a copy, so we can mutate it directly.
        replace(str, StringFlattener::NEWLINE, StringFlattener::SUBTITUTION_STR);
        return str;
    }

    auto StringFlattener::expand(std::string str) -> std::string 
    {
        // str is a copy, so we can mutate it directly.
        replace(str, StringFlattener::SUBTITUTION_STR, StringFlattener::NEWLINE);
        return str;
    }
} // namespace utils
