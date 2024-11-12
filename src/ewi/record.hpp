// record.hpp
#ifndef INCLUDED_EWI_RECORD
#define INCLUDED_EWI_RECORD

#ifndef INCLUDE_EWI_ENTRY
#include "entry.hpp"
#endif

#ifndef INCLUDED_STD_CHRONO
#include <chrono>
#define INCLUDED_STD_CHRONO
#endif

#ifndef INCLUDED_STD_EXPECTED
#include <expected>
#define INCLUDED_STD_EXPECTED
#endif

#ifndef INCLUDED_STD_OPTIONAL
#include <optional>
#define INCLUDED_STD_OPTIONAL
#endif

#ifndef INCLUDED_STD_OSTREAM
#include <ostream>
#define INCLUDED_STD_OSTREAM
#endif

#ifndef INCLUDED_STD_VECTOR
#include <vector>
#define INCLUDED_STD_VECTOR
#endif

#ifndef INCLUDED_EIGEN
#include <Eigen/Eigen>
#define INCLUDED_EIGEN
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
    using DateRange = InclusiveRange<std::chrono::year_month_day>;
    using IndexRange = InclusiveRange<int>;
    // std::ostream& operator<< (std::ostream& os, DateRange const&);
    std::ostream& operator<< (std::ostream& os, IndexRange const&);

    
    /// A collection of entries
    class Record 
    {
        /*  TODO Space
         *
         *  - Should I implement an iterator for this
         *  class? It would help further encapsulate the
         *  vector data member.
         */
        public:
            enum class Err { InconsistentMetrics, DisorderedDate, };
            // CONSTRUCTORS
            Record() = default;
            Record(std::vector<Entry>& entries);

            // ACCESSORS and METHODS

            /// Inserts an entry to the record.
            auto add_entry(Entry const& entry) noexcept -> std::expected<void, Err>;
            /// Gets the direct index for a single entry, if it exists.
            auto find_entry(std::chrono::year_month_day date) const noexcept -> std::optional<int>;
            /// Get the index range of entries within a given date range.
            auto find_entries(DateRange range) const noexcept -> std::optional<IndexRange>;
            /// Retrieves a copy of the entry with the given date if it exists.
            auto get_entry(std::chrono::year_month_day date) const noexcept -> std::optional<Entry>;

            /// Iterators
            inline auto begin() const noexcept { return d_entries.begin(); }
            inline auto end() const noexcept { return d_entries.end(); }
            //TODO: Adjust this to have better
            //encapsulation.
            inline auto entries() const noexcept -> std::vector<Entry> const& { return d_entries; }
            inline auto is_empty() const noexcept -> bool { return size() == 0;}
            inline auto operator[] (int idx) const -> Entry const& { return d_entries[idx]; }
            /// Query how many metrics are recorded per entry.
            inline auto metric_dim() const noexcept -> int { return is_empty() ? 0 : d_entries[0].metrics().size(); }
            /// Query number of entries in the record.
            inline auto size() const noexcept -> int { return (int) d_entries.size(); }

            /// Removes entry with specified date.
            /// If no such entry exists, do nothing.
            void remove_entry(std::chrono::year_month_day date);

            /// Replace exisiting entry with a new one.
            /// If no such entry exists, it's added.
            void update_entry(Entry const& entry);
            auto operator<=> (Record const& rhs) const = default;
        private:
            std::vector<Entry> d_entries {};
    };
    auto operator<<(std::ostream& os, Record const& rec) noexcept -> std::ostream&;
    /// Produce a matrix of metric data. Each row corresponds
    /// to a given entry.
    auto get_record_metrics(Record const& r) -> Eigen::MatrixXd;

} // namespace ewi
#endif // INCLUDED_EWI_RECORD
