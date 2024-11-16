// metrics.t.cpp
#include "metrics.hpp"

#include <cassert>
#include <iostream>

#include <Eigen/Eigen>

#include <ewi/record.hpp>
#include <ewi/entry.hpp>

// imports 
using namespace std::chrono_literals;
using Date = std::chrono::year_month_day;
using ewi::Entry, ewi::Record, ewi::DateRange, ewi::IndexRange;

// Helper functions
namespace 
{
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
        return Entry(d, "", std::vector<double> { 0.0, 1.0, 5.0, 6.0 });
    }

    auto gen_record(int num_entries) -> Record
    {
        std::vector<Entry> entries {};
        for (int i=0; i<num_entries; i++)
            entries.push_back(entry_from(dates[i]));
        return Record(entries);
    }
}

void test_record_metrics()
{
    assert( !(ewi::get_record_metrics(Record {})) );

    Record r = gen_record((int) dates.size());
    auto metrics = ewi::get_record_metrics(r);
    assert(metrics);
    assert((int) (*metrics).rows() == r.size());
    assert((int) (*metrics).cols() == r.metric_dim());
    std::cout << "The Record:\n" << r << "\n";
    std::cout << "Record metrics:\n" << *metrics << "\n";
}

int main()
{
    test_record_metrics();
}
