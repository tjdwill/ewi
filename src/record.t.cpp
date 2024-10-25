// Test for record.cpp
#include "record.hpp"
#include "entry.hpp"
#include <Eigen/src/Geometry/AlignedBox.h>
#include <cassert>
#include <chrono>
#include <vector>
#include <iostream>

// imports 
using namespace std::chrono_literals;
using Date = std::chrono::year_month_day;
using ewi::Entry, ewi::Record, ewi::DateRange, ewi::IndexRange;

// Declare helpful objects
constexpr auto date0 = Date{ 2020y / std::chrono::April / 25d };
constexpr auto date1 = Date{ 2024y / std::chrono::October / 26d };
constexpr auto date2 = Date{ 2025y / std::chrono::October / 27d };
constexpr auto date3 = Date{ 2028y / std::chrono::October / 28d };
// constexpr auto d4 = Date(std::chrono::sys_days(date0) + std::chrono::days{100});
// Global date vector for instantiation
std::vector<Date> const dates
{
    Date{ 2024y / std::chrono::April / 25d },
    Date{ 2024y / std::chrono::October / 26d },
    Date{ 2025y / std::chrono::September / 27d },
    Date{ 2028y / std::chrono::October / 28d },
};


auto entry_from(Date d) -> Entry
{
    return Entry(d, "", std::vector<double>{});
}
auto gen_record(int num_entries) -> Record
{
    std::vector<Entry> entries {};
    for (int i=0; i<num_entries; i++)
        entries.push_back(entry_from(dates[i]));
    return Record(entries);
}

// test functions
void test_find_entries()
{
    constexpr auto MAX_DATE = Date {2026y / std::chrono::January / 1d };
    constexpr auto MIN_DATE = Date {2021y / std::chrono::December / 31d };
    DateRange singularity { dates[0], dates[0] };
    DateRange all {};
    
    Record empty_rec {}; // Record with no entries
    assert(!empty_rec.find_entries(all));
    
    // Test one-element case.
    Record r1 = gen_record(1);
    auto result = r1.find_entries(singularity);
    assert(singularity.min == dates[0]);  // future-proof
                                          // the test.
    assert(result.has_value() && (result->min == result->max));
    assert(*result == r1.find_entries(all).value());

    // Test two-element case
    Record r2 = gen_record(2);
    //// Full-date range (2024y)
    result = r2.find_entries(DateRange { 2024y/std::chrono::January/1d, 2024y/std::chrono::December/31d } );
    assert(result);
    std::cerr << *result << "\n";
    assert( (*result == IndexRange{0, 1}) );
    result = r2.find_entries(DateRange {.min=2024y/std::chrono::September/30d } );
    assert(result && (*result == IndexRange{1,1}));
    
    // Test 3+-element case
    //// Before 2026
    assert(dates.size() > 2);
    Record r3 = gen_record((int) dates.size());
    result = r3.find_entries(DateRange { .max=2025y/std::chrono::December/31d });
    assert(result && (*result == IndexRange{0, 2}));
    //// After 2026
    result = r3.find_entries(all);
    assert(result && (*result == IndexRange {0, ((int) dates.size())-1 })); 
    //// No entries match
    result = r3.find_entries(DateRange { .max=1999y/std::chrono::December/31d });
    assert(!result);
}



int main()
{
    test_find_entries();
}
