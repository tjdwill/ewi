// employee_record.t.cpp
// EmployeeRecord Test Driver
#include "employee_record.hpp"
#include "utils/string_flattener/string_flattener.hpp"

#include <cassert>
#include <chrono>
#include <cpperrors>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <utils/string_flattener/string_flattener.hpp>

using namespace ewi;
using namespace std::chrono_literals;
using utils::StringFlattener;

void test_entry_parse()
{
    std::stringstream ss {};
    ss << "0260 " 
        << 'T' 
        << " 2024-11-11 " 
        << "'''These are notes" << StringFlattener::SUBTITUTION_STR << "that have multiple lines.''' "
        << "0 3.22 4.556 10";
    
    RecordType rec_type;
    std::chrono::year_month_day date;
    std::string notes {};
    std::vector<double> metrics {};

    std::istringstream iss {ss.str()};

    auto id = EmployeeRecordIOUtils::parse_job(iss);
    rec_type = EmployeeRecordIOUtils::parse_recordtype(iss);
    date = EmployeeRecordIOUtils::parse_date(iss);
    notes = EmployeeRecordIOUtils::parse_notes(iss);
    metrics = EmployeeRecordIOUtils::parse_metrics(iss);

    assert(id == JobID{ "0260" });
    assert(rec_type == RecordType::Technical);
    assert(date == std::chrono::year_month_day(2024y, std::chrono::November, 11d));
    assert(notes == "These are notes\nthat have multiple lines.");
    std::vector<double> vec { 0, 3.22, 4.556, 10 };
    assert(metrics == vec);
}

int main()
{
    using cpperrors::Exception, cpperrors::TypedException;
    try {
        test_entry_parse();
    } catch (TypedException<std::string> const& e) {
        std::cerr << e.err().report(true) << "\n" 
            << "Data: " << e.data() << "\n";
    } catch (Exception const& e) {
        std::cerr << e.report(true) << "\n";
    }
}
