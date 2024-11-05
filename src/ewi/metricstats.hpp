// metricstats.hpp
// Defines a data aggregate for statistical data.
#ifndef INCLUDED_EWI_METRICSTATS
#define INCLUDED_EWI_METRICSTATS

#ifndef INCLUDED_STD_CASSERT
#include <cassert>
#define INCLUDED_STD_CASSERT
#endif

#ifndef INCLUDED_STD_OPTIONAL
#include <optional>
#define INCLUDED_STD_OPTIONAL
#endif

#ifndef INCLUDED_EIGEN
#include <Eigen/Eigen>
#define INCLUDED_EIGEN
#endif

namespace ewi
{
    
    /// Stores statistics for a given set of metrics.
    class MetricStats
    {
        public:
            // CONSTRUCTORS
            // TODO: Check semantics of Eigen objects. Pass-by-value, reference, or move?
            MetricStats() = delete;

            explicit MetricStats(
                    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> metrics,
                    Eigen::RowVectorXd deviations
            )
                : d_metrics { std::move(metrics) }, d_deviations { std::move(deviations) }
            {
                assert(d_deviations);
                assert(d_metrics.cols() == d_deviations->size()); 
            }

            explicit MetricStats(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> metrics) noexcept
                : d_metrics { std::move(metrics) } {}
            // ACCESSORS

            /// Averages each column of the metrics matrix,
            /// resulting in a row vector.
            auto averages() const -> Eigen::RowVectorXd const&;
            inline auto deviations() const -> std::optional<Eigen::RowVectorXd> const& { return d_deviations; }
        private:
            Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> d_metrics;  // mean value for each metric
            std::optional<Eigen::RowVectorXd> d_deviations {};
    };

}
#endif // INCLUDED_EWI_METRICSTATS

