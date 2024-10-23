// entry.hpp
#ifndef INCLUDED_ENTRY
#define INCLUDED_ENTRY

#ifndef INCLUDED_STD_CHRONO
#include <chrono>
#define INCLUDED_STD_CHRONO
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
    struct Entry 
    {
        public:
            // CONSTRUCTORS
            Entry() = delete;
            Entry(
                    std::chrono::year_month_day date,
                    std::string const& notes,
                    Eigen::VectorXd const& entry_data
            ) noexcept
                : d_date(date), d_notes(std::move(notes)), d_data(std::move(entry_data)) {}
                
            Entry(
                    std::chrono::year_month_day date,
                    std::string const& notes,
                    std::vector<double> const& entry_data
            );


            inline auto date() const noexcept -> std::chrono::year_month_day const& { return d_date; }
            inline auto notes() const noexcept -> std::string const& { return d_notes; }
            inline auto data() const noexcept -> Eigen::VectorXd const& { return d_data; }

            friend auto operator<=>(Entry const& a, Entry const& b) noexcept { return a.d_date <=> b.d_date; }; // The dates are equal.
            friend auto operator==(Entry const& a, Entry const& b) noexcept -> bool = default;  // all data members are equal.
        private:
            std::chrono::year_month_day d_date;
            std::string d_notes;
            Eigen::VectorXd d_data;      
    };
} // namespace ewi
#endif // INCLUDED_ENTRY
