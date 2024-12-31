// string_flattener.t.cpp
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
#include <cassert>
#include <string>

std::string const EXPANDED {"Hello\nthis\nis\na\nreally\nspaced\nstring."};
std::string const COMPRESSED {"Hello\u0001this\u0001is\u0001a\u0001really\u0001spaced\u0001string."};
std::string const INCOMPRESSIBLE {"Hi."};

using Flattener = utils::StringFlattener;
void test_flatten();

int main()
{
    test_flatten();
}
//--------------------------------------------------------------------------------------------------
void test_flatten()
{
    auto flattened = Flattener::flatten(EXPANDED);
    auto expanded = Flattener::expand(COMPRESSED);

    assert(flattened == COMPRESSED);
    assert(expanded == EXPANDED);
    assert(Flattener::flatten(INCOMPRESSIBLE) == INCOMPRESSIBLE);
    assert(Flattener::expand(INCOMPRESSIBLE) == INCOMPRESSIBLE);
}
