#ifndef INCLUDED_EXPLORE
#define INCLUDED_EXPLORE

#ifndef INCLUDED_STD_CASSERT
#include <cassert>
#define INCLUDED_STD_CASSERT
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

namespace ewi 
{
    /// A class for representing identification.
    /// Includes unambiguous data member (d_id)
    /// and a more "human-readable" member (d_name).
    struct AbstractID
    {
        std::string d_id;
        std::string d_name;
    };
    struct JobID : AbstractID {};
    struct EmployeeID : AbstractID {};

    /// Represents an employee's entire survey information
    /// for a given job.
    struct JobRecord
    {
        public:
            Record d_job;
            Record d_emotion;
    };

    /// Represents an employee's entire workload record for a given company.
    /// This type is meant to enable tracking the employee's workload through
    /// multiple positions in their tenure.
    struct EmployeeRecord
    {
        public:
            EmployeeID d_employee;
            std::map<JobID, JobRecord> d_records;
    };

    /// Export record data to a file
    void export_employee(EmployeeRecord const&, std::string path="");
    /// load employee from file
    EmployeeRecord load_employee(std::string id_string);
} // namespace ewi


#endif // INCLUDED_EXPLORE
