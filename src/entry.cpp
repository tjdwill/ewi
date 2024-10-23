#include "entry.hpp"
#include <Eigen/Eigen>


namespace ewi {
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
