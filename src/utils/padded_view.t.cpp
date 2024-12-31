// padded_view.t.cpp
// Test Driver for utils component
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
#include "padded_view.hpp"
//- STL
#include <cassert>
#include <iostream>
#include <vector>
//- Third-party
#include <cpperrors>


using utils::PaddedView;
void test_padded();

int main()
{
    test_padded();
}
//--------------------------------------------------------------------------------------------------
void test_padded()
{
    std::vector<int> vec { 0, 1, 2, 3, 4 };
    auto padded = PaddedView(vec); 
    assert(padded.size() == (int) vec.size() + 2);
    assert(padded[0] == padded[1] && padded[1] == vec[0]);
    assert(padded[padded.size() - 1] == vec[vec.size() - 1]);
    for (int i {1}; i < padded.size() - 1; ++i)
        assert(padded[i] == vec[i-1]);


    std::vector<float> bad_vec {};
    try {
        auto bad_padded = PaddedView(bad_vec);
        bad_padded[0]; // just to silence the unused
                       // variable warning.
    }
    catch (cpperrors::Exception const& e)
    {
        std::cout << e.what() << "\n";    
    }
}
