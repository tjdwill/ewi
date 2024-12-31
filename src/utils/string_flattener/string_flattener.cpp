// string_flattener.cpp
/*
* Copyright (C) 2024 Terrance Williams
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "string_flattener.hpp"
//- STL
#include <string>

namespace {
    void replace(std::string& str, char old_c, char new_c)
    {
        for (auto& c: str)
            if (c == old_c)
                c = new_c;
    }
}
namespace utils
{
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
