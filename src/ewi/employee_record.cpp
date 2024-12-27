// employee_record.cpp
#include "employee_record.hpp"
// STL
#include <cassert>
#include <chrono>
#include <expected>
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
// Third-party
#include <cpperrors>
// In-house
#include <utils/string_flattener/string_flattener.hpp>


using cpperrors::Exception, cpperrors::TypedException;
using std::istringstream;
using utils::StringFlattener;
namespace ewi
{
    /* EmployeeRecord */
    auto EmployeeRecord::add(JobID job, WIRecord const& wi_rec) -> std::expected<void, std::string> 
    { 
        if (!d_data.insert({job, wi_rec}).second)
            return std::unexpected("Job already exists for this record.");
        else 
            return {};
    }

    auto EmployeeRecord::add(JobID job, RecordType type, Entry const& e) -> std::expected<void, std::string>
    {
        if (auto test = d_data.find(job); test != d_data.end())
        {
            // Try to add the Entry to the specified Record.
            std::string msg {};
            auto& wi_rec = get_mut(job);
            std::expected<void, Record::Err> result;
            switch (type) 
            {
                case RecordType::Technical:
                    result = wi_rec.technical.add(e);
                    break;
                case RecordType::Personal:
                    result = wi_rec.personal.add(e);
                    break;
                default:
                    throw Exception("Unknown RecordType");
            }
            // check results of the operation.
            if (!result)
            {
                switch (result.error())
                {
                    case Record::Err::DisorderedDate:
                        msg = "Attempted to add Entry earlier than last Entry date.";
                        break;
                    case Record::Err::InconsistentMetrics:
                        msg = "Entry metrics are incompatible in quantity with those already present in the record.";
                        break;
                    default:
                        throw Exception("Unknown Record::Err type.");
                }
            }
            
            if (!msg.empty())
                return std::unexpected(msg);
            else 
                return {};
        }
        // Key doesn't exist, so create record.
        WIRecord wi_rec {};
        switch (type)
        {
            case RecordType::Technical:
                assert(wi_rec.technical.add(e));  // should never fail.
                break;
            case RecordType::Personal:
                assert(wi_rec.personal.add(e));
                break;
            default:
                throw Exception("Unknown RecordType");
        }
        return add(job, wi_rec);
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
    /* EXPORT Functions */
    
    void EmployeeRecordIOUtils::export_entry(
            std::ostream &os,
            Entry const& e,
            JobID const& job,
            RecordType type
    )
    {
        os << job.formal() << ' ' << get_token(type) << ' '
            << std::format("{:%F}", e.date()) << ' ';
        // Export Notes
        for (int i {0}; i < EmployeeRecordIOUtils::NUM_NOTES_DELIMS; ++i)
            os << EmployeeRecordIOUtils::NOTES_DELIM;
        os << StringFlattener::flatten(e.notes());
        for (int i {0}; i < EmployeeRecordIOUtils::NUM_NOTES_DELIMS; ++i)
            os << EmployeeRecordIOUtils::NOTES_DELIM;
        os << ' ';
        // Export metrics
        for (auto val: e.metrics())
            os << val << ' ';

        os << "\n";
    }

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
        using type_pair = std::pair<Record const&, RecordType>;
        for (auto const& job: rec.jobs())
        {
            auto const& wi_rec = rec.get(job);
            // Write both technical and personal records to file
            for (auto [record, rec_type]: {
                    type_pair(wi_rec.technical, RecordType::Technical),
                    type_pair(wi_rec.personal, RecordType::Personal) 
                 }
            ) 
                for (auto const& e: record)
                    export_entry(file, e, job, rec_type);
        }
        file.flush();
    }

    /* IMPORT Functions */

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
            if (iss >> check)
                break;
        }
        // Parse Entries
        //
        // Condition: non-blank line
        //
        // Since we are parsing files that were created with
        // `EmployeeRecordIOUtils::export_record()`, we can make some assumptions (and
        // guarantees) about what the file's structure. All lines from here contain
        // complete information regarding the Entry, its containing Record's type, and the
        // JobID associated with it. The sole exception to this statement is the trailing
        // blank line at the end of the file. We don't wnat to deal with that, so we check
        // if the line's non-empty.
        while (!line.empty())
        {
            iss.clear(); // clears eof status flag
            iss.str(line);

            auto job = parse_job(iss);
            auto type = parse_recordtype(iss);
            auto date = parse_date(iss);
            auto notes = parse_notes(iss);
            auto metrics = parse_metrics(iss);
            assert(iss.eof());  // eof in this case means "end of line"
            Entry e (date, notes, metrics);
            assert(output.add(job, type, e)); 

            // Get next line; This is done at the end of the iteration due to the way the
            // function is structured. Not doing so would skip the first entry since we
            // loaded that line previously when skipping the blank line(s).
            std::getline(file, line);
        }

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
        if (!iss)
            throw Exception("Incorrect format: Could not read date.");
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
            if (tkn != EmployeeRecordIOUtils::NOTES_DELIM)
                throw Exception("Incorrect User Data Format.");
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
        if(!iss && !iss.eof())
            throw Exception("Could not get user metric data.");
        return metrics;
    }
    void EmployeeRecordIOUtils::seek_nonws(istringstream& iss)
    {
        iss >> std::ws;
    }
}

