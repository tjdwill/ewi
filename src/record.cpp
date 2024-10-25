// record.cpp
#include "record.hpp"
#include "entry.hpp"
#include "metricstats.hpp"

#include <chrono>
#include <expected>
#include <iostream>
#include <optional>
#include <vector>
#include <cpperrors>


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

    /*
     * find_entries required test cases:
     *  - 0-element Record
     *  - 1-element Record
     *      - Successful and non-successful
     *  - 2-element Record
     *  - 3+ element Record
     *
     *  - DateRanges:
     *      - Singularity
     *      - Fully-specified
     *      - None:Max
     *      - Min:None
     *
     */
    auto Record::find_entries(DateRange date_range) const noexcept -> std::optional<IndexRange>
    {
        if (d_entries.empty())
            return std::nullopt;
        
        // Values for resultant output.
        std::optional<int> index_min {};
        std::optional<int> index_max {}; 

        int global_min {};  // container's left boundary
        int global_max { (int) d_entries.size() - 1}; // container's right boundary
        // Find the entries with dates greater than or
        // equal to the minimum
        if (!date_range.min)
            index_min = global_min;
        else {
            // find index_min, the floor of the index range
            std::chrono::year_month_day smallest_date = *date_range.min ;
            std::chrono::year_month_day poll_date;

            // TODO: Test Case: What happens if global_max and global_min
            // are both 0?
            //
            // The following while-loop condition guarantees that the index
            // within the loop will always be within the
            // container's extremes. Therefore, we can
            // access the left and right sibling elements.
            //
            // Condition: (poll_idx > global_min && poll_idx < global_max)
            //
            // This allows me to cheat a bit because it enables me to
            // go past the assigned search space (off by one)
            // with impugnity. Why? The search space is
            // always confined to some range within the
            // global search space, and the poll_index
            // within the loop is always within
            // (global_min, global_max). True off-by-one access is impossible 
            // within the loop.
            // Therefore, the only time the
            // loop exhausts is if the search space has
            // only one element, making the two edge cases
            // of 1- or 2-element containers collapse into
            // a single boolean check.
            //
            // Due to the chosen search space update
            // method (non-overlapping), the poll index
            // will always be biased toward the left.
            // Meaning, given
            //
            //     0 1 2 3 4
            //
            // if we start at 2 and search left, the new
            // index to poll is (0 + 1)/2 -> 0. Searching
            // right, the index would be (3 + 4)/2 -> 3.
            // As a result, in the case of only two
            // elements, the smallest value greater than or
            // equal to the minimum will always be found
            // in one comparison. If the left element is
            // greater than or equal to the target, that
            // element is the answer. If not, there is no
            // answer.
            int ss_min { global_min }; // search space left bound
            int ss_max { global_max }; // search space right bound
            int poll_idx { floored_avg(ss_min, ss_max) };  // the index used to split the search space.
            while (poll_idx > global_min && poll_idx < global_max) {
                poll_date = d_entries[poll_idx].date();
                if (poll_date > smallest_date) {
                    // look left to see if the sibling is
                    // also within date_range.
                    if (d_entries[poll_idx - 1].date() >= smallest_date)
                    {
                        // search left half of search space
                        ss_max = poll_idx - 1;
                        poll_idx = floored_avg(ss_min, ss_max);
                    } else {
                        index_min = poll_idx;
                        break;
                    }
                } else if (poll_date < smallest_date) {
                    // search to the right
                    ss_min = poll_idx + 1;
                    poll_idx = floored_avg(ss_min, ss_max);
                } else {
                    index_min = poll_idx;
                    break;
                }
            }
            // check edge cases (container of one or two elements)
            if (poll_idx == global_min || poll_idx == global_max) {
                if (d_entries[poll_idx].date() < smallest_date)
                    index_min = std::nullopt;
                else  
                // couldn't find an entry within minimum
                // date range.
                    index_min = poll_idx;
            }    
        }

       
        // find index_max, the ceiling of the date_range.
        if (!date_range.max)
            index_max = global_max;
        else {
            std::chrono::year_month_day largest_date = *date_range.max ;
            std::chrono::year_month_day poll_date;
            
            int ss_min { global_min }; // search space left bound
            int ss_max { global_max }; // search space right bound
            int poll_idx { floored_avg(ss_min, ss_max) }; 
            while (poll_idx > global_min && poll_idx < global_max) {
                poll_date = d_entries[poll_idx].date();
                if (poll_date < largest_date) {
                    // look right to see if the sibling is
                    // also within date_range.
                    if (d_entries[poll_idx + 1].date() <= largest_date)
                    {
                        // search right half of search space
                        ss_min = poll_idx + 1;
                        poll_idx = floored_avg(ss_min, ss_max);
                    } else {
                        index_max = poll_idx;
                        break;
                    }
                } else if (poll_date > largest_date) {
                    // search to the left
                    ss_max = poll_idx - 1;
                    poll_idx = floored_avg(ss_min, ss_max);
                } else {
                    index_max = poll_idx;
                    break;
                }
            }
            // minimum > largest? Nothing there.
            if (poll_idx == global_min || poll_idx == global_max) {
                if (d_entries[poll_idx].date() > largest_date)
                    index_max = std::nullopt;
                else
                    index_max = poll_idx;
            }
        }

        if (!index_min && !index_max)
            return std::nullopt;
        else
            return IndexRange { index_min, index_max };
    }
} // namespace ewi

