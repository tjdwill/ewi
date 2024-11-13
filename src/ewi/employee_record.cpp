// employee_record.cpp
#include "employee_record.hpp"

#include <cassert>
#include <chrono>
#include <cpperrors>
#include <format>
#include <fstream>
#include <ios>       // std::{skipws, noskipws}
#include <map>
#include <sstream>
#include <ranges>    // std::views::keys
#include <stdexcept> // for std::out_of_range
#include <string>
#include <utility>   // std::pair
#include <vector>

#include <utils/string_flattener/string_flattener.hpp>


using cpperrors::Exception, cpperrors::TypedException;
using std::istringstream;
using utils::StringFlattener;
namespace ewi
{
    /* EmployeeRecord */
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


    /* EmployeeRecordIOUtils */
    void EmployeeRecordIOUtils::export_record(EmployeeRecord const& rec, std::string const& path)
    {
        std::ofstream file {path, std::ios::trunc};
        if (!file.is_open())
            throw Exception("Could not open file.");

        // Write Employee information
        auto person = rec.who();
        file << person.id.formal() << ": " << person.name << "\n";
        file << "\n";

        // Write out all Job WIRecords
        for (auto const& job: rec.jobs())
        {
            auto const& wirec = rec.get(job);
            // Write both techincal and personal records to file
            using type_pair = std::pair<Record const&, char>;
            for (
                    auto [record, rec_type]: 
                    { type_pair(wirec.technical, EmployeeRecordIOUtils::TECHINCAL_TKN), 
                    type_pair(wirec.personal, EmployeeRecordIOUtils::PERSONAL_TKN) }
            ) {

                for (auto const& e: record)
                {
                    file << job.formal() << ' ' << rec_type << ' '
                        << std::format("{:%F} ", e.date());
                    // Export Notes
                    for (int i {0}; i < EmployeeRecordIOUtils::NUM_NOTES_DELIMS; ++i)
                        file << EmployeeRecordIOUtils::NOTES_DELIM;
                    file << StringFlattener::flatten(e.notes());
                    for (int i {0}; i < EmployeeRecordIOUtils::NUM_NOTES_DELIMS; ++i)
                        file << EmployeeRecordIOUtils::NOTES_DELIM;
                    file << ' ';
                    // Export metrics
                    for (auto val: e.metrics())
                        file << val << ' ';

                    file << "\n";
                }
            }
        }
        file.flush();
    }

    auto EmployeeRecordIOUtils::import_record(std::string const& path) -> EmployeeRecord
    {
        std::ifstream file {path};
        if (!file.is_open()) {
            std::ostringstream err_msg {};
            err_msg << "Could not open file: " << std::quoted(path);
            throw Exception(err_msg.str());
        }

        std::string line {};
        std::getline(file, line); 
        std::istringstream iss {line};
        
        // Get Employee
        auto employee = parse_employee(iss);
        if (!iss && !iss.eof())
            throw Exception("Read Error.");
        EmployeeRecord output {employee};

        // Skip blank line(s)
        char check {};
        while (true)
        {
            iss.clear();
            std::getline(file, line);
            iss.str(line);
            // Chew up whitespace by attempting to read a character. This works due to the
            // default behavior of an istream to skip whitespace.
            if (iss >> check) {
                iss.putback(check);
                break;
            }
        }
        // Set up loop by parsing the first Entry
        JobID prev = parse_job(iss);
        auto type = parse_recordtype(iss);
        auto date = parse_date(iss);
        auto notes = parse_notes(iss);
        auto metrics = parse_metrics(iss);
        Entry entry { date, notes, metrics };
        assert(iss.eof());
        WIRecord curr_rec {};
        switch (type) {
            case RecordType::Technical:
                assert(curr_rec.technical.add_entry(entry));
                break;
            case RecordType::Personal:
                assert(curr_rec.personal.add_entry(entry));
                break;
            default:
                throw Exception("Unidentified RecordType.");
        }

        // Parse remaining Entries
        // Condition: (successful read + non-blank line)
        //
        // Since we are parsing files that were created with
        // `EmployeeRecordIOUtils::export_record()`, we can make some assumptions (and
        // guarantees) about what the file's structure. All lines from here contain
        // complete information regarding the Entry, its containing Record's type, and the
        // JobID associated with it. The sole exception to this statement is the trailing
        // blank line at the end of the file. We don't wnat to deal with that, so we check
        // if the line's non-empty.
        while (std::getline(file, line) && !line.empty())
        {
            iss.clear();
            iss.str(line);

            auto job = parse_job(iss);

            if (job != prev)
            {
                // Add WIRecord to EmployeeRecord
                assert(output.add(prev, curr_rec));
                curr_rec = WIRecord {};
                prev = job;
            }

            auto type = parse_recordtype(iss);
            auto date = parse_date(iss);
            auto notes = parse_notes(iss);
            auto metrics = parse_metrics(iss);
            assert(iss.eof());
            Entry e (date, notes, metrics);
            
            switch (type) {
                case RecordType::Technical:
                    assert(curr_rec.technical.add_entry(e));
                    break;
                case RecordType::Personal:
                    assert(curr_rec.personal.add_entry(e));
                    break;
                default:
                    throw Exception("Unidentified RecordType.");
            }
        }
        // add the final record
        assert(output.add(prev, curr_rec));

        return output;
    }
    auto EmployeeRecordIOUtils::parse_employee(std::istringstream &iss) -> Employee
    {
        EmployeeRecordIOUtils::seek_nonws(iss);
        std::string id_str {};
        std::string name {};
        
        std::getline(iss, id_str, EmployeeRecordIOUtils::ID_DELIM);
        EmployeeID id { id_str };

        EmployeeRecordIOUtils::seek_nonws(iss);
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
    auto EmployeeRecordIOUtils::parse_recordtype(std::istringstream &iss) -> RecordType
    {
        // This seek_nonws call is unnecessary because input extraction skips whitespace,
        // but I'll do it anyway for the sake of uniformity among the parsing functions.
        EmployeeRecordIOUtils::seek_nonws(iss); 
        char type {};
        iss >> type;
        RecordType ret;
        switch(type) {
            case EmployeeRecordIOUtils::TECHINCAL_TKN:
                ret = RecordType::Technical;
                break;
            case EmployeeRecordIOUtils::PERSONAL_TKN:
                ret = RecordType::Personal;
                break;
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
        /// Strip delimiter
        for (int i {}; i < EmployeeRecordIOUtils::NUM_NOTES_DELIMS; ++i) {
            iss >> tkn;
            assert(tkn == EmployeeRecordIOUtils::NOTES_DELIM);
        }

        iss >> std::noskipws; 
        int consec_tkns {};
        // NOTE: The order of this conditional matters.
        // Switching them will result in reading the space following the delimiter BEFORE the loop breaks.
        while (consec_tkns < EmployeeRecordIOUtils::NUM_NOTES_DELIMS && iss >> tkn ) {
            if (tkn == EmployeeRecordIOUtils::NOTES_DELIM)
                consec_tkns += 1;
            else
                consec_tkns = 0;
            notes.push_back(tkn); 
        }

        // Restore default behavior
        iss >> std::skipws;

        if (tkn != EmployeeRecordIOUtils::NOTES_DELIM)
            throw TypedException<std::string>(notes, Exception("Something went wrong when parsing notes."));
        assert(notes.length() >= EmployeeRecordIOUtils::NUM_NOTES_DELIMS);
        // Remove last delimiters
        for (int i {0}; i < EmployeeRecordIOUtils::NUM_NOTES_DELIMS; ++i)
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
            // upper level in the development hierarchy.
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

