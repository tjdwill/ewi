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

// std::reference_wrapper
#ifndef INCLUDED_STD_FUNCTIONAL
#include <functional>
#define INCLUDED_STD_FUNCTIONAL
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
        public:
            enum class Err { InconsistentMetrics, DisorderedDate, };
            // CONSTRUCTORS
            Record() = default;
            Record(std::vector<Entry>& entries);

            // ACCESSORS

            /// Iterators
            /// TODO: Read about std::ranges to understand how it works and what type I
            /// should return. Currently, I'm deriving the type via `auto` (and
            /// inlining the function) because I don't
            /// know how to define the signature myself.
            inline auto begin() const noexcept { return d_entries.begin(); }
            inline auto end() const noexcept { return d_entries.end(); }

            /// Gets the direct index for a single entry, if it exists.
            auto find(std::chrono::year_month_day date) const noexcept -> std::optional<int>;
            /// Get the index range of entries within a given date range. Returns
            /// singularity IndexRange (ex. {0, 0}) if a singularity DateRange is passed in
            /// and an Entry exists for that date.
            auto find(DateRange range) const noexcept -> std::optional<IndexRange>;

            /// Retrieves a reference to the entry with the given date(s) if it exists.
            auto get(std::chrono::year_month_day date) const noexcept -> std::optional<std::reference_wrapper<Entry const>>;
            auto get(DateRange const& dates) const noexcept -> std::optional<std::vector<std::reference_wrapper<Entry const>>>;

            /// Query if Record has no entries.
            auto is_empty() const noexcept -> bool; 
            /// Query how many metrics are recorded per entry.
            auto metric_dim() const noexcept -> int;
            /// Retrieve metrics for a given date range.
            ///
            /// NOTE:
            ///     The const on the metrics is cast away to be compatible with Eigen.  The
            ///     assumption is that this method will never be used to modify metrics
            ///     directly.
            ///
            ///     The alternative was to define multiple const/non-const variations of
            ///     each method, even extending to changes in ewi::Entry. That may be
            ///     viable in the future, but for now, I'll stick to this simple solution.
            auto metrics(std::chrono::year_month_day date) const noexcept -> std::optional<std::reference_wrapper<std::vector<double>>>;
            auto metrics(DateRange const& dates) const noexcept -> std::optional<std::vector<std::reference_wrapper<std::vector<double>>>>;

            /// Query number of entries in the record.
            auto size() const noexcept -> int;
            /// Access the Record via numeric index. 
            ///
            /// Note: This method does not perform bounds checking. It's recommended to use
            /// this method of access only after receiving valid indices from a call to
            /// `Record::find()`;
            auto operator[] (int idx) const -> Entry const&; 
            auto operator<=> (Record const& rhs) const = default;

            // MANIPULATORS

            /// Inserts an entry to the record.
            auto add(Entry const& entry) noexcept -> std::expected<void, Err>;
            /// Removes entry with specified date.
            /// If no such entry exists, do nothing.
            void remove(std::chrono::year_month_day date);
            /// Replace exisiting entry with a new one.
            /// If no such entry exists, it's added.
            void update(Entry const& entry);
        private:
            std::vector<Entry> d_entries {};
    };
    auto operator<<(std::ostream& os, Record const& rec) noexcept -> std::ostream&;

} // namespace ewi
#endif // INCLUDED_EWI_RECORD
