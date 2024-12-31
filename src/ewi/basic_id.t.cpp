// basic_id.t.cpp
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
#include "basic_id.hpp"
//- STL
#include <cassert>


using ewi::BasicID;
void test_basic_id_comp();


int main()
{
    test_basic_id_comp();
}
//--------------------------------------------------------------------------------------------------
void test_basic_id_comp()
{
    BasicID a {"ID01"};
    BasicID b {"ID02"};

    assert(a < b);
    assert(a==a);
    assert(a != b);
    assert(b > a);
}
