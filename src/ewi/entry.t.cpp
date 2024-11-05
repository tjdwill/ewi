/// Entry test
/// Tests:
///     - Date comparison
///     - Entry equality comparison
#include "entry.hpp"
#include <cassert>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>

using Date = std::chrono::year_month_day;
using ewi::Entry;

void date_comp_test() 
{    
    using namespace std::chrono;
    // different date
    constexpr auto date = Date(2024y, std::chrono::October, 23d);
    Entry a(
            date,
            "",
            std::vector<double> {0.}
    );
    Entry b(
            date + std::chrono::months(2),
            "",
            std::vector<double> {0.}
    );

    assert(a < b);
    assert(a <= b);
    assert(!(a < a));
}

void equal_comp_test() 
{    
    using namespace std::chrono;
    // different date
    constexpr auto date = Date(2024y, std::chrono::October, 23d);
    Entry a(
            date,
            "",
            std::vector<double> {0., 0.1}
    );
    Entry b(
            date,
            "",
            std::vector<double> {0.}
    );
    Entry c(
            date,
            "",
            std::vector<double> {}
    );


    assert(a == a);
    assert(a != b);
    assert((b != c) && (a != c));
}

int main() 
{
    date_comp_test();
    equal_comp_test();
}
