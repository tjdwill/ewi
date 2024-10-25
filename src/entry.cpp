#include "entry.hpp"
#include <chrono>
#include <vector>
#include <Eigen/Eigen>

namespace ewi {
    
    Entry::Entry(
            std::chrono::year_month_day date,
            std::string const& notes,
            std::vector<double> const& metrics
    ) : d_date{ date }, d_notes{ std::move(notes) }
    {
        Eigen::VectorXd data = Eigen::VectorXd::Zero(metrics.size());
        for (int i=0; i < (int) metrics.size(); i++)
            data[i] = metrics[i];
        d_metrics = data;
    }
            

    auto operator== (Entry const& a, Entry const& b) noexcept -> bool
    {
        if (a.date() != b.date())
            return false;
        if (a.notes() != b.notes())
            return false;
        // vector comp
        Eigen::VectorXd a_vec = a.metrics();
        Eigen::VectorXd b_vec = b.metrics();
        if (a_vec.size() != b_vec.size())
            return false;
        return a_vec.isApprox(b_vec);
         
    }
}
