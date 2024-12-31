// metrics.cpp
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
#include "metrics.hpp"
//- STL
// #include <iostream>
#include <functional>
#include <optional>
//- Third-party
#include <Eigen/Eigen>
#include <matplot/matplot.h>
#include <vector>


namespace ewi
{

    auto get_means(Eigen::MatrixXd const& metrics, bool colwise) -> Eigen::VectorXd
    {
       assert(metrics.size() > 0);
       if (colwise)
           return metrics.colwise().mean();
       else
           return metrics.rowwise().mean();
    }

    auto calculate_ewi(Eigen::VectorXd const& local_means, Eigen::VectorXd const& global_means) -> Eigen::VectorXd
    {
        assert(local_means.size() > 0);
        assert(local_means.rows() == global_means.rows() && local_means.cols() == global_means.cols());
        /* General flow:
         * Step 1: Normalize the local_means by the global_means
         * Step 2: Handle divide by zero errors
         *      - Inf. case
         *      - NaN case
         *      Both inf and nan are to be replaced by an exponential function for a simple
         *      model depicting distance from zero.
                When global average is 0:

                EWI(x) = 
                {
                    -(1.5^|x|), x<0

                      1.5^x,    x>=0
                }

                This way, the calculate value is still 1 if the local and global values are equivalent, and
                we get rapid group as the local average trails away from 0.
         *
         */
        constexpr double BASE { 1.5 };
        auto normalized_local = local_means.array() / global_means.array();

        // Track which elements are positive or negative
        // std::cerr << "Local CHECK: " << -local_means.array().transpose()  << "\n";
        auto plus_minus = (local_means.array() != 0).select(local_means.array()/local_means.array().abs(), local_means);
        // std::cerr << "Normalized (+/-) CHECK: " << plus_minus.transpose() << "\n";
        // std::cerr << "INF CHECK: " << normalized_local.array().isInf().transpose() << "\n"; 

        // Raise BASE by the absolute value of each element in the normalized array. This
        // allows potentially negative values to have the intended behavior.
        auto temp0 = (normalized_local.array().isInf() or normalized_local.array().isNaN())
            .select(
                    Eigen::pow(BASE, Eigen::abs(local_means.array())),
                    normalized_local
            );
        // std::cerr << "TEMP CHECK (Replacement): " << temp0.transpose() << "\n";

        // Replace any value that is NOT 1 with the proper sense (+/- 1):
        auto temp1 = (temp0 != 1).select(temp0 * plus_minus, temp0);
        // std::cerr << "TEMP CHECK (+/-): " << temp1.transpose() << "\n";
        Eigen::VectorXd ewi_vals { temp1.matrix() };

        return ewi_vals;
    }

    auto calculate_ewi(double local_mean, double global_mean, double min_lim, double max_lim) -> double
    {
        assert(global_mean == (min_lim + max_lim)/2);
        // Normalize local_mean to a value between [-1, 1]
        // Ex. Given a range of values from [1, 5], the mean is 3.
        // Subtracting three from all values results in [-2, 2].
        // Normalizing by the upper limit results in [-1, 1].
        double ewi = (local_mean - global_mean)/(max_lim-global_mean);
        return ewi;
    }

    auto to_std_vec(Eigen::VectorXd const& input) -> std::vector<double>
    {
        // https://stackoverflow.com/questions/26094379/typecasting-eigenvectorxd-to-stdvector
        // https://stackoverflow.com/questions/39951553/is-it-possible-to-flatten-an-eigen-matrix-without-copying

        std::vector<double> stl {};
        stl.resize(input.size());
        Eigen::VectorXd::Map(&stl[0], input.size()) = input;

        return stl;
    }

    auto to_eigen(std::vector<double>& data) -> Eigen::VectorXd
    {
        Eigen::VectorXd vec = Eigen::Map<Eigen::VectorXd>(data.data(), data.size());
        assert(vec.size() == data.size());
        return vec;
    }
    
    auto to_eigen(std::vector<std::reference_wrapper<std::vector<double>>> const& data) -> Eigen::MatrixXd
    {
        int rows = static_cast<int>(data.size());
        int cols = static_cast<int>(data[0].get().size());
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> m(rows, cols);
        for (int i {0}; i < rows; ++i)
            m.row(i) = to_eigen(data[i].get());

        assert(m.rows() == rows && m.cols() == cols);
        return m;
    }

    auto plot_ewi(std::vector<double> const& ewi_vals, PlotCustomization const& opts, std::optional<double> personal_ewi) -> bool
    {
        namespace mpl = matplot;
        
        // How many subplots?
        int cols {};
        if (personal_ewi)
            cols = 2;
        else
            cols = 1;

        auto fig = mpl::figure(true);
        fig->title(opts.title);
        fig->size(opts.img_width, opts.img_height); 

        // Create the technical plot
        auto ax = fig->add_subplot(1, cols, 0);
        // Customization
        ax->grid(true);
        if (opts.xlim.has_value())
            ax->xlim(opts.xlim.value());
        else
            ax->xlim({0, static_cast<double>(ewi_vals.size() + 1)});
        if (opts.ylim.has_value())
            ax->ylim(opts.ylim.value());
        ax->title(opts.tech_title);
        ax->legend({"Base", "Idx"});
        ax->legend()
            ->location(mpl::legend::general_alignment::bottomright);
        ax->ylabel(opts.ylabel);
        ax->xlabel(opts.xlabel);
        
        // Plot the data
        ax->line(0, 1, ewi_vals.size() + 1, 1);  // Create a baseline at y=1
        ax->hold(true);                                                          
        std::vector<double> x = mpl::linspace(1, ewi_vals.size(), ewi_vals.size());
        ax->scatter(x, ewi_vals, opts.dot_size)
            ->marker_color({ 1.f, 0.f, 0.f }) // Red
            .marker_face(true);
        
        // Create the personal subplot
        if (personal_ewi)
        {
            auto pax = fig->add_subplot(1, cols, 1);
            pax->title(opts.personal_title);
            pax->xlim({-1, 1});
            pax->ylim({-1, 1});
            //pax->grid(true);
            pax->hold(true);

            pax->scatter({0}, {0}, opts.dot_size)
                ->color("black")
                .marker_face(true);
            pax->scatter({0}, {personal_ewi.value()}, opts.dot_size)
                ->color("blue")
                .marker_face(true);
            pax->line(0, -1, 0, 1);
            pax->legend({"Base", "Idx"});
            pax->legend()
                ->location(mpl::legend::general_alignment::bottomright);

        }

        // Export plot
        return fig->save(opts.filename);
    }

} // namespace ewi
