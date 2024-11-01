// entry.hpp
#ifndef INCLUDED_ENTRY
#define INCLUDED_ENTRY

#ifndef INCLUDED_STD_CHRONO
#include <chrono>
#define INCLUDED_STD_CHRONO
#endif

#ifndef INCLUDED_STD_OSTREAM
#include <ostream>
#define INCLUDED_STD_OSTREAM
#endif

#ifndef INCLUDED_STD_STRING
#include <string>
#define INCLUDED_STD_STRING
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
    /// A given data entry. Can be for either 
    /// job-related surveys or emotion surveys.
    /// TODO: Should I require metrics to be non-empty?
    /// Or, should I allow it and enforce non-empty entries
    /// up the design chain (like when declaring a job profile?)
    struct Entry 
    {
        public:
            // CONSTRUCTORS
            Entry() = delete;
            Entry(
                    std::chrono::year_month_day date,
                    std::string const& notes,
                    Eigen::VectorXd const& metrics
            ) noexcept
                : d_date(date), d_notes(std::move(notes)), d_metrics(std::move(metrics)) {}
                
            Entry(
                    std::chrono::year_month_day date,
                    std::string const& notes,
                    std::vector<double> const& metrics
            );


            inline auto date() const noexcept -> std::chrono::year_month_day const& { return d_date; }
            inline auto notes() const noexcept -> std::string const& { return d_notes; }
            inline auto metrics() const noexcept -> Eigen::VectorXd const& { return d_metrics; }

        private:
            std::chrono::year_month_day d_date;
            std::string d_notes;
            Eigen::VectorXd d_metrics;      
    };
    inline auto operator<=> (Entry const& a, Entry const& b) noexcept { return a.date() <=> b.date(); }  // The dates are equal.
    auto operator==(Entry const& a, Entry const& b) noexcept -> bool;  // all data members are equal.
    auto operator<<(std::ostream& os, Entry const& e) noexcept -> std::ostream&;
} // namespace ewi
#endif // INCLUDED_ENTRY
