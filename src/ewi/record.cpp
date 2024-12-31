// record.cpp
/*
* Copyright (C) 2024 Terrance Williams
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "record.hpp"
//- STL
#include <algorithm>
#include <chrono>
#include <iostream>
#include <optional>
#include <vector>
//- Third-party
#include <cpperrors>
#include <Eigen/Eigen>
//- In-house
#include "entry.hpp"
#include <utils/padded_view.hpp>   // PaddedView


// Helper Functions
namespace
{
    constexpr inline int floored_avg(int a, int b)
    {
        return (a+b)/2;
    }
}
namespace ewi
{
    using cpperrors::Exception;

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
/* Record */
    Record::Record (std::vector<Entry>& entries)
        : d_entries{ std::move(entries) }
    {
        /* Invariants
         * Each entry in a record must have a unique date and must have the
         * same number of metric elements as the other
         * entries.
         */
        if (!d_entries.empty()) {
           for (int i=0; i < static_cast<int>( d_entries.size() ) - 1; ++i){
                if (!(d_entries[i+1].date() > d_entries[i].date()))
                    throw Exception("Each entry must be a later date than the previous.");
                if (d_entries[i+1].metrics().size() != d_entries[i].metrics().size())
                    throw Exception("Entry found with different number of numeric responses.");
           }
        }
    }

    auto Record::find(std::chrono::year_month_day date) const noexcept -> std::optional<int>
    {
        auto idx = static_cast<int>(std::distance(
                d_entries.begin(), 
                std::find_if(
                    d_entries.begin(), d_entries.end(), 
                    [date](Entry const& e)-> bool { return e.date() == date; }
                )
        ));
        if (idx == size())
            return std::nullopt;
        else 
            return idx;
    }

    auto Record::find(DateRange date_range) const noexcept -> std::optional<IndexRange>
    {
        // Empty case
        if (d_entries.empty())
            return std::nullopt;
        // Handle single date case
        if (date_range.min && date_range.max && (*date_range.min == *date_range.max)) {
            auto idx = find(*date_range.min);
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
        int global_max { static_cast<int>(entries.size() - 1)}; // padded container's right boundary
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
            else if (poll_idx == global_max)
            {
                if (entries[poll_idx].date() < smallest_date)
                    index_min = std::nullopt;
                else
                // padding adjustment
                    index_min = poll_idx - 2;
            }
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
            else if (poll_idx == global_max)
            {
                if (entries[poll_idx].date() > largest_date)
                    index_max = std::nullopt;
                else
                    // Padding adjustment
                    index_max = poll_idx - 2;
            }
            else 
                index_max = poll_idx - 1;
        }

        if (!index_min || !index_max)
            return std::nullopt;
        else 
            return IndexRange{ index_min, index_max };
    }

    auto Record::get(std::chrono::year_month_day date) const noexcept -> std::optional<std::reference_wrapper<Entry const>>
    {
        auto idx = find(date);
        if (!idx)
            return std::nullopt;
        else 
            return d_entries[*idx]; // this copies!
    }

    auto Record::get(DateRange const& dates) const noexcept -> std::optional<std::vector<std::reference_wrapper<Entry const>>>
    {
        auto idxs = find(dates);
        if (!idxs)
            return std::nullopt;
        std::vector<std::reference_wrapper<Entry const>> entries {};
        for (int i { *(idxs->min) }; i <= *(idxs->max); ++i)
            entries.push_back(operator[](i));
        return entries;
    }

    auto Record::is_empty() const noexcept -> bool { return size() == 0; }
    
    auto Record::metric_dim() const noexcept -> int { return is_empty() ? 0 : d_entries[0].metrics().size(); }
    
    auto Record::metrics(std::chrono::year_month_day date) const noexcept -> std::optional<std::reference_wrapper<std::vector<double>>>
    {
        auto test = metrics({date, date});
        if (!test)
            return std::nullopt;
        else
            return (*test)[0].get();
    }

    auto Record::metrics(DateRange const& dates) const noexcept -> std::optional<std::vector<std::reference_wrapper<std::vector<double>>>>
    {
        auto entries = get(dates);
        if (!entries)
            return std::nullopt;
        std::vector<std::reference_wrapper<std::vector<double>>> metrics {};
        /// The const on the metrics is cast away to be compatible with Eigen.
        /// The assumption is that this method will never be used to modify metrics
        /// directly.
        for (Entry const& e : *entries)
            metrics.push_back(std::ref(
                        const_cast<std::vector<double>&>(e.metrics())
            ));

        return metrics;
    }

    auto Record::size() const noexcept -> int { return static_cast<int>(d_entries.size()); }
    
    auto Record::operator[] (int idx) const -> Entry const& { return d_entries[idx]; }

    void Record::add(Entry const& entry)
    {
        if (!d_entries.empty())
        {
            Entry prev = d_entries.back();
            if (!(entry.date() > prev.date()))
                throw Exception("Could not add entry to record; Date is earlier than latest entry currently in record.");
            if (entry.metrics().size() != prev.metrics().size())
                throw Exception("Could not add entry to record; Metric count is inconsistent with previous entries.");
        }
        d_entries.push_back(std::move(entry));
    }

    void Record::remove(std::chrono::year_month_day date)
    {
       auto idx = find(date);
        if (idx)
           d_entries.erase(d_entries.begin() + *idx);
    }

    void Record::update(Entry const& entry)
    {
        auto date = entry.date();
        auto idx = find(date);
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
        os << "[\n";
        for (Entry const& e: rec)
            os  << e << ",\n";
        os << "]";
        return os;
    }

} // namespace ewi
