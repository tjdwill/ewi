// metrics.hpp
// Defines various functions for processing and presenting user metric data.
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
    /// Return the means of the given vector. Taken column-eise by default
    auto get_means(Eigen::MatrixXd const& metrics, bool colwise=true) -> Eigen::VectorXd;

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
    auto calculate_ewi(Eigen::VectorXd const& local_means, Eigen::VectorXd const& global_means ) -> Eigen::VectorXd;
    
    /// Calculate EWI for Personal Surveys
    /// 
    /// Parameters:
    ///     
    ///     - `local_mean`: The mean value for the personal survey responses.
    ///     - `global_mean`: The global mean value.
    ///     - `min_lim`: The lowest value a given survey field can take.
    ///     - `max_lim`: The max value a given survey field can take.
    ///
    /// Precondition(s):
    ///     
    ///     - The global_mean is the average between the min and max values.
    ///
    /// Note:
    ///     
    ///     This is a separate function because the method of calculation differs from that
    ///     for the technical surveys.
    auto calculate_ewi(double local_mean, double global_mean, double min_lim, double max_lim) -> double;

    /// Convert to STL container
    auto to_std_vec(Eigen::VectorXd const& input) -> std::vector<double>;

    /// Convert to Eigen
    auto to_eigen(std::vector<double>& data) -> Eigen::VectorXd;
    auto to_eigen(std::vector<std::reference_wrapper<std::vector<double>>> const& data) -> Eigen::MatrixXd;


    /// A simple data structure for passing in plot
    /// customization options.
    struct PlotCustomization
    {
        std::string filename {};
        std::string title {};
        std::string xlabel {"Question Num."};
        std::string ylabel { "Index Value" };
        double dot_size { 10 }; // how large is a given scatter point
        std::optional<std::array<double, 2>> xlim {};
        std::optional<std::array<double, 2>> ylim {};
        // Default image resolution
        unsigned int img_width { 1280 };
        unsigned int img_height { 720 };
    };
    /// Visualizes the workload and exports to a specified save location.
    auto plot_ewi(
            std::vector<double> const& ewi_vals,
            PlotCustomization const& opts,
            std::optional<double> personal_ewi={}
    ) -> bool;

}
#endif // INCLUDED_EWI_METRICSTATS

