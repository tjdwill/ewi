/// Entry test
/// Tests:
///     - Date comparison
///     - Entry equality comparison
#include "entry.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <Eigen/Eigen>

using Date = std::chrono::year_month_day;
using ewi::Entry;
using Eigen::VectorXd;

void date_comp_test() 
{    
    using namespace std::chrono;
    // different date
    constexpr auto date = Date(2024y, std::chrono::October, 23d);
    Entry a(
            date,
            "",
            VectorXd::Zero(0)
    );
    Entry b(
            date + std::chrono::months(2),
            "",
            VectorXd::Zero(0)
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
            VectorXd::Zero(2)
    );
    Entry b(
            date,
            "",
            VectorXd::Zero(1)
    );
    Entry c(
            date,
            "",
            VectorXd::Ones(1)
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
