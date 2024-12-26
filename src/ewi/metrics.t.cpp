// metrics.t.cpp
#include "metrics.hpp"

#include <cassert>
#include <functional>
#include <iostream>

#include <Eigen/Eigen>


// Helper functions
void test_to_eigen();
void test_mean_calc();
void test_ewi_calc();
void test_plot_ewi();


int main()
{
    test_to_eigen();
    test_mean_calc();
    test_ewi_calc();
    test_plot_ewi();
}

using namespace ewi;
using namespace Eigen;
namespace 
{
    static std::vector<double> METRICS { 0.0, 1.0, 5.0, 6.0 };
    // Global date vector for instantiation
    std::vector<std::reference_wrapper<std::vector<double>>> const DATA
    {
        std::ref(METRICS),
        std::ref(METRICS),
        std::ref(METRICS),
        std::ref(METRICS),
    };
}

void test_to_eigen()
{
    std::cout << "\n<test_to_eigen>\n---------------" << "\n";

    Matrix4d m {
        {0, 1, 5, 6},
        {0, 1, 5, 6},
        {0, 1, 5, 6},
        {0, 1, 5, 6},
    };

    auto test_m = to_eigen(DATA);
    assert(test_m.isApprox(m));
}

void test_mean_calc()
{
    std::cout << "\n<test_mean_calc>\n----------------" << "\n";

    auto metrics = to_eigen(DATA);
    std::cout << "Matrix: \n" << metrics << "\n";

    auto means = get_means(metrics);
    /// All entries in the defined record have the same metrics, so the means should equal
    /// the original values.
    auto baseline = Eigen::Map<Eigen::Vector<double, 4> >(METRICS.data());
    assert(means.isApprox(baseline));


    auto rowwise_means = get_means(metrics, false);
    /// The METRICS vector has a mean of 3.
    assert(rowwise_means.isApprox(Eigen::VectorXd::Ones(METRICS.size()) * 3));
    
    // Printouts
    std::cout << "Means (col-wise): " << means.transpose() << "\n";
    std::cout << "Means (row-wise): " << rowwise_means.transpose() << "\n";
    
}

void test_ewi_calc()
{
    std::cout << "\n<test_comparison_calc>\n----------------------" << "\n";

    Eigen::VectorXd global_means { { 1, 4.5, 0, 2, 10 } };
    Eigen::VectorXd local_means { { 2, 2.35, 4, 2.5, 100 } };

    std::cout << calculate_ewi(local_means, global_means).transpose() << "\n";

    // Test divide by zero method
    Eigen::VectorXd zeros = Eigen::VectorXd::Zero(5);
    Eigen::Vector<double, 5> locals { { -2, -1, 0, 1, 2 } };
    
    constexpr double BASE { 1.5 };
    Eigen::Vector<double, 5> check 
    { 
        {
            -std::pow(BASE, 2),  
            -std::pow(BASE, 1),
            1, 
            std::pow(BASE, 1),
            std::pow(BASE, 2) 
        }
    };
    auto compare = calculate_ewi(locals, zeros);
    std::cout << "Zero Check: " << compare.transpose() << "\n";
    assert(compare.isApprox(check));
}

void test_stl_conversion()
{
    std::cout << "\n<test_stl_conversion>\n---------------------" << "\n";
    /*
    Eigen::MatrixXd m 
    {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
    };
    */
    Eigen::VectorXd m { { 0, 1, 2, 3, 4, 5, 6, 7, 8} }; 

    std::vector<double> check 
    {
        0, 1, 2, 3, 4, 5, 6, 7, 8
    };

    assert(to_std_vec(m) == check);
    // std::cerr << "Vec Size: " << to_std_vec(m).size() << "\n";
}

void test_plot_ewi()
{
    std::cout << "\n<test_plot_ewi>\n---------------" << "\n";

    PlotCustomization opts
    {
       "test_ewi.png",
       "Test EWI Printout",
    };
    Eigen::VectorXd zeros = Eigen::VectorXd::Zero(5);
    Eigen::Vector<double, 5> locals { { -2, -1, 0, 1, 2 } };
    auto eigen = calculate_ewi(locals, zeros);
    auto data = to_std_vec(eigen);
    double personal = calculate_ewi(3.5, 3.0, 1., 5.);

    assert(data.size() > 0);

    assert(plot_ewi(data, opts, personal));
    assert(plot_ewi(data, {"test_ewi2.png", "No Personal"}));
}
