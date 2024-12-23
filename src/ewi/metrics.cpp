// metrics.cpp
#include "metrics.hpp"

#include <optional>
//-
#include <Eigen/Eigen>
#include <matplot/matplot.h>
//-
#include <ewi/record.hpp>

// #include <iostream>

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

    auto get_record_means(Eigen::MatrixXd const& metrics, bool colwise) -> Eigen::VectorXd
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

    auto to_std_vec(Eigen::VectorXd const& input) -> std::vector<double>
    {
        // https://stackoverflow.com/questions/26094379/typecasting-eigenvectorxd-to-stdvector
        // https://stackoverflow.com/questions/39951553/is-it-possible-to-flatten-an-eigen-matrix-without-copying

        std::vector<double> stl {};
        stl.resize(input.size());
        Eigen::VectorXd::Map(&stl[0], input.size()) = input;

        return stl;
    }

    auto plot_ewi(std::vector<double> const& ewi_vals, PlotCustomization const& opts) -> bool
    {
        namespace mpl = matplot;
        
        auto fig = mpl::figure(true);
        auto ax = fig->current_axes();
        // Customization
        ax->grid(true);
        if (opts.xlim.has_value())
            ax->xlim(opts.xlim.value());
        if (opts.ylim.has_value())
            ax->ylim(opts.ylim.value());
        ax->title(opts.title);
        ax->legend({"Baseline", "EWI"});
        ax->legend()
            ->location(mpl::legend::general_alignment::bottomright);
        ax->ylabel(opts.ylabel);
        ax->xlabel(opts.xlabel);
        
        // Create the plot
        ax->line(0, 1, ewi_vals.size(), 1);  // Create a baseline at y=1
        ax->hold(true);                                                          
        std::vector<double> x = mpl::linspace(1, ewi_vals.size(), ewi_vals.size());
        ax->scatter(x, ewi_vals, opts.dot_size)
            ->marker_color({ 1.f, 0.f, 0.f }) // Red
            .marker_face(true);
        
        // Export plot
        return fig->save(opts.filename);
    }

} // namespace ewi
