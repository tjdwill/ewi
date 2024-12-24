// metrics.hpp
// Defines various functions for processing and presenting user metric data.
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

#ifndef INCLUDED_STD_STRING
#include <string>
#define INCLUDED_STD_STRING
#endif


#ifndef INCLUDED_EIGEN
#include <Eigen/Eigen>
#define INCLUDED_EIGEN
#endif

namespace ewi
{
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

    /// Convert to STL container
    auto to_std_vec(Eigen::VectorXd const& input) -> std::vector<double>;


    /// A simple data structure for passing in plot
    /// customization options.
    struct PlotCustomization
    {
        std::string filename {};
        std::string title {};
        std::string xlabel {};
        std::string ylabel { "EWI" };
        double dot_size { 10 }; // how large is a given scatter point
        std::optional< std::array<double, 2> > xlim {};
        std::optional< std::array<double, 2> > ylim {};
    };
    /// Visualizes the workload and exports to a specified save location.
    auto plot_ewi(std::vector<double> const& ewi_vals, PlotCustomization const& opts) -> bool;

}
#endif // INCLUDED_EWI_METRICSTATS

