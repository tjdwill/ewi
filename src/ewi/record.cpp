// record.cpp
#include "record.hpp"
#include "entry.hpp"
#include <utils/utils.hpp>   // PaddedView

#include <algorithm>
#include <chrono>
#include <expected>
#include <iostream>
#include <optional>
#include <vector>
#include <cpperrors>
#include <Eigen/Eigen>

// Helper Functions
constexpr inline int floored_avg(int a, int b)
{
    return (a+b)/2;
}
namespace ewi
{
    using err::Exception;

    using std::ostream;
    ostream& operator<< (ostream& os, IndexRange const& rng)
    {
        if (!(rng.min))
            os << "[None,";
        else
            os << '[' << *rng.min << ',';
        if (!(rng.max))
            os << " None]";
        else
            os << ' ' << *rng.max << ']';
        return os;
    }

    Record::Record (std::vector<Entry>& entries)
        : d_entries{ std::move(entries) }
    {
        /* Invariants
         * Each entry in a record must have a unique date and must have the
         * same number of metric elements as the other
         * entries.
         */
        if (!d_entries.empty()) {
           for (int i=0; i < (int) d_entries.size()-1; ++i){
                if (!(d_entries[i+1].date() > d_entries[i].date()))
                    throw Exception("Each entry must be a later date than the previous.");
                if (d_entries[i+1].metrics().size() != d_entries[i].metrics().size())
                    throw Exception("Entry found with different number of numeric responses.");
           }
        }
    }

    auto Record::add_entry(Entry const& entry) noexcept -> std::expected<void, Err>
    {
        if (!d_entries.empty())
        {
            Entry prev = d_entries.back();
            if (!(entry.date() > prev.date()))
                return std::unexpected(Err::DisorderedDate);
            if (entry.metrics().size() != prev.metrics().size())
                return std::unexpected(Err::InconsistentMetrics);
        }
        d_entries.push_back(std::move(entry));
        return {};
    }

    auto Record::find_entry(std::chrono::year_month_day date) const noexcept -> std::optional<int>
    {
        auto idx = std::distance(
                d_entries.begin(), 
                std::find_if(d_entries.begin(), d_entries.end(), 
                    [date](Entry const& e )-> bool { return e.date() == date; })
         );
        if ((int) idx == (int) d_entries.size())
            return std::nullopt;
        else 
            return idx;
    }

