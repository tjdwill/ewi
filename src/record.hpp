// record.hpp
#ifndef INCLUDED_RECORD
#define INCLUDED_RECORD

#ifndef INCLUDE_ENTRY
#include "entry.hpp"
#endif

#ifndef INCLUDE_METRICSTATS
#include "metricstats.hpp"
#endif

#ifndef INCLUDED_STD_CHRONO
#include <chrono>
#define INCLUDED_STD_CHRONO
#endif

#ifndef INCLUDED_STD_MAP
#include <map>
#define INCLUDED_STD_MAP
#endif

#ifndef INCLUDED_STD_OPTIONAL
#include <optional>
#define INCLUDED_STD_OPTIONAL
#endif

#ifndef INCLUDED_STD_VECTOR
#include <vector>
#define INCLUDED_STD_VECTOR
#endif

namespace ewi
{
    /// A general type that represents an ordered,
    /// monotonically-increasing range [from, until) when
    /// fully defined.
    /// If an optional member is empty, that given member
    /// semantically takes the value that makes the range
    /// widest.
    ///
    /// Ex. if we are representing a range of real numbers,
    ///
    /// ```c++
    /// using RealRange = Range<int>;
    /// // define some RealRange rng;
    /// if (!rng.from) {
    ///    // assume negative infinity as the lower bound.
    /// }
    /// ```
    template<typename T>
    struct Range
    {
        std::optional<T> from;
        std::optional<T> until;
    };

    using IndexRange = Range<int>;
    using DateRange = Range<std::chrono::year_month_day>;
    /// A collection of entries
    class Record 
    {
        public:
            // CONSTRUCTORS
            Record(std::vector<Entry>& entries) noexcept
                : d_entries{ std::move(entries) } {
                    assert(!d_entries.empty());
                }

            // ACCESSORS and METHODS
            
            // returns the total entry count
            inline auto num_entries() const noexcept -> int { return (int) d_entries.size(); }
            inline auto entries() const noexcept -> std::vector<Entry> const& { return d_entries; }
            auto find_entries(DateRange const& range) const noexcept -> std::optional<IndexRange> ;
            // Replace exisiting entry with a new one.
            void update_entry(Entry const&);
            // Inserts an entry to the record.
            void add_entry(Entry const&);
            // Sorts entries by date (least to most recent).
            void sort() noexcept;

        private:
            std::vector<Entry> d_entries {};
    };
    // Generate statistics for the given date
    // range [from, until)
    auto get_stats(
            Record const& record,
            DateRange const& range
    ) -> MetricStats;

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

            JobRecord() = delete;
            // TODO: Assert that the two records aren't
            // equal
            JobRecord(Record& job_record, Record& emotion_record);
    };

    /// Represents an employee's entire workload record for a given company.
    /// This type is meant to enable tracking the employee's workload through
    /// multiple positions in their tenure.
    struct EmployeeRecord 
    {
        public:
            EmployeeID d_employee;
            std::map<JobID, JobRecord> d_records;

            EmployeeRecord() = delete;
    };

    /// Export record data to a file
    void export_employee(EmployeeRecord const&, std::string path="");
    /// load employee from file
    EmployeeRecord load_employee(std::string id_string);
} // namespace ewi
#endif // INCLUDED_RECORD
