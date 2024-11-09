// string_flattener.t.cpp
#include "string_flattener.hpp"

#include <cassert>
#include <string>

const std::string EXPANDED {"Hello\nthis\nis\na\nreally\nspaced\nstring."};
const std::string COMPRESSED {"Hello\u0001this\u0001is\u0001a\u0001really\u0001spaced\u0001string."};
const std::string INCOMPRESSIBLE {"Hi."};

using Flattener = utils::StringFlattener;
void test_flatten()
{
    auto flattened = Flattener::flatten(EXPANDED);
    auto expanded = Flattener::expand(COMPRESSED);

    assert(flattened == COMPRESSED);
    assert(expanded == EXPANDED);
    assert(Flattener::flatten(INCOMPRESSIBLE) == INCOMPRESSIBLE);
    assert(Flattener::expand(INCOMPRESSIBLE) == INCOMPRESSIBLE);
}

int main()
{
    test_flatten();
}
