// employee_record.hpp 
/// Meant to represent a given employee's workload history throughout their tenure at a
/// given company.
#ifndef INCLUDED_EWI_EMPLOYEE_RECORD
#define INCLUDED_EWI_EMPLOYEE_RECORD

#ifndef INCLUDED_EWI_RECORD
#include <ewi/record.hpp>
#endif 

#ifndef INCLUDED_EWI_BASIC_ID
#include <ewi/basic_id.hpp>
#endif


#ifndef INCLUDED_STD_CHRONO
#include <chrono>
#define INCLUDED_STD_CHRONO
#endif

#ifndef INCLUDED_STD_EXPECTED
#include <expected>
#define INCLUDED_STD_EXPECTED
#endif

#ifndef INCLUDED_STD_MAP
#include <map>
#define INCLUDED_STD_MAP
#endif

#ifndef INCLUDED_STD_OPTIONAL
#include <optional>
#define INCLUDED_STD_OPTIONAL
#endif

#ifndef INCLUDED_STD_SSTREAM
#include <sstream>
#define INCLUDED_STD_SSTREAM
#endif

#ifndef INCLUDED_STD_STRING
#include <string>
#define INCLUDED_STD_STRING
#endif

#ifndef INCLUDED_STD_VECTOR
#include <vector>
#define INCLUDED_STD_VECTOR
#endif

namespace ewi
{
    using JobID = BasicID;
    using EmployeeID = BasicID;
    struct Employee 
    {
        EmployeeID id;
        std::string name;
        auto operator<=>(Employee const& rhs) const = default;
        auto operator==(Employee const& rhs) const { return id == rhs.id; } 
    };
    /// Work Index Record: A structure binding the technical-based entries with the personal (emotional
    /// component) entries. Together, the records semantically form a unit representing an
    /// employee's workload record for a given job.
    struct WIRecord
    {
        Record technical {};
        Record personal {};

        auto operator<=>(WIRecord const& rhs) const  = default;
    };

    /// A type to help determine which Record a parsed
    /// Entry should be placed while forming a WIRecord.
    enum class RecordType { Technical, Personal };

    /// The class representing the employee's workload history for all job roles undertaken
    /// at a given company.
    class EmployeeRecord
    {
        public:
            EmployeeRecord(Employee emp)
                : d_employee{ emp } {}

            // MANIPULATORS

            /// Adds a new job to the data set. Returns `false` and does no operation if the job is
            /// already present.
            auto add(JobID job, WIRecord const& wi_rec) -> std::expected<void, std::string>;
            /// Adds an Entry to the structure
            auto add(JobID job, RecordType type, Entry const& entry) -> std::expected<void, std::string>;
            /// Returns a mutable reference to tbe given work record.
            /// Throws exception if the job isn't present.
            auto get_mut(JobID job) -> WIRecord&;

            // ACCESSORS

            /// Retrieves a specific job's record. Due to how `optional`, `expected`, and
            /// `variant` work, this method instead throws an exception if the key doesn't
            /// exist.
            auto get(JobID job) const -> WIRecord const&;

            /// Return an iterator over the current job IDs in the record
            auto jobs() const;
            /// Return a refernce to the Employee
            inline auto who() const -> Employee const& { return d_employee; }
            auto operator<=>(EmployeeRecord const& rhs) const  = default;
        private:
            Employee d_employee;
            std::map<JobID, WIRecord> d_data {};
    };

    /// A type that facilitates importing and exporting `EmployeeRecord` objects.
    struct EmployeeRecordIOUtils 
    {

        /// Symbols that describe which Record an Entry comes from.
        static constexpr char TECHINCAL_TKN {'T'};
        static constexpr char PERSONAL_TKN {'P'};
        /// The delimiter used to signal the start of an Entry's notes.
        /// There must be NUM_NOTES_DELIMS consecutive delimiter characters to signal start
        /// and end of the notes string.
        static constexpr char NOTES_DELIM {'\''}; 
        static constexpr int NUM_NOTES_DELIMS { 3 };  
        /// The delimiter to separate EmployeeID from informal name.
        static constexpr char ID_DELIM {':'};
        
        // IMPORT Functions

        /// Loads record from file based on provided employee ID.  In theory, the application
        /// stores employee data in a designated directory. Each employee is represented by
        /// their formal ID value (`formalIDVal.txt`).  Throws an exception if the file doesn't
        /// exist or if the file is ill-formatted.
        static auto import_record(std::string const& path) -> EmployeeRecord;

        /// All parsing functions assume a well-formed file. This is reasonable, however,
        /// because all parsable EmployeeRecord files are to be created with this struct. 
        /// The functions will, however, propagate any unexpected I/O errors.
 
        /// Parses the Employee from the file.
        /// Assumes the EmployeeID does not contain a
        /// colon character `:`. The results will be
        /// malformed if this does not hold.
        static auto parse_employee(std::istringstream& iss) -> Employee;
        /// Parses a JobID from the current line.
        /// JobID must be contiguous (contains no
        /// whitespace).
        static auto parse_job(std::istringstream& iss) -> JobID;
        /// Parses the RecordType
        static auto parse_recordtype(std::istringstream& iss) -> RecordType;
        /// Parses the Entry date
        static auto parse_date(std::istringstream& iss) -> std::chrono::year_month_day;
        /// Parses the Entry Notes If something goes awry, throws a
        /// cpperrors::TypedException<std::string> containing the information
        /// parsed up to that point.
        static auto parse_notes(std::istringstream& iss) -> std::string;
        /// Parses the Entry metrics
        /// This is the last operation performed on a given Entry line due to
        /// the parsing method.  Assumes reasonable inputs.  This function does
        /// not check for Inf or NaN, as metrics should be validated when an
        /// Entry is created.
        static auto parse_metrics(std::istringstream& iss) -> std::vector<double>;
        /// Seeks next non-whitespace character.
        static void seek_nonws(std::istringstream& iss);

        // EXPORT Functions

        /// Write an employee record to file in a format that is parsable by `load_record`.
        /// Throws exception on I/O error.
        static void export_record(EmployeeRecord const& rec, std::string const& path);
    };
}  // namespace ewi
#endif // INCLUDED_EWI_EMPLOYEE_RECORD
