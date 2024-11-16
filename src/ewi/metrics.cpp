// metrics.cpp
#include "metrics.hpp"

#include <optional>
#include <Eigen/Eigen>

#include <ewi/record.hpp>


namespace ewi
{

    auto get_record_metrics(Record const& r) -> std::optional<Eigen::MatrixXd>
    {
        if (r.is_empty())
            return std::nullopt;
        int rows = r.size();
        int cols = r.metric_dim(); 
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> r_metrics = Eigen::MatrixXd::Zero(rows, cols);

        /// TODO: Likely the least-efficient implementation for filling the matrix, but we
        /// will worry about performance later.
        for (int i {0}; i < rows; ++i)
        {
            auto const& metric = r[i].metrics();
            for (int j {0}; j < cols; ++j)
               r_metrics(i, j) = metric[j];
        }

        return r_metrics;
    }
} // namespace ewi
