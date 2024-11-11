// employee_record.cpp
#include "employee_record.hpp"

#include <cassert>
#include <chrono>
#include <cpperrors>
#include <iostream>
#include <map>
#include <sstream>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

#include <utils/string_flattener/string_flattener.hpp>


using cpperrors::Exception, cpperrors::TypedException;
using std::istringstream;
using utils::StringFlattener;
namespace ewi
{
    auto EmployeeRecord::add(JobID job, WIRecord const& wi_rec) -> bool 
    { 
        return d_data.insert({job, wi_rec}).second; 
    }
    auto EmployeeRecord::get_mut(JobID job) -> WIRecord& 
    {
        try {
            return d_data.at(job);
        } catch (std::out_of_range const& e) {
            throw Exception(e.what());
        }
    }
    auto EmployeeRecord::get(JobID job) const -> WIRecord const& 
    {
        try {
            return d_data.at(job);
        } catch (std::out_of_range const& e) {
            throw Exception(e.what());
        }
    }
    auto EmployeeRecord::jobs() const 
    {
        return std::views::keys(d_data);
    }


    // EmployeeRecordIOUtils
    auto EmployeeRecordIOUtils::parse_employee(std::istringstream &iss) -> Employee
    {
        EmployeeRecordIOUtils::seek_nonws(iss);
        std::string id_str {};
        std::string name {};
        
        std::getline(iss, id_str, EmployeeRecordIOUtils::ID_DELIM);
        EmployeeID id { id_str };
        std::getline(iss, name);
        assert(iss.eof());
        return Employee{ id, name };

    }
    auto EmployeeRecordIOUtils::parse_job(std::istringstream &iss) -> JobID
    {
        EmployeeRecordIOUtils::seek_nonws(iss);
        std::string job_id {};
        iss >> job_id;

        return JobID{ job_id };
    }
    auto EmployeeRecordIOUtils::parse_recordtype(std::istringstream &iss) -> EmployeeRecordIOUtils::RecordType
    {
        EmployeeRecordIOUtils::seek_nonws(iss);
        char type {};
        iss >> type;
        EmployeeRecordIOUtils::RecordType ret;
        switch(type) {
            case EmployeeRecordIOUtils::TECHINCAL_TKN:
                ret = EmployeeRecordIOUtils::RecordType::Technical;
                break;
            case EmployeeRecordIOUtils::PERSONAL_TKN:
                ret = EmployeeRecordIOUtils::RecordType::Personal;
            default:
                throw Exception("Incorrect RecordType value.");
        }
        return ret;
    }
    auto EmployeeRecordIOUtils::parse_date(std::istringstream &iss) -> std::chrono::year_month_day
    {
        EmployeeRecordIOUtils::seek_nonws(iss);
        std::chrono::year_month_day date;
        std::chrono::from_stream(iss, "%Y-%m-%d", date);
        assert(iss);
        return date;
    }
    auto EmployeeRecordIOUtils::parse_notes(std::istringstream& iss) -> std::string
    {
        EmployeeRecordIOUtils::seek_nonws(iss);
        std::string notes {};
        char tkn {};
        /// strip delimiter
        for (int i {}; i < EmployeeRecordIOUtils::NUM_NOTES_DELIMS; ++i) {
            iss >> tkn;
            assert(tkn == EmployeeRecordIOUtils::NOTES_DELIM);
        }

        int consec_tkns {};
        while (iss >> tkn && consec_tkns < EmployeeRecordIOUtils::NUM_NOTES_DELIMS) {
            if (tkn == EmployeeRecordIOUtils::NOTES_DELIM)
                consec_tkns += 1;
            else
                consec_tkns = 0;
            notes.push_back(tkn); 
        }
        if (tkn != EmployeeRecordIOUtils::NOTES_DELIM)
            throw TypedException<std::string>(notes, Exception("Something went wrong when parsing notes."));
        assert(notes.length() >= EmployeeRecordIOUtils::NUM_NOTES_DELIMS);
        // remove last delimiters
        for (int i {}; i < EmployeeRecordIOUtils::NUM_NOTES_DELIMS; ++i)
            notes.pop_back();
        
        return StringFlattener::expand(notes);
    }
    auto EmployeeRecordIOUtils::parse_metrics(std::istringstream& iss) -> std::vector<double>
    {
        EmployeeRecordIOUtils::seek_nonws(iss);
        /*
         * At this point, the only data left on the line should be metrics.
         * Therefore, we can continuously parse using std::strtod until the line ends.
         */

        std::vector<double> metrics {};
        std::string double_str {};
        char* remaining_str {};
        while (iss >> double_str)
        {
            // NOTE: I considered doing +inf checks here, but I think I will pass that off to an
            // upper level in the 
            metrics.push_back(std::strtod(double_str.c_str(), &remaining_str));
        }
        assert(iss.eof());
        return metrics;
    }
    void EmployeeRecordIOUtils::seek_nonws(istringstream& iss)
    {
        iss >> std::ws;
    }
}

