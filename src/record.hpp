// record.hpp
#ifndef INCLUDED_RECORD
#define INCLUDED_RECORD

#ifndef INCLUDE_ENTRY
#include "entry.hpp"
#endif

#ifndef INCLUDE_METRICSTATS
#include "metricstats.hpp"
#endif

#ifndef INCLUDED_STD_EXPECTED
#include <expected>
#define INCLUDED_STD_EXPECTED
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
    /// monotonically-increasing range `[min, max]` when
    /// fully defined.
    /// If an optional member is empty, that given member
    /// semantically takes the value that makes the range
    /// widest.
    ///
    /// Ex. if we are representing a range of container indices:
    ///
    /// ```cpp
    ///
    /// using IndexRange = InclusiveRange<int>;
    /// // define some IndexRange idxs;
    /// if (!idxs.min) { 
    ///     // use 0 as the index 
    ///
    /// }
    /// else if (!idxs.max) {
    ///     // use vec.size() - 1 (ranges are inclusive) 
    /// 
    /// }
    /// ```
    template<typename T>
    struct InclusiveRange
    {
        std::optional<T> min;
        std::optional<T> max;
    };
    template<typename T>
    inline auto operator==(InclusiveRange<T> const& a, InclusiveRange<T> const& b) { return a.min==b.min && a.max==b.max; }
    using IndexRange = InclusiveRange<int>;
    using DateRange = InclusiveRange<std::chrono::year_month_day>;

    /// A collection of entries
    class Record 
    {
        enum class Err { InconsistentMetrics, DisorderedDate, EntryNotFound };
        public:
            // CONSTRUCTORS
            Record() = default;
            Record(std::vector<Entry>& entries);

            // ACCESSORS and METHODS
            
            inline auto num_entries() const noexcept -> int { return (int) d_entries.size(); }
            inline auto entries() const noexcept -> std::vector<Entry> const& { return d_entries; }

            /// Get the index range of entries within a given date range.
            auto find_entries(DateRange range) const noexcept -> std::optional<IndexRange>;
            
            /// Inserts an entry to the record.
            auto add_entry(Entry const& entry) noexcept -> std::expected<void, Err>;

            /// Replace exisiting entry with a new one.
            /// If no such entry exists, it's added.
            void update_entry(Entry const& entry);

            /// Removes entry with specified date.
            /// If entry doesn't exist, do nothing.
            void remove_entry(std::chrono::year_month_day date);

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
