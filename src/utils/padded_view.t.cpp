// padded_view.t.cpp
// Test Driver for utils component
#include <utils/padded_view.hpp>
#include <cassert>
#include <cpperrors>
#include <iostream>
#include <vector>


using utils::PaddedView;
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

int main()
{
    test_padded();
}
