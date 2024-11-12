// basic_id.t.cpp
#include "basic_id.hpp"

#include <cassert>

using ewi::BasicID;
void test_basic_id_comp()
{
    BasicID a {"ID01"};
    BasicID b {"ID02"};

    assert(a < b);
    assert(a==a);
    assert(a != b);
    assert(b > a);
}

int main()
{
    test_basic_id_comp();
}
