// record.t.cpp
// Test for record.cpp
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
#include "record.hpp"
//- STL
#include <cassert>
#include <chrono>
//#include <iostream>
#include <vector>
//- In-house
#include "entry.hpp"


void test_find_entries();
void test_record_ops();
void test_metric_retrieval();

int main()
{
    test_find_entries();
    test_record_ops();
    test_metric_retrieval();
}

//-----------------------------------------Implementation--------------------------------------
// imports 
using namespace std::chrono_literals;
using Date = std::chrono::year_month_day;
using ewi::Entry, ewi::Record, ewi::DateRange, ewi::IndexRange;

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
    static const int SIZE { static_cast<int>(dates.size()) };
    static const std::vector<double> METRICS { 0.0, 1.0, 5.0, 6.0 };

    auto entry_from(Date d) -> Entry
    {
        return Entry(d, "", METRICS);
    }

    auto gen_record(int num_entries) -> Record
    {
        std::vector<Entry> entries {};
        for (int i=0; i<num_entries; i++)
            entries.push_back(entry_from(dates[i]));
        return Record(entries);
    }
}
// Test functions

/// Testing the date-based inquiry mechanism.
void test_find_entries()
{
    /*
     * find_entries required test cases:
     *  - 0-element Record
     *  - 1-element Record
     *      - Successful and non-successful
     *  - 2-element Record
     *  - 3+ element Record
     *
     *  - DateRanges:
     *      - Singularity
     *      - Fully-specified
     *      - None:Max
     *      - Min:None
     */
    DateRange singularity { dates[0], dates[0] };
    DateRange all {};
    
    // Test empty case
    Record empty_rec {};
    assert(!empty_rec.find(all));
    
    // Test one-element case.
    assert(singularity.min == dates[0]);  // future-proof the test in case `dates` changes
    Record r1 = gen_record(1);
    auto result = r1.find(singularity);
    assert(result && (result->min == result->max));
    assert(*result == r1.find(all).value());

    // Test two-element case
    Record r2 = gen_record(2);
    //// Full-date range (2024y)
    result = r2.find(DateRange { 2024y/std::chrono::January/1d, 2024y/std::chrono::December/31d } );
    assert(result);
    assert(( *result == IndexRange{ 0, 1 } ));
    result = r2.find(DateRange { .min=2024y/std::chrono::September/30d } );
    assert(result && (*result == IndexRange{ 1,1 }));
    
    // Test 3+-element case
    //// Before 2026
    assert(dates.size() > 2);
    Record r3 = gen_record(static_cast<int>(dates.size()));
    result = r3.find(DateRange { .max=2025y/std::chrono::December/31d });
    assert(result && (*result == IndexRange{ 0, 2 }));
    //// After 2026
    result = r3.find(DateRange { .min=2026y/std::chrono::January/1d });
    assert(result && (*result == IndexRange { 3, 3 }));
    //// Between Summer 2024 and Beginning of 2026
    result = r3.find(DateRange {2024y/std::chrono::June/20d, 2025y/std::chrono::December/31d });
    assert(result && (*result == IndexRange { 1, 2 }));
    //// All entries
    result = r3.find(all);
    assert(result && (*result == IndexRange { 0, (static_cast<int>(dates.size())-1) })); 
    //// No entries match
    result = r3.find(DateRange { .max=1999y/std::chrono::December/31d });
    assert(!result);
}

/// Tests entry insertion, update, and removal
void test_record_ops()
{
    std::vector<Entry> vec {
        Entry(dates[0], "First entry", std::vector<double>{0.0}),
        Entry(dates[1], "Second entry", std::vector<double>{0.0}),
        Entry(dates[2], "Third Entry", std::vector<double>{0.0}),
    };
    int ORIG_SIZE = vec.size();
    Record rec { vec };
    //std::cout << "Initial Record\n" << rec << "\n";

    Entry replacement (dates[1], "Replacement Entry", std::vector<double>{1.0});
    Entry insertion0 (2025y/std::chrono::April/22d, "Inserted from `update`", std::vector<double>{2.0});
    Entry insertion1 (dates[3], "Inserted from `add`", std::vector<double>{2.0});

    rec.update(replacement);
    assert(rec.get(dates[1])->get().notes() == "Replacement Entry");
    //std::cout << "\nFirst Replacement\n" << rec << "\n";
    rec.update(insertion0);
    assert(rec.size() == ORIG_SIZE + 1);
    rec.add(insertion1);
    assert(rec.size() == ORIG_SIZE + 2);
    
    //std::cout << "\nBefore Deletion\n" << rec << "\n";
    rec.remove(dates[3]);
    assert((rec[rec.size() - 1].date() != dates[3]) && rec.size() == ORIG_SIZE + 1);
    //std::cout << "\nAfter Deletion\n" << rec << "\n";
}

void test_metric_retrieval()
{
    int NUM_ENTRIES { 2 };
    auto rec = gen_record(SIZE);
    auto metrics = rec.metrics({ std::nullopt, 2024y/std::chrono::December/31d });
    assert(metrics);
    assert(metrics->size() == NUM_ENTRIES);
    for (auto const& vec : *metrics)
       assert (vec.get() == METRICS); 
}
