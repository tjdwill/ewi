// metrics.hpp
// Defines a data aggregate for statistical data.
#ifndef INCLUDED_EWI_METRICS
#define INCLUDED_EWI_METRICS

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
    /*
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
*/
    class Record; 
    
    /// Produce a matrix of metric data. Each row corresponds
    /// to a given entry.
    auto get_record_metrics(Record const& r) -> std::optional<Eigen::MatrixXd>;
    
    /// Return the means of the given vector. Taken column-eise by default
    auto get_record_means(Eigen::MatrixXd const& metrics, bool colwise=true) -> Eigen::VectorXd;

    /// Calculate the Employee Workload Index (EWI).
    /// Produces the numeric comparison between each metric and its global mean value.
    ///
    /// Precondition:
    ///     The two input vectors have the same (non-zero) dimensionality.
    ///
    /// Note: 
    ///     To handle divide by zero errors, an internal conversion is done to communicate the
    ///     local mean's distance from zero (the global mean). This corresponds to the idea
    ///     that if the normal condition is to have zero (ex. bug reports for instance),
    ///     handling an average of 11 reports should be much more significant than handling an
    ///     average of about 1. Since the idea is to give a visual indication of workload, this
    ///     conversion does not affect the efficacy of the data; instead, it aids in
    ///     communicating the desired information.
    auto calculate_ewi(Eigen::VectorXd const& local_means, Eigen::VectorXd const& global_means) -> Eigen::VectorXd;
}
#endif // INCLUDED_EWI_METRICSTATS

