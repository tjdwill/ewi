// metricstats.hpp
// Defines a data aggregate for statistical data.
#ifndef INCLUDED_METRICSTATS
#define INCLUDED_METRICSTATS

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

            MetricStats(Eigen::VectorXd averages, Eigen::VectorXd deviations)
                : d_averages { std::move(averages) }, d_deviations { std::move(deviations) }
            {
                assert(d_deviations);
                assert(d_averages.size() == d_deviations->size()); 
            }

            MetricStats(Eigen::VectorXd averages)
                : d_averages { std::move(averages) } {}

            // ACCESSORS
            inline auto averages() const -> Eigen::VectorXd const& { return d_averages; }
            inline auto deviations() const -> std::optional<Eigen::VectorXd> const& { return d_deviations; }
        private:
            Eigen::VectorXd d_averages;  // mean value for each metric
            std::optional<Eigen::VectorXd> d_deviations {};
    };

}
#endif // INCLUDED_METRICSTATS