    auto Record::find_entries(DateRange date_range) const noexcept -> std::optional<IndexRange>
    {
        // Empty case
        if (d_entries.empty())
            return std::nullopt;
        // Handle single date case
        if (date_range.min && date_range.max && (*date_range.min == *date_range.max)) {
            auto idx = find_entry(*date_range.min);
            if (!idx) 
                return std::nullopt;
            else
                return IndexRange{idx, idx};
        }
        // Use a PaddedView in order to treat
        // all non-empty vectors as if there
        // are at least three elements. This
        // allows me to remove tedious edge
        // case checks.
        using utils::PaddedView;
        PaddedView<Entry> entries {d_entries};
        // Values for resultant output.
        std::optional<int> index_min {};
        std::optional<int> index_max {}; 

        int global_min { 0 };  // padded container's left boundary
        int global_max { (int) entries.size() - 1}; // padded container's right boundary
        // Find the entries with dates greater than or
        // equal to the minimum
        if (!date_range.min)
            index_min = global_min;
        else {
            // find index_min, the floor of the index range
            std::chrono::year_month_day smallest_date = *date_range.min ;
            int ss_min { global_min }; // search space left bound
            int ss_max { global_max }; // search space right bound
            // the index used to split the search space (in padded view).
            int poll_idx { floored_avg(ss_min, ss_max) };  


            // The following while-loop condition guarantees that the index used within the
            // loop will always be within the container's extremes. Therefore, we use the
            // PaddedView to allow a fake "off-by-one" error. The point being, instead of
            // dealing with edge cases on the borders of the real vector, the PaddedView
            // allows use to treat the borders as if they were non-border values; there's
            // no special case that make the code harder to read.
            //
            // Condition: (poll_idx > ss_min && poll_idx < ss_max)
            //
            // Due to the chosen search space update method (non-overlapping), the poll
            // index will always be biased toward the left.  Meaning, given
            //
            //     0 1 2 3 4
            //
            // if we start at 2 and search left, the new index to poll is (0 + 1)/2 -> 0.
            // Searching right, the index would be (3 + 4)/2 -> 3.  As a result, in the
            // case of only two elements, the smallest value greater than or equal to the
            // minimum will always be found in one comparison. If the left element is
            // greater than or equal to the target, that element is the answer. If not,
            // there is no answer.
            //
            // Due to the left bias, technically the polling index would never exceed the
            // ss_max, but I'm leaving the condition in for symmetry and future
            // flexibility.
            //
            // What happens when we end with a search space with two elements on the left
            // end of the global search space? For example, what if we have
            //
            //     0 1
            //
            // in the PaddedView reference frame? Well, the polling index would be 0 (the
            // edge of the PaddedView) which is programmed to be the same as PaddedView[1]
            // which is InternalVector[0]. In other words, we don't lose any information.
            //
            // The same logic can be applied to the right end of the search space:
            //
            //    3 4 
            //    _   // poll_index
            //
            // `3` is ss_min, so the loop breaks. However, index 4 maps right back to index
            // 3 because it's the end of the PaddedView, so no information is lost.
            while (poll_idx > ss_min && poll_idx < ss_max) {
                std::chrono::year_month_day poll_date = entries[poll_idx].date();
                if (poll_date > smallest_date) {
                    // search left half of search space to see if there is an even smaller
                    // floor.
                    if (entries[poll_idx - 1].date() >= smallest_date) {
                        ss_max = poll_idx - 1;
                        poll_idx = floored_avg(ss_min, ss_max);
                    } else break;
                } else if (poll_date < smallest_date) {
                    // search to the right
                    ss_min = poll_idx + 1;
                    poll_idx = floored_avg(ss_min, ss_max);
                } else break; // found exact match
            }
            if (poll_idx == global_min) { 
                // NOTE: The following logic only works because of PaddedView's behavior in
                // which PaddedView[0] just returns the first element of the wrapped
                // vector.
                if (entries[poll_idx].date() < smallest_date)
                    index_min = std::nullopt;
                else  
                    index_min = 0;
            } 
            else if (poll_idx == global_max && (entries[poll_idx].date() < smallest_date))
                index_min = std::nullopt;
            else 
                // padding adjustment
                index_min = poll_idx-1;
        }


        // find index_max, the ceiling of the date_range.
        if (!date_range.max)
            index_max = global_max - 2; // adjust for padding
        else {
            std::chrono::year_month_day largest_date = *date_range.max ;
            
            int ss_min { global_min }; // search space left bound
            int ss_max { global_max }; // search space right bound
            int poll_idx { floored_avg(ss_min, ss_max) }; 
            while (poll_idx > ss_min && poll_idx < ss_max) {
                std::chrono::year_month_day poll_date = entries[poll_idx].date();
                if (poll_date < largest_date) {
                    // look to the right to see if we need
                    // to search the right half of search space
                    if (entries[poll_idx + 1].date() <= largest_date){
                        ss_min = poll_idx + 1;
                        poll_idx = floored_avg(ss_min, ss_max);
                    } else break;
                } else if (poll_date > largest_date) {
                    // search to the left
                    ss_max = poll_idx - 1;
                    poll_idx = floored_avg(ss_min, ss_max);
                } else break;
                
            }
            // minimum > largest? Nothing there.
            if (poll_idx == global_min) { 
                if (entries[poll_idx].date() > largest_date)
                    index_max = std::nullopt;
                else  
                    index_max = 0;
            }
            else if (poll_idx == global_max && (entries[poll_idx].date() > largest_date)) 
                index_max = std::nullopt;
            else 
                index_max = poll_idx - 1;
        }

        if (!index_min || !index_max)
            return std::nullopt;
        else 
            return IndexRange{ index_min, index_max };
    }

    auto Record::get_entry(std::chrono::year_month_day date) const noexcept -> std::optional<Entry>
    {
        auto idx = find_entry(date);
        if (!idx)
            return std::nullopt;
        else 
            return d_entries[*idx]; // this copies!
    }

    void Record::remove_entry(std::chrono::year_month_day date)
    {
       auto idx = find_entry(date);
        if (idx)
           d_entries.erase(d_entries.begin() + *idx);
    }

    void Record::update_entry(Entry const& entry)
    {
        auto date = entry.date();
        auto idx = find_entry(date);
        if(idx) {
            d_entries[*idx] = std::move(entry);
        } else {  
            // Add entry to the record
            // We sort here instead of in add_entry to
            // prevent frequent sorting operations.
            d_entries.push_back(std::move(entry));
            std::sort(d_entries.begin(), d_entries.end(), 
                    [] (Entry const& a, Entry const& b) { return a < b; });
        }
    }

    auto operator<<(std::ostream& os, Record const& rec) noexcept -> std::ostream&
    {
        os << "[";
        for (Entry const& e: rec.entries())
            os  << e << ",\n";
        os << "]";
        return os;
    }

    auto get_record_metrics(Record const& r) -> Eigen::MatrixXd
    {
        int rows = r.size();
        int cols = r.metric_dim(); 
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> r_metrics = Eigen::MatrixXd::Zero(rows, cols);

        /// TODO: Likely the least-efficient implementation for filling the matrix, but we
        /// will worry about performance later.
        for (int i {0}; i < rows; ++i)
        {
            auto const& metric = r[i].metrics();
            for (int j {0}; j < cols; ++j)
            {
               r_metrics(i, j) = metric[j];
            }
        }

        return r_metrics;
    }
} // namespace ewi
