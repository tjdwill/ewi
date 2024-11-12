// employee_record.t.cpp
// EmployeeRecord Test Driver
#include "employee_record.hpp"

#include <cassert>
#include <chrono>
#include <cpperrors>
#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <utils/string_flattener/string_flattener.hpp>

using namespace ewi;
using namespace std::chrono_literals;
using utils::StringFlattener;


auto gen_record() -> Record 
{
    Entry a0 (
            std::chrono::year_month_day(2024y, std::chrono::November, 12d),
            "Multi-lined\nNotes.",
            std::vector<double> {0., 0.1, 7., 3.14159, -11.7}
    );
    Entry a1 (
            std::chrono::year_month_day(2024y, std::chrono::November, 13d),
            "Single-line Notes.",
            std::vector<double> {0.,  4.0, 7.0, 3.14159, -11.7}
    );
    Entry a2 (
            std::chrono::year_month_day(2024y, std::chrono::November, 14d),
            "", // Try empty case.
            std::vector<double> {0.,  4.0, 7.0, 3.14159, -11.7}
    );
    std::vector<Entry> entries { a0, a1, a2 };

    return Record(entries);
}

void test_ER_IO()
{
    // I have to build a record manually :(
    Employee person { EmployeeID { "55555"}, "Bugs Bunny" };
    JobID merry_melodies { "1940" };
    JobID looney_tunes { "1970" };
    
    WIRecord mm {gen_record(), gen_record()};
    WIRecord lt {gen_record(), gen_record()};

    EmployeeRecord emp_rec { person };
    emp_rec.add(merry_melodies, mm);
    emp_rec.add(looney_tunes, lt);

    std::string file_name {"bugs_record_00.txt"};
    EmployeeRecordIOUtils::export_record(emp_rec, file_name);

    auto parsed_rec = EmployeeRecordIOUtils::import_record(file_name);
    EmployeeRecordIOUtils::export_record(parsed_rec, "bugs_bunny_01.txt");
    assert(parsed_rec == emp_rec);
}

void test_employee_parse()
{
    std::stringstream ss {};
    ss << "ID2456791: Terrance Williams";
    std::istringstream iss {ss.str()};
    auto test = EmployeeRecordIOUtils::parse_employee(iss);
    assert(test.id == EmployeeID{"ID2456791"});
    assert(test.name == "Terrance Williams");
}

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
        test_employee_parse();
        test_entry_parse();
        test_ER_IO();
    } catch (TypedException<std::string> const& e) {
        std::cerr << e.err().report(true) << "\n" 
            << "Data: " << e.data() << "\n";
        throw;
    } catch (Exception const& e) {
        std::cerr << e.report(true) << "\n";
        throw;
    }
}
