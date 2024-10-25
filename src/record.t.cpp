// Test for record.cpp
#include "record.hpp"
#include "entry.hpp"
#include <Eigen/src/Geometry/AlignedBox.h>
#include <cassert>
#include <chrono>
#include <vector>

using namespace std::chrono_literals;
using Date = std::chrono::year_month_day;
using ewi::Entry, ewi::Record, ewi::DateRange;
// Declare helpful objects
constexpr auto date0 = Date{ 2020y / std::chrono::April / 25d };
constexpr auto date1 = Date{ 2024y / std::chrono::October / 26d };
constexpr auto date2 = Date{ 2025y / std::chrono::October / 27d };
constexpr auto date3 = Date{ 2028y / std::chrono::October / 28d };
constexpr auto MAX_DATE = Date {2026y / std::chrono::January / 1d };
constexpr auto MIN_DATE = Date {2021y / std::chrono::December / 31d };
// constexpr auto d4 = Date(std::chrono::sys_days(date0) + std::chrono::days{100});

// Global date vector for instantiation
std::vector<Date> const dates{
    Date{ 2020y / std::chrono::April / 25d },
    Date{ 2024y / std::chrono::October / 26d },
    Date{ 2025y / std::chrono::October / 27d },
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
auto e0 = entry_from(date0);
auto e1 = entry_from(date1);
auto e2 = entry_from(date2);
auto e3 = entry_from(date3);

// test functions
void test_find_entries()
{
    Record empty_rec {}; // Record with no entries
    Record r1 = gen_record(1);
    Record r2 = gen_record(2);
    Record r3 = gen_record(3);

    DateRange defined_range { MIN_DATE, MAX_DATE }; 
    DateRange singularity { dates[0], dates[0] };
    DateRange all_after { .min=MIN_DATE };
    DateRange all_before { .max=MAX_DATE };
    DateRange all {};
    
    assert(!empty_rec.find_entries(all));
    
    // Test one-element case.
    auto result = r1.find_entries(singularity);
    assert(singularity.min == dates[0]);  // future-proof
                                          // the test.
    assert(result.has_value() && (result->min == result->max));
    assert(*result == r1.find_entries(all).value());

    // Test two-element case
}



int main()
{
    test_find_entries();
}
