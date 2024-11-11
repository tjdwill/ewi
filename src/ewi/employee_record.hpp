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

#ifndef INCLUDED_STD_STRING
#include <string>
#define INCLUDED_STD_STRING
#endif

#ifndef INCLUDED_STD_VECTOR
#include <vector>
#define INCLUDED_STD_VECTOR
#endif


// Forward Declarations
class ifstream;
class ofstream;
class year_month_day;

namespace ewi
{
    using JobID = BasicID;
    using EmployeeID = BasicID;
    struct Employee 
    {
        EmployeeID id;
        std::string name;
    };
    /// A structure binding the technical-based entries with the personal (emotional
    /// component) entries. Together, the records semantically form a unit representing an
    /// employee's workload record for a given job.
    struct WIRecord
    {
        Record technical {};
        Record personal {};
    };

    /// The class representing the employee's workload history for all job roles undertaken
    /// at a given company.
    class EmployeeRecord
    {
        public:
            EmployeeRecord(Employee emp)
                : d_employee{ emp } {}


            /// Adds a new job to the data set. Returns `std::nullopt` if the job is
            /// already present.
            auto add(JobID job, WIRecord const& wi_rec) -> std::optional<void>;
            /// Retrieves a specific job's record. Due to how `optional`, `expected`, and
            /// `variant` work, this method instead throws an exception if the key doesn't
            /// exist.
            auto get(JobID job) const -> WIRecord const&;
            /// Returns a mutable reference to tbe given work record.
            /// Throws exception if the job isn't present.
            auto get_mut(JobID job) -> WIRecord&;
            /// Return an iterator over the current job IDs in the record
            auto jobs() const;
        private:
            Employee d_employee;
            std::map<JobID, WIRecord> d_data {};
    };

    /// A type that facilitates importing and exporting `EmployeeRecord` objects.
    struct EmployeeRecordIOUtils 
    {
        /// A type to help determine which Record a parsed
        /// Entry should be placed while forming a WIRecord.
        enum class RecordType { Technical, Personal };

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
        static auto parse_employee(ifstream& ifs) -> Employee;
        /// Parses a JobID from the current line.
        static auto parse_job(ifstream& ifs) -> JobID;
        /// Parses the RecordType
        static auto parse_recordtype(ifstream& ifs) -> RecordType;
        /// Parses the Entry date
        static auto parse_date(ifstream& ifs) -> year_month_day;
        /// Parses the Entry metrics
        static auto parse_metrics(ifstream& ifs) -> std::vector<double>;
        /// Parses the Entry Notes
        static auto parse_notes(ifstream& ifs) -> std::string;
        /// Seeks next non-whitespace character.
        static void seek_nonws(ifstream& ifs);
        // EXPORT Functions

        /// Write an employee record to file in a format that is parsable by `load_record`.
        /// Throws exception on I/O error.
        static void export_record(EmployeeRecord const& rec, std::string const& path);
    };
}  // namespace ewi
#endif // INCLUDED_EWI_EMPLOYEE_RECORD
